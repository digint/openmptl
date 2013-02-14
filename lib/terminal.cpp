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

#include <terminal.hpp>
#include <terminal_hooks.hpp>

void Terminal::help() {
  tx_stream << "List of commands:"
            << newline;
  terminal_hooks::commands::list(*this);
}

void Terminal::process_input()
{
  bool flush_tx = false;
  char c;
  while(rx_fifo.pop(c)) {
    flush_tx = true;
    if(c == 13) {  // CR
      tx_stream << newline;  // echo

      cmd_buf[cmd_index] = 0;
      
      int exec_count = 0;
      // TODO: consider passing terminal hooks as template args
      terminal_hooks::commands::execute(*this, exec_count);
      if(cmd_index && !exec_count) {
        tx_stream << cmd_buf << text::notfound << newline;
      }
      tx_stream << prompt;
      cmd_index = 0;
    }
    else if((c >= 32) && (c <= 126) && (cmd_index < cmd_buf_size))
    {
      tx_stream.put(c);  // echo
      cmd_buf[cmd_index++] = c;
    }
  }
  if(flush_tx) // prevent unnecessary usart interrupts
    tx_stream.flush();
}
