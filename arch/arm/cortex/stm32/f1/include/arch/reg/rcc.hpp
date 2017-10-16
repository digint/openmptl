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

#ifndef ARCH_REG_RCC_HPP_INCLUDED
#define ARCH_REG_RCC_HPP_INCLUDED

#include <register.hpp>

namespace mptl {

/**
 * Reset and clock control
 */
struct RCC
{
  static constexpr reg_addr_t base_addr = 0x40021000;

  /**
   * Clock control register
   */
  struct CR
  : public reg< uint32_t, base_addr + 0x0, rw, 0x00000083 >
  {
    using HSION    = regbits< type,  0,  1 >;  /**< Internal High Speed clock enable       */
    using HSIRDY   = regbits< type,  1,  1 >;  /**< Internal High Speed clock ready flag   */
    using HSITRIM  = regbits< type,  3,  5 >;  /**< Internal High Speed clock trimming     */
    using HSICAL   = regbits< type,  8,  8 >;  /**< Internal High Speed clock Calibration  */
    using HSEON    = regbits< type, 16,  1 >;  /**< External High Speed clock enable       */
    using HSERDY   = regbits< type, 17,  1 >;  /**< External High Speed clock ready flag   */
    using HSEBYP   = regbits< type, 18,  1 >;  /**< External High Speed clock Bypass       */
    using CSSON    = regbits< type, 19,  1 >;  /**< Clock Security System enable           */
    using PLLON    = regbits< type, 24,  1 >;  /**< PLL enable                             */
    using PLLRDY   = regbits< type, 25,  1 >;  /**< PLL clock ready flag                   */
    using PLL2ON   = regbits< type, 26,  1 >;  /**< PLL2 enable           (only available on connectivity line devices!)  */
    using PLL2RDY  = regbits< type, 27,  1 >;  /**< PLL2 clock ready flag (only available on connectivity line devices!)  */
    using PLL3ON   = regbits< type, 28,  1 >;  /**< PLL3 enable           (only available on connectivity line devices!)  */
    using PLL3RDY  = regbits< type, 29,  1 >;  /**< PLL3 clock ready flag (only available on connectivity line devices!)  */
  };

