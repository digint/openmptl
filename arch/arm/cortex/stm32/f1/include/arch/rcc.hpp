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
#include <typelist.hpp>
#include <freq.hpp>
#include <type_traits>

namespace mptl {


////////////////////  rcc  ////////////////////


class rcc
{
public:  /* ------ configuration traits ------ */

  /** Enable external high speed oscillator */
  using hse_enable  = regval< RCC::CR::HSEON, 1 >;

  /** Enable external high speed oscillator */
  using hse_disable = regval< RCC::CR::HSEON, 0 >;

  /** Enable Internal low speed oscillator */
  using lsi_enable  = regval< RCC::CSR::LSION, 1 >;

  /** Disable Internal low speed oscillator */
  using lsi_disable = regval< RCC::CSR::LSION, 0 >;

  /** Enable external low speed oscillator */
  using lse_enable  = regval< RCC::BDCR::LSEON, 1 >;

  /** Disable external low speed oscillator */
  using lse_disable = regval< RCC::BDCR::LSEON, 0 >;

  struct rtc_clock_source {
    /* Use LSE oscillator clock as RTC clock */
    using lse = RCC::BDCR::RTCSEL::LSE;

    /* Use LSI oscillator clock as RTC clock */
    using lsi = RCC::BDCR::RTCSEL::LSI;

    /* Use HSE oscillator clock as RTC clock */
    using hse = RCC::BDCR::RTCSEL::HSE;
  };

  /** Enable RTC clock */
  using rtc_enable = regval< RCC::BDCR::RTCEN, 1>;


public:  /* ------ static member functions ------ */

  static void backup_domain_software_reset(void) {
    RCC::BDCR::BDRST::set();
    RCC::BDCR::BDRST::clear();
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

  static void wait_lse_ready(void) {
    while(RCC::BDCR::LSERDY::test() == false);
  }
};


////////////////////  system_clock_hse  ////////////////////


template< typename... Tp >
struct system_clock_hse_impl
{
  static void init(void) {
    rcc::hse_enable::set();
    rcc::wait_hse_ready();
  }

