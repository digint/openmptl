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

#ifndef ARM_CORTEX_COMMON_BITBAND_HPP_INCLUDED
#define ARM_CORTEX_COMMON_BITBAND_HPP_INCLUDED

#include <register_type.hpp>

namespace mptl {

struct bitband_periph
{
  static constexpr reg_addr_t region_start = 0x40000000;
  static constexpr reg_addr_t region_end   = 0x40100000;
  static constexpr reg_addr_t alias_base   = 0x42000000;

  using value_type = uint32_t;

  static constexpr bool covered(reg_addr_t addr) {
    return (addr >= region_start) && (addr < region_end);
  }

  template<reg_addr_t addr, unsigned bit_no>
  static __always_inline void bitset(void) {
    //    static_assert(covered(addr), "addr is not covered by the bit-band region");
    static constexpr reg_addr_t addr_bb = alias_base + ((addr - region_start) * 32) + (bit_no * 4);
    *reinterpret_cast<volatile value_type *>(addr_bb) = 1;
  }

  template<reg_addr_t addr, unsigned bit_no>
  static __always_inline void bitclear(void) {
    //    static_assert(covered(addr), "addr is not covered by the bit-band region");
    static constexpr reg_addr_t addr_bb = alias_base + ((addr - region_start) * 32) + (bit_no * 4);
    *reinterpret_cast<volatile value_type *>(addr_bb) = 0;
  }

  template<reg_addr_t addr, unsigned bit_no>
  static __always_inline bool bittest(void) {
    //    static_assert(covered(addr), "addr is not covered by the bit-band region");
    static constexpr reg_addr_t addr_bb = alias_base + ((addr - region_start) * 32) + (bit_no * 4);
    return *reinterpret_cast<volatile value_type *>(addr_bb);
  }
};



} // namespace mptl

#endif // ARM_CORTEX_COMMON_BITBAND_HPP_INCLUDED
