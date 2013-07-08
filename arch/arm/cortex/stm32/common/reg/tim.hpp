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

#ifndef ARM_CORTEX_STM32_COMMON_REG_TIM_HPP_INCLUDED
#define ARM_CORTEX_STM32_COMMON_REG_TIM_HPP_INCLUDED

#include <register.hpp>

namespace mptl { namespace reg {

/**
 * Advanced timer
 *
 * Note that the registers are actually only 16bit wide, but accessing
 * them with 32bit is faster in general.
 *
 * NOTE: not all register bits are valid for all timer, especially:
 * - General-porpose timers (TIM2 to TIM5) are a subset of "advanced timer"
 * - General-porpose timers (TIM9 to TIM14) are another subset of "advanced timer"
 * - Basic timers (TIM6, TIM7) are another subset of "advanced timer"
 *
 * For simplicity reasons, they are not defined separately.
 * Check the reference manual for details about which register bits
 * are valid for the timers you use.
 */
template<reg_addr_t _base_addr>
struct TIM_common
{
  static constexpr reg_addr_t base_addr = _base_addr;

  /**
   * Control register 1
   */
  struct CR1
  : public regdef< std::uint_fast16_t, base_addr + 0x0, reg_access::rw, 0x0000 >
  {
    using type = regdef< std::uint_fast16_t, base_addr + 0x0, reg_access::rw, 0x0000 >;

    using CKD   = regbits< type,  8,  2 >;  /**< Clock division                 */
    using ARPE  = regbits< type,  7,  1 >;  /**< Auto-reload preload enable     */
    using CMS   = regbits< type,  5,  2 >;  /**< Center-aligned mode selection  */
    using DIR   = regbits< type,  4,  1 >;  /**< Direction                      */
    using OPM   = regbits< type,  3,  1 >;  /**< One-pulse mode                 */
    using URS   = regbits< type,  2,  1 >;  /**< Update request source          */
    using UDIS  = regbits< type,  1,  1 >;  /**< Update disable                 */
    using CEN   = regbits< type,  0,  1 >;  /**< Counter enable                 */
  };

  /**
   * Control register 2
   */
  struct CR2
  : public regdef< std::uint_fast16_t, base_addr + 0x4, reg_access::rw, 0x0000 >
  {
    using type = regdef< std::uint_fast16_t, base_addr + 0x4, reg_access::rw, 0x0000 >;

    using OIS4   = regbits< type, 14,  1 >;  /**< Output Idle state 4                       */
    using OIS3N  = regbits< type, 13,  1 >;  /**< Output Idle state 3                       */
    using OIS3   = regbits< type, 12,  1 >;  /**< Output Idle state 3                       */
    using OIS2N  = regbits< type, 11,  1 >;  /**< Output Idle state 2                       */
    using OIS2   = regbits< type, 10,  1 >;  /**< Output Idle state 2                       */
    using OIS1N  = regbits< type,  9,  1 >;  /**< Output Idle state 1                       */
    using OIS1   = regbits< type,  8,  1 >;  /**< Output Idle state 1                       */
    using TI1S   = regbits< type,  7,  1 >;  /**< TI1 selection                             */
    using MMS    = regbits< type,  4,  3 >;  /**< Master mode selection                     */
    using CCDS   = regbits< type,  3,  1 >;  /**< Capture/compare DMA selection             */
    using CCUS   = regbits< type,  2,  1 >;  /**< Capture/compare control update selection  */
    using CCPC   = regbits< type,  0,  1 >;  /**< Capture/compare preloaded control         */
  };

  /**
   * Slave mode control register
   */
  struct SMCR
  : public regdef< std::uint_fast16_t, base_addr + 0x8, reg_access::rw, 0x0000 >
  {
    using type = regdef< std::uint_fast16_t, base_addr + 0x8, reg_access::rw, 0x0000 >;

    using ETP   = regbits< type, 15,  1 >;  /**< External trigger polarity   */
    using ECE   = regbits< type, 14,  1 >;  /**< External clock enable       */
    using ETPS  = regbits< type, 12,  2 >;  /**< External trigger prescaler  */
    using ETF   = regbits< type,  8,  4 >;  /**< External trigger filter     */
    using MSM   = regbits< type,  7,  1 >;  /**< Master/Slave mode           */
    using TS    = regbits< type,  4,  3 >;  /**< Trigger selection           */
    using SMS   = regbits< type,  0,  3 >;  /**< Slave mode selection        */
  };

