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

#ifndef REGISTER_LIST_HPP_INCLUDED
#define REGISTER_LIST_HPP_INCLUDED

namespace mptl {

namespace mpl {

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
      condition_type::template filter<Head>::value,
      typename list_type::template append<Head>::type,
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
struct unique_list_impl;

template<typename list_type>
struct unique_list_impl<list_type> {
  using type = list_type;
};
template<typename list_type, typename Head, typename... Args>
struct unique_list_impl<list_type, Head, Args...> {
  using type = typename unique_list_impl<
    typename std::conditional<
      list_type::template contains<Head>::value,
      list_type,
      typename list_type::template append<Head>::type
      >::type,
    Args...>::type;
};


template<typename T, typename reglist_type, typename... Args>
struct execute_impl
{
  using type = execute_impl<T, reglist_type, Args...>;
  static void shoot() { }
};
template<typename T, typename reglist_type, typename Head, typename... Args>
struct execute_impl<T, reglist_type, Head, Args...>
{
  using type = execute_impl<T, reglist_type, Head, Args...>;

  static void shoot() {
    T::template shoot<Head, reglist_type>();
    execute_impl<T, reglist_type, Args...>::shoot();
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


} // namespace mpl


////////////////////  reglist  ////////////////////


template<typename... Rm>
class reglist
{
public:
  using list_type = reglist<Rm...>;

  template<typename T>
  struct append {
    using type = reglist<Rm..., T>;
  };

  template<typename T>
  struct contains {
    static constexpr bool value = mpl::contains_impl<T, Rm...>::value;
  };

  struct uniq {
    using type = typename mpl::unique_list_impl<reglist<>, Rm...>::type;
  };

  template<typename condition_type>
  struct filter {
    using type = typename mpl::make_filtered_list< reglist<>, condition_type, Rm... >::type;
  };

  struct merge {
    using type = typename mpl::merge_impl<Rm...>::type;
  };

  template<typename T>
  struct transform {
    using type = reglist< typename T::template transform<Rm, list_type >::type... >;
  };

  template<typename T>
  static void execute(void) {
    mpl::execute_impl<T, list_type, Rm...>::type::shoot();
  }
};


} // namespace mptl


#endif // REGISTER_LIST_HPP_INCLUDED
