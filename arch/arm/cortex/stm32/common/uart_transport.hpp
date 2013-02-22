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

#ifndef STM32_COMMON_UART_TRANSPORT_HPP_INCLUDED
#define STM32_COMMON_UART_TRANSPORT_HPP_INCLUDED

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
    if(flags & SR::RXNE::value) {
      // usart::ClearFlag(UsartFlags::RXNE);
      uint32_t data = usart::Receive();
      rx_fifo.push(data);
      // usart_rx++;
    }
    if(flags & SR::TXE::value) {
      char c;
      if(tx_fifo.pop(c)) {
        usart::Send(c); /* implicitely clears TXE flag */
        // usart_tx++;
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
struct UartStreamDevice
{  // TODO: class
  typedef char char_type;

  typedef RingBuffer<char_type, 512> fifo_type;

  static constexpr bool crlf = true;

  static fifo_type rx_fifo;
  static fifo_type tx_fifo;

  static void flush() {
    usart::EnableTxInterrupt();
  }

  static void open(void) {
    usart::init();
    usart::Enable();
    usart::GlobalIrq::Enable();
    usart::template EnableInterrupt<true, false, true, false, false>();
  }

  struct Irq : public usart::GlobalIrq {
    struct Handler {
      static void isr(void) {
        UartIrqTransport<usart> transport;
#if 0
        irq_count++;
        if(transport.HasErrors()) {
          irq_errors++;
        }
#endif
        transport.ProcessIO(rx_fifo, tx_fifo);
      }
    };
  };

  typedef ResourceList< typename usart::resources,
                        IrqResource< Irq >
                        > resources;
};

template<typename usart>
RingBuffer<char, 512> UartStreamDevice<usart>::rx_fifo;

template<typename usart>
RingBuffer<char, 512> UartStreamDevice<usart>::tx_fifo;



#endif // STM32_COMMON_UART_TRANSPORT_HPP_INCLUDED

