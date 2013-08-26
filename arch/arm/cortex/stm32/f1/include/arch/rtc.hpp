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

#ifndef RTC_HPP_INCLUDED
#define RTC_HPP_INCLUDED

#include <arch/nvic.hpp>
#include <arch/pwr.hpp>
#include <arch/rcc.hpp>
#include <arch/reg/rtc.hpp>
#include <isr_wrap.hpp>

namespace mptl {

template< freq_t rtcclk_freq = khz(0x8000) >  /* LSE clock frequency */
class rtc
{
  using RTC = reg::RTC;

private:
  static void enter_config_mode(void) {
    wait_config_done();
    RTC::CRL::CNF::set();
  }
  static void exit_config_mode(void) {
    RTC::CRL::CNF::clear();
    wait_config_done();
  }
  static void wait_config_done(void) {
    while(RTC::CRL::RTOFF::test() == false);
  }

public:

  using irq_global = irq::rtc;        /**< RTC global Interrupt                   */
  using irq_alarm  = irq::rtc_alarm;  /**< RTC Alarm through EXTI Line Interrupt  */

  static void wait_sync(void) {
    while(reg::RCC::BDCR::LSERDY::test() == false);
    RTC::CRL::RSF::clear();
    while(RTC::CRL::RSF::test() == false);
  }

  // TODO: enable/disable more than one at once
  static void enable_overflow_interrupt(void) {
    RTC::CRH::OWIE::set();
  }
  static void enable_alarm_interrupt(void) {
    RTC::CRH::ALRIE::set();
  }
  static void enable_second_interrupt(void) {
    RTC::CRH::SECIE::set();
  }

  static void disable_overflow_interrupt(void) {
    RTC::CRH::OWIE::clear();
  }
  static void disable_alarm_interrupt(void) {
    RTC::CRH::ALRIE::clear();
  }
  static void disable_second_interrupt(void) {
    RTC::CRH::SECIE::clear();
  }

  static void clear_overflow_flag(void) {
    RTC::CRL::OWF::clear();
  }
  static void clear_alarm_flag(void) {
    RTC::CRL::ALRF::clear();
  }
  static void clear_second_flag(void) {
    RTC::CRL::SECF::clear();
  }

  static uint32_t get_counter(void) {
    return (((uint32_t)RTC::CNTH::load() << 16) | (uint32_t)RTC::CNTL::load());
  }

  static void set_counter(uint32_t value) {
    enter_config_mode();
    RTC::CNTH::store(value >> 16);
    RTC::CNTL::store(value & 0xFFFF);
    exit_config_mode();
  }

  static void set_prescaler(uint32_t value) {
    enter_config_mode();
    RTC::PRLH::store((value & 0xF0000) >> 16);
    RTC::PRLL::store(value & 0xFFFF);
    exit_config_mode();
  }

  template< freq_t tr_clk_freq >
  static void set_time_base(void) {
    static constexpr unsigned prescaler_value = (rtcclk_freq / ( tr_clk_freq * khz(1) )) - 1;
    set_prescaler(prescaler_value);
  }

  static void set_alarm(uint32_t value) {
    enter_config_mode();
    RTC::ALRH::store(value >> 16);
    RTC::ALRL::store(value & 0xFFFF);
    exit_config_mode();
  }

  static uint32_t get_divider(void) {
    return ((uint32_t)RTC::DIVH::load() << 16 ) | (uint32_t)RTC::DIVL::load();

    /* paranoid variant: applies bitmask on high/low register before shifting */
    // return ((uint32_t)RTC::DIVH::regbits_type::test() << 16 ) | (uint32_t)RTC::DIVL::bits_type::test();
  }

  struct static_isr_wrap : public isr_wrap {
    static_isr_wrap() { clear_second_flag(); }
  };
  struct alarm_isr_wrap : public isr_wrap {
    alarm_isr_wrap() { clear_alarm_flag(); }
  };

  using resources = rcc_rtc_clock_resources;

  template< freq_t tr_clk_freq = hz(1) >
  static void init(void) {
    /* Disable backup domain write protection */
    pwr::disable_backup_domain_write_protection();

    /* Backup domain software reset */
    reg::RCC::BDCR::BDRST::set();  // TODO: rcc.hpp
    reg::RCC::BDCR::BDRST::clear();  // TODO: rcc.hpp

    /* External low speed oscillator enable */
    reg::RCC::BDCR::LSEON::clear();  // TODO: rcc.hpp
    reg::RCC::BDCR::LSEON::set();  // TODO: rcc.hpp

    /* Internal low speed oscillator disable */
    reg::RCC::CSR::LSION::clear();

    /* LSE oscillator clock used as RTC clock (hardcoded for now) */
    reg::RCC::BDCR::set(reg::RCC::BDCR::RTCSEL::LSE::value);

    /* RTC clock enable */
    reg::RCC::BDCR::RTCEN::set();

    wait_sync();

    /* set prescaler */
    set_time_base<tr_clk_freq>();
  }
};

} // namespace mptl

#endif // RTC_HPP_INCLUDED
