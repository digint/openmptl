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

#ifndef PWR_HPP_INCLUDED
#define PWR_HPP_INCLUDED

#include <arch/reg/pwr.hpp>
#include <voltage.hpp>

template<typename  rcc,
         voltage_t _system_voltage = 3.3_volt,
         bool      power_save = false>
class Pwr
{
  static_assert(_system_voltage >= 1.8_volt && _system_voltage <= 3.6_volt, "unsupported system voltage");
  static_assert(power_save == false || rcc::hclk_freq <= 144_mhz, "system clock frequency too high for power save feature");

  using PWR = reg::PWR;

public:

  static constexpr voltage_t system_voltage = _system_voltage;

  static void disable_backup_domain_write_protection(void) {
    PWR::CR::DBP::set();
  }

  /* enable/disable high performance mode */
  static void enable_power_save(void) {
    PWR::CR::VOS::clear();
  }

  static void init(void) {
    if(power_save)
      enable_power_save();
  }
};


#endif // PWR_HPP_INCLUDED
