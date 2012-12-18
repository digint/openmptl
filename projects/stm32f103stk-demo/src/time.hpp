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

#ifndef TIME_HPP_INCLUDED
#define TIME_HPP_INCLUDED

#include "resources.hpp"
#include <atomic>

typedef unsigned int systick_t;

class time
{
  friend CoreException<IrqNumber::CoreException::SysTick>;

  static std::atomic<systick_t> systick_count;
  //static systick_t systick_count;

public:

  static systick_t get_systick() {
    return systick_count.load(std::memory_order_relaxed);
    //return systick_count;
  }

  //  static constexpr long long nop_s = 11999940; // nop's per second when using Core::nop(n) (-Os, 72Mhz)
  static void nanosleep(unsigned int ns);
};

#endif
