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

#ifndef TERMINAL_HOOKS_HPP_INCLUDED
#define TERMINAL_HOOKS_HPP_INCLUDED

#include <terminal.hpp>
#include <arch/scb.hpp>

namespace terminal_hooks
{
  struct cpuid
  : public TerminalHook
  {
    static constexpr const char * cmd  = "cpuid";
    static constexpr const char * desc = "prints the SCB::CPUID register";
    void run(Terminal & term) {
      term.tx_stream << reg::SCB::CPUID::load() << endl;
    }
  };

  // ----------------------------------------------------------------------------
  // Terminal Commands
  //

  typedef TerminalHookList< cpuid > commands;
}
#endif
