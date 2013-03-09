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

#ifndef COMMON_ARM_CORTEX_REG_DEBUG_HPP_INCLUDED
#define COMMON_ARM_CORTEX_REG_DEBUG_HPP_INCLUDED

namespace reg {

  /**
   * Debug Register
   *
   * Imported from the Cortex-M3 Technical Reference Manual  (Revision: r2p1)
   * <http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3/index.html>
   */
  struct DEBUG
  {
    /**
     * Debug Exception and Monitor Control Register
     */
    template<typename R>
    struct __DEMCR
    : public R
    {
      typedef RegisterBits< R, 24,  1 > TRCENA;   /**< Enable DWT */
    };

    typedef Register<uint32_t, 0xE000ED30, Access::rw> DFSR;   /**< Debug Fault Status Register                   */
    typedef Register<uint32_t, 0xE000EDF0, Access::rw> DHCSR;  /**< Debug Halting Control and Status Register     */
    typedef Register<uint32_t, 0xE000EDF4, Access::wo> DCRSR;  /**< Debug Core Register Selector Register         */
    typedef Register<uint32_t, 0xE000EDF8, Access::rw> DCRDR;  /**< Debug Core Register Data Register             */
    typedef __DEMCR< Register<uint32_t, 0xE000EDFC, Access::rw> > DEMCR;  /**< Debug Exception and Monitor Control Register  */
  };
}

#endif // COMMON_ARM_CORTEX_REG_DEBUG_HPP_INCLUDED
