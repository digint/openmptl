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

#ifndef REG_SPI_HPP_INCLUDED
#define REG_SPI_HPP_INCLUDED

#include "../../../../common/reg/spi.hpp"

namespace reg
{
  template<unsigned spi_no>
  class SPI
  {
    /* See available template specialisations below if the compiler asserts here! */
    static_assert(spi_no == !spi_no, "unsupported SPI number");  // assertion needs to be dependent of template parameter
  };

  template<> class SPI<1> : public SPI_Common< 0x40013000 > { };
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
  template<> class SPI<2> : public SPI_Common< 0x40003800 > { };
#endif
#if defined (STM32F10X_HD) || defined (STM32F10X_CL)
  template<> class SPI<3> : public SPI_Common< 0x40003C00 > { };
#endif
}

#endif // REG_SPI_HPP_INCLUDED
