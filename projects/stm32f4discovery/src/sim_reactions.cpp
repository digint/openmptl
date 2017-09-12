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

#include <arch/scb.hpp>
#include <arch/rcc.hpp>
#include <terminal_sim.hpp>
#include <thread>
#include <atomic>
#include <chrono>
#include <ratio>
#include "kernel.hpp"

std::atomic<bool> systick_thread_terminate; // TODO: make static (bug in clang-3.3 ?)

/** run Kernel::systick_isr() in intervals defined by Kernel::systick::freq */
static void systick_thread() {
  using systick_interval = std::chrono::duration< int, std::ratio<1, Kernel::systick::freq> >;
  while(!systick_thread_terminate)
  {
    Kernel::systick_isr();
    std::this_thread::sleep_for( systick_interval(1) );
  }
}

void mptl::sim::reg_reaction::react()
{
  /* simulate the system clock setup */
  if(bits_set< RCC::CR::HSEON >()) {
    info("reacting to RCC::CR::HSEON, by setting RCC::CR::HSERDY");
    RCC::CR::HSERDY::set();
  }
  if(bits_set< RCC::CR::PLLON >()) {
    info("reacting to RCC::CR::PLLON, by setting RCC::CR::PLLRDY");
    RCC::CR::PLLRDY::set();
  }
  if(bits_set< RCC::CFGR::SW::PLL >()) {
    info("reacting to RCC::CFGR::SW::PLL, by setting RCC::CFGR::SWS::PLL");
    RCC::CFGR::SWS::PLL::set();
  }

  /* start/stop systick thread on SCB::STCSR::TICKINT */
  if(bits_set< SCB::STCSR::TICKINT >()) {
    systick_thread_terminate = false;
    std::thread(systick_thread).detach();
  }
  else if(bits_cleared< SCB::STCSR::TICKINT >()) {
    systick_thread_terminate = true;
  }

  /* provide a terminal on stdin/stdout */
  stdio_terminal< Kernel::terminal_type >(*this).react<
    Kernel::usart::USARTx::CR1::RXNEIE,    /* start/stop terminal rx thread on RXNEIE */
    Kernel::usart::USARTx::CR1::TXEIE      /* start/stop terminal tx thread on TXEIE */
    >();
}

#endif // OPENMPTL_SIMULATION
