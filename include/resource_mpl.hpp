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


////////////////////  Resource  ////////////////////

/**
 * T: resource type, used for filtering and executing commands
 * R: resource value, must provide combine<> type
 */
template<typename T, typename R = T>
struct Resource {
  /* Append resource R to the filtered_list (Tf) if Filter matches our type (T). */
  template<typename Tf, typename Filter>
  using append_filtered_list = typename std::conditional<
    std::is_same<Filter, T>::value,
    typename Tf::template append<R>,
    Tf>::type;

  /* Append type T to the unique_list (Tu) if unique_list does not yet hold our type (T). */
  template<typename Tu>
  using append_ulist = typename std::conditional<
    Tu::template contains<T>::value,
    Tu,
    typename Tu::template append<T>
    >::type;
};


////////////////////  SharedRegister  ////////////////////


template<typename R>
struct SharedRegisterFunc
{
  using type = SharedRegisterFunc<R>;

  // TODO: implement something like reset_register(), since on startup we don't care about the actual value of the register
  // T::store((T::reset_value & ~combined_type<T>::clear_mask) | combined_type<T>::set_mask);

  /* Set a shared register to combined set/clear-mask of all shared registers of same type */
  template<typename Rl>  /* Rl must be a ResourceList */
  static typename std::enable_if<Rl::template resource_filtered_list<type>::size>::type configure() {
    R::set(Rl::template combined_type<type>::set_mask,
           Rl::template combined_type<type>::clear_mask);
  }

  template<typename Rl> static constexpr void check() { }
};


template<typename R, typename R::value_type _set_mask, typename R::value_type _clear_mask = 0 >
struct SharedRegister
: Resource< SharedRegisterFunc<R>, SharedRegister<R, _set_mask, _clear_mask> >
{
  using reg_type = R;
  using reg_value_type = typename R::value_type;

  static constexpr reg_value_type set_mask   = _set_mask;
  static constexpr reg_value_type clear_mask = _clear_mask;

  /* Combine two SharedRegister of same reg_type:                      */
  /* Returns SharedRegister with or'ed set_mask and or'ed clear_mask.  */
  template<typename Rc>
  struct combine {
    typedef SharedRegister<R, set_mask | Rc::set_mask, clear_mask | Rc::clear_mask> type;
    static_assert(std::is_same<reg_type, typename Rc::reg_type>::value, "oops, combining values for different register...");
  };
};


////////////////////  UniqueResource  ////////////////////


template<typename T, typename U>
struct assert_different {
  static constexpr bool value = !std::is_same<T, U>::value;
  static_assert(value, "UniqueResource<...> is not unique! (see compiler output \"assert_different<...>\" above)");
};

template<typename T, typename... Args>
struct unique_types;
template<typename T0, typename T1, typename... Args>
struct unique_types<T0, T1, Args...> {
  /* not very efficient, O(n^2) */
  static constexpr bool value =
    assert_different<T0, T1>::value &&
    unique_types<T0, Args...>::value &&
    unique_types<T1, Args...>::value;
};
template<typename T>
struct unique_types<T> {
  static constexpr bool value = true;
};


template<typename... Args>
struct unique_resource_value {
  using value_type = unique_resource_value<Args...>;
  static constexpr bool is_unique = unique_types<Args...>::value;

  template<typename T>
  using append = unique_resource_value<T, Args...>;

  template<typename U>
  struct combine {
    typedef typename U::template append<Args...> type;
  };
};

struct UniqueResourceFunc
{
  template<typename Rl> static void configure() { }

  template<typename Rl>
  static constexpr void check() {
    static_assert(Rl::template combined_type< UniqueResourceFunc >::is_unique,
                  "ResourceList contains UniqueResource which are not unique. (see compile messages above)");
  }
};

template<typename T>
struct UniqueResource
: Resource< UniqueResourceFunc, unique_resource_value<T> >
{ };


////////////////////  IsrResource  ////////////////////

template<typename T>
struct IrqResource : Resource< IrqResource<T> > {
  using value_type = T;

  template<typename U>
  struct combine {
    using type = T;
    static_assert(std::is_same<type, typename U::value_type>::value, "gna");
  };

  template<typename Rl> static constexpr void check() { }
  template<typename Rl> static           void configure() { }
};


////////////////////  filtered_list  ////////////////////


