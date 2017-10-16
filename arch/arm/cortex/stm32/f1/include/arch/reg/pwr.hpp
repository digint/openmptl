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

#ifndef ARCH_REG_PWR_HPP_INCLUDED
#define ARCH_REG_PWR_HPP_INCLUDED

#include <register.hpp>

namespace mptl {

/**
 * Power control
 */
struct PWR
{
  static constexpr reg_addr_t base_addr = 0x40007000;

  /**
   * Power control register (PWR_CR)
   */
  struct CR
  : public reg< uint32_t, base_addr + 0x0, rw, 0x00000000 >
  {
    using type = reg< uint32_t, base_addr + 0x0, rw, 0x00000000 >;

    using LPDS  = regbits< type,  0,  1 >;    /**< Low Power Deep Sleep                    */
    using PDDS  = regbits< type,  1,  1 >;    /**< Power Down Deep Sleep                   */
    using CWUF  = regbits< type,  2,  1 >;    /**< Clear Wake-up Flag                      */
    using CSBF  = regbits< type,  3,  1 >;    /**< Clear STANDBY Flag                      */
    using PVDE  = regbits< type,  4,  1 >;    /**< Power Voltage Detector Enable           */

    /** PVD Level Selection */
    struct PLS  : public regbits< type,  5,  3 >
    {
      template<unsigned int fraction>
      struct voltage_threshold
      : public regval< regbits_type, fraction - 2 >
      { static_assert((fraction >= 2) && (fraction <= 9), "invalid fraction"); };
    };

    using DBP   = regbits< type,  8,  1 >;    /**< Disable Backup Domain write protection  */
  };

  /**
   * Power control register (PWR_CR)
   */
  struct CSR
  : public reg< uint32_t, base_addr + 0x4, rw, 0x00000000 >
  {
    using type = reg< uint32_t, base_addr + 0x4, rw, 0x00000000 >;

    using WUF   = regbits< type,  0,  1 >;  /**< Wake-Up Flag     */
    using SBF   = regbits< type,  1,  1 >;  /**< STANDBY Flag     */
    using PVDO  = regbits< type,  2,  1 >;  /**< PVD Output       */
    using EWUP  = regbits< type,  8,  1 >;  /**< Enable WKUP pin  */
  };
};

} // namespace mptl

#endif // ARCH_REG_PWR_HPP_INCLUDED
