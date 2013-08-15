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
  vector_table<&stack_top, resource_list, default_isr> vt;

#ifdef UNITTEST_MUST_FAIL
  // fail: list contains more than one element
  vector_table<&stack_top, resource_fail_list, default_isr> vt_fail;
#endif

  //  resource_list::check();
  //  resource_list::configure();

  isr_t *isr_vector = vt.isr_vector;

  assert((unsigned long)isr_vector[0] == (unsigned long)&stack_top);
  assert((unsigned long)isr_vector[vt.irq_channel_offset + 41] == (unsigned long)default_isr);
  assert((unsigned long)isr_vector[vt.irq_channel_offset + 42] == (unsigned long)isr_42);


  std::cout << "vector table size = " << vt.size << " (" <<
    "1 stack_top_ptr, " <<
    irq::numof_interrupt_channels << " irq_channels)" << std::endl;

  std::cout << std::endl;
  std::cout << "vector table dump" << std::endl;
  std::cout << "----------------------" << std::endl;

  for(unsigned i = 0; i < vt.size; i++) {
    std::cout << std::dec << std::setw(3) << i << " " <<
      "(" << std::setw(3) << (int)i - (int)vt.irq_channel_offset << ")" <<
      " :  0x" << std::hex << (unsigned long)isr_vector[i] << std::endl;
  }

  return 0;
}
