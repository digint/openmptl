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

#ifndef CORE_STARTUP_HPP_INCLUDED
#define CORE_STARTUP_HPP_INCLUDED

#include <core_setup.hpp>
#include <arch/rcc.hpp>
#include <arch/flash.hpp>

struct CoreStartup : CoreSetup {
  /* Make sure clock_frequency is set correctly in "core_setup.hpp" */
  static_assert(clock_frequency <= 72_mhz, "unsupported system clock frequency");

  static constexpr int hse_startup_timeout = 1500;

  static void init_clocks(void) {
    int success;

    Rcc::enable_hse();
    success = Rcc::wait_hse_ready(hse_startup_timeout);

    if(!success)
      Panic();

    Flash::enable_prefetch_buffer();
    Flash::set_latency<clock_frequency>();

    Rcc::set_system_clock<clock_frequency>();
  }
};

#endif // CORE_STARTUP_HPP_INCLUDED
