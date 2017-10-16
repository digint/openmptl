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

#ifndef ARM_CORTEX_COMMON_REG_NVIC_HPP_INCLUDED
#define ARM_CORTEX_COMMON_REG_NVIC_HPP_INCLUDED

#include <register.hpp>

namespace mptl {

/**
 *  NVIC (Nested Vectored Interrupt Controller) Register
 *
 * For details, see "Cortex-M3 Technical Reference Manual":
 * <http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3/index.html>
 */
struct NVIC
{
  /** Interrupt Controller Type Register */
  using ICTR  = reg< uint32_t, 0xE000E004, ro >;

  // TODO: The following registers are actually only 8bit wide.
  //       Check if access is better using 32bit or 8bit pointer

  /** Interrupt Set-Enable Registers */
  template<unsigned reg_index>
  class ISER : public reg<uint32_t, 0xE000E100 + 4 * reg_index, rw >
  { static_assert(reg_index < 8, "invalid index for register"); };

  /** Interrupt Set-Enable Registers */
  template<unsigned reg_index>
  class ICER : public reg<uint32_t, 0xE000E180 + 4 * reg_index, rw >
  { static_assert(reg_index < 8, "invalid index for register"); };

  /** Interrupt Set-Pending Registers */
  template<unsigned reg_index>
  class ISPR : public reg<uint32_t, 0xE000E200 + 4 * reg_index, rw >
  { static_assert(reg_index < 8, "invalid index for register"); };

  /** Interrupt Clear-Pending Registers */
  template<unsigned reg_index>
  class ICPR : public reg<uint32_t, 0xE000E280 + 4 * reg_index, rw >
  { static_assert(reg_index < 8, "invalid index for register"); };

  /** Interrupt Active Bit Register */
  template<unsigned reg_index>
  class IABR : public reg<uint32_t, 0xE000E300 + 4 * reg_index, ro >
  { static_assert(reg_index < 8, "invalid index for register"); };

  /** Interrupt Priority Register */
  template<unsigned reg_index>
  class IPR  : public reg<uint32_t, 0xE000E400 + 4 * reg_index, rw >
  { static_assert(reg_index < 60, "invalid index for register"); };
};

} // namespace mptl

#endif // ARM_CORTEX_COMMON_REG_NVIC_HPP_INCLUDED
