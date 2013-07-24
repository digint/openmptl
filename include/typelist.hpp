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

#ifndef TYPELIST_HPP_INCLUDED
#define TYPELIST_HPP_INCLUDED

#include <type_traits>
#include "typelist_mpl.hpp"

#ifdef CONFIG_USE_STD_TUPLE
#  include <tuple>
#endif // CONFIG_USE_STD_TUPLE

namespace mptl {

////////////////////  sane_typelist  ////////////////////


/**
 * NOTE: Don't use this class directly, use typelist<> instead, which
 * wraps into a sane_typelist.
 *
 * NOTE: This class treats all Tp identical, which means that
 * "sane_typelist< A, sane_typelist<B> >" will not expand to sane_typelist<A, B>.
 *
 */
template<typename... Tp>
class sane_typelist
{
  /**
   * firends are *DISABLED* by default, this is subject to change.
   *
   * friends, hmpf. needed to hide the "_typelist_append<T, Tp...>"
   * traits from the list element types. We try to be nice and don't
   * pollute the namespace of every type derived from the
   * typelist_element class.
   *
   * But then, we hide the append<> trait from the user. well, he
   * still can use "list<list<A>, B>" instead of
   * "list<A>::append<B>". (which is usually all you need for mpl)
   *
   * If you know of a nicer way to unfold "typelist<A,typelist<B>>" into
   * "typelist<A,B>", please contact me!
   *
   * TODO: note about boost and why we don't use it here.
   */
#ifdef CONFIG_USE_FRIENDS
  template<typename...>                       friend class  sane_typelist;
  template<typename, typename...>             friend struct mpl::make_typelist;
  template<typename, typename... T>           friend struct mpl::make_unique_list;
  template<typename, typename, typename... T> friend struct mpl::make_filtered_list;
#else
public: // see note about friends above
#endif

  template<typename T>
  struct filter_element_tag {
    template<typename Tf>
    using filter = std::is_same<
      typename T::__typelist_tag_type,
      typename Tf::_typelist_tag_type
      >;
  };

  template<typename T>
  struct filter_is_base_of {
    template<typename Tf>
    using filter = std::is_base_of<T, Tf>;
  };

  template<typename T, typename... U>
  struct _typelist_append {
    using type = typename T::template push_front< U... >::type;
  };

  /* private, since "list::push_front< typelist< X > >" would not be a sane list */
  template<typename... T>
  using push_front = sane_typelist< T..., Tp... >;

  /* private, since "list::push_back< typelist< X > >" would not be a sane list */
  template<typename... T>
  using push_back = sane_typelist< Tp..., T... >;

public:

  using type = sane_typelist< Tp... >;

#ifdef CONFIG_USE_STD_TUPLE
  using tuple_type = std::tuple<Tp...>;
#endif // CONFIG_USE_STD_TUPLE

  /**
   * Append an element (derived from typelist_element) or another
   * sane_typelist to the list.
   *
   * Note that the append<> trait is required by the list_type<> in:
   *
   *     mpl::make_typelist< list_type, Tp... >
   *
   * which sanitizes and performss recursive unfolding (e.g. unfold
   * list of lists using the "::append<>" trait to successively
   * add elements to the (initially empty) list provided.
   */
  template<typename T>
  using append = typename T::template _typelist_append<T, Tp...>::type;

  /**
   * Type trait, providing ::value. True if the list contains an
   * element of exact type T, false otherwise.
   */
  template<typename T>
  struct contains {
    static constexpr bool value = mpl::contains_impl< T, Tp... >::value;
  };

  /**
   * Type trait, providing ::value. True if the list contains an
   * element derived from (or same) type T, false otherwise.
   */
  template<typename T>
  struct contains_derived_from {
    static constexpr bool value = mpl::contains_derived_from_impl< T, Tp... >::value;
  };

