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

#ifndef ARCH_REG_FLASH_HPP_INCLUDED
#define ARCH_REG_FLASH_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /**
   * FLASH
   */
  struct FLASH
  {
    static constexpr reg_addr_t base_addr = 0x40023C00;

    /**
     * Flash access control register
     */
    struct ACR
    : public Register< uint32_t, base_addr + 0x00, Access::rw, 0x00000000 >
    {
      using DCRST    = RegisterBits< type, 12,  1 >;  /**< Data cache reset          */
      using ICRST    = RegisterBits< type, 11,  1 >;  /**< Instruction cache reset   */
      using DCEN     = RegisterBits< type, 10,  1 >;  /**< Data cache enable         */
      using ICEN     = RegisterBits< type,  9,  1 >;  /**< Instruction cache enable  */
      using PRFTEN   = RegisterBits< type,  8,  1 >;  /**< Prefetch enable           */
      using LATENCY  = RegisterBits< type,  0,  3 >;  /**< Latency                   */
    };

    /**
     * Flash key register
     */
    using KEYR     = Register< uint32_t, base_addr + 0x04, Access::wo, 0x00000000 >;

    /**
     * Flash option key register
     */
    using OPTKEYR  = Register< uint32_t, base_addr + 0x08, Access::wo, 0x00000000 >;

    /**
     * Status register
     */
    struct SR
    : public Register< uint32_t, base_addr + 0x0C, Access::rw, 0x00000000 >
    {
      using BSY     = RegisterBits< type, 16,  1 >;  /**< Busy                           */
      using PGSERR  = RegisterBits< type,  7,  1 >;  /**< Programming sequence error     */
      using PGPERR  = RegisterBits< type,  6,  1 >;  /**< Programming parallelism error  */
      using PGAERR  = RegisterBits< type,  5,  1 >;  /**< Programming alignment error    */
      using WRPERR  = RegisterBits< type,  4,  1 >;  /**< Write protection error         */
      using OPERR   = RegisterBits< type,  1,  1 >;  /**< Operation error                */
      using EOP     = RegisterBits< type,  0,  1 >;  /**< End of operation               */
    };

    /**
     * Control register
     */
    struct CR
    : public Register< uint32_t, base_addr + 0x10, Access::rw, 0x80000000 >
    {
      using LOCK   = RegisterBits< type, 31,  1 >;  /**< Lock                               */
      using ERRIE  = RegisterBits< type, 25,  1 >;  /**< Error interrupt enable             */
      using EOPIE  = RegisterBits< type, 24,  1 >;  /**< End of operation interrupt enable  */
      using STRT   = RegisterBits< type, 16,  1 >;  /**< Start                              */
      using PSIZE  = RegisterBits< type,  8,  2 >;  /**< Program size                       */
      using SNB    = RegisterBits< type,  3,  4 >;  /**< Sector number                      */
      using MER    = RegisterBits< type,  2,  1 >;  /**< Mass erase                         */
      using SER    = RegisterBits< type,  1,  1 >;  /**< Sector erase                       */
      using PG     = RegisterBits< type,  0,  1 >;  /**< Programming                        */
    };

    /**
     * Flash option control register
     */
    struct OPTCR
    : public Register< uint32_t, base_addr + 0x14, Access::rw, 0x00000014 >
    {
      /** User option bytes  */
      template<typename Rb>
      struct __USER
      : public Rb
      {
        using WDG_SW      = RegisterConst< Rb, 0x1 >;
        using NRST_STOP   = RegisterConst< Rb, 0x2 >; 
        using NRST_STDBY  = RegisterConst< Rb, 0x4 >;
      };

      using NWRP     =          RegisterBits< type, 16, 12 >;    /**< Not write protect  */
      using RDP      =          RegisterBits< type,  8,  8 >;    /**< Read protect       */
      using USER     = __USER < RegisterBits< type,  5,  3 > >;  /**< User option bytes  */
      using BOR_LEV  =          RegisterBits< type,  2,  2 >;    /**< BOR reset Level    */
      using OPTSTRT  =          RegisterBits< type,  1,  1 >;    /**< Option start       */
      using OPTLOCK  =          RegisterBits< type,  0,  1 >;    /**< Option lock        */
    };
  };
}

#endif // ARCH_REG_FLASH_HPP_INCLUDED
