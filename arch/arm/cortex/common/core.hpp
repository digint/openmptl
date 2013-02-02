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

#ifndef COMMON_ARM_CORTEX_CORE_HPP_INCLUDED
#define COMMON_ARM_CORTEX_CORE_HPP_INCLUDED

#include <arch/core_startup.hpp>


////////////////////  CoreFunctions  ////////////////////


struct CoreFunctions
{
  static inline void enable_irq()        { __asm volatile ("cpsie i"); }  /**< global interrupt enable   */
  static inline void disable_irq()       { __asm volatile ("cpsid i"); }  /**< global interrupt disable  */

  static inline void enable_fault_irq()  { __asm volatile ("cpsie f"); }
  static inline void disable_fault_irq() { __asm volatile ("cpsid f"); }

  static inline void nop()               { __asm volatile ("nop"); }
  static inline void wfi()               { __asm volatile ("wfi"); }
  static inline void wfe()               { __asm volatile ("wfe"); }
  static inline void sev()               { __asm volatile ("sev"); }
  static inline void isb()               { __asm volatile ("isb"); }
  static inline void dsb()               { __asm volatile ("dsb"); }
  static inline void dmb()               { __asm volatile ("dmb"); }
  static inline void clrex()             { __asm volatile ("clrex"); }

  static void nop(unsigned value)        { while(value--) nop(); }
};


////////////////////  Core  ////////////////////


struct Core : CoreStartup, CoreFunctions
{
  /* Initialize the hardware.
   * NOTE: This function is called BEFORE ANY static object constructors are called!
   */
  static void Init(void) {
    CoreStartup::InitClocks();

    // TODO: NVIC Priority Grouping MPL magic
  }

#if 0
  struct CriticalSection
  {
    CriticalSection () { disable_irq(); }
    ~CriticalSection() { enable_irq(); }
  };
#endif
};

#endif // COMMON_ARM_CORTEX_CORE_HPP_INCLUDED
