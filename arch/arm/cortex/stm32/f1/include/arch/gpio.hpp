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

#ifndef ARCH_GPIO_HPP_INCLUDED
#define ARCH_GPIO_HPP_INCLUDED

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

#include <type_traits>
#include <typelist.hpp>
#include <arch/rcc.hpp>
#include <freq.hpp>
#include <arch/reg/gpio.hpp>
#include <gpio_base.hpp>

namespace mptl {


////////////////////  gpio  ////////////////////


template< char port, int pin_no >
class gpio
{
  static_assert(port >= 'A', "invalid GPIO port");
#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
  static_assert(port <= 'G', "invalid GPIO port");
#else
  static_assert(port <= 'E', "invalid GPIO port");
#endif
  static_assert((pin_no >= 0) && (pin_no < 16), "invalid GPIO pin-no");

public:

  using GPIOx = GPIO<port>;

  using resources = rcc_gpio_clock_resources<port>;

protected:

  static constexpr uint32_t pin_mask = (uint32_t)1 << pin_no;

public:  /* ------ configuration resources ------ */

  struct mode
  {
    /** Input mode */
    using input  = regval< typename GPIOx::template CRx<pin_no>::MODE, 0 >;

    /** Output mode */
    template<freq_t speed>
    using output = typename std::enable_if<
      speed == mhz(2) || speed == mhz(10) || speed == mhz(50),
      regval< typename GPIOx::template CRx<pin_no>::MODE,
        speed == mhz(10) ? 1 :
        speed == mhz(2)  ? 2 :
        speed == mhz(50) ? 3 : 0xff
        >
      >::type;
  };

  struct input_type
  {
    /** Analog input */
    using analog       = regval< typename GPIOx::template CRx<pin_no>::CNF, 0 >;

    /** Floating input */
    using floating     = regval< typename GPIOx::template CRx<pin_no>::CNF, 1 >;

    /** Input with pull-up / pull-down */
    using pull_up_down = regval< typename GPIOx::template CRx<pin_no>::CNF, 2 >;
  };

  struct output_type
  {
    /** General purpose output push-pull (e.g. LED's) */
    using push_pull     = regval< typename GPIOx::template CRx<pin_no>::CNF, 0 >;

    /** General purpose output open-drain */
    using open_drain    = regval< typename GPIOx::template CRx<pin_no>::CNF, 1 >;

    /** Alternate function output push-pull */
    using af_push_pull  = regval< typename GPIOx::template CRx<pin_no>::CNF, 2 >;

    /** Alternate function output open-drain */
    using af_open_drain = regval< typename GPIOx::template CRx<pin_no>::CNF, 3 >;
  };

public:  /* ------ static member functions ------ */

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


////////////////////  gpio_input  ////////////////////


template<char port, unsigned pin_no, gpio_active_state active_state = gpio_active_state::high >
class gpio_input
: public gpio_input_base< gpio< port, pin_no >, active_state >
{
  using gpio_type = gpio< port, pin_no >;
public:
  using resources = typelist<
    typename gpio_type::resources,
    typename gpio_type::mode::input
    >;
};


////////////////////  gpio_output  ////////////////////


template<
  char port,
  unsigned pin_no,
  gpio_active_state active_state = gpio_active_state::high,
  freq_t speed = mhz(50)
  >
class gpio_output
: public gpio_output_base< gpio< port, pin_no >, active_state >
{
  using gpio_type = gpio< port, pin_no >;
public:
  using resources = typelist<
    typename gpio_type::resources,
    typename gpio_type::mode::template output<speed>
    >;
};


////////////////////  gpio_led  ////////////////////


template<
  char port,
  unsigned pin_no,
  gpio_active_state active_state = gpio_active_state::high,
  freq_t speed = mhz(2)
  >
class gpio_led
: public gpio_led_base< gpio_output< port, pin_no, active_state, speed > >
{
  using gpio_type = gpio_output< port, pin_no, active_state, speed >;
public:
  using resources = typelist<
    typename gpio_type::resources,
    typename gpio_type::output_type::push_pull
    >;
};

} // namespace mptl

#endif // ARCH_GPIO_HPP_INCLUDED
