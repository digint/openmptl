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

#include <arch/rcc.hpp>
#include <arch/reg/adc.hpp>

/**
 * CR1[19:16] DUALMOD: Dual mode selection.
 * These bits are written by software to select the operating mode.
 */
enum class AdcMode : uint32_t {
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
enum class AdcScanMode : uint32_t {
  disabled = 0,  /**< 0: Scan mode disabled  */
  enabled = 1    /**< 1: Scan mode enabled   */
};

/**
 * CR2[1] CONT: Continuous conversion.
 * This bit is set and cleared by software. If set conversion takes place
 * continuously till this bit is reset.
 */
enum class AdcContinuousConvMode : uint32_t {
  single     = 0,  /**< 0: Single conversion mode      */
  continuous = 1   /**< 1: Continuous conversion mode  */
};

/**
 * CR2[19:17] EXTSEL: External event select for regular group.
 * These bits select the external event used to trigger the start of
 * conversion of a regular group:
 */
enum class AdcExternalTrigConv : uint32_t {
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
enum class AdcDataAlign : uint32_t {
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
enum class AdcSampleTime {
  cycles_1_5   = 0,   /**< 000:   1.5 cycles  */
  cycles_7_5   = 1,   /**< 001:   7.5 cycles  */
  cycles_13_5  = 2,   /**< 010:  13.5 cycles  */
  cycles_28_5  = 3,   /**< 011:  28.5 cycles  */
  cycles_41_5  = 4,   /**< 100:  41.5 cycles  */
  cycles_55_5  = 5,   /**< 101:  55.5 cycles  */
  cycles_71_5  = 6,   /**< 110:  71.5 cycles  */
  cycles_239_5 = 7    /**< 111: 239.5 cycles  */
};


// TODO: nice defaults
template<std::size_t adc_no,
         AdcMode mode,
         AdcScanMode scan_mode,
         AdcContinuousConvMode cont_conv_mode,
         AdcExternalTrigConv ext_trig_conv,
         AdcDataAlign data_align,
         unsigned numof_channel // Regular channel sequence length 1<=n<=16 (SQR1[23:20] L)
         >
class Adc
{
// TODO: support ADC3
//  static_assert((adc_no >= 1) && (adc_no <= 3), "Invalid ADC number");
  static_assert((adc_no >= 1) && (adc_no <= 2), "Invalid ADC number");
  static_assert((numof_channel >= 1) && (numof_channel <= 16), "Invalid ADC channel sequence length");

public:
  typedef Rcc::adc_clock_resources<adc_no> resources;

  using ADCx = reg::ADC<adc_no>;

  static void init(void) {
    // ADCx CR1 config
    ADCx::CR1::template set<typename ADCx::CR1::DUALMOD,
                            typename ADCx::CR1::SCAN>
      (ADCx::CR1::DUALMOD::shifted_value((uint32_t)mode) |
       ADCx::CR1::SCAN::   shifted_value((uint32_t)scan_mode));

    // ADCx CR2 config
    ADCx::CR2::template set<typename ADCx::CR2::CONT,
                            typename ADCx::CR2::ALIGN,
                            typename ADCx::CR2::EXTSEL>
      (ADCx::CR2::ALIGN:: shifted_value((uint32_t)data_align) |
       ADCx::CR2::EXTSEL::shifted_value((uint32_t)ext_trig_conv) |
       ADCx::CR2::CONT::  shifted_value((uint32_t)cont_conv_mode));

    // ADCx SQR1 config
    ADCx::SQR1::L::shift_and_set((uint32_t)numof_channel - 1);
  }

  static void deinit(void) {
    switch(adc_no) {
    case 1:
      // Enable ADC1 reset state
      reg::RCC::APB2RSTR::ADC1RST::set();
      // Release ADC1 from reset state
      reg::RCC::APB2RSTR::ADC1RST::clear();
      break;
    case 2:
      // Enable ADC2 reset state
      reg::RCC::APB2RSTR::ADC2RST::set();
      // Release ADC2 from reset state
      reg::RCC::APB2RSTR::ADC2RST::clear();
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
  template<unsigned channel, unsigned rank, AdcSampleTime sample_time>
  static void regular_channel_config(void) {

    static_assert((channel >= 0) && (channel <= 17), "Invalid channel");
    static_assert((rank >= 1) && (rank <= 16), "Invalid rank");

    if(channel > 9) {
      unsigned shift = 3 * (channel - 10);
      ADCx::SMPR1::set((uint32_t)sample_time << shift,   /* set_mask */
                       (uint32_t)0x7 << shift);          /* clear_mask */
    }
    else {
      unsigned shift = 3 * channel;
      ADCx::SMPR2::set((uint32_t)sample_time << shift,   /* set_mask */
                       (uint32_t)0x7 << shift);          /* clear_mask */
    }

    if(rank < 7) {
      unsigned shift = 5 * (rank - 1);
      ADCx::SQR3::set((uint32_t)channel << shift,   /* set_mask */
                      (uint32_t)0x1F << shift);     /* clear_mask */
    }
    else if(rank < 13) {
      unsigned shift = 5 * (rank - 7);
      ADCx::SQR2::set((uint32_t)channel << shift,   /* set_mask */
                      (uint32_t)0x1F << shift);     /* clear_mask */
    }
    else {
      unsigned shift = 5 * (rank - 13);
      ADCx::SQR1::set((uint32_t)channel << shift,   /* set_mask */
                      (uint32_t)0x1F << shift);     /* clear_mask */
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

  static void wait_eoc(void) {
    while(ADCx::SR::EOC::test() == 0);
  }

  static uint16_t get_conversion_value(void) {
    return ADCx::DR::load();
  }

};

#endif // ADC_HPP_INCLUDED


