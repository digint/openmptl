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

#ifndef REG_USART_HPP_INCLUDED
#define REG_USART_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /** 
   * Universal synchronous asynchronous receiver transmitter (USART)
   *
   * Note that the registers are actually only 16bit wide, but accessing
   * them with 32bit is faster in general.
   */
  template<std::size_t usart_no>
  class USART
  {
    static_assert((usart_no >= 1) && (usart_no <= 3), "invalid index for register");

    static constexpr reg_addr_t reg_base = usart_no == 1 ? 0x40013800 :
                                           usart_no == 2 ? 0x40004400 :
                                           usart_no == 3 ? 0x40004800 :
                                           0;

    /**
     * Status register                   
     */
    template<typename R>
    struct __SR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > PE;            /**< [ 0: 0] Parity Error                   */
      typedef RegisterBits< R,  1,  1 > FE;            /**< [ 1: 1] Framing Error                  */
      typedef RegisterBits< R,  2,  1 > NE;            /**< [ 2: 2] Noise Error Flag               */
      typedef RegisterBits< R,  3,  1 > ORE;           /**< [ 3: 3] OverRun Error                  */
      typedef RegisterBits< R,  4,  1 > IDLE;          /**< [ 4: 4] IDLE line detected             */
      typedef RegisterBits< R,  5,  1 > RXNE;          /**< [ 5: 5] Read Data Register Not Empty   */
      typedef RegisterBits< R,  6,  1 > TC;            /**< [ 6: 6] Transmission Complete          */
      typedef RegisterBits< R,  7,  1 > TXE;           /**< [ 7: 7] Transmit Data Register Empty   */
      typedef RegisterBits< R,  8,  1 > LBD;           /**< [ 8: 8] LIN Break Detection Flag       */
      typedef RegisterBits< R,  9,  1 > CTS;           /**< [ 9: 9] CTS Flag                       */
    };


    /**
     * Data register                     
     */
    template<typename R>
    struct __DR
    : public R
    {
      typedef RegisterBits< R,  0,  9 > DR_;           /**< [ 8: 0] Data value   */

      /* Bits [31:9] Reserved, forced by hardware to 0. */
    };


    /**
     * Baud rate register                
     */
    template<typename R>
    struct __BRR
    : public R
    {
      typedef RegisterBits< R,  0,  4 > DIV_Fraction;  /**< [ 3: 0] Fraction of USARTDIV   */
      typedef RegisterBits< R,  4, 12 > DIV_Mantissa;  /**< [15: 4] Mantissa of USARTDIV   */
    };


    /**
     * Control register 1                
     */
    template<typename R>
    struct __CR1
    : public R
    {
      typedef RegisterBits< R,  0,  1 > SBK;           /**< [ 0: 0] Send Break                               */
      typedef RegisterBits< R,  1,  1 > RWU;           /**< [ 1: 1] Receiver wakeup                          */
      typedef RegisterBits< R,  2,  1 > RE;            /**< [ 2: 2] Receiver Enable                          */
      typedef RegisterBits< R,  3,  1 > TE;            /**< [ 3: 3] Transmitter Enable                       */
      typedef RegisterBits< R,  4,  1 > IDLEIE;        /**< [ 4: 4] IDLE Interrupt Enable                    */
      typedef RegisterBits< R,  5,  1 > RXNEIE;        /**< [ 5: 5] RXNE Interrupt Enable                    */
      typedef RegisterBits< R,  6,  1 > TCIE;          /**< [ 6: 6] Transmission Complete Interrupt Enable   */
      typedef RegisterBits< R,  7,  1 > TXEIE;         /**< [ 7: 7] PE Interrupt Enable                      */
      typedef RegisterBits< R,  8,  1 > PEIE;          /**< [ 8: 8] PE Interrupt Enable                      */
      typedef RegisterBits< R,  9,  1 > PS;            /**< [ 9: 9] Parity Selection                         */
      typedef RegisterBits< R, 10,  1 > PCE;           /**< [10:10] Parity Control Enable                    */
      typedef RegisterBits< R, 11,  1 > WAKE;          /**< [11:11] Wakeup method                            */
      typedef RegisterBits< R, 12,  1 > M;             /**< [12:12] Word length                              */
      typedef RegisterBits< R, 13,  1 > UE;            /**< [13:13] USART Enable                             */
      typedef RegisterBits< R, 15,  1 > OVER8;         /**< [15:15] USART Oversmapling 8-bits                */
    };


    /**
     * Control register 2                
     */
    template<typename R>
    struct __CR2
    : public R
    {
      typedef RegisterBits< R,  0,  4 > ADD;           /**< [ 3: 0] Address of the USART node              */
      typedef RegisterBits< R,  5,  1 > LBDL;          /**< [ 5: 5] LIN Break Detection Length             */
      typedef RegisterBits< R,  6,  1 > LBDIE;         /**< [ 6: 6] LIN Break Detection Interrupt Enable   */
      typedef RegisterBits< R,  8,  1 > LBCL;          /**< [ 8: 8] Last Bit Clock pulse                   */
      typedef RegisterBits< R,  9,  1 > CPHA;          /**< [ 9: 9] Clock Phase                            */
      typedef RegisterBits< R, 10,  1 > CPOL;          /**< [10:10] Clock Polarity                         */
      typedef RegisterBits< R, 11,  1 > CLKEN;         /**< [11:11] Clock Enable                           */
      typedef RegisterBits< R, 12,  2 > STOP;          /**< [13:12] STOP bits                              */
      typedef RegisterBits< R, 14,  1 > LINEN;         /**< [14:14] LIN mode enable                        */
    };


    /**
     * Control register 3                
     */
    template<typename R>
    struct __CR3
    : public R
    {
      typedef RegisterBits< R,  0,  1 > EIE;           /**< [ 0: 0] Error Interrupt Enable   */
      typedef RegisterBits< R,  1,  1 > IREN;          /**< [ 1: 1] IrDA mode Enable         */
      typedef RegisterBits< R,  2,  1 > IRLP;          /**< [ 2: 2] IrDA Low-Power           */
      typedef RegisterBits< R,  3,  1 > HDSEL;         /**< [ 3: 3] Half-Duplex Selection    */
      typedef RegisterBits< R,  4,  1 > NACK;          /**< [ 4: 4] Smartcard NACK enable    */
      typedef RegisterBits< R,  5,  1 > SCEN;          /**< [ 5: 5] Smartcard mode enable    */
      typedef RegisterBits< R,  6,  1 > DMAR;          /**< [ 6: 6] DMA Enable Receiver      */
      typedef RegisterBits< R,  7,  1 > DMAT;          /**< [ 7: 7] DMA Enable Transmitter   */
      typedef RegisterBits< R,  8,  1 > RTSE;          /**< [ 8: 8] RTS Enable               */
      typedef RegisterBits< R,  9,  1 > CTSE;          /**< [ 9: 9] CTS Enable               */
      typedef RegisterBits< R, 10,  1 > CTSIE;         /**< [10:10] CTS Interrupt Enable     */
      typedef RegisterBits< R, 11,  1 > ONEBIT;        /**< [11:11] One Bit method           */
    };


    /**
     * Guard time and prescaler register 
     */
    template<typename R>
    struct __GTPR
    : public R
    {
      typedef RegisterBits< R,  0,  8 > PSC;           /**< [ 7: 0] Prescaler value    */
      typedef RegisterBits< R,  8,  8 > GT;            /**< [15: 8] Guard time value   */
    };


  public:

    typedef __SR  < Register< uint32_t, reg_base + 0x00, Access::rw, 0x00C0 > > SR;
    typedef __DR  < Register< uint32_t, reg_base + 0x04, Access::rw         > > DR;
    typedef __BRR < Register< uint32_t, reg_base + 0x08, Access::rw         > > BRR;
    typedef __CR1 < Register< uint32_t, reg_base + 0x0c, Access::rw         > > CR1;
    typedef __CR2 < Register< uint32_t, reg_base + 0x10, Access::rw         > > CR2;
    typedef __CR3 < Register< uint32_t, reg_base + 0x14, Access::rw         > > CR3;
    typedef __GTPR< Register< uint32_t, reg_base + 0x18, Access::rw         > > GTPR;
  };
}

#endif // REG_USART_HPP_INCLUDED
