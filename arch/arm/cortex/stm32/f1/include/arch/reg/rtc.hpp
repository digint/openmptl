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

#ifndef REG_RTC_HPP_INCLUDED
#define REG_RTC_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /**
   * Real-time clock (RTC)
   *
   * Note that the registers are actually only 16bit wide, but accessing
   * them with 32bit is faster in general.
   */
  class RTC
  {
    static constexpr reg_addr_t reg_base = 0x40002800;

    /**
     * RTC control register high
     */
    template<typename R>
    struct __CRH
    : public R
    {
      typedef RegisterBits< R,  0,  1 > SECIE;         /**< [ 0: 0] Second Interrupt Enable     */
      typedef RegisterBits< R,  1,  1 > ALRIE;         /**< [ 1: 1] Alarm Interrupt Enable      */
      typedef RegisterBits< R,  2,  1 > OWIE;          /**< [ 2: 2] OverfloW Interrupt Enable   */
    };


    /**
     * RTC control register low
     */
    template<typename R>
    struct __CRL
    : public R
    {
      typedef RegisterBits< R,  0,  1 > SECF;          /**< [ 0: 0] Second Flag                   */
      typedef RegisterBits< R,  1,  1 > ALRF;          /**< [ 1: 1] Alarm Flag                    */
      typedef RegisterBits< R,  2,  1 > OWF;           /**< [ 2: 2] OverfloW Flag                 */
      typedef RegisterBits< R,  3,  1 > RSF;           /**< [ 3: 3] Registers Synchronized Flag   */
      typedef RegisterBits< R,  4,  1 > CNF;           /**< [ 4: 4] Configuration Flag            */
      typedef RegisterBits< R,  5,  1 > RTOFF;         /**< [ 5: 5] RTC operation OFF             */
    };

    /**
     * RTC prescaler load register high
     */
    template<typename R>
    struct __PRLH
    : public R
    {
      typedef RegisterBits< R,  0,  4 > PRL;           /**< [ 3: 0] RTC Prescaler Reload Value High   */
    };

    /**
     * RTC prescaler load register low
     */
    template<typename R>
    struct __PRLL
    : public R
    {
      typedef RegisterBits< R,  0, 16 > PRL;           /**< [15: 0] RTC Prescaler Reload Value Low   */
    };

    /**
     * RTC prescaler divider register high
     */
    template<typename R>
    struct __DIVH
    : public R
    {
      typedef RegisterBits< R,  0,  4 > RTC_DIV;       /**< [ 3: 0] RTC Clock Divider High   */
    };

    /**
     * RTC prescaler divider register low
     */
    template<typename R>
    struct __DIVL
    : public R
    {
      typedef RegisterBits< R,  0, 16 > RTC_DIV;       /**< [15: 0] RTC Clock Divider Low   */
    };

    /**
     * RTC counter register high
     */
    template<typename R>
    struct __CNTH
    : public R
    {
      typedef RegisterBits< R,  0, 16 > RTC_CNT;       /**< [15: 0] RTC Counter High   */
    };

    /**
     * RTC counter register low
     */
    template<typename R>
    struct __CNTL
    : public R
    {
      typedef RegisterBits< R,  0, 16 > RTC_CNT;       /**< [15: 0] RTC Counter Low   */
    };

    /**
     * RTC alarm register high
     */
    template<typename R>
    struct __ALRH
    : public R
    {
      typedef RegisterBits< R,  0, 16 > RTC_ALR;       /**< [15: 0] RTC Alarm High   */
    };

    /**
     * RTC alarm register low
     */
    template<typename R>
    struct __ALRL
    : public R
    {
      typedef RegisterBits< R,  0, 16 > RTC_ALR;       /**< [15: 0] RTC Alarm Low   */
    };


  public:

    // TODO: use "uint_fast16_t", this is way cooler!
    typedef __CRH < Register< uint32_t, reg_base + 0x00, Access::rw         > > CRH;  /**< RTC control register high             */
    typedef __CRL < Register< uint32_t, reg_base + 0x04, Access::rw, 0x0020 > > CRL;  /**< RTC control register low              */
    typedef __PRLH< Register< uint32_t, reg_base + 0x08, Access::wo         > > PRLH; /**< RTC prescaler load register high      */
    typedef __PRLL< Register< uint32_t, reg_base + 0x0c, Access::wo, 0x8000 > > PRLL; /**< RTC prescaler load register low       */
    typedef __DIVH< Register< uint32_t, reg_base + 0x10, Access::ro         > > DIVH; /**< RTC prescaler divider register high   */
    typedef __DIVL< Register< uint32_t, reg_base + 0x14, Access::ro, 0x8000 > > DIVL; /**< RTC prescaler divider register low    */
    typedef __CNTH< Register< uint32_t, reg_base + 0x18, Access::rw         > > CNTH; /**< RTC counter register high             */
    typedef __CNTL< Register< uint32_t, reg_base + 0x1c, Access::rw         > > CNTL; /**< RTC counter register low              */
    typedef __ALRH< Register< uint32_t, reg_base + 0x20, Access::wo         > > ALRH; /**< RTC alarm register high               */
    typedef __ALRL< Register< uint32_t, reg_base + 0x24, Access::wo         > > ALRL; /**< RTC alarm register low                */
  };
}

#endif // REG_RTC_HPP_INCLUDED
