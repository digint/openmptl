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
  /* Do not append a resource to the filtered_list by default.  */
  template<typename T, typename Filter>
  using append_filtered_list = T;
};


////////////////////  SharedRegister  ////////////////////


template<uint32_t _set_mask, uint32_t _clear_mask>
struct reg_value {
  static constexpr uint32_t set_mask   = _set_mask;
  static constexpr uint32_t clear_mask = _clear_mask;

  template<typename T>
  using combine = reg_value<set_mask | T::set_mask, clear_mask | T::clear_mask>;
};

template<typename R, uint32_t _set_mask, uint32_t _clear_mask = 0 >
struct SharedRegister : ResourceBase {
  typedef R reg_type;
  typedef reg_value<_set_mask, _clear_mask> value_type;

  /* If the filter matches our reg_type, append reg_value<value, mask>  */
  /* to the filtered_list.                                              */
  template<typename T, typename Filter>
  using append_filtered_list = typename std::conditional<
    std::is_same<Filter, reg_type>::value,
    typename T::template append<value_type>,
    T>::type;
};


////////////////////  UniqueResource  ////////////////////


template<typename T, typename U>
struct assert_different {
  static constexpr bool value = !std::is_same<T, U>::value;
  static_assert(value, "UniqueResource<...> is not unique! (see compiler output \"assert_different<...>\" above)");
};

template<typename T, typename... Args>
struct unique_types;
template<typename T0, typename T1, typename... Args>
struct unique_types<T0, T1, Args...> {
  /* not very efficient, O(n^2) */
  static constexpr bool value =
    assert_different<T0, T1>::value &&
    unique_types<T0, Args...>::value &&
    unique_types<T1, Args...>::value;
};
template<typename T>
struct unique_types<T> {
  static constexpr bool value = true;
};


template<typename... Args>
struct unique_list {
  static constexpr bool is_unique = unique_types<Args...>::value;

  template<typename T>
  using append = unique_list<T, Args...>;

  template<typename U>
  struct combine : U::template append<Args...> { };
};

struct UniqueResourceFilter
{ };

template<typename T>
struct UniqueResource : ResourceBase {
  typedef unique_list<T> value_type;

  template<typename Tf, typename Filter>
  using append_filtered_list = typename std::conditional<
    std::is_same<Filter, UniqueResourceFilter>::value,
    typename Tf::template append<value_type>,
    Tf>::type;
};


////////////////////  filtered_list  ////////////////////


template<typename... Args>
struct filtered_list_impl;

template<typename Head, typename... Args>
struct filtered_list_impl<Head, Args...>  {
  typedef typename Head::template combine<typename filtered_list_impl<Args...>::value_type> value_type;
};
template<typename Head>
struct filtered_list_impl<Head> {
  typedef Head value_type;
};
template<>
struct filtered_list_impl<> {
  /* value_type is void for empty filtered list */
  typedef void value_type;
};

template<typename... Args>
struct filtered_list : filtered_list_impl<Args...> {
  template<typename T>
  using append = filtered_list<Args..., T>;
};


/* Creates a filtered_list<...>: list of value_types from resource    */
/* classes, with it's type being the combined value_type (e.g. or'ed  */
/* register values for SharedRegister).                               */
template<typename T, typename Filter, typename... Args>
struct make_filtered_list;

template<typename T, typename Filter, typename Head, typename... Args>
struct make_filtered_list<T, Filter, Head, Args...> {
  typedef typename make_filtered_list<typename Head::template append_filtered_list<T, Filter>, Filter, Args...>::type type;
};

template<typename T, typename Filter>
struct make_filtered_list<T, Filter> {
  typedef T type;
};


////////////////////  ResourceList  ////////////////////


template<typename... Args>
struct ResourceList {
  /* Append a ResourceList to a filtered_list. Needed by make_filtered_list in  */
  /* order to create its list from nested ResourceList's                */
  template<typename T, typename Filter>
  using append_filtered_list = typename make_filtered_list<T, Filter, Args...>::type;

  /* Combined resource value_type (e.g. reg_value for SharedRegister */
  template<typename Filter>
  using resource_value_type = typename make_filtered_list<filtered_list<>, Filter, Args...>::type::value_type;

  /* Set the register specified by T from SharedResource<T, x, y> */
  template<typename R>
  static typename std::enable_if<!std::is_void<resource_value_type<R> >::value>::type set_register() {
    R::set(resource_value_type<R>::set_mask, resource_value_type<R>::clear_mask);
    // TODO: implement something like reset_register(), since on startup we don't care about the actual value of the register
    // T::store((T::reset_value & ~resource_value_type<T>::clear_mask) | resource_value_type<T>::set_mask);
  }
  template<typename R>
  static typename std::enable_if<std::is_void<resource_value_type<R> >::value>::type set_register() {
    /* The register is not listed, do nothing */
  }

  static constexpr void assert_unique() {
    static_assert(resource_value_type<UniqueResourceFilter>::is_unique, "ResourceList contains UniqueResource which are not unique. (see compile messages above)");
  }

  static void set_shared_register();  // implemented in core_resource.hpp (TODO: generic functions which sets all register)
};

#endif // RESOURCE_MPL_HPP_INCLUDED
