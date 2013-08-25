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

namespace mptl {


template<unsigned adc_no>
class adc
{
  static_assert((adc_no >= 1) && (adc_no <= 3), "invalid ADC number");

  using ADCx = reg::ADC< adc_no >;

public:
  using resources = rcc_adc_clock_resources< adc_no >;

  /** Sample time, for regular_channel_config<>  */
  enum class sample_time {
    cycles_1_5   = 0,   /**< 000:   1.5 cycles  */
    cycles_7_5   = 1,   /**< 001:   7.5 cycles  */
    cycles_13_5  = 2,   /**< 010:  13.5 cycles  */
    cycles_28_5  = 3,   /**< 011:  28.5 cycles  */
    cycles_41_5  = 4,   /**< 100:  41.5 cycles  */
    cycles_55_5  = 5,   /**< 101:  55.5 cycles  */
    cycles_71_5  = 6,   /**< 110:  71.5 cycles  */
    cycles_239_5 = 7    /**< 111: 239.5 cycles  */
  };

private:

  /** CR1::DUALMOD is only valid for ADC1 */
  template< unsigned value >
  using dualmod_impl =
    typename std::enable_if<
      (adc_no == 1) || (value == 0),
      regval< typename ADCx::CR1::DUALMOD, value >
    >::type;

  template< unsigned timer, unsigned cc >
  struct capture_compare_impl {
    static constexpr uint32_t value =
      (adc_no  < 3) && (timer == 1) && (cc == 1) ? 0 :
      (adc_no  < 3) && (timer == 1) && (cc == 2) ? 1 :
      (adc_no  < 3) && (timer == 1) && (cc == 3) ? 2 :
      (adc_no  < 3) && (timer == 2) && (cc == 1) ? 3 :
      (adc_no  < 3) && (timer == 4) && (cc == 4) ? 6 :
      (adc_no == 3) && (timer == 3) && (cc == 1) ? 0 :
      (adc_no == 3) && (timer == 2) && (cc == 3) ? 1 :
      (adc_no == 3) && (timer == 1) && (cc == 3) ? 2 :
      (adc_no == 3) && (timer == 8) && (cc == 1) ? 3 :
      (adc_no == 3) && (timer == 5) && (cc == 1) ? 5 :
      (adc_no == 3) && (timer == 5) && (cc == 3) ? 6 :
      0xff;
    static_assert((value & 0x7) == value, "invalid timer / cc combination");
    using type = regval< typename ADCx::CR2::EXTSEL, value >;
  };

  template<unsigned timer>
  struct trigger_output_impl {
    static constexpr uint32_t value =
      (adc_no  < 3) && (timer == 3) ? 4 :
      (adc_no == 3) && (timer == 8) ? 4 :
#if defined (STM32F10X_XL)
      (adc_no  < 3) && (timer == 8) ? 6 :
#endif
      0xff;
    static_assert((value & 0x7) == value, "invalid timer");
    using type = regval< typename ADCx::CR2::EXTSEL, value >;
  };

  template<unsigned channel, unsigned rank, sample_time _sample_time>
  struct regular_channel_config_impl {
    static constexpr unsigned smp_value = (unsigned)_sample_time;

    static_assert(channel <= 17, "invalid channel");
    static_assert((rank >= 1) && (rank <= 16), "invalid rank");
    static_assert(smp_value < 8, "invalid sample_time");

    using type = reglist<
      regval< typename ADCx::template SMPRx<channel>::SMP, smp_value >,
      regval< typename ADCx::template SQRx<rank>::SQ, channel >
      >;
  };

public:  /* ------ configuration traits ------ */

