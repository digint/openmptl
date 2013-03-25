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

#ifndef NRF24L01_HPP_INCLUDED
#define NRF24L01_HPP_INCLUDED

#include "arch/gpio.hpp"
#include "arch/spi.hpp"

enum class NrfCommand : uint8_t {
  nop             = 0xff,
  r_rx_payload    = 0x61,
  w_tx_payload    = 0xa0,
  flush_tx        = 0xe1,
  flush_rx        = 0xe2,
  reuse_tx_pl     = 0xe3
};

enum class NrfRegister : uint8_t {
  rx_addr_p0      = 0x0a,
  rx_addr_p1      = 0x0b,
  tx_addr         = 0x10,
  rx_pw_p0        = 0x11,
  rx_pw_p1        = 0x12,
  fifo_status     = 0x17,

  max_rt          = 0x10,

  config_reg_addr = 0x00,
  status_addr     = 0x07,

  flush_tx        = 0xe1,
  tx_full         = 0x01,
  rx_dr           = 0x40,
  tx_ds           = 0x20
};

template<typename spi_type,
         typename nrf_ce,
         typename nrf_csn,  // nss
         typename nrf_irq
         >
class Nrf24l01
{
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
    return spi_master::writeread_blocking(data);
  }

  static uint8_t writeread(NrfCommand cmd) {
    return writeread(static_cast<uint8_t>(cmd));
  }

  static uint8_t read_cmd(NrfRegister reg) {
    return static_cast<uint8_t>(reg)  & 0x1f;
  }
  static uint8_t write_cmd(NrfRegister reg) {
    return (1 << 5) | (static_cast<uint8_t>(reg) & 0x1f);
  }
public:

  using spi = spi_type;

  using spi_master = SpiMaster<
    spi,
    8_mhz,  // max frequency
    8,      // 8bit data
    SpiClockPolarity::low,
    SpiClockPhase::first_edge,
    SpiDataDirection::two_lines_full_duplex,
    SpiSoftwareSlaveManagement::enabled,
    SpiFrameFormat::msb_first
    >;

  using resources = ResourceList<
    typename spi_master::resources,
    typename nrf_ce::resources,
    typename nrf_csn::resources,
    typename nrf_irq::resources
    >;

  static uint8_t read_register(NrfRegister reg) {
    uint8_t ret;

    enable_slave_select();
    writeread(read_cmd(reg));
    ret = writeread(NrfCommand::nop);
    disable_slave_select();
    return ret;
  }

  static void read_address_register(NrfRegister reg, uint8_t *ret_addr) {
    enable_slave_select();
    writeread(read_cmd(reg));
    for (int i=0; i < 5; i++) {
      ret_addr[i] = writeread(NrfCommand::nop);
    }
    disable_slave_select();
  }

  static void write_address_register(NrfRegister reg, const uint8_t data[] ) {
    if(reg == NrfRegister::rx_addr_p0 ||
       reg == NrfRegister::rx_addr_p1 ||
       reg == NrfRegister::tx_addr)
    {
      enable_slave_select();
      writeread(write_cmd(reg));
      for (int i = 0; i < 5; i++) {
        writeread(data[i]);
      }
      disable_slave_select();
    }
  }

  static uint8_t write_register(NrfRegister reg, uint8_t data) {
    uint8_t ret;

    enable_slave_select();
    ret = writeread(write_cmd(reg));
    writeread(data);
    disable_slave_select();
    return ret;
  }


  static void init(void) {
    spi_master::init();

    nrf_csn::init();
    nrf_ce::init();
    nrf_irq::init();

    nrf_csn::disable();
    nrf_ce::enable();
  }

  static void configure() {
    spi_master::configure();
  }

#if 0
  static void assign_addr(const uint8_t * rx_addr_p0, const uint8_t * rx_addr_p1, const uint8_t * tx_addr) {
    uint8_t buf[5];

    // Write CONFIG register (addres - 0x00)
    //00001010 - CRC enable, power-up, RX
    write_register(CONFIG_REG_ADDR, 0x0B);
    // read
    read_register(CONFIG_REG_ADDR);

    // Write RX_ADDR_P0 register -> Set receive address data Pipe0 -> address in RX_ADDRESS_P0 array
    //???    write_address_register(RX_ADDR_P0, rx_addr_p0);
    write_address_register(RX_ADDR_P0, rx_addr_p0);
    // read
    read_address_register(RX_ADDR_P0, buf);  // TODO: compare

    // Write RX_ADDR_P1 register -> Set receive address data Pipe1 -> address in RX_ADDRESS_P1 array
    write_address_register(RX_ADDR_P1, rx_addr_p1);
    // read
    read_address_register(RX_ADDR_P1, buf);

    // Write TX_ADDR register -> Transmit address. Used for a PTX device only. Address in TX_ADDRESS array
    write_address_register(TX_ADDR, tx_addr);
    // read
    read_address_register(TX_ADDR, buf);

    // Write RX_PW_P0 register -> Set number of bytes in RX payload in data pipe0 -> 1 byte
    write_register(RX_PW_P0, 1);
    // read
    read_register(RX_PW_P0);

    // Write RX_PW_P1 register -> Set number of bytes in RX payload in data pipe1 -> 1 byte
    write_register(RX_PW_P1, 1);
    // read
    read_register(RX_PW_P1);

  }
#endif
};

#endif // NRF24L01_HPP_INCLUDED
