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
#include <arch/reg/spi.hpp>
#include <arch/core.hpp>

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


template<typename    rcc_type,
         std::size_t _spi_no>
class Spi
{
  using SPIx = reg::SPI<_spi_no>;

public:
  using rcc = rcc_type;

  static constexpr std::size_t spi_no = _spi_no;

  using resources = Rcc_spi_clock_resources<spi_no>;

  template<SpiMasterSelection master_selection = SpiMasterSelection::master,
           unsigned baud_rate_prescaler = 2,  // TODO: use baud-rate here, calculate correct presscaler below
           unsigned data_size = 8,
           SpiClockPolarity clk_pol = SpiClockPolarity::high,
           SpiClockPhase clk_phase = SpiClockPhase::first_edge,
           SpiDataDirection data_dir = SpiDataDirection::two_lines_full_duplex,
           SpiSoftwareSlaveManagement ssm = SpiSoftwareSlaveManagement::enabled
           >
  static void configure(void) {  // TODO: this goes to spi_register::configure() (spi_resource inherits from spi_register)
    static_assert(baud_rate_prescaler == 2 ||
                  baud_rate_prescaler == 4 ||
                  baud_rate_prescaler == 8 ||
                  baud_rate_prescaler == 16 ||
                  baud_rate_prescaler == 32 ||
                  baud_rate_prescaler == 64 ||
                  baud_rate_prescaler == 128 ||
                  baud_rate_prescaler == 256
                  , "Invalid baud rate prescaler");
    static_assert(data_size == 8 || data_size == 16, "Invalid data size");

    auto cr1 = SPIx::CR1::load(); // TODO: check if we set all bits here. if yes, no load is required.

    /* Clear BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL and CPHA bits */
    cr1 &= SPIx::CR1::SPE::value | SPIx::CR1::CRCNEXT::value | SPIx::CR1::CRCEN::value;

    if(data_dir == SpiDataDirection::two_lines_rx_only)
      cr1 |= SPIx::CR1::RXONLY::value;
    else if(data_dir == SpiDataDirection::one_line_rx)
      cr1 |= SPIx::CR1::BIDIMODE::value;
    else if(data_dir == SpiDataDirection::one_line_tx)
      cr1 |= SPIx::CR1::BIDIMODE::value | SPIx::CR1::BIDIOE::value;

    if(master_selection == SpiMasterSelection::master)
      cr1 |= SPIx::CR1::MSTR::value | SPIx::CR1::SSI::value; // SPI Master: Master Selection / Internal slave select
    if(data_size == 16)
      cr1 |= SPIx::CR1::DFF::value; // Data Size 16bit
    if(clk_pol == SpiClockPolarity::high)
      cr1 |= SPIx::CR1::CPOL::value; // Clock polarity = high
    if(clk_phase == SpiClockPhase::second_edge)
      cr1 |= SPIx::CR1::CPHA::value; // Clock phase = edge
    if(ssm == SpiSoftwareSlaveManagement::enabled)
      cr1 |= SPIx::CR1::SSM::value;  // Software slave management

#if 1
    cr1 |= SPIx::CR1::BR::template Prescaler<baud_rate_prescaler>::value;
    // TODO: find out why the code gets smaller when using this with -Os:
    // cr1 |= SPIx::CR1::BR::_prescaler_value(baud_rate_prescaler);
#else
    switch(baud_rate_prescaler) {
    case 2:   cr1 |= 0x0000; break;
    case 4:   cr1 |= 0x0008; break;
    case 8:   cr1 |= 0x0010; break;
    case 16:  cr1 |= 0x0018; break;
    case 32:  cr1 |= 0x0020; break;
    case 64:  cr1 |= 0x0028; break;
    case 128: cr1 |= 0x0030; break;
    case 256: cr1 |= 0x0038; break;
    }
#endif
    cr1 |= 0; // First Bit MSB

    SPIx::CR1::store(cr1);
  }

  // reset CRC Polynomial
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
    while(SPIx::SR::BSY::test() == 1);
  }

  static void send(uint16_t data) {
    SPIx::DR::store(data);
  }

  // TODO: return value depends on the data frame format (DFF) bit in CR1 (8bit or 16bit)
  static uint16_t receive(void) {
    return SPIx::DR::load();
  }
};



template<typename spi_type,
         freq_t max_frequency = 0, /* Hz, 0=maximum available */
         unsigned data_size = 8,
         SpiClockPolarity clk_pol = SpiClockPolarity::high,
         SpiClockPhase clk_phase = SpiClockPhase::first_edge,
         SpiDataDirection data_dir = SpiDataDirection::two_lines_full_duplex,
         SpiSoftwareSlaveManagement ssm = SpiSoftwareSlaveManagement::enabled
         >
class SpiMaster : public spi_type {

  using spi = spi_type;

public:

  using rcc = typename spi_type::rcc;

  using resources = typename spi::resources;

  /* TODO: check if these calculations are correct (not sure if we use pclk2 for SPI1 on all of stm32) */
  static constexpr unsigned freq = (spi::spi_no == 1 ? rcc::pclk2_freq : rcc::pclk1_freq );

  static constexpr unsigned baud_rate_prescaler =
    max_frequency == 0          ?   2 :
    max_frequency >= freq / 2   ?   2 :
    max_frequency >= freq / 4   ?   4 :
    max_frequency >= freq / 8   ?   8 :
    max_frequency >= freq / 16  ?  16 :
    max_frequency >= freq / 32  ?  32 :
    max_frequency >= freq / 64  ?  64 :
    max_frequency >= freq / 128 ? 128 :
    256;

  static void configure(void) {
    // spi::wait_transmit_empty();
    // spi::wait_not_busy();
    spi::disable();  // TODO: this is actually only needed for a "reconfigure()"
    spi::template configure<SpiMasterSelection::master, baud_rate_prescaler, data_size, clk_pol, clk_phase, data_dir, ssm>();
    spi::enable();   // TODO: this is actually only needed for a "reconfigure()"
  }

  static void init(void) {
    /* Configure SPI1 pins: NSS, SCK, MISO and MOSI */
    // spi_sck::init();
    // spi_miso::init();
    // spi_mosi::init();
    spi::reset_crc();
    configure();
    // spi::enable();
  }

  static unsigned char send_byte(unsigned char data) {
    spi::wait_transmit_empty();
    spi::send(data);
    spi::wait_receive_not_empty();
    return spi::receive();
  }
};

#endif // STM32_COMMON_SPI_HPP_INCLUDED
