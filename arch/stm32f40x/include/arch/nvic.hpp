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

#include "../../../common/arm-cortex/nvic.hpp"
#include "../../../common/arm-cortex/vector_table.hpp"

extern const uint32_t _stack_top;  // provided by linker script

/* Configure vector table with 82 irq channels */
typedef VectorTableBuilder<82, &_stack_top>::value VectorTable;

static_assert(sizeof(VectorTable::vector_table) == 4 * (82 + NvicCortexSetup::core_exceptions), "IRQ vector table size error");

#endif // NVIC_HPP_INCLUDED
