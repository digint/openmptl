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
#include <thread>
#include <chrono>
#include <ratio>
#include "kernel.hpp"

static void systick_thread() {
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
  };
}

} // namespace mptl

#endif // OPENMPTL_SIMULATION
