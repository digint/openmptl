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

#ifndef COMMON_ARM_CORTEX_REG_SCB_HPP_INCLUDED
#define COMMON_ARM_CORTEX_REG_SCB_HPP_INCLUDED

#include <register.hpp>

namespace mptl { namespace reg {

/**
 * System Control Register
 *
 * For details, see "Cortex-M3 Technical Reference Manual":
 * <http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3/index.html>
 */
struct SCB
{
  /** Auxiliary Control Register, ACTLR  */
  using ACTLR  = regdef< uint32_t, 0xE000E008, reg_access::rw >;

  /**
   * SysTick Control and Status Register
   */
  struct STCSR
  : public regdef< uint32_t, 0xE000E010, reg_access::rw >
  {
    using ENABLE     = regbits< type,  0,  1 >;  /**< [ 0: 0] Counter enable                                 */
    using TICKINT    = regbits< type,  1,  1 >;  /**< [ 1: 1] Counting down to 0 pends the SysTick handler   */
    using CLKSOURCE  = regbits< type,  2,  1 >;  /**< [ 2: 2] Clock source                                   */
    using COUNTFLAG  = regbits< type, 16,  1 >;  /**< [16:16] Count Flag                                     */
  };

  /**
   * SysTick Reload Value Register
   */
  using STRVR  = regdef< uint32_t, 0xE000E014, reg_access::rw >;

  /**
   * SysTick Current Value Register
   */
  using STCVR  = regdef< uint32_t, 0xE000E018, reg_access::rw >;

  /**
   * SysTick Calilbration Value Register
   */
  struct STCR
  : public regdef< uint32_t, 0xE000E01C, reg_access::ro >
  {
    using SKEW   = regbits< type, 30,  1 >;  /**< [30:30] Calibration value is not exactly 10 ms   */
    using NOREF  = regbits< type, 31,  1 >;  /**< [31:31] The reference clock is not provided      */
  };

  /**
   * CPUID Base Register, CPUID
   */
  struct CPUID
  : public regdef< uint32_t, 0xE000ED00, reg_access::ro >
  {
    using REVISION     = regbits< type,  0,  4 >;  /**< [ 3: 0] Implementation defined revision number   */
    using PARTNO       = regbits< type,  4, 12 >;  /**< [15: 4] Number of processor within family        */
    using Constant     = regbits< type, 16,  4 >;  /**< [19:16] Reads as 0x0F                            */
    using VARIANT      = regbits< type, 20,  4 >;  /**< [23:20] Implementation defined variant number    */
    using IMPLEMENTER  = regbits< type, 24,  8 >;  /**< [31:24] Implementer code. ARM is 0x41            */
  };

  /**
   * Interrupt Control and State Register
   */
  struct ICSR
  : public regdef< uint32_t, 0xE000ED04, reg_access::rw >
  {
    using VECTACTIVE   = regbits< type,  0,  9 >;  /**< [ 8: 0] Active ISR number field                                                          */
    using RETTOBASE    = regbits< type, 11,  1 >;  /**< [11:11] All active exceptions minus the IPSR_current_exception yields the empty set      */
    using VECTPENDING  = regbits< type, 12, 10 >;  /**< [21:12] Pending ISR number field                                                         */
    using ISRPENDING   = regbits< type, 22,  1 >;  /**< [22:22] Interrupt pending flag                                                           */
    using ISRPREEMPT   = regbits< type, 23,  1 >;  /**< [23:23] It indicates that a pending interrupt becomes active in the next running cycle   */
    using PENDSTCLR    = regbits< type, 25,  1 >;  /**< [25:25] Clear pending SysTick bit                                                        */
    using PENDSTSET    = regbits< type, 26,  1 >;  /**< [26:26] Set pending SysTick bit                                                          */
    using PENDSVCLR    = regbits< type, 27,  1 >;  /**< [27:27] Clear pending pendSV bit                                                         */
    using PENDSVSET    = regbits< type, 28,  1 >;  /**< [28:28] Set pending pendSV bit                                                           */
    using NMIPENDSET   = regbits< type, 31,  1 >;  /**< [31:31] Set pending NMI bit                                                              */
  };

