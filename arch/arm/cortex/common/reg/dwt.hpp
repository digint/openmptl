/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright (C) 2012-2017 Axel Burri <axel@tty0.ch>
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

#ifndef ARM_CORTEX_COMMON_REG_DWT_HPP_INCLUDED
#define ARM_CORTEX_COMMON_REG_DWT_HPP_INCLUDED

#include <register.hpp>

namespace mptl {

/**
 * DWT Register
 *
 * For details, see "Cortex-M3 Technical Reference Manual":
 * <http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3/index.html>
 */
struct DWT
{
  using CTRL       = reg<uint32_t, 0xE0001000, rw>;        /**< Control Register  */
  using CYCCNT     = reg<uint32_t, 0xE0001004, rw>;        /**< Cycle Count Register  */
  using CPICNT     = reg<uint32_t, 0xE0001008, rw>;        /**< CPI Count Register  */
  using EXCCNT     = reg<uint32_t, 0xE000100C, rw>;        /**< Exception Overhead Count Register  */
  using SLEEPCNT   = reg<uint32_t, 0xE0001010, rw>;        /**< Sleep Count Register  */
  using LSUCNT     = reg<uint32_t, 0xE0001014, rw>;        /**< LSU Count Register  */
  using FOLDCNT    = reg<uint32_t, 0xE0001018, rw>;        /**< Folded-instruction Count Register  */
  using PCSR       = reg<uint32_t, 0xE000101C, ro>;        /**< Program Counter Sample Register  */
  using COMP0      = reg<uint32_t, 0xE0001020, rw>;        /**< Comparator Register0  */
  using MASK0      = reg<uint32_t, 0xE0001024, rw>;        /**< Mask Register0  */
  using FUNCTION0  = reg<uint32_t, 0xE0001028, rw>;        /**< Function Register0  */
  using COMP1      = reg<uint32_t, 0xE0001030, rw>;        /**< Comparator Register1  */
  using MASK1      = reg<uint32_t, 0xE0001034, rw>;        /**< Mask Register1  */
  using FUNCTION1  = reg<uint32_t, 0xE0001038, rw>;        /**< Function Register1  */
  using COMP2      = reg<uint32_t, 0xE0001040, rw>;        /**< Comparator Register2  */
  using MASK2      = reg<uint32_t, 0xE0001044, rw>;        /**< Mask Register2  */
  using FUNCTION2  = reg<uint32_t, 0xE0001048, rw>;        /**< Function Register2  */
  using COMP3      = reg<uint32_t, 0xE0001050, rw>;        /**< Comparator Register3  */
  using MASK3      = reg<uint32_t, 0xE0001054, rw>;        /**< Mask Register3  */
  using FUNCTION3  = reg<uint32_t, 0xE0001058, rw>;        /**< Function Register3  */
  using PID4       = reg<uint32_t, 0xE0001FD0, ro, 0x04 >; /**< Peripheral identification registers  */
  using PID5       = reg<uint32_t, 0xE0001FD4, ro, 0x00 >;
  using PID6       = reg<uint32_t, 0xE0001FD8, ro, 0x00 >;
  using PID7       = reg<uint32_t, 0xE0001FDC, ro, 0x00 >;
  using PID0       = reg<uint32_t, 0xE0001FE0, ro, 0x02 >;
  using PID1       = reg<uint32_t, 0xE0001FE4, ro, 0xB0 >;
  using PID2       = reg<uint32_t, 0xE0001FE8, ro, 0x3B >;
  using PID3       = reg<uint32_t, 0xE0001FEC, ro, 0x00 >;
  using CID0       = reg<uint32_t, 0xE0001FF0, ro, 0x0D >; /**< Component identification registers  */
  using CID1       = reg<uint32_t, 0xE0001FF4, ro, 0xE0 >;
  using CID2       = reg<uint32_t, 0xE0001FF8, ro, 0x05 >;
  using CID3       = reg<uint32_t, 0xE0001FFC, ro, 0xB1 >;
};

} // namespace mptl

#endif // ARM_CORTEX_COMMON_REG_DWT_HPP_INCLUDED
