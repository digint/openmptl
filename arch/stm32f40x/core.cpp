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

#include <arch/rcc.hpp>
#include <arch/flash.hpp>
#include <arch/core.hpp>

void Core::InitClocks() {
  Rcc::EnableHSE();
  Rcc::WaitHSERDY();

  //  Flash::EnablePrefetchBuffer();
  Flash::EnableInstructionCache();
  Flash::EnableDataCache();
  Flash::SetLatency<clock_frequency>();

  Rcc::SetSysClock<clock_frequency>();
}

/* NOTE: this function is called BEFORE ANY constructors of static objects are called! */
void Core::InitHW(void) {
  Rcc::Init();
  InitClocks();

// TODO  InterruptController::Init();
//	NVIC_SetPriorityGrouping(7);	// no preemption, 4 bit of subprio
//      NVIC_SetPriorityGrouping(6);	// 1 bit preemption, 3 bit of subprio
//	NVIC_SetPriorityGrouping(5);	// 2 bit preemption, 2 bit of subprio
//	NVIC_SetPriorityGrouping(4);	// 3 bit preemption, 1 bit of subprio
//	NVIC_SetPriorityGrouping(3);	// 4 bit preemption, 0 bit of subprio
}
