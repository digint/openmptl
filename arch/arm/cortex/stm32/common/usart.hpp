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

#include <arch/gpio.hpp>
#include <arch/nvic.hpp>
#include <arch/reg/usart.hpp>

namespace cUsart
{
  enum class StopBits : uint32_t {
    stop_bits_1   = 0,
    stop_bits_0_5 = 1,
    stop_bits_2   = 2,
    stop_bits_1_5 = 3
  };

  enum class Parity {
    disabled,
    even,
    odd
  };

  enum class FlowControl : uint32_t {
    disabled = 0,
    rts      = 1,
    cts      = 2,
    rts_cts  = 3
  };

  enum class ClockPolarity {
    low,
    high
  };

  enum class ClockPhase {
    first,   /* The first clock transition is the first data capture edge.  */
    second   /* The second clock transition is the first data capture edge. */
  };
}


struct UsartFlags {
  static constexpr uint32_t PE    = 0x0001;   /**< Parity Error                  */
  static constexpr uint32_t FE    = 0x0002;   /**< Framing Error                 */
  static constexpr uint32_t NE    = 0x0004;   /**< Noise Error Flag              */
  static constexpr uint32_t ORE   = 0x0008;   /**< OverRun Error                 */
  static constexpr uint32_t IDLE  = 0x0010;   /**< IDLE line detected            */
  static constexpr uint32_t RXNE  = 0x0020;   /**< Read Data Register Not Empty  */
  static constexpr uint32_t TC    = 0x0040;   /**< Transmission Complete         */
  static constexpr uint32_t TXE   = 0x0080;   /**< Transmit Data Register Empty  */
  static constexpr uint32_t LBD   = 0x0100;   /**< LIN Break Detection Flag      */
  static constexpr uint32_t CTS   = 0x0200;   /**< CTS Flag                      */
};


template< std::size_t           usart_no,
          unsigned              baud_rate    = 9600,
          unsigned              word_length  = 8,   /* supported: 8 and 9 bits */
          cUsart::Parity        parity       = cUsart::Parity::disabled,
          cUsart::StopBits      stop_bits    = cUsart::StopBits::stop_bits_1,
          cUsart::FlowControl   flow_control = cUsart::FlowControl::disabled,
          bool                  enable_rx    = true,
          bool                  enable_tx    = true,
          bool                  clock_enable = false,
          cUsart::ClockPolarity cpol         = cUsart::ClockPolarity::low,
          cUsart::ClockPhase    cpha         = cUsart::ClockPhase::first,
          bool                  lbcl         = false
          >
class Usart
{
  static_assert((usart_no >= 1) && (usart_no <= 3), "Invalid USART number");
  static_assert((word_length == 8) || (word_length == 9), "Invalid word length");

public:  

  using USARTx = reg::USART<usart_no>;

  typedef GpioOutput< 'A', 2,  cGpio::OutputConfig::alt_push_pull > gpio_tx;
  typedef GpioInput < 'A', 3,  cGpio::InputConfig::floating >       gpio_rx;

  typedef Irq::USART2   GlobalIrq; /**< USART2 global Interrupt */


  
  static_assert(usart_no != 1, "usart 1 is not yet supported, sorry...");
  static constexpr uint32_t apb1enr = ( // usart_no == 1 ? Reg::RCC::APB2ENR::USART1EN::value : 
                                       usart_no == 2 ? reg::RCC::APB1ENR::USART2EN::value : 
                                       usart_no == 3 ? reg::RCC::APB1ENR::USART3EN::value : 
                                       0);

  typedef ResourceList< typename gpio_tx::resources,
                        typename gpio_rx::resources,
                        SharedRegister< reg::RCC::APB1ENR, apb1enr >
                        > resources;

  static void Send(typename USARTx::DR::value_type data) {
    //    USARTx::DR::store(data & (uint32_t)0x01ff);
    USARTx::DR::store(data);
  }
  static typename USARTx::DR::value_type Receive(void) {
    /* This also clears the RXNE bit in the SR register. */
    /* When receiving the parity enabled, the value read in the MSB
       bit is the received parity bit. */
    //    return USARTx::DR::DR_::test();
    return USARTx::DR::load();
  }

  static void Enable(void) {
    USARTx::CR1::UE::set();
  }
  static void Disable(void) {
    USARTx::CR1::UE::clear();
  }

