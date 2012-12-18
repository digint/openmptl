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

#ifndef SCB_HPP_INCLUDED
#define SCB_HPP_INCLUDED

#include <core.hpp>


class Scb : private InterruptControllerSetup
{

  using SCB = Core::SCB;

public:
  /**
   * @brief  Set the Priority Grouping in NVIC Interrupt Controller
   *
   * @param  PriorityGroup is priority grouping field
   *
   * Set the priority grouping field using the required unlock sequence.
   * The parameter priority_grouping is assigned to the field 
   * SCB->AIRCR [10:8] PRIGROUP field. Only values from 0..7 are used.
   * In case of a conflict between priority grouping and available
   * priority bits (__NVIC_PRIO_BITS) the smallest possible priority group is set.
   */
  static void SetPriorityGrouping(uint32_t group) {
    uint32_t reg;
//TODO      assert(group == (group & 0x07));

    reg  =  SCB::AIRCR::load();
    reg &= ~(SCB::AIRCR::VECTKEY::value | SCB::AIRCR::PRIGROUP::value);
    reg |= (0x5FA << 16) | (group << 8);
    SCB::AIRCR::store(reg);
  }

  /**
   * @brief  Get the Priority Grouping from NVIC Interrupt Controller
   *
   * @return priority grouping field 
   *
   * Get the priority grouping from NVIC Interrupt Controller.
   * priority grouping is SCB->AIRCR [10:8] PRIGROUP field.
   */
  static uint32_t GetPriorityGrouping(void) {
    return SCB::AIRCR::PRIGROUP::test_and_shift();
  }


  /**
   * @brief  Set the priority for an interrupt
   *
   * @param  IRQn      The number of the interrupt for set priority
   * @param  priority  The priority to set
   *
   * Set the priority for the specified interrupt. The interrupt 
   * number can be positive to specify an external (device specific) 
   * interrupt, or negative to specify an internal (core) interrupt.
   *
   * Note: The priority cannot be set for every core interrupt.
   */
  template<IrqNumber::CoreException irqn>
  static void SetPriority(uint32_t priority) {
    // TODO: core_reg magic
    SCB::SHPR<((uint32_t)irqn & 0xF)-4>::store((priority << (8 - priority_bits)) & 0xff);
  }

  /**
   * @brief  Read the priority for an interrupt
   *
   * @param  IRQn      The number of the interrupt for get priority
   * @return           The priority for the interrupt
   *
   * Read the priority for the specified interrupt. The interrupt 
   * number can be positive to specify an external (device specific) 
   * interrupt, or negative to specify an internal (core) interrupt.
   *
   * The returned priority value is automatically aligned to the implemented
   * priority bits of the microcontroller.
   *
   * Note: The priority cannot be set for every core interrupt.
   */
  template<IrqNumber::CoreException irqn>
  static uint32_t GetPriority(void) {
    // TODO: core_reg magic
    return((uint32_t)(SCB::SHPR<((uint32_t)irqn & 0xF)-4>::load() >> (8 - priority_bits)));
  }
};


#endif // SCB_HPP_INCLUDED
