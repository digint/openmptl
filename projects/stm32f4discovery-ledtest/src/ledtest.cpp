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

#include <arch/vector_table.hpp>
#include <arch/core.hpp>
#include <arch/rcc.hpp>
#include <arch/pwr.hpp>
#include <arch/flash.hpp>
#include <arch/nvic.hpp>
#include <arch/reg/gpio.hpp>
#include <typelist.hpp>
#include <simulation.hpp>

extern const uint32_t _stack_top;  /* provided by linker script */

static constexpr char     led_port = 'D';
static constexpr unsigned led_pin  = 12;

using sysclk  = mptl::system_clock_hse< mptl::mhz(168) >;
using pwr     = mptl::pwr< mptl::volt(3.3) >;

using early_cfg = mptl::flash::latency::minimum< sysclk, pwr >;

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
  mptl::core::startup< sysclk, early_cfg >();

  /* set all register from our resources<> typelist */
  mptl::make_reglist< resources >::reset_to();

#ifdef HIGH_LEVEL
  led_green::on();
#else
  static constexpr uint32_t pin_mask = (uint32_t)0x1 << led_pin;
  mptl::RCC::AHB1ENR::GPIODEN::set();  // enable AHB1 clock for GPIO D
  mptl::regval< mptl::GPIO< led_port >::MODERx< led_pin >, 1 >::set();  // configure GPIOD::MODERx (set GPIOD12 to output mode)
  mptl::GPIO< led_port >::BSRR::store(pin_mask);  // enable led
#endif

  while(1) {
    mptl::core::nop();

    SIM_RELAX; // sleep a bit (don't eat up all cpu power)
  }
}

/* Build the vector table:
 * - use irq handler from irq_handler<> traits in "resources"
 * - use null_isr as default isr (useful for debugging)
 */
using vector_table = mptl::vector_table<&_stack_top, resources, null_isr>;
const auto isr_vector __attribute__((used,section(".isr_vector"))) = vector_table::value;


#ifdef OPENMPTL_SIMULATION

const uint32_t _stack_top = 0;

void mptl::sim::reg_reaction::react() {
  /* simulate the system clock setup */
  if(bits_set< RCC::CR::HSEON >()) {
    info("reacting to RCC::CR::HSEON, by setting RCC::CR::HSERDY");
    RCC::CR::HSERDY::set();
  }
  if(bits_set< RCC::CR::PLLON >()) {
    info("reacting to RCC::CR::PLLON, by setting RCC::CR::PLLRDY");
    RCC::CR::PLLRDY::set();
  }
  if(bits_set< RCC::CFGR::SW::PLL >()) {
    info("reacting to RCC::CFGR::SW::PLL, by setting RCC::CFGR::SWS::PLL");
    RCC::CFGR::SWS::PLL::set();
  }
}

//int main(int argc, char *argv[])
int main(void)
{
  std::cout << "*** stm32f4discovery ledtest: starting simulation..." << std::endl;

#ifdef DUMP_VECTOR_TABLE
  vector_table.dump_size();
  vector_table.dump_types();
  // vector_table.dump_vector();
#endif

  reset_isr();
}

#endif // OPENMPTL_SIMULATION
