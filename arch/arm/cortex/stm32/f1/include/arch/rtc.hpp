/*
 * CppCore - C++ microprocessor core library
 *
 * Copyright 2012 Axel Burri <axel@tty0.ch>
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
#include <irq_wrap.hpp>


class Rtc
{
  using RTC = reg::RTC;

private:
  static void enter_config_mode(void) {
    RTC::CRL::CNF::set();
  }
  static void exit_config_mode(void) {
    RTC::CRL::CNF::set();
    wait_config_done();
  }
  static void wait_config_done(void) {
    while(RTC::CRL::RTOFF::test() == 0);
  }

public:

  using GlobalIrq = Irq::RTC;       /**< RTC global Interrupt                   */
  using AlarmIrq  = Irq::RTCAlarm;  /**< RTC Alarm through EXTI Line Interrupt  */

  static void wait_sync(void) {
    while(reg::RCC::BDCR::LSERDY::test() == 0);
#if 1
    RTC::CRL::RSF::clear();
    while(RTC::CRL::RSF::test() == 0);
#endif
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


  static void set_counter(uint32_t value) { 
    enter_config_mode();
    RTC::CNTH::store(value >> 16);
    RTC::CNTL::store(value & 0xFFFF);
    exit_config_mode();
  }

  static void set_prescaler(uint32_t value) {
    // TODO: assert(value <= 0xFFFFF);
    //TODO: set PRLH subbits
    enter_config_mode();
    RTC::PRLH::store((value & 0xF0000) >> 16);
    RTC::PRLL::store(value & 0xFFFF);
    exit_config_mode();
  }

  static void set_alarm(uint32_t value) {  
    //TODO: set ALRH subbits
    enter_config_mode();
    RTC::ALRH::store(value >> 16);
    RTC::ALRL::store(value & 0xFFFF);
    exit_config_mode();
  }

  static uint32_t get_divider(void) {
    // TODO: better return type
    //    return (((uint32_t)RTC::DIVH::load() & (uint32_t)0x000F) << 16 ) | RTC::DIVL::load();
    return (RTC::DIVH::RTC_DIV::test() << 16 ) | RTC::DIVL::load();
  }

  struct StaticIrqWrap : public IrqWrap {
    StaticIrqWrap() { clear_second_flag(); }
  };
  struct AlarmIrqWrap : public IrqWrap {
    AlarmIrqWrap() { clear_alarm_flag(); }
  };

  typedef Rcc::rtc_clock_resources resources;

  static void init(void) {
    /* Disable backup domain write protection */
    Pwr::disable_backup_domain_write_protection();

    /* Backup domain software reset */
    reg::RCC::BDCR::BDRST::set();  // TODO: rcc.hpp
    reg::RCC::BDCR::BDRST::clear();  // TODO: rcc.hpp

    /* External Low Speed oscillator enable */
    reg::RCC::BDCR::LSEON::clear();  // TODO: rcc.hpp
    reg::RCC::BDCR::LSEON::set();  // TODO: rcc.hpp

    /* Internal Low Speed oscillator disable */
    reg::RCC::CSR::LSION::clear();

    /* LSE oscillator clock used as RTC clock */
    reg::RCC::BDCR::set(reg::RCC::BDCR::RTCSEL::LSE::value);

    /* RTC clock enable */
    reg::RCC::BDCR::RTCEN::set();

    wait_sync();

    // TODO: no hardcoded stuff!
    /* one second (dependent on clock frequency!) */
    set_prescaler(0x7FFF);
  }
};


#endif // RTC_HPP_INCLUDED


