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

#ifndef RESOURCE_MPL_HPP_INCLUDED
#define RESOURCE_MPL_HPP_INCLUDED

#include <type_traits>
#include <cstdint>
#include <compiler.h>

// Hint: template debugging:
//template<typename T> struct incomplete;
//incomplete<bad_type> debug;

namespace mptl { namespace resource { namespace mpl {

template<typename... Args>
struct merge_impl {
  /* note: assertion needs to be dependent of template parameter */
  // static_assert(sizeof...(Args), "cannot merge an empty argument list");
  using type = void;
};

template<typename Front, typename... Args>
struct merge_impl<Front, Args...> {
  using type = typename Front::template merge< typename merge_impl<Args...>::type >::type;
};

template<typename Front>
struct merge_impl<Front> {
  using type = Front;
};

template<typename list_type, typename condition_type, typename... Args>
struct make_filtered_list;

template<typename list_type, typename condition_type, typename... Args>
struct make_filtered_list<list_type, condition_type, void, Args...> {
  /* filter out "void" list member */
  using type = typename make_filtered_list<list_type, condition_type, Args...>::type;
};

template<typename list_type, typename condition_type, typename Head, typename... Args>
struct make_filtered_list<list_type, condition_type, Head, Args...> {
  using type = typename make_filtered_list<
    typename std::conditional<
      condition_type::template filter<Head>::type::value,
      typename list_type::template push_back<Head>,
      list_type
      >::type,
    condition_type,
    Args...
    >::type;
};

template<typename list_type, typename condition_type>
struct make_filtered_list<list_type, condition_type> {
  using type = list_type;
};


template<typename list_type, typename... Args>
struct make_unique_list;

template<typename list_type>
struct make_unique_list<list_type> {
  using type = list_type;
};
template<typename list_type, typename Head, typename... Args>
struct make_unique_list<list_type, Head, Args...> {
  using type = typename make_unique_list<
    typename std::conditional<
      list_type::template contains<Head>::value,
      list_type,
      typename list_type::template push_back<Head>
      >::type,
    Args...>::type;
};


template<typename cmd_type, typename... Args>
struct for_each_impl {
  static void command() { }
};
template<typename cmd_type, typename Head, typename... Args>
struct for_each_impl<cmd_type, Head, Args...> {
  static void command() {
    cmd_type::template command<Head>();
    for_each_impl<cmd_type, Args...>::command();
  }
};


template<typename T, typename... Args>
struct contains_impl {
  static constexpr bool value = false;
};
template<typename T, typename Head, typename... Args>
struct contains_impl<T, Head, Args...>
{
  static constexpr bool value = ( std::is_same<Head, T>::value ||
                                  contains_impl<T, Args...>::value );
};


/**
 * Returns the first element in list, or void if list is
 * empty. Asserts if more than one element is in list.
 */
template<typename... T>
struct unique_element_impl {
  static_assert(sizeof...(T) <= 1, "list contains more than one element");
  using type = void;
};
template<typename T>
struct unique_element_impl<T> {
  using type = T;
};

} } } // namespace mptl::resource::mpl

#endif // RESOURCE_MPL_HPP_INCLUDED
