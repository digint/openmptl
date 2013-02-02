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

#ifndef REGISTER_DEFS_HPP_INCLUDED
#define REGISTER_DEFS_HPP_INCLUDED

//
//  core register definition: wrap bitfields into Register classes
//

//  Pros:
//
//  - uses Register MPL class to wrap the register access. All type
//    checks are performed on register write, AND on register bitfield
//    access.
//
//  - register bitfields are defined as type "static constexpr uint32_t",
//    which is very common, simple and straight forward.
//
//
//  Cons:
//
//  - MPL stype "::value" variable access
//
//
// *** Don't miss the text from previous commit! ***


// Example:
//
// TODO: declaration example
//
// LINKS: 
// "uint_fast16_t": http://en.cppreference.com/w/cpp/types/integer
//
// static void enable(void) {
//   SPIx::CR1::SPE::set()  /* SPI Enable */
// }
//
// void MyFunction() {
//   /* a load() call results in:                                                 */
//   /*                                                                           */
//   /* constexpr volatile T * value_ptr = reinterpret_cast<volatile T *>(addr);  */
//   /* auto reg = *value_ptr;                                                    */
//   /*                                                                           */
//   /* notes:                                                                    */
//   /* - "addr" (constexpr) holds the address of (non-pointer-type) SPIx::CR1    */
//   /* - type "T" is the register type (e.g. uint32_t or uint8_t)                */
//   /* - reg becomes type "volatile uint32_t                                     */
//   /*                                                                           */
//
//   auto reg = SPIx::CR1::load();
//   reg &= SPIx::CR1::SPE::value | SPIx::CR1::CRCNEXT::value | SPIx::CR1::CRCEN::value;
//   SPIx::CR1::store(reg);

//   /* a store() call results in:                                                */
//   /* *value_ptr = reg;                                                         */
// }
//


#include <register.hpp>

namespace reg {

  static constexpr reg_addr_t PERIPH_BASE = 0x40000000;
  static constexpr reg_addr_t APB1_BASE   = PERIPH_BASE;
  static constexpr reg_addr_t APB2_BASE   = PERIPH_BASE + 0x10000;
  static constexpr reg_addr_t AHB_BASE    = PERIPH_BASE + 0x20000;

  /**
   * Reset and clock control (RCC)
   */
  class RCC
  {
    static constexpr reg_addr_t reg_base = AHB_BASE + 0x1000;

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
        typedef RegisterConst< Rb, 0x0 > PREDIV1;      /**< 00: PREDIV1 clock not divided for PLL entry    */
        typedef RegisterConst< Rb, 0x1 > PREDIV1_DIV2; /**< 01: PREDIV1 clock divided by 2 for PLL entry   */
#else
        typedef RegisterConst< Rb, 0x0 > HSE;          /**< 00: HSE clock not divided    */
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

    typedef __CR      < Register< uint32_t, reg_base + 0x00, Access::rw, 0x00000083 /*TODO: 0x0000xx83*/ > > CR;
    typedef __CFGR    < Register< uint32_t, reg_base + 0x04, Access::rw > > CFGR;
    typedef __CIR     < Register< uint32_t, reg_base + 0x08, Access::rw > > CIR;
    typedef __APB2RSTR< Register< uint32_t, reg_base + 0x0c, Access::rw > > APB2RSTR;
    typedef __APB1RSTR< Register< uint32_t, reg_base + 0x10, Access::rw > > APB1RSTR;
    typedef __AHBENR  < Register< uint32_t, reg_base + 0x14, Access::rw > > AHBENR;
    typedef __APB2ENR < Register< uint32_t, reg_base + 0x18, Access::rw > > APB2ENR;
    typedef __APB1ENR < Register< uint32_t, reg_base + 0x1c, Access::rw > > APB1ENR;
    typedef __BDCR    < Register< uint32_t, reg_base + 0x20, Access::rw > > BDCR;
    typedef __CSR     < Register< uint32_t, reg_base + 0x24, Access::rw, 0x0C000000 > > CSR;
#ifdef STM32F10X_CL
    typedef __AHBRSTR < Register< uint32_t, reg_base + 0x28, Access::rw > > AHBRSTR;
    typedef __CFGR2   < Register< uint32_t, reg_base + 0x2c, Access::rw > > CFGR2;
#endif
  };


  /**
   * Power control registers (PWR)
   */
  class PWR
  {
    static constexpr reg_addr_t reg_base = APB1_BASE + 0x7000;

    /**
     * Power control register
     */
    template<typename R>
    class __CR
    : public R
    {
      /** PVD Level Selection  */
      template<typename Rb>
      struct __PLS
      : public Rb
      {
        /** PVD Threshold of 2.<fraction> volts (e.g. 2.5V if fraction=5) */
        template<unsigned int fraction>
        struct VoltageThreshold
        : public RegisterConst< Rb, fraction - 2 >
        { static_assert((fraction >= 2) && (fraction <= 9), "invalid fraction"); };
      };

    public:

      typedef         RegisterBits< R,  0,  1 >   LPDS; /**< [ 0: 0] Low-Power Deepsleep                      */
      typedef         RegisterBits< R,  1,  1 >   PDDS; /**< [ 1: 1] Power Down Deepsleep                     */
      typedef         RegisterBits< R,  2,  1 >   CWUF; /**< [ 2: 2] Clear Wakeup Flag                        */
      typedef         RegisterBits< R,  3,  1 >   CSBF; /**< [ 3: 3] Clear Standby Flag                       */
      typedef         RegisterBits< R,  4,  1 >   PVDE; /**< [ 4: 4] Power Voltage Detector Enable            */
      typedef __PLS < RegisterBits< R,  5,  3 > > PLS;  /**< [ 7: 5] PVD Level Selection                      */
      typedef         RegisterBits< R,  8,  1 >   DBP;  /**< [ 8: 8] Disable Backup Domain write protection   */
    };


    /**
     * Power control/status register
     */
    template<typename R>
    struct __CSR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > WUF;           /**< [ 0: 0] Wakeup Flag       */
      typedef RegisterBits< R,  1,  1 > SBF;           /**< [ 1: 1] Standby Flag      */
      typedef RegisterBits< R,  2,  1 > PVDO;          /**< [ 2: 2] PVD Output        */
      typedef RegisterBits< R,  8,  1 > EWUP;          /**< [ 8: 8] Enable WKUP pin   */
    };

