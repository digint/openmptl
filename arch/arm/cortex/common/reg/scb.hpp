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

namespace reg
{
  /**
   * System Control Register
   *
   * For details, see "Cortex-M3 Technical Reference Manual":
   * <http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3/index.html>
   */
  struct SCB
  {
    /** Auxiliary Control Register, ACTLR  */
    using ACTLR  = Register< uint32_t, 0xE000E008, Access::rw >;

    /**
     * SysTick Control and Status Register
     */
    struct STCSR
    : public Register< uint32_t, 0xE000E010, Access::rw >
    {
      using ENABLE     = RegisterBits< type,  0,  1 >;  /**< [ 0: 0] Counter enable                                 */
      using TICKINT    = RegisterBits< type,  1,  1 >;  /**< [ 1: 1] Counting down to 0 pends the SysTick handler   */
      using CLKSOURCE  = RegisterBits< type,  2,  1 >;  /**< [ 2: 2] Clock source                                   */
      using COUNTFLAG  = RegisterBits< type, 16,  1 >;  /**< [16:16] Count Flag                                     */
    };

    /**
     * SysTick Reload Value Register
     */
    using STRVR  = Register< uint32_t, 0xE000E014, Access::rw >;

    /**
     * SysTick Current Value Register
     */
    using STCVR  = Register< uint32_t, 0xE000E018, Access::rw >;

    /**
     * SysTick Calilbration Value Register
     */
    struct STCR
    : public Register< uint32_t, 0xE000E01C, Access::ro >
    {
      using SKEW   = RegisterBits< type, 30,  1 >;  /**< [30:30] Calibration value is not exactly 10 ms   */
      using NOREF  = RegisterBits< type, 31,  1 >;  /**< [31:31] The reference clock is not provided      */
    };

    /**
     * CPUID Base Register, CPUID
     */
    struct CPUID
    : public Register< uint32_t, 0xE000ED00, Access::ro >
    {
      using REVISION     = RegisterBits< type,  0,  4 >;  /**< [ 3: 0] Implementation defined revision number   */
      using PARTNO       = RegisterBits< type,  4, 12 >;  /**< [15: 4] Number of processor within family        */
      using Constant     = RegisterBits< type, 16,  4 >;  /**< [19:16] Reads as 0x0F                            */
      using VARIANT      = RegisterBits< type, 20,  4 >;  /**< [23:20] Implementation defined variant number    */
      using IMPLEMENTER  = RegisterBits< type, 24,  8 >;  /**< [31:24] Implementer code. ARM is 0x41            */
    };

    /**
     * Interrupt Control and State Register
     */
    struct ICSR
    : public Register< uint32_t, 0xE000ED04, Access::rw >
    {
      using VECTACTIVE   = RegisterBits< type,  0,  9 >;  /**< [ 8: 0] Active ISR number field                                                          */
      using RETTOBASE    = RegisterBits< type, 11,  1 >;  /**< [11:11] All active exceptions minus the IPSR_current_exception yields the empty set      */
      using VECTPENDING  = RegisterBits< type, 12, 10 >;  /**< [21:12] Pending ISR number field                                                         */
      using ISRPENDING   = RegisterBits< type, 22,  1 >;  /**< [22:22] Interrupt pending flag                                                           */
      using ISRPREEMPT   = RegisterBits< type, 23,  1 >;  /**< [23:23] It indicates that a pending interrupt becomes active in the next running cycle   */
      using PENDSTCLR    = RegisterBits< type, 25,  1 >;  /**< [25:25] Clear pending SysTick bit                                                        */
      using PENDSTSET    = RegisterBits< type, 26,  1 >;  /**< [26:26] Set pending SysTick bit                                                          */
      using PENDSVCLR    = RegisterBits< type, 27,  1 >;  /**< [27:27] Clear pending pendSV bit                                                         */
      using PENDSVSET    = RegisterBits< type, 28,  1 >;  /**< [28:28] Set pending pendSV bit                                                           */
      using NMIPENDSET   = RegisterBits< type, 31,  1 >;  /**< [31:31] Set pending NMI bit                                                              */
    };

