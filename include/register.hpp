/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright 2013 Axel Burri <axel@tty0.ch>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef REGISTER_HPP_INCLUDED
#define REGISTER_HPP_INCLUDED

/*
 * MPL-style Register class.
 *
 * - includes declarations of the Register type for:
 *   - embedded: register writes go to "constexpr volatile" register address
 *   - simulation: register writes go to variable in memory
 * - prohibits writing to read-only registers
 * - prohibits reading from write-only registers
 *
 */

//
//  microprocessor register definition: wrap bitfields into Register classes
//

//  Pros:
//
//  - uses Register MPL class to wrap the register access. All type
//    checks are performed on register write, AND on register bitfield
//    access.
//
//  - register bitfields are defined as type "static constexpr uint32_t",
//    which is very common, simple and straight forward.
//
//
//  Cons:
//
//  - MPL stype "::value" variable access
//
//
// *** Don't miss the text from previous commit! ***


// Example:
//
// TODO: declaration example
//
// LINKS:
// "uint_fast16_t": http://en.cppreference.com/w/cpp/types/integer
//
// static void enable(void) {
//   SPIx::CR1::SPE::set()  /* SPI Enable */
// }
//
// void MyFunction() {
//   /* a load() call results in:                                                 */
//   /*                                                                           */
//   /* constexpr volatile T * value_ptr = reinterpret_cast<volatile T *>(addr);  */
//   /* auto reg = *value_ptr;                                                    */
//   /*                                                                           */
//   /* notes:                                                                    */
//   /* - "addr" (constexpr) holds the address of (non-pointer-type) SPIx::CR1    */
//   /* - type "T" is the register type (e.g. uint32_t or uint8_t)                */
//   /* - reg becomes type "volatile uint32_t                                     */
//   /*                                                                           */
//
//   auto reg = SPIx::CR1::load();
//   reg &= SPIx::CR1::SPE::value | SPIx::CR1::CRCNEXT::value | SPIx::CR1::CRCEN::value;
//   SPIx::CR1::store(reg);

//   /* a store() call results in:                                                */
//   /* *value_ptr = reg;                                                         */
// }
//


#include <type_traits>
#include <cstdint>
#include <register_storage.hpp>
#include <compiler.h>

namespace mpl
{
  template<typename T, typename Head, typename... Tail>
  struct all_same {
    static constexpr bool value = std::is_same<T, Head>::value && all_same<T, Tail...>::value;
  };
  template<typename T, typename Head>
  struct all_same<T, Head> {
    static constexpr bool value = std::is_same<T, Head>::value;
  };


  template<typename, typename...>
  struct reg_combined;

  template<typename T>
  struct reg_combined<T>
  {
    static constexpr T set_mask           = 0;
    static constexpr T clear_mask         = 0;
    static constexpr T cropped_clear_mask = 0;
  };

  template<typename T, typename Head, typename... Tail>
  struct reg_combined<T, Head, Tail...>
  {
    static constexpr T set_mask   = Head::set_mask   | reg_combined<T, Tail...>::set_mask;
    static constexpr T clear_mask = Head::clear_mask | reg_combined<T, Tail...>::clear_mask;

    /**
     * Cropped clear mask (USE WITH CARE!)
     *
     * The bits in cropped_clear_mask are only set if they do not
     * appear in the set_mask. This helps in some cases where the
     * compiler is not smart enough.
     * Example: (gcc-4.8 generating Arm-Thumb2)
     *
     *   x = (x & ~0x1ff00) | 0x0ff00
     *   x = (x & ~0x10000) | 0x0ff00   <-- one BIC instruction less!
     *
     * It does not matter which form you use, as long as you perform a
     * load-clear-set-store sequence.
     */
    static constexpr T cropped_clear_mask =
      (Head::clear_mask & ~Head::set_mask) |
      reg_combined<T, Tail...>::cropped_clear_mask;
  };
}


namespace reg
{
  template< typename, unsigned, unsigned >
  class RegisterBits;

  template< typename   T,
            reg_addr_t addr,
            Access     access,
            T          _reset_value = 0 >
  class Register : public RegisterStorage<T, addr, access, _reset_value>
  {
    /* private constructor: instantiation would only cause confusion with set/clear functions */
    Register() {};

  public:
    using type       = Register<T, addr, access, _reset_value>;
    using bits_type  = RegisterBits< type, 0, sizeof(T) * 8>;
    using reg_type   = type;
    using value_type = T;

    static constexpr value_type reset_value = _reset_value;

    static __always_inline value_type test(value_type const value) {
      return type::load() & value;
    }
    static __always_inline void set(value_type const value) {
      type::store( type::load() | value );
    }
    static __always_inline void set(value_type const set_mask, value_type const clear_mask) {
      type::store( (type::load() & ~clear_mask) | set_mask );
    }
    static __always_inline void clear(value_type const value) {
      type::store( type::load() & ~value );
    }
    static __always_inline void mask(value_type const value) {
      type::store( type::load() & value );
    }
    static __always_inline void reset() {
      type::store(reset_value);
    }

