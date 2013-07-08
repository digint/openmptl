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

#ifndef ARCH_REG_SPI_HPP_INCLUDED
#define ARCH_REG_SPI_HPP_INCLUDED

#include "../../../../common/reg/spi.hpp"

namespace mptl { namespace reg {

template<unsigned spi_no>
class SPI
{
  /* See available template specialisations below if the compiler asserts here! */
  static_assert(spi_no == !spi_no, "unsupported SPI number");  // assertion needs to be dependent of template parameter
};

/* NOTE: available SPI depends on cpu sub-arch */
template<> class SPI<1> : public SPI_Common_Ext< 0x40013000 > { };
template<> class SPI<2> : public SPI_Common_Ext< 0x40003800 > { };
template<> class SPI<3> : public SPI_Common_Ext< 0x40003C00 > { };
template<> class SPI<4> : public SPI_Common_Ext< 0x40013400 > { };
template<> class SPI<5> : public SPI_Common_Ext< 0x40015000 > { };
template<> class SPI<6> : public SPI_Common_Ext< 0x40015400 > { };

} } // namespace mptl::reg

#endif // ARCH_REG_SPI_HPP_INCLUDED
