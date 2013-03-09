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

// #define CORE_SIMULATION

#include <resource.hpp>
#include <register.hpp>
#include "unittest_static_assert.hpp"

#include <cassert>
#include <iostream>
#include <iomanip>

using namespace reg;

struct A : Register< uint32_t, 0x1000, Access::rw, 0 > {};
struct B : Register< uint32_t, 0x2000, Access::rw, 0x44444444 > {};
struct C : Register< uint8_t,  0x3000, Access::rw, 0 > {};
struct D : Register< uint32_t, 0x4000, Access::rw, 0x55555555 > {};

typedef SharedRegister< A, 0x00000011, 0x000000ff > test_a_0;
typedef SharedRegister< A, 0x00001100, 0x0000ff00 > test_a_1;
typedef SharedRegister< B, 0x00110000, 0x00ff0000 > test_b;
typedef SharedRegister< A, 0x11000000, 0xff000000 > test_a_2;
typedef SharedRegister< C, 0x10,       0xff       > test_c;

typedef SharedRegister< A, 0x00000000, 0x00000010 > anti_test_a_0; /* clears a bit which is set by test_a_0 */

typedef UniqueResource< A > uniq_a;
typedef UniqueResource< B > uniq_b;
typedef UniqueResource< C > uniq_c;
typedef UniqueResource< D > uniq_d;



typedef ResourceList < uniq_a,
                       ResourceList<ResourceList<test_a_0,
                                                 test_a_1,
                                                 test_b> >,
                       test_a_1,
                       ResourceList<uniq_b>,
                       uniq_c, uniq_d,
                       ResourceList<test_b,
                                    ResourceList<test_a_2>,
                                    test_a_0>,
                       test_c > list;

typedef ResourceList < list, uniq_c > uniq_fail_list;
typedef ResourceList < anti_test_a_0, list > bitmask_fail_list;


int main()
{
  /* check unique resources */
  list::check();
  UNITTEST_STATIC_ASSERT( uniq_fail_list::check() );
  UNITTEST_STATIC_ASSERT( bitmask_fail_list::configure() );

  assert(A::load() == 0);
  assert(B::load() == 0x44444444);
  assert(C::load() == 0);
  assert(D::load() == 0x55555555);

  //  SharedRegisterFunc<A>::configure<list>();

  /* set all shared register from list */
  list::configure();

  assert(A::load() == 0x11001111);
  assert(B::load() == 0x44114444);
  assert(C::load() == 0x10);
  assert(D::load() == 0x55555555);

  return 0;
}
