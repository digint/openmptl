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

#ifndef ARM_CORTEX_STM32_COMMON_USART_STREAM_HPP_INCLUDED
#define ARM_CORTEX_STM32_COMMON_USART_STREAM_HPP_INCLUDED

#include <arch/usart.hpp>
#include <fifo.hpp>

namespace mptl {

template<typename usart>
class usart_irq_transport
{
  using SR = typename usart::USARTx::SR;

  const typename SR::value_type flags;

public:

  usart_irq_transport(void) : flags(SR::load()) { };

  void process_io(fifo<char> &rx_fifo, fifo<char> &tx_fifo) {
    if(flags & SR::RXNE::value) {
      uint32_t data = usart::receive(); /* implicitely clears RXNE flag */
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
         typename _fifo_type = ring_buffer<char, 256>,
         bool     _crlf      = true,
         bool     debug_irqs = false>
class usart_irq_stream : public usart
{
  using char_type = char;

  static void isr(void) {
    usart_irq_transport<usart> transport;

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

  using irq_resources = typelist<
    irq_handler< typename usart::irq, isr >
    >;

  using resources = typelist<
    typename usart::resources,
    irq_resources
    >;

  static void flush() {
    usart::enable_tx_interrupt();
  }

  /** 
   * open the stream
   *
   * - enable USARTx
   * - enable usart irq channel
   * - enable RXNE and PE interrupts
   *
   * NOTE: Make sure the device is correctly setup before calling this
   *       function. e.g. by calling usart_device.configure()
   */
  static void open(void) {
    usart::enable();
    usart::irq::enable();
    usart::enable_interrupt(true, false, true, false, false);
  }

  /**
   * close the stream
   *
   * - disable interrupts enabled by open()
   * - disable the usartX irq
   * - disable USARTx
   */
  static void close(void) {
    usart::disable_interrupt(true, false, true, false, false);
    usart::irq::disable();
    usart::disable();
  }
};

template<typename usart, typename fifo_type, bool crlf, bool debug_irqs>
volatile unsigned int usart_irq_stream<usart,  fifo_type, crlf, debug_irqs>::irq_count;
template<typename usart, typename fifo_type, bool crlf, bool debug_irqs>
volatile unsigned int usart_irq_stream<usart,  fifo_type, crlf, debug_irqs>::irq_errors;

template<typename usart, typename fifo_type, bool crlf, bool debug_irqs>
fifo_type usart_irq_stream<usart, fifo_type, crlf, debug_irqs>::rx_fifo;

template<typename usart, typename fifo_type, bool crlf, bool debug_irqs>
fifo_type usart_irq_stream<usart, fifo_type, crlf, debug_irqs>::tx_fifo;


} // namespace mptl

#endif // ARM_CORTEX_STM32_COMMON_USART_STREAM_HPP_INCLUDED
