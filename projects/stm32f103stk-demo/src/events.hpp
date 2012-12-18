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

#ifndef EVENTS_HPP_INCLUDED
#define EVENTS_HPP_INCLUDED

#include <tinyfsm.hpp>
#include <joystick/stm32f103stk/joystick.hpp>

// #include "fsmlist.hpp"  // used for BoundEvent

struct Stick
: public tinyfsm::Event
{
  Joystick::Position pos;
};

struct JoystickButton : public tinyfsm::Event { };
struct JoystickUp     : public tinyfsm::Event { };
struct JoystickDown   : public tinyfsm::Event { };
struct JoystickLeft   : public tinyfsm::Event { };
struct JoystickRight  : public tinyfsm::Event { };
struct JoystickCenter : public tinyfsm::Event { };

#endif
