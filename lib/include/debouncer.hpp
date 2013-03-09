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

#ifndef DEBOUNCER_HPP_INCLUDED
#define DEBOUNCER_HPP_INCLUDED

/**
 * Debounce a value on a given time base
 *
 * NOTE: time_base must implement get_systick() function, returning a growing value
 */
template<typename T, typename time_base, unsigned wait_time = 10>
class Debouncer
{
  T value;
  T current;
  unsigned set_time = 0;

public:

  Debouncer(T _value) : value(_value), current(_value) { };

  void set(T new_value) {
    if(current != new_value) {
      current = new_value;
      set_time = time_base::get_systick();
    }
  }

  bool get(T & _value) {
    bool changed = false;
    if((value != current) && (time_base::get_systick() + wait_time > set_time)) {
      value = current;
      changed = true;
    }

    _value = value;
    return changed;
  }
};

#endif // DEBOUNCER_HPP_INCLUDED
