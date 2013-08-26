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

#ifndef REGISTER_HPP_INCLUDED
#define REGISTER_HPP_INCLUDED

/*
 * MPL-style Register class.
 *
 * - includes declarations of the regdef<> type for:
 *   - embedded: register writes go to "constexpr volatile" register address
 *   - simulation: register writes go to variable in memory
 * - prohibits writing to read-only registers
 * - prohibits reading from write-only registers
 *
 */

//
//  microprocessor register definition: wrap bitfields into regdef classes
//

//  Pros:
//
//  - uses regdef<> template class to wrap the register access. All type
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
#include <register_backend.hpp>
#include <typelist.hpp>
#include <compiler.h>
#include "register_mpl.hpp"

namespace mptl {

#if 0 // TODO: provide comfort functions (map to _impl without ::type)
template<typename... Tp>
using merged_regmask = mpl::merged_regmask<Tp...>::type;
#endif

////////////////////  regmask  ////////////////////


template<
  typename Tp,    /* regdef<> type */
  typename Tp::value_type _set_mask,
  typename Tp::value_type _clear_mask = _set_mask
  >
class regmask
: public mpl::regmask_base
{
  static_assert(std::is_same<typename Tp::type, typename Tp::regdef_type>::value, "template argument Tp is not of type: regdef<>");
  static_assert((_set_mask | _clear_mask) == _clear_mask, "clear_mask does not cover all bits of set_mask");

#ifndef CONFIG_USE_STD_TUPLE
  /* private constructor: instantiation would only cause confusion with set/clear functions */
  regmask() {};
#endif // CONFIG_USE_STD_TUPLE

public:
  using type         = regmask<typename Tp::regdef_type, _set_mask, _clear_mask>;
  using regdef_type  = typename Tp::regdef_type;
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

  static __always_inline void set() {
    // TODO: improvement: check for clear_mask covering ALL bits of
    // our regdef_type. if yes, use store() instead!
    if((set_mask != 0) || (clear_mask != 0))  /* evaluated at compile-time */
      regdef_type::set(set_mask, cropped_clear_mask);
  }
  static __always_inline void clear() {
    if(clear_mask != 0)  /* evaluated at compile-time */
      regdef_type::clear(clear_mask);
  }
  static __always_inline bool test() {
    if(clear_mask != 0)  /* evaluated at compile-time */
      return (regdef_type::load() & clear_mask) == set_mask;
    return set_mask == 0;
  }

  /** reset register, combined with set/clear mask (results in single store()) */
  static __always_inline void reset_to(void) {
    regdef_type::store((regdef_type::reset_value & ~cropped_clear_mask) | set_mask);
  }

  /**
   * Merge set_mask and clear_mask with the values of another
   * regmask<> type Up.
   */
  template<typename Rm>
  struct merge {
    static_assert(std::is_same<typename Rm::regdef_type, regdef_type>::value, "template argument is not of same regdef<> type");

    /* assert if we set a bit which was previously cleared (and vice versa) */
    static_assert(!((Rm::set_mask & clear_mask) & (~set_mask)),     "set/clear check failed: setting a bit which was previously cleared");
    static_assert(!((set_mask & Rm::clear_mask) & (~Rm::set_mask)), "set/clear check failed: clearing a bit which was previously set");

    using type = regmask<regdef_type, (set_mask | Rm::set_mask), (clear_mask | Rm::clear_mask)>;
  };
};


////////////////////  regbits  ////////////////////


template<
  typename Tp,         /* regdef<> type */
  unsigned offset,     /* bit offset */
  unsigned width = 1   /* number of bits */
  >
class regbits
: public regmask<Tp, ((1ul << width) - 1) << offset>
{
  static_assert(std::is_same<typename Tp::type, typename Tp::regdef_type>::value, "template argument Tp is not of type: regdef<>");
  static_assert(width >= 1, "invalid width");
  static_assert(offset + width <= sizeof(typename Tp::value_type) * 8, "invalid width/offset");

#ifndef CONFIG_USE_STD_TUPLE
  /* private constructor: instantiation would only cause confusion with set/clear functions */
  regbits() {};
#endif // CONFIG_USE_STD_TUPLE

 public:
  using type         = regbits<typename Tp::regdef_type, offset, width>;
  using regdef_type  = typename Tp::regdef_type;
  using regmask_type = regmask<Tp, ((1ul << width) - 1) << offset>;
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
    regdef_type::set(value_from(val), regmask_type::clear_mask);
  }
  static __always_inline bool test() /* override */ {
    return (regdef_type::load() & regmask_type::clear_mask) != 0;
  }
  static __always_inline bool test_from(value_type const val) {
    // assert((val & (clear_mask >> offset)) == val);  /* input value does not match clear_mask */
    return (regdef_type::load() & regmask_type::clear_mask) == value_from(val);
  }
  static __always_inline value_type get() {
    return (regdef_type::load() & regmask_type::clear_mask) >> offset;
  }

