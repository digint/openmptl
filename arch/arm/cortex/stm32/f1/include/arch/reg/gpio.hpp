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

#ifndef ARCH_REG_GPIO_HPP_INCLUDED
#define ARCH_REG_GPIO_HPP_INCLUDED

#include <register.hpp>

namespace mptl {

/**
 * General-purpose and alternate-function I/Os (GPIOs and AFIOs)
 */
template< char port >
struct GPIO
{
  static_assert((port >= 'A') && (port <= 'G'), "invalid index for register");

  static constexpr unsigned    gpio_no   = port - 'A';
  static constexpr reg_addr_t  base_addr = 0x40010800 + gpio_no * 0x0400;

  using CRL   = reg< uint32_t, base_addr + 0x00, rw, 0x44444444 >;  /**< Port configuration register low    */
  using CRH   = reg< uint32_t, base_addr + 0x04, rw, 0x44444444 >;  /**< Port configuration register high   */
  using IDR   = reg< uint32_t, base_addr + 0x08, ro, 0x00000000 >;  /**< Port input data register           */
  using ODR   = reg< uint32_t, base_addr + 0x0c, rw, 0x00000000 >;  /**< Port output data register          */
  using BSRR  = reg< uint32_t, base_addr + 0x10, wo, 0x00000000 >;  /**< Port bit set/reset register        */
  using BRR   = reg< uint32_t, base_addr + 0x14, wo, 0x00000000 >;  /**< Port bit reset register            */
  using LCKR  = reg< uint32_t, base_addr + 0x18, rw, 0x00000000 >;  /**< Port configuration lock register   */

  /**
   * GPIO port configuration register: returns CRL or CRH type dependent on pin_no.
   *
   * NOTE: this is not from the reference manual 
   */
  template<unsigned pin_no>
  struct CRx
  : public std::conditional< (pin_no < 8), CRL, CRH >::type
  {
    using type = typename std::conditional< (pin_no < 8), CRL, CRH >::type;

    using CNF  = regbits< type, (pin_no % 8) * 4 + 2, 2 >;
    using MODE = regbits< type, (pin_no % 8) * 4    , 2 >;
  };
};

} // namespace mptl

#endif // ARCH_REG_GPIO_HPP_INCLUDED
