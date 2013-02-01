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

#ifndef CORE_RESOURCE_HPP_INCLUDED
#define CORE_RESOURCE_HPP_INCLUDED

#include "resource_mpl.hpp"

template<typename... Args>
void ResourceList<Args...>::set_shared_register(void)
{
  using namespace reg;

  /* Enable peripheral clocks */
  set_register<RCC::APB1ENR>();
  set_register<RCC::APB2ENR>();
  set_register<RCC::AHB1ENR>();

  set_register<GPIO<'A'>::MODER>();
  set_register<GPIO<'A'>::OTYPER>();
  set_register<GPIO<'A'>::OSPEEDR>();
  set_register<GPIO<'A'>::PUPDR>();

  set_register<GPIO<'B'>::MODER>();
  set_register<GPIO<'B'>::OTYPER>();
  set_register<GPIO<'B'>::OSPEEDR>();
  set_register<GPIO<'B'>::PUPDR>();

  set_register<GPIO<'C'>::MODER>();
  set_register<GPIO<'C'>::OTYPER>();
  set_register<GPIO<'C'>::OSPEEDR>();
  set_register<GPIO<'C'>::PUPDR>();

  set_register<GPIO<'D'>::MODER>();
  set_register<GPIO<'D'>::OTYPER>();
  set_register<GPIO<'D'>::OSPEEDR>();
  set_register<GPIO<'D'>::PUPDR>();

  set_register<GPIO<'E'>::MODER>();
  set_register<GPIO<'E'>::OTYPER>();
  set_register<GPIO<'E'>::OSPEEDR>();
  set_register<GPIO<'E'>::PUPDR>();

  set_register<GPIO<'F'>::MODER>();
  set_register<GPIO<'F'>::OTYPER>();
  set_register<GPIO<'F'>::OSPEEDR>();
  set_register<GPIO<'F'>::PUPDR>();

  set_register<GPIO<'G'>::MODER>();
  set_register<GPIO<'G'>::OTYPER>();
  set_register<GPIO<'G'>::OSPEEDR>();
  set_register<GPIO<'G'>::PUPDR>();

  set_register<GPIO<'H'>::MODER>();
  set_register<GPIO<'H'>::OTYPER>();
  set_register<GPIO<'H'>::OSPEEDR>();
  set_register<GPIO<'H'>::PUPDR>();

  set_register<GPIO<'I'>::MODER>();
  set_register<GPIO<'I'>::OTYPER>();
  set_register<GPIO<'I'>::OSPEEDR>();
  set_register<GPIO<'I'>::PUPDR>();
}

#if 0
#ifndef CORE_SIMULATION
// recursively assert count<=1 for UniqueGPIO
template<typename resources, char port, int pin_no>
struct UniqueGPIO_Assert {
  static_assert(resources::template value<UniqueGPIO<port, pin_no> >() <= 1, "UniqueGPIO is not unique");
  UniqueGPIO_Assert<resources, port, pin_no + 1> recursion;
};
template<typename resources, char port>
struct UniqueGPIO_Assert<resources, port, 15> {
  static_assert(resources::template value<UniqueGPIO<port, 0> >() <= 1, "UniqueGPIO is not unique");
  UniqueGPIO_Assert<resources, port + 1, 0> recursion;
};
template<typename resources>
struct UniqueGPIO_Assert<resources, 'G', 15> {
  static_assert(resources::template value<UniqueGPIO<'G', 15> >() <= 1, "UniqueGPIO is not unique");
};

template<typename T, typename... Args>
void ResourceList<T, Args...>::assert(void)
{
  UniqueGPIO_Assert<ResourceList<T, Args...>, 'A', 0> assert;
}
#endif
#endif


#endif // CORE_RESOURCE_HPP_INCLUDED
