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

#ifndef ADC_HPP_INCLUDED
#define ADC_HPP_INCLUDED

#include <core.hpp>
#include <arch/rcc.hpp>

namespace cAdc
{
  /**
   * CR1[19:16] DUALMOD: Dual mode selection.
   * These bits are written by software to select the operating mode.
   */
  enum class Mode : uint32_t {
    independent              = 0,  /**<  0000: Independent mode.                                           */
    regr_injec_simult        = 1,  /**<  0001: Combined regular simultaneous + injected simultaneous mode  */
    reg_simult_alter_trig    = 2,  /**<  0010: Combined regular simultaneous + alternate trigger mode      */
    injec_simult_fast_interl = 3,  /**<  0011: Combined injected simultaneous + fast interleaved mode      */
    injec_simult_slow_interl = 4,  /**<  0100: Combined injected simultaneous + slow Interleaved mode      */
    injec_simult             = 5,  /**<  0101: Injected simultaneous mode only                             */
    reg_simult               = 6,  /**<  0110: Regular simultaneous mode only                              */
    fast_interl              = 7,  /**<  0111: Fast interleaved mode only                                  */
    slow_interl              = 8,  /**<  1000: Slow interleaved mode only                                  */
    alter_trig               = 9   /**<  1001: Alternate trigger mode only                                 */
  };

  /**
   * CR1[8] SCAN: Scan mode.
   * This bit is set and cleared by software to enable/disable Scan
   * mode. In Scan mode, the inputs selected through the ADC_SQRx or
   * ADC_JSQRx registers are converted.
   */
  enum class ScanMode : uint32_t {
    disabled = 0,  /**< 0: Scan mode disabled  */
    enabled = 1    /**< 1: Scan mode enabled   */
  };

  /** 
   * CR2[1] CONT: Continuous conversion.
   * This bit is set and cleared by software. If set conversion takes place
   * continuously till this bit is reset.
   */
  enum class ContinuousConvMode : uint32_t {
    single     = 0,  /**< 0: Single conversion mode      */
    continuous = 1   /**< 1: Continuous conversion mode  */
  };

  /**
   * CR2[19:17] EXTSEL: External event select for regular group.
   * These bits select the external event used to trigger the start of
   * conversion of a regular group:
   */
  enum class ExternalTrigConv : uint32_t {
    // For ADC1 and ADC2, the assigned triggers are:
    T1_CC1    = 0,  /**< 000: Timer 1 CC1 event   */
    T1_CC2    = 1,  /**< 001: Timer 1 CC2 event   */
    T1_CC3    = 2,  /**< 010: Timer 1 CC3 event   */
    T2_CC2    = 3,  /**< 011: Timer 2 CC2 event   */
    T3_TRGO   = 4,  /**< 100: Timer 3 TRGO event  */
    T4_CC4    = 5,  /**< 101: Timer 4 CC4 event   */
    ext_IT11  = 6,  /**< 110: EXTI line11/TIM8_TRGO event (TIM8_TRGO is available only in high-density and XL-density devices) */
    none      = 7   /**< 111: SWSTART             */
#if 0 // TODO: special treatment for ADC3
    // For ADC3, the assigned triggers are:
    T3_CC1    = 0,  /**< 000: Timer 3 CC1 event   */
    T2_CC3    = 1,  /**< 001: Timer 2 CC3 event   */
    T1_CC3    = 2,  /**< 010: Timer 1 CC3 event   */
    T8_CC1    = 3,  /**< 011: Timer 8 CC1 event   */
    T8_TRGO   = 4,  /**< 100: Timer 8 TRGO event  */
    T5_CC1    = 5,  /**< 101: Timer 5 CC1 event   */
    T5_CC3    = 6,  /**< 110: Timer 5 CC3 event   */
    none      = 7   /**< 111: SWSTART             */
#endif
  };

  /** CR2[11] ALIGN: Data alignment. */
  enum class DataAlign : uint32_t {
    right = 0,  /**< 0: Right Alignment  */
    left  = 1   /**< 1: Left Alignment   */
  };

  /**
   * Bit 20 EXTTRIG: External trigger conversion mode for regular channels.
   * This bit is set and cleared by software to enable/disable the external
   * trigger used to start conversion of a regular channel group.
   */
  // TODO

  /**
   * Bits 14:12 JEXTSEL[2:0]: External event select for injected group.
   * These bits select the external event used to trigger the start of
   * conversion of an injected group.
   */
  // TODO


  /**
   * SMPR1[2:0] SMPx: Channel x Sample time selection.
   * These bits are written by software to select the sample time
   * individually for each channel. During sample cycles channel selection
   * bits must remain unchanged.
   */
  enum class SampleTime {
    cycles_1_5   = 0,   /**< 000:   1.5 cycles  */
    cycles_7_5   = 1,   /**< 001:   7.5 cycles  */
    cycles_13_5  = 2,   /**< 010:  13.5 cycles  */
    cycles_28_5  = 3,   /**< 011:  28.5 cycles  */
    cycles_41_5  = 4,   /**< 100:  41.5 cycles  */
    cycles_55_5  = 5,   /**< 101:  55.5 cycles  */
    cycles_71_5  = 6,   /**< 110:  71.5 cycles  */
    cycles_239_5 = 7    /**< 111: 239.5 cycles  */
  };
}


