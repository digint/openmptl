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

namespace mptl {
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
  : public reg< uint32_t, base_addr + 0x0, rw, 0x00000000 >
  {
    using type = reg< uint32_t, base_addr + 0x0, rw, 0x00000000 >;

    using STRT   = regbits< type,  4,  1 >;  /**< Regular channel start flag          */
    using JSTRT  = regbits< type,  3,  1 >;  /**< Injected channel start flag         */
    using JEOC   = regbits< type,  2,  1 >;  /**< Injected channel end of conversion  */
    using EOC    = regbits< type,  1,  1 >;  /**< Regular channel end of conversion   */
    using AWD    = regbits< type,  0,  1 >;  /**< Analog watchdog flag                */
  };

  /**
   * Control register 1
   */
  struct CR1
  : public reg< uint32_t, base_addr + 0x4, rw, 0x00000000 >
  {
    using type = reg< uint32_t, base_addr + 0x4, rw, 0x00000000 >;

    using AWDEN    = regbits< type, 23,  1 >;  /**< Analog watchdog enable on regular channels            */
    using JAWDEN   = regbits< type, 22,  1 >;  /**< Analog watchdog enable on injected channels           */
    using DUALMOD  = regbits< type, 16,  4 >;  /**< Dual mode selection                                   */
    using DISCNUM  = regbits< type, 13,  3 >;  /**< Discontinuous mode channel count                      */
    using JDISCEN  = regbits< type, 12,  1 >;  /**< Discontinuous mode on injected channels               */
    using DISCEN   = regbits< type, 11,  1 >;  /**< Discontinuous mode on regular channels                */
    using JAUTO    = regbits< type, 10,  1 >;  /**< Automatic injected group conversion                   */
    using AWDSGL   = regbits< type,  9,  1 >;  /**< Enable the watchdog on a single channel in scan mode  */
    using SCAN     = regbits< type,  8,  1 >;  /**< Scan mode                                             */
    using JEOCIE   = regbits< type,  7,  1 >;  /**< Interrupt enable for injected channels                */
    using AWDIE    = regbits< type,  6,  1 >;  /**< Analog watchdog interrupt enable                      */
    using EOCIE    = regbits< type,  5,  1 >;  /**< Interrupt enable for EOC                              */
    using AWDCH    = regbits< type,  0,  5 >;  /**< Analog watchdog channel select bits                   */
  };

  /**
   * Control register 2
   */
  struct CR2
  : public reg< uint32_t, base_addr + 0x8, rw, 0x00000000 >
  {
    using type = reg< uint32_t, base_addr + 0x8, rw, 0x00000000 >;

    using TSVREFE   = regbits< type, 23,  1 >;  /**< Temperature sensor and VREFINT enable                   */
    using SWSTART   = regbits< type, 22,  1 >;  /**< Start conversion of regular channels                    */
    using JSWSTART  = regbits< type, 21,  1 >;  /**< Start conversion of injected channels                   */
    using EXTTRIG   = regbits< type, 20,  1 >;  /**< External trigger conversion mode for regular channels   */
    using EXTSEL    = regbits< type, 17,  3 >;  /**< External event select for regular group                 */
    using JEXTTRIG  = regbits< type, 15,  1 >;  /**< External trigger conversion mode for injected channels  */
    using JEXTSEL   = regbits< type, 12,  3 >;  /**< External event select for injected group                */
    using ALIGN     = regbits< type, 11,  1 >;  /**< Data alignment                                          */
    using DMA       = regbits< type,  8,  1 >;  /**< Direct memory access mode                               */
    using RSTCAL    = regbits< type,  3,  1 >;  /**< Reset calibration                                       */
    using CAL       = regbits< type,  2,  1 >;  /**< A/D calibration                                         */
    using CONT      = regbits< type,  1,  1 >;  /**< Continuous conversion                                   */
    using ADON      = regbits< type,  0,  1 >;  /**< A/D converter ON / OFF                                  */
  };

