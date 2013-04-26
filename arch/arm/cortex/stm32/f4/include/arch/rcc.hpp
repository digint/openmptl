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

#include <arch/reg/rcc.hpp>
#include <resource.hpp>
#include <freq.hpp>

template<freq_t cpu_clock_freq = 168_mhz>
class Rcc {
  static_assert(cpu_clock_freq == 168_mhz || cpu_clock_freq == 120_mhz, "unsupported cpu clock frequency");

  using RCC = reg::RCC;

  template<int m, int n, int p, int q>
  struct pllcfgr_hse {
    static_assert(m >= 2 && m <= 63,                    "invalid PLLM value");
    static_assert(n >= 2 && n <= 432,                   "invalid PLLN value");
    static_assert(p == 2 || p == 4 || p == 6 || p == 8, "invalid PLLP value");
    static_assert(q >= 2 && q <= 15,                    "invalid PLLQ value");

    static constexpr RCC::PLLCFGR::value_type value =
      RCC::PLLCFGR::PLLM::value_from(m)           |
      RCC::PLLCFGR::PLLN::value_from(n)           |
      RCC::PLLCFGR::PLLP::value_from((p >> 1) -1) |
      RCC::PLLCFGR::PLLQ::value_from(q)           |
      RCC::PLLCFGR::PLLSRC::value;  // HSE
  };

public:

  //  static constexpr freq_t cpu_clock_freq = _clock_freq;

  /* Note: this is only valid for clocks setup by set_system_clock() function */
  static constexpr freq_t hclk_freq  = cpu_clock_freq;
  static constexpr freq_t pclk1_freq = ( hclk_freq == 120_mhz ? 30_mhz :
                                         hclk_freq == 168_mhz ? 42_mhz :
                                         0 );
  static constexpr freq_t pclk2_freq = ( hclk_freq == 120_mhz ? 60_mhz :
                                         hclk_freq == 168_mhz ? 84_mhz :
                                         0 );

  static void enable_hse(void) {
    RCC::CR::HSEON::set();
  }
  static void wait_hse_ready() {
    while(RCC::CR::HSERDY::test() == false);
  }
  static bool wait_hse_ready(unsigned timeout) {
    while((RCC::CR::HSERDY::test() == false) && timeout) {
      timeout--;
    }
    return timeout;
  }
  static void enable_hsi(void) {
    RCC::CR::HSION::set();
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

  static void set_system_clock(void) {
    /* reset CFGR, and set HPRE, PPRE1, PPRE2 */
    RCC::CFGR::reset_to<RCC::CFGR::HPRE ::DIV1,
                        RCC::CFGR::PPRE1::DIV4,
                        RCC::CFGR::PPRE2::DIV2>();

    switch(hclk_freq) {
    case 120_mhz:
      RCC::PLLCFGR::store(pllcfgr_hse<8, 240, 2, 5>::value);
      break;
    case 168_mhz:
      RCC::PLLCFGR::store(pllcfgr_hse<8, 336, 2, 7>::value);
      break;
    }

    RCC::CR::PLLON::set();
    while(RCC::CR::PLLRDY::test() == false);
    RCC::CFGR::SW::PLL::set();
    while(RCC::CFGR::SWS::PLL::test() == false);
  }

  static void init(void) {
    enable_hse();
    wait_hse_ready();
  }
};

/* Clock resource declarations (enable peripheral clocks) */
template<char>     struct Rcc_gpio_clock_resources;
template<unsigned> struct Rcc_usart_clock_resources;

/*
 * Clock resource specialisation (enable peripheral clocks)
 */
template<> struct Rcc_gpio_clock_resources<'A'> : SharedRegister<reg::RCC::AHB1ENR::GPIOAEN> { };
template<> struct Rcc_gpio_clock_resources<'B'> : SharedRegister<reg::RCC::AHB1ENR::GPIOBEN> { };
template<> struct Rcc_gpio_clock_resources<'C'> : SharedRegister<reg::RCC::AHB1ENR::GPIOCEN> { };
template<> struct Rcc_gpio_clock_resources<'D'> : SharedRegister<reg::RCC::AHB1ENR::GPIODEN> { };
template<> struct Rcc_gpio_clock_resources<'E'> : SharedRegister<reg::RCC::AHB1ENR::GPIOEEN> { };
template<> struct Rcc_gpio_clock_resources<'F'> : SharedRegister<reg::RCC::AHB1ENR::GPIOFEN> { };
template<> struct Rcc_gpio_clock_resources<'G'> : SharedRegister<reg::RCC::AHB1ENR::GPIOGEN> { };
template<> struct Rcc_gpio_clock_resources<'H'> : SharedRegister<reg::RCC::AHB1ENR::GPIOHEN> { };
template<> struct Rcc_gpio_clock_resources<'I'> : SharedRegister<reg::RCC::AHB1ENR::GPIOIEN> { };

template<> struct Rcc_usart_clock_resources<1> : SharedRegister<reg::RCC::APB2ENR::USART1EN> { };
template<> struct Rcc_usart_clock_resources<2> : SharedRegister<reg::RCC::APB1ENR::USART2EN> { };
template<> struct Rcc_usart_clock_resources<3> : SharedRegister<reg::RCC::APB1ENR::USART3EN> { };
template<> struct Rcc_usart_clock_resources<6> : SharedRegister<reg::RCC::APB2ENR::USART6EN> { };

#endif // RCC_HPP_INCLUDED
