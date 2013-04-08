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

#ifndef REG_RCC_HPP_INCLUDED
#define REG_RCC_HPP_INCLUDED

#include <register.hpp>

namespace reg
{
  /**
   * Reset and clock control
   */
  class RCC
  {
    static constexpr reg_addr_t base_addr = 0x40021000;
    static constexpr const char * name_str = "RCC";

    /**
     * Clock control register
     */
    template<typename R>
    struct __CR
    : public R
    {
      // TODO: some (!) here are read-only bits
      typedef RegisterBits< R,  0,  1 > HSION;         /**< [ 0: 0] Internal High Speed clock enable        */
      typedef RegisterBits< R,  1,  1 > HSIRDY;        /**< [ 1: 1] Internal High Speed clock ready flag    */
      typedef RegisterBits< R,  3,  5 > HSITRIM;       /**< [ 7: 3] Internal High Speed clock trimming      */
      typedef RegisterBits< R,  8,  8 > HSICAL;        /**< [15: 8] Internal High Speed clock Calibration   */
      typedef RegisterBits< R, 16,  1 > HSEON;         /**< [16:16] External High Speed clock enable        */
      typedef RegisterBits< R, 17,  1 > HSERDY;        /**< [17:17] External High Speed clock ready flag    */
      typedef RegisterBits< R, 18,  1 > HSEBYP;        /**< [18:18] External High Speed clock Bypass        */
      typedef RegisterBits< R, 19,  1 > CSSON;         /**< [19:19] Clock Security System enable            */
      typedef RegisterBits< R, 24,  1 > PLLON;         /**< [24:24] PLL enable                              */
      typedef RegisterBits< R, 25,  1 > PLLRDY;        /**< [25:25] PLL clock ready flag                    */

#ifdef STM32F10X_CL
      typedef RegisterBits< R, 26,  1 > PLL2ON;        /**< [26:26] PLL2 enable             */
      typedef RegisterBits< R, 27,  1 > PLL2RDY;       /**< [27:27] PLL2 clock ready flag   */
      typedef RegisterBits< R, 28,  1 > PLL3ON;        /**< [28:28] PLL3 enable             */
      typedef RegisterBits< R, 29,  1 > PLL3RDY;       /**< [29:29] PLL3 clock ready flag   */
#endif /* STM32F10X_CL */
    };


