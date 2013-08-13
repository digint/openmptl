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
#include "kernel.hpp"

void mptl::sim::reg_reaction::react()
{
  /* RCC: simulate the system clock setup */
  if(bits_set< reg::RCC::CR::HSEON >())
    reg::RCC::CR::HSERDY::set();
  if(bits_set< reg::RCC::CR::PLLON >())
    reg::RCC::CR::PLLRDY::set();
  if(bits_set< reg::RCC::CFGR::SW::PLL >())
    reg::RCC::CFGR::SWS::PLL::set();

  /* RTC: simulate synchronisation */
  if(bits_set< reg::RCC::BDCR::RTCEN >()) {
    reg::RCC::BDCR::LSERDY::set();
    reg::RTC::CRL::RSF::set();
  }
  if(bits_cleared< reg::RTC::CRL::RSF >())
    reg::RTC::CRL::RSF::set();


  /* ADC: set "end of conversion" on SWSTART */
  if(bits_set< reg::ADC<1>::CR2::SWSTART >())
    reg::ADC<1>::SR::EOC::set();

  /* SPI: always ready to send / receive */
  if(bits_set< Kernel::spi::SPIx::CR1::SPE >()) {  // spi enable
    Kernel::spi::SPIx::SR::TXE::set();
    Kernel::spi::SPIx::SR::RXNE::set();
  }

  /* provide a terminal on stdin/stdout */
  stdio_terminal< Kernel::terminal_type >(*this).react<
    Kernel::usart::USARTx::CR1::RXNEIE,    /* start/stop terminal rx thread on RXNEIE */
    Kernel::usart::USARTx::CR1::TXEIE      /* start/stop terminal tx thread on TXEIE */
    >();
}

#endif // OPENMPTL_SIMULATION