  /**
   * DMA/Interrupt enable register
   */
  struct DIER
  : public regdef< std::uint_fast16_t, base_addr + 0xc, reg_access::rw, 0x0000 >
  {
    using type = regdef< std::uint_fast16_t, base_addr + 0xc, reg_access::rw, 0x0000 >;

    using TDE    = regbits< type, 14,  1 >;  /**< Trigger DMA request enable            */
    using COMDE  = regbits< type, 13,  1 >;  /**< COM DMA request enable                */
    using CC4DE  = regbits< type, 12,  1 >;  /**< Capture/Compare 4 DMA request enable  */
    using CC3DE  = regbits< type, 11,  1 >;  /**< Capture/Compare 3 DMA request enable  */
    using CC2DE  = regbits< type, 10,  1 >;  /**< Capture/Compare 2 DMA request enable  */
    using CC1DE  = regbits< type,  9,  1 >;  /**< Capture/Compare 1 DMA request enable  */
    using UDE    = regbits< type,  8,  1 >;  /**< Update DMA request enable             */
    using TIE    = regbits< type,  6,  1 >;  /**< Trigger interrupt enable              */
    using CC4IE  = regbits< type,  4,  1 >;  /**< Capture/Compare 4 interrupt enable    */
    using CC3IE  = regbits< type,  3,  1 >;  /**< Capture/Compare 3 interrupt enable    */
    using CC2IE  = regbits< type,  2,  1 >;  /**< Capture/Compare 2 interrupt enable    */
    using CC1IE  = regbits< type,  1,  1 >;  /**< Capture/Compare 1 interrupt enable    */
    using UIE    = regbits< type,  0,  1 >;  /**< Update interrupt enable               */
    using BIE    = regbits< type,  7,  1 >;  /**< Break interrupt enable                */
    using COMIE  = regbits< type,  5,  1 >;  /**< COM interrupt enable                  */
  };

  /**
   * Status register
   */
  struct SR
  : public regdef< std::uint_fast16_t, base_addr + 0x10, reg_access::rw, 0x0000 >
  {
    using type = regdef< std::uint_fast16_t, base_addr + 0x10, reg_access::rw, 0x0000 >;

    using CC4OF  = regbits< type, 12,  1 >;  /**< Capture/Compare 4 overcapture flag  */
    using CC3OF  = regbits< type, 11,  1 >;  /**< Capture/Compare 3 overcapture flag  */
    using CC2OF  = regbits< type, 10,  1 >;  /**< Capture/compare 2 overcapture flag  */
    using CC1OF  = regbits< type,  9,  1 >;  /**< Capture/Compare 1 overcapture flag  */
    using BIF    = regbits< type,  7,  1 >;  /**< Break interrupt flag                */
    using TIF    = regbits< type,  6,  1 >;  /**< Trigger interrupt flag              */
    using COMIF  = regbits< type,  5,  1 >;  /**< COM interrupt flag                  */
    using CC4IF  = regbits< type,  4,  1 >;  /**< Capture/Compare 4 interrupt flag    */
    using CC3IF  = regbits< type,  3,  1 >;  /**< Capture/Compare 3 interrupt flag    */
    using CC2IF  = regbits< type,  2,  1 >;  /**< Capture/Compare 2 interrupt flag    */
    using CC1IF  = regbits< type,  1,  1 >;  /**< Capture/compare 1 interrupt flag    */
    using UIF    = regbits< type,  0,  1 >;  /**< Update interrupt flag               */
  };

  /**
   * Event generation register
   */
  struct EGR
  : public regdef< std::uint_fast16_t, base_addr + 0x14, reg_access::wo, 0x0000 >
  {
    using type = regdef< std::uint_fast16_t, base_addr + 0x14, reg_access::wo, 0x0000 >;

    using BG    = regbits< type,  7,  1 >;  /**< Break generation                           */
    using TG    = regbits< type,  6,  1 >;  /**< Trigger generation                         */
    using COMG  = regbits< type,  5,  1 >;  /**< Capture/Compare control update generation  */
    using CC4G  = regbits< type,  4,  1 >;  /**< Capture/compare 4 generation               */
    using CC3G  = regbits< type,  3,  1 >;  /**< Capture/compare 3 generation               */
    using CC2G  = regbits< type,  2,  1 >;  /**< Capture/compare 2 generation               */
    using CC1G  = regbits< type,  1,  1 >;  /**< Capture/compare 1 generation               */
    using UG    = regbits< type,  0,  1 >;  /**< Update generation                          */
  };