    /*
     *  Clock configuration register
     */
    template<typename R>
    class __CFGR
    : public R
    {
      /** System clock Switch (Status) */
      template<typename Rb>
      struct __SWx
      : public Rb
      {
        typedef RegisterConst< Rb, 0x0 > HSI;          /**< 00: HSI selected as system clock   */
        typedef RegisterConst< Rb, 0x1 > HSE;          /**< 01: HSE selected as system clock   */
        typedef RegisterConst< Rb, 0x2 > PLL;          /**< 10: PLL selected as system clock   */
      };

      /** AHB prescaler  */
      template<typename Rb>
      struct __HPRE
      : public Rb
      {
        typedef RegisterConst< Rb, 0x0 > DIV1;         /**< 0xxx: SYSCLK not divided      */
        typedef RegisterConst< Rb, 0x8 > DIV2;         /**< 1000: SYSCLK divided by 2     */
        typedef RegisterConst< Rb, 0x9 > DIV4;         /**< 1001: SYSCLK divided by 4     */
        typedef RegisterConst< Rb, 0xA > DIV8;         /**< 1010: SYSCLK divided by 8     */
        typedef RegisterConst< Rb, 0xB > DIV16;        /**< 1011: SYSCLK divided by 16    */
        typedef RegisterConst< Rb, 0xC > DIV64;        /**< 1100: SYSCLK divided by 64    */
        typedef RegisterConst< Rb, 0xD > DIV128;       /**< 1101: SYSCLK divided by 128   */
        typedef RegisterConst< Rb, 0xE > DIV256;       /**< 1110: SYSCLK divided by 256   */
        typedef RegisterConst< Rb, 0xF > DIV512;       /**< 1111: SYSCLK divided by 512   */
      };

      /** APB1/2 prescaler  */
      template<typename Rb>
      struct __PPREx
      : public Rb
      {
        typedef RegisterConst< Rb, 0x0 > DIV1;         /**< 0xx: HCLK not divided     */
        typedef RegisterConst< Rb, 0x4 > DIV2;         /**< 100: HCLK divided by 2    */
        typedef RegisterConst< Rb, 0x5 > DIV4;         /**< 101: HCLK divided by 4    */
        typedef RegisterConst< Rb, 0x6 > DIV8;         /**< 110: HCLK divided by 8    */
        typedef RegisterConst< Rb, 0x7 > DIV16;        /**< 111: HCLK divided by 16   */
      };

      /** ADC prescaler  */
      template<typename Rb>
      struct __ADCPRE
      : public Rb
      {
        typedef RegisterConst< Rb, 0x0 > DIV2;         /**< 00: PCLK2 divided by 2   */
        typedef RegisterConst< Rb, 0x1 > DIV4;         /**< 01: PCLK2 divided by 4   */
        typedef RegisterConst< Rb, 0x2 > DIV6;         /**< 10: PCLK2 divided by 6   */
        typedef RegisterConst< Rb, 0x3 > DIV8;         /**< 11: PCLK2 divided by 8   */
      };

      /** PLL entry clock source  */
      template<typename Rb>
      struct __PLLSRC
      : public Rb
      {
        typedef RegisterConst< Rb, 0x0 > HSI_DIV2;     /**< 00: HSI clock divided by 2 selected as PLL input clock   */
#ifdef STM32F10X_CL
        typedef RegisterConst< Rb, 0x1 > PREDIV1;      /**< 01: Clock from PREDIV1 selected as PLL input clock   */
#else
        typedef RegisterConst< Rb, 0x1 > HSE;          /**< 01: HSE oscillator clock selected as PLL input clock   */
#endif
      };

      /**
       * - LSB of division factor PREDIV1 (Connectivity Line Devices)
       * - HSE divider for PLL entry (Low-, medium-, high- and XL-density Devices)
       */
      template<typename Rb>
      struct __PLLXTPRE
      : public Rb
      {
#ifdef STM32F10X_CL
        typedef RegisterConst< Rb, 0x0 > PREDIV1_DIV1; /**< 00: PREDIV1 clock not divided for PLL entry    */
        typedef RegisterConst< Rb, 0x1 > PREDIV1_DIV2; /**< 01: PREDIV1 clock divided by 2 for PLL entry   */
#else
        typedef RegisterConst< Rb, 0x0 > HSE_DIV1;     /**< 00: HSE clock not divided    */
        typedef RegisterConst< Rb, 0x1 > HSE_DIV2;     /**< 01: HSE clock divided by 2   */
#endif
      };

      /** PLL multiplication factor  */
      template<typename Rb>
      struct __PLLMUL
      : public Rb
      {
#ifdef STM32F10X_CL
        typedef RegisterConst< Rb, 0x2 > MUL4;         /**< 0010: PLL input clock * 4     */
        typedef RegisterConst< Rb, 0x3 > MUL5;         /**< 0011: PLL input clock * 5     */
        typedef RegisterConst< Rb, 0x4 > MUL6;         /**< 0100: PLL input clock * 6     */
        typedef RegisterConst< Rb, 0x5 > MUL7;         /**< 0101: PLL input clock * 7     */
        typedef RegisterConst< Rb, 0x6 > MUL8;         /**< 0110: PLL input clock * 8     */
        typedef RegisterConst< Rb, 0x7 > MUL9;         /**< 0111: PLL input clock * 9     */
        typedef RegisterConst< Rb, 0xD > MUL6_5;       /**< 1101: PLL input clock * 6.5   */
#else
        typedef RegisterConst< Rb, 0x0 > MUL2;         /**< 0000: PLL input clock*2    */
        typedef RegisterConst< Rb, 0x1 > MUL3;         /**< 0001: PLL input clock*3    */
        typedef RegisterConst< Rb, 0x2 > MUL4;         /**< 0010: PLL input clock*4    */
        typedef RegisterConst< Rb, 0x3 > MUL5;         /**< 0011: PLL input clock*5    */
        typedef RegisterConst< Rb, 0x4 > MUL6;         /**< 0100: PLL input clock*6    */
        typedef RegisterConst< Rb, 0x5 > MUL7;         /**< 0101: PLL input clock*7    */
        typedef RegisterConst< Rb, 0x6 > MUL8;         /**< 0110: PLL input clock*8    */
        typedef RegisterConst< Rb, 0x7 > MUL9;         /**< 0111: PLL input clock*9    */
        typedef RegisterConst< Rb, 0x8 > MUL10;        /**< 1000: PLL input clock10    */
        typedef RegisterConst< Rb, 0x9 > MUL11;        /**< 1001: PLL input clock*11   */
        typedef RegisterConst< Rb, 0xA > MUL12;        /**< 1010: PLL input clock*12   */
        typedef RegisterConst< Rb, 0xB > MUL13;        /**< 1011: PLL input clock*13   */
        typedef RegisterConst< Rb, 0xC > MUL14;        /**< 1100: PLL input clock*14   */
        typedef RegisterConst< Rb, 0xD > MUL15;        /**< 1101: PLL input clock*15   */
        typedef RegisterConst< Rb, 0xE > MUL16;        /**< 1110: PLL input clock*16   */
#endif
      };

#ifdef STM32F10X_CL
      /** USB OTG FS prescaler  */
      template<typename Rb>
      struct __OTGFSPRE
      : public Rb
      {
        typedef RegisterConst< Rb, 0x0 > DIV3;
        typedef RegisterConst< Rb, 0x1 > DIV2;
      };
#else
      /** USB prescaler  */
      template<typename Rb>
      struct __USBPRE
      : public Rb
      {
        typedef RegisterConst< Rb, 0x0 > DIV1_5;
        typedef RegisterConst< Rb, 0x1 > DIV1;
      };
#endif

      /** Microcontroller Clock Output  */
      template<typename Rb>
      struct __MCO
      : public Rb
      {
        typedef RegisterConst< Rb, 0x0 > NOCLOCK;      /**< 00xx: No clock                          */
        typedef RegisterConst< Rb, 0x4 > SYSCLK;       /**< 0100: System clock selected             */
        typedef RegisterConst< Rb, 0x5 > HSI;          /**< 0101: HSI clock selected                */
        typedef RegisterConst< Rb, 0x6 > HSE;          /**< 0110: HSE clock selected                */
        typedef RegisterConst< Rb, 0x7 > PLL_DIV2;     /**< 0111: PLL clock divided by 2 selected   */
#ifdef STM32F10X_CL
        typedef RegisterConst< Rb, 0x8 > PLL2;         /**< 1000: PLL2 clock selected                                              */
        typedef RegisterConst< Rb, 0x9 > PLL3_DIV2;    /**< 1001: PLL3 clock divided by 2 selected                                 */
        typedef RegisterConst< Rb, 0xA > Ext_HSE;      /**< 1010: XT1 external 3-25 MHz oscillator clock selected (for Ethernet)   */
        typedef RegisterConst< Rb, 0xB > PLL3;         /**< 1011: PLL3 clock selected (for Ethernet)                               */
#endif
      };

    public:

      typedef __SWx     < RegisterBits< R,  0,  2 > > SW;       /**< [ 1: 0] System clock Switch                                          */
      typedef __SWx     < RegisterBits< R,  2,  2 > > SWS;      /**< [ 3: 2] System Clock Switch Status                                   */
      typedef __HPRE    < RegisterBits< R,  4,  4 > > HPRE;     /**< [ 7: 4] AHB prescaler                                                */
      typedef __PPREx   < RegisterBits< R,  8,  3 > > PPRE1;    /**< [10: 8] APB low-speed prescaler (APB1)                               */
      typedef __PPREx   < RegisterBits< R, 11,  3 > > PPRE2;    /**< [13:11] APB high-speed prescaler (APB2)                              */
      typedef __ADCPRE  < RegisterBits< R, 14,  2 > > ADCPRE;   /**< [15:14] ADC prescaler                                                */
      typedef __PLLSRC  < RegisterBits< R, 16,  1 > > PLLSRC;   /**< [16:16] PLL entry clock source                                       */
      typedef __PLLXTPRE< RegisterBits< R, 17,  1 > > PLLXTPRE; /**< [17:17] HSE divider for PLL entry / LSB of division factor PREDIV1   */
      typedef __PLLMUL  < RegisterBits< R, 18,  4 > > PLLMUL;   /**< [21:18] PLL multiplication factor                                    */
#ifdef STM32F10X_CL
      typedef __OTGFSPRE< RegisterBits< R, 22,  1 > > OTGFSPRE; /**< [22:22] USB OTG FS prescaler                                         */
      typedef __MCO     < RegisterBits< R, 24,  4 > > MCO; /**< [27:24] Microcontroller Clock Output   */
#else
      typedef __USBPRE  < RegisterBits< R, 22,  1 > > USBPRE;   /**< [22:22] USB prescaler                                                */
      typedef __MCO     < RegisterBits< R, 24,  3 > > MCO; /**< [26:24] Microcontroller Clock Output   */
#endif
    };


