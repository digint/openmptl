/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright 2013 Axel Burri <axel@tty0.ch>
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

#ifndef ARM_CORTEX_STM32_COMMON_UART_TRANSPORT_HPP_INCLUDED
#define ARM_CORTEX_STM32_COMMON_UART_TRANSPORT_HPP_INCLUDED

#include <arch/usart.hpp>
#include <fifo.hpp>

template<typename usart>
class UartIrqTransport
{
  using SR = typename usart::USARTx::SR;

  const typename SR::value_type flags;

public:

  UartIrqTransport(void) : flags(SR::load()) { };

  void process_io(Fifo<char> &rx_fifo, Fifo<char> &tx_fifo) {
    if(flags & SR::RXNE::value) {
      // usart::ClearFlag(UsartFlags::RXNE);
      uint32_t data = usart::receive();
      rx_fifo.push(data);
    }
    if(flags & SR::TXE::value) {
      char c;
      if(tx_fifo.pop(c)) {
        usart::send(c); /* implicitely clears TXE flag */
      }
      else {
        usart::disable_tx_interrupt();
      }
    }
  }

  bool has_errors(void) {
    if(flags & (SR::ORE::value | SR::FE::value | SR::NE::value | SR::PE::value))
      return true;
    return false;
  }

  bool overrun_error(void) { return (flags & SR::ORE::value) ? true : false; }
  bool framing_error(void) { return (flags & SR::FE::value)  ? true : false; }
  bool noise_error(void)   { return (flags & SR::NE::value)  ? true : false; }
  bool parity_error(void)  { return (flags & SR::PE::value)  ? true : false; }
};


template<typename usart,
         typename _fifo_type = RingBuffer<char, 256>,
         bool     _crlf      = true,
         bool     debug_irqs = false>
class UartIrqStream
{
  using char_type = char;

  static void isr(void) {
    UartIrqTransport<usart> transport;

    if(debug_irqs) {
      irq_count++;
      if(transport.has_errors()) {
        irq_errors++;
      }
    }
    transport.process_io(rx_fifo, tx_fifo);
  }

public:
  static constexpr bool crlf = _crlf;

  using fifo_type = _fifo_type;

  static fifo_type rx_fifo;
  static fifo_type tx_fifo;

  static volatile unsigned int irq_count;
  static volatile unsigned int irq_errors;

  using resources = ResourceList<
    typename usart::resources,
    IrqResource< typename usart::Irq, isr >
    >;

  static void flush() {
    usart::enable_tx_interrupt();
  }

  /** NOTE: Make sure the device is setup (by calling
   *        UsartDevice.configure()) before calling this function
   */
  static void open(void) {
    usart::enable();
    usart::Irq::enable();
    usart::enable_interrupt(true, false, true, false, false);
  }
};

template<typename usart, typename fifo_type, bool crlf, bool debug_irqs>
volatile unsigned int UartIrqStream<usart,  fifo_type, crlf, debug_irqs>::irq_count;
template<typename usart, typename fifo_type, bool crlf, bool debug_irqs>
volatile unsigned int UartIrqStream<usart,  fifo_type, crlf, debug_irqs>::irq_errors;

template<typename usart, typename fifo_type, bool crlf, bool debug_irqs>
fifo_type UartIrqStream<usart, fifo_type, crlf, debug_irqs>::rx_fifo;

template<typename usart, typename fifo_type, bool crlf, bool debug_irqs>
fifo_type UartIrqStream<usart, fifo_type, crlf, debug_irqs>::tx_fifo;



#endif // ARM_CORTEX_STM32_COMMON_UART_TRANSPORT_HPP_INCLUDED

