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

#include <core.hpp>
#include <irq_shell.hpp>
#include <cstdint>

extern uint32_t _data_lma; /* load address of data section */
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern void (*__init_array_start []) (void);
extern void (*__init_array_end   []) (void);
extern void (*__fini_array_start []) (void);
extern void (*__fini_array_end   []) (void);


class CoreStartup : public Core {
public:
  static void init_data_section(void) {
    uint32_t *src = &_data_lma;
    uint32_t *dst = &_sdata;
    while(dst < &_edata)
      *(dst++) = *(src++);
  }

  static void init_bss_section(void) {
    uint32_t *dst = &_sbss;
    while(dst < &_ebss)
      *(dst++) = 0;
  }

  static void call_ctors(void) {
    uintptr_t n = __init_array_end - __init_array_start;
    uintptr_t i = 0;
    while(i < n)
      __init_array_start[i++]();
  }

  static void call_dtors(void) {
    uintptr_t n = __fini_array_end - __fini_array_start;
    uintptr_t i = 0;
    while(i < n)
      __fini_array_start[i++]();
  }
};


struct CoreStartupIrqShell : public IrqShell {
  CoreStartupIrqShell() {
    CoreStartup::init_data_section();
    CoreStartup::init_bss_section();
    CoreStartup::InitHW();    /* init hardware BEFORE calling ctors, they might depend on it */
    CoreStartup::call_ctors();
  };
  ~CoreStartupIrqShell() { 
    CoreStartup::call_dtors();
  };
};
