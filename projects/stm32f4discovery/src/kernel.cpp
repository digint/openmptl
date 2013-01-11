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

#include <kernel.hpp>
#include <arch/core.hpp>

#include "resources.hpp"

using namespace resources;

// Hint: template debugging:
//template<typename T> struct incomplete;
//incomplete<Core::SPI<1>::CR1::SPE::type> debug;


void Kernel::init(void)
{
//  resources::list::assert();  // TODO: this fails because of multiple SpiMaster (lcd+nrf) use same GPIO's
  resources::list::configure();  // TODO: this should be called init(), since configure is special (e.g. spi)

  led::init(); led::off();
}


void Kernel::run(void)
{
  while(1)
  {
    led::toggle();
    for (int i = 0; i < 10000000; i++)    __asm__("nop");
  }
}
