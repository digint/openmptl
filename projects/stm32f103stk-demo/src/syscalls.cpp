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

#include "kernel.hpp"
#include "terminal_hooks.hpp"


#ifndef CORE_SIMULATION

#include "printf.h"

#include <unistd.h>
#include <stdint.h>
#include <errno.h>

/* define a fixed heap size for tesing purposes */
//#define HEAP_FIXED_SIZE (1024 * 8)

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HEAP_FIXED_SIZE
char heap[HEAP_FIXED_SIZE];
char *heap_ptr = heap;
char *heap_end = heap + HEAP_FIXED_SIZE;
#else
/* NOTE: This assumes that the stack is growing down towards the heap */
extern char end[];
char *heap_ptr = end;
register char *heap_end asm ("sp");  /* heap_end is current stack pointer */
#endif

caddr_t _sbrk(ptrdiff_t incr)
{
  char *chunk_start = heap_ptr;

  if(heap_ptr + incr > heap_end)
  {
    errno = ENOMEM;
    return (caddr_t) -1;
  }
  heap_ptr += incr;
  return (caddr_t)chunk_start;
}

int _kill(int pid, int sig)
{
  (void)pid; (void)sig;
  errno = ENOTSUP;
  return -1;
}

void _exit(int status)
{
  _kill(status, -1);

  Kernel::led::on();

  /* Try to print out-of-memory message to LCD */
  Kernel::lcd lcd;
  char buf[16];

  lcd.configure();
  lcd.clear();
  lcd.print_line(0, "oops...");
  lcd.print_line(1, "out of memory");
  sprintf(buf, "stat: %08x", status);
  lcd.print_line(2, buf);
  sprintf(buf, "heap: %08x", heap_ptr);
  lcd.print_line(3, buf);
  sprintf(buf, "hend: %08x", heap_end);
  lcd.print_line(4, buf);
  lcd.update();

  while (1);
}

int _getpid(int n)
{
  (void)n;
  return 1;
}

#ifdef __cplusplus
}
#endif


struct A {
  char buf[1024];
  A() { for(int i = 0; i < 1024; i++) buf[i] = 42; };
};

A *a;

void terminal_hooks::SyscallsTest::run(poorman_ostream<char> & cout) {
  cout << "new A(): size=" << sizeof(A) << endl;
  cout << "heap_ptr: old=" << (unsigned)heap_ptr << " end=" << (unsigned)heap_end << endl;
  a = new A();
  cout << "heap_ptr: new=" << (unsigned)heap_ptr << " end=" << (unsigned)heap_end << endl;
}

#else

void terminal_hooks::SyscallsTest::run(poorman_ostream<char> & cout) {
  cout << "core_simulation: empty syscalls test" << endl;
}

#endif // CORE_SIMULATION
