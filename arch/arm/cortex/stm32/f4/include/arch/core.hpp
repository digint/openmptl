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

#ifndef CORE_HPP_INCLUDED
#define CORE_HPP_INCLUDED

#include "../../../../common/core.hpp"

#include <crt.hpp>
#include <arch/flash.hpp>

namespace mptl {

struct core
: public core_asm
{
  template<
    typename system_clock_type,
    typename flash_cfg_reglist_type,
    typename pwr_type
    >
  static void startup(void) {
    crt::init_data_section();
    crt::init_bss_section();

    system_clock_type::init();
    flash::configure< flash_cfg_reglist_type >();
    pwr_type::init();
    system_clock_type::configure();

    crt::call_ctors();
  }
};

} // namespace mptl

#endif // CORE_HPP_INCLUDED