  /**
   * Vector Table Offset Register
   */
  struct VTOR
  : public regdef< uint32_t, 0xE000ED08, reg_access::rw >
  {
    using TBLOFF   = regbits< type,  7, 22 >;  /**< [28: 7] Vector table base offset field    */
    using TBLBASE  = regbits< type, 29,  1 >;  /**< [29:29] Table base in code(0) or RAM(1)   */
  };

  /**
   * Application Interrupt and Reset Control Register
   */
  struct AIRCR
  : public regdef< uint32_t, 0xE000ED0C, reg_access::rw >
  {
    using VECTRESET      = regbits< type,  0,  1 >;  /**< [ 0: 0] System Reset bit                                         */
    using VECTCLRACTIVE  = regbits< type,  1,  1 >;  /**< [ 1: 1] Clear active vector bit                                  */
    using SYSRESETREQ    = regbits< type,  2,  1 >;  /**< [ 2: 2] Requests chip control logic to generate a reset          */
    using PRIGROUP       = regbits< type,  8,  3 >;  /**< [10: 8] Priority group                                           */
    using ENDIANESS      = regbits< type, 15,  1 >;  /**< [15:15] Data endianness bit                                      */
    using VECTKEY        = regbits< type, 16, 16 >;  /**< [31:16] Register key (VECTKEY) - Reads as 0xFA05 (VECTKEYSTAT)   */
  };

  /**
   * System Control Register
   */
  struct SCR
  : public regdef< uint32_t, 0xE000ED10, reg_access::rw >
  {
    using SLEEPONEXIT  = regbits< type,  1,  1 >;  /**< [ 1: 1] Sleep on exit bit   */
    using SLEEPDEEP    = regbits< type,  2,  1 >;  /**< [ 2: 2] Sleep deep bit      */
    using SEVONPEND    = regbits< type,  4,  1 >;  /**< [ 4: 4] Wake up from WFE    */
  };

  /**
   * Configuration and Control Register
   */
  struct CCR
  : public regdef< uint32_t, 0xE000ED14, reg_access::rw, 0x00000200 >
  {
    using NONBASETHRDENA  = regbits< type,  0,  1 >;  /**< [ 0: 0] Thread mode can be entered from any level in Handler mode by controlled return value                    */
    using USERSETMPEND    = regbits< type,  1,  1 >;  /**< [ 1: 1] Enables user code to write the Software Trigger Interrupt register to trigger (pend) a Main exception   */
    using UNALIGN_TRP     = regbits< type,  3,  1 >;  /**< [ 3: 3] Trap for unaligned access                                                                               */
    using DIV_0_TRP       = regbits< type,  4,  1 >;  /**< [ 4: 4] Trap on Divide by 0                                                                                     */
    using BFHFNMIGN       = regbits< type,  8,  1 >;  /**< [ 8: 8] Handlers running at priority -1 and -2                                                                  */
    using STKALIGN        = regbits< type,  9,  1 >;  /**< [ 9: 9] On exception entry, the SP used prior to the exception is adjusted to be 8-byte aligned                 */
  };

  /**
   * System Handler Priority Register
   */
  template<unsigned reg_index>
  struct SHPR
  : public regdef< uint32_t, 0xE000ED18 + 4 * reg_index, reg_access::rw >
  {
    static_assert(reg_index < 3, "invalid index for register");

    using type = regdef< uint32_t, 0xE000ED18 + 4 * reg_index, reg_access::rw >;

    // TODO: template PRIx
    using PRI_N   = regbits< type,  0,  8 >;  /**< [ 7: 0] Priority of system handler 4,8, and 12. Mem Manage, reserved and Debug Monitor   */
    using PRI_N1  = regbits< type,  8,  8 >;  /**< [15: 8] Priority of system handler 5,9, and 13. Bus Fault, reserved and reserved         */
    using PRI_N2  = regbits< type, 16,  8 >;  /**< [23:16] Priority of system handler 6,10, and 14. Usage Fault, reserved and PendSV        */
    using PRI_N3  = regbits< type, 24,  8 >;  /**< [31:24] Priority of system handler 7,11, and 15. Reserved, SVCall and SysTick            */
  };

