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

#ifdef CORE_SIMULATION

#include <register.hpp>
#include <arch/reg/spi.hpp>

namespace reg {

// example reaction. triggered as soon as SPI1_CR1 is touched.
template<>
void RegisterReaction<reg::SPI<1>::CR1::value_type, reg::SPI<1>::CR1::addr>::react(reg::SPI<1>::CR1::value_type const value) {

  std::cout << __PRETTY_FUNCTION__ << std::endl;  // NOTE: this is gcc specific

  //  assert(value != 0x0040, "gna");
  std::cout << "XXXXXXXXXXXXXXXXXXXXX value= " << std::hex << reg::SPI<1>::CR1::SPE::value << std::endl;
  std::cout << "XXXXXXXXXXXXXXXXXXXXX test= " << std::hex << reg::SPI<1>::CR1::SPE::test() << std::endl;
  std::cout << "XXXXXXXXXXXXXXXXXXXXX shift= " << std::hex << reg::SPI<1>::CR1::SPE::test_and_shift() << std::endl;

  std::cout << "XXXXXXXXXXXXXXXXXXXXX value= " << std::hex << reg::SPI<1>::CR1::BR::value << std::endl;
  std::cout << "XXXXXXXXXXXXXXXXXXXXX test= " << std::hex << reg::SPI<1>::CR1::BR::test() << std::endl;
  std::cout << "XXXXXXXXXXXXXXXXXXXXX shift= " << std::hex << reg::SPI<1>::CR1::BR::test_and_shift() << std::endl;
  std::cout << "XXXXXXXXXXXXXXXXXXXXX bit0= " << std::hex << reg::SPI<1>::CR1::BR::bit<0>::value << std::endl;
  std::cout << "XXXXXXXXXXXXXXXXXXXXX bit1= " << std::hex << reg::SPI<1>::CR1::BR::bit<1>::value << std::endl;
  std::cout << "XXXXXXXXXXXXXXXXXXXXX bit2= " << std::hex << reg::SPI<1>::CR1::BR::bit<2>::value << std::endl;

  std::cout << "CAST value= " << std::hex << reg::SPI<1>::CR1::SPE() << std::endl;

  std::cout << "prescaler<>= " << std::hex << reg::SPI<1>::CR1::BR::Prescaler<256>::value << std::endl;

  reg::SPI<1>::CR1::BR::Prescaler<256>::set();

  //  std::cout << "XXXXXXXXXXXXXXXXXXXXX bit2= " << std::hex << reg::SPI<1>::CR1::BR::bit<3>::value << std::endl;
#if 1
  if(value & reg::SPI<1>::CR1::SPE::value) {
    std::cout << "RegisterReaction: SPIx::CR1::SPE::value -> set(TXE | RXNE)" << std::endl;
    reg::SPI<1>::SR::set(reg::SPI<1>::SR::TXE::value |
                          reg::SPI<1>::SR::RXNE::value);
  }
#endif
}

} // namespace reg

#endif // CORE_SIMULATION