template<typename... Args>
struct filtered_list_impl;

template<typename Head, typename... Args>
struct filtered_list_impl<Head, Args...>  {
  typedef typename Head::template combine<typename filtered_list_impl<Args...>::combined_type>::type combined_type;
};
template<typename Head>
struct filtered_list_impl<Head> {
  typedef Head combined_type;
};
template<>
struct filtered_list_impl<> {
  /* combined_type is void for empty filtered list */
  typedef void combined_type;
};

template<typename... Args>
struct filtered_list : filtered_list_impl<Args...> {
  template<typename T>
  using append = filtered_list<Args..., T>;
  static constexpr std::size_t size = sizeof...(Args);
};


/* Creates a filtered_list<...>: list of value_types from resource    */
/* classes, with it's type being the combined value_type (e.g. or'ed  */
/* register values for SharedRegister).                               */
template<typename T, typename Filter, typename... Args>
struct make_filtered_list;

template<typename T, typename Filter, typename Head, typename... Args>
struct make_filtered_list<T, Filter, Head, Args...> {
  typedef typename make_filtered_list<typename Head::template append_filtered_list<T, Filter>, Filter, Args...>::type type;
};

template<typename T, typename Filter>
struct make_filtered_list<T, Filter> {
  typedef T type;
};




////////////////////  ulist (unique resource type list) ////////////////////


template<typename... Args>
struct ulist_impl;

template<typename Head, typename... Args>
struct ulist_impl<Head, Args...>  {
  template<typename T>
  struct contains {
    static constexpr bool value = ( std::is_same<Head, T>::value || 
                                    ulist_impl<Args...>::template contains<T>::value );
  };

  template<typename Rl>
  static void configure() {
    Head::template configure<Rl>();
    ulist_impl<Args...>::template configure<Rl>();
  }
  template<typename Rl>
  static void check() {
    Head::template check<Rl>();
    ulist_impl<Args...>::template check<Rl>();
  }
};
template<typename Head>
struct ulist_impl<Head> {
  template<typename T>
  using contains = std::is_same<Head, T>;

  template<typename Rl> static void configure() {  Head::template configure<Rl>(); }
  template<typename Rl> static void check()     {  Head::template check<Rl>(); }
};
template<>
struct ulist_impl<> {
  template<typename T>
  using contains = std::false_type;

  template<typename Rl> static void configure() { }
  template<typename Rl> static void check() { }
};

template<typename... Args>
struct ulist : ulist_impl<Args...> {
  template<typename T>
  using append = ulist<Args..., T>;

  static constexpr std::size_t size = sizeof...(Args);
};


/* Creates a ulist<...>: list of value_types from resource    */
template<typename T, typename... Args>
struct make_ulist;

template<typename T, typename Head, typename... Args>
struct make_ulist<T, Head, Args...> {
  typedef typename make_ulist<typename Head::template append_ulist<T>, Args...>::type type;
};

template<typename T>
struct make_ulist<T> {
  typedef T type;
};




////////////////////  ResourceList  ////////////////////


template<typename... Args>
struct ResourceList {
  using type = ResourceList<Args...>;

  /* Append a ResourceList to a filtered_list. Needed by make_filtered_list in  */
  /* order to create its list from nested ResourceList's                        */
  template<typename T, typename Filter>
  using append_filtered_list = typename make_filtered_list<T, Filter, Args...>::type;

  template<typename T>
  using append_ulist = typename make_ulist<T, Args...>::type;

  template<typename Filter>
  using resource_filtered_list = typename make_filtered_list<filtered_list<>, Filter, Args...>::type;

  using resource_ulist = typename make_ulist<ulist<>, Args...>::type;

  /* Combined resource value_type (e.g. reg_value for SharedRegister) */
  template<typename Filter>
  using combined_type = typename resource_filtered_list<Filter>::combined_type;


  static void set_shared_register();  // implemented in core_resource.hpp (TODO: generic functions which sets all register)

  template<typename Filter>
  struct irq_resource
  {
    /* Returns Irq from ResourceList, or void if not found */
    typedef combined_type<Filter> type;
  };

  static           void configure() { resource_ulist::template configure<type>();  }
  static constexpr void check()     { resource_ulist::template check<type>();  }
};

#endif // RESOURCE_MPL_HPP_INCLUDED