    /**
     * Clock interrupt register
     */
    template<typename R>
    struct __CIR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > LSIRDYF;       /**< [ 0: 0] LSI Ready Interrupt flag                */
      typedef RegisterBits< R,  1,  1 > LSERDYF;       /**< [ 1: 1] LSE Ready Interrupt flag                */
      typedef RegisterBits< R,  2,  1 > HSIRDYF;       /**< [ 2: 2] HSI Ready Interrupt flag                */
      typedef RegisterBits< R,  3,  1 > HSERDYF;       /**< [ 3: 3] HSE Ready Interrupt flag                */
      typedef RegisterBits< R,  4,  1 > PLLRDYF;       /**< [ 4: 4] PLL Ready Interrupt flag                */
      typedef RegisterBits< R,  7,  1 > CSSF;          /**< [ 7: 7] Clock Security System Interrupt flag    */
      typedef RegisterBits< R,  8,  1 > LSIRDYIE;      /**< [ 8: 8] LSI Ready Interrupt Enable              */
      typedef RegisterBits< R,  9,  1 > LSERDYIE;      /**< [ 9: 9] LSE Ready Interrupt Enable              */
      typedef RegisterBits< R, 10,  1 > HSIRDYIE;      /**< [10:10] HSI Ready Interrupt Enable              */
      typedef RegisterBits< R, 11,  1 > HSERDYIE;      /**< [11:11] HSE Ready Interrupt Enable              */
      typedef RegisterBits< R, 12,  1 > PLLRDYIE;      /**< [12:12] PLL Ready Interrupt Enable              */
      typedef RegisterBits< R, 16,  1 > LSIRDYC;       /**< [16:16] LSI Ready Interrupt Clear               */
      typedef RegisterBits< R, 17,  1 > LSERDYC;       /**< [17:17] LSE Ready Interrupt Clear               */
      typedef RegisterBits< R, 18,  1 > HSIRDYC;       /**< [18:18] HSI Ready Interrupt Clear               */
      typedef RegisterBits< R, 19,  1 > HSERDYC;       /**< [19:19] HSE Ready Interrupt Clear               */
      typedef RegisterBits< R, 20,  1 > PLLRDYC;       /**< [20:20] PLL Ready Interrupt Clear               */
      typedef RegisterBits< R, 23,  1 > CSSC;          /**< [23:23] Clock Security System Interrupt Clear   */

#ifdef STM32F10X_CL
      typedef RegisterBits< R,  5,  1 > PLL2RDYF;      /**< [ 5: 5] PLL2 Ready Interrupt flag     */
      typedef RegisterBits< R,  6,  1 > PLL3RDYF;      /**< [ 6: 6] PLL3 Ready Interrupt flag     */
      typedef RegisterBits< R, 13,  1 > PLL2RDYIE;     /**< [13:13] PLL2 Ready Interrupt Enable   */
      typedef RegisterBits< R, 14,  1 > PLL3RDYIE;     /**< [14:14] PLL3 Ready Interrupt Enable   */
      typedef RegisterBits< R, 21,  1 > PLL2RDYC;      /**< [21:21] PLL2 Ready Interrupt Clear    */
      typedef RegisterBits< R, 22,  1 > PLL3RDYC;      /**< [22:22] PLL3 Ready Interrupt Clear    */
#endif /* STM32F10X_CL */
    };


