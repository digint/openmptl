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

#include <arch/irq.hpp>

#ifndef CORE_SIMULATION

extern uint32_t _eram;

static_assert(sizeof(irq_func_t) == 4, "wrong size for irq function pointer");
static_assert(alignof(irq_func_t) == 4, "wrong alignment for irq function pointer table");

irq_func_t IrqVector::vector_table[] =
{
  /* first entry is the top-of-stack value */
  reinterpret_cast<irq_func_t>(reinterpret_cast<uint32_t>(&_eram)),

  /* fixed core exceptions */

  CoreExceptionReset     ::Handler,
  CoreExceptionNMI       ::Handler,
  CoreExceptionHardFault ::Handler,

  /* settable core exceptions */

  CoreException< IrqNumber::CoreException::MemoryManagement >::Handler,
  CoreException< IrqNumber::CoreException::BusFault         >::Handler,
  CoreException< IrqNumber::CoreException::UsageFault       >::Handler,
  0, 0, 0, 0,    /* reserved */
  CoreException< IrqNumber::CoreException::SVCall           >::Handler,
  CoreException< IrqNumber::CoreException::DebugMonitor     >::Handler,
  0,             /* reserved */
  CoreException< IrqNumber::CoreException::PendSV           >::Handler,
  CoreException< IrqNumber::CoreException::SysTick          >::Handler,

  /* external interrupts */
#if 0
  Irq< IrqNumber::Interrupt::WWDG            >::Handler,
  Irq< IrqNumber::Interrupt::PVD             >::Handler,
  Irq< IrqNumber::Interrupt::TAMPER          >::Handler,
  Irq< IrqNumber::Interrupt::RTC             >::Handler,
  Irq< IrqNumber::Interrupt::FLASH           >::Handler,
  Irq< IrqNumber::Interrupt::RCC             >::Handler,
  Irq< IrqNumber::Interrupt::EXTI0           >::Handler,
  Irq< IrqNumber::Interrupt::EXTI1           >::Handler,
  Irq< IrqNumber::Interrupt::EXTI2           >::Handler,
  Irq< IrqNumber::Interrupt::EXTI3           >::Handler,
  Irq< IrqNumber::Interrupt::EXTI4           >::Handler,
  Irq< IrqNumber::Interrupt::DMA1_Channel1   >::Handler,
  Irq< IrqNumber::Interrupt::DMA1_Channel2   >::Handler,
  Irq< IrqNumber::Interrupt::DMA1_Channel3   >::Handler,
  Irq< IrqNumber::Interrupt::DMA1_Channel4   >::Handler,
  Irq< IrqNumber::Interrupt::DMA1_Channel5   >::Handler,
  Irq< IrqNumber::Interrupt::DMA1_Channel6   >::Handler,
  Irq< IrqNumber::Interrupt::DMA1_Channel7   >::Handler,
  Irq< IrqNumber::Interrupt::ADC1_2          >::Handler,
  Irq< IrqNumber::Interrupt::USB_HP_CAN1_TX  >::Handler,
  Irq< IrqNumber::Interrupt::USB_LP_CAN1_RX0 >::Handler,
  Irq< IrqNumber::Interrupt::CAN1_RX1        >::Handler,
  Irq< IrqNumber::Interrupt::CAN1_SCE        >::Handler,
  Irq< IrqNumber::Interrupt::EXTI9_5         >::Handler,
  Irq< IrqNumber::Interrupt::TIM1_BRK        >::Handler,
  Irq< IrqNumber::Interrupt::TIM1_UP         >::Handler,
  Irq< IrqNumber::Interrupt::TIM1_TRG_COM    >::Handler,
  Irq< IrqNumber::Interrupt::TIM1_CC         >::Handler,
  Irq< IrqNumber::Interrupt::TIM2            >::Handler,
  Irq< IrqNumber::Interrupt::TIM3            >::Handler,
  Irq< IrqNumber::Interrupt::TIM4            >::Handler,
  Irq< IrqNumber::Interrupt::I2C1_EV         >::Handler,
  Irq< IrqNumber::Interrupt::I2C1_ER         >::Handler,
  Irq< IrqNumber::Interrupt::I2C2_EV         >::Handler,
  Irq< IrqNumber::Interrupt::I2C2_ER         >::Handler,
  Irq< IrqNumber::Interrupt::SPI1            >::Handler,
  Irq< IrqNumber::Interrupt::SPI2            >::Handler,
  Irq< IrqNumber::Interrupt::USART1          >::Handler,
  Irq< IrqNumber::Interrupt::USART2          >::Handler,
  Irq< IrqNumber::Interrupt::USART3          >::Handler,
  Irq< IrqNumber::Interrupt::EXTI15_10       >::Handler,
  Irq< IrqNumber::Interrupt::RTCAlarm        >::Handler,
  Irq< IrqNumber::Interrupt::USBWakeUp       >::Handler
#endif
};

#endif // CORE_SIMULATION