  /**
   * Clock configuration register (RCC_CFGR)
   */
  struct CFGR
  : public reg< uint32_t, base_addr + 0x4, rw, 0x00000000 >
  {
    /** System clock switch  */
    struct SW
    : public regbits< type,  0,  2 >
    {
      using HSI  = regval< regbits_type, 0x0 >;          /**< 00: HSI selected as system clock   */
      using HSE  = regval< regbits_type, 0x1 >;          /**< 01: HSE selected as system clock   */
      using PLL  = regval< regbits_type, 0x2 >;          /**< 10: PLL selected as system clock   */
    };

    /** System clock switch status  */
    struct SWS
    : public regbits< type,  2,  2 >
    {
      using HSI  = regval< regbits_type, 0x0 >;          /**< 00: HSI selected as system clock   */
      using HSE  = regval< regbits_type, 0x1 >;          /**< 01: HSE selected as system clock   */
      using PLL  = regval< regbits_type, 0x2 >;          /**< 10: PLL selected as system clock   */
    };

    /** AHB prescaler  */
    struct HPRE
    : public regbits< type,  4,  4 >
    {
      using DIV1    = regval< regbits_type, 0x0 >;       /**< 0xxx: SYSCLK not divided      */
      using DIV2    = regval< regbits_type, 0x8 >;       /**< 1000: SYSCLK divided by 2     */
      using DIV4    = regval< regbits_type, 0x9 >;       /**< 1001: SYSCLK divided by 4     */
      using DIV8    = regval< regbits_type, 0xa >;       /**< 1010: SYSCLK divided by 8     */
      using DIV16   = regval< regbits_type, 0xb >;       /**< 1011: SYSCLK divided by 16    */
      using DIV64   = regval< regbits_type, 0xc >;       /**< 1100: SYSCLK divided by 64    */
      using DIV128  = regval< regbits_type, 0xd >;       /**< 1101: SYSCLK divided by 128   */
      using DIV256  = regval< regbits_type, 0xe >;       /**< 1110: SYSCLK divided by 256   */
      using DIV512  = regval< regbits_type, 0xf >;       /**< 1111: SYSCLK divided by 512   */
    };

    /** APB low speed prescaler (APB1)  */
    struct PPRE1
    : public regbits< type,  8,  3 >
    {
      using DIV1   = regval< regbits_type, 0x0 >;        /**< 0xx: HCLK not divided     */
      using DIV2   = regval< regbits_type, 0x4 >;        /**< 100: HCLK divided by 2    */
      using DIV4   = regval< regbits_type, 0x5 >;        /**< 101: HCLK divided by 4    */
      using DIV8   = regval< regbits_type, 0x6 >;        /**< 110: HCLK divided by 8    */
      using DIV16  = regval< regbits_type, 0x7 >;        /**< 111: HCLK divided by 16   */
    };

    /** APB high speed prescaler (APB2)  */
    struct PPRE2
    : public regbits< type, 11,  3 >
    {
      using DIV1   = regval< regbits_type, 0x0 >;        /**< 0xx: HCLK not divided     */
      using DIV2   = regval< regbits_type, 0x4 >;        /**< 100: HCLK divided by 2    */
      using DIV4   = regval< regbits_type, 0x5 >;        /**< 101: HCLK divided by 4    */
      using DIV8   = regval< regbits_type, 0x6 >;        /**< 110: HCLK divided by 8    */
      using DIV16  = regval< regbits_type, 0x7 >;        /**< 111: HCLK divided by 16   */
    };

    /** ADC prescaler  */
    struct ADCPRE
    : public regbits< type, 14,  2 >
    {
      using DIV2  = regval< regbits_type, 0x0 >;         /**< 00: PCLK2 divided by 2   */
      using DIV4  = regval< regbits_type, 0x1 >;         /**< 01: PCLK2 divided by 4   */
      using DIV6  = regval< regbits_type, 0x2 >;         /**< 10: PCLK2 divided by 6   */
      using DIV8  = regval< regbits_type, 0x3 >;         /**< 11: PCLK2 divided by 8   */
    };

    /** PLL entry clock source  */
    struct PLLSRC
    : public regbits< type, 16,  1 >
    {
      using HSI_DIV2  = regval< regbits_type, 0x0 >;     /**< 00: HSI clock divided by 2 selected as PLL input clock   */
      using HSE       = regval< regbits_type, 0x1 >;     /**< 01: HSE oscillator clock selected as PLL input clock (not available on connectivity line devices!)  */
      using PREDIV1   = regval< regbits_type, 0x1 >;     /**< 01: Clock from PREDIV1 selected as PLL input clock (only available on connectivity line devices!)  */
    };

    /**
     * - HSE divider for PLL entry (low-, medium-, high- and XL-density devices)
     * - LSB of division factor PREDIV1 (connectivity line devices)
     */
    struct PLLXTPRE
    : public regbits< type, 17,  1 >
    {
      /* Low-, medium-, high- and XL-density Devices */
      using HSE_DIV1  = regval< regbits_type, 0x0 >;     /**< 00: HSE clock not divided (not available on connectivity line devices!)   */
      using HSE_DIV2  = regval< regbits_type, 0x1 >;     /**< 01: HSE clock divided by 2 (not available on connectivity line devices!)  */

      /* Connectivity line devices */
      using PREDIV1_DIV1  = regval< regbits_type, 0x0 >; /**< 00: PREDIV1 clock not divided for PLL entry (only available on connectivity line devices!)   */
      using PREDIV1_DIV2  = regval< regbits_type, 0x1 >; /**< 01: PREDIV1 clock divided by 2 for PLL entry (only available on connectivity line devices!)  */
    };

    /** PLL Multiplication Factor  */
    struct PLLMUL
    : public regbits< type, 18,  4 >
    {
      using MUL2    = regval< regbits_type, 0x0 >;       /**< 0000: PLL input clock * 2  (not available on connectivity line devices!)  */
      using MUL3    = regval< regbits_type, 0x1 >;       /**< 0001: PLL input clock * 3  (not available on connectivity line devices!)  */
      using MUL4    = regval< regbits_type, 0x2 >;       /**< 0010: PLL input clock * 4  */
      using MUL5    = regval< regbits_type, 0x3 >;       /**< 0011: PLL input clock * 5  */
      using MUL6    = regval< regbits_type, 0x4 >;       /**< 0100: PLL input clock * 6  */
      using MUL7    = regval< regbits_type, 0x5 >;       /**< 0101: PLL input clock * 7  */
      using MUL8    = regval< regbits_type, 0x6 >;       /**< 0110: PLL input clock * 8  */
      using MUL9    = regval< regbits_type, 0x7 >;       /**< 0111: PLL input clock * 9  */
      using MUL10   = regval< regbits_type, 0x8 >;       /**< 1000: PLL input clock * 10  (not available on connectivity line devices!)  */
      using MUL11   = regval< regbits_type, 0x9 >;       /**< 1001: PLL input clock * 11  (not available on connectivity line devices!)  */
      using MUL12   = regval< regbits_type, 0xa >;       /**< 1010: PLL input clock * 12  (not available on connectivity line devices!)  */
      using MUL13   = regval< regbits_type, 0xb >;       /**< 1011: PLL input clock * 13  (not available on connectivity line devices!)  */
      using MUL14   = regval< regbits_type, 0xc >;       /**< 1100: PLL input clock * 14  (not available on connectivity line devices!)  */
      using MUL15   = regval< regbits_type, 0xd >;       /**< 1101: PLL input clock * 15  (not available on connectivity line devices!)  */
      using MUL16   = regval< regbits_type, 0xe >;       /**< 1110: PLL input clock * 16  (not available on connectivity line devices!)  */
      using MUL6_5  = regval< regbits_type, 0xd >;       /**< 1101: PLL input clock * 6.5 (only available on connectivity line devices!)  */
    };

    /** USB OTG FS prescaler (only available on connectivity line devices!)  */
    struct OTGFSPRE
    : public regbits< type, 22,  1 >
    {
      using DIV3  = regval< regbits_type, 0x0 >;
      using DIV2  = regval< regbits_type, 0x1 >;
    };

    /** USB prescaler (not available on connectivity line devices!) */
    struct USBPRE
    : public regbits< type, 22,  1 >
    {
      using DIV1_5  = regval< regbits_type, 0x0 >;
      using DIV1    = regval< regbits_type, 0x1 >;
    };

    /** Microcontroller Clock Output  */
    struct MCO
    : public regbits< type, 24,  4 >
    {
      using NOCLOCK    = regval< regbits_type, 0x0 >;   /**< 00xx: No clock                          */
      using SYSCLK     = regval< regbits_type, 0x4 >;   /**< 0100: System clock selected             */
      using HSI        = regval< regbits_type, 0x5 >;   /**< 0101: HSI clock selected                */
      using HSE        = regval< regbits_type, 0x6 >;   /**< 0110: HSE clock selected                */
      using PLL_DIV2   = regval< regbits_type, 0x7 >;   /**< 0111: PLL clock divided by 2 selected   */
      using PLL2       = regval< regbits_type, 0x8 >;   /**< 1000: PLL2 clock selected (only available on connectivity line devices!)                                             */
      using PLL3_DIV2  = regval< regbits_type, 0x9 >;   /**< 1001: PLL3 clock divided by 2 selected (only available on connectivity line devices!)                                 */
      using Ext_HSE    = regval< regbits_type, 0xa >;   /**< 1010: XT1 external 3-25 MHz oscillator clock selected (for Ethernet) (only available on connectivity line devices!)   */
      using PLL3       = regval< regbits_type, 0xb >;   /**< 1011: PLL3 clock selected (for Ethernet) (only available on connectivity line devices!)                               */
    };
  };

