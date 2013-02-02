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

#ifndef REG_PWR_HPP_INCLUDED
#define REG_PWR_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /**
   * Power control registers (PWR)
   */
  class PWR
  {
    static constexpr reg_addr_t reg_base = 0x40007000;

    /**
     * Power control register
     */
    template<typename R>
    class __CR
    : public R
    {
      /** PVD Level Selection  */
      template<typename Rb>
      struct __PLS
      : public Rb
      {
        /** PVD Threshold of 2.<fraction> volts (e.g. 2.5V if fraction=5) */
        template<unsigned int fraction>
        struct VoltageThreshold
        : public RegisterConst< Rb, fraction - 2 >
        { static_assert((fraction >= 2) && (fraction <= 9), "invalid fraction"); };
      };

    public:

      typedef         RegisterBits< R,  0,  1 >   LPDS; /**< [ 0: 0] Low-Power Deepsleep                      */
      typedef         RegisterBits< R,  1,  1 >   PDDS; /**< [ 1: 1] Power Down Deepsleep                     */
      typedef         RegisterBits< R,  2,  1 >   CWUF; /**< [ 2: 2] Clear Wakeup Flag                        */
      typedef         RegisterBits< R,  3,  1 >   CSBF; /**< [ 3: 3] Clear Standby Flag                       */
      typedef         RegisterBits< R,  4,  1 >   PVDE; /**< [ 4: 4] Power Voltage Detector Enable            */
      typedef __PLS < RegisterBits< R,  5,  3 > > PLS;  /**< [ 7: 5] PVD Level Selection                      */
      typedef         RegisterBits< R,  8,  1 >   DBP;  /**< [ 8: 8] Disable Backup Domain write protection   */
    };


    /**
     * Power control/status register
     */
    template<typename R>
    struct __CSR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > WUF;           /**< [ 0: 0] Wakeup Flag       */
      typedef RegisterBits< R,  1,  1 > SBF;           /**< [ 1: 1] Standby Flag      */
      typedef RegisterBits< R,  2,  1 > PVDO;          /**< [ 2: 2] PVD Output        */
      typedef RegisterBits< R,  8,  1 > EWUP;          /**< [ 8: 8] Enable WKUP pin   */
    };

  public:

    typedef __CR < Register< uint32_t, reg_base + 0x00, Access::rw > > CR;
    typedef __CSR< Register< uint32_t, reg_base + 0x04, Access::rw > > CSR;
  };
}

#endif // REG_PWR_HPP_INCLUDED
