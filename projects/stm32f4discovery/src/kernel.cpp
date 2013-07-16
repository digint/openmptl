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

#include <terminal.hpp>
#include <resource.hpp>
#include "terminal_hooks.hpp"
#include "kernel.hpp"

static volatile int systick_count = 1000;
static volatile int second = 0;


void Kernel::systick_isr() {
  systick_count--;
  if(systick_count == 0) {
    systick_count = 1000;
    second++;
    led_green::toggle();
  }
}

void Kernel::init(void)
{
  //  resources::check();      /* check unique resources */
  //  resources::configure();  /* configure resources (set all shared register) */
  mptl::reg_configure<resources>();

  led_green ::off();
  led_orange::off();
  led_red   ::off();
  led_blue  ::off();

  systick::init();
  systick::enable_interrupt();
}

void Kernel::run(void)
{
  mptl::terminal<usart_stream_device, terminal_hooks::commands> term;

  term.open();
  term.tx_stream << "\r\n\r\nWelcome to OpenMPTL terminal console!\r\n# " << poorman::flush;

  while(1)
  {
    /* poll terminal */
    term.process_input();
  }
}
