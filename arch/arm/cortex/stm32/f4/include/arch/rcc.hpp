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

#ifndef RCC_HPP_INCLUDED
#define RCC_HPP_INCLUDED

#include <simulation.hpp>
#include <arch/reg/rcc.hpp>
#include <typelist.hpp>
#include <freq.hpp>

namespace mptl {


////////////////////  rcc  ////////////////////


class rcc
{
public:  /* ------ configuration traits ------ */

  using hse_enable  = regval< RCC::CR::HSEON, 1 >;
  using hse_disable = regval< RCC::CR::HSEON, 0 >;

  using hsi_enable  = regval< RCC::CR::HSION, 1 >;
  using hsi_disable = regval< RCC::CR::HSION, 0 >;

public:  /* ------ static member functions ------ */

  static void wait_hse_ready() {
    while(RCC::CR::HSERDY::test() == false);
  }
  static bool wait_hse_ready(unsigned timeout) {
    while((RCC::CR::HSERDY::test() == false) && timeout) {
      timeout--;
    }
    return timeout;
  }

  static void wait_hsi_ready() {
    while(RCC::CR::HSIRDY::test() == false);
  }
  static bool wait_hsi_ready(unsigned timeout) {
    while((RCC::CR::HSIRDY::test() == false) && timeout) {
      timeout--;
    }
    return timeout;
  }
};


////////////////////  system_clock_hse  ////////////////////


template< typename... Tp >
struct system_clock_hse_base
{
  using cfg_list = reglist<
    Tp...,
    RCC::PLLCFGR::PLLSRC  /* select HSE */
    >;

  static void init(void) {
    rcc::hse_enable::set();
    rcc::wait_hse_ready();
  }

  static void configure(void) {
    cfg_list::template strict_reset_to<
      RCC::CFGR,
      RCC::PLLCFGR
      >();

    RCC::CR::PLLON::set();
    while(RCC::CR::PLLRDY::test() == false);
    RCC::CFGR::SW::PLL::set();
    while(RCC::CFGR::SWS::PLL::test() == false);
  }
};


/**
 * PLL configuration with fixed PLLP=2
 */
template< freq_t output_freq, freq_t hse_freq >
struct system_clock_pllcfgr {
  static constexpr freq_t vco_input = mhz(1); // It is recommended to select a frequency of 2 MHz to limit PLL jitter.

  static constexpr unsigned p = 2;
  static constexpr unsigned m = hse_freq / vco_input;
  static constexpr unsigned n = (output_freq / vco_input ) * p;
  static constexpr unsigned q = n / 48 + (n % 48 ? 1 : 0); // use next higher value if n is not divisible without reminder

  static_assert(p==2 || p==4 || p==6 || p==8,  "illegal PLLP value");
  static_assert(q >= 2   && q <= 15,  "illegal PLLQ value");
  static_assert(n >= 192 && n <= 432, "illegal PLLN value");
  static_assert(m >= 2   && m <= 63,  "illegal PLLM value");

  static constexpr freq_t vco_output = vco_input * n;
  static constexpr freq_t pll_output = vco_output / p;
  static constexpr freq_t pll48clk   = vco_output / q;

  static_assert(vco_input >= mhz(1) && vco_input <= mhz(2), "illegal VCO input frequency");
  static_assert(vco_output >= mhz(192) && vco_output <= mhz(432), "illegal VCO output frequency");
  static_assert(pll_output <= mhz(168), "illegal PLL output frequency");

  /* The USB OTG FS requires a 48 MHz clock to work correctly */
  static constexpr bool usb_otg_available = ( pll48clk == mhz(48) );

  /* The SDIO and the random number generator need a frequency lower
   * than or equal to 48 MHz to work correctly. */
  static constexpr bool sdio_available = ( pll48clk <= mhz(48) );
  static constexpr bool rng_available  = ( pll48clk <= mhz(48) );

  /* assert on above restrictions for now */
  static_assert(usb_otg_available, "USB OTG FS is not available");
  static_assert(sdio_available,    "SDIO is not available");
  static_assert(rng_available,     "RNG is not available");

