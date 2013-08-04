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

/** Register address type (uintptr_t: unsigned integer type capable of holding a pointer)  */
using reg_addr_t = uintptr_t;

template< typename   Tp,
          reg_addr_t _addr,
          reg_access _access,
          Tp         reset_value >
struct regdef_backend
{
  static_assert(std::is_integral<Tp>::value,  "Tp is not an integral type");
  static_assert(std::is_unsigned<Tp>::value,  "Tp is not an unsigned type");
  static_assert(!std::is_volatile<Tp>::value, "Tp is a volatile-qualified type (why would you want this?)");

  static constexpr reg_access access = _access;
  static constexpr reg_addr_t addr   = _addr;

  /** Integral type used for register access. */
  typedef Tp value_type;

#ifdef CONSTEXPR_REINTERPRET_CAST_ALLOWED
  /* For some reasons, reinterpret_cast is not allowed anymore in
   * constant expressions (C++11 standard N3376, 5.19). In older
   * drafts (N3242), this was allowed. Disabled here, since
   * clang-3.2 does not allow it (gcc-4.8 is less permissive here).
   */
  static constexpr volatile Tp * value_ptr = reinterpret_cast<volatile Tp *>(addr);
#endif

  /** Load (read) register value. */
  static __always_inline Tp load(void) {
    static_assert(access != reg_access::wo, "read access to a write-only register");
#ifdef CONSTEXPR_REINTERPRET_CAST_ALLOWED
    return *value_ptr;
#else
    return *reinterpret_cast<volatile Tp *>(addr);
#endif
  }

  /** Store (write) a register value. */
  static __always_inline void store(Tp const value) {
    static_assert(access != reg_access::ro, "write access to a read-only register");
#ifdef CONSTEXPR_REINTERPRET_CAST_ALLOWED
    *value_ptr = value;
#else
    *reinterpret_cast<volatile Tp *>(addr) = value;
#endif
  }
};


#else  ////////////////////  OPENMPTL_SIMULATION  ////////////////////


using reg_addr_t = uint32_t;

#ifdef CONFIG_REGISTER_REACTION
class reg_reaction
{
  static thread_local int refcount;

  reg_addr_t addr;
  uint32_t old_value;

  template<typename Tp>
  bool bits_set(void) {
    return (Tp::test() && !(old_value & Tp::value));
  }

  template<typename Tp>
  bool bits_cleared(void) {
    return ((old_value & Tp::value) && !Tp::test());
  }

public:

  static bool running(void) {
    // assert(refcount >= 0);
    return refcount != 0;
  };

  reg_reaction(reg_addr_t _addr, uint32_t _old_value) : addr(_addr), old_value(_old_value) {
    refcount++;
  };
  ~reg_reaction(void) {
    refcount--;
  };
  void react();
};
#endif // CONFIG_REGISTER_REACTION


template< typename   Tp,
          reg_addr_t _addr,
          reg_access _access,
          Tp         reset_value >
struct regdef_backend
{
  using value_type = Tp;
  static Tp reg_value;

  static constexpr reg_access access = _access;
  static constexpr reg_addr_t addr   = _addr;

  static constexpr unsigned dump_bitsize = 32; /**< limit size to 32bit, since uint_fast16_t on 64bit system has 64bit width, making the output look crappy... */
  static constexpr unsigned additional_space = 6;
  static constexpr unsigned desc_max_width = 14;
  static constexpr unsigned addr_width = sizeof(reg_addr_t) * 2 + 2;

  static std::string bitfield_str(Tp const value) {
    static constexpr std::size_t bitsize = sizeof(Tp) * 8;
    static constexpr std::size_t print_bitsize = bitsize > dump_bitsize ? dump_bitsize : bitsize;

    std::string s = std::bitset<print_bitsize>(value).to_string();
    s.insert(0, (dump_bitsize - print_bitsize), ' ');    /* insert blanks to front */
    for(unsigned i = dump_bitsize - 8; i > 0; i -= 8)
      s.insert(i, 1, ' ');

    return "[ " + s + " ]";
  }

  static void print_reg(Tp const value) {
    static constexpr std::size_t size = sizeof(Tp);
    static constexpr std::size_t print_size = size > (dump_bitsize / 8) ? (dump_bitsize / 8) : size;

    std::cerr << std::setw((4 - print_size) * 2 + 2) << std::hex << std::right << "0x"
              << std::setfill('0') << std::setw(print_size * 2) << +value;  // '+value' makes sure a char is printed as number

#ifdef CONFIG_DUMP_REGISTER_BITFIELD
    std::cerr << "   ";
    std::cerr << bitfield_str(value);
#endif // CONFIG_DUMP_REGISTER_BITFIELD

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

  static void print_info_line(const char * desc, Tp value) {
    print_address_map();
    print_desc(desc);
    print_reg(value);
    std::cerr << std::endl;
  }

#ifdef CONFIG_DEBUG_DUMP_CURRENT_REGISTER_VALUE
  static void print_info_line(const char * desc, Tp value_cur, Tp value_new) {
    print_info_line("==", value_cur);
    print_info_line(desc, value_new);
  }
#else
  static void print_info_line(const char * desc, Tp, Tp value_new) {
    print_info_line(desc, value_new);
  }
#endif // CONFIG_DEBUG_DUMP_CURRENT_REGISTER_VALUE

  static Tp load() {
    static_assert(access != reg_access::wo, "read access to a write-only register");
#ifdef CONFIG_DUMP_REGISTER_ACCESS
#ifdef CONFIG_REGISTER_REACTION
    if(!reg_reaction::running())
#endif
      print_info_line("::load()", reg_value);
#endif // CONFIG_DUMP_REGISTER_ACCESS
    return reg_value;
  }

  static void store(Tp const value) {
    static_assert(access != reg_access::ro, "write access to a read-only register");
#ifdef CONFIG_DUMP_REGISTER_ACCESS
#ifdef CONFIG_REGISTER_REACTION
    if(reg_reaction::running())
      print_info_line("++react", reg_value, value);
    else
#endif // CONFIG_REGISTER_REACTION
      if(reg_value == value)  // notify with '~' if cur=new (candidates for optimization!)
        print_info_line("::store()~", reg_value, value);
      else
        print_info_line("::store()", reg_value, value);
#endif // CONFIG_DUMP_REGISTER_ACCESS

#ifdef CONFIG_REGISTER_REACTION
    reg_reaction reaction(addr, reg_value);
#endif
    reg_value = value;
#ifdef CONFIG_REGISTER_REACTION
    reaction.react();
#endif
  }
};

/* initialize reg_value to the reset value */
template< typename   Tp,
          reg_addr_t addr,
          reg_access access,
          Tp         reset_value >
Tp regdef_backend<Tp, addr, access, reset_value>::reg_value = reset_value;

#endif // OPENMPTL_SIMULATION

} // namespace mptl

#endif // REGISTER_BACKEND_HPP_INCLUDED
