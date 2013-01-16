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

#ifndef SETUP_HPP_INCLUDED
#define SETUP_HPP_INCLUDED

#include <freq.hpp>

struct CoreSetup {
  // static constexpr freq_t clock_frequency = 24_mhz;
  static constexpr freq_t clock_frequency = 72_mhz;

  /* note that clocks might not be setup when this is called */
  static void Panic(void) { while(1); };
};

struct InterruptControllerSetup {
  static constexpr unsigned priority_bits = 4;
};

#endif // SETUP_HPP_INCLUDED
