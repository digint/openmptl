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

#ifndef FLASH_HPP_INCLUDED
#define FLASH_HPP_INCLUDED

#include <arch/core.hpp>


class Flash
{
  using FLASH = Core::FLASH;

public:

  static void EnablePrefetchBuffer(void) {
    FLASH::ACR::PRFTEN::set();
  }
  static void DisablePrefetchBuffer(void) {
    FLASH::ACR::PRFTEN::clear();
  }

  static void EnableInstructionCache(void) {
    FLASH::ACR::ICEN::set();
  }
  static void DisableInstructionCache(void) {
    FLASH::ACR::ICEN::clear();
  }

  static void EnableDataCache(void) {
    FLASH::ACR::DCEN::set();
  }
  static void DisableDataCache(void) {
    FLASH::ACR::DCEN::clear();
  }

  template<freq_t freq>
  static void SetLatency(void) {
    static_assert(freq == 168_mhz,
                  "unsupported system clock frequency");

    switch(freq) {
    case 168_mhz:
      FLASH::ACR::LATENCY::shift_and_set(0x5);  // 5WS
      break;
    }
  }
};


#endif // FLASH_HPP_INCLUDED


