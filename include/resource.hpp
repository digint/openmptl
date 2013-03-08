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

#ifndef RESOURCE_HPP_INCLUDED
#define RESOURCE_HPP_INCLUDED

#include "resource_mpl.hpp"
#include <type_traits>


////////////////////  SharedRegister  ////////////////////


template<typename T>
struct SharedRegisterGroup : mpl::resource_group
{ };


template<typename R, typename R::value_type _set_mask, typename R::value_type _clear_mask = 0 >
struct SharedRegister
: mpl::resource< SharedRegisterGroup<R>, SharedRegister<R, _set_mask, _clear_mask> >
{
  using reg_type = R;
  using reg_value_type = typename R::value_type;

  static constexpr reg_value_type set_mask   = _set_mask;
  static constexpr reg_value_type clear_mask = _clear_mask;

  /* Combine two SharedRegister of same reg_type:                      */
  /* Returns SharedRegister with or'ed set_mask and or'ed clear_mask.  */
  template<typename U>
  struct combine {
    // TODO: assert here if bits in A.set_mask match B.clear_mask (set_mask | U::clear_mask) and vice versa
    using type = SharedRegister<R, set_mask | U::set_mask, clear_mask | U::clear_mask>;
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


template<typename T>
struct UniqueResource
: mpl::unique_resource< UniqueResource<T> >
{ };


////////////////////  IrqResource  ////////////////////


template<int irqn>
struct IrqResourceGroup : mpl::resource_group
{ };

template<int irqn, isr_t isr>
struct IrqResource
: mpl::unique_resource< IrqResourceGroup<irqn>, IrqResource<irqn, isr> >
{
  static constexpr isr_t value = isr;
};


////////////////////  ResourceList  ////////////////////


template<typename... Args>
struct ResourceList : mpl::resource_list_impl<Args...>
{
  using type = ResourceList<Args...>;

  /* filtered_list, containing all resources of type T */
  template<typename T>
  using resource_filtered_list = typename mpl::make_filtered_list<T, Args...>::type;

  /* resource_type_list, containing all resource types (unique) */
  using resource_type_list = typename mpl::make_resource_type_list<Args...>::type;

  /* Combined resource type */
  template<typename T>
  using combined_type = typename resource_filtered_list<T>::combined_type;

  /* Returns Handler from ResourceList, or void if not found */
  template<int irqn>
  struct irq_resource {
    using type = combined_type< IrqResourceGroup<irqn> >;
    using irq_resource_group_type = IrqResourceGroup<irqn>;
    static constexpr isr_t value = combined_type< IrqResourceGroup<irqn> >::value;
  };

  /* Instantiate and run configure() on all combined resource types.  */
  /* NOTE: We instantiate here because for some reason calling a      */
  /* static configure() function results code not being inlined.      */
  /* TODO: check again, find out why this is so.                      */
  static void configure() {
    resource_type_list::template configure<type>();
  }

  /* Compile-time consistency check of the resource list (calls static_assert() on errors) */
  static constexpr void check() {
    static_assert(resource_type_list::template assert_type<type>(), "ResourceList check failed.");
  }
};

#endif // RESOURCE_HPP_INCLUDED
