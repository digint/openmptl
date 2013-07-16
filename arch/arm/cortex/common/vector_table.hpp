/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright 2013 Axel Burri <axel@tty0.ch>
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

#ifndef ARM_CORTEX_COMMON_VECTOR_TABLE_HPP_INCLUDED
#define ARM_CORTEX_COMMON_VECTOR_TABLE_HPP_INCLUDED

#include <arch/nvic.hpp>
#include <resource.hpp>
#include <type_traits>
#include <compiler.h>

namespace mptl {

#ifndef OPENMPTL_SIMULATION
static_assert(sizeof(isr_t)  == 4, "wrong size for isr function pointer");
static_assert(alignof(isr_t) == 4, "wrong alignment for isr function pointer table");
#endif

namespace mpl
{
  /**
   * vector_table_impl: Provides a static vector table isr_vector[]:
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

    static __used isr_t isr_vector[size] __attribute__ ((section(".isr_vector")));
  };

  template<const uint32_t *stack_top, isr_t... isr_list>
  isr_t vector_table_impl<stack_top, isr_list...>::isr_vector[size] = {
    reinterpret_cast<isr_t>(stack_top),
    isr_list...
  };


  template<unsigned int N, int irqn_offset, typename resource_list, isr_t default_isr, const uint32_t *stack_top, isr_t... isr_list>
  struct make_vector_table
  {
    static constexpr int irqn = (N - 1) + irqn_offset;
    using irq_handler = mpl::unique_irq_handler<resource_list, irqn>;

    struct irq_handler_default {
      static constexpr isr_t value = default_isr;
    };

    static constexpr isr_t isr = irq::reserved_irqn(irqn) ? nullptr :
      std::conditional<  /* handler from mpl::irq_handler<irqn> in resource::list if present */
        std::is_void< irq_handler >::value,
        irq_handler_default,
        irq_handler
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

  /* last irq number (N) is irq::reset::irqn */
  template<int irqn_offset, typename resource_list, isr_t default_isr, const uint32_t *stack_top, isr_t... isr_list>
  struct make_vector_table<0, irqn_offset, resource_list, default_isr, stack_top, isr_list...> {
    using type = vector_table_impl<stack_top, isr_list...>;
  };
} // namespace mpl


/**
 * vector_table: Provides a static vector table.
 *
 * Instantiate this class somewhere in your startup code to put the
 * table to the ".isr_vector" section.
 */
template<const uint32_t *stack_top, typename resource_list, isr_t default_isr = nullptr >
struct vector_table
: mpl::make_vector_table<
  irq::numof_interrupt_channels - irq::reset::irqn,  /* start index */
  irq::reset::irqn,  /* irqn_offset (negative) */
  resource_list,
  default_isr,
  stack_top
  >::type
{
  /** offset of irq_channel<0> in isr_vector[] */
  static constexpr int irq_channel_offset = -irq::reset::irqn + 1;

  static_assert(irq::reset::irqn < 0, "irq::reset::irqn must be a negative value");
  static_assert(irq::numof_interrupt_channels >= 0, "invalid irq::numof_interrupt_channels");
  static_assert(sizeof(vector_table<stack_top, resource_list, default_isr>::isr_vector) == sizeof(isr_t) * (1 + irq::numof_interrupt_channels + -irq::reset::irqn),
                "IRQ vector table size error");
};

} // namespace mptl

#endif // ARM_CORTEX_COMMON_VECTOR_TABLE_HPP_INCLUDED
