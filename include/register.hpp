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
#include <compiler.h>

namespace mptl {


////////////////////  regmask  ////////////////////


template< typename R,    /* regdef<> type */
          typename R::value_type _set_mask,
          typename R::value_type _clear_mask = _set_mask
          >
class regmask
{
  static_assert(std::is_same<typename R::type, typename R::reg_type>::value, "template argument R is not of type: regdef<>");
  static_assert((_set_mask | _clear_mask) == _clear_mask, "clear_mask does not cover all bits of set_mask");

  /* private constructor: instantiation would only cause confusion with set/clear functions */
  regmask() {};

public:
  using type       = regmask<typename R::reg_type, _set_mask, _clear_mask>;
  using mask_type  = type;
  using reg_type   = typename R::reg_type;
  using value_type = typename R::value_type;

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
   *   x = (x & ~0x1ff00) | 0x0ff00
   *   x = (x & ~0x10000) | 0x0ff00   <-- one BIC instruction less!
   *
   * As long as a load-clear-set-store sequence is performed, the
   * above statements are equivalent, and the cropped_clear_mask can
   * be used.
   */
  static constexpr value_type cropped_clear_mask = clear_mask & ~set_mask;

  static __always_inline void set() {
    if((set_mask != 0) || (clear_mask != 0))  /* evaluated at compile-time */
      reg_type::set(set_mask, cropped_clear_mask);
  }
  static __always_inline void clear() {
    if(clear_mask != 0)  /* evaluated at compile-time */
      reg_type::clear(clear_mask);
  }
  static __always_inline bool test() {
    if(clear_mask != 0)  /* evaluated at compile-time */
      return (reg_type::load() & clear_mask) == set_mask;
    return set_mask == 0;
  }

  template<typename Rm>
  struct combine {
    static_assert(std::is_same<typename Rm::reg_type, reg_type>::value, "template argument is not of same regdef<> type");

    /* assert if we set a bit which was previously cleared (and vice versa) */
    static_assert(!((Rm::set_mask & clear_mask) & (~set_mask)),     "set/clear check failed: setting a bit which was previously cleared leads to undefined behaviour.");
    static_assert(!((set_mask & Rm::clear_mask) & (~Rm::set_mask)), "set/clear check failed: clearing a bit which was previously set leads to undefined behaviour.");

    using type = regmask<reg_type, (set_mask | Rm::set_mask), (clear_mask | Rm::clear_mask)>;
  };
};


////////////////////  regbits  ////////////////////


template< typename R,          /* regdef<> type */
          unsigned offset,     /* bit offset */
          unsigned width = 1   /* number of bits */
          >
class regbits : public regmask<R, ((1ul << width) - 1) << offset>
{
  static_assert(std::is_same<typename R::type, typename R::reg_type>::value, "template argument R is not of type: regdef<>");
  static_assert(width >= 1, "invalid width");
  static_assert(offset + width <= sizeof(typename R::value_type) * 8, "invalid width/offset");

  /* private constructor: instantiation would only cause confusion with set/clear functions */
  regbits() {};

 public:
  using type       = regbits<typename R::reg_type, offset, width>;
  using bits_type  = type;
  using reg_type   = typename R::reg_type;
  using value_type = typename R::value_type;
  using mask_type  = regmask<R, ((1ul << width) - 1) << offset>;  // TODO: should be private/protected

  static constexpr value_type value = mask_type::set_mask;

  static __always_inline constexpr value_type value_from(value_type const val) {
    // assert((val & (clear_mask >> offset)) == val);  /* input value does not match clear_mask */
    return (val << offset);
  }
  /** NOTE: this does not check if _value is masked correctly! */
  static __always_inline void set_from(value_type const val) {
    // assert((val & (clear_mask >> offset)) == val);  /* input value does not match clear_mask */
    reg_type::set(value_from(val), mask_type::clear_mask);
  }
  static __always_inline bool test() /* override */ {
    return (reg_type::load() & mask_type::clear_mask) != 0;
  }
  static __always_inline bool test_from(value_type const val) {
    // assert((val & (clear_mask >> offset)) == val);  /* input value does not match clear_mask */
    return (reg_type::load() & mask_type::clear_mask) == value_from(val);
  }
  static __always_inline value_type get() {
    return (reg_type::load() & mask_type::clear_mask) >> offset;
  }

  template<unsigned bit_no>
    struct bit : regbits< reg_type, offset + bit_no, 1 > {
    static_assert(bit_no < width, "invalid bit_no");
  };
};


////////////////////  regval  ////////////////////


/** NOTE: _value is shifted with offset of R! */
template< typename R, typename R::value_type _value >
class regval : public regmask<typename R::reg_type, R::value_from(_value), R::clear_mask>
{
  static_assert(std::is_same<typename R::type, typename R::bits_type>::value, "template argument R is not of type: regbits<>");

  /* private constructor: instantiation would only cause confusion with set/clear functions */
  regval() {};

  /* NOTE: clear() is declared private. Clearing a contant value in
   * a register does not make sense. What you actually want to do to
   * is clearing all bits (offset, width) where regval<> is
   * defined on. In order to clear the constant, you must refer to
   * the underlying regbits<> type by either accessing it
   * directly (preferred), or use the bits_type:
   *
   *   regval<>::bits_type::clear()
   *
   */
  static __always_inline void clear() {
    R::bits_type::clear();
  }

public:
  using type       = regval<R, _value>;
  using bits_type  = typename R::bits_type;
  using reg_type   = typename R::reg_type;
  using mask_type  = regmask<typename R::reg_type, R::value_from(_value), R::clear_mask>;
  using value_type = typename R::value_type;

  static constexpr value_type value = mask_type::set_mask;
};


////////////////////  regdef  ////////////////////


template< typename   T,
          reg_addr_t addr,
          reg_access access,
          T          _reset_value = 0 >
class regdef : public regdef_backend<T, addr, access, _reset_value>
{
  /* private constructor: instantiation would only cause confusion with set/clear functions */
  regdef() {};

public:
  using type       = regdef<T, addr, access, _reset_value>;
  using bits_type  = regbits< type, 0, sizeof(T) * 8>;
  using reg_type   = type;
  using value_type = T;

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

  template<typename Front, typename... Args>
  struct combined_mask { // TODO: rename "accumulate" (maybe derive from "type" below, so that we can use it without ::type notation)
    static_assert(std::is_same<typename Front::reg_type, reg_type>::value, "template argument is not of same regdef<> type");
    using type = typename Front::template combine< typename combined_mask<Args...>::type >::type;
  };
  template<typename Front>
  struct combined_mask<Front> {
    static_assert(std::is_same<typename Front::reg_type, reg_type>::value, "template argument is not of same regdef<> type");
    using type = Front;
  };

  /* clear register bits (or'ed clear_mask of regmask Rm) */
  template<typename... Rm>
  static __always_inline void clear(void) {
    combined_mask<Rm...>::type::clear();
  }

  /* set constants */
  template<typename... Rm>
  static __always_inline void set(void) {
    combined_mask<Rm...>::type::set();
  }

  /* set value, masked by clear_mask of regmask (Rm) */
  template<typename... Rm>
  static __always_inline void set(value_type const value) {
    set(value, combined_mask<Rm...>::type::clear_mask);
  }

  /* reset register, and set constants (results in single store()) */
  template<typename... Rm>
  static __always_inline void reset_to(void) {
    using Rmc = typename combined_mask<Rm...>::type;
    type::store((reset_value & ~Rmc::cropped_clear_mask) | Rmc::set_mask);
  }
};

} // namespace mptl

#endif // REGISTER_HPP_INCLUDED
