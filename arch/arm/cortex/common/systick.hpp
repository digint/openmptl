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

#ifndef ARM_CORTEX_COMMON_SYSTICK_HPP_INCLUDED
#define ARM_CORTEX_COMMON_SYSTICK_HPP_INCLUDED

#include <arch/nvic.hpp>
#include <typelist.hpp>
#include <freq.hpp>


namespace mptl {


struct systick_clock
{
  /** Select external clock (HCLK_DIV8) as systick clock source */
  template< typename system_clock_type, freq_t _freq >
  struct external {
    static constexpr freq_t freq = _freq;
    static constexpr freq_t counter_freq = system_clock_type::hclk_freq / 8;

    using resources = reglist<
      regval< SCB::STCSR::CLKSOURCE, 0 >,
      regval< SCB::STRVR::regbits_type, (system_clock_type::hclk_freq / (freq * 8)) >
      >;
  };

  /** Select core clock (HCLK) as systick clock source */
  template< typename system_clock_type, freq_t _freq >
  struct core {
    static constexpr freq_t freq = _freq;
    static constexpr freq_t counter_freq = system_clock_type::hclk_freq;

    using resources = reglist<
      regval< SCB::STCSR::CLKSOURCE, 1 >,
      regval< SCB::STRVR::regbits_type, (system_clock_type::hclk_freq / freq) >
      >;
  };
};


template< typename clock_source_type >
class systick
{
public:
  static constexpr freq_t freq = clock_source_type::freq;
  static constexpr freq_t counter_freq = clock_source_type::counter_freq;
  static constexpr uint32_t reload_value = counter_freq / freq;

  /** picoseconds per counter tick
   *
   * min: 72MHz, hclk:    13'888
   * max: 24MHz, hclk/8: 333'333
   */
  static constexpr uint32_t ps_per_tick  = (1000 * 1000 * 1000) / (counter_freq / 1000);

  using resources = typename clock_source_type::resources;

  using irq = mptl::irq::systick; /**< System Tick Interrupt */

  static void set_reload(SCB::STRVR::value_type reload) {
    // assert((reload >= 1) && (reload <= 0xFFFFFF));
    SCB::STRVR::store(reload);
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

  static void enable(void) {
    clear_counter();
    enable_counter();
  }
};

} // namespace mptl

#endif // ARM_CORTEX_COMMON_SYSTICK_HPP_INCLUDED
