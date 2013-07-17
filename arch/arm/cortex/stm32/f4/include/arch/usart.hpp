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

namespace mptl { namespace cfg { namespace usart {

/** 
 * Provide GPIO port/pin_no for RX (used for configuration of the GPIO registers).
 * NOTE: this implicitely sets the enable_rx option!
 */
// TODO: provide a matrix for the gpio port/pin_no
template<char port, unsigned pin_no>
struct gpio_rx
: public enable_rx
{
  template<typename usart>
  using resources = typename mptl::gpio< // gpio_input_af<
    port,
    pin_no,
    cfg::gpio::resistor::floating,

    cfg::gpio::mode::alternate_function,  // implicitely set by alt_func_num, but does not harm
    cfg::gpio::alt_func_num< (usart::usart_no <= 3) ? 7 : 8 >
  >::resources;
};


/** 
 * Provide GPIO port/pin_no for TX (used for configuration of the GPIO registers).
 * NOTE: this implicitely sets the enable_tx option!
 */
template<char port, unsigned pin_no>
struct gpio_tx
: public enable_tx
{
  template<typename usart>
  using resources = typename mptl::gpio< // gpio_output_af<
    port,
    pin_no,
    cfg::gpio::output_type::push_pull,
    cfg::gpio::resistor::floating,
    cfg::gpio::speed< mhz(50) >,

    cfg::gpio::mode::alternate_function,  // implicitely set by alt_func_num, but does not harm
    cfg::gpio::alt_func_num< (usart::usart_no <= 3) ? 7 : 8 >
  >::resources;
};

} } } // namespace mptl::cfg::usart


#endif // USART_HPP_INCLUDED

