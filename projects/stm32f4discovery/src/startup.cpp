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


#include <crt.hpp>
#include <arch/nvic.hpp>
#include <kernel.hpp>

#ifndef CORE_SIMULATION

#include "resources.hpp"
#include <arch/uart_transport.hpp>

extern void systick_isr();
extern const uint32_t _stack_top;  // provided by linker script

void error_handler(void) {
  while(1) {
    resources::led_red::on();
  }
}

void null_handler(void) { }

void warn_handler(void) {
  resources::led_orange::on();
}


/* Default isr for all core exceptions */
template<int irqn>
isr_t CoreExceptionImpl<irqn>::isr = error_handler;

template<> isr_t CoreException::NMI::isr = null_handler;
template<> isr_t CoreException::SVCall::isr = null_handler;
template<> isr_t CoreException::DebugMonitor::isr = null_handler;
template<> isr_t CoreException::PendSV::isr = null_handler;
//template<> isr_t CoreException::SysTick::isr = null_handler;


/* Default isr for all irq channels */
template<unsigned irqn>
struct IrqChannel<irqn>::Handler {
  static constexpr isr_t isr = warn_handler;
};

/* Set our systick isr */
template<> isr_t resources::systick::Irq::isr = systick_isr;

/* Build the vector table (use irq handler from all IsrResource<> in resources::list) */
VectorTable<&_stack_top, resources::list> vector_table;



#else // CORE_SIMULATION


#include <iostream>

int main(int argc, char *argv[])
{
  std::cout << "cppcore simulation" << std::endl
            << "------------------" << std::endl
            << std::endl;

  Core::Init();

  Kernel::init();
  Kernel::run();
}

#endif // CORE_SIMULATION
