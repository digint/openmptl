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
#include <resource.hpp>
#include <type_traits>

#ifndef CORE_SIMULATION
static_assert(sizeof(isr_t)  == 4, "wrong size for isr function pointer");
static_assert(alignof(isr_t) == 4, "wrong alignment for isr function pointer table");
#endif

namespace mpl
{
  /**
   * vector_table_impl: Provides a static vector table:
   *
   * - pointer to reset value of stack pointer
   * - pointer to N handler (isr_list)
   *
   * Instantiating this class puts the table to the ".isr_vector" section.
   *
   */
  template<const uint32_t *stack_top, isr_t... isr_list>
  struct vector_table_impl {
    static constexpr std::size_t size = sizeof...(isr_list) + 1;

    static isr_t vector_table[size] __attribute__ ((section(".isr_vector")));

    /* Build the vector table by declaring a pointer to it */
    isr_t *vector_table_p = vector_table;
  };

  template<const uint32_t *stack_top, isr_t... isr_list>
  isr_t vector_table_impl<stack_top, isr_list...>::vector_table[size] = {
    reinterpret_cast<isr_t>(stack_top),
    isr_list...
  };


  template<unsigned int N, int irqn_offset, typename resource_list, isr_t default_isr, const uint32_t *stack_top, isr_t... isr_list>
  struct make_vector_table
  {
    static constexpr int irqn = (N - 1) + irqn_offset;
    struct default_irq_resource {
      static constexpr isr_t value = default_isr;
    };

    static constexpr isr_t isr = irq::reserved_irqn(irqn) ? nullptr :
      std::conditional<  /* handler from IrqResource<irqn> in ResourceList if present */
      std::is_void<typename resource_list::template irq_resource<irqn>::type>::value,
      default_irq_resource,
      typename resource_list::template irq_resource<irqn>::type
      >::type::value;

    using type = typename make_vector_table<
      N - 1,
      irqn_offset,
      resource_list,
      default_isr,
      stack_top,
      isr,
      isr_list...
      >::type;
  };

  /* last irq number (N) is irq::Reset::irqn */
  template<int irqn_offset, typename resource_list, isr_t default_isr, const uint32_t *stack_top, isr_t... isr_list>
  struct make_vector_table<0, irqn_offset, resource_list, default_isr, stack_top, isr_list...> {
    using type = vector_table_impl<stack_top, isr_list...>;
  };
} // namespace mpl


/**
 * VectorTable: Provides a static vector table.
 *
 * Instantiate this class somewhere in your startup code to put the
 * table to the ".isr_vector" section.
 */
template<const uint32_t *stack_top, typename resource_list, isr_t default_isr = nullptr >
struct VectorTable
: mpl::make_vector_table<
  irq::numof_interrupt_channels - irq::Reset::irqn,  /* start index */
  irq::Reset::irqn,  /* irqn_offset (negative) */
  resource_list,
  default_isr,
  stack_top
  >::type
{
  static constexpr std::size_t irq_channel_offset = -irq::Reset::irqn + 1;

  static_assert(irq::Reset::irqn < 0, "irq::Reset::irqn must be a negative value");
  static_assert(irq::numof_interrupt_channels >= 0, "invalid Irq::numof_interrupt_channels");
  static_assert(sizeof(VectorTable<stack_top, resource_list, default_isr>::vector_table) == sizeof(isr_t) * (1 + irq::numof_interrupt_channels + -irq::Reset::irqn),
                "IRQ vector table size error");
};

/* Explicit template instantiation (does not work, vector_table is stripped)  */
// template class VectorTable<82, &_stack_top>;

#endif // COMMON_ARM_CORTEX_VECTOR_TABLE_HPP_INCLUDED
