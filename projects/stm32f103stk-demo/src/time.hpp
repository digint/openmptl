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
#include <resource.hpp>
#include <atomic>

typedef unsigned int systick_t;

class SystemTime
{
  static std::atomic<systick_t> systick_count;
  // static systick_t systick_count;

  static volatile unsigned int seconds;
  // static std::atomic<unsigned int> seconds;

protected:
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
  using rtc     = Rtc;
  using systick = _systick;

  using resources = ResourceList<
    IrqResource< typename systick::Irq,   systick_isr >,
    IrqResource< typename rtc::GlobalIrq, rtc_isr     >,
    typename systick::resources,
    rtc::resources
  >;

  static void init(void) {
    systick::init();
    rtc::init();
    // TODO: play around with prescaler (measurements!)
    rtc::set_prescaler(0x7FFF); // 1sec
  }

  static void enable(void) {
    systick::enable_interrupt();
    rtc::enable_second_interrupt();
    rtc::GlobalIrq::enable();
  }

  static unsigned int get_rtc_seconds(void) {
    // return seconds.load(std::memory_order_relaxed);
    return seconds;
  }

  static systick_t get_systick() {
    return systick_count.load(std::memory_order_relaxed);
    // return systick_count;
  }
};

#endif // TIME_HPP_INCLUDED
