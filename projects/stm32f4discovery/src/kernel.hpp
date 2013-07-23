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
#include <terminal.hpp>
#include <typelist.hpp>
#include <compiler.h>

struct Kernel
{
  using rcc           = mptl::rcc< mptl::mhz(168) >;
  using pwr           = mptl::pwr< rcc, mptl::volt(3.3), false >;
  using flash         = mptl::flash< rcc, pwr >;

  using systick       = mptl::systick< rcc, mptl::khz(1), mptl::cfg::systick::clock_source::hclk >;

  using usart = mptl::usart<
    2, rcc,
#ifdef DYNAMIC_BAUD_RATE
    // explicitely do NOT set the baud_rate. this way it's not in the
    // usart_stream_device::resources, and we set BRR by hand using
    // usart::set_baud_rate() in Kernel::init().
#else
    mptl::cfg::usart::baud_rate< 115200 >,
#endif
    mptl::cfg::usart::gpio_rx< 'A', 3 >,
    mptl::cfg::usart::gpio_tx< 'A', 2 >
    >;

  using usart_stream_device = mptl::usart_irq_stream< usart, mptl::ring_buffer<char, 512> >;

  using terminal_type = mptl::terminal< usart_stream_device >;

  using led_green     = mptl::gpio_led< 'D', 12 >;
  using led_orange    = mptl::gpio_led< 'D', 13 >;
  using led_red       = mptl::gpio_led< 'D', 14 >;
  /* fake active_state on led_blue (refer to Kernel::systick_isr() definition in kernel.cpp) */
  using led_blue      = mptl::gpio_led< 'D', 15, mptl::cfg::gpio::active_state::low >;

  /* our static terminal */
  static terminal_type terminal;

  /* Reset exception: triggered on system startup (system entry point). */
  static void __naked reset_isr(void);

  /* Execute the systick isr in RAM, which makes it faster, with predictive execution time */
  static void systick_isr(void) __attribute__ ((long_call, section (".ram_functions")));

  static void null_isr(void)  { }
  static void warn_isr(void)  { Kernel::led_orange::on(); }
  static void error_isr(void) { while(1) { Kernel::led_red::on(); } }

  static void init(void);
  static void __noreturn run(void);

  using irq_resources = mptl::typelist<
    mptl::irq_handler< typename mptl::irq::reset        , reset_isr   >,
    mptl::irq_handler< typename systick::irq            , systick_isr >,
    mptl::irq_handler< typename mptl::irq::nmi          , null_isr    >,
    mptl::irq_handler< typename mptl::irq::sv_call      , null_isr    >,
    mptl::irq_handler< typename mptl::irq::debug_monitor, null_isr    >,
    mptl::irq_handler< typename mptl::irq::pend_sv      , null_isr    >
    >;

  using resources = mptl::typelist<
    irq_resources,
    systick::resources,
    led_green::resources,
    led_orange::resources,
    led_red::resources,
    led_blue::resources,
    usart_stream_device::resources
    >::type;
};

#endif // KERNEL_HPP_INCLUDED
