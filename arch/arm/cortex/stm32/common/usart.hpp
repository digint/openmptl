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


template<unsigned baud_rate>
struct UsartBaudRate {
  template<typename usart>
  struct brr_value
  {
    static constexpr unsigned pclk = (usart::usart_no == 1 ? usart::rcc::pclk2_freq : usart::rcc::pclk1_freq );
    static constexpr unsigned div  = (pclk * 25) / (4 * baud_rate);
    static constexpr unsigned mant = div / 100;
    static constexpr unsigned fraq = ((div - (mant * 100)) * 16 + 50) / 100;

    static constexpr typename usart::USARTx::BRR::value_type value = (mant << 4) | (fraq & 0x0f);
  };
};

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


namespace mpl {
  template<typename usart, unsigned baud_rate>
  struct baud_to_brr
  {
    static constexpr unsigned pclk = (usart::usart_no == 1 ? usart::rcc::pclk2_freq : usart::rcc::pclk1_freq );
    static constexpr unsigned div  = (pclk * 25) / (4 * baud_rate);
    static constexpr unsigned mant = div / 100;
    static constexpr unsigned fraq = ((div - (mant * 100)) * 16 + 50) / 100;

    static constexpr typename usart::USARTx::BRR::value_type value = (mant << 4) | (fraq & 0x0f);
  };
}

template< unsigned _usart_no, typename _rcc >
class Usart
{
  static_assert((_usart_no >= 1) && (_usart_no <= 3), "invalid USART number");
  static_assert(_usart_no != 1, "usart 1 is not yet supported, sorry...");

public:

  using rcc = _rcc;

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
};



template< typename _usart >
class UartDevice
{
  using usart = _usart; // TODO: rename usart_type

  using USARTx = typename usart::USARTx;

  const typename USARTx::CR1::value_type cr1_value;
  const typename USARTx::CR2::value_type cr2_value;
  const typename USARTx::CR3::value_type cr3_value;
  const typename USARTx::BRR::value_type brr_value;

  const typename usart::USARTx::BRR::value_type baud_to_brr(unsigned baud_rate) {
    unsigned pclk = (usart::usart_no == 1 ? usart::rcc::pclk2_freq : usart::rcc::pclk1_freq );
    unsigned div  = (pclk * 25) / (4 * baud_rate);
    unsigned mant = div / 100;
    unsigned fraq = ((div - (mant * 100)) * 16 + 50) / 100;

    return (mant << 4) | (fraq & 0x0f);
  };

public:

  UartDevice( unsigned           baud_rate,
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
              )
  : cr1_value((word_length == 9            ? USARTx::CR1::M::value : 0) |
              (parity == UsartParity::even ? USARTx::CR1::PCE::value : 0) |
              (parity == UsartParity::odd  ? (USARTx::CR1::PCE::value | USARTx::CR1::PS::value) : 0) |
              (enable_tx                   ? USARTx::CR1::TE::value : 0) |
              (enable_rx                   ? USARTx::CR1::RE::value : 0) ),
    cr2_value(USARTx::CR2::STOP::shifted_value((uint32_t)stop_bits) |
              (clock_enable ? USARTx::CR2::CLKEN::value : 0) |
              (cpol == UsartClockPolarity::high ? USARTx::CR2::CPOL::value : 0) |
              (cpha == UsartClockPhase::second ? USARTx::CR2::CPHA::value : 0) |
              (lbcl ? USARTx::CR2::LBCL::value : 0) ),
#if 0
    cr3_value(((uint32_t)flow_control << 8)),
#else
    cr3_value(((flow_control == UsartFlowControl::rts || flow_control == UsartFlowControl::rts_cts) ? USARTx::CR3::RTSE::value : 0 ) |
              ((flow_control == UsartFlowControl::cts || flow_control == UsartFlowControl::rts_cts) ? USARTx::CR3::CTSE::value : 0 )),
#endif
    brr_value(baud_to_brr(baud_rate))
  { }

  /* NOTE: this implicitely clears all other bits, including "usart enable" (CR1::UE) ! */
  void configure(void) const
  {
    /* USARTx CR1 config */
    USARTx::CR1::store(cr1_value);
    USARTx::CR2::store(cr2_value);
    USARTx::CR3::store(cr3_value);
    USARTx::BRR::store(brr_value);
  }
};


template< typename           _usart,
          unsigned           baud_rate,
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
struct UartStaticDevice
{
  static_assert((word_length == 8) || (word_length == 9), "invalid word length");

  using usart = _usart; // TODO: rename usart_type

#if 1
  static void configure()
  {
    UartDevice<usart> dev(baud_rate, word_length, parity, stop_bits, flow_control, enable_rx, enable_tx, clock_enable, cpol, cpha, lbcl);
    dev.configure();
  }
#else

    /* calculate values for baud rate register */
  static constexpr unsigned pclk = (usart::usart_no == 1 ? usart::rcc::pclk2_freq : usart::rcc::pclk1_freq );
  static constexpr unsigned div  = (pclk * 25) / (4 * baud_rate);
  static constexpr unsigned mant = div / 100;
  static constexpr unsigned fraq = ((div - (mant * 100)) * 16 + 50) / 100;

  static void configure()
  {
    using USARTx = typename usart::USARTx;

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

    USARTx::BRR::store((mant << 4) | (fraq & 0x0f));
  }
#endif
};

#endif // STM32_COMMON_USART_HPP_INCLUDED

