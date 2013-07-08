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

#include <resource.hpp>
#include <arch/rcc.hpp>
#include <arch/reg/gpio.hpp>

namespace mptl {

namespace cfg { namespace gpio {

enum class resistor {
  floating,       //< Floating input
  pull_up,        //< Input with pull-up
  pull_down       //< Input with pull-down
};

enum class output_type {
  push_pull,      //< General purpose output push-pull (e.g. LED's)
  open_drain,     //< General purpose output open-drain
};

enum class active_state {
  low,            //< pin is low-active
  high            //< pin is high-active
};

enum class mode {
  input,
  output,
  alternate_function,
  analog
};

} } // namespace cfg::gpio


////////////////////  gpio  ////////////////////


template<char port,
         unsigned pin_no,
         cfg::gpio::mode moder_cfg = cfg::gpio::mode::input,
         cfg::gpio::output_type otype_cfg = cfg::gpio::output_type::push_pull,
         freq_t speed = mhz(2),
         cfg::gpio::resistor resistor_cfg = cfg::gpio::resistor::floating,
         unsigned alt_func_num = 0>
class gpio
{
  static_assert((port >= 'A') && (port <= 'I'), "invalid GPIO port");
  static_assert(pin_no < 16, "invalid GPIO pin-no");

  static_assert(alt_func_num < 16, "illegal alternate function number");

  static_assert((speed == mhz(2)) ||
                (speed == mhz(25)) ||
                (speed == mhz(50)) ||
                (speed == mhz(100)),
                "Illegal frequency for gpio output speed (allowed: mhz(2), mhz(25), mhz(50), mhz(100))");
protected:
  static constexpr uint32_t pin_mask         = (uint32_t)0x1 << pin_no;
  static constexpr uint32_t pin_mask_double  = (uint32_t)0x3 << (pin_no * 2);

  static constexpr uint32_t moder_value = (moder_cfg == cfg::gpio::mode::output ? 1 :
                                           moder_cfg == cfg::gpio::mode::alternate_function ? 2 :
                                           moder_cfg == cfg::gpio::mode::analog ? 3 :
                                           0) << (pin_no * 2);

  static constexpr uint32_t otyper_value = (otype_cfg == cfg::gpio::output_type::open_drain) ? pin_mask : 0;

  static constexpr uint32_t ospeedr_value =  (speed == mhz(25)  ? 1 :
                                              speed == mhz(50)  ? 2 :
                                              speed == mhz(100) ? 3 :
                                              0) << (pin_no * 2) ;

  static constexpr uint32_t pupdr_value =  (resistor_cfg == cfg::gpio::resistor::pull_up   ? 1 :
                                            resistor_cfg == cfg::gpio::resistor::pull_down ? 2 :
                                            0x00) << (pin_no * 2) ;

  static constexpr uint32_t afrl_value = pin_no <  8 ? alt_func_num << ((pin_no % 8) * 4) : 0;
  static constexpr uint32_t afrl_mask  = pin_no <  8 ? 0xf          << ((pin_no % 8) * 4) : 0;
  static constexpr uint32_t afrh_value = pin_no >= 8 ? alt_func_num << ((pin_no % 8) * 4) : 0;
  static constexpr uint32_t afrh_mask  = pin_no >= 8 ? 0xf          << ((pin_no % 8) * 4) : 0;

  typedef reg::GPIO<port> GPIOx;

public:

  typedef gpio<port, pin_no, moder_cfg, otype_cfg, speed, resistor_cfg, alt_func_num> type;

  using resources = resource::list<
    rcc_gpio_clock_resources<port>,
    resource::unique< gpio<port, pin_no> >,

