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

#ifndef CORE_HPP_INCLUDED
#define CORE_HPP_INCLUDED

#include <cppcore_setup.hpp>
#include <arch/register_defs.hpp>


////////////////////  CoreExceptionNumber  ////////////////////


/*****  ARM Cortex Processor Exceptions Numbers *****/
enum class CoreExceptionNumber : int
{
  MemoryManagement       = -12,    /**< 4 Cortex-M3 Memory Management Interrupt                           */
  BusFault               = -11,    /**< 5 Cortex-M3 Bus Fault Interrupt                                   */
  UsageFault             = -10,    /**< 6 Cortex-M3 Usage Fault Interrupt                                 */
  SVCall                 = -5,     /**< 11 Cortex-M3 SV Call Interrupt                                    */
  DebugMonitor           = -4,     /**< 12 Cortex-M3 Debug Monitor Interrupt                              */
  PendSV                 = -2,     /**< 14 Cortex-M3 Pend SV Interrupt                                    */
  SysTick                = -1      /**< 15 Cortex-M3 System Tick Interrupt                                */
};


////////////////////  CoreFunctions  ////////////////////


struct CoreFunctions
{
  static inline void enable_irq()        { __asm volatile ("cpsie i"); } //< global interrupt enable
  static inline void disable_irq()       { __asm volatile ("cpsid i"); } //< global interrupt disable

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
};


////////////////////  Core  ////////////////////


class Core : public CoreRegister,
             public PeripheralRegister,
             public CoreSetup,
             public CoreFunctions
{
public:

  /* Initialize the hardware. */
  /* NOTE: this function is called BEFORE ANY static object constructors are called! */
  static void InitHW(void);

  static void nop(unsigned value) {
    while(value-- > 0 ) { CoreFunctions::nop(); }
  }

  /* make sure clock_frequency is set correctly in cppcore_setup.hpp */
  static_assert(clock_frequency >= 0, "invalid clock frequency (maybe you forgot to set it in cppcore_setup.hpp ?)");

#if 0
  struct CriticalSection
  {
    CriticalSection () { disable_irq(); }
    ~CriticalSection() { enable_irq(); }
  };
#endif

private:
  static inline void InitClocks();
};

#endif // CORE_HPP_INCLUDED
