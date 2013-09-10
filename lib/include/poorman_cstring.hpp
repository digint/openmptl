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

namespace poorman {

/**
 * Prints "value" as hexadecimal number to buf, zero terminated.
 *
 * NOTE: sizeof(buf) must be at least sizeof(Tp)*2+1
 */
template<typename Tp>
void itoa_hex(char * buf, Tp value) {
  unsigned v;
  for(int i = sizeof(Tp) * 8 - 4; i >= 0; i -= 4) {
    v = (value >> i) & 0xf;
    *buf++ = v < 10 ? '0' + v : 'a' + v - 10;
  }
  *buf = 0;
}

/**
 * Prints n digits of "value" to buf, right aligned, zero terminated.
 *
 * NOTE: sizeof(buf) must be at least n+1
 * Simple, but terrible performance.
 */
template<typename Tp>
void itoa(char * buf, Tp value, int n) {
  buf[n] = 0;
  while(--n >= 0) {
    buf[n] = '0' + (value % 10);
    value /= 10;
  }
}

} // namespace poorman

#endif // POORMAN_CSTRING_HPP_INCLUDED
