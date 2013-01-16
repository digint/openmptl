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

/**
 * VectorTableImpl: Provides a static vector table:
 *
 * - pointer to reset value of stack pointer
 * - pointer to 16 exception vector handler
 * - pointer to N irq channel handler
 *
 * Instantiating this class puts the table to the ".isr_vector" section.
 *
 */
template<const uint32_t *stack_top, typename... irqs>
struct VectorTableImpl {
  static constexpr std::size_t core_exceptions = NvicCortexSetup::core_exceptions;
  static constexpr std::size_t irq_channels    = sizeof...(irqs);
  static constexpr std::size_t table_size      = core_exceptions + irq_channels;
  
  static const irq_handler_t vector_table[table_size] __attribute__ ((section(".isr_vector")));

  /* Build the vector table by declaring a pointer to it */
  irq_handler_t *vector_table_start = vector_table;
};

template<const uint32_t *stack_top, typename... irqs>
const irq_handler_t VectorTableImpl<stack_top, irqs...>::vector_table[table_size] = {
  reinterpret_cast<irq_handler_t>(stack_top),

  /* fixed core exceptions */
  CoreException::Reset            ::Handler,
  CoreException::NMI              ::Handler,
  CoreException::HardFault        ::Handler,

  /* settable core exceptions */
  CoreException::MemoryManagement ::Handler,
  CoreException::BusFault         ::Handler,
  CoreException::UsageFault       ::Handler,
  0, 0, 0, 0,    /* reserved */
  CoreException::SVCall           ::Handler,
  CoreException::DebugMonitor     ::Handler,
  0,             /* reserved */
  CoreException::PendSV           ::Handler,
  CoreException::SysTick          ::Handler,

  /* device specific irq channels */
  irqs::Handler...
};


/**
 * VectorTableBuilder: Provides a VectorTableImpl with N irq channels
 */
template<int N, const uint32_t *stack_top, typename... irqs>
struct VectorTableBuilder : VectorTableBuilder<N - 1, stack_top, IrqChannel<N - 1>, irqs... >
{ };

template<const uint32_t *stack_top, typename... irqs>
struct VectorTableBuilder<0, stack_top, irqs...> {
  typedef VectorTableImpl<stack_top, irqs...> value;
};


/* Explicit template instantiation (does not work, vector_table is stripped)  */
// template class VectorTable<82, &_stack_top>;

#endif // COMMON_ARM_CORTEX_VECTOR_TABLE_HPP_INCLUDED