    /**
     * Vector Table Offset Register
     */
    struct VTOR
    : public Register< uint32_t, 0xE000ED08, Access::rw >
    {
      using TBLOFF   = RegisterBits< type,  7, 22 >;  /**< [28: 7] Vector table base offset field    */
      using TBLBASE  = RegisterBits< type, 29,  1 >;  /**< [29:29] Table base in code(0) or RAM(1)   */
    };

    /**
     * Application Interrupt and Reset Control Register
     */
    struct AIRCR
    : public Register< uint32_t, 0xE000ED0C, Access::rw >
    {
      using VECTRESET      = RegisterBits< type,  0,  1 >;  /**< [ 0: 0] System Reset bit                                         */
      using VECTCLRACTIVE  = RegisterBits< type,  1,  1 >;  /**< [ 1: 1] Clear active vector bit                                  */
      using SYSRESETREQ    = RegisterBits< type,  2,  1 >;  /**< [ 2: 2] Requests chip control logic to generate a reset          */
      using PRIGROUP       = RegisterBits< type,  8,  3 >;  /**< [10: 8] Priority group                                           */
      using ENDIANESS      = RegisterBits< type, 15,  1 >;  /**< [15:15] Data endianness bit                                      */
      using VECTKEY        = RegisterBits< type, 16, 16 >;  /**< [31:16] Register key (VECTKEY) - Reads as 0xFA05 (VECTKEYSTAT)   */
    };

    /**
     * System Control Register
     */
    struct SCR
    : public Register< uint32_t, 0xE000ED10, Access::rw >
    {
      using SLEEPONEXIT  = RegisterBits< type,  1,  1 >;  /**< [ 1: 1] Sleep on exit bit   */
      using SLEEPDEEP    = RegisterBits< type,  2,  1 >;  /**< [ 2: 2] Sleep deep bit      */
      using SEVONPEND    = RegisterBits< type,  4,  1 >;  /**< [ 4: 4] Wake up from WFE    */
    };

    /**
     * Configuration and Control Register
     */
    struct CCR
    : public Register< uint32_t, 0xE000ED14, Access::rw, 0x00000200 >
    {
      using NONBASETHRDENA  = RegisterBits< type,  0,  1 >;  /**< [ 0: 0] Thread mode can be entered from any level in Handler mode by controlled return value                    */
      using USERSETMPEND    = RegisterBits< type,  1,  1 >;  /**< [ 1: 1] Enables user code to write the Software Trigger Interrupt register to trigger (pend) a Main exception   */
      using UNALIGN_TRP     = RegisterBits< type,  3,  1 >;  /**< [ 3: 3] Trap for unaligned access                                                                               */
      using DIV_0_TRP       = RegisterBits< type,  4,  1 >;  /**< [ 4: 4] Trap on Divide by 0                                                                                     */
      using BFHFNMIGN       = RegisterBits< type,  8,  1 >;  /**< [ 8: 8] Handlers running at priority -1 and -2                                                                  */
      using STKALIGN        = RegisterBits< type,  9,  1 >;  /**< [ 9: 9] On exception entry, the SP used prior to the exception is adjusted to be 8-byte aligned                 */
    };

    /**
     * System Handler Priority Register
     */
    template<unsigned reg_index>
    struct SHPR
    : public Register< uint32_t, 0xE000ED18 + 4 * reg_index, Access::rw >
    {
      static_assert(reg_index < 3, "invalid index for register");

      using type = Register< uint32_t, 0xE000ED18 + 4 * reg_index, Access::rw >;

      // TODO: magic template for PRI_xx
      using PRI_N   = RegisterBits< type,  0,  8 >;  /**< [ 7: 0] Priority of system handler 4,8, and 12. Mem Manage, reserved and Debug Monitor   */
      using PRI_N1  = RegisterBits< type,  8,  8 >;  /**< [15: 8] Priority of system handler 5,9, and 13. Bus Fault, reserved and reserved         */
      using PRI_N2  = RegisterBits< type, 16,  8 >;  /**< [23:16] Priority of system handler 6,10, and 14. Usage Fault, reserved and PendSV        */
      using PRI_N3  = RegisterBits< type, 24,  8 >;  /**< [31:24] Priority of system handler 7,11, and 15. Reserved, SVCall and SysTick            */
    };

