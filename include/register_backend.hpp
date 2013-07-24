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

#ifndef REGISTER_BACKEND_HPP_INCLUDED
#define REGISTER_BACKEND_HPP_INCLUDED

#include <compiler.h>

#ifdef OPENMPTL_SIMULATION
#include <arch/reg/address_map.hpp>
#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <bitset>
#endif // OPENMPTL_SIMULATION

namespace mptl {

/** Register access */
enum class reg_access { ro, wo, rw };

#ifndef OPENMPTL_SIMULATION

typedef uintptr_t  reg_addr_t;  /**< Register address type (uintptr_t: unsigned integer type capable of holding a pointer)  */

template< typename   T,
          reg_addr_t _addr,
          reg_access _access,
          T          reset_value >
struct regdef_backend
{
  static_assert(std::is_integral<T>::value, "T is not an integral type");
  static_assert(std::is_unsigned<T>::value, "T is not an unsigned type");
  static_assert(!std::is_volatile<T>::value, "T is is a volatile-qualified type (why would you want this?)");

  static constexpr reg_access access = _access;
  static constexpr reg_addr_t addr   = _addr;

  /** Integral type used for register access. */
  typedef T value_type;

#ifdef CONSTEXPR_REINTERPRET_CAST_ALLOWED
  /* For some reasons, reinterpret_cast is not allowed anymore in
   * constant expressions (C++11 standard N3376, 5.19). In older
   * drafts (N3242), this was allowed. Disabled here, since
   * clang-3.2 does not allow it (gcc-4.8 is less permissive here).
   */
  static constexpr volatile T * value_ptr = reinterpret_cast<volatile T *>(addr);
#endif

  /** Load (read) register value. */
  static __always_inline T load(void) {
    static_assert(access != reg_access::wo, "read access to a write-only register");
#ifdef CONSTEXPR_REINTERPRET_CAST_ALLOWED
    return *value_ptr;
#else
    return *reinterpret_cast<volatile T *>(addr);
#endif
  }

  /** Store (write) a register value. */
  static __always_inline void store(T const value) {
    static_assert(access != reg_access::ro, "write access to a read-only register");
#ifdef CONSTEXPR_REINTERPRET_CAST_ALLOWED
    *value_ptr = value;
#else
    *reinterpret_cast<volatile T *>(addr) = value;
#endif
  }
};


#else  ////////////////////  OPENMPTL_SIMULATION  ////////////////////


typedef uint32_t  reg_addr_t;

static int reaction_running;

class reg_reaction
{
  reg_addr_t addr;
  uint32_t old_value;

  template<typename R>
  bool bits_set(void) {
    return (R::test() && !(old_value & R::value));
  }

  template<typename R>
  bool bits_cleared(void) {
    return ((old_value & R::value) && !R::test());
  }

public:
  reg_reaction(reg_addr_t _addr, uint32_t _old_value) : addr(_addr), old_value(_old_value) {
    reaction_running++;
  };
  ~reg_reaction(void) {
    reaction_running--;
  };
  void react();
};

template< typename   T,
          reg_addr_t _addr,
          reg_access _access,
          T          reset_value >
struct regdef_backend
{
  using value_type = T;
  static T reg_value;

  static constexpr reg_access access = _access;
  static constexpr reg_addr_t addr   = _addr;

  static constexpr unsigned dump_bitsize = 32; /**< limit size to 32bit, since uint_fast16_t on 64bit system has 64bit width, making the output look crappy... */
  static constexpr unsigned additional_space = 6;
  static constexpr unsigned desc_max_width = 14;
  static constexpr unsigned addr_width = sizeof(reg_addr_t) * 2 + 2;

  static std::string bitfield_str(T const value) {
    static constexpr std::size_t bitsize = sizeof(T) * 8;
    static constexpr std::size_t print_bitsize = bitsize > dump_bitsize ? dump_bitsize : bitsize;

    std::string s = std::bitset<print_bitsize>(value).to_string();
    s.insert(0, (dump_bitsize - print_bitsize), ' ');    /* insert blanks to front */
    for(unsigned i = dump_bitsize - 8; i > 0; i -= 8)
      s.insert(i, 1, ' ');

    return "[ " + s + " ]";
  }

  static void print_reg(T const value) {
    static constexpr std::size_t size = sizeof(T);
    static constexpr std::size_t print_size = size > (dump_bitsize / 8) ? (dump_bitsize / 8) : size;

    std::cerr << std::setw((4 - print_size) * 2 + 2) << std::hex << std::right << "0x"
              << std::setfill('0') << std::setw(print_size * 2) << +value;  // '+value' makes sure a char is printed as number

#ifdef DEBUG_DUMP_BITFIELD
    std::cerr << "   ";
    std::cerr << bitfield_str(value);
#endif // DEBUG_DUMP_BITFIELD

#if 0    // mark cropped register with '~'
    if(print_size < size)
      std::cerr << "~";
#endif
  }

  static void print_address_map(void) {
    if(reg::address_map<addr>::name_str)  /* lookup register name */
      std::cerr << std::left << std::setfill(' ') << std::setw(addr_width + additional_space) << reg::address_map<addr>::name_str;
    else
      std::cerr << "0x" << std::hex << std::right << std::setfill('0') << std::setw(sizeof(reg_addr_t) * 2) << addr << std::setfill(' ')  << std::setw(additional_space) << "";
  }

  static void print_desc(const char * desc) {
    std::cerr << std::left << std::setfill(' ') << std::setw(desc_max_width) << desc;
  }

  static void print_info_line(const char * desc, T value) {
    print_address_map();
    print_desc(desc);
    print_reg(value);
    std::cerr << std::endl;
  }

#ifdef DEBUG_DUMP_CURRENT_REGISTER_VALUE
  static void print_info_line(const char * desc, T value_cur, T value_new) {
    print_info_line("==", value_cur);
    print_info_line(desc, value_new);
  }
#else
  static void print_info_line(const char * desc, T, T value_new) {
    print_info_line(desc, value_new);
  }
#endif // DEBUG_DUMP_CURRENT_REGISTER_VALUE

  static T load() {
    static_assert(access != reg_access::wo, "read access to a write-only register");
#ifdef DEBUG_REGISTER
    if(!reaction_running)
      print_info_line("::load()", reg_value);
#endif
    return reg_value;
  }

  static void store(T const value) {
    static_assert(access != reg_access::ro, "write access to a read-only register");
#ifdef DEBUG_REGISTER
    if(reaction_running)
      print_info_line("++react", reg_value, value);
    else
      if(reg_value == value)  // notify with '~' if cur=new (candidates for optimization!)
        print_info_line("::store()~", reg_value, value);
      else
        print_info_line("::store()", reg_value, value);
#endif
    reg_reaction reaction(addr, reg_value);
    reg_value = value;
    reaction.react();
  }
};

/* initialize reg_value to the reset value */
template< typename   T,
          reg_addr_t addr,
          reg_access access,
          T          reset_value >
T regdef_backend<T, addr, access, reset_value>::reg_value = reset_value;

#endif // OPENMPTL_SIMULATION

} // namespace mptl

#endif // REGISTER_BACKEND_HPP_INCLUDED
