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

#include "timepoint.hpp"
#include "time.hpp"

void Time::systick_isr(void) {
  Time::tick();
}

//systick_t time::systick_count;
std::atomic<systick_t> Time::systick_count;

void Time::nanosleep(unsigned int ns) {
  TimePoint end;
  end.set();
  end.add_ns(ns);
  TimePoint now;
  now.set();
  while(now < end)
    now.set();
}
