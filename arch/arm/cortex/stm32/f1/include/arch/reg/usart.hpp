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

#ifndef ARCH_REG_USART_HPP_INCLUDED
#define ARCH_REG_USART_HPP_INCLUDED

#include "../../../../common/reg/usart.hpp"

namespace reg
{
  template<unsigned usart_no>
  class USART
  {
    /* See available template specialisations below if the compiler asserts here! */
    static_assert(usart_no == !usart_no, "unsupported USART number");  // assertion needs to be dependent of template parameter
  };

  template<> class USART<1> : public USART_Common< 0x40013800 > { };
  template<> class USART<2> : public USART_Common< 0x40004400 > { };
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
  template<> class USART<3> : public USART_Common< 0x40004800 > { };
#endif
}

#endif // ARCH_REG_USART_HPP_INCLUDED
