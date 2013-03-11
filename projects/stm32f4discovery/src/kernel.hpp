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
#include <arch/systick.hpp>
#include <arch/gpio.hpp>
#include <arch/uart_transport.hpp>
#include <crt.hpp>
#include <resource_mpl.hpp>

class Kernel
{
  /* Reset core exception: triggered on system startup (system entry point). */
  static void reset_isr(void) __attribute__ ((naked)) {
    CRunTimeIrqWrap<core> crt;  /* clear data, init bss, initialize cpu clocks, call constructors */

    Kernel::init();
    Kernel::run();
  };

  static void null_isr(void)  { }
  static void warn_isr(void)  { Kernel::led_orange::on(); }
  static void error_isr(void) { while(1) { Kernel::led_red::on(); } }

  /* Execute the systick isr in RAM, which makes it faster, with predictive execution time */
  static void systick_isr(void) __attribute__ ((long_call, section (".ram_functions")));

  using core       = Core< 168_mhz, 3.3_volt, false >;
  using systick    = SysTick<core, 1_khz, SysTickClockSource::hclk>;

  using uart_stream_device = UartStreamDevice< Usart<core, 2, 115200> >;
  using uart_gpio_tx = GpioOutputAF<'A', 2, 7, GpioOutputType::push_pull, GpioResistorConfig::floating, 25_mhz>;
  using uart_gpio_rx = GpioInputAF <'A', 3, 7>;

  using graceful_irq_resources = ResourceList <
    IrqResource<CoreException::NMI         ::irq_number, null_isr>,
    IrqResource<CoreException::SVCall      ::irq_number, null_isr>,
    IrqResource<CoreException::DebugMonitor::irq_number, null_isr>,
    IrqResource<CoreException::PendSV      ::irq_number, null_isr>
    // IrqResource<CoreException::SysTick     ::irq_number, null_isr>
    >;

public:

  using led_green  = GpioLed<'D', 12>;
  using led_orange = GpioLed<'D', 13>;
  using led_red    = GpioLed<'D', 14>;
  using led_blue   = GpioLed<'D', 15>;

  using resources = ResourceList<
    IrqResource< CoreException::Reset::irq_number, reset_isr >,
    IrqResource< systick::Irq::irq_number, systick_isr >,
    graceful_irq_resources,

    systick::resources,
    led_green::resources,
    led_orange::resources,
    led_red::resources,
    led_blue::resources,
    uart_stream_device::resources,
    uart_gpio_rx::resources,
    uart_gpio_tx::resources >;

  static void init(void);
  static void run(void) __attribute__ ((noreturn));
};

#endif // KERNEL_HPP_INCLUDED
