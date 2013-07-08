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

#ifndef DEBOUNCER_HPP_INCLUDED
#define DEBOUNCER_HPP_INCLUDED

/**
 * Debounce a value on a given time base
 *
 * NOTE: time_func() must return a growing value!
 */
template< typename T,
          T (*poll_func)(void),         // polling function
          unsigned (*time_func)(void),  // timer get function (must return a growing value)
          unsigned int time_freq,       // time_func frequency in hz
          unsigned wait_time_ms = 50    // debounce time in milliseconds
          >
class debouncer
{
  T value;
  T current;
  unsigned hold_time;

  static constexpr unsigned wait_time_ticks = (((unsigned long long)time_freq * (unsigned long long)wait_time_ms) / 1000L);

public:

  debouncer(T _value) : value(_value), current(_value), hold_time(0) { };

  /**
   * Feed debouncer with a new value.
   * Returns true if the value has changed.
   */
  bool poll(void) {
    T new_value = poll_func();
    unsigned now = time_func();
    if(current != new_value) {
      current = new_value;
      hold_time = now + wait_time_ticks;
    }

    if((hold_time > now) || (value == current))
      return false;

    value = current;
    return true;
  }

  T get(void) const {
    return value;
  }

  operator T() const {
    return get();
  }
};

#endif // DEBOUNCER_HPP_INCLUDED