  /**
   * Capture/compare mode register 1 (output mode)
   */
  struct CCMR1_Output
  : public regdef< std::uint_fast16_t, base_addr + 0x18, reg_access::rw, 0x0000 >
  {
    using type = regdef< std::uint_fast16_t, base_addr + 0x18, reg_access::rw, 0x0000 >;

    using OC2CE  = regbits< type, 15,  1 >;  /**< Output Compare 2 clear enable    */
    using OC2M   = regbits< type, 12,  3 >;  /**< Output Compare 2 mode            */
    using OC2PE  = regbits< type, 11,  1 >;  /**< Output Compare 2 preload enable  */
    using OC2FE  = regbits< type, 10,  1 >;  /**< Output Compare 2 fast enable     */
    using CC2S   = regbits< type,  8,  2 >;  /**< Capture/Compare 2 selection      */
    using OC1CE  = regbits< type,  7,  1 >;  /**< Output Compare 1 clear enable    */
    using OC1M   = regbits< type,  4,  3 >;  /**< Output Compare 1 mode            */
    using OC1PE  = regbits< type,  3,  1 >;  /**< Output Compare 1 preload enable  */
    using OC1FE  = regbits< type,  2,  1 >;  /**< Output Compare 1 fast enable     */
    using CC1S   = regbits< type,  0,  2 >;  /**< Capture/Compare 1 selection      */
  };

  /**
   * Capture/compare mode register 1 (input mode)
   */
  struct CCMR1_Input
  : public regdef< std::uint_fast16_t, base_addr + 0x18, reg_access::rw, 0x0000 >
  {
    using type = regdef< std::uint_fast16_t, base_addr + 0x18, reg_access::rw, 0x0000 >;

    using IC2F    = regbits< type, 12,  4 >;  /**< Input capture 2 filter       */
    using IC2PSC  = regbits< type, 10,  2 >;  /**< Input capture 2 prescaler    */
    using CC2S    = regbits< type,  8,  2 >;  /**< Capture/Compare 2 selection  */
    using IC1F    = regbits< type,  4,  4 >;  /**< Input capture 1 filter       */
    using IC1PSC  = regbits< type,  2,  2 >;  /**< Input capture 1 prescaler    */
    using CC1S    = regbits< type,  0,  2 >;  /**< Capture/Compare 1 selection  */
  };

  /**
   * Capture/compare mode register 2 (output mode)
   */
  struct CCMR2_Output
  : public regdef< std::uint_fast16_t, base_addr + 0x1c, reg_access::rw, 0x0000 >
  {
    using type = regdef< std::uint_fast16_t, base_addr + 0x1c, reg_access::rw, 0x0000 >;

    using OC4CE  = regbits< type, 15,  1 >;  /**< Output compare 4 clear enable    */
    using OC4M   = regbits< type, 12,  3 >;  /**< Output compare 4 mode            */
    using OC4PE  = regbits< type, 11,  1 >;  /**< Output compare 4 preload enable  */
    using OC4FE  = regbits< type, 10,  1 >;  /**< Output compare 4 fast enable     */
    using CC4S   = regbits< type,  8,  2 >;  /**< Capture/Compare 4 selection      */
    using OC3CE  = regbits< type,  7,  1 >;  /**< Output compare 3 clear enable    */
    using OC3M   = regbits< type,  4,  3 >;  /**< Output compare 3 mode            */
    using OC3PE  = regbits< type,  3,  1 >;  /**< Output compare 3 preload enable  */
    using OC3FE  = regbits< type,  2,  1 >;  /**< Output compare 3 fast enable     */
    using CC3S   = regbits< type,  0,  2 >;  /**< Capture/Compare 3 selection      */
  };

  /**
   * Capture/compare mode register 2 (input mode)
   */
  struct CCMR2_Input
  : public regdef< std::uint_fast16_t, base_addr + 0x1c, reg_access::rw, 0x0000 >
  {
    using type = regdef< std::uint_fast16_t, base_addr + 0x1c, reg_access::rw, 0x0000 >;

    using IC4F    = regbits< type, 12,  4 >;  /**< Input capture 4 filter       */
    using IC4PSC  = regbits< type, 10,  2 >;  /**< Input capture 4 prescaler    */
    using CC4S    = regbits< type,  8,  2 >;  /**< Capture/Compare 4 selection  */
    using IC3F    = regbits< type,  4,  4 >;  /**< Input capture 3 filter       */
    using IC3PSC  = regbits< type,  2,  2 >;  /**< Input capture 3 prescaler    */
    using CC3S    = regbits< type,  0,  2 >;  /**< Capture/compare 3 selection  */
  };

