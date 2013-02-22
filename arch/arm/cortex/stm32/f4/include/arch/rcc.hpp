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

#ifndef RCC_HPP_INCLUDED
#define RCC_HPP_INCLUDED

#include <arch/reg/rcc.hpp>
#include <resource_mpl.hpp>
#include <freq.hpp>

// TODO: access functions, change public to private
class Rcc {

  using RCC = reg::RCC;

  template<int m, int n, int p, int q>
  struct pllcfgr_hse {
    static_assert(m >= 2 && m <= 63,                    "Invalid PLLM value");
    static_assert(n >= 2 && n <= 432,                   "Invalid PLLN value");
    static_assert(p == 2 || p == 4 || p == 6 || p == 8, "Invalid PLLP value");
    static_assert(q >= 2 && q <= 15,                    "Invalid PLLQ value");

    static constexpr RCC::PLLCFGR::value_type value =
      RCC::PLLCFGR::PLLM::shifted_value(m)           |
      RCC::PLLCFGR::PLLN::shifted_value(n)           |
      RCC::PLLCFGR::PLLP::shifted_value((p >> 1) -1) |
      RCC::PLLCFGR::PLLQ::shifted_value(q)           |
      RCC::PLLCFGR::PLLSRC::value;  // HSE
  };

public:

  static void EnableHSE(void) {
    RCC::CR::HSEON::set();
  }
  static void WaitHSERDY() {
    while(RCC::CR::HSERDY::test() == false);
  }
  static void EnableHSI(void) {
    RCC::CR::HSION::set();
  }
  static void WaitHSIRDY() {
    while(RCC::CR::HSIRDY::test() == false);
  }

  /* Note: this is only valid for clocks setup by SetSysClock() function */
  template<freq_t freq>
  struct ClockFrequency {
    static_assert(freq == 168_mhz || freq == 120_mhz, "unsupported system clock frequency");

    static constexpr freq_t hclk  = freq;
    static constexpr freq_t pclk1 = ( freq == 120_mhz ? 30_mhz :
                                      freq == 168_mhz ? 42_mhz :
                                      0 );
    static constexpr freq_t pclk2 = ( freq == 120_mhz ? 60_mhz :
                                      freq == 168_mhz ? 84_mhz :
                                      0 );
  };

  template<freq_t freq>
  static void SetSysClock(void) {
    static_assert(freq == 168_mhz || freq == 120_mhz, "unsupported system clock frequency");

    // auto cfgr = RCC::CFGR::load();
    auto cfgr = RCC::CFGR::reset_value;
    cfgr &= ~(RCC::CFGR::HPRE::value       | RCC::CFGR::PPRE1::value       | RCC::CFGR::PPRE2::value);
    cfgr |=   RCC::CFGR::HPRE::DIV1::value | RCC::CFGR::PPRE1::DIV4::value | RCC::CFGR::PPRE2::DIV2::value;
    RCC::CFGR::store(cfgr);

    switch(freq) {
    case 120_mhz:
      RCC::PLLCFGR::store(pllcfgr_hse<8, 240, 2, 5>::value);
      break;
    case 168_mhz:
      RCC::PLLCFGR::store(pllcfgr_hse<8, 336, 2, 7>::value);
      break;
    }

    /* Enable PLL */
    RCC::CR::PLLON::set();

    /* Wait till PLL is ready */
    while(RCC::CR::PLLRDY::test() == 0);

    /* Select PLL as system clock source */
    RCC::CFGR::SW::PLL::set();

    /* Wait for PLL to be used */
    while(RCC::CFGR::SWS::PLL::test() == false);
  }

  static void Init(void) {
    /* Reset the RCC clock configuration to the default reset state (for debug purpose) */
    RCC::CR::HSION::set();
    RCC::CFGR::reset();
    RCC::CR::clear< RCC::CR::HSEON, RCC::CR::CSSON, RCC::CR::PLLON >();
    RCC::PLLCFGR::reset();
    RCC::CR::HSEBYP::clear();
    RCC::CIR::reset();
  }

  /*
   * Clock resource declaration (enable peripheral clocks)
   */
  template<char>        struct gpio_clock_resources;
  template<std::size_t> struct usart_clock_resources;
};


/*
 * Clock resource specialisation (enable peripheral clocks)
 */
template<> struct Rcc::gpio_clock_resources<'A'> : ResourceList< SharedRegister<reg::RCC::AHB1ENR, reg::RCC::AHB1ENR::GPIOAEN::value> > { };
template<> struct Rcc::gpio_clock_resources<'B'> : ResourceList< SharedRegister<reg::RCC::AHB1ENR, reg::RCC::AHB1ENR::GPIOBEN::value> > { };
template<> struct Rcc::gpio_clock_resources<'C'> : ResourceList< SharedRegister<reg::RCC::AHB1ENR, reg::RCC::AHB1ENR::GPIOCEN::value> > { };
template<> struct Rcc::gpio_clock_resources<'D'> : ResourceList< SharedRegister<reg::RCC::AHB1ENR, reg::RCC::AHB1ENR::GPIODEN::value> > { };
template<> struct Rcc::gpio_clock_resources<'E'> : ResourceList< SharedRegister<reg::RCC::AHB1ENR, reg::RCC::AHB1ENR::GPIOEEN::value> > { };
template<> struct Rcc::gpio_clock_resources<'F'> : ResourceList< SharedRegister<reg::RCC::AHB1ENR, reg::RCC::AHB1ENR::GPIOFEN::value> > { };
template<> struct Rcc::gpio_clock_resources<'G'> : ResourceList< SharedRegister<reg::RCC::AHB1ENR, reg::RCC::AHB1ENR::GPIOGEN::value> > { };
template<> struct Rcc::gpio_clock_resources<'H'> : ResourceList< SharedRegister<reg::RCC::AHB1ENR, reg::RCC::AHB1ENR::GPIOHEN::value> > { };
template<> struct Rcc::gpio_clock_resources<'I'> : ResourceList< SharedRegister<reg::RCC::AHB1ENR, reg::RCC::AHB1ENR::GPIOIEN::value> > { };

template<> struct Rcc::usart_clock_resources<1> : ResourceList< SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::USART1EN::value> > { };
template<> struct Rcc::usart_clock_resources<2> : ResourceList< SharedRegister<reg::RCC::APB1ENR, reg::RCC::APB1ENR::USART2EN::value> > { };
template<> struct Rcc::usart_clock_resources<3> : ResourceList< SharedRegister<reg::RCC::APB1ENR, reg::RCC::APB1ENR::USART3EN::value> > { };
//template<> struct Rcc::usart_clock_resources<4> : ResourceList< SharedRegister<reg::RCC::APB1ENR, reg::RCC::APB1ENR::UART4EN::value> > { };
//template<> struct Rcc::usart_clock_resources<5> : ResourceList< SharedRegister<reg::RCC::APB1ENR, reg::RCC::APB1ENR::UART5EN::value> > { };
template<> struct Rcc::usart_clock_resources<6> : ResourceList< SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::USART6EN::value> > { };


#endif // RCC_HPP_INCLUDED
