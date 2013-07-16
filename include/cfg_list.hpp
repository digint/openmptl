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

#ifndef CFG_LIST_HPP_INCLUDED
#define CFG_LIST_HPP_INCLUDED

#include <type_traits>

namespace mptl {

namespace cfg {

  struct config_base {
    template<typename T> using regmask_type = void;
    template<typename T> using resources = resource::list<>;
  };

} // namespace cfg

template< typename... CFG >
class periph_config_list
{
protected:
  template<typename T, typename Head, typename... Args>
  struct contains_impl {
    static constexpr bool value = ( std::is_base_of<T, Head>::value ||
                                    contains_impl<T, Args...>::value );
  };
  template<typename T, typename... Args>
  struct contains_impl<T, void, Args...> {
    /* ignore void */
    static constexpr bool value = contains_impl<T, Args...>::value;
  };
  template<typename T, typename Head>
  struct contains_impl<T, Head> {
    static constexpr bool value = std::is_base_of<T, Head>::value;
  };

public:

  template<typename T>
  struct contains {
    //    using type = contains<T>;
    static constexpr bool value = contains_impl<T, CFG...>::value;
  };
};

} // namespace mptl

#endif // CFG_LIST_HPP_INCLUDED
