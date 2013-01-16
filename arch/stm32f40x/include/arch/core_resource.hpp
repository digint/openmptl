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

template<char port, uint32_t val> struct SharedGpioMODER        : SharedResourceOR, SharedResource<SharedGpioMODER       <port, 0>, val> { };
template<char port, uint32_t val> struct SharedGpioMODER_Mask   : SharedResourceOR, SharedResource<SharedGpioMODER_Mask  <port, 0>, val> { };
template<char port, uint32_t val> struct SharedGpioOTYPER       : SharedResourceOR, SharedResource<SharedGpioOTYPER      <port, 0>, val> { };
template<char port, uint32_t val> struct SharedGpioOTYPER_Mask  : SharedResourceOR, SharedResource<SharedGpioOTYPER_Mask <port, 0>, val> { };
template<char port, uint32_t val> struct SharedGpioOSPEEDR      : SharedResourceOR, SharedResource<SharedGpioOSPEEDR     <port, 0>, val> { };
template<char port, uint32_t val> struct SharedGpioOSPEEDR_Mask : SharedResourceOR, SharedResource<SharedGpioOSPEEDR_Mask<port, 0>, val> { };
template<char port, uint32_t val> struct SharedGpioPUPDR        : SharedResourceOR, SharedResource<SharedGpioPUPDR       <port, 0>, val> { };
template<char port, uint32_t val> struct SharedGpioPUPDR_Mask   : SharedResourceOR, SharedResource<SharedGpioPUPDR_Mask  <port, 0>, val> { };

template<char port, int pin_no>   struct UniqueGPIO     : CountedResource,  SharedResource<UniqueGPIO<port, pin_no>, 1>  { };


