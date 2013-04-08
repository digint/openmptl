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
    static constexpr reg_addr_t base_addr = 0x40023800;
    static constexpr const char * name_str = "RCC";

    /**
     * Clock control register
     */
    struct CR
    : public Register< uint32_t, base_addr + 0x00, Access::rw, 0x00000083 >
    {
      using PLLI2SRDY  = RegisterBits< type, 27,  1 >;  /**< PLLI2S clock ready flag                */
      using PLLI2SON   = RegisterBits< type, 26,  1 >;  /**< PLLI2S enable                          */
      using PLLRDY     = RegisterBits< type, 25,  1 >;  /**< Main PLL clock ready flag              */
      using PLLON      = RegisterBits< type, 24,  1 >;  /**< Main PLL enable                        */
      using CSSON      = RegisterBits< type, 19,  1 >;  /**< Clock security system enable           */
      using HSEBYP     = RegisterBits< type, 18,  1 >;  /**< HSE clock bypass                       */
      using HSERDY     = RegisterBits< type, 17,  1 >;  /**< HSE clock ready flag                   */
      using HSEON      = RegisterBits< type, 16,  1 >;  /**< HSE clock enable                       */
      using HSICAL     = RegisterBits< type,  8,  8 >;  /**< Internal high-speed clock calibration  */
      using HSITRIM    = RegisterBits< type,  3,  5 >;  /**< Internal high-speed clock trimming     */
      using HSIRDY     = RegisterBits< type,  1,  1 >;  /**< Internal high-speed clock ready flag   */
      using HSION      = RegisterBits< type,  0,  1 >;  /**< Internal high-speed clock enable       */
    };

    /**
     * PLL configuration register
     */
    struct PLLCFGR
    : public Register< uint32_t, base_addr + 0x04, Access::rw, 0x24003010 >
    {
      using PLLQ    = RegisterBits< type, 24,  4 >;  /**< Main PLL division factor for USB OTG FS, SDIO and random number generator clocks  */
      using PLLSRC  = RegisterBits< type, 22,  1 >;  /**< Main PLL and audio PLL entry clock source                                         */
      using PLLP    = RegisterBits< type, 16,  2 >;  /**< Main PLL division factor for main system clock                                    */
      using PLLN    = RegisterBits< type,  6,  9 >;  /**< Main PLL multiplication factor for VCO                                            */
      using PLLM    = RegisterBits< type,  0,  6 >;  /**< Division factor for the main PLL and audio PLL input clock                        */
    };

    /**
     * Clock configuration register
     */
    struct CFGR
    : public Register< uint32_t, base_addr + 0x08, Access::rw, 0x00000000 >
    {
      /** APB1/2 prescaler  */
      template<typename Rb>
      struct __PPREx
      : public Rb
      {
        using DIV1   = RegisterConst< Rb, 0x0 >;  /**< 0xx: AHB clock not divided     */
        using DIV2   = RegisterConst< Rb, 0x4 >;  /**< 100: AHB clock divided by 2    */
        using DIV4   = RegisterConst< Rb, 0x5 >;  /**< 101: AHB clock divided by 4    */
        using DIV8   = RegisterConst< Rb, 0x6 >;  /**< 110: AHB clock divided by 8    */
        using DIV16  = RegisterConst< Rb, 0x7 >;  /**< 111: AHB clock divided by 16   */
      };

      /** AHB prescaler  */
      template<typename Rb>
      struct __HPRE
      : public Rb
      {
        using DIV1    = RegisterConst< Rb, 0x0 >;  /**< 0xxx: system clock not divided      */
        using DIV2    = RegisterConst< Rb, 0x8 >;  /**< 1000: system clock divided by 2     */
        using DIV4    = RegisterConst< Rb, 0x9 >;  /**< 1001: system clock divided by 4     */
        using DIV8    = RegisterConst< Rb, 0xA >;  /**< 1010: system clock divided by 8     */
        using DIV16   = RegisterConst< Rb, 0xB >;  /**< 1011: system clock divided by 16    */
        using DIV64   = RegisterConst< Rb, 0xC >;  /**< 1100: system clock divided by 64    */
        using DIV128  = RegisterConst< Rb, 0xD >;  /**< 1101: system clock divided by 128   */
        using DIV256  = RegisterConst< Rb, 0xE >;  /**< 1110: system clock divided by 256   */
        using DIV512  = RegisterConst< Rb, 0xF >;  /**< 1111: system clock divided by 512   */
      };

      /** System clock Switch  */
      template<typename Rb>
      struct __SWx
      : public Rb
      {
        using HSI  = RegisterConst< Rb, 0x0 >;  /**< 00: HSI selected as system clock   */
        using HSE  = RegisterConst< Rb, 0x1 >;  /**< 01: HSE selected as system clock   */
        using PLL  = RegisterConst< Rb, 0x2 >;  /**< 10: PLL selected as system clock   */
      };

      using MCO2     =           RegisterBits< type, 30,  2 >;  /**< Microcontroller clock output 2     */
      using MCO2PRE  =           RegisterBits< type, 27,  3 >;  /**< MCO2 prescaler                     */
      using MCO1PRE  =           RegisterBits< type, 24,  3 >;  /**< MCO1 prescaler                     */
      using I2SSRC   =           RegisterBits< type, 23,  1 >;  /**< I2S clock selection                */
      using MCO1     =           RegisterBits< type, 21,  2 >;  /**< Microcontroller clock output 1     */
      using RTCPRE   =           RegisterBits< type, 16,  5 >;  /**< HSE division factor for RTC clock  */
      using PPRE2    = __PPREx < RegisterBits< type, 13,  3 > >;  /**< APB high-speed prescaler (APB2)    */
      using PPRE1    = __PPREx < RegisterBits< type, 10,  3 > >;  /**< APB low-speed prescaler (APB1)     */
      using HPRE     = __HPRE  < RegisterBits< type,  4,  4 > >;  /**< AHB prescaler                      */
      using SWS      = __SWx   < RegisterBits< type,  2,  2 > >;  /**< System clock switch status         */
      using SW       = __SWx   < RegisterBits< type,  0,  2 > >;  /**< System clock switch                */
    };

    /**
     * Clock interrupt register
     */
    struct CIR
    : public Register< uint32_t, base_addr + 0x0C, Access::rw, 0x00000000 >
    {
      using CSSC         = RegisterBits< type, 23,  1 >;  /**< Clock security system interrupt clear  */
      using PLLI2SRDYC   = RegisterBits< type, 21,  1 >;  /**< PLLI2S ready interrupt clear           */
      using PLLRDYC      = RegisterBits< type, 20,  1 >;  /**< Main PLL ready interrupt clear         */
      using HSERDYC      = RegisterBits< type, 19,  1 >;  /**< HSE ready interrupt clear              */
      using HSIRDYC      = RegisterBits< type, 18,  1 >;  /**< HSI ready interrupt clear              */
      using LSERDYC      = RegisterBits< type, 17,  1 >;  /**< LSE ready interrupt clear              */
      using LSIRDYC      = RegisterBits< type, 16,  1 >;  /**< LSI ready interrupt clear              */
      using PLLI2SRDYIE  = RegisterBits< type, 13,  1 >;  /**< PLLI2S ready interrupt enable          */
      using PLLRDYIE     = RegisterBits< type, 12,  1 >;  /**< Main PLL ready interrupt enable        */
      using HSERDYIE     = RegisterBits< type, 11,  1 >;  /**< HSE ready interrupt enable             */
      using HSIRDYIE     = RegisterBits< type, 10,  1 >;  /**< HSI ready interrupt enable             */
      using LSERDYIE     = RegisterBits< type,  9,  1 >;  /**< LSE ready interrupt enable             */
      using LSIRDYIE     = RegisterBits< type,  8,  1 >;  /**< LSI ready interrupt enable             */
      using CSSF         = RegisterBits< type,  7,  1 >;  /**< Clock security system interrupt flag   */
      using PLLI2SRDYF   = RegisterBits< type,  5,  1 >;  /**< PLLI2S ready interrupt flag            */
      using PLLRDYF      = RegisterBits< type,  4,  1 >;  /**< Main PLL ready interrupt flag          */
      using HSERDYF      = RegisterBits< type,  3,  1 >;  /**< HSE ready interrupt flag               */
      using HSIRDYF      = RegisterBits< type,  2,  1 >;  /**< HSI ready interrupt flag               */
      using LSERDYF      = RegisterBits< type,  1,  1 >;  /**< LSE ready interrupt flag               */
      using LSIRDYF      = RegisterBits< type,  0,  1 >;  /**< LSI ready interrupt flag               */
    };

    /**
     * AHB1 peripheral reset register
     */
    struct AHB1RSTR
    : public Register< uint32_t, base_addr + 0x10, Access::rw, 0x00000000 >
    {
      using OTGHSRST   = RegisterBits< type, 29,  1 >;  /**< USB OTG HS module reset  */
      using ETHMACRST  = RegisterBits< type, 25,  1 >;  /**< Ethernet MAC reset       */
      using DMA2RST    = RegisterBits< type, 22,  1 >;  /**< DMA2 reset               */
      using DMA1RST    = RegisterBits< type, 21,  1 >;  /**< DMA1 reset               */
      using CRCRST     = RegisterBits< type, 12,  1 >;  /**< CRC reset                */
      using GPIOIRST   = RegisterBits< type,  8,  1 >;  /**< IO port I reset          */
      using GPIOHRST   = RegisterBits< type,  7,  1 >;  /**< IO port H reset          */
      using GPIOGRST   = RegisterBits< type,  6,  1 >;  /**< IO port G reset          */
      using GPIOFRST   = RegisterBits< type,  5,  1 >;  /**< IO port F reset          */
      using GPIOERST   = RegisterBits< type,  4,  1 >;  /**< IO port E reset          */
      using GPIODRST   = RegisterBits< type,  3,  1 >;  /**< IO port D reset          */
      using GPIOCRST   = RegisterBits< type,  2,  1 >;  /**< IO port C reset          */
      using GPIOBRST   = RegisterBits< type,  1,  1 >;  /**< IO port B reset          */
      using GPIOARST   = RegisterBits< type,  0,  1 >;  /**< IO port A reset          */
    };

    /**
     * AHB2 peripheral reset register
     */
    struct AHB2RSTR
    : public Register< uint32_t, base_addr + 0x14, Access::rw, 0x00000000 >
    {
      using OTGFSRST  = RegisterBits< type,  7,  1 >;  /**< USB OTG FS module reset               */
      using RNGRST    = RegisterBits< type,  6,  1 >;  /**< Random number generator module reset  */
      using DCMIRST   = RegisterBits< type,  0,  1 >;  /**< Camera interface reset                */
    };

    /**
     * AHB3 peripheral reset register
     */
    struct AHB3RSTR
    : public Register< uint32_t, base_addr + 0x18, Access::rw, 0x00000000 >
    {
      using FSMCRST  = RegisterBits< type,  0,  1 >;  /**< Flexible static memory controller module reset  */
    };

    /**
     * APB1 peripheral reset register
     */
    struct APB1RSTR
    : public Register< uint32_t, base_addr + 0x20, Access::rw, 0x00000000 >
    {
      using DACRST    = RegisterBits< type, 29,  1 >;  /**< DAC reset              */
      using PWRRST    = RegisterBits< type, 28,  1 >;  /**< Power interface reset  */
      using CAN2RST   = RegisterBits< type, 26,  1 >;  /**< CAN2 reset             */
      using CAN1RST   = RegisterBits< type, 25,  1 >;  /**< CAN1 reset             */
      using I2C3RST   = RegisterBits< type, 23,  1 >;  /**< I2C3 reset             */
      using I2C2RST   = RegisterBits< type, 22,  1 >;  /**< I2C 2 reset            */
      using I2C1RST   = RegisterBits< type, 21,  1 >;  /**< I2C 1 reset            */
      using UART5RST  = RegisterBits< type, 20,  1 >;  /**< USART 5 reset          */
      using UART4RST  = RegisterBits< type, 19,  1 >;  /**< USART 4 reset          */
      using UART3RST  = RegisterBits< type, 18,  1 >;  /**< USART 3 reset          */
      using UART2RST  = RegisterBits< type, 17,  1 >;  /**< USART 2 reset          */
      using SPI3RST   = RegisterBits< type, 15,  1 >;  /**< SPI 3 reset            */
      using SPI2RST   = RegisterBits< type, 14,  1 >;  /**< SPI 2 reset            */
      using WWDGRST   = RegisterBits< type, 11,  1 >;  /**< Window watchdog reset  */
      using TIM14RST  = RegisterBits< type,  8,  1 >;  /**< TIM14 reset            */
      using TIM13RST  = RegisterBits< type,  7,  1 >;  /**< TIM13 reset            */
      using TIM12RST  = RegisterBits< type,  6,  1 >;  /**< TIM12 reset            */
      using TIM7RST   = RegisterBits< type,  5,  1 >;  /**< TIM7 reset             */
      using TIM6RST   = RegisterBits< type,  4,  1 >;  /**< TIM6 reset             */
      using TIM5RST   = RegisterBits< type,  3,  1 >;  /**< TIM5 reset             */
      using TIM4RST   = RegisterBits< type,  2,  1 >;  /**< TIM4 reset             */
      using TIM3RST   = RegisterBits< type,  1,  1 >;  /**< TIM3 reset             */
      using TIM2RST   = RegisterBits< type,  0,  1 >;  /**< TIM2 reset             */
    };

    /**
     * APB2 peripheral reset register
     */
    struct APB2RSTR
    : public Register< uint32_t, base_addr + 0x24, Access::rw, 0x00000000 >
    {
      using TIM11RST   = RegisterBits< type, 18,  1 >;  /**< TIM11 reset                               */
      using TIM10RST   = RegisterBits< type, 17,  1 >;  /**< TIM10 reset                               */
      using TIM9RST    = RegisterBits< type, 16,  1 >;  /**< TIM9 reset                                */
      using SYSCFGRST  = RegisterBits< type, 14,  1 >;  /**< System configuration controller reset     */
      using SPI1RST    = RegisterBits< type, 12,  1 >;  /**< SPI 1 reset                               */
      using SDIORST    = RegisterBits< type, 11,  1 >;  /**< SDIO reset                                */
      using ADCRST     = RegisterBits< type,  8,  1 >;  /**< ADC interface reset (common to all ADCs)  */
      using USART6RST  = RegisterBits< type,  5,  1 >;  /**< USART6 reset                              */
      using USART1RST  = RegisterBits< type,  4,  1 >;  /**< USART1 reset                              */
      using TIM8RST    = RegisterBits< type,  1,  1 >;  /**< TIM8 reset                                */
      using TIM1RST    = RegisterBits< type,  0,  1 >;  /**< TIM1 reset                                */
    };

    /**
     * AHB1 peripheral clock register
     */
    struct AHB1ENR
    : public Register< uint32_t, base_addr + 0x30, Access::rw, 0x00100000 >
    {
      using OTGHSULPIEN  = RegisterBits< type, 30,  1 >;  /**< USB OTG HSULPI clock enable         */
      using OTGHSEN      = RegisterBits< type, 29,  1 >;  /**< USB OTG HS clock enable             */
      using ETHMACPTPEN  = RegisterBits< type, 28,  1 >;  /**< Ethernet PTP clock enable           */
      using ETHMACRXEN   = RegisterBits< type, 27,  1 >;  /**< Ethernet Reception clock enable     */
      using ETHMACTXEN   = RegisterBits< type, 26,  1 >;  /**< Ethernet Transmission clock enable  */
      using ETHMACEN     = RegisterBits< type, 25,  1 >;  /**< Ethernet MAC clock enable           */
      using DMA2EN       = RegisterBits< type, 22,  1 >;  /**< DMA2 clock enable                   */
      using DMA1EN       = RegisterBits< type, 21,  1 >;  /**< DMA1 clock enable                   */
      using BKPSRAMEN    = RegisterBits< type, 18,  1 >;  /**< Backup SRAM interface clock enable  */
      using CRCEN        = RegisterBits< type, 12,  1 >;  /**< CRC clock enable                    */
      using GPIOIEN      = RegisterBits< type,  8,  1 >;  /**< IO port I clock enable              */
      using GPIOHEN      = RegisterBits< type,  7,  1 >;  /**< IO port H clock enable              */
      using GPIOGEN      = RegisterBits< type,  6,  1 >;  /**< IO port G clock enable              */
      using GPIOFEN      = RegisterBits< type,  5,  1 >;  /**< IO port F clock enable              */
      using GPIOEEN      = RegisterBits< type,  4,  1 >;  /**< IO port E clock enable              */
      using GPIODEN      = RegisterBits< type,  3,  1 >;  /**< IO port D clock enable              */
      using GPIOCEN      = RegisterBits< type,  2,  1 >;  /**< IO port C clock enable              */
      using GPIOBEN      = RegisterBits< type,  1,  1 >;  /**< IO port B clock enable              */
      using GPIOAEN      = RegisterBits< type,  0,  1 >;  /**< IO port A clock enable              */
    };

    /**
     * AHB2 peripheral clock enable register
     */
    struct AHB2ENR
    : public Register< uint32_t, base_addr + 0x34, Access::rw, 0x00000000 >
    {
      using OTGFSEN  = RegisterBits< type,  7,  1 >;  /**< USB OTG FS clock enable               */
      using RNGEN    = RegisterBits< type,  6,  1 >;  /**< Random number generator clock enable  */
      using DCMIEN   = RegisterBits< type,  0,  1 >;  /**< Camera interface enable               */
    };

    /**
     * AHB3 peripheral clock enable register
     */
    struct AHB3ENR
    : public Register< uint32_t, base_addr + 0x38, Access::rw, 0x00000000 >
    {
      using FSMCEN  = RegisterBits< type,  0,  1 >;  /**< Flexible static memory controller module clock enable  */
    };

    /**
     * APB1 peripheral clock enable register
     */
    struct APB1ENR
    : public Register< uint32_t, base_addr + 0x40, Access::rw, 0x00000000 >
    {
      using DACEN     = RegisterBits< type, 29,  1 >;  /**< DAC interface clock enable    */
      using PWREN     = RegisterBits< type, 28,  1 >;  /**< Power interface clock enable  */
      using CAN2EN    = RegisterBits< type, 26,  1 >;  /**< CAN 2 clock enable            */
      using CAN1EN    = RegisterBits< type, 25,  1 >;  /**< CAN 1 clock enable            */
      using I2C3EN    = RegisterBits< type, 23,  1 >;  /**< I2C3 clock enable             */
      using I2C2EN    = RegisterBits< type, 22,  1 >;  /**< I2C2 clock enable             */
      using I2C1EN    = RegisterBits< type, 21,  1 >;  /**< I2C1 clock enable             */
      using UART5EN   = RegisterBits< type, 20,  1 >;  /**< UART5 clock enable            */
      using UART4EN   = RegisterBits< type, 19,  1 >;  /**< UART4 clock enable            */
      using USART3EN  = RegisterBits< type, 18,  1 >;  /**< USART3 clock enable           */
      using USART2EN  = RegisterBits< type, 17,  1 >;  /**< USART 2 clock enable          */
      using SPI3EN    = RegisterBits< type, 15,  1 >;  /**< SPI3 clock enable             */
      using SPI2EN    = RegisterBits< type, 14,  1 >;  /**< SPI2 clock enable             */
      using WWDGEN    = RegisterBits< type, 11,  1 >;  /**< Window watchdog clock enable  */
      using TIM14EN   = RegisterBits< type,  8,  1 >;  /**< TIM14 clock enable            */
      using TIM13EN   = RegisterBits< type,  7,  1 >;  /**< TIM13 clock enable            */
      using TIM12EN   = RegisterBits< type,  6,  1 >;  /**< TIM12 clock enable            */
      using TIM7EN    = RegisterBits< type,  5,  1 >;  /**< TIM7 clock enable             */
      using TIM6EN    = RegisterBits< type,  4,  1 >;  /**< TIM6 clock enable             */
      using TIM5EN    = RegisterBits< type,  3,  1 >;  /**< TIM5 clock enable             */
      using TIM4EN    = RegisterBits< type,  2,  1 >;  /**< TIM4 clock enable             */
      using TIM3EN    = RegisterBits< type,  1,  1 >;  /**< TIM3 clock enable             */
      using TIM2EN    = RegisterBits< type,  0,  1 >;  /**< TIM2 clock enable             */
    };

    /**
     * APB2 peripheral clock enable register
     */
    struct APB2ENR
    : public Register< uint32_t, base_addr + 0x44, Access::rw, 0x00000000 >
    {
      using TIM11EN   = RegisterBits< type, 18,  1 >;  /**< TIM11 clock enable                            */
      using TIM10EN   = RegisterBits< type, 17,  1 >;  /**< TIM10 clock enable                            */
      using TIM9EN    = RegisterBits< type, 16,  1 >;  /**< TIM9 clock enable                             */
      using SYSCFGEN  = RegisterBits< type, 14,  1 >;  /**< System configuration controller clock enable  */
      using SPI1EN    = RegisterBits< type, 12,  1 >;  /**< SPI1 clock enable                             */
      using SDIOEN    = RegisterBits< type, 11,  1 >;  /**< SDIO clock enable                             */
      using ADC3EN    = RegisterBits< type, 10,  1 >;  /**< ADC3 clock enable                             */
      using ADC2EN    = RegisterBits< type,  9,  1 >;  /**< ADC2 clock enable                             */
      using ADC1EN    = RegisterBits< type,  8,  1 >;  /**< ADC1 clock enable                             */
      using USART6EN  = RegisterBits< type,  5,  1 >;  /**< USART6 clock enable                           */
      using USART1EN  = RegisterBits< type,  4,  1 >;  /**< USART1 clock enable                           */
      using TIM8EN    = RegisterBits< type,  1,  1 >;  /**< TIM8 clock enable                             */
      using TIM1EN    = RegisterBits< type,  0,  1 >;  /**< TIM1 clock enable                             */
    };

    /**
     * AHB1 peripheral clock enable in low power mode register
     */
    struct AHB1LPENR
    : public Register< uint32_t, base_addr + 0x50, Access::rw, 0x7E6791FF >
    {
      using OTGHSULPILPEN  = RegisterBits< type, 30,  1 >;  /**< USB OTG HS ULPI clock enable during sleep mode        */
      using OTGHSLPEN      = RegisterBits< type, 29,  1 >;  /**< USB OTG HS clock enable during sleep mode             */
      using ETHMACPTPLPEN  = RegisterBits< type, 28,  1 >;  /**< Ethernet PTP clock enable during sleep mode           */
      using ETHMACRXLPEN   = RegisterBits< type, 27,  1 >;  /**< Ethernet reception clock enable during sleep mode     */
      using ETHMACTXLPEN   = RegisterBits< type, 26,  1 >;  /**< Ethernet transmission clock enable during sleep mode  */
      using ETHMACLPEN     = RegisterBits< type, 25,  1 >;  /**< Ethernet MAC clock enable during sleep mode           */
      using DMA2LPEN       = RegisterBits< type, 22,  1 >;  /**< DMA2 clock enable during sleep mode                   */
      using DMA1LPEN       = RegisterBits< type, 21,  1 >;  /**< DMA1 clock enable during sleep mode                   */
      using BKPSRAMLPEN    = RegisterBits< type, 18,  1 >;  /**< Backup SRAM interface clock enable during sleep mode  */
      using SRAM2LPEN      = RegisterBits< type, 17,  1 >;  /**< SRAM 2 interface clock enable during sleep mode       */
      using SRAM1LPEN      = RegisterBits< type, 16,  1 >;  /**< SRAM 1 interface clock enable during sleep mode        */
      using FLITFLPEN      = RegisterBits< type, 15,  1 >;  /**< Flash interface clock enable during sleep mode        */
      using CRCLPEN        = RegisterBits< type, 12,  1 >;  /**< CRC clock enable during sleep mode                    */
      using GPIOILPEN      = RegisterBits< type,  8,  1 >;  /**< IO port I clock enable during sleep mode              */
      using GPIOHLPEN      = RegisterBits< type,  7,  1 >;  /**< IO port H clock enable during sleep mode              */
      using GPIOGLPEN      = RegisterBits< type,  6,  1 >;  /**< IO port G clock enable during sleep mode              */
      using GPIOFLPEN      = RegisterBits< type,  5,  1 >;  /**< IO port F clock enable during sleep mode              */
      using GPIOELPEN      = RegisterBits< type,  4,  1 >;  /**< IO port E clock enable during sleep mode              */
      using GPIODLPEN      = RegisterBits< type,  3,  1 >;  /**< IO port D clock enable during sleep mode              */
      using GPIOCLPEN      = RegisterBits< type,  2,  1 >;  /**< IO port C clock enable during sleep mode              */
      using GPIOBLPEN      = RegisterBits< type,  1,  1 >;  /**< IO port B clock enable during sleep mode              */
      using GPIOALPEN      = RegisterBits< type,  0,  1 >;  /**< IO port A clock enable during sleep mode              */
    };

    /**
     * AHB2 peripheral clock enable in low power mode register
     */
    struct AHB2LPENR
    : public Register< uint32_t, base_addr + 0x54, Access::rw, 0x000000F1 >
    {
      using OTGFSLPEN  = RegisterBits< type,  7,  1 >;  /**< USB OTG FS clock enable during sleep mode               */
      using RNGLPEN    = RegisterBits< type,  6,  1 >;  /**< Random number generator clock enable during sleep mode  */
      using DCMILPEN   = RegisterBits< type,  0,  1 >;  /**< Camera interface enable during sleep mode               */
    };

    /**
     * AHB3 peripheral clock enable in low power mode register
     */
    struct AHB3LPENR
    : public Register< uint32_t, base_addr + 0x58, Access::rw, 0x00000001 >
    {
      using FSMCLPEN  = RegisterBits< type,  0,  1 >;  /**< Flexible static memory controller module clock enable during sleep mode  */
    };

    /**
     * APB1 peripheral clock enable in low power mode register
     */
    struct APB1LPENR
    : public Register< uint32_t, base_addr + 0x60, Access::rw, 0x36FEC9FF >
    {
      using DACLPEN     = RegisterBits< type, 29,  1 >;  /**< DAC interface clock enable during sleep mode    */
      using PWRLPEN     = RegisterBits< type, 28,  1 >;  /**< Power interface clock enable during sleep mode  */
      using CAN2LPEN    = RegisterBits< type, 26,  1 >;  /**< CAN 2 clock enable during sleep mode            */
      using CAN1LPEN    = RegisterBits< type, 25,  1 >;  /**< CAN 1 clock enable during sleep mode            */
      using I2C3LPEN    = RegisterBits< type, 23,  1 >;  /**< I2C3 clock enable during sleep mode             */
      using I2C2LPEN    = RegisterBits< type, 22,  1 >;  /**< I2C2 clock enable during sleep mode             */
      using I2C1LPEN    = RegisterBits< type, 21,  1 >;  /**< I2C1 clock enable during sleep mode             */
      using UART5LPEN   = RegisterBits< type, 20,  1 >;  /**< UART5 clock enable during sleep mode            */
      using UART4LPEN   = RegisterBits< type, 19,  1 >;  /**< UART4 clock enable during sleep mode            */
      using USART3LPEN  = RegisterBits< type, 18,  1 >;  /**< USART3 clock enable during sleep mode           */
      using USART2LPEN  = RegisterBits< type, 17,  1 >;  /**< USART2 clock enable during sleep mode           */
      using SPI3LPEN    = RegisterBits< type, 15,  1 >;  /**< SPI3 clock enable during sleep mode             */
      using SPI2LPEN    = RegisterBits< type, 14,  1 >;  /**< SPI2 clock enable during sleep mode             */
      using WWDGLPEN    = RegisterBits< type, 11,  1 >;  /**< Window watchdog clock enable during sleep mode  */
      using TIM14LPEN   = RegisterBits< type,  8,  1 >;  /**< TIM14 clock enable during sleep mode            */
      using TIM13LPEN   = RegisterBits< type,  7,  1 >;  /**< TIM13 clock enable during sleep mode            */
      using TIM12LPEN   = RegisterBits< type,  6,  1 >;  /**< TIM12 clock enable during sleep mode            */
      using TIM7LPEN    = RegisterBits< type,  5,  1 >;  /**< TIM7 clock enable during sleep mode             */
      using TIM6LPEN    = RegisterBits< type,  4,  1 >;  /**< TIM6 clock enable during sleep mode             */
      using TIM5LPEN    = RegisterBits< type,  3,  1 >;  /**< TIM5 clock enable during sleep mode             */
      using TIM4LPEN    = RegisterBits< type,  2,  1 >;  /**< TIM4 clock enable during sleep mode             */
      using TIM3LPEN    = RegisterBits< type,  1,  1 >;  /**< TIM3 clock enable during sleep mode             */
      using TIM2LPEN    = RegisterBits< type,  0,  1 >;  /**< TIM2 clock enable during sleep mode             */
    };

    /**
     * APB2 peripheral clock enabled in low power mode register
     */
    struct APB2LPENR
    : public Register< uint32_t, base_addr + 0x64, Access::rw, 0x00075F33 >
    {
      using TIM11LPEN   = RegisterBits< type, 18,  1 >;  /**< TIM11 clock enable during sleep mode                            */
      using TIM10LPEN   = RegisterBits< type, 17,  1 >;  /**< TIM10 clock enable during sleep mode                            */
      using TIM9LPEN    = RegisterBits< type, 16,  1 >;  /**< TIM9 clock enable during sleep mode                             */
      using SYSCFGLPEN  = RegisterBits< type, 14,  1 >;  /**< System configuration controller clock enable during sleep mode  */
      using SPI1LPEN    = RegisterBits< type, 12,  1 >;  /**< SPI 1 clock enable during sleep mode                            */
      using SDIOLPEN    = RegisterBits< type, 11,  1 >;  /**< SDIO clock enable during sleep mode                             */
      using ADC3LPEN    = RegisterBits< type, 10,  1 >;  /**< ADC 3 clock enable during sleep mode                            */
      using ADC2LPEN    = RegisterBits< type,  9,  1 >;  /**< ADC2 clock enable during sleep mode                             */
      using ADC1LPEN    = RegisterBits< type,  8,  1 >;  /**< ADC1 clock enable during sleep mode                             */
      using USART6LPEN  = RegisterBits< type,  5,  1 >;  /**< USART6 clock enable during sleep mode                           */
      using USART1LPEN  = RegisterBits< type,  4,  1 >;  /**< USART1 clock enable during sleep mode                           */
      using TIM8LPEN    = RegisterBits< type,  1,  1 >;  /**< TIM8 clock enable during sleep mode                             */
      using TIM1LPEN    = RegisterBits< type,  0,  1 >;  /**< TIM1 clock enable during sleep mode                             */
    };

    /**
     * Backup domain control register
     */
    struct BDCR
    : public Register< uint32_t, base_addr + 0x70, Access::rw, 0x00000000 >
    {
      using BDRST    = RegisterBits< type, 16,  1 >;  /**< Backup domain software reset          */
      using RTCEN    = RegisterBits< type, 15,  1 >;  /**< RTC clock enable                      */
      using RTCSEL1  = RegisterBits< type,  9,  1 >;  /**< RTC clock source selection            */
      using RTCSEL0  = RegisterBits< type,  8,  1 >;  /**< RTC clock source selection            */
      using LSEBYP   = RegisterBits< type,  2,  1 >;  /**< External low-speed oscillator bypass  */
      using LSERDY   = RegisterBits< type,  1,  1 >;  /**< External low-speed oscillator ready   */
      using LSEON    = RegisterBits< type,  0,  1 >;  /**< External low-speed oscillator enable  */
    };

    /**
     * Clock control and status register
     */
    struct CSR
    : public Register< uint32_t, base_addr + 0x74, Access::rw, 0x0E000000 >
    {
      using LPWRRSTF  = RegisterBits< type, 31,  1 >;  /**< Low-power reset flag                  */
      using WWDGRSTF  = RegisterBits< type, 30,  1 >;  /**< Window watchdog reset flag            */
      using WDGRSTF   = RegisterBits< type, 29,  1 >;  /**< Independent watchdog reset flag       */
      using SFTRSTF   = RegisterBits< type, 28,  1 >;  /**< Software reset flag                   */
      using PORRSTF   = RegisterBits< type, 27,  1 >;  /**< POR/PDR reset flag                    */
      using PADRSTF   = RegisterBits< type, 26,  1 >;  /**< PIN reset flag                        */
      using BORRSTF   = RegisterBits< type, 25,  1 >;  /**< BOR reset flag                        */
      using RMVF      = RegisterBits< type, 24,  1 >;  /**< Remove reset flag                     */
      using LSIRDY    = RegisterBits< type,  1,  1 >;  /**< Internal low-speed oscillator ready   */
      using LSION     = RegisterBits< type,  0,  1 >;  /**< Internal low-speed oscillator enable  */
    };

    /**
     * Spread spectrum clock generation register
     */
    struct SSCGR
    : public Register< uint32_t, base_addr + 0x80, Access::rw, 0x00000000 >
    {
      using SSCGEN     = RegisterBits< type, 31,  1 >;  /**< Spread spectrum modulation enable  */
      using SPREADSEL  = RegisterBits< type, 30,  1 >;  /**< Spread Select                      */
      using INCSTEP    = RegisterBits< type, 13, 15 >;  /**< Incrementation step                */
      using MODPER     = RegisterBits< type,  0, 13 >;  /**< Modulation period                  */
    };

    /**
     * PLLI2S configuration register
     */
    struct PLLI2SCFGR
    : public Register< uint32_t, base_addr + 0x84, Access::rw, 0x20003000 >
    {
      using PLLI2SR  = RegisterBits< type, 28,  3 >;  /**< PLLI2S division factor for I2S clocks  */
      using PLLI2SN  = RegisterBits< type,  6,  9 >;  /**< PLLI2S multiplication factor for VCO   */
    };
  };
}

#endif // ARCH_REG_RCC_HPP_INCLUDED
