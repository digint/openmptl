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

// TODO: provide a matrix for the gpio port/pin_no
// TODO: this goes to gpio.hpp (rename gpio_usart_rx)

template< typename usart,
          char     port,
          unsigned pin_no >
class usart_gpio_rx
: public gpio_input_af< port,
                        pin_no,
                        usart::usart_no <= 3 ? 7 : 8, // alt_func_num
                        cfg::gpio::resistor::floating
                    >
{ };


template< typename usart,
          char     port,
          unsigned pin_no >
class usart_gpio_tx
: public gpio_output_af< port,
                         pin_no,
                         usart::usart_no <= 3 ? 7 : 8, // alt_func_num
                         cfg::gpio::output_type::push_pull,
                         cfg::gpio::resistor::floating,
                         mhz(50)
                     >
{ };

} // namespace mptl

#endif // USART_HPP_INCLUDED

