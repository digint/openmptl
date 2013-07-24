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

#ifndef FIFO_STREAM_HPP_INCLUDED
#define FIFO_STREAM_HPP_INCLUDED

#include <poorman_ostream.hpp>

namespace mptl {

template<typename fifoT, typename deviceT>
class fifo_stream
: public poorman::ostream<typename fifoT::char_type>
{
  fifoT & fifo;

public:

  fifo_stream(fifoT & f) : fifo(f) { };

  typedef typename fifoT::char_type char_type;

  poorman::ostream<char_type> & put(char_type c) {
    fifo.push(c);
    return *this;
  }

  poorman::ostream<char_type> & write(const char_type* s, unsigned int count) {
    fifo.pushs(s, count);
    return *this;
  }

  poorman::ostream<char_type> & puts(const char_type* s) {
    fifo.pushs(s);
    return *this;
  }

  poorman::ostream<char_type> & flush() {
    deviceT::flush();
    return *this;
  }

  poorman::ostream<char_type> & endl() {
    if(deviceT::crlf)
      fifo.push('\r');
    fifo.push('\n');
    deviceT::flush();
    return *this;
  }
};

} // namespace mptl

#endif // FIFO_STREAM_HPP_INCLUDED
