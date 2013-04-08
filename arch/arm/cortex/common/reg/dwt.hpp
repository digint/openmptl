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

namespace reg
{
  /**
   * DWT Register
   *
   * For details, see "Cortex-M3 Technical Reference Manual":
   * <http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3/index.html>
   */
  struct DWT
  {
    using CTRL       = Register<uint32_t, 0xE0001000, Access::rw>;        /**< Control Register  */
    using CYCCNT     = Register<uint32_t, 0xE0001004, Access::rw>;        /**< Cycle Count Register  */
    using CPICNT     = Register<uint32_t, 0xE0001008, Access::rw>;        /**< CPI Count Register  */
    using EXCCNT     = Register<uint32_t, 0xE000100C, Access::rw>;        /**< Exception Overhead Count Register  */
    using SLEEPCNT   = Register<uint32_t, 0xE0001010, Access::rw>;        /**< Sleep Count Register  */
    using LSUCNT     = Register<uint32_t, 0xE0001014, Access::rw>;        /**< LSU Count Register  */
    using FOLDCNT    = Register<uint32_t, 0xE0001018, Access::rw>;        /**< Folded-instruction Count Register  */
    using PCSR       = Register<uint32_t, 0xE000101C, Access::ro>;        /**< Program Counter Sample Register  */
    using COMP0      = Register<uint32_t, 0xE0001020, Access::rw>;        /**< Comparator Register0  */
    using MASK0      = Register<uint32_t, 0xE0001024, Access::rw>;        /**< Mask Register0  */
    using FUNCTION0  = Register<uint32_t, 0xE0001028, Access::rw>;        /**< Function Register0  */
    using COMP1      = Register<uint32_t, 0xE0001030, Access::rw>;        /**< Comparator Register1  */
    using MASK1      = Register<uint32_t, 0xE0001034, Access::rw>;        /**< Mask Register1  */
    using FUNCTION1  = Register<uint32_t, 0xE0001038, Access::rw>;        /**< Function Register1  */
    using COMP2      = Register<uint32_t, 0xE0001040, Access::rw>;        /**< Comparator Register2  */
    using MASK2      = Register<uint32_t, 0xE0001044, Access::rw>;        /**< Mask Register2  */
    using FUNCTION2  = Register<uint32_t, 0xE0001048, Access::rw>;        /**< Function Register2  */
    using COMP3      = Register<uint32_t, 0xE0001050, Access::rw>;        /**< Comparator Register3  */
    using MASK3      = Register<uint32_t, 0xE0001054, Access::rw>;        /**< Mask Register3  */
    using FUNCTION3  = Register<uint32_t, 0xE0001058, Access::rw>;        /**< Function Register3  */
    using PID4       = Register<uint32_t, 0xE0001FD0, Access::ro, 0x04 >; /**< Peripheral identification registers  */
    using PID5       = Register<uint32_t, 0xE0001FD4, Access::ro, 0x00 >;
    using PID6       = Register<uint32_t, 0xE0001FD8, Access::ro, 0x00 >;
    using PID7       = Register<uint32_t, 0xE0001FDC, Access::ro, 0x00 >;
    using PID0       = Register<uint32_t, 0xE0001FE0, Access::ro, 0x02 >;
    using PID1       = Register<uint32_t, 0xE0001FE4, Access::ro, 0xB0 >;
    using PID2       = Register<uint32_t, 0xE0001FE8, Access::ro, 0x3B >;
    using PID3       = Register<uint32_t, 0xE0001FEC, Access::ro, 0x00 >;
    using CID0       = Register<uint32_t, 0xE0001FF0, Access::ro, 0x0D >; /**< Component identification registers  */
    using CID1       = Register<uint32_t, 0xE0001FF4, Access::ro, 0xE0 >;
    using CID2       = Register<uint32_t, 0xE0001FF8, Access::ro, 0x05 >;
    using CID3       = Register<uint32_t, 0xE0001FFC, Access::ro, 0xB1 >;
  };
}

#endif // COMMON_ARM_CORTEX_REG_DWT_HPP_INCLUDED
