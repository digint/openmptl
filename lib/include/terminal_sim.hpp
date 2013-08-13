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

#ifndef TERMINAL_SIM_HPP_INCLUDED
#define TERMINAL_SIM_HPP_INCLUDED

#ifdef OPENMPTL_SIMULATION
#ifdef CONFIG_REGISTER_REACTION

#include <simulation.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <ratio>
#include <poll.h>

namespace mptl {

/**
 * Simulate a terminal (Tp) on stdin/stdout, by starting rx/tx threads
 * directly manipulating the fifo of Tp::stream_device_type.
 *
 * For use in reg_reaction::react().
 *
 * NOTE: The threads will use std::cin and std::cout, which are not
 * thread safe!
 */
template<typename Tp>
class terminal_reaction
{
  reg_reaction const & reaction;

  static std::atomic<bool> terminal_rx_thread_terminate;
  static std::atomic<bool> terminal_tx_thread_terminate;

public:

  terminal_reaction(reg_reaction const & r): reaction(r) { }

  /**
   * Poll stdin, and feed result into stream_device_type::rx_fifo.
   *
   * NOTE: we use std::cin, which is not thread safe.
   */
  static void terminal_rx_thread() {
    char c;
    pollfd cinfd[1];
    cinfd[0].fd = fileno(stdin);
    cinfd[0].events = POLLIN;

    //  std::cout << "*** terminal_rx_thread() running" << std::endl;
    while(!terminal_rx_thread_terminate)
    {
      if(poll(cinfd, 1, 0))
      {
        c = std::cin.get();
        // std::cout << '<' << +c << '>' <<  std::endl;
        if(c == 10) c = 13; // convert LF into CR (hacky...)

        /* feed rx_fifo, will pe polled in terminal.process_input() */
        Tp::stream_device_type::rx_fifo.push(c);
      }
      SIM_RELAX; // sleep a bit (don't eat up all cpu power)
    }
    //  std::cout << "*** terminal_rx_thread() terminated" << std::endl;
  }

  /**
   * Hook into stream_device_type::tx_fifo, and print output to stdout.
   *
   * NOTE: we use std::cout, which is not thread safe.
   */
  static void terminal_tx_thread() {
    //  std::cout << "*** terminal_tx_thread() running" << std::endl;
    while(!terminal_tx_thread_terminate)
    {
      char c;
      while(Tp::stream_device_type::tx_fifo.pop(c)) {
        std::cout << c;
      }
      std::cout << std::flush;
      SIM_RELAX; // sleep a bit (don't eat up all cpu power)
    }
    //  std::cout << "*** terminal_tx_thread() terminated" << std::endl;
  }

  template<
    typename rx_trigger_regmask_type,
    typename tx_trigger_regmask_type
    >
  void react(void)
  {
    /* start/stop terminal rx thread on rx_trigger_regmask_type */
    if(reaction.bits_set< rx_trigger_regmask_type >()) {
      terminal_rx_thread_terminate = false;
      std::thread(terminal_rx_thread).detach();
    }
    else if(reaction.bits_cleared< rx_trigger_regmask_type >()) {
      terminal_rx_thread_terminate = true;
    }

    /* start/stop terminal tx thread on tx_trigger_regmask_type */
    if(reaction.bits_set< tx_trigger_regmask_type >()) {
      terminal_tx_thread_terminate = false;
      std::thread(terminal_tx_thread).detach();
    }
    else if(reaction.bits_cleared< tx_trigger_regmask_type >()) {
      terminal_tx_thread_terminate = true;
    }
  }
};

template<typename Tp> std::atomic<bool> terminal_reaction<Tp>::terminal_rx_thread_terminate;
template<typename Tp> std::atomic<bool> terminal_reaction<Tp>::terminal_tx_thread_terminate;

} // namespace mptl

#endif // CONFIG_REGISTER_REACTION
#endif // OPENMPTL_SIMULATION

#endif // TERMINAL_SIM_HPP_INCLUDED
