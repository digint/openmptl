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


#include <core_startup.hpp>
#include <arch/nvic.hpp>
#include <kernel.hpp>


#ifndef CORE_SIMULATION

/* Reset core exception: triggered on system startup (system entry point). */
void CoreExceptionReset::Handler(void) {
  CoreStartupIrqWrap wrap;

  Kernel::init();
  Kernel::run();
}

#else // CORE_SIMULATION

#include <iostream>


int main(int argc, char *argv[])
{
  std::cout << "cppcore simulation" << std::endl
            << "------------------" << std::endl
            << std::endl;

  Core::InitHW();

  Kernel::init();
  Kernel::run();
}

#endif // CORE_SIMULATION
