/*
 * CppCore - C++ microprocessor core library
 *
 * Copyright 2012 Axel Burri <axel@tty0.ch>
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
//  core register definition: wrap bitfields into Register classes
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
#include "register_storage.hpp"

namespace mpl
{
  template<typename T, typename Head, typename... Tail>
  struct all_same {
    static constexpr bool value = std::is_same<T, Head>::value && all_same<T, Tail...>::value;
  };
  template<typename T, typename U>
  struct all_same<T, U> {
    static constexpr bool value = std::is_same<T, U>::value;
  };


  template<typename, typename...>
  struct reg_combined;

  template<typename T>
  struct reg_combined<T>
  {
    static constexpr T set_mask       = 0;
    static constexpr T clear_mask     = 0;
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
  template< typename   T,
            reg_addr_t _addr,
            Access     _access,
            T          _reset_value = 0 >
  struct Register : public RegisterStorage<T, _addr, _access, _reset_value>
  {
    static_assert(std::is_integral<T>::value, "T is not an integral type");
    static_assert(std::is_unsigned<T>::value, "T is not an unsigned type");

    using type     = Register<T, _addr, _access, _reset_value>;
    using reg_type = type;

    static constexpr reg_addr_t addr        = _addr;
    static constexpr Access     access      = _access;
    static constexpr T          reset_value = _reset_value;

    static T    test (T const value) { return type::load() & value;          }
    static void set  (T const value) { type::store( type::load() | value );  }
    static void set  (T const set_mask, T const clear_mask) { type::store( (type::load() & ~clear_mask) | set_mask ); }
    static void clear(T const value) { type::store( type::load() & ~value ); }
    static void mask (T const value) { type::store( type::load() & value );  }
    static void reset()              { type::store(reset_value);             }

    template<typename... Rb>
    static void clear(void) {
      static_assert(mpl::all_same<type, typename Rb::reg_type...>::value, "template arguments are not of same type as our class (Register<...>)");
      clear(mpl::reg_combined<T, Rb...>::clear_mask);
    }

    /* set constants */
    template<typename... Rc>
    static void set(void) {
      static_assert(mpl::all_same<type, typename Rc::reg_type...>::value, "template arguments are not of same type as our class (Register<...>)");
      set(mpl::reg_combined<T, Rc...>::set_mask, mpl::reg_combined<T, Rc...>::cropped_clear_mask);
    }

    /* set value, masked by Rb */
    template<typename... Rb>
    static void set(T const value) {
      static_assert(mpl::all_same<type, typename Rb::reg_type...>::value, "template arguments are not of same type as our class (Register<...>)");
      set(value, mpl::reg_combined<T, Rb...>::clear_mask);
    }
  };


  ////////////////////  RegisterBits  ////////////////////


  template< typename R,          /* Register type */
            unsigned _offset,    /* bit offset */
            unsigned width = 1   /* width of sub-register (numof bits) */
            >
  struct RegisterBits
  {
    static_assert(std::is_same<typename R::type, typename R::reg_type>::value, "template argument R is not o type: Register<>");
    using type       = RegisterBits<R, _offset, width>;
    using bits_type  = type;
    using reg_type   = R;
    using value_type = typename R::value_type;

    static constexpr unsigned   offset     = _offset;
    static constexpr value_type value      = ((1 << width) - 1) << offset;  /* (e.g. width(3) = 0b111 = 7) */
    static constexpr value_type clear_mask = value;  // TODO: rename bitmask ?
    static constexpr value_type set_mask   = value;

    static_assert(width >= 1, "invalid width");
    static_assert(offset + width <= sizeof(value_type) * 8, "invalid width/offset");

    static value_type test()           { return R::load() & clear_mask; }
    static value_type test_and_shift() { return test() >> offset; }

    static bool       test(value_type const test_value) { return test() == test_value; }

    static void       set()            { R::set(set_mask); }
    static void       clear()          { R::clear(clear_mask); }

    /** NOTE: this does not check if _value is masked correctly! */
    static void set(value_type const set_value) { R::set(set_value, set_mask); }
    static void shift_and_set(value_type const set_value) {
      R::set(shifted_value(set_value), set_mask);
    }

    // TODO: better naming for this
    static constexpr value_type shifted_value(value_type const input_value) {
      // assert((input_value << offset) | clear_mask, "input value does not match clear_mask");
      return (input_value << offset);
    }

    template<unsigned bit_no>
    struct bit : RegisterBits< reg_type, offset + bit_no, 1 > {
      static_assert(bit_no < width, "invalid bit_no");
    };

    /* cast operator */
    constexpr operator value_type() { return value; }
  };


  ////////////////////  RegisterConst  ////////////////////


  /** NOTE: _value is shifted with offset of R! */
  template< typename R, typename R::value_type _value >
  struct RegisterConst
  {
    using type       = RegisterConst<R, _value>;
    using bits_type  = R;
    using value_type = typename R::value_type;
    using reg_type   = typename R::reg_type;

    static constexpr value_type offset     = R::offset;
    static constexpr value_type value      = _value << R::offset;
    static constexpr value_type set_mask   = value;
    static constexpr value_type clear_mask = R::clear_mask;

    static_assert((set_mask & clear_mask) == value, "value does not fit into bits of R");

    static void set()     { R::set(value);   }
    static bool test()    { return R::test(value); }

    /* NOTE: clear() is NOT declared. Clearing a contant value in a
     * register not make sense imho.  What you actually want to do to
     * is clearing all bits (offset, width) where RegisterConst<> is
     * defined on. In order to clear the constant, you must refer to
     * the underlying RegisterBits<> type by either accessing it
     * directly (preferred), or use the bits_type:
     *
     *   RegisterConst<>::bits_type::clear()
     *
     */
#if 0 /* clear(): INTENTIONALLY NOT DECLARED */
    //! static void clear()   { R::clear(); }  /* INTENTIONALLY NOT DECLARED */
#endif

    // cast operator
    constexpr operator value_type() { return value; }
  };
}

#endif // REGISTER_HPP_INCLUDED
