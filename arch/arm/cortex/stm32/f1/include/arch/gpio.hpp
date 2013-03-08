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

// === GPIO Ports ===
//
// GPIO ports are configured as //input floating// upon reset and
// alternate functions are not enabled.
//
// Each GPIO port has two 32 bit configuration registers - GPIOx_CRL and
// GPIOx_CRH, two 32 bit data registers, GPIOx_IDR and GPIOx_ODR, a 32
// bit set-reset register, GPIO_xBSRR, a 16 bit reset register, GPIOx_BRR
// and a 32 bit locking register, GPIOx_LCKR.
//
// Each GPIO pin can be configured as one of:
//
//   *Input floating
//   *Input pull-up
//   *Input pull-down
//   *Analog input
//   *Output open-drain
//   *Output push-pull
//   *Alternate function push-pull
//   *Alternate function open-drain

// === Configuring the GPIO ports ===
//
// Each I/O pin of a GPIO port has four configuration bits - GPIOx_CRL
// holds configuration bits for 8 pins and GPIOx_CRH holds the
// configuration bits for the remaining 8 pins.
//
// The four configuration bits are divided into two MODE bits and two CNF
// bits - with MODE bits occupying lower bit positions. For example, in
// the case GPIO port B, bits 0 and 1 of GPIOB_CRL are the MODE bits and
// bits 2 and 3 are the CNF bits for pin 0. The MODE bits can have the
// following values:
//
//   *00 - Input mode (reset state)
//   *01 - Output mode (max 10MHz speed)
//   *10 - Output mode (max 2MHz speed)
//   *11 - Output mode (max 50MHz speed)
//
// The CNF bit values are interpreted differently according to the
// MODE. If MODE is input, the CNF bit values are interpreted as:

//   *00 - Analog input
//   *01 - Floating input
//   *10 - Input with pull-up/pull-down
//   *11 - reserved
//
// If MODE is output, the CNF bit values are interpreted as:
//
//   *00 - General purpose output push-pull
//   *01 - General purpose output open-drain
//   *10 - Alternate function output push-pull
//   *11 - Alternate function output open-drain
//
// Normally, to drive LED's, we will configure the output pins as
// push-pull.

// === push/pull and open-drain ===
//
// The push-pull output actually uses two transistors. Each will be on to
// drive the output to the appropriate level: the top transistor will be
// on when the output has to be driven high and the bottom transistor
// will turn on when the output has to go low. 
//
// The open-drain output lacks the top transistor. When the output has to
// go high you simply turn off the bottom transistor, but the line is now
// pulled high only by the pullup resistor. 
//
// Your micro allows you to select between the two types, which means
// that by setting some bits in some register you actually enable/
// disable the top transistor and enable/disable the pullup (if internal,
// otherwise you just disable the top transistor and have to use an
// external pullup) 
//
// The advantage of the push-pull output is the higher speed, because the
// line is driven both ways. With the pullup the line can only rise as
// fast as the RC time constant allows. The R is the pullup, the C is the
// parasitic capacitance, including the pin capacitance and the board
// capacitance. 
// The push-pull can typically source more current. With the open-drain
// the current is limited by the R and R cannot be made very small,
// because the lower transistor has to sink that current when the output
// is low; that means higher power consumption. 
//
// However, the open-drain allows you to cshort several outputs together,
// with a common pullup. This is called an wired-OR connection. Now you
// can drive the output low with any of the IO pins. To drive it high all
// ouputs have to be high. This is advantageous in some situations,
// because it eliminates the external gates that would otherwise be
// required.  


#include <resource.hpp>
#include <arch/rcc.hpp>
#include <freq.hpp>
#include <arch/reg/gpio.hpp>

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
  static_assert(port >= 'A', "Invalid GPIO port");
#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
  static_assert(port <= 'G', "Invalid GPIO port");
#else
  static_assert(port <= 'E', "Invalid GPIO port");
#endif
  static_assert((pin_no >= 0) && (pin_no < 16), "Invalid GPIO pin-no");