  /**
   * Clock interrupt register (RCC_CIR)
   */
  struct CIR
  : public reg< uint32_t, base_addr + 0x8, rw, 0x00000000 >
  {
    using LSIRDYF    = regbits< type,  0,  1 >;  /**< LSI Ready Interrupt flag  */
    using LSERDYF    = regbits< type,  1,  1 >;  /**< LSE Ready Interrupt flag  */
    using HSIRDYF    = regbits< type,  2,  1 >;  /**< HSI Ready Interrupt flag  */
    using HSERDYF    = regbits< type,  3,  1 >;  /**< HSE Ready Interrupt flag  */
    using PLLRDYF    = regbits< type,  4,  1 >;  /**< PLL Ready Interrupt flag  */
    using PLL2RDYF   = regbits< type,  5,  1 >;  /**< PLL2 Ready Interrupt flag (only available on connectivity line devices!)  */
    using PLL3RDYF   = regbits< type,  6,  1 >;  /**< PLL3 Ready Interrupt flag (only available on connectivity line devices!)  */
    using CSSF       = regbits< type,  7,  1 >;  /**< Clock Security System Interrupt flag   */
    using LSIRDYIE   = regbits< type,  8,  1 >;  /**< LSI Ready Interrupt Enable  */
    using LSERDYIE   = regbits< type,  9,  1 >;  /**< LSE Ready Interrupt Enable  */
    using HSIRDYIE   = regbits< type, 10,  1 >;  /**< HSI Ready Interrupt Enable  */
    using HSERDYIE   = regbits< type, 11,  1 >;  /**< HSE Ready Interrupt Enable  */
    using PLLRDYIE   = regbits< type, 12,  1 >;  /**< PLL Ready Interrupt Enable  */
    using PLL2RDYIE  = regbits< type, 13,  1 >;  /**< PLL2 Ready Interrupt Enable (only available on connectivity line devices!)  */
    using PLL3RDYIE  = regbits< type, 14,  1 >;  /**< PLL3 Ready Interrupt Enable (only available on connectivity line devices!)  */
    using LSIRDYC    = regbits< type, 16,  1 >;  /**< LSI Ready Interrupt Clear  */
    using LSERDYC    = regbits< type, 17,  1 >;  /**< LSE Ready Interrupt Clear  */
    using HSIRDYC    = regbits< type, 18,  1 >;  /**< HSI Ready Interrupt Clear  */
    using HSERDYC    = regbits< type, 19,  1 >;  /**< HSE Ready Interrupt Clear  */
    using PLLRDYC    = regbits< type, 20,  1 >;  /**< PLL Ready Interrupt Clear  */
    using PLL2RDYC   = regbits< type, 21,  1 >;  /**< PLL2 Ready Interrupt Clear (only available on connectivity line devices!)  */
    using PLL3RDYC   = regbits< type, 22,  1 >;  /**< PLL3 Ready Interrupt Clear (only available on connectivity line devices!)  */
    using CSSC       = regbits< type, 23,  1 >;  /**< Clock security system interrupt clear  */
  };

