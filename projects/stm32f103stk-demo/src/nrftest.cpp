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

#include "terminal_hooks.hpp"
#include "resources.hpp"

namespace terminal_hooks
{
  void NrfTest::run(Terminal & term) {
    unsigned char c;
    unsigned char nrf_buf[5];

    using namespace resources;
    nrf::configure();

    //        nrf::assign_addr();

    c = nrf::readRegister(STATUS_ADDR);
    term << "status=0x" << c << term.newline;

    nrf::writeRegister(CONFIG_REG_ADDR, 0x0B);
    c = nrf::readRegister(CONFIG_REG_ADDR);
    term << "c=0x" << c << term.newline;
    nrf_buf[0] = 5;
    nrf_buf[1] = 6;
    nrf_buf[2] = 7;
    nrf_buf[3] = 8;
    nrf_buf[4] = 9;
    nrf::writeAddressRegister(RX_ADDR_P0, nrf_buf);

    nrf_buf[0] = 0;
    nrf_buf[1] = 0;
    nrf_buf[2] = 0;
    nrf_buf[3] = 0;
    nrf_buf[4] = 0;
    nrf::readAddressRegister(RX_ADDR_P0, nrf_buf);
    term << "rx_addr_p0=0x" << nrf_buf[0] << nrf_buf[1] << nrf_buf[2] << nrf_buf[3] << nrf_buf[4] << term.newline;
  }
}
