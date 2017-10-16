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

#ifndef ARCH_FLASH_HPP_INCLUDED
#define ARCH_FLASH_HPP_INCLUDED

#include <arch/reg/flash.hpp>
#include <freq.hpp>

namespace mptl {

class flash
{
  static constexpr FLASH::ACR::LATENCY::value_type min_latency(freq_t hclk_freq)
  {
    return
      ((hclk_freq <= mhz(24))  ?  0  :
       (hclk_freq <= mhz(48))  ?  1  :
       (hclk_freq <= mhz(72))  ?  2  :
       0xff );
  };

public:  /* ------ configuration traits ------ */

  using prefetch_buffer_enable  = regval< FLASH::ACR::PRFTBE, 1>;
  using prefetch_buffer_disable = regval< FLASH::ACR::PRFTBE, 0>;

  struct latency {
    template< typename system_clock_type >
    using minimum = regval<
      FLASH::ACR::LATENCY,
      min_latency(system_clock_type::hclk_freq)
      >;

    template< unsigned ws >
    using wait_states = regval< FLASH::ACR::LATENCY, ws >;
  };

public:  /* ------ static member functions ------ */

  /**
   * Configure FLASH register using configuration traits (Tp).
   */
  template< typename... Tp >
  static void configure(void) {
    reglist< Tp... >::template strict_reset_to< FLASH::ACR >();
  }
};

} // namespace mptl

#endif // ARCH_FLASH_HPP_INCLUDED
