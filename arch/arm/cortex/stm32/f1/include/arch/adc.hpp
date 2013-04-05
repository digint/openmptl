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

#ifndef ADC_HPP_INCLUDED
#define ADC_HPP_INCLUDED

#include <arch/rcc.hpp>
#include <arch/reg/adc.hpp>
#include <type_traits>

/** Dual mode selection. */
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
 * In Scan mode, the inputs selected through the ADC_SQRx or
 * ADC_JSQRx registers are converted.
 */
enum class AdcScanMode : uint32_t {
  disabled = 0,  /**< 0: Scan mode disabled  */
  enabled = 1    /**< 1: Scan mode enabled   */
};

/**
 * Continuous conversion.
 * If set conversion takes place continuously.
 */
enum class AdcContinuousConvMode : uint32_t {
  single     = 0,  /**< 0: Single conversion mode      */
  continuous = 1   /**< 1: Continuous conversion mode  */
};

/**
 * External event select for regular group.
 * Select the external event used to trigger the start of
 * conversion of a regular group:
 */
namespace AdcExternalTrigConv
{
  /* SWSTART */
  struct SoftwareStart
  {
    template<unsigned adc_no>
    struct extsel : std::integral_constant<uint32_t, 7>
    { };
  };

  /* CC */
  template<unsigned timer, unsigned capture_compare>
  struct CaptureCompare
  {
    template<unsigned adc_no>
    struct extsel {
      static constexpr uint32_t value =
        (adc_no  < 3) && (timer == 1) && (capture_compare == 1) ? 0 :
        (adc_no  < 3) && (timer == 1) && (capture_compare == 2) ? 1 :
        (adc_no  < 3) && (timer == 1) && (capture_compare == 3) ? 2 :
        (adc_no  < 3) && (timer == 2) && (capture_compare == 1) ? 3 :
        (adc_no  < 3) && (timer == 4) && (capture_compare == 4) ? 6 :
        (adc_no == 3) && (timer == 3) && (capture_compare == 1) ? 0 :
        (adc_no == 3) && (timer == 2) && (capture_compare == 3) ? 1 :
        (adc_no == 3) && (timer == 1) && (capture_compare == 3) ? 2 :
        (adc_no == 3) && (timer == 8) && (capture_compare == 1) ? 3 :
        (adc_no == 3) && (timer == 5) && (capture_compare == 1) ? 5 :
        (adc_no == 3) && (timer == 5) && (capture_compare == 3) ? 6 :
        0xff;
      static_assert((value & 0x7) == value, "invalid timer / capture_compare combination");
    };
  };

  /* TRGO */
  template<unsigned timer>
  struct TriggerOutput
  {
    template<unsigned adc_no>
    struct extsel {
      static constexpr uint32_t value =
        (adc_no  < 3) && (timer == 3) ? 4 :
        (adc_no == 3) && (timer == 8) ? 4 :
#if defined (STM32F10X_XL)
        (adc_no  < 3) && (timer == 8) ? 6 :
#endif
        0xff;
      static_assert((value & 0x7) == value, "invalid timer / capture_compare combination");
    };
  };

  struct EXTI_line11
  {
    template<unsigned adc_no>
    struct extsel : std::integral_constant<uint32_t, 6>
    {
      static_assert(adc_no < 3, "invalid ADC number for EXTI_line11 external trigger conversion");
    };
  };
}

/** Data alignment */
enum class AdcDataAlign : uint32_t {
  right = 0,  /**< 0: Right Alignment  */
  left  = 1   /**< 1: Left Alignment   */
};

/**
 * External trigger conversion mode for regular channels.
 */
// TODO

/**
 * External event select for injected group.
 * Select the external event used to trigger the start of
 * conversion of an injected group.
 */
// TODO


/**
 * Channel x Sample time selection.
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

/**< Regular channel sequence length 1<=n<=16 (SQR1[23:20] L) */
template<unsigned len>
struct AdcRegularChannelSequenceLength
: std::integral_constant<uint32_t, len - 1>
{
  static_assert((len >= 1) && (len <= 16), "invalid ADC channel sequence length");
};


template<unsigned              adc_no,
         AdcMode               mode           = AdcMode::independent,
         AdcScanMode           scan_mode      = AdcScanMode::disabled,
         AdcContinuousConvMode cont_conv_mode = AdcContinuousConvMode::single,
         typename              ext_trig_conv  = AdcExternalTrigConv::CaptureCompare<1, 1>,
         AdcDataAlign          data_align     = AdcDataAlign::right,
         typename              chan_seq_len   = AdcRegularChannelSequenceLength<1>
         >
class Adc
{
  static_assert((adc_no >= 1) && (adc_no <= 3), "invalid ADC number");
  static_assert((adc_no > 1) || (mode == AdcMode::independent), "dual mode is only available for ADC1");

  using ADCx = reg::ADC<adc_no>;

public:
  typedef Rcc_adc_clock_resources<adc_no> resources;

  static void configure(void) {
    /* ADCx CR1 config */
    auto cr1 = ADCx::CR1::load();
    if(adc_no == 1)
      cr1 &= ~ADCx::CR1::DUALMOD::value;
    cr1 &= ~ADCx::CR1::SCAN::value;

    if(adc_no == 1)
      cr1 |= ADCx::CR1::DUALMOD::shifted_value((uint32_t)mode);
    cr1 |= ADCx::CR1::SCAN::shifted_value((uint32_t)scan_mode);
    ADCx::CR1::store(cr1);

    /* ADCx CR2 config */
    ADCx::CR2::template set<typename ADCx::CR2::CONT,
                            typename ADCx::CR2::ALIGN,
                            typename ADCx::CR2::EXTSEL>
      (ADCx::CR2::ALIGN:: shifted_value((uint32_t)data_align) |
       ADCx::CR2::EXTSEL::shifted_value((uint32_t)ext_trig_conv::template extsel<adc_no>::value) |
       ADCx::CR2::CONT::  shifted_value((uint32_t)cont_conv_mode));

    /* ADCx SQR1 config */
    ADCx::SQR1::L::shift_and_set(chan_seq_len::value);
  }

  static void reset(void) {
    switch(adc_no) {
    case 1:
      reg::RCC::APB2RSTR::ADC1RST::set();
      reg::RCC::APB2RSTR::ADC1RST::clear();
      break;
    case 2:
      reg::RCC::APB2RSTR::ADC2RST::set();
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

    static_assert(channel <= 17, "invalid channel");
    static_assert((rank >= 1) && (rank <= 16), "invalid rank");

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
    ADCx::CR2::template set<typename ADCx::CR2::EXTTRIG,
                            typename ADCx::CR2::SWSTART>();
  }
  static void disable_software_start_conversion(void) {
    // - Stop ADC conversion
    // - Disable ADC conversion on external event
    ADCx::CR2::template clear<typename ADCx::CR2::EXTTRIG,
                              typename ADCx::CR2::SWSTART>();
  }

  static void wait_eoc(void) {
    while(ADCx::SR::EOC::test() == 0);
  }

  static uint16_t get_conversion_value(void) {
    return ADCx::DR::load();
  }

};

#endif // ADC_HPP_INCLUDED


