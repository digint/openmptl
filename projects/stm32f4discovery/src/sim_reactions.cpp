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
#include <atomic>
#include <chrono>
#include <ratio>
#include <poll.h>
#include "kernel.hpp"

// non-static atomics (bug in clang-3.3 ?)
std::atomic<bool> systick_thread_terminate;
std::atomic<bool> terminal_rx_thread_terminate;
std::atomic<bool> terminal_tx_thread_terminate;


/** run Kernel::systick_isr() in intervals defined by Kernel::systick::freq */
static void systick_thread() {
  using systick_interval = std::chrono::duration< int, std::ratio<1, Kernel::systick::freq> >;
  while(!systick_thread_terminate)
  {
    Kernel::systick_isr();
    std::this_thread::sleep_for( systick_interval(1) );
  }
}


/**
 * Poll stdin, and feed result into stream_device_type::rx_fifo.
 *
 * NOTE: we use std::cin, which is not thread safe.
 */
static void terminal_rx_thread() {
  char c;
  pollfd cinfd[1];
  cinfd[0].fd = fileno(stdin);
  cinfd[0].events = POLLIN;

  //  std::cout << "*** terminal_rx_thread() running" << std::endl;
  while(!terminal_rx_thread_terminate)
  {
    if(poll(cinfd, 1, 0))
    {
      c = std::cin.get();
      // std::cout << '<' << +c << '>' <<  std::endl;
      if(c == 10) c = 13; // convert LF into CR (hacky...)

      /* feed rx_fifo, will pe polled in terminal.process_input() */
      Kernel::terminal_type::stream_device_type::rx_fifo.push(c);
    }
    SIM_RELAX; // sleep a bit (don't eat up all cpu power)
  }
  //  std::cout << "*** terminal_rx_thread() terminated" << std::endl;
}


/**
 * Hook into stream_device_type::tx_fifo, and print output to stdout.
 *
 * NOTE: we use std::cout, which is not thread safe.
 */
static void terminal_tx_thread() {
  //  std::cout << "*** terminal_tx_thread() running" << std::endl;
  while(!terminal_tx_thread_terminate)
  {
    char c;
    while(Kernel::terminal_type::stream_device_type::tx_fifo.pop(c)) {
      std::cout << c;
    }
    std::cout << std::flush;
    SIM_RELAX; // sleep a bit (don't eat up all cpu power)
  }
  //  std::cout << "*** terminal_tx_thread() terminated" << std::endl;
}



namespace mptl {

thread_local int reg_reaction::refcount;

void reg_reaction::react()
{
  switch(addr) {
  case reg::RCC::CR::addr:
    if(bits_set< reg::RCC::CR::HSEON >()) {
      reg::RCC::CR::HSERDY::set();
    }
    if(bits_set< reg::RCC::CR::PLLON >()) {
      reg::RCC::CR::PLLRDY::set();
    }
    break;

  case reg::RCC::CFGR::addr:
    if(bits_set< reg::RCC::CFGR::SW::PLL >()) {
      reg::RCC::CFGR::SWS::PLL::set();
    }
    break;

  case reg::SCB::STCSR::addr:
    if(bits_set< reg::SCB::STCSR::TICKINT >()) {
      systick_thread_terminate = false;
      std::thread(systick_thread).detach();
    }
    else if(bits_cleared< reg::SCB::STCSR::TICKINT >()) {
      systick_thread_terminate = true;
    }
    break;

  case Kernel::usart::USARTx::CR1::addr:
    /* start/stop terminal rx thread on RXNEIE */
    if(bits_set< Kernel::usart::USARTx::CR1::RXNEIE >()) {
      terminal_rx_thread_terminate = false;
      std::thread(terminal_rx_thread).detach();
    }
    else if(bits_cleared< Kernel::usart::USARTx::CR1::RXNEIE >()) {
      terminal_rx_thread_terminate = true;
    }

    /* start/stop terminal tx thread on TXEIE */
    if(bits_set< Kernel::usart::USARTx::CR1::TXEIE >()) {
      terminal_tx_thread_terminate = false;
      std::thread(terminal_tx_thread).detach();
    }
    else if(bits_cleared< Kernel::usart::USARTx::CR1::TXEIE >()) {
      terminal_tx_thread_terminate = true;
    }
    break;
  };
}

} // namespace mptl

#endif // OPENMPTL_SIMULATION
