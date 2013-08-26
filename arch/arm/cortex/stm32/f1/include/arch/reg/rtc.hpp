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

#ifndef ARCH_REG_RTC_HPP_INCLUDED
#define ARCH_REG_RTC_HPP_INCLUDED

#include <register.hpp>

namespace mptl { namespace reg {

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
  : public regdef< std::uint_fast16_t, base_addr + 0x0, reg_access::rw, 0x0000 >
  {
    using SECIE  = regbits< type,  0,  1 >;  /**< Second interrupt Enable    */
    using ALRIE  = regbits< type,  1,  1 >;  /**< Alarm interrupt Enable     */
    using OWIE   = regbits< type,  2,  1 >;  /**< Overflow interrupt Enable  */
  };

  /**
   * RTC Control register low
   */
  struct CRL
  : public regdef< std::uint_fast16_t, base_addr + 0x4, reg_access::rw, 0x0020 >
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
  : public regdef< std::uint_fast16_t, base_addr + 0x8, reg_access::wo, 0x0000 >
  {
    using regbits_type = regbits< type,  0,  4 >;  /**< RTC Prescaler Load Register High  */
  };

  /**
   * RTC Prescaler load register low
   */
  struct PRLL
  : public regdef< std::uint_fast16_t, base_addr + 0xc, reg_access::wo, 0x8000 >
  {
    using regbits_type = regbits< type,  0, 16 >;  /**< RTC Prescaler Divider Register Low  */
  };

  /**
   * RTC Prescaler divider register high
   */
  struct DIVH
  : public regdef< std::uint_fast16_t, base_addr + 0x10, reg_access::ro, 0x0000 >
  {
    using regbits_type = regbits< type,  0,  4 >;  /**< RTC prescaler divider register high  */
  };

  /**
   * RTC Prescaler divider register low
   */
  struct DIVL
  : public regdef< std::uint_fast16_t, base_addr + 0x14, reg_access::ro, 0x8000 >
  {
    using regbits_type = regbits< type,  0, 16 >;  /**< RTC prescaler divider register Low  */
  };

  /**
   * RTC Counter register high
   */
  struct CNTH
  : public regdef< std::uint_fast16_t, base_addr + 0x18, reg_access::rw, 0x0000 >
  {
    using regbits_type = regbits< type,  0, 16 >;  /**< RTC counter register high  */
  };

  /**
   * RTC Counter register low
   */
  struct CNTL
  : public regdef< std::uint_fast16_t, base_addr + 0x1c, reg_access::rw, 0x0000 >
  {
    using regbits_type = regbits< type,  0, 16 >;  /**< RTC counter register Low  */
  };

  /**
   * RTC Alarm register high
   */
  struct ALRH
  : public regdef< std::uint_fast16_t, base_addr + 0x20, reg_access::wo, 0xFFFF >
  {
    using regbits_type = regbits< type,  0, 16 >;  /**< RTC alarm register high  */
  };

  /**
   * RTC Alarm register low
   */
  struct ALRL
  : public regdef< std::uint_fast16_t, base_addr + 0x24, reg_access::wo, 0xFFFF >
  {
    using regbits_type = regbits< type,  0, 16 >;  /**< RTC alarm register low  */
  };
};

} } // namespace mptl::reg

#endif // ARCH_REG_RTC_HPP_INCLUDED