  /**
   * APB2 peripheral reset register (RCC_APB2RSTR)
   */
  struct APB2RSTR
  : public reg< uint32_t, base_addr + 0xc, rw, 0x000000000 >
  {
    using AFIORST    = regbits< type,  0,  1 >;  /**< Alternate function I/O reset  */
    using IOPARST    = regbits< type,  2,  1 >;  /**< IO port A reset               */
    using IOPBRST    = regbits< type,  3,  1 >;  /**< IO port B reset               */
    using IOPCRST    = regbits< type,  4,  1 >;  /**< IO port C reset               */
    using IOPDRST    = regbits< type,  5,  1 >;  /**< IO port D reset               */
    using IOPERST    = regbits< type,  6,  1 >;  /**< IO port E reset (not available on low-density devices!)  */
    using IOPFRST    = regbits< type,  7,  1 >;  /**< IO port F reset (only available on high- and XL-density devices!)     */
    using IOPGRST    = regbits< type,  8,  1 >;  /**< IO port G reset (only available on high- and XL-density devices!)     */
    using ADC1RST    = regbits< type,  9,  1 >;  /**< ADC 1 interface reset         */
    using ADC2RST    = regbits< type, 10,  1 >;  /**< ADC 2 interface reset (not available on low- and medium-density devices!) */
    using TIM1RST    = regbits< type, 11,  1 >;  /**< TIM1 timer reset              */
    using SPI1RST    = regbits< type, 12,  1 >;  /**< SPI 1 reset                   */
    using TIM8RST    = regbits< type, 13,  1 >;  /**< TIM8 Timer reset (only available on high- and XL-density devices!)      */
    using USART1RST  = regbits< type, 14,  1 >;  /**< USART1 reset                  */
    using ADC3RST    = regbits< type, 15,  1 >;  /**< ADC3 interface reset (only available on high- and XL-density devices!)  */
    using TIM15RST   = regbits< type, 16,  1 >;  /**< TIM15 Timer reset (only available on low- and medium-density devices!)  */
    using TIM16RST   = regbits< type, 17,  1 >;  /**< TIM16 Timer reset (only available on low- and medium-density devices!)  */
    using TIM17RST   = regbits< type, 18,  1 >;  /**< TIM17 Timer reset (only available on low- and medium-density devices!)  */
    using TIM9RST    = regbits< type, 19,  1 >;  /**< TIM9 Timer reset (only available on XL-density devices!)  */
    using TIM10RST   = regbits< type, 20,  1 >;  /**< TIM10 Timer reset (only available on XL-density devices!)  */
    using TIM11RST   = regbits< type, 21,  1 >;  /**< TIM11 Timer reset (only available on XL-density devices!)  */
  };

