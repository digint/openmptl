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

#ifndef CORE_HPP_INCLUDED
#define CORE_HPP_INCLUDED

#include "../../../../common/core.hpp"

#include <arch/rcc.hpp>
#include <arch/flash.hpp>
#include <arch/pwr.hpp>

template<freq_t    _clock_frequency = 168_mhz,
         voltage_t _system_voltage  = 3.3_volt,
         bool      _power_save      = false >
struct Core : public CoreFunctions
{
  static_assert(_clock_frequency <= 168_mhz, "unsupported system clock frequency");
  static_assert(_system_voltage >= 1.8_volt && _system_voltage <= 3.6_volt, "unsupported system voltage");
  static_assert(_power_save == false || _clock_frequency <= 144_mhz, "system clock frequency too high for power save feature");

  static constexpr freq_t    clock_frequency = _clock_frequency;
  static constexpr voltage_t system_voltage  = _system_voltage;
  static constexpr bool      power_save      = _power_save;


  static void init_clocks() {
    Rcc::enable_hse();
    Rcc::wait_hse_ready();

    //  Flash::enable_prefetch_buffer();
    Flash::enable_instruction_cache();
    Flash::enable_data_cache();
    Flash::set_latency<clock_frequency, system_voltage>();

    Pwr::set_power_save<power_save>();

    Rcc::set_system_clock<clock_frequency>();
  }

  /* Initialize the hardware.
   * NOTE: This function is called BEFORE ANY static object constructors are called!
   */
  static void init(void) {
    init_clocks();

    // TODO: NVIC Priority Grouping MPL magic
  }
};

#endif // CORE_HPP_INCLUDED
