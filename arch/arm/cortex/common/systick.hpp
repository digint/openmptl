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

#ifndef COMMON_ARM_CORTEX_SYSTICK_HPP_INCLUDED
#define COMMON_ARM_CORTEX_SYSTICK_HPP_INCLUDED

#include <arch/core.hpp>
#include <arch/nvic.hpp>
#include <resource.hpp>
#include <freq.hpp>

enum class SysTickClockSource {
  hclk,       /**< AHB clock (HCLK)                      */
  hclk_div8   /**< AHB clock (HCLK) divided by 8 (9MHz)  */
};


template<typename rcc,
         freq_t interrupt_rate,   //< interrupt rate in Hz
         SysTickClockSource clock_source = SysTickClockSource::hclk_div8>
class SysTick
{
  using SCB = reg::SCB;

public:

  static constexpr freq_t freq = interrupt_rate;

  static constexpr uint32_t counter_freq = rcc::hclk_freq /
                                           (clock_source == SysTickClockSource::hclk_div8 ? 8 : 1);

  static constexpr uint32_t reload_value = counter_freq / interrupt_rate;
  static_assert((reload_value >= 1) && (reload_value <= 0x00FFFFFF), "illegal reload value");

  /** picoseconds per counter tick
   *
   * min: 72MHz, hclk:    13'888
   * max: 24MHz, hclk/8: 333'333
   */
  static constexpr uint32_t ps_per_tick  = (1000 * 1000 * 1000) / (counter_freq / 1000);

  typedef void resources;

  typedef irq::SysTick Irq; /**< System Tick Interrupt */

  static void set_reload(SCB::STRVR::value_type reload) {
//    assert((reload >= 1) && (reload <= 0xFFFFFF));
    SCB::STRVR::store(reload);
  }

  static void set_clock_source(void) {
    if(clock_source == SysTickClockSource::hclk) {
      SCB::STCSR::CLKSOURCE::set();
    } else { // hclk_div8
      SCB::STCSR::CLKSOURCE::clear();
    }
  }
  static void enable_counter(void) {
    SCB::STCSR::ENABLE::set();
  }
  static void disable_counter(void) {
    SCB::STCSR::ENABLE::clear();
  }
  static void clear_counter(void) {
    SCB::STCVR::store(0);
  }
  static SCB::STCVR::value_type get_counter(void) {
    return SCB::STCVR::load();
  }

  static void enable_interrupt(void) {
    SCB::STCSR::TICKINT::set();
  }
  static void disable_interrupt(void) {
    SCB::STCSR::TICKINT::clear();
  }

  static bool get_count_flag(void) {
    return SCB::STCSR::COUNTFLAG::test();
  }
  static bool get_skew_flag(void) {
    return SCB::STCR::SKEW::test();
  }
  static bool get_no_ref_flag(void) {
    return SCB::STCR::NOREF::test();
  }

  static void init(void) {
    clear_counter();
    set_reload(reload_value);
    set_clock_source();
    enable_counter();
  }

};

#endif // COMMON_ARM_CORTEX_SYSTICK_HPP_INCLUDED


