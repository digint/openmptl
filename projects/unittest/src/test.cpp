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
#include <register_list.hpp>

#include <cassert>
#include <iostream>
#include <iomanip>

using namespace mptl;

struct foreign_type { };

struct A : regdef< uint32_t, 0x1000, reg_access::rw, 0 > {};
struct B : regdef< uint32_t, 0x2000, reg_access::rw, 0x44444444 > {};
struct C : regdef< uint8_t,  0x3000, reg_access::rw, 0 > {};
struct D : regdef< uint32_t, 0x4000, reg_access::rw, 0x55555555 > {};

using list = reglist<
  regmask<A, 0x00000011, 0x000000ff>,
  foreign_type,
  regmask<A, 0x00001100, 0x0000ff00>,
  regmask<B, 0x00110000, 0x00ff0000>,
  regmask<A, 0x11000000, 0xff000000>,
  regmask<C, 0x10,       0xff      >
  >;

void reg_reaction::react() { }

struct regmask_filter
{
  template<typename T>
  struct filter {
    static constexpr bool value = std::is_base_of<regmask_base, T>::value;
  };
};


struct merge_transformation
{
  template<typename Rm>
  struct filter_condition
  {
    template<typename T>
    struct filter {
      static constexpr bool value = std::is_same<typename Rm::reg_type, typename T::reg_type>::value;
    };
  };

  template<typename R, typename reglist_type>
  struct transform {
    using filtered_list = typename reglist_type::template filter< filter_condition<R> >::type;
    using type = typename filtered_list::merge::type;
  };
};

struct reset_register_new
{
  template<typename R, typename reglist_type>
  static void shoot(void) {
    R::reg_type::template reset_to< R >();
  }
};


int main()
{
  using filtered_list = list::filter<regmask_filter>::type;
  using merged_type_list = filtered_list::template transform<merge_transformation>::type;
  using unique_merged_type_list = typename merged_type_list::uniq::type;
 

#if 1
  assert(A::load() == 0);
  assert(B::load() == 0x44444444);
  assert(C::load() == 0);
  assert(D::load() == 0x55555555);

  /* set all shared register from list */
  unique_merged_type_list::template execute<reset_register_new>();

  assert(A::load() == 0x11001111);
  assert(B::load() == 0x44114444);
  assert(C::load() == 0x10);
  assert(D::load() == 0x55555555);
#endif
  return 0;
}