  /**
   * Sample time register 1
   */
  struct SMPR1
  : public reg< uint32_t, base_addr + 0xc, rw, 0x00000000 >
  {
    using type = reg< uint32_t, base_addr + 0xc, rw, 0x00000000 >;

    using SMP10  = regbits< type,  0,  3 >;  /**< Channel 10 sample time selection  */
    using SMP11  = regbits< type,  3,  3 >;  /**< Channel 11 sample time selection  */
    using SMP12  = regbits< type,  6,  3 >;  /**< Channel 12 sample time selection  */
    using SMP13  = regbits< type,  9,  3 >;  /**< Channel 13 sample time selection  */
    using SMP14  = regbits< type, 12,  3 >;  /**< Channel 14 sample time selection  */
    using SMP15  = regbits< type, 15,  3 >;  /**< Channel 15 sample time selection  */
    using SMP16  = regbits< type, 18,  3 >;  /**< Channel 16 sample time selection  */
    using SMP17  = regbits< type, 21,  3 >;  /**< Channel 17 sample time selection  */
  };

  /**
   * Sample time register 2
   */
  struct SMPR2
  : public reg< uint32_t, base_addr + 0x10, rw, 0x00000000 >
  {
    using type = reg< uint32_t, base_addr + 0x10, rw, 0x00000000 >;

    using SMP0  = regbits< type,  0,  3 >;  /**< Channel 0 sample time selection  */
    using SMP1  = regbits< type,  3,  3 >;  /**< Channel 1 sample time selection  */
    using SMP2  = regbits< type,  6,  3 >;  /**< Channel 2 sample time selection  */
    using SMP3  = regbits< type,  9,  3 >;  /**< Channel 3 sample time selection  */
    using SMP4  = regbits< type, 12,  3 >;  /**< Channel 4 sample time selection  */
    using SMP5  = regbits< type, 15,  3 >;  /**< Channel 5 sample time selection  */
    using SMP6  = regbits< type, 18,  3 >;  /**< Channel 6 sample time selection  */
    using SMP7  = regbits< type, 21,  3 >;  /**< Channel 7 sample time selection  */
    using SMP8  = regbits< type, 24,  3 >;  /**< Channel 8 sample time selection  */
    using SMP9  = regbits< type, 27,  3 >;  /**< Channel 9 sample time selection  */
  };

  /**
   * Sample time register: provides SMPR1 or SMPR2, depending on channel
   *
   * NOTE: this is not from the reference manual 
   */
  template<unsigned channel>
  struct SMPRx
  : public std::conditional< (channel > 9), SMPR1, SMPR2 >::type
  {
    static_assert(channel <= 17, "invalid channel");
    using type = typename std::conditional< (channel > 9), SMPR1, SMPR2 >::type;
    using SMP = regbits< type, (channel % 10) * 3, 3 >;
  };

  /**
   * Injected channel data offset register x
   */
  template<unsigned jofr_no>
  struct JOFR
  : public reg< uint32_t, base_addr + 0x14 + (jofr_no - 1) * 4, rw, 0x00000000 >
  {
    static_assert((jofr_no >= 1) && (jofr_no <= 4), "invalid index for JOFR register");
    using type = reg< uint32_t, base_addr + 0x14 + (jofr_no - 1) * 4, rw, 0x00000000 >;

    using JOFFSET  = regbits< type,  0, 12 >;  /**< Data offset for injected channel x  */
  };

  /**
   * Watchdog higher threshold register
   */
  struct HTR
  : public reg< uint32_t, base_addr + 0x24, rw, 0x00000FFF >
  {
    using type = reg< uint32_t, base_addr + 0x24, rw, 0x00000FFF >;

    using HT  = regbits< type,  0, 12 >;  /**< Analog watchdog higher threshold  */
  };

  /**
   * Watchdog lower threshold register
   */
  struct LTR
  : public reg< uint32_t, base_addr + 0x28, rw, 0x00000000 >
  {
    using type = reg< uint32_t, base_addr + 0x28, rw, 0x00000000 >;

    using LT  = regbits< type,  0, 12 >;  /**< Analog watchdog lower threshold  */
  };

  /**
   * Regular sequence register 1
   */
  struct SQR1
  : public reg< uint32_t, base_addr + 0x2c, rw, 0x00000000 >
  {
    using type = reg< uint32_t, base_addr + 0x2c, rw, 0x00000000 >;

    using L     = regbits< type, 20,  4 >;  /**< Regular channel sequence length      */
    using SQ16  = regbits< type, 15,  5 >;  /**< 16th conversion in regular sequence  */
    using SQ15  = regbits< type, 10,  5 >;  /**< 15th conversion in regular sequence  */
    using SQ14  = regbits< type,  5,  5 >;  /**< 14th conversion in regular sequence  */
    using SQ13  = regbits< type,  0,  5 >;  /**< 13th conversion in regular sequence  */
  };