    // TODO: make sure the registers are only set if they differ from reset value. does this make sense?
    resource::reg_shared< regmask< typename reg::GPIO<port>::MODER,   moder_value,   pin_mask_double > >,
    resource::reg_shared< regmask< typename reg::GPIO<port>::OTYPER,  otyper_value,  pin_mask        > >,
    resource::reg_shared< regmask< typename reg::GPIO<port>::OSPEEDR, ospeedr_value, pin_mask_double > >,
    resource::reg_shared< regmask< typename reg::GPIO<port>::PUPDR,   pupdr_value,   pin_mask_double > >,
    resource::reg_shared< regmask< typename reg::GPIO<port>::AFRL,    afrl_value,    afrl_mask       > >,
    resource::reg_shared< regmask< typename reg::GPIO<port>::AFRH,    afrh_value,    afrh_mask       > >
    >;

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


template<char port,
         unsigned pin_no,
         cfg::gpio::resistor resistor_cfg,
         cfg::gpio::active_state active_state = cfg::gpio::active_state::low>
class gpio_input
: public gpio< port,
               pin_no,
               cfg::gpio::mode::input,
               cfg::gpio::output_type::push_pull,
               mhz(2),
               resistor_cfg >
{
  typedef gpio_input<port, pin_no, resistor_cfg, active_state> type;
public:
  static bool active(void) {
    bool input = type::read_input_bit();
    return active_state == cfg::gpio::active_state::low ? !input : input;
  }
};


////////////////////  gpio_input_af  ////////////////////


template<char port,
         unsigned pin_no,
         unsigned alt_func_num,
         cfg::gpio::resistor resistor_cfg = cfg::gpio::resistor::floating
         >
class gpio_input_af
: public gpio< port,
               pin_no,
               cfg::gpio::mode::alternate_function,
               cfg::gpio::output_type::push_pull,
               mhz(2),
               resistor_cfg,
               alt_func_num >
{ };


////////////////////  gpio_output  ////////////////////


template<char port,
         unsigned pin_no,
         cfg::gpio::output_type otype_cfg,
         cfg::gpio::resistor resistor_cfg = cfg::gpio::resistor::floating,
         freq_t speed = mhz(50),
         cfg::gpio::active_state active_state = cfg::gpio::active_state::low>
class gpio_output
: public gpio< port,
               pin_no,
               cfg::gpio::mode::output,
               otype_cfg,
               speed,
               resistor_cfg >
{
public:
  typedef gpio_output<port, pin_no, otype_cfg, resistor_cfg, speed, active_state> type;

  static void enable() {
    if(active_state == cfg::gpio::active_state::low) {
      type::reset();
    } else {
      type::set();
    }
  }

  static void disable() {
    if(active_state == cfg::gpio::active_state::low) {
      type::set();
    } else {
      type::reset();
    }
  }

  static bool active() {
    bool input = type::read_input_bit();
    return active_state == cfg::gpio::active_state::low ? !input : input;
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
    return active_state == cfg::gpio::active_state::low ? !output : output;
  }
};


////////////////////  gpio_output_af  ////////////////////


template<char port,
         unsigned pin_no,
         unsigned alt_func_num,
         cfg::gpio::output_type otype_cfg = cfg::gpio::output_type::open_drain,
         cfg::gpio::resistor resistor_cfg = cfg::gpio::resistor::floating,
         freq_t speed = mhz(50)>
class gpio_output_af
: public gpio< port,
               pin_no,
               cfg::gpio::mode::alternate_function,
               otype_cfg,
               speed,
               resistor_cfg,
               alt_func_num >
{ };


////////////////////  gpio_analog_io  ////////////////////


template<char port,
         unsigned pin_no>
class gpio_analog_io
: public gpio< port,
               pin_no,
               cfg::gpio::mode::analog,
               cfg::gpio::output_type::push_pull,
               mhz(2),
               cfg::gpio::resistor::floating >
{
  // TODO: get/set analog value
};


////////////////////  gpio_led  ////////////////////


template<char port,
         unsigned pin_no,
         cfg::gpio::output_type otype_cfg = cfg::gpio::output_type::push_pull,
         cfg::gpio::resistor resistor_cfg = cfg::gpio::resistor::floating,
         freq_t speed = mhz(50),
         cfg::gpio::active_state active_state = cfg::gpio::active_state::high>
class gpio_led : public gpio_output<port, pin_no, otype_cfg, resistor_cfg, speed, active_state> {
public:
  typedef gpio_led<port, pin_no, otype_cfg, resistor_cfg, speed, active_state> type;

  static void on() {
    type::enable();
  }
  static void off() {
    type::disable();
  }
};

} // namespace mptl

#endif // GPIO_HPP_INCLUDED
