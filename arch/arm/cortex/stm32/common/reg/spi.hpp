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

#ifndef STM32_COMMON_REG_SPI_HPP_INCLUDED
#define STM32_COMMON_REG_SPI_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /**
   * Serial peripheral interface (SPI)
   *
   * Note that the registers are actually only 16bit wide, but accessing
   * them with 32bit is faster in general.
   */
  template<reg_addr_t base_addr>
  struct __SPI_COMMON
  {
    static_assert(base_addr > 0, "invalid base address");
    static constexpr const char * name_str = "SPIx";

    /**
     * Control register 1
     */
    struct CR1
    : public Register< uint32_t, base_addr + 0x00, Access::rw, 0x0000 >
    {
      typedef Register< uint32_t, base_addr + 0x00, Access::rw, 0x0000 > reg_type;
      static constexpr const char * name_str = "CR1";

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

      typedef        RegisterBits< reg_type, 15,  1 >   BIDIMODE;   /**< Bidirectional data mode enable       */
      typedef        RegisterBits< reg_type, 14,  1 >   BIDIOE;     /**< Output enable in bidirectional mode  */
      typedef        RegisterBits< reg_type, 13,  1 >   CRCEN;      /**< Hardware CRC calculation enable      */
      typedef        RegisterBits< reg_type, 12,  1 >   CRCNEXT;    /**< CRC transfer next                    */
      typedef        RegisterBits< reg_type, 11,  1 >   DFF;        /**< Data frame format                    */
      typedef        RegisterBits< reg_type, 10,  1 >   RXONLY;     /**< Receive only                         */
      typedef        RegisterBits< reg_type,  9,  1 >   SSM;        /**< Software slave management            */
      typedef        RegisterBits< reg_type,  8,  1 >   SSI;        /**< Internal slave select                */
      typedef        RegisterBits< reg_type,  7,  1 >   LSBFIRST;   /**< Frame format                         */
      typedef        RegisterBits< reg_type,  6,  1 >   SPE;        /**< SPI enable                           */
      typedef __BR < RegisterBits< reg_type,  3,  3 > > BR;         /**< Baud rate control                    */
      typedef        RegisterBits< reg_type,  2,  1 >   MSTR;       /**< Master selection                     */
      typedef        RegisterBits< reg_type,  1,  1 >   CPOL;       /**< Clock polarity                       */
      typedef        RegisterBits< reg_type,  0,  1 >   CPHA;       /**< Clock phase                          */
    };

    /**
     * Control register 2
     */
    struct CR2
    : public Register< uint32_t, base_addr + 0x04, Access::rw, 0x0000 >
    {
      typedef Register< uint32_t, base_addr + 0x04, Access::rw, 0x0000 > reg_type;
      static constexpr const char * name_str = "CR2";

      typedef RegisterBits< reg_type,  7,  1 > TXEIE;     /**< Tx buffer empty interrupt enable      */
      typedef RegisterBits< reg_type,  6,  1 > RXNEIE;    /**< RX buffer not empty interrupt enable  */
      typedef RegisterBits< reg_type,  5,  1 > ERRIE;     /**< Error interrupt enable                */
      typedef RegisterBits< reg_type,  2,  1 > SSOE;      /**< SS output enable                      */
      typedef RegisterBits< reg_type,  1,  1 > TXDMAEN;   /**< Tx buffer DMA enable                  */
      typedef RegisterBits< reg_type,  0,  1 > RXDMAEN;   /**< Rx buffer DMA enable                  */
    };

    /**
     * Status register
     */
    struct SR
    : public Register< uint32_t, base_addr + 0x08, Access::rw, 0x0002 >
    {
      typedef Register< uint32_t, base_addr + 0x08, Access::rw, 0x0002 > reg_type;
      static constexpr const char * name_str = "SR";

      typedef RegisterBits< reg_type,  7,  1 > BSY;      /**< Busy flag                 */
      typedef RegisterBits< reg_type,  6,  1 > OVR;      /**< Overrun flag              */
      typedef RegisterBits< reg_type,  5,  1 > MODF;     /**< Mode fault                */
      typedef RegisterBits< reg_type,  4,  1 > CRCERR;   /**< CRC error flag            */
      typedef RegisterBits< reg_type,  3,  1 > UDR;      /**< Underrun flag             */
      typedef RegisterBits< reg_type,  2,  1 > CHSIDE;   /**< Channel side              */
      typedef RegisterBits< reg_type,  1,  1 > TXE;      /**< Transmit buffer empty     */
      typedef RegisterBits< reg_type,  0,  1 > RXNE;     /**< Receive buffer not empty  */
    };

    /**
     * Data register
     */
    struct DR
    : public Register< uint32_t, base_addr + 0x0C, Access::rw, 0x0000 >
    {
      typedef Register< uint32_t, base_addr + 0x0C, Access::rw, 0x0000 > reg_type;
      static constexpr const char * name_str = "DR";

      typedef RegisterBits< reg_type,  0, 16 > DR_;   /**< Data register  */
    };

    /**
     * CRC polynomial register
     */
    struct CRCPR
    : public Register< uint32_t, base_addr + 0x10, Access::rw, 0x0007 >
    {
      typedef Register< uint32_t, base_addr + 0x10, Access::rw, 0x0007 > reg_type;
      static constexpr const char * name_str = "CRCPR";

      typedef RegisterBits< reg_type,  0, 16 > CRCPOLY;   /**< CRC polynomial register  */
    };

    /**
     * Rx CRC register
     */
    struct RXCRCR
    : public Register< uint32_t, base_addr + 0x14, Access::ro, 0x0000 >
    {
      typedef Register< uint32_t, base_addr + 0x14, Access::ro, 0x0000 > reg_type;
      static constexpr const char * name_str = "RXCRCR";

      typedef RegisterBits< reg_type,  0, 16 > RXCRC_;   /**< Rx CRC register  */
    };

    /**
     * Tx CRC register
     */
    struct TXCRCR
    : public Register< uint32_t, base_addr + 0x18, Access::ro, 0x0000 >
    {
      typedef Register< uint32_t, base_addr + 0x18, Access::ro, 0x0000 > reg_type;
      static constexpr const char * name_str = "TXCRCR";

      typedef RegisterBits< reg_type,  0, 16 > TXCRC_;   /**< Tx CRC register  */
    };

    /**
     * I2S configuration register
     */
    struct I2SCFGR
    : public Register< uint32_t, base_addr + 0x1C, Access::rw, 0x0000 >
    {
      typedef Register< uint32_t, base_addr + 0x1C, Access::rw, 0x0000 > reg_type;
      static constexpr const char * name_str = "I2SCFGR";

      typedef RegisterBits< reg_type, 11,  1 > I2SMOD;    /**< I2S mode selection                                 */
      typedef RegisterBits< reg_type, 10,  1 > I2SE;      /**< I2S Enable                                         */
      typedef RegisterBits< reg_type,  8,  2 > I2SCFG;    /**< I2S configuration mode                             */
      typedef RegisterBits< reg_type,  7,  1 > PCMSYNC;   /**< PCM frame synchronization                          */
      typedef RegisterBits< reg_type,  4,  2 > I2SSTD;    /**< I2S standard selection                             */
      typedef RegisterBits< reg_type,  3,  1 > CKPOL;     /**< Steady state clock polarity                        */
      typedef RegisterBits< reg_type,  1,  2 > DATLEN;    /**< Data length to be transferred                      */
      typedef RegisterBits< reg_type,  0,  1 > CHLEN;     /**< Channel length (number of bits per audio channel)  */
    };

    /**
     * I2S prescaler register
     */
    struct I2SPR
    : public Register< uint32_t, base_addr + 0x20, Access::rw, 00000010 >
    {
      typedef Register< uint32_t, base_addr + 0x20, Access::rw, 00000010 > reg_type;
      static constexpr const char * name_str = "I2SPR";

      typedef RegisterBits< reg_type,  9,  1 > MCKOE;    /**< Master clock output enable    */
      typedef RegisterBits< reg_type,  8,  1 > ODD;      /**< Odd factor for the prescaler  */
      typedef RegisterBits< reg_type,  0,  8 > I2SDIV;   /**< I2S Linear prescaler          */
    };
  };


  /**
   * Some architectures (e.g. stm32f4xx) support multiple frame formats
   */
  template<reg_addr_t base_addr>
  class __SPI_COMMON_EXT : public __SPI_COMMON<base_addr>
  {
    typedef __SPI_COMMON<base_addr> base;

  public:
    struct CR2 : public base::CR2 {
      typedef RegisterBits< typename base::CR2::reg_type,  4,  1 > FRF;  /**< Frame format  */
    };
    struct SR : public base::SR {
      typedef RegisterBits< typename base::SR::reg_type,  8,  1 > FRE;  /**< Frame format error  */
    };
  };
}

#endif // STM32_COMMON_REG_SPI_HPP_INCLUDED
