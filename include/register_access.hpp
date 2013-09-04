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
#include <register_type.hpp>
#include <arch/bitband.hpp>

#ifdef OPENMPTL_SIMULATION
#  include <register_sim.hpp>
#endif

namespace mptl {

#ifndef OPENMPTL_SIMULATION

template<
  typename   Tp,
  reg_addr_t _addr,
  reg_perm   _permission,
  Tp         reset_value
  >
struct reg_access
{
  static_assert(std::is_integral<Tp>::value,  "Tp is not an integral type");
  static_assert(std::is_unsigned<Tp>::value,  "Tp is not an unsigned type");
  static_assert(!std::is_volatile<Tp>::value, "Tp is a volatile-qualified type (why would you want this?)");

  /** Integral type used for register access. */
  using value_type = Tp;

  static constexpr reg_addr_t addr       = _addr;
  static constexpr reg_perm   permission = _permission;

  static constexpr bool bitop_enabled = bitband_periph::covered(addr);

  /** Load (read) register value. */
  static __always_inline Tp load(void) {
    static_assert(permission != wo, "read access to a write-only register");
    return *reinterpret_cast<volatile Tp *>(addr);
  }

  /** Store (write) a register value. */
  static __always_inline void store(Tp const value) {
    static_assert(permission != ro, "write access to a read-only register");
    *reinterpret_cast<volatile Tp *>(addr) = value;
  }

  /** Set a single bit using bitband_periph::bitset<> */
  template<unsigned bit_no>
  static __always_inline void bitset() {
    static_assert(permission != ro, "write access to a read-only register");
    bitband_periph::bitset<addr, bit_no>();
  }

  /** Clear a single bit using bitband_periph::bitclear<> */
  template<unsigned bit_no>
  static __always_inline void bitclear() {
    static_assert(permission != ro, "write access to a read-only register");
    bitband_periph::bitclear<addr, bit_no>();
  }

  /** Returns the value of a single bit using bitband_periph::bittest<> */
  template<unsigned bit_no>
  static __always_inline bool bittest() {
    static_assert(permission != wo, "read access to a write-only register");
    return bitband_periph::bittest<addr, bit_no>();
  }
};


#else  ////////////////////  OPENMPTL_SIMULATION  ////////////////////


template<
  typename   Tp,
  reg_addr_t _addr,
  reg_perm   _permission,
  Tp         reset_value
  >
class reg_access
{
  static void store_impl(Tp const value) {
    static_assert(permission != ro, "write access to a read-only register");

#ifdef CONFIG_REGISTER_REACTION
    sim::reg_reaction reaction(addr, reg_value);
#endif
    reg_value = value;
#ifdef CONFIG_REGISTER_REACTION
    reaction.react();
#endif
  }

public:
  using value_type = Tp;
  static Tp reg_value;

  static constexpr reg_addr_t addr       = _addr;
  static constexpr reg_perm   permission = _permission;

  static constexpr bool bitop_enabled = bitband_periph::covered(addr);

#ifdef CONFIG_DUMP_REGISTER_ACCESS
  using dumper = sim::reg_dumper<Tp, addr>;
#endif

  static Tp load() {
    static_assert(permission != wo, "read access to a write-only register");
#ifdef CONFIG_DUMP_REGISTER_ACCESS
    dumper::dump_register_load(reg_value);
#endif // CONFIG_DUMP_REGISTER_ACCESS
    return reg_value;
  }

  static void store(Tp const value) {
#ifdef CONFIG_DUMP_REGISTER_ACCESS
    dumper::dump_register_store(reg_value, value);
#endif
    store_impl(value);
  }

  template<unsigned bit_no>
  static __always_inline void bitset() {
    value_type value = reg_value | (1 << bit_no);
#ifdef CONFIG_DUMP_REGISTER_ACCESS
    dumper::dump_register_bitset(reg_value, value);
#endif
    store_impl(value);
  }

  template<unsigned bit_no>
  static __always_inline void bitclear() {
    value_type value = reg_value & ~(1 << bit_no);
#ifdef CONFIG_DUMP_REGISTER_ACCESS
    dumper::dump_register_bitclear(reg_value, value);
#endif
    store_impl(value);
  }

  template<unsigned bit_no>
  static __always_inline bool bittest() {
    static_assert(permission != wo, "read access to a write-only register");
    value_type value = reg_value & (1 << bit_no);
#ifdef CONFIG_DUMP_REGISTER_ACCESS
    dumper::dump_register_bittest(value);
#endif
    return value;
  }
};

/* initialize reg_value to the reset value */
template<
  typename   Tp,
  reg_addr_t addr,
  reg_perm   permission,
  Tp         reset_value
  >
Tp reg_access<Tp, addr, permission, reset_value>::reg_value = reset_value;

#endif // OPENMPTL_SIMULATION

} // namespace mptl

#endif // REGISTER_BACKEND_HPP_INCLUDED
