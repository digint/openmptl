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

#ifndef COMMON_ARM_CORTEX_REG_DWT_HPP_INCLUDED
#define COMMON_ARM_CORTEX_REG_DWT_HPP_INCLUDED

#include <register.hpp>

namespace mptl { namespace reg {

/**
 * DWT Register
 *
 * For details, see "Cortex-M3 Technical Reference Manual":
 * <http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3/index.html>
 */
struct DWT
{
  using CTRL       = regdef<uint32_t, 0xE0001000, reg_access::rw>;        /**< Control Register  */
  using CYCCNT     = regdef<uint32_t, 0xE0001004, reg_access::rw>;        /**< Cycle Count Register  */
  using CPICNT     = regdef<uint32_t, 0xE0001008, reg_access::rw>;        /**< CPI Count Register  */
  using EXCCNT     = regdef<uint32_t, 0xE000100C, reg_access::rw>;        /**< Exception Overhead Count Register  */
  using SLEEPCNT   = regdef<uint32_t, 0xE0001010, reg_access::rw>;        /**< Sleep Count Register  */
  using LSUCNT     = regdef<uint32_t, 0xE0001014, reg_access::rw>;        /**< LSU Count Register  */
  using FOLDCNT    = regdef<uint32_t, 0xE0001018, reg_access::rw>;        /**< Folded-instruction Count Register  */
  using PCSR       = regdef<uint32_t, 0xE000101C, reg_access::ro>;        /**< Program Counter Sample Register  */
  using COMP0      = regdef<uint32_t, 0xE0001020, reg_access::rw>;        /**< Comparator Register0  */
  using MASK0      = regdef<uint32_t, 0xE0001024, reg_access::rw>;        /**< Mask Register0  */
  using FUNCTION0  = regdef<uint32_t, 0xE0001028, reg_access::rw>;        /**< Function Register0  */
  using COMP1      = regdef<uint32_t, 0xE0001030, reg_access::rw>;        /**< Comparator Register1  */
  using MASK1      = regdef<uint32_t, 0xE0001034, reg_access::rw>;        /**< Mask Register1  */
  using FUNCTION1  = regdef<uint32_t, 0xE0001038, reg_access::rw>;        /**< Function Register1  */
  using COMP2      = regdef<uint32_t, 0xE0001040, reg_access::rw>;        /**< Comparator Register2  */
  using MASK2      = regdef<uint32_t, 0xE0001044, reg_access::rw>;        /**< Mask Register2  */
  using FUNCTION2  = regdef<uint32_t, 0xE0001048, reg_access::rw>;        /**< Function Register2  */
  using COMP3      = regdef<uint32_t, 0xE0001050, reg_access::rw>;        /**< Comparator Register3  */
  using MASK3      = regdef<uint32_t, 0xE0001054, reg_access::rw>;        /**< Mask Register3  */
  using FUNCTION3  = regdef<uint32_t, 0xE0001058, reg_access::rw>;        /**< Function Register3  */
  using PID4       = regdef<uint32_t, 0xE0001FD0, reg_access::ro, 0x04 >; /**< Peripheral identification registers  */
  using PID5       = regdef<uint32_t, 0xE0001FD4, reg_access::ro, 0x00 >;
  using PID6       = regdef<uint32_t, 0xE0001FD8, reg_access::ro, 0x00 >;
  using PID7       = regdef<uint32_t, 0xE0001FDC, reg_access::ro, 0x00 >;
  using PID0       = regdef<uint32_t, 0xE0001FE0, reg_access::ro, 0x02 >;
  using PID1       = regdef<uint32_t, 0xE0001FE4, reg_access::ro, 0xB0 >;
  using PID2       = regdef<uint32_t, 0xE0001FE8, reg_access::ro, 0x3B >;
  using PID3       = regdef<uint32_t, 0xE0001FEC, reg_access::ro, 0x00 >;
  using CID0       = regdef<uint32_t, 0xE0001FF0, reg_access::ro, 0x0D >; /**< Component identification registers  */
  using CID1       = regdef<uint32_t, 0xE0001FF4, reg_access::ro, 0xE0 >;
  using CID2       = regdef<uint32_t, 0xE0001FF8, reg_access::ro, 0x05 >;
  using CID3       = regdef<uint32_t, 0xE0001FFC, reg_access::ro, 0xB1 >;
};

} } // namespace mptl::reg

#endif // COMMON_ARM_CORTEX_REG_DWT_HPP_INCLUDED
