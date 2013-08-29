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
#include <type_traits>

namespace mptl {

template< unsigned _usart_no, typename system_clock_type >
class usart_stm32_common
{
  static_assert((_usart_no >= 1) && (_usart_no <= 3), "invalid USART number");
  static_assert(_usart_no != 1, "usart 1 is not yet supported, sorry...");

  using type = usart_stm32_common<_usart_no, system_clock_type>;

public:
  static constexpr unsigned usart_no = _usart_no;

  using USARTx = USART<usart_no>;

  using irq = irq::usart<usart_no>;

  using resources = rcc_usart_clock_resources<usart_no>;

private:

  template<unsigned value>
  struct baud_rate_impl {
    static constexpr unsigned pclk = (usart_no == 1 ? system_clock_type::pclk2_freq : system_clock_type::pclk1_freq );
    static constexpr unsigned div  = (pclk * 25) / (4 * value);
    static constexpr unsigned mant = div / 100;
    static constexpr unsigned fraq = ((div - (mant * 100)) * 16 + 50) / 100;

    using type = typename USARTx::BRR::template merge<
        regval< typename USARTx::BRR::DIV_Mantissa, mant>,
        regval< typename USARTx::BRR::DIV_Fraction, fraq>
        >::type;
  };

  template<unsigned value>
  struct word_length_impl {
    static_assert((value == 8) || (value == 9), "illegal word_length (supported values: 8, 9)");
    using type = regval< typename USARTx::CR1::M, (value == 9) ? 1 : 0 >;
  };

  template<unsigned a, unsigned b = 0>
  struct stop_bits_impl {
    static_assert(((a == 1) && (b == 0)) ||
                  ((a == 0) && (b == 5)) ||
                  ((a == 2) && (b == 0)) ||
                  ((a == 1) && (b == 5)),
                  "illegal stop_bits (supported values: 1, 0.5, 2, 1.5)");

    template<typename usart>
    using type = regval< typename USARTx::CR2::STOP,
      ((a == 1) && (b == 0)) ? 0 :
      ((a == 0) && (b == 5)) ? 1 :
      ((a == 2) && (b == 0)) ? 2 :
      ((a == 1) && (b == 5)) ? 3 : 0xff
      >;
  };

public:  /* ------ configuration traits ------ */

  using enable_rx = regval< typename USARTx::CR1::RE, 1 >;
  using enable_tx = regval< typename USARTx::CR1::TE, 1 >;

  template<unsigned value>
  using baud_rate = typename baud_rate_impl<value>::type;

  template<unsigned value>
  using word_length = typename word_length_impl<value>::type;

  struct parity
  {
    using even = reglist<
      regval< typename USARTx::CR1::PCE, 1 >,
      regval< typename USARTx::CR1::PS,  0 >
      >;
    using odd = reglist<
      regval< typename USARTx::CR1::PCE, 1 >,
      regval< typename USARTx::CR1::PS,  1 >
      >;
    using disabled = reglist<
      regval< typename USARTx::CR1::PCE, 0 >,
      regval< typename USARTx::CR1::PS,  0 >
      >;
  };

  template<unsigned a, unsigned b = 0>
  using stop_bits = typename stop_bits_impl<a,b>::type;

  using clock_enable = regval< typename USARTx::CR2::CLKEN, 1 >;

  struct clock_polarity {
    using high = regval< typename USARTx::CR2::CPOL, 1 >;
    using low  = regval< typename USARTx::CR2::CPOL, 0 >;
  };

  struct clock_phase {
    /** The first clock transition is the first data capture edge.  */
    using first_edge  = regval< typename USARTx::CR2::CPHA, 0 >;

    /** The second clock transition is the first data capture edge. */
    using second_edge = regval< typename USARTx::CR2::CPHA, 1 >;
  };

  using last_bit_clock_pulse = regval< typename USARTx::CR2::LBCL, 1 >;

  struct flow_control {
    using rts = regval< typename USARTx::CR3::RTSE, 1 >;
    using cts = regval< typename USARTx::CR3::CTSE, 1 >;
  };


  static typename USARTx::BRR::value_type baud_to_brr(unsigned baud_rate) {
    unsigned pclk = (_usart_no == 1 ? system_clock_type::pclk2_freq : system_clock_type::pclk1_freq );
    unsigned div  = (pclk * 25) / (4 * baud_rate);
    unsigned mant = div / 100;
    unsigned fraq = ((div - (mant * 100)) * 16 + 50) / 100;

    return (mant << 4) | (fraq & 0x0f);
  };


public:  /* ------ static member functions ------ */

  /**
   * Configure USART register using Tp type traits.
   *
   * NOTE: make sure no communication is ongoing when calling this function.
   */
  template< typename... Tp >
  static void configure(void) {
    reglist< Tp... >::template strict_reset_to<
      typename USARTx::CR1,
      typename USARTx::CR2,
      typename USARTx::CR3,
      typename USARTx::BRR
      >();
  }

  /**
   * Set the BRR register to the value corresponding to the baud_rate
   * provided.
   *
   * NOTE: make sure no communication is ongoing when calling this function.
   */
  static void set_baudrate(unsigned value) {
    USARTx::BRR::store(baud_to_brr(value));
  }

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
