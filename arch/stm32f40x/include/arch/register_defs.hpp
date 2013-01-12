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

#ifndef CORE_REG_HPP_INCLUDED
#define CORE_REG_HPP_INCLUDED

#include <register.hpp>
#include "../../../common/arm-cortex/core_register.hpp"

using access = RegisterAccess;

struct PeripheralRegister {

  /**
   * Reset and clock control
   */
  class RCC
  {
  public:
    static constexpr reg_addr_t reg_base = 0x40023800;
    static constexpr const char * name_str = "RCC";

    /**
     * clock control register
     */
    struct CR
    : public Register< uint32_t, reg_base + 0x0, access::rw, 0x00000083 >
    {
      typedef Register< uint32_t, reg_base + 0x0, access::rw, 0x00000083 > reg_type;
      static constexpr const char * name_str = "CR";

      typedef RegisterBits< reg_type, 27,  1 > PLLI2SRDY;   /**< PLLI2S clock ready flag                */
      typedef RegisterBits< reg_type, 26,  1 > PLLI2SON;    /**< PLLI2S enable                          */
      typedef RegisterBits< reg_type, 25,  1 > PLLRDY;      /**< Main PLL (PLL) clock ready flag        */
      typedef RegisterBits< reg_type, 24,  1 > PLLON;       /**< Main PLL (PLL) enable                  */
      typedef RegisterBits< reg_type, 19,  1 > CSSON;       /**< Clock security system enable           */
      typedef RegisterBits< reg_type, 18,  1 > HSEBYP;      /**< HSE clock bypass                       */
      typedef RegisterBits< reg_type, 17,  1 > HSERDY;      /**< HSE clock ready flag                   */
      typedef RegisterBits< reg_type, 16,  1 > HSEON;       /**< HSE clock enable                       */
      typedef RegisterBits< reg_type,  8,  8 > HSICAL;      /**< Internal high-speed clock calibration  */
      typedef RegisterBits< reg_type,  3,  5 > HSITRIM;     /**< Internal high-speed clock trimming     */
      typedef RegisterBits< reg_type,  1,  1 > HSIRDY;      /**< Internal high-speed clock ready flag   */
      typedef RegisterBits< reg_type,  0,  1 > HSION;       /**< Internal high-speed clock enable       */
    };

    /**
     * PLL configuration register
     */
    struct PLLCFGR
    : public Register< uint32_t, reg_base + 0x4, access::rw, 0x24003010 >
    {
      typedef Register< uint32_t, reg_base + 0x4, access::rw, 0x24003010 > reg_type;
      static constexpr const char * name_str = "PLLCFGR";

      typedef RegisterBits< reg_type, 24,  4 > PLLQ;     /**< Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks  */
      typedef RegisterBits< reg_type, 22,  1 > PLLSRC;   /**< Main PLL (PLL) and audio PLL (PLLI2S) entry clock source                                */
      typedef RegisterBits< reg_type, 16,  2 > PLLP;     /**< Main PLL (PLL) division factor for main system clock                                    */
      typedef RegisterBits< reg_type,  6,  9 > PLLN;     /**< Main PLL (PLL) multiplication factor for VCO                                            */
      typedef RegisterBits< reg_type,  0,  6 > PLLM;     /**< Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock               */
    };

    /**
     * clock configuration register
     */
    struct CFGR
    : public Register< uint32_t, reg_base + 0x8, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x8, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "CFGR";

      /** APB1/2 prescaler  */
      template<typename Rb>
      struct __PPREx
      : public Rb
      {
        typedef RegisterConst< Rb, 0x0 > DIV1;         /**< 0xx: AHB clock not divided     */
        typedef RegisterConst< Rb, 0x4 > DIV2;         /**< 100: AHB clock divided by 2    */
        typedef RegisterConst< Rb, 0x5 > DIV4;         /**< 101: AHB clock divided by 4    */
        typedef RegisterConst< Rb, 0x6 > DIV8;         /**< 110: AHB clock divided by 8    */
        typedef RegisterConst< Rb, 0x7 > DIV16;        /**< 111: AHB clock divided by 16   */
      };

      /** AHB prescaler  */
      template<typename Rb>
      struct __HPRE
      : public Rb
      {
        typedef RegisterConst< Rb, 0x0 > DIV1;         /**< 0xxx: system clock not divided      */
        typedef RegisterConst< Rb, 0x8 > DIV2;         /**< 1000: system clock divided by 2     */
        typedef RegisterConst< Rb, 0x9 > DIV4;         /**< 1001: system clock divided by 4     */
        typedef RegisterConst< Rb, 0xA > DIV8;         /**< 1010: system clock divided by 8     */
        typedef RegisterConst< Rb, 0xB > DIV16;        /**< 1011: system clock divided by 16    */
        typedef RegisterConst< Rb, 0xC > DIV64;        /**< 1100: system clock divided by 64    */
        typedef RegisterConst< Rb, 0xD > DIV128;       /**< 1101: system clock divided by 128   */
        typedef RegisterConst< Rb, 0xE > DIV256;       /**< 1110: system clock divided by 256   */
        typedef RegisterConst< Rb, 0xF > DIV512;       /**< 1111: system clock divided by 512   */
      };

      /** System clock Switch  */
      template<typename Rb>
      struct __SWx
      : public Rb
      {
        typedef RegisterConst< Rb, 0x0 > HSI;          /**< 00: HSI selected as system clock   */
        typedef RegisterConst< Rb, 0x1 > HSE;          /**< 01: HSE selected as system clock   */
        typedef RegisterConst< Rb, 0x2 > PLL;          /**< 10: PLL selected as system clock   */
      };