  /**
   * Capture/compare enable register
   */
  struct CCER
  : public regdef< std::uint_fast16_t, base_addr + 0x20, reg_access::rw, 0x0000 >
  {
    using type = regdef< std::uint_fast16_t, base_addr + 0x20, reg_access::rw, 0x0000 >;

    using CC4P   = regbits< type, 13,  1 >;  /**< Capture/Compare 3 output Polarity              */
    using CC4E   = regbits< type, 12,  1 >;  /**< Capture/Compare 4 output enable                */
    using CC3NP  = regbits< type, 11,  1 >;  /**< Capture/Compare 3 output Polarity              */
    using CC3NE  = regbits< type, 10,  1 >;  /**< Capture/Compare 3 complementary output enable  */
    using CC3P   = regbits< type,  9,  1 >;  /**< Capture/Compare 3 output Polarity              */
    using CC3E   = regbits< type,  8,  1 >;  /**< Capture/Compare 3 output enable                */
    using CC2NP  = regbits< type,  7,  1 >;  /**< Capture/Compare 2 output Polarity              */
    using CC2NE  = regbits< type,  6,  1 >;  /**< Capture/Compare 2 complementary output enable  */
    using CC2P   = regbits< type,  5,  1 >;  /**< Capture/Compare 2 output Polarity              */
    using CC2E   = regbits< type,  4,  1 >;  /**< Capture/Compare 2 output enable                */
    using CC1NP  = regbits< type,  3,  1 >;  /**< Capture/Compare 1 output Polarity              */
    using CC1NE  = regbits< type,  2,  1 >;  /**< Capture/Compare 1 complementary output enable  */
    using CC1P   = regbits< type,  1,  1 >;  /**< Capture/Compare 1 output Polarity              */
    using CC1E   = regbits< type,  0,  1 >;  /**< Capture/Compare 1 output enable                */
  };

  /**
   * Counter
   */
  using CNT = regdef< std::uint_fast16_t, base_addr + 0x24, reg_access::rw, 0x0000 >;

  /**
   * Prescaler
   */
  using PSC = regdef< std::uint_fast16_t, base_addr + 0x28, reg_access::rw, 0x0000 >;

  /**
   * Auto-reload register
   */
  using ARR = regdef< std::uint_fast16_t, base_addr + 0x2c, reg_access::rw, 0x0000 >;

  /**
   * Capture/compare register 1
   */
  using CCR1 = regdef< std::uint_fast16_t, base_addr + 0x34, reg_access::rw, 0x0000 >;

  /**
   * Capture/compare register 2
   */
  using CCR2 = regdef< std::uint_fast16_t, base_addr + 0x38, reg_access::rw, 0x0000 >;

  /**
   * Capture/compare register 3
   */
  using CCR3 = regdef< std::uint_fast16_t, base_addr + 0x3c, reg_access::rw, 0x0000 >;

  /**
   * Capture/compare register 4
   */
  using CCR4 = regdef< std::uint_fast16_t, base_addr + 0x40, reg_access::rw, 0x0000 >;

  /**
   * DMA control register
   */
  struct DCR
  : public regdef< std::uint_fast16_t, base_addr + 0x48, reg_access::rw, 0x0000 >
  {
    using type = regdef< std::uint_fast16_t, base_addr + 0x48, reg_access::rw, 0x0000 >;

    using DBL  = regbits< type,  8,  5 >;  /**< DMA burst length  */
    using DBA  = regbits< type,  0,  5 >;  /**< DMA base address  */
  };

  /**
   * DMA address for full transfer
   */
  using DMAR = regdef< std::uint_fast16_t, base_addr + 0x4c, reg_access::rw, 0x0000 >;

  /**
   * Repetition counter register
   */
  struct RCR
  : public regdef< std::uint_fast16_t, base_addr + 0x30, reg_access::rw, 0x0000 >
  {
    using type = regdef< std::uint_fast16_t, base_addr + 0x30, reg_access::rw, 0x0000 >;

    using REP  = regbits< type,  0,  8 >;  /**< Repetition counter value  */
  };

  /**
   * Break and dead-time register
   */
  struct BDTR
  : public regdef< std::uint_fast16_t, base_addr + 0x44, reg_access::rw, 0x0000 >
  {
    using type = regdef< std::uint_fast16_t, base_addr + 0x44, reg_access::rw, 0x0000 >;

    using MOE   = regbits< type, 15,  1 >;  /**< Main output enable                 */
    using AOE   = regbits< type, 14,  1 >;  /**< Automatic output enable            */
    using BKP   = regbits< type, 13,  1 >;  /**< Break polarity                     */
    using BKE   = regbits< type, 12,  1 >;  /**< Break enable                       */
    using OSSR  = regbits< type, 11,  1 >;  /**< Off-state selection for Run mode   */
    using OSSI  = regbits< type, 10,  1 >;  /**< Off-state selection for Idle mode  */
    using LOCK  = regbits< type,  8,  2 >;  /**< Lock configuration                 */
    using DTG   = regbits< type,  0,  8 >;  /**< Dead-time generator setup          */
  };
};

} } // namespace mptl::reg

#endif // ARM_CORTEX_STM32_COMMON_REG_TIM_HPP_INCLUDED