    /**
     * APB2 peripheral reset register
     */
    template<typename R>
    struct __APB2RSTR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > AFIORST;       /**< [ 0: 0] Alternate Function I/O reset   */
      typedef RegisterBits< R,  2,  1 > IOPARST;       /**< [ 2: 2] I/O port A reset               */
      typedef RegisterBits< R,  3,  1 > IOPBRST;       /**< [ 3: 3] I/O port B reset               */
      typedef RegisterBits< R,  4,  1 > IOPCRST;       /**< [ 4: 4] I/O port C reset               */
      typedef RegisterBits< R,  5,  1 > IOPDRST;       /**< [ 5: 5] I/O port D reset               */
      typedef RegisterBits< R,  9,  1 > ADC1RST;       /**< [ 9: 9] ADC 1 interface reset          */

#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL)
      typedef RegisterBits< R, 10,  1 > ADC2RST;       /**< [10:10] ADC 2 interface reset   */
#endif

      typedef RegisterBits< R, 11,  1 > TIM1RST;       /**< [11:11] TIM1 Timer reset   */
      typedef RegisterBits< R, 12,  1 > SPI1RST;       /**< [12:12] SPI 1 reset        */
      typedef RegisterBits< R, 14,  1 > USART1RST;     /**< [14:14] USART1 reset       */

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL)
      typedef RegisterBits< R, 16,  1 > TIM15RST;      /**< [16:16] TIM15 Timer reset   */
      typedef RegisterBits< R, 17,  1 > TIM16RST;      /**< [17:17] TIM16 Timer reset   */
      typedef RegisterBits< R, 18,  1 > TIM17RST;      /**< [18:18] TIM17 Timer reset   */
#endif

#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
      typedef RegisterBits< R,  6,  1 > IOPERST;       /**< [ 6: 6] I/O port E reset   */
#endif /* STM32F10X_LD && STM32F10X_LD_VL */

#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
      typedef RegisterBits< R,  7,  1 > IOPFRST;       /**< [ 7: 7] I/O port F reset       */
      typedef RegisterBits< R,  8,  1 > IOPGRST;       /**< [ 8: 8] I/O port G reset       */
      typedef RegisterBits< R, 13,  1 > TIM8RST;       /**< [13:13] TIM8 Timer reset       */
      typedef RegisterBits< R, 15,  1 > ADC3RST;       /**< [15:15] ADC3 interface reset   */
#endif

#ifdef STM32F10X_XL
      typedef RegisterBits< R, 19,  1 > TIM9RST;       /**< [19:19] TIM9 Timer reset    */
      typedef RegisterBits< R, 20,  1 > TIM10RST;      /**< [20:20] TIM10 Timer reset   */
      typedef RegisterBits< R, 21,  1 > TIM11RST;      /**< [21:21] TIM11 Timer reset   */
#endif /* STM32F10X_XL */
    };


    /**
     * APB1 peripheral reset register
     */
    template<typename R>
    struct __APB1RSTR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > TIM2RST;       /**< [ 0: 0] Timer 2 reset           */
      typedef RegisterBits< R,  1,  1 > TIM3RST;       /**< [ 1: 1] Timer 3 reset           */
      typedef RegisterBits< R, 11,  1 > WWDGRST;       /**< [11:11] Window Watchdog reset   */
      typedef RegisterBits< R, 17,  1 > USART2RST;     /**< [17:17] USART 2 reset           */
      typedef RegisterBits< R, 21,  1 > I2C1RST;       /**< [21:21] I2C 1 reset             */

