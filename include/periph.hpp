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

#include <register.hpp>
#include <simulation.hpp>

namespace mptl {

namespace cfg {

  struct config_base
  : typelist_element
  {
    /**
     * Resources needed by this element. Added to peripheral device
     * resources typelist.
     *
     * Defaults to void.
     */
    template<typename T> using resources = void;

    /**
     * Configuration portion. 
     *
     * regmask<> type, or typelist<regmask<>...> type defining the
     * configuration of the peripheral device.
     *
     * Used by the configure() member function, in order to set
     * corresponding register bits defined by the given regmask<>.
     *
     * Also appended to the periph class resources typelist.
     * Defaults to void.
     */
    template<typename T> using config_regmask = void;
  };

} // namespace cfg


/**
 * Peripheral resources class
 *
 * Template arguments:
 * - Tp     : derived type (real peripheral type), e.g. spi<0>
 * - CFG... : configuration list: e.g. spi_default_config
 */
template< typename Tp, typename... CFG >
struct periph_cfg
{
  using derived_type = Tp;
  using cfg_list = typelist< CFG... >;

  using resources = typelist<
    typename CFG::template resources< derived_type >...,
    typename CFG::template config_regmask< derived_type >...
    >;
};


/**
 * Peripheral device class
 *
 * Template arguments:
 * - Tp        : derived type (real peripheral type), e.g. spi<0>
 * - type_list : typelist<regdef<>...> type, for resetting in configure() function
 * - CFG...    : configuration list: e.g. spi_default_config
 */
template< typename Tp, typename cfg_reg_type_list, typename... CFG >
class periph
: public periph_cfg< Tp, CFG... >
{
  /**
   * Set register list_element_type (aka: regdef<>) to its default
   * value, combined with the merged regmask<> types from the local
   * config_list<>.
   *
   * 1. Filter the local config_list<> using the regdef<> (aka:
   * list_element_type) type trait, by for_each<>() call (e.g. in
   * configure() function below for each element in the
   * cfg_reg_type_list).
   *
   * 2. Merges the regmask<> types from the filtered config_list<>.
   *
   * 3. Call:
   *
   *     list_element_type::reset_to< merged_regmask<> >();
   *
   */
  // TODO: need unittest
  struct functor_reset_to_config_list {
    template<typename list_element_type>
    static void __always_inline command(void) {
      using filtered_list =
        typename config_list::template filter<
          mpl::filter_reg_type< list_element_type >
        >::type;

      using merged_regmask_type =
        typename filtered_list::template pack<
          mpl::pack_merged_regmask
        >::type;

      list_element_type::template reset_to< merged_regmask_type >();
    }
  };

public:

  using derived_type = Tp;
  using cfg_list = typelist< CFG... >;

  /**
   * List of all regmask<> types from CFG.
   */
  using config_list = typelist<
    typename CFG::template config_regmask< derived_type >...
    >;

  /**
   * Configure peripheral device.
   *
   * Sets all registers listed in the cfg_reg_type_list (aka:
   * typelist<>). Takes register default value, or'ed by set/clear
   * mask of the merged config_regmask (aka: regmask<> or
   * typelist<regmask<>...>) types from the CFG traits.
   *
   * NOTE: make sure no communication is ongoing when calling this function.
   */
  static void configure(void) {
    SIM_DEBUG("periph::configure()");
    cfg_reg_type_list::template for_each< functor_reset_to_config_list >();
    SIM_DEBUG("~periph::configure()");
  }

  /**
   * Reconfigure and enable peripheral device.
   * NOTE: make sure no communication is ongoing when calling this function.
   */
  static void reconfigure(void) {
    derived_type::disable();
    configure();
    derived_type::enable();
  }
};

} // namespace mptl

#endif // PERIPH_HPP_INCLUDED