  public:

    typedef __CR < Register< uint32_t, reg_base + 0x00, Access::rw > > CR;
    typedef __CSR< Register< uint32_t, reg_base + 0x04, Access::rw > > CSR;
  };


  /**
   * Embedded Flash memory (FLASH)
   */
  class FLASH
  {
    static constexpr reg_addr_t reg_base = AHB_BASE + 0x2000;

    /**
     * Flash access control register
     */
    template<typename R>
    struct __ACR
    : public R
    {
      typedef RegisterBits< R,  0,  2 > LATENCY;       /**< [ 1: 0] Latency                          */
      typedef RegisterBits< R,  3,  1 > HLFCYA;        /**< [ 3: 3] Flash Half Cycle Access Enable   */
      typedef RegisterBits< R,  4,  1 > PRFTBE;        /**< [ 4: 4] Prefetch Buffer Enable           */
      typedef RegisterBits< R,  5,  1 > PRFTBS;        /**< [ 5: 5] Prefetch Buffer Status           */
    };

    /**
     * FPEC Key
     */
    template<typename R>
    struct __KEYR
    : public R
    {
    };

    /**
     * Option Byte Key
     */
    template<typename R>
    struct __OPTKEYR
    : public R
    {
    };

    /**
     * document me!
     */
    template<typename R>
    struct __SR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > BSY;           /**< [ 0: 0] Busy                     */
      typedef RegisterBits< R,  2,  1 > PGERR;         /**< [ 2: 2] Programming Error        */
      typedef RegisterBits< R,  4,  1 > WRPRTERR;      /**< [ 4: 4] Write Protection Error   */
      typedef RegisterBits< R,  5,  1 > EOP;           /**< [ 5: 5] End of operation         */
    };


    /**
     * document me!
     */
    template<typename R>
    struct __CR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > PG;            /**< [ 0: 0] Programming                         */
      typedef RegisterBits< R,  1,  1 > PER;           /**< [ 1: 1] Page Erase                          */
      typedef RegisterBits< R,  2,  1 > MER;           /**< [ 2: 2] Mass Erase                          */
      typedef RegisterBits< R,  4,  1 > OPTPG;         /**< [ 4: 4] Option Byte Programming             */
      typedef RegisterBits< R,  5,  1 > OPTER;         /**< [ 5: 5] Option Byte Erase                   */
      typedef RegisterBits< R,  6,  1 > STRT;          /**< [ 6: 6] Start                               */
      typedef RegisterBits< R,  7,  1 > LOCK;          /**< [ 7: 7] Lock                                */
      typedef RegisterBits< R,  9,  1 > OPTWRE;        /**< [ 9: 9] Option Bytes Write Enable           */
      typedef RegisterBits< R, 10,  1 > ERRIE;         /**< [10:10] Error Interrupt Enable              */
      typedef RegisterBits< R, 12,  1 > EOPIE;         /**< [12:12] End of operation interrupt enable   */
    };

    /**
     * Flash Address
     */
    template<typename R>
    struct __AR
    : public R
    {
    };


    /* reserved: 0x4002 2018 - 0x4002 201B */


    /**
     * Option Byte Register
     */
    template<typename R>
    struct __OBR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > OPTERR;        /**< [ 0: 0] Option Byte Error   */
      typedef RegisterBits< R,  1,  1 > RDPRT;         /**< [ 1: 1] Read protection     */
      typedef RegisterBits< R,  2,  8 > USER;          /**< [ 9: 2] User Option Bytes   */
      typedef RegisterBits< R,  2,  1 > WDG_SW;        /**< [ 2: 2] WDG_SW              */
      typedef RegisterBits< R,  3,  1 > nRST_STOP;     /**< [ 3: 3] nRST_STOP           */
      typedef RegisterBits< R,  4,  1 > nRST_STDBY;    /**< [ 4: 4] nRST_STDBY          */
      typedef RegisterBits< R,  5,  1 > BFB2;          /**< [ 5: 5] BFB2                */
    };

    /**
     * Write Protect
     */
    template<typename R>
    struct __WRPR
    : public R
    {
    };


    /* reserved: 0x4002 2024 - 0x4002 2043 */


    /**
     * document me!
     *
     * NOTE: only available for XL-density Flash modules
     */
    template<typename R>
    struct __KEYR2
    : public R
    {
    };


    /* reserved: 0x4002 2048 - 0x4002 204B */


    /**
     * document me!
     *
     * NOTE: only available for XL-density Flash modules
     */
    template<typename R>
    struct __SR2
    : public R
    {
    };

    /**
     * document me!
     *
     * NOTE: only available for XL-density Flash modules
     */
    template<typename R>
    struct __CR2
    : public R
    {
    };

    /**
     * document me!
     *
     * NOTE: only available for XL-density Flash modules
     */
    template<typename R>
    struct __AR2
    : public R
    {
    };


  public:

    typedef __ACR    < Register< uint32_t, reg_base + 0x00, Access::rw, 0x00000030 > > ACR;
    typedef __KEYR   < Register< uint32_t, reg_base + 0x04, Access::rw > > KEYR;
    typedef __OPTKEYR< Register< uint32_t, reg_base + 0x08, Access::rw > > OPTKEYR;
    typedef __SR     < Register< uint32_t, reg_base + 0x0c, Access::rw > > SR;
    typedef __CR     < Register< uint32_t, reg_base + 0x10, Access::rw > > CR;
    typedef __AR     < Register< uint32_t, reg_base + 0x14, Access::rw > > AR;
    typedef __OBR    < Register< uint32_t, reg_base + 0x1c, Access::rw > > OBR;
    typedef __WRPR   < Register< uint32_t, reg_base + 0x20, Access::rw > > WRPR;
    typedef __KEYR2  < Register< uint32_t, reg_base + 0x44, Access::rw > > KEYR2;
    typedef __SR2    < Register< uint32_t, reg_base + 0x4c, Access::rw > > SR2;
    typedef __CR2    < Register< uint32_t, reg_base + 0x50, Access::rw > > CR2;
    typedef __AR2    < Register< uint32_t, reg_base + 0x54, Access::rw > > AR2;
  };


  /**
   * Real-time clock (RTC)
   *
   * Note that the registers are actually only 16bit wide, but accessing
   * them with 32bit is faster in general.
   */
  class RTC
  {
    static constexpr reg_addr_t reg_base = APB1_BASE + 0x2800;

    /**
     * RTC control register high
     */
    template<typename R>
    struct __CRH
    : public R
    {
      typedef RegisterBits< R,  0,  1 > SECIE;         /**< [ 0: 0] Second Interrupt Enable     */
      typedef RegisterBits< R,  1,  1 > ALRIE;         /**< [ 1: 1] Alarm Interrupt Enable      */
      typedef RegisterBits< R,  2,  1 > OWIE;          /**< [ 2: 2] OverfloW Interrupt Enable   */
    };


    /**
     * RTC control register low
     */
    template<typename R>
    struct __CRL
    : public R
    {
      typedef RegisterBits< R,  0,  1 > SECF;          /**< [ 0: 0] Second Flag                   */
      typedef RegisterBits< R,  1,  1 > ALRF;          /**< [ 1: 1] Alarm Flag                    */
      typedef RegisterBits< R,  2,  1 > OWF;           /**< [ 2: 2] OverfloW Flag                 */
      typedef RegisterBits< R,  3,  1 > RSF;           /**< [ 3: 3] Registers Synchronized Flag   */
      typedef RegisterBits< R,  4,  1 > CNF;           /**< [ 4: 4] Configuration Flag            */
      typedef RegisterBits< R,  5,  1 > RTOFF;         /**< [ 5: 5] RTC operation OFF             */
    };

    /**
     * RTC prescaler load register high
     */
    template<typename R>
    struct __PRLH
    : public R
    {
      typedef RegisterBits< R,  0,  4 > PRL;           /**< [ 3: 0] RTC Prescaler Reload Value High   */
    };

    /**
     * RTC prescaler load register low
     */
    template<typename R>
    struct __PRLL
    : public R
    {
      typedef RegisterBits< R,  0, 16 > PRL;           /**< [15: 0] RTC Prescaler Reload Value Low   */
    };

    /**
     * RTC prescaler divider register high
     */
    template<typename R>
    struct __DIVH
    : public R
    {
      typedef RegisterBits< R,  0,  4 > RTC_DIV;       /**< [ 3: 0] RTC Clock Divider High   */
    };

    /**
     * RTC prescaler divider register low
     */
    template<typename R>
    struct __DIVL
    : public R
    {
      typedef RegisterBits< R,  0, 16 > RTC_DIV;       /**< [15: 0] RTC Clock Divider Low   */
    };

    /**
     * RTC counter register high
     */
    template<typename R>
    struct __CNTH
    : public R
    {
      typedef RegisterBits< R,  0, 16 > RTC_CNT;       /**< [15: 0] RTC Counter High   */
    };

    /**
     * RTC counter register low
     */
    template<typename R>
    struct __CNTL
    : public R
    {
      typedef RegisterBits< R,  0, 16 > RTC_CNT;       /**< [15: 0] RTC Counter Low   */
    };

    /**
     * RTC alarm register high
     */
    template<typename R>
    struct __ALRH
    : public R
    {
      typedef RegisterBits< R,  0, 16 > RTC_ALR;       /**< [15: 0] RTC Alarm High   */
    };

    /**
     * RTC alarm register low
     */
    template<typename R>
    struct __ALRL
    : public R
    {
      typedef RegisterBits< R,  0, 16 > RTC_ALR;       /**< [15: 0] RTC Alarm Low   */
    };


  public:

    // TODO: use "uint_fast16_t", this is way cooler!
    typedef __CRH < Register< uint32_t, reg_base + 0x00, Access::rw         > > CRH;  /**< RTC control register high             */
    typedef __CRL < Register< uint32_t, reg_base + 0x04, Access::rw, 0x0020 > > CRL;  /**< RTC control register low              */
    typedef __PRLH< Register< uint32_t, reg_base + 0x08, Access::wo         > > PRLH; /**< RTC prescaler load register high      */
    typedef __PRLL< Register< uint32_t, reg_base + 0x0c, Access::wo, 0x8000 > > PRLL; /**< RTC prescaler load register low       */
    typedef __DIVH< Register< uint32_t, reg_base + 0x10, Access::ro         > > DIVH; /**< RTC prescaler divider register high   */
    typedef __DIVL< Register< uint32_t, reg_base + 0x14, Access::ro, 0x8000 > > DIVL; /**< RTC prescaler divider register low    */
    typedef __CNTH< Register< uint32_t, reg_base + 0x18, Access::rw         > > CNTH; /**< RTC counter register high             */
    typedef __CNTL< Register< uint32_t, reg_base + 0x1c, Access::rw         > > CNTL; /**< RTC counter register low              */
    typedef __ALRH< Register< uint32_t, reg_base + 0x20, Access::wo         > > ALRH; /**< RTC alarm register high               */
    typedef __ALRL< Register< uint32_t, reg_base + 0x24, Access::wo         > > ALRL; /**< RTC alarm register low                */
  };


  /**
   * General-purpose and alternate-function I/Os (GPIOs and AFIOs)
   */
  template<char reg_name>
  struct GPIO
  {
    static_assert((reg_name >= 'A') && (reg_name <= 'G'), "invalid index for register");

    static constexpr std::size_t gpio_no   = reg_name - 'A';
    static constexpr reg_addr_t  reg_base = APB2_BASE + 0x0800 + gpio_no * 0x0400;
    
    typedef Register< uint32_t, reg_base + 0x00, Access::rw, 0x44444444 > CRL;            /**< Port configuration register low    */
    typedef Register< uint32_t, reg_base + 0x04, Access::rw, 0x44444444 > CRH;            /**< Port configuration register high   */
    typedef Register< uint32_t, reg_base + 0x08, Access::ro /* TODO: 0x0000XXXX */ > IDR; /**< Port input data register           */
    typedef Register< uint32_t, reg_base + 0x0c, Access::rw             > ODR;            /**< Port output data register          */
    typedef Register< uint32_t, reg_base + 0x10, Access::wo             > BSRR;           /**< Port bit set/reset register        */
    typedef Register< uint32_t, reg_base + 0x14, Access::wo             > BRR;            /**< Port bit reset register            */
    typedef Register< uint32_t, reg_base + 0x18, Access::rw             > LCKR;           /**< Port configuration lock register   */
  };


  /**
   * Analog-to-digital converter (ADC)
   */
  template<std::size_t adc_no>
  class ADC
  {
    static_assert((adc_no >= 1) && (adc_no <= 3), "invalid index for register");

    static constexpr reg_addr_t reg_base = adc_no == 1 ? APB2_BASE + 0x2400 :
                                           adc_no == 2 ? APB2_BASE + 0x2800 :
                                           adc_no == 3 ? APB2_BASE + 0x3C00 :
                                           0;

    /**
     * ADC status register
     */
    template<typename R>
    struct __SR
    : public R
    {
      // TODO: special access rc_w0
      typedef RegisterBits< R,  0,  1 > AWD;           /**< [ 0: 0] Analog watchdog flag                 */
      typedef RegisterBits< R,  1,  1 > EOC;           /**< [ 1: 1] End of conversion                    */
      typedef RegisterBits< R,  2,  1 > JEOC;          /**< [ 2: 2] Injected channel end of conversion   */
      typedef RegisterBits< R,  3,  1 > JSTRT;         /**< [ 3: 3] Injected channel Start flag          */
      typedef RegisterBits< R,  4,  1 > STRT;          /**< [ 4: 4] Regular channel Start flag           */
    };


    /**
     * ADC control register 1
     */
    template<typename R>
    struct __CR1
    : public R
    {
      typedef RegisterBits< R,  0,  5 > AWDCH;         /**< [ 4: 0] Analog watchdog channel select bits                    */
      typedef RegisterBits< R,  5,  1 > EOCIE;         /**< [ 5: 5] Interrupt enable for EOC                               */
      typedef RegisterBits< R,  6,  1 > AWDIE;         /**< [ 6: 6] AAnalog Watchdog interrupt enable                      */
      typedef RegisterBits< R,  7,  1 > JEOCIE;        /**< [ 7: 7] Interrupt enable for injected channels                 */
      typedef RegisterBits< R,  8,  1 > SCAN;          /**< [ 8: 8] Scan mode                                              */
      typedef RegisterBits< R,  9,  1 > AWDSGL;        /**< [ 9: 9] Enable the watchdog on a single channel in scan mode   */
      typedef RegisterBits< R, 10,  1 > JAUTO;         /**< [10:10] Automatic injected group conversion                    */
      typedef RegisterBits< R, 11,  1 > DISCEN;        /**< [11:11] Discontinuous mode on regular channels                 */
      typedef RegisterBits< R, 12,  1 > JDISCEN;       /**< [12:12] Discontinuous mode on injected channels                */
      typedef RegisterBits< R, 13,  3 > DISCNUM;       /**< [15:13] Discontinuous mode channel count                       */
      typedef RegisterBits< R, 16,  4 > DUALMOD;       /**< [19:16] Dual mode selection                                    */
      typedef RegisterBits< R, 22,  1 > JAWDEN;        /**< [22:22] Analog watchdog enable on injected channels            */
      typedef RegisterBits< R, 23,  1 > AWDEN;         /**< [23:23] Analog watchdog enable on regular channels             */
    };


    /**
     * ADC control register 2
     */
    template<typename R>
    struct __CR2
    : public R
    {
      typedef RegisterBits< R,  0,  1 > ADON;          /**< [ 0: 0] A/D Converter ON / OFF                                   */
      typedef RegisterBits< R,  1,  1 > CONT;          /**< [ 1: 1] Continuous Conversion                                    */
      typedef RegisterBits< R,  2,  1 > CAL;           /**< [ 2: 2] A/D Calibration                                          */
      typedef RegisterBits< R,  3,  1 > RSTCAL;        /**< [ 3: 3] Reset Calibration                                        */
      typedef RegisterBits< R,  8,  1 > DMA;           /**< [ 8: 8] Direct Memory access mode                                */
      typedef RegisterBits< R, 11,  1 > ALIGN;         /**< [11:11] Data Alignment                                           */
      typedef RegisterBits< R, 12,  3 > JEXTSEL;       /**< [14:12] External event select for injected group                 */
      typedef RegisterBits< R, 15,  1 > JEXTTRIG;      /**< [15:15] External Trigger Conversion mode for injected channels   */
      typedef RegisterBits< R, 17,  3 > EXTSEL;        /**< [19:17] External Event Select for regular group                  */
      typedef RegisterBits< R, 20,  1 > EXTTRIG;       /**< [20:20] External Trigger Conversion mode for regular channels    */
      typedef RegisterBits< R, 21,  1 > JSWSTART;      /**< [21:21] Start Conversion of injected channels                    */
      typedef RegisterBits< R, 22,  1 > SWSTART;       /**< [22:22] Start Conversion of regular channels                     */
      typedef RegisterBits< R, 23,  1 > TSVREFE;       /**< [23:23] Temperature Sensor and VREFINT Enable                    */
    };


    /**
     * ADC sample time register 1
     */
    template<typename R>
    struct __SMPR1
    : public R
    {
      typedef RegisterBits< R,  0,  3 > SMP10;         /**< [ 2: 0] Channel 10 Sample time selection   */
      typedef RegisterBits< R,  3,  3 > SMP11;         /**< [ 5: 3] Channel 11 Sample time selection   */
      typedef RegisterBits< R,  6,  3 > SMP12;         /**< [ 8: 6] Channel 12 Sample time selection   */
      typedef RegisterBits< R,  9,  3 > SMP13;         /**< [11: 9] Channel 13 Sample time selection   */
      typedef RegisterBits< R, 12,  3 > SMP14;         /**< [14:12] Channel 14 Sample time selection   */
      typedef RegisterBits< R, 15,  3 > SMP15;         /**< [17:15] Channel 15 Sample time selection   */
      typedef RegisterBits< R, 18,  3 > SMP16;         /**< [20:18] Channel 16 Sample time selection   */
      typedef RegisterBits< R, 21,  3 > SMP17;         /**< [23:21] Channel 17 Sample time selection   */
    };


    /**
     * ADC sample time register 2
     */
    template<typename R>
    struct __SMPR2
    : public R
    {
      typedef RegisterBits< R,  0,  3 > SMP0;          /**< [ 2: 0] Channel 0 Sample time selection   */
      typedef RegisterBits< R,  3,  3 > SMP1;          /**< [ 5: 3] Channel 1 Sample time selection   */
      typedef RegisterBits< R,  6,  3 > SMP2;          /**< [ 8: 6] Channel 2 Sample time selection   */
      typedef RegisterBits< R,  9,  3 > SMP3;          /**< [11: 9] Channel 3 Sample time selection   */
      typedef RegisterBits< R, 12,  3 > SMP4;          /**< [14:12] Channel 4 Sample time selection   */
      typedef RegisterBits< R, 15,  3 > SMP5;          /**< [17:15] Channel 5 Sample time selection   */
      typedef RegisterBits< R, 18,  3 > SMP6;          /**< [20:18] Channel 6 Sample time selection   */
      typedef RegisterBits< R, 21,  3 > SMP7;          /**< [23:21] Channel 7 Sample time selection   */
      typedef RegisterBits< R, 24,  3 > SMP8;          /**< [26:24] Channel 8 Sample time selection   */
      typedef RegisterBits< R, 27,  3 > SMP9;          /**< [29:27] Channel 9 Sample time selection   */
    };


    /**
     * ADC injected channel data offset register
     */
    template<typename R>
    struct __JOFR
    : public R
    {
      typedef RegisterBits< R,  0, 12 > JOFFSET;       /**< [11: 0] Data offset for injected channel 1   */
    };


    /**
     * ADC watchdog high threshold register
     */
    template<typename R>
    struct __HTR
    : public R
    {
      typedef RegisterBits< R,  0, 12 > HT;            /**< [11: 0] Analog watchdog high threshold   */
    };


    /**
     * ADC watchdog low threshold register
     */
    template<typename R>
    struct __LTR
    : public R
    {
      typedef RegisterBits< R,  0, 12 > LT;            /**< [11: 0] Analog watchdog low threshold   */
    };


    /**
     * ADC regular sequence register 1
     */
    template<typename R>
    struct __SQR1
    : public R
    {
      typedef RegisterBits< R,  0,  5 > SQ13;          /**< [ 4: 0] 13th conversion in regular sequence   */
      typedef RegisterBits< R,  5,  5 > SQ14;          /**< [ 9: 5] 14th conversion in regular sequence   */
      typedef RegisterBits< R, 10,  5 > SQ15;          /**< [14:10] 15th conversion in regular sequence   */
      typedef RegisterBits< R, 15,  5 > SQ16;          /**< [19:15] 16th conversion in regular sequence   */
      typedef RegisterBits< R, 20,  4 > L;             /**< [23:20] Regular channel sequence length       */
    };


    /**
     * ADC regular sequence register 2
     */
    template<typename R>
    struct __SQR2
    : public R
    {
      typedef RegisterBits< R,  0,  5 > SQ7;           /**< [ 4: 0] 7th conversion in regular sequence    */
      typedef RegisterBits< R,  5,  5 > SQ8;           /**< [ 9: 5] 8th conversion in regular sequence    */
      typedef RegisterBits< R, 10,  5 > SQ9;           /**< [14:10] 9th conversion in regular sequence    */
      typedef RegisterBits< R, 15,  5 > SQ10;          /**< [19:15] 10th conversion in regular sequence   */
      typedef RegisterBits< R, 20,  5 > SQ11;          /**< [24:20] 11th conversion in regular sequence   */
      typedef RegisterBits< R, 25,  5 > SQ12;          /**< [29:25] 12th conversion in regular sequence   */
    };


    /**
     * ADC regular sequence register 3
     */
    template<typename R>
    struct __SQR3
    : public R
    {
      typedef RegisterBits< R,  0,  5 > SQ1;           /**< [ 4: 0] 1st conversion in regular sequence   */
      typedef RegisterBits< R,  5,  5 > SQ2;           /**< [ 9: 5] 2nd conversion in regular sequence   */
      typedef RegisterBits< R, 10,  5 > SQ3;           /**< [14:10] 3rd conversion in regular sequence   */
      typedef RegisterBits< R, 15,  5 > SQ4;           /**< [19:15] 4th conversion in regular sequence   */
      typedef RegisterBits< R, 20,  5 > SQ5;           /**< [24:20] 5th conversion in regular sequence   */
      typedef RegisterBits< R, 25,  5 > SQ6;           /**< [29:25] 6th conversion in regular sequence   */
    };


    /**
     * ADC injected sequence register
     */
    template<typename R>
    struct __JSQR
    : public R
    {
      typedef RegisterBits< R,  0,  5 > JSQ1;          /**< [ 4: 0] 1st conversion in injected sequence   */
      typedef RegisterBits< R,  5,  5 > JSQ2;          /**< [ 9: 5] 2nd conversion in injected sequence   */
      typedef RegisterBits< R, 10,  5 > JSQ3;          /**< [14:10] 3rd conversion in injected sequence   */
      typedef RegisterBits< R, 15,  5 > JSQ4;          /**< [19:15] 4th conversion in injected sequence   */
      typedef RegisterBits< R, 20,  2 > JL;            /**< [21:20] Injected Sequence length)             */
    };


    /**
     * ADC injected data register 1
     */
    template<typename R>
    struct __JDR
    : public R
    {
      typedef RegisterBits< R,  0, 16 > JDATA;         /**< [15: 0] Injected data   */
    };

    /**
     * ADC regular data register
     */
    template<typename R>
    struct __DR
    : public R
    {
      typedef RegisterBits< R,  0, 16 > DATA;          /**< [15: 0] Regular data   */
      typedef RegisterBits< R, 16, 16 > ADC2DATA;      /**< [31:16] ADC2 data      */
    };


  public:

    typedef __SR   < Register< uint32_t, reg_base + 0x00, Access::rw > > SR;
    typedef __CR1  < Register< uint32_t, reg_base + 0x04, Access::rw > > CR1;
    typedef __CR2  < Register< uint32_t, reg_base + 0x08, Access::rw > > CR2;
    typedef __SMPR1< Register< uint32_t, reg_base + 0x0c, Access::rw > > SMPR1;
    typedef __SMPR2< Register< uint32_t, reg_base + 0x10, Access::rw > > SMPR2;

    template<std::size_t jofr_no>
    struct JOFR
    : __JOFR<Register< uint32_t, reg_base + 0x14 + (jofr_no - 1) * 4, Access::rw >>
    { static_assert((jofr_no >= 1) && (jofr_no <= 4), "invalid index for register"); };

    typedef __HTR  < Register< uint32_t, reg_base + 0x24, Access::rw, 0x00000FFF > > HTR;
    typedef __LTR  < Register< uint32_t, reg_base + 0x28, Access::rw             > > LTR;
    typedef __SQR1 < Register< uint32_t, reg_base + 0x2c, Access::rw             > > SQR1;
    typedef __SQR2 < Register< uint32_t, reg_base + 0x30, Access::rw             > > SQR2;
    typedef __SQR3 < Register< uint32_t, reg_base + 0x34, Access::rw             > > SQR3;
    typedef __JSQR < Register< uint32_t, reg_base + 0x38, Access::rw             > > JSQR;

    template<std::size_t jdr_no>
    struct JDR
    : __JDR<Register< uint32_t, reg_base + 0x3c + (jdr_no - 1) * 4, Access::ro >>
    { static_assert((jdr_no >= 1) && (jdr_no <= 4), "invalid index for register"); };

    typedef __DR   < Register< uint32_t, reg_base + 0x4c, Access::ro > > DR;
  };                                                    


  /**
   * Serial peripheral interface (SPI)
   *
   * Note that the registers are actually only 16bit wide, but accessing
   * them with 32bit is faster in general.
   */
  template<std::size_t spi_no>
  class SPI
  {
    static_assert((spi_no >= 1) && (spi_no <= 3), "invalid SPI number"); // TODO: depends on cpu sub-arch

    static constexpr reg_addr_t reg_base = spi_no == 1 ? APB2_BASE + 0x3000 :
                                           spi_no == 2 ? APB1_BASE + 0x3800 :
                                           spi_no == 3 ? APB1_BASE + 0x3C00 :
                                           0;

    /** SPI control register 1 */
    template<typename R>
    class __CR1
    : public R
    {

      /** Baud Rate Control  */
      template<typename Rb>
      struct __BR
      : public Rb
      {
        template<unsigned prescaler>
        struct Prescaler
        : RegisterConst< Rb,
                         prescaler == 2   ? 0x0 :      /**< f_PCLK / 2     */
                         prescaler == 4   ? 0x1 :      /**< f_PCLK / 4     */
                         prescaler == 8   ? 0x2 :      /**< f_PCLK / 8     */
                         prescaler == 16  ? 0x3 :      /**< f_PCLK / 16    */
                         prescaler == 32  ? 0x4 :      /**< f_PCLK / 32    */
                         prescaler == 64  ? 0x5 :      /**< f_PCLK / 64    */
                         prescaler == 128 ? 0x6 :      /**< f_PCLK / 128   */
                         prescaler == 256 ? 0x7 :      /**< f_PCLK / 256   */
                         0
                         >
        {
          static_assert(prescaler == 2  || prescaler == 4  || prescaler == 8   || prescaler == 16 ||
                        prescaler == 32 || prescaler == 64 || prescaler == 128 || prescaler == 256
                        , "invalid baud rate prescaler");
        };
      };

    public:

      typedef        RegisterBits< R,  0,  1 >   CPHA;     /**< [ 0: 0] Clock Phase                           */
      typedef        RegisterBits< R,  1,  1 >   CPOL;     /**< [ 1: 1] Clock Polarity                        */
      typedef        RegisterBits< R,  2,  1 >   MSTR;     /**< [ 2: 2] Master Selection                      */
      typedef __BR < RegisterBits< R,  3,  3 > > BR;       /**< [ 5: 3] Baud Rate Control                     */
      typedef        RegisterBits< R,  6,  1 >   SPE;      /**< [ 6: 6] SPI Enable                            */
      typedef        RegisterBits< R,  7,  1 >   LSBFIRST; /**< [ 7: 7] Frame Format                          */
      typedef        RegisterBits< R,  8,  1 >   SSI;      /**< [ 8: 8] Internal slave select                 */
      typedef        RegisterBits< R,  9,  1 >   SSM;      /**< [ 9: 9] Software slave management             */
      typedef        RegisterBits< R, 10,  1 >   RXONLY;   /**< [10:10] Receive only                          */
      typedef        RegisterBits< R, 11,  1 >   DFF;      /**< [11:11] Data Frame Format                     */
      typedef        RegisterBits< R, 12,  1 >   CRCNEXT;  /**< [12:12] Transmit CRC next                     */
      typedef        RegisterBits< R, 13,  1 >   CRCEN;    /**< [13:13] Hardware CRC calculation enable       */
      typedef        RegisterBits< R, 14,  1 >   BIDIOE;   /**< [14:14] Output enable in bidirectional mode   */
      typedef        RegisterBits< R, 15,  1 >   BIDIMODE; /**< [15:15] Bidirectional data mode enable        */
    };

    /** SPI control register 2 */
    template<typename R>
    struct __CR2
    : public R
    {
      typedef RegisterBits< R,  0,  1 > RXDMAEN;       /**< [ 0: 0] Rx Buffer DMA Enable                   */
      typedef RegisterBits< R,  1,  1 > TXDMAEN;       /**< [ 1: 1] Tx Buffer DMA Enable                   */
      typedef RegisterBits< R,  2,  1 > SSOE;          /**< [ 2: 2] SS Output Enable                       */
      typedef RegisterBits< R,  5,  1 > ERRIE;         /**< [ 5: 5] Error Interrupt Enable                 */
      typedef RegisterBits< R,  6,  1 > RXNEIE;        /**< [ 6: 6] RX buffer Not Empty Interrupt Enable   */
      typedef RegisterBits< R,  7,  1 > TXEIE;         /**< [ 7: 7] Tx buffer Empty Interrupt Enable       */
    };

    /** SPI status register */
    template<typename R>
    struct __SR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > RXNE;          /**< [ 0: 0] Receive buffer Not Empty   */
      typedef RegisterBits< R,  1,  1 > TXE;           /**< [ 1: 1] Transmit buffer Empty      */
      typedef RegisterBits< R,  2,  1 > CHSIDE;        /**< [ 2: 2] Channel side               */
      typedef RegisterBits< R,  3,  1 > UDR;           /**< [ 3: 3] Underrun flag              */
      typedef RegisterBits< R,  4,  1 > CRCERR;        /**< [ 4: 4] CRC Error flag             */
      typedef RegisterBits< R,  5,  1 > MODF;          /**< [ 5: 5] Mode fault                 */
      typedef RegisterBits< R,  6,  1 > OVR;           /**< [ 6: 6] Overrun flag               */
      typedef RegisterBits< R,  7,  1 > BSY;           /**< [ 7: 7] Busy flag                  */
    };
    
    /** SPI data register */
    template<typename R>
    struct __DR
    : public R
    {
    };

    /** SPI CRC polynomial register */
    template<typename R>
    struct __CRCPR
    : public R
    {
    };

    /** SPI Rx CRC register */
    template<typename R>
    struct __RXCRCR
    : public R
    {
    };

    /** SPI Tx CRC register */
    template<typename R>
    struct __TXCRCR
    : public R
    {
    };

    /** SPI_I2S configuration register */
    template<typename R>
    struct __I2SCFGR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > CHLEN;         /**< [ 0: 0] Channel length (number of bits per audio channel)   */
      typedef RegisterBits< R,  1,  2 > DATLEN;        /**< [ 2: 1] Data length to be transferred                       */
      typedef RegisterBits< R,  3,  1 > CKPOL;         /**< [ 3: 3] steady state clock polarity                         */
      typedef RegisterBits< R,  4,  2 > I2SSTD;        /**< [ 5: 4] I2S standard selection                              */
      typedef RegisterBits< R,  7,  1 > PCMSYNC;       /**< [ 7: 7] PCM frame synchronization                           */
      typedef RegisterBits< R,  8,  2 > I2SCFG;        /**< [ 9: 8] I2S configuration mode                              */
      typedef RegisterBits< R, 10,  1 > I2SE;          /**< [10:10] I2S Enable                                          */
      typedef RegisterBits< R, 11,  1 > I2SMOD;        /**< [11:11] I2S mode selection                                  */
    };

    /** SPI_I2S prescaler register */
    template<typename R>
    struct __I2SPR
    : public R
    {
      typedef RegisterBits< R,  0,  8 > I2SDIV;        /**< [ 7: 0] I2S Linear prescaler           */
      typedef RegisterBits< R,  8,  1 > ODD;           /**< [ 8: 8] Odd factor for the prescaler   */
      typedef RegisterBits< R,  9,  1 > MCKOE;         /**< [ 9: 9] Master Clock Output Enable     */
    };


  public:

    typedef __CR1    < Register< uint32_t, reg_base + 0x00, Access::rw         > > CR1;
    typedef __CR2    < Register< uint32_t, reg_base + 0x04, Access::rw         > > CR2;
    typedef __SR     < Register< uint32_t, reg_base + 0x08, Access::rw, 0x0002 > > SR;
    typedef __DR     < Register< uint32_t, reg_base + 0x0c, Access::rw         > > DR;
    typedef __CRCPR  < Register< uint32_t, reg_base + 0x10, Access::rw, 0x0007 > > CRCPR;
    typedef __RXCRCR < Register< uint32_t, reg_base + 0x14, Access::ro         > > RXCRCR;
    typedef __TXCRCR < Register< uint32_t, reg_base + 0x18, Access::ro         > > TXCRCR;
    typedef __I2SCFGR< Register< uint32_t, reg_base + 0x1c, Access::rw         > > I2SCFGR;
    typedef __I2SPR  < Register< uint32_t, reg_base + 0x20, Access::rw, 0x0010 > > I2SPR;
  };


  /** 
   * Universal synchronous asynchronous receiver transmitter (USART)
   *
   * Note that the registers are actually only 16bit wide, but accessing
   * them with 32bit is faster in general.
   */
  template<std::size_t usart_no>
  class USART
  {
    static_assert((usart_no >= 1) && (usart_no <= 3), "invalid index for register");

    static constexpr reg_addr_t reg_base = usart_no == 1 ? APB2_BASE + 0x3800 :
                                           usart_no == 2 ? APB1_BASE + 0x4400 :
                                           usart_no == 3 ? APB1_BASE + 0x4800 :
                                           0;

    /**
     * Status register                   
     */
    template<typename R>
    struct __SR
    : public R
    {
      typedef RegisterBits< R,  0,  1 > PE;            /**< [ 0: 0] Parity Error                   */
      typedef RegisterBits< R,  1,  1 > FE;            /**< [ 1: 1] Framing Error                  */
      typedef RegisterBits< R,  2,  1 > NE;            /**< [ 2: 2] Noise Error Flag               */
      typedef RegisterBits< R,  3,  1 > ORE;           /**< [ 3: 3] OverRun Error                  */
      typedef RegisterBits< R,  4,  1 > IDLE;          /**< [ 4: 4] IDLE line detected             */
      typedef RegisterBits< R,  5,  1 > RXNE;          /**< [ 5: 5] Read Data Register Not Empty   */
      typedef RegisterBits< R,  6,  1 > TC;            /**< [ 6: 6] Transmission Complete          */
      typedef RegisterBits< R,  7,  1 > TXE;           /**< [ 7: 7] Transmit Data Register Empty   */
      typedef RegisterBits< R,  8,  1 > LBD;           /**< [ 8: 8] LIN Break Detection Flag       */
      typedef RegisterBits< R,  9,  1 > CTS;           /**< [ 9: 9] CTS Flag                       */
    };


    /**
     * Data register                     
     */
    template<typename R>
    struct __DR
    : public R
    {
      typedef RegisterBits< R,  0,  9 > DR_;           /**< [ 8: 0] Data value   */

      /* Bits [31:9] Reserved, forced by hardware to 0. */
    };


    /**
     * Baud rate register                
     */
    template<typename R>
    struct __BRR
    : public R
    {
      typedef RegisterBits< R,  0,  4 > DIV_Fraction;  /**< [ 3: 0] Fraction of USARTDIV   */
      typedef RegisterBits< R,  4, 12 > DIV_Mantissa;  /**< [15: 4] Mantissa of USARTDIV   */
    };


    /**
     * Control register 1                
     */
    template<typename R>
    struct __CR1
    : public R
    {
      typedef RegisterBits< R,  0,  1 > SBK;           /**< [ 0: 0] Send Break                               */
      typedef RegisterBits< R,  1,  1 > RWU;           /**< [ 1: 1] Receiver wakeup                          */
      typedef RegisterBits< R,  2,  1 > RE;            /**< [ 2: 2] Receiver Enable                          */
      typedef RegisterBits< R,  3,  1 > TE;            /**< [ 3: 3] Transmitter Enable                       */
      typedef RegisterBits< R,  4,  1 > IDLEIE;        /**< [ 4: 4] IDLE Interrupt Enable                    */
      typedef RegisterBits< R,  5,  1 > RXNEIE;        /**< [ 5: 5] RXNE Interrupt Enable                    */
      typedef RegisterBits< R,  6,  1 > TCIE;          /**< [ 6: 6] Transmission Complete Interrupt Enable   */
      typedef RegisterBits< R,  7,  1 > TXEIE;         /**< [ 7: 7] PE Interrupt Enable                      */
      typedef RegisterBits< R,  8,  1 > PEIE;          /**< [ 8: 8] PE Interrupt Enable                      */
      typedef RegisterBits< R,  9,  1 > PS;            /**< [ 9: 9] Parity Selection                         */
      typedef RegisterBits< R, 10,  1 > PCE;           /**< [10:10] Parity Control Enable                    */
      typedef RegisterBits< R, 11,  1 > WAKE;          /**< [11:11] Wakeup method                            */
      typedef RegisterBits< R, 12,  1 > M;             /**< [12:12] Word length                              */
      typedef RegisterBits< R, 13,  1 > UE;            /**< [13:13] USART Enable                             */
      typedef RegisterBits< R, 15,  1 > OVER8;         /**< [15:15] USART Oversmapling 8-bits                */
    };


    /**
     * Control register 2                
     */
    template<typename R>
    struct __CR2
    : public R
    {
      typedef RegisterBits< R,  0,  4 > ADD;           /**< [ 3: 0] Address of the USART node              */
      typedef RegisterBits< R,  5,  1 > LBDL;          /**< [ 5: 5] LIN Break Detection Length             */
      typedef RegisterBits< R,  6,  1 > LBDIE;         /**< [ 6: 6] LIN Break Detection Interrupt Enable   */
      typedef RegisterBits< R,  8,  1 > LBCL;          /**< [ 8: 8] Last Bit Clock pulse                   */
      typedef RegisterBits< R,  9,  1 > CPHA;          /**< [ 9: 9] Clock Phase                            */
      typedef RegisterBits< R, 10,  1 > CPOL;          /**< [10:10] Clock Polarity                         */
      typedef RegisterBits< R, 11,  1 > CLKEN;         /**< [11:11] Clock Enable                           */
      typedef RegisterBits< R, 12,  2 > STOP;          /**< [13:12] STOP bits                              */
      typedef RegisterBits< R, 14,  1 > LINEN;         /**< [14:14] LIN mode enable                        */
    };


    /**
     * Control register 3                
     */
    template<typename R>
    struct __CR3
    : public R
    {
      typedef RegisterBits< R,  0,  1 > EIE;           /**< [ 0: 0] Error Interrupt Enable   */
      typedef RegisterBits< R,  1,  1 > IREN;          /**< [ 1: 1] IrDA mode Enable         */
      typedef RegisterBits< R,  2,  1 > IRLP;          /**< [ 2: 2] IrDA Low-Power           */
      typedef RegisterBits< R,  3,  1 > HDSEL;         /**< [ 3: 3] Half-Duplex Selection    */
      typedef RegisterBits< R,  4,  1 > NACK;          /**< [ 4: 4] Smartcard NACK enable    */
      typedef RegisterBits< R,  5,  1 > SCEN;          /**< [ 5: 5] Smartcard mode enable    */
      typedef RegisterBits< R,  6,  1 > DMAR;          /**< [ 6: 6] DMA Enable Receiver      */
      typedef RegisterBits< R,  7,  1 > DMAT;          /**< [ 7: 7] DMA Enable Transmitter   */
      typedef RegisterBits< R,  8,  1 > RTSE;          /**< [ 8: 8] RTS Enable               */
      typedef RegisterBits< R,  9,  1 > CTSE;          /**< [ 9: 9] CTS Enable               */
      typedef RegisterBits< R, 10,  1 > CTSIE;         /**< [10:10] CTS Interrupt Enable     */
      typedef RegisterBits< R, 11,  1 > ONEBIT;        /**< [11:11] One Bit method           */
    };


    /**
     * Guard time and prescaler register 
     */
    template<typename R>
    struct __GTPR
    : public R
    {
      typedef RegisterBits< R,  0,  8 > PSC;           /**< [ 7: 0] Prescaler value    */
      typedef RegisterBits< R,  8,  8 > GT;            /**< [15: 8] Guard time value   */
    };


  public:

    typedef __SR  < Register< uint32_t, reg_base + 0x00, Access::rw, 0x00C0 > > SR;
    typedef __DR  < Register< uint32_t, reg_base + 0x04, Access::rw         > > DR;
    typedef __BRR < Register< uint32_t, reg_base + 0x08, Access::rw         > > BRR;
    typedef __CR1 < Register< uint32_t, reg_base + 0x0c, Access::rw         > > CR1;
    typedef __CR2 < Register< uint32_t, reg_base + 0x10, Access::rw         > > CR2;
    typedef __CR3 < Register< uint32_t, reg_base + 0x14, Access::rw         > > CR3;
    typedef __GTPR< Register< uint32_t, reg_base + 0x18, Access::rw         > > GTPR;
  };
}

#endif // REGISTER_DEFS_HPP_INCLUDED
