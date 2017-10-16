/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright (C) 2012-2017 Axel Burri <axel@tty0.ch>
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

template<
  typename usart_type,
  typename _fifo_type, // = ring_buffer<char, 256>,
  bool     _crlf      = true,
  bool     debug_irqs = false
  >
class usart_irq_stream
{
public:
  static constexpr bool crlf = _crlf;
  using fifo_type = _fifo_type;
  using char_type = char;

  static fifo_type rx_fifo;
  static fifo_type tx_fifo;

  static volatile unsigned int irq_count;
  static volatile unsigned int irq_errors;

private:

  using SR = typename usart_type::USARTx::SR;

  static void isr(void) {
    auto flags = SR::load();

    if(debug_irqs) {
      irq_count++;
      if(flags & (SR::ORE::value | SR::FE::value | SR::NE::value | SR::PE::value))
        irq_errors++;
    }

    if(flags & SR::RXNE::value) {
      uint32_t data = usart_type::receive(); /* implicitely clears RXNE flag */
      rx_fifo.push(data);
    }
    if(flags & SR::TXE::value) {
      char c;
      if(tx_fifo.pop(c)) {
        usart_type::send(c); /* implicitely clears TXE flag */
      }
      else {
        usart_type::disable_tx_interrupt();
      }
    }
  }

public:

  using irq_resources = typelist<
    irq_handler< typename usart_type::irq, isr >
    >;

  using resources = typelist<
    typename usart_type::resources,
    irq_resources
    >;

  static void flush() {
    usart_type::enable_tx_interrupt();
  }

  /** 
   * open the stream
   *
   *   - enable USARTx
   *   - enable usart irq channel
   *   - enable RXNE and PE interrupts
   *
   * NOTE: Make sure the device is correctly setup before calling this
   *       function. e.g. by calling usart_device.configure()
   */
  static void open(void) {
    tx_fifo.reset();
    rx_fifo.reset();

    usart_type::enable();
    usart_type::irq::enable();
    usart_type::enable_interrupt(true, false, true, false, false);
  }

  /**
   * close the stream
   *
   *   - disable interrupts enabled by open()
   *   - disable the usartX irq
   *   - disable USARTx
   */
  static void close(void) {
    usart_type::disable_interrupt(true, false, true, false, false);
    usart_type::irq::disable();
    usart_type::disable();
  }
};

template<typename usart_type, typename fifo_type, bool crlf, bool debug_irqs>
fifo_type usart_irq_stream<usart_type, fifo_type, crlf, debug_irqs>::rx_fifo;

template<typename usart_type, typename fifo_type, bool crlf, bool debug_irqs>
fifo_type usart_irq_stream<usart_type, fifo_type, crlf, debug_irqs>::tx_fifo;

template<typename usart_type, typename fifo_type, bool crlf, bool debug_irqs>
volatile unsigned int usart_irq_stream<usart_type, fifo_type, crlf, debug_irqs>::irq_count;

template<typename usart_type, typename fifo_type, bool crlf, bool debug_irqs>
volatile unsigned int usart_irq_stream<usart_type, fifo_type, crlf, debug_irqs>::irq_errors;


} // namespace mptl

#endif // ARM_CORTEX_STM32_COMMON_USART_STREAM_HPP_INCLUDED
