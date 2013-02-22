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

#include <kernel.hpp>
#include <arch/core.hpp>
#include <arch/uart_transport.hpp>

#include "resources.hpp"
#include "terminal_hooks.hpp"

using namespace resources;

static volatile int systick_count = 1000;
static volatile int second = 0;
Terminal<uart_stream_device, terminal_hooks::commands> terminal;

void systick_isr(void) {
  systick_count--;
  if(systick_count == 0) {
    systick_count = 1000;
    second++;
    led_green::toggle();
  }
}

void Kernel::init(void)
{
  /* check unique resources */
  resources::list::check();

  /* set all shared register from list */
  resources::list::configure();

  led_green::init(); led_green::off();
  led_orange::init(); led_orange::off();
  led_red::init(); led_red::off();
  led_blue::init(); led_blue::off();

  systick::Init();
  systick::EnableInterrupt();
}

void Kernel::run(void)
{
  terminal.open();
  terminal.tx_stream << "\r\n\r\nWelcome to CppCore terminal console!\r\n# " << flush;

  while(1)
  {
    // poll terminal
    terminal.process_input();
#if 0
    Core::nop(10000000);
    led_orange::toggle();
#endif
  }
}
