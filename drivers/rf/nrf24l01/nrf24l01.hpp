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

//#include <initializer_list>
//#include <algorithm>  // for_each


// Commands
#define   NOP             0xff
#define   R_RX_PAYLOAD    0x61
#define   W_TX_PAYLOAD    0xA0
#define   FLUSH_TX        0xE1
#define   FLUSH_RX        0xE2
#define   REUSE_TX_PL     0xE3

// Register
#define   RX_ADDR_P0      0x0A
#define   RX_ADDR_P1      0x0B
#define   TX_ADDR         0x10
#define   RX_PW_P0        0x11
#define   RX_PW_P1        0x12
#define   FIFO_STATUS     0x17

#define   MAX_RT          0x10

#define   CONFIG_REG_ADDR 0x00
#define   STATUS_ADDR     0x07

#define   FLUSH_TX        0xE1
#define   TX_FULL         0x01
#define   RX_DR           0x40
#define   TX_DS           0x20

template<typename spi_type,
         typename nrf_ce,
         typename nrf_csn,  // nss
         typename nrf_irq
         >
class Nrf24l01
{

  // Enable slave select
  static void enable(void) {
    nrf_csn::enable();
  }
  // Disable slave select
  static void disable(void) {
    nrf_csn::disable();
  }

public:

  using spi = spi_type;

  using spi_master = SpiMaster<
    spi,
    8_mhz,  // max frequency
    8,      // 8bit data
    SpiClockPolarity::low
    >;

  using resources = ResourceList<
    typename spi_master::resources,
    typename nrf_ce::resources,
    typename nrf_csn::resources,
    typename nrf_irq::resources
    >;


  static constexpr unsigned numof_nops = 10;  // Tcwh: CSN Inactive time: min. 50ns   // TODO: measurements, nanosleep

  // TODO: assert 5 bit address
  static unsigned char read_register(unsigned char addr) {
    unsigned char ret;
    unsigned char command = addr;

    // Tcwh: CSN Inactive time: min. 50ns
    // NOTE: this goes before ALL enable(). Time between calls to disable() -> enable().
    CoreFunctions::nop(numof_nops);

    enable();
    //  Tcc: CSN to SCK Setup: 2ns

    spi_master::send_byte(command);
    ret = spi_master::send_byte(NOP);
    // Tcch: SCK to CSN Hold: 2ns
    disable();
    return ret;
  }

  static void read_address_register(unsigned char addr, unsigned char *ret_addr) {
    unsigned char command = (0 << 5) | addr;

    CoreFunctions::nop(numof_nops);
    enable();

    spi_master::send_byte(command);
    for (int i=0; i < 5; i++) {
      ret_addr[i] = spi_master::send_byte(NOP);
    }
    disable();
  }

  //  static void writeAddressRegister(unsigned char addr, std::initializer_list<unsigned char> data) {
  static void write_address_register(unsigned char addr, const unsigned char data[] ) {
    unsigned char command = (1 << 5) | addr;

    CoreFunctions::nop(numof_nops);
    enable();

    if (addr == RX_ADDR_P0 || addr == RX_ADDR_P1 || addr == TX_ADDR) {
      spi_master::send_byte(command);

#if 0 // this is how initializer_lists are handled
      std::for_each(data.begin(),
                    data.end(),
                    [](unsigned char c){ spi_master::send_byte(c); } );
#else
      for (int i = 0; i < 5; i++) {
        spi_master::send_byte(data[i]);
      }
#endif
    }
    disable();
  }

  static unsigned char write_register(unsigned char addr, unsigned char data) {
    unsigned char ret;
    unsigned char command = (1 << 5) | addr;

    CoreFunctions::nop(numof_nops);
    enable();

    ret = spi_master::send_byte(command);
    spi_master::send_byte(data);
    disable();
    return ret;
  }


  static void init(void) {

    spi_master::init();

    nrf_csn::init();
    nrf_ce::init();
    nrf_irq::init();

    nrf_csn::disable();

    // TODO: maybe move this to separate commands (enable/disable)
    nrf_ce::enable();
  }

  static void configure() {
    spi_master::configure();
  }

#if 0
  static void assign_addr(const unsigned char * rx_addr_p0, const unsigned char * rx_addr_p1, const unsigned char * tx_addr) {
    unsigned char buf[5];

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
