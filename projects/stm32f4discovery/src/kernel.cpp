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

#include <arch/core.hpp>
#include <terminal.hpp>
#include <simulation.hpp>
#include "terminal_hooks.hpp"
#include "kernel.hpp"

Kernel::terminal_type Kernel::terminal;

void Kernel::systick_isr() {
  static constexpr int toggle_interval = systick::freq / 2;  /* 0.5 seconds interval */
  static int systick_count = toggle_interval;

  systick_count--;
  if(systick_count == 0) {
    systick_count = toggle_interval;

    /* Demonstrate the impact of the active_state configuration element:
     * faking active_state::low for Kernel::led_blue<> has the effect of
     * green/blue leds toggling alternately.
     */
    led_green::toggle();
    led_blue::toggle();
  }
}

void Kernel::init(void)
{
  /* check unique resources */
  // resources::check(); // TODO: do we still need this?

  /* set all register from Kernel::resources<> */
  mptl::make_reglist< resources >::reset_to();

  /* turn all leds off */
  led_green ::off();
  led_orange::off();
  led_red   ::off();
  led_blue  ::off();

#ifdef DYNAMIC_BAUD_RATE
  /* set the baud rate, since it is not set in usart<> peripheral
   * configuration (and thus was NOT set by
   * "mptl::core::configure<resources>()" above).
   */
  usart::set_baudrate(115200);
#endif

  /* finally start systick */
  systick::enable();
  systick::enable_interrupt();
}

void Kernel::run(void)
{
  /* open terminal and print welcome message */
  terminal.open();
  terminal.tx_stream << "\r\n\r\nWelcome to OpenMPTL terminal console!\r\n# " << poorman::flush;

  while(1)
  {
    /* poll terminal */
    terminal.process_input< terminal_hooks::commands >();

    SIM_RELAX; // sleep a bit (don't eat up all cpu power)
  }
}
