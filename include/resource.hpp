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

#include <isr.hpp>  // isr_t
#include <type_traits>

namespace mptl { namespace resource {

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


} // namespace mpl


////////////////////  unique  ////////////////////


template<typename T>
struct unique
{ };


////////////////////  irq  ////////////////////


struct irq_base
{ };

template<typename _irq_type, isr_t isr>
struct irq : irq_base
{
  using irq_type = _irq_type;
  static constexpr isr_t value = isr;
};


////////////////////  filter  ////////////////////

namespace filter {

  template<typename filter_type>
  struct is_base_of {
    template<typename T>
    using filter = std::is_base_of< filter_type, T >;
  };

} // namespace filter


////////////////////  resource::list  ////////////////////


template<typename L, typename... R>
struct list_cat {
  using type = typename L::type::template append_list< typename list_cat< typename R::type... >::type >::type;
};
template<typename L, typename R>
struct list_cat<L, R> {
  using type = typename L::type::template append_list< typename R::type >::type;
};


template<typename... Rm>
class list
{
protected:

public:
  using type = list< Rm... >;

  template<typename T>
  struct contains {
    static constexpr bool value = mpl::contains_impl< T, Rm... >::value;
  };

  template<typename... T>
  using push_back = list< Rm..., T... >;

  template<typename T>
  using append_list = typename T::template push_front< Rm... >;

  template<typename... T>
  using push_front = list< T..., Rm... >;

  using uniq = typename mpl::make_unique_list< list<>, Rm... >::type;

  template<typename condition_type>
  using filter = typename mpl::make_filtered_list< list<>, condition_type, Rm... >::type;

  template<typename T>
  using transform = list< typename T::template transform< Rm, type >::type... >;

  struct merge {
    using type = typename mpl::merge_impl< Rm... >::type;
  };

  template<typename cmd_type>
  static void for_each(void) {
    mpl::for_each_impl<cmd_type, Rm...>::command();
  }
};

} } // namespace mptl::resource

#endif // RESOURCE_HPP_INCLUDED
