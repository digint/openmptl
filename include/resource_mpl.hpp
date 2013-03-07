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
#include <isr.hpp>  // isr_t

////////////////////  Resource  ////////////////////

/**
 * Template arguments:
 *
 * - T (group_type): resource group type. Acts as filter for resource_type_list. This type (T)
 *                   MUST provide the configure() and assert_type() functions.
 *
 * - R (type): resource type, MUST provide combine<> type and configure() function.
 */
template<typename T, typename R = T>
struct Resource {
  using group_type = T;
  using type = R;

  /* Append resource R to the filtered_list (Tf) if Filter matches our type (T). */
  template<typename Tf, typename Filter>
  using append_filtered_list = typename std::conditional<
    std::is_same<Filter, group_type>::value,
    typename Tf::template append<type>,
    Tf>::type;

  /* Append type T to the resource_type_list (Tu) if resource_type_list does not yet hold our type (T). */
  template<typename Tu>
  using append_resource_type_list = typename std::conditional<
    Tu::template contains<group_type>::value,
    Tu,
    typename Tu::template append<group_type>
    >::type;

  void configure() { }

  template<typename U>
  struct combine {
    /* combine asserts by default */
    typedef void type;
    static_assert(std::is_void<U>::value,
                  "ResourceList contains a resource group which does not implement the combine functor.");
  };
};


struct ResourceGroup
{
  template<typename Rl> static constexpr bool assert_type() { return true; }
};


////////////////////  SharedRegister  ////////////////////


template<typename T>
struct SharedRegisterGroup : ResourceGroup
{ };


template<typename R, typename R::value_type _set_mask, typename R::value_type _clear_mask = 0 >
struct SharedRegister
: Resource< SharedRegisterGroup<R>, SharedRegister<R, _set_mask, _clear_mask> >
{
  //  using type = SharedRegister<R, _set_mask, _clear_mask>;
  using reg_type = R;
  using reg_value_type = typename R::value_type;

  static constexpr reg_value_type set_mask   = _set_mask;
  static constexpr reg_value_type clear_mask = _clear_mask;

  /* Combine two SharedRegister of same reg_type:                      */
  /* Returns SharedRegister with or'ed set_mask and or'ed clear_mask.  */
  template<typename U>
  struct combine {
    typedef SharedRegister<R, set_mask | U::set_mask, clear_mask | U::clear_mask> type;
    static_assert(std::is_same<reg_type, typename U::reg_type>::value, "oops, combining values for different register...");
  };

  /* Called by resource_type_list_impl::configure() on a combined */
  /* SharedRegister type. (which we are in this context, see "combine" above)  */
  void configure() {
    if((set_mask != 0) || (clear_mask != 0))
      R::set(set_mask, clear_mask);

    // TODO: implement something like reset_register(), since on startup we don't care about the actual value of the register
    // T::store((T::reset_value & ~combined_type<T>::clear_mask) | combined_type<T>::set_mask);
  }
};


////////////////////  UniqueResource  ////////////////////


template<typename T, typename R = T>
struct UniqueResourceBase
: Resource< T, R >
{
  template<typename U>
  struct combine {
    using type = void;
    static_assert(std::is_void<U>::value,  /* always assert. combining unique resources means that two same types exist */
                  "ResourceList contains a resource derived from UniqueResource which is not unique. (see compile message \"filtered_list<MyResource< XXX >, MyResource< XXX >, ... >\" above.)");
  };

  template<typename Rl>
  static constexpr bool assert_type() {
    /* We check for "Rl::combined_type<type>::type here", and assert    */
    /* in "combine" above. The reason for this is to get nicer          */
    /* compiler messages than if we were just asserting for             */
    /* "Rl::resource_filtered_list<type>::size <= 1" (which is also     */
    /* correct).                                                        */
    return !std::is_void<typename Rl::template combined_type<T>::type >::value;
  }
};

template<typename T>
struct UniqueResource
: UniqueResourceBase< UniqueResource<T> >
{ };


////////////////////  IsrResource  ////////////////////

template<int irqn>
struct IrqResourceGroup : ResourceGroup
{ };

template<int irqn, isr_t isr>
struct IrqResource
: UniqueResourceBase< IrqResourceGroup<irqn>, IrqResource<irqn, isr> >
{
  static constexpr isr_t value = isr;
};


////////////////////  filtered_list  ////////////////////


template<typename... Args>
struct filtered_list_impl;