      typedef           RegisterBits< reg_type, 30,  2 >   MCO2;      /**< Microcontroller clock output 2     */
      typedef           RegisterBits< reg_type, 27,  3 >   MCO2PRE;   /**< MCO2 prescaler                     */
      typedef           RegisterBits< reg_type, 24,  3 >   MCO1PRE;   /**< MCO1 prescaler                     */
      typedef           RegisterBits< reg_type, 23,  1 >   I2SSRC;    /**< I2S clock selection                */
      typedef           RegisterBits< reg_type, 21,  2 >   MCO1;      /**< Microcontroller clock output 1     */
      typedef           RegisterBits< reg_type, 16,  5 >   RTCPRE;    /**< HSE division factor for RTC clock  */
      typedef __PPREx < RegisterBits< reg_type, 13,  3 > > PPRE2;     /**< APB high-speed prescaler (APB2)    */
      typedef __PPREx < RegisterBits< reg_type, 10,  3 > > PPRE1;     /**< APB Low speed prescaler (APB1)     */
      typedef __HPRE  < RegisterBits< reg_type,  4,  4 > > HPRE;      /**< AHB prescaler                      */
      typedef __SWx   < RegisterBits< reg_type,  2,  2 > > SWS;       /**< System clock switch status         */
      typedef __SWx   < RegisterBits< reg_type,  0,  2 > > SW;        /**< System clock switch                */
    };

    /**
     * clock interrupt register
     */
    struct CIR
    : public Register< uint32_t, reg_base + 0xC, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0xC, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "CIR";

