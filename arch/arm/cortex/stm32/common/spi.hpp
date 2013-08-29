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

#ifndef ARM_CORTEX_STM32_COMMON_SPI_HPP_INCLUDED
#define ARM_CORTEX_STM32_COMMON_SPI_HPP_INCLUDED

#include <arch/rcc.hpp>
#include <arch/nvic.hpp>
#include <arch/reg/spi.hpp>
#include <type_traits>

namespace mptl {

template< unsigned _spi_no, typename system_clock_type >
class spi_stm32_common
{
public:

  static constexpr unsigned spi_no = _spi_no;
  using SPIx = SPI<spi_no>;
  using irq  = irq::spi<spi_no>;

  using resources = rcc_spi_clock_resources<spi_no>;

  static constexpr unsigned clk_freq = (spi_no == 1 ? system_clock_type::pclk2_freq : system_clock_type::pclk1_freq );

private:

  template<unsigned value>
  struct data_size_impl {
    static_assert(value == 8 || value == 16, "invalid data_size (supported values: 8, 16)");
    using type = regval< typename SPIx::CR1::DFF,
      ( value == 8  ? 0 :
        value == 16 ? 1 : 0xff )
      >;
  };

public:  /* ------ configuration traits ------ */

  using master = reglist<
    regval< typename SPIx::CR1::MSTR, 1 >,
    regval< typename SPIx::CR1::SSI,  1 >
  >;

  using slave = regval< typename SPIx::CR1::MSTR, 0 >;

  struct clock_polarity {
    using low  = regval< typename SPIx::CR1::CPOL, 0 >;
    using high = regval< typename SPIx::CR1::CPOL, 1 >;
  };

  struct clock_phase {
    using first_edge  = regval< typename SPIx::CR1::CPHA, 0 >;
    using second_edge = regval< typename SPIx::CR1::CPHA, 1 >;
  };

  using software_slave_management = regval< typename SPIx::CR1::SSM, 1 >;

  struct data_direction {
    using two_lines_full_duplex = reglist<
      regval< typename SPIx::CR1::BIDIMODE, 0 >,
      regval< typename SPIx::CR1::BIDIOE,   0 >,
      regval< typename SPIx::CR1::RXONLY,   0 >
      >;
    using two_lines_rx_only = reglist<
      regval< typename SPIx::CR1::BIDIMODE, 0 >,
      regval< typename SPIx::CR1::BIDIOE,   0 >,
      regval< typename SPIx::CR1::RXONLY,   1 >
      >;
    using one_line_rx = reglist<
      regval< typename SPIx::CR1::BIDIMODE, 1 >,
      regval< typename SPIx::CR1::BIDIOE,   0 >,
      regval< typename SPIx::CR1::RXONLY,   0 >
      >;
    using one_line_tx = reglist<
      regval< typename SPIx::CR1::BIDIMODE, 1 >,
      regval< typename SPIx::CR1::BIDIOE,   1 >,
      regval< typename SPIx::CR1::RXONLY,   0 >
      >;
  };

  struct frame_format {
    using msb_first = regval< typename SPIx::CR1::LSBFIRST, 0 >;
    using lsb_first = regval< typename SPIx::CR1::LSBFIRST, 1 >;
  };

  /** Maximum frequency (hz, 0=maximum available) */
  template<freq_t value>
  using max_frequency = regval< typename SPIx::CR1::BR,
    ( value == 0 ? 0 :
      value >= clk_freq / 2   ? 0 :
      value >= clk_freq / 4   ? 1 :
      value >= clk_freq / 8   ? 2 :
      value >= clk_freq / 16  ? 3 :
      value >= clk_freq / 32  ? 4 :
      value >= clk_freq / 64  ? 5 :
      value >= clk_freq / 128 ? 6 :
      7 )
  >;

  /** 8 bit or 16 bit data size */
  template<unsigned value>
  using data_size = typename std::enable_if<
    value == 8 || value == 16,
    regval< typename SPIx::CR1::DFF, value == 16  ? 1 : 0 >
    >::type;

public:  /* ------ static member functions ------ */

  /**
   * Configure SPI register using Tp type traits.
   *
   * NOTE: make sure no communication is ongoing when calling this function.
   */
  template< typename... Tp >
  static void configure(void) {
    reglist< Tp... >::template strict_reset_to<
      typename SPIx::CR1
      >();
  }

  /**
   * Disable SPI, configure SPI register using Tp type traits, and enable SPI.
   *
   * NOTE: make sure no communication is ongoing when calling this function.
   */
  template< typename... Tp >
  static void reconfigure(void) {
    disable();
    /* configure and enable SPI in one register write */
    configure< typename SPIx::CR1::SPE, Tp... >();
  }

  static void reset_crc(void) {
    SPIx::CRCPR::reset();
  }

  static void enable(void) {
    SPIx::CR1::SPE::set();
  }
  static void disable(void) {
    SPIx::CR1::SPE::clear();
  }

  static void wait_transmit_empty(void) {
    while(SPIx::SR::TXE::test() == false);
  }
  static void wait_receive_not_empty(void) {
    while(SPIx::SR::RXNE::test() == false);
  }
  static void wait_not_busy(void) {
    while(SPIx::SR::BSY::test());
  }

  static void send(uint16_t data) {
    SPIx::DR::store(data);
  }

  static void send_blocking(uint16_t data) {
    wait_transmit_empty();
    send(data);
  }

  /** NOTE: return value depends on the data frame format (CR1::DFF, 8 or 16 bit) */
  static typename SPIx::DR::value_type receive(void) {
    return SPIx::DR::load();
  }

  static typename SPIx::DR::value_type receive_blocking(void) {
    wait_receive_not_empty();
    return receive();
  }

  static unsigned char writeread_blocking(uint16_t data) {
    wait_transmit_empty();
    send(data);
    wait_receive_not_empty();
    return receive();
  }
};

} // namespace mptl

#endif // ARM_CORTEX_STM32_COMMON_SPI_HPP_INCLUDED
