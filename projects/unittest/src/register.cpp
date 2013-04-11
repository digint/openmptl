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
#include "unittest_static_assert.hpp"

namespace reg
{
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
      typedef RegisterConst< Rb, 0x0 > CONST_0;
      typedef RegisterConst< Rb, 0x1 > BIT_0;
      typedef RegisterConst< Rb, 0x2 > BIT_1;
      typedef RegisterConst< Rb, 0x3 > BIT_0_1;
      typedef RegisterConst< Rb, 0xd > CONST_d;
    };


  public:

    typedef __CONST < RegisterBits< R,  0,  4 > > BITS_0_3;
    typedef __CONST < RegisterBits< R,  4,  4 > > BITS_4_7;
    typedef              RegisterBits< R,  8, 24 >   BITS_8_31;

  };

public:
  typedef __REG < Register< uint32_t, base_addr + 0x00, Access::rw, 0x55555555 > > REG;

  struct REG2
  : public Register< uint32_t, base_addr + 0x10, Access::rw, 0xaaaaaaaa >
  {
    typedef Register< uint32_t, base_addr + 0x10, Access::rw, 0xaaaaaaaa > reg_type;

    typedef RegisterBits< reg_type, 0,  8 > BITS_0_7;
  };
};

  template<> struct AddressMap< 0x00001234 > { static constexpr const char * name_str = "TEST::REG"; };
}

using namespace reg;


void reg::RegisterReaction::react() { }

void unittest_register_manip()
{
  std::cout << "*** unittest_register_manip ***" << std::endl; 

  TEST::REG::reset();

  RegisterManip<TEST::REG> reg;
  reg |= 0xff;
  reg &= 0x00ffffff;
  reg.set<TEST::REG::BITS_4_7::CONST_d>();
  reg.store();
  assert(TEST::REG::load() == 0x005555df);

  uint32_t set_mask           = mpl::reg_combined<uint32_t, TEST::REG::BITS_4_7::CONST_d>::set_mask;
  uint32_t clear_mask         = mpl::reg_combined<uint32_t, TEST::REG::BITS_4_7::CONST_d>::clear_mask;
  uint32_t cropped_clear_mask = mpl::reg_combined<uint32_t, TEST::REG::BITS_4_7::CONST_d>::cropped_clear_mask;
  assert(set_mask           == 0x000000d0);
  assert(clear_mask         == 0x000000f0);
  assert(cropped_clear_mask == 0x00000020);
}

int main()
{
  std::cout << "*** main ***" << std::endl; 

  /* Register */
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

  /* RegisterBits */

  TEST::REG::store(0xffff0000);
  assert(TEST::REG::BITS_0_3::test() == false);
  assert(TEST::REG::BITS_4_7::test() == false);
  assert(TEST::REG::BITS_8_31::get() == 0xffff00);

  TEST::REG::BITS_0_3::set();
  assert(TEST::REG::BITS_0_3::get() == 0xf);
  assert(TEST::REG::BITS_0_3::test(0xf) == true);
  assert(TEST::REG::BITS_4_7::test() == false);
  assert(TEST::REG::BITS_4_7::test(0x0) == true);
  assert(TEST::REG::BITS_8_31::get() == 0xffff00);

  TEST::REG::BITS_0_3::set(0xc);
  assert(TEST::REG::BITS_0_3::test(0xc) == true);
  assert(TEST::REG::load() == 0xffff000c);

#if 0
  TEST::REG::store(0);
  TEST::REG::BITS_4_7::set(0xff);  // illegal, but not checked by RegisterBits
  assert(TEST::REG::load() == 0x000000f0); // fails, since the bits were overwritten by illegal set() above
#endif

  TEST::REG::store(0xffffffff);
  TEST::REG::BITS_0_3::set(0x0);
  TEST::REG::BITS_4_7::set(0xc);
  assert(TEST::REG::BITS_4_7::test() == true);
  assert(TEST::REG::BITS_4_7::test(0xc) == true);
  assert(TEST::REG::BITS_0_3::test() == false);

  TEST::REG::BITS_4_7::set();
  assert(TEST::REG::BITS_4_7::test(0xf) == true);
  assert(TEST::REG::BITS_0_3::test() == false);

  TEST::REG::store(0xffffffff);
  TEST::REG::BITS_4_7::clear();
  assert(TEST::REG::BITS_4_7::test() == false);
  assert(TEST::REG::BITS_4_7::test(0x0) == true);
  assert(TEST::REG::BITS_0_3::test(0xf) == true);

  TEST::REG::store(0xffffffff);
  assert(TEST::REG::BITS_0_3::test(0xf) == true);
  assert(TEST::REG::BITS_4_7::test(0xf) == true);
  assert(TEST::REG::BITS_0_3::test(0x1) == false);
  assert(TEST::REG::BITS_0_3::test(0xe) == false);
  assert(TEST::REG::BITS_8_31::get() == 0x00ffffff);

  TEST::REG::store(0x000000cc);
  assert(TEST::REG::BITS_0_3::test(0x1) == false);
  assert(TEST::REG::BITS_0_3::test(0xc) == true);
  assert(TEST::REG::BITS_0_3::test(0xd) == false);
  assert(TEST::REG::BITS_4_7::test(0xc) == true);
  assert(TEST::REG::BITS_4_7::test(0xc0) == false);
  assert(TEST::REG::BITS_4_7::test(0xd0) == false);

  /* RegisterConst */

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

  // TODO: fail: 
  //  UNITTEST_STATIC_ASSERT(( using fail_const = reg::RegisterConst<TEST::REG::BITS_4_7, 0x1f>; ))


  /* Enhanced register functionality */

  TEST::REG::store(0xffffffff);
  TEST::REG::clear<TEST::REG::BITS_0_3, TEST::REG::BITS_8_31>();
  assert(TEST::REG::load() == 0x000000f0);

  TEST::REG::store(0xffffffff);
  TEST::REG::set<TEST::REG::BITS_4_7::BIT_1, TEST::REG::BITS_0_3::CONST_d>();
  assert(TEST::REG::load() == 0xffffff2d);

  TEST::REG::store(0xffffffff);
  TEST::REG::set<TEST::REG::BITS_4_7>(0x50);
  assert(TEST::REG::load() == 0xffffff5f);

  TEST::REG::store(0xffffff0f);
  TEST::REG::set<TEST::REG::BITS_4_7::bit<1> >();
  assert(TEST::REG::load() == 0xffffff2f);

  unittest_register_manip(); // TODO: separate register_manip.cpp

  // fail: clearing bits from different register
  UNITTEST_STATIC_ASSERT( TEST::REG::clear<TEST::REG2::BITS_0_7>(); )

  return 0;
}