  using type = reglist <
    regval< RCC::PLLCFGR::PLLM, m >,
    regval< RCC::PLLCFGR::PLLN, n >,
    regval< RCC::PLLCFGR::PLLP, (p >> 1) - 1 >,
    regval< RCC::PLLCFGR::PLLQ, q >
    >;

#ifdef OPENMPTL_SIMULATION
  static void dump() {
    std::cout << std::dec << "--------------------" << std::endl;
    std::cout << "HCLK = " << output_freq / mhz(1) << " MHz" << std::endl;
    std::cout << "HSE  = " << hse_freq / mhz(1) << " MHz" << std::endl;
    std::cout << std::dec << "--------------------" << std::endl;
  
    std::cout << "m=" << m << std::endl;
    std::cout << "n=" << n << std::endl;
    std::cout << "p=" << p << std::endl;
    std::cout << "q=" << q << std::endl;

    std::cout << "vco_input  = " << (vco_input  / mhz(1)) << " MHz" << std::endl;
    std::cout << "vco_output = " << (vco_output / mhz(1)) << " MHz" << std::endl;
    std::cout << "pll_output = " << (pll_output / mhz(1)) << " MHz" << std::endl;
    std::cout << "pll48clk   = " << (pll48clk   / mhz(1)) << " MHz" << std::endl;
  }
#endif
};


/**
 * Generic HSE system clock setup.
 *
 * Provides decent clock setup for
 * output_freq (HCLK): 168MHz, 144MHz, 120Mhz, 96MHz.
 *
 * Template arguments:
 *
 *   - output_freq: desired output (HCLK) frequency
 *   - hse_freq: frequency of the HSE external oscillator
 */
template< freq_t output_freq, freq_t hse_freq = mhz(8) >
struct system_clock_hse
: public system_clock_hse_base<
  RCC::CFGR::HPRE ::DIV1,
  RCC::CFGR::PPRE1::DIV4,
  RCC::CFGR::PPRE2::DIV2,
  typename system_clock_pllcfgr< output_freq, hse_freq >::type
  >
{
  static_assert( output_freq <= mhz(168), "illegal output (HCLK) frequency" );
  static_assert( hse_freq >= mhz(4) && hse_freq <= mhz(26), "illegal HSE frequency" );

  static constexpr freq_t hclk_freq  = output_freq;
  static constexpr freq_t pclk1_freq = hclk_freq / 4;
  static constexpr freq_t pclk2_freq = hclk_freq / 2;
};


////////////////////  peripheral clock traits  ////////////////////


/* Clock resource declarations (enable peripheral clocks) */
template<char>     struct rcc_gpio_clock_resources;
template<unsigned> struct rcc_usart_clock_resources;

/*
 * Clock resource specialisation (enable peripheral clocks)
 */
template<> struct rcc_gpio_clock_resources<'A'> : RCC::AHB1ENR::GPIOAEN { };
template<> struct rcc_gpio_clock_resources<'B'> : RCC::AHB1ENR::GPIOBEN { };
template<> struct rcc_gpio_clock_resources<'C'> : RCC::AHB1ENR::GPIOCEN { };
template<> struct rcc_gpio_clock_resources<'D'> : RCC::AHB1ENR::GPIODEN { };
template<> struct rcc_gpio_clock_resources<'E'> : RCC::AHB1ENR::GPIOEEN { };
template<> struct rcc_gpio_clock_resources<'F'> : RCC::AHB1ENR::GPIOFEN { };
template<> struct rcc_gpio_clock_resources<'G'> : RCC::AHB1ENR::GPIOGEN { };
template<> struct rcc_gpio_clock_resources<'H'> : RCC::AHB1ENR::GPIOHEN { };
template<> struct rcc_gpio_clock_resources<'I'> : RCC::AHB1ENR::GPIOIEN { };

template<> struct rcc_usart_clock_resources<1>  : RCC::APB2ENR::USART1EN { };
template<> struct rcc_usart_clock_resources<2>  : RCC::APB1ENR::USART2EN { };
template<> struct rcc_usart_clock_resources<3>  : RCC::APB1ENR::USART3EN { };
template<> struct rcc_usart_clock_resources<6>  : RCC::APB2ENR::USART6EN { };

} // namespace mptl

#endif // RCC_HPP_INCLUDED
