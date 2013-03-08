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
#include <resource.hpp>
#include <freq.hpp>


// TODO: access functions, change public to private
class Rcc {
private:

  static constexpr int hse_startup_timeout = 1500;

public:

  using RCC = reg::RCC;

  static void EnableHSE(void) {
    RCC::CR::HSEON::set();
  }
  static bool WaitHSERDY() {
    int timeout = hse_startup_timeout;
    while(!(RCC::CR::HSERDY::test()) && timeout){
      timeout--;
    }
    return timeout;
  }

  /* Note: this is only valid for clocks setup by SetSysClock() function */
  template<freq_t freq>
  struct ClockFrequency {
    static constexpr freq_t hclk  = freq;
    static constexpr freq_t pclk1 = freq <= 36_mhz ? freq : freq / 2;
    static constexpr freq_t pclk2 = freq;
  };

  /**
   * @brief  Sets System clock frequency and configure HCLK, PCLK2 
   *         and PCLK1 prescalers.
   * @note   This function should be used only after reset.
   * @note   HSE must be enabled before this function is called.
   * @param  freq frequency (Hz) to be set
   */
  template<freq_t freq>
  static void SetSysClock(void) {

    static_assert(freq == 24_mhz ||
                  freq == 36_mhz ||
                  freq == 48_mhz ||
                  freq == 56_mhz ||
                  freq == 72_mhz,
                  "unsupported system clock frequency");

    auto cfgr = RCC::CFGR::load();

    switch(freq) {
    case 24_mhz:
      /* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK    */
      /* PLLCLK = 8MHz / 2 * 6 = 24 MHz               */
      cfgr |= (RCC::CFGR::HPRE::DIV1::value | RCC::CFGR::PPRE2::DIV1::value | RCC::CFGR::PPRE1::DIV1::value);
      cfgr &= ~(RCC::CFGR::PLLSRC::value | RCC::CFGR::PLLXTPRE::value | RCC::CFGR::PLLMUL::value);
      cfgr |= (RCC::CFGR::PLLSRC::HSE::value | RCC::CFGR::PLLXTPRE::HSE_DIV2::value | RCC::CFGR::PLLMUL::MUL6::value);
      break;
    case 36_mhz:
      /* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK    */
      /* PLLCLK = 8MHz / 2 * 9 = 36 MHz               */
      cfgr |= (RCC::CFGR::HPRE::DIV1::value | RCC::CFGR::PPRE2::DIV1::value | RCC::CFGR::PPRE1::DIV1::value);
      cfgr &= ~(RCC::CFGR::PLLSRC::value | RCC::CFGR::PLLXTPRE::value | RCC::CFGR::PLLMUL::value);
      cfgr |= (RCC::CFGR::PLLSRC::HSE::value | RCC::CFGR::PLLXTPRE::HSE_DIV2::value | RCC::CFGR::PLLMUL::MUL9::value);
      break;
    case 48_mhz:
      /* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK/2  */
      /* PLLCLK = 8MHz * 6 = 48 MHz                   */
      cfgr |= (RCC::CFGR::HPRE::DIV1::value | RCC::CFGR::PPRE2::DIV1::value | RCC::CFGR::PPRE1::DIV2::value);
      cfgr &= ~(RCC::CFGR::PLLSRC::value | RCC::CFGR::PLLXTPRE::value | RCC::CFGR::PLLMUL::value);
      cfgr |= (RCC::CFGR::PLLSRC::HSE::value | RCC::CFGR::PLLMUL::MUL6::value);
      break;
    case 56_mhz:
      /* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK/2  */
      /* PLLCLK = 8MHz * 7 = 56 MHz                   */
      cfgr |= (RCC::CFGR::HPRE::DIV1::value | RCC::CFGR::PPRE2::DIV1::value | RCC::CFGR::PPRE1::DIV2::value);
      cfgr &= ~(RCC::CFGR::PLLSRC::value | RCC::CFGR::PLLXTPRE::value | RCC::CFGR::PLLMUL::value);
      cfgr |= (RCC::CFGR::PLLSRC::HSE::value | RCC::CFGR::PLLMUL::MUL7::value);
      break;
    case 72_mhz:
      /* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK/2  */
      /* PLLCLK = 8MHz * 9 = 72 MHz                   */
      cfgr |= (RCC::CFGR::HPRE::DIV1::value | RCC::CFGR::PPRE2::DIV1::value | RCC::CFGR::PPRE1::DIV2::value);
      cfgr &= ~(RCC::CFGR::PLLSRC::value | RCC::CFGR::PLLXTPRE::value | RCC::CFGR::PLLMUL::value);
      cfgr |= (RCC::CFGR::PLLSRC::HSE::value | RCC::CFGR::PLLMUL::MUL9::value);
      break;
    }
    RCC::CFGR::store(cfgr);

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

    RCC::CFGR::clear(RCC::CFGR::SW::value |
                     RCC::CFGR::SWS::value |
                     RCC::CFGR::HPRE::value |
                     RCC::CFGR::PPRE1::value | 
                     RCC::CFGR::PPRE2::value | 
                     RCC::CFGR::ADCPRE::value | 
                     RCC::CFGR::MCO::value );

    RCC::CR::clear(RCC::CR::HSEON::value |
                   RCC::CR::CSSON::value |
                   RCC::CR::PLLON::value);

    RCC::CR::clear(RCC::CR::HSEBYP::value);

    RCC::CFGR::clear(RCC::CFGR::PLLSRC::value |
                     RCC::CFGR::PLLXTPRE::value |
                     RCC::CFGR::PLLMUL::value |
#ifdef STM32F10X_CL
                     RCC::CFGR::OTGFSPRE::value
#else
                     RCC::CFGR::USBPRE::value 
#endif
                     );

#ifdef STM32F10X_CL
    RCC::CR::clear(RCC::CR::PLL2ON::value |
                   RCC::CR::PLL3ON::value);
#endif

    /* Disable all interrupts and clear pending bits  */
    RCC::CIR::store(RCC::CIR::LSIRDYC::value |
                    RCC::CIR::LSERDYC::value |
                    RCC::CIR::HSIRDYC::value |
                    RCC::CIR::HSERDYC::value |
                    RCC::CIR::PLLRDYC::value |
#ifdef STM32F10X_CL
                    RCC::CIR::PLL2RDYC::value |
                    RCC::CIR::PLL3RDYC::value |
#endif
                    RCC::CIR::CSSC::value);

#ifdef STM32F10X_CL
    RCC::CFGR2::reset();
#endif
  }


