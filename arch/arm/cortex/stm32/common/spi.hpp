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

namespace mptl {

namespace cfg { namespace spi {

enum class master_selection {
  master,
  slave
};
enum class clock_polarity {
  low,
  high
};
enum class clock_phase {
  first_edge,
  second_edge
};
enum class software_slave_management {
  enabled,
  disabled
};
enum class data_direction {
  two_lines_full_duplex,
  two_lines_rx_only,
  one_line_rx,
  one_line_tx
};
enum class frame_format {
  msb_first,
  lsb_first
};

struct preset
{
  static constexpr cfg::spi::master_selection          master_selection = cfg::spi::master_selection::slave;
  static constexpr freq_t                              max_frequency    = 0;  /* Hz, 0=maximum available */
  static constexpr unsigned                            data_size        = 8;  /* 8 bit or 16 bit data size */
  static constexpr cfg::spi::clock_polarity            clk_pol          = cfg::spi::clock_polarity::low;
  static constexpr cfg::spi::clock_phase               clk_phase        = cfg::spi::clock_phase::first_edge;
  static constexpr cfg::spi::data_direction            data_dir         = cfg::spi::data_direction::two_lines_full_duplex;
  static constexpr cfg::spi::software_slave_management ssm              = cfg::spi::software_slave_management::disabled;
  static constexpr cfg::spi::frame_format              frame_format     = cfg::spi::frame_format::msb_first;
};

} } // namespace cfg::spi


////////////////////  spi  ////////////////////


template<typename  rcc_type,
         unsigned _spi_no>
class spi
{
protected:
  using SPIx = reg::SPI<_spi_no>;
  using rcc = rcc_type;

  static constexpr unsigned clk_freq = (_spi_no == 1 ? rcc::pclk2_freq : rcc::pclk1_freq );

  static constexpr typename SPIx::CR1::value_type baud_rate_prescaler_cr1_set_mask(freq_t max_frequency)
  {
    return ( max_frequency == 0              ? SPIx::CR1::BR::value_from(0) :
             max_frequency >= clk_freq / 2   ? SPIx::CR1::BR::value_from(0) :
             max_frequency >= clk_freq / 4   ? SPIx::CR1::BR::value_from(1) :
             max_frequency >= clk_freq / 8   ? SPIx::CR1::BR::value_from(2) :
             max_frequency >= clk_freq / 16  ? SPIx::CR1::BR::value_from(3) :
             max_frequency >= clk_freq / 32  ? SPIx::CR1::BR::value_from(4) :
             max_frequency >= clk_freq / 64  ? SPIx::CR1::BR::value_from(5) :
             max_frequency >= clk_freq / 128 ? SPIx::CR1::BR::value_from(6) :
             SPIx::CR1::BR::value_from(7) );
  }

  template<typename spi_config_type>
  static constexpr typename SPIx::CR1::value_type cr1_set_mask(spi_config_type const & cfg)
  {
    // assert(cfg.data_size == 8 || cfg.data_size == 16);
    return
      ((cfg.master_selection == cfg::spi::master_selection::master) ? (SPIx::CR1::MSTR::value | SPIx::CR1::SSI::value) : 0)

      | ((cfg.data_dir == cfg::spi::data_direction::two_lines_rx_only) ? SPIx::CR1::RXONLY::value   :
         (cfg.data_dir == cfg::spi::data_direction::one_line_rx)       ? SPIx::CR1::BIDIMODE::value :
         (cfg.data_dir == cfg::spi::data_direction::one_line_tx)       ? (SPIx::CR1::BIDIMODE::value | SPIx::CR1::BIDIOE::value) : 0)

      | ((cfg.data_size    == 16)                                 ? SPIx::CR1::DFF::value      : 0)
      | ((cfg.clk_pol      == cfg::spi::clock_polarity::high)     ? SPIx::CR1::CPOL::value     : 0)
      | ((cfg.clk_phase    == cfg::spi::clock_phase::second_edge) ? SPIx::CR1::CPHA::value     : 0)
      | ((cfg.ssm          == cfg::spi::software_slave_management::enabled) ? SPIx::CR1::SSM::value : 0)
      | ((cfg.frame_format == cfg::spi::frame_format::lsb_first)  ? SPIx::CR1::LSBFIRST::value : 0)
      
      | baud_rate_prescaler_cr1_set_mask(cfg.max_frequency)
      ;
  }

public:

  static constexpr unsigned spi_no = _spi_no;
  using resources = rcc_spi_clock_resources<spi_no>;
  using irq = irq::spi<spi_no>;

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

  template<typename spi_config_type>
  static void configure(spi_config_type const & cfg)
  {
    auto cr1 = SPIx::CR1::load();
    /* clear all bits except SPE, CRCNEXT, CRCEN (stm32 does strange things when these are all set together) */
    cr1 &= SPIx::CR1::SPE::value | SPIx::CR1::CRCNEXT::value | SPIx::CR1::CRCEN::value;
    cr1 |= cr1_set_mask(cfg);
    SPIx::CR1::store(cr1);
  }

#if 0
  static void configure(cfg::spi::master_selection          master_selection,
                        freq_t                              max_frequency,   /* Hz, 0=maximum available */
                        unsigned                            data_size,
                        cfg::spi::clock_polarity            clk_pol,
                        cfg::spi::clock_phase               clk_phase,
                        cfg::spi::data_direction            data_dir,
                        cfg::spi::software_slave_management ssm,
                        cfg::spi::frame_format              frame_format)
  {
    // assert(data_size == 8 || data_size == 16, "invalid data size");

    auto cr1 = SPIx::CR1::load();

    /* clear all bits except SPE, CRCNEXT, CRCEN (stm32 does strange things when these are all set together) */
    cr1 &= SPIx::CR1::SPE::value | SPIx::CR1::CRCNEXT::value | SPIx::CR1::CRCEN::value;

    if(master_selection == cfg::spi::master_selection::master)
      cr1 |= SPIx::CR1::MSTR::value | SPIx::CR1::SSI::value; // SPI Master: Master Selection / Internal slave select
    // if(ssm == cfg::spi::software_slave_management::enabled) cr1 |= SPIx::CR1::SSI::value; // SPI Master: Master Selection / Internal slave select

    if(data_dir == cfg::spi::data_direction::two_lines_rx_only)
      cr1 |= SPIx::CR1::RXONLY::value;
    else if(data_dir == cfg::spi::data_direction::one_line_rx)
      cr1 |= SPIx::CR1::BIDIMODE::value;
    else if(data_dir == cfg::spi::data_direction::one_line_tx)
      cr1 |= SPIx::CR1::BIDIMODE::value | SPIx::CR1::BIDIOE::value;

    if(data_size == 16)
      cr1 |= SPIx::CR1::DFF::value;
    if(clk_pol == cfg::spi::clock_polarity::high)
      cr1 |= SPIx::CR1::CPOL::value;
    if(clk_phase == cfg::spi::clock_phase::second_edge)
      cr1 |= SPIx::CR1::CPHA::value;
    if(ssm == cfg::spi::software_slave_management::enabled)
      cr1 |= SPIx::CR1::SSM::value;
    if(frame_format == cfg::spi::frame_format::lsb_first)
      cr1 |= SPIx::CR1::LSBFIRST::value;
      
    cr1 |= baud_rate_prescaler_cr1_set_mask(max_frequency);

    SPIx::CR1::store(cr1);
  }
#endif
};

} // namespace mptl

#endif // ARM_CORTEX_STM32_COMMON_SPI_HPP_INCLUDED
