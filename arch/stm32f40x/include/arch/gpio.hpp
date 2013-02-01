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

#ifndef GPIO_HPP_INCLUDED
#define GPIO_HPP_INCLUDED


#include <arch/core.hpp>
#include <arch/core_resource.hpp>

namespace cGpio
{
  enum class InputConfig {
    analog,         //< Analog input                
    floating,       //< Floating input              
    pull_up,        //< Input with pull-up
    pull_down       //< Input with pull-down
  };

  enum class OutputConfig {
    push_pull,      //< General purpose output push-pull (e.g. LED's)
    open_drain,     //< General purpose output open-drain            
    alt_push_pull,  //< Alternate function output push-pull          
    alt_open_drain  //< Alternate function output open-drain         
  };

  enum class ActiveState {
    low,            //< pin is low-active
    high            //< pin is high-active
  };
}


////////////////////  GpioBase  ////////////////////


template<char port, int pin_no>
class GpioBase
{
  static_assert((port >= 'A') && (port <= 'I'), "Invalid GPIO port");
  static_assert((pin_no >= 0) && (pin_no < 16), "Invalid GPIO pin-no");

protected:

  static constexpr uint32_t pin_mask = (uint32_t)1 << pin_no;
  static constexpr uint32_t pin_mask_double  = (uint32_t)3 << (pin_no * 2);

  typedef reg::GPIO<port> GPIOx;


public:

  static constexpr uint32_t ahb1enr = ( port == 'A' ? reg::RCC::AHB1ENR::GPIOAEN::value : 
                                        port == 'B' ? reg::RCC::AHB1ENR::GPIOBEN::value : 
                                        port == 'C' ? reg::RCC::AHB1ENR::GPIOCEN::value : 
                                        port == 'D' ? reg::RCC::AHB1ENR::GPIODEN::value : 
                                        port == 'E' ? reg::RCC::AHB1ENR::GPIOEEN::value : 
                                        port == 'F' ? reg::RCC::AHB1ENR::GPIOFEN::value : 
                                        port == 'G' ? reg::RCC::AHB1ENR::GPIOGEN::value : 
                                        port == 'H' ? reg::RCC::AHB1ENR::GPIOHEN::value : 
                                        port == 'I' ? reg::RCC::AHB1ENR::GPIOIEN::value : 
                                        0 );

  typedef ResourceList< SharedRegister< reg::RCC::AHB1ENR, ahb1enr >,
                        UniqueResource< GpioBase<port, pin_no> >
                        > resources;


  static void set() {
    GPIOx::BSRR::store(pin_mask);
  }
  static void reset() {
    GPIOx::BSRR::store(pin_mask << 16);
  }

  static uint32_t read_input_bit() {
    return GPIOx::IDR::test(pin_mask);
  }

  static uint32_t read_output_bit() {
    return GPIOx::ODR::test(pin_mask);
  }
};


////////////////////  GpioInput  ////////////////////


template<char port,
         int pin_no,
         cGpio::InputConfig cnf,
         cGpio::ActiveState active_state = cGpio::ActiveState::low>
class GpioInput : public GpioBase<port, pin_no>
{

  typedef GpioBase<port, pin_no> base;

public:
  static void init() {
  }

  // TODO: alternate function mode (new class for this?)
  static constexpr uint32_t moder_value = (cnf == cGpio::InputConfig::analog ? 0x11 : 0x00) << (base::pin_no * 2);
  static constexpr uint32_t pupdr_value = (cnf == cGpio::InputConfig::pull_up   ? 0x01 :
                                           cnf == cGpio::InputConfig::pull_down ? 0x10 :
                                           0x00) << (base::pin_no * 2) ;

  typedef ResourceList< typename base::resources,
                        SharedRegister< typename reg::GPIO<port>::MODER,   moder_value, base::pin_mask_double >,
                        SharedRegister< typename reg::GPIO<port>::OTYPER,  0,           base::pin_mask        >,
                        SharedRegister< typename reg::GPIO<port>::OSPEEDR, 0,           base::pin_mask_double >,
                        SharedRegister< typename reg::GPIO<port>::PUPDR,   pupdr_value, base::pin_mask_double >
                        > resources;

  static bool active(void) {
    bool input = base::read_input_bit();
    return active_state == cGpio::ActiveState::low ? !input : input;
  }
};


////////////////////  GpioOutput  ////////////////////


template<char port,
         int pin_no,
         cGpio::OutputConfig cnf,
         freq_t speed = 50_mhz,
         cGpio::ActiveState active_state = cGpio::ActiveState::low>
class GpioOutput : public GpioBase<port, pin_no> {

  static_assert((speed == 2_mhz) || 
                (speed == 25_mhz) || 
                (speed == 50_mhz) || 
                (speed == 100_mhz),
                "Illegal frequency for GpioOutput speed (allowed: 2_mhz, 25_mhz, 50_mhz, 100_mhz)");

  typedef GpioBase<port, pin_no> base;

public:
  static void init() {
  }

  static constexpr uint32_t moder_value = (uint32_t)1 << (pin_no * 2); /* output mode */
  static constexpr uint32_t otyper_value = ((cnf == cGpio::OutputConfig::open_drain) ||
                                            (cnf == cGpio::OutputConfig::alt_open_drain)
                                            ) ? base::pin_mask : 0;
  static constexpr uint32_t ospeedr_value = (speed == 25_mhz  ? 1 :
                                             speed == 50_mhz  ? 2 :
                                             speed == 100_mhz ? 3 :
                                             0) << (pin_no * 2) ;


  typedef ResourceList< typename base::resources,
                        SharedRegister< typename reg::GPIO<port>::MODER,   moder_value,   base::pin_mask_double >,
                        SharedRegister< typename reg::GPIO<port>::OTYPER,  otyper_value,  base::pin_mask        >,
                        SharedRegister< typename reg::GPIO<port>::OSPEEDR, ospeedr_value, base::pin_mask_double >,
                        SharedRegister< typename reg::GPIO<port>::PUPDR,   0,             base::pin_mask_double >
                        > resources;

  static void enable() {
    if(active_state == cGpio::ActiveState::low) {
      base::reset();
    } else {
      base::set();
    }
  }

  static void disable() {
    if(active_state == cGpio::ActiveState::low) {
      base::set();
    } else {
      base::reset();
    }
  }

  static bool active() {
    bool input = base::read_input_bit();
    return active_state == cGpio::ActiveState::low ? !input : input;
  }

  static void toggle() {
    if(base::read_input_bit()) {
      base::reset();
    }
    else {
      base::set();
    }
  }

  static bool latched() {
    bool output = base::read_output_bit();
    return active_state == cGpio::ActiveState::low ? !output : output;
  }
};


////////////////////  GpioLed  ////////////////////


template<char port,
         int pin_no,
         cGpio::OutputConfig cnf = cGpio::OutputConfig::push_pull,
         freq_t speed = 50_mhz,
         cGpio::ActiveState active_state = cGpio::ActiveState::low>
class GpioLed : public GpioOutput<port, pin_no, cnf, speed, active_state> {

  typedef GpioOutput<port, pin_no, cnf, speed, active_state> base;

public:
  static void on() {
    base::enable();
  }
  static void off() {
    base::disable();
  }
};

#endif // GPIO_HPP_INCLUDED


