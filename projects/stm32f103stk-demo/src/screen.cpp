/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright (C) 2012-2017 Axel Burri <axel@tty0.ch>
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

#include <tinyfsm.hpp>
#include "screen.hpp"

// ----------------------------------------------------------------------------
// States
//

class Main
: public Screen
{
  virtual void react(EvJoystickUp const &) {
    if(item_top == item_list->begin())
      item_top = item_list->end();
    item_top--;
  }

  virtual void react(EvJoystickDown const &) {
    item_top++;
    if(item_top == item_list->end())
      item_top = item_list->begin();
  }
};


// ----------------------------------------------------------------------------
// Base State: default implementations
//

void Screen::update(void) {
  ScreenItemList::iterator item(item_top);
  lcd.clear();

  unsigned i = 0;
  while((item != item_list->end()) && (i < lcd.rows)) {
#ifdef OPENMPTL_USE_BOOST
    lcd.print_line(i, item->c_str(), item->get_inverted());
#else
    lcd.print_line(i, (*item)->c_str(), (*item)->get_inverted());
#endif
    i++;
    item++;
  }

  lcd.update();
}

void Screen::set_item_list(ScreenItemList & list) {
  item_list = &list;
  item_top = item_list->begin();
}

Kernel::lcd Screen::lcd;

ScreenItemList *Screen::item_list;
ScreenItemList::iterator Screen::item_top;


// ----------------------------------------------------------------------------
// Initial state definition
//
FSM_INITIAL_STATE(Screen, Main)
