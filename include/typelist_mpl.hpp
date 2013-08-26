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

#ifdef CONFIG_USE_STD_TUPLE
#  include <tuple>
#endif // CONFIG_USE_STD_TUPLE

// Hint: template debugging:
//template<typename T> struct incomplete;
//incomplete<bad_type> debug;

namespace mptl { namespace mpl {


////////////////////  list_cat_impl  ////////////////////

#if 0 // disabled: use typelist< Tp... > instead.

template<typename L, typename... R>
struct list_cat_impl {
  using type = typename L::type::template append_list< typename list_cat_impl< typename R::type... >::type >::type;
};
template<typename L, typename R>
struct list_cat_impl<L, R> {
  using type = typename L::type::template append_list< typename R::type >::type;
};

#endif // disabled


////////////////////  make_typelist  ////////////////////


/**
 * Create a typelist by successively adding elements to the provided
 * "list_type<Tp...>".
 *
 * NOTE: this removes any void types in the list.
 *
 * NOTE: used in combination with "sane_typelist<Tp...>::append<T>"
 * and the "_typelist_append<T, Tp...>" types to automatically unfold
 * lists of lists. (meditate a bit...)
 *
 * Usage:
 *
 *     mpl::make_typelist< list_type<>, Tp... >::type;
 *
 * Template arguments:
 *
 *   - list_type<>: must implement the "append< Tp >" trait.
 *   - Tp...      : any type derived by typelist_element.
 *
 */
template<typename list_type, typename... Tp>
struct make_typelist;
template<typename list_type>
struct make_typelist<list_type> {
  using type = list_type;
};
template<typename list_type, typename... Tp>
struct make_typelist<list_type, void, Tp...> {
  using type = typename make_typelist<list_type, Tp...>::type;
};
template<typename list_type, typename T0, typename... Tp>
struct make_typelist<list_type, T0, Tp...> {
  using type = typename make_typelist<typename list_type::template append<T0>, Tp...>::type;
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


#ifdef CONFIG_USE_STD_TUPLE  // experimental

template<typename func_type, std::size_t N = 0, typename... Tp>
inline typename std::enable_if<N == sizeof...(Tp), void>::type
for_each(std::tuple<Tp...> &)
{ }

template<typename func_type, std::size_t N = 0, typename... Tp>
inline typename std::enable_if<N < sizeof...(Tp), void>::type
for_each(std::tuple<Tp...> & t)
{
  using ele_type = typename std::tuple_element<N, std::tuple<Tp...> >::type;

  func_type::template command< ele_type >();
  for_each<func_type, N+1, Tp...>(t);
}
#endif // CONFIG_USE_STD_TUPLE


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

/** uses std::is_same<> */
template<typename T, typename... Args>
struct contains_impl {
  static constexpr bool value = false;
};
template<typename T, typename Head, typename... Args>
struct contains_impl<T, Head, Args...>
{
  static constexpr bool value =
    ( std::is_same<T, Head>::value ||
      contains_impl<T, Args...>::value );
};


////////////////////  contains_derived_from_impl  ////////////////////


/** uses std::is_base_of<> */
template<typename T, typename... Args>
struct contains_derived_from_impl {
  static constexpr bool value = false;
};
template<typename T, typename Head, typename... Args>
struct contains_derived_from_impl<T, Head, Args...>
{
  static constexpr bool value =
    ( std::is_base_of<T, Head>::value ||
      contains_derived_from_impl<T, Args...>::value );
};


////////////////////  all_derived_from_impl  ////////////////////


/** uses std::is_base_of<> */
template<typename T, typename... Args>
struct all_derived_from_impl {
  static constexpr bool value = true;
};
template<typename T, typename Head, typename... Args>
struct all_derived_from_impl<T, Head, Args...>
{
  static constexpr bool value =
    ( std::is_base_of<T, Head>::value &&
      all_derived_from_impl<T, Args...>::value );
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


#if 0  // fix this
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
#endif


////////////////////  all_true  ////////////////////


/**
 * Type trait providing value=true if every element Tp has value==true.
 */
template< typename... Tp >
struct all_true {
  static constexpr bool value = true;
};
template< typename Head, typename... Args >
struct all_true< Head, Args... > {
  static constexpr bool value = Head::value && all_true< Args... >::value;
};


} } // namespace mptl::mpl

#endif // RESOURCE_MPL_HPP_INCLUDED
