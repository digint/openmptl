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

namespace mptl {

#if 1
template< typename Tp, typename... CFG >
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
};
#else
/**
 * Peripheral Device Class
 *
 * Template arguments:
 * - peripheral_type: e.g. spi<0>
 * - peripheral_config_type: e.g. spi_default_config
 */
class periph
{
public:
  //  using cfg_list = periph_config<CFG...>;

  /**
   * Configure peripheral device.
   * NOTE: make sure no communication is ongoing when calling this function.
   */
  void configure(void) const {
    //!!!    periph_type::configure(*this);
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
#endif

} // namespace mptl

#endif // PERIPH_HPP_INCLUDED
