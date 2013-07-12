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
#include <register_manip.hpp>
#include <cassert>

namespace mptl { namespace reg {

class TEST
{
  static constexpr reg_addr_t base_addr = 0x1234;

  template<typename R>
  class __REG
  : public R
  {
    template<typename Rb>
    struct __CONST
    : public Rb
    {
      using CONST_0 = regval< Rb, 0x0 >;
      using BIT_0   = regval< Rb, 0x1 >;
      using BIT_1   = regval< Rb, 0x2 >;
      using BIT_0_1 = regval< Rb, 0x3 >;
      using CONST_d = regval< Rb, 0xd >;
    };


  public:

    using BITS_0_3  = __CONST < regbits< R,  0,  4 > >;
    using BITS_4_7  = __CONST < regbits< R,  4,  4 > >;
    using BITS_8_31 =           regbits< R,  8, 24 >;

  };

public:
  using REG = __REG < regdef< uint32_t, base_addr + 0x00, reg_access::rw, 0x55555555 > >;

  struct REG2
  : public regdef< uint32_t, base_addr + 0x10, reg_access::rw, 0xaaaaaaaa >
  {
    using reg_type = regdef< uint32_t, base_addr + 0x10, reg_access::rw, 0xaaaaaaaa >;

    using BITS_0_7 = regbits< reg_type, 0,  8 >;
  };
};

template<> struct address_map< 0x00001234 > { static constexpr const char * name_str = "TEST::REG"; };

} } //namespace mptl::reg

using namespace mptl;
using namespace reg;

void reg_reaction::react() { }

void unittest_register_manip()
{
  std::cout << "*** unittest_register_manip ***" << std::endl; 

  TEST::REG::reset();

  reg_manip<TEST::REG> reg;
  reg |= 0xff;
  reg &= 0x00ffffff;
  reg.set<TEST::REG::BITS_4_7::CONST_d>();
  reg.store();
  assert(TEST::REG::load() == 0x005555df);

  reg.clear<TEST::REG::BITS_0_3, TEST::REG::BITS_8_31>();
  reg.store();
  assert(TEST::REG::load() == 0x000000d0);
}

