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

#ifndef ISR_WRAP_HPP_INCLUDED
#define ISR_WRAP_HPP_INCLUDED

namespace mptl {

struct isr_wrap {
// TODO: check virtual declaration
  isr_wrap() { }
  ~isr_wrap() { }
};

struct default_isr_wrap : public isr_wrap {
  ~default_isr_wrap () { while(1); }
};

} // namespace mptl

#endif //ISR_WRAP_HPP_INCLUDED