  /**
   * APB1 peripheral reset register (RCC_APB1RSTR)
   */
  struct APB1RSTR
  : public reg< uint32_t, base_addr + 0x10, rw, 0x00000000 >
  {
    using TIM2RST    = regbits< type,  0,  1 >;  /**< Timer 2 reset         */
    using TIM3RST    = regbits< type,  1,  1 >;  /**< Timer 3 reset         */
    using TIM4RST    = regbits< type,  2,  1 >;  /**< Timer 4 reset (not available on low-density devices!)  */
    using TIM5RST    = regbits< type,  3,  1 >;  /**< Timer 5 reset         */
    using TIM6RST    = regbits< type,  4,  1 >;  /**< Timer 6 reset         */
    using TIM7RST    = regbits< type,  5,  1 >;  /**< Timer 7 reset         */
    using TIM12RST   = regbits< type,  6,  1 >;  /**< Timer 12 reset (only available on XL-density devices!)  */
    using TIM13RST   = regbits< type,  7,  1 >;  /**< Timer 13 reset (only available on XL-density devices!)  */
    using TIM14RST   = regbits< type,  8,  1 >;  /**< Timer 14 reset (only available on XL-density devices!)  */
    using WWDGRST    = regbits< type, 11,  1 >;  /**< Window Watchdog reset  */
    using SPI2RST    = regbits< type, 14,  1 >;  /**< SPI 2 reset (not available on low-density devices!)     */
    using SPI3RST    = regbits< type, 15,  1 >;  /**< SPI 3 reset            */
    using USART2RST  = regbits< type, 17,  1 >;  /**< USART 2 reset          */
    using USART3RST  = regbits< type, 18,  1 >;  /**< RUSART 3 reset (not available on low-density devices!)  */
    using UART4RST   = regbits< type, 19,  1 >;  /**< UART 4 reset           */
    using UART5RST   = regbits< type, 20,  1 >;  /**< UART 5 reset           */
    using I2C1RST    = regbits< type, 21,  1 >;  /**< I2C 1 reset            */
    using I2C2RST    = regbits< type, 22,  1 >;  /**< I2C 2 reset (not available on low-density devices!)     */
    using USBRST     = regbits< type, 23,  1 >;  /**< USB Device reset (not available on connectivity line devices!)  */
    using CAN1RST    = regbits< type, 25,  1 >;  /**< CAN1 reset (not available on low- and medium-density devices!)  */
    using CAN2RST    = regbits< type, 26,  1 >;  /**< CAN2 reset (only available on connectivity line devices!)       */
    using BKPRST     = regbits< type, 27,  1 >;  /**< Backup interface reset  */
    using PWRRST     = regbits< type, 28,  1 >;  /**< Power interface reset   */
    using DACRST     = regbits< type, 29,  1 >;  /**< DAC interface reset     */
    using CECRST     = regbits< type, 30,  1 >;  /**< CEC interface reset     */
  };

