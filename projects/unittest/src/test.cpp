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

#include <resource.hpp>
#include <register.hpp>

#include <cassert>
#include <iostream>
#include <iomanip>

using namespace mptl;

#if 0
struct foreign_type { };

struct A : regdef< uint32_t, 0x1000, reg_access::rw, 0 > {};
struct B : regdef< uint32_t, 0x2000, reg_access::rw, 0x44444444 > {};
struct C : regdef< uint8_t,  0x3000, reg_access::rw, 0 > {};
struct D : regdef< uint32_t, 0x4000, reg_access::rw, 0x55555555 > {};

using list = resource::list<
  regmask<A, 0x00000011, 0x000000ff>,
  foreign_type,
  regmask<A, 0x00001100, 0x0000ff00>,
  regmask<B, 0x00110000, 0x00ff0000>,
  regmask<A, 0x11000000, 0xff000000>,
  regmask<C, 0x10,       0xff      >
  >;


using list2 = resource::list<
  regmask<B, 0x11000000, 0xff000000>
  >;

// using list3 = resource::list_cat< list, list2 >;

#endif

struct X : resource::typelist_element { };
struct Y : resource::typelist_element { };
struct Z : resource::typelist_element { };


using list4 = resource::list< void, X, void, Y, void >;
using list5 = resource::list< Z, list4, void, Z >;
using list6 = list4::append< list5 >;

// Hint: template debugging:
template<typename T> struct incomplete;
incomplete<list6> debug;

void reg_reaction::react() { }


int main()
{
#if 0
  assert(A::load() == 0);
  assert(B::load() == 0x44444444);
  assert(C::load() == 0);
  assert(D::load() == 0x55555555);

#if 0
  /* set all shared register from list */
  unique_merged_type_list::template for_each< reg_reset_to >();
#else
  reg_configure<list>();
#endif

  assert(A::load() == 0x11001111);
  assert(B::load() == 0x44114444);
  assert(C::load() == 0x10);
  assert(D::load() == 0x55555555);
#endif
  return 0;
}
