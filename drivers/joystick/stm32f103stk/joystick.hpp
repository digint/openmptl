/*
 * CppCore - C++ microprocessor core library
 *
 * Copyright 2012 Axel Burri <axel@tty0.ch>
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

#include "arch/adc.hpp"

class Joystick
{
  typedef GpioInput<'C', 6, cGpio::InputConfig::floating, cGpio::ActiveState::high> button;
  typedef Adc<1, cAdc::Mode::independent, cAdc::ScanMode::disabled, cAdc::ContinuousConvMode::single, cAdc::ExternalTrigConv::none, cAdc::DataAlign::right, 1> adc;
  
  static int getValue(void) {
    adc::regular_channel_config<15, 1, cAdc::SampleTime::cycles_55_5>();
    adc::enable_software_start_conversion();
    adc::wait_EOC();
    return adc::get_conversion_value();
  }

public:

  typedef ResourceList< typename adc::resources,
                        typename button::resources
                        > resources;


  enum class Position { center, up, down, left, right };

  static void init(void) {
//    Core::RCC->APB2ENR |= Core::RCC->APB2ENR_ADC1EN | Core::RCC->APB2ENR_ADC2EN | Core::RCC->APB2ENR_IOPAEN;
    adc::deinit();
    adc::init();
    adc::enable();

    button::init();
  }

  static Position getPosition(void) {
    int value = getValue();

    int up_value    =  960;
    int down_value  =  190;
    int left_value  = 1990;
    int right_value =  470;
    int diversion   =   30;

    if((value > (up_value    - diversion)) && (value < (up_value    + diversion)) ) { return Position::up;    }
    if((value > (down_value  - diversion)) && (value < (down_value  + diversion)) ) { return Position::down;  }
    if((value > (left_value  - diversion)) && (value < (left_value  + diversion)) ) { return Position::left;  }
    if((value > (right_value - diversion)) && (value < (right_value + diversion)) ) { return Position::right; }

    return Position::center;
  }

  static bool buttonPressed(void) {
    return button::active();
  }
};

#endif // JOYSTICK_HPP_INCLUDED


