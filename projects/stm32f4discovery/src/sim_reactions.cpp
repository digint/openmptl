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

#ifdef OPENMPTL_SIMULATION

#include <register.hpp>
#include <arch/scb.hpp>
#include <arch/rcc.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <ratio>
#include <poll.h>
#include "kernel.hpp"

/* poll stdin, and feed result into stream_device_type::rx_fifo */
static void sim_poll_stdin() {
  char c;
  pollfd cinfd[1];
  cinfd[0].fd = fileno(stdin);
  cinfd[0].events = POLLIN;
  if(poll(cinfd, 1, 0))
  {
    c = std::cin.get();
    if(c == 10) c = 13; // convert LF into CR (hacky...)

    /* feed rx_fifo, will pe polled in terminal.process_input() */
    Kernel::terminal_type::stream_device_type::rx_fifo.push(c);
  }
}

/* hook into stream_device_type::tx_fifo, and print output to screen */
static void sim_poll_terminal_fifo() {
  char c;
  while(Kernel::terminal_type::stream_device_type::tx_fifo.pop(c)) {
    std::cout << c;
    std::cout << std::flush;  // too bad, we need to flush after every character...
  }
}

static void terminal_thread() {
  // std::cout << "*** terminal_thread() running" << std::endl;
  static constexpr int duration = 20; // poll frequency (ms)
  while(true) {
    sim_poll_terminal_fifo();
    std::this_thread::sleep_for(std::chrono::milliseconds( duration ));
    sim_poll_stdin();
    std::this_thread::sleep_for(std::chrono::milliseconds( duration ));
  }
}

static void systick_thread() {
  // std::cout << "*** systick_thread() running" << std::endl;
  std::chrono::duration< int, std::ratio<1, Kernel::systick::freq> > duration(1);
  while(true) {
    Kernel::systick_isr();
    std::this_thread::sleep_for(duration);
  }
}

namespace mptl {

void reg_reaction::react() {
  switch(addr) {
  case reg::RCC::CR::addr:
    if(bits_set<reg::RCC::CR::HSEON>()) {
      reg::RCC::CR::HSERDY::set();
    }
    if(bits_set<reg::RCC::CR::PLLON>()) {
      reg::RCC::CR::PLLRDY::set();
    }
    break;

  case reg::RCC::CFGR::addr:
    if(bits_set<reg::RCC::CFGR::SW::PLL>()) {
      reg::RCC::CFGR::SWS::PLL::set();
    }
    break;

  case reg::SCB::STCSR::addr:
    if(bits_set<reg::SCB::STCSR::TICKINT>()) {
      std::thread(systick_thread).detach();
    }

  case Kernel::usart::USARTx::CR1::addr:
    if(bits_set<Kernel::usart::USARTx::CR1::RXNEIE>()) {
      std::thread(terminal_thread).detach();
    }
  };
}

} // namespace mptl

#endif // OPENMPTL_SIMULATION
