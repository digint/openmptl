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

#ifndef PERIPHERAL_DEVICE_HPP_INCLUDED
#define PERIPHERAL_DEVICE_HPP_INCLUDED

namespace mptl {

/**
 * Peripheral Device Class
 *
 * Template arguments:
 * - peripheral_type: e.g. spi<0>
 * - peripheral_config_type: e.g. spi_default_config
 */
template<typename peripheral_type, typename peripheral_config_type >
class peripheral_device
: public peripheral_type, public peripheral_config_type
{
public:

  using resources = typename peripheral_type::resources;

  /**
   * Configure peripheral.
   * NOTE: make sure no communication is ongoing when calling this function.
   */
  void configure(void) const {
    peripheral_type::configure(*this);
  }

  /**
   * Reconfigure and enable peripheral.
   * NOTE: make sure no communication is ongoing when calling this function.
   */
  void reconfigure(void) const {
    peripheral_type::disable();
    configure();
    peripheral_type::enable();
  }
};

} // namespace mptl

#endif // PERIPHERAL_DEVICE_HPP_INCLUDED