  /**
   * AHB Peripheral Clock enable register (RCC_AHBENR)
   */
  struct AHBENR
  : public reg< uint32_t, base_addr + 0x14, rw, 0x00000014 >
  {
    using DMA1EN      = regbits< type,  0,  1 >;  /**< DMA1 clock enable             */
    using DMA2EN      = regbits< type,  1,  1 >;  /**< DMA2 clock enable (only available on high-density and connectivity line devices!)  */
    using SRAMEN      = regbits< type,  2,  1 >;  /**< SRAM interface clock enable   */
    using FLITFEN     = regbits< type,  4,  1 >;  /**< FLITF clock enable            */
    using CRCEN       = regbits< type,  6,  1 >;  /**< CRC clock enable              */
    using FSMCEN      = regbits< type,  8,  1 >;  /**< FSMC clock enable (only available on high- and XL-density devices!)  */
    using SDIOEN      = regbits< type, 10,  1 >;  /**< SDIO clock enable (only available on high- and XL-density devices!) */
    using OTGFSEN     = regbits< type, 12,  1 >;  /**< USB OTG FS clock enable (only available on connectivity line devices!)      */
    using ETHMACEN    = regbits< type, 14,  1 >;  /**< Ethernet MAC clock enable (only available on connectivity line devices!)    */
    using ETHMACTXEN  = regbits< type, 15,  1 >;  /**< Ethernet MAC TX clock enable (only available on connectivity line devices!) */
    using ETHMACRXEN  = regbits< type, 16,  1 >;  /**< Ethernet MAC RX clock enable (only available on connectivity line devices!) */
  };

  /**
   * APB2 peripheral clock enable register (RCC_APB2ENR)
   */
  struct APB2ENR
  : public reg< uint32_t, base_addr + 0x18, rw, 0x00000000 >
  {
    using AFIOEN    = regbits< type,  0,  1 >;  /**< Alternate function I/O clock enable  */
    using IOPAEN    = regbits< type,  2,  1 >;  /**< I/O port A clock enable              */
    using IOPBEN    = regbits< type,  3,  1 >;  /**< I/O port B clock enable              */
    using IOPCEN    = regbits< type,  4,  1 >;  /**< I/O port C clock enable              */
    using IOPDEN    = regbits< type,  5,  1 >;  /**< I/O port D clock enable              */
    using IOPEEN    = regbits< type,  6,  1 >;  /**< I/O port E clock enable (not available on low-density devices!)  */
    using IOPFEN    = regbits< type,  7,  1 >;  /**< I/O port F clock enable (only available on high- and XL-density devices!)  */
    using IOPGEN    = regbits< type,  8,  1 >;  /**< I/O port G clock enable (only available on high- and XL-density devices!)  */
    using ADC1EN    = regbits< type,  9,  1 >;  /**< ADC 1 interface clock enable         */
    using ADC2EN    = regbits< type, 10,  1 >;  /**< ADC 2 interface clock enable (not available on low- and medium-density devices!)  */
    using TIM1EN    = regbits< type, 11,  1 >;  /**< TIM1 Timer clock enable              */
    using SPI1EN    = regbits< type, 12,  1 >;  /**< SPI 1 clock enable                   */
    using TIM8EN    = regbits< type, 13,  1 >;  /**< TIM8 Timer clock enable (only available on high- and XL-density devices!)  */
    using USART1EN  = regbits< type, 14,  1 >;  /**< USART1 clock enable                  */
    using ADC3EN    = regbits< type, 15,  1 >;  /**< ADC 3 interface clock enable (only available on high- and XL-density devices!)  */
    using TIM15EN   = regbits< type, 16,  1 >;  /**< TIM15 Timer clock enable (only available on low- and medium-density devices!)   */
    using TIM16EN   = regbits< type, 17,  1 >;  /**< TIM16 Timer clock enable (only available on low- and medium-density devices!)   */
    using TIM17EN   = regbits< type, 18,  1 >;  /**< TIM17 Timer clock enable (only available on low- and medium-density devices!)   */
    using TIM9EN    = regbits< type, 19,  1 >;  /**< TIM9 Timer clock enable (only available on XL-density devices!)   */
    using TIM10EN   = regbits< type, 20,  1 >;  /**< TIM10 Timer clock enable (only available on XL-density devices!)  */
    using TIM11EN   = regbits< type, 21,  1 >;  /**< TIM11 Timer clock enable (only available on XL-density devices!)  */
  };