    /**
     * System Handler Control and State Register
     */
    struct SHCSR
    : public Register< uint32_t, 0xE000ED24, Access::rw >
    {
      using MEMFAULTACT     = RegisterBits< type,  0,  1 >;  /**< [ 0: 0] MemManage is active     */
      using BUSFAULTACT     = RegisterBits< type,  1,  1 >;  /**< [ 1: 1] BusFault is active      */
      using USGFAULTACT     = RegisterBits< type,  3,  1 >;  /**< [ 3: 3] UsageFault is active    */
      using SVCALLACT       = RegisterBits< type,  7,  1 >;  /**< [ 7: 7] SVCall is active        */
      using MONITORACT      = RegisterBits< type,  8,  1 >;  /**< [ 8: 8] Monitor is active       */
      using PENDSVACT       = RegisterBits< type, 10,  1 >;  /**< [10:10] PendSV is active        */
      using SYSTICKACT      = RegisterBits< type, 11,  1 >;  /**< [11:11] SysTick is active       */
      using USGFAULTPENDED  = RegisterBits< type, 12,  1 >;  /**< [12:12] Usage Fault is pended   */
      using MEMFAULTPENDED  = RegisterBits< type, 13,  1 >;  /**< [13:13] MemManage is pended     */
      using BUSFAULTPENDED  = RegisterBits< type, 14,  1 >;  /**< [14:14] Bus Fault is pended     */
      using SVCALLPENDED    = RegisterBits< type, 15,  1 >;  /**< [15:15] SVCall is pended        */
      using MEMFAULTENA     = RegisterBits< type, 16,  1 >;  /**< [16:16] MemManage enable        */
      using BUSFAULTENA     = RegisterBits< type, 17,  1 >;  /**< [17:17] Bus Fault enable        */
      using USGFAULTENA     = RegisterBits< type, 18,  1 >;  /**< [18:18] UsageFault enable       */
    };

    /**
     * Configurable Fault Status Registers
     */
    struct CFSR
    : public Register< uint32_t, 0xE000ED28, Access::rw >
    {
      /* MFSR */
      // TODO: byte-accessible registers
      // RegisterBits< type,  0,  8 > MFSR;
      // RegisterBits< type,  8, 16 > BFSR;
      // RegisterBits< type, 16, 16 > UFSR;
      using IACCVIOL     = RegisterBits< type,  0,  1 >;  /**< [ 0: 0] Instruction access violation   */
      using DACCVIOL     = RegisterBits< type,  1,  1 >;  /**< [ 1: 1] Data access violation          */
      using MUNSTKERR    = RegisterBits< type,  3,  1 >;  /**< [ 3: 3] Unstacking error               */
      using MSTKERR      = RegisterBits< type,  4,  1 >;  /**< [ 4: 4] Stacking error                 */
      /* BFSR */
      using IBUSERR      = RegisterBits< type,  8,  1 >;  /**< [ 8: 8] Instruction bus error flag                      */
      using PRECISERR    = RegisterBits< type,  9,  1 >;  /**< [ 9: 9] Precise data bus error                          */
      using IMPRECISERR  = RegisterBits< type, 10,  1 >;  /**< [10:10] Imprecise data bus error                        */
      using UNSTKERR     = RegisterBits< type, 11,  1 >;  /**< [11:11] Unstacking error                                */
      using STKERR       = RegisterBits< type, 12,  1 >;  /**< [12:12] Stacking error                                  */
      using BFARVALID    = RegisterBits< type, 15,  1 >;  /**< [15:15] Bus Fault Address Register address valid flag   */
      /* UFSR */
      using UNDEFINSTR   = RegisterBits< type, 16,  1 >;  /**< [16:16] The processor attempt to excecute an undefined instruction                 */
      using INVSTATE     = RegisterBits< type, 17,  1 >;  /**< [17:17] Invalid combination of EPSR and instruction                                */
      using INVPC        = RegisterBits< type, 18,  1 >;  /**< [18:18] Attempt to load EXC_RETURN into pc illegally                               */
      using NOCP         = RegisterBits< type, 19,  1 >;  /**< [19:19] Attempt to use a coprocessor instruction                                   */
      using UNALIGNED    = RegisterBits< type, 24,  1 >;  /**< [24:24] Fault occurs when there is an attempt to make an unaligned memory access   */
      using DIVBYZERO    = RegisterBits< type, 25,  1 >;  /**< [25:25] Fault occurs when SDIV or DIV instruction is used with a divisor of 0      */
    };

