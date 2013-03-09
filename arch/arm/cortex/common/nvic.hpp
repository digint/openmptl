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
#include <core_setup.hpp>
#include "reg/nvic.hpp"
#include <isr.hpp>
#include <type_traits>


////////////////////  NvicPriority  ////////////////////


template<int irqn>
class NvicPriority : public InterruptControllerSetup {
public:
  /**
   * @brief  Encode the priority for an interrupt
   *
   * @param  PriorityGroup    The used priority group
   * @param  PreemptPriority  The preemptive priority value (starting from 0)
   * @param  SubPriority      The sub priority value (starting from 0)
   * @return                  The encoded priority for the interrupt
   *
   * Encode the priority for an interrupt with the given priority group,
   * preemptive priority value and sub priority value.
   * In case of a conflict between priority grouping and available
   * priority bits (priority_bits) the samllest possible priority group is set.
   *
   * The returned priority value can be used for NVIC_SetPriority(...) function
   */
  static inline uint32_t EncodePriority (uint32_t group, uint32_t preempt_priority, uint32_t sub_priority) {
//TODO      assert(group == (group & 0x07));
    uint32_t preempt_priority_bits;
    uint32_t sub_priority_bits;

    preempt_priority_bits = ((7 - group) > priority_bits) ? priority_bits : 7 - group;
    sub_priority_bits     = ((group + priority_bits) < 7) ? 0 : group - 7 + priority_bits;

    return (
      ((preempt_priority & ((1 << (preempt_priority_bits)) - 1)) << sub_priority_bits) |
      ((sub_priority     & ((1 << (sub_priority_bits    )) - 1)))
      );
  }


  /**
   * @brief  Decode the priority of an interrupt
   *
   * @param  Priority           The priority for the interrupt
   * @param  PriorityGroup      The used priority group
   * @param  pPreemptPriority   The preemptive priority value (starting from 0)
   * @param  pSubPriority       The sub priority value (starting from 0)
   *
   * Decode an interrupt priority value with the given priority group to
   * preemptive priority value and sub priority value.
   * In case of a conflict between priority grouping and available
   * priority bits (priority_bits) the samllest possible priority group is set.
   *
   * The priority value can be retrieved with NVIC_GetPriority(...) function
   */
  static inline void DecodePriority (uint32_t priority, uint32_t group, uint32_t* preempt_priority, uint32_t* sub_priority) {
//TODO      assert(group == (group & 0x07));
    uint32_t preempt_priority_bits;
    uint32_t sub_priority_bits;

    preempt_priority_bits = ((7 - group) > priority_bits) ? priority_bits : 7 - group;
    sub_priority_bits     = ((group + priority_bits) < 7) ? 0 : group - 7 + priority_bits;

    *preempt_priority = (priority >> sub_priority_bits) & ((1 << (preempt_priority_bits)) - 1);
    *sub_priority     = (priority                   ) & ((1 << (sub_priority_bits    )) - 1);
  }
};


////////////////////  CoreException  ////////////////////


template<int irqn>
class CoreExceptionImpl : public NvicPriority<irqn> {
  static_assert(irqn < 0 && irqn > -16, "illegal core exception interrupt number");

public:
  static constexpr int irq_number = irqn;

#if 0
  static constexpr bool priority_available = irqn > -13;

  static typename std::enable_if<priority_available>::type SetPriority(uint32_t priority) {
    Scb::SetPriority<irqn>(priority);
  }

  static typename std::enable_if<priority_available, uint32_t>::type GetPriority(void) {
    return Scb::GetPriority<irqn>();
  }
#endif
};

namespace CoreException
{
  /* fixed core exceptions */
  typedef CoreExceptionImpl<-15>  Reset;       // TODO: priority is fixed to -3
  typedef CoreExceptionImpl<-14>  NMI;         // TODO: priority is fixed to -2
  typedef CoreExceptionImpl<-13>  HardFault;   // TODO: priority is fixed to -1

  /* settable core exceptions */
  typedef CoreExceptionImpl<-12>  MemoryManagement;
  typedef CoreExceptionImpl<-11>  BusFault;
  typedef CoreExceptionImpl<-10>  UsageFault;
  typedef CoreExceptionImpl<-5>   SVCall;
  typedef CoreExceptionImpl<-4>   DebugMonitor;
  typedef CoreExceptionImpl<-2>   PendSV;
  typedef CoreExceptionImpl<-1>   SysTick;

  static constexpr bool reserved_irqn(int irqn) {
    return ((irqn == -3) ||
            (irqn == -6) ||
            (irqn == -7) ||
            (irqn == -8) ||
            (irqn == -9));
  }
}


////////////////////  Irq  ////////////////////


template<unsigned int irqn>
class IrqChannel : public NvicPriority<irqn> {

  static constexpr std::size_t  reg_index = (uint32_t)irqn >> 5;
  static constexpr std::size_t  irq_bit = 1 << ((uint32_t)irqn & 0x1F);

  using NVIC  = reg::NVIC;
  using ISERx = NVIC::ISER<reg_index>;
  using ICERx = NVIC::ICER<reg_index>;
  using ISPRx = NVIC::ISPR<reg_index>;
  using ICPRx = NVIC::ICPR<reg_index>;
  using IABRx = NVIC::IABR<reg_index>;
  using IPRx  = NVIC::IPR<irqn>;

public:

  static constexpr int irq_number = irqn;

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

  static void set_priority(uint32_t priority) {
    IPRx::store((priority << (8 - InterruptControllerSetup::priority_bits)) & 0xff);
  }

  static uint32_t get_priority(void) {
    return((uint32_t)(IPRx::load() >> (8 - InterruptControllerSetup::priority_bits)));
  }
};

#endif // COMMON_ARM_CORTEX_NVIC_HPP_INCLUDED
