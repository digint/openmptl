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

#ifndef REG_GPIO_HPP_INCLUDED
#define REG_GPIO_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /**
   * General-purpose and alternate-function I/Os (GPIOs and AFIOs)
   */
  template<char reg_name>
  struct GPIO
  {
    static_assert((reg_name >= 'A') && (reg_name <= 'G'), "invalid index for register");

    static constexpr unsigned    gpio_no   = reg_name - 'A';
    static constexpr reg_addr_t  base_addr = 0x40010800 + gpio_no * 0x0400;

    using CRL   = Register< uint32_t, base_addr + 0x00, Access::rw, 0x44444444 >;  /**< Port configuration register low    */
    using CRH   = Register< uint32_t, base_addr + 0x04, Access::rw, 0x44444444 >;  /**< Port configuration register high   */
    using IDR   = Register< uint32_t, base_addr + 0x08, Access::ro, 0x00000000 >;  /**< Port input data register           */
    using ODR   = Register< uint32_t, base_addr + 0x0c, Access::rw, 0x00000000 >;  /**< Port output data register          */
    using BSRR  = Register< uint32_t, base_addr + 0x10, Access::wo, 0x00000000 >;  /**< Port bit set/reset register        */
    using BRR   = Register< uint32_t, base_addr + 0x14, Access::wo, 0x00000000 >;  /**< Port bit reset register            */
    using LCKR  = Register< uint32_t, base_addr + 0x18, Access::rw, 0x00000000 >;  /**< Port configuration lock register   */
  };
}

#endif // REG_GPIO_HPP_INCLUDED
