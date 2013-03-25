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
  template<typename T>
  static constexpr T bit_or(void) {
    return 0;
  }
  template<typename T, T Head, T... Tail>
  static constexpr T bit_or(void) {
    return Head | bit_or<T, Tail...>();
  }

  template<typename T, typename Head, typename... Tail>
  struct all_same {
    static constexpr bool value = std::is_same<T, Head>::value && all_same<T, Tail...>::value;
  };
  template<typename T, typename U>
  struct all_same<T, U> {
    static constexpr bool value = std::is_same<T, U>::value;
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
    static_assert(std::is_integral<T>::value, "T must be an integral type");
    static_assert(std::is_unsigned<T>::value, "T must be an unsigned type");

    typedef Register<T, _addr, _access, _reset_value> type;

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
      clear(mpl::bit_or<T, Rb::value...>());
    }

    /* set constants */
    template<typename... Rc>
    static void set(void) {
      // TODO: enable only if all_RegisterConst
      static_assert(mpl::all_same<type, typename Rc::reg_type...>::value, "template arguments are not of same type as our class (Register<...>)");
      set( mpl::bit_or<T, Rc::value...>(), mpl::bit_or<T, Rc::bitmask...>() );
    }

    /* set value, masked by Rb */
    template<typename... Rb>
    static void set(T const value) {
      static_assert(mpl::all_same<type, typename Rb::reg_type...>::value, "template arguments are not of same type as our class (Register<...>)");
      set( value, mpl::bit_or<T, Rb::bitmask...>() );
    }
  };


  ////////////////////  RegisterBits  ////////////////////


  template< typename R,          /* Register type */
            unsigned _offset,    /* bit offset */
            unsigned width = 1   /* width of sub-register (numof bits) */
            >
  struct RegisterBits
  {
    typedef R reg_type;
    typedef typename R::value_type value_type;

    static constexpr unsigned   offset  = _offset;
    static constexpr value_type bitmask = ((1 << width) - 1) << offset;  /* (e.g. width(3) = 0b111 = 7) */
    static constexpr value_type value   = bitmask;

    static_assert(width >= 1, "invalid width");
    static_assert(offset + width <= sizeof(value_type) * 8, "invalid width/offset");

    static value_type test()           { return R::load() & bitmask;  }
    static value_type test_and_shift() { return test() >> offset; }

    static bool       test(value_type const _value) { return test() == _value; }

    static void       set()            { R::set(value);            }
    static void       clear()          { R::clear(value);          }

    /** NOTE: this does not check if _value is masked correctly! */
    static void set(value_type const _value) { R::set(_value, bitmask); }
    static void shift_and_set(value_type const _value) {
      R::set(shifted_value(_value), bitmask);
    }

    // TODO: better naming for this
    static constexpr value_type shifted_value(value_type const _value) {
      return (_value << offset);
    }

    template<unsigned bit_no>
    struct bit : RegisterBits< R, offset + bit_no, 1 > {
      static_assert(bit_no < width, "invalid bit_no");
    };

    // cast operator
    constexpr operator value_type() { return value; }
  };


  ////////////////////  RegisterConst  ////////////////////


  /** NOTE: _value is shifted with offset of R! */
  template< typename R, typename R::value_type _value >
  struct RegisterConst
  {
    typedef typename R::value_type value_type;
    typedef typename R::reg_type reg_type;

    static constexpr value_type value   = _value << R::offset;
    static constexpr value_type bitmask = R::bitmask;

    static_assert((value & bitmask) == value, "value does not fit into bits of R");

    static void set()     { R::set(value);   }
    static bool test()    { return R::test(value); }

    // cast operator
    constexpr operator value_type() { return value; }
  };

}

#endif // REGISTER_HPP_INCLUDED
