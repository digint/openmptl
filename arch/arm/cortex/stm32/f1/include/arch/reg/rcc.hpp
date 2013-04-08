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

#ifndef ARCH_REG_RCC_HPP_INCLUDED
#define ARCH_REG_RCC_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
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
    : public Register< uint32_t, base_addr + 0x0, Access::rw, 0x00000083 >
    {
      using HSION    = RegisterBits< type,  0,  1 >;  /**< Internal High Speed clock enable       */
      using HSIRDY   = RegisterBits< type,  1,  1 >;  /**< Internal High Speed clock ready flag   */
      using HSITRIM  = RegisterBits< type,  3,  5 >;  /**< Internal High Speed clock trimming     */
      using HSICAL   = RegisterBits< type,  8,  8 >;  /**< Internal High Speed clock Calibration  */
      using HSEON    = RegisterBits< type, 16,  1 >;  /**< External High Speed clock enable       */
      using HSERDY   = RegisterBits< type, 17,  1 >;  /**< External High Speed clock ready flag   */
      using HSEBYP   = RegisterBits< type, 18,  1 >;  /**< External High Speed clock Bypass       */
      using CSSON    = RegisterBits< type, 19,  1 >;  /**< Clock Security System enable           */
      using PLLON    = RegisterBits< type, 24,  1 >;  /**< PLL enable                             */
      using PLLRDY   = RegisterBits< type, 25,  1 >;  /**< PLL clock ready flag                   */
      using PLL2ON   = RegisterBits< type, 26,  1 >;  /**< PLL2 enable           (only available on connectivity line devices!)  */
      using PLL2RDY  = RegisterBits< type, 27,  1 >;  /**< PLL2 clock ready flag (only available on connectivity line devices!)  */
      using PLL3ON   = RegisterBits< type, 28,  1 >;  /**< PLL3 enable           (only available on connectivity line devices!)  */
      using PLL3RDY  = RegisterBits< type, 29,  1 >;  /**< PLL3 clock ready flag (only available on connectivity line devices!)  */
    };

    /**
     * Clock configuration register (RCC_CFGR)
     */
    struct CFGR
    : public Register< uint32_t, base_addr + 0x4, Access::rw, 0x00000000 >
    {
      /** System clock switch  */
      struct SW
      : public RegisterBits< type,  0,  2 >
      {
        using HSI  = RegisterConst< bits_type, 0x0 >;          /**< 00: HSI selected as system clock   */
        using HSE  = RegisterConst< bits_type, 0x1 >;          /**< 01: HSE selected as system clock   */
        using PLL  = RegisterConst< bits_type, 0x2 >;          /**< 10: PLL selected as system clock   */
      };

      /** System clock switch status  */
      struct SWS
      : public RegisterBits< type,  2,  2 >
      {
        using HSI  = RegisterConst< bits_type, 0x0 >;          /**< 00: HSI selected as system clock   */
        using HSE  = RegisterConst< bits_type, 0x1 >;          /**< 01: HSE selected as system clock   */
        using PLL  = RegisterConst< bits_type, 0x2 >;          /**< 10: PLL selected as system clock   */
      };

      /** AHB prescaler  */
      struct HPRE
      : public RegisterBits< type,  4,  4 >
      {
        using DIV1    = RegisterConst< bits_type, 0x0 >;       /**< 0xxx: SYSCLK not divided      */
        using DIV2    = RegisterConst< bits_type, 0x8 >;       /**< 1000: SYSCLK divided by 2     */
        using DIV4    = RegisterConst< bits_type, 0x9 >;       /**< 1001: SYSCLK divided by 4     */
        using DIV8    = RegisterConst< bits_type, 0xa >;       /**< 1010: SYSCLK divided by 8     */
        using DIV16   = RegisterConst< bits_type, 0xb >;       /**< 1011: SYSCLK divided by 16    */
        using DIV64   = RegisterConst< bits_type, 0xc >;       /**< 1100: SYSCLK divided by 64    */
        using DIV128  = RegisterConst< bits_type, 0xd >;       /**< 1101: SYSCLK divided by 128   */
        using DIV256  = RegisterConst< bits_type, 0xe >;       /**< 1110: SYSCLK divided by 256   */
        using DIV512  = RegisterConst< bits_type, 0xf >;       /**< 1111: SYSCLK divided by 512   */
      };

      /** APB low speed prescaler (APB1)  */
      struct PPRE1
      : public RegisterBits< type,  8,  3 >
      {
        using DIV1   = RegisterConst< bits_type, 0x0 >;        /**< 0xx: HCLK not divided     */
        using DIV2   = RegisterConst< bits_type, 0x4 >;        /**< 100: HCLK divided by 2    */
        using DIV4   = RegisterConst< bits_type, 0x5 >;        /**< 101: HCLK divided by 4    */
        using DIV8   = RegisterConst< bits_type, 0x6 >;        /**< 110: HCLK divided by 8    */
        using DIV16  = RegisterConst< bits_type, 0x7 >;        /**< 111: HCLK divided by 16   */
      };

      /** APB high speed prescaler (APB2)  */
      struct PPRE2
      : public RegisterBits< type, 11,  3 >
      {
        using DIV1   = RegisterConst< bits_type, 0x0 >;        /**< 0xx: HCLK not divided     */
        using DIV2   = RegisterConst< bits_type, 0x4 >;        /**< 100: HCLK divided by 2    */
        using DIV4   = RegisterConst< bits_type, 0x5 >;        /**< 101: HCLK divided by 4    */
        using DIV8   = RegisterConst< bits_type, 0x6 >;        /**< 110: HCLK divided by 8    */
        using DIV16  = RegisterConst< bits_type, 0x7 >;        /**< 111: HCLK divided by 16   */
      };

      /** ADC prescaler  */
      struct ADCPRE
      : public RegisterBits< type, 14,  2 >
      {
        using DIV2  = RegisterConst< bits_type, 0x0 >;         /**< 00: PCLK2 divided by 2   */
        using DIV4  = RegisterConst< bits_type, 0x1 >;         /**< 01: PCLK2 divided by 4   */
        using DIV6  = RegisterConst< bits_type, 0x2 >;         /**< 10: PCLK2 divided by 6   */
        using DIV8  = RegisterConst< bits_type, 0x3 >;         /**< 11: PCLK2 divided by 8   */
      };

      /** PLL entry clock source  */
      struct PLLSRC
      : public RegisterBits< type, 16,  1 >
      {
        using HSI_DIV2  = RegisterConst< bits_type, 0x0 >;     /**< 00: HSI clock divided by 2 selected as PLL input clock   */
        using HSE       = RegisterConst< bits_type, 0x1 >;     /**< 01: HSE oscillator clock selected as PLL input clock (not available on connectivity line devices!)  */
        using PREDIV1   = RegisterConst< bits_type, 0x1 >;     /**< 01: Clock from PREDIV1 selected as PLL input clock (only available on connectivity line devices!)  */
      };

      /**
       * - HSE divider for PLL entry (low-, medium-, high- and XL-density devices)
       * - LSB of division factor PREDIV1 (connectivity line devices)
       */
      struct PLLXTPRE
      : public RegisterBits< type, 17,  1 >
      {
        /* Low-, medium-, high- and XL-density Devices */
        using HSE_DIV1  = RegisterConst< bits_type, 0x0 >;     /**< 00: HSE clock not divided (not available on connectivity line devices!)   */
        using HSE_DIV2  = RegisterConst< bits_type, 0x1 >;     /**< 01: HSE clock divided by 2 (not available on connectivity line devices!)  */

        /* Connectivity line devices */
        using PREDIV1_DIV1  = RegisterConst< bits_type, 0x0 >; /**< 00: PREDIV1 clock not divided for PLL entry (only available on connectivity line devices!)   */
        using PREDIV1_DIV2  = RegisterConst< bits_type, 0x1 >; /**< 01: PREDIV1 clock divided by 2 for PLL entry (only available on connectivity line devices!)  */
      };

      /** PLL Multiplication Factor  */
      struct PLLMUL
      : public RegisterBits< type, 18,  4 >
      {
        using MUL2    = RegisterConst< bits_type, 0x0 >;       /**< 0000: PLL input clock * 2  (not available on connectivity line devices!)  */
        using MUL3    = RegisterConst< bits_type, 0x1 >;       /**< 0001: PLL input clock * 3  (not available on connectivity line devices!)  */
        using MUL4    = RegisterConst< bits_type, 0x2 >;       /**< 0010: PLL input clock * 4  */
        using MUL5    = RegisterConst< bits_type, 0x3 >;       /**< 0011: PLL input clock * 5  */
        using MUL6    = RegisterConst< bits_type, 0x4 >;       /**< 0100: PLL input clock * 6  */
        using MUL7    = RegisterConst< bits_type, 0x5 >;       /**< 0101: PLL input clock * 7  */
        using MUL8    = RegisterConst< bits_type, 0x6 >;       /**< 0110: PLL input clock * 8  */
        using MUL9    = RegisterConst< bits_type, 0x7 >;       /**< 0111: PLL input clock * 9  */
        using MUL10   = RegisterConst< bits_type, 0x8 >;       /**< 1000: PLL input clock * 10  (not available on connectivity line devices!)  */
        using MUL11   = RegisterConst< bits_type, 0x9 >;       /**< 1001: PLL input clock * 11  (not available on connectivity line devices!)  */
        using MUL12   = RegisterConst< bits_type, 0xa >;       /**< 1010: PLL input clock * 12  (not available on connectivity line devices!)  */
        using MUL13   = RegisterConst< bits_type, 0xb >;       /**< 1011: PLL input clock * 13  (not available on connectivity line devices!)  */
        using MUL14   = RegisterConst< bits_type, 0xc >;       /**< 1100: PLL input clock * 14  (not available on connectivity line devices!)  */
        using MUL15   = RegisterConst< bits_type, 0xd >;       /**< 1101: PLL input clock * 15  (not available on connectivity line devices!)  */
        using MUL16   = RegisterConst< bits_type, 0xe >;       /**< 1110: PLL input clock * 16  (not available on connectivity line devices!)  */
        using MUL6_5  = RegisterConst< bits_type, 0xd >;       /**< 1101: PLL input clock * 6.5 (only available on connectivity line devices!)  */
      };

      /** USB OTG FS prescaler (only available on connectivity line devices!)  */
      struct OTGFSPRE
      : public RegisterBits< type, 22,  1 >
      {
        using DIV3  = RegisterConst< bits_type, 0x0 >;
        using DIV2  = RegisterConst< bits_type, 0x1 >;
      };

      /** USB prescaler (not available on connectivity line devices!) */
      struct USBPRE
      : public RegisterBits< type, 22,  1 >
      {
        using DIV1_5  = RegisterConst< bits_type, 0x0 >;
        using DIV1    = RegisterConst< bits_type, 0x1 >;
      };

      /** Microcontroller Clock Output  */
      struct MCO
      : public RegisterBits< type, 24,  4 >
      {
        using NOCLOCK    = RegisterConst< bits_type, 0x0 >;   /**< 00xx: No clock                          */
        using SYSCLK     = RegisterConst< bits_type, 0x4 >;   /**< 0100: System clock selected             */
        using HSI        = RegisterConst< bits_type, 0x5 >;   /**< 0101: HSI clock selected                */
        using HSE        = RegisterConst< bits_type, 0x6 >;   /**< 0110: HSE clock selected                */
        using PLL_DIV2   = RegisterConst< bits_type, 0x7 >;   /**< 0111: PLL clock divided by 2 selected   */
        using PLL2       = RegisterConst< bits_type, 0x8 >;   /**< 1000: PLL2 clock selected (only available on connectivity line devices!)                                             */
        using PLL3_DIV2  = RegisterConst< bits_type, 0x9 >;   /**< 1001: PLL3 clock divided by 2 selected (only available on connectivity line devices!)                                 */
        using Ext_HSE    = RegisterConst< bits_type, 0xa >;   /**< 1010: XT1 external 3-25 MHz oscillator clock selected (for Ethernet) (only available on connectivity line devices!)   */
        using PLL3       = RegisterConst< bits_type, 0xb >;   /**< 1011: PLL3 clock selected (for Ethernet) (only available on connectivity line devices!)                               */
      };
    };

    /**
     * Clock interrupt register (RCC_CIR)
     */
    struct CIR
    : public Register< uint32_t, base_addr + 0x8, Access::rw, 0x00000000 >
    {
      using LSIRDYF    = RegisterBits< type,  0,  1 >;  /**< LSI Ready Interrupt flag  */
      using LSERDYF    = RegisterBits< type,  1,  1 >;  /**< LSE Ready Interrupt flag  */
      using HSIRDYF    = RegisterBits< type,  2,  1 >;  /**< HSI Ready Interrupt flag  */
      using HSERDYF    = RegisterBits< type,  3,  1 >;  /**< HSE Ready Interrupt flag  */
      using PLLRDYF    = RegisterBits< type,  4,  1 >;  /**< PLL Ready Interrupt flag  */
      using PLL2RDYF   = RegisterBits< type,  5,  1 >;  /**< PLL2 Ready Interrupt flag (only available on connectivity line devices!)  */
      using PLL3RDYF   = RegisterBits< type,  6,  1 >;  /**< PLL3 Ready Interrupt flag (only available on connectivity line devices!)  */
      using CSSF       = RegisterBits< type,  7,  1 >;  /**< Clock Security System Interrupt flag   */
      using LSIRDYIE   = RegisterBits< type,  8,  1 >;  /**< LSI Ready Interrupt Enable  */
      using LSERDYIE   = RegisterBits< type,  9,  1 >;  /**< LSE Ready Interrupt Enable  */
      using HSIRDYIE   = RegisterBits< type, 10,  1 >;  /**< HSI Ready Interrupt Enable  */
      using HSERDYIE   = RegisterBits< type, 11,  1 >;  /**< HSE Ready Interrupt Enable  */
      using PLLRDYIE   = RegisterBits< type, 12,  1 >;  /**< PLL Ready Interrupt Enable  */
      using PLL2RDYIE  = RegisterBits< type, 13,  1 >;  /**< PLL2 Ready Interrupt Enable (only available on connectivity line devices!)  */
      using PLL3RDYIE  = RegisterBits< type, 14,  1 >;  /**< PLL3 Ready Interrupt Enable (only available on connectivity line devices!)  */
      using LSIRDYC    = RegisterBits< type, 16,  1 >;  /**< LSI Ready Interrupt Clear  */
      using LSERDYC    = RegisterBits< type, 17,  1 >;  /**< LSE Ready Interrupt Clear  */
      using HSIRDYC    = RegisterBits< type, 18,  1 >;  /**< HSI Ready Interrupt Clear  */
      using HSERDYC    = RegisterBits< type, 19,  1 >;  /**< HSE Ready Interrupt Clear  */
      using PLLRDYC    = RegisterBits< type, 20,  1 >;  /**< PLL Ready Interrupt Clear  */
      using PLL2RDYC   = RegisterBits< type, 21,  1 >;  /**< PLL2 Ready Interrupt Clear (only available on connectivity line devices!)  */
      using PLL3RDYC   = RegisterBits< type, 22,  1 >;  /**< PLL3 Ready Interrupt Clear (only available on connectivity line devices!)  */
      using CSSC       = RegisterBits< type, 23,  1 >;  /**< Clock security system interrupt clear  */
    };

    /**
     * APB2 peripheral reset register (RCC_APB2RSTR)
     */
    struct APB2RSTR
    : public Register< uint32_t, base_addr + 0xc, Access::rw, 0x000000000 >
    {
      using AFIORST    = RegisterBits< type,  0,  1 >;  /**< Alternate function I/O reset  */
      using IOPARST    = RegisterBits< type,  2,  1 >;  /**< IO port A reset               */
      using IOPBRST    = RegisterBits< type,  3,  1 >;  /**< IO port B reset               */
      using IOPCRST    = RegisterBits< type,  4,  1 >;  /**< IO port C reset               */
      using IOPDRST    = RegisterBits< type,  5,  1 >;  /**< IO port D reset               */
      using IOPERST    = RegisterBits< type,  6,  1 >;  /**< IO port E reset (not available on low-density devices!)  */
      using IOPFRST    = RegisterBits< type,  7,  1 >;  /**< IO port F reset (only available on high- and XL-density devices!)     */
      using IOPGRST    = RegisterBits< type,  8,  1 >;  /**< IO port G reset (only available on high- and XL-density devices!)     */
      using ADC1RST    = RegisterBits< type,  9,  1 >;  /**< ADC 1 interface reset         */
      using ADC2RST    = RegisterBits< type, 10,  1 >;  /**< ADC 2 interface reset (not available on low- and medium-density devices!) */
      using TIM1RST    = RegisterBits< type, 11,  1 >;  /**< TIM1 timer reset              */
      using SPI1RST    = RegisterBits< type, 12,  1 >;  /**< SPI 1 reset                   */
      using TIM8RST    = RegisterBits< type, 13,  1 >;  /**< TIM8 Timer reset (only available on high- and XL-density devices!)      */
      using USART1RST  = RegisterBits< type, 14,  1 >;  /**< USART1 reset                  */
      using ADC3RST    = RegisterBits< type, 15,  1 >;  /**< ADC3 interface reset (only available on high- and XL-density devices!)  */
      using TIM15RST   = RegisterBits< type, 16,  1 >;  /**< TIM15 Timer reset (only available on low- and medium-density devices!)  */
      using TIM16RST   = RegisterBits< type, 17,  1 >;  /**< TIM16 Timer reset (only available on low- and medium-density devices!)  */
      using TIM17RST   = RegisterBits< type, 18,  1 >;  /**< TIM17 Timer reset (only available on low- and medium-density devices!)  */
      using TIM9RST    = RegisterBits< type, 19,  1 >;  /**< TIM9 Timer reset (only available on XL-density devices!)  */
      using TIM10RST   = RegisterBits< type, 20,  1 >;  /**< TIM10 Timer reset (only available on XL-density devices!)  */
      using TIM11RST   = RegisterBits< type, 21,  1 >;  /**< TIM11 Timer reset (only available on XL-density devices!)  */
    };

    /**
     * APB1 peripheral reset register (RCC_APB1RSTR)
     */
    struct APB1RSTR
    : public Register< uint32_t, base_addr + 0x10, Access::rw, 0x00000000 >
    {
      using TIM2RST    = RegisterBits< type,  0,  1 >;  /**< Timer 2 reset         */
      using TIM3RST    = RegisterBits< type,  1,  1 >;  /**< Timer 3 reset         */
      using TIM4RST    = RegisterBits< type,  2,  1 >;  /**< Timer 4 reset (not available on low-density devices!)  */
      using TIM5RST    = RegisterBits< type,  3,  1 >;  /**< Timer 5 reset         */
      using TIM6RST    = RegisterBits< type,  4,  1 >;  /**< Timer 6 reset         */
      using TIM7RST    = RegisterBits< type,  5,  1 >;  /**< Timer 7 reset         */
      using TIM12RST   = RegisterBits< type,  6,  1 >;  /**< Timer 12 reset (only available on XL-density devices!)  */
      using TIM13RST   = RegisterBits< type,  7,  1 >;  /**< Timer 13 reset (only available on XL-density devices!)  */
      using TIM14RST   = RegisterBits< type,  8,  1 >;  /**< Timer 14 reset (only available on XL-density devices!)  */
      using WWDGRST    = RegisterBits< type, 11,  1 >;  /**< Window Watchdog reset  */
      using SPI2RST    = RegisterBits< type, 14,  1 >;  /**< SPI 2 reset (not available on low-density devices!)     */
      using SPI3RST    = RegisterBits< type, 15,  1 >;  /**< SPI 3 reset            */
      using USART2RST  = RegisterBits< type, 17,  1 >;  /**< USART 2 reset          */
      using USART3RST  = RegisterBits< type, 18,  1 >;  /**< RUSART 3 reset (not available on low-density devices!)  */
      using UART4RST   = RegisterBits< type, 19,  1 >;  /**< UART 4 reset           */
      using UART5RST   = RegisterBits< type, 20,  1 >;  /**< UART 5 reset           */
      using I2C1RST    = RegisterBits< type, 21,  1 >;  /**< I2C 1 reset            */
      using I2C2RST    = RegisterBits< type, 22,  1 >;  /**< I2C 2 reset (not available on low-density devices!)     */
      using USBRST     = RegisterBits< type, 23,  1 >;  /**< USB Device reset (not available on connectivity line devices!)  */
      using CAN1RST    = RegisterBits< type, 25,  1 >;  /**< CAN1 reset (not available on low- and medium-density devices!)  */
      using CAN2RST    = RegisterBits< type, 26,  1 >;  /**< CAN2 reset (only available on connectivity line devices!)       */
      using BKPRST     = RegisterBits< type, 27,  1 >;  /**< Backup interface reset  */
      using PWRRST     = RegisterBits< type, 28,  1 >;  /**< Power interface reset   */
      using DACRST     = RegisterBits< type, 29,  1 >;  /**< DAC interface reset     */
      using CECRST     = RegisterBits< type, 30,  1 >;  /**< CEC interface reset     */
    };

    /**
     * AHB Peripheral Clock enable register (RCC_AHBENR)
     */
    struct AHBENR
    : public Register< uint32_t, base_addr + 0x14, Access::rw, 0x00000014 >
    {
      using DMA1EN      = RegisterBits< type,  0,  1 >;  /**< DMA1 clock enable             */
      using DMA2EN      = RegisterBits< type,  1,  1 >;  /**< DMA2 clock enable (only available on high-density and connectivity line devices!)  */
      using SRAMEN      = RegisterBits< type,  2,  1 >;  /**< SRAM interface clock enable   */
      using FLITFEN     = RegisterBits< type,  4,  1 >;  /**< FLITF clock enable            */
      using CRCEN       = RegisterBits< type,  6,  1 >;  /**< CRC clock enable              */
      using FSMCEN      = RegisterBits< type,  8,  1 >;  /**< FSMC clock enable (only available on high- and XL-density devices!)  */
      using SDIOEN      = RegisterBits< type, 10,  1 >;  /**< SDIO clock enable (only available on high- and XL-density devices!) */
      using OTGFSEN     = RegisterBits< type, 12,  1 >;  /**< USB OTG FS clock enable (only available on connectivity line devices!)      */
      using ETHMACEN    = RegisterBits< type, 14,  1 >;  /**< Ethernet MAC clock enable (only available on connectivity line devices!)    */
      using ETHMACTXEN  = RegisterBits< type, 15,  1 >;  /**< Ethernet MAC TX clock enable (only available on connectivity line devices!) */
      using ETHMACRXEN  = RegisterBits< type, 16,  1 >;  /**< Ethernet MAC RX clock enable (only available on connectivity line devices!) */
    };

    /**
     * APB2 peripheral clock enable register (RCC_APB2ENR)
     */
    struct APB2ENR
    : public Register< uint32_t, base_addr + 0x18, Access::rw, 0x00000000 >
    {
      using AFIOEN    = RegisterBits< type,  0,  1 >;  /**< Alternate function I/O clock enable  */
      using IOPAEN    = RegisterBits< type,  2,  1 >;  /**< I/O port A clock enable              */
      using IOPBEN    = RegisterBits< type,  3,  1 >;  /**< I/O port B clock enable              */
      using IOPCEN    = RegisterBits< type,  4,  1 >;  /**< I/O port C clock enable              */
      using IOPDEN    = RegisterBits< type,  5,  1 >;  /**< I/O port D clock enable              */
      using IOPEEN    = RegisterBits< type,  6,  1 >;  /**< I/O port E clock enable (not available on low-density devices!)  */
      using IOPFEN    = RegisterBits< type,  7,  1 >;  /**< I/O port F clock enable (only available on high- and XL-density devices!)  */
      using IOPGEN    = RegisterBits< type,  8,  1 >;  /**< I/O port G clock enable (only available on high- and XL-density devices!)  */
      using ADC1EN    = RegisterBits< type,  9,  1 >;  /**< ADC 1 interface clock enable         */
      using ADC2EN    = RegisterBits< type, 10,  1 >;  /**< ADC 2 interface clock enable (not available on low- and medium-density devices!)  */
      using TIM1EN    = RegisterBits< type, 11,  1 >;  /**< TIM1 Timer clock enable              */
      using SPI1EN    = RegisterBits< type, 12,  1 >;  /**< SPI 1 clock enable                   */
      using TIM8EN    = RegisterBits< type, 13,  1 >;  /**< TIM8 Timer clock enable (only available on high- and XL-density devices!)  */
      using USART1EN  = RegisterBits< type, 14,  1 >;  /**< USART1 clock enable                  */
      using ADC3EN    = RegisterBits< type, 15,  1 >;  /**< ADC 3 interface clock enable (only available on high- and XL-density devices!)  */
      using TIM15EN   = RegisterBits< type, 16,  1 >;  /**< TIM15 Timer clock enable (only available on low- and medium-density devices!)   */
      using TIM16EN   = RegisterBits< type, 17,  1 >;  /**< TIM16 Timer clock enable (only available on low- and medium-density devices!)   */
      using TIM17EN   = RegisterBits< type, 18,  1 >;  /**< TIM17 Timer clock enable (only available on low- and medium-density devices!)   */
      using TIM9EN    = RegisterBits< type, 19,  1 >;  /**< TIM9 Timer clock enable (only available on XL-density devices!)   */
      using TIM10EN   = RegisterBits< type, 20,  1 >;  /**< TIM10 Timer clock enable (only available on XL-density devices!)  */
      using TIM11EN   = RegisterBits< type, 21,  1 >;  /**< TIM11 Timer clock enable (only available on XL-density devices!)  */
    };

    /**
     * APB1 peripheral clock enable register (RCC_APB1ENR)
     */
    struct APB1ENR
    : public Register< uint32_t, base_addr + 0x1c, Access::rw, 0x00000000 >
    {
      using TIM2EN    = RegisterBits< type,  0,  1 >;  /**< Timer 2 clock enable           */
      using TIM3EN    = RegisterBits< type,  1,  1 >;  /**< Timer 3 clock enable           */
      using TIM4EN    = RegisterBits< type,  2,  1 >;  /**< Timer 4 clock enable (not available on low-density devices!)          */
      using TIM5EN    = RegisterBits< type,  3,  1 >;  /**< Timer 5 clock enable           */
      using TIM6EN    = RegisterBits< type,  4,  1 >;  /**< Timer 6 clock enable           */
      using TIM7EN    = RegisterBits< type,  5,  1 >;  /**< Timer 7 clock enable           */
      using TIM12EN   = RegisterBits< type,  6,  1 >;  /**< Timer 12 clock enable (only available on XL-density devices!)  */
      using TIM13EN   = RegisterBits< type,  7,  1 >;  /**< Timer 13 clock enable (only available on XL-density devices!)  */
      using TIM14EN   = RegisterBits< type,  8,  1 >;  /**< Timer 14 clock enable (only available on XL-density devices!)  */
      using WWDGEN    = RegisterBits< type, 11,  1 >;  /**< Window watchdog clock enable   */
      using SPI2EN    = RegisterBits< type, 14,  1 >;  /**< SPI 2 clock enable (not available on low-density devices!)            */
      using SPI3EN    = RegisterBits< type, 15,  1 >;  /**< SPI 3 clock enable             */
      using USART2EN  = RegisterBits< type, 17,  1 >;  /**< USART 2 clock enable           */
      using USART3EN  = RegisterBits< type, 18,  1 >;  /**< USART 3 clock enable (not available on low-density devices!)          */
      using UART4EN   = RegisterBits< type, 19,  1 >;  /**< UART 4 clock enable            */
      using UART5EN   = RegisterBits< type, 20,  1 >;  /**< UART 5 clock enable            */
      using I2C1EN    = RegisterBits< type, 21,  1 >;  /**< I2C 1 clock enable             */
      using I2C2EN    = RegisterBits< type, 22,  1 >;  /**< I2C 2 clock enable (not available on low-density devices!)            */
      using USBEN     = RegisterBits< type, 23,  1 >;  /**< USB device clock enable (not available on connectivity line devices!)   */
      using CAN1EN    = RegisterBits< type, 25,  1 >;  /**< CAN1 clock enable (not available on low- and medium-density devices!)  */
      using CAN2EN    = RegisterBits< type, 26,  1 >;  /**< CAN2 clock enable (only available on connectivity line devices!)       */
      using BKPEN     = RegisterBits< type, 27,  1 >;  /**< Backup interface clock enable  */
      using PWREN     = RegisterBits< type, 28,  1 >;  /**< Power interface clock enable   */
      using DACEN     = RegisterBits< type, 29,  1 >;  /**< DAC interface clock enable     */
      using CECEN     = RegisterBits< type, 30,  1 >;  /**< CEC interface clock enable     */

    };

    /**
     * Backup domain control register (RCC_BDCR)
     */
    struct BDCR
    : public Register< uint32_t, base_addr + 0x20, Access::rw, 0x00000000 >
    {
      using LSEON   = RegisterBits< type,  0,  1 >;  /**< External Low Speed oscillator enable  */
      using LSERDY  = RegisterBits< type,  1,  1 >;  /**< External Low Speed oscillator ready   */
      using LSEBYP  = RegisterBits< type,  2,  1 >;  /**< External Low Speed oscillator bypass  */

      /** RTC clock source selection */
      struct RTCSEL
      : public RegisterBits< type,  8,  2 >
      {
        using LSE = RegisterConst< bits_type, 0x1 >;  /**< 01: LSE oscillator clock used as RTC clock                  */
        using LSI = RegisterConst< bits_type, 0x2 >;  /**< 10: LSI oscillator clock used as RTC clock                  */
        using HSE = RegisterConst< bits_type, 0x3 >;  /**< 11: HSE oscillator clock divided by 128 used as RTC clock   */
      };

      using RTCEN   = RegisterBits< type, 15,  1 >;  /**< RTC clock enable                      */
      using BDRST   = RegisterBits< type, 16,  1 >;  /**< Backup domain software reset          */
    };

    /**
     * Control/status register (RCC_CSR)
     */
    struct CSR
    : public Register< uint32_t, base_addr + 0x24, Access::rw, 0x0C000000 >
    {
      using LSION     = RegisterBits< type,  0,  1 >;  /**< Internal low speed oscillator enable  */
      using LSIRDY    = RegisterBits< type,  1,  1 >;  /**< Internal low speed oscillator ready   */
      using RMVF      = RegisterBits< type, 24,  1 >;  /**< Remove reset flag                     */
      using PINRSTF   = RegisterBits< type, 26,  1 >;  /**< PIN reset flag                        */
      using PORRSTF   = RegisterBits< type, 27,  1 >;  /**< POR/PDR reset flag                    */
      using SFTRSTF   = RegisterBits< type, 28,  1 >;  /**< Software reset flag                   */
      using IWDGRSTF  = RegisterBits< type, 29,  1 >;  /**< Independent watchdog reset flag       */
      using WWDGRSTF  = RegisterBits< type, 30,  1 >;  /**< Window watchdog reset flag            */
      using LPWRRSTF  = RegisterBits< type, 31,  1 >;  /**< Low-power reset flag                  */
    };

    /**
     * AHB peripheral clock reset register (RCC_AHBRSTR)
     * (only available on connectivity line devices!)
     */
    struct AHBRSTR
    : public Register< uint32_t, base_addr + 0x28, Access::rw, 0x00000000 >
    {
      using OTGFSRST   = RegisterBits< type, 12,  1 >;  /**< USB OTG FS reset    */
      using ETHMACRST  = RegisterBits< type, 14,  1 >;  /**< Ethernet MAC reset  */
    };

    /**
     * Clock configuration register2 (RCC_CFGR2)
     * (only available on connectivity line devices!)
     */
    struct CFGR2
    : public Register< uint32_t, base_addr + 0x2c, Access::rw, 0x00000000 >
    {
      using PREDIV1     = RegisterBits< type,  0,  4 >;  /**< PREDIV1 division factor     */
      using PREDIV2     = RegisterBits< type,  4,  4 >;  /**< PREDIV2 division factor     */
      using PLL2MUL     = RegisterBits< type,  8,  4 >;  /**< PLL2 Multiplication Factor  */
      using PLL3MUL     = RegisterBits< type, 12,  4 >;  /**< PLL3 Multiplication Factor  */

      /** PREDIV1 entry clock source  */
      struct PREDIV1SRC
      : public RegisterBits< type, 16,  1 >
      {
        using HSE   = RegisterConst< bits_type, 0x0 >;  /**< 0: HSE oscillator clock selected as PREDIV1 clock entry   */
        using PLL2  = RegisterConst< bits_type, 0x1 >;  /**< 1: PLL2 selected as PREDIV1 clock entry                   */
      };

      /** I2S2 clock source  */
      struct I2S2SRC
      : public RegisterBits< type, 17,  1 >
      {
        using SYSCLK  = RegisterConst< bits_type, 0x0 >;  /**< 0: System clock (SYSCLK) selected as I2Sx clock entry   */
        using PLL3    = RegisterConst< bits_type, 0x1 >;  /**< 1: PLL3 VCO clock selected as I2Sx clock entry          */
      };

      /** I2S3 clock source  */
      struct I2S3SRC
      : public RegisterBits< type, 18,  1 >
      {
        using SYSCLK  = RegisterConst< bits_type, 0x0 >;  /**< 0: System clock (SYSCLK) selected as I2S3 clock entry   */
        using PLL3    = RegisterConst< bits_type, 0x1 >;  /**< 1: PLL3 VCO clock selected as I2S3 clock entry          */
      };
    };
  };
}

#endif // ARCH_REG_RCC_HPP_INCLUDED
