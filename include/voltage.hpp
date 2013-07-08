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

#ifndef VOLTAGE_HPP_INCLUDED
#define VOLTAGE_HPP_INCLUDED

namespace mptl {

typedef unsigned int voltage_t;

#if 0 // literals don't like namespaces...

static constexpr voltage_t operator"" _volt(long double x) { return x * 1000; }

//static_assert(1.0009_volt == 1000, "doubles are always rounded down");

#endif

static constexpr voltage_t volt(long double x) { return x * 1000; }

static_assert(volt(1.0009) == 1000, "doubles are always rounded down");

} // namespace mptl

#endif // VOLTAGE_HPP_INCLUDED
