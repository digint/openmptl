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


////////////////////  list_cat_impl  ////////////////////


template<typename L, typename... R>
struct list_cat_impl {
  using type = typename L::type::template append_list< typename list_cat_impl< typename R::type... >::type >::type;
};
template<typename L, typename R>
struct list_cat_impl<L, R> {
  using type = typename L::type::template append_list< typename R::type >::type;
};


////////////////////  make_sane_list  ////////////////////


template<typename list_type, typename... Tp>
struct make_sane_list;
template<typename list_type>
struct make_sane_list<list_type> {
  using type = list_type;
};
template<typename list_type, typename... Tp>
struct make_sane_list<list_type, void, Tp...> {
  using type = typename make_sane_list<list_type, Tp...>::type;
};
template<typename list_type, typename T0, typename... Tp>
struct make_sane_list<list_type, T0, Tp...> {
  using type = typename make_sane_list<typename list_type::template append<T0>, Tp...>::type;
};


////////////////////  make_filtered_list  ////////////////////


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


////////////////////  make_unique_list  ////////////////////


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


////////////////////  for_each_impl  ////////////////////


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


////////////////////  contains_impl  ////////////////////


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


////////////////////  unique_element_impl  ////////////////////


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


////////////////////  unique_check_impl  ////////////////////


struct unique_base
{ };

/**
 * Returns the first element in list, or void if list is
 * empty. Asserts if more than one element is in list.
 */
template<typename Tp>
struct unique_check_impl {

  template<typename T>
  struct filter_real_type {
    template<typename Tf>
    using filter = std::is_same< typename T::real_type, typename Tf::real_type >;
  };

  struct map_unique {
    template<typename T, typename list_type>
    struct map {
      /* filter list by real_type, and map to unique_element */
      using filtered_list = typename list_type::template filter_type< filter_real_type<T> >::type;
      using type = typename filtered_list::unique_element::type;
    };
  };

  using filtered_list = typename Tp::template filter_type< unique_base >::type;
  using type = typename filtered_list::template map< map_unique >::type;
  static constexpr bool value = std::is_void<type>::value;
};

} } } // namespace mptl::resource::mpl

#endif // RESOURCE_MPL_HPP_INCLUDED