protected:

  static constexpr uint32_t pin_mask = (uint32_t)1 << pin_no;
  static constexpr uint32_t crx_shift = (pin_no % 8) << 2;
  static constexpr uint32_t crx_mask = 0x0F << crx_shift;

  typedef reg::GPIO<port> GPIOx;

#if 0
  // TODO: finish this. needs to go to something like "template<int port> GpioPort" in order to work correctly
  struct SharedBSRR : SharedRegister {
    template<typename result>
    static void shoot(void) {
      GPIOx::BSRR::set(result::value);
    }
  };
  struct SharedBRR : SharedRegister {
    template<typename result>
    static void shoot(void) {
      GPIOx::BRR::set(result::value);
    }
  };
#endif


public:

  typedef ResourceList< Rcc::gpio_clock_resources<port>
                        //!!!! TODO: fixme:  UniqueResource<GpioBase<port, pin_no> >
                        > resources;


  static void set() {
    GPIOx::BSRR::store(pin_mask);
  }
  static void reset() {
    GPIOx::BRR::store(pin_mask);
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
    if(cnf == cGpio::InputConfig::pull_up) {
      base::set(); // configure pull-up
//      base::set_mode(0, 2); //  10 - Input with pull-up/pull-down
    }
    else if(cnf == cGpio::InputConfig::pull_down) {
      base::reset(); // configure pull-down
//      base::set_mode(0, 2); //  10 - Input with pull-up/pull-down
    }
    else {
//      base::set_mode(0, cnf);
    }
  }


  static constexpr uint32_t crx_value = (((cnf == cGpio::InputConfig::analog)   ? 0 :
                                          (cnf == cGpio::InputConfig::floating) ? 1 :
                                          2) << 2) << base::crx_shift;

  static constexpr uint32_t crl_mask  = pin_no <  8 ? base::crx_mask : 0;
  static constexpr uint32_t crl_value = pin_no <  8 ? crx_value : 0;
  static constexpr uint32_t crh_mask  = pin_no >= 8 ? base::crx_mask : 0;
  static constexpr uint32_t crh_value = pin_no >= 8 ? crx_value : 0;

  typedef ResourceList< typename base::resources,
                        SharedRegister< typename reg::GPIO<port>::CRL, crl_value, crl_mask >,
                        SharedRegister< typename reg::GPIO<port>::CRH, crh_value, crh_mask >
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
                (speed == 10_mhz) || 
                (speed == 50_mhz),
                "Illegal frequency for GpioOutput speed (allowed: 2_mhz, 10_mhz, 50_mhz)");

  typedef GpioBase<port, pin_no> base;

public:
  static void init() {
//    base::set_mode(mode, cnf);
  }

  /* setup CRx register values */
  static constexpr uint32_t cnf_value = (((cnf == cGpio::OutputConfig::open_drain)     ? 1 :
                                          (cnf == cGpio::OutputConfig::alt_push_pull)  ? 2 :
                                          (cnf == cGpio::OutputConfig::alt_open_drain) ? 3 :
                                          0) << 2) << base::crx_shift;
  static constexpr uint32_t mode_value = ((speed == 10_mhz) ? 1 :
                                          (speed == 2_mhz)  ? 2 :
                                          3) << base::crx_shift;
  static constexpr uint32_t crx_value = mode_value | cnf_value;

  static constexpr uint32_t crl_mask  = pin_no <  8 ? base::crx_mask : 0;
  static constexpr uint32_t crl_value = pin_no <  8 ? crx_value : 0;
  static constexpr uint32_t crh_mask  = pin_no >= 8 ? base::crx_mask : 0;
  static constexpr uint32_t crh_value = pin_no >= 8 ? crx_value : 0;

  typedef ResourceList< typename base::resources,
                        SharedRegister< typename reg::GPIO<port>::CRL, crl_value, crl_mask >,
                        SharedRegister< typename reg::GPIO<port>::CRH, crh_value, crh_mask >
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


