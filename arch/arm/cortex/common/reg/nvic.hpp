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

#ifndef COMMON_ARM_CORTEX_REG_NVIC_HPP_INCLUDED
#define COMMON_ARM_CORTEX_REG_NVIC_HPP_INCLUDED

namespace reg {

  /**
   *  NVIC (Nested Vectored Interrupt Controller) Register
   *
   * Imported from the Cortex-M3 Technical Reference Manual  (Revision: r2p1)
   * <http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3/index.html>
   */
  struct NVIC
  {
    /** Interrupt Controller Type Register */
    typedef Register< uint32_t, 0xE000E004, Access::ro > ICTR;

    // TODO: The following registers are actually only 8bit wide.
    //       Check if access is better using 32bit or 8bit pointer

    /** Interrupt Set-Enable Registers */
    template<unsigned reg_index>
    struct ISER
    : Register<uint32_t, 0xE000E100 + 4 * reg_index, Access::rw >
    { static_assert(reg_index < 8, "invalid index for register"); };

    /** Interrupt Set-Enable Registers */
    template<unsigned reg_index>
    struct ICER : Register<uint32_t, 0xE000E180 + 4 * reg_index, Access::rw >
    { static_assert(reg_index < 8, "invalid index for register"); };

    /** Interrupt Set-Pending Registers */
    template<unsigned reg_index>
    struct ISPR : Register<uint32_t, 0xE000E200 + 4 * reg_index, Access::rw >
    { static_assert(reg_index < 8, "invalid index for register"); };

    /** Interrupt Clear-Pending Registers */
    template<unsigned reg_index>
    struct ICPR : Register<uint32_t, 0xE000E280 + 4 * reg_index, Access::rw >
    { static_assert(reg_index < 8, "invalid index for register"); };

    /** Interrupt Active Bit Register */
    template<unsigned reg_index>
    struct IABR : Register<uint32_t, 0xE000E300 + 4 * reg_index, Access::ro >
    { static_assert(reg_index < 8, "invalid index for register"); };

   /** Interrupt Priority Register */
    template<unsigned reg_index>
    struct IPR  : Register<uint32_t, 0xE000E400 + 4 * reg_index, Access::rw >
    { static_assert(reg_index < 60, "invalid index for register"); };
  };
}

#endif // COMMON_ARM_CORTEX_REG_NVIC_HPP_INCLUDED
