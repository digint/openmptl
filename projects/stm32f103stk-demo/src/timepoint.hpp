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

#ifndef TIMEPOINT_HPP_INCLUDED
#define TIMEPOINT_HPP_INCLUDED

#include "printf.h"
#include "time.hpp"
#include "kernel.hpp"

class TimePoint
{
  using time = Kernel::time;
protected:

  typedef decltype(time::systick::get_counter()) counter_type;

  systick_t     systick;
  counter_type  counter;     // note: counter decreases in time

public:

  static constexpr auto counter_base = time::systick::reload_value;
  static constexpr auto ps_per_tick  = time::systick::ps_per_tick;

  void set() volatile {
    systick = time::get_systick();
    counter = time::systick::get_counter();
  }

  systick_t    get_systick() const { return systick; }
  counter_type get_counter() const { return counter; }

  /** add nanoseconds */
  void add_ns(unsigned int ns) {
    counter_type counter_ticks = (ns * 1000) / time::systick::ps_per_tick + 1; // always rounded up
    systick += counter_ticks / counter_base;
    counter_type wait_frac = counter_ticks % counter_base;
    if(wait_frac > counter) {
      systick++;
      counter += counter_base;
    }
    counter -= wait_frac;
  }

  unsigned int get_ns(void) {
    unsigned int t = systick * counter_base - counter + counter_base;
    t = t * 1000 / Kernel::systick::ps_per_tick;
    return t;
  };

  bool operator<(TimePoint & tp) const {
    if(systick < tp.systick)
      return true;
    if(systick > tp.systick)
      return false;
    if(counter > tp.counter)
      return true;
    return false;
  }

  char * c_str(char * buf) const {
    sprintf(buf, "%u:%08x", systick, counter_base - counter);
    return buf;
  }
};

class TimePointDiff
: public TimePoint
{
public:
  /** creates a TimePoint from tp1 - tp2.
   * if tp2 > tp1, the result is undefined.
   */
  TimePointDiff(const TimePoint & tp1, const TimePoint & tp2) {
    systick = tp1.get_systick() - tp2.get_systick();
    counter = 0;
    counter_type c1 = counter_base - tp1.get_counter();
    counter_type c2 = counter_base - tp2.get_counter();
    if(c1 > c2) {
      counter = counter_base - (c1 - c2);
    }
    else {
      //      counter = counter_base - (c2 - c1);
      counter = (c2 - c1);
      systick--;
    }
  }
};

class TimeMeasurement
{
public:

  TimePoint tp_start;
  TimePoint tp_end;

  void start() volatile { tp_start.set(); }
  void stop()  volatile { tp_end.set();  }

  /** diff time in nanoseconds */
  unsigned int diff(void) {
    TimePointDiff tp(tp_end, tp_start);
    return tp.get_ns();
  }
};

#endif // TIMEPOINT_HPP_INCLUDED
