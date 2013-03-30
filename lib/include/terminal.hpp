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


namespace text
{
  static constexpr const char * term_cmd_notfound = ": command not found (type \"help\" for a list of available commands)";
  static constexpr const char * term_cmd_list     = "List of commands:";
}


// ----------------------------------------------------------------------------
// Terminal
//

template<typename stream_device_type, typename cmd_hooks>
class Terminal
{
  using char_type      = typename stream_device_type::fifo_type::char_type;
  using tx_stream_type = FifoStream< typename stream_device_type::fifo_type, stream_device_type >;

  static constexpr std::size_t cmd_buf_size = 80;  // TODO: use cmd_hooks::cmd_buf_size, which is the maximum of all command sizes (can be computed at compile-time!)

  char_type cmd_buf[cmd_buf_size];
  unsigned cmd_index = 0;

public:
  tx_stream_type tx_stream;

  static constexpr const char * newline = "\r\n";
  static constexpr const char * prompt  = "# ";

  using resources = typename stream_device_type::resources;

  Terminal() : tx_stream(stream_device_type::tx_fifo) { }

  template<typename device_cfg_type>  // TODO: more specific, only allow devices by stream_device class
  static void open(device_cfg_type const & device_cfg) {
    stream_device_type::open(device_cfg); // !!!!
  }

  void process_input(void)
  {
    bool flush_tx = false;
    char c;
    while(stream_device_type::rx_fifo.pop(c)) {
      flush_tx = true;
      if(c == 13) {  // CR
        tx_stream << newline;  // echo
        cmd_buf[cmd_index] = 0;

        if(cmd_index) {
          cmd_hooks::template execute<cmd_hooks>(cmd_buf, tx_stream);
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
    if(flush_tx)
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

namespace mpl
{
  template<std::size_t... Args>
  struct max {
    static constexpr std::size_t value = 0;
  };

  template<std::size_t T, std::size_t... Args>
  struct max<T, Args...> {
    static constexpr std::size_t value = T > max<Args...>::value ? T : max<Args...>::value;
  };
}


template<typename... Args>
struct TerminalHookList;

template<>
struct TerminalHookList<>
{
  template<typename HL>
  static void execute(const char * cmd_buf, poorman_ostream<char> & ostream) {
    if(strcmp("help", cmd_buf) == 0) {
      ostream << text::term_cmd_list << endl;
      HL::template list<HL>(ostream);
    }
    else {
      ostream << cmd_buf << text::term_cmd_notfound << endl;
    }
  }

  template<typename HL>
  static void list(poorman_ostream<char> &) { }
};

template<typename T, typename... Args>
struct TerminalHookList<T, Args...> {

  template<typename HL>
  static void execute(const char * cmd_buf, poorman_ostream<char> & ostream) {
    if(strcmp(T::cmd, cmd_buf) == 0) {
      T().run(ostream);
    }
    else {
      TerminalHookList<Args...>::template execute<HL>(cmd_buf, ostream);
    }
  }

  /** maximum length of all commands (minimum 8, for formatting help text) */
  static constexpr unsigned cmd_maxlen = mpl::max<8, strlen(T::cmd), strlen(Args::cmd)...>::value;

  template<typename HL>
  static void list(poorman_ostream<char> & ostream) {
    ostream << "   " << T::cmd;
    for(int n = HL::cmd_maxlen - strlen(T::cmd) + 3 ; n > 0; n--)
      ostream.put(' ');
    ostream << T::desc << endl;

    TerminalHookList<Args...>::template list<HL>(ostream);
  }
};

#endif // TERMINAL_HPP_INCLUDED
