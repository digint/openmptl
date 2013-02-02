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
   * Note that the registers are actually only 16bit wide, but accessing
   * them with 32bit is faster in general.
   */
  template<reg_addr_t base_addr>
  struct __USART_COMMON
  {
    static_assert(base_addr > 0, "invalid base address");
    static constexpr const char * name_str = "USARTx";

    /**
     * Status register
     */
    struct SR
    : public Register< uint32_t, base_addr + 0x00, Access::rw, 0x00C0 >
    {
      typedef Register< uint32_t, base_addr + 0x00, Access::rw, 0x00C0 > reg_type;
      static constexpr const char * name_str = "SR";

      typedef RegisterBits< reg_type,  9,  1 > CTS;    /**< CTS flag                      */
      typedef RegisterBits< reg_type,  8,  1 > LBD;    /**< LIN break detection flag      */
      typedef RegisterBits< reg_type,  7,  1 > TXE;    /**< Transmit data register empty  */
      typedef RegisterBits< reg_type,  6,  1 > TC;     /**< Transmission complete         */
      typedef RegisterBits< reg_type,  5,  1 > RXNE;   /**< Read data register not empty  */
      typedef RegisterBits< reg_type,  4,  1 > IDLE;   /**< IDLE line detected            */
      typedef RegisterBits< reg_type,  3,  1 > ORE;    /**< Overrun error                 */
      typedef RegisterBits< reg_type,  2,  1 > NE;     /**< Noise error flag              */
      typedef RegisterBits< reg_type,  1,  1 > FE;     /**< Framing error                 */
      typedef RegisterBits< reg_type,  0,  1 > PE;     /**< Parity error                  */
    };

    /**
     * Data register
     */
    struct DR
    : public Register< uint32_t, base_addr + 0x04, Access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, base_addr + 0x04, Access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "DR";

      typedef RegisterBits< reg_type,  0,  9 > DR_;   /**< Data value  */
    };

    /**
     * Baud rate register
     */
    struct BRR
    : public Register< uint32_t, base_addr + 0x08, Access::rw, 0x0000 >
    {
      typedef Register< uint32_t, base_addr + 0x08, Access::rw, 0x0000 > reg_type;
      static constexpr const char * name_str = "BRR";

      typedef RegisterBits< reg_type,  4, 12 > DIV_Mantissa;   /**< mantissa of USARTDIV  */
      typedef RegisterBits< reg_type,  0,  4 > DIV_Fraction;   /**< fraction of USARTDIV  */
    };

    /**
     * Control register 1
     */
    struct CR1
    : public Register< uint32_t, base_addr + 0x0C, Access::rw, 0x0000 >
    {
      typedef Register< uint32_t, base_addr + 0x0C, Access::rw, 0x0000 > reg_type;
      static constexpr const char * name_str = "CR1";

      typedef RegisterBits< reg_type, 13,  1 > UE;       /**< USART enable                            */
      typedef RegisterBits< reg_type, 12,  1 > M;        /**< Word length                             */
      typedef RegisterBits< reg_type, 11,  1 > WAKE;     /**< Wakeup method                           */
      typedef RegisterBits< reg_type, 10,  1 > PCE;      /**< Parity control enable                   */
      typedef RegisterBits< reg_type,  9,  1 > PS;       /**< Parity selection                        */
      typedef RegisterBits< reg_type,  8,  1 > PEIE;     /**< PE interrupt enable                     */
      typedef RegisterBits< reg_type,  7,  1 > TXEIE;    /**< TXE interrupt enable                    */
      typedef RegisterBits< reg_type,  6,  1 > TCIE;     /**< Transmission complete interrupt enable  */
      typedef RegisterBits< reg_type,  5,  1 > RXNEIE;   /**< RXNE interrupt enable                   */
      typedef RegisterBits< reg_type,  4,  1 > IDLEIE;   /**< IDLE interrupt enable                   */
      typedef RegisterBits< reg_type,  3,  1 > TE;       /**< Transmitter enable                      */
      typedef RegisterBits< reg_type,  2,  1 > RE;       /**< Receiver enable                         */
      typedef RegisterBits< reg_type,  1,  1 > RWU;      /**< Receiver wakeup                         */
      typedef RegisterBits< reg_type,  0,  1 > SBK;      /**< Send break                              */
    };

    /**
     * Control register 2
     */
    struct CR2
    : public Register< uint32_t, base_addr + 0x10, Access::rw, 0x0000 >
    {
      typedef Register< uint32_t, base_addr + 0x10, Access::rw, 0x0000 > reg_type;
      static constexpr const char * name_str = "CR2";

      typedef RegisterBits< reg_type, 14,  1 > LINEN;   /**< LIN mode enable                       */
      typedef RegisterBits< reg_type, 12,  2 > STOP;    /**< STOP bits                             */
      typedef RegisterBits< reg_type, 11,  1 > CLKEN;   /**< Clock enable                          */
      typedef RegisterBits< reg_type, 10,  1 > CPOL;    /**< Clock polarity                        */
      typedef RegisterBits< reg_type,  9,  1 > CPHA;    /**< Clock phase                           */
      typedef RegisterBits< reg_type,  8,  1 > LBCL;    /**< Last bit clock pulse                  */
      typedef RegisterBits< reg_type,  6,  1 > LBDIE;   /**< LIN break detection interrupt enable  */
      typedef RegisterBits< reg_type,  5,  1 > LBDL;    /**< LIN break detection length            */
      typedef RegisterBits< reg_type,  0,  4 > ADD;     /**< Address of the USART node             */
    };

    /**
     * Control register 3
     */
    struct CR3
    : public Register< uint32_t, base_addr + 0x14, Access::rw, 0x0000 >
    {
      typedef Register< uint32_t, base_addr + 0x14, Access::rw, 0x0000 > reg_type;
      static constexpr const char * name_str = "CR3";

      typedef RegisterBits< reg_type, 10,  1 > CTSIE;   /**< CTS interrupt enable    */
      typedef RegisterBits< reg_type,  9,  1 > CTSE;    /**< CTS enable              */
      typedef RegisterBits< reg_type,  8,  1 > RTSE;    /**< RTS enable              */
      typedef RegisterBits< reg_type,  7,  1 > DMAT;    /**< DMA enable transmitter  */
      typedef RegisterBits< reg_type,  6,  1 > DMAR;    /**< DMA enable receiver     */
      typedef RegisterBits< reg_type,  5,  1 > SCEN;    /**< Smartcard mode enable   */
      typedef RegisterBits< reg_type,  4,  1 > NACK;    /**< Smartcard NACK enable   */
      typedef RegisterBits< reg_type,  3,  1 > HDSEL;   /**< Half-duplex selection   */
      typedef RegisterBits< reg_type,  2,  1 > IRLP;    /**< IrDA low-power          */
      typedef RegisterBits< reg_type,  1,  1 > IREN;    /**< IrDA mode enable        */
      typedef RegisterBits< reg_type,  0,  1 > EIE;     /**< Error interrupt enable  */
    };

    /**
     * Guard time and prescaler register
     */
    struct GTPR
    : public Register< uint32_t, base_addr + 0x18, Access::rw, 0x0000 >
    {
      typedef Register< uint32_t, base_addr + 0x18, Access::rw, 0x0000 > reg_type;
      static constexpr const char * name_str = "GTPR";

      typedef RegisterBits< reg_type,  8,  8 > GT;    /**< Guard time value  */
      typedef RegisterBits< reg_type,  0,  8 > PSC;   /**< Prescaler value   */
    };
  };


  /**
   * Some architectures (e.g. stm32f4xx) provide oversampling and
   * one-sample-bit mode.
   */
  template<reg_addr_t base_addr>
  class __USART_COMMON_EXT : public __USART_COMMON<base_addr>
  {
    typedef __USART_COMMON<base_addr> base;

  public:
    struct CR1 : public base::CR1 {
      typedef RegisterBits< typename base::CR1::reg_type, 15,  1 > OVER8;    /**< Oversampling mode  */
    };
    struct CR3 : public base::CR3 {
      typedef RegisterBits< typename base::CR3::reg_type, 11,  1 > ONEBIT;   /**< One sample bit method enable  */
    };
  };
}

#endif // STM32_COMMON_REG_USART_HPP_INCLUDED