  /**
   * APB1 peripheral clock enable register (RCC_APB1ENR)
   */
  struct APB1ENR
  : public reg< uint32_t, base_addr + 0x1c, rw, 0x00000000 >
  {
    using TIM2EN    = regbits< type,  0,  1 >;  /**< Timer 2 clock enable           */
    using TIM3EN    = regbits< type,  1,  1 >;  /**< Timer 3 clock enable           */
    using TIM4EN    = regbits< type,  2,  1 >;  /**< Timer 4 clock enable (not available on low-density devices!)          */
    using TIM5EN    = regbits< type,  3,  1 >;  /**< Timer 5 clock enable           */
    using TIM6EN    = regbits< type,  4,  1 >;  /**< Timer 6 clock enable           */
    using TIM7EN    = regbits< type,  5,  1 >;  /**< Timer 7 clock enable           */
    using TIM12EN   = regbits< type,  6,  1 >;  /**< Timer 12 clock enable (only available on XL-density devices!)  */
    using TIM13EN   = regbits< type,  7,  1 >;  /**< Timer 13 clock enable (only available on XL-density devices!)  */
    using TIM14EN   = regbits< type,  8,  1 >;  /**< Timer 14 clock enable (only available on XL-density devices!)  */
    using WWDGEN    = regbits< type, 11,  1 >;  /**< Window watchdog clock enable   */
    using SPI2EN    = regbits< type, 14,  1 >;  /**< SPI 2 clock enable (not available on low-density devices!)            */
    using SPI3EN    = regbits< type, 15,  1 >;  /**< SPI 3 clock enable             */
    using USART2EN  = regbits< type, 17,  1 >;  /**< USART 2 clock enable           */
    using USART3EN  = regbits< type, 18,  1 >;  /**< USART 3 clock enable (not available on low-density devices!)          */
    using UART4EN   = regbits< type, 19,  1 >;  /**< UART 4 clock enable            */
    using UART5EN   = regbits< type, 20,  1 >;  /**< UART 5 clock enable            */
    using I2C1EN    = regbits< type, 21,  1 >;  /**< I2C 1 clock enable             */
    using I2C2EN    = regbits< type, 22,  1 >;  /**< I2C 2 clock enable (not available on low-density devices!)            */
    using USBEN     = regbits< type, 23,  1 >;  /**< USB device clock enable (not available on connectivity line devices!)   */
    using CAN1EN    = regbits< type, 25,  1 >;  /**< CAN1 clock enable (not available on low- and medium-density devices!)  */
    using CAN2EN    = regbits< type, 26,  1 >;  /**< CAN2 clock enable (only available on connectivity line devices!)       */
    using BKPEN     = regbits< type, 27,  1 >;  /**< Backup interface clock enable  */
    using PWREN     = regbits< type, 28,  1 >;  /**< Power interface clock enable   */
    using DACEN     = regbits< type, 29,  1 >;  /**< DAC interface clock enable     */
    using CECEN     = regbits< type, 30,  1 >;  /**< CEC interface clock enable     */

  };

  /**
   * Backup domain control register (RCC_BDCR)
   */
  struct BDCR
  : public reg< uint32_t, base_addr + 0x20, rw, 0x00000000 >
  {
    using LSEON   = regbits< type,  0,  1 >;  /**< External Low Speed oscillator enable  */
    using LSERDY  = regbits< type,  1,  1 >;  /**< External Low Speed oscillator ready   */
    using LSEBYP  = regbits< type,  2,  1 >;  /**< External Low Speed oscillator bypass  */

    /** RTC clock source selection */
    struct RTCSEL
    : public regbits< type,  8,  2 >
    {
      using LSE = regval< regbits_type, 0x1 >;  /**< 01: LSE oscillator clock used as RTC clock                  */
      using LSI = regval< regbits_type, 0x2 >;  /**< 10: LSI oscillator clock used as RTC clock                  */
      using HSE = regval< regbits_type, 0x3 >;  /**< 11: HSE oscillator clock divided by 128 used as RTC clock   */
    };

    using RTCEN   = regbits< type, 15,  1 >;  /**< RTC clock enable                      */
    using BDRST   = regbits< type, 16,  1 >;  /**< Backup domain software reset          */
  };

