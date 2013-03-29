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

#ifndef REGISTER_MANIP_HPP_INCLUDED
#define REGISTER_MANIP_HPP_INCLUDED

template<typename T>
class RegisterManip
{
public:
  using value_type = typename T::value_type;

  RegisterManip() { reg = T::load(); }
  RegisterManip(T val) : reg(val) { }


  RegisterManip<T> operator|(const T & rhs) { return RegisterManip<T>(reg | rhs); }

  RegisterManip<T> & operator=(const RegisterManip<T> & rhs) { return *this; }
  RegisterManip<T> & operator|=(value_type rhs) { this->reg |= rhs; return *this; }
  RegisterManip<T> & operator&=(value_type rhs) { this->reg &= rhs; return *this; }

  void store(void) { T::store(reg); }
  void set(value_type const set_mask) { reg |= set_mask; }
  void set(value_type const set_mask, value_type const clear_mask) { reg = (reg & ~clear_mask) | set_mask; }
  void clear(value_type const value) { reg &= ~value; }

  template<typename... Rb>
  void clear(void) {
    clear(mpl::reg_combined<value_type, Rb...>::clear_mask);
  }

  template<typename... Rc>
  void set(void) {
    using combined = mpl::reg_combined<value_type, Rc...>;
    set( combined::set_mask, combined::cropped_clear_mask );
  }

private:
  value_type reg;
};

#endif // REGISTER_MANIP_HPP_INCLUDED
