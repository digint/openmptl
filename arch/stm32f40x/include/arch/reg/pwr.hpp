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

#ifndef REG_PWR_HPP_INCLUDED
#define REG_PWR_HPP_INCLUDED

#include <register.hpp>

namespace reg {

  /**
   * Power control
   */
  struct PWR
  {
    static constexpr reg_addr_t reg_base = 0x40007000;
    static constexpr const char * name_str = "PWR";

    /**
     * power control register
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
     * power control/status register
     */
    struct CSR
    : public Register< uint32_t, reg_base + 0x4, Access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x4, Access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "CSR";

      typedef RegisterBits< reg_type,  0,  1 > WUF;      /**< Wakeup flag                                           */
      typedef RegisterBits< reg_type,  1,  1 > SBF;      /**< Standby flag                                          */
      typedef RegisterBits< reg_type,  2,  1 > PVDO;     /**< PVD output                                            */
      typedef RegisterBits< reg_type,  3,  1 > BRR;      /**< Backup regulator ready                                */
      typedef RegisterBits< reg_type,  8,  1 > EWUP;     /**< Enable WKUP pin                                       */
      typedef RegisterBits< reg_type,  9,  1 > BRE;      /**< Backup regulator enable                               */
      typedef RegisterBits< reg_type, 14,  1 > VOSRDY;   /**< Regulator voltage scaling output selection ready bit  */
    };
  };
}

#endif // REG_PWR_HPP_INCLUDED
