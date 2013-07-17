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
#include <periph.hpp>
#include <type_traits>

namespace mptl {

namespace cfg { namespace gpio {

namespace mode
{
  struct input
  : public config_base
  {
    template<typename gpio_type>
    using regmask_type = regval< typename gpio_type::GPIOx::template MODERx<gpio_type::pin_no>, 0 >;
  };

  struct output
  : public config_base
  {
    template<typename gpio_type>
    using regmask_type = regval< typename gpio_type::GPIOx::template MODERx<gpio_type::pin_no>, 1 >;
  };

  /**
   * Alternate function mode selection
   *
   * NOTE: usually in combination with "alt_func_num<>".
   */
  struct alternate_function
  : public config_base
  {
    template<typename gpio_type>
    using regmask_type = regval< typename gpio_type::GPIOx::template MODERx<gpio_type::pin_no>, 2 >;
  };

  struct analog
  : public config_base
  {
    template<typename gpio_type>
    using regmask_type = regval< typename gpio_type::GPIOx::template MODERx<gpio_type::pin_no>, 3 >;
  };
} // namespace mode

namespace output_type
{
  /** General purpose output push-pull (e.g. LED's) */
  struct push_pull
  : public config_base
  {
    template<typename gpio_type>
    using regmask_type = regval< typename gpio_type::GPIOx::template OTYPERx<gpio_type::pin_no>, 0 >;
  };

  /** General purpose output open-drain */
  struct open_drain
  : public config_base
  {
    template<typename gpio_type>
    using regmask_type = regval< typename gpio_type::GPIOx::template OTYPERx<gpio_type::pin_no>, 1 >;
  };
} // namespace output_type

/** I/O output speed */
template<freq_t value>
struct speed
: public config_base
{
  static_assert((value == mhz(2))  ||
                (value == mhz(25)) ||
                (value == mhz(50)) ||
                (value == mhz(100)),
                "Illegal frequency for gpio output speed (allowed: mhz(2), mhz(25), mhz(50), mhz(100))");

  template<typename gpio_type>
  using regmask_type = regval<
    typename gpio_type::GPIOx::template OSPEEDRx<gpio_type::pin_no>,
    (value == mhz(25)  ? 1 :
     value == mhz(50)  ? 2 :
     value == mhz(100) ? 3 :
     0)
    >;
};

namespace resistor
{
  /** Floating input */
  struct floating
  : public config_base
  {
    template<typename gpio_type>
    using regmask_type = regval< typename gpio_type::GPIOx::template PUPDRx<gpio_type::pin_no>, 0 >;
  };

  /** Input with pull-up */
  struct pull_up
  : public config_base
  {
    template<typename gpio_type>
    using regmask_type = regval< typename gpio_type::GPIOx::template PUPDRx<gpio_type::pin_no>, 1 >;
  };

  /** Input with pull-down */
  struct pull_down
  : public config_base
  {
    template<typename gpio_type>
    using regmask_type = regval< typename gpio_type::GPIOx::template PUPDRx<gpio_type::pin_no>, 2 >;
  };
} // namespace resistor

namespace active_state
{
  /** pin is low-active */
  struct low
  : public config_base
  { };

