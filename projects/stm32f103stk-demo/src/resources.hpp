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
#include <lcd/nokia3310/lcd.hpp>
#include <rf/nrf24l01/nrf24l01.hpp>
#include <joystick/stm32f103stk/joystick.hpp>
#include <arch/rtc.hpp>
#include <arch/usart.hpp>
#include <arch/nvic.hpp>


namespace resources
{
  typedef GpioOutput<'B', 2,  cGpio::OutputConfig::push_pull>  lcd_ds;     //< low=command, high=data
  typedef GpioOutput<'C', 7,  cGpio::OutputConfig::push_pull>  lcd_reset;  //< reset pin (active low)
  typedef GpioOutput<'C', 10, cGpio::OutputConfig::push_pull>  lcd_e;      //< display controller spi enable (active low)

  typedef Lcd_Nokia3310<1, lcd_ds, lcd_reset, lcd_e> lcd_n3310;

  typedef GpioOutput<'C', 8, cGpio::OutputConfig::push_pull>  nrf_ce;     //< chip enable
  typedef GpioOutput<'A', 4, cGpio::OutputConfig::push_pull>  nrf_csn;    //< spi enable (active low)
  typedef GpioInput <'C', 9, cGpio::InputConfig::pull_down>   nrf_irq;    //< IRQ

  typedef Nrf24l01<1, nrf_ce, nrf_csn, nrf_irq> nrf;

  typedef Joystick joy;
  typedef GpioLed<'C', 12> led;
  typedef Rtc rtc;
  //  typedef SysTick<100_hz, cSysTick::ClockSource::hclk_div8> systick;
  typedef SysTick<100_hz, cSysTick::ClockSource::hclk> systick;
  //  typedef Usart<2, 2250000> usart; // works
  typedef Usart<2, 115200> usart;


  typedef ResourceList< typename lcd_n3310::resources,
                        typename nrf::resources,
                        typename joy::resources,
                        typename led::resources,
                        typename rtc::resources,
                        typename systick::resources,
                        typename usart::resources
                        > list;

}
#endif
