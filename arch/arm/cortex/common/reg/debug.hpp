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

#ifndef COMMON_ARM_CORTEX_REG_DEBUG_HPP_INCLUDED
#define COMMON_ARM_CORTEX_REG_DEBUG_HPP_INCLUDED

#include <register.hpp>

namespace mptl {

/**
 * Debug Register
 *
 * For details, see "Cortex-M3 Technical Reference Manual":
 * <http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3/index.html>
 */
struct DEBUG
{
  using DFSR   = regdef<uint32_t, 0xE000ED30, reg_access::rw>;  /**< Debug Fault Status Register                   */
  using DHCSR  = regdef<uint32_t, 0xE000EDF0, reg_access::rw>;  /**< Debug Halting Control and Status Register     */
  using DCRSR  = regdef<uint32_t, 0xE000EDF4, reg_access::wo>;  /**< Debug Core Register Selector Register         */
  using DCRDR  = regdef<uint32_t, 0xE000EDF8, reg_access::rw>;  /**< Debug Core Register Data Register             */

  /**
   * Debug Exception and Monitor Control Register
   */
  struct DEMCR
  : public regdef<uint32_t, 0xE000EDFC, reg_access::rw>
  {
    using TRCENA  = regbits< type, 24,  1 >;   /**< Enable DWT */
  };
};

} // namespace mptl

#endif // COMMON_ARM_CORTEX_REG_DEBUG_HPP_INCLUDED
