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

#ifndef POORMAN_CSTRING_HPP_INCLUDED
#define POORMAN_CSTRING_HPP_INCLUDED

#include <type_traits>

namespace poorman {

/**
 * Prints "value" as hexadecimal number to buf, '0'-padded, zero
 * terminated.
 *
 * Negative values are printed as two's complement.
 *
 * NOTE: sizeof(buf) must be at least n+1
 */
template<typename Tp>
void itoa_hex(char * buf, Tp value, int n = sizeof(Tp)*2, char padding = '0') {
  using value_type = typename std::make_unsigned<Tp>::type;
  value_type val = (value_type)value;
  unsigned v;

  buf[n--] = 0;
  if(val == 0)
    buf[n--] = '0';
  while(n >= 0 && val) {
    v = val & 0xf;
    buf[n--] = v < 10 ? '0' + v : 'a' + v - 10;
    val >>= 4;
  };
  while(n >= 0) {
    buf[n--] = padding;
  }
  if(val) {
    buf[0] = '~';
  }
}

/**
 * Prints n digits of "value" to buf, right aligned, "padding"-padded,
 * zero terminated.
 *
 * Prints '~' as first character if "value" has more digits than n.
 *
 * NOTE: sizeof(buf) must be at least n+1
 */
template<typename Tp>
typename std::enable_if< std::is_unsigned< Tp >::value >::type
itoa(char * buf, Tp value, int n, char padding = '0') {
  buf[n--] = 0;
  if(value == 0)
    buf[n--] = '0';
  else {
    while(n >= 0 && value > 0) {
      buf[n--] = '0' + (value % 10);
      value /= 10;
    }
  }
  while(n >= 0) {
    buf[n--] = padding;
  }
  if(value > 0) {
    buf[0] = '~';
  }
}
template<typename Tp>
typename std::enable_if< std::is_signed< Tp >::value >::type
itoa(char * buf, Tp value, int n, char padding = '0') {
  *buf++ = '-';
  itoa(buf, (unsigned)(-value), n-1, padding);
}

} // namespace poorman

#endif // POORMAN_CSTRING_HPP_INCLUDED
