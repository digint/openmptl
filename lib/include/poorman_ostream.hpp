/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright (C) 2012-2017 Axel Burri <axel@tty0.ch>
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

#ifndef POORMAN_OSTREAM_HPP_INCLUDED
#define POORMAN_OSTREAM_HPP_INCLUDED

#include <type_traits>

namespace poorman {

template<typename Tp>
class ostream
{
  static char nibble(unsigned val) {
    val &= 0xf;
    return val < 10 ? '0' + val : 'a' + val - 10;
  }

public:
  using char_type = Tp;

  virtual ostream & put(char_type c) = 0;
  virtual ostream & puts(const char_type* s) = 0;
  virtual ostream & write(const char_type* s, unsigned int count) = 0;
  virtual ostream & flush() = 0;
  virtual ostream & endl() = 0;
  //  virtual ostream & widen(char_type c) { };  // TODO: implement in terminal_ostream

  /** hexadecimal output of any integral type */
  template<typename valT>
  typename std::enable_if<std::is_integral<valT>::value, ostream &>::type
  friend operator <<(ostream & st, valT val)
  {
    for(int i = sizeof(valT) * 8 - 4; i >= 0; i -= 4) {
      st.put(nibble(val >> i));
    }
    return st;
  }

  friend ostream & operator<<(ostream & st, const char * s) {
    st.puts(s);
    return st;
  }

  ostream& operator<<(ostream& (*func)(ostream&)) {
    return func(*this);
  }
};

/** manipulator, flushes the output stream */
template<typename Tp>
inline ostream<Tp> & flush(ostream<Tp> & st) {
  return st.flush();
}

/** manipulator, outputs newline and flushes the output stream */
template<typename Tp>
inline ostream<Tp> & endl(ostream<Tp> & st) {
  return st.endl();
}

} // namespace poorman

#endif // POORMAN_OSTREAM_HPP_INCLUDED