  static void configure(void) {
    RCC::CFGR::reset_to< Tp... >();

    RCC::CR::PLLON::set();
    while(RCC::CR::PLLRDY::test() == false);
    RCC::CFGR::SW::PLL::set();
    while(RCC::CFGR::SWS::PLL::test() == false);
  }
};

template< freq_t output_freq, freq_t hse_freq = mhz(8) >
struct system_clock_hse {
  static_assert(output_freq == !output_freq,
                "unsupported system clock frequency (see available template specialisations below)");
};

/* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK    */
/* PLLCLK = 8MHz / 2 * 6 = 24 MHz               */
template<>
struct system_clock_hse< mhz(24), mhz(8) >
: public system_clock_hse_impl <
  RCC::CFGR::HPRE    ::DIV1,
  RCC::CFGR::PPRE1   ::DIV1,
  RCC::CFGR::PPRE2   ::DIV1,
  RCC::CFGR::PLLSRC  ::HSE,
  RCC::CFGR::PLLXTPRE::HSE_DIV2,
  RCC::CFGR::PLLMUL  ::MUL6
  >
{
  static constexpr freq_t hclk_freq  = mhz(24);
  static constexpr freq_t pclk1_freq = hclk_freq;
  static constexpr freq_t pclk2_freq = hclk_freq;
};

/* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK    */
/* PLLCLK = 8MHz / 2 * 9 = 36 MHz               */
template<>
struct system_clock_hse< mhz(36), mhz(8) >
: public system_clock_hse_impl <
  RCC::CFGR::HPRE    ::DIV1,
  RCC::CFGR::PPRE1   ::DIV1,
  RCC::CFGR::PPRE2   ::DIV1,
  RCC::CFGR::PLLSRC  ::HSE,
  RCC::CFGR::PLLXTPRE::HSE_DIV2,
  RCC::CFGR::PLLMUL  ::MUL9
  >
{
  static constexpr freq_t hclk_freq  = mhz(36);
  static constexpr freq_t pclk1_freq = hclk_freq;
  static constexpr freq_t pclk2_freq = hclk_freq;
};

/* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK/2  */
/* PLLCLK = 8MHz * 6 = 48 MHz                   */
template<>
struct system_clock_hse< mhz(48), mhz(8) >
: public system_clock_hse_impl <
  RCC::CFGR::HPRE    ::DIV1,
  RCC::CFGR::PPRE1   ::DIV2,
  RCC::CFGR::PPRE2   ::DIV1,
  RCC::CFGR::PLLSRC  ::HSE,
  RCC::CFGR::PLLXTPRE::HSE_DIV1,
  RCC::CFGR::PLLMUL  ::MUL6
  >
{
  static constexpr freq_t hclk_freq  = mhz(48);
  static constexpr freq_t pclk1_freq = hclk_freq / 2;
  static constexpr freq_t pclk2_freq = hclk_freq;
};

/* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK/2  */
/* PLLCLK = 8MHz * 7 = 56 MHz                   */
template<>
struct system_clock_hse< mhz(56), mhz(8) >
: public system_clock_hse_impl <
  RCC::CFGR::HPRE    ::DIV1,
  RCC::CFGR::PPRE1   ::DIV2,
  RCC::CFGR::PPRE2   ::DIV1,
  RCC::CFGR::PLLSRC  ::HSE,
  RCC::CFGR::PLLXTPRE::HSE_DIV1,
  RCC::CFGR::PLLMUL  ::MUL7
  >
{
  static constexpr freq_t hclk_freq  = mhz(56);
  static constexpr freq_t pclk1_freq = hclk_freq / 2;
  static constexpr freq_t pclk2_freq = hclk_freq;
};


/* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK/2  */
/* PLLCLK = 8MHz * 9 = 72 MHz                   */
template<>
struct system_clock_hse< mhz(72), mhz(8) >
: public system_clock_hse_impl <
  RCC::CFGR::HPRE    ::DIV1,
  RCC::CFGR::PPRE1   ::DIV2,
  RCC::CFGR::PPRE2   ::DIV1,
  RCC::CFGR::PLLSRC  ::HSE,
  RCC::CFGR::PLLXTPRE::HSE_DIV1,
  RCC::CFGR::PLLMUL  ::MUL9
  >
{
  static constexpr freq_t hclk_freq  = mhz(72);
  static constexpr freq_t pclk1_freq = hclk_freq / 2;
  static constexpr freq_t pclk2_freq = hclk_freq;
};


////////////////////  peripheral clock traits  ////////////////////


/*
 * Clock resource declaration (enable peripheral clocks)
 */
template<char>     struct rcc_gpio_clock_resources;
template<unsigned> struct rcc_spi_clock_resources;
template<unsigned> struct rcc_usart_clock_resources;
template<unsigned> struct rcc_adc_clock_resources;

using rcc_rtc_clock_resources = typelist<
  RCC::APB1ENR::PWREN,
  RCC::APB1ENR::BKPEN
  >;

/*
 * Clock resource specialisation (enable peripheral clocks)
 */
template<> struct rcc_gpio_clock_resources<'A'> : RCC::APB2ENR::IOPAEN { };
template<> struct rcc_gpio_clock_resources<'B'> : RCC::APB2ENR::IOPBEN { };
template<> struct rcc_gpio_clock_resources<'C'> : RCC::APB2ENR::IOPCEN { };
template<> struct rcc_gpio_clock_resources<'D'> : RCC::APB2ENR::IOPDEN { };
template<> struct rcc_gpio_clock_resources<'E'> : RCC::APB2ENR::IOPEEN { };
#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
template<> struct rcc_gpio_clock_resources<'F'> : RCC::APB2ENR::IOPFEN { };
template<> struct rcc_gpio_clock_resources<'G'> : RCC::APB2ENR::IOPGEN { };
#endif

template<> struct rcc_spi_clock_resources<1> : RCC::APB2ENR::SPI1EN { };
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
template<> struct rcc_spi_clock_resources<2> : RCC::APB1ENR::SPI2EN { };
#endif
#if defined (STM32F10X_HD) || defined (STM32F10X_CL)
template<> struct rcc_spi_clock_resources<3> : RCC::APB1ENR::SPI3EN { };
#endif

template<> struct rcc_usart_clock_resources<1> : RCC::APB2ENR::USART1EN { };
template<> struct rcc_usart_clock_resources<2> : RCC::APB1ENR::USART2EN { };
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
template<> struct rcc_usart_clock_resources<3> : RCC::APB1ENR::USART3EN { };
#endif

template<> struct rcc_adc_clock_resources<1> : RCC::APB2ENR::ADC1EN { };
#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL)
template<> struct rcc_adc_clock_resources<2> : RCC::APB2ENR::ADC2EN { };
#endif
#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
template<> struct rcc_adc_clock_resources<1> : RCC::APB2ENR::ADC3EN { };
#endif

} // namespace mptl

#endif // RCC_HPP_INCLUDED
