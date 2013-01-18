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
#include <cstdint>


template<uint32_t... Args>
struct bit_or;

template< uint32_t first, uint32_t... Args >
struct bit_or<first, Args...> {
  static constexpr uint32_t value = first | bit_or<Args...>::value;
};

template< uint32_t last >
struct bit_or<last> {
  static constexpr uint32_t value = last;
};

template<>
struct bit_or<> {
  static constexpr uint32_t value = 0;
};



template< typename R, uint32_t _value, uint32_t _mask = 0 >
struct SharedRegister {
  static constexpr uint32_t value = _value;
  static constexpr uint32_t mask = _mask;
  typedef R reg_type;

  static void set(void) {
    R::set(value, mask);
  }
};

 
template<class... Args>
struct is_member;

template<class T, class Arg, class... Args>
struct is_member<T, Arg, Args...> : std::conditional<std::is_same<T, Arg>::value, std::true_type, is_member<T, Args...> >::type
{ };
template<class T, class Arg>
struct is_member<T, Arg> : std::is_same<T, Arg>::type
{ };
template<class T>
struct is_member<T> : std::false_type::type
{ };



template <class T, class... Args>
struct filter_type;



template<class... Args>
struct sequence;

template <class Arg, class... Args>
struct sequence<Arg, Args...>
{
  static void set() {
    Arg::reg_type::set(bit_or<Arg::value, Args::value...>::value);
  }

  template<class... SR>
  static void set_resources()
  {
    typedef typename filter_type<Arg, SR...>::type filtered;
    filtered::set();
    sequence<Args...>::template set_resources<SR...>();
  }

  template<class T>
  struct has_member : is_member<T, Arg, Args...>::type
  { };
};

template<>
struct sequence<>
{
  template<class T>
  struct has_member : std::false_type::type
  { };

  template<class... SR>
  static void set_resources()
  { }
};



template <class U, class V>
struct sequence_cat;

template <class... U, class... V>
struct sequence_cat<sequence<U...>, sequence<V...>> {
  typedef sequence<U..., V...> type;
};

template <class... V>
struct sequence_cat<void, sequence<V...>> {
  typedef sequence<V...> type;
};

template <class... U>
struct sequence_cat<sequence<U...>, void> {
  typedef sequence<U...> type;
};

template <>
struct sequence_cat<void, void> {
  typedef void type;
};




template <class T, class Arg, class... Args>
struct filter_type<T, Arg, Args...>
{
  typedef typename
  std::conditional<std::is_same<typename Arg::reg_type, T>::value,
                   typename sequence_cat<sequence<Arg>,
                                         typename filter_type<T, Args...>::type
                                         >::type,
                   typename filter_type<T, Args...>::type
                   >::type type;       
};

template <class T, class Arg>
struct filter_type<T, Arg>
{
    typedef typename
    std::conditional<std::is_same<typename Arg::reg_type, T>::value, sequence<Arg>, void>::type
    type;
};




template <class S, class... Args>
struct unique_type_impl;

template <class S, class Arg, class... Args>
struct unique_type_impl<S, Arg, Args...>
{
  typedef typename
  std::conditional<S::template has_member<Arg>::value,
                   typename unique_type_impl<S, Args...>::type,
                   typename unique_type_impl<typename sequence_cat<S, sequence<Arg> >::type,
                                             Args...
                                             >::type
                   >::type type;
};

template <class S, class Arg>
struct unique_type_impl<S, Arg>
{
    typedef typename
    std::conditional<S::template has_member<Arg>::value,
                     S,
                     typename sequence_cat<S, sequence<Arg> >::type>::type
    type;
};

template<class... Args>
struct unique_type : unique_type_impl<sequence<>, Args...>
{ };




template< typename... SR >
struct ResourceList;

template<>
struct ResourceList<> {
  static void set() { }
};

template<typename... SR>
struct ResourceList {
  /* unique_regs_sequence holds all registers once, keeping order */
  typedef typename unique_type<typename SR::reg_type...>::type unique_regs_sequence;

  static void set(void) {
    unique_regs_sequence::template set_resources<SR...>();
  }
};


#endif // RESOURCE_MPL_HPP_INCLUDED
