#define CORE_SIMULATION

#include <register.hpp>
#include <cassert>

class TEST
{
  static constexpr reg_addr_t reg_base = 0x1234;

  template<typename R>
  class __REG
  : public R
  {
    template<typename Rb>
    struct __BITS_4_7
    : public Rb
    {
      typedef RegisterConst< Rb, 0x0 > CONST_0;
      typedef RegisterConst< Rb, 0x1 > BIT_0;
      typedef RegisterConst< Rb, 0x2 > BIT_1;
      typedef RegisterConst< Rb, 0x3 > BIT_0_1;
      typedef RegisterConst< Rb, 0xd > CONST_d;
    };


  public:

    typedef              RegisterBits< R,  0,  4 >   BITS_0_3;
    typedef __BITS_4_7 < RegisterBits< R,  4,  4 > > BITS_4_7;
    typedef              RegisterBits< R,  8, 24 >   BITS_8_31;

  };

public:
  typedef __REG < Register< uint32_t, reg_base + 0x00, RegisterAccess::rw, 0x55555555 > > REG;
};


int main()
{
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
  assert(TEST::REG::BITS_0_3::test() == 0);
  assert(TEST::REG::BITS_4_7::test() == 0);
  assert(TEST::REG::BITS_8_31::test() == 0xffff0000);

  TEST::REG::BITS_0_3::set();
  assert(TEST::REG::BITS_0_3::test() == 0xf);
  assert(TEST::REG::BITS_4_7::test() == 0);
  assert(TEST::REG::BITS_8_31::test() == 0xffff0000);

  TEST::REG::BITS_0_3::set(0xc);
  assert(TEST::REG::BITS_0_3::test() == 0xc);


#if 0
  TEST::REG::BITS_0_3::set(0x0);
  TEST::REG::BITS_4_7::set(0xf);  // illegal, but not checked by RegisterBits
  assert(TEST::REG::BITS_4_7::test_and_shift() == 0xf); // fail
  assert(TEST::REG::BITS_0_3::test() == 0x0); // fail
#endif

  TEST::REG::BITS_0_3::set(0x0);
  TEST::REG::BITS_4_7::shift_and_set(0xc);
  assert(TEST::REG::BITS_4_7::test() == 0xc0);
  assert(TEST::REG::BITS_4_7::test_and_shift() == 0xc);
  assert(TEST::REG::BITS_0_3::test() == 0x0);

  TEST::REG::BITS_4_7::set();
  assert(TEST::REG::BITS_4_7::test_and_shift() == 0xf);
  assert(TEST::REG::BITS_0_3::test() == 0x0);

  TEST::REG::store(0xffffffff);
  TEST::REG::BITS_4_7::clear();
  assert(TEST::REG::BITS_4_7::test() == 0x0);
  assert(TEST::REG::BITS_4_7::test_and_shift() == 0x0);
  assert(TEST::REG::BITS_0_3::test() == 0xf);

  TEST::REG::store(0xffffffff);
  assert(TEST::REG::BITS_0_3::test() == 0xf);
  assert(TEST::REG::BITS_4_7::test() == 0xf0);
  assert(TEST::REG::BITS_8_31::test() == 0xffffff00);

  TEST::REG::store(0xffffffff);
  assert(TEST::REG::BITS_0_3::test() == 0xf);
  assert(TEST::REG::BITS_4_7::test() == 0xf0);
  assert(TEST::REG::BITS_8_31::test() == 0xffffff00);
  assert(TEST::REG::BITS_0_3::test(0x1) == false);
  assert(TEST::REG::BITS_0_3::test(0xe) == false);

  TEST::REG::store(0x000000cc);
  assert(TEST::REG::BITS_0_3::test(0x1) == false);
  assert(TEST::REG::BITS_0_3::test(0xc) == true);
  assert(TEST::REG::BITS_0_3::test(0xd) == false);
  assert(TEST::REG::BITS_4_7::test(0xc) == false);
  assert(TEST::REG::BITS_4_7::test(0xc0) == true);
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

  return 0;
}
