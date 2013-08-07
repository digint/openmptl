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

#include "kernel.hpp"

#include "events.hpp"
#include "fsmlist.hpp"
#include "terminal_hooks.hpp"
#include "screen.hpp"
#include "time.hpp"

#include "printf.h"
#include <terminal.hpp>
#include <debouncer.hpp>
#include <arch/dwt.hpp>  // cycle_counter


//#define DEBUG_ASSERT_REGISTER_AGAINST_FIXED_VALUES
#ifdef DEBUG_ASSERT_REGISTER_AGAINST_FIXED_VALUES
/* Check the shared registers against fixed values.
 * You can always explicitely set the registers using this notation.
 * NOTE: combined_type<reg_shared_group<X>> is of type reg_shared<...>
 */
#include <arch/reg/rcc.hpp>
#include <arch/reg/gpio.hpp>

static_assert(Kernel::resources::combined_type< mptl::resource::reg_shared_group< mptl::reg::RCC::APB1ENR::reg_type   > >::mask_type::set_mask == 0x18020000, "apb1enr");
static_assert(Kernel::resources::combined_type< mptl::resource::reg_shared_group< mptl::reg::RCC::APB2ENR::reg_type   > >::mask_type::set_mask == 0x0000121c, "apb2enr");
static_assert(Kernel::resources::combined_type< mptl::resource::reg_shared_group< mptl::reg::GPIO<'C'>::CRL::reg_type > >::mask_type::set_mask == 0x34000000, "crl");
static_assert(Kernel::resources::combined_type< mptl::resource::reg_shared_group< mptl::reg::GPIO<'C'>::CRH::reg_type > >::mask_type::set_mask == 0x00020383, "crh");
#endif // DEBUG_ASSERT_REGISTER_AGAINST_FIXED_VALUES

Kernel::terminal_type Kernel::terminal;

void Kernel::init(void)
{
  /* check unique resources */
  // resources::check(); // TODO: do we still need this?

  /* set all register from Kernel::resources<> */
  mptl::core::configure< resources >();

  led::off();

  time::init();
  time::enable();

  lcd::init();
  nrf::init();

  joy::init();

  // lcd::set_contrast(0x45);
}

void Kernel::run(void)
{
  char joytext_buf[16] = { 0 };
  const char * joypos_text = "center";
  mptl::cycle_counter cycle_counter;
  debouncer< joy::position,
             joy::get_position,
             time::get_systick,
             time::systick::freq,
             10 > joypos(joy::position::center);

  /* define the screen item list */
#ifdef OPENMPTL_USE_BOOST
  // also works with std::list<>, but slower, bigger
  ScreenItemList item_list;
  TextRow    title0    (item_list, ">> OpenMPTL <<");
  TextRow    title1    (item_list, "--------------");
  TextRow    joytext   (item_list, joytext_buf);
  DataRow    rtc_sec   (item_list, "rtc");
  DataRowHex tick      (item_list, "tick");
  DataRow    cycle     (item_list, "cyc");
  DataRowHex irq_count (item_list, "#irq");
  DataRowHex eirq      (item_list, "eirq");
#else
  // this is much faster, and reduces code size by 100 (list) to 200 (vector) bytes!
  TextRow    title0    (">> OpenMPTL <<");
  TextRow    title1    ("--------------");
  TextRow    joytext   (joytext_buf);
  DataRow    rtc_sec   ("rtc");
  DataRowHex tick      ("tick");
  DataRow    cycle     ("cyc");
  DataRowHex irq_count ("#irq");
  DataRowHex eirq      ("eirq");
  ScreenItemList item_list{&title0, &title1, &joytext, &rtc_sec, &tick, &cycle, &irq_count, &eirq};
#endif

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
        joypos_text = "up";
        break;
      case joy::position::down:
        send_event(EvJoystickDown());
        joypos_text = "down";
        break;
      case joy::position::left:
        send_event(EvJoystickLeft());
        joypos_text = "left";
        break;
      case joy::position::right:
        send_event(EvJoystickRight());
        joypos_text = "right";
        break;
      case joy::position::center:
        send_event(EvJoystickCenter());
        joypos_text = "center";
        break;
      }
    }
    sprintf(joytext_buf, " joy: %s %s", (joy::button_pressed() ? "x" : "o"), joypos_text);

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
  }
}
