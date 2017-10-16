/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright (C) 2012-2017 Axel Burri <axel@tty0.ch>
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

namespace mptl {

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
  : public reg< std::uint_fast16_t, base_addr + 0x00, rw, 0x0000 >
  {
    using type = reg< std::uint_fast16_t, base_addr + 0x0, rw, 0x0000 >;

    /** Baud Rate Control  */
    template<typename Rb>
      struct __BR
      : public Rb
    {
      template<unsigned prescaler>
      struct Prescaler
      : public regval< Rb,
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

    using BIDIMODE  =        regbits< type, 15,  1 >;    /**< Bidirectional data mode enable       */
    using BIDIOE    =        regbits< type, 14,  1 >;    /**< Output enable in bidirectional mode  */
    using CRCEN     =        regbits< type, 13,  1 >;    /**< Hardware CRC calculation enable      */
    using CRCNEXT   =        regbits< type, 12,  1 >;    /**< CRC transfer next                    */
    using DFF       =        regbits< type, 11,  1 >;    /**< Data frame format                    */
    using RXONLY    =        regbits< type, 10,  1 >;    /**< Receive only                         */
    using SSM       =        regbits< type,  9,  1 >;    /**< Software slave management            */
    using SSI       =        regbits< type,  8,  1 >;    /**< Internal slave select                */
    using LSBFIRST  =        regbits< type,  7,  1 >;    /**< Frame format                         */
    using SPE       =        regbits< type,  6,  1 >;    /**< SPI enable                           */
    using BR        = __BR < regbits< type,  3,  3 > >;  /**< Baud rate control                    */
    using MSTR      =        regbits< type,  2,  1 >;    /**< Master selection                     */
    using CPOL      =        regbits< type,  1,  1 >;    /**< Clock polarity                       */
    using CPHA      =        regbits< type,  0,  1 >;    /**< Clock phase                          */
  };

  /**
   * Control register 2
   */
  struct CR2
  : public reg< std::uint_fast16_t, base_addr + 0x04, rw, 0x0000 >
  {
    using type = reg< std::uint_fast16_t, base_addr + 0x4, rw, 0x0000 >;

    using TXEIE    = regbits< type,  7,  1 >;  /**< Tx buffer empty interrupt enable      */
    using RXNEIE   = regbits< type,  6,  1 >;  /**< Rx buffer not empty interrupt enable  */
    using ERRIE    = regbits< type,  5,  1 >;  /**< Error interrupt enable                */
    using SSOE     = regbits< type,  2,  1 >;  /**< SS output enable                      */
    using TXDMAEN  = regbits< type,  1,  1 >;  /**< Tx buffer DMA enable                  */
    using RXDMAEN  = regbits< type,  0,  1 >;  /**< Rx buffer DMA enable                  */
  };

  /**
   * Status register
   */
  struct SR
  : public reg< std::uint_fast16_t, base_addr + 0x08, rw, 0x0002 >
  {
    using type = reg< std::uint_fast16_t, base_addr + 0x8, rw, 0x0002 >;

    using BSY     = regbits< type,  7,  1 >;  /**< Busy flag                 */
    using OVR     = regbits< type,  6,  1 >;  /**< Overrun flag              */
    using MODF    = regbits< type,  5,  1 >;  /**< Mode fault                */
    using CRCERR  = regbits< type,  4,  1 >;  /**< CRC error flag            */
    using UDR     = regbits< type,  3,  1 >;  /**< Underrun flag             */
    using CHSIDE  = regbits< type,  2,  1 >;  /**< Channel side              */
    using TXE     = regbits< type,  1,  1 >;  /**< Transmit buffer empty     */
    using RXNE    = regbits< type,  0,  1 >;  /**< Receive buffer not empty  */
  };

  /**
   * Data register
   */
  using DR = reg< std::uint_fast16_t, base_addr + 0x0c, rw, 0x0000 >;

  /**
   * CRC polynomial register
   */
  using CRCPR = reg< std::uint_fast16_t, base_addr + 0x10, rw, 0x0007 >;

  /**
   * Rx CRC register
   */
  using RXCRCR = reg< std::uint_fast16_t, base_addr + 0x14, ro, 0x0000 >;

  /**
   * Tx CRC register
   */
  using TXCRCR = reg< std::uint_fast16_t, base_addr + 0x18, ro, 0x0000 >;

  /**
   * I2S configuration register
   */
  struct I2SCFGR
  : public reg< std::uint_fast16_t, base_addr + 0x1c, rw, 0x0000 >
  {
    using type = reg< std::uint_fast16_t, base_addr + 0x1c, rw, 0x0000 >;

    using I2SMOD   = regbits< type, 11,  1 >;  /**< I2S mode selection                                 */
    using I2SE     = regbits< type, 10,  1 >;  /**< I2S Enable                                         */
    using I2SCFG   = regbits< type,  8,  2 >;  /**< I2S configuration mode                             */
    using PCMSYNC  = regbits< type,  7,  1 >;  /**< PCM frame synchronization                          */
    using I2SSTD   = regbits< type,  4,  2 >;  /**< I2S standard selection                             */
    using CKPOL    = regbits< type,  3,  1 >;  /**< Steady state clock polarity                        */
    using DATLEN   = regbits< type,  1,  2 >;  /**< Data length to be transferred                      */
    using CHLEN    = regbits< type,  0,  1 >;  /**< Channel length (number of bits per audio channel)  */
  };

  /**
   * I2S prescaler register
   */
  struct I2SPR
  : public reg< std::uint_fast16_t, base_addr + 0x20, rw, 00000010 >
  {
    using type = reg< std::uint_fast16_t, base_addr + 0x20, rw, 00000010 >;

    using MCKOE    = regbits< type,  9,  1 >;  /**< Master clock output enable    */
    using ODD      = regbits< type,  8,  1 >;  /**< Odd factor for the prescaler  */
    using I2SDIV   = regbits< type,  0,  8 >;  /**< I2S Linear prescaler          */
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
    using FRF = regbits< typename base_type::CR2::type,  4,  1 >;  /**< Frame format  */
  };
  struct SR : public base_type::SR {
    using RFE = regbits< typename base_type::SR::type,  8,  1 >;  /**< Frame format error  */
  };
};

} // namespace mptl

#endif // ARM_CORTEX_STM32_COMMON_REG_SPI_HPP_INCLUDED
