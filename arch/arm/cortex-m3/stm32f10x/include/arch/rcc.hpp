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

#include <core.hpp>


// TODO: access functions, change public to private
class Rcc {
private:

  static constexpr int hse_startup_timeout = 1500;

public:

  using RCC = Core::RCC;

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


  // TODO: use constants
  static void Init(void) {
    /* Reset the RCC clock configuration to the default reset state(for debug purpose) */
    /* Set HSION bit */
    RCC::CR::set(0x00000001);

    /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
#ifndef STM32F10X_CL
    RCC::CFGR::mask(0xF8FF0000);
#else
    RCC::CFGR::mask(0xF0FF0000);
#endif /* STM32F10X_CL */   
  
    /* Reset HSEON, CSSON and PLLON bits */
    RCC::CR::mask(0xFEF6FFFF);

    /* Reset HSEBYP bit */
    RCC::CR::mask(0xFFFBFFFF);

    /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
    RCC::CFGR::mask(0xFF80FFFF);

#ifdef STM32F10X_CL
    /* Reset PLL2ON and PLL3ON bits */
    RCC::CR::mask(0xEBFFFFFF);

    /* Disable all interrupts and clear pending bits  */
    RCC::CIR::store(0x00FF0000);

    /* Reset CFGR2 register */
    RCC::CFGR2::reset();
#elif defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) 
    /* Disable all interrupts and clear pending bits  */
    RCC::CIR::store(0x009F0000);

    /* Reset CFGR2 register */
    RCC::CFGR2::reset();
#else
    /* Disable all interrupts and clear pending bits  */
    RCC::CIR::store(0x009F0000);
#endif /* STM32F10X_CL */
  }
};


#endif // RCC_HPP_INCLUDED
