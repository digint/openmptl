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

#ifndef REGISTER_HPP_INCLUDED
#define REGISTER_HPP_INCLUDED

/*
 * MPL-style Register class.
 *
 * - includes declarations of the reg<> type for:
 *   - embedded: register writes go to "constexpr volatile" register address
 *   - simulation: register writes go to variable in memory
 * - prohibits writing to read-only registers
 * - prohibits reading from write-only registers
 *
 */

//
//  microprocessor register definition: wrap bitfields into reg classes
//

//  Pros:
//
//  - uses reg<> template class to wrap the register access. All type
//    checks are performed on register write, AND on register bitfield
//    access.
//
//  - register bitfields are defined as type "static constexpr uint32_t",
//    which is very common, simple and straight forward.
//
//
//  Cons:
//
//  - MPL style "::value" variable access
//
//
// *** Don't miss the text from previous commit! ***


// Example:
//
// TODO: declaration example
//
// LINKS:
// "uint_fast16_t": http://en.cppreference.com/w/cpp/types/integer
//
// static void enable(void) {
//   SPIx::CR1::SPE::set()  /* SPI Enable */
// }
//
// void MyFunction() {
//   /* a load() call results in:                                                 */
//   /*                                                                           */
//   /* constexpr volatile T * value_ptr = reinterpret_cast<volatile T *>(addr);  */
//   /* auto reg = *value_ptr;                                                    */
//   /*                                                                           */
//   /* notes:                                                                    */
//   /* - "addr" (constexpr) holds the address of (non-pointer-type) SPIx::CR1    */
//   /* - type "T" is the register type (e.g. uint32_t or uint8_t)                */
//   /* - reg becomes type "volatile uint32_t                                     */
//   /*                                                                           */
//
//   auto reg = SPIx::CR1::load();
//   reg &= SPIx::CR1::SPE::value | SPIx::CR1::CRCNEXT::value | SPIx::CR1::CRCEN::value;
//   SPIx::CR1::store(reg);

//   /* a store() call results in:                                                */
//   /* *value_ptr = reg;                                                         */
// }
//


#include <type_traits>
#include <cstdint>
#include <register_access.hpp>
#include <typelist.hpp>
#include <compiler.h>
#include "register_mpl.hpp"

#ifdef CONFIG_DISABLE_AUTO_BITBAND
# define CONFIG_DISABLE_AUTO_BITBAND_WRITE
# define CONFIG_DISABLE_AUTO_BITBAND_READ
#endif

