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

#include <resource_mpl.hpp>
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

typedef SharedRegister< A, 0x00000011, 0x000000ff > test0;
typedef SharedRegister< A, 0x00001100, 0x0000ff00 > test1;
typedef SharedRegister< B, 0x00110000, 0x00ff0000 > test2;
typedef SharedRegister< A, 0x11000000, 0xff000000 > test3;
typedef SharedRegister< C, 0x10,       0xff       > test4;

typedef UniqueResource< A > uniq0;
typedef UniqueResource< B > uniq1;
typedef UniqueResource< C > uniq2;
typedef UniqueResource< D > uniq3;



typedef ResourceList < uniq0,
                       ResourceList<ResourceList<test0,
                                                 test1,
                                                 test2> >,
                       test1,
                       ResourceList<uniq1>,
                       uniq2, uniq3,
                       ResourceList<test2,
                                    ResourceList<test3>,
                                    test0>,
                       test4 > list;

typedef ResourceList < list, uniq2 > fail_list;


int main()
{
  /* check unique resources */
  list::check();
  UNITTEST_STATIC_ASSERT( fail_list::check() );

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
