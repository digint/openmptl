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

namespace reg
{
  template< typename   T,
            reg_addr_t _addr,
            Access     _access,
            T          _reset_value = 0 >
  struct Register
  {
    static_assert(std::is_integral<T>::value, "T must be an integral type");
    static_assert(std::is_unsigned<T>::value, "T must be an unsigned type");

    typedef RegisterStorage<T, _addr, _access, _reset_value> type;
    typedef typename type::value_type value_type;

    static constexpr reg_addr_t addr        = _addr;
    static constexpr T          reset_value = _reset_value;

    static T load() {
      static_assert(_access != Access::wo, "read access to a write-only register");
      return type::load();
    }

    static void store(T const value) {
      static_assert(_access != Access::ro, "write access to a read-only register");
      type::store(value);
    }

    static T    test (T const value) { return type::load() & value;          }
    static void set  (T const value) { type::store( type::load() | value );  }
    static void set  (T const value, T const mask) { type::store( (type::load() & ~mask) | value );  }
    static void clear(T const value) { type::store( type::load() & ~value ); }
    static void mask (T const value) { type::store( type::load() & value );  }
    static void reset()              { type::store(reset_value);             }
  };


  ////////////////////  RegisterBits  ////////////////////


  template< typename R,             /* Register type */
            std::size_t _offset,    /* bit offset */
            std::size_t width = 1   /* width of sub-register (numof bits) */
            >
  struct RegisterBits  // TODO: consider derived from integral_type<>
  {
    typedef R type;
    typedef typename type::value_type value_type;

    static constexpr std::size_t offset = _offset;

    static_assert(width >= 1, "invalid width");
    static_assert(offset + width <= sizeof(value_type) * 8, "invalid width/offset");

    /** create mask bits (e.g. width(3) = 0b111 = 7) */
    static constexpr value_type bitmask = ((1 << width) - 1) << offset;
    static constexpr value_type value   = bitmask;  // TODO: try enable_if<> in order to prevent access if width > 1

    static value_type test()           { return type::load() & bitmask;  }
    static value_type test_and_shift() { return test() >> offset; }

    static bool       test(value_type const _value) { return test() == _value; }

    static void       set()            { type::set(value);            }
    static void       clear()          { type::clear(value);          }

    /** NOTE: this does not check if _value is masked correctly! */
    static void set(value_type _value) {
      // assert((_value & bitmask) == _value);
      auto r = type::load();
      r &= ~bitmask;
      r |= _value;
      type::store(r);
    }
    static void shift_and_set(value_type _value) {
      set(shifted_value(_value));
    }

    // TODO: better naming for this
    static constexpr value_type shifted_value(value_type _value) {
      return (_value << offset);
    }

    template<std::size_t bit_no>
    struct bit : RegisterBits< type, offset + bit_no, 1 > {
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
    typedef R type;

    static constexpr value_type value = _value << R::offset;

    static_assert((value & R::bitmask) == value, "value does not fit into bits of R");

    static void set()     { type::set(value);   }
    static bool test()    { return type::test(value); }

    // cast operator
    constexpr operator value_type() { return value; }
  };

}

#endif // REGISTER_HPP_INCLUDED
