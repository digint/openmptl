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

#include <terminal.hpp>
#include <arch/uart_transport.hpp>

#if 0
template<typename uart, typename cmd_hooks>
class UartTerminal
: public Terminal< UsartStreamDevice<uart>, cmd_hooks >
#if 0
: public Terminal< RingBuffer<char, 512>,
                   FifoStream< RingBuffer<char, 512>, UsartStreamDevice<uart> >,
                   cmd_hooks >
#endif
{
  typedef typename uart::resources resources;

  typedef typename uart::GlobalIrq Irq;

public:
  UartTerminal(void) {
    uart::init();
    uart::Enable();
    uart::GlobalIrq::Enable();
    uart::template EnableInterrupt<true, false, true, false, false>();

  }
};
#endif
