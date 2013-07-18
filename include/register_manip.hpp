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

// TODO: implement make_reg<> class, analog to
// std::make_tuple<>. This way we can put them into a std::tuple<>
// class and perform the merge as instances
//
// ^^^ allows to mix compile-time traits with reg_manip<> types !


#ifndef REGISTER_MANIP_HPP_INCLUDED
#define REGISTER_MANIP_HPP_INCLUDED

#include <compiler.h>

namespace mptl {

// TODO: rename reg<>
template<typename T>
class reg_manip
{
public:
  using value_type = typename T::value_type;

  // TODO: wrong. only construct from known regmask<> traits, or
  // set/clear mask tuple. NEVER do load() magic in constructors!
  reg_manip() : reg(T::load()) { }
  reg_manip(value_type val) : reg(val) { }

  reg_manip<T> operator|(const T & rhs) { return reg_manip<T>(reg | rhs); }

  reg_manip<T> & operator=(const reg_manip<T> & rhs) { return *this; }
  reg_manip<T> & operator|=(value_type rhs) { this->reg |= rhs; return *this; }
  reg_manip<T> & operator&=(value_type rhs) { this->reg &= rhs; return *this; }

  constexpr operator value_type() { return reg; }

  void __always_inline load(void) { reg = T::load(); }
  void __always_inline store(void) const { T::store(reg); }

  void __always_inline set(value_type const set_mask) { reg |= set_mask; }
  void __always_inline set(value_type const set_mask, value_type const clear_mask) { reg = (reg & ~clear_mask) | set_mask; }
  void __always_inline clear(value_type const value) { reg &= ~value; }

  template<typename... Rm>
  void __always_inline clear(void) {
    clear(T::template merge<Rm...>::type::clear_mask);
  }

  template<typename... Rm>
  void __always_inline set(void) {
    using merged_type = typename T::template merge<Rm...>::type;
    set( merged_type::set_mask, merged_type::cropped_clear_mask );
  }

  reg_manip<T> & 

private:
  value_type reg;
};

} // namespace mptl

#endif // REGISTER_MANIP_HPP_INCLUDED
