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

#ifndef PWR_HPP_INCLUDED
#define PWR_HPP_INCLUDED

#include <arch/reg/pwr.hpp>

namespace mptl {

class pwr
{
public:

  static void disable_backup_domain_write_protection(void) {
    PWR::CR::DBP::set();
  }
};

} // namespace mptl

#endif // PWR_HPP_INCLUDED
