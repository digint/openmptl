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

#ifndef COMMON_ARM_CORTEX_CORE_REGISTER_HPP_INCLUDED
#define COMMON_ARM_CORTEX_CORE_REGISTER_HPP_INCLUDED


////////////////////  CoreMemoryMap  ////////////////////


/* Memory mapping of Cortex-M3 Hardware */
struct CoreMemoryMap
{
  static constexpr reg_addr_t SCS_BASE        = 0xE000E000;         /**< System Control Space Base Address   */
  static constexpr reg_addr_t ITM_BASE        = 0xE0000000;         /**< ITM Base Address                    */
  static constexpr reg_addr_t CoreDebug_BASE  = 0xE000EDF0;         /**< Core Debug Base Address             */
  static constexpr reg_addr_t SysTick_BASE    = SCS_BASE +  0x0010; /**< SysTick Base Address                */
  static constexpr reg_addr_t NVIC_BASE       = SCS_BASE +  0x0100; /**< NVIC Base Address                   */
  static constexpr reg_addr_t SCB_BASE        = SCS_BASE +  0x0D00; /**< System Control Block Base Address   */
  static constexpr reg_addr_t MPU_BASE        = SCS_BASE +  0x0D90; /**< Memory Protection Unit              */
};


////////////////////  CoreRegister  ////////////////////


class CoreRegister {

public:

  using access = RegisterAccess;

  /**
   * System Control Register
   *
   * Imported from the Cortex-M3 Technical Reference Manual  (Revision: r2p1)
   * <http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3/index.html>
   */
  class SCB
  {
    /**
     * SysTick Control and Status Register
     */
    template<typename R>
    struct __STCSR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > ENABLE;        /**< [ 0: 0] Counter enable                                 */
      typedef RegisterBits< R,  1,  1 > TICKINT;       /**< [ 1: 1] Counting down to 0 pends the SysTick handler   */
      typedef RegisterBits< R,  2,  1 > CLKSOURCE;     /**< [ 2: 2] Clock source                                   */
      typedef RegisterBits< R, 16,  1 > COUNTFLAG;     /**< [16:16] Count Flag                                     */
    };

    /**
     * SysTick Calilbration Value Register
     */
    template<typename R>
    struct __STCR
    : public R
    {
      typedef RegisterBits< R, 30,  1 > SKEW;          /**< [30:30] Calibration value is not exactly 10 ms   */
      typedef RegisterBits< R, 31,  1 > NOREF;         /**< [31:31] The reference clock is not provided      */
    };

    /**
     * CPUID Base Register, CPUID
     */
    template<typename R>
    struct __CPUID
    : public R
    {
      typedef RegisterBits< R,  0,  4 > REVISION;      /**< [ 3: 0] Implementation defined revision number   */
      typedef RegisterBits< R,  4, 12 > PARTNO;        /**< [15: 4] Number of processor within family        */
      typedef RegisterBits< R, 16,  4 > Constant;      /**< [19:16] Reads as 0x0F                            */
      typedef RegisterBits< R, 20,  4 > VARIANT;       /**< [23:20] Implementation defined variant number    */
      typedef RegisterBits< R, 24,  8 > IMPLEMENTER;   /**< [31:24] Implementer code. ARM is 0x41            */
    };

    /**
     * Interrupt Control and State Register
     */
    template<typename R>
    struct __ICSR
    : public R
    {
      typedef RegisterBits< R,  0,  9 > VECTACTIVE;    /**< [ 8: 0] Active ISR number field                                                          */
      typedef RegisterBits< R, 11,  1 > RETTOBASE;     /**< [11:11] All active exceptions minus the IPSR_current_exception yields the empty set      */
      typedef RegisterBits< R, 12, 10 > VECTPENDING;   /**< [21:12] Pending ISR number field                                                         */
      typedef RegisterBits< R, 22,  1 > ISRPENDING;    /**< [22:22] Interrupt pending flag                                                           */
      typedef RegisterBits< R, 23,  1 > ISRPREEMPT;    /**< [23:23] It indicates that a pending interrupt becomes active in the next running cycle   */
      typedef RegisterBits< R, 25,  1 > PENDSTCLR;     /**< [25:25] Clear pending SysTick bit                                                        */
      typedef RegisterBits< R, 26,  1 > PENDSTSET;     /**< [26:26] Set pending SysTick bit                                                          */
      typedef RegisterBits< R, 27,  1 > PENDSVCLR;     /**< [27:27] Clear pending pendSV bit                                                         */
      typedef RegisterBits< R, 28,  1 > PENDSVSET;     /**< [28:28] Set pending pendSV bit                                                           */
      typedef RegisterBits< R, 31,  1 > NMIPENDSET;    /**< [31:31] Set pending NMI bit                                                              */
    };

