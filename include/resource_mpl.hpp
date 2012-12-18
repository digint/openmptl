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

// debug mpl
//template<typename T> struct incomplete;
//incomplete<resources> debug;


template<typename tag, uint32_t val>
struct SharedResource
{
  template<typename tag_filter>
  static constexpr uint32_t value(void) {
    return std::is_base_of<tag_filter, tag>::value ? val : 0;
  }
};

// resource types (tags, used in ResourceList::value() )
struct SharedResourceOR { };
struct CountedResource  { };


template<typename... Args>
struct ResourceList;

template<>
struct ResourceList<>
{
  template<typename tag_filter>
  static constexpr uint32_t value(void) { return 0; }
};

template<typename T, typename... Args>
struct ResourceList<T, Args...> {

#if 0
  // this does not work
  template<typename tag_filter, uint32_t val>
  static constexpr uint32_t unique_value(void) {
    static_assert(val <= 1, "UniqueResource is not unique!");
    return val;
  }
#endif

  template<typename tag_filter>
  static constexpr uint32_t value(void) {
    return
      std::is_base_of<SharedResourceOR, tag_filter>::value ? T::template value<tag_filter>() | ResourceList<Args...>::template value<tag_filter>() :
      //      __is_base_of(UniqueResource,   tag_filter) ? unique_value<tag_filter, T::template value<tag_filter>() + ResourceList<Args...>::template value<tag_filter>()>() :
      std::is_base_of<CountedResource,   tag_filter>::value ? T::template value<tag_filter>() + ResourceList<Args...>::template value<tag_filter>() :
      0; // TODO: somehow assert here
  }

  static void assert(void);
  static void configure(void);
};


#endif // RESOURCE_MPL_HPP_INCLUDED
