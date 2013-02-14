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
#include <cstring> // strcmp
#include <type_traits>

// ----------------------------------------------------------------------------
// Terminal
//

namespace text
{
  static constexpr const char * term_cmd_notfound = ": command not found (type \"help\" for a list of available commands)";
}

class Terminal
{
public:
  typedef char char_type;

protected:
  Fifo<char_type> & rx_fifo;

public:
  poorman_ostream<char_type> & tx_stream;

  static constexpr const char_type * newline = "\r\n";
  static constexpr const char_type * prompt  = "# ";

  Terminal(Fifo<char_type> &_rx_fifo, poorman_ostream<char_type> &_tx_stream) : rx_fifo(_rx_fifo), tx_stream(_tx_stream) { }

  template<typename T>
  poorman_ostream<char> & operator<<(T & st) {
    return tx_stream << st;
  }
};


// ----------------------------------------------------------------------------
// CommandTerminal
//

template<typename cmd_hooks>
class CommandTerminal : public Terminal
{
  static constexpr std::size_t cmd_buf_size = 80;

  char_type cmd_buf[cmd_buf_size];
  unsigned cmd_index = 0;

public:

  CommandTerminal(Fifo<char_type> &_rx_fifo, poorman_ostream<char_type> &_tx_stream) : Terminal(_rx_fifo, _tx_stream) { }

  void process_input(void)
  {
    bool flush_tx = false;
    char c;
    while(rx_fifo.pop(c)) {
      flush_tx = true;
      if(c == 13) {  // CR
        tx_stream << newline;  // echo
        cmd_buf[cmd_index] = 0;

        if(cmd_index) {
          cmd_hooks::template execute<cmd_hooks>(cmd_buf, *this);
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
};

// ----------------------------------------------------------------------------
// TerminalHook
//

struct TerminalHook {
};


// ----------------------------------------------------------------------------
// TerminalHookList
//

template<std::size_t... Args>
struct max {
  static constexpr std::size_t value = 0;
};

template<std::size_t T, std::size_t... Args>
struct max<T, Args...> {
  static constexpr std::size_t value = T > max<Args...>::value ? T : max<Args...>::value;
};





template<typename... Args>
struct TerminalHookList;

template<>
struct TerminalHookList<>
{
  template<typename HL>
  static void execute(const char * cmd_buf, Terminal & term) {
    if(strcmp("help", cmd_buf) == 0) {
      term << "List of commands:" << term.newline;
      HL::template list<HL>(term);
    }
    else {
      term << cmd_buf << text::term_cmd_notfound << term.newline;
    }
  }

  template<typename HL>
  static void list(Terminal &) { }
};

template<typename T, typename... Args>
struct TerminalHookList<T, Args...> {

  template<typename HL>
  static void execute(const char * cmd_buf, Terminal & term) {
    if(strcmp(T::cmd, cmd_buf) == 0) {
      T().run(term);
    }
    else {
      TerminalHookList<Args...>::template execute<HL>(cmd_buf, term);
    }
  }

  /** maximum length of all commands (minimum 8) */
  static constexpr unsigned cmd_maxlen = max<8, strlen(T::cmd), strlen(Args::cmd)...>::value;

  template<typename HL>
  static void list(Terminal & term) {
    term << "   " << T::cmd;
    for(int n = HL::cmd_maxlen - strlen(T::cmd) + 3 ; n > 0; n--)
      term.tx_stream.put(' ');
    term << T::desc << endl;

    TerminalHookList<Args...>::template list<HL>(term);
  }
};


#endif
