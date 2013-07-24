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

#ifndef TERMINAL_HPP_INCLUDED
#define TERMINAL_HPP_INCLUDED

#include <fifo_stream.hpp>
#include <cstring> // strcmp
#include <type_traits>

namespace mptl {

namespace i18n { namespace terminal {
  static constexpr const char * cmd_notfound = ": command not found (type \"help\" for a list of available commands)";
  static constexpr const char * cmd_list     = "List of commands:";
} } // namespace i18n::terminal


// ----------------------------------------------------------------------------
// terminal
//

template<typename stream_device_type>
class terminal
{
  using char_type      = typename stream_device_type::fifo_type::char_type;
#ifndef OPENMPTL_SIMULATION
  using tx_stream_type = poorman::fifo_stream< typename stream_device_type::fifo_type, stream_device_type >;
#else
  using tx_stream_type = poorman::fifo_stream< typename stream_device_type::fifo_type, std::ostream >;
#endif

  static constexpr std::size_t cmd_buf_size = 80;  // TODO: use cmd_hooks::cmd_buf_size, which is the maximum of all command sizes (can be computed at compile-time!)

  char_type cmd_buf[cmd_buf_size];
  unsigned cmd_index = 0;

public:
  tx_stream_type tx_stream;

  static constexpr const char * newline = "\r\n";
  static constexpr const char * prompt  = "# ";

#ifdef OPENMPTL_SIMULATION
  static constexpr bool terminal_echo = false;
#else
  static constexpr bool terminal_echo = true;
#endif

  using resources = typename stream_device_type::resources;

  terminal() : tx_stream(stream_device_type::tx_fifo) { }

  void open() const {
    // stream_device_type::configure();
    stream_device_type::open();
  }

  void close() {
    stream_device_type::close();
    cmd_index = 0;
  }

  template<typename cmd_hooks>
  void process_input(void)
  {
    bool flush_tx = false;
    char c;
#ifndef OPENMPTL_SIMULATION
    while(stream_device_type::rx_fifo.pop(c)) {
#else
    while((c = std::cin.get())) {
      if(c == 10) c = 13; // convert LF into CR (hacky...)
#endif
      flush_tx = true;
      if(c == 13)  // CR
      {
        if(terminal_echo)
          tx_stream << newline;

        cmd_buf[cmd_index] = 0;

        if(cmd_index) {
          cmd_hooks::template execute<cmd_hooks>(cmd_buf, tx_stream);
        }
        tx_stream << prompt;
        cmd_index = 0;
      }
      else if((c >= 32) && (c <= 126) && (cmd_index < cmd_buf_size))
      {
        if(terminal_echo)
          tx_stream.put(c);

        cmd_buf[cmd_index++] = c;
      }
    }
    if(flush_tx)
      tx_stream.flush();
  }
};


// ----------------------------------------------------------------------------
// terminal_hook
//

struct terminal_hook {
};


// ----------------------------------------------------------------------------
// terminal_hook_list
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
struct terminal_hook_list;

template<>
struct terminal_hook_list<>
{
  template<typename HL>
  static void execute(const char * cmd_buf, poorman::ostream<char> & cout) {
    if(strcmp("help", cmd_buf) == 0) {
      cout << i18n::terminal::cmd_list << poorman::endl;
      HL::template list<HL>(cout);
    }
    else {
      cout << cmd_buf << i18n::terminal::cmd_notfound << poorman::endl;
    }
  }

  template<typename HL>
  static void list(poorman::ostream<char> &) { }
};

template<typename T, typename... Args>
struct terminal_hook_list<T, Args...> {

  template<typename HL>
  static void execute(const char * cmd_buf, poorman::ostream<char> & cout) {
    if(strcmp(T::cmd, cmd_buf) == 0) {
      T().run(cout);
    }
    else {
      terminal_hook_list<Args...>::template execute<HL>(cmd_buf, cout);
    }
  }

#ifdef CONFIG_CLANG
  // for some reason, clang-3.3 does not treat strlen() as constexpr
  static constexpr unsigned cmd_maxlen = 8;
#else
  /** maximum length of all commands (minimum 8, for formatting help text) */
  static constexpr unsigned cmd_maxlen = mpl::max<8, strlen(T::cmd), strlen(Args::cmd)...>::value;
#endif

  template<typename HL>
  static void list(poorman::ostream<char> & cout) {
    cout << "   " << T::cmd;
    for(int n = HL::cmd_maxlen - strlen(T::cmd) + 3 ; n > 0; n--)
      cout.put(' ');
    cout << T::desc << poorman::endl;

    terminal_hook_list<Args...>::template list<HL>(cout);
  }
};

} // namespace mptl

#endif // TERMINAL_HPP_INCLUDED
