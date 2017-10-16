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

#ifndef ARCH_REG_FLASH_HPP_INCLUDED
#define ARCH_REG_FLASH_HPP_INCLUDED

#include <register.hpp>

namespace mptl {

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
  : public reg< uint32_t, base_addr + 0x00, rw, 0x00000000 >
  {
    using DCRST    = regbits< type, 12,  1 >;  /**< Data cache reset          */
    using ICRST    = regbits< type, 11,  1 >;  /**< Instruction cache reset   */
    using DCEN     = regbits< type, 10,  1 >;  /**< Data cache enable         */
    using ICEN     = regbits< type,  9,  1 >;  /**< Instruction cache enable  */
    using PRFTEN   = regbits< type,  8,  1 >;  /**< Prefetch enable           */
    using LATENCY  = regbits< type,  0,  3 >;  /**< Latency                   */
  };

  /**
   * Flash key register
   */
  using KEYR     = reg< uint32_t, base_addr + 0x04, wo, 0x00000000 >;

  /**
   * Flash option key register
   */
  using OPTKEYR  = reg< uint32_t, base_addr + 0x08, wo, 0x00000000 >;

  /**
   * Status register
   */
  struct SR
  : public reg< uint32_t, base_addr + 0x0C, rw, 0x00000000 >
  {
    using BSY     = regbits< type, 16,  1 >;  /**< Busy                           */
    using PGSERR  = regbits< type,  7,  1 >;  /**< Programming sequence error     */
    using PGPERR  = regbits< type,  6,  1 >;  /**< Programming parallelism error  */
    using PGAERR  = regbits< type,  5,  1 >;  /**< Programming alignment error    */
    using WRPERR  = regbits< type,  4,  1 >;  /**< Write protection error         */
    using OPERR   = regbits< type,  1,  1 >;  /**< Operation error                */
    using EOP     = regbits< type,  0,  1 >;  /**< End of operation               */
  };

  /**
   * Control register
   */
  struct CR
  : public reg< uint32_t, base_addr + 0x10, rw, 0x80000000 >
  {
    using LOCK   = regbits< type, 31,  1 >;  /**< Lock                               */
    using ERRIE  = regbits< type, 25,  1 >;  /**< Error interrupt enable             */
    using EOPIE  = regbits< type, 24,  1 >;  /**< End of operation interrupt enable  */
    using STRT   = regbits< type, 16,  1 >;  /**< Start                              */
    using PSIZE  = regbits< type,  8,  2 >;  /**< Program size                       */
    using SNB    = regbits< type,  3,  4 >;  /**< Sector number                      */
    using MER    = regbits< type,  2,  1 >;  /**< Mass erase                         */
    using SER    = regbits< type,  1,  1 >;  /**< Sector erase                       */
    using PG     = regbits< type,  0,  1 >;  /**< Programming                        */
  };

  /**
   * Flash option control register
   */
  struct OPTCR
  : public reg< uint32_t, base_addr + 0x14, rw, 0x00000014 >
  {
    /** User option bytes  */
    template<typename Rb>
      struct __USER
      : public Rb
    {
      using WDG_SW      = regval< Rb, 0x1 >;
      using NRST_STOP   = regval< Rb, 0x2 >; 
      using NRST_STDBY  = regval< Rb, 0x4 >;
    };

    using NWRP     =          regbits< type, 16, 12 >;    /**< Not write protect  */
    using RDP      =          regbits< type,  8,  8 >;    /**< Read protect       */
    using USER     = __USER < regbits< type,  5,  3 > >;  /**< User option bytes  */
    using BOR_LEV  =          regbits< type,  2,  2 >;    /**< BOR reset Level    */
    using OPTSTRT  =          regbits< type,  1,  1 >;    /**< Option start       */
    using OPTLOCK  =          regbits< type,  0,  1 >;    /**< Option lock        */
  };
};

} // namespace mptl

#endif // ARCH_REG_FLASH_HPP_INCLUDED
