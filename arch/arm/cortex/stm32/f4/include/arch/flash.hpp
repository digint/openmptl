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

namespace mptl {

template<typename rcc,
         typename pwr,
         bool prefetch_buffer = false,
         bool instruction_cache = true,
         bool data_cache = true
         >
class flash
{
  static_assert((pwr::system_voltage >= volt(2.1)) || (prefetch_buffer == false), "prefetch buffer must be disabled when the supply voltage is below 2.1V");

  using FLASH = reg::FLASH;

  static_assert(rcc::hclk_freq <= mhz(168), "unsupported system clock frequency");

  static constexpr FLASH::ACR::LATENCY::value_type latency =
    (pwr::system_voltage <= volt(2.1)) ? 
    ((rcc::hclk_freq <= mhz(20))  ?  0  :
     (rcc::hclk_freq <= mhz(40))  ?  1  :
     (rcc::hclk_freq <= mhz(60))  ?  2  :
     (rcc::hclk_freq <= mhz(80))  ?  3  :
     (rcc::hclk_freq <= mhz(100)) ?  4  :
     (rcc::hclk_freq <= mhz(120)) ?  5  :
     (rcc::hclk_freq <= mhz(140)) ?  6  :
     (rcc::hclk_freq <= mhz(160)) ?  7  :
     -1 ) :
    (pwr::system_voltage <= volt(2.4)) ?
    ((rcc::hclk_freq <= mhz(22))  ?  0  :
     (rcc::hclk_freq <= mhz(44))  ?  1  :
     (rcc::hclk_freq <= mhz(66))  ?  2  :
     (rcc::hclk_freq <= mhz(88))  ?  3  :
     (rcc::hclk_freq <= mhz(110)) ?  4  :
     (rcc::hclk_freq <= mhz(132)) ?  5  :
     (rcc::hclk_freq <= mhz(154)) ?  6  :
     (rcc::hclk_freq <= mhz(168)) ?  7  :
     -1 ) :
    (pwr::system_voltage <= volt(2.7)) ?
    ((rcc::hclk_freq <= mhz(24))  ?  0  :
     (rcc::hclk_freq <= mhz(48))  ?  1  :
     (rcc::hclk_freq <= mhz(72))  ?  2  :
     (rcc::hclk_freq <= mhz(96))  ?  3  :
     (rcc::hclk_freq <= mhz(120)) ?  4  :
     (rcc::hclk_freq <= mhz(144)) ?  5  :
     (rcc::hclk_freq <= mhz(168)) ?  6  :
     -1 ) :
    (pwr::system_voltage <= volt(3.6)) ?
    ((rcc::hclk_freq <= mhz(30))  ?  0  :
     (rcc::hclk_freq <= mhz(60))  ?  1  :
     (rcc::hclk_freq <= mhz(90))  ?  2  :
     (rcc::hclk_freq <= mhz(120)) ?  3  :
     (rcc::hclk_freq <= mhz(150)) ?  4  :
     (rcc::hclk_freq <= mhz(168)) ?  5  :
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
    FLASH::ACR::LATENCY::set_from(latency);
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

} // namespace mptl

#endif // FLASH_HPP_INCLUDED


