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

/**
 * Peripheral device class
 *
 * Template arguments:
 *
 * - Tp        : derived type (real peripheral type, e.g. spi<0>),
 *               providing a config_reg_list
 *
 * - Tcfg...   : configuration list: e.g. spi_default_config
 */
template< typename Tp, typename... Tcfg >
class periph
: public Tp
{
  using derived_type = Tp;

  /**
   * Set register list_element_type (aka: regdef<>) to its default
   * value, combined with the merged regmask<> types from all
   * cfg_list<> elements.
   *
   * 1. Filter cfg_list<> using the regdef<> (aka: list_element_type)
   * type trait, by for_each<>() call (e.g. in configure() function
   * below for each element in the config_reg_list).
   *
   * 2. Merges the regmask<> types from the filtered cfg_list<>.
   *
   * 3. Call:
   *
   *     list_element_type::reset_to< merged_regmask<> >();
   *
   */
  // TODO: need unittest
  struct functor_reset_to_cfg {
    using cfg_list = typelist< Tcfg... >;

    template<typename list_element_type>
    static void __always_inline command(void) {
      using filtered_list =
        typename cfg_list::template filter<
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

  using resources = typelist<
    typename Tp::resources,
    Tcfg...
    >;

  /**
   * Configure peripheral device.
   *
   * Sets all registers listed in Tp::config_reg_list (aka:
   * typelist<>). Takes register default value, or'ed by set/clear
   * mask of the merged Tcfg (aka: regmask<> or
   * typelist<regmask<>...>) traits.
   *
   * NOTE: make sure no communication is ongoing when calling this function.
   */
  static void configure(void) {
    SIM_DEBUG("periph::configure()");
    Tp::config_reg_list::template for_each< functor_reset_to_cfg >();
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
