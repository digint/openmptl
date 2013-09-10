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
#include <poorman_cstring.hpp>

void DataRow::init(const char * name) {
  unsigned i = 0;
  value = 0;

  while(*name)
    buf[i++] = *name++;
  while(i < name_width)
    buf[i++] = ' ';
  buf[i++] = ':';
  buf[i++] = ' ';
  while(i < name_width + colon_width + value_width)
    buf[i++] = '0';
  buf[i++] = 0;
}

void DataRow::set(uint32_t _value) {
  if(value != _value) {
    value = _value;
    poorman::itoa(&buf[name_width + colon_width], value, value_width);
  }
}

void DataRowHex::set(uint32_t _value) {
  if(value != _value) {
    value = _value;
    poorman::itoa_hex(&buf[name_width + colon_width], value);
  }
}
