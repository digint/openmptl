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

#ifndef REG_TIM_HPP_INCLUDED
#define REG_TIM_HPP_INCLUDED

#include "../../../../common/reg/tim.hpp"

namespace reg
{
  template<unsigned tim_no>
  class TIM
  {
    /* See available template specialisations below if the compiler asserts here! */
    static_assert(tim_no == !tim_no, "unsupported TIM number");  // assertion needs to be dependent of template parameter
  };

  template<> class TIM<  1 > : public __TIM_COMMON< 0x40012c00 > { };
  template<> class TIM<  2 > : public __TIM_COMMON< 0x40000000 > { };
  template<> class TIM<  3 > : public __TIM_COMMON< 0x40000400 > { };
  template<> class TIM<  4 > : public __TIM_COMMON< 0x40000800 > { };
  template<> class TIM<  5 > : public __TIM_COMMON< 0x40000c00 > { };
  template<> class TIM<  6 > : public __TIM_COMMON< 0x40001000 > { };
  template<> class TIM<  7 > : public __TIM_COMMON< 0x40001400 > { };
  template<> class TIM<  8 > : public __TIM_COMMON< 0x40013400 > { };
  template<> class TIM<  9 > : public __TIM_COMMON< 0x40014c00 > { };
  template<> class TIM< 10 > : public __TIM_COMMON< 0x40015000 > { };
  template<> class TIM< 11 > : public __TIM_COMMON< 0x40015400 > { };
  template<> class TIM< 12 > : public __TIM_COMMON< 0x40001800 > { };
  template<> class TIM< 13 > : public __TIM_COMMON< 0x40001c00 > { };
  template<> class TIM< 14 > : public __TIM_COMMON< 0x40002000 > { };
}

#endif // REG_TIM_HPP_INCLUDED
