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

#ifndef COMMON_ARM_CORTEX_DWT_HPP_INCLUDED
#define COMMON_ARM_CORTEX_DWT_HPP_INCLUDED

#include "reg/dwt.hpp"
#include "reg/debug.hpp"

class Dwt
{
  using DWT = reg::DWT;

public:

  static void enable(void) {
    reg::DEBUG::DEMCR::TRCENA::set();
  }

  static void disable(void) {
    reg::DEBUG::DEMCR::TRCENA::clear();
  }

  static void cycle_counter_enable(void) {
    DWT::CYCCNT::store(0); // reset counter
    DWT::CTRL::set(1);     // enable counter
  }

  static DWT::CYCCNT::value_type cycle_counter_load(void) {
    return DWT::CYCCNT::load();
  }
};


/**
 * CycleCounter: Count processor clock cycles
 */
class CycleCounter
{
  using DWT = reg::DWT;
  using value_type = decltype(Dwt::cycle_counter_load());

  value_type value;

public:

  CycleCounter(void) : value(0) { 
    Dwt::enable();
    Dwt::cycle_counter_enable();
  }

  ~CycleCounter(void) { 
    Dwt::disable();
  }

  void start(void) {
    value = Dwt::cycle_counter_load();
  }

  void stop(void) {
    value = Dwt::cycle_counter_load() - value;
  }

  value_type get(void) {
    return value;
  }
};

#endif // COMMON_ARM_CORTEX_DWT_HPP_INCLUDED
