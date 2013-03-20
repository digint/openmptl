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

#include <arch/nvic.hpp>
#include <arch/rcc.hpp>
#include <arch/pwr.hpp>
#include <arch/flash.hpp>
#include <arch/systick.hpp>
#include <arch/gpio.hpp>
#include <arch/uart_transport.hpp>
#include <arch/core.hpp>
#include <resource_mpl.hpp>

struct Kernel
{
  using rcc   = Rcc<168_mhz>;
  using pwr   = Pwr<rcc, 3.3_volt, false>;
  using flash = Flash<rcc, pwr>;

  using systick    = SysTick<rcc, 1_khz, SysTickClockSource::hclk>;

  using usart = Usart<rcc, 2, 115200>;
  using usart_gpio_tx = UsartGpioTx< usart, 'A', 2 >;
  using usart_gpio_rx = UsartGpioRx< usart, 'A', 3 >;

  using uart_stream_device = UartStreamDevice< usart >;

  using led_green  = GpioLed<'D', 12>;
  using led_orange = GpioLed<'D', 13>;
  using led_red    = GpioLed<'D', 14>;
  using led_blue   = GpioLed<'D', 15>;

  /* Reset exception: triggered on system startup (system entry point). */
  static void reset_isr(void) __attribute__ ((naked)) {
    Core::startup<rcc, flash, pwr>();

    Kernel::init();
    Kernel::run();
  };

  /* Execute the systick isr in RAM, which makes it faster, with predictive execution time */
  static void systick_isr(void) __attribute__ ((long_call, section (".ram_functions")));

  static void null_isr(void)  { }
  static void warn_isr(void)  { Kernel::led_orange::on(); }
  static void error_isr(void) { while(1) { Kernel::led_red::on(); } }

  static void init(void);
  static void run(void) __attribute__ ((noreturn));

  using resources = ResourceList<
    IrqResource< typename irq::Reset       , reset_isr   >,
    IrqResource< typename systick::Irq     , systick_isr >,
    IrqResource< typename irq::NMI         , null_isr    >,
    IrqResource< typename irq::SVCall      , null_isr    >,
    IrqResource< typename irq::DebugMonitor, null_isr    >,
    IrqResource< typename irq::PendSV      , null_isr    >,

    systick::resources,
    led_green::resources,
    led_orange::resources,
    led_red::resources,
    led_blue::resources,
    usart_gpio_rx::resources,
    usart_gpio_tx::resources,
    uart_stream_device::resources
    >;
};

#endif // KERNEL_HPP_INCLUDED
