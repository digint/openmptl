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
#include <typelist.hpp>
#include <simulation.hpp>
#include <type_traits>
#include <compiler.h>

namespace mptl {

#ifndef OPENMPTL_SIMULATION
static_assert(sizeof(isr_t)       == 4, "wrong size for isr function pointer");
static_assert(alignof(isr_t)      == 4, "wrong alignment for isr function pointer table");
static_assert(sizeof(uint32_t *)  == 4, "wrong size for top-of-stack pointer");
static_assert(alignof(uint32_t *) == 4, "wrong alignment for top-of-stack pointer");
#endif

template<std::size_t vt_size>
struct arm_cortex_vector_table {
  const uint32_t * stack_top;
  isr_t isr_vector[vt_size];
};


namespace mpl
{
  /**
   * Provides a arm_cortex_vector_table in static member "value" which
   * can be used to fill linker section ".isr_vector", containing
   * stack_top and all values of the mptl::irq_handler<> traits.
   *
   * Template arguments:
   *
   *   - stack_top:  pointer to reset value of stack pointer
   *   - Tp:         mptl::irq_handler<> traits
   */
  template<const uint32_t *stack_top, typename... Tp>
  struct vector_table_impl {
    static constexpr std::size_t vt_size = sizeof...(Tp);
    static constexpr std::size_t size = vt_size + 1;
    static constexpr arm_cortex_vector_table<vt_size> value = { stack_top, { Tp::value... } };

#ifdef OPENMPTL_SIMULATION
    /** Dump demangled irq_handler types to std::cout */
    static void dump_types(void) {
      std::cout << "*** irq handler types:" << std::endl;
      dump_irq_types<Tp...>()();
    }
#endif

#if 0
    /* With gcc 4.x, the section attribute was honored within static
     * template member variables. To create the vector table, the
     * following statement was used:
     *
     *     mptl::vector_table< &_stack_top, resources > vector_table;
     *
     * As of gcc 5.x (as well as clang), this does not work any more,
     * section attributes are not honored for static template
     * variables. Maybe related to:
     * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=70435
     */
    static isr_t isr_vector[size] __used __attribute__ ((section(".isr_vector")));
#endif
  };

#if 0
  /* see comment above */
  template<const uint32_t *stack_top, typename... Tp>
  isr_t vector_table_impl<stack_top, Tp...>::isr_vector[size] = {
    (isr_t)stack_top,
    Tp::value...
  };
#endif

  /** recursively build the vector table  */
  template< unsigned int N,
            int          irqn_offset,
            typename     irq_handler_list,
            isr_t        default_isr,
            const        uint32_t *stack_top,
            typename...  Tp >
  struct make_vector_table
  {
    static constexpr int irqn = (N - 1) + irqn_offset;
    using irq_handler_resource = mpl::unique_irq_handler<irq_handler_list, irqn>;
    using irq_handler_default = irq_handler< irq_base< irqn >, default_isr >;

    /** irq_handler<> from irq_handler<irqn> in irq_handler_list if
     *  present, default_isr if not. default_isr if irqn is a
     *  reserved_irqn() */
    using irq_handler_type = typename std::conditional<
      ( irq::reserved_irqn(irqn) ||
        std::is_void< irq_handler_resource >::value ),
      irq_handler_default,
      irq_handler_resource >::type;

    /** recursion */
    using type = typename make_vector_table<
      N - 1,
      irqn_offset,
      irq_handler_list,
      default_isr,
      stack_top,
      irq_handler_type,
      Tp...
      >::type;
  };

  /* last irq number (N) is irq::reset::irqn */
  template< int              irqn_offset,
            typename         irq_handler_list,
            isr_t            default_isr,
            const uint32_t * stack_top,
            typename...      Tp >
  struct make_vector_table< 0, irqn_offset, irq_handler_list, default_isr, stack_top, Tp... > {
    using type = vector_table_impl<stack_top, Tp...>;
  };
} // namespace mpl


/**
 * Provides a static vector table (value[], see vector_table_impl
 * above), to be initialized in section ".isr_vector".
 *
 * Example:
 *
 *     extern const uint32_t _stack_top; // provided by linker script
 *     using resources = mptl::typelist<
 *       mptl::irq_handler< typename mptl::irq::reset, my_reset_isr >,
 *       ...
 *       >;
 *     using vector_table = mptl::vector_table<&_stack_top, resources>;
 *     const auto isr_vector __attribute__((used, section(".isr_vector"))) = vector_table::value;
 *
 * Template arguments:
 *
 *   - stack_top: pointer to reset value of stack pointer
 *
 *   - irq_handler_list: typelist<>, containing mptl::irq_handler<>
 *        traits (other traits are ignored)
 *
 *   - default_isr: isr_t function pointer, used for all irq's which
 *        are not listed in irq_handler_list. Defaults to "nullptr".
 *
 */
template<const uint32_t *stack_top, typename irq_handler_list, isr_t default_isr = nullptr >
struct vector_table
: mpl::make_vector_table<
  irq::numof_interrupt_channels - irq::reset::irqn,  /* start index */
  irq::reset::irqn,  /* irqn_offset (negative) */
  irq_handler_list,
  default_isr,
  stack_top
  >::type
{
  using type = vector_table<stack_top, irq_handler_list, default_isr>;

  /** offset of irq_channel<0> in arm_cortex_vector_table::isr_vector[] */
  static constexpr int irq_channel_offset = -irq::reset::irqn;
  static constexpr int numof_core_exceptions = -irq::reset::irqn;

  static_assert(irq::reset::irqn < 0, "irq::reset::irqn must be a negative value");
  static_assert(irq::numof_interrupt_channels >= 0, "invalid irq::numof_interrupt_channels");
  static_assert(sizeof(type::value) == sizeof(isr_t) * (1 + irq::numof_interrupt_channels + numof_core_exceptions),
                "IRQ vector table size error");

#ifdef OPENMPTL_SIMULATION
  void dump_size(void) {
    int w = 3;
    std::cout << "*** vector table size:" << std::endl;
    std::cout << "stack_top pointer:      " << std::setw(w) << 1 << std::endl;
    std::cout << "cortex core exceptions: " << std::setw(w) << (this->size - irq::numof_interrupt_channels - 1) << std::endl;
    std::cout << "irq channels:           " << std::setw(w) << irq::numof_interrupt_channels << std::endl;
    std::cout << "total size:             " << std::setw(w) << this->size << std::endl;
  }

  /**
   * Dump the isr_vector table.
   *
   * Not very useful, as it prints the pointers to irq_handler from
   * simulation functions.
   */
  void dump_vector(void) {
    std::cout << "*** vector table dump:" << std::endl;
    //    std::cout << "-----------------" << std::endl;

    for(int i = 0; i < (int)this->size; i++) {
      std::cout << std::dec << std::setw(3) << i << " " <<
        "(" << std::setw(3) << i - this->irq_channel_offset << ")" <<
        " :  0x" << std::hex << (unsigned long)this->isr_vector[i] << std::endl;
    }
  }
#endif // OPENMPTL_SIMULATION
};

} // namespace mptl

#endif // ARM_CORTEX_COMMON_VECTOR_TABLE_HPP_INCLUDED
