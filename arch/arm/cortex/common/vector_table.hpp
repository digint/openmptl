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
#include <resource_mpl.hpp>
#include <type_traits>

#ifndef CORE_SIMULATION
static_assert(sizeof(isr_t)  == 4, "wrong size for irq function pointer");
static_assert(alignof(isr_t) == 4, "wrong alignment for irq function pointer table");
#endif

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
  static constexpr std::size_t core_exceptions = 16;  /* this includes the stack_top pointer */
  static constexpr std::size_t irq_channels    = sizeof...(irqs);
  static constexpr std::size_t size            = core_exceptions + irq_channels;
  
  static isr_t vector_table[size] __attribute__ ((section(".isr_vector")));

  /* Build the vector table by declaring a pointer to it */
  isr_t *vector_table_p = vector_table;
};

template<const uint32_t *stack_top, typename... irqs>
isr_t VectorTableImpl<stack_top, irqs...>::vector_table[size] = {
  reinterpret_cast<isr_t>(stack_top),

  /* fixed core exceptions */
  CoreException::Reset            ::isr,
  CoreException::NMI              ::isr,
  CoreException::HardFault        ::isr,

  /* settable core exceptions */
  CoreException::MemoryManagement ::isr,
  CoreException::BusFault         ::isr,
  CoreException::UsageFault       ::isr,
  0, 0, 0, 0,    /* reserved */
  CoreException::SVCall           ::isr,
  CoreException::DebugMonitor     ::isr,
  0,             /* reserved */
  CoreException::PendSV           ::isr,
  CoreException::SysTick          ::isr,

  /* device specific irq channels */
  irqs::Handler::isr...
};


template<typename R, unsigned int N, const uint32_t *stack_top, typename... irqs>
struct VectorTableBuilderImpl {
  typedef typename VectorTableBuilderImpl<
    R,
    N - 1,
    stack_top,
    typename std::conditional<  /* take handler from ResourceList if present */
      std::is_void< typename R::template irq_resource< IrqChannel< N - 1 > >::type >::value,
      IrqChannel<N - 1>,
      typename R::template irq_resource< IrqChannel< N - 1 > >::type
      >::type,
    irqs...
    >::type type;
};

template<typename R, const uint32_t *stack_top, typename... irqs>
struct VectorTableBuilderImpl<R, 0, stack_top, irqs...> {
  typedef VectorTableImpl<stack_top, irqs...> type;
};


/**
 * VectorTableBuilder: Provides a VectorTableImpl by setting the
 * number of irq channels and the top-of-stack.
 */
template<typename R, std::size_t irq_channels, const uint32_t *stack_top>
struct VectorTableBuilder : VectorTableBuilderImpl<R, irq_channels, stack_top>::type
{
#ifndef CORE_SIMULATION
  static_assert(sizeof(VectorTableBuilderImpl<R, irq_channels, stack_top>::type::vector_table)
                == 4 * (irq_channels + VectorTableImpl<stack_top>::core_exceptions),
                "IRQ vector table size error");
#endif
};


/* Explicit template instantiation (does not work, vector_table is stripped)  */
// template class VectorTable<82, &_stack_top>;

#endif // COMMON_ARM_CORTEX_VECTOR_TABLE_HPP_INCLUDED
