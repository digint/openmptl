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
    typedef Register<uint32_t, 0xE0001000, Access::rw> CTRL;        /**< Control Register  */
    typedef Register<uint32_t, 0xE0001004, Access::rw> CYCCNT;      /**< Cycle Count Register  */
    typedef Register<uint32_t, 0xE0001008, Access::rw> CPICNT;      /**< CPI Count Register  */
    typedef Register<uint32_t, 0xE000100C, Access::rw> EXCCNT;      /**< Exception Overhead Count Register  */
    typedef Register<uint32_t, 0xE0001010, Access::rw> SLEEPCNT;    /**< Sleep Count Register  */
    typedef Register<uint32_t, 0xE0001014, Access::rw> LSUCNT;      /**< LSU Count Register  */
    typedef Register<uint32_t, 0xE0001018, Access::rw> FOLDCNT;     /**< Folded-instruction Count Register  */
    typedef Register<uint32_t, 0xE000101C, Access::ro> PCSR;        /**< Program Counter Sample Register  */
    typedef Register<uint32_t, 0xE0001020, Access::rw> COMP0;       /**< Comparator Register0  */
    typedef Register<uint32_t, 0xE0001024, Access::rw> MASK0;       /**< Mask Register0  */
    typedef Register<uint32_t, 0xE0001028, Access::rw> FUNCTION0;   /**< Function Register0  */
    typedef Register<uint32_t, 0xE0001030, Access::rw> COMP1;       /**< Comparator Register1  */
    typedef Register<uint32_t, 0xE0001034, Access::rw> MASK1;       /**< Mask Register1  */
    typedef Register<uint32_t, 0xE0001038, Access::rw> FUNCTION1;   /**< Function Register1  */
    typedef Register<uint32_t, 0xE0001040, Access::rw> COMP2;       /**< Comparator Register2  */
    typedef Register<uint32_t, 0xE0001044, Access::rw> MASK2;       /**< Mask Register2  */
    typedef Register<uint32_t, 0xE0001048, Access::rw> FUNCTION2;   /**< Function Register2  */
    typedef Register<uint32_t, 0xE0001050, Access::rw> COMP3;       /**< Comparator Register3  */
    typedef Register<uint32_t, 0xE0001054, Access::rw> MASK3;       /**< Mask Register3  */
    typedef Register<uint32_t, 0xE0001058, Access::rw> FUNCTION3;   /**< Function Register3  */
    typedef Register<uint32_t, 0xE0001FD0, Access::ro, 0x04 > PID4; /**< Peripheral identification registers  */
    typedef Register<uint32_t, 0xE0001FD4, Access::ro, 0x00 > PID5;
    typedef Register<uint32_t, 0xE0001FD8, Access::ro, 0x00 > PID6;
    typedef Register<uint32_t, 0xE0001FDC, Access::ro, 0x00 > PID7;
    typedef Register<uint32_t, 0xE0001FE0, Access::ro, 0x02 > PID0;
    typedef Register<uint32_t, 0xE0001FE4, Access::ro, 0xB0 > PID1;
    typedef Register<uint32_t, 0xE0001FE8, Access::ro, 0x3B > PID2;
    typedef Register<uint32_t, 0xE0001FEC, Access::ro, 0x00 > PID3;
    typedef Register<uint32_t, 0xE0001FF0, Access::ro, 0x0D > CID0; /**< Component identification registers  */
    typedef Register<uint32_t, 0xE0001FF4, Access::ro, 0xE0 > CID1;
    typedef Register<uint32_t, 0xE0001FF8, Access::ro, 0x05 > CID2;
    typedef Register<uint32_t, 0xE0001FFC, Access::ro, 0xB1 > CID3;
  };
}

#endif // COMMON_ARM_CORTEX_REG_DWT_HPP_INCLUDED
