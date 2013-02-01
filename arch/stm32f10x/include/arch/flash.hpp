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

#include <arch/register_defs.hpp>


class Flash
{
  using FLASH = reg::FLASH;

public:

  static void EnablePrefetchBuffer(void) {
    FLASH::ACR::PRFTBE::set();
  }
  static void DisablePrefetchBuffer(void) {
    FLASH::ACR::PRFTBE::clear();
  }

  template<freq_t freq>
  static void SetLatency(void) {
    switch(freq) {
    case 24_mhz:
      FLASH::ACR::LATENCY::shift_and_set(0);
      break;
    case 36_mhz:
    case 48_mhz:
      FLASH::ACR::LATENCY::shift_and_set(1);
      break;
    case 56_mhz:
    case 72_mhz:
      FLASH::ACR::LATENCY::shift_and_set(2);
      break;
    }
  }
};


#endif // FLASH_HPP_INCLUDED


