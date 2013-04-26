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

#include "resource_mpl.hpp"
#include <type_traits>
#include <compiler.h>

////////////////////  SharedRegister  ////////////////////


template<typename T>
struct SharedRegisterGroup : mpl::resource_group
{ };


template<typename R>
struct SharedRegister
: mpl::resource< SharedRegisterGroup<typename R::reg_type>, SharedRegister<typename R::mask_type> >
{
  using mask_type = typename R::mask_type;

  /* Combine two SharedRegister of same SharedRegisterGroup:           */
  /* Returns SharedRegister with or'ed set_mask and or'ed clear_mask.  */
  template<typename U>
  struct combine {
    using type = SharedRegister< typename mask_type::template combine<typename U::mask_type>::type >;
  };

  /* Called by resource_type_list_impl::configure() on a combined */
  /* SharedRegister type. (which we are in this context, see "combine" above)  */
  static __always_inline void configure() {
    //    if((R::set_mask != 0) || (R::clear_mask != 0))
      R::set();

    // TODO: implement something like reset_register(), since on startup we don't care about the actual value of the register:
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

template<typename irq_type, isr_t isr>
struct IrqResource
: mpl::unique_resource< IrqResourceGroup<irq_type::irqn>, IrqResource<irq_type, isr> >
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
    // this fails since combined_type can be of type "void":
    // static constexpr isr_t value = combined_type< IrqResourceGroup<irqn> >::value;
  };

  /* Run configure() on all combined resource types.  */
  static void configure() {
    resource_type_list::template configure<type>();
  }

  /* Compile-time consistency check of the resource list (calls static_assert() on errors) */
  static constexpr bool check() {
    static_assert(resource_type_list::template assert_type<type>(), "ResourceList check failed.");
    return true;
  }
};

#endif // RESOURCE_HPP_INCLUDED
