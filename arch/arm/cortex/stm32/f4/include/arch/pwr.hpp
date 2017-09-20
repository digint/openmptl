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

#ifndef ARCH_PWR_HPP_INCLUDED
#define ARCH_PWR_HPP_INCLUDED

#include <arch/reg/pwr.hpp>
#include <voltage.hpp>

namespace mptl {

template< voltage_t _system_voltage = volt(3.3) >
class pwr
{
  static_assert(_system_voltage >= volt(1.8) && _system_voltage <= volt(3.6), "unsupported system voltage");

  template< typename system_clock_type >
  struct power_save_enable_impl {
    static_assert(system_clock_type::hclk_freq <= mhz(144), "system clock frequency too high for power save feature");
    using type = regval< PWR::CR::VOS, 0 >;
  };

public:

  static constexpr voltage_t system_voltage = _system_voltage;

public:  /* ------ configuration traits ------ */

  /** disable high performance mode */
  template< typename system_clock_type >
  using power_save_enable = typename power_save_enable_impl<system_clock_type>::type;

public:  /* ------ static member functions ------ */

  static void disable_backup_domain_write_protection(void) {
    PWR::CR::DBP::set();
  }
};

} // namespace mptl

#endif // ARCH_PWR_HPP_INCLUDED
