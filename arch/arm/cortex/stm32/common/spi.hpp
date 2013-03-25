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

#ifndef STM32_COMMON_SPI_HPP_INCLUDED
#define STM32_COMMON_SPI_HPP_INCLUDED

#include <arch/rcc.hpp>
#include <arch/nvic.hpp>
#include <arch/reg/spi.hpp>

enum class SpiClockPolarity {
  low,
  high
};
enum class SpiMasterSelection {
  master,
  slave
};
enum class SpiClockPhase {
  first_edge,
  second_edge
};
enum class SpiSoftwareSlaveManagement {
  enabled,
  disabled
};
enum class SpiDataDirection {
  two_lines_full_duplex,
  two_lines_rx_only,
  one_line_rx,
  one_line_tx
};
enum class SpiFrameFormat {
  msb_first,
  lsb_first
};


template<typename  rcc_type,
         unsigned _spi_no>
class Spi
{
  using SPIx = reg::SPI<_spi_no>;
  using rcc = rcc_type;

public:

  static constexpr unsigned spi_no = _spi_no;
  using resources = Rcc_spi_clock_resources<spi_no>;
  using Irq = irq::SPI<spi_no>;

  static constexpr unsigned freq = (spi_no == 1 ? rcc::pclk2_freq : rcc::pclk1_freq );

  static unsigned baud_rate_prescaler(freq_t max_frequency) {
    return ( max_frequency == 0          ?   2 :
             max_frequency >= freq / 2   ?   2 :
             max_frequency >= freq / 4   ?   4 :
             max_frequency >= freq / 8   ?   8 :
             max_frequency >= freq / 16  ?  16 :
             max_frequency >= freq / 32  ?  32 :
             max_frequency >= freq / 64  ?  64 :
             max_frequency >= freq / 128 ? 128 :
             256 );
  }

  static void configure(SpiMasterSelection         master_selection,
                        freq_t                     max_frequency,   /* Hz, 0=maximum available */
                        unsigned                   data_size,
                        SpiClockPolarity           clk_pol,
                        SpiClockPhase              clk_phase,
                        SpiDataDirection           data_dir,
                        SpiSoftwareSlaveManagement ssm,
                        SpiFrameFormat             frame_format)
  {
    auto cr1 = SPIx::CR1::load();

    /* clear all bits except SPE, CRCNEXT, CRCEN (stm32 does strange things when these are all set together) */
    cr1 &= SPIx::CR1::SPE::value | SPIx::CR1::CRCNEXT::value | SPIx::CR1::CRCEN::value;

    if(data_dir == SpiDataDirection::two_lines_rx_only)
      cr1 |= SPIx::CR1::RXONLY::value;
    else if(data_dir == SpiDataDirection::one_line_rx)
      cr1 |= SPIx::CR1::BIDIMODE::value;
    else if(data_dir == SpiDataDirection::one_line_tx)
      cr1 |= SPIx::CR1::BIDIMODE::value | SPIx::CR1::BIDIOE::value;

    if(master_selection == SpiMasterSelection::master)
      cr1 |= SPIx::CR1::MSTR::value | 
        //        (ssm == SpiSoftwareSlaveManagement::enabled ? SPIx::CR1::SSI::value : 0); // SPI Master: Master Selection / Internal slave select
        SPIx::CR1::SSI::value; // SPI Master: Master Selection / Internal slave select
    if(data_size == 16)
      cr1 |= SPIx::CR1::DFF::value;
    if(clk_pol == SpiClockPolarity::high)
      cr1 |= SPIx::CR1::CPOL::value;
    if(clk_phase == SpiClockPhase::second_edge)
      cr1 |= SPIx::CR1::CPHA::value;
    if(ssm == SpiSoftwareSlaveManagement::enabled)
      cr1 |= SPIx::CR1::SSM::value;
    if(frame_format == SpiFrameFormat::lsb_first)
      cr1 |= SPIx::CR1::LSBFIRST::value;
      
    switch(baud_rate_prescaler(max_frequency)) {
    case 2:   cr1 |= reg::RegisterConst< typename SPIx::CR1::BR, 0 >::value; break;
    case 4:   cr1 |= reg::RegisterConst< typename SPIx::CR1::BR, 1 >::value; break;
    case 8:   cr1 |= reg::RegisterConst< typename SPIx::CR1::BR, 2 >::value; break;
    case 16:  cr1 |= reg::RegisterConst< typename SPIx::CR1::BR, 3 >::value; break;
    case 32:  cr1 |= reg::RegisterConst< typename SPIx::CR1::BR, 4 >::value; break;
    case 64:  cr1 |= reg::RegisterConst< typename SPIx::CR1::BR, 5 >::value; break;
    case 128: cr1 |= reg::RegisterConst< typename SPIx::CR1::BR, 6 >::value; break;
    case 256: cr1 |= reg::RegisterConst< typename SPIx::CR1::BR, 7 >::value; break;
    }

    SPIx::CR1::store(cr1);
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
    while(SPIx::SR::TXE::test() == 0);
  }
  static void wait_receive_not_empty(void) {
    while(SPIx::SR::RXNE::test() == 0);
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

  // NOTE: return value depends on the data frame format (DFF) bit in CR1 (8bit or 16bit)
  static uint16_t receive(void) {
    return SPIx::DR::load();
  }

  static uint16_t receive_blocking(void) {
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



template<typename spi,
         freq_t max_frequency = 0, /* Hz, 0=maximum available */
         unsigned data_size = 8,
         SpiClockPolarity clk_pol = SpiClockPolarity::low,
         SpiClockPhase clk_phase = SpiClockPhase::first_edge,
         SpiDataDirection data_dir = SpiDataDirection::two_lines_full_duplex,
         SpiSoftwareSlaveManagement ssm = SpiSoftwareSlaveManagement::disabled,
         SpiFrameFormat frame_format = SpiFrameFormat::msb_first
         >
class SpiMaster : public spi {

  static_assert(data_size == 8 || data_size == 16, "Invalid data size");

public:

  using resources = typename spi::resources;

  /**
   * Configure SPI.
   * NOTE: make sure no communication is ongoing when calling this function.
   */
  static void configure(void) {
    spi::configure(SpiMasterSelection::master, max_frequency, data_size, clk_pol, clk_phase, data_dir, ssm, frame_format);
  }

  /**
   * Reconfigure and enable SPI.
   * NOTE: make sure no communication is ongoing when calling this function.
   */
  static void reconfigure(void) {
    spi::disable();
    configure();
    spi::enable();
  }
};

#endif // STM32_COMMON_SPI_HPP_INCLUDED
