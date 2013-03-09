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
    static_assert((reg_name >= 'A') && (reg_name <= 'I'), "Invalid index for GPIO register");

    static constexpr std::size_t gpio_no  = reg_name - 'A';
    static constexpr reg_addr_t  reg_base = 0x40020000 + (gpio_no * 0x0400);

    static constexpr uint32_t moder_reset   = ( reg_name == 'A' ? 0xA8000000 :
                                                reg_name == 'B' ? 0x00000280 :
                                                0x00000000 );

    static constexpr uint32_t ospeedr_reset = ( reg_name == 'B' ? 0x000000C0 :
                                                0x00000000 );

    static constexpr uint32_t pupdr_reset   = ( reg_name == 'A' ? 0x64000000 :
                                                reg_name == 'B' ? 0x00000100 :
                                                0x00000000 );

    typedef Register< uint32_t, reg_base + 0x00, Access::rw, moder_reset   > MODER;   /**< GPIO port mode register               */
    typedef Register< uint32_t, reg_base + 0x04, Access::rw                > OTYPER;  /**< GPIO port output type register        */
    typedef Register< uint32_t, reg_base + 0x08, Access::rw, ospeedr_reset > OSPEEDR; /**< GPIO port output speed register       */
    typedef Register< uint32_t, reg_base + 0x0c, Access::rw, pupdr_reset   > PUPDR;   /**< GPIO port pull-up/pull-down register  */
    typedef Register< uint32_t, reg_base + 0x10, Access::ro /*0x0000XXXX*/ > IDR;     /**< GPIO port input data register         */
    typedef Register< uint32_t, reg_base + 0x14, Access::rw                > ODR;     /**< GPIO port output data register        */
    typedef Register< uint32_t, reg_base + 0x18, Access::wo                > BSRR;    /**< GPIO port bit set/reset register      */
    typedef Register< uint32_t, reg_base + 0x1c, Access::rw                > LCKR;    /**< GPIO port configuration lock register */
    typedef Register< uint32_t, reg_base + 0x20, Access::rw                > AFRL;    /**< GPIO alternate function low register  */
    typedef Register< uint32_t, reg_base + 0x24, Access::rw                > AFRH;    /**< GPIO alternate function high register */
  };
}

#endif // REG_GPIO_HPP_INCLUDED
