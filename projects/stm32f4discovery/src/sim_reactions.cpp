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

#ifdef CORE_SIMULATION

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

namespace reg {
  void RegisterReaction::react() {
    switch(addr) {
    case RCC::CR::addr:
      if(bits_set<RCC::CR::HSEON>()) {
        RCC::CR::HSERDY::set();
      }
      if(bits_set<RCC::CR::PLLON>()) {
        RCC::CR::PLLRDY::set();
      }
      break;

    case RCC::CFGR::addr:
      if(bits_set<RCC::CFGR::SW::PLL>()) {
        RCC::CFGR::SWS::PLL::set();
      }
      break;

    case SCB::STCSR::addr:
      if(bits_set<SCB::STCSR::TICKINT>()) {
        std::thread(systick_thread).detach();
      }
    };
  }
} // namespace reg

#endif // CORE_SIMULATION
