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

#ifndef RESOURCE_HPP_INCLUDED
#define RESOURCE_HPP_INCLUDED

#include "resource_mpl.hpp"
#include "core.hpp"
#include "arch/rcc.hpp"

// NOTE: SharedXXX<0> is used as a tag for the values in SharedXXX
template<uint32_t val> struct SharedAPB1ENR : SharedResourceOR, SharedResource<SharedAPB1ENR<0>, val> { };
template<uint32_t val> struct SharedAPB2ENR : SharedResourceOR, SharedResource<SharedAPB2ENR<0>, val> { };
template<uint32_t val> struct SharedAHB1ENR : SharedResourceOR, SharedResource<SharedAHB1ENR<0>, val> { };

template<char port, uint32_t val> struct SharedGpioMODER      : SharedResourceOR, SharedResource<SharedGpioMODER<port, 0>, val>      { };
template<char port, uint32_t val> struct SharedGpioMODER_Mask : SharedResourceOR, SharedResource<SharedGpioMODER_Mask<port, 0>, val> { };
template<char port, uint32_t val> struct SharedGpioOTYPER      : SharedResourceOR, SharedResource<SharedGpioOTYPER<port, 0>, val>      { };
template<char port, uint32_t val> struct SharedGpioOTYPER_Mask : SharedResourceOR, SharedResource<SharedGpioOTYPER_Mask<port, 0>, val> { };
template<char port, uint32_t val> struct SharedGpioOSPEEDR      : SharedResourceOR, SharedResource<SharedGpioOSPEEDR<port, 0>, val>      { };
template<char port, uint32_t val> struct SharedGpioOSPEEDR_Mask : SharedResourceOR, SharedResource<SharedGpioOSPEEDR_Mask<port, 0>, val> { };
template<char port, uint32_t val> struct SharedGpioPUPDR      : SharedResourceOR, SharedResource<SharedGpioPUPDR<port, 0>, val>      { };
template<char port, uint32_t val> struct SharedGpioPUPDR_Mask : SharedResourceOR, SharedResource<SharedGpioPUPDR_Mask<port, 0>, val> { };

template<char port, int pin_no>   struct UniqueGPIO     : CountedResource,  SharedResource<UniqueGPIO<port, pin_no>, 1>  { };



template<typename T, typename... Args>
void ResourceList<T, Args...>::configure(void)
{
  Core::RCC::APB1ENR::store( value< SharedAPB1ENR<0> >() );
  Core::RCC::APB2ENR::store( value< SharedAPB2ENR<0> >() );
  Core::RCC::AHB1ENR::store( value< SharedAHB1ENR<0> >() );

  if(value< SharedGpioMODER_Mask<'D', 0> >()) Core::GPIO<'D'>::MODER::store((Core::GPIO<'D'>::MODER::load() & ~value< SharedGpioMODER_Mask<'D', 0> >()) | value< SharedGpioMODER<'D', 0> >());
  if(value< SharedGpioOTYPER_Mask<'D', 0> >()) Core::GPIO<'D'>::OTYPER::store((Core::GPIO<'D'>::OTYPER::load() & ~value< SharedGpioOTYPER_Mask<'D', 0> >()) | value< SharedGpioOTYPER<'D', 0> >());
  if(value< SharedGpioOSPEEDR_Mask<'D', 0> >()) Core::GPIO<'D'>::OSPEEDR::store((Core::GPIO<'D'>::OSPEEDR::load() & ~value< SharedGpioOSPEEDR_Mask<'D', 0> >()) | value< SharedGpioOSPEEDR<'D', 0> >());
  if(value< SharedGpioPUPDR_Mask<'D', 0> >()) Core::GPIO<'D'>::PUPDR::store((Core::GPIO<'D'>::PUPDR::load() & ~value< SharedGpioPUPDR_Mask<'D', 0> >()) | value< SharedGpioPUPDR<'D', 0> >());
}


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



#endif // RESOURCE_HPP_INCLUDED
