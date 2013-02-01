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

#ifndef SPI_HPP_INCLUDED
#define SPI_HPP_INCLUDED

#include <arch/core.hpp>

namespace cSpi
{
  enum class ClockPolarity {
    low,
    high
  };
  enum class MasterSelection {
    master,
    slave
  };
  enum class ClockPhase {
    first_edge,
    second_edge
  };
  enum class SoftwareSlaveManagement {
    enabled,
    disabled
  };
  enum class DataDirection {
    two_lines_full_duplex,
    two_lines_rx_only,
    one_line_rx,
    one_line_tx
  };
}


template<unsigned spi_no>
class Spi  // TODO: rename -> spi_resource (EVERYWHERE!)
{
  using SPIx = reg::SPI<spi_no>;

public:

  static constexpr uint32_t apb2enr = (spi_no == 1 ? reg::RCC::APB2ENR::SPI1EN::value : 0);
  static constexpr uint32_t apb1enr = (
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
                                       spi_no == 2 ? reg::RCC::APB1ENR::SPI2EN::value : 
#endif
#if defined (STM32F10X_HD) || defined  (STM32F10X_CL)
                                       spi_no == 3 ? reg::RCC::APB1ENR::SPI3EN::value :
#endif
                                       0);

  typedef ResourceList< SharedRegister< reg::RCC::APB1ENR, apb1enr >,
                        SharedRegister< reg::RCC::APB2ENR, apb2enr >
                        > resources;

  template<cSpi::MasterSelection master_selection = cSpi::MasterSelection::master,
           unsigned baud_rate_prescaler = 2,  // TODO: use baud-rate here, calculate correct presscaler below
           unsigned data_size = 8,
           cSpi::ClockPolarity clk_pol = cSpi::ClockPolarity::high,
           cSpi::ClockPhase clk_phase = cSpi::ClockPhase::first_edge,
           cSpi::DataDirection data_dir = cSpi::DataDirection::two_lines_full_duplex,
           cSpi::SoftwareSlaveManagement ssm = cSpi::SoftwareSlaveManagement::enabled
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

    if(data_dir == cSpi::DataDirection::two_lines_rx_only)
      cr1 |= SPIx::CR1::RXONLY::value;
    else if(data_dir == cSpi::DataDirection::one_line_rx)
      cr1 |= SPIx::CR1::BIDIMODE::value;
    else if(data_dir == cSpi::DataDirection::one_line_tx)
      cr1 |= SPIx::CR1::BIDIMODE::value | SPIx::CR1::BIDIOE::value;

    if(master_selection == cSpi::MasterSelection::master)
      cr1 |= SPIx::CR1::MSTR::value | SPIx::CR1::SSI::value; // SPI Master: Master Selection / Internal slave select
    if(data_size == 16)
      cr1 |= SPIx::CR1::DFF::value; // Data Size 16bit
    if(clk_pol == cSpi::ClockPolarity::high)
      cr1 |= SPIx::CR1::CPOL::value; // Clock polarity = high
    if(clk_phase == cSpi::ClockPhase::second_edge)
      cr1 |= SPIx::CR1::CPHA::value; // Clock phase = edge
    if(ssm == cSpi::SoftwareSlaveManagement::enabled)
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
  static void resetCRC(void) {
    SPIx::CRCPR::reset();
  }

  static void enable(void) {
    SPIx::CR1::SPE::set();
  }
  static void disable(void) {
    SPIx::CR1::SPE::clear();
  }

  static void waitTransmitEmpty(void) {
    while(SPIx::SR::TXE::test() == 0);
  }
  static void waitReceiveNotEmpty(void) {
    while(SPIx::SR::RXNE::test() == 0);
  }
  static void waitNotBusy(void) {
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



template<unsigned spi_no,
         freq_t max_frequency = 0, /* Hz, 0=maximum available */
         unsigned data_size = 8,
         cSpi::ClockPolarity clk_pol = cSpi::ClockPolarity::high,
         cSpi::ClockPhase clk_phase = cSpi::ClockPhase::first_edge,
         cSpi::DataDirection data_dir = cSpi::DataDirection::two_lines_full_duplex,
         cSpi::SoftwareSlaveManagement ssm = cSpi::SoftwareSlaveManagement::enabled
         >
class SpiMaster : public Spi<spi_no> {

  typedef Spi<spi_no> base;  // TODO: rename "spi"

private:

  // TODO: ugly hardcoded! get this out of the class!
  typedef GpioOutput<'A', 5, cGpio::OutputConfig::alt_push_pull> spi_sck;
  typedef GpioOutput<'A', 6, cGpio::OutputConfig::alt_push_pull> spi_miso;
  typedef GpioOutput<'A', 7, cGpio::OutputConfig::alt_push_pull> spi_mosi;

public:

  typedef ResourceList< typename base::resources,
                        typename spi_sck::resources,
                        typename spi_miso::resources,
                        typename spi_mosi::resources
                        > resources;

  /* TODO: check if these calculations are correct (not sure if we use pclk2 for SPI1) */
  static constexpr unsigned freq = (spi_no == 1 ?
                                    Rcc::ClockFrequency<Core::clock_frequency>::pclk2 : 
                                    Rcc::ClockFrequency<Core::clock_frequency>::pclk1 );

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
    //    base::waitTransmitEmpty();
    //    base::waitNotBusy();
    base::disable();  // TODO: this is actually only needed for a "reconfigure()"
    base::template configure<cSpi::MasterSelection::master, baud_rate_prescaler, data_size, clk_pol, clk_phase, data_dir, ssm>();
    base::enable();   // TODO: this is actually only needed for a "reconfigure()"
  }

  static void init(void) {
    // Configure SPI1 pins: NSS, SCK, MISO and MOSI
    spi_sck::init();
    spi_miso::init();
    spi_mosi::init();
    base::resetCRC();
    configure();
    //    base::enable();
  }

  static unsigned char sendByte(unsigned char data) {
    base::waitTransmitEmpty();
    base::send(data);	
    base::waitReceiveNotEmpty();
    return base::receive();
  }
};

#endif // SPI_HPP_INCLUDED
