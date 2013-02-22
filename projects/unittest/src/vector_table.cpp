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

#include <resource_mpl.hpp>
#include <arch/nvic.hpp>
#include <cassert>
#include "unittest_static_assert.hpp"

static unsigned int stack_top = 0;
static int isr_test = 0;

void testfunc() {
}

struct Irq_42 : public IrqChannel<42> {
  struct Handler {
    static void isr(void) { isr_test = 42; }
  };
};

typedef IrqResource< Irq_42 > irq42;


typedef ResourceList < irq42 > resource_list;
typedef ResourceList < irq42, irq42 > resource_fail_list;


/* default isr for all irq channels */
template<unsigned irqn>
struct IrqChannel<irqn>::Handler {
  static constexpr isr_t isr = nullptr;
};

/* default isr for all other core exceptions */
template<int irqn>
isr_t CoreExceptionImpl<irqn>::isr = nullptr;


template<>
struct IrqChannel<10>::Handler {
  static void isr() { isr_test = 10; }
};

template<>
struct IrqChannel<42>::Handler {
  static void isr() { isr_test = 42000; } /* should get overridden by IrqResource<Irq_42> */
};

void Kernel::init() { }
void Kernel::run() { while(1); }

int main()
{
  VectorTable<&stack_top, resource_list> vector_table;

  UNITTEST_STATIC_ASSERT( VectorTable<&stack_top, resource_fail_list> vector_table_fail; )
    

  resource_list::check();

  isr_t *vt = vector_table.vector_table_p;
  
  assert((unsigned long)vt[0] == (unsigned long)&stack_top);
  assert((unsigned long)vt[1] == (unsigned long)CoreException::Reset::isr);
  assert((unsigned long)vt[10 + vector_table.core_exceptions] == (unsigned long)IrqChannel<10>::Handler::isr);
  assert((unsigned long)vt[42 + vector_table.core_exceptions] == (unsigned long)Irq_42::Handler::isr);


  //  p[42 + vector_table.core_exceptions + 1]();
  std::cout << "vector table size = " << vector_table.size << " (" <<
    vector_table.core_exceptions << " core_exceptions, " <<
    vector_table.irq_channels    << " irq_channels)" << std::endl;

  std::cout << std::endl;
  std::cout << "vector table dump" << std::endl;
  std::cout << "-----------------" << std::endl;

  for(unsigned i = 0; i < vector_table.size; i++)
    std::cout << std::dec << std::setw(3) << i << ": 0x" << std::hex << (unsigned long)vt[i] << std::endl;

  return 0;
}