int main()
{
  std::cout << "*** main ***" << std::endl; 

  /* regmask */
  using merged_type = regmask<TEST::REG, 0xabcd0000>::merge<TEST::REG::BITS_4_7::CONST_d>::type;
  static_assert(merged_type::set_mask           == 0xabcd00d0, "");
  static_assert(merged_type::clear_mask         == 0xabcd00f0, "");
  static_assert(merged_type::cropped_clear_mask == 0x00000020, "");

  static_assert(regmask<TEST::REG, 0x11111111>::merge<
                regmask<TEST::REG, 0x222200ef>
                >::type::set_mask ==         0x333311ff, "");

  static_assert(TEST::REG::merge<
                regmask<TEST::REG, 0x11111111>,
                regmask<TEST::REG, 0x24824800>,
                regmask<TEST::REG, 0x48011107>
                >::type::set_mask ==         0x7d935917, "");

  using merged_reg = typename reglist<
    regmask<TEST::REG, 0x11111111>,
    regmask<TEST::REG, 0x24824800>,
    regmask<TEST::REG2, 0xffffffff>, // this is filtered out
    regmask<TEST::REG, 0x48011107>
    >::filter<TEST::REG>::type::merge::type;
  static_assert(merged_reg::set_mask == 0x7d935917, "");

#ifdef UNITTEST_MUST_FAIL
  // fail: template argument is not of same regdef<> type
  using merge_fail = TEST::REG::merge<
    regmask<TEST::REG, 0x11111111>,
    regmask<TEST::REG, 0x24824800>,
    regmask<TEST::REG2, 0xffffffff>, // this asserts
    regmask<TEST::REG, 0x48011107>
    >::type;
#endif

#ifdef UNITTEST_MUST_FAIL
  // fail: not same register
  using merged_type_fail = regmask<TEST::REG2, 0xabcd0000>::merge<TEST::REG::BITS_4_7::CONST_d>::type;
#endif



  /* regdef */
  TEST::REG::store(0xffff0000);
  assert(TEST::REG::test(0x000f0000) == 0x000f0000);
  assert(TEST::REG::test(0x0000c000) == 0x00000000);

  TEST::REG::mask(0x000c0000);
  assert(TEST::REG::load() == 0x000c0000);

  TEST::REG::store(0xffff0000);
  TEST::REG::mask (0x0000c000);
  assert(TEST::REG::load() == 0x00000000);

  TEST::REG::store(0xffff0000);
  TEST::REG::clear(0x00f00000);
  assert(TEST::REG::load() == 0xff0f0000);


  TEST::REG::reset();
  assert(TEST::REG::load() == 0x55555555);

  /* regbits */

  TEST::REG::store(0xffff0000);
  assert(TEST::REG::BITS_0_3::test() == false);
  assert(TEST::REG::BITS_4_7::test() == false);
  assert(TEST::REG::BITS_8_31::get() == 0xffff00);

  TEST::REG::BITS_0_3::set();
  assert(TEST::REG::BITS_0_3::get() == 0xf);
  assert(TEST::REG::BITS_0_3::test_from(0xf) == true);
  assert(TEST::REG::BITS_4_7::test() == false);
  assert(TEST::REG::BITS_4_7::test_from(0x0) == true);
  assert(TEST::REG::BITS_8_31::get() == 0xffff00);

  TEST::REG::BITS_0_3::set_from(0xc);
  assert(TEST::REG::BITS_0_3::test_from(0xc) == true);
  assert(TEST::REG::load() == 0xffff000c);

#if 0
  TEST::REG::store(0);
  TEST::REG::BITS_4_7::set(0xff);  // illegal, but not checked by regbits
  assert(TEST::REG::load() == 0x000000f0); // fails, since the bits were overwritten by illegal set() above
#endif

  TEST::REG::store(0xffffffff);
  TEST::REG::BITS_0_3::set_from(0x0);
  TEST::REG::BITS_4_7::set_from(0xc);
  assert(TEST::REG::BITS_4_7::test() == true);
  assert(TEST::REG::BITS_4_7::test_from(0xc) == true);
  assert(TEST::REG::BITS_0_3::test() == false);

  TEST::REG::BITS_4_7::set();
  assert(TEST::REG::BITS_4_7::test_from(0xf) == true);
  assert(TEST::REG::BITS_0_3::test() == false);

  TEST::REG::store(0xffffffff);
  TEST::REG::BITS_4_7::clear();
  assert(TEST::REG::BITS_4_7::test() == false);
  assert(TEST::REG::BITS_4_7::test_from(0x0) == true);
  assert(TEST::REG::BITS_0_3::test_from(0xf) == true);

  TEST::REG::store(0xffffffff);
  assert(TEST::REG::BITS_0_3::test_from(0xf) == true);
  assert(TEST::REG::BITS_4_7::test_from(0xf) == true);
  assert(TEST::REG::BITS_0_3::test_from(0x1) == false);
  assert(TEST::REG::BITS_0_3::test_from(0xe) == false);
  assert(TEST::REG::BITS_8_31::get() == 0x00ffffff);

  TEST::REG::store(0x000000cc);
  assert(TEST::REG::BITS_0_3::test_from(0x1) == false);
  assert(TEST::REG::BITS_0_3::test_from(0xc) == true);
  assert(TEST::REG::BITS_0_3::test_from(0xd) == false);
  assert(TEST::REG::BITS_4_7::test_from(0xc) == true);
  assert(TEST::REG::BITS_4_7::test_from(0xc0) == false);
  assert(TEST::REG::BITS_4_7::test_from(0xd0) == false);

  /* regval */

  TEST::REG::store(0xffffff0f);
  assert(TEST::REG::BITS_4_7::CONST_0::test() == true);
  assert(TEST::REG::BITS_4_7::BIT_0::test() == false);
  assert(TEST::REG::BITS_4_7::BIT_1::test() == false);
  assert(TEST::REG::BITS_4_7::BIT_0_1::test() == false);
  assert(TEST::REG::BITS_4_7::CONST_d::test() == false);

  TEST::REG::store(0x77777777);
  assert(TEST::REG::BITS_4_7::CONST_0::test() == false);
  assert(TEST::REG::BITS_4_7::BIT_0::test() == false);
  assert(TEST::REG::BITS_4_7::BIT_1::test() == false);
  assert(TEST::REG::BITS_4_7::BIT_0_1::test() == false);
  assert(TEST::REG::BITS_4_7::CONST_d::test() == false);

  TEST::REG::store(0x77777737);
  assert(TEST::REG::BITS_4_7::CONST_0::test() == false);
  assert(TEST::REG::BITS_4_7::BIT_0::test() == false);
  assert(TEST::REG::BITS_4_7::BIT_1::test() == false);
  assert(TEST::REG::BITS_4_7::BIT_0_1::test() == true);
  assert(TEST::REG::BITS_4_7::CONST_d::test() == false);

  TEST::REG::BITS_4_7::CONST_d::set();
  assert(TEST::REG::BITS_4_7::CONST_0::test() == false);
  assert(TEST::REG::BITS_4_7::BIT_0::test() == false);
  assert(TEST::REG::BITS_4_7::BIT_1::test() == false);
  assert(TEST::REG::BITS_4_7::BIT_0_1::test() == false);
  assert(TEST::REG::BITS_4_7::CONST_d::test() == true);

#ifdef UNITTEST_MUST_FAIL
  // fail: value=0x1f does not fit into bits of R=TEST::REG::BITS_4_7
  regval<TEST::REG::BITS_4_7, 0x1f>::set();
#endif


  /* Enhanced register functionality */

  TEST::REG::store(0xffffffff);
  TEST::REG::clear<TEST::REG::BITS_0_3, TEST::REG::BITS_8_31>();
  assert(TEST::REG::load() == 0x000000f0);

  TEST::REG::store(0xffffffff);
  TEST::REG::set<TEST::REG::BITS_4_7::BIT_1, TEST::REG::BITS_0_3::CONST_d>();


#ifdef UNITTEST_MUST_FAIL
  // fail: clear() is private on regval
  TEST::REG::BITS_0_3::CONST_d::clear();
#endif

  assert(TEST::REG::load() == 0xffffff2d);

  TEST::REG::store(0xffffffff);
  TEST::REG::set<TEST::REG::BITS_4_7>(0x50);
  assert(TEST::REG::load() == 0xffffff5f);

  TEST::REG::store(0xffffff0f);
  TEST::REG::set<TEST::REG::BITS_4_7::bit<1> >();
  assert(TEST::REG::load() == 0xffffff2f);

  unittest_register_manip(); // TODO: separate register_manip.cpp

#ifdef UNITTEST_MUST_FAIL
  // clearing bits from different register
  // fail: template arguments are not of same regdef<> type
  TEST::REG::clear<TEST::REG2::BITS_0_7>();
#endif

  return 0;
}
