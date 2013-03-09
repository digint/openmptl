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

#ifndef KERNEL_HPP_INCLUDED
#define KERNEL_HPP_INCLUDED

#include <crt.hpp>
#include <resource.hpp>
#include <arch/gpio.hpp>
#include <lcd/nokia3310/lcd.hpp>
#include <rf/nrf24l01/nrf24l01.hpp>
#include <joystick/stm32f103stk/joystick.hpp>
#include <arch/usart.hpp>
#include <arch/uart_transport.hpp>
#include <arch/nvic.hpp>
#include "time.hpp"

class Kernel
{
  /* Reset core exception: triggered on system startup (system entry point). */
  static void reset_isr(void) __attribute__ ((naked)) {
    CRunTimeIrqWrap crt;  /* clear data, init bss, initialize cpu clocks, call constructors */

    Kernel::init();
    Kernel::run();
  };

  static void null_isr(void)  { }
  static void warn_isr(void)  { Kernel::led::on(); }
  static void error_isr(void) { while(1) { Kernel::led::on(); } }

  using lcd_ds    = GpioOutput<'B', 2,  GpioOutputConfig::push_pull>;  //< low=command, high=data
  using lcd_reset = GpioOutput<'C', 7,  GpioOutputConfig::push_pull>;  //< reset pin (active low)
  using lcd_e     = GpioOutput<'C', 10, GpioOutputConfig::push_pull>;  //< display controller spi enable (active low)

  using nrf_ce    = GpioOutput<'C', 8, GpioOutputConfig::push_pull>;  //< chip enable
  using nrf_csn   = GpioOutput<'A', 4, GpioOutputConfig::push_pull>;  //< spi enable (active low)
  using nrf_irq   = GpioInput <'C', 9, GpioInputConfig::pull_down>;   //< IRQ

  using usart     = Usart<2, 115200>;  // tested up to 2250000 baud

  using uart_stream_device = UartStreamDevice<usart, true>; /* irq debug enabled */
  using uart_gpio_tx = GpioOutput< 'A', 2,  GpioOutputConfig::alt_push_pull >;
  using uart_gpio_rx = GpioInput < 'A', 3,  GpioInputConfig::floating >;

  using spi      = Spi<1>;
  using spi_sck  = GpioOutput<'A', 5, GpioOutputConfig::alt_push_pull>;
  using spi_miso = GpioOutput<'A', 6, GpioOutputConfig::alt_push_pull>;
  using spi_mosi = GpioOutput<'A', 7, GpioOutputConfig::alt_push_pull>;

public:

  using lcd      = Lcd_Nokia3310<spi, lcd_ds, lcd_reset, lcd_e>;
  using nrf      = Nrf24l01<spi, nrf_ce, nrf_csn, nrf_irq>;
  using joy      = Joystick;
  using led      = GpioLed<'C', 12>;
  using time     = Time;

  using resources = ResourceList<
    IrqResource< CoreException::Reset::irq_number, reset_isr >,

    time::resources,
    joy::resources,
    led::resources,

    spi_sck::resources,
    spi_miso::resources,
    spi_mosi::resources,
    lcd::resources,
    nrf::resources,

    uart_gpio_rx::resources,
    uart_gpio_tx::resources,
    uart_stream_device::resources
  >;

  static void init(void);
  static void run(void) __attribute__ ((noreturn));
};

#endif // KERNEL_HPP_INCLUDED