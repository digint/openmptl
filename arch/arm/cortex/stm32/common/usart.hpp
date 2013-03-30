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


template< typename           _rcc,
          unsigned           _baud_rate    = 9600,
          unsigned           _word_length  = 8,   /* supported: 8 and 9 bits */
          UsartParity        _parity       = UsartParity::disabled,
          UsartStopBits      _stop_bits    = UsartStopBits::stop_bits_1,
          UsartFlowControl   _flow_control = UsartFlowControl::disabled,
          bool               _enable_rx    = true,
          bool               _enable_tx    = true,
          bool               _clock_enable = false,
          UsartClockPolarity _cpol         = UsartClockPolarity::low,
          UsartClockPhase    _cpha         = UsartClockPhase::first,
          bool               _lbcl         = false
          >
struct UsartConfig
{
  static_assert((_word_length == 8) || (_word_length == 9), "invalid word length");

  using rcc = _rcc;

  static constexpr unsigned           baud_rate    = _baud_rate;
  static constexpr unsigned           word_length  = _word_length;
  static constexpr UsartParity        parity       = _parity;
  static constexpr UsartStopBits      stop_bits    = _stop_bits;
  static constexpr UsartFlowControl   flow_control = _flow_control;
  static constexpr bool               enable_rx    = _enable_rx;
  static constexpr bool               enable_tx    = _enable_tx;
  static constexpr bool               clock_enable = _clock_enable;
  static constexpr UsartClockPolarity cpol         = _cpol;
  static constexpr UsartClockPhase    cpha         = _cpha;
  static constexpr bool               lbcl         = _lbcl;
};


template< unsigned _usart_no >
class Usart
{
  static_assert((_usart_no >= 1) && (_usart_no <= 3), "invalid USART number");
  static_assert(_usart_no != 1, "usart 1 is not yet supported, sorry...");

public:

  using USARTx = reg::USART<_usart_no>;

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

  template<typename T>
  static void configure(T const & cfg)
  {
    /* USARTx CR1 config */
    auto cr1 = USARTx::CR1::load();
    cr1 &= ~(USARTx::CR1::M::value | USARTx::CR1::PCE::value | USARTx::CR1::PS::value | USARTx::CR1::TE::value | USARTx::CR1::RE::value);
    if(cfg.word_length == 9)
      cr1 |= USARTx::CR1::M::value;
    if(cfg.parity == UsartParity::even)
      cr1 |= USARTx::CR1::PCE::value;
    if(cfg.parity == UsartParity::odd)
      cr1 |= USARTx::CR1::PCE::value | USARTx::CR1::PS::value;
    if(cfg.enable_tx)
      cr1 |= USARTx::CR1::TE::value;
    if(cfg.enable_rx)
      cr1 |= USARTx::CR1::RE::value;
    USARTx::CR1::store(cr1);

    /* USARTx CR2 config */
    auto cr2 = USARTx::CR2::load();
    cr2 &= ~(USARTx::CR2::STOP::value | USARTx::CR2::CLKEN::value | USARTx::CR2::CPOL::value | USARTx::CR2::CPHA::value | USARTx::CR2::LBCL::value);
    cr2 |= (uint32_t)cfg.stop_bits << 12;
    if(cfg.clock_enable)
      cr2 |= USARTx::CR2::CLKEN::value;
    if(cfg.cpol == UsartClockPolarity::high)
      cr2 |= USARTx::CR2::CPOL::value;
    if(cfg.cpha == UsartClockPhase::second)
      cr2 |= USARTx::CR2::CPHA::value;
    if(cfg.lbcl)
      cr2 |= USARTx::CR2::LBCL::value;
    USARTx::CR2::store(cr2);

    /* USARTx CR3 config */
    USARTx::CR3::template set<typename USARTx::CR3::CTSE,
                              typename USARTx::CR3::RTSE>
      ((uint32_t)cfg.flow_control << 8);

    /* calculate values for baud rate register */
    constexpr unsigned pclk = (usart_no == 1 ? T::rcc::pclk2_freq : T::rcc::pclk1_freq );
    constexpr unsigned div  = (pclk * 25) / (4 * cfg.baud_rate);
    constexpr unsigned mant = div / 100;
    constexpr unsigned fraq = ((div - (mant * 100)) * 16 + 50) / 100;

    USARTx::BRR::store((mant << 4) | (fraq & 0x0f));
  }

};

#endif // STM32_COMMON_USART_HPP_INCLUDED

