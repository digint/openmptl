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

#include <arch/core.hpp>
#include <arch/rcc.hpp>
#include <arch/pwr.hpp>
#include <arch/flash.hpp>
#include <arch/nvic.hpp>
#include <arch/reg/gpio.hpp>
#include <typelist.hpp>
#include <simulation.hpp>

static constexpr char     led_port = 'D';
static constexpr unsigned led_pin  = 12;

using rcc     = mptl::rcc< mptl::mhz(168) >;
using pwr     = mptl::pwr< rcc, mptl::volt(3.3), false >;
using flash   = mptl::flash< rcc, pwr >;

#ifdef HIGH_LEVEL
#include <arch/gpio.hpp>
using led_green  = mptl::gpio_led< led_port, led_pin >;
#endif

void __naked reset_isr(void);
void null_isr(void) { }

using resources = mptl::typelist<
  mptl::irq_handler< typename mptl::irq::reset, reset_isr >
#ifdef HIGH_LEVEL
  , led_green::resources
#endif
  >;


/* Reset exception: triggered on system startup (system entry point). */
void __naked reset_isr(void) {
  mptl::core::startup<rcc, flash, pwr>();

  /* set all register from our resources<> typelist */
  mptl::core::configure< resources >();

#ifdef HIGH_LEVEL
  led_green ::on();
#else
  static constexpr uint32_t pin_mask = (uint32_t)0x1 << led_pin;
  mptl::reg::RCC::AHB1ENR::GPIODEN::reset_to();  // enable AHB1 clock for GPIO D
  mptl::regval< mptl::reg::GPIO< led_port >::MODERx< led_pin >, 1 >::set();  // configure GPIOD::MODERx (set GPIOD12 to output mode)
  mptl::reg::GPIO< led_port >::BSRR::store(pin_mask);  // enable led
#endif

  while(1) {
    mptl::core::nop();

    SIM_RELAX; // sleep a bit (don't eat up all cpu power)
  }
}


#ifndef OPENMPTL_SIMULATION

#include <arch/vector_table.hpp>
extern const uint32_t _stack_top;  /* provided by linker script */
mptl::vector_table<&_stack_top, resources, null_isr > vector_table;

#ifdef CONFIG_CLANG
// clang-3.3 ignores "__attribute__((used))" on vector_table_impl::isr_vector[]
// WORKAROUND: use something from isr_vector[] in an unused function
mptl::isr_t clang_workaround_attribute_used(void) {
  return vector_table.isr_vector[0];
}
#endif // CONFIG_CLANG


#else // OPENMPTL_SIMULATION


void mptl::sim::reg_reaction::react() {
  /* simulate the system clock setup */
  if(bits_set< reg::RCC::CR::HSEON >())
    reg::RCC::CR::HSERDY::set();
  if(bits_set< reg::RCC::CR::PLLON >())
    reg::RCC::CR::PLLRDY::set();
  if(bits_set< reg::RCC::CFGR::SW::PLL >())
    reg::RCC::CFGR::SWS::PLL::set();
}

//int main(int argc, char *argv[])
int main(void)
{
  std::cout << "*** stm32f4discovery ledtest: starting simulation..." << std::endl;

  reset_isr();
}

#endif // OPENMPTL_SIMULATION
