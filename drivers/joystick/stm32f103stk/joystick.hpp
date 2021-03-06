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

#ifndef JOYSTICK_HPP_INCLUDED
#define JOYSTICK_HPP_INCLUDED

#include <arch/adc.hpp>

namespace mptl { namespace device {

class joystick
{
  using button = mptl::gpio_input< 'C', 6, gpio_active_state::high >;

  using adc = mptl::adc< 1 >;
  using adc_cfg = mptl::reglist<
    adc::external_trigger_conversion::software_start,
    adc::regular_channel_sequence_length< 1 >,
    adc::regular_channel_config< 15, 1, adc::sample_time::cycles_55_5 >
    >;
  
  static int adc_convert_blocking(void) {
    adc::enable_software_start_conversion();
    adc::wait_eoc();
    return adc::get_conversion_value();
  }

public:

  using resources = mptl::typelist<
    typename adc::resources,
    adc_cfg,
    typename button::resources,

    /* configure GPIO's */
    typename button::input_type::floating
  >;

  enum class position { center, up, down, left, right };

  static void enable(void) {
    adc::enable();
  }

  static void configure(void) {
    adc::template configure< adc_cfg >();
  }

  static position get_position_blocking(void) {
    int value = adc_convert_blocking();

    int up_value    =  960;
    int down_value  =  190;
    int left_value  = 1990;
    int right_value =  470;
    int threshold   =   30;

    if((value > (up_value    - threshold)) && (value < (up_value    + threshold)) ) { return position::up;    }
    if((value > (down_value  - threshold)) && (value < (down_value  + threshold)) ) { return position::down;  }
    if((value > (left_value  - threshold)) && (value < (left_value  + threshold)) ) { return position::left;  }
    if((value > (right_value - threshold)) && (value < (right_value + threshold)) ) { return position::right; }

    return position::center;
  }

  static bool button_pressed(void) {
    return button::active();
  }
};

} } // namespace mptl::device

#endif // JOYSTICK_HPP_INCLUDED
