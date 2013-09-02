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
#include <terminal.hpp>
#include <typelist.hpp>
#include <compiler.h>
#include "time.hpp"

struct Kernel
{
  using sysclk = mptl::system_clock_hse< mptl::mhz(72) >;

  using flash_cfg = mptl::reglist<
    mptl::flash::latency::minimum< sysclk >,
    mptl::flash::prefetch_buffer_enable
    >;

  using systick = mptl::systick<
    mptl::systick_clock::external< sysclk, mptl::hz(100) >
    >;

  using usart = mptl::usart< 2, sysclk, mptl::gpio< 'A', 3 >, mptl::gpio< 'A', 2 > >;
  using usart_cfg = mptl::reglist<
    usart::baud_rate< 115200 >,
    usart::enable_rx,
    usart::enable_tx
    >;

  using usart_stream_device = mptl::usart_irq_stream< usart, mptl::ring_buffer<char, 512>, true, true >; /* irq debug enabled */
  using terminal_type = mptl::terminal< usart_stream_device >;

  using spi = mptl::spi< 1, sysclk, mptl::gpio< 'A', 5 >, mptl::gpio< 'A', 6 >, mptl::gpio< 'A', 7 > >;

  using lcd = mptl::device::nokia3310<
    spi,
    mptl::gpio_output< 'B', 2  >,  /* lcd_ds     */
    mptl::gpio_output< 'C', 7  >,  /* lcd_reset  */
    mptl::gpio_output< 'C', 10 >   /* lcd_e      */
    >;

  using nrf = mptl::device::nrf24l01<
    spi,
    mptl::gpio_output< 'C', 8 >,  /* nrf_ce   */
    mptl::gpio_output< 'A', 4 >,  /* nrf_csn  */
    mptl::gpio_input < 'C', 9 >   /* nrf_irq  */
    >;

  using joy = mptl::device::joystick;

  using led = mptl::gpio_led< 'C', 12, mptl::gpio_active_state::low >;

  using rtc = mptl::rtc< mptl::hz(0x8000) >;  /* 32.768 kHz LSE clock */
  using time = Time< systick, rtc >;

  static terminal_type terminal;

  /* Reset core exception: triggered on system startup (system entry point). */
  static void  __naked reset_isr(void);

  static void null_isr(void)  { }
  static void warn_isr(void)  { Kernel::led::on(); }
  static void error_isr(void) { while(1) { Kernel::led::on(); } }

  static void init(void);
  static void __noreturn run(void);

  using resources = mptl::typelist<
    mptl::irq_handler< typename mptl::irq::reset, reset_isr >,

    time::resources,
    joy::resources,
    led::resources,
    spi::resources,
    lcd::resources,
    nrf::resources,

    usart::resources,
    usart_cfg,
    terminal_type::resources
  >;
};

#endif // KERNEL_HPP_INCLUDED
