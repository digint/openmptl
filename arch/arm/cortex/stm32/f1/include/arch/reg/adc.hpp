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

#ifndef ARCH_REG_ADC_HPP_INCLUDED
#define ARCH_REG_ADC_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /**
   * Analog-to-digital converter (ADC)
   */
  template<unsigned adc_no>
  struct ADC
  {
    static_assert((adc_no >= 1) && (adc_no <= 3), "invalid index for register");

    static constexpr reg_addr_t base_addr =
      adc_no == 1 ? 0x40012400 :
      adc_no == 2 ? 0x40012800 :
      adc_no == 3 ? 0x40013C00 :
      0;

    /**
     * Status register
     */
    struct SR
    : public Register< uint32_t, base_addr + 0x0, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x0, Access::rw, 0x00000000 >;

      using STRT   = RegisterBits< type,  4,  1 >;  /**< Regular channel start flag          */
      using JSTRT  = RegisterBits< type,  3,  1 >;  /**< Injected channel start flag         */
      using JEOC   = RegisterBits< type,  2,  1 >;  /**< Injected channel end of conversion  */
      using EOC    = RegisterBits< type,  1,  1 >;  /**< Regular channel end of conversion   */
      using AWD    = RegisterBits< type,  0,  1 >;  /**< Analog watchdog flag                */
    };

    /**
     * Control register 1
     */
    struct CR1
    : public Register< uint32_t, base_addr + 0x4, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x4, Access::rw, 0x00000000 >;

      using AWDEN    = RegisterBits< type, 23,  1 >;  /**< Analog watchdog enable on regular channels            */
      using JAWDEN   = RegisterBits< type, 22,  1 >;  /**< Analog watchdog enable on injected channels           */
      using DUALMOD  = RegisterBits< type, 16,  4 >;  /**< Dual mode selection                                   */
      using DISCNUM  = RegisterBits< type, 13,  3 >;  /**< Discontinuous mode channel count                      */
      using JDISCEN  = RegisterBits< type, 12,  1 >;  /**< Discontinuous mode on injected channels               */
      using DISCEN   = RegisterBits< type, 11,  1 >;  /**< Discontinuous mode on regular channels                */
      using JAUTO    = RegisterBits< type, 10,  1 >;  /**< Automatic injected group conversion                   */
      using AWDSGL   = RegisterBits< type,  9,  1 >;  /**< Enable the watchdog on a single channel in scan mode  */
      using SCAN     = RegisterBits< type,  8,  1 >;  /**< Scan mode                                             */
      using JEOCIE   = RegisterBits< type,  7,  1 >;  /**< Interrupt enable for injected channels                */
      using AWDIE    = RegisterBits< type,  6,  1 >;  /**< Analog watchdog interrupt enable                      */
      using EOCIE    = RegisterBits< type,  5,  1 >;  /**< Interrupt enable for EOC                              */
      using AWDCH    = RegisterBits< type,  0,  5 >;  /**< Analog watchdog channel select bits                   */
    };

    /**
     * Control register 2
     */
    struct CR2
    : public Register< uint32_t, base_addr + 0x8, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x8, Access::rw, 0x00000000 >;

      using TSVREFE   = RegisterBits< type, 23,  1 >;  /**< Temperature sensor and VREFINT enable                   */
      using SWSTART   = RegisterBits< type, 22,  1 >;  /**< Start conversion of regular channels                    */
      using JSWSTART  = RegisterBits< type, 21,  1 >;  /**< Start conversion of injected channels                   */
      using EXTTRIG   = RegisterBits< type, 20,  1 >;  /**< External trigger conversion mode for regular channels   */
      using EXTSEL    = RegisterBits< type, 17,  3 >;  /**< External event select for regular group                 */
      using JEXTTRIG  = RegisterBits< type, 15,  1 >;  /**< External trigger conversion mode for injected channels  */
      using JEXTSEL   = RegisterBits< type, 12,  3 >;  /**< External event select for injected group                */
      using ALIGN     = RegisterBits< type, 11,  1 >;  /**< Data alignment                                          */
      using DMA       = RegisterBits< type,  8,  1 >;  /**< Direct memory access mode                               */
      using RSTCAL    = RegisterBits< type,  3,  1 >;  /**< Reset calibration                                       */
      using CAL       = RegisterBits< type,  2,  1 >;  /**< A/D calibration                                         */
      using CONT      = RegisterBits< type,  1,  1 >;  /**< Continuous conversion                                   */
      using ADON      = RegisterBits< type,  0,  1 >;  /**< A/D converter ON / OFF                                  */
    };

    /**
     * Sample time register 1
     */
    struct SMPR1
    : public Register< uint32_t, base_addr + 0xc, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0xc, Access::rw, 0x00000000 >;

      using SMP10  = RegisterBits< type,  0,  3 >;  /**< Channel 10 sample time selection  */
      using SMP11  = RegisterBits< type,  3,  3 >;  /**< Channel 11 sample time selection  */
      using SMP12  = RegisterBits< type,  6,  3 >;  /**< Channel 12 sample time selection  */
      using SMP13  = RegisterBits< type,  9,  3 >;  /**< Channel 13 sample time selection  */
      using SMP14  = RegisterBits< type, 12,  3 >;  /**< Channel 14 sample time selection  */
      using SMP15  = RegisterBits< type, 15,  3 >;  /**< Channel 15 sample time selection  */
      using SMP16  = RegisterBits< type, 18,  3 >;  /**< Channel 16 sample time selection  */
      using SMP17  = RegisterBits< type, 21,  3 >;  /**< Channel 17 sample time selection  */
    };

    /**
     * Sample time register 2
     */
    struct SMPR2
    : public Register< uint32_t, base_addr + 0x10, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x10, Access::rw, 0x00000000 >;

      using SMP0  = RegisterBits< type,  0,  3 >;  /**< Channel 0 sample time selection  */
      using SMP1  = RegisterBits< type,  3,  3 >;  /**< Channel 1 sample time selection  */
      using SMP2  = RegisterBits< type,  6,  3 >;  /**< Channel 2 sample time selection  */
      using SMP3  = RegisterBits< type,  9,  3 >;  /**< Channel 3 sample time selection  */
      using SMP4  = RegisterBits< type, 12,  3 >;  /**< Channel 4 sample time selection  */
      using SMP5  = RegisterBits< type, 15,  3 >;  /**< Channel 5 sample time selection  */
      using SMP6  = RegisterBits< type, 18,  3 >;  /**< Channel 6 sample time selection  */
      using SMP7  = RegisterBits< type, 21,  3 >;  /**< Channel 7 sample time selection  */
      using SMP8  = RegisterBits< type, 24,  3 >;  /**< Channel 8 sample time selection  */
      using SMP9  = RegisterBits< type, 27,  3 >;  /**< Channel 9 sample time selection  */
    };

    /**
     * Injected channel data offset register x
     */
    template<unsigned jofr_no>
    struct JOFR
    : public Register< uint32_t, base_addr + 0x14 + (jofr_no - 1) * 4, Access::rw, 0x00000000 >
    {
      static_assert((jofr_no >= 1) && (jofr_no <= 4), "invalid index for JOFR register");
      using type = Register< uint32_t, base_addr + 0x14 + (jofr_no - 1) * 4, Access::rw, 0x00000000 >;

      using JOFFSET  = RegisterBits< type,  0, 12 >;  /**< Data offset for injected channel x  */
    };

    /**
     * Watchdog higher threshold register
     */
    struct HTR
    : public Register< uint32_t, base_addr + 0x24, Access::rw, 0x00000FFF >
    {
      using type = Register< uint32_t, base_addr + 0x24, Access::rw, 0x00000FFF >;

      using HT  = RegisterBits< type,  0, 12 >;  /**< Analog watchdog higher threshold  */
    };

    /**
     * Watchdog lower threshold register
     */
    struct LTR
    : public Register< uint32_t, base_addr + 0x28, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x28, Access::rw, 0x00000000 >;

      using LT  = RegisterBits< type,  0, 12 >;  /**< Analog watchdog lower threshold  */
    };

    /**
     * Regular sequence register 1
     */
    struct SQR1
    : public Register< uint32_t, base_addr + 0x2c, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x2c, Access::rw, 0x00000000 >;

      using L     = RegisterBits< type, 20,  4 >;  /**< Regular channel sequence length      */
      using SQ16  = RegisterBits< type, 15,  5 >;  /**< 16th conversion in regular sequence  */
      using SQ15  = RegisterBits< type, 10,  5 >;  /**< 15th conversion in regular sequence  */
      using SQ14  = RegisterBits< type,  5,  5 >;  /**< 14th conversion in regular sequence  */
      using SQ13  = RegisterBits< type,  0,  5 >;  /**< 13th conversion in regular sequence  */
    };

    /**
     * Regular sequence register 2
     */
    struct SQR2
    : public Register< uint32_t, base_addr + 0x30, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x30, Access::rw, 0x00000000 >;

      using SQ12  = RegisterBits< type, 25,  5 >;  /**< 12th conversion in regular sequence  */
      using SQ11  = RegisterBits< type, 20,  5 >;  /**< 11th conversion in regular sequence  */
      using SQ10  = RegisterBits< type, 15,  5 >;  /**< 10th conversion in regular sequence  */
      using SQ9   = RegisterBits< type, 10,  5 >;  /**< 9th conversion in regular sequence   */
      using SQ8   = RegisterBits< type,  5,  5 >;  /**< 8th conversion in regular sequence   */
      using SQ7   = RegisterBits< type,  0,  5 >;  /**< 7th conversion in regular sequence   */
    };

    /**
     * Regular sequence register 3
     */
    struct SQR3
    : public Register< uint32_t, base_addr + 0x34, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x34, Access::rw, 0x00000000 >;

      using SQ6  = RegisterBits< type, 25,  5 >;  /**< 6th conversion in regular sequence  */
      using SQ5  = RegisterBits< type, 20,  5 >;  /**< 5th conversion in regular sequence  */
      using SQ4  = RegisterBits< type, 15,  5 >;  /**< 4th conversion in regular sequence  */
      using SQ3  = RegisterBits< type, 10,  5 >;  /**< 3rd conversion in regular sequence  */
      using SQ2  = RegisterBits< type,  5,  5 >;  /**< 2nd conversion in regular sequence  */
      using SQ1  = RegisterBits< type,  0,  5 >;  /**< 1st conversion in regular sequence  */
    };

    /**
     * Injected sequence register
     */
    struct JSQR
    : public Register< uint32_t, base_addr + 0x38, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x38, Access::rw, 0x00000000 >;

      using JL    = RegisterBits< type, 20,  2 >;  /**< Injected sequence length             */
      using JSQ4  = RegisterBits< type, 15,  5 >;  /**< 4th conversion in injected sequence  */
      using JSQ3  = RegisterBits< type, 10,  5 >;  /**< 3rd conversion in injected sequence  */
      using JSQ2  = RegisterBits< type,  5,  5 >;  /**< 2nd conversion in injected sequence  */
      using JSQ1  = RegisterBits< type,  0,  5 >;  /**< 1st conversion in injected sequence  */
    };

    /**
     * Injected data register x
     */
    template<unsigned jdr_no>
    struct JDR
    : public Register< uint32_t, base_addr + 0x3c + (jdr_no - 1) * 4, Access::ro, 0x00000000 >
    {
      static_assert((jdr_no >= 1) && (jdr_no <= 4), "invalid index for JDR register");
      using type = Register< uint32_t, base_addr + 0x3c + (jdr_no - 1) * 4, Access::ro, 0x00000000 >;

      using JDATA  = RegisterBits< type,  0, 16 >;  /**< Injected data  */
    };

    /**
     * regular data register
     */
    struct DR
    : public Register< uint32_t, base_addr + 0x4c, Access::ro, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x4c, Access::ro, 0x00000000 >;

      using DATA      = RegisterBits< type,  0, 16 >;  /**< Regular data  */
      using ADC2DATA  = RegisterBits< type, 16, 16 >;  /**< ADC2 data     */
    };
  };
}

#endif // ARCH_REG_ADC_HPP_INCLUDED