template<typename Head, typename... Args>
struct filtered_list_impl<Head, Args...>  {
 using combined_type = typename Head::template combine<typename filtered_list_impl<Args...>::combined_type>::type;
};
template<typename Head>
struct filtered_list_impl<Head> {
  using combined_type = typename Head::type;
};
template<>
struct filtered_list_impl<> {
  /* combined_type is void for empty filtered list */
  using combined_type = void ;
};

template<typename... Args>
struct filtered_list : filtered_list_impl<Args...> {
  template<typename T>
  using append = filtered_list<Args..., T>;
  static constexpr std::size_t size = sizeof...(Args);
};


/* Creates a filtered_list<...>: list of resource class types,               */
/* providing combined_type (e.g. or'ed  register values for SharedRegister). */
template<typename T, typename Filter, typename... Args>
struct make_filtered_list;

template<typename T, typename Filter, typename Head, typename... Args>
struct make_filtered_list<T, Filter, Head, Args...> {
  using type = typename make_filtered_list<typename Head::template append_filtered_list<T, Filter>, Filter, Args...>::type;
};

template<typename T, typename Filter>
struct make_filtered_list<T, Filter> {
  using type = T;
};


////////////////////  resource_type_list  ////////////////////


template<typename... Args>
struct resource_type_list_impl;

template<typename Head, typename... Args>
struct resource_type_list_impl<Head, Args...>  {
  template<typename T>
  struct contains {
    static constexpr bool value = ( std::is_same<Head, T>::value || 
                                    resource_type_list_impl<Args...>::template contains<T>::value );
  };

  template<typename Rl>
  static void configure() {
    typename Rl::template resource_filtered_list<Head>::combined_type().configure();
    resource_type_list_impl<Args...>::template configure<Rl>();
  }

  template<typename Rl>
  static constexpr bool assert_type() {
    return (Head::template assert_type<Rl>() &&
            resource_type_list_impl<Args...>::template assert_type<Rl>() );
  }
};
template<>
struct resource_type_list_impl<> {
  template<typename T>
  using contains = std::false_type;

  template<typename Rl> static void configure() { }
  template<typename Rl> static constexpr bool assert_type() { return true; }
};

template<typename... Args>
struct resource_type_list : resource_type_list_impl<Args...> {
  template<typename T>
  using append = resource_type_list<Args..., T>;

  static constexpr std::size_t size = sizeof...(Args);
};


/* Creates a resource_type_list<...>: list of unique resource types */
template<typename T, typename... Args>
struct make_resource_type_list;

template<typename T, typename Head, typename... Args>
struct make_resource_type_list<T, Head, Args...> {
  typedef typename make_resource_type_list<typename Head::template append_resource_type_list<T>, Args...>::type type;
};

template<typename T>
struct make_resource_type_list<T> {
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

  /* Append a ResourceList to a filtered_list. Needed by make_resource_type_list in    */
  /* order to create its list from nested ResourceList's                        */
  template<typename T>
  using append_resource_type_list = typename make_resource_type_list<T, Args...>::type;

  /* filtered_list, containing all resources of type T */
  template<typename T>
  using resource_filtered_list = typename make_filtered_list<filtered_list<>, T, Args...>::type;

  /* resource_type_list, containing all resource types once */
  using resource_resource_type_list = typename make_resource_type_list<resource_type_list<>, Args...>::type;

  /* Combined resource type */
  template<typename T>
  using combined_type = typename resource_filtered_list<T>::combined_type;

  /* Returns Handler from ResourceList, or void if not found */
  template<int irqn>
  struct irq_resource {
    using irq_resource_group_type = IrqResourceGroup<irqn>;
    typedef combined_type< IrqResourceGroup<irqn> > type;
    static constexpr isr_t value = combined_type< IrqResourceGroup<irqn> >::value;
  };

  /* Instantiate and run configure() on all combined resource types.  */
  /* NOTE: We instantiate here because for some reason calling a      */
  /* static configure() function results code not being inlined.      */
  /* TODO: check again, find out why this is so.                      */
  static void configure() {
    resource_resource_type_list::template configure<type>();
  }

  /* Compile-time consistency check of the resource list (calls static_assert() on errors) */
  static constexpr void check() {
    static_assert(resource_resource_type_list::template assert_type<type>(), "ResourceList check failed.");
  }
};

#endif // RESOURCE_MPL_HPP_INCLUDED
