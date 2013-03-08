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

#ifndef FSM_SCREEN_HPP_INCLUDED
#define FSM_SCREEN_HPP_INCLUDED

#include <tinyfsm.hpp>
#include "kernel.hpp"
#include "events.hpp"
#include "screenitem.hpp"


// ----------------------------------------------------------------------------
// FSM base class declaration
//

class Screen
: public tinyfsm::Fsm<Screen>
{
  friend class Fsm;

  void react(tinyfsm::Event const &) { };    /* default reaction (unhandled events) */

  virtual void react(JoystickUp   const &);
  virtual void react(JoystickDown const &);

  void entry(void) { };
  void exit(void)  { };

protected:

  static Kernel::lcd_n3310 lcd;

  static ItemList::iterator item_top;
  static ItemList * itemlist;

public:

  static void assign(ItemList * list) {
    itemlist = list;
    item_top = list->begin();
  }

  static void update(void);
};


#endif
