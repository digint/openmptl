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

namespace mpl
{
  template< typename gpio_type, freq_t gpio_speed >
  struct spi_gpio_output_resources {
    using type = typelist<
      typename gpio_type::resources,
      typename gpio_type::mode::template output< gpio_speed >,
      typename gpio_type::output_type::af_push_pull
      >;
  };
  template< freq_t gpio_speed >
  struct spi_gpio_output_resources< void, gpio_speed > {
    using type = void;
  };

  template< typename gpio_type >
  struct spi_gpio_input_resources {
    using type = typelist<
      typename gpio_type::resources,
      typename gpio_type::mode::input,
      typename gpio_type::input_type::pull_up_down
      >;
  };
  template<>
  struct spi_gpio_input_resources< void > {
    using type = void;
  };
} // namespace mpl


template<
  unsigned spi_no,
  typename rcc_type,
  typename gpio_sck_type     = void,
  typename gpio_miso_type    = void,
  typename gpio_mosi_type    = void,
  freq_t   gpio_output_speed = mhz(50)
  >
class spi : public spi_stm32_common< spi_no, rcc_type >
{
  using base_type = spi_stm32_common< spi_no, rcc_type >;

public:

  using resources = typelist<
    typename base_type::resources,
    typename mpl::spi_gpio_output_resources< gpio_sck_type,  gpio_output_speed >::type,
    typename mpl::spi_gpio_input_resources< gpio_miso_type >::type,
    typename mpl::spi_gpio_output_resources< gpio_mosi_type, gpio_output_speed >::type
    >;
};

} // namespace mptl

#endif // SPI_HPP_INCLUDED
