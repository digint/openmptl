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

  /* Note that setting gpio_rx_type (and gpio_tx_type respectively)
   * template parameter implicitely adds the correct mptl::gpio<>
   * configuration traits to usart::resources. This means that by
   * calling mptl::core::configure<resources>(), the GPIO registers
   * are automatically setup the way we need them for USART
   * communication.
   */
  using usart = mptl::usart< 2, rcc, mptl::gpio< 'A', 3 >, mptl::gpio< 'A', 2 > >;
  using usart_device = mptl::periph<
    usart,
#ifdef DYNAMIC_BAUD_RATE
    /* Explicitely do NOT set the baud_rate.
     * This way it's not in usart_device::resources, and we set BRR by
     * hand using usart::set_baud_rate(unsigned) in Kernel::init().
     */
#else
    usart::baud_rate< 115200 >,
#endif
    usart::enable_rx,
    usart::enable_tx
    >;
  using usart_stream_device = mptl::usart_irq_stream< usart_device, mptl::ring_buffer<char, 512> >;
  using terminal_type = mptl::terminal< usart_stream_device >;

  using led_green     = mptl::gpio_led< 'D', 12 >;
  using led_orange    = mptl::gpio_led< 'D', 13 >;
  using led_red       = mptl::gpio_led< 'D', 14 >;
  /* fake active_state on led_blue (refer to Kernel::systick_isr() definition in kernel.cpp) */
  using led_blue      = mptl::gpio_led< 'D', 15, mptl::gpio_active_state::low >;

  /* our static terminal (bound to usart_irq_stream<usart_device>) */
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

  /* Define the irq handler resources.
   *
   * Override the default handler by our interupt service
   * routines (*_isr).
   *
   * Note that mptl::vector_table<> filters the resources list by
   * irq_handler<> types, and lists them in the interrupt vector table
   * (isr_vector[] in section ".isr_vector").
   *
   * Each mptl::irq::* type in mptl::irq_handler<Tp,isr_t> must appear
   * only once in the typelist<>. This is asserted by
   * mptl::vector_table<> using typelist::unique_element<>:
   *
   *     In instantiation of 'struct mptl::mpl::unique_element_impl<...>':
   *     error: static assertion failed: list contains more than one element
   *
   * (refer to section "interpreting mpl compiler output" if you're
   * not familiar with template metaprogramming and see the error above)
   *
   * NOTE: peripheral types (e.g. usart_irq_stream<>) also list
   * irq_handler<> in their ::resources.
   *
   * NOTE: the default irq handler is set by the mptl::vector_table<>
   * instantiation in startup.cpp.
   */
  using irq_resources = mptl::typelist<
    mptl::irq_handler< typename mptl::irq::reset        , reset_isr   >,
    mptl::irq_handler< typename systick::irq            , systick_isr >,
    mptl::irq_handler< typename mptl::irq::nmi          , null_isr    >,
    mptl::irq_handler< typename mptl::irq::sv_call      , null_isr    >,
    mptl::irq_handler< typename mptl::irq::debug_monitor, null_isr    >,
    mptl::irq_handler< typename mptl::irq::pend_sv      , null_isr    >
    >;

  /* Define the resources typelist.
   *
   * Note: listing usart::resources is actually not needed, since it
   * is implicitely inherited in usart_device::resources (inherited by
   * usart_stream_device::resources, inherited by terminal_type::resources).
   * Listing it multiple times would not harm, and is explicitely
   * allowed by mptl::core::configure<resources>().
   */
  using resources = mptl::typelist<
    irq_resources,
    systick::resources,
    led_green::resources,
    led_orange::resources,
    led_red::resources,
    led_blue::resources,
    // usart::resources,                // implicit in usart_device::resources
    // usart_device::resources,         // implicit in usart_stream_device::resources
    // usart_stream_device::resources,  // implicit in terminal_type::resources
    terminal_type::resources
    >::type;
};

#endif // KERNEL_HPP_INCLUDED
