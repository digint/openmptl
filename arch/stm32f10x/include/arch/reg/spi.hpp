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

#ifndef REG_SPI_HPP_INCLUDED
#define REG_SPI_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /**
   * Serial peripheral interface (SPI)
   *
   * Note that the registers are actually only 16bit wide, but accessing
   * them with 32bit is faster in general.
   */
  template<std::size_t spi_no>
  class SPI
  {
    static_assert((spi_no >= 1) && (spi_no <= 3), "invalid SPI number"); // TODO: depends on cpu sub-arch

    static constexpr reg_addr_t reg_base = spi_no == 1 ? 0x40013000 :
                                           spi_no == 2 ? 0x40003800 :
                                           spi_no == 3 ? 0x40003C00 :
                                           0;

    /** SPI control register 1 */
    template<typename R>
    class __CR1
    : public R
    {

      /** Baud Rate Control  */
      template<typename Rb>
      struct __BR
      : public Rb
      {
        template<unsigned prescaler>
        struct Prescaler
        : RegisterConst< Rb,
                         prescaler == 2   ? 0x0 :      /**< f_PCLK / 2     */
                         prescaler == 4   ? 0x1 :      /**< f_PCLK / 4     */
                         prescaler == 8   ? 0x2 :      /**< f_PCLK / 8     */
                         prescaler == 16  ? 0x3 :      /**< f_PCLK / 16    */
                         prescaler == 32  ? 0x4 :      /**< f_PCLK / 32    */
                         prescaler == 64  ? 0x5 :      /**< f_PCLK / 64    */
                         prescaler == 128 ? 0x6 :      /**< f_PCLK / 128   */
                         prescaler == 256 ? 0x7 :      /**< f_PCLK / 256   */
                         0
                         >
        {
          static_assert(prescaler == 2  || prescaler == 4  || prescaler == 8   || prescaler == 16 ||
                        prescaler == 32 || prescaler == 64 || prescaler == 128 || prescaler == 256
                        , "invalid baud rate prescaler");
        };
      };

    public:

      typedef        RegisterBits< R,  0,  1 >   CPHA;     /**< [ 0: 0] Clock Phase                           */
      typedef        RegisterBits< R,  1,  1 >   CPOL;     /**< [ 1: 1] Clock Polarity                        */
      typedef        RegisterBits< R,  2,  1 >   MSTR;     /**< [ 2: 2] Master Selection                      */
      typedef __BR < RegisterBits< R,  3,  3 > > BR;       /**< [ 5: 3] Baud Rate Control                     */
      typedef        RegisterBits< R,  6,  1 >   SPE;      /**< [ 6: 6] SPI Enable                            */
      typedef        RegisterBits< R,  7,  1 >   LSBFIRST; /**< [ 7: 7] Frame Format                          */
      typedef        RegisterBits< R,  8,  1 >   SSI;      /**< [ 8: 8] Internal slave select                 */
      typedef        RegisterBits< R,  9,  1 >   SSM;      /**< [ 9: 9] Software slave management             */
      typedef        RegisterBits< R, 10,  1 >   RXONLY;   /**< [10:10] Receive only                          */
      typedef        RegisterBits< R, 11,  1 >   DFF;      /**< [11:11] Data Frame Format                     */
      typedef        RegisterBits< R, 12,  1 >   CRCNEXT;  /**< [12:12] Transmit CRC next                     */
      typedef        RegisterBits< R, 13,  1 >   CRCEN;    /**< [13:13] Hardware CRC calculation enable       */
      typedef        RegisterBits< R, 14,  1 >   BIDIOE;   /**< [14:14] Output enable in bidirectional mode   */
      typedef        RegisterBits< R, 15,  1 >   BIDIMODE; /**< [15:15] Bidirectional data mode enable        */
    };

    /** SPI control register 2 */
    template<typename R>
    struct __CR2
    : public R
    {
      typedef RegisterBits< R,  0,  1 > RXDMAEN;       /**< [ 0: 0] Rx Buffer DMA Enable                   */
      typedef RegisterBits< R,  1,  1 > TXDMAEN;       /**< [ 1: 1] Tx Buffer DMA Enable                   */
      typedef RegisterBits< R,  2,  1 > SSOE;          /**< [ 2: 2] SS Output Enable                       */
      typedef RegisterBits< R,  5,  1 > ERRIE;         /**< [ 5: 5] Error Interrupt Enable                 */
      typedef RegisterBits< R,  6,  1 > RXNEIE;        /**< [ 6: 6] RX buffer Not Empty Interrupt Enable   */
      typedef RegisterBits< R,  7,  1 > TXEIE;         /**< [ 7: 7] Tx buffer Empty Interrupt Enable       */
    };

    /** SPI status register */
    template<typename R>
    struct __SR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > RXNE;          /**< [ 0: 0] Receive buffer Not Empty   */
      typedef RegisterBits< R,  1,  1 > TXE;           /**< [ 1: 1] Transmit buffer Empty      */
      typedef RegisterBits< R,  2,  1 > CHSIDE;        /**< [ 2: 2] Channel side               */
      typedef RegisterBits< R,  3,  1 > UDR;           /**< [ 3: 3] Underrun flag              */
      typedef RegisterBits< R,  4,  1 > CRCERR;        /**< [ 4: 4] CRC Error flag             */
      typedef RegisterBits< R,  5,  1 > MODF;          /**< [ 5: 5] Mode fault                 */
      typedef RegisterBits< R,  6,  1 > OVR;           /**< [ 6: 6] Overrun flag               */
      typedef RegisterBits< R,  7,  1 > BSY;           /**< [ 7: 7] Busy flag                  */
    };
    
    /** SPI data register */
    template<typename R>
    struct __DR
    : public R
    {
    };

    /** SPI CRC polynomial register */
    template<typename R>
    struct __CRCPR
    : public R
    {
    };

    /** SPI Rx CRC register */
    template<typename R>
    struct __RXCRCR
    : public R
    {
    };

    /** SPI Tx CRC register */
    template<typename R>
    struct __TXCRCR
    : public R
    {
    };

    /** SPI_I2S configuration register */
    template<typename R>
    struct __I2SCFGR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > CHLEN;         /**< [ 0: 0] Channel length (number of bits per audio channel)   */
      typedef RegisterBits< R,  1,  2 > DATLEN;        /**< [ 2: 1] Data length to be transferred                       */
      typedef RegisterBits< R,  3,  1 > CKPOL;         /**< [ 3: 3] steady state clock polarity                         */
      typedef RegisterBits< R,  4,  2 > I2SSTD;        /**< [ 5: 4] I2S standard selection                              */
      typedef RegisterBits< R,  7,  1 > PCMSYNC;       /**< [ 7: 7] PCM frame synchronization                           */
      typedef RegisterBits< R,  8,  2 > I2SCFG;        /**< [ 9: 8] I2S configuration mode                              */
      typedef RegisterBits< R, 10,  1 > I2SE;          /**< [10:10] I2S Enable                                          */
      typedef RegisterBits< R, 11,  1 > I2SMOD;        /**< [11:11] I2S mode selection                                  */
    };

    /** SPI_I2S prescaler register */
    template<typename R>
    struct __I2SPR
    : public R
    {
      typedef RegisterBits< R,  0,  8 > I2SDIV;        /**< [ 7: 0] I2S Linear prescaler           */
      typedef RegisterBits< R,  8,  1 > ODD;           /**< [ 8: 8] Odd factor for the prescaler   */
      typedef RegisterBits< R,  9,  1 > MCKOE;         /**< [ 9: 9] Master Clock Output Enable     */
    };


  public:

    typedef __CR1    < Register< uint32_t, reg_base + 0x00, Access::rw         > > CR1;
    typedef __CR2    < Register< uint32_t, reg_base + 0x04, Access::rw         > > CR2;
    typedef __SR     < Register< uint32_t, reg_base + 0x08, Access::rw, 0x0002 > > SR;
    typedef __DR     < Register< uint32_t, reg_base + 0x0c, Access::rw         > > DR;
    typedef __CRCPR  < Register< uint32_t, reg_base + 0x10, Access::rw, 0x0007 > > CRCPR;
    typedef __RXCRCR < Register< uint32_t, reg_base + 0x14, Access::ro         > > RXCRCR;
    typedef __TXCRCR < Register< uint32_t, reg_base + 0x18, Access::ro         > > TXCRCR;
    typedef __I2SCFGR< Register< uint32_t, reg_base + 0x1c, Access::rw         > > I2SCFGR;
    typedef __I2SPR  < Register< uint32_t, reg_base + 0x20, Access::rw, 0x0010 > > I2SPR;
  };
}

#endif // REG_SPI_HPP_INCLUDED
