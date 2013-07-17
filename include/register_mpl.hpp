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

#ifndef REGISTER_MPL_HPP_INCLUDED
#define REGISTER_MPL_HPP_INCLUDED

#include <type_traits>
#include <cstdint>
#include <register_backend.hpp>
#include <resource.hpp>
#include <compiler.h>

namespace mptl { namespace mpl {

/**
 * Base class for regmask class. Used for filtering in resource::list.
 */
struct regmask_base
: public typelist_element
{ };

/**
 * Merge all regmask types (Tp...) into a new regmask type of same
 * regdef_type. Silently ignores void types in Tp. Returns void type
 * on an empty list.
 *
 * NOTE: all template arguments must be of same regdef_type, or void.
 */
template<typename... Tp>
struct merged_regmask;

template<>
struct merged_regmask<> {
  using type = void;
};
template<typename Tp>
struct merged_regmask<Tp> {
  using type = Tp;
};
template<typename... Tp>
struct merged_regmask<void, Tp...> {
  using type = typename merged_regmask<Tp...>::type;
};
template<typename T0, typename... Tp>
struct merged_regmask<T0, Tp...> {
  using type = typename T0::mask_type::template merge< typename merged_regmask<Tp...>::type >::type;
};


/**
 * Provides a list, whose types all share the same underlying
 * reg_type = Tp
 */
template<typename Tp>
struct filter_reg_type {
  template<typename Tf>
  using filter = std::is_same< typename Tp::reg_type, typename Tf::reg_type >;
};


/**
 * Packs elements to a merged_regmask (aka: regmask<>).
 *
 * NOTE: merged_regmask<> asserts at compile-time that ALL the
 * elements in Tp... are of same regdef_type.
 */
struct pack_merged_regmask {
  template<typename... Tp>
  struct pack {
    using type = typename merged_regmask<Tp...>::type;
  };
};

/**
 * Map each element in list to a merged_regmask of all elements in
 * list having the same reg_type.
 */
struct map_merged_regmask {
  template<typename T, typename list_type>
  struct map {
    using filtered_list = typename list_type::template filter< filter_reg_type<T> >::type;
    using type = typename filtered_list::template pack< pack_merged_regmask >::type;
  };
};

/**
 * Calls regdef_type::reset_to<>() on a given typelist element type.
 *
 * NOTE: list_element_type MUST provide the reset_to<>() static
 * member (e.g. regmask<> type), so you might want to filter your
 * typelist first.
 *
 * Sets a register to its reset value (regdef::reset_value), merged
 * with the set/clear mask of the given regmask element type
 * (list_element_type).
 *
 * This results in a single write (regdef_backend::store())
 * instruction, NOT a read-modify-write!.
 *
 * Used e.g. in core::configure() on typelist::for_each<>() in order
 * to reset all register from the accumulated resources list at once.
 *
 * See regdef::reset_to<>() documentation for more information.
 */
struct functor_reg_reset_to {
  template<typename list_element_type>
  static void __always_inline command(void) {
    list_element_type::reg_type::template reset_to< list_element_type >();
  }
};

/**
 * Analog to functor_reg_reset_to, but executes the
 * regmask_type::set() static member function instead.
 *
 * Use this if you only WANT the register bits to keep their old
 * value if not touched by set/clear mask of regmask<>.
 *
 * This results in a read-modify-write access and is thus much
 * slower. Depending on register and/or processor type, you might
 * want to choose this functor.
 */
struct functor_reg_set {
  template<typename list_element_type>
  static void __always_inline command(void) {
    list_element_type::set();
  }
};

enum class write_strategy {
  read_modify_write,
  reset_to
}; 

/**
 * Call regdef::reset_to() on each distinct merged regmask from list.
 *
 * Also see the "functor_reg_reset_to" documentation in
 * register_mpl.hpp for a discussion about reset_to() and set().
 */
template< typename typelist_type,
  write_strategy strategy = write_strategy::read_modify_write,
  typename... Tf
        >
static void regmask_write() {
  using regmask_list = typename typelist_type::template filter_type< regmask_base >;
  using merged_list  = typename regmask_list::template map< mpl::map_merged_regmask >;
  using unique_merged_list = typename merged_list::filter_unique::type;

  if(strategy == write_strategy::read_modify_write)
    unique_merged_list::template for_each< mpl::functor_reg_set >();
  else
    unique_merged_list::template for_each< mpl::functor_reg_reset_to >();
}

} } // namespace mptl::mpl

#endif // REGISTER_MPL_HPP_INCLUDED
