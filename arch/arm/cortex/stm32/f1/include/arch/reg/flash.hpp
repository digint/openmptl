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

#ifndef REG_FLASH_HPP_INCLUDED
#define REG_FLASH_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /**
   * Embedded Flash memory (FLASH)
   */
  class FLASH
  {
    static constexpr reg_addr_t reg_base = 0x40022000;

    /**
     * Flash access control register
     */
    template<typename R>
    struct __ACR
    : public R
    {
      typedef RegisterBits< R,  0,  2 > LATENCY;       /**< [ 1: 0] Latency                          */
      typedef RegisterBits< R,  3,  1 > HLFCYA;        /**< [ 3: 3] Flash Half Cycle Access Enable   */
      typedef RegisterBits< R,  4,  1 > PRFTBE;        /**< [ 4: 4] Prefetch Buffer Enable           */
      typedef RegisterBits< R,  5,  1 > PRFTBS;        /**< [ 5: 5] Prefetch Buffer Status           */
    };

    /**
     * FPEC Key
     */
    template<typename R>
    struct __KEYR
    : public R
    {
    };

    /**
     * Option Byte Key
     */
    template<typename R>
    struct __OPTKEYR
    : public R
    {
    };

    /**
     * document me!
     */
    template<typename R>
    struct __SR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > BSY;           /**< [ 0: 0] Busy                     */
      typedef RegisterBits< R,  2,  1 > PGERR;         /**< [ 2: 2] Programming Error        */
      typedef RegisterBits< R,  4,  1 > WRPRTERR;      /**< [ 4: 4] Write Protection Error   */
      typedef RegisterBits< R,  5,  1 > EOP;           /**< [ 5: 5] End of operation         */
    };


    /**
     * document me!
     */
    template<typename R>
    struct __CR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > PG;            /**< [ 0: 0] Programming                         */
      typedef RegisterBits< R,  1,  1 > PER;           /**< [ 1: 1] Page Erase                          */
      typedef RegisterBits< R,  2,  1 > MER;           /**< [ 2: 2] Mass Erase                          */
      typedef RegisterBits< R,  4,  1 > OPTPG;         /**< [ 4: 4] Option Byte Programming             */
      typedef RegisterBits< R,  5,  1 > OPTER;         /**< [ 5: 5] Option Byte Erase                   */
      typedef RegisterBits< R,  6,  1 > STRT;          /**< [ 6: 6] Start                               */
      typedef RegisterBits< R,  7,  1 > LOCK;          /**< [ 7: 7] Lock                                */
      typedef RegisterBits< R,  9,  1 > OPTWRE;        /**< [ 9: 9] Option Bytes Write Enable           */
      typedef RegisterBits< R, 10,  1 > ERRIE;         /**< [10:10] Error Interrupt Enable              */
      typedef RegisterBits< R, 12,  1 > EOPIE;         /**< [12:12] End of operation interrupt enable   */
    };

    /**
     * Flash Address
     */
    template<typename R>
    struct __AR
    : public R
    {
    };


    /* reserved: 0x4002 2018 - 0x4002 201B */


    /**
     * Option Byte Register
     */
    template<typename R>
    struct __OBR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > OPTERR;        /**< [ 0: 0] Option Byte Error   */
      typedef RegisterBits< R,  1,  1 > RDPRT;         /**< [ 1: 1] Read protection     */
      typedef RegisterBits< R,  2,  8 > USER;          /**< [ 9: 2] User Option Bytes   */
      typedef RegisterBits< R,  2,  1 > WDG_SW;        /**< [ 2: 2] WDG_SW              */
      typedef RegisterBits< R,  3,  1 > nRST_STOP;     /**< [ 3: 3] nRST_STOP           */
      typedef RegisterBits< R,  4,  1 > nRST_STDBY;    /**< [ 4: 4] nRST_STDBY          */
      typedef RegisterBits< R,  5,  1 > BFB2;          /**< [ 5: 5] BFB2                */
    };

    /**
     * Write Protect
     */
    template<typename R>
    struct __WRPR
    : public R
    {
    };


    /* reserved: 0x4002 2024 - 0x4002 2043 */


    /**
     * document me!
     *
     * NOTE: only available for XL-density Flash modules
     */
    template<typename R>
    struct __KEYR2
    : public R
    {
    };


    /* reserved: 0x4002 2048 - 0x4002 204B */


    /**
     * document me!
     *
     * NOTE: only available for XL-density Flash modules
     */
    template<typename R>
    struct __SR2
    : public R
    {
    };

    /**
     * document me!
     *
     * NOTE: only available for XL-density Flash modules
     */
    template<typename R>
    struct __CR2
    : public R
    {
    };

    /**
     * document me!
     *
     * NOTE: only available for XL-density Flash modules
     */
    template<typename R>
    struct __AR2
    : public R
    {
    };


  public:

    typedef __ACR    < Register< uint32_t, reg_base + 0x00, Access::rw, 0x00000030 > > ACR;
    typedef __KEYR   < Register< uint32_t, reg_base + 0x04, Access::rw > > KEYR;
    typedef __OPTKEYR< Register< uint32_t, reg_base + 0x08, Access::rw > > OPTKEYR;
    typedef __SR     < Register< uint32_t, reg_base + 0x0c, Access::rw > > SR;
    typedef __CR     < Register< uint32_t, reg_base + 0x10, Access::rw > > CR;
    typedef __AR     < Register< uint32_t, reg_base + 0x14, Access::rw > > AR;
    typedef __OBR    < Register< uint32_t, reg_base + 0x1c, Access::rw > > OBR;
    typedef __WRPR   < Register< uint32_t, reg_base + 0x20, Access::rw > > WRPR;
    typedef __KEYR2  < Register< uint32_t, reg_base + 0x44, Access::rw > > KEYR2;
    typedef __SR2    < Register< uint32_t, reg_base + 0x4c, Access::rw > > SR2;
    typedef __CR2    < Register< uint32_t, reg_base + 0x50, Access::rw > > CR2;
    typedef __AR2    < Register< uint32_t, reg_base + 0x54, Access::rw > > AR2;
  };
}

#endif // REG_FLASH_HPP_INCLUDED
