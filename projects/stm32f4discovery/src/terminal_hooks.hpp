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

namespace terminal_hooks
{
  struct Help
  : public TerminalHook
  {
    static constexpr const char * cmd      = "help";
    static constexpr const char * cmd_desc = "display available commands";
    void run(Terminal & term) {
      term.help();
    }
  };

  // ----------------------------------------------------------------------------
  // Terminal Commands
  //

  typedef TerminalHookList< Help > commands;
}
#endif
