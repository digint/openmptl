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

#include "kernel.hpp"

#include "events.hpp"
#include "fsmlist.hpp"
#include "terminal_hooks.hpp"
#include "screen.hpp"
#include "time.hpp"

#include "printf.h"
#include <terminal.hpp>
#include <resource.hpp>
#include <debouncer.hpp>
#include <arch/uart_transport.hpp>
#include <arch/dwt.hpp>  // CycleCounter
#include <atomic>


#ifdef DEBUG_ASSERT_REGISTER_AGAINST_FIXED_VALUES
/* Check the shared registers against fixed values.
 * You can always explicitely set the registers using this notation.
 * NOTE: combined_type<SharedRegisterGroup<X>> is of type SharedRegister<...>
 */
#include <arch/reg/rcc.hpp>
#include <arch/reg/gpio.hpp>
static_assert(Kernel::resources::combined_type<SharedRegisterGroup<reg::RCC::APB1ENR> >::set_mask == 0x18020000, "apb1enr");
static_assert(Kernel::resources::combined_type<SharedRegisterGroup<reg::RCC::APB2ENR> >::set_mask == 0x0000121c, "apb2enr");

static_assert(Kernel::resources::combined_type<SharedRegisterGroup<reg::GPIO<'C'>::CRL> >::set_mask == 0x34000000, "crl");
static_assert(Kernel::resources::combined_type<SharedRegisterGroup<reg::GPIO<'C'>::CRH> >::set_mask == 0x00030383, "crh");
#endif // DEBUG_ASSERT_REGISTER_AGAINST_FIXED_VALUES


void Kernel::init(void)
{
  resources::check();      /* check unique resources */
  resources::configure();  /* configure resources (set all shared register) */

#ifdef CORE_SIMULATION
  // hack: set TXE and RXNE bits, SPI::sendByte loops on it
  Reg::SPI<1>::SR::set(0x0002 | 0x0001);
  Reg::RCC::BDCR::set(0x00000002);
  Reg::RTC::CRL::set(0x0008);
#endif

  led::init(); led::off();

  time::init();
  time::enable();

  lcd::init();
  lcd::set_contrast(0x45);

  nrf::init();

  joy::init();

  usart::init();
  usart::enable();
  usart::GlobalIrq::enable();
  usart::enable_interrupt<true, false, true, false, false>();
}


void Kernel::run(void)
{
  CycleCounter cycle_counter;
  Terminal<uart_stream_device, terminal_hooks::commands> terminal;
  char joytext_buf[16] = "              ";
  const char * joypos_text = "center";
  Joystick::Position joypos = Joystick::Position::center;
  Debouncer<Joystick::Position, time, 10> debouncer(joypos);

  /* define the screem item list */
  ItemList item_list;
  TextRow    title0    (item_list, " CppCore demo ");
  TextRow    title1    (item_list, "--------------");
  TextRow    joytext   (item_list, joytext_buf);
  DataRow    rtc_sec   (item_list, "rtc");
  DataRowHex tick      (item_list, "tick");
  DataRow    cycle     (item_list, "cyc");
  DataRowHex irq_count (item_list, "#irq");
  DataRowHex eirq      (item_list, "eirq");
  Screen::assign(&item_list);

  /* open terminal and print welcome message */
  terminal.open();
  terminal.tx_stream << "\r\n\r\nWelcome to CppCore terminal console!\r\n# " << flush;

  /* start kernel loop */
  fsm_list::start();
  while(1)
  {
    cycle_counter.start();
    /* poll joystick */
    debouncer.set(joy::get_position());
    if(debouncer.get(joypos)) {
      switch(joypos) {
      case Joystick::Position::up:
        send_event(JoystickUp());
        joypos_text = "up";
        break;
      case Joystick::Position::down:
        send_event(JoystickDown());
        joypos_text = "down";
        break;
      case Joystick::Position::left:
        send_event(JoystickLeft());
        joypos_text = "left";
        break;
      case Joystick::Position::right:
        send_event(JoystickRight());
        joypos_text = "right";
        break;
      case Joystick::Position::center:
        send_event(JoystickCenter());
        joypos_text = "center";
        break;
      }
    }
    sprintf(joytext_buf, " joy: %s %s", (joy::button_pressed() ? "x" : "o"), joypos_text);

    /* poll terminal */
    terminal.process_input();

    /* update screen rows */
    rtc_sec   = time::get_rtc_seconds();
    tick      = time::get_systick();
    irq_count = uart_stream_device::irq_count;
    eirq      = uart_stream_device::irq_errors;
    cycle_counter.stop();
    cycle     = cycle_counter.get();

    /* update screen */
    lcd::configure();
    Screen::update();
  }
}
