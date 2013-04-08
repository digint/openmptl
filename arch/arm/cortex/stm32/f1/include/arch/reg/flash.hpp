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

#ifndef REG_FLASH_HPP_INCLUDED
#define REG_FLASH_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /**
   * Embedded Flash memory (FLASH)
   */

  /**
   * FLASH
   */
  struct FLASH
  {
    static constexpr reg_addr_t base_addr = 0x40022000;

    /**
     * Flash access control register
     */
    struct ACR
    : public Register< uint32_t, base_addr + 0x0, Access::rw, 0x00000030 >
    {
      using type = Register< uint32_t, base_addr + 0x0, Access::rw, 0x00000030 >;

      using LATENCY  = RegisterBits< type,  0,  3 >;  /**< Latency                         */
      using HLFCYA   = RegisterBits< type,  3,  1 >;  /**< Flash half cycle access enable  */
      using PRFTBE   = RegisterBits< type,  4,  1 >;  /**< Prefetch buffer enable          */
      using PRFTBS   = RegisterBits< type,  5,  1 >;  /**< Prefetch buffer status          */
    };

    /**
     * Flash key register
     */
    struct KEYR
    : public Register< uint32_t, base_addr + 0x4, Access::wo, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x4, Access::wo, 0x00000000 >;

      using KEY  = RegisterBits< type,  0, 32 >;  /**< FPEC key  */
    };

    /**
     * Flash option key register
     */
    struct OPTKEYR
    : public Register< uint32_t, base_addr + 0x8, Access::wo, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x8, Access::wo, 0x00000000 >;

      using OPTKEY  = RegisterBits< type,  0, 32 >;  /**< Option byte key  */
    };

    /**
     * Status register
     */
    struct SR
    : public Register< uint32_t, base_addr + 0xc, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0xc, Access::rw, 0x00000000 >;

      using EOP       = RegisterBits< type,  5,  1 >;  /**< End of operation        */
      using WRPRTERR  = RegisterBits< type,  4,  1 >;  /**< Write protection error  */
      using PGERR     = RegisterBits< type,  2,  1 >;  /**< Programming error       */
      using BSY       = RegisterBits< type,  0,  1 >;  /**< Busy                    */
    };

    /**
     * Control register
     */
    struct CR
    : public Register< uint32_t, base_addr + 0x10, Access::rw, 0x00000080 >
    {
      using type = Register< uint32_t, base_addr + 0x10, Access::rw, 0x00000080 >;

      using PG      = RegisterBits< type,  0,  1 >;  /**< Programming                        */
      using PER     = RegisterBits< type,  1,  1 >;  /**< Page Erase                         */
      using MER     = RegisterBits< type,  2,  1 >;  /**< Mass Erase                         */
      using OPTPG   = RegisterBits< type,  4,  1 >;  /**< Option byte programming            */
      using OPTER   = RegisterBits< type,  5,  1 >;  /**< Option byte erase                  */
      using STRT    = RegisterBits< type,  6,  1 >;  /**< Start                              */
      using LOCK    = RegisterBits< type,  7,  1 >;  /**< Lock                               */
      using OPTWRE  = RegisterBits< type,  9,  1 >;  /**< Option bytes write enable          */
      using ERRIE   = RegisterBits< type, 10,  1 >;  /**< Error interrupt enable             */
      using EOPIE   = RegisterBits< type, 12,  1 >;  /**< End of operation interrupt enable  */
    };

    /**
     * Flash address register
     */
    struct AR
    : public Register< uint32_t, base_addr + 0x14, Access::wo, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x14, Access::wo, 0x00000000 >;

      using FAR  = RegisterBits< type,  0, 32 >;  /**< Flash Address  */
    };

    /**
     * Option byte register
     */
    struct OBR
    : public Register< uint32_t, base_addr + 0x1c, Access::ro, 0x03FFFFFC >
    {
      using type = Register< uint32_t, base_addr + 0x1c, Access::ro, 0x03FFFFFC >;

      using OPTERR      = RegisterBits< type,  0,  1 >;  /**< Option byte error  */
      using RDPRT       = RegisterBits< type,  1,  1 >;  /**< Read protection    */
      using WDG_SW      = RegisterBits< type,  2,  1 >;  /**< WDG_SW             */
      using nRST_STOP   = RegisterBits< type,  3,  1 >;  /**< nRST_STOP          */
      using nRST_STDBY  = RegisterBits< type,  4,  1 >;  /**< nRST_STDBY         */
      using Data0       = RegisterBits< type, 10,  8 >;  /**< Data0              */
      using Data1       = RegisterBits< type, 18,  8 >;  /**< Data1              */
    };

    /**
     * Write protection register
     */
    struct WRPR
    : public Register< uint32_t, base_addr + 0x20, Access::ro, 0xFFFFFFFF >
    {
      using type = Register< uint32_t, base_addr + 0x20, Access::ro, 0xFFFFFFFF >;

      using WRP  = RegisterBits< type,  0, 32 >;  /**< Write protect  */
    };


#if 0
    /**
     * document me!
     *
     * NOTE: only available for XL-density Flash modules
     */
    struct KEYR2
    : public Register< uint32_t, base_addr + 0x44, Access::rw >
    { };

    /**
     * document me!
     *
     * NOTE: only available for XL-density Flash modules
     */
    struct SR2
    : public Register< uint32_t, base_addr + 0x4c, Access::rw >
    { };

    /**
     * document me!
     *
     * NOTE: only available for XL-density Flash modules
     */
    struct CR2
    : public Register< uint32_t, base_addr + 0x50, Access::rw >
    { };

    /**
     * document me!
     *
     * NOTE: only available for XL-density Flash modules
     */
    struct AR2
    : public Register< uint32_t, base_addr + 0x54, Access::rw >
    { };
#endif
  };
}

#endif // REG_FLASH_HPP_INCLUDED
