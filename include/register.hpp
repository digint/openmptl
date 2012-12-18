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

#include <type_traits> // std::is_integral<>
#include <cstdint>


typedef uintptr_t  reg_addr_t;  /**< Register address type (uintptr_t: unsigned integer type capable of holding a pointer)  */

enum class RegisterAccess { ro, wo, rw };


#ifndef CORE_SIMULATION

template< typename        T,
          reg_addr_t      addr,
          RegisterAccess  access,
          T               reset >
struct RegisterStorage
{
  typedef T value_type; // TODO: volatile
  static constexpr volatile T * value_ptr = reinterpret_cast<volatile T *>(addr);

  static T    load()               { return *value_ptr;  }
  static void store(T const value) { *value_ptr = value; }
};

#else /* CORE_SIMULATION */

#include <bitset>
#include <iostream>
#include <iomanip>
#include <typeinfo>

template< typename T,
          reg_addr_t      addr
          >
struct RegisterReaction
{
  static void react(T const value);
};

template< typename        T,
          reg_addr_t      addr
          >
void RegisterReaction<T, addr>::react(T const) {
  std::cout << "RegisterReaction <default>" << std::endl;
}



template< typename        T,
          reg_addr_t      addr,
          RegisterAccess  access,
          T               reset >
struct RegisterStorage
{
  typedef T value_type;

  static T reg_value;

  static T load() {
    return reg_value;
  }

  // TODO: global function
  static std::string bitvalue(T const value) {
    std::string s = std::bitset<sizeof(value_type) * 8>(value).to_string();
    for(unsigned i = sizeof(value_type) * 8 - 8; i > 0; i -= 8)
      s.insert(i, 1, ' ');
    return "[ " + s + " ]";
  }

  static void store(T const value) {
    std::cout <<  __PRETTY_FUNCTION__ << std::endl
              << "addr = 0x" << std::hex << std::setfill('0') << std::setw(sizeof(value_type) * 2) << addr  << std::endl
              << "cur  = 0x" << std::hex << std::setfill('0') << std::setw(sizeof(value_type) * 2) << value
              << " = " << bitvalue(value) << std::endl
              << "new  = 0x" << std::hex << std::setfill('0') << std::setw(sizeof(value_type) * 2) << reg_value
              << " = " << bitvalue(reg_value) << std::endl;

    reg_value = value;

    RegisterReaction<T, addr>::react(value);
  }
};

/* initialize reg_value to the reset value */
template< typename        T,
          reg_addr_t      addr,
          RegisterAccess  access,
          T               reset >
T RegisterStorage<T, addr, access, reset>::reg_value = reset;

#endif /* CORE_SIMULATION */




template< typename        T,
          reg_addr_t      _addr,
          RegisterAccess  _access,
          T               _reset_value = 0 >
struct Register
{
  static_assert(std::is_integral<T>::value, "T must be an integral type");
  static_assert(std::is_unsigned<T>::value, "T must be an unsigned type");

  typedef RegisterStorage<T, _addr, _access, _reset_value> type;
  typedef typename type::value_type value_type;

  static constexpr reg_addr_t addr        = _addr;
  static constexpr T          reset_value = _reset_value;

  static T load() {
    static_assert(_access != RegisterAccess::wo, "read access to a write-only register");
    return type::load();
  }

  static void store(T const value) {
    static_assert(_access != RegisterAccess::ro, "write access to a read-only register");
    type::store(value);
  }

  static T    test (T const value) { return type::load() & value;          }
  static void set  (T const value) { type::store( type::load() | value );  }
  static void clear(T const value) { type::store( type::load() & ~value ); }
  static void mask (T const value) { type::store( type::load() & value );  }
  static void reset()              { type::store(reset_value);             }
};



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

  // TODO: use R::, not type::
  static value_type test()           { return type::load() & bitmask;  }
  static value_type test_and_shift() { return test() >> offset; }

  // TODO: prevent access if width == 1
  static bool       test(value_type const _value) {
    return test() == _value;
  }

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
    _value <<= offset;
    set(_value);
  }

  template<std::size_t bit_no>
  struct bit : RegisterBits< type, offset + bit_no, 1 > {
    static_assert(bit_no < width, "invalid bit_no");
  };

  // cast operator
  constexpr operator value_type() { return value; }
};



/** NOTE: _value is shifted with offset of R! */

/*  TODO: _value should actually be of type R::value_type, but
    template declaration forbids this, and we don't want to introduce
    another template parameter. */
template< typename R, uintmax_t _value >
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


#endif // REGISTER_HPP_INCLUDED
