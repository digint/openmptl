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

#ifndef KERNEL_HPP_INCLUDED
#define KERNEL_HPP_INCLUDED

#include <arch/nvic.hpp>
#include <arch/rcc.hpp>
#include <arch/pwr.hpp>
#include <arch/flash.hpp>
#include <arch/systick.hpp>
#include <arch/gpio.hpp>
#include <arch/usart.hpp>
#include <arch/usart_stream.hpp>
#include <peripheral_device.hpp>
#include <resource.hpp>
#include <compiler.h>

struct Kernel
{
  using rcc           = mptl::rcc< mptl::mhz(168) >;
  using pwr           = mptl::pwr< rcc, mptl::volt(3.3), false >;
  using flash         = mptl::flash< rcc, pwr >;

  using systick       = mptl::systick< rcc, mptl::khz(1), mptl::cfg::systick::clock_source::hclk >;

  using usart         = mptl::usart< 2, rcc >;
  using usart_gpio_tx = mptl::usart_gpio_tx< usart, 'A', 2 >;
  using usart_gpio_rx = mptl::usart_gpio_rx< usart, 'A', 3 >;

  struct usart_tty0_config : mptl::cfg::usart::preset {
    static constexpr unsigned baud_rate = 115200;
  };
  using tty0_device   = mptl::peripheral_device< usart, usart_tty0_config >;
  using usart_stream_device = mptl::usart_irq_stream< usart, mptl::ring_buffer<char, 512> >;

  using led_green     = mptl::gpio_led< 'D', 12 >;
  using led_orange    = mptl::gpio_led< 'D', 13 >;
  using led_red       = mptl::gpio_led< 'D', 14 >;
  using led_blue      = mptl::gpio_led< 'D', 15 >;

  /* Reset exception: triggered on system startup (system entry point). */
  static void __naked reset_isr(void);

  /* Execute the systick isr in RAM, which makes it faster, with predictive execution time */
  static void systick_isr(void) __attribute__ ((long_call, section (".ram_functions")));

  static void null_isr(void)  { }
  static void warn_isr(void)  { Kernel::led_orange::on(); }
  static void error_isr(void) { while(1) { Kernel::led_red::on(); } }

  static void init(void);
  static void __noreturn run(void);

  using resources = mptl::resource::list<
    mptl::resource::irq< typename mptl::irq::reset        , reset_isr   >,
    mptl::resource::irq< typename systick::irq            , systick_isr >,
    mptl::resource::irq< typename mptl::irq::nmi          , null_isr    >,
    mptl::resource::irq< typename mptl::irq::sv_call      , null_isr    >,
    mptl::resource::irq< typename mptl::irq::debug_monitor, null_isr    >,
    mptl::resource::irq< typename mptl::irq::pend_sv      , null_isr    >,

    systick::resources,
    led_green::resources,
    led_orange::resources,
    led_red::resources,
    led_blue::resources,
    usart_gpio_rx::resources,
    usart_gpio_tx::resources,
    usart_stream_device::resources
    >;
};

#endif // KERNEL_HPP_INCLUDED
