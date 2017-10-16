/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright (C) 2012-2017 Axel Burri <axel@tty0.ch>
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

#ifndef ISR_HPP_INCLUDED
#define ISR_HPP_INCLUDED

#include <typelist.hpp>
#include <simulation.hpp>

namespace mptl {

typedef void( *const isr_t )( void );

struct irq_handler_base
: public typelist_unique_element< irq_handler_base >
{ };

template<typename Tp, isr_t isr>
struct irq_handler : irq_handler_base
{
  using irq_type = Tp;
  static constexpr isr_t value = isr;

#ifdef OPENMPTL_SIMULATION
  static void dump_type(void) {
    std::string s = __PRETTY_FUNCTION__;
    std::cout << "irq_handler<Tp, isr> " << s.substr(s.find('['), s.rfind(']')) << std::endl;
  }
#endif
};


namespace mpl
{
  template<int _irqn>
  struct filter_irqn {
    template<typename T>
    using filter = std::integral_constant< bool, (T::irq_type::irqn == _irqn) >;
  };

  /** typelist, filtered to only hold irq_handler_base type */
  template<typename typelist_type>
  using irq_handler_list = typename typelist_type::template filter_type< irq_handler_base >;

  /** typelist, filtered to only hold irq_handler with given irq number (int irqn) */
  template<typename typelist_type, int irqn>
  using irqn_list = typename irq_handler_list<typelist_type>::template filter< filter_irqn< irqn > >;


  /**
   * Provides the mptl::irq_handler<> element type from a given typelist<> and
   * irq number (int irqn). 
   *
   * NOTE: Asserts at compile-time (static_assert) that the returned
   * irq_handler is unique in the typelist.
   */
  template<typename typelist_type, int irqn>
  using unique_irq_handler = typename irqn_list<typelist_type, irqn>::unique_element::type;

#ifdef OPENMPTL_SIMULATION
  template<typename... Tp>
  struct dump_irq_types {
    void operator()() { };
  };

  template<typename Head, typename... Tp>
  struct dump_irq_types<Head, Tp...> {
    void operator()() {
      std::cout << std::setw(3) << Head::irq_type::irqn << " : ";
      Head::dump_type();
      dump_irq_types<Tp...>()();
    };
  };
#endif // OPENMPTL_SIMULATION

} // namespace mpl

} // namespace mptl

#endif // ISR_HPP_INCLUDED
