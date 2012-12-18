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

#include "time.hpp"
#include "timepoint.hpp"

using namespace resources;

template<>
void systick::SysTickIrq::Handler(void) {
  time::systick_count.fetch_add(1, std::memory_order_relaxed);
  //  time::systick_count++;
}

//systick_t time::systick_count;
std::atomic<systick_t> time::systick_count;

void time::nanosleep(unsigned int ns) {
  TimePoint end;
  end.set();
  end.add_ns(ns);
  TimePoint now;
  now.set();
  while(now < end)
    now.set();
}