  /*
   * Clock resource declaration (enable peripheral clocks)
   */
  template<char>        struct gpio_clock_resources;
  template<std::size_t> struct spi_clock_resources;
  template<std::size_t> struct usart_clock_resources;
  template<std::size_t> struct adc_clock_resources;

  using rtc_clock_resources = ResourceList<
    SharedRegister< RCC::APB1ENR,
                    ( RCC::APB1ENR::PWREN::value |
                      RCC::APB1ENR::BKPEN::value ) > >;
};


/*
 * Clock resource specialisation (enable peripheral clocks)
 */
template<> struct Rcc::gpio_clock_resources<'A'> : ResourceList< SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::IOPAEN::value> > { };
template<> struct Rcc::gpio_clock_resources<'B'> : ResourceList< SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::IOPBEN::value> > { };
template<> struct Rcc::gpio_clock_resources<'C'> : ResourceList< SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::IOPCEN::value> > { };
template<> struct Rcc::gpio_clock_resources<'D'> : ResourceList< SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::IOPDEN::value> > { };
template<> struct Rcc::gpio_clock_resources<'E'> : ResourceList< SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::IOPEEN::value> > { };
#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
template<> struct Rcc::gpio_clock_resources<'F'> : ResourceList< SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::IOPFEN::value> > { };
template<> struct Rcc::gpio_clock_resources<'G'> : ResourceList< SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::IOPGEN::value> > { };
#endif

template<> struct Rcc::spi_clock_resources<1> : ResourceList< SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::SPI1EN::value> > { };
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
template<> struct Rcc::spi_clock_resources<2> : ResourceList< SharedRegister<reg::RCC::APB1ENR, reg::RCC::APB1ENR::SPI2EN::value> > { };
#endif
#if defined (STM32F10X_HD) || defined (STM32F10X_CL)
template<> struct Rcc::spi_clock_resources<3> : ResourceList< SharedRegister<reg::RCC::APB1ENR, reg::RCC::APB1ENR::SPI3EN::value> > { };
#endif

template<> struct Rcc::usart_clock_resources<1> : ResourceList< SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::USART1EN::value> > { };
template<> struct Rcc::usart_clock_resources<2> : ResourceList< SharedRegister<reg::RCC::APB1ENR, reg::RCC::APB1ENR::USART2EN::value> > { };
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
template<> struct Rcc::usart_clock_resources<3> : ResourceList< SharedRegister<reg::RCC::APB1ENR, reg::RCC::APB1ENR::USART3EN::value> > { };
#endif

template<> struct Rcc::adc_clock_resources<1> : ResourceList< SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::ADC1EN::value> > { };
#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL)
template<> struct Rcc::adc_clock_resources<2> : ResourceList< SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::ADC2EN::value> > { };
#endif
#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
template<> struct Rcc::adc_clock_resources<1> : ResourceList< SharedRegister<reg::RCC::APB2ENR, reg::RCC::APB2ENR::ADC3EN::value> > { };
#endif
  

#endif // RCC_HPP_INCLUDED
