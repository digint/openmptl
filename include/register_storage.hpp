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

#ifndef REGISTER_STORAGE_HPP_INCLUDED
#define REGISTER_STORAGE_HPP_INCLUDED

#ifdef CORE_SIMULATION
#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <bitset>
#endif // CORE_SIMULATION

namespace reg {

  enum class Access { ro, wo, rw };

#ifndef CORE_SIMULATION

  typedef uintptr_t  reg_addr_t;  /**< Register address type (uintptr_t: unsigned integer type capable of holding a pointer)  */

  template< typename   T,
            reg_addr_t addr,
            Access     access,
            T          reset_value >
  struct RegisterStorage
  {
    typedef T value_type; // TODO: volatile?
    static constexpr volatile T * value_ptr = reinterpret_cast<volatile T *>(addr);

    static T    load(void) {
      static_assert(access != Access::wo, "read access to a write-only register");
      return *value_ptr;
    }
    static void store(T const value) {
      static_assert(access != Access::ro, "write access to a read-only register");
      *value_ptr = value;
    }
  };


#else ////////////////////  CORE_SIMULATION  ////////////////////


  typedef uint32_t  reg_addr_t;  /**< Register address type (uintptr_t: unsigned integer type capable of holding a pointer)  */

  template< typename T, reg_addr_t addr >
  struct RegisterReaction
  {
    static void react(T const value);
  };

  template< typename T, reg_addr_t addr >
  void RegisterReaction<T, addr>::react(T const) {
    //  std::cout << "react <default>" << std::endl;
  }


  template< typename   T,
            reg_addr_t addr,
            Access     access,
            T          reset >
  struct RegisterStorage
  {
    typedef T value_type;
    static T reg_value;

    static std::string bitvalue(T const value) {
      std::string s = std::bitset<sizeof(value_type) * 8>(value).to_string();
      for(unsigned i = sizeof(value_type) * 8 - 8; i > 0; i -= 8)
        s.insert(i, 1, ' ');
      return "[ " + s + " ]";
    }

    static T load() {
      static_assert(access != Access::wo, "read access to a write-only register");
      std::cout << "load  "
                << "0x" << std::hex << std::setfill('0') << std::setw(sizeof(reg_addr_t) * 2) << addr
                << "  cur:  0x" << std::hex << std::setfill('0') << std::setw(sizeof(value_type) * 2) << +reg_value  // '+value' makes sure a char is printed as number
                << " = " << bitvalue(reg_value) << std::endl;

      return reg_value;
    }

    static void store(T const value) {
      static_assert(access != Access::ro, "write access to a read-only register");
      std::cout << "store "
                << "0x" << std::hex << std::setfill('0') << std::setw(sizeof(reg_addr_t) * 2) << addr

                << "  cur:  0x" << std::hex << std::setfill('0') << std::setw(sizeof(value_type) * 2) << +reg_value  // '+value' makes sure a char is printed as number
                << " = " << bitvalue(reg_value) << std::endl
                << "                "
                << "  new:  0x" << std::hex << std::setfill('0') << std::setw(sizeof(value_type) * 2) << +value      // '+value' makes sure a char is printed as number
                << " = " << bitvalue(value) << std::endl;

      reg_value = value;
      RegisterReaction<T, addr>::react(value);
    }
  };

  /* initialize reg_value to the reset value */
  template< typename   T,
            reg_addr_t addr,
            Access     access,
            T          reset_value >
  T RegisterStorage<T, addr, access, reset_value>::reg_value = reset_value;

#endif // CORE_SIMULATION

}

#endif // REGISTER_STORAGE_HPP_INCLUDED
