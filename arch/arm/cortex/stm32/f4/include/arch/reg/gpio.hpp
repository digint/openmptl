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

#ifndef ARCH_REG_GPIO_HPP_INCLUDED
#define ARCH_REG_GPIO_HPP_INCLUDED

#include <register.hpp>

namespace mptl { namespace reg {

/**
 * General-purpose and alternate-function I/Os (GPIOs and AFIOs)
 */
template<char reg_name>
struct GPIO
{
  static_assert((reg_name >= 'A') && (reg_name <= 'I'), "invalid index for GPIO register");

  static constexpr unsigned   gpio_no   = reg_name - 'A';
  static constexpr reg_addr_t base_addr = 0x40020000 + (gpio_no * 0x0400);

  static constexpr uint32_t moder_reset   = ( reg_name == 'A' ? 0xA8000000 :
                                              reg_name == 'B' ? 0x00000280 :
                                              0x00000000 );

  static constexpr uint32_t ospeedr_reset = ( reg_name == 'B' ? 0x000000C0 :
                                              0x00000000 );

  static constexpr uint32_t pupdr_reset   = ( reg_name == 'A' ? 0x64000000 :
                                              reg_name == 'B' ? 0x00000100 :
                                              0x00000000 );

  using MODER    = regdef< uint32_t, base_addr + 0x00, reg_access::rw, moder_reset   >;  /**< GPIO port mode register               */
  using OTYPER   = regdef< uint32_t, base_addr + 0x04, reg_access::rw                >;  /**< GPIO port output type register        */
  using OSPEEDR  = regdef< uint32_t, base_addr + 0x08, reg_access::rw, ospeedr_reset >;  /**< GPIO port output speed register       */
  using PUPDR    = regdef< uint32_t, base_addr + 0x0c, reg_access::rw, pupdr_reset   >;  /**< GPIO port pull-up/pull-down register  */
  using IDR      = regdef< uint32_t, base_addr + 0x10, reg_access::ro /*0x0000XXXX*/ >;  /**< GPIO port input data register         */
  using ODR      = regdef< uint32_t, base_addr + 0x14, reg_access::rw                >;  /**< GPIO port output data register        */
  using BSRR     = regdef< uint32_t, base_addr + 0x18, reg_access::wo                >;  /**< GPIO port bit set/reset register      */
  using LCKR     = regdef< uint32_t, base_addr + 0x1c, reg_access::rw                >;  /**< GPIO port configuration lock register */
  using AFRL     = regdef< uint32_t, base_addr + 0x20, reg_access::rw                >;  /**< GPIO alternate function low register  */
  using AFRH     = regdef< uint32_t, base_addr + 0x24, reg_access::rw                >;  /**< GPIO alternate function high register */
};

} } // namespace mptl::reg

#endif // ARCH_REG_GPIO_HPP_INCLUDED