  /**
   * Type trait, providing ::value. True if all elements in the list
   * are derived from (or same) type T, false otherwise.
   */
  template<typename T>
  struct all_derived_from {
    static constexpr bool value = mpl::all_derived_from_impl< T, Tp... >::value;
  };

  /**
   * Generic filter, takes a condition_type type trait as argument (which must
   * provide "::type::value").
   */
  template<typename condition_type>
  using filter = typename mpl::make_filtered_list< sane_typelist<>, condition_type, Tp... >::type;

  /**
   * Provides a list holding only elements with types are of class T,
   * or derived from class T.
   */
  template<typename T>
  using filter_type = typename filter< filter_is_base_of< T > >::type;

  // TODO
  template<typename T>
  using filter_tag = typename filter< filter_element_tag< T > >::type;

  /**
   * Provides a list filtered to hold at most one element of identical
   * type.
   *
   * (This is not related with the typelist_unique_element<> class.)
   */
  using filter_unique = typename mpl::make_unique_list< sane_typelist<>, Tp... >::type;

  /**
   * Provides a list, where each element Tp is replaced by:
   * "T::map<Tp, type>::type", where "type" is our list class type.
   */
  template<typename T>
  using map = sane_typelist< typename T::template map< Tp, type >::type... >;

  /**
   * Provides the first element in list, or void if the list is
   * empty. Asserts at compile-time that the list does not hold more
   * than one element.
   */
  struct unique_element {
    using type = typename mpl::unique_element_impl<Tp...>::type;
  };

  /**
   * Provides a type defined by T::pack<...>::type, by passing all elements
   * to: "T::pack<Tp...>".
   */
  template<typename T>
  struct pack {
    using type = typename T::template pack< Tp... >::type;
  };

  /**
   * Calls static function T::command() for each element Tp.
   */
  template<typename T>
  static void for_each(void) {
#ifdef CONFIG_USE_STD_TUPLE
    auto tup = tuple_type();
    mpl::for_each<T>(tup);
    // mpl::for_each<T>(tuple_type());
#else
    mpl::for_each_impl<T, Tp...>::command();
#endif // CONFIG_USE_STD_TUPLE
  }

  /**
   * Check if elements derived from typelist_unique_element class are really
   * unique in the list.
   */
  static bool check(void) {
    using unique_check = typename mpl::unique_check_impl< type >;
    // TODO: fix this
    return true;
  }
};


////////////////////  list  ////////////////////


/**
 * MPL-style list, holding type traits.
 * Automatically expands to sane_typelist.
 *
 * Example: the expression "typelist<A,void,typelist<B>>" is of type "sane_typelist<A,B>".
 */
template<typename... Tp>
using typelist = typename mpl::make_typelist< sane_typelist<>, Tp... >::type;


////////////////////  typelist_element  ////////////////////


/**
 * Base type for all elements which want to be part of the
 * self-expanding typelist.
 */
class typelist_element {
#ifdef CONFIG_USE_FRIENDS
  template<typename...>                       friend class  sane_typelist;
  template<typename, typename...>             friend struct mpl::make_typelist;
  template<typename, typename... T>           friend struct mpl::make_unique_list;
  template<typename, typename, typename... T> friend struct mpl::make_filtered_list;
#else
public: // see note about friends above
#endif

  template<typename T, typename... Tp>
  struct _typelist_append {
    using type = sane_typelist<Tp..., T>;
  };
};


////////////////////  unique  ////////////////////


template<typename unique_type>
struct typelist_unique_element : typelist_element
{
  /* tag type, used in reglist::filter_type<> */
  using _typelist_unique_type = unique_type;
};


////////////////////  list_cat  ////////////////////

#if 0 // disabled: use typelist< Tp... > instead.

template<typename L, typename... R>
using list_cat = typename mpl::list_cat_impl<L, R...>::type;

#endif // disabled

} // namespace mptl

#endif // TYPELIST_HPP_INCLUDED
