/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright (C) 2012-2017 Axel Burri <axel@tty0.ch>
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

namespace terminal_hooks {

void nrf_test::run(poorman::ostream<char> & cout) {
  unsigned char c;

  using namespace poorman;
  using nrf = Kernel::nrf;

  nrf::enable();

  c = nrf::read_register(nrf::dev_register::status);
  cout << "status=0x" << c << endl;

  nrf::write_register(nrf::dev_register::config, 0x0B);
  c = nrf::read_register(nrf::dev_register::config);
  cout << "c=0x" << c << endl;

  nrf::dev_address addr(5, 6, 7, 8, 9);
  nrf::write_address_register(nrf::dev_register::rx_addr_p0, addr);

  nrf::dev_address r_addr;
  nrf::read_address_register(nrf::dev_register::rx_addr_p0, r_addr);
  cout << "rx_addr_p0=0x" << r_addr.buf[0] << r_addr.buf[1] << r_addr.buf[2] << r_addr.buf[3] << r_addr.buf[4] << endl;
}

} // namespace terminal_hooks
