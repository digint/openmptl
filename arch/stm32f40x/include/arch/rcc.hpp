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

// TODO: this is merely a copy from stm32f10x/rcc.hpp
//       fix this!!!

#ifndef RCC_HPP_INCLUDED
#define RCC_HPP_INCLUDED

#include <arch/core.hpp>


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

    static_assert(freq == 168_mhz,
                  "unsupported system clock frequency");

    //    auto cfgr = RCC::CFGR::load();

    switch(freq) {
    case 168_mhz:
      /* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK/2  */
      /* PLLCLK = 8MHz * 9 = 72 MHz                   */
      RCC::CFGR::HPRE::shift_and_set(0x0); // DIV1
      RCC::CFGR::PPRE1::shift_and_set(0x5); // DIV4
      RCC::CFGR::PPRE2::shift_and_set(0x4); // DIV2

      RCC::PLLCFGR::store(RCC::PLLCFGR::PLLM::shifted_value(8)   | 
                          RCC::PLLCFGR::PLLN::shifted_value(336) |
                          RCC::PLLCFGR::PLLP::shifted_value(2)   |
                          RCC::PLLCFGR::PLLQ::shifted_value(7)   |
                          RCC::PLLCFGR::PLLSRC::value
                          );
      break;
    }
    //    RCC::CFGR::store(cfgr);

    /* Enable PLL */
    RCC::CR::PLLON::set();

    /* Wait till PLL is ready */
    while(RCC::CR::PLLRDY::test() == 0);

    /* Select PLL as system clock source */
    //    RCC::CFGR::SW::PLL::set();
    RCC::CFGR::SW1::set();

    /* Wait for PLL to be used */
    //    while(RCC::CFGR::SWS::PLL::test() == false);
    while(RCC::CFGR::SWS1::test() == 0);
  }


  static void Init(void) {
  }
};


#endif // RCC_HPP_INCLUDED
