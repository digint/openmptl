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

#ifndef ARM_CORTEX_COMMON_REG_MPU_HPP_INCLUDED
#define ARM_CORTEX_COMMON_REG_MPU_HPP_INCLUDED

#include <register.hpp>

namespace mptl {

/**
 *  MPU (Memory Protection Unit) Register
 *
 * For details, see "Cortex-M3 Technical Reference Manual":
 * <http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3/index.html>
 */
struct MPU
{
  using TYPE     = reg< uint32_t, 0xE000ED90, ro, 0x00000800 >;  /**< MPU Type Register                        */
  using CTRL     = reg< uint32_t, 0xE000ED94, rw             >;  /**< MPU Control Register                     */
  using RNR      = reg< uint32_t, 0xE000ED98, rw             >;  /**< MPU Region Number Register               */
  using RBAR     = reg< uint32_t, 0xE000ED9C, rw             >;  /**< MPU Region Base Address Register         */
  using RASR     = reg< uint32_t, 0xE000EDA0, rw             >;  /**< MPU Region Attribute and Size Register   */
  using RBAR_A1  = reg< uint32_t, 0xE000EDA4, rw             >;  /**< MPU alias registers                      */
  using RASR_A1  = reg< uint32_t, 0xE000EDA8, rw             >;  /**<                                          */
  using RBAR_A2  = reg< uint32_t, 0xE000EDAC, rw             >;  /**<                                          */
  using RASR_A2  = reg< uint32_t, 0xE000EDB0, rw             >;  /**<                                          */
  using RBAR_A3  = reg< uint32_t, 0xE000EDB4, rw             >;  /**<                                          */
  using RASR_A3  = reg< uint32_t, 0xE000EDB8, rw             >;  /**<                                          */
};

} // namespace mptl

#endif // ARM_CORTEX_COMMON_REG_MPU_HPP_INCLUDED