      typedef RegisterBits< reg_type, 23,  1 > CSSC;          /**< Clock security system interrupt clear  */
      typedef RegisterBits< reg_type, 21,  1 > PLLI2SRDYC;    /**< PLLI2S ready interrupt clear           */
      typedef RegisterBits< reg_type, 20,  1 > PLLRDYC;       /**< Main PLL(PLL) ready interrupt clear    */
      typedef RegisterBits< reg_type, 19,  1 > HSERDYC;       /**< HSE ready interrupt clear              */
      typedef RegisterBits< reg_type, 18,  1 > HSIRDYC;       /**< HSI ready interrupt clear              */
      typedef RegisterBits< reg_type, 17,  1 > LSERDYC;       /**< LSE ready interrupt clear              */
      typedef RegisterBits< reg_type, 16,  1 > LSIRDYC;       /**< LSI ready interrupt clear              */
      typedef RegisterBits< reg_type, 13,  1 > PLLI2SRDYIE;   /**< PLLI2S ready interrupt enable          */
      typedef RegisterBits< reg_type, 12,  1 > PLLRDYIE;      /**< Main PLL (PLL) ready interrupt enable  */
      typedef RegisterBits< reg_type, 11,  1 > HSERDYIE;      /**< HSE ready interrupt enable             */
      typedef RegisterBits< reg_type, 10,  1 > HSIRDYIE;      /**< HSI ready interrupt enable             */
      typedef RegisterBits< reg_type,  9,  1 > LSERDYIE;      /**< LSE ready interrupt enable             */
      typedef RegisterBits< reg_type,  8,  1 > LSIRDYIE;      /**< LSI ready interrupt enable             */
      typedef RegisterBits< reg_type,  7,  1 > CSSF;          /**< Clock security system interrupt flag   */
      typedef RegisterBits< reg_type,  5,  1 > PLLI2SRDYF;    /**< PLLI2S ready interrupt flag            */
      typedef RegisterBits< reg_type,  4,  1 > PLLRDYF;       /**< Main PLL (PLL) ready interrupt flag    */
      typedef RegisterBits< reg_type,  3,  1 > HSERDYF;       /**< HSE ready interrupt flag               */
      typedef RegisterBits< reg_type,  2,  1 > HSIRDYF;       /**< HSI ready interrupt flag               */
      typedef RegisterBits< reg_type,  1,  1 > LSERDYF;       /**< LSE ready interrupt flag               */
      typedef RegisterBits< reg_type,  0,  1 > LSIRDYF;       /**< LSI ready interrupt flag               */
    };

    /**
     * AHB1 peripheral reset register
     */
    struct AHB1RSTR
    : public Register< uint32_t, reg_base + 0x10, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x10, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "AHB1RSTR";

      typedef RegisterBits< reg_type, 29,  1 > OTGHSRST;    /**< USB OTG HS module reset  */
      typedef RegisterBits< reg_type, 25,  1 > ETHMACRST;   /**< Ethernet MAC reset       */
      typedef RegisterBits< reg_type, 22,  1 > DMA2RST;     /**< DMA2 reset               */
      typedef RegisterBits< reg_type, 21,  1 > DMA1RST;     /**< DMA2 reset               */
      typedef RegisterBits< reg_type, 12,  1 > CRCRST;      /**< CRC reset                */
      typedef RegisterBits< reg_type,  8,  1 > GPIOIRST;    /**< IO port I reset          */
      typedef RegisterBits< reg_type,  7,  1 > GPIOHRST;    /**< IO port H reset          */
      typedef RegisterBits< reg_type,  6,  1 > GPIOGRST;    /**< IO port G reset          */
      typedef RegisterBits< reg_type,  5,  1 > GPIOFRST;    /**< IO port F reset          */
      typedef RegisterBits< reg_type,  4,  1 > GPIOERST;    /**< IO port E reset          */
      typedef RegisterBits< reg_type,  3,  1 > GPIODRST;    /**< IO port D reset          */
      typedef RegisterBits< reg_type,  2,  1 > GPIOCRST;    /**< IO port C reset          */
      typedef RegisterBits< reg_type,  1,  1 > GPIOBRST;    /**< IO port B reset          */
      typedef RegisterBits< reg_type,  0,  1 > GPIOARST;    /**< IO port A reset          */
    };

    /**
     * AHB2 peripheral reset register
     */
    struct AHB2RSTR
    : public Register< uint32_t, reg_base + 0x14, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x14, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "AHB2RSTR";

      typedef RegisterBits< reg_type,  7,  1 > OTGFSRST;   /**< USB OTG FS module reset               */
      typedef RegisterBits< reg_type,  6,  1 > RNGRST;     /**< Random number generator module reset  */
      typedef RegisterBits< reg_type,  0,  1 > DCMIRST;    /**< Camera interface reset                */
    };

    /**
     * AHB3 peripheral reset register
     */
    struct AHB3RSTR
    : public Register< uint32_t, reg_base + 0x18, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x18, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "AHB3RSTR";

      typedef RegisterBits< reg_type,  0,  1 > FSMCRST;   /**< Flexible static memory controller module reset  */
    };

    /**
     * APB1 peripheral reset register
     */
    struct APB1RSTR
    : public Register< uint32_t, reg_base + 0x20, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x20, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "APB1RSTR";

      typedef RegisterBits< reg_type, 29,  1 > DACRST;     /**< DAC reset              */
      typedef RegisterBits< reg_type, 28,  1 > PWRRST;     /**< Power interface reset  */
      typedef RegisterBits< reg_type, 26,  1 > CAN2RST;    /**< CAN2 reset             */
      typedef RegisterBits< reg_type, 25,  1 > CAN1RST;    /**< CAN1 reset             */
      typedef RegisterBits< reg_type, 23,  1 > I2C3RST;    /**< I2C3 reset             */
      typedef RegisterBits< reg_type, 22,  1 > I2C2RST;    /**< I2C 2 reset            */
      typedef RegisterBits< reg_type, 21,  1 > I2C1RST;    /**< I2C 1 reset            */
      typedef RegisterBits< reg_type, 20,  1 > UART5RST;   /**< USART 5 reset          */
      typedef RegisterBits< reg_type, 19,  1 > UART4RST;   /**< USART 4 reset          */
      typedef RegisterBits< reg_type, 18,  1 > UART3RST;   /**< USART 3 reset          */
      typedef RegisterBits< reg_type, 17,  1 > UART2RST;   /**< USART 2 reset          */
      typedef RegisterBits< reg_type, 15,  1 > SPI3RST;    /**< SPI 3 reset            */
      typedef RegisterBits< reg_type, 14,  1 > SPI2RST;    /**< SPI 2 reset            */
      typedef RegisterBits< reg_type, 11,  1 > WWDGRST;    /**< Window watchdog reset  */
      typedef RegisterBits< reg_type,  8,  1 > TIM14RST;   /**< TIM14 reset            */
      typedef RegisterBits< reg_type,  7,  1 > TIM13RST;   /**< TIM13 reset            */
      typedef RegisterBits< reg_type,  6,  1 > TIM12RST;   /**< TIM12 reset            */
      typedef RegisterBits< reg_type,  5,  1 > TIM7RST;    /**< TIM7 reset             */
      typedef RegisterBits< reg_type,  4,  1 > TIM6RST;    /**< TIM6 reset             */
      typedef RegisterBits< reg_type,  3,  1 > TIM5RST;    /**< TIM5 reset             */
      typedef RegisterBits< reg_type,  2,  1 > TIM4RST;    /**< TIM4 reset             */
      typedef RegisterBits< reg_type,  1,  1 > TIM3RST;    /**< TIM3 reset             */
      typedef RegisterBits< reg_type,  0,  1 > TIM2RST;    /**< TIM2 reset             */
    };

    /**
     * APB2 peripheral reset register
     */
    struct APB2RSTR
    : public Register< uint32_t, reg_base + 0x24, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x24, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "APB2RSTR";

      typedef RegisterBits< reg_type, 18,  1 > TIM11RST;    /**< TIM11 reset                               */
      typedef RegisterBits< reg_type, 17,  1 > TIM10RST;    /**< TIM10 reset                               */
      typedef RegisterBits< reg_type, 16,  1 > TIM9RST;     /**< TIM9 reset                                */
      typedef RegisterBits< reg_type, 14,  1 > SYSCFGRST;   /**< System configuration controller reset     */
      typedef RegisterBits< reg_type, 12,  1 > SPI1RST;     /**< SPI 1 reset                               */
      typedef RegisterBits< reg_type, 11,  1 > SDIORST;     /**< SDIO reset                                */
      typedef RegisterBits< reg_type,  8,  1 > ADCRST;      /**< ADC interface reset (common to all ADCs)  */
      typedef RegisterBits< reg_type,  5,  1 > USART6RST;   /**< USART6 reset                              */
      typedef RegisterBits< reg_type,  4,  1 > USART1RST;   /**< USART1 reset                              */
      typedef RegisterBits< reg_type,  1,  1 > TIM8RST;     /**< TIM8 reset                                */
      typedef RegisterBits< reg_type,  0,  1 > TIM1RST;     /**< TIM1 reset                                */
    };

    /**
     * AHB1 peripheral clock register
     */
    struct AHB1ENR
    : public Register< uint32_t, reg_base + 0x30, access::rw, 0x00100000 >
    {
      typedef Register< uint32_t, reg_base + 0x30, access::rw, 0x00100000 > reg_type;
      static constexpr const char * name_str = "AHB1ENR";

      typedef RegisterBits< reg_type, 30,  1 > OTGHSULPIEN;   /**< USB OTG HSULPI clock enable         */
      typedef RegisterBits< reg_type, 29,  1 > OTGHSEN;       /**< USB OTG HS clock enable             */
      typedef RegisterBits< reg_type, 28,  1 > ETHMACPTPEN;   /**< Ethernet PTP clock enable           */
      typedef RegisterBits< reg_type, 27,  1 > ETHMACRXEN;    /**< Ethernet Reception clock enable     */
      typedef RegisterBits< reg_type, 26,  1 > ETHMACTXEN;    /**< Ethernet Transmission clock enable  */
      typedef RegisterBits< reg_type, 25,  1 > ETHMACEN;      /**< Ethernet MAC clock enable           */
      typedef RegisterBits< reg_type, 22,  1 > DMA2EN;        /**< DMA2 clock enable                   */
      typedef RegisterBits< reg_type, 21,  1 > DMA1EN;        /**< DMA1 clock enable                   */
      typedef RegisterBits< reg_type, 18,  1 > BKPSRAMEN;     /**< Backup SRAM interface clock enable  */
      typedef RegisterBits< reg_type, 12,  1 > CRCEN;         /**< CRC clock enable                    */
      typedef RegisterBits< reg_type,  8,  1 > GPIOIEN;       /**< IO port I clock enable              */
      typedef RegisterBits< reg_type,  7,  1 > GPIOHEN;       /**< IO port H clock enable              */
      typedef RegisterBits< reg_type,  6,  1 > GPIOGEN;       /**< IO port G clock enable              */
      typedef RegisterBits< reg_type,  5,  1 > GPIOFEN;       /**< IO port F clock enable              */
      typedef RegisterBits< reg_type,  4,  1 > GPIOEEN;       /**< IO port E clock enable              */
      typedef RegisterBits< reg_type,  3,  1 > GPIODEN;       /**< IO port D clock enable              */
      typedef RegisterBits< reg_type,  2,  1 > GPIOCEN;       /**< IO port C clock enable              */
      typedef RegisterBits< reg_type,  1,  1 > GPIOBEN;       /**< IO port B clock enable              */
      typedef RegisterBits< reg_type,  0,  1 > GPIOAEN;       /**< IO port A clock enable              */
    };

    /**
     * AHB2 peripheral clock enable register
     */
    struct AHB2ENR
    : public Register< uint32_t, reg_base + 0x34, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x34, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "AHB2ENR";

      typedef RegisterBits< reg_type,  7,  1 > OTGFSEN;   /**< USB OTG FS clock enable               */
      typedef RegisterBits< reg_type,  6,  1 > RNGEN;     /**< Random number generator clock enable  */
      typedef RegisterBits< reg_type,  0,  1 > DCMIEN;    /**< Camera interface enable               */
    };

    /**
     * AHB3 peripheral clock enable register
     */
    struct AHB3ENR
    : public Register< uint32_t, reg_base + 0x38, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x38, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "AHB3ENR";

      typedef RegisterBits< reg_type,  0,  1 > FSMCEN;   /**< Flexible static memory controller module clock enable  */
    };

    /**
     * APB1 peripheral clock enable register
     */
    struct APB1ENR
    : public Register< uint32_t, reg_base + 0x40, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x40, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "APB1ENR";

      typedef RegisterBits< reg_type, 29,  1 > DACEN;      /**< DAC interface clock enable    */
      typedef RegisterBits< reg_type, 28,  1 > PWREN;      /**< Power interface clock enable  */
      typedef RegisterBits< reg_type, 26,  1 > CAN2EN;     /**< CAN 2 clock enable            */
      typedef RegisterBits< reg_type, 25,  1 > CAN1EN;     /**< CAN 1 clock enable            */
      typedef RegisterBits< reg_type, 23,  1 > I2C3EN;     /**< I2C3 clock enable             */
      typedef RegisterBits< reg_type, 22,  1 > I2C2EN;     /**< I2C2 clock enable             */
      typedef RegisterBits< reg_type, 21,  1 > I2C1EN;     /**< I2C1 clock enable             */
      typedef RegisterBits< reg_type, 20,  1 > UART5EN;    /**< UART5 clock enable            */
      typedef RegisterBits< reg_type, 19,  1 > UART4EN;    /**< UART4 clock enable            */
      typedef RegisterBits< reg_type, 18,  1 > USART3EN;   /**< USART3 clock enable           */
      typedef RegisterBits< reg_type, 17,  1 > USART2EN;   /**< USART 2 clock enable          */
      typedef RegisterBits< reg_type, 15,  1 > SPI3EN;     /**< SPI3 clock enable             */
      typedef RegisterBits< reg_type, 14,  1 > SPI2EN;     /**< SPI2 clock enable             */
      typedef RegisterBits< reg_type, 11,  1 > WWDGEN;     /**< Window watchdog clock enable  */
      typedef RegisterBits< reg_type,  8,  1 > TIM14EN;    /**< TIM14 clock enable            */
      typedef RegisterBits< reg_type,  7,  1 > TIM13EN;    /**< TIM13 clock enable            */
      typedef RegisterBits< reg_type,  6,  1 > TIM12EN;    /**< TIM12 clock enable            */
      typedef RegisterBits< reg_type,  5,  1 > TIM7EN;     /**< TIM7 clock enable             */
      typedef RegisterBits< reg_type,  4,  1 > TIM6EN;     /**< TIM6 clock enable             */
      typedef RegisterBits< reg_type,  3,  1 > TIM5EN;     /**< TIM5 clock enable             */
      typedef RegisterBits< reg_type,  2,  1 > TIM4EN;     /**< TIM4 clock enable             */
      typedef RegisterBits< reg_type,  1,  1 > TIM3EN;     /**< TIM3 clock enable             */
      typedef RegisterBits< reg_type,  0,  1 > TIM2EN;     /**< TIM2 clock enable             */
    };

    /**
     * APB2 peripheral clock enable register
     */
    struct APB2ENR
    : public Register< uint32_t, reg_base + 0x44, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x44, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "APB2ENR";

      typedef RegisterBits< reg_type, 18,  1 > TIM11EN;    /**< TIM11 clock enable                            */
      typedef RegisterBits< reg_type, 17,  1 > TIM10EN;    /**< TIM10 clock enable                            */
      typedef RegisterBits< reg_type, 16,  1 > TIM9EN;     /**< TIM9 clock enable                             */
      typedef RegisterBits< reg_type, 14,  1 > SYSCFGEN;   /**< System configuration controller clock enable  */
      typedef RegisterBits< reg_type, 12,  1 > SPI1EN;     /**< SPI1 clock enable                             */
      typedef RegisterBits< reg_type, 11,  1 > SDIOEN;     /**< SDIO clock enable                             */
      typedef RegisterBits< reg_type, 10,  1 > ADC3EN;     /**< ADC3 clock enable                             */
      typedef RegisterBits< reg_type,  9,  1 > ADC2EN;     /**< ADC2 clock enable                             */
      typedef RegisterBits< reg_type,  8,  1 > ADC1EN;     /**< ADC1 clock enable                             */
      typedef RegisterBits< reg_type,  5,  1 > USART6EN;   /**< USART6 clock enable                           */
      typedef RegisterBits< reg_type,  4,  1 > USART1EN;   /**< USART1 clock enable                           */
      typedef RegisterBits< reg_type,  1,  1 > TIM8EN;     /**< TIM8 clock enable                             */
      typedef RegisterBits< reg_type,  0,  1 > TIM1EN;     /**< TIM1 clock enable                             */
    };

    /**
     * AHB1 peripheral clock enable in low power mode register
     */
    struct AHB1LPENR
    : public Register< uint32_t, reg_base + 0x50, access::rw, 0x7E6791FF >
    {
      typedef Register< uint32_t, reg_base + 0x50, access::rw, 0x7E6791FF > reg_type;
      static constexpr const char * name_str = "AHB1LPENR";

      typedef RegisterBits< reg_type, 30,  1 > OTGHSULPILPEN;   /**< USB OTG HS ULPI clock enable during Sleep mode        */
      typedef RegisterBits< reg_type, 29,  1 > OTGHSLPEN;       /**< USB OTG HS clock enable during Sleep mode             */
      typedef RegisterBits< reg_type, 28,  1 > ETHMACPTPLPEN;   /**< Ethernet PTP clock enable during Sleep mode           */
      typedef RegisterBits< reg_type, 27,  1 > ETHMACRXLPEN;    /**< Ethernet reception clock enable during Sleep mode     */
      typedef RegisterBits< reg_type, 26,  1 > ETHMACTXLPEN;    /**< Ethernet transmission clock enable during Sleep mode  */
      typedef RegisterBits< reg_type, 25,  1 > ETHMACLPEN;      /**< Ethernet MAC clock enable during Sleep mode           */
      typedef RegisterBits< reg_type, 22,  1 > DMA2LPEN;        /**< DMA2 clock enable during Sleep mode                   */
      typedef RegisterBits< reg_type, 21,  1 > DMA1LPEN;        /**< DMA1 clock enable during Sleep mode                   */
      typedef RegisterBits< reg_type, 18,  1 > BKPSRAMLPEN;     /**< Backup SRAM interface clock enable during Sleep mode  */
      typedef RegisterBits< reg_type, 17,  1 > SRAM2LPEN;       /**< SRAM 2 interface clock enable during Sleep mode       */
      typedef RegisterBits< reg_type, 16,  1 > SRAM1LPEN;       /**< SRAM 1interface clock enable during Sleep mode        */
      typedef RegisterBits< reg_type, 15,  1 > FLITFLPEN;       /**< Flash interface clock enable during Sleep mode        */
      typedef RegisterBits< reg_type, 12,  1 > CRCLPEN;         /**< CRC clock enable during Sleep mode                    */
      typedef RegisterBits< reg_type,  8,  1 > GPIOILPEN;       /**< IO port I clock enable during Sleep mode              */
      typedef RegisterBits< reg_type,  7,  1 > GPIOHLPEN;       /**< IO port H clock enable during Sleep mode              */
      typedef RegisterBits< reg_type,  6,  1 > GPIOGLPEN;       /**< IO port G clock enable during Sleep mode              */
      typedef RegisterBits< reg_type,  5,  1 > GPIOFLPEN;       /**< IO port F clock enable during Sleep mode              */
      typedef RegisterBits< reg_type,  4,  1 > GPIOELPEN;       /**< IO port E clock enable during Sleep mode              */
      typedef RegisterBits< reg_type,  3,  1 > GPIODLPEN;       /**< IO port D clock enable during Sleep mode              */
      typedef RegisterBits< reg_type,  2,  1 > GPIOCLPEN;       /**< IO port C clock enable during Sleep mode              */
      typedef RegisterBits< reg_type,  1,  1 > GPIOBLPEN;       /**< IO port B clock enable during Sleep mode              */
      typedef RegisterBits< reg_type,  0,  1 > GPIOALPEN;       /**< IO port A clock enable during sleep mode              */
    };

    /**
     * AHB2 peripheral clock enable in low power mode register
     */
    struct AHB2LPENR
    : public Register< uint32_t, reg_base + 0x54, access::rw, 0x000000F1 >
    {
      typedef Register< uint32_t, reg_base + 0x54, access::rw, 0x000000F1 > reg_type;
      static constexpr const char * name_str = "AHB2LPENR";

      typedef RegisterBits< reg_type,  7,  1 > OTGFSLPEN;   /**< USB OTG FS clock enable during Sleep mode               */
      typedef RegisterBits< reg_type,  6,  1 > RNGLPEN;     /**< Random number generator clock enable during Sleep mode  */
      typedef RegisterBits< reg_type,  0,  1 > DCMILPEN;    /**< Camera interface enable during Sleep mode               */
    };

    /**
     * AHB3 peripheral clock enable in low power mode register
     */
    struct AHB3LPENR
    : public Register< uint32_t, reg_base + 0x58, access::rw, 0x00000001 >
    {
      typedef Register< uint32_t, reg_base + 0x58, access::rw, 0x00000001 > reg_type;
      static constexpr const char * name_str = "AHB3LPENR";

      typedef RegisterBits< reg_type,  0,  1 > FSMCLPEN;   /**< Flexible static memory controller module clock enable during Sleep mode  */
    };

    /**
     * APB1 peripheral clock enable in low power mode register
     */
    struct APB1LPENR
    : public Register< uint32_t, reg_base + 0x60, access::rw, 0x36FEC9FF >
    {
      typedef Register< uint32_t, reg_base + 0x60, access::rw, 0x36FEC9FF > reg_type;
      static constexpr const char * name_str = "APB1LPENR";

      typedef RegisterBits< reg_type, 29,  1 > DACLPEN;      /**< DAC interface clock enable during Sleep mode    */
      typedef RegisterBits< reg_type, 28,  1 > PWRLPEN;      /**< Power interface clock enable during Sleep mode  */
      typedef RegisterBits< reg_type, 26,  1 > CAN2LPEN;     /**< CAN 2 clock enable during Sleep mode            */
      typedef RegisterBits< reg_type, 25,  1 > CAN1LPEN;     /**< CAN 1 clock enable during Sleep mode            */
      typedef RegisterBits< reg_type, 23,  1 > I2C3LPEN;     /**< I2C3 clock enable during Sleep mode             */
      typedef RegisterBits< reg_type, 22,  1 > I2C2LPEN;     /**< I2C2 clock enable during Sleep mode             */
      typedef RegisterBits< reg_type, 21,  1 > I2C1LPEN;     /**< I2C1 clock enable during Sleep mode             */
      typedef RegisterBits< reg_type, 20,  1 > UART5LPEN;    /**< UART5 clock enable during Sleep mode            */
      typedef RegisterBits< reg_type, 19,  1 > UART4LPEN;    /**< UART4 clock enable during Sleep mode            */
      typedef RegisterBits< reg_type, 18,  1 > USART3LPEN;   /**< USART3 clock enable during Sleep mode           */
      typedef RegisterBits< reg_type, 17,  1 > USART2LPEN;   /**< USART2 clock enable during Sleep mode           */
      typedef RegisterBits< reg_type, 15,  1 > SPI3LPEN;     /**< SPI3 clock enable during Sleep mode             */
      typedef RegisterBits< reg_type, 14,  1 > SPI2LPEN;     /**< SPI2 clock enable during Sleep mode             */
      typedef RegisterBits< reg_type, 11,  1 > WWDGLPEN;     /**< Window watchdog clock enable during Sleep mode  */
      typedef RegisterBits< reg_type,  8,  1 > TIM14LPEN;    /**< TIM14 clock enable during Sleep mode            */
      typedef RegisterBits< reg_type,  7,  1 > TIM13LPEN;    /**< TIM13 clock enable during Sleep mode            */
      typedef RegisterBits< reg_type,  6,  1 > TIM12LPEN;    /**< TIM12 clock enable during Sleep mode            */
      typedef RegisterBits< reg_type,  5,  1 > TIM7LPEN;     /**< TIM7 clock enable during Sleep mode             */
      typedef RegisterBits< reg_type,  4,  1 > TIM6LPEN;     /**< TIM6 clock enable during Sleep mode             */
      typedef RegisterBits< reg_type,  3,  1 > TIM5LPEN;     /**< TIM5 clock enable during Sleep mode             */
      typedef RegisterBits< reg_type,  2,  1 > TIM4LPEN;     /**< TIM4 clock enable during Sleep mode             */
      typedef RegisterBits< reg_type,  1,  1 > TIM3LPEN;     /**< TIM3 clock enable during Sleep mode             */
      typedef RegisterBits< reg_type,  0,  1 > TIM2LPEN;     /**< TIM2 clock enable during Sleep mode             */
    };

    /**
     * APB2 peripheral clock enabled in low power mode register
     */
    struct APB2LPENR
    : public Register< uint32_t, reg_base + 0x64, access::rw, 0x00075F33 >
    {
      typedef Register< uint32_t, reg_base + 0x64, access::rw, 0x00075F33 > reg_type;
      static constexpr const char * name_str = "APB2LPENR";

      typedef RegisterBits< reg_type, 18,  1 > TIM11LPEN;    /**< TIM11 clock enable during Sleep mode                            */
      typedef RegisterBits< reg_type, 17,  1 > TIM10LPEN;    /**< TIM10 clock enable during Sleep mode                            */
      typedef RegisterBits< reg_type, 16,  1 > TIM9LPEN;     /**< TIM9 clock enable during sleep mode                             */
      typedef RegisterBits< reg_type, 14,  1 > SYSCFGLPEN;   /**< System configuration controller clock enable during Sleep mode  */
      typedef RegisterBits< reg_type, 12,  1 > SPI1LPEN;     /**< SPI 1 clock enable during Sleep mode                            */
      typedef RegisterBits< reg_type, 11,  1 > SDIOLPEN;     /**< SDIO clock enable during Sleep mode                             */
      typedef RegisterBits< reg_type, 10,  1 > ADC3LPEN;     /**< ADC 3 clock enable during Sleep mode                            */
      typedef RegisterBits< reg_type,  9,  1 > ADC2LPEN;     /**< ADC2 clock enable during Sleep mode                             */
      typedef RegisterBits< reg_type,  8,  1 > ADC1LPEN;     /**< ADC1 clock enable during Sleep mode                             */
      typedef RegisterBits< reg_type,  5,  1 > USART6LPEN;   /**< USART6 clock enable during Sleep mode                           */
      typedef RegisterBits< reg_type,  4,  1 > USART1LPEN;   /**< USART1 clock enable during Sleep mode                           */
      typedef RegisterBits< reg_type,  1,  1 > TIM8LPEN;     /**< TIM8 clock enable during Sleep mode                             */
      typedef RegisterBits< reg_type,  0,  1 > TIM1LPEN;     /**< TIM1 clock enable during Sleep mode                             */
    };

    /**
     * Backup domain control register
     */
    struct BDCR
    : public Register< uint32_t, reg_base + 0x70, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x70, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "BDCR";

      typedef RegisterBits< reg_type, 16,  1 > BDRST;     /**< Backup domain software reset          */
      typedef RegisterBits< reg_type, 15,  1 > RTCEN;     /**< RTC clock enable                      */
      typedef RegisterBits< reg_type,  9,  1 > RTCSEL1;   /**< RTC clock source selection            */
      typedef RegisterBits< reg_type,  8,  1 > RTCSEL0;   /**< RTC clock source selection            */
      typedef RegisterBits< reg_type,  2,  1 > LSEBYP;    /**< External low-speed oscillator bypass  */
      typedef RegisterBits< reg_type,  1,  1 > LSERDY;    /**< External low-speed oscillator ready   */
      typedef RegisterBits< reg_type,  0,  1 > LSEON;     /**< External low-speed oscillator enable  */
    };

    /**
     * clock control & status register
     */
    struct CSR
    : public Register< uint32_t, reg_base + 0x74, access::rw, 0x0E000000 >
    {
      typedef Register< uint32_t, reg_base + 0x74, access::rw, 0x0E000000 > reg_type;
      static constexpr const char * name_str = "CSR";

      typedef RegisterBits< reg_type, 31,  1 > LPWRRSTF;   /**< Low-power reset flag                  */
      typedef RegisterBits< reg_type, 30,  1 > WWDGRSTF;   /**< Window watchdog reset flag            */
      typedef RegisterBits< reg_type, 29,  1 > WDGRSTF;    /**< Independent watchdog reset flag       */
      typedef RegisterBits< reg_type, 28,  1 > SFTRSTF;    /**< Software reset flag                   */
      typedef RegisterBits< reg_type, 27,  1 > PORRSTF;    /**< POR/PDR reset flag                    */
      typedef RegisterBits< reg_type, 26,  1 > PADRSTF;    /**< PIN reset flag                        */
      typedef RegisterBits< reg_type, 25,  1 > BORRSTF;    /**< BOR reset flag                        */
      typedef RegisterBits< reg_type, 24,  1 > RMVF;       /**< Remove reset flag                     */
      typedef RegisterBits< reg_type,  1,  1 > LSIRDY;     /**< Internal low-speed oscillator ready   */
      typedef RegisterBits< reg_type,  0,  1 > LSION;      /**< Internal low-speed oscillator enable  */
    };

    /**
     * spread spectrum clock generation register
     */
    struct SSCGR
    : public Register< uint32_t, reg_base + 0x80, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x80, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "SSCGR";

      typedef RegisterBits< reg_type, 31,  1 > SSCGEN;      /**< Spread spectrum modulation enable  */
      typedef RegisterBits< reg_type, 30,  1 > SPREADSEL;   /**< Spread Select                      */
      typedef RegisterBits< reg_type, 13, 15 > INCSTEP;     /**< Incrementation step                */
      typedef RegisterBits< reg_type,  0, 13 > MODPER;      /**< Modulation period                  */
    };

    /**
     * PLLI2S configuration register
     */
    struct PLLI2SCFGR
    : public Register< uint32_t, reg_base + 0x84, access::rw, 0x20003000 >
    {
      typedef Register< uint32_t, reg_base + 0x84, access::rw, 0x20003000 > reg_type;
      static constexpr const char * name_str = "PLLI2SCFGR";

      typedef RegisterBits< reg_type, 28,  3 > PLLI2SRx;   /**< PLLI2S division factor for I2S clocks  */
      typedef RegisterBits< reg_type,  6,  9 > PLLI2SNx;   /**< PLLI2S multiplication factor for VCO   */
    };
  };


  /**
   * General-purpose and alternate-function I/Os (GPIOs and AFIOs)
   */
  template<char reg_name>
  struct GPIO
  {
    static_assert((reg_name >= 'A') && (reg_name <= 'I'), "Invalid index for GPIO register");

    static constexpr std::size_t gpio_no  = reg_name - 'A';
    static constexpr reg_addr_t  reg_base = 0x40020000 + (gpio_no * 0x0400);

    static constexpr uint32_t moder_reset   = ( reg_name == 'A' ? 0xA8000000 :
                                                reg_name == 'B' ? 0x00000280 :
                                                0x00000000 );

    static constexpr uint32_t ospeedr_reset = ( reg_name == 'B' ? 0x000000C0 :
                                                0x00000000 );

    static constexpr uint32_t pupdr_reset   = ( reg_name == 'A' ? 0x64000000 :
                                                reg_name == 'B' ? 0x00000100 :
                                                0x00000000 );
    
    typedef Register< uint32_t, reg_base + 0x00, access::rw, moder_reset   > MODER;   /**< GPIO port mode register               */
    typedef Register< uint32_t, reg_base + 0x04, access::rw                > OTYPER;  /**< GPIO port output type register        */
    typedef Register< uint32_t, reg_base + 0x08, access::rw, ospeedr_reset > OSPEEDR; /**< GPIO port output speed register       */
    typedef Register< uint32_t, reg_base + 0x0c, access::rw, pupdr_reset   > PUPDR;   /**< GPIO port pull-up/pull-down register  */
    typedef Register< uint32_t, reg_base + 0x10, access::ro /*0x0000XXXX*/ > IDR;     /**< GPIO port input data register         */
    typedef Register< uint32_t, reg_base + 0x14, access::rw                > ODR;     /**< GPIO port output data register        */
    typedef Register< uint32_t, reg_base + 0x18, access::wo                > BSRR;    /**< GPIO port bit set/reset register      */
    typedef Register< uint32_t, reg_base + 0x1c, access::rw                > LCKR;    /**< GPIO port configuration lock register */
    typedef Register< uint32_t, reg_base + 0x20, access::rw                > AFRL;    /**< GPIO alternate function low register  */
    typedef Register< uint32_t, reg_base + 0x24, access::rw                > AFRH;    /**< GPIO alternate function high register */
  };


  /**
   * FLASH
   */
  class FLASH
  {
  public:
    static constexpr reg_addr_t reg_base = 0x40023C00;
    static constexpr const char * name_str = "FLASH";

    /**
     * Flash access control register
     */
    struct ACR
    : public Register< uint32_t, reg_base + 0x0, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x0, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "ACR";

      typedef RegisterBits< reg_type,  0,  3 > LATENCY;   /**< Latency                   */
      typedef RegisterBits< reg_type,  8,  1 > PRFTEN;    /**< Prefetch enable           */
      typedef RegisterBits< reg_type,  9,  1 > ICEN;      /**< Instruction cache enable  */
      typedef RegisterBits< reg_type, 10,  1 > DCEN;      /**< Data cache enable         */
      typedef RegisterBits< reg_type, 11,  1 > ICRST;     /**< Instruction cache reset   */
      typedef RegisterBits< reg_type, 12,  1 > DCRST;     /**< Data cache reset          */
    };

    /**
     * Flash key register
     */
    struct KEYR
    : public Register< uint32_t, reg_base + 0x4, access::wo, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x4, access::wo, 0x00000000 > reg_type;
      static constexpr const char * name_str = "KEYR";
    };

    /**
     * Flash option key register
     */
    struct OPTKEYR
    : public Register< uint32_t, reg_base + 0x8, access::wo, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x8, access::wo, 0x00000000 > reg_type;
      static constexpr const char * name_str = "OPTKEYR";
    };

    /**
     * Status register
     */
    struct SR
    : public Register< uint32_t, reg_base + 0xC, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0xC, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "SR";

      typedef RegisterBits< reg_type,  0,  1 > EOP;      /**< End of operation               */
      typedef RegisterBits< reg_type,  1,  1 > OPERR;    /**< Operation error                */
      typedef RegisterBits< reg_type,  4,  1 > WRPERR;   /**< Write protection error         */
      typedef RegisterBits< reg_type,  5,  1 > PGAERR;   /**< Programming alignment error    */
      typedef RegisterBits< reg_type,  6,  1 > PGPERR;   /**< Programming parallelism error  */
      typedef RegisterBits< reg_type,  7,  1 > PGSERR;   /**< Programming sequence error     */
      typedef RegisterBits< reg_type, 16,  1 > BSY;      /**< Busy                           */
    };

    /**
     * Control register
     */
    struct CR
    : public Register< uint32_t, reg_base + 0x10, access::rw, 0x80000000 >
    {
      typedef Register< uint32_t, reg_base + 0x10, access::rw, 0x80000000 > reg_type;
      static constexpr const char * name_str = "CR";

      typedef RegisterBits< reg_type,  0,  1 > PG;      /**< Programming                        */
      typedef RegisterBits< reg_type,  1,  1 > SER;     /**< Sector Erase                       */
      typedef RegisterBits< reg_type,  2,  1 > MER;     /**< Mass Erase                         */
      typedef RegisterBits< reg_type,  3,  4 > SNB;     /**< Sector number                      */
      typedef RegisterBits< reg_type,  8,  2 > PSIZE;   /**< Program size                       */
      typedef RegisterBits< reg_type, 16,  1 > STRT;    /**< Start                              */
      typedef RegisterBits< reg_type, 24,  1 > EOPIE;   /**< End of operation interrupt enable  */
      typedef RegisterBits< reg_type, 25,  1 > ERRIE;   /**< Error interrupt enable             */
      typedef RegisterBits< reg_type, 31,  1 > LOCK;    /**< Lock                               */
    };

    /**
     * Flash option control register
     */
    struct OPTCR
    : public Register< uint32_t, reg_base + 0x14, access::rw, 0x00000014 >
    {
      typedef Register< uint32_t, reg_base + 0x14, access::rw, 0x00000014 > reg_type;
      static constexpr const char * name_str = "OPTCR";

      typedef RegisterBits< reg_type,  0,  1 > OPTLOCK;      /**< Option lock                   */
      typedef RegisterBits< reg_type,  1,  1 > OPTSTRT;      /**< Option start                  */
      typedef RegisterBits< reg_type,  2,  2 > BOR_LEV;      /**< BOR reset Level               */
      typedef RegisterBits< reg_type,  5,  1 > WDG_SW;       /**< WDG_SW User option bytes      */
      typedef RegisterBits< reg_type,  6,  1 > nRST_STOP;    /**< nRST_STOP User option bytes   */
      typedef RegisterBits< reg_type,  7,  1 > nRST_STDBY;   /**< nRST_STDBY User option bytes  */
      typedef RegisterBits< reg_type,  8,  8 > RDP;          /**< Read protect                  */
      typedef RegisterBits< reg_type, 16, 12 > nWRP;         /**< Not write protect             */
    };
  };


  /**
   * Power control
   */
  class PWR
  {
  public:
    static constexpr reg_addr_t reg_base = 0x40007000;
    static constexpr const char * name_str = "PWR";

    /**
     * power control register
     */
    struct CR
    : public Register< uint32_t, reg_base + 0x0, access::rw, 0x00004000 >
    {
      typedef Register< uint32_t, reg_base + 0x0, access::rw, 0x00004000 > reg_type;
      static constexpr const char * name_str = "CR";

      typedef RegisterBits< reg_type, 14,  1 > VOS;    /**< Regulator voltage scaling output selection   */
      typedef RegisterBits< reg_type,  9,  1 > FPDS;   /**< Flash power down in Stop mode           */
      typedef RegisterBits< reg_type,  8,  1 > DBP;    /**< Disable backup domain write protection  */
      typedef RegisterBits< reg_type,  5,  3 > PLS;    /**< PVD level selection                     */
      typedef RegisterBits< reg_type,  4,  1 > PVDE;   /**< Power voltage detector enable           */
      typedef RegisterBits< reg_type,  3,  1 > CSBF;   /**< Clear standby flag                      */
      typedef RegisterBits< reg_type,  2,  1 > CWUF;   /**< Clear wakeup flag                       */
      typedef RegisterBits< reg_type,  1,  1 > PDDS;   /**< Power down deepsleep                    */
      typedef RegisterBits< reg_type,  0,  1 > LPDS;   /**< Low-power deep sleep                    */
    };

    /**
     * power control/status register
     */
    struct CSR
    : public Register< uint32_t, reg_base + 0x4, access::rw, 0x00000000 >
    {
      typedef Register< uint32_t, reg_base + 0x4, access::rw, 0x00000000 > reg_type;
      static constexpr const char * name_str = "CSR";

      typedef RegisterBits< reg_type,  0,  1 > WUF;      /**< Wakeup flag                                           */
      typedef RegisterBits< reg_type,  1,  1 > SBF;      /**< Standby flag                                          */
      typedef RegisterBits< reg_type,  2,  1 > PVDO;     /**< PVD output                                            */
      typedef RegisterBits< reg_type,  3,  1 > BRR;      /**< Backup regulator ready                                */
      typedef RegisterBits< reg_type,  8,  1 > EWUP;     /**< Enable WKUP pin                                       */
      typedef RegisterBits< reg_type,  9,  1 > BRE;      /**< Backup regulator enable                               */
      typedef RegisterBits< reg_type, 14,  1 > VOSRDY;   /**< Regulator voltage scaling output selection ready bit  */
    };
  };
};


#endif // CORE_REG_HPP_INCLUDED
