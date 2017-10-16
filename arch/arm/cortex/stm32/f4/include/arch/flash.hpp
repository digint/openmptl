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
#include <voltage.hpp>

namespace mptl {

class flash
{
  template< typename pwr_type >
  struct prefetch_buffer_enable_impl {
    static_assert(pwr_type::system_voltage >= volt(2.1), "prefetch buffer must be disabled when the supply voltage is below 2.1V");
    using type = regval< FLASH::ACR::PRFTEN, 1>;
  };

  static constexpr FLASH::ACR::LATENCY::value_type min_latency(freq_t hclk_freq, voltage_t system_voltage)
  {
    return
      (system_voltage <= volt(2.1)) ? 
      ((hclk_freq <= mhz(20))  ?  0  :
       (hclk_freq <= mhz(40))  ?  1  :
       (hclk_freq <= mhz(60))  ?  2  :
       (hclk_freq <= mhz(80))  ?  3  :
       (hclk_freq <= mhz(100)) ?  4  :
       (hclk_freq <= mhz(120)) ?  5  :
       (hclk_freq <= mhz(140)) ?  6  :
       (hclk_freq <= mhz(160)) ?  7  :
       0xff ) :
      (system_voltage <= volt(2.4)) ?
      ((hclk_freq <= mhz(22))  ?  0  :
       (hclk_freq <= mhz(44))  ?  1  :
       (hclk_freq <= mhz(66))  ?  2  :
       (hclk_freq <= mhz(88))  ?  3  :
       (hclk_freq <= mhz(110)) ?  4  :
       (hclk_freq <= mhz(132)) ?  5  :
       (hclk_freq <= mhz(154)) ?  6  :
       (hclk_freq <= mhz(168)) ?  7  :
       0xff ) :
      (system_voltage <= volt(2.7)) ?
      ((hclk_freq <= mhz(24))  ?  0  :
       (hclk_freq <= mhz(48))  ?  1  :
       (hclk_freq <= mhz(72))  ?  2  :
       (hclk_freq <= mhz(96))  ?  3  :
       (hclk_freq <= mhz(120)) ?  4  :
       (hclk_freq <= mhz(144)) ?  5  :
       (hclk_freq <= mhz(168)) ?  6  :
       0xff ) :
      (system_voltage <= volt(3.6)) ?
      ((hclk_freq <= mhz(30))  ?  0  :
       (hclk_freq <= mhz(60))  ?  1  :
       (hclk_freq <= mhz(90))  ?  2  :
       (hclk_freq <= mhz(120)) ?  3  :
       (hclk_freq <= mhz(150)) ?  4  :
       (hclk_freq <= mhz(168)) ?  5  :
       0xff ) : 0xff;
  };

public:  /* ------ configuration traits ------ */

  template< typename pwr_type >
  using prefetch_buffer_enable  = typename prefetch_buffer_enable_impl< pwr_type >::type;

  using prefetch_buffer_disable = regval< FLASH::ACR::PRFTEN, 0>;

  using instruction_cache_enable  = regval< FLASH::ACR::ICEN, 1>;
  using instruction_cache_disable = regval< FLASH::ACR::ICEN, 0>;

  using data_cache_enable  = regval< FLASH::ACR::DCEN, 1>;
  using data_cache_disable = regval< FLASH::ACR::DCEN, 0>;

  struct latency {
    template< typename system_clock_type, typename pwr_type >
    using minimum = regval<
      FLASH::ACR::LATENCY,
      min_latency(system_clock_type::hclk_freq, pwr_type::system_voltage)
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


