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

#include <arch/core.hpp>
#include <typelist.hpp>
#include <register.hpp>

#include <cassert>
#include <iostream>
#include <iomanip>

using namespace mptl;

struct A : regdef< uint32_t, 0x1000, reg_access::rw, 0 > {};
struct B : regdef< uint32_t, 0x2000, reg_access::rw, 0x44444444 > {};
struct C : regdef< uint8_t,  0x3000, reg_access::rw, 0 > {};
struct D : regdef< uint32_t, 0x4000, reg_access::rw, 0x55555555 > {};

using list = typelist<
  regmask<A, 0x00000011, 0x000000ff>,
  regmask<A, 0x00001100, 0x0000ff00>,
  regmask<B, 0x00110000, 0x00ff0000>,
  regmask<A, 0x11000000, 0xff000000>,
  regmask<C, 0x10,       0xff      >
  >;


using list2 = typelist<
  regmask<B, 0x11000000, 0xff000000>
  >;

struct X : typelist_element { };
struct Y : typelist_element { };
struct Z : typelist_element { };


using list4 = typelist< void, X, void, Y, void >;
using list5 = typelist< Z, list4, void, Z >;
/* NOTE: append<> is not allowed, see note about friends in typelist.hpp */
using list6 = list4::append< list5 >; 

// Hint: template debugging:
#if 0
template<typename T> struct incomplete;
incomplete<list5> debug;
#endif


using merged = mpl::merged_regmask<
  void,
  regmask<D, 0x00000011, 0x000000ff>,
  void,
  regmask<D, 0x00001100, 0x0000ff00>,
  void
  >::type;


int main()
{
  assert(A::load() == 0);
  assert(B::load() == 0x44444444);
  assert(C::load() == 0);
  assert(D::load() == 0x55555555);

  /* set all register from list */
  mptl::core::configure<list>();

  assert(A::load() == 0x11001111);
  assert(B::load() == 0x44114444);
  assert(C::load() == 0x10);
  assert(D::load() == 0x55555555);

  D::reset_to<merged>();
  assert(D::load() == 0x55551111);

  return 0;
}