#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL)
      typedef RegisterBits< R, 25,  1 > CAN1RST;       /**< [25:25] CAN1 reset   */
#endif

      typedef RegisterBits< R, 27,  1 > BKPRST;        /**< [27:27] Backup interface reset   */
      typedef RegisterBits< R, 28,  1 > PWRRST;        /**< [28:28] Power interface reset    */

#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
      typedef RegisterBits< R,  2,  1 > TIM4RST;       /**< [ 2: 2] Timer 4 reset    */
      typedef RegisterBits< R, 14,  1 > SPI2RST;       /**< [14:14] SPI 2 reset      */
      typedef RegisterBits< R, 18,  1 > USART3RST;     /**< [18:18] RUSART 3 reset   */
      typedef RegisterBits< R, 22,  1 > I2C2RST;       /**< [22:22] I2C 2 reset      */
#endif /* STM32F10X_LD && STM32F10X_LD_VL */

#if defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined (STM32F10X_LD) || defined  (STM32F10X_XL)
      typedef RegisterBits< R, 23,  1 > USBRST;        /**< [23:23] USB Device reset   */
#endif

#if defined (STM32F10X_HD) || defined  (STM32F10X_CL) || defined  (STM32F10X_XL)
      typedef RegisterBits< R,  3,  1 > TIM5RST;       /**< [ 3: 3] Timer 5 reset         */
      typedef RegisterBits< R,  4,  1 > TIM6RST;       /**< [ 4: 4] Timer 6 reset         */
      typedef RegisterBits< R,  5,  1 > TIM7RST;       /**< [ 5: 5] Timer 7 reset         */
      typedef RegisterBits< R, 15,  1 > SPI3RST;       /**< [15:15] SPI 3 reset           */
      typedef RegisterBits< R, 19,  1 > UART4RST;      /**< [19:19] UART 4 reset          */
      typedef RegisterBits< R, 20,  1 > UART5RST;      /**< [20:20] UART 5 reset          */
      typedef RegisterBits< R, 29,  1 > DACRST;        /**< [29:29] DAC interface reset   */
#endif

#if defined (STM32F10X_LD_VL) || defined  (STM32F10X_MD_VL)
      typedef RegisterBits< R,  4,  1 > TIM6RST;       /**< [ 4: 4] Timer 6 reset         */
      typedef RegisterBits< R,  5,  1 > TIM7RST;       /**< [ 5: 5] Timer 7 reset         */
      typedef RegisterBits< R, 29,  1 > DACRST;        /**< [29:29] DAC interface reset   */
      typedef RegisterBits< R, 30,  1 > CECRST;        /**< [30:30] CEC interface reset   */
#endif

#ifdef STM32F10X_CL
      typedef RegisterBits< R, 26,  1 > CAN2RST;       /**< [26:26] CAN2 reset   */
#endif /* STM32F10X_CL */

#ifdef STM32F10X_XL
      typedef RegisterBits< R,  6,  1 > TIM12RST;      /**< [ 6: 6] TIM12 Timer reset   */
      typedef RegisterBits< R,  7,  1 > TIM13RST;      /**< [ 7: 7] TIM13 Timer reset   */
      typedef RegisterBits< R,  8,  1 > TIM14RST;      /**< [ 8: 8] TIM14 Timer reset   */
#endif /* STM32F10X_XL */
    };


    /**
     * AHB peripheral clock enable register
     */
    template<typename R>
    struct __AHBENR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > DMA1EN;        /**< [ 0: 0] DMA1 clock enable             */
      typedef RegisterBits< R,  2,  1 > SRAMEN;        /**< [ 2: 2] SRAM interface clock enable   */
      typedef RegisterBits< R,  4,  1 > FLITFEN;       /**< [ 4: 4] FLITF clock enable            */
      typedef RegisterBits< R,  6,  1 > CRCEN;         /**< [ 6: 6] CRC clock enable              */

#if defined (STM32F10X_HD) || defined  (STM32F10X_CL)
      typedef RegisterBits< R,  1,  1 > DMA2EN;        /**< [ 1: 1] DMA2 clock enable   */
#endif

#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
      typedef RegisterBits< R,  8,  1 > FSMCEN;        /**< [ 8: 8] FSMC clock enable   */
      typedef RegisterBits< R, 10,  1 > SDIOEN;        /**< [10:10] SDIO clock enable   */
#endif

