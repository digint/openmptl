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

#include <arch/core.hpp>
#include "kernel.hpp"

/* Reset exception: triggered on system startup (system entry point). */
void Kernel::reset_isr(void) {
  Core::startup<rcc, flash>();

  Kernel::init();
  Kernel::run();
}

#ifndef CORE_SIMULATION

#include <arch/vector_table.hpp>

extern const uint32_t _stack_top;  /* provided by linker script */

/* Build the vector table:
 * - use irq handler from IsrResource<> in Kernel::resources
 * - use Kernel::error_isr as default isr
 */
VectorTable<&_stack_top, Kernel::resources, Kernel::error_isr> vector_table;


#else // CORE_SIMULATION


#include <iostream>

int main(int argc, char *argv[])
{
  std::cout << "cppcore simulation" << std::endl
            << "------------------" << std::endl
            << std::endl;

  Kernel::reset_isr();
}

#endif // CORE_SIMULATION
