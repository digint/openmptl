/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright (C) 2012-2017 Axel Burri <axel@tty0.ch>
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

#ifndef ARCH_USART_HPP_INCLUDED
#define ARCH_USART_HPP_INCLUDED

#include "../../../common/usart.hpp"

namespace mptl {

namespace mpl
{
  template< typename gpio_type >
  struct usart_gpio_rx_resources {
    using type = typelist<
      typename gpio_type::resources,
      typename gpio_type::mode::input,
      typename gpio_type::input_type::floating
      >;
  };
  template<>
  struct usart_gpio_rx_resources< void > {
    using type = void;
  };

  template< typename gpio_type, freq_t gpio_speed >
  struct usart_gpio_tx_resources {
    using type = typelist<
      typename gpio_type::resources,
      typename gpio_type::mode::template output< gpio_speed >,
      typename gpio_type::output_type::af_push_pull
      >;
  };
  template< freq_t gpio_speed >
  struct usart_gpio_tx_resources< void, gpio_speed > {
    using type = void;
  };
} // namespace mpl


template<
  unsigned usart_no,
  typename system_clock_type,
  typename gpio_rx_type  = void,
  typename gpio_tx_type  = void,
  freq_t   gpio_tx_speed = mhz(50)
  >
class usart : public usart_stm32_common< usart_no, system_clock_type >
{
  using base_type = usart_stm32_common< usart_no, system_clock_type >;

public:

  using resources = typelist<
    typename base_type::resources,
    typename mpl::usart_gpio_rx_resources< gpio_rx_type >::type,
    typename mpl::usart_gpio_tx_resources< gpio_tx_type, gpio_tx_speed >::type
    >;
};

} // namespace mptl

#endif // ARCH_USART_HPP_INCLUDED