// TODO: nice defaults
template<std::size_t adc_no,
         cAdc::Mode mode,
         cAdc::ScanMode scan_mode,
         cAdc::ContinuousConvMode cont_conv_mode,
         cAdc::ExternalTrigConv ext_trig_conv,
         cAdc::DataAlign data_align,
         unsigned numof_channel // Regular channel sequence length 1<=n<=16 (SQR1[23:20] L)
         >
class Adc
{
// TODO: support ADC3
//  static_assert((adc_no >= 1) && (adc_no <= 3), "Invalid ADC number");
  static_assert((adc_no >= 1) && (adc_no <= 2), "Invalid ADC number");
  static_assert((numof_channel >= 1) && (numof_channel <= 16), "Invalid ADC channel sequence length");

public:
  static constexpr uint32_t apb2enr = (adc_no == 1 ? Core::RCC::APB2ENR::ADC1EN::value : 
#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL)
                                   adc_no == 2 ? Core::RCC::APB2ENR::ADC2EN::value : 
#endif
#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
                                   adc_no == 3 ? Core::RCC::APB2ENR::ADC3EN::value : 
#endif
                                   0);

  typedef SharedAPB2ENR< apb2enr > resources;

  using ADCx = Core::ADC<adc_no>;

  static void init(void) {
    // ADCx CR1 config
    auto cr1 = ADCx::CR1::load();
    cr1 &= ~(ADCx::CR1::DUALMOD::value | ADCx::CR1::SCAN::value);
    cr1 |= (uint32_t)mode      << 16;
    cr1 |= (uint32_t)scan_mode <<  8;
    ADCx::CR1::store(cr1);

    // ADCx CR2 config
    auto cr2 = ADCx::CR2::load();
    cr2 &= ~(ADCx::CR2::CONT::value | ADCx::CR2::ALIGN::value | ADCx::CR2::EXTSEL::value);
    cr2 |= (uint32_t)data_align     << 11;
    cr2 |= (uint32_t)ext_trig_conv  << 17;
    cr2 |= (uint32_t)cont_conv_mode <<  1;
    ADCx::CR2::store(cr2);

    // ADCx SQR1 config
    auto sqr1 = ADCx::SQR1::load();
    sqr1 &= ~(ADCx::SQR1::L::value);
    sqr1 |= ((uint32_t)numof_channel - 1) << 20; // set L
    ADCx::SQR1::store(sqr1);
  }

  static void deinit(void) {
    switch(adc_no) {
    case 1:
      // Enable ADC1 reset state
      Core::RCC::APB2RSTR::ADC1RST::set();
      // Release ADC1 from reset state
      Core::RCC::APB2RSTR::ADC1RST::clear();
      break;
    case 2:
      // Enable ADC2 reset state
      Core::RCC::APB2RSTR::ADC2RST::set();
      // Release ADC2 from reset state
      Core::RCC::APB2RSTR::ADC2RST::clear();
    }
  }

  static void enable(void) {
    ADCx::CR2::ADON::set();
  }
  static void disable(void) {
    ADCx::CR2::ADON::clear();
  }

  static void get_channel(unsigned channel) {
    
  }

  // TODO: injected, AnalogWatchdogSingle
  template<unsigned channel, unsigned rank, cAdc::SampleTime sample_time>
  static void regular_channel_config(void) {

    static_assert((channel >= 0) && (channel <= 17), "Invalid channel");
    static_assert((rank >= 1) && (rank <= 16), "Invalid rank");

    if(channel > 9) {
      unsigned shift = 3 * (channel - 10);
      auto smpr1 = ADCx::SMPR1::load();
      smpr1 &= ~((uint32_t)0x7 << shift);
      smpr1 |= (uint32_t)sample_time << shift;
      ADCx::SMPR1::store(smpr1);
    }
    else {
      unsigned shift = 3 * channel;
      auto smpr2 = ADCx::SMPR2::load();
      smpr2 &= ~((uint32_t)0x7 << shift);
      smpr2 |= (uint32_t)sample_time << shift;
      ADCx::SMPR2::store(smpr2);
    }

    if(rank < 7) {
      unsigned shift = 5 * (rank - 1);
      auto sqr3 = ADCx::SQR3::load();
      sqr3 &= ~((uint32_t)0x1F << shift);
      sqr3 |= (uint32_t)channel << shift;
      ADCx::SQR3::store(sqr3);
    }
    else if(rank < 13) {
      unsigned shift = 5 * (rank - 7);
      auto sqr2 = ADCx::SQR2::load();
      sqr2 &= ~((uint32_t)0x1F << shift);
      sqr2 |= (uint32_t)channel << shift;
      ADCx::SQR2::store(sqr2);
    }
    else {
      unsigned shift = 5 * (rank - 13);
      auto sqr1 = ADCx::SQR1::load();
      sqr1 &= ~((uint32_t)0x1F << shift);
      sqr1 |= (uint32_t)channel << shift;
      ADCx::SQR1::store(sqr1);
    }
  }

  static void enable_software_start_conversion(void) {
    // - Enable ADC conversion on external event
    // - Start ADC conversion
    ADCx::CR2::set(ADCx::CR2::EXTTRIG::value | ADCx::CR2::SWSTART::value);
  }
  static void disable_software_start_conversion(void) {
    // - Stop ADC conversion
    // - Disable ADC conversion on external event
    ADCx::CR2::clear(ADCx::CR2::EXTTRIG::value | ADCx::CR2::SWSTART::value);
  }

  static void wait_EOC(void) {
    while(ADCx::SR::EOC::test() == 0);
  }

  static uint16_t get_conversion_value(void) {
    return ADCx::DR::load();
  }

};

#endif // ADC_HPP_INCLUDED


