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

#ifndef USART_HPP_INCLUDED
#define USART_HPP_INCLUDED

#include <arch/gpio.hpp>
#include "../../../common/usart.hpp"

namespace mptl {

namespace mpl
{
  template< typename gpio_type, unsigned gpio_alt_func_num >
  struct usart_gpio_rx_resources {
    using type = typelist<
      typename gpio_type::resources,
      typename gpio_type::resistor::floating,
      typename gpio_type::mode::alternate_function,  // implicitely set by alt_func_num, but does not harm
      typename gpio_type::template alt_func_num< gpio_alt_func_num >
      >;
  };
  template< unsigned gpio_alt_func_num >
  struct usart_gpio_rx_resources< void, gpio_alt_func_num > {
    using type = void;
  };

  template< typename gpio_type, freq_t gpio_speed, unsigned gpio_alt_func_num >
  struct usart_gpio_tx_resources {
    using type = typelist<
      typename gpio_type::resources,
      typename gpio_type::output_type::push_pull,
      typename gpio_type::resistor::floating,
      typename gpio_type::template speed< gpio_speed >,
      typename gpio_type::mode::alternate_function,  // implicitely set by alt_func_num, but does not harm
      typename gpio_type::template alt_func_num< gpio_alt_func_num >
      >;
  };
  template< freq_t gpio_speed, unsigned gpio_alt_func_num >
  struct usart_gpio_tx_resources< void, gpio_speed, gpio_alt_func_num > {
    using type = void;
  };
} // namespace mpl


template<
  unsigned usart_no,
  typename system_clock_type,
  typename gpio_rx_type,
  typename gpio_tx_type,
  freq_t gpio_tx_speed = mhz(50)
  >
class usart : public usart_stm32_common< usart_no, system_clock_type >
{
  using base_type = usart_stm32_common< usart_no, system_clock_type >;

  static constexpr unsigned gpio_alt_func_num = (usart_no <= 3) ? 7 : 8;

public:

  using resources = typelist<
    typename base_type::resources,
    typename mpl::usart_gpio_rx_resources< gpio_rx_type, gpio_alt_func_num >::type,
    typename mpl::usart_gpio_tx_resources< gpio_tx_type, gpio_tx_speed, gpio_alt_func_num >::type
    >;
};

} // namespace mptl


#endif // USART_HPP_INCLUDED
