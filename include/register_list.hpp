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

namespace mpl {

template<typename... Args>
struct merge_impl {
  /* note: assertion needs to be dependent of template parameter */
  static_assert(sizeof...(Args), "cannot merge an empty argument list");
};

template<typename Front, typename... Args>
struct merge_impl<Front, Args...> {
  using type = typename Front::template merge< typename merge_impl<Args...>::type >::type;
};

template<typename Front>
struct merge_impl<Front> {
  using type = Front;
};


template<typename list_type, typename reg_type, typename... Args>
struct make_filtered_list;

template<typename list_type, typename reg_type, typename... Args>
struct make_filtered_list<list_type, reg_type, void, Args...> {
  /* filter out "void" list member */
  using type = typename make_filtered_list<list_type, reg_type, Args...>::type;
};

template<typename list_type, typename reg_type, typename Head, typename... Args>
struct make_filtered_list<list_type, reg_type, Head, Args...> {
  using type = typename make_filtered_list<
    typename std::conditional<
      std::is_same<typename Head::reg_type, reg_type>::value,
      typename list_type::template append<Head>::type,
      list_type
      >::type,
    reg_type,
    Args...
    >::type;
};

template<typename list_type, typename reg_type>
struct make_filtered_list<list_type, reg_type> {
  using type = list_type;
};

} // namespace mpl


////////////////////  reglist  ////////////////////


template<typename... Rm>
struct reglist
{
  using type = reglist<Rm...>;

  template<typename T>
  struct append {
    using type = reglist<Rm..., T>;
  };

  template<typename reg_type>
  struct filter {
    using type = typename mpl::make_filtered_list< reglist<>, typename reg_type::reg_type, Rm... >::type;
  };

  struct merge {
    using type = typename mpl::merge_impl<Rm...>::type;
  };
};