  /** pin is high-active */
  struct high
  : public config_base
  { };
} // namespace active_state



/**
 * Alternate function selection
 *
 * NOTE: usually in combination with "mode::alternate_function".
 */
template<unsigned value>
struct alt_func_num
: public config_base // deriving from mode::alternate_function would introduce too much magic.
{
  static_assert(value < 16, "illegal alternate function number");

  template<typename gpio_type>
  using afr_regmask_type = typename std::conditional<
    gpio_type::pin_no < 8,
      regval< typename gpio_type::GPIOx::template AFRLx<gpio_type::pin_no>, value >,
      regval< typename gpio_type::GPIOx::template AFRHx<gpio_type::pin_no>, value >
      >::type;

#if 0  // deriving from mode::alternate_function would introduce too much magic.
  template<typename gpio_type>
  using regmask_type = typelist<
    mode::alternate_function::regmask_type< gpio_type >, // derive from base
    afr_regmask_type< gpio_type >                        // add own
    >;
#else
  template<typename gpio_type>
  using regmask_type = afr_regmask_type< gpio_type >;
#endif
};


} } // namespace cfg::gpio


////////////////////  gpio  ////////////////////


// template<char port,
//          unsigned pin_no,
//          cfg::gpio::mode moder_cfg = cfg::gpio::mode::input,
//          cfg::gpio::output_type otype_cfg = cfg::gpio::output_type::push_pull,
//          freq_t speed = mhz(2),
//          cfg::gpio::resistor resistor_cfg = cfg::gpio::resistor::floating,
//          unsigned alt_func_num = 0>
template< char port, unsigned _pin_no, typename... CFG >
class gpio
: public periph_cfg<
  gpio<port, _pin_no>,
  //  rcc_gpio_clock_resources<port>, // TODO: make config object out of these
  CFG...
  >
{
  static_assert((port >= 'A') && (port <= 'I'), "invalid GPIO port");
  static_assert(_pin_no < 16, "invalid GPIO pin-no");

protected:
  static constexpr uint32_t pin_mask = (uint32_t)0x1 << _pin_no;

public:

  static constexpr unsigned pin_no = _pin_no;
  using GPIOx = reg::GPIO<port>;
  using type = gpio< port, pin_no, CFG... >;
  using periph_cfg_type = periph_cfg< type, CFG... >;
  using cfg_list = typename periph_cfg_type::cfg_list;

  using resources = typelist<
    rcc_gpio_clock_resources<port>,
#if 0
    typename CFG::template resources< type >...,
    typename CFG::template regmask_type< type >...
#endif
    typename periph_cfg_type::resources // TODO: add defaults to periph_config, then we can remove the whole list here!
    // TODO: unique type
    //    resource::unique< gpio<port, pin_no> >,
    >;

  static constexpr bool is_low_active = cfg_list::template contains< cfg::gpio::active_state::low >::value;

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


// template<char port,
//          unsigned pin_no,
//          cfg::gpio::resistor resistor_cfg,
//          cfg::gpio::active_state active_state = cfg::gpio::active_state::low>
template< char port, unsigned pin_no, typename... CFG >
class gpio_input
// : public gpio< port,
//                pin_no,
//                cfg::gpio::mode::input,
//                cfg::gpio::output_type::push_pull,
//                mhz(2),
//                resistor_cfg >
: public gpio< port,
               pin_no,
               cfg::gpio::mode::input,
               CFG... >
{
//  typedef gpio_input<port, pin_no, resistor_cfg, active_state> type;
  using type = gpio_input< port, pin_no, CFG... >;
public:

  static bool active(void) {
    bool input = type::read_input_bit();
    return type::is_low_active ? !input : input;
  }
};


////////////////////  gpio_output  ////////////////////


// template<char port,
//          unsigned pin_no,
//          cfg::gpio::output_type otype_cfg,
//          cfg::gpio::resistor resistor_cfg = cfg::gpio::resistor::floating,
//          freq_t speed = mhz(50),
//          cfg::gpio::active_state active_state = cfg::gpio::active_state::low>
template<char port, unsigned pin_no, typename... CFG>
class gpio_output
: public gpio< port,
               pin_no,
               cfg::gpio::mode::output,
               CFG... >
{
public:
  using type = gpio_output<port, pin_no, CFG...>;

  static void enable() {
    if(type::is_low_active) {
      type::reset();
    } else {
      type::set();
    }
  }

  static void disable() {
    if(type::is_low_active) {
      type::set();
    } else {
      type::reset();
    }
  }

  static bool active() {
    bool input = type::read_input_bit();
    return type::is_low_active ? !input : input;
  }

  static void toggle() {
    if(type::read_input_bit()) {
      type::reset();
    }
    else {
      type::set();
    }
  }

  static bool latched() {
    bool output = type::read_output_bit();
    return type::is_low_active ? !output : output;
  }
};


////////////////////  gpio_input_af  ////////////////////


// template<char port,
//          unsigned pin_no,
//          unsigned alt_func_num,
//          cfg::gpio::resistor resistor_cfg = cfg::gpio::resistor::floating
//          >
#if 0  // deprecated: use gpio<mode::alt_func_num<>> instead.
// TODO: reenable when we support default configuration
template<char port, unsigned pin_no, typename... CFG>
class gpio_input_af
: public gpio< port,
               pin_no,
               cfg::gpio::mode::alternate_function,
               CFG... >
{
#if 0  // we don't want that
  using type = gpio_input_af<port, pin_no, CFG...>;
  static_assert(type::cfg_list::template contains< cfg::gpio::mode::alternate_function >::value,
                "gpio_input_af needs mode::alternate_function present in CFG template parameter");
#endif
};
#endif // deprecated


////////////////////  gpio_output_af  ////////////////////


// template<char port,
//          unsigned pin_no,
//          unsigned alt_func_num,
//          cfg::gpio::output_type otype_cfg = cfg::gpio::output_type::open_drain,
//          cfg::gpio::resistor resistor_cfg = cfg::gpio::resistor::floating,
//          freq_t speed = mhz(50)>
#if 0  // deprecated: use gpio<mode::alt_func_num<>> instead.
// TODO: reenable when we support default configuration
template<char port, unsigned pin_no, typename... CFG>
class gpio_output_af
: public gpio< port,
               pin_no,
#if 1 // we want this (TODO: really?)
               cfg::gpio::mode::alternate_function,
#endif
               CFG...>
{
#if 0  // we don't want that
  using type = gpio_output_af<port, pin_no, CFG...>;
  static_assert(type::cfg_list::template contains< cfg::gpio::mode::alternate_function >::value,
                "gpio_output_af needs mode::alternate_function present in CFG template parameter");
#endif
};
#endif // deprecated


////////////////////  gpio_analog_io  ////////////////////


template<char port, unsigned pin_no>
class gpio_analog_io
: public gpio< port,
               pin_no,
               cfg::gpio::mode::analog,
               cfg::gpio::output_type::push_pull,
               cfg::gpio::speed< mhz(2) >,
               cfg::gpio::resistor::floating >
{
  // TODO: get/set analog value
};


////////////////////  gpio_led  ////////////////////


// template<char port,
//          unsigned pin_no,
//          cfg::gpio::output_type otype_cfg = cfg::gpio::output_type::push_pull,
//          cfg::gpio::resistor resistor_cfg = cfg::gpio::resistor::floating,
//          freq_t speed = mhz(50),
//          cfg::gpio::active_state active_state = cfg::gpio::active_state::high>
template<char port, unsigned pin_no, typename... CFG>
class gpio_led
//: public gpio_output<port, pin_no, otype_cfg, resistor_cfg, speed, active_state>
: public gpio_output<port, pin_no, CFG...>
{
public:
  using type = gpio_led<port, pin_no, CFG...>;

  static void on() {
    type::enable();
  }
  static void off() {
    type::disable();
  }
};

} // namespace mptl

#endif // GPIO_HPP_INCLUDED
