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

#include <arch/core.hpp>
#include <typelist.hpp>
#include <register.hpp>

#include <cassert>
#include <iostream>
#include <iomanip>

std::ostream & mptl::sim::regdump_ostream = std::cout;

using namespace mptl;

struct A : reg< uint32_t, 0x1000, rw, 0 > {};
struct B : reg< uint32_t, 0x2000, rw, 0x44444444 > {};
struct C : reg< uint8_t,  0x3000, rw, 0 > {};
struct D : reg< uint32_t, 0x4000, rw, 0x55555555 > {};

using test_a_0 = regmask< A, 0x00000011, 0x000000ff >;
using test_a_1 = regmask< A, 0x00001100, 0x0000ff00 >;
using test_b   = regmask< B, 0x00110000, 0x00ff0000 >;
using test_a_2 = regmask< A, 0x11000000, 0xff000000 >;
using test_c   = regmask< C, 0x10,       0xff       >;

using anti_test_a_0 = regmask< A, 0x00000000, 0x00000010 >; /* clears a bit which is set by test_a_0 */

using uniq_a = typelist_unique_element< A >;
using uniq_b = typelist_unique_element< B >;
using uniq_c = typelist_unique_element< C >;
using uniq_d = typelist_unique_element< D >;

using list = typelist <
  void,
  uniq_a,
  typelist<
    typelist<
      test_a_0,
      test_a_1,
      test_b
      >
    >,
  test_a_1,
  typelist<
    uniq_b
    >,
  uniq_c,
  uniq_d,
  void,
  typelist<
    void
    >,
  typelist<
    test_b,
    typelist<
      test_a_2
      >,
    void,
    test_a_0
    >,
  test_c,
  void
  >;

using uniq_fail_list    = typelist < list, uniq_c >;
using bitmask_fail_list = typelist < anti_test_a_0, list >;

int main()
{
#ifdef UNITTEST_MUST_FAIL
#warning UNITTEST_MUST_FAIL: set/clear check failed: setting a bit which was previously cleared
  make_reglist< bitmask_fail_list >::reset_to();
#endif

  assert(A::load() == 0);
  assert(B::load() == 0x44444444);
  assert(C::load() == 0);
  assert(D::load() == 0x55555555);

  /* set all shared register from list */
  make_reglist< list >::reset_to();

  assert(A::load() == 0x11001111);
  assert(B::load() == 0x44114444);
  assert(C::load() == 0x10);
  assert(D::load() == 0x55555555);

  return 0;
}
