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

#include <core.hpp>
#include <arch/irq.hpp>
#include <arch/pwr.hpp>


class Rtc
{
  using RTC = Core::RTC;

private:
  static void EnterConfigMode(void) {
    RTC::CRL::CNF::set();
  }
  static void ExitConfigMode(void) {
    RTC::CRL::CNF::set();
    WaitConfigDone();
  }
  static void WaitConfigDone(void) {
    while(RTC::CRL::RTOFF::test() == 0);
  }

public:

  typedef Irq<IrqNumber::Interrupt::RTC>      GlobalIrq; /**< RTC global Interrupt                   */
  typedef Irq<IrqNumber::Interrupt::RTCAlarm> AlarmIrq;  /**< RTC Alarm through EXTI Line Interrupt  */


  static void WaitSync(void) {
    while(Core::RCC::BDCR::LSERDY::test() == 0);
#if 1
    RTC::CRL::RSF::clear();
    while(RTC::CRL::RSF::test() == 0);
#endif
  }

  // TODO: enable/disable more than one at once
  static void EnableOverflowInterrupt(void) {
    RTC::CRH::OWIE::set();
  }
  static void EnableAlarmInterrupt(void) {
    RTC::CRH::ALRIE::set();
  }
  static void EnableSecondInterrupt(void) {
    RTC::CRH::SECIE::set();
  }

  static void DisableOverflowInterrupt(void) {
    RTC::CRH::OWIE::clear();
  }
  static void DisableAlarmInterrupt(void) {
    RTC::CRH::ALRIE::clear();
  }
  static void DisableSecondInterrupt(void) {
    RTC::CRH::SECIE::clear();
  }

  static void ClearOverflowFlag(void) {
    RTC::CRL::OWF::clear();
  }
  static void ClearAlarmFlag(void) {
    RTC::CRL::ALRF::clear();
  }
  static void ClearSecondFlag(void) {
    RTC::CRL::SECF::clear();
  }


  static void SetCounter(uint32_t value) { 
    EnterConfigMode();
    RTC::CNTH::store(value >> 16);
    RTC::CNTL::store(value & 0xFFFF);
    ExitConfigMode();
  }

  static void SetPrescaler(uint32_t value) {
    // TODO: assert(value <= 0xFFFFF);
    //TODO: set PRLH subbits
    EnterConfigMode();
    RTC::PRLH::store((value & 0xF0000) >> 16);
    RTC::PRLL::store(value & 0xFFFF);
    ExitConfigMode();
  }

  static void SetAlarm(uint32_t value) {  
    //TODO: set ALRH subbits
    EnterConfigMode();
    RTC::ALRH::store(value >> 16);
    RTC::ALRL::store(value & 0xFFFF);
    ExitConfigMode();
  }

  static uint32_t GetDivider(void) {
    // TODO: better return type
    //    return (((uint32_t)RTC::DIVH::load() & (uint32_t)0x000F) << 16 ) | RTC::DIVL::load();
    return (RTC::DIVH::RTC_DIV::test() << 16 ) | RTC::DIVL::load();
  }

  struct StaticIrqWrap : public IrqWrap {
    StaticIrqWrap() { ClearSecondFlag(); }
  };
  struct AlarmIrqWrap : public IrqWrap {
    AlarmIrqWrap() { ClearAlarmFlag(); }
  };

  /* Power interface clock enable; Backup interface clock enable */
  static constexpr uint32_t apb1enr = Core::RCC::APB1ENR::PWREN::value | Core::RCC::APB1ENR::BKPEN::value;
  typedef ResourceList< SharedAPB1ENR< apb1enr > > resources;

  static void Init(void) {
    /* Disable backup domain write protection */
    Pwr::DisableBackupDomainWriteProtection();

    /* Backup domain software reset */
    Core::RCC::BDCR::BDRST::set();  // TODO: rcc.hpp
    Core::RCC::BDCR::BDRST::clear();  // TODO: rcc.hpp

    /* External Low Speed oscillator enable */
    Core::RCC::BDCR::LSEON::clear();  // TODO: rcc.hpp
    Core::RCC::BDCR::LSEON::set();  // TODO: rcc.hpp

    /* Internal Low Speed oscillator disable */
    Core::RCC::CSR::LSION::clear();

    /* LSE oscillator clock used as RTC clock */
    Core::RCC::BDCR::set(Core::RCC::BDCR::RTCSEL::LSE::value);

    /* RTC clock enable */
    Core::RCC::BDCR::RTCEN::set();

    WaitSync();

    // TODO: no hardcoded stuff!
    /* one second (dependent on clock frequency!) */
    SetPrescaler(0x7FFF);
  }
};


#endif // RTC_HPP_INCLUDED


