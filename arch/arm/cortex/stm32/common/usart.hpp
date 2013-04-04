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


#if 0
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
#endif


struct UartDeviceDefaultConfig
{
  static constexpr unsigned           baud_rate    = 9600;
  static constexpr unsigned           word_length  = 8;  /* supported: 8 and 9 bits */
  static constexpr UsartParity        parity       = UsartParity::disabled;
  static constexpr UsartStopBits      stop_bits    = UsartStopBits::stop_bits_1;
  static constexpr UsartFlowControl   flow_control = UsartFlowControl::disabled;
  static constexpr bool               enable_rx    = true;
  static constexpr bool               enable_tx    = true;
  static constexpr bool               clock_enable = false;
  static constexpr UsartClockPolarity cpol         = UsartClockPolarity::low;
  static constexpr UsartClockPhase    cpha         = UsartClockPhase::first;
  static constexpr bool               lbcl         = false;
};



template< typename usart_type, typename cfg >
class UartDevice
{
  //  static_assert((cfg::word_length == 8) || (cfg::word_length == 9), "invalid word length");

public:
  using usart = usart_type; // TODO: rename usart_type

  using USARTx = typename usart::USARTx;

  const typename USARTx::CR1::value_type cr1_value;
  const typename USARTx::CR2::value_type cr2_value;
  const typename USARTx::CR3::value_type cr3_value;
  const typename USARTx::BRR::value_type brr_value;

  typename usart::USARTx::BRR::value_type baud_to_brr(unsigned baud_rate) const {
    unsigned pclk = (usart::usart_no == 1 ? usart::rcc::pclk2_freq : usart::rcc::pclk1_freq );
    unsigned div  = (pclk * 25) / (4 * baud_rate);
    unsigned mant = div / 100;
    unsigned fraq = ((div - (mant * 100)) * 16 + 50) / 100;

    return (mant << 4) | (fraq & 0x0f);
  };

public:

  UartDevice()
    : cr1_value((cfg::word_length == 9            ? USARTx::CR1::M::value : 0) |
                (cfg::parity == UsartParity::even ? USARTx::CR1::PCE::value : 0) |
                (cfg::parity == UsartParity::odd  ? (USARTx::CR1::PCE::value | USARTx::CR1::PS::value) : 0) |
                (cfg::enable_tx                   ? USARTx::CR1::TE::value : 0) |
                (cfg::enable_rx                   ? USARTx::CR1::RE::value : 0) ),
      cr2_value(USARTx::CR2::STOP::shifted_value((uint32_t)cfg::stop_bits) |
                (cfg::clock_enable ? USARTx::CR2::CLKEN::value : 0) |
                (cfg::cpol == UsartClockPolarity::high ? USARTx::CR2::CPOL::value : 0) |
                (cfg::cpha == UsartClockPhase::second ? USARTx::CR2::CPHA::value : 0) |
                (cfg::lbcl ? USARTx::CR2::LBCL::value : 0) ),
#if 0
    cr3_value(((uint32_t)cfg::flow_control << 8)),
#else
    cr3_value(((cfg::flow_control == UsartFlowControl::rts || cfg::flow_control == UsartFlowControl::rts_cts) ? USARTx::CR3::RTSE::value : 0 ) |
              ((cfg::flow_control == UsartFlowControl::cts || cfg::flow_control == UsartFlowControl::rts_cts) ? USARTx::CR3::CTSE::value : 0 )),
#endif
    brr_value(baud_to_brr(cfg::baud_rate))
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


#endif // STM32_COMMON_USART_HPP_INCLUDED

