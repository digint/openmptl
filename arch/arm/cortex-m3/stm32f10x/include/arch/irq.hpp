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

#ifndef IRQ_HPP_INCLUDED
#define IRQ_HPP_INCLUDED

#include <core.hpp>
#include <arch/nvic.hpp>
#include <arch/scb.hpp>
#include <cppcore_setup.hpp>
#include <irq_shell.hpp>
//#include <functional>

typedef void( *const irq_func_t )( void );


////////////////////  IrqVector  ////////////////////


class IrqVector {
private:
  static irq_func_t vector_table[] __attribute__ ((section(".isr_vector")));
  //    static const std::function<void(void)> vector_table[] __attribute__ ((section(".isr_vector")));
};


////////////////////  IrqBase  ////////////////////


class IrqBase : public InterruptControllerSetup {
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


////////////////////  CoreExceptionReset  ////////////////////


class CoreExceptionReset {
  friend class IrqVector;
private:
  // no default handler, this one MUST be implemented.
//  static void Handler(void)  __attribute__((__interrupt__));
  static void Handler(void);
};


////////////////////  CoreExceptionHardFault  ////////////////////


class CoreExceptionHardFault {
  friend class IrqVector;
private:
//  static void Handler(void)  __attribute__((__interrupt__)) {
  static void Handler(void) {
    DefaultIrqShell shell;
  }
};


////////////////////  CoreExceptionNMI  ////////////////////


class CoreExceptionNMI {
  friend class IrqVector;
private:
//  static void Handler(void)  __attribute__((__interrupt__)) {
  static void Handler(void) {
    DefaultIrqShell shell;
  }
};


////////////////////  CoreException  ////////////////////


template<IrqNumber::CoreException irqn>
class CoreException : public IrqBase {
  friend class IrqVector;

//  static void Handler(void)  __attribute__((__interrupt__)) {
  static void Handler(void) {
    DefaultIrqShell shell;
  }

  static void SetPriority(uint32_t priority) {
    Scb::SetPriority<irqn>(priority);
  }

  static uint32_t GetPriority(void) {
    return Scb::GetPriority<irqn>();
  }
};


////////////////////  Irq  ////////////////////


template<IrqNumber::Interrupt irqn>
class Irq : public IrqBase {
  friend class IrqVector;

//  static const uint32_t irqn = static_cast<uint32_t>(irq_number);

  // TODO: find a way to tell the compiler not to duplicate all this code.
  //       code bloat is about 600 bytes for all functions defined in irq.cpp.
//  static void Handler(void)  __attribute__((__interrupt__)) {
  static void Handler(void) {
    DefaultIrqShell shell;
  }
  //  static const std::function<void(void)> Handler;
  //  static irq_func_t Handler;

public:
  static void Enable(void) {
    Nvic<irqn>::EnableIrq();
  }
  static void Disable(void) {
    Nvic<irqn>::DisableIrq();
  }

  static bool IsPending(void) {
    return Nvic<irqn>::IsPending();
  }
  static void SetPending(void) {
    Nvic<irqn>::SetPending();
  }

  static void ClearPending(void) {
    Nvic<irqn>::ClearPending();
  }
  static bool IsActive(void) {
    return Nvic<irqn>::IsActive();
  }

  static void SetPriority(uint32_t priority) {
    Nvic<irqn>::SetPriority(priority);
  }

  static uint32_t GetPriority(void) {
    return Nvic<irqn>::GetPriority();
  }
};


#endif // IRQ_HPP_INCLUDED
