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

#include "kernel.hpp"
#include "terminal_hooks.hpp"

namespace terminal_hooks
{
  void NrfTest::run(poorman_ostream<char> & ostream) {
    unsigned char c;
    unsigned char nrf_buf[5];

    using nrf = Kernel::nrf;

    nrf::configure();

    // nrf::assign_addr();

    c = nrf::read_register(STATUS_ADDR);
    ostream << "status=0x" << c << endl;

    nrf::write_register(CONFIG_REG_ADDR, 0x0B);
    c = nrf::read_register(CONFIG_REG_ADDR);
    ostream << "c=0x" << c << endl;
    nrf_buf[0] = 5;
    nrf_buf[1] = 6;
    nrf_buf[2] = 7;
    nrf_buf[3] = 8;
    nrf_buf[4] = 9;
    nrf::write_address_register(RX_ADDR_P0, nrf_buf);

    nrf_buf[0] = 0;
    nrf_buf[1] = 0;
    nrf_buf[2] = 0;
    nrf_buf[3] = 0;
    nrf_buf[4] = 0;
    nrf::read_address_register(RX_ADDR_P0, nrf_buf);
    ostream << "rx_addr_p0=0x" << nrf_buf[0] << nrf_buf[1] << nrf_buf[2] << nrf_buf[3] << nrf_buf[4] << endl;
  }
}