namespace mptl {


////////////////////  regmask  ////////////////////


template<
  typename Tp,    /* reg<> type */
  typename Tp::value_type _set_mask,
  typename Tp::value_type _clear_mask
  >
class regmask
: public mpl::regmask_tag, public typelist_element
{
  static_assert(std::is_same<typename Tp::type, typename Tp::reg_type>::value, "template argument Tp is not of type: reg<>");
  static_assert((_set_mask | _clear_mask) == _clear_mask, "clear_mask does not cover all bits of set_mask");

#ifndef CONFIG_USE_STD_TUPLE
  /* private constructor: instantiation would only cause confusion with set/clear functions */
  regmask() {};
#endif // CONFIG_USE_STD_TUPLE

public:
  using type         = regmask<typename Tp::reg_type, _set_mask, _clear_mask>;
  using reg_type     = typename Tp::reg_type;
  using regmask_type = type;
  using value_type   = typename Tp::value_type;

  static constexpr value_type set_mask   = _set_mask;
  static constexpr value_type clear_mask = _clear_mask;

  /**
   * Cropped clear_mask (USE WITH CARE!)
   *
   * The bits in cropped_clear_mask are only set if they do not
   * appear in the set_mask. This helps in some cases where the
   * compiler is not smart enough.
   * Example: (gcc-4.8 generating Arm-Thumb2)
   *
   *     x = (x & ~0x1ff00) | 0x0ff00
   *     x = (x & ~0x10000) | 0x0ff00   <-- one BIC instruction less!
   *
   * As long as a load-clear-set-store sequence is performed, the
   * above statements are equivalent, and the cropped_clear_mask can
   * be used.
   */
  static constexpr value_type cropped_clear_mask = clear_mask & ~set_mask;

#if !defined(CONFIG_DISABLE_AUTO_BITBAND_WRITE) || !defined(CONFIG_DISABLE_AUTO_BITBAND_READ)
  using bitcount = mpl::bitcount<clear_mask>;
  static constexpr bool bitop_enabled = reg_type::bitop_enabled && (bitcount::value == 1);
#endif

  static __always_inline void set(void) {
    // TODO: improvement: check for clear_mask covering ALL bits of
    // our reg_type. if yes, use store() instead!
#ifndef CONFIG_DISABLE_AUTO_BITBAND_WRITE
    if(bitop_enabled && (set_mask == clear_mask)) {
      reg_type::template bitset< bitcount::significant_bits - 1 >();
      return;
    }
#endif
    if((set_mask != 0) || (clear_mask != 0))  /* evaluated at compile-time */
      reg_type::set(set_mask, cropped_clear_mask);
  }

  static __always_inline void clear(void) {
#ifndef CONFIG_DISABLE_AUTO_BITBAND_WRITE
    if(bitop_enabled) {
      reg_type::template bitclear< bitcount::significant_bits - 1 >();
      return;
    }
#endif
    if(clear_mask != 0)  /* evaluated at compile-time */
      reg_type::clear(clear_mask);
  }

  static __always_inline bool test(void) {
    if(clear_mask == 0)
      return false;
#ifndef CONFIG_DISABLE_AUTO_BITBAND_READ
    if(bitop_enabled && (set_mask != 0))
      return reg_type::template bittest< bitcount::significant_bits - 1 >();
    if(bitop_enabled && (set_mask == 0))
      return !reg_type::template bittest< bitcount::significant_bits - 1 >();
#endif
    if(clear_mask == set_mask)
      return (reg_type::load() & clear_mask);
    return (reg_type::load() & clear_mask) == set_mask;
  }

  /** reset register, combined with set/clear mask (results in single store()) */
  static __always_inline void reset_to(void) {
    reg_type::store((reg_type::reset_value & ~cropped_clear_mask) | set_mask);
  }

  /**
   * Merge set_mask and clear_mask with the values of another
   * regmask<> type Up.
   */
  template<typename Rm>
  struct merge {
    static_assert(std::is_same<typename Rm::reg_type, reg_type>::value, "template argument is not of same reg<> type");

    /* assert if we set a bit which was previously cleared (and vice versa) */
    static_assert(!((Rm::set_mask & clear_mask) & (~set_mask)),     "set/clear check failed: setting a bit which was previously cleared");
    static_assert(!((set_mask & Rm::clear_mask) & (~Rm::set_mask)), "set/clear check failed: clearing a bit which was previously set");

    using type = regmask<reg_type, (set_mask | Rm::set_mask), (clear_mask | Rm::clear_mask)>;
  };
};


template<typename... Tp>
using merged_regmask = typename mpl::merged_regmask<Tp...>::type;


////////////////////  regbits  ////////////////////


template<
  typename Tp,         /* reg<> type */
  unsigned offset,     /* bit offset */
  unsigned width       /* number of bits */
  >
class regbits
: public regmask< Tp, ((1ul << width) - 1) << offset, ((1ul << width) - 1) << offset >
{
  static_assert(std::is_same<typename Tp::type, typename Tp::reg_type>::value, "template argument Tp is not of type: reg<>");
  static_assert(width >= 1, "invalid width");
  static_assert(offset + width <= sizeof(typename Tp::value_type) * 8, "invalid width/offset");

#ifndef CONFIG_USE_STD_TUPLE
  /* private constructor: instantiation would only cause confusion with set/clear functions */
  regbits() {};
#endif // CONFIG_USE_STD_TUPLE

 public:
  using type         = regbits<typename Tp::reg_type, offset, width>;
  using reg_type     = typename Tp::reg_type;
  using regmask_type = regmask<Tp, ((1ul << width) - 1) << offset, ((1ul << width) - 1) << offset>;
  using regbits_type = type;
  using value_type   = typename Tp::value_type;

  static constexpr value_type value = regmask_type::set_mask;

  static __always_inline constexpr value_type value_from(value_type const val) {
    // assert((val & (clear_mask >> offset)) == val);  /* input value does not match clear_mask */
    return (val << offset);
  }
  /** NOTE: this does not check if _value is masked correctly! */
  static __always_inline void set_from(value_type const val) {
    // assert((val & (clear_mask >> offset)) == val);  /* input value does not match clear_mask */
    reg_type::set(value_from(val), regmask_type::clear_mask);
  }
  static __always_inline bool test_from(value_type const val) {
    // assert((val & (clear_mask >> offset)) == val);  /* input value does not match clear_mask */
    return (reg_type::load() & regmask_type::clear_mask) == value_from(val);
  }
  static __always_inline value_type load_and_shift() {
    return (reg_type::load() & regmask_type::clear_mask) >> offset;
  }

  template<unsigned bit_no>
    struct bit : regbits< reg_type, offset + bit_no, 1 > {
    static_assert(bit_no < width, "invalid bit_no");
  };
};


////////////////////  regval  ////////////////////


/** NOTE: _value is shifted with offset of Tp! */
template<
  typename Tp,
  typename Tp::value_type _value
  >
class regval
: public regmask<typename Tp::reg_type, Tp::value_from(_value), Tp::clear_mask>
{
  static_assert(std::is_same<typename Tp::type, typename Tp::regbits_type>::value, "template argument Tp is not of type: regbits<>");

#ifndef CONFIG_USE_STD_TUPLE
  /* private constructor: instantiation would only cause confusion with set/clear functions */
  regval() {};
#endif // CONFIG_USE_STD_TUPLE

  /* NOTE: clear() is declared private. Clearing a contant value in
   * a register does not make sense. What you actually want to do to
   * is clearing all bits (offset, width) where regval<> is
   * defined on. In order to clear the constant, you must refer to
   * the underlying regbits<> type by either accessing it
   * directly (preferred), or use the regbits_type:
   *
   *   regval<>::regbits_type::clear()
   *
   */
  static __always_inline void clear() {
    Tp::regbits_type::clear();
  }

public:
  using type         = regval<Tp, _value>;
  using reg_type     = typename Tp::reg_type;
  using regmask_type = regmask<typename Tp::reg_type, Tp::value_from(_value), Tp::clear_mask>;
  using regbits_type = typename Tp::regbits_type;
  using value_type   = typename Tp::value_type;

  static constexpr value_type value = regmask_type::set_mask;
};


////////////////////  reg  ////////////////////


template<
  typename   Tp,
  reg_addr_t addr,
  reg_perm   permission,
  Tp         _reset_value = 0 >
class reg
: public reg_access<Tp, addr, permission, _reset_value>, public typelist_element
{
#ifndef CONFIG_USE_STD_TUPLE
  /* private constructor: instantiation would only cause confusion with set/clear functions */
  reg() {};
#endif // CONFIG_USE_STD_TUPLE

public:
  using type         = reg<Tp, addr, permission, _reset_value>;
  using reg_type     = type;
  using regbits_type = regbits< type, 0, sizeof(Tp) * 8>;
  using value_type   = Tp;

  /**
   * regmask<> type, with following properties:
   *
   *   - some_regmask<>::merge< neutral_regmask >::type == some_regmask<>::type
   *   - neutral_regmask::reset_to() == reg_type::store(reset_value)
   *
   * Note that having a neutral_regmask element in a reglist<> has the
   * effect of resetting the register on a reglist<>::reset_to() call!
   */
  using neutral_regmask = regmask< reg_type, 0, 0 >;

  static constexpr value_type reset_value = _reset_value;

  static __always_inline value_type test(value_type const value) {
    return type::load() & value;
  }
  static __always_inline void set(value_type const value) {
    type::store( type::load() | value );
  }
  static __always_inline void set(value_type const set_mask, value_type const clear_mask) {
    type::store( (type::load() & ~clear_mask) | set_mask );
  }
  static __always_inline void clear(value_type const value) {
    type::store( type::load() & ~value );
  }
  static __always_inline void mask(value_type const value) {
    type::store( type::load() & value );
  }
  static __always_inline void reset() {
    type::store(reset_value);
  }

  template<typename Rm0, typename... Rm>
  struct merge {
    using type = typename mpl::merged_regmask<Rm0, Rm...>::type;
    static_assert(std::is_same<typename type::reg_type, reg_type>::value, "merged template arguments have different reg<> type");
  };

  /** clear register bits (or'ed clear_mask of regmask Rm) */
  template<typename Rm0, typename... Rm>
  static __always_inline void clear(void) {
    merge<Rm0, Rm...>::type::clear();
  }

  /** set constants */
  template<typename Rm0, typename... Rm>
  static __always_inline void set(void) {
    merge<Rm0, Rm...>::type::set();
  }

  /** set value, masked by clear_mask of regmask (Rm) */
  template<typename Rm0, typename... Rm>
  static __always_inline void set(value_type const value) {
    set(value, merge<Rm0, Rm...>::type::clear_mask);
  }

  /** reset register, and set constants (results in single store()) */
  template<typename... Rm>
  static __always_inline void reset_to(void) {
    /* add a neutral regmask to the merge list, in order to handle empty template arguments correctly */
    using merged_regmask = typename merge<neutral_regmask, Rm...>::type;
    type::store((reset_value & ~merged_regmask::cropped_clear_mask) | merged_regmask::set_mask);
  }
};


////////////////////  reglist  ////////////////////


/**
 * List of regmask<> types. Expands reglist<> types to its elements.
 */
template< typename... Tp >
class reglist
: public typelist< Tp... >
{
  using type = reglist< Tp... >;

  using merged_list = typename type::template map< mpl::map_merged_regmask >;
  using unique_merged_list = typename merged_list::filter_unique::type;

public:

  /**
   * Call ::set() on each distinct merged regmask from reglist.
   *
   * Refer to the "mpl::functor_reg_set" documentation in
   * register_mpl.hpp for a discussion about reset_to() and set().
   */
  static __always_inline void set(void) {
    unique_merged_list::template for_each< mpl::functor_reg_set >();
  }

  /**
   * Call ::clear() on each distinct merged regmask from reglist.
   */
  static __always_inline void clear(void) {
    unique_merged_list::template for_each< mpl::functor_reg_clear >();
  }

#if 0  // TODO: implement 
  static __always_inline void test(void) {
  }
#endif

  /**
   * Call ::reset_to() on each distinct merged regmask from reglist.
   *
   * Refer to the "mpl::functor_reg_reset_to" documentation in
   * register_mpl.hpp for a discussion about reset_to() and set().
   */
  static __always_inline void reset_to(void) {
    unique_merged_list::template for_each< mpl::functor_reg_reset_to >();
  }

  /**
   * Call ::reset_to() on each distinct merged regmask from
   * reglist.
   *
   * Resets regs from strict_reg_type if no regmask of same
   * reg_type is in reglist.
   *
   * Asserts all regmasks in reglist to be of reg_type from any
   * reg in strict_reg_type list.
   */
  template< typename... strict_reg_type >
  static __always_inline void strict_reset_to(void) {
    static_assert(reglist< Tp... >::template all_reg_type< strict_reg_type... >::value,
                  "one or more elements (aka: Tp...) are not of reg_type listed in strict_reg_type");

    /* add neutral regmasks to the list, which enforces these registers to be set in reset_to() call. */
    using strict_list = reglist<
      typename strict_reg_type::neutral_regmask...,
      Tp...
      >;

    strict_list::reset_to();
  }

  /**
   * Type trait providing value=true if every reglist<> element (aka:
   * Tp...) is of same reg_type as one of Treg, or if reglist<>
   * contains no elements.
   */
  template< typename... Treg >
  struct all_reg_type {
    using bool_list = typename type::template map< mpl::map_contains_reg_type< Treg... > >;
    static constexpr bool value = bool_list::all_true::value;
  };

  /**
   * Provides a typelist, whose elements are a filtered subset of all
   * reglist<> elements with underlying reg_type = Treg
   */
  template< typename Treg >
  using filter_reg_type = typename type::template filter< mpl::filter_reg_type< Treg > >::type;

  /**
   * Provides a regmask<> type, merged by all elements in reglist<> of
   * reg_type Treg.
   */
  template< typename Treg >
  using merged_regmask = typename reglist< unique_merged_list >::template filter_reg_type< Treg >::unique_element::type;
};


/**
 * Create a reglist<> from a list of any types Tp, while expanding
 * typelist<> types.
 *
 * Ignores all non-regmask<> types in list (more exact: all types
 * which are not derived from mpl::regmask_tag).
 */
template< typename... Tp >
using make_reglist = reglist< typename typelist< Tp... >::template filter_type< mpl::regmask_tag > >;

} // namespace mptl

#endif // REGISTER_HPP_INCLUDED
