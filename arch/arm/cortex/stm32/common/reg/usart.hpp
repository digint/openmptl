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

#ifndef STM32_COMMON_REG_USART_HPP_INCLUDED
#define STM32_COMMON_REG_USART_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /**
   * Universal synchronous asynchronous receiver transmitter (USART),
   * common to all stm32 processors.
   *
   * NOTE: We use std::uint_fast16_t to access the registers. This way
   * we leave it up to the compiler to use either 16-bit or 32-bit
   * integers for the access_type. More precicely: "use fastest
   * unsigned integer type with width of at least 16 bits". In most
   * situations the compiler toolchain (gcc-4.8) will choose 32bit
   * integers for ARM / ARM-Thumb(2).
   *
   * TODO: move ^^^this^^^ text to the "register access" discussion.
   */
  template<reg_addr_t base_addr>
  struct USART_Common
  {
    /**
     * Status register
     */
    struct SR
    : public Register< std::uint_fast16_t, base_addr + 0x00, Access::rw, 0x00c0 >
    {
      // TODO: document why it sucks to have to define reg_type here
      // again.  This is required here (by the standard) because the
      // template parameter "base_addr" is used as template-parameter
      // in our Register<> base class:
      //
      //   Register< ..., base_addr + 0x00, ... >
      //                  ^^^
      // C++ Standard says that you should fully qualify
      // name according to 14.6.2/3:
      //
      //   In the definition of a class template or a member of a
      //   class template, if a base class of the class template
      //   depends on a template-parameter, the base class scope is
      //   not examined during unqualified name lookup either at the
      //   point of definition of the class template or member or
      //   during an instantiation of the class template or member.
      //
      // This all has to do with specialization: the Register<>
      // template could be specialized later on, possibly making
      // reg_type unavailable.
      // 
      // see discussion here: http://stackoverflow.com/questions/1643035/propagating-typedef-from-based-to-derived-class-for-template
      //
      using reg_type = Register< std::uint_fast16_t, base_addr + 0x00, Access::rw, 0x00c0 >;

      using CTS   = RegisterBits< reg_type,  9,  1 >;   /**< CTS flag                      */
      using LBD   = RegisterBits< reg_type,  8,  1 >;   /**< LIN break detection flag      */
      using TXE   = RegisterBits< reg_type,  7,  1 >;   /**< Transmit data register empty  */
      using TC    = RegisterBits< reg_type,  6,  1 >;   /**< Transmission complete         */
      using RXNE  = RegisterBits< reg_type,  5,  1 >;   /**< Read data register not empty  */
      using IDLE  = RegisterBits< reg_type,  4,  1 >;   /**< IDLE line detected            */
      using ORE   = RegisterBits< reg_type,  3,  1 >;   /**< Overrun error                 */
      using NE    = RegisterBits< reg_type,  2,  1 >;   /**< Noise error flag              */
      using FE    = RegisterBits< reg_type,  1,  1 >;   /**< Framing error                 */
      using PE    = RegisterBits< reg_type,  0,  1 >;   /**< Parity error                  */
    };

    /**
     * Data register
     */
    struct DR
    : public Register< std::uint_fast16_t, base_addr + 0x04, Access::rw, 0x00000000 >
    {
      using reg_type = Register< std::uint_fast16_t, base_addr + 0x04, Access::rw, 0x00000000 >;

      using bits_type = RegisterBits< reg_type,  0,  9 >;   /**< Data value  */
    };

    /**
     * Baud rate register
     */
    struct BRR
    : public Register< std::uint_fast16_t, base_addr + 0x08, Access::rw, 0x0000 >
    {
      using reg_type = Register< std::uint_fast16_t, base_addr + 0x08, Access::rw, 0x0000 >;

      using DIV_Mantissa = RegisterBits< reg_type,  4, 12 >;   /**< mantissa of USARTDIV  */
      using DIV_Fraction = RegisterBits< reg_type,  0,  4 >;   /**< fraction of USARTDIV  */
    };

    /**
     * Control register 1
     */
    struct CR1
    : public Register< std::uint_fast16_t, base_addr + 0x0c, Access::rw, 0x0000 >
    {
      using reg_type = Register< std::uint_fast16_t, base_addr + 0x0c, Access::rw, 0x0000 >;

      using UE      = RegisterBits< reg_type, 13,  1 >;   /**< USART enable                            */
      using M       = RegisterBits< reg_type, 12,  1 >;   /**< Word length                             */
      using WAKE    = RegisterBits< reg_type, 11,  1 >;   /**< Wakeup method                           */
      using PCE     = RegisterBits< reg_type, 10,  1 >;   /**< Parity control enable                   */
      using PS      = RegisterBits< reg_type,  9,  1 >;   /**< Parity selection                        */
      using PEIE    = RegisterBits< reg_type,  8,  1 >;   /**< PE interrupt enable                     */
      using TXEIE   = RegisterBits< reg_type,  7,  1 >;   /**< TXE interrupt enable                    */
      using TCIE    = RegisterBits< reg_type,  6,  1 >;   /**< Transmission complete interrupt enable  */
      using RXNEIE  = RegisterBits< reg_type,  5,  1 >;   /**< RXNE interrupt enable                   */
      using IDLEIE  = RegisterBits< reg_type,  4,  1 >;   /**< IDLE interrupt enable                   */
      using TE      = RegisterBits< reg_type,  3,  1 >;   /**< Transmitter enable                      */
      using RE      = RegisterBits< reg_type,  2,  1 >;   /**< Receiver enable                         */
      using RWU     = RegisterBits< reg_type,  1,  1 >;   /**< Receiver wakeup                         */
      using SBK     = RegisterBits< reg_type,  0,  1 >;   /**< Send break                              */
    };

    /**
     * Control register 2
     */
    struct CR2
    : public Register< std::uint_fast16_t, base_addr + 0x10, Access::rw, 0x0000 >
    {
      using reg_type = Register< std::uint_fast16_t, base_addr + 0x10, Access::rw, 0x0000 >;

      using LINEN  = RegisterBits< reg_type, 14,  1 >;   /**< LIN mode enable                       */
      using STOP   = RegisterBits< reg_type, 12,  2 >;   /**< STOP bits                             */
      using CLKEN  = RegisterBits< reg_type, 11,  1 >;   /**< Clock enable                          */
      using CPOL   = RegisterBits< reg_type, 10,  1 >;   /**< Clock polarity                        */
      using CPHA   = RegisterBits< reg_type,  9,  1 >;   /**< Clock phase                           */
      using LBCL   = RegisterBits< reg_type,  8,  1 >;   /**< Last bit clock pulse                  */
      using LBDIE  = RegisterBits< reg_type,  6,  1 >;   /**< LIN break detection interrupt enable  */
      using LBDL   = RegisterBits< reg_type,  5,  1 >;   /**< LIN break detection length            */
      using ADD    = RegisterBits< reg_type,  0,  4 >;   /**< Address of the USART node             */
    };

    /**
     * Control register 3
     */
    struct CR3
    : public Register< std::uint_fast16_t, base_addr + 0x14, Access::rw, 0x0000 >
    {
      using reg_type = Register< std::uint_fast16_t, base_addr + 0x14, Access::rw, 0x0000 >;

      using CTSIE  = RegisterBits< reg_type, 10,  1 >;   /**< CTS interrupt enable    */
      using CTSE   = RegisterBits< reg_type,  9,  1 >;   /**< CTS enable              */
      using RTSE   = RegisterBits< reg_type,  8,  1 >;   /**< RTS enable              */
      using DMAT   = RegisterBits< reg_type,  7,  1 >;   /**< DMA enable transmitter  */
      using DMAR   = RegisterBits< reg_type,  6,  1 >;   /**< DMA enable receiver     */
      using SCEN   = RegisterBits< reg_type,  5,  1 >;   /**< Smartcard mode enable   */
      using NACK   = RegisterBits< reg_type,  4,  1 >;   /**< Smartcard NACK enable   */
      using HDSEL  = RegisterBits< reg_type,  3,  1 >;   /**< Half-duplex selection   */
      using IRLP   = RegisterBits< reg_type,  2,  1 >;   /**< IrDA low-power          */
      using IREN   = RegisterBits< reg_type,  1,  1 >;   /**< IrDA mode enable        */
      using EIE    = RegisterBits< reg_type,  0,  1 >;   /**< Error interrupt enable  */
    };

    /**
     * Guard time and prescaler register
     */
    struct GTPR
    : public Register< std::uint_fast16_t, base_addr + 0x18, Access::rw, 0x0000 >
    {
      using reg_type = Register< std::uint_fast16_t, base_addr + 0x18, Access::rw, 0x0000 >;

      using GT   = RegisterBits< reg_type,  8,  8 >;   /**< Guard time value  */
      using PSC  = RegisterBits< reg_type,  0,  8 >;   /**< Prescaler value   */
    };
  };


  /**
   * Some architectures (e.g. stm32f4xx) provide oversampling and
   * one-sample-bit mode.
   */
  template<reg_addr_t base_addr>
  class USART_Common_Ext : public USART_Common<base_addr>
  {
    using base_type = USART_Common<base_addr>;

  public:
    struct CR1 : public base_type::CR1 {
      using OVER8  = RegisterBits< typename base_type::CR1::type, 15,  1 >;   /**< Oversampling mode  */
    };
    struct CR3 : public base_type::CR3 {
      using ONEBIT = RegisterBits< typename base_type::CR3::type, 11,  1 >;   /**< One sample bit method enable  */
    };
  };
}

#endif // STM32_COMMON_REG_USART_HPP_INCLUDED
