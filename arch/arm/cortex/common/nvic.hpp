/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright (C) 2012-2017 Axel Burri <axel@tty0.ch>
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

#ifndef ARM_CORTEX_COMMON_NVIC_HPP_INCLUDED
#define ARM_CORTEX_COMMON_NVIC_HPP_INCLUDED

#include <arch/scb.hpp>
#include "reg/nvic.hpp"
#include <isr.hpp>
#include <type_traits>

namespace mptl {

////////////////////  irq_base  ////////////////////


template<int _irqn>
struct irq_base {
  static constexpr int irqn = _irqn;
};


////////////////////  core_exception  ////////////////////


template<int irqn>
class core_exception : public irq_base<irqn> {
  static_assert(irqn < 0 && irqn > -16, "illegal core exception interrupt number");

#if 0
public:

  static constexpr bool priority_available = irqn > -13;

  static typename std::enable_if<priority_available>::type set_priority(uint32_t priority) {
    scb::set_priority<irqn>(priority);
  }

  static typename std::enable_if<priority_available, uint32_t>::type get_priority(void) {
    return scb::set_priority<irqn>();
  }
#endif
};


////////////////////  irq_channel  ////////////////////


template<int irqn>
class irq_channel : public irq_base<irqn> {
  static_assert(irqn >= 0, "illegal irq channel interrupt number");

  static constexpr unsigned reg_index = (uint32_t)irqn >> 5;
  static constexpr unsigned irq_bit = 1 << ((uint32_t)irqn & 0x1F);

  using ISERx = NVIC::ISER<reg_index>;
  using ICERx = NVIC::ICER<reg_index>;
  using ISPRx = NVIC::ISPR<reg_index>;
  using ICPRx = NVIC::ICPR<reg_index>;
  using IABRx = NVIC::IABR<reg_index>;
  using IPRx  = NVIC::IPR<irqn>;

public:

  static void enable(void) {
    ISERx::store(irq_bit);
  }
  static void disable(void) {
    ICERx::store(irq_bit);
  }

  static bool is_pending(void) {
    return ISPRx::load() & irq_bit;
  }
  static void set_pending(void) {
    ISPRx::store(irq_bit);
  }

  static void clear_pending(void) {
    ICPRx::store(irq_bit);
  }
  static bool is_active(void) {
    return IABRx::load() & (irq_bit);
  }

#if 0
  static void set_priority(uint32_t priority) {
    IPRx::store((priority << (8 - priority_bits)) & 0xff);
  }

  static uint32_t get_priority(void) {
    return((uint32_t)(IPRx::load() >> (8 - priority_bits)));
  }
#endif
};


////////////////////  irq typedefs  ////////////////////


namespace irq {

/* Fixed core exceptions */
using reset      = core_exception<-15>;  // NOTE: priority is fixed to -3
using nmi        = core_exception<-14>;  // NOTE: priority is fixed to -2
using hard_fault = core_exception<-13>;  // NOTE: priority is fixed to -1

/* Settable core exceptions */
using memory_management = core_exception<-12>;
using bus_fault         = core_exception<-11>;
using usage_fault       = core_exception<-10>;
using sv_call           = core_exception<-5>;
using debug_monitor     = core_exception<-4>;
using pend_sv           = core_exception<-2>;
using systick           = core_exception<-1>;

static inline constexpr bool reserved_irqn(int irqn) {
  return ((irqn == -3) ||
          (irqn == -6) ||
          (irqn == -7) ||
          (irqn == -8) ||
          (irqn == -9));
}

} // namespace irq

} // namespace mptl

#endif // ARM_CORTEX_COMMON_NVIC_HPP_INCLUDED
