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

#ifndef SPI_HPP_INCLUDED
#define SPI_HPP_INCLUDED

#include "../../../common/spi.hpp"

namespace mptl {

template<typename  rcc_type, unsigned _spi_no>
class spi : public spi_stm32_common<rcc_type, _spi_no>
{
  // TODO: provide a matrix for the gpio port/pin_no
  template<char port, unsigned pin_no>
  struct gpio_impl  /* same for sck, miso, mosi */
  {
    using gpio_type = gpio_output< port, pin_no >; // gpio_input_af<port, pin_no>
    using type = typelist<
      typename gpio_type::resources,
      typename gpio_type::output_type::af_push_pull
      >;
  };

public:

  template<char port, unsigned pin_no>
  using gpio_sck = typename gpio_impl<port, pin_no>::type;

  template<char port, unsigned pin_no>
  using gpio_miso = typename gpio_impl<port, pin_no>::type;

  template<char port, unsigned pin_no>
  using gpio_mosi = typename gpio_impl<port, pin_no>::type;
};

} // namespace mptl

#endif // SPI_HPP_INCLUDED
