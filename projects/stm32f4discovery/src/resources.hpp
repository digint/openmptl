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

#ifndef RESOURCES_HPP_INCLUDED
#define RESOURCES_HPP_INCLUDED

#include <arch/systick.hpp>
#include <arch/gpio.hpp>


namespace resources
{
  typedef SysTick<100_hz, cSysTick::ClockSource::hclk_div8> systick;

  typedef GpioLed<'D', 12, cGpio::OutputConfig::push_pull, 50_mhz, cGpio::ActiveState::high> led_green;
  typedef GpioLed<'D', 13, cGpio::OutputConfig::push_pull, 50_mhz, cGpio::ActiveState::high> led_orange;
  typedef GpioLed<'D', 14, cGpio::OutputConfig::push_pull, 50_mhz, cGpio::ActiveState::high> led_red;
  typedef GpioLed<'D', 15, cGpio::OutputConfig::push_pull, 50_mhz, cGpio::ActiveState::high> led_blue;

  typedef ResourceList< typename systick::resources,
                        typename led_green::resources,
                        typename led_orange::resources,
                        typename led_red::resources,
                        typename led_blue::resources
                        > list;
}

#endif // RESOURCES_HPP_INCLUDED