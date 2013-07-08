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

struct A : regdef< uint32_t, 0x1000, reg_access::rw, 0 > {};
struct B : regdef< uint32_t, 0x2000, reg_access::rw, 0x44444444 > {};
struct C : regdef< uint8_t,  0x3000, reg_access::rw, 0 > {};
struct D : regdef< uint32_t, 0x4000, reg_access::rw, 0x55555555 > {};

typedef resource::reg_shared< regmask<A, 0x00000011, 0x000000ff> > test_a_0;
typedef resource::reg_shared< regmask<A, 0x00001100, 0x0000ff00> > test_a_1;
typedef resource::reg_shared< regmask<B, 0x00110000, 0x00ff0000> > test_b;
typedef resource::reg_shared< regmask<A, 0x11000000, 0xff000000> > test_a_2;
typedef resource::reg_shared< regmask<C, 0x10,       0xff      > > test_c;

typedef resource::reg_shared< regmask<A, 0x00000000, 0x00000010> > anti_test_a_0; /* clears a bit which is set by test_a_0 */

typedef resource::unique< A > uniq_a;
typedef resource::unique< B > uniq_b;
typedef resource::unique< C > uniq_c;
typedef resource::unique< D > uniq_d;

typedef resource::list <
  void,
  uniq_a,
  resource::list<
    resource::list<
      test_a_0,
      test_a_1,
      test_b
      >
    >,
  test_a_1,
  resource::list<
    uniq_b
    >,
  uniq_c,
  uniq_d,
  void,
  resource::list<
    void
    >,
  resource::list<
    test_b,
    resource::list<
      test_a_2
      >,
    void,
    test_a_0
    >,
  test_c,
  void
  > list;

typedef resource::list < list, uniq_c > uniq_fail_list;
typedef resource::list < anti_test_a_0, list > bitmask_fail_list;

void reg_reaction::react() { }

int main()
{
  /* check unique resources */
  list::check();

#ifdef UNITTEST_MUST_FAIL
  // fail: resource::list contains a resource derived from unique_resource (-> uniq_c) which is not unique.
  uniq_fail_list::check();
#endif

#ifdef UNITTEST_MUST_FAIL
  // fail: set/clear check failed: setting a bit which was previously cleared leads to undefined behaviour
  bitmask_fail_list::configure();
#endif

  assert(A::load() == 0);
  assert(B::load() == 0x44444444);
  assert(C::load() == 0);
  assert(D::load() == 0x55555555);

  /* set all shared register from list */
  list::configure();

  assert(A::load() == 0x11001111);
  assert(B::load() == 0x44114444);
  assert(C::load() == 0x10);
  assert(D::load() == 0x55555555);

  return 0;
}
