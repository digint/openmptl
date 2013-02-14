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

#ifndef POORMAN_OSTREAM_HPP_INCLUDED
#define POORMAN_OSTREAM_HPP_INCLUDED

#include <type_traits>

template<typename charT>
class poorman_ostream
{
  static char nibble(unsigned val) {
    val &= 0xf;
    return val < 10 ? '0' + val : 'a' + val - 10;
  }

public:
  typedef charT char_type;

  virtual poorman_ostream & put(char_type c) = 0;
  virtual poorman_ostream & puts(const char_type* s) = 0;
  virtual poorman_ostream & write(const char_type* s, unsigned int count) = 0;
  virtual poorman_ostream & flush() = 0;
  virtual poorman_ostream & endl() = 0;
  //  virtual poorman_ostream & widen(char_type c) { };  // TODO: implement in terminal_ostream

  /** hexadecimal output of any integral type */
  template<typename valT>
  typename std::enable_if<std::is_integral<valT>::value, poorman_ostream &>::type 
  friend operator <<(poorman_ostream & st, valT val)
  {
    for(int i = sizeof(valT) * 8 - 4; i >= 0; i -= 4) {
      st.put(nibble(val >> i));
    }
    return st;
  }

  friend poorman_ostream & operator<<(poorman_ostream & st, const char * s) {
    st.puts(s);
    return st;
  }

  poorman_ostream& operator<<(poorman_ostream& (*func)(poorman_ostream&)) {
    return func(*this);
  }
};

/** manipulator, flushes the output stream */
template<typename charT>
inline poorman_ostream<charT> & flush(poorman_ostream<charT> & st) {
  return st.flush();
}

/** manipulator, outputs newline and flushes the output stream */
template<typename charT>
inline poorman_ostream<charT> & endl(poorman_ostream<charT> & st) {
  return st.endl();
}

#endif // POORMAN_OSTREAM_HPP_INCLUDED
