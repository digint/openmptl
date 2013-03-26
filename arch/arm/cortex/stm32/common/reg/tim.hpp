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

#ifndef STM32_COMMON_REG_TIM_HPP_INCLUDED
#define STM32_COMMON_REG_TIM_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /**
   * Advanced timer
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
  template<reg_addr_t base_addr>
  struct __TIM_COMMON
  {
    /**
     * control register 1
     */
    struct CR1
    : public Register< uint32_t, base_addr + 0x0, Access::rw, 0x0000 >
    {
      using type = Register< uint32_t, base_addr + 0x0, Access::rw, 0x0000 >;

      using CKD   = RegisterBits< type,  8,  2 >;  /**< Clock division                 */
      using ARPE  = RegisterBits< type,  7,  1 >;  /**< Auto-reload preload enable     */
      using CMS   = RegisterBits< type,  5,  2 >;  /**< Center-aligned mode selection  */
      using DIR   = RegisterBits< type,  4,  1 >;  /**< Direction                      */
      using OPM   = RegisterBits< type,  3,  1 >;  /**< One-pulse mode                 */
      using URS   = RegisterBits< type,  2,  1 >;  /**< Update request source          */
      using UDIS  = RegisterBits< type,  1,  1 >;  /**< Update disable                 */
      using CEN   = RegisterBits< type,  0,  1 >;  /**< Counter enable                 */
    };

    /**
     * control register 2
     */
    struct CR2
    : public Register< uint32_t, base_addr + 0x4, Access::rw, 0x0000 >
    {
      using type = Register< uint32_t, base_addr + 0x4, Access::rw, 0x0000 >;

      using OIS4   = RegisterBits< type, 14,  1 >;  /**< Output Idle state 4                       */
      using OIS3N  = RegisterBits< type, 13,  1 >;  /**< Output Idle state 3                       */
      using OIS3   = RegisterBits< type, 12,  1 >;  /**< Output Idle state 3                       */
      using OIS2N  = RegisterBits< type, 11,  1 >;  /**< Output Idle state 2                       */
      using OIS2   = RegisterBits< type, 10,  1 >;  /**< Output Idle state 2                       */
      using OIS1N  = RegisterBits< type,  9,  1 >;  /**< Output Idle state 1                       */
      using OIS1   = RegisterBits< type,  8,  1 >;  /**< Output Idle state 1                       */
      using TI1S   = RegisterBits< type,  7,  1 >;  /**< TI1 selection                             */
      using MMS    = RegisterBits< type,  4,  3 >;  /**< Master mode selection                     */
      using CCDS   = RegisterBits< type,  3,  1 >;  /**< Capture/compare DMA selection             */
      using CCUS   = RegisterBits< type,  2,  1 >;  /**< Capture/compare control update selection  */
      using CCPC   = RegisterBits< type,  0,  1 >;  /**< Capture/compare preloaded control         */
    };

    /**
     * slave mode control register
     */
    struct SMCR
    : public Register< uint32_t, base_addr + 0x8, Access::rw, 0x0000 >
    {
      using type = Register< uint32_t, base_addr + 0x8, Access::rw, 0x0000 >;

      using ETP   = RegisterBits< type, 15,  1 >;  /**< External trigger polarity   */
      using ECE   = RegisterBits< type, 14,  1 >;  /**< External clock enable       */
      using ETPS  = RegisterBits< type, 12,  2 >;  /**< External trigger prescaler  */
      using ETF   = RegisterBits< type,  8,  4 >;  /**< External trigger filter     */
      using MSM   = RegisterBits< type,  7,  1 >;  /**< Master/Slave mode           */
      using TS    = RegisterBits< type,  4,  3 >;  /**< Trigger selection           */
      using SMS   = RegisterBits< type,  0,  3 >;  /**< Slave mode selection        */
    };

    /**
     * DMA/Interrupt enable register
     */
    struct DIER
    : public Register< uint32_t, base_addr + 0xc, Access::rw, 0x0000 >
    {
      using type = Register< uint32_t, base_addr + 0xc, Access::rw, 0x0000 >;

      using TDE    = RegisterBits< type, 14,  1 >;  /**< Trigger DMA request enable            */
      using COMDE  = RegisterBits< type, 13,  1 >;  /**< COM DMA request enable                */
      using CC4DE  = RegisterBits< type, 12,  1 >;  /**< Capture/Compare 4 DMA request enable  */
      using CC3DE  = RegisterBits< type, 11,  1 >;  /**< Capture/Compare 3 DMA request enable  */
      using CC2DE  = RegisterBits< type, 10,  1 >;  /**< Capture/Compare 2 DMA request enable  */
      using CC1DE  = RegisterBits< type,  9,  1 >;  /**< Capture/Compare 1 DMA request enable  */
      using UDE    = RegisterBits< type,  8,  1 >;  /**< Update DMA request enable             */
      using TIE    = RegisterBits< type,  6,  1 >;  /**< Trigger interrupt enable              */
      using CC4IE  = RegisterBits< type,  4,  1 >;  /**< Capture/Compare 4 interrupt enable    */
      using CC3IE  = RegisterBits< type,  3,  1 >;  /**< Capture/Compare 3 interrupt enable    */
      using CC2IE  = RegisterBits< type,  2,  1 >;  /**< Capture/Compare 2 interrupt enable    */
      using CC1IE  = RegisterBits< type,  1,  1 >;  /**< Capture/Compare 1 interrupt enable    */
      using UIE    = RegisterBits< type,  0,  1 >;  /**< Update interrupt enable               */
      using BIE    = RegisterBits< type,  7,  1 >;  /**< Break interrupt enable                */
      using COMIE  = RegisterBits< type,  5,  1 >;  /**< COM interrupt enable                  */
    };

    /**
     * status register
     */
    struct SR
    : public Register< uint32_t, base_addr + 0x10, Access::rw, 0x0000 >
    {
      using type = Register< uint32_t, base_addr + 0x10, Access::rw, 0x0000 >;

      using CC4OF  = RegisterBits< type, 12,  1 >;  /**< Capture/Compare 4 overcapture flag  */
      using CC3OF  = RegisterBits< type, 11,  1 >;  /**< Capture/Compare 3 overcapture flag  */
      using CC2OF  = RegisterBits< type, 10,  1 >;  /**< Capture/compare 2 overcapture flag  */
      using CC1OF  = RegisterBits< type,  9,  1 >;  /**< Capture/Compare 1 overcapture flag  */
      using BIF    = RegisterBits< type,  7,  1 >;  /**< Break interrupt flag                */
      using TIF    = RegisterBits< type,  6,  1 >;  /**< Trigger interrupt flag              */
      using COMIF  = RegisterBits< type,  5,  1 >;  /**< COM interrupt flag                  */
      using CC4IF  = RegisterBits< type,  4,  1 >;  /**< Capture/Compare 4 interrupt flag    */
      using CC3IF  = RegisterBits< type,  3,  1 >;  /**< Capture/Compare 3 interrupt flag    */
      using CC2IF  = RegisterBits< type,  2,  1 >;  /**< Capture/Compare 2 interrupt flag    */
      using CC1IF  = RegisterBits< type,  1,  1 >;  /**< Capture/compare 1 interrupt flag    */
      using UIF    = RegisterBits< type,  0,  1 >;  /**< Update interrupt flag               */
    };

    /**
     * event generation register
     */
    struct EGR
    : public Register< uint32_t, base_addr + 0x14, Access::wo, 0x0000 >
    {
      using type = Register< uint32_t, base_addr + 0x14, Access::wo, 0x0000 >;

      using BG    = RegisterBits< type,  7,  1 >;  /**< Break generation                           */
      using TG    = RegisterBits< type,  6,  1 >;  /**< Trigger generation                         */
      using COMG  = RegisterBits< type,  5,  1 >;  /**< Capture/Compare control update generation  */
      using CC4G  = RegisterBits< type,  4,  1 >;  /**< Capture/compare 4 generation               */
      using CC3G  = RegisterBits< type,  3,  1 >;  /**< Capture/compare 3 generation               */
      using CC2G  = RegisterBits< type,  2,  1 >;  /**< Capture/compare 2 generation               */
      using CC1G  = RegisterBits< type,  1,  1 >;  /**< Capture/compare 1 generation               */
      using UG    = RegisterBits< type,  0,  1 >;  /**< Update generation                          */
    };

    /**
     * capture/compare mode register 1 (output mode)
     */
    struct CCMR1_Output
    : public Register< uint32_t, base_addr + 0x18, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x18, Access::rw, 0x00000000 >;

      using OC2CE  = RegisterBits< type, 15,  1 >;  /**< Output Compare 2 clear enable    */
      using OC2M   = RegisterBits< type, 12,  3 >;  /**< Output Compare 2 mode            */
      using OC2PE  = RegisterBits< type, 11,  1 >;  /**< Output Compare 2 preload enable  */
      using OC2FE  = RegisterBits< type, 10,  1 >;  /**< Output Compare 2 fast enable     */
      using CC2S   = RegisterBits< type,  8,  2 >;  /**< Capture/Compare 2 selection      */
      using OC1CE  = RegisterBits< type,  7,  1 >;  /**< Output Compare 1 clear enable    */
      using OC1M   = RegisterBits< type,  4,  3 >;  /**< Output Compare 1 mode            */
      using OC1PE  = RegisterBits< type,  3,  1 >;  /**< Output Compare 1 preload enable  */
      using OC1FE  = RegisterBits< type,  2,  1 >;  /**< Output Compare 1 fast enable     */
      using CC1S   = RegisterBits< type,  0,  2 >;  /**< Capture/Compare 1 selection      */
    };

    /**
     * capture/compare mode register 1 (input mode)
     */
    struct CCMR1_Input
    : public Register< uint32_t, base_addr + 0x18, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x18, Access::rw, 0x00000000 >;

      using IC2F    = RegisterBits< type, 12,  4 >;  /**< Input capture 2 filter       */
      using IC2PSC  = RegisterBits< type, 10,  2 >;  /**< Input capture 2 prescaler    */
      using CC2S    = RegisterBits< type,  8,  2 >;  /**< Capture/Compare 2 selection  */
      using IC1F    = RegisterBits< type,  4,  4 >;  /**< Input capture 1 filter       */
      using IC1PSC  = RegisterBits< type,  2,  2 >;  /**< Input capture 1 prescaler    */
      using CC1S    = RegisterBits< type,  0,  2 >;  /**< Capture/Compare 1 selection  */
    };

    /**
     * capture/compare mode register 2 (output mode)
     */
    struct CCMR2_Output
    : public Register< uint32_t, base_addr + 0x1c, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x1c, Access::rw, 0x00000000 >;

      using OC4CE  = RegisterBits< type, 15,  1 >;  /**< Output compare 4 clear enable    */
      using OC4M   = RegisterBits< type, 12,  3 >;  /**< Output compare 4 mode            */
      using OC4PE  = RegisterBits< type, 11,  1 >;  /**< Output compare 4 preload enable  */
      using OC4FE  = RegisterBits< type, 10,  1 >;  /**< Output compare 4 fast enable     */
      using CC4S   = RegisterBits< type,  8,  2 >;  /**< Capture/Compare 4 selection      */
      using OC3CE  = RegisterBits< type,  7,  1 >;  /**< Output compare 3 clear enable    */
      using OC3M   = RegisterBits< type,  4,  3 >;  /**< Output compare 3 mode            */
      using OC3PE  = RegisterBits< type,  3,  1 >;  /**< Output compare 3 preload enable  */
      using OC3FE  = RegisterBits< type,  2,  1 >;  /**< Output compare 3 fast enable     */
      using CC3S   = RegisterBits< type,  0,  2 >;  /**< Capture/Compare 3 selection      */
    };

    /**
     * capture/compare mode register 2 (input mode)
     */
    struct CCMR2_Input
    : public Register< uint32_t, base_addr + 0x1c, Access::rw, 0x00000000 >
    {
      using type = Register< uint32_t, base_addr + 0x1c, Access::rw, 0x00000000 >;

      using IC4F    = RegisterBits< type, 12,  4 >;  /**< Input capture 4 filter       */
      using IC4PSC  = RegisterBits< type, 10,  2 >;  /**< Input capture 4 prescaler    */
      using CC4S    = RegisterBits< type,  8,  2 >;  /**< Capture/Compare 4 selection  */
      using IC3F    = RegisterBits< type,  4,  4 >;  /**< Input capture 3 filter       */
      using IC3PSC  = RegisterBits< type,  2,  2 >;  /**< Input capture 3 prescaler    */
      using CC3S    = RegisterBits< type,  0,  2 >;  /**< Capture/compare 3 selection  */
    };

    /**
     * capture/compare enable register
     */
    struct CCER
    : public Register< uint32_t, base_addr + 0x20, Access::rw, 0x0000 >
    {
      using type = Register< uint32_t, base_addr + 0x20, Access::rw, 0x0000 >;

      using CC4P   = RegisterBits< type, 13,  1 >;  /**< Capture/Compare 3 output Polarity              */
      using CC4E   = RegisterBits< type, 12,  1 >;  /**< Capture/Compare 4 output enable                */
      using CC3NP  = RegisterBits< type, 11,  1 >;  /**< Capture/Compare 3 output Polarity              */
      using CC3NE  = RegisterBits< type, 10,  1 >;  /**< Capture/Compare 3 complementary output enable  */
      using CC3P   = RegisterBits< type,  9,  1 >;  /**< Capture/Compare 3 output Polarity              */
      using CC3E   = RegisterBits< type,  8,  1 >;  /**< Capture/Compare 3 output enable                */
      using CC2NP  = RegisterBits< type,  7,  1 >;  /**< Capture/Compare 2 output Polarity              */
      using CC2NE  = RegisterBits< type,  6,  1 >;  /**< Capture/Compare 2 complementary output enable  */
      using CC2P   = RegisterBits< type,  5,  1 >;  /**< Capture/Compare 2 output Polarity              */
      using CC2E   = RegisterBits< type,  4,  1 >;  /**< Capture/Compare 2 output enable                */
      using CC1NP  = RegisterBits< type,  3,  1 >;  /**< Capture/Compare 1 output Polarity              */
      using CC1NE  = RegisterBits< type,  2,  1 >;  /**< Capture/Compare 1 complementary output enable  */
      using CC1P   = RegisterBits< type,  1,  1 >;  /**< Capture/Compare 1 output Polarity              */
      using CC1E   = RegisterBits< type,  0,  1 >;  /**< Capture/Compare 1 output enable                */
    };

    /**
     * counter
     */
    using CNT = Register< uint32_t, base_addr + 0x24, Access::rw, 0x00000000 >;

    /**
     * prescaler
     */
    using PSC = Register< uint32_t, base_addr + 0x28, Access::rw, 0x0000 >;

    /**
     * auto-reload register
     */
    using ARR = Register< uint32_t, base_addr + 0x2c, Access::rw, 0x00000000 >;

    /**
     * capture/compare register 1
     */
    using CCR1 = Register< uint32_t, base_addr + 0x34, Access::rw, 0x00000000 >;

    /**
     * capture/compare register 2
     */
    using CCR2 = Register< uint32_t, base_addr + 0x38, Access::rw, 0x00000000 >;

    /**
     * capture/compare register 3
     */
    using CCR3 = Register< uint32_t, base_addr + 0x3c, Access::rw, 0x00000000 >;

    /**
     * capture/compare register 4
     */
    using CCR4 = Register< uint32_t, base_addr + 0x40, Access::rw, 0x00000000 >;

    /**
     * DMA control register
     */
    struct DCR
    : public Register< uint32_t, base_addr + 0x48, Access::rw, 0x0000 >
    {
      using type = Register< uint32_t, base_addr + 0x48, Access::rw, 0x0000 >;

      using DBL  = RegisterBits< type,  8,  5 >;  /**< DMA burst length  */
      using DBA  = RegisterBits< type,  0,  5 >;  /**< DMA base address  */
    };

    /**
     * DMA address for full transfer
     */
    using DMAR = Register< uint32_t, base_addr + 0x4c, Access::rw, 0x0000 >;

    /**
     * repetition counter register
     */
    struct RCR
    : public Register< uint32_t, base_addr + 0x30, Access::rw, 0x0000 >
    {
      using type = Register< uint32_t, base_addr + 0x30, Access::rw, 0x0000 >;

      using REP  = RegisterBits< type,  0,  8 >;  /**< Repetition counter value  */
    };

    /**
     * break and dead-time register
     */
    struct BDTR
    : public Register< uint32_t, base_addr + 0x44, Access::rw, 0x0000 >
    {
      using type = Register< uint32_t, base_addr + 0x44, Access::rw, 0x0000 >;

      using MOE   = RegisterBits< type, 15,  1 >;  /**< Main output enable                 */
      using AOE   = RegisterBits< type, 14,  1 >;  /**< Automatic output enable            */
      using BKP   = RegisterBits< type, 13,  1 >;  /**< Break polarity                     */
      using BKE   = RegisterBits< type, 12,  1 >;  /**< Break enable                       */
      using OSSR  = RegisterBits< type, 11,  1 >;  /**< Off-state selection for Run mode   */
      using OSSI  = RegisterBits< type, 10,  1 >;  /**< Off-state selection for Idle mode  */
      using LOCK  = RegisterBits< type,  8,  2 >;  /**< Lock configuration                 */
      using DTG   = RegisterBits< type,  0,  8 >;  /**< Dead-time generator setup          */
    };
  };
}

#endif // STM32_COMMON_REG_TIM_HPP_INCLUDED
