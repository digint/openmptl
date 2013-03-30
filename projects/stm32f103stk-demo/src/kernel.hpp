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

#include <resource.hpp>
#include <lcd/nokia3310/lcd.hpp>
#include <rf/nrf24l01/nrf24l01.hpp>
#include <joystick/stm32f103stk/joystick.hpp>
#include <arch/rcc.hpp>
#include <arch/flash.hpp>
#include <arch/usart.hpp>
#include <arch/uart_transport.hpp>
#include <arch/gpio.hpp>
#include <arch/nvic.hpp>
#include <compiler.h>
#include "time.hpp"

struct Kernel
{
  using rcc   = Rcc< 72_mhz >;
  using flash = Flash< rcc >;

  using systick = SysTick<rcc, 100_hz, SysTickClockSource::hclk>;

  using usart        = Usart<2>;

  using usart_gpio_tx = UsartGpioTx< 'A', 2 >;
  using usart_gpio_rx = UsartGpioRx< 'A', 3 >;
  using uart_stream_device = UartIrqStream< usart, RingBuffer<char, 512>, true, true >; /* irq debug enabled */

#ifndef USART_DYNAMIC
  using usart_config = UsartConfig< rcc, 115200 >;  // tested up to 2250000 baud
  using uart_device = UartDevice< uart_stream_device, usart_config>;
#endif

  //  using uart_stream_device = UartStreamDevice< usart, RingBuffer<char, 512>, true, true >; /* irq debug enabled */

  using spi       = Spi< rcc, 1 >;
  using spi_sck   = SpiGpio< 'A', 5 >;
  using spi_miso  = SpiGpio< 'A', 6 >;
  using spi_mosi  = SpiGpio< 'A', 7 >;

  using lcd_ds    = GpioOutput< 'B', 2,  GpioOutputConfig::push_pull >;  //< low=command, high=data
  using lcd_reset = GpioOutput< 'C', 7,  GpioOutputConfig::push_pull >;  //< reset pin (active low)
  using lcd_e     = GpioOutput< 'C', 10, GpioOutputConfig::push_pull >;  //< display controller spi enable (active low)
  using lcd       = Lcd_Nokia3310< spi, lcd_ds, lcd_reset, lcd_e >;

  using nrf_ce    = GpioOutput< 'C', 8, GpioOutputConfig::push_pull >;  //< chip enable
  using nrf_csn   = GpioOutput< 'A', 4, GpioOutputConfig::push_pull >;  //< spi enable (active low)
  using nrf_irq   = GpioInput < 'C', 9, GpioInputConfig::pull_down >;   //< IRQ
  using nrf       = Nrf24l01< spi, nrf_ce, nrf_csn, nrf_irq >;

  using joy       = Joystick;
  using led       = GpioLed< 'C', 12, GpioOutputConfig::push_pull, 50_mhz, GpioActiveState::low >;
  using time      = Time< systick >;

  /* Reset core exception: triggered on system startup (system entry point). */
  static void  __naked reset_isr(void);

  static void null_isr(void)  { }
  static void warn_isr(void)  { Kernel::led::on(); }
  static void error_isr(void) { while(1) { Kernel::led::on(); } }

  static void init(void);
  static void __noreturn run(void);

  using resources = ResourceList<
    IrqResource< typename irq::Reset, reset_isr >,

    time::resources,
    joy::resources,
    led::resources,

    spi_sck::resources,
    spi_miso::resources,
    spi_mosi::resources,
    spi::resources,
    lcd::resources,
    nrf::resources,

    usart_gpio_rx::resources,
    usart_gpio_tx::resources,
    uart_stream_device::resources
  >;
};

#endif // KERNEL_HPP_INCLUDED
