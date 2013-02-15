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
#include "terminal_hooks.hpp"

#if 0

#if 0
template<typename uart, typename cmd_hooks>
typename UartTerminal<uart, cmd_hooks>::fifo_type UartTerminal<uart, cmd_hooks>::usart_rx_fifo;
template<typename uart, typename cmd_hooks>
typename UartTerminal<uart, cmd_hooks>::fifo_type UartTerminal<uart, cmd_hooks>::usart_tx_fifo;
#endif


typedef UartTerminal<resources::usart, terminal_hooks::commands> uart_terminal;

template<>
void uart_terminal::Irq::Handler(void) {
  UartIrqTransport<resources::usart> transport;

#if 0
  usart_irq_count++;

  if(transport.HasErrors()) {
    usart_irq_errors++;
  }
#endif

  transport.ProcessIO(uart_terminal::usart_rx_fifo, uart_terminal::usart_tx_fifo);
}
#endif
