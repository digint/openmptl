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

#ifndef COMMON_GPIO_BASE_HPP_INCLUDED
#define COMMON_GPIO_BASE_HPP_INCLUDED

namespace mptl {

enum class gpio_active_state { low, high };

////////////////////  gpio_input  ////////////////////


template< typename base_type, gpio_active_state active_state >
class gpio_input_base
: public base_type
{
public:
  static bool active(void) {
    bool input = base_type::read_input_bit();
    return active_state == gpio_active_state::low ? !input : input;
  }
};


////////////////////  gpio_output  ////////////////////


template< typename base_type, gpio_active_state active_state >
class gpio_output_base
: public base_type
{
public:
  static void enable() {
    if(active_state == gpio_active_state::low) {
      base_type::reset();
    } else {
      base_type::set();
    }
  }

  static void disable() {
    if(active_state == gpio_active_state::low) {
      base_type::set();
    } else {
      base_type::reset();
    }
  }

  static bool active() {
    bool input = base_type::read_input_bit();
    return active_state == gpio_active_state::low ? !input : input;
  }

  static void toggle() {
    if(base_type::read_input_bit()) {
      base_type::reset();
    }
    else {
      base_type::set();
    }
  }

  static bool latched() {
    bool output = base_type::read_output_bit();
    return active_state == gpio_active_state::low ? !output : output;
  }
};


////////////////////  gpio_analog_io  ////////////////////


template< typename base_type >
class gpio_analog_io_base
: public base_type
{
  // TODO: get/set analog value
};


////////////////////  gpio_led  ////////////////////


/** NOTE: needs class derived from gpio_output_base<> as base type */
template< typename base_type >
class gpio_led_base
: public base_type
{
public:
  static void on() {
    base_type::enable();
  }
  static void off() {
    base_type::disable();
  }
};

} // namespace mptl


#endif // COMMON_GPIO_BASE_HPP_INCLUDED