    /**
     * HardFault Status Register
     */
    struct HFSR
    : public Register< uint32_t, 0xE000ED2C, Access::rw >
    {
      using VECTTBL   = RegisterBits< type,  1,  1 >;  /**< [ 1: 1] Fault occures because of vector table read on exception processing                */
      using FORCED    = RegisterBits< type, 30,  1 >;  /**< [30:30] Hard Fault activated when a configurable Fault was received and cannot activate   */
      using DEBUGEVT  = RegisterBits< type, 31,  1 >;  /**< [31:31] Fault related to debug                                                            */
    };

    /**
     * Debug Fault Status Register
     */
    struct DFSR
    : public Register< uint32_t, 0xE000ED30, Access::rw >
    {
      using HALTED    = RegisterBits< type,  0,  1 >;  /**< [ 0: 0] Halt request flag                      */
      using BKPT      = RegisterBits< type,  1,  1 >;  /**< [ 1: 1] BKPT flag                              */
      using DWTTRAP   = RegisterBits< type,  2,  1 >;  /**< [ 2: 2] Data Watchpoint and Trace (DWT) flag   */
      using VCATCH    = RegisterBits< type,  3,  1 >;  /**< [ 3: 3] Vector catch flag                      */
      using EXTERNAL  = RegisterBits< type,  4,  1 >;  /**< [ 4: 4] External debug request flag            */
    };


    using MMFAR     = Register< uint32_t, 0xE000ED34, Access::rw             >;  /**< MemManage Fault Address Register[b]     */
    using BFAR      = Register< uint32_t, 0xE000ED38, Access::rw             >;  /**< BusFault Address Register[b]            */
    using AFSR      = Register< uint32_t, 0xE000ED3C, Access::rw             >;  /**< Auxiliary Fault Status Register, AFSR   */
    using ID_PFR0   = Register< uint32_t, 0xE000ED40, Access::ro, 0x00000030 >;  /**< Processor Feature Register 0            */
    using ID_PFR1   = Register< uint32_t, 0xE000ED44, Access::ro, 0x00000200 >;  /**< Processor Feature Register 1            */
    using ID_DFR0   = Register< uint32_t, 0xE000ED48, Access::ro, 0x00100000 >;  /**< Debug Features Register 0[c]            */
    using ID_AFR0   = Register< uint32_t, 0xE000ED4C, Access::ro             >;  /**< Auxiliary Features Register 0           */
    using ID_MMFR0  = Register< uint32_t, 0xE000ED50, Access::ro, 0x00100030 >;  /**< Memory Model Feature Register 0         */
    using ID_MMFR1  = Register< uint32_t, 0xE000ED54, Access::ro             >;  /**< Memory Model Feature Register 1         */
    using ID_MMFR2  = Register< uint32_t, 0xE000ED58, Access::ro, 0x01000000 >;  /**< Memory Model Feature Register 2         */
    using ID_MMFR3  = Register< uint32_t, 0xE000ED5C, Access::ro             >;  /**< Memory Model Feature Register 3         */
    using ID_ISAR0  = Register< uint32_t, 0xE000ED60, Access::ro, 0x01100110 >;  /**< Instruction Set Attributes Register 0   */
    using ID_ISAR1  = Register< uint32_t, 0xE000ED64, Access::ro, 0x02111000 >;  /**< Instruction Set Attributes Register 1   */
    using ID_ISAR2  = Register< uint32_t, 0xE000ED68, Access::ro, 0x21112231 >;  /**< Instruction Set Attributes Register 2   */
    using ID_ISAR3  = Register< uint32_t, 0xE000ED6C, Access::ro, 0x01111110 >;  /**< Instruction Set Attributes Register 3   */
    using ID_ISAR4  = Register< uint32_t, 0xE000ED70, Access::ro, 0x01310132 >;  /**< Instruction Set Attributes Register 4   */
    using CPACR     = Register< uint32_t, 0xE000ED88, Access::rw             >;  /**< Coprocessor Access Control Register     */
    using STIR      = Register< uint32_t, 0xE000EF00, Access::wo             >;  /**< Software Triggered Interrupt Register   */
  };
}

#endif // COMMON_ARM_CORTEX_REG_SCB_HPP_INCLUDED
