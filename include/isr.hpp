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

#ifndef ISR_HPP_INCLUDED
#define ISR_HPP_INCLUDED

#include <resource.hpp>

namespace mptl {

typedef void( *const isr_t )( void );

struct irq_handler_base
: public typelist_unique_element< irq_handler_base >
{ };

template<typename _irq_type, isr_t isr>
struct irq_handler : irq_handler_base
{
  using irq_type = _irq_type;
  static constexpr isr_t value = isr;
};


namespace resource
{
  namespace filter
  {
    template<int _irqn>
    struct irqn {
      template<typename T>
      using filter = std::integral_constant< bool, (T::irq_type::irqn == _irqn) >;
    };
  } // namespace filter

  template<typename list_type>
  using irq_handler_list = typename list_type::template filter_type< irq_handler_base >::type;

  template<typename list_type, int irqn>
  using irqn_list = typename irq_handler_list<list_type>::template filter< resource::filter::irqn< irqn > >::type;

  template<typename list_type, int irqn>
  using irq_handler = typename irqn_list<list_type, irqn>::unique_element::type;
} // namespace resource

} // namespace mptl

#endif // ISR_HPP_INCLUDED
