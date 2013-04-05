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

template<freq_t cpu_clock_freq = 72_mhz>
class Rcc {
  static_assert(cpu_clock_freq == 24_mhz ||
                cpu_clock_freq == 36_mhz ||
                cpu_clock_freq == 48_mhz ||
                cpu_clock_freq == 56_mhz ||
                cpu_clock_freq == 72_mhz,
                "unsupported system clock frequency");

  using RCC = reg::RCC;

public:

  /* Note: this is only valid for clocks setup by set_system_clock() function */
  static constexpr freq_t hclk_freq  = cpu_clock_freq;
  static constexpr freq_t pclk1_freq = cpu_clock_freq <= 36_mhz ? cpu_clock_freq : cpu_clock_freq / 2;
  static constexpr freq_t pclk2_freq = cpu_clock_freq;

  static void enable_hse(void) {
    RCC::CR::HSEON::set();
  }

  static void wait_hse_ready(void) {
    while(RCC::CR::HSERDY::test() == false);
  }
  static bool wait_hse_ready(unsigned timeout) {
    while((RCC::CR::HSERDY::test() == false) && timeout) {
      timeout--;
    }
    return timeout;
  }

  static void set_system_clock(void) {
    switch(cpu_clock_freq) {
    case 24_mhz:
      /* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK    */
      /* PLLCLK = 8MHz / 2 * 6 = 24 MHz               */
      RCC::CFGR::set< RCC::CFGR::HPRE    ::DIV1,
                      RCC::CFGR::PPRE1   ::DIV1,
                      RCC::CFGR::PPRE2   ::DIV1,
                      RCC::CFGR::PLLSRC  ::HSE,
                      RCC::CFGR::PLLXTPRE::HSE_DIV2,
                      RCC::CFGR::PLLMUL  ::MUL6
                      >();
      break;
    case 36_mhz:
      /* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK    */
      /* PLLCLK = 8MHz / 2 * 9 = 36 MHz               */
      RCC::CFGR::set< RCC::CFGR::HPRE    ::DIV1,
                      RCC::CFGR::PPRE1   ::DIV1,
                      RCC::CFGR::PPRE2   ::DIV1,
                      RCC::CFGR::PLLSRC  ::HSE,
                      RCC::CFGR::PLLXTPRE::HSE_DIV2,
                      RCC::CFGR::PLLMUL  ::MUL9
                      >();
      break;
    case 48_mhz:
      /* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK/2  */
      /* PLLCLK = 8MHz * 6 = 48 MHz                   */
      RCC::CFGR::set< RCC::CFGR::HPRE    ::DIV1,
                      RCC::CFGR::PPRE1   ::DIV2,
                      RCC::CFGR::PPRE2   ::DIV1,
                      RCC::CFGR::PLLSRC  ::HSE,
                      RCC::CFGR::PLLXTPRE::HSE_DIV1,
                      RCC::CFGR::PLLMUL  ::MUL6
                      >();
      break;
    case 56_mhz:
      /* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK/2  */
      /* PLLCLK = 8MHz * 7 = 56 MHz                   */
      RCC::CFGR::set< RCC::CFGR::HPRE    ::DIV1,
                      RCC::CFGR::PPRE1   ::DIV2,
                      RCC::CFGR::PPRE2   ::DIV1,
                      RCC::CFGR::PLLSRC  ::HSE,
                      RCC::CFGR::PLLXTPRE::HSE_DIV1,
                      RCC::CFGR::PLLMUL  ::MUL7
                      >();
      break;
    case 72_mhz:
      /* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK/2  */
      /* PLLCLK = 8MHz * 9 = 72 MHz                   */
      RCC::CFGR::set<RCC::CFGR::HPRE    ::DIV1,
                     RCC::CFGR::PPRE1   ::DIV2,
                     RCC::CFGR::PPRE2   ::DIV1,
                     RCC::CFGR::PLLSRC  ::HSE,
                     RCC::CFGR::PLLXTPRE::HSE_DIV1,
                     RCC::CFGR::PLLMUL  ::MUL9
                     >();
      break;
    }

    RCC::CR::PLLON::set();
    while(RCC::CR::PLLRDY::test() == 0);
    RCC::CFGR::SW::PLL::set();
    while(RCC::CFGR::SWS::PLL::test() == false);
  }

  static void init(void) {
    enable_hse();
    wait_hse_ready();
  }
};

/*
 * Clock resource declaration (enable peripheral clocks)
 */
template<char>     struct Rcc_gpio_clock_resources;
template<unsigned> struct Rcc_spi_clock_resources;
template<unsigned> struct Rcc_usart_clock_resources;
template<unsigned> struct Rcc_adc_clock_resources;

using Rcc_rtc_clock_resources = SharedRegister<
  reg::RCC::APB1ENR,
  ( reg::RCC::APB1ENR::PWREN::value |
    reg::RCC::APB1ENR::BKPEN::value )
  >;

/*
 * Clock resource specialisation (enable peripheral clocks)
 */
template<> struct Rcc_gpio_clock_resources<'A'> : SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::IOPAEN::value> { };
template<> struct Rcc_gpio_clock_resources<'B'> : SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::IOPBEN::value> { };
template<> struct Rcc_gpio_clock_resources<'C'> : SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::IOPCEN::value> { };
template<> struct Rcc_gpio_clock_resources<'D'> : SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::IOPDEN::value> { };
template<> struct Rcc_gpio_clock_resources<'E'> : SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::IOPEEN::value> { };
#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
template<> struct Rcc_gpio_clock_resources<'F'> : SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::IOPFEN::value> { };
template<> struct Rcc_gpio_clock_resources<'G'> : SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::IOPGEN::value> { };
#endif

template<> struct Rcc_spi_clock_resources<1> : SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::SPI1EN::value> { };
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
template<> struct Rcc_spi_clock_resources<2> : SharedRegister<reg::RCC::APB1ENR, reg::RCC::APB1ENR::SPI2EN::value> { };
#endif
#if defined (STM32F10X_HD) || defined (STM32F10X_CL)
template<> struct Rcc_spi_clock_resources<3> : SharedRegister<reg::RCC::APB1ENR, reg::RCC::APB1ENR::SPI3EN::value> { };
#endif

template<> struct Rcc_usart_clock_resources<1> : SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::USART1EN::value> { };
template<> struct Rcc_usart_clock_resources<2> : SharedRegister<reg::RCC::APB1ENR, reg::RCC::APB1ENR::USART2EN::value> { };
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
template<> struct Rcc_usart_clock_resources<3> : SharedRegister<reg::RCC::APB1ENR, reg::RCC::APB1ENR::USART3EN::value> { };
#endif

template<> struct Rcc_adc_clock_resources<1> : SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::ADC1EN::value> { };
#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL)
template<> struct Rcc_adc_clock_resources<2> : SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::ADC2EN::value> { };
#endif
#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
template<> struct Rcc_adc_clock_resources<1> : SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::ADC3EN::value> { };
#endif


#endif // RCC_HPP_INCLUDED
