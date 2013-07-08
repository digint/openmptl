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

#ifndef NRF24L01_HPP_INCLUDED
#define NRF24L01_HPP_INCLUDED

#include <arch/gpio.hpp>
#include <arch/spi.hpp>
#include <peripheral_device.hpp>

namespace mptl { namespace device {

template<typename spi_type,
         typename nrf_ce,
         typename nrf_csn,  // nss
         typename nrf_irq
         >
class nrf24l01
{
public:

  enum class dev_command : uint8_t {
    r_rx_payload = 0x61,
    w_tx_payload = 0xa0,
    flush_tx     = 0xe1,
    flush_rx     = 0xe2,
    reuse_tx_pl  = 0xe3,
    activate     = 0x50,
    nop          = 0xff
  };

  enum class dev_register : uint8_t {
    config       = 0x00,
    en_aa        = 0x01,
    en_rxaddr    = 0x02,
    setup_aw     = 0x03,
    setup_retr   = 0x04,
    rf_ch        = 0x05,
    rf_setup     = 0x06,
    status       = 0x07,
    observe_tx   = 0x08, 
    cd           = 0x09,
    rx_addr_p0   = 0x0a,
    rx_addr_p1   = 0x0b,
    rx_addr_p2   = 0x0c,
    rx_addr_p3   = 0x0d,
    rx_addr_p4   = 0x0e,
    rx_addr_p5   = 0x0f,
    tx_addr      = 0x10,
    rx_pw_p0     = 0x11,
    rx_pw_p1     = 0x12,
    rx_pw_p2     = 0x13,
    rx_pw_p3     = 0x14,
    rx_pw_p4     = 0x15,
    rx_pw_p5     = 0x16,
    fifo_status  = 0x17,
    dynpd        = 0x1c,
    feature      = 0x1d
  };

  struct dev_address {
    uint8_t buf[5];

    dev_address(void) {
      buf[0] = 0; buf[1] = 0; buf[2] = 0; buf[3] = 0; buf[4] = 0;
    }
    dev_address(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4, uint8_t p5) {
      buf[0] = p1; buf[1] = p2; buf[2] = p3; buf[3] = p4; buf[4] = p5;
    }
  };

private:

  struct spi_device_config
  {
    static constexpr mptl::cfg::spi::master_selection          master_selection = mptl::cfg::spi::master_selection::master;
    static constexpr mptl::freq_t                              max_frequency    = mptl::mhz(8);
    static constexpr unsigned                                  data_size        = 8;
    static constexpr mptl::cfg::spi::clock_polarity            clk_pol          = mptl::cfg::spi::clock_polarity::low;
    static constexpr mptl::cfg::spi::clock_phase               clk_phase        = mptl::cfg::spi::clock_phase::first_edge;
    static constexpr mptl::cfg::spi::data_direction            data_dir         = mptl::cfg::spi::data_direction::two_lines_full_duplex;
    static constexpr mptl::cfg::spi::software_slave_management ssm              = mptl::cfg::spi::software_slave_management::enabled;
    static constexpr mptl::cfg::spi::frame_format              frame_format     = mptl::cfg::spi::frame_format::msb_first;
  };

  using spi_device = mptl::peripheral_device< spi_type, spi_device_config >;

  /* Tcwh: CSN Inactive time: min. 50ns */
  /* Time between calls of disable() -> enable() */
  static void wait_tcwh(void) {
    // Core::nop(10);
  }

  /* Tcc: CSN to SCK Setup: 2ns */
  static void wait_tcc(void) {
    // Core::nop(2);
  }

  /* Tcch: SCK to CSN Hold: 2ns */
  static void wait_tcch(void) {
    // Core::nop(2);
  }

  static void enable_slave_select(void) {
    wait_tcwh();
    nrf_csn::enable();
    wait_tcc();
  }
  static void disable_slave_select(void) {
    wait_tcch();
    nrf_csn::disable();
  }

  static uint8_t writeread(uint8_t data) {
    return spi_device::writeread_blocking(data);
  }

  static uint8_t writeread(dev_command cmd) {
    return writeread(static_cast<uint8_t>(cmd));
  }

  static uint8_t read_cmd(dev_register reg) {
    return static_cast<uint8_t>(reg) & 0x1f;
  }
  static uint8_t write_cmd(dev_register reg) {
    return (1 << 5) | (static_cast<uint8_t>(reg) & 0x1f);
  }
public:

  using resources = mptl::resource::list<
    typename spi_device::resources,
    typename nrf_ce::resources,
    typename nrf_csn::resources,
    typename nrf_irq::resources
    >;

  static uint8_t read_register(dev_register reg) {
    uint8_t ret;

    enable_slave_select();
    writeread(read_cmd(reg));
    ret = writeread(dev_command::nop);
    disable_slave_select();
    return ret;
  }

  static void read_address_register(dev_register reg, dev_address & ret_addr) {
    enable_slave_select();
    writeread(read_cmd(reg));
    for (int i=0; i < 5; i++) {
      ret_addr.buf[i] = writeread(dev_command::nop);
    }
    disable_slave_select();
  }

  static void write_address_register(dev_register reg, dev_address const & addr) {
    if(reg == dev_register::rx_addr_p0 ||
       reg == dev_register::rx_addr_p1 ||
       reg == dev_register::tx_addr)
    {
      enable_slave_select();
      writeread(write_cmd(reg));
      for (int i = 0; i < 5; i++) {
        writeread(addr.buf[i]);
      }
      disable_slave_select();
    }
  }

  static uint8_t write_register(dev_register reg, uint8_t data) {
    uint8_t ret;

    enable_slave_select();
    ret = writeread(write_cmd(reg));
    writeread(data);
    disable_slave_select();
    return ret;
  }


  static void init(void) {
    nrf_csn::disable();
    nrf_ce::enable();
  }

  static void enable() {
    spi_device().reconfigure();
  }
};

} } // namespace mptl::device

#endif // NRF24L01_HPP_INCLUDED
