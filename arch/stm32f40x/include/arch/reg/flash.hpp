/*
 * Derived from CMSIS-SVD: STM32F40x, version 1.5
 * <https://cmsis.arm.com/vendor/stmicroelectronics/>
 *
 * You should have received a copy of the "STMicroelectronics EULA:
 * version 1.0" along with this program.  If not, see
 * <https://cmsis.arm.com/vendor/stmicroelectronics/>.
 */

/*
 * Adaptions:
 * - Axel Burri <axel@tty0.ch>
 */

#ifndef REG_FLASH_HPP_INCLUDED
#define REG_FLASH_HPP_INCLUDED

#include <register.hpp>

namespace reg {

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
    : public Register< uint32_t, reg_base + 0x0, Access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x0, Access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "ACR";

      typedef RegisterBits< reg_type,  0,  3 > LATENCY;   /**< Latency                   */
      typedef RegisterBits< reg_type,  8,  1 > PRFTEN;    /**< Prefetch enable           */
      typedef RegisterBits< reg_type,  9,  1 > ICEN;      /**< Instruction cache enable  */
      typedef RegisterBits< reg_type, 10,  1 > DCEN;      /**< Data cache enable         */
      typedef RegisterBits< reg_type, 11,  1 > ICRST;     /**< Instruction cache reset   */
      typedef RegisterBits< reg_type, 12,  1 > DCRST;     /**< Data cache reset          */
    };

    /**
     * Flash key register
     */
    struct KEYR
    : public Register< uint32_t, reg_base + 0x4, Access::wo, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x4, Access::wo, 0x00000000 > reg_type;
      static constexpr const char * name_str = "KEYR";
    };

    /**
     * Flash option key register
     */
    struct OPTKEYR
    : public Register< uint32_t, reg_base + 0x8, Access::wo, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x8, Access::wo, 0x00000000 > reg_type;
      static constexpr const char * name_str = "OPTKEYR";
    };

    /**
     * Status register
     */
    struct SR
    : public Register< uint32_t, reg_base + 0xC, Access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0xC, Access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "SR";

      typedef RegisterBits< reg_type,  0,  1 > EOP;      /**< End of operation               */
      typedef RegisterBits< reg_type,  1,  1 > OPERR;    /**< Operation error                */
      typedef RegisterBits< reg_type,  4,  1 > WRPERR;   /**< Write protection error         */
      typedef RegisterBits< reg_type,  5,  1 > PGAERR;   /**< Programming alignment error    */
      typedef RegisterBits< reg_type,  6,  1 > PGPERR;   /**< Programming parallelism error  */
      typedef RegisterBits< reg_type,  7,  1 > PGSERR;   /**< Programming sequence error     */
      typedef RegisterBits< reg_type, 16,  1 > BSY;      /**< Busy                           */
    };

    /**
     * Control register
     */
    struct CR
    : public Register< uint32_t, reg_base + 0x10, Access::rw, 0x80000000 >
    {
      typedef Register< uint32_t, reg_base + 0x10, Access::rw, 0x80000000 > reg_type;
      static constexpr const char * name_str = "CR";

      typedef RegisterBits< reg_type,  0,  1 > PG;      /**< Programming                        */
      typedef RegisterBits< reg_type,  1,  1 > SER;     /**< Sector Erase                       */
      typedef RegisterBits< reg_type,  2,  1 > MER;     /**< Mass Erase                         */
      typedef RegisterBits< reg_type,  3,  4 > SNB;     /**< Sector number                      */
      typedef RegisterBits< reg_type,  8,  2 > PSIZE;   /**< Program size                       */
      typedef RegisterBits< reg_type, 16,  1 > STRT;    /**< Start                              */
      typedef RegisterBits< reg_type, 24,  1 > EOPIE;   /**< End of operation interrupt enable  */
      typedef RegisterBits< reg_type, 25,  1 > ERRIE;   /**< Error interrupt enable             */
      typedef RegisterBits< reg_type, 31,  1 > LOCK;    /**< Lock                               */
    };

    /**
     * Flash option control register
     */
    struct OPTCR
    : public Register< uint32_t, reg_base + 0x14, Access::rw, 0x00000014 >
    {
      typedef Register< uint32_t, reg_base + 0x14, Access::rw, 0x00000014 > reg_type;
      static constexpr const char * name_str = "OPTCR";

      typedef RegisterBits< reg_type,  0,  1 > OPTLOCK;      /**< Option lock                   */
      typedef RegisterBits< reg_type,  1,  1 > OPTSTRT;      /**< Option start                  */
      typedef RegisterBits< reg_type,  2,  2 > BOR_LEV;      /**< BOR reset Level               */
      typedef RegisterBits< reg_type,  5,  1 > WDG_SW;       /**< WDG_SW User option bytes      */
      typedef RegisterBits< reg_type,  6,  1 > nRST_STOP;    /**< nRST_STOP User option bytes   */
      typedef RegisterBits< reg_type,  7,  1 > nRST_STDBY;   /**< nRST_STDBY User option bytes  */
      typedef RegisterBits< reg_type,  8,  8 > RDP;          /**< Read protect                  */
      typedef RegisterBits< reg_type, 16, 12 > nWRP;         /**< Not write protect             */
    };
  };
}

#endif // REG_FLASH_HPP_INCLUDED
