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

template<typename uart>
class UartTerminal : public Terminal
{
  typedef RingBuffer<char, 512> fifo_type;

public:

  static fifo_type usart_rx_fifo;
  static fifo_type usart_tx_fifo;

  FifoStream<fifo_type, UsartStreamDevice<uart> > usart_tx_fifo_stream;


  typedef typename uart::resources resources;

  typedef typename uart::GlobalIrq Irq;

  static void irq_handler(void);

  UartTerminal(void) : Terminal(usart_rx_fifo, usart_tx_fifo_stream),
                       usart_tx_fifo_stream(usart_tx_fifo)
  {
    uart::init();
    uart::Enable();
    uart::GlobalIrq::Enable();
    uart::template EnableInterrupt<true, false, true, false, false>();

  }
};
