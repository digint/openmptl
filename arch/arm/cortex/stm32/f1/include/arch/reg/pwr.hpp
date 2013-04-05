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

#ifndef REG_PWR_HPP_INCLUDED
#define REG_PWR_HPP_INCLUDED

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
     * Power control register (PWR_CR)
     */
    struct CR
    : public Register< uint32_t, base_addr + 0x0, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x0, Access::rw, 0x00000000 >;

      using LPDS  = RegisterBits< type,  0,  1 >;    /**< Low Power Deep Sleep                    */
      using PDDS  = RegisterBits< type,  1,  1 >;    /**< Power Down Deep Sleep                   */
      using CWUF  = RegisterBits< type,  2,  1 >;    /**< Clear Wake-up Flag                      */
      using CSBF  = RegisterBits< type,  3,  1 >;    /**< Clear STANDBY Flag                      */
      using PVDE  = RegisterBits< type,  4,  1 >;    /**< Power Voltage Detector Enable           */

      /** PVD Level Selection */
      struct PLS  : RegisterBits< type,  5,  3 >
      {
        template<unsigned int fraction>
        struct VoltageThreshold
        : public RegisterConst< bits_type, fraction - 2 >
        { static_assert((fraction >= 2) && (fraction <= 9), "invalid fraction"); };
      };

      using DBP   = RegisterBits< type,  8,  1 >;    /**< Disable Backup Domain write protection  */
    };

    /**
     * Power control register (PWR_CR)
     */
    struct CSR
    : public Register< uint32_t, base_addr + 0x4, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x4, Access::rw, 0x00000000 >;

      using WUF   = RegisterBits< type,  0,  1 >;  /**< Wake-Up Flag     */
      using SBF   = RegisterBits< type,  1,  1 >;  /**< STANDBY Flag     */
      using PVDO  = RegisterBits< type,  2,  1 >;  /**< PVD Output       */
      using EWUP  = RegisterBits< type,  8,  1 >;  /**< Enable WKUP pin  */
    };
  };
}

#endif // REG_PWR_HPP_INCLUDED
