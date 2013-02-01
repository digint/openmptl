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
#include <arch/core_resource.hpp>

namespace cSysTick
{
  enum class ClockSource {
    hclk,       /**< AHB clock (HCLK)                      */
    hclk_div8   /**< AHB clock (HCLK) divided by 8 (9MHz)  */
  };
}


#if 0
  static constexpr uint32_t LOAD_RELOAD     = 0x00FFFFFF;   /**< Value to load into the SysTick Current Value Register when the counter reaches 0  */
  static constexpr uint32_t STCVR_CURRENT   = 0x00FFFFFF;   /**< Current value at the time the register is accessed                                */
  static constexpr uint32_t STCR_TENMS      = 0x00FFFFFF;   /**< Reload value to use for 10ms timing                                               */
#endif


template<freq_t interrupt_rate,   //< interrupt rate in Hz
         cSysTick::ClockSource clock_source = cSysTick::ClockSource::hclk_div8>
class SysTick
{
  using SCB = reg::SCB;

public:

  static constexpr uint32_t counter_freq = Core::clock_frequency /
                                           (clock_source == cSysTick::ClockSource::hclk_div8 ? 8 : 1);

  static constexpr uint32_t reload_value = counter_freq / interrupt_rate;
  static_assert((reload_value >= 1) && (reload_value <= 0x00FFFFFF), "illegal reload value");

  /** picoseconds per counter tick
   *
   * min: 72MHz, hclk:    13'888
   * max: 24MHz, hclk/8: 333'333
   */
  static constexpr uint32_t ps_per_tick  = (1000 * 1000 * 1000) / (counter_freq / 1000);


  typedef ResourceList<> resources;

  typedef CoreException::SysTick Irq; /**< System Tick Interrupt */

  static void SetReload(SCB::STRVR::value_type reload) {
//    assert((reload >= 1) && (reload <= 0xFFFFFF));
    SCB::STRVR::store(reload);
  }

  static void SetClockSource(void) {
    if(clock_source == cSysTick::ClockSource::hclk) {
      SCB::STCSR::CLKSOURCE::set();
    } else { // hclk_div8
      SCB::STCSR::CLKSOURCE::clear();
    }
  }
  static void EnableCounter(void) {
    SCB::STCSR::ENABLE::set();
  }
  static void DisableCounter(void) {
    SCB::STCSR::ENABLE::clear();
  }
  static void ClearCounter(void) {
    SCB::STCVR::store(0);
  }
  static SCB::STCVR::value_type GetCounter(void) {
    return SCB::STCVR::load();
  }

  static void EnableInterrupt(void) {
    SCB::STCSR::TICKINT::set();
  }
  static void DisableInterrupt(void) {
    SCB::STCSR::TICKINT::clear();
  }

  static bool GetCountFlag(void) {
    return SCB::STCSR::COUNTFLAG::test();
  }
  static bool GetSkewFlag(void) {
    return SCB::STCR::SKEW::test();
  }
  static bool GetNoRefFlag(void) {
    return SCB::STCR::NOREF::test();
  }

  static void Init(void) {
    ClearCounter();
    SetReload(reload_value);
    SetClockSource();
    EnableCounter();
  }

};

#endif // COMMON_ARM_CORTEX_SYSTICK_HPP_INCLUDED