    /**
     * Vector Table Offset Register
     */
    template<typename R>
    struct __VTOR
    : public R
    {
      typedef RegisterBits< R,  7, 22 > TBLOFF;        /**< [28: 7] Vector table base offset field    */
      typedef RegisterBits< R, 29,  1 > TBLBASE;       /**< [29:29] Table base in code(0) or RAM(1)   */
    };

    /**
     * Application Interrupt and Reset Control Register
     */
    template<typename R>
    struct __AIRCR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > VECTRESET;     /**< [ 0: 0] System Reset bit                                         */
      typedef RegisterBits< R,  1,  1 > VECTCLRACTIVE; /**< [ 1: 1] Clear active vector bit                                  */
      typedef RegisterBits< R,  2,  1 > SYSRESETREQ;   /**< [ 2: 2] Requests chip control logic to generate a reset          */
      typedef RegisterBits< R,  8,  3 > PRIGROUP;      /**< [10: 8] Priority group                                           */
      typedef RegisterBits< R, 15,  1 > ENDIANESS;     /**< [15:15] Data endianness bit                                      */
      typedef RegisterBits< R, 16, 16 > VECTKEY;       /**< [31:16] Register key (VECTKEY) - Reads as 0xFA05 (VECTKEYSTAT)   */
    };

    /**
     * System Control Register
     */
    template<typename R>
    struct __SCR
    : public R
    {
      typedef RegisterBits< R,  1,  1 > SLEEPONEXIT;   /**< [ 1: 1] Sleep on exit bit   */
      typedef RegisterBits< R,  2,  1 > SLEEPDEEP;     /**< [ 2: 2] Sleep deep bit      */
      typedef RegisterBits< R,  4,  1 > SEVONPEND;     /**< [ 4: 4] Wake up from WFE    */
    };

    /**
     * Configuration and Control Register
     */
    template<typename R>
    struct __CCR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > NONBASETHRDENA; /**< [ 0: 0] Thread mode can be entered from any level in Handler mode by controlled return value                    */
      typedef RegisterBits< R,  1,  1 > USERSETMPEND;   /**< [ 1: 1] Enables user code to write the Software Trigger Interrupt register to trigger (pend) a Main exception   */
      typedef RegisterBits< R,  3,  1 > UNALIGN_TRP;    /**< [ 3: 3] Trap for unaligned access                                                                               */
      typedef RegisterBits< R,  4,  1 > DIV_0_TRP;      /**< [ 4: 4] Trap on Divide by 0                                                                                     */
      typedef RegisterBits< R,  8,  1 > BFHFNMIGN;      /**< [ 8: 8] Handlers running at priority -1 and -2                                                                  */
      typedef RegisterBits< R,  9,  1 > STKALIGN;       /**< [ 9: 9] On exception entry, the SP used prior to the exception is adjusted to be 8-byte aligned                 */
    };

    /**
     * System Handler Priority Register
     */
    template<typename R>
    struct __SHPR
    : public R
    {
      // TODO: magic template for PRI_xx
      typedef RegisterBits< R,  0,  8 > PRI_N;         /**< [ 7: 0] Priority of system handler 4,8, and 12. Mem Manage, reserved and Debug Monitor   */
      typedef RegisterBits< R,  8,  8 > PRI_N1;        /**< [15: 8] Priority of system handler 5,9, and 13. Bus Fault, reserved and reserved         */
      typedef RegisterBits< R, 16,  8 > PRI_N2;        /**< [23:16] Priority of system handler 6,10, and 14. Usage Fault, reserved and PendSV        */
      typedef RegisterBits< R, 24,  8 > PRI_N3;        /**< [31:24] Priority of system handler 7,11, and 15. Reserved, SVCall and SysTick            */
    };

    /**
     * System Handler Control and State Register
     */
    template<typename R>
    struct __SHCSR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > MEMFAULTACT;    /**< [ 0: 0] MemManage is active     */
      typedef RegisterBits< R,  1,  1 > BUSFAULTACT;    /**< [ 1: 1] BusFault is active      */
      typedef RegisterBits< R,  3,  1 > USGFAULTACT;    /**< [ 3: 3] UsageFault is active    */
      typedef RegisterBits< R,  7,  1 > SVCALLACT;      /**< [ 7: 7] SVCall is active        */
      typedef RegisterBits< R,  8,  1 > MONITORACT;     /**< [ 8: 8] Monitor is active       */
      typedef RegisterBits< R, 10,  1 > PENDSVACT;      /**< [10:10] PendSV is active        */
      typedef RegisterBits< R, 11,  1 > SYSTICKACT;     /**< [11:11] SysTick is active       */
      typedef RegisterBits< R, 12,  1 > USGFAULTPENDED; /**< [12:12] Usage Fault is pended   */
      typedef RegisterBits< R, 13,  1 > MEMFAULTPENDED; /**< [13:13] MemManage is pended     */
      typedef RegisterBits< R, 14,  1 > BUSFAULTPENDED; /**< [14:14] Bus Fault is pended     */
      typedef RegisterBits< R, 15,  1 > SVCALLPENDED;   /**< [15:15] SVCall is pended        */
      typedef RegisterBits< R, 16,  1 > MEMFAULTENA;    /**< [16:16] MemManage enable        */
      typedef RegisterBits< R, 17,  1 > BUSFAULTENA;    /**< [17:17] Bus Fault enable        */
      typedef RegisterBits< R, 18,  1 > USGFAULTENA;    /**< [18:18] UsageFault enable       */
    };

    /**
     * Configurable Fault Status Registers
     */
    template<typename R>
    struct __CFSR
    : public R
    {
      /* MFSR */
      // TODO: byte-accessible registers
      // RegisterBits< R,  0,  8 > MFSR;
      // RegisterBits< R,  8, 16 > BFSR;
      // RegisterBits< R, 16, 16 > UFSR;
      typedef RegisterBits< R,  0,  1 > IACCVIOL;      /**< [ 0: 0] Instruction access violation   */
      typedef RegisterBits< R,  1,  1 > DACCVIOL;      /**< [ 1: 1] Data access violation          */
      typedef RegisterBits< R,  3,  1 > MUNSTKERR;     /**< [ 3: 3] Unstacking error               */
      typedef RegisterBits< R,  4,  1 > MSTKERR;       /**< [ 4: 4] Stacking error                 */
      /* BFSR */
      typedef RegisterBits< R,  8,  1 > IBUSERR;       /**< [ 8: 8] Instruction bus error flag                      */
      typedef RegisterBits< R,  9,  1 > PRECISERR;     /**< [ 9: 9] Precise data bus error                          */
      typedef RegisterBits< R, 10,  1 > IMPRECISERR;   /**< [10:10] Imprecise data bus error                        */
      typedef RegisterBits< R, 11,  1 > UNSTKERR;      /**< [11:11] Unstacking error                                */
      typedef RegisterBits< R, 12,  1 > STKERR;        /**< [12:12] Stacking error                                  */
      typedef RegisterBits< R, 15,  1 > BFARVALID;     /**< [15:15] Bus Fault Address Register address valid flag   */
      /* UFSR */
      typedef RegisterBits< R, 16,  1 > UNDEFINSTR;    /**< [16:16] The processor attempt to excecute an undefined instruction                 */
      typedef RegisterBits< R, 17,  1 > INVSTATE;      /**< [17:17] Invalid combination of EPSR and instruction                                */
      typedef RegisterBits< R, 18,  1 > INVPC;         /**< [18:18] Attempt to load EXC_RETURN into pc illegally                               */
      typedef RegisterBits< R, 19,  1 > NOCP;          /**< [19:19] Attempt to use a coprocessor instruction                                   */
      typedef RegisterBits< R, 24,  1 > UNALIGNED;     /**< [24:24] Fault occurs when there is an attempt to make an unaligned memory access   */
      typedef RegisterBits< R, 25,  1 > DIVBYZERO;     /**< [25:25] Fault occurs when SDIV or DIV instruction is used with a divisor of 0      */
    };

    /**
     * HardFault Status Register
     */
    template<typename R>
    struct __HFSR
    : public R
    {
      typedef RegisterBits< R,  1,  1 > VECTTBL;       /**< [ 1: 1] Fault occures because of vector table read on exception processing                */
      typedef RegisterBits< R, 30,  1 > FORCED;        /**< [30:30] Hard Fault activated when a configurable Fault was received and cannot activate   */
      typedef RegisterBits< R, 31,  1 > DEBUGEVT;      /**< [31:31] Fault related to debug                                                            */
    };

    /**
     * Debug Fault Status Register
     */
    template<typename R>
    struct __DFSR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > HALTED;        /**< [ 0: 0] Halt request flag                      */
      typedef RegisterBits< R,  1,  1 > BKPT;          /**< [ 1: 1] BKPT flag                              */
      typedef RegisterBits< R,  2,  1 > DWTTRAP;       /**< [ 2: 2] Data Watchpoint and Trace (DWT) flag   */
      typedef RegisterBits< R,  3,  1 > VCATCH;        /**< [ 3: 3] Vector catch flag                      */
      typedef RegisterBits< R,  4,  1 > EXTERNAL;      /**< [ 4: 4] External debug request flag            */
    };

  public:

    typedef          Register< uint32_t, 0xE000E008, access::rw >   ACTLR; /**< Auxiliary Control Register, ACTLR   */
    typedef __STCSR< Register< uint32_t, 0xE000E010, access::rw > > STCSR;
    typedef          Register< uint32_t, 0xE000E014, access::rw >   STRVR; /**< SysTick Reload Value Register        */
    typedef          Register< uint32_t, 0xE000E018, access::rw >   STCVR; /**< SysTick Current Value Register   */
    typedef __STCR < Register< uint32_t, 0xE000E01C, access::ro > > STCR;
    typedef __CPUID< Register< uint32_t, 0xE000ED00, access::ro, 0x412FC231 > > CPUID;
    typedef __ICSR < Register< uint32_t, 0xE000ED04, access::rw > > ICSR;
    typedef __VTOR < Register< uint32_t, 0xE000ED08, access::rw > > VTOR;
    typedef __AIRCR< Register< uint32_t, 0xE000ED0C, access::rw > > AIRCR;
    typedef __SCR  < Register< uint32_t, 0xE000ED10, access::rw > > SCR;
    typedef __CCR  < Register< uint32_t, 0xE000ED14, access::rw, 0x00000200 > > CCR; // TODO: 16-bit

    template<std::size_t index>
    struct SHPR
    : __SHPR < Register< uint32_t, 0xE000ED18 + 4 * index, access::rw > >
    { static_assert(index < 3, "invalid index for register"); };

    typedef __SHCSR< Register< uint32_t, 0xE000ED24, access::rw > > SHCSR;
    typedef __CFSR < Register< uint32_t, 0xE000ED28, access::rw > > CFSR;
    typedef __HFSR < Register< uint32_t, 0xE000ED2C, access::rw > > HFSR;
    typedef __DFSR < Register< uint32_t, 0xE000ED30, access::rw > > DFSR;
    typedef          Register< uint32_t, 0xE000ED34, access::rw             > MMFAR;    /**< MemManage Fault Address Register[b]     */
    typedef          Register< uint32_t, 0xE000ED38, access::rw             > BFAR;     /**< BusFault Address Register[b]            */
    typedef          Register< uint32_t, 0xE000ED3C, access::rw             > AFSR;     /**< Auxiliary Fault Status Register, AFSR   */
    typedef          Register< uint32_t, 0xE000ED40, access::ro, 0x00000030 > ID_PFR0;  /**< Processor Feature Register 0            */
    typedef          Register< uint32_t, 0xE000ED44, access::ro, 0x00000200 > ID_PFR1;  /**< Processor Feature Register 1            */
    typedef          Register< uint32_t, 0xE000ED48, access::ro, 0x00100000 > ID_DFR0;  /**< Debug Features Register 0[c]            */
    typedef          Register< uint32_t, 0xE000ED4C, access::ro             > ID_AFR0;  /**< Auxiliary Features Register 0           */
    typedef          Register< uint32_t, 0xE000ED50, access::ro, 0x00100030 > ID_MMFR0; /**< Memory Model Feature Register 0         */
    typedef          Register< uint32_t, 0xE000ED54, access::ro             > ID_MMFR1; /**< Memory Model Feature Register 1         */
    typedef          Register< uint32_t, 0xE000ED58, access::ro, 0x01000000 > ID_MMFR2; /**< Memory Model Feature Register 2         */
    typedef          Register< uint32_t, 0xE000ED5C, access::ro             > ID_MMFR3; /**< Memory Model Feature Register 3         */
    typedef          Register< uint32_t, 0xE000ED60, access::ro, 0x01100110 > ID_ISAR0; /**< Instruction Set Attributes Register 0   */
    typedef          Register< uint32_t, 0xE000ED64, access::ro, 0x02111000 > ID_ISAR1; /**< Instruction Set Attributes Register 1   */
    typedef          Register< uint32_t, 0xE000ED68, access::ro, 0x21112231 > ID_ISAR2; /**< Instruction Set Attributes Register 2   */
    typedef          Register< uint32_t, 0xE000ED6C, access::ro, 0x01111110 > ID_ISAR3; /**< Instruction Set Attributes Register 3   */
    typedef          Register< uint32_t, 0xE000ED70, access::ro, 0x01310132 > ID_ISAR4; /**< Instruction Set Attributes Register 4   */
    typedef          Register< uint32_t, 0xE000ED88, access::rw             > CPACR;    /**< Coprocessor Access Control Register     */
    typedef          Register< uint32_t, 0xE000EF00, access::wo             > STIR;     /**< Software Triggered Interrupt Register   */
  };



  /**
   *  MPU (Memory Protection Unit) Register
   *
   * Imported from the Cortex-M3 Technical Reference Manual  (Revision: r2p1)
   * <http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3/index.html>
   */
  struct MPU
  {
    typedef   Register< uint32_t, 0xE000ED90, access::ro, 0x00000800 > TYPE;    /**< MPU Type Register                        */
    typedef   Register< uint32_t, 0xE000ED94, access::rw             > CTRL;    /**< MPU Control Register                     */
    typedef   Register< uint32_t, 0xE000ED98, access::rw             > RNR;     /**< MPU Region Number Register               */
    typedef   Register< uint32_t, 0xE000ED9C, access::rw             > RBAR;    /**< MPU Region Base Address Register         */
    typedef   Register< uint32_t, 0xE000EDA0, access::rw             > RASR;    /**< MPU Region Attribute and Size Register   */
    typedef   Register< uint32_t, 0xE000EDA4, access::rw             > RBAR_A1; /**< MPU alias registers                      */
    typedef   Register< uint32_t, 0xE000EDA8, access::rw             > RASR_A1; /**<                                          */
    typedef   Register< uint32_t, 0xE000EDAC, access::rw             > RBAR_A2; /**<                                          */
    typedef   Register< uint32_t, 0xE000EDB0, access::rw             > RASR_A2; /**<                                          */
    typedef   Register< uint32_t, 0xE000EDB4, access::rw             > RBAR_A3; /**<                                          */
    typedef   Register< uint32_t, 0xE000EDB8, access::rw             > RASR_A3; /**<                                          */
  };



  /**
   *  NVIC (Nested Vectored Interrupt Controller) Register
   *
   * Imported from the Cortex-M3 Technical Reference Manual  (Revision: r2p1)
   * <http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3/index.html>
   */
  struct NVIC
  {
    /** Interrupt Controller Type Register */
    typedef Register< uint32_t, 0xE000E004, access::ro > ICTR;

    // TODO: The following registers are actually only 8bit wide.
    //       Check if access is better using 32bit or 8bit pointer

    /** Interrupt Set-Enable Registers */
    template<std::size_t reg_index>
    struct ISER
    : Register<uint32_t, 0xE000E100 + 4 * reg_index, access::rw >
    { static_assert(reg_index < 8, "invalid index for register"); };

    /** Interrupt Set-Enable Registers */
    template<std::size_t reg_index>
    struct ICER : Register<uint32_t, 0xE000E180 + 4 * reg_index, access::rw >
    { static_assert(reg_index < 8, "invalid index for register"); };

    /** Interrupt Set-Pending Registers */
    template<std::size_t reg_index>
    struct ISPR : Register<uint32_t, 0xE000E200 + 4 * reg_index, access::rw >
    { static_assert(reg_index < 8, "invalid index for register"); };

    /** Interrupt Clear-Pending Registers */
    template<std::size_t reg_index>
    struct ICPR : Register<uint32_t, 0xE000E280 + 4 * reg_index, access::rw >
    { static_assert(reg_index < 8, "invalid index for register"); };

    /** Interrupt Active Bit Register */
    template<std::size_t reg_index>
    struct IABR : Register<uint32_t, 0xE000E300 + 4 * reg_index, access::ro >
    { static_assert(reg_index < 8, "invalid index for register"); };

   /** Interrupt Priority Register */
    template<std::size_t index>
    struct IPR  : Register<uint32_t, 0xE000E400 + 4 * index, access::rw >
    { static_assert(index < 60, "invalid index for register"); };
  };
};




#endif // COMMON_ARM_CORTEX_CORE_REGISTER_HPP_INCLUDED
