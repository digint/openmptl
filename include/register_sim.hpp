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
#include <algorithm>
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

#ifdef CONFIG_REGISTER_REACTION
#  define RETURN_IF_REACTION if(regdump_reaction_running != 0) { return; }
#  define REACTION_CONDITIONAL(a,b) ((regdump_reaction_running == 0) ? a : b)
#else
#  define RETURN_IF_REACTION
#  define REACTION_CONDITIONAL(a,b) a
#endif

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

  reg_reaction(reg_addr_t _addr, uint32_t _old_value) : addr(_addr), old_value(_old_value) { };

  /** Reaction function, must be implemented per-project */
  void react();

  void info(const std::string & str) const {
    REGDUMP_LOCK;
    regdump_ostream << "[INFO] " << str << std::endl;
    REGDUMP_UNLOCK;
  }
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

  static std::string bitmask_str(value_type const value, const char bitmask_char = '1') {
    std::string s = bitfield_str(value);
    std::replace(s.begin(), s.end(), '0', '.');
    if(bitmask_char != '1')
      std::replace(s.begin(), s.end(), '1', bitmask_char);
    return s;
  }

  static void print_hex_value(value_type const value) {
    static constexpr std::size_t size = sizeof(value_type);
    static constexpr std::size_t print_size = size > (max_bits / 8) ? (max_bits / 8) : size;

    regdump_ostream << std::setw((4 - print_size) * 2 + 2) << std::hex << std::right << "0x"
                    << std::setfill('0') << std::setw(print_size * 2) << +value;  // '+value' makes sure a char is printed as number

#if 0    // mark cropped register with '~'
    if(print_size < size)
      regdump_ostream << "~";
#endif
  }

  static void print_address(void) {
    static constexpr unsigned addr_width = sizeof(reg_addr_t) * 2;

    if(address_map<addr>::name_str)  /* lookup register name */
      regdump_ostream << std::left << std::setfill(' ') << std::setw(addr_max_width) << address_map<addr>::name_str;
    else
      regdump_ostream << "0x" << std::hex << std::right << std::setfill('0') << std::setw(addr_width) << addr << std::setfill(' ')  << std::setw(addr_max_width - 2 - addr_width) << "";
  }

  static void print_desc(const std::string & desc) {
    regdump_ostream << std::left << std::setfill(' ') << std::setw(desc_max_width) << desc;
  }

  static void print_action(const std::string & desc, value_type value, value_type bitfield_value, const char bitmask_char = 0) {
    print_address();
    print_desc(desc);
    print_hex_value(value);
#ifdef CONFIG_DUMP_REGISTER_BITFIELD
    regdump_ostream << "   " << (bitmask_char ? bitmask_str(bitfield_value, bitmask_char) : bitfield_str(bitfield_value));
#else
    // silence "unused parameter" warning
    (void)bitfield_value;
    (void)bitmask_char;
#endif // CONFIG_DUMP_REGISTER_BITFIELD
    regdump_ostream << std::endl;
  }

  static void print_action(const std::string & desc, value_type value) {
    print_action(desc, value, value, 0);
  }

#ifdef CONFIG_DUMP_CURRENT_REGISTER_VALUE
  static void print_reg_value(value_type value) {
    print_action("", value);
  }
#else
  static void print_reg_value(value_type) { }
#endif

public:

  static void dump_register_load(value_type value) {
    RETURN_IF_REACTION;

    REGDUMP_LOCK;
    print_action("::load()", value);
    REGDUMP_UNLOCK;
  }

  static void dump_register_store(value_type cur_value, value_type new_value) {
    std::string desc = REACTION_CONDITIONAL("::store()", "##store()");
    if(cur_value == new_value)  // notify with '~' if cur=new (candidates for optimization!)
      desc.append("~");

    REGDUMP_LOCK;
    print_reg_value(cur_value);
    print_action(desc, new_value);
    REGDUMP_UNLOCK;
  }

  static void dump_register_bitset(value_type cur_value, unsigned bit_no) {
    std::string desc = REACTION_CONDITIONAL("::bitset()", "##bitset()");
    value_type mask = 1 << bit_no;
    if(cur_value & (mask))  // notify with '~' if bit is already set (candidates for optimization!)
      desc.append("~");

    REGDUMP_LOCK;
    print_reg_value(cur_value);
    print_action(desc, mask, mask, '1');
    REGDUMP_UNLOCK;
  }

  static void dump_register_bitclear(value_type cur_value, unsigned bit_no) {
    std::string desc = REACTION_CONDITIONAL("::bitclear()", "##bitclear()");
    value_type mask = 1 << bit_no;
    if((cur_value & mask) == 0)  // notify with '~' if bit is already cleared (candidates for optimization!)
      desc.append("~");

    REGDUMP_LOCK;
    print_reg_value(cur_value);
    print_action(desc, mask, mask, 'X');
    REGDUMP_UNLOCK;
  }

  static void dump_register_bittest(value_type cur_value, value_type bit_no) {
    RETURN_IF_REACTION;

    value_type mask = 1 << bit_no;
    value_type value = cur_value & mask;
    const char bitmask_char = (value) ? '1' : '0';

    REGDUMP_LOCK;
    print_reg_value(cur_value);
    print_action("::bittest()", value, mask, bitmask_char);
    REGDUMP_UNLOCK;
  }
};

} } // namespace mptl::sim

#endif // REGISTER_SIM_HPP_INCLUDED
