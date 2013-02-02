/*
 * CppCore - C++ microprocessor core library
 *
 * Copyright 2012 Axel Burri <axel@tty0.ch>
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
    static constexpr reg_addr_t reg_base = 0x40007000;
    static constexpr const char * name_str = "PWR";

    /**
     * Power control register
     */
    struct CR
    : public Register< uint32_t, reg_base + 0x0, Access::rw, 0x00004000 >
    {
      typedef Register< uint32_t, reg_base + 0x0, Access::rw, 0x00004000 > reg_type;
      static constexpr const char * name_str = "CR";

      typedef RegisterBits< reg_type, 14,  1 > VOS;    /**< Regulator voltage scaling output selection */
      typedef RegisterBits< reg_type,  9,  1 > FPDS;   /**< Flash power down in Stop mode              */
      typedef RegisterBits< reg_type,  8,  1 > DBP;    /**< Disable backup domain write protection     */
      typedef RegisterBits< reg_type,  5,  3 > PLS;    /**< PVD level selection                        */
      typedef RegisterBits< reg_type,  4,  1 > PVDE;   /**< Power voltage detector enable              */
      typedef RegisterBits< reg_type,  3,  1 > CSBF;   /**< Clear standby flag                         */
      typedef RegisterBits< reg_type,  2,  1 > CWUF;   /**< Clear wakeup flag                          */
      typedef RegisterBits< reg_type,  1,  1 > PDDS;   /**< Power down deepsleep                       */
      typedef RegisterBits< reg_type,  0,  1 > LPDS;   /**< Low-power deep sleep                       */
    };

    /**
     * Power control/status register
     */
    struct CSR
    : public Register< uint32_t, reg_base + 0x4, Access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x4, Access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "CSR";

      typedef RegisterBits< reg_type, 14,  1 > VOSRDY;   /**< Regulator voltage scaling output selection ready bit  */
      typedef RegisterBits< reg_type,  9,  1 > BRE;      /**< Backup regulator enable                               */
      typedef RegisterBits< reg_type,  8,  1 > EWUP;     /**< Enable WKUP pin                                       */
      typedef RegisterBits< reg_type,  3,  1 > BRR;      /**< Backup regulator ready                                */
      typedef RegisterBits< reg_type,  2,  1 > PVDO;     /**< PVD output                                            */
      typedef RegisterBits< reg_type,  1,  1 > SBF;      /**< Standby flag                                          */
      typedef RegisterBits< reg_type,  0,  1 > WUF;      /**< Wakeup flag                                           */
    };
  };
}

#endif // REG_PWR_HPP_INCLUDED
