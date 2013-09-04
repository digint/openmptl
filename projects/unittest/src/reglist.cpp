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

#include <register.hpp>
#include <cassert>
#include <iostream>

using namespace mptl;

std::ostream & mptl::sim::regdump_ostream = std::cout;

using A = reg< uint8_t,  0x00, rw, 0 >;
using B = reg< uint16_t, 0x04, rw, 0 >;
using C = reg< uint8_t,  0x08, rw, 0xff >;

template<> struct address_map< 0x00 > { static constexpr const char * name_str = "TEST::A"; };
template<> struct address_map< 0x04 > { static constexpr const char * name_str = "TEST::B"; };
template<> struct address_map< 0x08 > { static constexpr const char * name_str = "TEST::C"; };

using A0 = regmask< A, 0x01, 0x01 >;
using A1 = regmask< A, 0x02, 0x02 >;
using A2 = regmask< A, 0x04, 0x04 >;

using B0 = regmask< B, 0x01, 0x01 >;
using B1 = regmask< B, 0x02, 0x02 >;
using B2 = regmask< B, 0x04, 0x04 >;

using C0 = regmask< C, 0x00, 0x01 >;
using C1 = regmask< C, 0x00, 0x02 >;
using C2 = regmask< C, 0x00, 0x04 >;

struct Dummy : typelist_element { };


int main()
{
  std::cout << "*** main ***" << std::endl; 

  using list_A         = reglist< A2, void, A0, A1, A2, void >;
  using list_AB        = reglist< void, A0, B2, void, A2, A0, A2, void, B0 >;
  using list_ABC       = reglist< A0, B0, C0 >;
  using list_merged    = reglist< B0, list_A, B2, void, list_AB, B2 >;
  using list_empty     = reglist< void, void >;
  using list_dirty     = typelist< B0, list_A, Dummy, B2, void, list_AB, B2 >;
  using list_converted = make_reglist< list_dirty, B0, list_A >;

  static_assert(list_A::all_reg_type< A >::value, "");
  static_assert(list_A::all_reg_type< B >::value == false, "");

  static_assert(list_AB::all_reg_type< A, B >::value, "");
  static_assert(list_AB::all_reg_type< A, B, C >::value, "");
  static_assert(list_AB::all_reg_type< A >::value == false, "");
  static_assert(list_AB::all_reg_type< B >::value == false, "");

  static_assert(list_empty::all_reg_type< A >::value, "");  // no elements are ok

  A::store(0xf0); 
  list_A::set();
  assert(A::load() == 0xf7);

  list_AB::reset_to();
  assert(A::load() == 0x05);
  assert(B::load() == 0x05);

  list_ABC::reset_to();
  assert(A::load() == 0x01);
  assert(B::load() == 0x01);
  assert(C::load() == 0xfe);

  list_merged::reset_to();
  assert(A::load() == 0x07);
  assert(B::load() == 0x05);

  C::store(0x00);
  reglist< A0, C::neutral_regmask, B0 >::reset_to();
  assert(C::load() == 0xff);

  A::store(0xff);
  list_A::strict_reset_to< A >();
  assert(A::load() == 0x07);

  A::store(0xf0);
  B::store(0xf0);
  C::store(0xf0);
  list_AB::strict_reset_to< A, B, C >();
  assert(A::load() == 0x05);
  assert(B::load() == 0x05);
  assert(C::load() == 0xff);

  C::store(0x00);
  list_empty::strict_reset_to< C >();
  assert(C::load() == 0xff);

#ifdef UNITTEST_MUST_FAIL
#warning UNITTEST_MUST_FAIL: static_assert failed "one or more elements (aka: Tp...) are not of reg_type listed in strict_reg_type"
  list_AB::strict_reset_to< A >();
#endif

  A::store(0xf0);
  B::store(0xf0);
  C::store(0xf0);
  list_converted::set();
  assert(A::load() == 0xf7);
  assert(B::load() == 0xf5);
  assert(C::load() == 0xf0);

  return 0;
}