    /* clear register bits */
    template<typename... Rb>
    static __always_inline void clear(void) {
      static_assert(mpl::all_same<type, typename Rb::reg_type...>::value, "template arguments are not of same Register<> type");
      clear(mpl::reg_combined<value_type, Rb...>::clear_mask);
    }

    /* set constants */
    template<typename... Rc>
    static __always_inline void set(void) {
      static_assert(mpl::all_same<type, typename Rc::reg_type...>::value, "template arguments are not of same Register<> type");
      set(mpl::reg_combined<value_type, Rc...>::set_mask, mpl::reg_combined<value_type, Rc...>::cropped_clear_mask);
    }

    /* set value, masked by register bits (Rb) */
    template<typename... Rb>
    static __always_inline void set(value_type const value) {
      static_assert(mpl::all_same<type, typename Rb::reg_type...>::value, "template arguments are not of same Register<> type");
      set(value, mpl::reg_combined<value_type, Rb...>::clear_mask);
    }
  };


  ////////////////////  RegisterBits  ////////////////////


  template< typename R,          /* Register type */
            unsigned offset,     /* bit offset */
            unsigned width = 1   /* number of bits */
            >
  class RegisterBits
  {
    static_assert(std::is_same<typename R::type, typename R::reg_type>::value, "template argument R is not of type: Register<>");
    static_assert(width >= 1, "invalid width");
    static_assert(offset + width <= sizeof(typename R::value_type) * 8, "invalid width/offset");

    /* private constructor: instantiation would only cause confusion with set/clear functions */
    RegisterBits() {};

  public:
    using type       = RegisterBits<R, offset, width>;
    using bits_type  = type;
    using reg_type   = R;
    using value_type = typename R::value_type;

    static constexpr value_type value      = ((1 << width) - 1) << offset;  /* (e.g. width(3) = 0b111 = 7) */
    static constexpr value_type set_mask   = value;
    static constexpr value_type clear_mask = value;

    static __always_inline constexpr value_type value_from(value_type const val) {
      // assert((val & (clear_mask >> offset)) == val);  /* input value does not match clear_mask */
      return (val << offset);
    }
    static __always_inline void set() {
      reg_type::set(set_mask);
    }
    /** NOTE: this does not check if _value is masked correctly! */
    static __always_inline void set(value_type const val) {
      // assert((val & (clear_mask >> offset)) == val);  /* input value does not match clear_mask */
      reg_type::set(value_from(val), clear_mask);
    }
    static __always_inline void clear() {
      reg_type::clear(clear_mask);
    }
    static __always_inline bool test() {
      return (reg_type::load() & clear_mask) != 0;
    }
    static __always_inline bool test(value_type const val) {
      // assert((val & (clear_mask >> offset)) == val);  /* input value does not match clear_mask */
      return (reg_type::load() & clear_mask) == value_from(val);
    }
    static __always_inline value_type get() {
      return (reg_type::load() & clear_mask) >> offset;
    }

    template<unsigned bit_no>
    struct bit : RegisterBits< reg_type, offset + bit_no, 1 > {
      static_assert(bit_no < width, "invalid bit_no");
    };
  };


  ////////////////////  RegisterConst  ////////////////////


  /** NOTE: _value is shifted with offset of R! */
  template< typename R, typename R::value_type _value >
  class RegisterConst
  {
    static_assert(std::is_same<typename R::type, typename R::bits_type>::value, "template argument R is not of type: RegisterBits<>");

    /* private constructor: instantiation would only cause confusion with set/clear functions */
    RegisterConst() {};

  public:
    using type       = RegisterConst<R, _value>;
    using bits_type  = R;
    using reg_type   = typename R::reg_type;
    using value_type = typename R::value_type;

    static constexpr value_type value      = bits_type::value_from(_value);
    static constexpr value_type set_mask   = value;
    static constexpr value_type clear_mask = bits_type::clear_mask;

    static_assert((set_mask & clear_mask) == value, "value does not fit into bits of R");

    static __always_inline void set() {
      reg_type::set(set_mask, clear_mask);
    }
    static __always_inline bool test() {
      return (reg_type::load() & clear_mask) == value;
    }

    /* NOTE: clear() is NOT declared. Clearing a contant value in a
     * register does not make sense. What you actually want to do to
     * is clearing all bits (offset, width) where RegisterConst<> is
     * defined on. In order to clear the constant, you must refer to
     * the underlying RegisterBits<> type by either accessing it
     * directly (preferred), or use the bits_type:
     *
     *   RegisterConst<>::bits_type::clear()
     *
     */
    //! static __always_inline void clear() { bits_type::clear(); }  /* INTENTIONALLY NOT DECLARED */
  };
}

#endif // REGISTER_HPP_INCLUDED
