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

#include <arch/core.hpp>
#include <arch/scb.hpp>
#include <core_setup.hpp>
#include <irq_wrap.hpp>


typedef void( *const irq_handler_t )( void );


struct NvicCortexSetup {
  static constexpr std::size_t core_exceptions = 16;
};


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
class CoreExceptionImpl : public NvicPriority<static_cast<int>(irqn)> {
  static_assert(irqn < 0 && irqn > -16, "illegal core exception interrupt number");
public:

  static constexpr int irq_number = irqn;

  static void Handler(void) {
    DefaultIrqWrap wrap;
  }

  static void SetPriority(uint32_t priority) {
    Scb::SetPriority<irqn>(priority);
  }

  static uint32_t GetPriority(void) {
    return Scb::GetPriority<irqn>();
  }
};


namespace CoreException {
  struct Reset {
    /* no default handler, this one MUST be implemented. */
    static void Handler(void);
  };

  struct HardFault {
    static void Handler(void) {
      DefaultIrqWrap wrap;
    }
  };

  struct NMI {
    static void Handler(void) {
      DefaultIrqWrap wrap;
    }
  };

  typedef CoreExceptionImpl<-12>  MemoryManagement;
  typedef CoreExceptionImpl<-11>  BusFault;
  typedef CoreExceptionImpl<-10>  UsageFault;
  typedef CoreExceptionImpl<-5>   SVCall;
  typedef CoreExceptionImpl<-4>   DebugMonitor;
  typedef CoreExceptionImpl<-2>   PendSV;
  typedef CoreExceptionImpl<-1>   SysTick;
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

  // static void Handler(void)  __attribute__((__interrupt__)) {
  static void Handler(void) {
    DefaultIrqWrap wrap;
  }

  static void Enable(void) {
    ISERx::store(irq_bit);
  }
  static void Disable(void) {
    ICERx::store(irq_bit);
  }

  static bool IsPending(void) {
    return ISPRx::load() & irq_bit;
  }
  static void SetPending(void) {
    ISPRx::store(irq_bit);
  }

  static void ClearPending(void) {
    ICPRx::store(irq_bit);
  }
  static bool IsActive(void) {
    return IABRx::load() & (irq_bit);
  }

  static void SetPriority(uint32_t priority) {
    IPRx::store((priority << (8 - InterruptControllerSetup::priority_bits)) & 0xff);
  }

  static uint32_t GetPriority(void) {
    return((uint32_t)(IPRx::load() >> (8 - InterruptControllerSetup::priority_bits)));
  }
};



#endif // COMMON_ARM_CORTEX_NVIC_HPP_INCLUDED
