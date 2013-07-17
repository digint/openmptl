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

#ifndef ARM_CORTEX_STM32_COMMON_USART_HPP_INCLUDED
#define ARM_CORTEX_STM32_COMMON_USART_HPP_INCLUDED

#include <arch/nvic.hpp>
#include <arch/rcc.hpp>
#include <arch/reg/usart.hpp>
#include <periph.hpp>
#include <type_traits>

namespace mptl {

namespace cfg { namespace usart {

template<unsigned value>
struct baud_rate
: public config_base
{
  template<typename usart>
  struct regmask_type_impl {
    static constexpr unsigned pclk = (usart::usart_no == 1 ? usart::rcc::pclk2_freq : usart::rcc::pclk1_freq );
    static constexpr unsigned div  = (pclk * 25) / (4 * value);
    static constexpr unsigned mant = div / 100;
    static constexpr unsigned fraq = ((div - (mant * 100)) * 16 + 50) / 100;

    // using type = regmask< typename usart::USARTx::BRR, (mant << 4) | (fraq & 0x0f), 0xffff >;
    using type = typename usart::USARTx::BRR::template merge<
      regval< typename usart::USARTx::BRR::DIV_Mantissa, mant>,
      regval< typename usart::USARTx::BRR::DIV_Fraction, fraq>
      >::type;
  };

  template<typename usart>
  using regmask_type = typename regmask_type_impl<usart>::type;
};


struct enable_rx
: public config_base
{
  template<typename usart>
  using regmask_type = regval< typename usart::USARTx::CR1::RE, 1 >;
};


struct enable_tx
: public config_base
{
  template<typename usart>
  using regmask_type = regval< typename usart::USARTx::CR1::TE, 1 >;
};


template<unsigned value>
struct word_length
: public config_base
{
  static_assert((value == 8) || (value == 9), "illegal word_length (supported values: 8, 9)");
  template<typename usart>
  using regmask_type = regval< typename usart::USARTx::CR1::M, (value == 9) ? 1 : 0 >;
};


namespace parity
{
  struct even
  : public config_base
  {
    template<typename usart>
    using regmask_type = typename usart::USARTx::CR1::template combined_mask<
      regval< typename usart::USARTx::CR1::PCE, 1 >,
      regval< typename usart::USARTx::CR1::PS,  0 >
      >::type;
  };

  struct odd
  : public config_base
  {
    template<typename usart>
    using regmask_type = typename usart::USARTx::CR1::template combined_mask<
      regval< typename usart::USARTx::CR1::PCE, 1 >,
      regval< typename usart::USARTx::CR1::PS,  1 >
      >::type;
  };

  struct disabled
  : public config_base
  {
    template<typename usart>
    using regmask_type = typename usart::USARTx::CR1::template combined_mask<
      regval< typename usart::USARTx::CR1::PCE, 0 >,
      regval< typename usart::USARTx::CR1::PS,  0 >
      >::type;
  };
} // namespace parity


template<unsigned a, unsigned b = 0>
struct stop_bits
: public config_base
{
  static_assert(((a == 1) && (b == 0)) ||
                ((a == 0) && (b == 5)) ||
                ((a == 2) && (b == 0)) ||
                ((a == 1) && (b == 5)),
                "illegal stop_bits (supported values: 1, 0.5, 2, 1.5)");

  template<typename usart>
  using regmask_type = regval< typename usart::USARTx::CR2::STOP,
    ((a == 1) && (b == 0)) ? 0 :
    ((a == 0) && (b == 5)) ? 1 :
    ((a == 2) && (b == 0)) ? 2 :
    ((a == 1) && (b == 5)) ? 3 : 0xff
    >;
};


struct clock_enable
: public config_base
{
  template<typename usart>
  using regmask_type = regval< typename usart::USARTx::CR2::CLKEN, 1 >;
};


namespace clock_polarity
{
  struct high
  : public config_base
  {
    template<typename usart>
    using regmask_type = regval< typename usart::USARTx::CR2::CPOL, 1 >;
  };

  struct low
  : public config_base
  {
    template<typename usart>
    using regmask_type = regval< typename usart::USARTx::CR2::CPOL, 0 >;
  };
} // namespace clock_polarity


namespace clock_phase
{
  /** The first clock transition is the first data capture edge.  */
  struct first_edge
  : public config_base
  {
    template<typename usart>
    using regmask_type = regval< typename usart::USARTx::CR2::CPHA, 0 >;
  };

  /** The second clock transition is the first data capture edge. */
  struct second_edge
  : public config_base
  {
    template<typename usart>
    using regmask_type = regval< typename usart::USARTx::CR2::CPHA, 1 >;
  };
} // namespace clock_phase


struct last_bit_clock_pulse
: public config_base
{
  template<typename usart>
  using regmask_type = regval< typename usart::USARTx::CR2::LBCL, 1 >;
};


namespace flow_control
{
  struct rts
  : public config_base
  {
    template<typename usart>
    using regmask_type = regval< typename usart::USARTx::CR3::RTSE, 1 >;
  };

  struct cts
  : public config_base
  {
    template<typename usart>
    using regmask_type = regval< typename usart::USARTx::CR3::CTSE, 1 >;
  };
} // namespace flow_control

} } // namespace cfg::usart


////////////////////  usart  ////////////////////


template< unsigned _usart_no, typename _rcc, typename... CFG >
class usart
: public periph<
  usart<_usart_no, _rcc>,
  typelist<                  /* list of regdef<> register types to reset in configure() function */
      typename reg::USART<_usart_no>::CR1,
      typename reg::USART<_usart_no>::CR2,
      typename reg::USART<_usart_no>::CR3,
      typename reg::USART<_usart_no>::BRR
    >,
  CFG...
  >
{
  static_assert((_usart_no >= 1) && (_usart_no <= 3), "invalid USART number");
  static_assert(_usart_no != 1, "usart 1 is not yet supported, sorry...");

  using type = usart<_usart_no, _rcc>;

public:
  static constexpr unsigned usart_no = _usart_no;

  using USARTx = reg::USART<usart_no>;
  using rcc = _rcc;

  using irq = irq::usart<usart_no>;
  using periph_cfg_type = periph_cfg< type, CFG... >;

  using resources = typelist<
    rcc_usart_clock_resources<usart_no>,
    typename periph_cfg_type::resources
    >;

  static void send(typename USARTx::DR::value_type data) {
    /* Implicitely clears the TXE bit in the SR register.  */
    // assert((data & 0x01ff) == data);
    USARTx::DR::store(data);
  }
  static typename USARTx::DR::value_type receive(void) {
    /* Implicitely clears the RXNE bit in the SR register.           */
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

} // namespace mptl

#endif // ARM_CORTEX_STM32_COMMON_USART_HPP_INCLUDED
