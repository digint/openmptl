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

// TODO: define these somewhere else
typedef unsigned int freq_t;

static constexpr freq_t operator"" _hz (unsigned long long x) { return x; }
static constexpr freq_t operator"" _khz(unsigned long long x) { return x * 1000; }
static constexpr freq_t operator"" _mhz(unsigned long long x) { return x * 1000 * 1000; }

static constexpr freq_t operator"" _khz(long double x)        { return x * 1000; }
static constexpr freq_t operator"" _mhz(long double x)        { return x * 1000 * 1000; }

//static_assert(1.0009_khz == 1000, "doubles are always rounded down");


struct CoreSetup {
  // static constexpr freq_t clock_frequency = 24_mhz;
  static constexpr freq_t clock_frequency = 72_mhz;

  /* note that clocks might not be setup when this is called */
  static void Panic(void) { while(1); };
};

struct InterruptControllerSetup {
  static const unsigned priority_bits = 4;
};

// TODO: nice interrupt priority setup
//
//    SetPriorityGrouping(3 + priority_bits); // TODO: check this!
//	NVIC_SetPriorityGrouping(7);	// no preemption, 4 bit of subprio
//      NVIC_SetPriorityGrouping(6);	// 1 bit preemption, 3 bit of subprio
//	NVIC_SetPriorityGrouping(5);	// 2 bit preemption, 2 bit of subprio
//	NVIC_SetPriorityGrouping(4);	// 3 bit preemption, 1 bit of subprio
//	NVIC_SetPriorityGrouping(3);	// 4 bit preemption, 0 bit of subprio

#endif // SETUP_HPP_INCLUDED