template<typename T, typename... Args>
void ResourceList<T, Args...>::configure(void)
{
  /* Enable peripheral clocks */
  if(value< SharedAPB1ENR<0> >()) Reg::RCC::APB1ENR::store( Reg::RCC::APB1ENR::reset_value | value< SharedAPB1ENR<0> >() );
  if(value< SharedAPB2ENR<0> >()) Reg::RCC::APB2ENR::store( Reg::RCC::APB2ENR::reset_value | value< SharedAPB2ENR<0> >() );
  if(value< SharedAHB1ENR<0> >()) Reg::RCC::AHB1ENR::store( Reg::RCC::AHB1ENR::reset_value | value< SharedAHB1ENR<0> >() );

  /* Setup GPIO register */
  if(value< SharedGpioMODER_Mask  <'A', 0> >()) Reg::GPIO<'A'>::MODER::set(   value< SharedGpioMODER  <'A', 0> >(), value< SharedGpioMODER_Mask  <'A', 0> >() );
  if(value< SharedGpioOTYPER_Mask <'A', 0> >()) Reg::GPIO<'A'>::OTYPER::set(  value< SharedGpioOTYPER <'A', 0> >(), value< SharedGpioOTYPER_Mask <'A', 0> >() );
  if(value< SharedGpioOSPEEDR_Mask<'A', 0> >()) Reg::GPIO<'A'>::OSPEEDR::set( value< SharedGpioOSPEEDR<'A', 0> >(), value< SharedGpioOSPEEDR_Mask<'A', 0> >() );
  if(value< SharedGpioPUPDR_Mask  <'A', 0> >()) Reg::GPIO<'A'>::PUPDR::set(   value< SharedGpioPUPDR  <'A', 0> >(), value< SharedGpioPUPDR_Mask  <'A', 0> >() );

  if(value< SharedGpioMODER_Mask  <'B', 0> >()) Reg::GPIO<'B'>::MODER::set(   value< SharedGpioMODER  <'B', 0> >(), value< SharedGpioMODER_Mask  <'B', 0> >() );
  if(value< SharedGpioOTYPER_Mask <'B', 0> >()) Reg::GPIO<'B'>::OTYPER::set(  value< SharedGpioOTYPER <'B', 0> >(), value< SharedGpioOTYPER_Mask <'B', 0> >() );
  if(value< SharedGpioOSPEEDR_Mask<'B', 0> >()) Reg::GPIO<'B'>::OSPEEDR::set( value< SharedGpioOSPEEDR<'B', 0> >(), value< SharedGpioOSPEEDR_Mask<'B', 0> >() );
  if(value< SharedGpioPUPDR_Mask  <'B', 0> >()) Reg::GPIO<'B'>::PUPDR::set(   value< SharedGpioPUPDR  <'B', 0> >(), value< SharedGpioPUPDR_Mask  <'B', 0> >() );

  if(value< SharedGpioMODER_Mask  <'C', 0> >()) Reg::GPIO<'C'>::MODER::set(   value< SharedGpioMODER  <'C', 0> >(), value< SharedGpioMODER_Mask  <'C', 0> >() );
  if(value< SharedGpioOTYPER_Mask <'C', 0> >()) Reg::GPIO<'C'>::OTYPER::set(  value< SharedGpioOTYPER <'C', 0> >(), value< SharedGpioOTYPER_Mask <'C', 0> >() );
  if(value< SharedGpioOSPEEDR_Mask<'C', 0> >()) Reg::GPIO<'C'>::OSPEEDR::set( value< SharedGpioOSPEEDR<'C', 0> >(), value< SharedGpioOSPEEDR_Mask<'C', 0> >() );
  if(value< SharedGpioPUPDR_Mask  <'C', 0> >()) Reg::GPIO<'C'>::PUPDR::set(   value< SharedGpioPUPDR  <'C', 0> >(), value< SharedGpioPUPDR_Mask  <'C', 0> >() );

  if(value< SharedGpioMODER_Mask  <'D', 0> >()) Reg::GPIO<'D'>::MODER::set(   value< SharedGpioMODER  <'D', 0> >(), value< SharedGpioMODER_Mask  <'D', 0> >() );
  if(value< SharedGpioOTYPER_Mask <'D', 0> >()) Reg::GPIO<'D'>::OTYPER::set(  value< SharedGpioOTYPER <'D', 0> >(), value< SharedGpioOTYPER_Mask <'D', 0> >() );
  if(value< SharedGpioOSPEEDR_Mask<'D', 0> >()) Reg::GPIO<'D'>::OSPEEDR::set( value< SharedGpioOSPEEDR<'D', 0> >(), value< SharedGpioOSPEEDR_Mask<'D', 0> >() );
  if(value< SharedGpioPUPDR_Mask  <'D', 0> >()) Reg::GPIO<'D'>::PUPDR::set(   value< SharedGpioPUPDR  <'D', 0> >(), value< SharedGpioPUPDR_Mask  <'D', 0> >() );

  if(value< SharedGpioMODER_Mask  <'E', 0> >()) Reg::GPIO<'E'>::MODER::set(   value< SharedGpioMODER  <'E', 0> >(), value< SharedGpioMODER_Mask  <'E', 0> >() );
  if(value< SharedGpioOTYPER_Mask <'E', 0> >()) Reg::GPIO<'E'>::OTYPER::set(  value< SharedGpioOTYPER <'E', 0> >(), value< SharedGpioOTYPER_Mask <'E', 0> >() );
  if(value< SharedGpioOSPEEDR_Mask<'E', 0> >()) Reg::GPIO<'E'>::OSPEEDR::set( value< SharedGpioOSPEEDR<'E', 0> >(), value< SharedGpioOSPEEDR_Mask<'E', 0> >() );
  if(value< SharedGpioPUPDR_Mask  <'E', 0> >()) Reg::GPIO<'E'>::PUPDR::set(   value< SharedGpioPUPDR  <'E', 0> >(), value< SharedGpioPUPDR_Mask  <'E', 0> >() );

  if(value< SharedGpioMODER_Mask  <'F', 0> >()) Reg::GPIO<'F'>::MODER::set(   value< SharedGpioMODER  <'F', 0> >(), value< SharedGpioMODER_Mask  <'F', 0> >() );
  if(value< SharedGpioOTYPER_Mask <'F', 0> >()) Reg::GPIO<'F'>::OTYPER::set(  value< SharedGpioOTYPER <'F', 0> >(), value< SharedGpioOTYPER_Mask <'F', 0> >() );
  if(value< SharedGpioOSPEEDR_Mask<'F', 0> >()) Reg::GPIO<'F'>::OSPEEDR::set( value< SharedGpioOSPEEDR<'F', 0> >(), value< SharedGpioOSPEEDR_Mask<'F', 0> >() );
  if(value< SharedGpioPUPDR_Mask  <'F', 0> >()) Reg::GPIO<'F'>::PUPDR::set(   value< SharedGpioPUPDR  <'F', 0> >(), value< SharedGpioPUPDR_Mask  <'F', 0> >() );

  if(value< SharedGpioMODER_Mask  <'G', 0> >()) Reg::GPIO<'G'>::MODER::set(   value< SharedGpioMODER  <'G', 0> >(), value< SharedGpioMODER_Mask  <'G', 0> >() );
  if(value< SharedGpioOTYPER_Mask <'G', 0> >()) Reg::GPIO<'G'>::OTYPER::set(  value< SharedGpioOTYPER <'G', 0> >(), value< SharedGpioOTYPER_Mask <'G', 0> >() );
  if(value< SharedGpioOSPEEDR_Mask<'G', 0> >()) Reg::GPIO<'G'>::OSPEEDR::set( value< SharedGpioOSPEEDR<'G', 0> >(), value< SharedGpioOSPEEDR_Mask<'G', 0> >() );
  if(value< SharedGpioPUPDR_Mask  <'G', 0> >()) Reg::GPIO<'G'>::PUPDR::set(   value< SharedGpioPUPDR  <'G', 0> >(), value< SharedGpioPUPDR_Mask  <'G', 0> >() );

  if(value< SharedGpioMODER_Mask  <'H', 0> >()) Reg::GPIO<'H'>::MODER::set(   value< SharedGpioMODER  <'H', 0> >(), value< SharedGpioMODER_Mask  <'H', 0> >() );
  if(value< SharedGpioOTYPER_Mask <'H', 0> >()) Reg::GPIO<'H'>::OTYPER::set(  value< SharedGpioOTYPER <'H', 0> >(), value< SharedGpioOTYPER_Mask <'H', 0> >() );
  if(value< SharedGpioOSPEEDR_Mask<'H', 0> >()) Reg::GPIO<'H'>::OSPEEDR::set( value< SharedGpioOSPEEDR<'H', 0> >(), value< SharedGpioOSPEEDR_Mask<'H', 0> >() );
  if(value< SharedGpioPUPDR_Mask  <'H', 0> >()) Reg::GPIO<'H'>::PUPDR::set(   value< SharedGpioPUPDR  <'H', 0> >(), value< SharedGpioPUPDR_Mask  <'H', 0> >() );

  if(value< SharedGpioMODER_Mask  <'I', 0> >()) Reg::GPIO<'I'>::MODER::set(   value< SharedGpioMODER  <'I', 0> >(), value< SharedGpioMODER_Mask  <'I', 0> >() );
  if(value< SharedGpioOTYPER_Mask <'I', 0> >()) Reg::GPIO<'I'>::OTYPER::set(  value< SharedGpioOTYPER <'I', 0> >(), value< SharedGpioOTYPER_Mask <'I', 0> >() );
  if(value< SharedGpioOSPEEDR_Mask<'I', 0> >()) Reg::GPIO<'I'>::OSPEEDR::set( value< SharedGpioOSPEEDR<'I', 0> >(), value< SharedGpioOSPEEDR_Mask<'I', 0> >() );
  if(value< SharedGpioPUPDR_Mask  <'I', 0> >()) Reg::GPIO<'I'>::PUPDR::set(   value< SharedGpioPUPDR  <'I', 0> >(), value< SharedGpioPUPDR_Mask  <'I', 0> >() );
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
