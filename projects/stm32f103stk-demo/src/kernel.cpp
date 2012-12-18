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
#include "resources.hpp"

#include "events.hpp"
#include "fsmlist.hpp"

#include "screen.hpp"
#include "terminal.hpp"
#include "time.hpp"

#include "printf.h"
#include <core_resource.hpp>
#include <arch/uart_transport.hpp>

#include <atomic>

using namespace resources;

// Hint: template debugging:
//template<typename T> struct incomplete;
//incomplete<Core::SPI<1>::CR1::SPE::type> debug;

#if 0
// If you don't believe in resources::list magic: compare them by hand
static_assert(resources::list::value<SharedAPB1ENR<0> >() == 0x18020000, "apb1enr");
static_assert(resources::list::value<SharedAPB2ENR<0> >() == 0x0000121c, "apb2enr");

static_assert(resources::list::value<SharedCRL<'C', 0> >() == 0x34000000, "crl");
static_assert(resources::list::value<SharedCRH<'C', 0> >() == 0x00030383, "crh");
#endif

//static std::atomic<unsigned int> seconds;
static volatile unsigned int seconds;
static volatile unsigned int usart_irq_count;
static volatile unsigned int usart_irq_errors;

template<>
void rtc::GlobalIrq::Handler(void) {
  rtc::StaticIrqShell shell;  // clears second flag in constructor

  //  seconds.store(seconds.load(std::memory_order_relaxed) + 1, std::memory_order_relaxed);
  seconds++;
  led::toggle();
}

typedef RingBuffer<char, 512> ringbuffer_t;

ringbuffer_t  usart_rx_fifo;
ringbuffer_t  usart_tx_fifo;

template<>
void usart::GlobalIrq::Handler(void) {
  UartIrqTransport<usart> transport;

  usart_irq_count++;

  if(transport.HasErrors()) {
    usart_irq_errors++;
  }

  transport.ProcessIO(usart_rx_fifo, usart_tx_fifo);
}


void Kernel::init(void)
{
//  resources::list::assert();  // TODO: this fails because of multiple SpiMaster (lcd+nrf) use same GPIO's
  resources::list::configure();  // TODO: this should be called init(), since configure is special (e.g. spi)
  
#ifdef CORE_SIMULATION
  // set TXE and RXNE bits, SPI::sendByte loops on it
  Core::SPI<1>::SR::set(0x0002 | 0x0001);

  
  Core::RCC::BDCR::set(0x00000002);
  Core::RTC::CRL::set(0x0008);
#endif

  systick::Init();
  led::init(); led::off();

  lcd_n3310::init();
  lcd_n3310::setContrast(0x45);

  nrf::init();

  joy::init();
  rtc::Init();

  systick::EnableInterrupt();

  // TODO: play around with prescaler (measurements!)
  rtc::SetPrescaler(0x7FFF); // 1sec
  rtc::EnableSecondInterrupt();
  rtc::GlobalIrq::Enable();

  usart::init();
  usart::Enable();
  usart::GlobalIrq::Enable();
  usart::EnableInterrupt<true, false, true, false, false>();

}

template<typename T, unsigned wait_time = 10>
class Debouncer
{
  T value;
  T current;
  unsigned set_time = 0;

public:

  Debouncer(T _value) : value(_value), current(_value) { };

  void set(T new_value) {
    if(current != new_value) {
      current = new_value;
      set_time = time::get_systick();
    }
  }

  bool get(T & _value) {
    bool changed = false;
    if((value != current) && (time::get_systick() + wait_time > set_time)) {
      value = current;
      changed = true;
    }

    _value = value;
    return changed;
  }
};


void Kernel::run(void)
{
  FifoStream<ringbuffer_t, UsartStreamDevice<usart> > usart_tx_fifo_stream(usart_tx_fifo);
  Terminal terminal(usart_rx_fifo, usart_tx_fifo_stream);

  char joytext_buf[16] = "              ";
  const char * joypos_text = "center";
  Joystick::Position joypos = Joystick::Position::center;
  Debouncer<Joystick::Position, 10> debouncer(joypos);
  
  ItemList ilist;

  TextRow    title0    (ilist, " CppCore demo ");
  TextRow    title1    (ilist, "--------------");
  TextRow    joytext   (ilist, joytext_buf);
  DataRow    rtc_sec   (ilist, "rtc");
  DataRowHex tick      (ilist, "tick");
  DataRowHex irq_count (ilist, "#irq");
  DataRowHex eirq      (ilist, "eirq");
 
  Screen::assign(&ilist);

  terminal << "\r\n\r\n\r\nWelcome to CppCore-demo terminal console\r\n" << flush;
  fsm_list::start();

  fsm_list::dispatch(JoystickUp());
  while(1)
  {
    // poll joystick
    debouncer.set(joy::getPosition());

    //    Stick().dispatch();

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
    sprintf(joytext_buf, " joy: %s %s", (joy::buttonPressed() ? "x" : "o"), joypos_text);

    // update screen rows
    rtc_sec = seconds;
    //    rtc_sec = seconds.load(std::memory_order_relaxed);
    tick = time::get_systick();
    irq_count = usart_irq_count;
    eirq = usart_irq_errors;


    // poll terminal
    terminal.process_input();

    // update screen
    lcd_n3310::configure();
    Screen::update();
  }
}
