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

#ifndef TIME_HPP_INCLUDED
#define TIME_HPP_INCLUDED

#include <arch/systick.hpp>
#include <arch/rtc.hpp>
#include <typelist.hpp>
#include <freq.hpp>
#include <atomic>

typedef unsigned int systick_t;

class SystemTime
{
protected:
  static std::atomic<systick_t> systick_count;
  // static systick_t systick_count;

  static void systick_isr(void);
  static void rtc_isr(void);

public:
  //  static constexpr long long nop_s = 11999940; // nop's per second when using Core::nop(n) (-Os, 72Mhz)
  static void nanosleep(unsigned int ns);

};


template<typename _systick>
class Time : public SystemTime
{

public:
  using rtc     = mptl::rtc< mptl::khz(0x8000) >;  /* 32.768 kHz LSE clock */
  using systick = _systick;

  static constexpr mptl::freq_t rtc_freq = mptl::hz(1);  /* 1sec signal period */

  using resources = mptl::typelist<
    mptl::irq_handler< typename systick::irq,    systick_isr >,
    mptl::irq_handler< typename rtc::irq_global, rtc_isr     >,
    typename systick::resources,
    rtc::resources
  >;

  static void init(void) {
    systick::init();
    rtc::init< rtc_freq >();
    rtc::set_counter(1);
  }

  static void enable(void) {
    systick::enable_interrupt();
    rtc::enable_second_interrupt();
    rtc::irq_global::enable();
  }

  static unsigned int get_rtc_seconds(void) {
    return rtc::get_counter();
  }

  static systick_t get_systick() {
    return systick_count.load(std::memory_order_relaxed);
    // return systick_count;
  }
};

#endif // TIME_HPP_INCLUDED
