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

#include "terminal_hooks.hpp"

#ifndef OPENMPTL_SIMULATION

extern char *heap_ptr;
#ifdef HEAP_FIXED_SIZE
extern char *heap_end;
#else
register char *heap_end asm ("sp");  /* heap_end is current stack pointer */
#endif

struct A {
  char buf[1024];
  A() { for(int i = 0; i < 1024; i++) buf[i] = 42; };
};

A *heap_eater;

void terminal_hooks::HeapEater::run(poorman_ostream<char> & cout) {
  cout << "new A(): size=" << sizeof(A) << endl;
  cout << "heap_ptr: old=" << (unsigned)heap_ptr << " end=" << (unsigned)heap_end << endl;
  heap_eater = new A();
  cout << "heap_ptr: new=" << (unsigned)heap_ptr << " end=" << (unsigned)heap_end << endl;
}

#else ///// OPENMPTL_SIMULATION /////

void terminal_hooks::HeapEater::run(poorman_ostream<char> & cout) {
  cout << "OpenMPTL simulation: HeapEater dummy" << endl;
}

#endif // OPENMPTL_SIMULATION
