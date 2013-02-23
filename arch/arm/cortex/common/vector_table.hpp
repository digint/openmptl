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
template<const uint32_t *stack_top, isr_t... isr_list>
struct VectorTableImpl {
  static constexpr std::size_t size = sizeof...(isr_list) + 1;

  static isr_t vector_table[size] __attribute__ ((section(".isr_vector")));

  /* Build the vector table by declaring a pointer to it */
  isr_t *vector_table_p = vector_table;
};

template<const uint32_t *stack_top, isr_t... isr_list>
isr_t VectorTableImpl<stack_top, isr_list...>::vector_table[size] = {
  reinterpret_cast<isr_t>(stack_top),
  isr_list...
};


template<typename resource_list, int N, const uint32_t *stack_top, isr_t default_isr, isr_t... isr_list>
struct make_vector_table
{
  struct default_isr_resource {
    static constexpr isr_t value = default_isr;
  };

  static constexpr isr_t isr = CoreException::reserved_irqn(N-1) ? nullptr :
    std::conditional<  /* handler from IrqResource<N-1> in ResourceList if present */
    std::is_void<typename resource_list::template irq_resource<N-1>::type>::value,
    default_isr_resource,
    typename resource_list::template irq_resource<N-1>::type
    >::type::value;

  typedef typename make_vector_table<
    resource_list,
    N - 1,
    stack_top,
    default_isr,
    isr,
    isr_list...
    >::type type;
};

/* last irq number (N) is CoreException::Reset::irq_number */
template<typename resource_list, const uint32_t *stack_top, isr_t default_isr, isr_t... isr_list>
struct make_vector_table<resource_list, CoreException::Reset::irq_number, stack_top, default_isr, isr_list...> {
  typedef VectorTableImpl<stack_top, isr_list...> type;
};


/**
 * VectorTable: Provides a static vector table.
 *
 * Instantiate this class somewhere in your starup code to puts the
 * table to the ".isr_vector" section.
 */
template<const uint32_t *stack_top, typename resource_list = ResourceList<>, isr_t default_isr = nullptr >
struct VectorTable : make_vector_table<resource_list, Irq::numof_interrupt_channels, stack_top, default_isr>::type
{
  typedef typename make_vector_table<resource_list, Irq::numof_interrupt_channels, stack_top, default_isr>::type type;

  static_assert(Irq::numof_interrupt_channels >= 0, "invalid Irq::numof_interrupt_channels");
  static_assert(sizeof(type::vector_table) == sizeof(isr_t) * (1 + Irq::numof_interrupt_channels + -CoreException::Reset::irq_number),
                "IRQ vector table size error");
};

/* Explicit template instantiation (does not work, vector_table is stripped)  */
// template class VectorTable<82, &_stack_top>;

#endif // COMMON_ARM_CORTEX_VECTOR_TABLE_HPP_INCLUDED
