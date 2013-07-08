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

namespace mptl {

template<typename rcc,
         bool prefetch_buffer = true
         >
class flash
{
  using FLASH = reg::FLASH;

  static_assert(rcc::hclk_freq <= mhz(72), "unsupported system clock frequency");

  static constexpr FLASH::ACR::LATENCY::value_type latency =
    ((rcc::hclk_freq <= mhz(24))  ?  0  :
     (rcc::hclk_freq <= mhz(48))  ?  1  :
     (rcc::hclk_freq <= mhz(72))  ?  2  :
     -1 );

public:

  static void enable_prefetch_buffer(void) {
    FLASH::ACR::PRFTBE::set();
  }
  static void disable_prefetch_buffer(void) {
    FLASH::ACR::PRFTBE::clear();
  }

  static void set_latency(void) {
    static_assert(latency <= 2, "invalid FLASH::ACR::LATENCY value");
    FLASH::ACR::LATENCY::set_from(latency);
  }

  static void init(void) {
    if(prefetch_buffer)
      enable_prefetch_buffer();

    set_latency();
  }
};

} // namespace mptl

#endif // FLASH_HPP_INCLUDED
