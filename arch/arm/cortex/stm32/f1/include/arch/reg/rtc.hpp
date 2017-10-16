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

/*
 * This program contains derivative representations of CMSIS System
 * View Description (SVD) files, and is subject to the "End User
 * License Agreement for STMicroelectronics" (see "STM_License.html"
 * in the containing directory).
 */

#ifndef ARCH_REG_RTC_HPP_INCLUDED
#define ARCH_REG_RTC_HPP_INCLUDED

#include <register.hpp>

namespace mptl {

/**
 * Real time clock
 */
struct RTC
{
  static constexpr reg_addr_t base_addr = 0x40002800;

  /**
   * RTC Control register high
   */
  struct CRH
  : public reg< std::uint_fast16_t, base_addr + 0x0, rw, 0x0000 >
  {
    using SECIE  = regbits< type,  0,  1 >;  /**< Second interrupt Enable    */
    using ALRIE  = regbits< type,  1,  1 >;  /**< Alarm interrupt Enable     */
    using OWIE   = regbits< type,  2,  1 >;  /**< Overflow interrupt Enable  */
  };

  /**
   * RTC Control register low
   */
  struct CRL
  : public reg< std::uint_fast16_t, base_addr + 0x4, rw, 0x0020 >
  {
    using SECF   = regbits< type,  0,  1 >;  /**< Second Flag                  */
    using ALRF   = regbits< type,  1,  1 >;  /**< Alarm Flag                   */
    using OWF    = regbits< type,  2,  1 >;  /**< Overflow Flag                */
    using RSF    = regbits< type,  3,  1 >;  /**< Registers Synchronized Flag  */
    using CNF    = regbits< type,  4,  1 >;  /**< Configuration Flag           */
    using RTOFF  = regbits< type,  5,  1 >;  /**< RTC operation OFF            */
  };

  /**
   * RTC Prescaler load register high
   */
  struct PRLH
  : public reg< std::uint_fast16_t, base_addr + 0x8, wo, 0x0000 >
  {
    using regbits_type = regbits< type,  0,  4 >;  /**< RTC Prescaler Load Register High  */
  };

  /**
   * RTC Prescaler load register low
   */
  struct PRLL
  : public reg< std::uint_fast16_t, base_addr + 0xc, wo, 0x8000 >
  {
    using regbits_type = regbits< type,  0, 16 >;  /**< RTC Prescaler Divider Register Low  */
  };

  /**
   * RTC Prescaler divider register high
   */
  struct DIVH
  : public reg< std::uint_fast16_t, base_addr + 0x10, ro, 0x0000 >
  {
    using regbits_type = regbits< type,  0,  4 >;  /**< RTC prescaler divider register high  */
  };

  /**
   * RTC Prescaler divider register low
   */
  struct DIVL
  : public reg< std::uint_fast16_t, base_addr + 0x14, ro, 0x8000 >
  {
    using regbits_type = regbits< type,  0, 16 >;  /**< RTC prescaler divider register Low  */
  };

  /**
   * RTC Counter register high
   */
  struct CNTH
  : public reg< std::uint_fast16_t, base_addr + 0x18, rw, 0x0000 >
  {
    using regbits_type = regbits< type,  0, 16 >;  /**< RTC counter register high  */
  };

  /**
   * RTC Counter register low
   */
  struct CNTL
  : public reg< std::uint_fast16_t, base_addr + 0x1c, rw, 0x0000 >
  {
    using regbits_type = regbits< type,  0, 16 >;  /**< RTC counter register Low  */
  };

  /**
   * RTC Alarm register high
   */
  struct ALRH
  : public reg< std::uint_fast16_t, base_addr + 0x20, wo, 0xFFFF >
  {
    using regbits_type = regbits< type,  0, 16 >;  /**< RTC alarm register high  */
  };

  /**
   * RTC Alarm register low
   */
  struct ALRL
  : public reg< std::uint_fast16_t, base_addr + 0x24, wo, 0xFFFF >
  {
    using regbits_type = regbits< type,  0, 16 >;  /**< RTC alarm register low  */
  };
};

} // namespace mptl

#endif // ARCH_REG_RTC_HPP_INCLUDED
