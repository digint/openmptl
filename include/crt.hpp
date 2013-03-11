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

/*
 * NOTE: In order to use global objects, you need to define:
 *
 *   void *__dso_handle;
 */

#ifndef CRT_HPP_INCLUDED
#define CRT_HPP_INCLUDED

#include <arch/core.hpp>
#include <irq_wrap.hpp>
#include <cstdint>

/* Make sure your linker script provides these: */
extern uint32_t _data_lma; /* load address of data section */
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern void (*__init_array_start []) (void);
extern void (*__init_array_end   []) (void);
extern void (*__fini_array_start []) (void);
extern void (*__fini_array_end   []) (void);


struct CRunTime {
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


/* Instantiate CRunTimeIrqWrap in your system entry point in order to
 * initialize data and bss sections, and to initialize the core.
 */
template<typename core>
struct CRunTimeIrqWrap : public IrqWrap {
#ifndef CORE_SIMULATION
  CRunTimeIrqWrap() {
    CRunTime::init_data_section();
    CRunTime::init_bss_section();

    /* Init hardware BEFORE calling ctors, they might depend on it */
    core::init();

    CRunTime::call_ctors();
  };

  ~CRunTimeIrqWrap() {
    CRunTime::call_dtors();
  };
#endif // CORE_SIMULATION
};

#endif // CRT_HPP_INCLUDED
