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

#ifndef PERIPH_HPP_INCLUDED
#define PERIPH_HPP_INCLUDED

#include "register.hpp"

namespace mptl {

/**
 * Peripheral Device Class
 *
 * Template arguments:
 * - Tp     : derived type (real peripheral type), e.g. spi<0>
 * - CFG... : configuration list: e.g. spi_default_config
 */
template< typename Tp, typename cfg_reg_type_list, typename... CFG >
struct periph
{
  using derived_type = Tp;
  using cfg_list = typelist< CFG... >;

  using regmask_list = typelist<
    typename CFG::template regmask_type< derived_type >...
    >;

  using resources = typelist<
    regmask_list,
    typename CFG::template resources< derived_type >...
    >;

#if 0
  /**
   * Set register reg_type to its default value, combined with the
   * merged CFG::regmask_type classes.
   */
  // TODO: need unittest
  template<typename reg_type>
  static void configure_reg() {
    using neutral_regmask = regmask< reg_type, 0, 0 >;

    /* Filter CFG by reg_type, append neutral regmask, and merge it */
    using filtered = typename regmask_list::template filter_type<reg_type>;
    using filtered_neutral = typelist< filtered, neutral_regmask >;

    mpl::regmask_write< filtered_neutral, mpl::write_strategy::reset_to >();
  }
#endif

  /**
   * Set register list_element_type (aka: regdef<>) to its default
   * value, combined with the merged regmask<> types from the local
   * regmask_list<>.
   *
   * 1. Filter the local regmask_list<> using the regdef<> (aka:
   * list_element_type) type trait, by for_each<>() call (e.g. in
   * configure() function below for each element in the
   * cfg_reg_type_list).
   *
   * 2. Merges the regmask<> types from the filtered regmask_list<>.
   *
   * 3. Call:
   *
   *     list_element_type::reset_to< merged_regmask<> >();
   *
   */
  struct functor_reset_to_regmask_list {
    template<typename list_element_type>
    static void __always_inline command(void) {
      using filtered_list =
        typename regmask_list::template filter<
          mpl::filter_reg_type<list_element_type>
        >::type;

      using merged_regmask_type =
        typename filtered_list::template pack<
          mpl::pack_merged_regmask
        >::type;

      list_element_type::template reset_to< merged_regmask_type >();
    }
  };

  /**
   * Configure peripheral device.
   * NOTE: make sure no communication is ongoing when calling this function.
   */
  static void configure(void) {
    cfg_reg_type_list::template for_each< functor_reset_to_regmask_list >();
  }

  /**
   * Reconfigure and enable peripheral device.
   * NOTE: make sure no communication is ongoing when calling this function.
   */
  void reconfigure(void) const {
    //!!!    periph_type::disable();
    configure();
    //!!!    periph_type::enable();
  }
};

} // namespace mptl

#endif // PERIPH_HPP_INCLUDED
