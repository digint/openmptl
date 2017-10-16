/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright (C) 2012-2017 Axel Burri <axel@tty0.ch>
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

#include "kernel.hpp"

#include "events.hpp"
#include "fsmlist.hpp"
#include "terminal_hooks.hpp"
#include "screen.hpp"
#include "time.hpp"

#include <terminal.hpp>
#include <debouncer.hpp>
#include <arch/dwt.hpp>  // cycle_counter
#include <simulation.hpp>


//#define DEBUG_ASSERT_REGISTER_AGAINST_FIXED_VALUES
#ifdef DEBUG_ASSERT_REGISTER_AGAINST_FIXED_VALUES
/* Check the shared registers against fixed values.
 * You can always explicitely set the registers using this notation.
 * NOTE: combined_type<reg_shared_group<X>> is of type reg_shared<...>
 */
#include <arch/reg/rcc.hpp>
#include <arch/reg/gpio.hpp>

static_assert(mptl::make_reglist< Kernel::resources >::merged_regmask< mptl::RCC::APB1ENR   >::set_mask == 0x18020000, "apb1enr");
static_assert(mptl::make_reglist< Kernel::resources >::merged_regmask< mptl::RCC::APB2ENR   >::set_mask == 0x0000121c, "apb2enr");
static_assert(mptl::make_reglist< Kernel::resources >::merged_regmask< mptl::GPIO<'C'>::CRL >::set_mask == 0x34000000, "crl");
static_assert(mptl::make_reglist< Kernel::resources >::merged_regmask< mptl::GPIO<'C'>::CRH >::set_mask == 0x00020383, "crh");

#endif // DEBUG_ASSERT_REGISTER_AGAINST_FIXED_VALUES

Kernel::terminal_type Kernel::terminal;

void Kernel::init(void)
{
  /* set all register from Kernel::resources<> */
  mptl::make_reglist< resources >::reset_to();

  led::off();

  time::init();
  time::enable();

  lcd::init();
  nrf::init();

  joy::enable();
}

void Kernel::run(void)
{
  char joytext_buf[16];
  char * const joypos_text = &joytext_buf[8];
  std::strcpy(joytext_buf, "joy : o center");

  mptl::cycle_counter cycle_counter;
  debouncer< joy::position,
             joy::get_position_blocking,
             time::get_systick,
             time::systick::freq,
             10 > joypos(joy::position::center);

  /* define the screen item list */
  ScreenItemList item_list;
  TextRow    title0    (item_list, ">> OpenMPTL <<");
  TextRow    title1    (item_list, "--------------");
  TextRow    joytext   (item_list, joytext_buf);
  DataRow    rtc_sec   (item_list, "rtc");
  DataRow    tick      (item_list, "tick", 0, NumberBase::hex);
  DataRow    cycle     (item_list, "cyc");
  DataRow    irq_count (item_list, "#irq", 0, NumberBase::hex);
  DataRow    eirq      (item_list, "eirq", 0, NumberBase::hex);

  // Note: it is also possible to define the ScreenItems without the
  // item_list, and use an initializer list. Initializer
  // lists are faster, and reduce code size by 100 (list) to 200
  // (vector) bytes! We don't care here in favor of code-beauty.
  //
  // TextRow title0(">> OpenMPTL <<");
  //  <...>
  //  ScreenItemList item_list{ &title0, <...> };

  Screen::set_item_list(item_list);

  /* open terminal and print welcome message */
  terminal.open();
  terminal.tx_stream << "\r\n\r\nWelcome to OpenMPTL terminal console!\r\n# " << poorman::flush;

  /* start kernel loop */
  fsm_list::start();
  while(1)
  {
    cycle_counter.start();

    /* poll joystick */
    if(joypos.poll()) {
      switch(joypos) {
      case joy::position::up:
        send_event(EvJoystickUp());
        std::strcpy(joypos_text, "up");
        break;
      case joy::position::down:
        send_event(EvJoystickDown());
        std::strcpy(joypos_text, "down");
        break;
      case joy::position::left:
        send_event(EvJoystickLeft());
        std::strcpy(joypos_text, "left");
        break;
      case joy::position::right:
        send_event(EvJoystickRight());
        std::strcpy(joypos_text, "right");
        break;
      case joy::position::center:
        send_event(EvJoystickCenter());
        std::strcpy(joypos_text, "center");
        break;
      }
    }
    joytext_buf[6] = joy::button_pressed() ? 'x' : 'o';

    /* poll terminal */
    terminal.process_input< terminal_hooks::commands >();

    /* update screen rows */
    rtc_sec   = time::get_rtc_seconds();
    tick      = time::get_systick();
    irq_count = usart_stream_device::irq_count;
    eirq      = usart_stream_device::irq_errors;

    cycle_counter.stop();
    cycle     = cycle_counter.get();

    /* update screen */
    lcd::enable();
    Screen::update();

    SIM_RELAX; // sleep a bit (don't eat up all cpu power)
  }
}
