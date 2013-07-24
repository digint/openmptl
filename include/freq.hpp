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

#ifndef FREQ_HPP_INCLUDED
#define FREQ_HPP_INCLUDED

namespace mptl {

typedef unsigned int freq_t;

#if 0 // literals don't like namespaces...

static constexpr freq_t operator"" _hz (unsigned long long x) { return x; }
static constexpr freq_t operator"" _khz(unsigned long long x) { return x * 1000; }
static constexpr freq_t operator"" _mhz(unsigned long long x) { return x * 1000 * 1000; }

static constexpr freq_t operator"" _khz(long double x)        { return x * 1000; }
static constexpr freq_t operator"" _mhz(long double x)        { return x * 1000 * 1000; }

//static_assert(1.0009_khz == 1000, "doubles are always rounded down");

#endif

static inline constexpr freq_t hz (unsigned long long x) { return x; }
static inline constexpr freq_t khz(unsigned long long x) { return x * 1000; }
static inline constexpr freq_t mhz(unsigned long long x) { return x * 1000 * 1000; }

} // namespace mptl

#endif // FREQ_HPP_INCLUDED
