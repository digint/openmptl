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

#include <arch/vector_table.hpp>
#include <iostream>
#include <cassert>

using namespace mptl;

static unsigned int stack_top = 0;
static int isr_test = 0;

static void default_isr(void) { isr_test = 0; }
static void isr_42(void) { isr_test = 42; }

/* Handler for irq number = 42 */
using irq42 = irq_handler< irq_base<42>, isr_42 >;

using resource_list      = mptl::typelist < irq42 >;
using resource_fail_list = mptl::typelist < irq42, irq42 >;

int main()
{
#ifdef UNITTEST_MUST_FAIL
#warning UNITTEST_MUST_FAIL: list contains more than one element
  using vt_fail = vector_table<&stack_top, resource_fail_list, default_isr>;
  auto arm_vector_table_fail = vt_fail::value;
#endif

  //  resource_list::check();
  //  resource_list::configure();

  using vt = vector_table<&stack_top, resource_list, default_isr>;
  mptl::arm_cortex_vector_table<vt::vt_size> arm_vector_table = vt::value;

  assert((unsigned long)arm_vector_table.stack_top == (unsigned long)&stack_top);
  assert((unsigned long)arm_vector_table.isr_vector[vt::irq_channel_offset + 41] == (unsigned long)default_isr);
  assert((unsigned long)arm_vector_table.isr_vector[vt::irq_channel_offset + 42] == (unsigned long)isr_42);

  vt::dump_size();
  vt::dump_types();
  vt::dump_vector();

  return 0;
}
