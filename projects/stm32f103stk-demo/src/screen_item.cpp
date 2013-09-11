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

#include "screen_item.hpp"

void DataRow::init_buf(const char * name) {
  unsigned i = 0;

  while(*name)
    buf[i++] = *name++;
  while(i < name_width)
    buf[i++] = ' ';
  buf[i++] = ':';
  buf[i++] = ' ';

  print_value();
}

DataRow::DataRow(const char * name, uint32_t _value, NumberBase _numbase, bool _inverted)
  : ScreenItem(_inverted), value(_value), numbase(_numbase)
{
  init_buf(name);
}

DataRow::DataRow(ScreenItemList & item_list, const char * name, uint32_t _value, NumberBase _numbase, bool _inverted)
  : ScreenItem(item_list, _inverted), value(_value), numbase(_numbase)
{
  init_buf(name);
}

void DataRow::print_value(void) {
  if(numbase == NumberBase::decimal)
    poorman::itoa(&buf[name_width + colon_width], value, value_width, padding_dec);
  else
    poorman::itoa_hex(&buf[name_width + colon_width], value, value_width, padding_hex);
}

void DataRow::set(uint32_t _value) {
  if(value != _value) {
    value = _value;
    print_value();
  }
}