  /**
   * System Handler Control and State Register
   */
  struct SHCSR
  : public regdef< uint32_t, 0xE000ED24, reg_access::rw >
  {
    using MEMFAULTACT     = regbits< type,  0,  1 >;  /**< [ 0: 0] MemManage is active     */
    using BUSFAULTACT     = regbits< type,  1,  1 >;  /**< [ 1: 1] BusFault is active      */
    using USGFAULTACT     = regbits< type,  3,  1 >;  /**< [ 3: 3] UsageFault is active    */
    using SVCALLACT       = regbits< type,  7,  1 >;  /**< [ 7: 7] SVCall is active        */
    using MONITORACT      = regbits< type,  8,  1 >;  /**< [ 8: 8] Monitor is active       */
    using PENDSVACT       = regbits< type, 10,  1 >;  /**< [10:10] PendSV is active        */
    using SYSTICKACT      = regbits< type, 11,  1 >;  /**< [11:11] SysTick is active       */
    using USGFAULTPENDED  = regbits< type, 12,  1 >;  /**< [12:12] Usage Fault is pended   */
    using MEMFAULTPENDED  = regbits< type, 13,  1 >;  /**< [13:13] MemManage is pended     */
    using BUSFAULTPENDED  = regbits< type, 14,  1 >;  /**< [14:14] Bus Fault is pended     */
    using SVCALLPENDED    = regbits< type, 15,  1 >;  /**< [15:15] SVCall is pended        */
    using MEMFAULTENA     = regbits< type, 16,  1 >;  /**< [16:16] MemManage enable        */
    using BUSFAULTENA     = regbits< type, 17,  1 >;  /**< [17:17] Bus Fault enable        */
    using USGFAULTENA     = regbits< type, 18,  1 >;  /**< [18:18] UsageFault enable       */
  };

  /**
   * Configurable Fault Status Registers
   */
  struct CFSR
  : public regdef< uint32_t, 0xE000ED28, reg_access::rw >
  {
    /* MFSR */
    // TODO: byte-accessible registers
    // regbits< type,  0,  8 > MFSR;
    // regbits< type,  8, 16 > BFSR;
    // regbits< type, 16, 16 > UFSR;
    using IACCVIOL     = regbits< type,  0,  1 >;  /**< [ 0: 0] Instruction access violation   */
    using DACCVIOL     = regbits< type,  1,  1 >;  /**< [ 1: 1] Data access violation          */
    using MUNSTKERR    = regbits< type,  3,  1 >;  /**< [ 3: 3] Unstacking error               */
    using MSTKERR      = regbits< type,  4,  1 >;  /**< [ 4: 4] Stacking error                 */
    /* BFSR */
    using IBUSERR      = regbits< type,  8,  1 >;  /**< [ 8: 8] Instruction bus error flag                      */
    using PRECISERR    = regbits< type,  9,  1 >;  /**< [ 9: 9] Precise data bus error                          */
    using IMPRECISERR  = regbits< type, 10,  1 >;  /**< [10:10] Imprecise data bus error                        */
    using UNSTKERR     = regbits< type, 11,  1 >;  /**< [11:11] Unstacking error                                */
    using STKERR       = regbits< type, 12,  1 >;  /**< [12:12] Stacking error                                  */
    using BFARVALID    = regbits< type, 15,  1 >;  /**< [15:15] Bus Fault Address Register address valid flag   */
    /* UFSR */
    using UNDEFINSTR   = regbits< type, 16,  1 >;  /**< [16:16] The processor attempt to excecute an undefined instruction                 */
    using INVSTATE     = regbits< type, 17,  1 >;  /**< [17:17] Invalid combination of EPSR and instruction                                */
    using INVPC        = regbits< type, 18,  1 >;  /**< [18:18] Attempt to load EXC_RETURN into pc illegally                               */
    using NOCP         = regbits< type, 19,  1 >;  /**< [19:19] Attempt to use a coprocessor instruction                                   */
    using UNALIGNED    = regbits< type, 24,  1 >;  /**< [24:24] Fault occurs when there is an attempt to make an unaligned memory access   */
    using DIVBYZERO    = regbits< type, 25,  1 >;  /**< [25:25] Fault occurs when SDIV or DIV instruction is used with a divisor of 0      */
  };

