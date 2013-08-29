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
struct system_clock_hse_impl
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

template< freq_t output_freq, freq_t hse_freq = mhz(8) >
struct system_clock_hse {
  static_assert(output_freq == !output_freq,
                "unsupported system clock frequency (see available template specialisations below)");
};

template< freq_t hse_freq >
struct system_clock_hse< mhz(48), hse_freq >
: public system_clock_hse_impl <
  RCC::CFGR::HPRE ::DIV1,
  RCC::CFGR::PPRE1::DIV4,
  RCC::CFGR::PPRE2::DIV2,
  regval< RCC::PLLCFGR::PLLM, hse_freq / mhz(1) >,
  regval< RCC::PLLCFGR::PLLN, 96 >,
  regval< RCC::PLLCFGR::PLLP, 0  >, // DIV2
  regval< RCC::PLLCFGR::PLLQ, 2  >
  >
{
  static constexpr freq_t hclk_freq  = mhz(120);
  static constexpr freq_t pclk1_freq = hclk_freq / 4;
  static constexpr freq_t pclk2_freq = hclk_freq / 2;
};

template< freq_t hse_freq >
struct system_clock_hse< mhz(120), hse_freq >
: public system_clock_hse_impl <
  RCC::CFGR::HPRE ::DIV1,
  RCC::CFGR::PPRE1::DIV4,
  RCC::CFGR::PPRE2::DIV2,
  regval< RCC::PLLCFGR::PLLM, hse_freq / mhz(1) >,
  regval< RCC::PLLCFGR::PLLN, 240 >,
  regval< RCC::PLLCFGR::PLLP, 0   >, // DIV2
  regval< RCC::PLLCFGR::PLLQ, 5   >
  >
{
  static constexpr freq_t hclk_freq  = mhz(120);
  static constexpr freq_t pclk1_freq = hclk_freq / 4;
  static constexpr freq_t pclk2_freq = hclk_freq / 2;
};

template< freq_t hse_freq >
struct system_clock_hse< mhz(168), hse_freq >
: public system_clock_hse_impl <
  RCC::CFGR::HPRE ::DIV1,
  RCC::CFGR::PPRE1::DIV4,
  RCC::CFGR::PPRE2::DIV2,
  regval< RCC::PLLCFGR::PLLM, hse_freq / mhz(1) >,
  regval< RCC::PLLCFGR::PLLN, 336 >,
  regval< RCC::PLLCFGR::PLLP, 0   >, // DIV2
  regval< RCC::PLLCFGR::PLLQ, 7   >
  >
{
  static constexpr freq_t hclk_freq  = mhz(160);
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
