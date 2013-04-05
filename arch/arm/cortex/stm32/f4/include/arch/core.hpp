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

struct Core : public CoreAsm
{
  template<typename rcc,
           typename flash,
           typename pwr>
  static void startup(void) {
    crt::init_data_section();
    crt::init_bss_section();

    rcc::init();
    flash::init();
    pwr::init();
    rcc::set_system_clock();

    crt::call_ctors();
  }
};

#endif // CORE_HPP_INCLUDED
