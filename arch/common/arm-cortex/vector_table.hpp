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


#ifndef COMMON_ARM_CORTEX_VECTOR_TABLE_HPP_INCLUDED
#define COMMON_ARM_CORTEX_VECTOR_TABLE_HPP_INCLUDED

#include <arch/nvic.hpp>

static_assert(sizeof(irq_handler_t)  == 4, "wrong size for irq function pointer");
static_assert(alignof(irq_handler_t) == 4, "wrong alignment for irq function pointer table");


template<int N, const uint32_t *stack_top, typename... irqs>
struct VectorTableBuilder : VectorTableBuilder<N - 1, stack_top, Irq<N - 1>, irqs... >
{ };

template<const uint32_t *stack_top, typename... irqs>
struct VectorTableBuilder<0, stack_top, irqs...> {
  static const irq_handler_t vector_table[sizeof...(irqs) + NvicCortexSetup::core_exceptions] __attribute__ ((section(".isr_vector")));
};

template<const uint32_t *stack_top, typename... irqs>
const irq_handler_t VectorTableBuilder<0, stack_top, irqs...>::vector_table[sizeof...(irqs) + NvicCortexSetup::core_exceptions] = {
  reinterpret_cast<irq_handler_t>(stack_top),

  /* fixed core exceptions */
  CoreExceptionReset     ::Handler,
  CoreExceptionNMI       ::Handler,
  CoreExceptionHardFault ::Handler,

  /* settable core exceptions */
  CoreException< CoreExceptionNumber::MemoryManagement >::Handler,
  CoreException< CoreExceptionNumber::BusFault         >::Handler,
  CoreException< CoreExceptionNumber::UsageFault       >::Handler,
  0, 0, 0, 0,    /* reserved */
  CoreException< CoreExceptionNumber::SVCall           >::Handler,
  CoreException< CoreExceptionNumber::DebugMonitor     >::Handler,
  0,             /* reserved */
  CoreException< CoreExceptionNumber::PendSV           >::Handler,
  CoreException< CoreExceptionNumber::SysTick          >::Handler,

  /* nvic irq channels */
  irqs::Handler...
};


/* Explicit template instantiation (does not work, vector_table is stripped)  */
// template class VectorTable<82, &_eram>;

#endif // COMMON_ARM_CORTEX_VECTOR_TABLE_HPP_INCLUDED