  template<unsigned bit_no>
    struct bit : regbits< regdef_type, offset + bit_no, 1 > {
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
: public regmask<typename Tp::regdef_type, Tp::value_from(_value), Tp::clear_mask>
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
  using regdef_type  = typename Tp::regdef_type;
  using regmask_type = regmask<typename Tp::regdef_type, Tp::value_from(_value), Tp::clear_mask>;
  using regbits_type = typename Tp::regbits_type;
  using value_type  = typename Tp::value_type;

  static constexpr value_type value = regmask_type::set_mask;
};


////////////////////  regdef  ////////////////////


template<
  typename   Tp,
  reg_addr_t addr,
  reg_access access,
  Tp         _reset_value = 0 >
class regdef
: public regdef_backend<Tp, addr, access, _reset_value>, public typelist_element
{
#ifndef CONFIG_USE_STD_TUPLE
  /* private constructor: instantiation would only cause confusion with set/clear functions */
  regdef() {};
#endif // CONFIG_USE_STD_TUPLE

public:
  using type         = regdef<Tp, addr, access, _reset_value>;
  using regdef_type  = type;
  using regbits_type = regbits< type, 0, sizeof(Tp) * 8>;
  using value_type   = Tp;

  /**
   * regmask<> type, with following properties:
   *
   *   - some_regmask<>::merge< neutral_regmask >::type == some_regmask<>::type
   *   - neutral_regmask::reset_to() == regdef_type::store(reset_value)
   *
   * Note that having a neutral_regmask element in a reglist<> has the
   * effect of resetting the register on a reglist<>::reset_to() call!
   */
  using neutral_regmask = regmask< regdef_type, 0, 0 >;

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
    static_assert(std::is_same<typename type::regdef_type, regdef_type>::value, "merged template arguments have different regdef<> type");
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
   * Call regdef::set() on each distinct merged regmask from reglist.
   *
   * Refer to the "mpl::functor_reg_set" documentation in
   * register_mpl.hpp for a discussion about reset_to() and set().
   */
  static __always_inline void set(void) {
    unique_merged_list::template for_each< mpl::functor_reg_set >();
  }

  /**
   * Call regdef::reset_to() on each distinct merged regmask from reglist.
   *
   * Refer to the "mpl::functor_reg_reset_to" documentation in
   * register_mpl.hpp for a discussion about reset_to() and set().
   */
  static __always_inline void reset_to(void) {
    unique_merged_list::template for_each< mpl::functor_reg_reset_to >();
  }

  /**
   * Call regdef::reset_to() on each distinct merged regmask from
   * reglist.
   *
   * Resets regdefs from strict_regdef_type if no regmask of same
   * regdef_type is in reglist.
   *
   * Asserts all regmasks in reglist to be of regdef_type from any
   * regdef in strict_regdef_type list.
   */
  template< typename... strict_regdef_type >
  static __always_inline void strict_reset_to(void) {
    static_assert(reglist< Tp... >::template all_regdef_type< strict_regdef_type... >::value,
                  "one or more elements (aka: Tp...) are not of regdef_type listed in strict_regdef_type");

    /* add neutral regmasks to the list, which enforces these registers to be set in reset_to() call. */
    using strict_list = reglist<
      typename strict_regdef_type::neutral_regmask...,
      Tp...
      >;

    strict_list::reset_to();
  }

  /**
   * Type trait providing value=true if every reglist<> element (aka:
   * Tp...) is of same regdef_type as one of Tregdef, or if reglist<>
   * contains no elements.
   */
  template< typename... Tregdef >
  struct all_regdef_type {
    using bool_list = typename type::template map< mpl::map_contains_regdef_type< Tregdef... > >;
    static constexpr bool value = bool_list::all_true::value;
  };

  /**
   * Provides a typelist, whose elements are a filtered subset of all
   * reglist<> elements with underlying regdef_type = Tregdef
   */
  template< typename Tregdef >
  using filter_regdef_type = typename type::template filter< mpl::filter_regdef_type< Tregdef > >::type;

  /**
   * Provides a regmask<> type, merged by all elements in reglist<> of
   * regdef_type Tregdef.
   */
  template< typename Tregdef >
  using merged_regmask = typename reglist< unique_merged_list >::template filter_regdef_type< Tregdef >::unique_element::type;
};


/**
 * Create a reglist<> from a list of any types Tp, while expanding
 * typelist<> types.
 *
 * Ignores all non-regmask<> types in list (more exact: all types
 * which are not derived from mpl::regmask_base).
 */
template< typename... Tp >
using make_reglist = reglist< typename typelist< Tp... >::template filter_type< mpl::regmask_base > >;

} // namespace mptl

#endif // REGISTER_HPP_INCLUDED
