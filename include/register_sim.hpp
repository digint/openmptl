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

#ifndef REGISTER_SIM_HPP_INCLUDED
#define REGISTER_SIM_HPP_INCLUDED

#include <arch/reg/address_map.hpp>
#include <ostream>
#include <iomanip>
#include <typeinfo>
#include <bitset>
#include <mutex>

namespace mptl { namespace sim {

extern std::ostream & regdump_ostream;
extern thread_local int regdump_reaction_running;

#ifdef CONFIG_SIM_THREADED
extern std::mutex regdump_mutex;
#  define REGDUMP_LOCK     regdump_mutex.lock()
#  define REGDUMP_UNLOCK   regdump_mutex.unlock()
#else
#  define REGDUMP_LOCK
#  define REGDUMP_UNLOCK
#endif // CONFIG_SIM_THREADED


////////////////////  reg_reaction  ////////////////////


class reg_reaction
{
  reg_addr_t addr;
  uint32_t old_value;

public:

  template<typename Tp>
  bool bits_set(void) const {
    if(Tp::reg_type::addr != addr)
      return false;
    return (Tp::test() && !(old_value & Tp::value));
  }

  template<typename Tp>
  bool bits_cleared(void) const {
    if(Tp::reg_type::addr != addr)
      return false;
    return ((old_value & Tp::value) && !Tp::test());
  }

  reg_reaction(reg_addr_t _addr, uint32_t _old_value) : addr(_addr), old_value(_old_value) {
    regdump_reaction_running++;
  };
  ~reg_reaction(void) {
    regdump_reaction_running--;
  };

  /** Reaction function, must be implemented per-project */
  void react();
};


////////////////////  reg_dumper  ////////////////////


template<typename value_type, reg_addr_t addr>
class reg_dumper
{
  static constexpr unsigned max_bits = 32;  /**< limit size to 32bit, since uint_fast16_t on 64bit system has 64bit width, making the output look crappy... */
  static constexpr unsigned desc_max_width = 14;
  static constexpr unsigned addr_max_width = 16;

  static std::string bitfield_str(value_type const value) {
    static constexpr std::size_t bitsize = sizeof(value_type) * 8;
    static constexpr std::size_t print_bitsize = bitsize > max_bits ? max_bits : bitsize;

    std::string s = std::bitset<print_bitsize>(value).to_string();
    s.insert(0, (max_bits - print_bitsize), ' ');    /* insert blanks to front */
    for(unsigned i = max_bits - 8; i > 0; i -= 8)
      s.insert(i, 1, ' ');

    return "[ " + s + " ]";
  }

  static void print_value(value_type const value) {
    static constexpr std::size_t size = sizeof(value_type);
    static constexpr std::size_t print_size = size > (max_bits / 8) ? (max_bits / 8) : size;

    regdump_ostream << std::setw((4 - print_size) * 2 + 2) << std::hex << std::right << "0x"
                    << std::setfill('0') << std::setw(print_size * 2) << +value;  // '+value' makes sure a char is printed as number

#ifdef CONFIG_DUMP_REGISTER_BITFIELD
    regdump_ostream << "   ";
    regdump_ostream << bitfield_str(value);
#endif // CONFIG_DUMP_REGISTER_BITFIELD

#if 0    // mark cropped register with '~'
    if(print_size < size)
      regdump_ostream << "~";
#endif
  }

  static void print_address(void) {
    static constexpr unsigned addr_width = sizeof(reg_addr_t) * 2;

    if(reg::address_map<addr>::name_str)  /* lookup register name */
      regdump_ostream << std::left << std::setfill(' ') << std::setw(addr_max_width) << reg::address_map<addr>::name_str;
    else
      regdump_ostream << "0x" << std::hex << std::right << std::setfill('0') << std::setw(addr_width) << addr << std::setfill(' ')  << std::setw(addr_max_width - 2 - addr_width) << "";
  }

  static void print_desc(const char * desc) {
    regdump_ostream << std::left << std::setfill(' ') << std::setw(desc_max_width) << desc;
  }


  static void print_info_line(const char * desc, value_type value) {
    print_address();
    print_desc(desc);
    print_value(value);
    regdump_ostream << std::endl;
  }

public:

  static void dump_register_access(const char * desc, value_type value, bool print_current_value = false) {
    REGDUMP_LOCK;
    if(print_current_value) {
#ifdef CONFIG_DEBUG_DUMP_CURRENT_REGISTER_VALUE
      print_info_line("==", reg_value);
#endif // CONFIG_DEBUG_DUMP_CURRENT_REGISTER_VALUE
    }
    print_info_line(desc, value);
    REGDUMP_UNLOCK;
  }

  static void dump_register_load(value_type value) {
#ifdef CONFIG_REGISTER_REACTION
    if(regdump_reaction_running == 0)
#endif
      dump_register_access("::load()", value);
  }

  static void dump_register_store(value_type cur_value, value_type new_value) {
#ifdef CONFIG_REGISTER_REACTION
    if(regdump_reaction_running != 0)
      dump_register_access("++react", new_value, true);
    else
#endif // CONFIG_REGISTER_REACTION
      if(cur_value == new_value)  // notify with '~' if cur=new (candidates for optimization!)
        dump_register_access("::store()~", new_value, true);
      else
        dump_register_access("::store()", new_value, true);
  }
};

} } // namespace mptl::sim

#endif // REGISTER_SIM_HPP_INCLUDED
