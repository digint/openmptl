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

#ifndef UART_TRANSPORT_HPP_INCLUDED
#define UART_TRANSPORT_HPP_INCLUDED

#include <arch/usart.hpp>
#include <fifo.hpp>

template<typename usart>
class UartIrqTransport
{
  using SR = typename usart::USARTx::SR;

  typename SR::value_type flags;

public:

  UartIrqTransport(void) : flags(SR::load()) { };

  void ProcessIO(Fifo<char> &rx_fifo, Fifo<char> &tx_fifo) {
    if(flags & UsartFlags::RXNE) {
      //    usart::ClearFlag(UsartFlags::RXNE);
      uint32_t data = usart::Receive();
      rx_fifo.push(data);
      //      usart_rx++;
    }
    if(flags & UsartFlags::TXE) {
      char c;
      if(tx_fifo.pop(c)) {
        usart::Send(c); /* implicitely clears TXE flag */
        //        usart_tx++;
      }
      else {
        usart::DisableTxInterrupt();
      }
    }
  }

  bool HasErrors(void) {
    if(flags & (SR::ORE::value | SR::FE::value | SR::NE::value | SR::PE::value))
      return true;
    return false;
  }

  bool OverrunError(void) { return (flags & SR::ORE::value) ? true : false; }
  bool FramingError(void) { return (flags & SR::FE::value)  ? true : false; }
  bool NoiseError(void)   { return (flags & SR::NE::value)  ? true : false; }
  bool ParityError(void)  { return (flags & SR::PE::value)  ? true : false; }
};


template<typename usart>
struct UsartStreamDevice
{
  static void flush() {
    usart::EnableTxInterrupt();
  }
};

#endif // UART_TRANSPORT_HPP_INCLUDED