#ifdef STM32F10X_CL
      typedef RegisterBits< R, 12,  1 > OTGFSEN;       /**< [12:12] USB OTG FS clock enable        */
      typedef RegisterBits< R, 14,  1 > ETHMACEN;      /**< [14:14] ETHERNET MAC clock enable      */
      typedef RegisterBits< R, 15,  1 > ETHMACTXEN;    /**< [15:15] ETHERNET MAC Tx clock enable   */
      typedef RegisterBits< R, 16,  1 > ETHMACRXEN;    /**< [16:16] ETHERNET MAC Rx clock enable   */
#endif /* STM32F10X_CL */
    };


    /**
     * APB2 peripheral clock enable register
     */
    template<typename R>
    struct __APB2ENR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > AFIOEN;        /**< [ 0: 0] Alternate Function I/O clock enable   */
      typedef RegisterBits< R,  2,  1 > IOPAEN;        /**< [ 2: 2] I/O port A clock enable               */
      typedef RegisterBits< R,  3,  1 > IOPBEN;        /**< [ 3: 3] I/O port B clock enable               */
      typedef RegisterBits< R,  4,  1 > IOPCEN;        /**< [ 4: 4] I/O port C clock enable               */
      typedef RegisterBits< R,  5,  1 > IOPDEN;        /**< [ 5: 5] I/O port D clock enable               */
      typedef RegisterBits< R,  9,  1 > ADC1EN;        /**< [ 9: 9] ADC 1 interface clock enable          */

#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL)
      typedef RegisterBits< R, 10,  1 > ADC2EN;        /**< [10:10] ADC 2 interface clock enable   */
#endif

      typedef RegisterBits< R, 11,  1 > TIM1EN;        /**< [11:11] TIM1 Timer clock enable   */
      typedef RegisterBits< R, 12,  1 > SPI1EN;        /**< [12:12] SPI 1 clock enable        */
      typedef RegisterBits< R, 14,  1 > USART1EN;      /**< [14:14] USART1 clock enable       */

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL)
      typedef RegisterBits< R, 16,  1 > TIM15EN;       /**< [16:16] TIM15 Timer clock enable   */
      typedef RegisterBits< R, 17,  1 > TIM16EN;       /**< [17:17] TIM16 Timer clock enable   */
      typedef RegisterBits< R, 18,  1 > TIM17EN;       /**< [18:18] TIM17 Timer clock enable   */
#endif

#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
      typedef RegisterBits< R,  6,  1 > IOPEEN;        /**< [ 6: 6] I/O port E clock enable   */
#endif /* STM32F10X_LD && STM32F10X_LD_VL */

#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
      typedef RegisterBits< R,  7,  1 > IOPFEN;        /**< [ 7: 7] I/O port F clock enable   */
      typedef RegisterBits< R,  8,  1 > IOPGEN;        /**< [ 8: 8] I/O port G clock enable   */
      typedef RegisterBits< R, 13,  1 > TIM8EN;        /**< [13:13] TIM8 Timer clock enable   */
      typedef RegisterBits< R, 15,  1 > ADC3EN;        /**< [15:15] DMA1 clock enable         */
#endif

#ifdef STM32F10X_XL
      typedef RegisterBits< R, 19,  1 > TIM9EN;        /**< [19:19] TIM9 Timer clock enable    */
      typedef RegisterBits< R, 20,  1 > TIM10EN;       /**< [20:20] TIM10 Timer clock enable   */
      typedef RegisterBits< R, 21,  1 > TIM11EN;       /**< [21:21] TIM11 Timer clock enable   */
#endif
    };


    /**
     * APB1 peripheral clock enable register
     */
    template<typename R>
    struct __APB1ENR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > TIM2EN;        /**< [ 0: 0] Timer 2 clock enabled          */
      typedef RegisterBits< R,  1,  1 > TIM3EN;        /**< [ 1: 1] Timer 3 clock enable           */
      typedef RegisterBits< R, 11,  1 > WWDGEN;        /**< [11:11] Window Watchdog clock enable   */
      typedef RegisterBits< R, 17,  1 > USART2EN;      /**< [17:17] USART 2 clock enable           */
      typedef RegisterBits< R, 21,  1 > I2C1EN;        /**< [21:21] I2C 1 clock enable             */

#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL)
      typedef RegisterBits< R, 25,  1 > CAN1EN;        /**< [25:25] CAN1 clock enable   */
#endif

      typedef RegisterBits< R, 27,  1 > BKPEN;         /**< [27:27] Backup interface clock enable   */
      typedef RegisterBits< R, 28,  1 > PWREN;         /**< [28:28] Power interface clock enable    */

#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
      typedef RegisterBits< R,  2,  1 > TIM4EN;        /**< [ 2: 2] Timer 4 clock enable   */
      typedef RegisterBits< R, 14,  1 > SPI2EN;        /**< [14:14] SPI 2 clock enable     */
      typedef RegisterBits< R, 18,  1 > USART3EN;      /**< [18:18] USART 3 clock enable   */
      typedef RegisterBits< R, 22,  1 > I2C2EN;        /**< [22:22] I2C 2 clock enable     */
#endif /* STM32F10X_LD && STM32F10X_LD_VL */

