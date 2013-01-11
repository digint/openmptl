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

#ifndef NVIC_HPP_INCLUDED
#define NVIC_HPP_INCLUDED

#include <arch/core.hpp>


template<IrqNumber::Interrupt irqn>
class Nvic : private InterruptControllerSetup
{
public:
  static constexpr std::size_t reg_index = (uint32_t)irqn >> 5;
  static constexpr std::size_t irq_bit = 1 << ((uint32_t)irqn & 0x1F);

  using NVIC  = Core::NVIC;
  using ISERx = NVIC::ISER<reg_index>;
  using ICERx = NVIC::ICER<reg_index>;
  using ISPRx = NVIC::ISPR<reg_index>;
  using ICPRx = NVIC::ICPR<reg_index>;
  using IABRx = NVIC::IABR<reg_index>;
  using IPRx  = NVIC::IPR<(std::size_t)irqn>;

  static void EnableIrq(void) {
    ISERx::store(irq_bit);
  }
  static void DisableIrq(void) {
    ICERx::store(irq_bit);
  }

  /**
   * @brief  Read the interrupt pending bit for a device specific interrupt source
   * 
   * @param  IRQn    The number of the device specifc interrupt
   * @return         1 = interrupt pending, 0 = interrupt not pending
   *
   * Read the pending register in NVIC and return 1 if its status is pending, 
   * otherwise it returns 0
   */
  static bool IsPending(void) {
    return ISPRx::load() & irq_bit;
  }

  /**
   * @brief  Set the pending bit for an external interrupt
   * 
   * @param  IRQn    The number of the interrupt for set pending
   *
   * Set the pending bit for the specified interrupt.
   * The interrupt number cannot be a negative value.
   */
  static void SetPending(void) {
    ISPRx::store(irq_bit); /* set interrupt pending */
  }

  /**
   * @brief  Clear the pending bit for an external interrupt
   *
   * @param  IRQn    The number of the interrupt for clear pending
   *
   * Clear the pending bit for the specified interrupt. 
   * The interrupt number cannot be a negative value.
   */
  static void ClearPending(void) {
    ICPRx::store(irq_bit); /* Clear pending interrupt */
  }

  /**
   * @brief  Read the active bit for an external interrupt
   *
   * @param  IRQn    The number of the interrupt for read active bit
   * @return         1 = interrupt active, 0 = interrupt not active
   *
   * Read the active register in NVIC and returns 1 if its status is active, 
   * otherwise it returns 0.
   */
  static bool IsActive(void) {
    return IABRx::load() & (irq_bit);
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
  static void SetPriority(uint32_t priority) {
    IPRx::store((priority << (8 - priority_bits)) & 0xff);
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
  static uint32_t GetPriority(void) {
    return((uint32_t)(IPRx::load() >> (8 - priority_bits)));
  }
};


#endif // NVIC_HPP_INCLUDED
