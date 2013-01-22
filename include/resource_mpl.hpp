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

#ifndef RESOURCE_MPL_HPP_INCLUDED
#define RESOURCE_MPL_HPP_INCLUDED

#include <type_traits>
#include <cstdint>

struct ResourceBase {
  /* Do not append a resource to the reg_tuple by default.  */
  template<typename RT, typename filter>
  using append_reg_tuple = RT;
};

template<uint32_t _value, uint32_t _mask>
struct reg_value {
  static constexpr uint32_t value = _value;
  static constexpr uint32_t mask = _mask;

  template<typename T>
  using combine = reg_value<value | T::value, mask | T::mask>;
};

template<typename R, uint32_t _value, uint32_t _mask = 0 >
struct SharedRegister : ResourceBase {
  typedef R reg_type;
  typedef reg_value<_value, _mask> value_type;

  /* Append reg_value<value, mask> to a reg_tuple, only if the filter  */
  /* matches our reg_type. Needed by make_reg_tuple.                   */
  template<typename RT, typename filter>
  using append_reg_tuple = typename std::conditional<
    std::is_same<filter, reg_type>::value,
    typename RT::template append<value_type>,
    RT>::type;
};

template<typename T>
struct UniqueResource : ResourceBase {
};


////////////////////  reg_tuple  ////////////////////


template<typename... Args>
struct reg_tuple_impl;

template<typename first, typename... Args>
struct reg_tuple_impl<first, Args...>  {
  typedef typename first::template combine<typename reg_tuple_impl<Args...>::value_type> value_type;
};
template<typename first>
struct reg_tuple_impl<first> {
  typedef first value_type;
};
template<>
struct reg_tuple_impl<> {
  typedef void value_type;
};

template<typename... Args>
struct reg_tuple : reg_tuple_impl<Args...> {
  template<typename T>
  using append = reg_tuple<Args..., T>;
};


/* Creates a reg_tuple<...>: list of reg_value<>, having a combined  */
/* value_type (e.g. or'ed register values for SharedRegister).       */
template<typename rt, typename filter, typename... Args>
struct make_reg_tuple;

template<typename rt, typename filter, typename first, typename... Args>
struct make_reg_tuple<rt, filter, first, Args...> {
  typedef typename make_reg_tuple<typename first::template append_reg_tuple<rt, filter>, filter, Args...>::type type;
};

template<typename rt, typename filter>
struct make_reg_tuple<rt, filter> {
  typedef rt type;
};


////////////////////  ResourceList  ////////////////////


template<typename... Args>
struct ResourceList {
  /* Append a ResourceList to a reg_tuple. Needed by make_reg_tuple in  */
  /* order to create its list from nested ResourceList's                */
  template<typename RT, typename filter>
  using append_reg_tuple = typename make_reg_tuple<RT, filter, Args...>::type;

  /* Combined reg_value for a single register (filter) */
  template<typename filter>
  using reg_value_type = typename make_reg_tuple<reg_tuple<>, filter, Args...>::type::value_type;

  template<typename T>
  static typename std::enable_if<!std::is_void<reg_value_type<T> >::value>::type set_register() {
    T::set(reg_value_type<T>::value, reg_value_type<T>::mask);
    // TODO: implement something like reset_register(), since on startup we don't care about the actual value of the register
    // T::store((T::reset_value & ~reg_value_type<T>::mask) | reg_value_type<T>::value);
  }
  template<typename T>
  static typename std::enable_if<std::is_void<reg_value_type<T> >::value>::type set_register() {
  }

  static void set_shared_register();  // implemented in core_resource.hpp (TODO: generic functions which sets all register)
};

#endif // RESOURCE_MPL_HPP_INCLUDED
