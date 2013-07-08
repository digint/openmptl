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

namespace mptl { namespace reg {

/**
 *  MPU (Memory Protection Unit) Register
 *
 * For details, see "Cortex-M3 Technical Reference Manual":
 * <http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3/index.html>
 */
struct MPU
{
  using TYPE     = regdef< uint32_t, 0xE000ED90, reg_access::ro, 0x00000800 >;  /**< MPU Type Register                        */
  using CTRL     = regdef< uint32_t, 0xE000ED94, reg_access::rw             >;  /**< MPU Control Register                     */
  using RNR      = regdef< uint32_t, 0xE000ED98, reg_access::rw             >;  /**< MPU Region Number Register               */
  using RBAR     = regdef< uint32_t, 0xE000ED9C, reg_access::rw             >;  /**< MPU Region Base Address Register         */
  using RASR     = regdef< uint32_t, 0xE000EDA0, reg_access::rw             >;  /**< MPU Region Attribute and Size Register   */
  using RBAR_A1  = regdef< uint32_t, 0xE000EDA4, reg_access::rw             >;  /**< MPU alias registers                      */
  using RASR_A1  = regdef< uint32_t, 0xE000EDA8, reg_access::rw             >;  /**<                                          */
  using RBAR_A2  = regdef< uint32_t, 0xE000EDAC, reg_access::rw             >;  /**<                                          */
  using RASR_A2  = regdef< uint32_t, 0xE000EDB0, reg_access::rw             >;  /**<                                          */
  using RBAR_A3  = regdef< uint32_t, 0xE000EDB4, reg_access::rw             >;  /**<                                          */
  using RASR_A3  = regdef< uint32_t, 0xE000EDB8, reg_access::rw             >;  /**<                                          */
};

} } // namespace mptl::reg

#endif // COMMON_ARM_CORTEX_REG_MPU_HPP_INCLUDED
