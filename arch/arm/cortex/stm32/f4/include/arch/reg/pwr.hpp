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

#ifndef ARCH_REG_PWR_HPP_INCLUDED
#define ARCH_REG_PWR_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
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
    : public Register< uint32_t, base_addr + 0x0, Access::rw, 0x00004000 >
    {
      using VOS   = RegisterBits< type, 14,  1 >;  /**< Regulator voltage scaling output selection */
      using FPDS  = RegisterBits< type,  9,  1 >;  /**< Flash power down in Stop mode              */
      using DBP   = RegisterBits< type,  8,  1 >;  /**< Disable backup domain write protection     */
      using PLS   = RegisterBits< type,  5,  3 >;  /**< PVD level selection                        */
      using PVDE  = RegisterBits< type,  4,  1 >;  /**< Power voltage detector enable              */
      using CSBF  = RegisterBits< type,  3,  1 >;  /**< Clear standby flag                         */
      using CWUF  = RegisterBits< type,  2,  1 >;  /**< Clear wakeup flag                          */
      using PDDS  = RegisterBits< type,  1,  1 >;  /**< Power down deepsleep                       */
      using LPDS  = RegisterBits< type,  0,  1 >;  /**< Low-power deep sleep                       */
    };

    /**
     * Power control/status register
     */
    struct CSR
    : public Register< uint32_t, base_addr + 0x4, Access::rw, 0x00000000 >
    {
      using VOSRDY  = RegisterBits< type, 14,  1 >;  /**< Regulator voltage scaling output selection ready bit  */
      using BRE     = RegisterBits< type,  9,  1 >;  /**< Backup regulator enable                               */
      using EWUP    = RegisterBits< type,  8,  1 >;  /**< Enable WKUP pin                                       */
      using BRR     = RegisterBits< type,  3,  1 >;  /**< Backup regulator ready                                */
      using PVDO    = RegisterBits< type,  2,  1 >;  /**< PVD output                                            */
      using SBF     = RegisterBits< type,  1,  1 >;  /**< Standby flag                                          */
      using WUF     = RegisterBits< type,  0,  1 >;  /**< Wakeup flag                                           */
    };
  };
}

#endif // ARCH_REG_PWR_HPP_INCLUDED
