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

#include <lcd/nokia3310/lcd.hpp>
#include <rf/nrf24l01/nrf24l01.hpp>
#include <joystick/stm32f103stk/joystick.hpp>
#include <arch/rcc.hpp>
#include <arch/flash.hpp>
#include <arch/usart.hpp>
#include <arch/usart_stream.hpp>
#include <arch/gpio.hpp>
#include <arch/nvic.hpp>
#include <peripheral_device.hpp>
#include <typelist.hpp>
#include <compiler.h>
#include "time.hpp"

struct Kernel
{
  using rcc     = mptl::rcc< mptl::mhz(72) >;
  using flash   = mptl::flash< rcc >;

  using systick = mptl::systick< rcc, mptl::hz(100), mptl::cfg::systick::clock_source::hclk >;


  using usart = mptl::usart< 2, rcc >;
  using tty0_device = mptl::periph<
    usart,
    usart::baud_rate< 115200 >,
    usart::gpio_rx< 'A', 3 >, /* implicitely sets USARTx::CR1::RE (rx enable) */
    usart::gpio_tx< 'A', 2 >  /* implicitely sets USARTx::CR1::TE (tx enable) */
    >;

  using usart_stream_device = mptl::usart_irq_stream< tty0_device, mptl::ring_buffer<char, 512>, true, true >; /* irq debug enabled */

  using spi       = mptl::spi< rcc, 1 >;
  using spi_sck   = mptl::spi_gpio< 'A', 5 >;
  using spi_miso  = mptl::spi_gpio< 'A', 6 >;
  using spi_mosi  = mptl::spi_gpio< 'A', 7 >;

  using lcd = mptl::device::nokia3310<
    spi,
    mptl::gpio_output< 'B', 2,  mptl::cfg::gpio::output::push_pull >,  //< lcd_ds: low=command, high=data
    mptl::gpio_output< 'C', 7,  mptl::cfg::gpio::output::push_pull >,  //< lcd_reset: reset pin (active low)
    mptl::gpio_output< 'C', 10, mptl::cfg::gpio::output::push_pull >   //< lcd_e: display controller spi enable (active low)
    >;

  using nrf = mptl::device::nrf24l01<
    spi,
    mptl::gpio_output< 'C', 8, mptl::cfg::gpio::output::push_pull >,  //< nrf_ce: chip enable
    mptl::gpio_output< 'A', 4, mptl::cfg::gpio::output::push_pull >,  //< nrf_csn: spi enable (active low)
    mptl::gpio_input < 'C', 9, mptl::cfg::gpio::input::pull_down >    //< nrf_irq: IRQ
    >;

  using joy = mptl::device::joystick;

  using led = mptl::gpio_led<
    'C', 12,
    mptl::cfg::gpio::output::push_pull,
    mptl::mhz(2),
    mptl::cfg::gpio::active_state::low
    >;

  using time = Time< systick >;

  /* Reset core exception: triggered on system startup (system entry point). */
  static void  __naked reset_isr(void);

  static void null_isr(void)  { }
  static void warn_isr(void)  { Kernel::led::on(); }
  static void error_isr(void) { while(1) { Kernel::led::on(); } }

  static void init(void);
  static void __noreturn run(void);

  using resources = mptl::typelist<
    mptl::resource::irq< typename mptl::irq::reset, reset_isr >,

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
    usart_stream_device::resources
  >;
};

#endif // KERNEL_HPP_INCLUDED
