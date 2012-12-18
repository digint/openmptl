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

#ifndef TERMINAL_HPP_INCLUDED
#define TERMINAL_HPP_INCLUDED

#include <fifo.hpp>
#include <arch/uart_transport.hpp>
#include <cstring> // strcmp
#include <type_traits>

// ----------------------------------------------------------------------------
// Terminal
//

struct text
{
  static constexpr const char * notfound = ": command not found (type \"help\" for a list of available commands)";
};

class Terminal
{
  typedef char char_type;
  static constexpr std::size_t cmd_buf_size = 80;

  char_type cmd_buf[cmd_buf_size];
  unsigned cmd_index = 0;

  Fifo<char_type> & rx_fifo;
  poorman_ostream<char_type> & tx_stream;

public:
  static constexpr const char_type * newline = "\r\n";
  static constexpr const char_type * prompt  = "# ";

  Terminal(Fifo<char_type> &_rx_fifo, poorman_ostream<char_type> &_tx_stream) : rx_fifo(_rx_fifo), tx_stream(_tx_stream) { }

  void process_input();
  void help();
  char_type * get_cmd() { return cmd_buf; };

  template<typename T>
  poorman_ostream<char> & operator<<(T & st) {
    return tx_stream << st;
  }

private:
};


// ----------------------------------------------------------------------------
// TerminalHook
//

struct TerminalHook {
  static bool parse(Terminal & term, const char * cmd) {
    return strcmp(term.get_cmd(), cmd) == 0;
  };
};

// ----------------------------------------------------------------------------
// TerminalHookList
//

template<typename... Args>
struct TerminalHookList;

template<>
struct TerminalHookList<>
{
  static void execute(Terminal &, int &) { }
  static void list(Terminal &) { }
};

template<typename T, typename... Args>
struct TerminalHookList<T, Args...> {
  static void execute(Terminal & term, int & exec_count) {
    if(T::parse(term, T::cmd)) {
      T().run(term);
      exec_count++;
    }
    TerminalHookList<Args...>::execute(term, exec_count);
  }

  static void list(Terminal & term) {
    term << T::cmd << "\t\t" << T::cmd_desc << term.newline;
    TerminalHookList<Args...>::list(term);
  }
};

#endif