  /** Dual mode selection. */
  struct dual_mode
  {
    using independent                            = typename dualmod_impl< 0 >::type;  /**<  0000: Independent mode.                                           */
    using regular_simultaneous_injected          = typename dualmod_impl< 1 >::type;  /**<  0001: Combined regular simultaneous + injected simultaneous mode  */
    using regular_simultaneous_alternate_trigger = typename dualmod_impl< 2 >::type;  /**<  0010: Combined regular simultaneous + alternate trigger mode      */
    using injected_simultaneous_fast_interleaved = typename dualmod_impl< 3 >::type;  /**<  0011: Combined injected simultaneous + fast interleaved mode      */
    using injected_simultaneous_slow_interleaved = typename dualmod_impl< 4 >::type;  /**<  0100: Combined injected simultaneous + slow Interleaved mode      */
    using injected_simultaneous                  = typename dualmod_impl< 5 >::type;  /**<  0101: Injected simultaneous mode only                             */
    using regular_simultaneous                   = typename dualmod_impl< 6 >::type;  /**<  0110: Regular simultaneous mode only                              */
    using fast_interleaved                       = typename dualmod_impl< 7 >::type;  /**<  0111: Fast interleaved mode only                                  */
    using slow_interleaved                       = typename dualmod_impl< 8 >::type;  /**<  1000: Slow interleaved mode only                                  */
    using alternate_trigger                      = typename dualmod_impl< 9 >::type;  /**<  1001: Alternate trigger mode only                                 */
  };

  /**
   * In Scan mode, the inputs selected through the ADC_SQRx or
   * ADC_JSQRx registers are converted.
   */
  using scan_mode = regval< typename ADCx::CR1::SCAN, 1 >;

  /**
   * Continuous conversion.
   * If set conversion takes place continuously.
   */
  using continuous_conversion_mode = regval< typename ADCx::CR2::CONT, 1 >;

  /**
   * External event select for regular group.
   * Select the external event used to trigger the start of
   * conversion of a regular group:
   */
  struct external_trigger_conversion
  {
    /** SWSTART */
    using software_start = regval< typename ADCx::CR2::EXTSEL, 7 >;

    /** CCx */
    template<unsigned timer, unsigned cc>
    using capture_compare = typename capture_compare_impl< timer, cc >::type;

    /* TRGO */
    template<unsigned timer>
    using trigger_output = typename trigger_output_impl< timer >::type;

    using exti_line11 =
      typename std::enable_if<
        (adc_no < 3),
        regval< typename ADCx::CR2::EXTSEL, 6 >
      >::type;
  };

  /** Data alignment */
  struct data_align
  {
    using right = regval< typename ADCx::CR2::ALIGN, 0 >;
    using left  = regval< typename ADCx::CR2::ALIGN, 1 >;
  };

  /** Regular channel sequence length 1<=n<=16 (SQR1[23:20] L) */
  template<unsigned value>
  using regular_channel_sequence_length =
    typename std::enable_if<
      (value >= 1) && (value <= 16),
      regval< typename ADCx::SQR1::L, value - 1 >
    >::type;

  /** Regular channel configuration */
  template<unsigned channel, unsigned rank, sample_time _sample_time>
  using regular_channel_config = typename regular_channel_config_impl< channel, rank, _sample_time >::type;


public:  /* ------ static member functions ------ */

  /**
   * Configure ADC register using Tp type traits.
   *
   * NOTE: make sure no communication is ongoing when calling this function.
   */
  template< typename... Tp >
  static void configure(void) {
    SIM_DEBUG("adc::configure()");
    reglist< Tp... >::template strict_reset_to<
      typename ADCx::CR1,
      typename ADCx::CR2,
      typename ADCx::SMPR1,
      typename ADCx::SMPR2,
      typename ADCx::SQR1,
      typename ADCx::SQR2,
      typename ADCx::SQR3
      >();
    SIM_DEBUG("~adc::configure()");
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

  /**
   * - Enable ADC conversion on external event
   * - Start ADC conversion
   */
  static void enable_software_start_conversion(void) {
    ADCx::CR2::template set<
      typename ADCx::CR2::EXTTRIG,
      typename ADCx::CR2::SWSTART
      >();
  }

  /**
   * - Stop ADC conversion
   * - Disable ADC conversion on external event
   */
  static void disable_software_start_conversion(void) {
    ADCx::CR2::template clear<
      typename ADCx::CR2::EXTTRIG,
      typename ADCx::CR2::SWSTART
      >();
  }

  static void wait_eoc(void) {
    while(ADCx::SR::EOC::test() == false);
  }

  static uint16_t get_conversion_value(void) {
    return ADCx::DR::load();
  }

};

} // namespace mptl

#endif // ADC_HPP_INCLUDED
