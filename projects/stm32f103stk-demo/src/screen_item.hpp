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

#ifndef SCREENITEM_HPP_INCLUDED
#define SCREENITEM_HPP_INCLUDED

#include <cstdint>
#include <poorman_cstring.hpp>

class ScreenItem;

#ifdef OPENMPTL_USE_BOOST
# include <boost/intrusive/list.hpp>
using ScreenItemList = boost::intrusive::list<
  ScreenItem,
  boost::intrusive::link_mode<boost::intrusive::link_mode_type::normal_link>
  >;
#else
# include <list>
using ScreenItemList = std::list<ScreenItem*>;
#endif // OPENMPTL_USE_BOOST



class ScreenItem
#ifdef OPENMPTL_USE_BOOST
: public boost::intrusive::list_base_hook<> /* doubly linked list */
#endif
{
  bool inverted = false;

public:
#ifdef OPENMPTL_USE_BOOST
  boost::intrusive::list_member_hook<> member_hook_;
#endif

  ScreenItem(bool _inverted = false) : inverted(_inverted) { }
  ScreenItem(ScreenItemList & item_list, bool _inverted = false) : ScreenItem(_inverted) {
#ifdef OPENMPTL_USE_BOOST
    item_list.push_back(*this);
#else
    item_list.push_back(this);
#endif
  }

  void set_inverted(bool inv = true) { inverted = inv; }
  bool get_inverted(void) const { return inverted; }

  virtual const char * c_str(void) const { return "base"; }
};

class TextRow
: public ScreenItem
{
  const char * text;

public:
  TextRow(const char * _text, bool _inverted = false)
    : ScreenItem(_inverted), text(_text)
  { }

  TextRow(ScreenItemList & item_list, const char * _text, bool _inverted = false)
    : ScreenItem(item_list, _inverted), text(_text)
  { }

  virtual const char * c_str(void) const {
    return text;
  }
};


enum class NumberBase { hex, decimal };

class DataRow
: public ScreenItem
{
protected:
  static constexpr unsigned name_width  = 4;
  static constexpr unsigned colon_width = 2;
  static constexpr unsigned value_width = 8;

  static constexpr char padding_dec = '0';
  static constexpr char padding_hex = '0';

  uint32_t value;
  char buf[name_width + colon_width + value_width + 1];

  NumberBase numbase;

  void init_buf(const char * name);
  void print_value(void);

public:
  DataRow(const char * name, uint32_t _value = 0, NumberBase _numbase = NumberBase::decimal, bool _inverted = false);
  DataRow(ScreenItemList & item_list, const char * name, uint32_t _value = 0, NumberBase _numbase = NumberBase::decimal, bool _inverted = false);

  virtual const char * c_str(void) const final { return buf; };

  void set(uint32_t _value);
  void operator=(uint32_t _value) { set(_value); }
};

#endif
