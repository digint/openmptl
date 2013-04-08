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

#ifndef COMMON_ARM_CORTEX_REG_MPU_HPP_INCLUDED
#define COMMON_ARM_CORTEX_REG_MPU_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /**
   *  MPU (Memory Protection Unit) Register
   *
   * For details, see "Cortex-M3 Technical Reference Manual":
   * <http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3/index.html>
   */
  struct MPU
  {
    using TYPE     = Register< uint32_t, 0xE000ED90, Access::ro, 0x00000800 >;  /**< MPU Type Register                        */
    using CTRL     = Register< uint32_t, 0xE000ED94, Access::rw             >;  /**< MPU Control Register                     */
    using RNR      = Register< uint32_t, 0xE000ED98, Access::rw             >;  /**< MPU Region Number Register               */
    using RBAR     = Register< uint32_t, 0xE000ED9C, Access::rw             >;  /**< MPU Region Base Address Register         */
    using RASR     = Register< uint32_t, 0xE000EDA0, Access::rw             >;  /**< MPU Region Attribute and Size Register   */
    using RBAR_A1  = Register< uint32_t, 0xE000EDA4, Access::rw             >;  /**< MPU alias registers                      */
    using RASR_A1  = Register< uint32_t, 0xE000EDA8, Access::rw             >;  /**<                                          */
    using RBAR_A2  = Register< uint32_t, 0xE000EDAC, Access::rw             >;  /**<                                          */
    using RASR_A2  = Register< uint32_t, 0xE000EDB0, Access::rw             >;  /**<                                          */
    using RBAR_A3  = Register< uint32_t, 0xE000EDB4, Access::rw             >;  /**<                                          */
    using RASR_A3  = Register< uint32_t, 0xE000EDB8, Access::rw             >;  /**<                                          */
  };
}

#endif // COMMON_ARM_CORTEX_REG_MPU_HPP_INCLUDED
