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
   * FLASH
   */
  struct FLASH
  {
    static constexpr reg_addr_t reg_base = 0x40023C00;
    static constexpr const char * name_str = "FLASH";

    /**
     * Flash access control register
     */
    struct ACR
    : public Register< uint32_t, reg_base + 0x00, Access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x00, Access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "ACR";

      typedef RegisterBits< reg_type, 12,  1 > DCRST;     /**< Data cache reset          */
      typedef RegisterBits< reg_type, 11,  1 > ICRST;     /**< Instruction cache reset   */
      typedef RegisterBits< reg_type, 10,  1 > DCEN;      /**< Data cache enable         */
      typedef RegisterBits< reg_type,  9,  1 > ICEN;      /**< Instruction cache enable  */
      typedef RegisterBits< reg_type,  8,  1 > PRFTEN;    /**< Prefetch enable           */
      typedef RegisterBits< reg_type,  0,  3 > LATENCY;   /**< Latency                   */
    };

    /**
     * Flash key register
     */
    struct KEYR
    : public Register< uint32_t, reg_base + 0x04, Access::wo, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x04, Access::wo, 0x00000000 > reg_type;
      static constexpr const char * name_str = "KEYR";
    };

    /**
     * Flash option key register
     */
    struct OPTKEYR
    : public Register< uint32_t, reg_base + 0x08, Access::wo, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x08, Access::wo, 0x00000000 > reg_type;
      static constexpr const char * name_str = "OPTKEYR";
    };

    /**
     * Status register
     */
    struct SR
    : public Register< uint32_t, reg_base + 0x0C, Access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x0C, Access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "SR";

      typedef RegisterBits< reg_type, 16,  1 > BSY;      /**< Busy                           */
      typedef RegisterBits< reg_type,  7,  1 > PGSERR;   /**< Programming sequence error     */
      typedef RegisterBits< reg_type,  6,  1 > PGPERR;   /**< Programming parallelism error  */
      typedef RegisterBits< reg_type,  5,  1 > PGAERR;   /**< Programming alignment error    */
      typedef RegisterBits< reg_type,  4,  1 > WRPERR;   /**< Write protection error         */
      typedef RegisterBits< reg_type,  1,  1 > OPERR;    /**< Operation error                */
      typedef RegisterBits< reg_type,  0,  1 > EOP;      /**< End of operation               */
    };

    /**
     * Control register
     */
    struct CR
    : public Register< uint32_t, reg_base + 0x10, Access::rw, 0x80000000 >
    {
      typedef Register< uint32_t, reg_base + 0x10, Access::rw, 0x80000000 > reg_type;
      static constexpr const char * name_str = "CR";

      typedef RegisterBits< reg_type, 31,  1 > LOCK;    /**< Lock                               */
      typedef RegisterBits< reg_type, 25,  1 > ERRIE;   /**< Error interrupt enable             */
      typedef RegisterBits< reg_type, 24,  1 > EOPIE;   /**< End of operation interrupt enable  */
      typedef RegisterBits< reg_type, 16,  1 > STRT;    /**< Start                              */
      typedef RegisterBits< reg_type,  8,  2 > PSIZE;   /**< Program size                       */
      typedef RegisterBits< reg_type,  3,  4 > SNB;     /**< Sector number                      */
      typedef RegisterBits< reg_type,  2,  1 > MER;     /**< Mass erase                         */
      typedef RegisterBits< reg_type,  1,  1 > SER;     /**< Sector erase                       */
      typedef RegisterBits< reg_type,  0,  1 > PG;      /**< Programming                        */
    };

    /**
     * Flash option control register
     */
    struct OPTCR
    : public Register< uint32_t, reg_base + 0x14, Access::rw, 0x00000014 >
    {
      typedef Register< uint32_t, reg_base + 0x14, Access::rw, 0x00000014 > reg_type;
      static constexpr const char * name_str = "OPTCR";

      /** User option bytes  */
      template<typename Rb>
      struct __USER
      : public Rb
      {
        typedef RegisterConst< Rb, 0x1 > WDG_SW;
        typedef RegisterConst< Rb, 0x2 > NRST_STOP;
        typedef RegisterConst< Rb, 0x4 > NRST_STDBY;
      };

      typedef          RegisterBits< reg_type, 16, 12 >   NWRP;     /**< Not write protect  */
      typedef          RegisterBits< reg_type,  8,  8 >   RDP;      /**< Read protect       */
      typedef __USER < RegisterBits< reg_type,  5,  3 > > USER;     /**< User option bytes  */
      typedef          RegisterBits< reg_type,  2,  2 >   BOR_LEV;  /**< BOR reset Level    */
      typedef          RegisterBits< reg_type,  1,  1 >   OPTSTRT;  /**< Option start       */
      typedef          RegisterBits< reg_type,  0,  1 >   OPTLOCK;  /**< Option lock        */
    };
  };
}

#endif // REG_FLASH_HPP_INCLUDED
