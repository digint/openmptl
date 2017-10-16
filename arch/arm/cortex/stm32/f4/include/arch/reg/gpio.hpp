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

#ifndef ARCH_REG_GPIO_HPP_INCLUDED
#define ARCH_REG_GPIO_HPP_INCLUDED

#include <register.hpp>
#include <type_traits>

namespace mptl {

/**
 * General-purpose and alternate-function I/Os (GPIOs and AFIOs)
 */
template< char port >
struct GPIO
{
  static_assert((port >= 'A') && (port <= 'I'), "invalid index for GPIO register");

  static constexpr unsigned   gpio_no   = port - 'A';
  static constexpr reg_addr_t base_addr = 0x40020000 + (gpio_no * 0x0400);

  static constexpr uint32_t moder_reset   = ( port == 'A' ? 0xA8000000 :
                                              port == 'B' ? 0x00000280 :
                                              0x00000000 );

  static constexpr uint32_t ospeedr_reset = ( port == 'B' ? 0x000000C0 :
                                              0x00000000 );

  static constexpr uint32_t pupdr_reset   = ( port == 'A' ? 0x64000000 :
                                              port == 'B' ? 0x00000100 :
                                              0x00000000 );

  using MODER    = reg< uint32_t, base_addr + 0x00, rw, moder_reset   >;  /**< GPIO port mode register               */
  using OTYPER   = reg< uint32_t, base_addr + 0x04, rw                >;  /**< GPIO port output type register        */
  using OSPEEDR  = reg< uint32_t, base_addr + 0x08, rw, ospeedr_reset >;  /**< GPIO port output speed register       */
  using PUPDR    = reg< uint32_t, base_addr + 0x0c, rw, pupdr_reset   >;  /**< GPIO port pull-up/pull-down register  */
  using IDR      = reg< uint32_t, base_addr + 0x10, ro /*0x0000XXXX*/ >;  /**< GPIO port input data register         */
  using ODR      = reg< uint32_t, base_addr + 0x14, rw                >;  /**< GPIO port output data register        */
  using BSRR     = reg< uint32_t, base_addr + 0x18, wo                >;  /**< GPIO port bit set/reset register      */
  using LCKR     = reg< uint32_t, base_addr + 0x1c, rw                >;  /**< GPIO port configuration lock register */
  using AFRL     = reg< uint32_t, base_addr + 0x20, rw                >;  /**< GPIO alternate function low register  */
  using AFRH     = reg< uint32_t, base_addr + 0x24, rw                >;  /**< GPIO alternate function high register */

  template< unsigned pin_no > using MODERx   = regbits< MODER  , pin_no * 2      , 2 >;
  template< unsigned pin_no > using OTYPERx  = regbits< OTYPER , pin_no          , 1 >;
  template< unsigned pin_no > using OSPEEDRx = regbits< OSPEEDR, pin_no * 2      , 2 >;
  template< unsigned pin_no > using PUPDRx   = regbits< PUPDR  , pin_no * 2      , 2 >;
  template< unsigned pin_no > using IDRx     = regbits< IDR    , pin_no          , 1 >;
  template< unsigned pin_no > using ODRx     = regbits< ODR    , pin_no          , 1 >;
  template< unsigned pin_no > using AFRLx    = regbits< AFRL   , pin_no * 4      , 4 >; /** pin_no = [0..7]   */
  template< unsigned pin_no > using AFRHx    = regbits< AFRH   , (pin_no % 8) * 4, 4 >; /** pin_no = [8..15]  */

  /**
   * GPIO alternate function: returns AFRLx or AFRHx type dependent on pin_no.
   *
   * NOTE: this is not from the reference manual 
   */
  template< unsigned pin_no > using AFRx =
    typename std::conditional<
      (pin_no < 8),
      AFRLx<pin_no>,
      AFRHx<pin_no>
    >::type;
};

} // namespace mptl

#endif // ARCH_REG_GPIO_HPP_INCLUDED
