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
#include <arch/rcc.hpp>
#include <arch/rtc.hpp>
#include <thread>
#include <chrono>
#include <ratio>
#include "kernel.hpp"

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

    case RCC::BDCR::addr:
      if(bits_set<reg::RCC::BDCR::RTCEN>()) {
        reg::RCC::BDCR::LSERDY::set();
        reg::RTC::CRL::RSF::set();
      }
      break;

    case RTC::CRL::addr:
      if(bits_cleared<RTC::CRL::RSF>()) {
        RTC::CRL::RSF::set();
      }
      break;

    case ADC<1>::CR2::addr:
      if(bits_set<ADC<1>::CR2::SWSTART>()) {
        ADC<1>::SR::EOC::set(); // end of conversion
      }
      break;

    case SPI<1>::CR1::addr:
      if(bits_set<SPI<1>::CR1::SPE>()) {  // spi enable
        reg::SPI<1>::SR::TXE::set();
        reg::SPI<1>::SR::RXNE::set();
      }
      break;
    };
  }
} // namespace reg

#endif // OPENMPTL_SIMULATION