  /**
   * HardFault Status Register
   */
  struct HFSR
  : public regdef< uint32_t, 0xE000ED2C, reg_access::rw >
  {
    using VECTTBL   = regbits< type,  1,  1 >;  /**< [ 1: 1] Fault occures because of vector table read on exception processing                */
    using FORCED    = regbits< type, 30,  1 >;  /**< [30:30] Hard Fault activated when a configurable Fault was received and cannot activate   */
    using DEBUGEVT  = regbits< type, 31,  1 >;  /**< [31:31] Fault related to debug                                                            */
  };

  /**
   * Debug Fault Status Register
   */
  struct DFSR
  : public regdef< uint32_t, 0xE000ED30, reg_access::rw >
  {
    using HALTED    = regbits< type,  0,  1 >;  /**< [ 0: 0] Halt request flag                      */
    using BKPT      = regbits< type,  1,  1 >;  /**< [ 1: 1] BKPT flag                              */
    using DWTTRAP   = regbits< type,  2,  1 >;  /**< [ 2: 2] Data Watchpoint and Trace (DWT) flag   */
    using VCATCH    = regbits< type,  3,  1 >;  /**< [ 3: 3] Vector catch flag                      */
    using EXTERNAL  = regbits< type,  4,  1 >;  /**< [ 4: 4] External debug request flag            */
  };


  using MMFAR     = regdef< uint32_t, 0xE000ED34, reg_access::rw             >;  /**< MemManage Fault Address Register[b]     */
  using BFAR      = regdef< uint32_t, 0xE000ED38, reg_access::rw             >;  /**< BusFault Address Register[b]            */
  using AFSR      = regdef< uint32_t, 0xE000ED3C, reg_access::rw             >;  /**< Auxiliary Fault Status Register, AFSR   */
  using ID_PFR0   = regdef< uint32_t, 0xE000ED40, reg_access::ro, 0x00000030 >;  /**< Processor Feature Register 0            */
  using ID_PFR1   = regdef< uint32_t, 0xE000ED44, reg_access::ro, 0x00000200 >;  /**< Processor Feature Register 1            */
  using ID_DFR0   = regdef< uint32_t, 0xE000ED48, reg_access::ro, 0x00100000 >;  /**< Debug Features Register 0[c]            */
  using ID_AFR0   = regdef< uint32_t, 0xE000ED4C, reg_access::ro             >;  /**< Auxiliary Features Register 0           */
  using ID_MMFR0  = regdef< uint32_t, 0xE000ED50, reg_access::ro, 0x00100030 >;  /**< Memory Model Feature Register 0         */
  using ID_MMFR1  = regdef< uint32_t, 0xE000ED54, reg_access::ro             >;  /**< Memory Model Feature Register 1         */
  using ID_MMFR2  = regdef< uint32_t, 0xE000ED58, reg_access::ro, 0x01000000 >;  /**< Memory Model Feature Register 2         */
  using ID_MMFR3  = regdef< uint32_t, 0xE000ED5C, reg_access::ro             >;  /**< Memory Model Feature Register 3         */
  using ID_ISAR0  = regdef< uint32_t, 0xE000ED60, reg_access::ro, 0x01100110 >;  /**< Instruction Set Attributes Register 0   */
  using ID_ISAR1  = regdef< uint32_t, 0xE000ED64, reg_access::ro, 0x02111000 >;  /**< Instruction Set Attributes Register 1   */
  using ID_ISAR2  = regdef< uint32_t, 0xE000ED68, reg_access::ro, 0x21112231 >;  /**< Instruction Set Attributes Register 2   */
  using ID_ISAR3  = regdef< uint32_t, 0xE000ED6C, reg_access::ro, 0x01111110 >;  /**< Instruction Set Attributes Register 3   */
  using ID_ISAR4  = regdef< uint32_t, 0xE000ED70, reg_access::ro, 0x01310132 >;  /**< Instruction Set Attributes Register 4   */
  using CPACR     = regdef< uint32_t, 0xE000ED88, reg_access::rw             >;  /**< Coprocessor Access Control Register     */
  using STIR      = regdef< uint32_t, 0xE000EF00, reg_access::wo             >;  /**< Software Triggered Interrupt Register   */
};

} } // namespace mptl::reg

#endif // COMMON_ARM_CORTEX_REG_SCB_HPP_INCLUDED