  template< bool rxne = true,   /**< read data register not empty interrupt    */
            bool txe  = true,   /**< transmitter data register empty interrupt */
            bool pe   = true,   /**< parity error interrupt                    */
            bool tc   = false,  /**< transmission complete interrupt           */
            bool idle = false   /**< idle interrupt                            */
            >
  static void EnableInterrupt(void) {
    auto cr1 = USARTx::CR1::load();
    if(rxne) cr1 |= USARTx::CR1::RXNEIE::value;
    if(txe)  cr1 |= USARTx::CR1::TXEIE::value;
    if(pe)   cr1 |= USARTx::CR1::PEIE::value;
    if(tc)   cr1 |= USARTx::CR1::TCIE::value;
    if(idle) cr1 |= USARTx::CR1::IDLEIE::value;
    USARTx::CR1::store(cr1);
  }

  template< bool rxne = true,   /**< read data register not empty interrupt    */
            bool txe  = true,   /**< transmitter data register empty interrupt */
            bool pe   = true,   /**< parity error interrupt                    */
            bool tc   = false,  /**< transmission complete interrupt           */
            bool idle = false   /**< idle interrupt                            */
            >
  static void DisableInterrupt(void) {
    auto cr1 = USARTx::CR1::load();
    if(rxne) cr1 &= ~USARTx::CR1::RXNEIE::value;
    if(txe)  cr1 &= ~USARTx::CR1::TXEIE::value;
    if(pe)   cr1 &= ~USARTx::CR1::PEIE::value;
    if(tc)   cr1 &= ~USARTx::CR1::TCIE::value;
    if(idle) cr1 &= ~USARTx::CR1::IDLEIE::value;
    USARTx::CR1::store(cr1);
  }

  static void EnableTxInterrupt(void)  { EnableInterrupt<false, true, false, false, false>(); }
  static void DisableTxInterrupt(void) { DisableInterrupt<false, true, false, false, false>(); }


  static void init(void) {
    /* USARTx CR2 config */
    auto cr2 = USARTx::CR2::load();
    cr2 &= ~(USARTx::CR2::STOP::value | USARTx::CR2::CLKEN::value | USARTx::CR2::CPOL::value | USARTx::CR2::CPHA::value | USARTx::CR2::LBCL::value);
    cr2 |= (uint32_t)stop_bits << 12;
    if(clock_enable)
      cr2 |= USARTx::CR2::CLKEN::value;
    if(cpol == cUsart::ClockPolarity::high)
      cr2 |= USARTx::CR2::CPOL::value;
    if(cpha == cUsart::ClockPhase::second)
      cr2 |= USARTx::CR2::CPHA::value;
    if(lbcl)
      cr2 |= USARTx::CR2::LBCL::value;
    USARTx::CR2::store(cr2);
    
    /* USARTx CR1 config */
    auto cr1 = USARTx::CR1::load();
    cr1 &= ~(USARTx::CR1::M::value | USARTx::CR1::PCE::value | USARTx::CR1::PS::value | USARTx::CR1::TE::value | USARTx::CR1::RE::value);
    if(word_length == 9)
      cr1 |= USARTx::CR1::M::value;
    if(parity == cUsart::Parity::even)
      cr1 |= USARTx::CR1::PCE::value;
    if(parity == cUsart::Parity::odd)
      cr1 |= USARTx::CR1::PCE::value | USARTx::CR1::PS::value;
    if(enable_tx)
      cr1 |= USARTx::CR1::TE::value;
    if(enable_rx)
      cr1 |= USARTx::CR1::RE::value;
    USARTx::CR1::store(cr1);

    /* USARTx CR3 config */
    auto cr3 = USARTx::CR3::load();
    cr3 &= ~(USARTx::CR3::CTSE::value | USARTx::CR3::RTSE::value);
    cr3 |= (uint32_t)flow_control << 8;
    USARTx::CR3::store(cr3);

    /* calculate values for baud rate register */
    unsigned pclk = (usart_no == 1 ?
                     Rcc::ClockFrequency<Core::clock_frequency>::pclk2 : 
                     Rcc::ClockFrequency<Core::clock_frequency>::pclk1 );
    unsigned div  = (pclk * 25) / (4 * baud_rate);
    unsigned mant = div / 100;
    unsigned fraq = ((div - (mant * 100)) * 16 + 50) / 100;

    USARTx::BRR::store((mant << 4) | (fraq & 0x0f));
  }

};

#endif // STM32_COMMON_USART_HPP_INCLUDED

