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

#include "screen.hpp"

#include <tinyfsm.hpp>
#include <cstring> // memcpy

// ----------------------------------------------------------------------------
// States
//

class Main
: public Screen
{
  virtual void react(EvJoystickUp const &) {
    if(item_top != item_list->begin()) {
      item_top--;
      update();
    }
  }

  virtual void react(EvJoystickDown const &) {
    item_top++;
    update();
  }
};


// ----------------------------------------------------------------------------
// Base State: default implementations
//

void Screen::update(void) {
  lcd.clear();
  ScreenItemList::iterator item(item_top);
  ScreenItemList::iterator end(item_list->end());

  unsigned i = 0;
  while((item != end) && (i < lcd.rows)) {
    lcd.print_line(i, item->c_str(), item->get_inverted());
    i++;
    item++;
  }

  lcd.update();
}

Kernel::lcd Screen::lcd;

ScreenItemList * Screen::item_list;
ScreenItemList::iterator Screen::item_top;


// ----------------------------------------------------------------------------
// Initial state definition
//
FSM_INITIAL_STATE(Screen, Main);

