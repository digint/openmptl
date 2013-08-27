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

#ifndef TERMINAL_HOOKS_HPP_INCLUDED
#define TERMINAL_HOOKS_HPP_INCLUDED

#include <terminal.hpp>
#include <arch/scb.hpp>

namespace terminal_hooks {

struct cpuid
: public mptl::terminal_hook
{
  static constexpr const char * cmd  = "cpuid";
  static constexpr const char * desc = "prints SCB::CPUID register hexadecimal value";
  void run(poorman::ostream<char> & cout) {
    cout << mptl::SCB::CPUID::load() << poorman::endl;
  }
};

struct heap_eater
: public mptl::terminal_hook
{
  static constexpr const char * cmd  = "heap";
  static constexpr const char * desc = "allocate (leak) 1k on heap";
  void run(poorman::ostream<char> &);
};

struct nrf_test
: public mptl::terminal_hook
{
  static constexpr const char * cmd  = "nrf";
  static constexpr const char * desc = "test the NRF24L01 chip (spi)";
  void run(poorman::ostream<char> &);
};


// ----------------------------------------------------------------------------
// Terminal Commands
//

using commands = mptl::terminal_hook_list<
  cpuid,
  heap_eater,
  nrf_test
  >;

} // namespace terminal_hooks

#endif // TERMINAL_HOOKS_HPP_INCLUDED
