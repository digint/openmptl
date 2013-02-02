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

    static constexpr std::size_t gpio_no   = reg_name - 'A';
    static constexpr reg_addr_t  reg_base = 0x40010800 + gpio_no * 0x0400;
    
    typedef Register< uint32_t, reg_base + 0x00, Access::rw, 0x44444444 > CRL;            /**< Port configuration register low    */
    typedef Register< uint32_t, reg_base + 0x04, Access::rw, 0x44444444 > CRH;            /**< Port configuration register high   */
    typedef Register< uint32_t, reg_base + 0x08, Access::ro /* TODO: 0x0000XXXX */ > IDR; /**< Port input data register           */
    typedef Register< uint32_t, reg_base + 0x0c, Access::rw             > ODR;            /**< Port output data register          */
    typedef Register< uint32_t, reg_base + 0x10, Access::wo             > BSRR;           /**< Port bit set/reset register        */
    typedef Register< uint32_t, reg_base + 0x14, Access::wo             > BRR;            /**< Port bit reset register            */
    typedef Register< uint32_t, reg_base + 0x18, Access::rw             > LCKR;           /**< Port configuration lock register   */
  };
}

#endif // REG_GPIO_HPP_INCLUDED
