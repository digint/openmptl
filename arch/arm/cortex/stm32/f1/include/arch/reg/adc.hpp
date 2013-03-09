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

#ifndef REG_ADC_HPP_INCLUDED
#define REG_ADC_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /**
   * Analog-to-digital converter (ADC)
   */
  template<std::size_t adc_no>
  class ADC
  {
    static_assert((adc_no >= 1) && (adc_no <= 3), "invalid index for register");

    static constexpr reg_addr_t reg_base = adc_no == 1 ? 0x40012400 :
                                           adc_no == 2 ? 0x40012800 :
                                           adc_no == 3 ? 0x40013C00 :
                                           0;

    /**
     * ADC status register
     */
    template<typename R>
    struct __SR
    : public R
    {
      // TODO: special access rc_w0
      typedef RegisterBits< R,  0,  1 > AWD;           /**< [ 0: 0] Analog watchdog flag                 */
      typedef RegisterBits< R,  1,  1 > EOC;           /**< [ 1: 1] End of conversion                    */
      typedef RegisterBits< R,  2,  1 > JEOC;          /**< [ 2: 2] Injected channel end of conversion   */
      typedef RegisterBits< R,  3,  1 > JSTRT;         /**< [ 3: 3] Injected channel Start flag          */
      typedef RegisterBits< R,  4,  1 > STRT;          /**< [ 4: 4] Regular channel Start flag           */
    };


    /**
     * ADC control register 1
     */
    template<typename R>
    struct __CR1
    : public R
    {
      typedef RegisterBits< R,  0,  5 > AWDCH;         /**< [ 4: 0] Analog watchdog channel select bits                    */
      typedef RegisterBits< R,  5,  1 > EOCIE;         /**< [ 5: 5] Interrupt enable for EOC                               */
      typedef RegisterBits< R,  6,  1 > AWDIE;         /**< [ 6: 6] AAnalog Watchdog interrupt enable                      */
      typedef RegisterBits< R,  7,  1 > JEOCIE;        /**< [ 7: 7] Interrupt enable for injected channels                 */
      typedef RegisterBits< R,  8,  1 > SCAN;          /**< [ 8: 8] Scan mode                                              */
      typedef RegisterBits< R,  9,  1 > AWDSGL;        /**< [ 9: 9] Enable the watchdog on a single channel in scan mode   */
      typedef RegisterBits< R, 10,  1 > JAUTO;         /**< [10:10] Automatic injected group conversion                    */
      typedef RegisterBits< R, 11,  1 > DISCEN;        /**< [11:11] Discontinuous mode on regular channels                 */
      typedef RegisterBits< R, 12,  1 > JDISCEN;       /**< [12:12] Discontinuous mode on injected channels                */
      typedef RegisterBits< R, 13,  3 > DISCNUM;       /**< [15:13] Discontinuous mode channel count                       */
      typedef RegisterBits< R, 16,  4 > DUALMOD;       /**< [19:16] Dual mode selection                                    */
      typedef RegisterBits< R, 22,  1 > JAWDEN;        /**< [22:22] Analog watchdog enable on injected channels            */
      typedef RegisterBits< R, 23,  1 > AWDEN;         /**< [23:23] Analog watchdog enable on regular channels             */
    };


    /**
     * ADC control register 2
     */
    template<typename R>
    struct __CR2
    : public R
    {
      typedef RegisterBits< R,  0,  1 > ADON;          /**< [ 0: 0] A/D Converter ON / OFF                                   */
      typedef RegisterBits< R,  1,  1 > CONT;          /**< [ 1: 1] Continuous Conversion                                    */
      typedef RegisterBits< R,  2,  1 > CAL;           /**< [ 2: 2] A/D Calibration                                          */
      typedef RegisterBits< R,  3,  1 > RSTCAL;        /**< [ 3: 3] Reset Calibration                                        */
      typedef RegisterBits< R,  8,  1 > DMA;           /**< [ 8: 8] Direct Memory access mode                                */
      typedef RegisterBits< R, 11,  1 > ALIGN;         /**< [11:11] Data Alignment                                           */
      typedef RegisterBits< R, 12,  3 > JEXTSEL;       /**< [14:12] External event select for injected group                 */
      typedef RegisterBits< R, 15,  1 > JEXTTRIG;      /**< [15:15] External Trigger Conversion mode for injected channels   */
      typedef RegisterBits< R, 17,  3 > EXTSEL;        /**< [19:17] External Event Select for regular group                  */
      typedef RegisterBits< R, 20,  1 > EXTTRIG;       /**< [20:20] External Trigger Conversion mode for regular channels    */
      typedef RegisterBits< R, 21,  1 > JSWSTART;      /**< [21:21] Start Conversion of injected channels                    */
      typedef RegisterBits< R, 22,  1 > SWSTART;       /**< [22:22] Start Conversion of regular channels                     */
      typedef RegisterBits< R, 23,  1 > TSVREFE;       /**< [23:23] Temperature Sensor and VREFINT Enable                    */
    };


    /**
     * ADC sample time register 1
     */
    template<typename R>
    struct __SMPR1
    : public R
    {
      typedef RegisterBits< R,  0,  3 > SMP10;         /**< [ 2: 0] Channel 10 Sample time selection   */
      typedef RegisterBits< R,  3,  3 > SMP11;         /**< [ 5: 3] Channel 11 Sample time selection   */
      typedef RegisterBits< R,  6,  3 > SMP12;         /**< [ 8: 6] Channel 12 Sample time selection   */
      typedef RegisterBits< R,  9,  3 > SMP13;         /**< [11: 9] Channel 13 Sample time selection   */
      typedef RegisterBits< R, 12,  3 > SMP14;         /**< [14:12] Channel 14 Sample time selection   */
      typedef RegisterBits< R, 15,  3 > SMP15;         /**< [17:15] Channel 15 Sample time selection   */
      typedef RegisterBits< R, 18,  3 > SMP16;         /**< [20:18] Channel 16 Sample time selection   */
      typedef RegisterBits< R, 21,  3 > SMP17;         /**< [23:21] Channel 17 Sample time selection   */
    };


    /**
     * ADC sample time register 2
     */
    template<typename R>
    struct __SMPR2
    : public R
    {
      typedef RegisterBits< R,  0,  3 > SMP0;          /**< [ 2: 0] Channel 0 Sample time selection   */
      typedef RegisterBits< R,  3,  3 > SMP1;          /**< [ 5: 3] Channel 1 Sample time selection   */
      typedef RegisterBits< R,  6,  3 > SMP2;          /**< [ 8: 6] Channel 2 Sample time selection   */
      typedef RegisterBits< R,  9,  3 > SMP3;          /**< [11: 9] Channel 3 Sample time selection   */
      typedef RegisterBits< R, 12,  3 > SMP4;          /**< [14:12] Channel 4 Sample time selection   */
      typedef RegisterBits< R, 15,  3 > SMP5;          /**< [17:15] Channel 5 Sample time selection   */
      typedef RegisterBits< R, 18,  3 > SMP6;          /**< [20:18] Channel 6 Sample time selection   */
      typedef RegisterBits< R, 21,  3 > SMP7;          /**< [23:21] Channel 7 Sample time selection   */
      typedef RegisterBits< R, 24,  3 > SMP8;          /**< [26:24] Channel 8 Sample time selection   */
      typedef RegisterBits< R, 27,  3 > SMP9;          /**< [29:27] Channel 9 Sample time selection   */
    };


    /**
     * ADC injected channel data offset register
     */
    template<typename R>
    struct __JOFR
    : public R
    {
      typedef RegisterBits< R,  0, 12 > JOFFSET;       /**< [11: 0] Data offset for injected channel 1   */
    };


    /**
     * ADC watchdog high threshold register
     */
    template<typename R>
    struct __HTR
    : public R
    {
      typedef RegisterBits< R,  0, 12 > HT;            /**< [11: 0] Analog watchdog high threshold   */
    };


    /**
     * ADC watchdog low threshold register
     */
    template<typename R>
    struct __LTR
    : public R
    {
      typedef RegisterBits< R,  0, 12 > LT;            /**< [11: 0] Analog watchdog low threshold   */
    };


    /**
     * ADC regular sequence register 1
     */
    template<typename R>
    struct __SQR1
    : public R
    {
      typedef RegisterBits< R,  0,  5 > SQ13;          /**< [ 4: 0] 13th conversion in regular sequence   */
      typedef RegisterBits< R,  5,  5 > SQ14;          /**< [ 9: 5] 14th conversion in regular sequence   */
      typedef RegisterBits< R, 10,  5 > SQ15;          /**< [14:10] 15th conversion in regular sequence   */
      typedef RegisterBits< R, 15,  5 > SQ16;          /**< [19:15] 16th conversion in regular sequence   */
      typedef RegisterBits< R, 20,  4 > L;             /**< [23:20] Regular channel sequence length       */
    };


    /**
     * ADC regular sequence register 2
     */
    template<typename R>
    struct __SQR2
    : public R
    {
      typedef RegisterBits< R,  0,  5 > SQ7;           /**< [ 4: 0] 7th conversion in regular sequence    */
      typedef RegisterBits< R,  5,  5 > SQ8;           /**< [ 9: 5] 8th conversion in regular sequence    */
      typedef RegisterBits< R, 10,  5 > SQ9;           /**< [14:10] 9th conversion in regular sequence    */
      typedef RegisterBits< R, 15,  5 > SQ10;          /**< [19:15] 10th conversion in regular sequence   */
      typedef RegisterBits< R, 20,  5 > SQ11;          /**< [24:20] 11th conversion in regular sequence   */
      typedef RegisterBits< R, 25,  5 > SQ12;          /**< [29:25] 12th conversion in regular sequence   */
    };


    /**
     * ADC regular sequence register 3
     */
    template<typename R>
    struct __SQR3
    : public R
    {
      typedef RegisterBits< R,  0,  5 > SQ1;           /**< [ 4: 0] 1st conversion in regular sequence   */
      typedef RegisterBits< R,  5,  5 > SQ2;           /**< [ 9: 5] 2nd conversion in regular sequence   */
      typedef RegisterBits< R, 10,  5 > SQ3;           /**< [14:10] 3rd conversion in regular sequence   */
      typedef RegisterBits< R, 15,  5 > SQ4;           /**< [19:15] 4th conversion in regular sequence   */
      typedef RegisterBits< R, 20,  5 > SQ5;           /**< [24:20] 5th conversion in regular sequence   */
      typedef RegisterBits< R, 25,  5 > SQ6;           /**< [29:25] 6th conversion in regular sequence   */
    };


    /**
     * ADC injected sequence register
     */
    template<typename R>
    struct __JSQR
    : public R
    {
      typedef RegisterBits< R,  0,  5 > JSQ1;          /**< [ 4: 0] 1st conversion in injected sequence   */
      typedef RegisterBits< R,  5,  5 > JSQ2;          /**< [ 9: 5] 2nd conversion in injected sequence   */
      typedef RegisterBits< R, 10,  5 > JSQ3;          /**< [14:10] 3rd conversion in injected sequence   */
      typedef RegisterBits< R, 15,  5 > JSQ4;          /**< [19:15] 4th conversion in injected sequence   */
      typedef RegisterBits< R, 20,  2 > JL;            /**< [21:20] Injected Sequence length)             */
    };


    /**
     * ADC injected data register 1
     */
    template<typename R>
    struct __JDR
    : public R
    {
      typedef RegisterBits< R,  0, 16 > JDATA;         /**< [15: 0] Injected data   */
    };

    /**
     * ADC regular data register
     */
    template<typename R>
    struct __DR
    : public R
    {
      typedef RegisterBits< R,  0, 16 > DATA;          /**< [15: 0] Regular data   */
      typedef RegisterBits< R, 16, 16 > ADC2DATA;      /**< [31:16] ADC2 data      */
    };


  public:

    typedef __SR   < Register< uint32_t, reg_base + 0x00, Access::rw > > SR;
    typedef __CR1  < Register< uint32_t, reg_base + 0x04, Access::rw > > CR1;
    typedef __CR2  < Register< uint32_t, reg_base + 0x08, Access::rw > > CR2;
    typedef __SMPR1< Register< uint32_t, reg_base + 0x0c, Access::rw > > SMPR1;
    typedef __SMPR2< Register< uint32_t, reg_base + 0x10, Access::rw > > SMPR2;

    template<std::size_t jofr_no>
    struct JOFR
    : __JOFR<Register< uint32_t, reg_base + 0x14 + (jofr_no - 1) * 4, Access::rw >>
    { static_assert((jofr_no >= 1) && (jofr_no <= 4), "invalid index for register"); };

    typedef __HTR  < Register< uint32_t, reg_base + 0x24, Access::rw, 0x00000FFF > > HTR;
    typedef __LTR  < Register< uint32_t, reg_base + 0x28, Access::rw             > > LTR;
    typedef __SQR1 < Register< uint32_t, reg_base + 0x2c, Access::rw             > > SQR1;
    typedef __SQR2 < Register< uint32_t, reg_base + 0x30, Access::rw             > > SQR2;
    typedef __SQR3 < Register< uint32_t, reg_base + 0x34, Access::rw             > > SQR3;
    typedef __JSQR < Register< uint32_t, reg_base + 0x38, Access::rw             > > JSQR;

    template<std::size_t jdr_no>
    struct JDR
    : __JDR<Register< uint32_t, reg_base + 0x3c + (jdr_no - 1) * 4, Access::ro >>
    { static_assert((jdr_no >= 1) && (jdr_no <= 4), "invalid index for register"); };

    typedef __DR   < Register< uint32_t, reg_base + 0x4c, Access::ro > > DR;
  };
}

#endif // REG_ADC_HPP_INCLUDED
