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

#ifndef COMMON_ARM_CORTEX_NVIC_HPP_INCLUDED
#define COMMON_ARM_CORTEX_NVIC_HPP_INCLUDED

#include <arch/scb.hpp>
#include "reg/nvic.hpp"
#include <isr.hpp>
#include <type_traits>


////////////////////  IrqBase  ////////////////////


template<int _irqn>
struct IrqBase {
  static constexpr int irqn = _irqn;
};


////////////////////  CoreException  ////////////////////


template<int irqn>
class CoreException : public IrqBase<irqn> {
  static_assert(irqn < 0 && irqn > -16, "illegal core exception interrupt number");

#if 0
public:

  static constexpr bool priority_available = irqn > -13;

  static typename std::enable_if<priority_available>::type set_priority(uint32_t priority) {
    Scb::SetPriority<irqn>(priority);
  }

  static typename std::enable_if<priority_available, uint32_t>::type get_priority(void) {
    return Scb::GetPriority<irqn>();
  }
#endif
};

namespace irq
{
  /* Fixed core exceptions */
  typedef CoreException<-15>  Reset;       // NOTE: priority is fixed to -3
  typedef CoreException<-14>  NMI;         // NOTE: priority is fixed to -2
  typedef CoreException<-13>  HardFault;   // NOTE: priority is fixed to -1

  /* Settable core exceptions */
  typedef CoreException<-12>  MemoryManagement;
  typedef CoreException<-11>  BusFault;
  typedef CoreException<-10>  UsageFault;
  typedef CoreException<-5>   SVCall;
  typedef CoreException<-4>   DebugMonitor;
  typedef CoreException<-2>   PendSV;
  typedef CoreException<-1>   SysTick;

  static constexpr bool reserved_irqn(int irqn) {
    return ((irqn == -3) ||
            (irqn == -6) ||
            (irqn == -7) ||
            (irqn == -8) ||
            (irqn == -9));
  }
}


////////////////////  IrqChannel  ////////////////////


template<int irqn>
class IrqChannel : public IrqBase<irqn> {
  static_assert(irqn >= 0, "illegal irq channel interrupt number");

  static constexpr unsigned reg_index = (uint32_t)irqn >> 5;
  static constexpr unsigned irq_bit = 1 << ((uint32_t)irqn & 0x1F);

  using NVIC  = reg::NVIC;
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

#endif // COMMON_ARM_CORTEX_NVIC_HPP_INCLUDED
