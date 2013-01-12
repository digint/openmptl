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
#include <freq.hpp>
#include <voltage.hpp>


class Flash
{
  using FLASH = Core::FLASH;

  template<freq_t freq, voltage_t voltage>
  struct latency {
    static_assert(freq <= 168_mhz, "unsupported system clock frequency");
    static_assert(voltage >= 1.8_volt && voltage <= 3.6_volt, "unsupported system voltage");

    static constexpr FLASH::ACR::LATENCY::value_type value = 
      (voltage <= 2.1_volt) ? ((freq <= 20_mhz)  ?  0  :
                               (freq <= 40_mhz)  ?  1  :
                               (freq <= 60_mhz)  ?  2  :
                               (freq <= 80_mhz)  ?  3  :
                               (freq <= 100_mhz) ?  4  :
                               (freq <= 120_mhz) ?  5  :
                               (freq <= 140_mhz) ?  6  :
                               (freq <= 160_mhz) ?  7  :
                               -1 ) :
      (voltage <= 2.4_volt) ? ((freq <= 22_mhz)  ?  0  :
                               (freq <= 44_mhz)  ?  1  :
                               (freq <= 66_mhz)  ?  2  :
                               (freq <= 88_mhz)  ?  3  :
                               (freq <= 110_mhz) ?  4  :
                               (freq <= 132_mhz) ?  5  :
                               (freq <= 154_mhz) ?  6  :
                               (freq <= 168_mhz) ?  7  :
                               -1 ) :
      (voltage <= 2.7_volt) ? ((freq <= 24_mhz)  ?  0  :
                               (freq <= 48_mhz)  ?  1  :
                               (freq <= 72_mhz)  ?  2  :
                               (freq <= 96_mhz)  ?  3  :
                               (freq <= 120_mhz) ?  4  :
                               (freq <= 144_mhz) ?  5  :
                               (freq <= 168_mhz) ?  6  :
                               -1 ) :
      (voltage <= 3.6_volt) ? ((freq <= 30_mhz)  ?  0  :
                               (freq <= 60_mhz)  ?  1  :
                               (freq <= 90_mhz)  ?  2  :
                               (freq <= 120_mhz) ?  3  :
                               (freq <= 150_mhz) ?  4  :
                               (freq <= 168_mhz) ?  5  :
                               -1 ) : -1;
  };

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

  template<freq_t freq, voltage_t voltage>
  static void SetLatency(void) {
    static_assert(latency<freq, voltage>::value <= 7, "invalid FLASH::ACR::LATENCY value");
    FLASH::ACR::LATENCY::shift_and_set(latency<freq, voltage>::value);
  }
};


#endif // FLASH_HPP_INCLUDED


