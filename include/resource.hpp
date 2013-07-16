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

#ifndef RESOURCE_HPP_INCLUDED
#define RESOURCE_HPP_INCLUDED

#include <type_traits>
#include "resource_mpl.hpp"

namespace mptl { namespace resource {


////////////////////  unique  ////////////////////


template<typename T>
struct unique
{ };


////////////////////  list_cat  ////////////////////


template<typename L, typename... R>
struct list_cat {
  using type = typename L::type::template append_list< typename list_cat< typename R::type... >::type >::type;
};
template<typename L, typename R>
struct list_cat<L, R> {
  using type = typename L::type::template append_list< typename R::type >::type;
};


////////////////////  resource::list  ////////////////////


template<typename... Tp>
class list
{
protected:

  template<typename T>
  struct is_base_of {
    template<typename Tf>
    using filter = std::is_base_of< T, Tf >;
  };

public:
  using type = list< Tp... >;

  template<typename T>
  struct contains {
    static constexpr bool value = mpl::contains_impl< T, Tp... >::value;
  };

  template<typename... T>
  using push_back = list< Tp..., T... >;

  template<typename T>
  using append_list = typename T::template push_front< Tp... >;

  template<typename... T>
  using push_front = list< T..., Tp... >;

  /**
   * Generic filter, takes a condition_type type trait as argument (which must
   * provide "::type::value").
   */
  template<typename condition_type>
  using filter = typename mpl::make_filtered_list< list<>, condition_type, Tp... >::type;

  /**
   * Provides a list holding only elements with types are of class T,
   * or derived from class T.
   */
  template<typename T>
  using filter_type = typename filter< is_base_of< T > >::type;

  /**
   * Provides a list filtered to hold at most one element of identical
   * type.
   *
   * NOTE: this has nothing to do with the resource::unique<> class.
   */
  using filter_unique = typename mpl::make_unique_list< list<>, Tp... >::type;

  /**
   * Provides a list, where each element Tp is replaced by:
   * "T::map<Tp, type>::type", where "type" is our list class type.
   */
  template<typename T>
  using map = list< typename T::template map< Tp, type >::type... >;

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
    mpl::for_each_impl<T, Tp...>::command();
  }
};

} } // namespace mptl::resource

#endif // RESOURCE_HPP_INCLUDED