  /**
   * Regular sequence register 2
   */
  struct SQR2
  : public reg< uint32_t, base_addr + 0x30, rw, 0x00000000 >
  {
    using type = reg< uint32_t, base_addr + 0x30, rw, 0x00000000 >;

    using SQ12  = regbits< type, 25,  5 >;  /**< 12th conversion in regular sequence  */
    using SQ11  = regbits< type, 20,  5 >;  /**< 11th conversion in regular sequence  */
    using SQ10  = regbits< type, 15,  5 >;  /**< 10th conversion in regular sequence  */
    using SQ9   = regbits< type, 10,  5 >;  /**< 9th conversion in regular sequence   */
    using SQ8   = regbits< type,  5,  5 >;  /**< 8th conversion in regular sequence   */
    using SQ7   = regbits< type,  0,  5 >;  /**< 7th conversion in regular sequence   */
  };

  /**
   * Regular sequence register 3
   */
  struct SQR3
  : public reg< uint32_t, base_addr + 0x34, rw, 0x00000000 >
  {
    using type = reg< uint32_t, base_addr + 0x34, rw, 0x00000000 >;

    using SQ6  = regbits< type, 25,  5 >;  /**< 6th conversion in regular sequence  */
    using SQ5  = regbits< type, 20,  5 >;  /**< 5th conversion in regular sequence  */
    using SQ4  = regbits< type, 15,  5 >;  /**< 4th conversion in regular sequence  */
    using SQ3  = regbits< type, 10,  5 >;  /**< 3rd conversion in regular sequence  */
    using SQ2  = regbits< type,  5,  5 >;  /**< 2nd conversion in regular sequence  */
    using SQ1  = regbits< type,  0,  5 >;  /**< 1st conversion in regular sequence  */
  };

  /**
   * Regular sequence register: provides SQR1, SQR2 or SQR3, depending on channel
   *
   * NOTE: this is not from the reference manual 
   */
  template<unsigned rank>
  struct SQRx
  : public std::conditional< (rank > 12), SQR1,
    typename std::conditional< (rank > 6), SQR2, SQR3 >::type >::type
  {
    static_assert((rank >= 1) && (rank <= 16), "invalid rank");

    using type = typename std::conditional< (rank > 12), SQR1,
      typename std::conditional< (rank > 6), SQR2, SQR3 >::type >::type;

    using SQ = regbits< type, ((rank - 1) % 6) * 5, 5 >;
  };

  /**
   * Injected sequence register
   */
  struct JSQR
  : public reg< uint32_t, base_addr + 0x38, rw, 0x00000000 >
  {
    using type = reg< uint32_t, base_addr + 0x38, rw, 0x00000000 >;

    using JL    = regbits< type, 20,  2 >;  /**< Injected sequence length             */
    using JSQ4  = regbits< type, 15,  5 >;  /**< 4th conversion in injected sequence  */
    using JSQ3  = regbits< type, 10,  5 >;  /**< 3rd conversion in injected sequence  */
    using JSQ2  = regbits< type,  5,  5 >;  /**< 2nd conversion in injected sequence  */
    using JSQ1  = regbits< type,  0,  5 >;  /**< 1st conversion in injected sequence  */
  };

  /**
   * Injected data register x
   */
  template<unsigned jdr_no>
  struct JDR
  : public reg< uint32_t, base_addr + 0x3c + (jdr_no - 1) * 4, ro, 0x00000000 >
  {
    static_assert((jdr_no >= 1) && (jdr_no <= 4), "invalid index for JDR register");
    using type = reg< uint32_t, base_addr + 0x3c + (jdr_no - 1) * 4, ro, 0x00000000 >;

    using JDATA  = regbits< type,  0, 16 >;  /**< Injected data  */
  };

  /**
   * regular data register
   */
  struct DR
  : public reg< uint32_t, base_addr + 0x4c, ro, 0x00000000 >
  {
    using type = reg< uint32_t, base_addr + 0x4c, ro, 0x00000000 >;

    using DATA      = regbits< type,  0, 16 >;  /**< Regular data  */
    using ADC2DATA  = regbits< type, 16, 16 >;  /**< ADC2 data     */
  };
};

} // namespace mptl

#endif // ARCH_REG_ADC_HPP_INCLUDED
