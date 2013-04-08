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

#ifndef ARM_CORTEX_STM32_COMMON_REG_SPI_HPP_INCLUDED
#define ARM_CORTEX_STM32_COMMON_REG_SPI_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /**
   * Serial peripheral interface (SPI)
   *
   * NOTE: We use std::uint_fast16_t to access the registers. This way we
   * leave it up to the compiler to use either 16-bit or 32-bit
   * integers for the access_type. More precicely: "use fastest
   * unsigned integer type with width of at least 16 bits". In most
   * situations the compiler (gcc-4.8) will use 32bit integers.
   */
  template<reg_addr_t _base_addr>
  struct SPI_Common
  {
    static constexpr reg_addr_t base_addr = _base_addr;

    /**
     * Control register 1
     */
    struct CR1
    : public Register< std::uint_fast16_t, base_addr + 0x00, Access::rw, 0x0000 >
    {
      using type = Register< std::uint_fast16_t, base_addr + 0x0, Access::rw, 0x0000 >;

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

      using BIDIMODE  =        RegisterBits< type, 15,  1 >;    /**< Bidirectional data mode enable       */
      using BIDIOE    =        RegisterBits< type, 14,  1 >;    /**< Output enable in bidirectional mode  */
      using CRCEN     =        RegisterBits< type, 13,  1 >;    /**< Hardware CRC calculation enable      */
      using CRCNEXT   =        RegisterBits< type, 12,  1 >;    /**< CRC transfer next                    */
      using DFF       =        RegisterBits< type, 11,  1 >;    /**< Data frame format                    */
      using RXONLY    =        RegisterBits< type, 10,  1 >;    /**< Receive only                         */
      using SSM       =        RegisterBits< type,  9,  1 >;    /**< Software slave management            */
      using SSI       =        RegisterBits< type,  8,  1 >;    /**< Internal slave select                */
      using LSBFIRST  =        RegisterBits< type,  7,  1 >;    /**< Frame format                         */
      using SPE       =        RegisterBits< type,  6,  1 >;    /**< SPI enable                           */
      using BR        = __BR < RegisterBits< type,  3,  3 > >;  /**< Baud rate control                    */
      using MSTR      =        RegisterBits< type,  2,  1 >;    /**< Master selection                     */
      using CPOL      =        RegisterBits< type,  1,  1 >;    /**< Clock polarity                       */
      using CPHA      =        RegisterBits< type,  0,  1 >;    /**< Clock phase                          */
    };

    /**
     * Control register 2
     */
    struct CR2
    : public Register< std::uint_fast16_t, base_addr + 0x04, Access::rw, 0x0000 >
    {
      using type = Register< std::uint_fast16_t, base_addr + 0x4, Access::rw, 0x0000 >;

      using TXEIE    = RegisterBits< type,  7,  1 >;  /**< Tx buffer empty interrupt enable      */
      using RXNEIE   = RegisterBits< type,  6,  1 >;  /**< Rx buffer not empty interrupt enable  */
      using ERRIE    = RegisterBits< type,  5,  1 >;  /**< Error interrupt enable                */
      using SSOE     = RegisterBits< type,  2,  1 >;  /**< SS output enable                      */
      using TXDMAEN  = RegisterBits< type,  1,  1 >;  /**< Tx buffer DMA enable                  */
      using RXDMAEN  = RegisterBits< type,  0,  1 >;  /**< Rx buffer DMA enable                  */
    };

    /**
     * Status register
     */
    struct SR
    : public Register< std::uint_fast16_t, base_addr + 0x08, Access::rw, 0x0002 >
    {
      using type = Register< std::uint_fast16_t, base_addr + 0x8, Access::rw, 0x0002 >;

      using BSY     = RegisterBits< type,  7,  1 >;  /**< Busy flag                 */
      using OVR     = RegisterBits< type,  6,  1 >;  /**< Overrun flag              */
      using MODF    = RegisterBits< type,  5,  1 >;  /**< Mode fault                */
      using CRCERR  = RegisterBits< type,  4,  1 >;  /**< CRC error flag            */
      using UDR     = RegisterBits< type,  3,  1 >;  /**< Underrun flag             */
      using CHSIDE  = RegisterBits< type,  2,  1 >;  /**< Channel side              */
      using TXE     = RegisterBits< type,  1,  1 >;  /**< Transmit buffer empty     */
      using RXNE    = RegisterBits< type,  0,  1 >;  /**< Receive buffer not empty  */
    };

    /**
     * Data register
     */
    using DR = Register< std::uint_fast16_t, base_addr + 0x0c, Access::rw, 0x0000 >;

    /**
     * CRC polynomial register
     */
    using CRCPR = Register< std::uint_fast16_t, base_addr + 0x10, Access::rw, 0x0007 >;

    /**
     * Rx CRC register
     */
    using RXCRCR = Register< std::uint_fast16_t, base_addr + 0x14, Access::ro, 0x0000 >;

    /**
     * Tx CRC register
     */
    using TXCRCR = Register< std::uint_fast16_t, base_addr + 0x18, Access::ro, 0x0000 >;

    /**
     * I2S configuration register
     */
    struct I2SCFGR
    : public Register< std::uint_fast16_t, base_addr + 0x1c, Access::rw, 0x0000 >
    {
      using type = Register< std::uint_fast16_t, base_addr + 0x1c, Access::rw, 0x0000 >;

      using I2SMOD   = RegisterBits< type, 11,  1 >;  /**< I2S mode selection                                 */
      using I2SE     = RegisterBits< type, 10,  1 >;  /**< I2S Enable                                         */
      using I2SCFG   = RegisterBits< type,  8,  2 >;  /**< I2S configuration mode                             */
      using PCMSYNC  = RegisterBits< type,  7,  1 >;  /**< PCM frame synchronization                          */
      using I2SSTD   = RegisterBits< type,  4,  2 >;  /**< I2S standard selection                             */
      using CKPOL    = RegisterBits< type,  3,  1 >;  /**< Steady state clock polarity                        */
      using DATLEN   = RegisterBits< type,  1,  2 >;  /**< Data length to be transferred                      */
      using CHLEN    = RegisterBits< type,  0,  1 >;  /**< Channel length (number of bits per audio channel)  */
    };

    /**
     * I2S prescaler register
     */
    struct I2SPR
    : public Register< std::uint_fast16_t, base_addr + 0x20, Access::rw, 00000010 >
    {
      using type = Register< std::uint_fast16_t, base_addr + 0x20, Access::rw, 00000010 >;

      using MCKOE    = RegisterBits< type,  9,  1 >;  /**< Master clock output enable    */
      using ODD      = RegisterBits< type,  8,  1 >;  /**< Odd factor for the prescaler  */
      using I2SDIV   = RegisterBits< type,  0,  8 >;  /**< I2S Linear prescaler          */
    };
  };


  /**
   * Some architectures (e.g. stm32f4xx) support multiple frame formats
   */
  template<reg_addr_t base_addr>
  class SPI_Common_Ext : public SPI_Common<base_addr>
  {
    using base_type = SPI_Common<base_addr>;

  public:

    struct CR2 : public base_type::CR2 {
      using FRF = RegisterBits< typename base_type::CR2::type,  4,  1 >;  /**< Frame format  */
    };
    struct SR : public base_type::SR {
      using RFE = RegisterBits< typename base_type::SR::type,  8,  1 >;  /**< Frame format error  */
    };
  };
}

#endif // ARM_CORTEX_STM32_COMMON_REG_SPI_HPP_INCLUDED