#if defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined  (STM32F10X_LD)
      typedef RegisterBits< R, 23,  1 > USBEN;         /**< [23:23] USB Device clock enable   */
#endif

#if defined (STM32F10X_HD) || defined  (STM32F10X_CL)
      typedef RegisterBits< R,  3,  1 > TIM5EN;        /**< [ 3: 3] Timer 5 clock enable         */
      typedef RegisterBits< R,  4,  1 > TIM6EN;        /**< [ 4: 4] Timer 6 clock enable         */
      typedef RegisterBits< R,  5,  1 > TIM7EN;        /**< [ 5: 5] Timer 7 clock enable         */
      typedef RegisterBits< R, 15,  1 > SPI3EN;        /**< [15:15] SPI 3 clock enable           */
      typedef RegisterBits< R, 19,  1 > UART4EN;       /**< [19:19] UART 4 clock enable          */
      typedef RegisterBits< R, 20,  1 > UART5EN;       /**< [20:20] UART 5 clock enable          */
      typedef RegisterBits< R, 29,  1 > DACEN;         /**< [29:29] DAC interface clock enable   */
#endif

#if defined (STM32F10X_LD_VL) || defined  (STM32F10X_MD_VL)
      typedef RegisterBits< R,  4,  1 > TIM6EN;        /**< [ 4: 4] Timer 6 clock enable         */
      typedef RegisterBits< R,  5,  1 > TIM7EN;        /**< [ 5: 5] Timer 7 clock enable         */
      typedef RegisterBits< R, 29,  1 > DACEN;         /**< [29:29] DAC interface clock enable   */
      typedef RegisterBits< R, 30,  1 > CECEN;         /**< [30:30] CEC interface clock enable   */
#endif

#ifdef STM32F10X_CL
      typedef RegisterBits< R, 26,  1 > CAN2EN;        /**< [26:26] CAN2 clock enable   */
#endif /* STM32F10X_CL */

#ifdef STM32F10X_XL
      typedef RegisterBits< R,  6,  1 > TIM12EN;       /**< [ 6: 6] TIM12 Timer clock enable   */
      typedef RegisterBits< R,  7,  1 > TIM13EN;       /**< [ 7: 7] TIM13 Timer clock enable   */
      typedef RegisterBits< R,  8,  1 > TIM14EN;       /**< [ 8: 8] TIM14 Timer clock enable   */
