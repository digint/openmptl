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

/*
 * NOTE: In order to use global objects, you need to define (somewhere in your code):
 *
 *   void *__dso_handle;
 */

#ifndef CRT_HPP_INCLUDED
#define CRT_HPP_INCLUDED

#ifndef OPENMPTL_SIMULATION

#include <cstdint>

/* Make sure your linker script provides these: */
extern uint32_t _data_lma; /* load address of data section */
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern void (*__preinit_array_start []) (void);
extern void (*__preinit_array_end   []) (void);
extern void (*__init_array_start    []) (void);
extern void (*__init_array_end      []) (void);
extern void (*__fini_array_start    []) (void);
extern void (*__fini_array_end      []) (void);

namespace crt
{
  static inline void init_data_section(void) {
    uint32_t *src = &_data_lma;
    uint32_t *dst = &_sdata;
    while(dst < &_edata)
      *(dst++) = *(src++);
  }

  static inline void init_bss_section(void) {
    uint32_t *dst = &_sbss;
    while(dst < &_ebss)
      *(dst++) = 0;
  }

  /** call functions in preinit_array */
  static inline void run_preinit_array(void) {
    uintptr_t n = __preinit_array_end - __preinit_array_start;
    uintptr_t i = 0;
    while(i < n)
      __preinit_array_start[i++]();
  }

  /** call functions in init_array (constructors) */
  static inline void run_init_array(void) {
    uintptr_t n = __init_array_end - __init_array_start;
    uintptr_t i = 0;
    while(i < n)
      __init_array_start[i++]();
  }

  /** call functions in fini_array (destructors) */
  static inline void run_fini_array(void) {
    uintptr_t n = __fini_array_end - __fini_array_start;
    uintptr_t i = 0;
    while(i < n)
      __fini_array_start[i++]();
  }
} // namespace crt

#else // OPENMPTL_SIMULATION

namespace crt
{
  static inline void init_data_section(void) { }
  static inline void init_bss_section(void) { }
  static inline void run_preinit_array(void) { }
  static inline void run_init_array(void) { }
  static inline void run_fini_array(void) { }
} // namespace crt

#endif //OPENMPTL_SIMULATION

#endif // CRT_HPP_INCLUDED
