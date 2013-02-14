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

#include "uart_terminal.hpp"
#include "resources.hpp"

template<typename uart>
void UartTerminal<uart>::irq_handler(void) {
  UartIrqTransport<uart> transport;

#if 0
  usart_irq_count++;

  if(transport.HasErrors()) {
    usart_irq_errors++;
  }
#endif

  transport.ProcessIO(usart_rx_fifo, usart_tx_fifo);
}

template<typename uart>
typename UartTerminal<uart>::fifo_type UartTerminal<uart>::usart_rx_fifo;
template<typename uart>
typename UartTerminal<uart>::fifo_type UartTerminal<uart>::usart_tx_fifo;



typedef UartTerminal<resources::usart> uart_terminal;

template<>
void uart_terminal::Irq::Handler(void) {
  UartIrqTransport<resources::usart> transport;

#if 0
  usart_irq_count++;

  if(transport.HasErrors()) {
    usart_irq_errors++;
  }
#endif

  transport.ProcessIO(UartTerminal<resources::usart>::usart_rx_fifo, UartTerminal<resources::usart>::usart_tx_fifo);
}