#endif /* STM32F10X_XL */
    };


    /**
     * Backup domain control register
     */
    template<typename R>
    class __BDCR
    : public R
    {
      /** RTC clock source selection  */
      template<typename Rb>
      struct __RTCSEL
      : public Rb
      {
        typedef RegisterConst< Rb, 0x1 > LSE;          /**< 01: LSE oscillator clock used as RTC clock                  */
        typedef RegisterConst< Rb, 0x2 > LSI;          /**< 10: LSI oscillator clock used as RTC clock                  */
        typedef RegisterConst< Rb, 0x3 > HSE;          /**< 11: HSE oscillator clock divided by 128 used as RTC clock   */
      };

    public:

      typedef            RegisterBits< R,  0,  1 >   LSEON;  /**< [ 0: 0] External Low Speed oscillator enable   */
      typedef            RegisterBits< R,  1,  1 >   LSERDY; /**< [ 1: 1] External Low Speed oscillator Ready    */
      typedef            RegisterBits< R,  2,  1 >   LSEBYP; /**< [ 2: 2] External Low Speed oscillator Bypass   */
      typedef __RTCSEL < RegisterBits< R,  8,  2 > > RTCSEL; /**< [ 9: 8] RTC clock source selection             */
      typedef            RegisterBits< R, 15,  1 >   RTCEN;  /**< [15:15] RTC clock enable                       */
      typedef            RegisterBits< R, 16,  1 >   BDRST;  /**< [16:16] Backup domain software reset           */
    };


    /**
     * Control/status register
     */
    template<typename R>
    struct __CSR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > LSION;         /**< [ 0: 0] Internal Low Speed oscillator enable   */
      typedef RegisterBits< R,  1,  1 > LSIRDY;        /**< [ 1: 1] Internal Low Speed oscillator Ready    */
      typedef RegisterBits< R, 24,  1 > RMVF;          /**< [24:24] Remove reset flag                      */
      typedef RegisterBits< R, 26,  1 > PINRSTF;       /**< [26:26] PIN reset flag                         */
      typedef RegisterBits< R, 27,  1 > PORRSTF;       /**< [27:27] POR/PDR reset flag                     */
      typedef RegisterBits< R, 28,  1 > SFTRSTF;       /**< [28:28] Software Reset flag                    */
      typedef RegisterBits< R, 29,  1 > IWDGRSTF;      /**< [29:29] Independent Watchdog reset flag        */
      typedef RegisterBits< R, 30,  1 > WWDGRSTF;      /**< [30:30] Window watchdog reset flag             */
      typedef RegisterBits< R, 31,  1 > LPWRRSTF;      /**< [31:31] Low-Power reset flag                   */
    };


    /**
     * AHB peripheral clock reset register
     *
     * NOTE: only available for Connectivity line (CL) devices
     */
    template<typename R>
    struct __AHBRSTR
    : public R
    {
#ifdef STM32F10X_CL
      typedef RegisterBits< R, 12,  1 > OTGFSRST;      /**< [12:12] USB OTG FS reset     */
      typedef RegisterBits< R, 14,  1 > ETHMACRST;     /**< [14:14] ETHERNET MAC reset   */
#endif /* STM32F10X_CL */
    };


    /**
     * Clock configuration register2
     */
    template<typename R>
    class __CFGR2
    : public R
    {

      /** PREDIVx division factor  */
      template<typename Rb>
      struct __PREDIVx
      : public Rb
      {
        template<unsigned divisor>
        struct DIV
        : public RegisterConst< Rb, divisor - 1 >
        { static_assert((divisor >= 1) && (divisor <= 16), "invalid divisor"); };
      };

      /** PLLx Multiplication Factor */
      template<typename Rb>
      struct __PLLxMUL
      : public Rb
      {
        template<unsigned factor>
        struct MUL
        : public RegisterConst< Rb, factor <= 14 ? factor - 2 :
                                    factor == 16 ? 0xE :
                                    factor == 20 ? 0xF : 0 >
        { static_assert(((factor >= 1) && (factor <= 14)) || (factor == 16) || (factor == 20), "invalid factor"); };
      };

      /** PREDIV1 entry clock source  */
      template<typename Rb>
      struct __PREDIV1SRC
      : public Rb
      {
        typedef RegisterConst< Rb, 0x0 > HSE;          /**< 0: HSE oscillator clock selected as PREDIV1 clock entry   */
        typedef RegisterConst< Rb, 0x1 > PLL2;         /**< 1: PLL2 selected as PREDIV1 clock entry                   */
      };

      /** I2Sx clock source  */
      template<typename Rb>
      struct __I2SxSRC
      : public Rb
      {
        typedef RegisterConst< Rb, 0x0 > SYSCLK;       /**< 0: System clock (SYSCLK) selected as I2Sx clock entry   */
        typedef RegisterConst< Rb, 0x1 > PLL3;         /**< 1: PLL3 VCO clock selected as I2Sx clock entry          */
      };

    public:

      typedef __PREDIVx    < RegisterBits< R,  0,  4 > > PREDIV1;    /**< [ 3: 0] PREDIV1 division factor      */
      typedef __PREDIVx    < RegisterBits< R,  4,  4 > > PREDIV2;    /**< [ 7: 4] PREDIV2 division factor      */
      typedef __PLLxMUL    < RegisterBits< R,  8,  4 > > PLL2MUL;    /**< [11: 8] PLL2 Multiplication Factor   */
      typedef __PLLxMUL    < RegisterBits< R, 12,  4 > > PLL3MUL;    /**< [15:12] PLL3 Multiplication Factor   */
      typedef __PREDIV1SRC < RegisterBits< R, 16,  1 > > PREDIV1SRC; /**< [16:16] PREDIV1 entry clock source   */
      typedef __I2SxSRC    < RegisterBits< R, 17,  1 > > I2S2SRC;    /**< [17:17] I2S2 clock source            */
      typedef __I2SxSRC    < RegisterBits< R, 18,  1 > > I2S3SRC;    /**< [18:18] I2S3 clock source            */
    };

  public:

    typedef __CR      < Register< uint32_t, base_addr + 0x00, Access::rw, 0x00000083 /*TODO: 0x0000xx83*/ > > CR;
    typedef __CFGR    < Register< uint32_t, base_addr + 0x04, Access::rw > > CFGR;
    typedef __CIR     < Register< uint32_t, base_addr + 0x08, Access::rw > > CIR;
    typedef __APB2RSTR< Register< uint32_t, base_addr + 0x0c, Access::rw > > APB2RSTR;
    typedef __APB1RSTR< Register< uint32_t, base_addr + 0x10, Access::rw > > APB1RSTR;
    typedef __AHBENR  < Register< uint32_t, base_addr + 0x14, Access::rw > > AHBENR;
    typedef __APB2ENR < Register< uint32_t, base_addr + 0x18, Access::rw > > APB2ENR;
    typedef __APB1ENR < Register< uint32_t, base_addr + 0x1c, Access::rw > > APB1ENR;
    typedef __BDCR    < Register< uint32_t, base_addr + 0x20, Access::rw > > BDCR;
    typedef __CSR     < Register< uint32_t, base_addr + 0x24, Access::rw, 0x0C000000 > > CSR;
#ifdef STM32F10X_CL
    typedef __AHBRSTR < Register< uint32_t, base_addr + 0x28, Access::rw > > AHBRSTR;
    typedef __CFGR2   < Register< uint32_t, base_addr + 0x2c, Access::rw > > CFGR2;
#endif
  };
}

#endif // REG_RCC_HPP_INCLUDED
