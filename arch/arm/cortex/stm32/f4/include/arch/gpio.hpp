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

#ifndef GPIO_HPP_INCLUDED
#define GPIO_HPP_INCLUDED

#include <arch/rcc.hpp>
#include <arch/reg/gpio.hpp>
#include <gpio_base.hpp>
#include <type_traits>

namespace mptl {


////////////////////  gpio  ////////////////////


template< char port, unsigned pin_no >
class gpio
{
  static_assert((port >= 'A') && (port <= 'I'), "invalid GPIO port");
  static_assert(pin_no < 16, "invalid GPIO pin-no");

public:

  using GPIOx = GPIO<port>;

  using resources = typelist<
    rcc_gpio_clock_resources<port>
    // TODO: unique type
    //    resource::unique< gpio<port, pin_no> >,
    >;

protected:

  static constexpr uint32_t pin_mask = (uint32_t)0x1 << pin_no;

private:

  template<freq_t value>
  struct speed_impl
  {
    static_assert((value == mhz(2))  ||
                  (value == mhz(25)) ||
                  (value == mhz(50)) ||
                  (value == mhz(100)),
                  "Illegal frequency for gpio output speed (allowed: mhz(2), mhz(25), mhz(50), mhz(100))");

    using type = regval<
      typename GPIOx::template OSPEEDRx<pin_no>,
      (value == mhz(25)  ? 1 :
       value == mhz(50)  ? 2 :
       value == mhz(100) ? 3 :
       0)
      >;
  };

  template<unsigned value>
  struct alt_func_num_impl
  {
    static_assert(value < 16, "illegal alternate function number");

    using type = typename std::conditional<
      pin_no < 8,
        regval< typename GPIOx::template AFRLx<pin_no>, value >,
        regval< typename GPIOx::template AFRHx<pin_no>, value >
        >::type;
  };


public:  /* ------ configuration resources ------ */

  struct mode
  {
    using input  = regval< typename GPIOx::template MODERx<pin_no>, 0 >;
    using output = regval< typename GPIOx::template MODERx<pin_no>, 1 >;

    /**
     * Alternate function mode selection
     *
     * NOTE: usually in combination with "alt_func_num<>".
     */
    using alternate_function = regval< typename GPIOx::template MODERx<pin_no>, 2 >;
    using analog             = regval< typename GPIOx::template MODERx<pin_no>, 3 >;
  };

  struct output_type
  {
    /** General purpose output push-pull (e.g. LED's) */
    using push_pull  = regval< typename GPIOx::template OTYPERx<pin_no>, 0 >;

    /** General purpose output open-drain */
    using open_drain = regval< typename GPIOx::template OTYPERx<pin_no>, 1 >;
  };


  /** I/O output speed */
  template<freq_t value>
  using speed = typename speed_impl<value>::type;


  struct resistor
  {
    /** Floating input */
    using floating  = regval< typename GPIOx::template PUPDRx<pin_no>, 0 >;

    /** Input with pull-up */
    using pull_up   = regval< typename GPIOx::template PUPDRx<pin_no>, 1 >;

    /** Input with pull-down */
    using pull_down = regval< typename GPIOx::template PUPDRx<pin_no>, 2 >;
  };

  /**
   * Alternate function selection
   *
   * NOTE: usually in combination with "mode::alternate_function".
   */
  template<unsigned value>
  using alt_func_num = typename alt_func_num_impl<value>::type;


public:  /* ------ static member functions ------ */

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


////////////////////  gpio_input  ////////////////////


template< char port, unsigned pin_no, gpio_active_state active_state = gpio_active_state::high >
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


template< char port, unsigned pin_no, gpio_active_state active_state = gpio_active_state::high >
class gpio_output
: public gpio_output_base< gpio< port, pin_no >, active_state >
{
  using gpio_type = gpio< port, pin_no >;
public:
  using resources = typelist<
    typename gpio_type::resources,
    typename gpio_type::mode::output
    >;
};


////////////////////  gpio_analog_io  ////////////////////


template<char port, unsigned pin_no, freq_t speed = mhz(2)>
class gpio_analog_io
: public gpio_analog_io_base< gpio< port, pin_no > >
{
  using gpio_type = gpio< port, pin_no >;
public:
  using resources = typelist<
    typename gpio_type::resources,
    typename gpio_type::mode::analog,
    typename gpio_type::output_type::push_pull,
    typename gpio_type::template speed< speed >,
    typename gpio_type::resistor::floating
    >;
};


////////////////////  gpio_led  ////////////////////


template<char port, unsigned pin_no, gpio_active_state active_state = gpio_active_state::high>
class gpio_led
: public gpio_led_base< gpio_output< port, pin_no, active_state > >
{
  using gpio_type = gpio_output< port, pin_no, active_state >;
public:
  using resources = typelist<
    typename gpio_type::resources,
    typename gpio_type::output_type::push_pull
    >;
};

} // namespace mptl

#endif // GPIO_HPP_INCLUDED
