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

#ifndef FLASH_HPP_INCLUDED
#define FLASH_HPP_INCLUDED

#include <arch/reg/flash.hpp>
#include <freq.hpp>
#include <voltage.hpp>

template<typename rcc,
         typename pwr,
         bool prefetch_buffer = false,
         bool instruction_cache = true,
         bool data_cache = true
         >
class Flash
{
  static_assert((pwr::system_voltage >= 2.1_volt) || (prefetch_buffer == false), "prefetch buffer must be disabled when the supply voltage is below 2.1V");

  using FLASH = reg::FLASH;

    static_assert(rcc::hclk_freq <= 168_mhz, "unsupported system clock frequency");

    static constexpr FLASH::ACR::LATENCY::value_type latency =
      (pwr::system_voltage <= 2.1_volt) ? 
          ((rcc::hclk_freq <= 20_mhz)  ?  0  :
           (rcc::hclk_freq <= 40_mhz)  ?  1  :
           (rcc::hclk_freq <= 60_mhz)  ?  2  :
           (rcc::hclk_freq <= 80_mhz)  ?  3  :
           (rcc::hclk_freq <= 100_mhz) ?  4  :
           (rcc::hclk_freq <= 120_mhz) ?  5  :
           (rcc::hclk_freq <= 140_mhz) ?  6  :
           (rcc::hclk_freq <= 160_mhz) ?  7  :
           -1 ) :
      (pwr::system_voltage <= 2.4_volt) ?
          ((rcc::hclk_freq <= 22_mhz)  ?  0  :
           (rcc::hclk_freq <= 44_mhz)  ?  1  :
           (rcc::hclk_freq <= 66_mhz)  ?  2  :
           (rcc::hclk_freq <= 88_mhz)  ?  3  :
           (rcc::hclk_freq <= 110_mhz) ?  4  :
           (rcc::hclk_freq <= 132_mhz) ?  5  :
           (rcc::hclk_freq <= 154_mhz) ?  6  :
           (rcc::hclk_freq <= 168_mhz) ?  7  :
           -1 ) :
      (pwr::system_voltage <= 2.7_volt) ?
          ((rcc::hclk_freq <= 24_mhz)  ?  0  :
           (rcc::hclk_freq <= 48_mhz)  ?  1  :
           (rcc::hclk_freq <= 72_mhz)  ?  2  :
           (rcc::hclk_freq <= 96_mhz)  ?  3  :
           (rcc::hclk_freq <= 120_mhz) ?  4  :
           (rcc::hclk_freq <= 144_mhz) ?  5  :
           (rcc::hclk_freq <= 168_mhz) ?  6  :
           -1 ) :
      (pwr::system_voltage <= 3.6_volt) ?
          ((rcc::hclk_freq <= 30_mhz)  ?  0  :
           (rcc::hclk_freq <= 60_mhz)  ?  1  :
           (rcc::hclk_freq <= 90_mhz)  ?  2  :
           (rcc::hclk_freq <= 120_mhz) ?  3  :
           (rcc::hclk_freq <= 150_mhz) ?  4  :
           (rcc::hclk_freq <= 168_mhz) ?  5  :
           -1 ) : -1;
public:

  static void enable_prefetch_buffer(void) {
    FLASH::ACR::PRFTEN::set();
  }
  static void disable_prefetch_buffer(void) {
    FLASH::ACR::PRFTEN::clear();
  }

  static void enable_instruction_cache(void) {
    FLASH::ACR::ICEN::set();
  }
  static void disable_instruction_cache(void) {
    FLASH::ACR::ICEN::clear();
  }

  static void enable_data_cache(void) {
    FLASH::ACR::DCEN::set();
  }
  static void disable_data_cache(void) {
    FLASH::ACR::DCEN::clear();
  }

  static void set_latency(void) {
    static_assert(latency <= 7, "invalid FLASH::ACR::LATENCY value");
    FLASH::ACR::LATENCY::shift_and_set(latency);
  }

  static void init(void) {
    auto acr = FLASH::ACR::load();
    if(prefetch_buffer)
      acr |= FLASH::ACR::PRFTEN::value;
    if(instruction_cache)
      acr |= FLASH::ACR::ICEN::value;
    if(data_cache)
      acr |= FLASH::ACR::DCEN::value;
    FLASH::ACR::store(acr);

    set_latency();
  }
};


#endif // FLASH_HPP_INCLUDED


