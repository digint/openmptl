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

#ifndef ARM_CORTEX_COMMON_CORE_HPP_INCLUDED
#define ARM_CORTEX_COMMON_CORE_HPP_INCLUDED

#include <register.hpp>
#include <crt.hpp>

namespace mptl {

struct core_base
{
  static void enable_irq()        { __asm volatile ("cpsie i"); }  /**< global interrupt enable   */
  static void disable_irq()       { __asm volatile ("cpsid i"); }  /**< global interrupt disable  */

  static void enable_fault_irq()  { __asm volatile ("cpsie f"); }
  static void disable_fault_irq() { __asm volatile ("cpsid f"); }

  static void nop()               { __asm volatile ("nop"); }
  static void wfi()               { __asm volatile ("wfi"); }
  static void wfe()               { __asm volatile ("wfe"); }
  static void sev()               { __asm volatile ("sev"); }
  static void isb()               { __asm volatile ("isb"); }
  static void dsb()               { __asm volatile ("dsb"); }
  static void dmb()               { __asm volatile ("dmb"); }
  static void clrex()             { __asm volatile ("clrex"); }

  static void nop(unsigned value) { while(value--) nop(); }

  /* Startup code.
   *
   *   - Initialize data and bss section
   *   - Set early-config registers
   *   - Set system clock
   *
   * Template arguments:
   *
   *   - system_clock_type: class providing init() and configure()
   *       static member functions.
   *
   *   - early_cfg_list: list of regmask<> or reglist<> type traits to
   *       be set before the system clock is configured.
   */
  template<
    typename system_clock_type,
    typename... early_cfg_list
    >
  static void startup(void) {
    crt::init_data_section();
    crt::init_bss_section();

    system_clock_type::init();
    reglist< early_cfg_list... >::reset_to();
    system_clock_type::configure();

    crt::run_preinit_array();
    crt::run_init_array();    /* call ctors */
  }
};

} // namespace mptl

#endif // ARM_CORTEX_COMMON_CORE_HPP_INCLUDED