  /**
   * Control/status register (RCC_CSR)
   */
  struct CSR
  : public reg< uint32_t, base_addr + 0x24, rw, 0x0C000000 >
  {
    using LSION     = regbits< type,  0,  1 >;  /**< Internal low speed oscillator enable  */
    using LSIRDY    = regbits< type,  1,  1 >;  /**< Internal low speed oscillator ready   */
    using RMVF      = regbits< type, 24,  1 >;  /**< Remove reset flag                     */
    using PINRSTF   = regbits< type, 26,  1 >;  /**< PIN reset flag                        */
    using PORRSTF   = regbits< type, 27,  1 >;  /**< POR/PDR reset flag                    */
    using SFTRSTF   = regbits< type, 28,  1 >;  /**< Software reset flag                   */
    using IWDGRSTF  = regbits< type, 29,  1 >;  /**< Independent watchdog reset flag       */
    using WWDGRSTF  = regbits< type, 30,  1 >;  /**< Window watchdog reset flag            */
    using LPWRRSTF  = regbits< type, 31,  1 >;  /**< Low-power reset flag                  */
  };

  /**
   * AHB peripheral clock reset register (RCC_AHBRSTR)
   * (only available on connectivity line devices!)
   */
  struct AHBRSTR
  : public reg< uint32_t, base_addr + 0x28, rw, 0x00000000 >
  {
    using OTGFSRST   = regbits< type, 12,  1 >;  /**< USB OTG FS reset    */
    using ETHMACRST  = regbits< type, 14,  1 >;  /**< Ethernet MAC reset  */
  };

  /**
   * Clock configuration register2 (RCC_CFGR2)
   * (only available on connectivity line devices!)
   */
  struct CFGR2
  : public reg< uint32_t, base_addr + 0x2c, rw, 0x00000000 >
  {
    using PREDIV1     = regbits< type,  0,  4 >;  /**< PREDIV1 division factor     */
    using PREDIV2     = regbits< type,  4,  4 >;  /**< PREDIV2 division factor     */
    using PLL2MUL     = regbits< type,  8,  4 >;  /**< PLL2 Multiplication Factor  */
    using PLL3MUL     = regbits< type, 12,  4 >;  /**< PLL3 Multiplication Factor  */

    /** PREDIV1 entry clock source  */
    struct PREDIV1SRC
    : public regbits< type, 16,  1 >
    {
      using HSE   = regval< regbits_type, 0x0 >;  /**< 0: HSE oscillator clock selected as PREDIV1 clock entry   */
      using PLL2  = regval< regbits_type, 0x1 >;  /**< 1: PLL2 selected as PREDIV1 clock entry                   */
    };

    /** I2S2 clock source  */
    struct I2S2SRC
    : public regbits< type, 17,  1 >
    {
      using SYSCLK  = regval< regbits_type, 0x0 >;  /**< 0: System clock (SYSCLK) selected as I2Sx clock entry   */
      using PLL3    = regval< regbits_type, 0x1 >;  /**< 1: PLL3 VCO clock selected as I2Sx clock entry          */
    };

    /** I2S3 clock source  */
    struct I2S3SRC
    : public regbits< type, 18,  1 >
    {
      using SYSCLK  = regval< regbits_type, 0x0 >;  /**< 0: System clock (SYSCLK) selected as I2S3 clock entry   */
      using PLL3    = regval< regbits_type, 0x1 >;  /**< 1: PLL3 VCO clock selected as I2S3 clock entry          */
    };
  };
};

} // namespace mptl

#endif // ARCH_REG_RCC_HPP_INCLUDED
