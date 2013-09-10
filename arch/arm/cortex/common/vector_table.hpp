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
static_assert(sizeof(isr_t)  == 4, "wrong size for isr function pointer");
static_assert(alignof(isr_t) == 4, "wrong alignment for isr function pointer table");
#endif

namespace mpl
{
  /**
   * Provides a static vector table isr_vector[] in section
   * ".isr_vector", containing stack_top and all values of the
   * mptl::irq_handler<> traits.
   *
   * Template arguments:
   *
   *   - stack_top:  pointer to reset value of stack pointer
   *   - Tp:         mptl::irq_handler<> traits
   */
  template<const uint32_t *stack_top, typename... Tp>
  struct vector_table_impl {
    static constexpr std::size_t size = sizeof...(Tp) + 1;

    static __used isr_t isr_vector[size] __attribute__ ((section(".isr_vector")));

#ifdef OPENMPTL_SIMULATION
    /** Dump demangled irq_handler types to std::cout */
    static void dump_types(void) {
      std::cout << "*** irq handler types:" << std::endl;
    //      std::cout << "------------------" << std::endl;
      dump_irq_types<Tp...>()();
    }
#endif
  };

  template<const uint32_t *stack_top, typename... Tp>
  isr_t vector_table_impl<stack_top, Tp...>::isr_vector[size] = {
    (isr_t)stack_top,
    Tp::value...
  };


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
 * Provides a static vector table isr_vector[] in section
 * ".isr_vector".
 *
 * NOTE: If your compiler does not support the "used" attribute, you
 * will have to instantiate this class somewhere so that the compiler
 * does not strip the unreferenced symbol.
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
 * Members:
 * 
 *   - isr_vector[]: interrupt vector table, "__used" in section
 *       ".isr_vector" (__attribute__((used)) __attribute__ ((section(".isr_vector"))).
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
  /** offset of irq_channel<0> in isr_vector[] */
  static constexpr int irq_channel_offset = -irq::reset::irqn + 1;

  static_assert(irq::reset::irqn < 0, "irq::reset::irqn must be a negative value");
  static_assert(irq::numof_interrupt_channels >= 0, "invalid irq::numof_interrupt_channels");
  static_assert(sizeof(vector_table<stack_top, irq_handler_list, default_isr>::isr_vector) == sizeof(isr_t) * (1 + irq::numof_interrupt_channels + -irq::reset::irqn),
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
