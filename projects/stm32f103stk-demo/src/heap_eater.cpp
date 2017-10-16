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

#include "terminal_hooks.hpp"

#ifndef OPENMPTL_SIMULATION

#include <cstring> // memset

extern char *heap_ptr;
#ifdef HEAP_FIXED_SIZE
extern char *heap_end;
#else
register char *heap_end asm ("sp");  /* heap_end is current stack pointer */
#endif

template<unsigned size>
struct adams_buf {
  char buf[size];
  adams_buf() { std::memset(buf, 42, sizeof(buf)); }
};

using buf = adams_buf<1024>;

buf *buf_p;

void terminal_hooks::heap_eater::run(poorman::ostream<char> & cout) {
  cout << "new buf(): size=" << sizeof(buf) << poorman::endl;
  cout << "heap_ptr: old=" << (unsigned)heap_ptr << " end=" << (unsigned)heap_end << poorman::endl;
  buf_p = new buf();
  cout << "heap_ptr: new=" << (unsigned)heap_ptr << " end=" << (unsigned)heap_end << poorman::endl;
}

#else ///// OPENMPTL_SIMULATION /////

void terminal_hooks::heap_eater::run(poorman::ostream<char> & cout) {
  cout << "OpenMPTL simulation: HeapEater dummy" << poorman::endl;
}

#endif // OPENMPTL_SIMULATION
