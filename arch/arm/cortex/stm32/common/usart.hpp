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

#ifndef STM32_COMMON_USART_HPP_INCLUDED
#define STM32_COMMON_USART_HPP_INCLUDED

#include <arch/core.hpp>
#include <arch/gpio.hpp>
#include <arch/nvic.hpp>
#include <arch/rcc.hpp>
#include <arch/reg/usart.hpp>
#include <type_traits>


enum class UsartStopBits : uint32_t {
  stop_bits_1   = 0,
  stop_bits_0_5 = 1,
  stop_bits_2   = 2,
  stop_bits_1_5 = 3
};

enum class UsartParity {
  disabled,
  even,
  odd
};

enum class UsartFlowControl : uint32_t {
  disabled = 0,
  rts      = 1,
  cts      = 2,
  rts_cts  = 3
};

enum class UsartClockPolarity {
  low,
  high
};

enum class UsartClockPhase {
  first,   /* The first clock transition is the first data capture edge.  */
  second   /* The second clock transition is the first data capture edge. */
};


template< typename           rcc,
          unsigned           _usart_no,
          unsigned           baud_rate    = 9600,
          unsigned           word_length  = 8,   /* supported: 8 and 9 bits */
          UsartParity        parity       = UsartParity::disabled,
          UsartStopBits      stop_bits    = UsartStopBits::stop_bits_1,
          UsartFlowControl   flow_control = UsartFlowControl::disabled,
          bool               enable_rx    = true,
          bool               enable_tx    = true,
          bool               clock_enable = false,
          UsartClockPolarity cpol         = UsartClockPolarity::low,
          UsartClockPhase    cpha         = UsartClockPhase::first,
          bool               lbcl         = false
          >
class Usart
{
  static_assert((_usart_no >= 1) && (_usart_no <= 3), "Invalid USART number");
  static_assert((word_length == 8) || (word_length == 9), "Invalid word length");

  static_assert(_usart_no != 1, "usart 1 is not yet supported, sorry...");

  using USARTx = reg::USART<_usart_no>;

public:
  static constexpr unsigned usart_no = _usart_no;
  using resources = Rcc_usart_clock_resources<usart_no>;
  using Irq = irq::USART<usart_no>;

  static void send(typename USARTx::DR::value_type data) {
    // USARTx::DR::store(data & (uint32_t)0x01ff);
    USARTx::DR::store(data);
  }
  static typename USARTx::DR::value_type receive(void) {
    /* This also clears the RXNE bit in the SR register.             */
    /* When receiving the parity enabled, the value read in the MSB  */
    /* bit is the received parity bit.                               */
    return USARTx::DR::load();
  }

  static void enable(void) {
    USARTx::CR1::UE::set();
  }
  static void disable(void) {
    USARTx::CR1::UE::clear();
  }

  static void enable_interrupt(bool rxne,           /**< read data register not empty interrupt    */
                               bool txe  = false,   /**< transmitter data register empty interrupt */
                               bool pe   = false,   /**< parity error interrupt                    */
                               bool tc   = false,   /**< transmission complete interrupt           */
                               bool idle = false)   /**< idle interrupt                            */
  {
    auto cr1 = USARTx::CR1::load();
    if(rxne) cr1 |= USARTx::CR1::RXNEIE::value;
    if(txe)  cr1 |= USARTx::CR1::TXEIE::value;
    if(pe)   cr1 |= USARTx::CR1::PEIE::value;
    if(tc)   cr1 |= USARTx::CR1::TCIE::value;
    if(idle) cr1 |= USARTx::CR1::IDLEIE::value;
    USARTx::CR1::store(cr1);
  }

  static void disable_interrupt(bool rxne,          /**< read data register not empty interrupt    */
                                bool txe  = false,  /**< transmitter data register empty interrupt */
                                bool pe   = false,  /**< parity error interrupt                    */
                                bool tc   = false,  /**< transmission complete interrupt           */
                                bool idle = false)  /**< idle interrupt                            */
  {
    auto cr1 = USARTx::CR1::load();
    if(rxne) cr1 &= ~USARTx::CR1::RXNEIE::value;
    if(txe)  cr1 &= ~USARTx::CR1::TXEIE::value;
    if(pe)   cr1 &= ~USARTx::CR1::PEIE::value;
    if(tc)   cr1 &= ~USARTx::CR1::TCIE::value;
    if(idle) cr1 &= ~USARTx::CR1::IDLEIE::value;
    USARTx::CR1::store(cr1);
  }

  static void enable_tx_interrupt(void)  { USARTx::CR1::TXEIE::set(); }
  static void disable_tx_interrupt(void) { USARTx::CR1::TXEIE::clear(); }


  static void configure(void) {
    /* USARTx CR1 config */
    auto cr1 = USARTx::CR1::load();
    cr1 &= ~(USARTx::CR1::M::value | USARTx::CR1::PCE::value | USARTx::CR1::PS::value | USARTx::CR1::TE::value | USARTx::CR1::RE::value);
    if(word_length == 9)
      cr1 |= USARTx::CR1::M::value;
    if(parity == UsartParity::even)
      cr1 |= USARTx::CR1::PCE::value;
    if(parity == UsartParity::odd)
      cr1 |= USARTx::CR1::PCE::value | USARTx::CR1::PS::value;
    if(enable_tx)
      cr1 |= USARTx::CR1::TE::value;
    if(enable_rx)
      cr1 |= USARTx::CR1::RE::value;
    USARTx::CR1::store(cr1);

    /* USARTx CR2 config */
    auto cr2 = USARTx::CR2::load();
    cr2 &= ~(USARTx::CR2::STOP::value | USARTx::CR2::CLKEN::value | USARTx::CR2::CPOL::value | USARTx::CR2::CPHA::value | USARTx::CR2::LBCL::value);
    cr2 |= (uint32_t)stop_bits << 12;
    if(clock_enable)
      cr2 |= USARTx::CR2::CLKEN::value;
    if(cpol == UsartClockPolarity::high)
      cr2 |= USARTx::CR2::CPOL::value;
    if(cpha == UsartClockPhase::second)
      cr2 |= USARTx::CR2::CPHA::value;
    if(lbcl)
      cr2 |= USARTx::CR2::LBCL::value;
    USARTx::CR2::store(cr2);

    /* USARTx CR3 config */
    USARTx::CR3::template set<typename USARTx::CR3::CTSE,
                              typename USARTx::CR3::RTSE>
      ((uint32_t)flow_control << 8);

    /* calculate values for baud rate register */
    constexpr unsigned pclk = (usart_no == 1 ? rcc::pclk2_freq : rcc::pclk1_freq );
    constexpr unsigned div  = (pclk * 25) / (4 * baud_rate);
    constexpr unsigned mant = div / 100;
    constexpr unsigned fraq = ((div - (mant * 100)) * 16 + 50) / 100;

    USARTx::BRR::store((mant << 4) | (fraq & 0x0f));
  }
};

#endif // STM32_COMMON_USART_HPP_INCLUDED

