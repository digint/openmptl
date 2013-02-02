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

#ifndef REG_USART_HPP_INCLUDED
#define REG_USART_HPP_INCLUDED

#include <register.hpp>
#include "../../../../common/reg/usart.hpp"

namespace reg
{
  template<std::size_t usart_no>
  class USART
  {
    static_assert(((usart_no >= 1) && (usart_no <= 3)) || (usart_no == 6), "unsupported USART number");
  };

  template<> class USART<1> : public __USART_STM32F4< 0x40011000 > { };
  template<> class USART<2> : public __USART_STM32F4< 0x40004400 > { };
  template<> class USART<3> : public __USART_STM32F4< 0x40004800 > { };
  template<> class USART<6> : public __USART_STM32F4< 0x40011400 > { };
}

#endif // REG_USART_HPP_INCLUDED
