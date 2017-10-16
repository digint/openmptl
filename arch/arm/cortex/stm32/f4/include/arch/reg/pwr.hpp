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

/*
 * This program contains derivative representations of CMSIS System
 * View Description (SVD) files, and is subject to the "End User
 * License Agreement for STMicroelectronics" (see "STM_License.html"
 * in the containing directory).
 */

#ifndef ARCH_REG_PWR_HPP_INCLUDED
#define ARCH_REG_PWR_HPP_INCLUDED

#include <register.hpp>

namespace mptl {
/**
 * Power control
 */
struct PWR
{
  static constexpr reg_addr_t base_addr = 0x40007000;

  /**
   * Power control register
   */
  struct CR
  : public reg< uint32_t, base_addr + 0x0, rw, 0x00004000 >
  {
    using VOS   = regbits< type, 14,  1 >;  /**< Regulator voltage scaling output selection */
    using FPDS  = regbits< type,  9,  1 >;  /**< Flash power down in Stop mode              */
    using DBP   = regbits< type,  8,  1 >;  /**< Disable backup domain write protection     */
    using PLS   = regbits< type,  5,  3 >;  /**< PVD level selection                        */
    using PVDE  = regbits< type,  4,  1 >;  /**< Power voltage detector enable              */
    using CSBF  = regbits< type,  3,  1 >;  /**< Clear standby flag                         */
    using CWUF  = regbits< type,  2,  1 >;  /**< Clear wakeup flag                          */
    using PDDS  = regbits< type,  1,  1 >;  /**< Power down deepsleep                       */
    using LPDS  = regbits< type,  0,  1 >;  /**< Low-power deep sleep                       */
  };

  /**
   * Power control/status register
   */
  struct CSR
  : public reg< uint32_t, base_addr + 0x4, rw, 0x00000000 >
  {
    using VOSRDY  = regbits< type, 14,  1 >;  /**< Regulator voltage scaling output selection ready bit  */
    using BRE     = regbits< type,  9,  1 >;  /**< Backup regulator enable                               */
    using EWUP    = regbits< type,  8,  1 >;  /**< Enable WKUP pin                                       */
    using BRR     = regbits< type,  3,  1 >;  /**< Backup regulator ready                                */
    using PVDO    = regbits< type,  2,  1 >;  /**< PVD output                                            */
    using SBF     = regbits< type,  1,  1 >;  /**< Standby flag                                          */
    using WUF     = regbits< type,  0,  1 >;  /**< Wakeup flag                                           */
  };
};

} // namespace mptl

#endif // ARCH_REG_PWR_HPP_INCLUDED
