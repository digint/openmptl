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

#ifndef CORE_HPP_INCLUDED
#define CORE_HPP_INCLUDED

#include <cppcore_setup.hpp>
#include <arch/register_defs.hpp>


////////////////////  cIrq  ////////////////////


namespace IrqNumber
{
  /*****  Cortex-M3 Processor Exceptions Numbers *****/
  enum class CoreException : int
  {
//    NonMaskableInt         = -14,    /**< 2 Non Maskable Interrupt                                        */
    MemoryManagement       = -12,    /**< 4 Cortex-M3 Memory Management Interrupt                           */
    BusFault               = -11,    /**< 5 Cortex-M3 Bus Fault Interrupt                                   */
    UsageFault             = -10,    /**< 6 Cortex-M3 Usage Fault Interrupt                                 */
    SVCall                 = -5,     /**< 11 Cortex-M3 SV Call Interrupt                                    */
    DebugMonitor           = -4,     /**< 12 Cortex-M3 Debug Monitor Interrupt                              */
    PendSV                 = -2,     /**< 14 Cortex-M3 Pend SV Interrupt                                    */
    SysTick                = -1      /**< 15 Cortex-M3 System Tick Interrupt                                */
  };

  /*****  STM32 specific Interrupt Numbers *****/
  enum class Interrupt : uint32_t
  {
    WWDG                   = 0,      /**< Window WatchDog Interrupt                                         */
    PVD                    = 1,      /**< PVD through EXTI Line detection Interrupt                         */
    TAMPER                 = 2,      /**< Tamper Interrupt                                                  */
    RTC                    = 3,      /**< RTC global Interrupt                                              */
    FLASH                  = 4,      /**< FLASH global Interrupt                                            */
    RCC                    = 5,      /**< RCC global Interrupt                                              */
    EXTI0                  = 6,      /**< EXTI Line0 Interrupt                                              */
    EXTI1                  = 7,      /**< EXTI Line1 Interrupt                                              */
    EXTI2                  = 8,      /**< EXTI Line2 Interrupt                                              */
    EXTI3                  = 9,      /**< EXTI Line3 Interrupt                                              */
    EXTI4                  = 10,     /**< EXTI Line4 Interrupt                                              */
    DMA1_Channel1          = 11,     /**< DMA1 Channel 1 global Interrupt                                   */
    DMA1_Channel2          = 12,     /**< DMA1 Channel 2 global Interrupt                                   */
    DMA1_Channel3          = 13,     /**< DMA1 Channel 3 global Interrupt                                   */
    DMA1_Channel4          = 14,     /**< DMA1 Channel 4 global Interrupt                                   */
    DMA1_Channel5          = 15,     /**< DMA1 Channel 5 global Interrupt                                   */
    DMA1_Channel6          = 16,     /**< DMA1 Channel 6 global Interrupt                                   */
    DMA1_Channel7          = 17,     /**< DMA1 Channel 7 global Interrupt                                   */

#ifdef STM32F10X_LD
    ADC1_2                 = 18,     /**< ADC1 and ADC2 global Interrupt                                    */
    USB_HP_CAN1_TX         = 19,     /**< USB Device High Priority or CAN1 TX Interrupts                    */
    USB_LP_CAN1_RX0        = 20,     /**< USB Device Low Priority or CAN1 RX0 Interrupts                    */
    CAN1_RX1               = 21,     /**< CAN1 RX1 Interrupt                                                */
    CAN1_SCE               = 22,     /**< CAN1 SCE Interrupt                                                */
    EXTI9_5                = 23,     /**< External Line[9:5] Interrupts                                     */
    TIM1_BRK               = 24,     /**< TIM1 Break Interrupt                                              */
    TIM1_UP                = 25,     /**< TIM1 Update Interrupt                                             */
    TIM1_TRG_COM           = 26,     /**< TIM1 Trigger and Commutation Interrupt                            */
    TIM1_CC                = 27,     /**< TIM1 Capture Compare Interrupt                                    */
    TIM2                   = 28,     /**< TIM2 global Interrupt                                             */
    TIM3                   = 29,     /**< TIM3 global Interrupt                                             */
    I2C1_EV                = 31,     /**< I2C1 Event Interrupt                                              */
    I2C1_ER                = 32,     /**< I2C1 Error Interrupt                                              */
    SPI1                   = 35,     /**< SPI1 global Interrupt                                             */
    USART1                 = 37,     /**< USART1 global Interrupt                                           */
    USART2                 = 38,     /**< USART2 global Interrupt                                           */
    EXTI15_10              = 40,     /**< External Line[15:10] Interrupts                                   */
    RTCAlarm               = 41,     /**< RTC Alarm through EXTI Line Interrupt                             */
    USBWakeUp              = 42      /**< USB Device WakeUp from suspend through EXTI Line Interrupt        */    
#endif /* STM32F10X_LD                                                                                      */  

#ifdef STM32F10X_LD_VL
    ADC1                   = 18,     /**< ADC1 global Interrupt                                             */
    EXTI9_5                = 23,     /**< External Line[9:5] Interrupts                                     */
    TIM1_BRK_TIM15         = 24,     /**< TIM1 Break and TIM15 Interrupts                                   */
    TIM1_UP_TIM16          = 25,     /**< TIM1 Update and TIM16 Interrupts                                  */
    TIM1_TRG_COM_TIM17     = 26,     /**< TIM1 Trigger and Commutation and TIM17 Interrupt                  */
    TIM1_CC                = 27,     /**< TIM1 Capture Compare Interrupt                                    */
    TIM2                   = 28,     /**< TIM2 global Interrupt                                             */
    TIM3                   = 29,     /**< TIM3 global Interrupt                                             */
    I2C1_EV                = 31,     /**< I2C1 Event Interrupt                                              */
    I2C1_ER                = 32,     /**< I2C1 Error Interrupt                                              */
    SPI1                   = 35,     /**< SPI1 global Interrupt                                             */
    USART1                 = 37,     /**< USART1 global Interrupt                                           */
    USART2                 = 38,     /**< USART2 global Interrupt                                           */
    EXTI15_10              = 40,     /**< External Line[15:10] Interrupts                                   */
    RTCAlarm               = 41,     /**< RTC Alarm through EXTI Line Interrupt                             */
    CEC                    = 42,     /**< HDMI-CEC Interrupt                                                */
    TIM6_DAC               = 54,     /**< TIM6 and DAC underrun Interrupt                                   */
    TIM7                   = 55      /**< TIM7 Interrupt                                                    */       
#endif /* STM32F10X_LD_VL                                                                                   */

#ifdef STM32F10X_MD
    ADC1_2                 = 18,     /**< ADC1 and ADC2 global Interrupt                                    */
    USB_HP_CAN1_TX         = 19,     /**< USB Device High Priority or CAN1 TX Interrupts                    */
    USB_LP_CAN1_RX0        = 20,     /**< USB Device Low Priority or CAN1 RX0 Interrupts                    */
    CAN1_RX1               = 21,     /**< CAN1 RX1 Interrupt                                                */
    CAN1_SCE               = 22,     /**< CAN1 SCE Interrupt                                                */
    EXTI9_5                = 23,     /**< External Line[9:5] Interrupts                                     */
    TIM1_BRK               = 24,     /**< TIM1 Break Interrupt                                              */
    TIM1_UP                = 25,     /**< TIM1 Update Interrupt                                             */
    TIM1_TRG_COM           = 26,     /**< TIM1 Trigger and Commutation Interrupt                            */
    TIM1_CC                = 27,     /**< TIM1 Capture Compare Interrupt                                    */
    TIM2                   = 28,     /**< TIM2 global Interrupt                                             */
    TIM3                   = 29,     /**< TIM3 global Interrupt                                             */
    TIM4                   = 30,     /**< TIM4 global Interrupt                                             */
    I2C1_EV                = 31,     /**< I2C1 Event Interrupt                                              */
    I2C1_ER                = 32,     /**< I2C1 Error Interrupt                                              */
    I2C2_EV                = 33,     /**< I2C2 Event Interrupt                                              */
    I2C2_ER                = 34,     /**< I2C2 Error Interrupt                                              */
    SPI1                   = 35,     /**< SPI1 global Interrupt                                             */
    SPI2                   = 36,     /**< SPI2 global Interrupt                                             */
    USART1                 = 37,     /**< USART1 global Interrupt                                           */
    USART2                 = 38,     /**< USART2 global Interrupt                                           */
    USART3                 = 39,     /**< USART3 global Interrupt                                           */
    EXTI15_10              = 40,     /**< External Line[15:10] Interrupts                                   */
    RTCAlarm               = 41,     /**< RTC Alarm through EXTI Line Interrupt                             */
    USBWakeUp              = 42      /**< USB Device WakeUp from suspend through EXTI Line Interrupt        */  
#endif /* STM32F10X_MD                                                                                      */  

#ifdef STM32F10X_MD_VL
    ADC1                   = 18,     /**< ADC1 global Interrupt                                             */
    EXTI9_5                = 23,     /**< External Line[9:5] Interrupts                                     */
    TIM1_BRK_TIM15         = 24,     /**< TIM1 Break and TIM15 Interrupts                                   */
    TIM1_UP_TIM16          = 25,     /**< TIM1 Update and TIM16 Interrupts                                  */
    TIM1_TRG_COM_TIM17     = 26,     /**< TIM1 Trigger and Commutation and TIM17 Interrupt                  */
    TIM1_CC                = 27,     /**< TIM1 Capture Compare Interrupt                                    */
    TIM2                   = 28,     /**< TIM2 global Interrupt                                             */
    TIM3                   = 29,     /**< TIM3 global Interrupt                                             */
    TIM4                   = 30,     /**< TIM4 global Interrupt                                             */
    I2C1_EV                = 31,     /**< I2C1 Event Interrupt                                              */
    I2C1_ER                = 32,     /**< I2C1 Error Interrupt                                              */
    I2C2_EV                = 33,     /**< I2C2 Event Interrupt                                              */
    I2C2_ER                = 34,     /**< I2C2 Error Interrupt                                              */
    SPI1                   = 35,     /**< SPI1 global Interrupt                                             */
    SPI2                   = 36,     /**< SPI2 global Interrupt                                             */
    USART1                 = 37,     /**< USART1 global Interrupt                                           */
    USART2                 = 38,     /**< USART2 global Interrupt                                           */
    USART3                 = 39,     /**< USART3 global Interrupt                                           */
    EXTI15_10              = 40,     /**< External Line[15:10] Interrupts                                   */
    RTCAlarm               = 41,     /**< RTC Alarm through EXTI Line Interrupt                             */
    CEC                    = 42,     /**< HDMI-CEC Interrupt                                                */
    TIM6_DAC               = 54,     /**< TIM6 and DAC underrun Interrupt                                   */
    TIM7                   = 55      /**< TIM7 Interrupt                                                    */       
#endif /* STM32F10X_MD_VL                                                                                   */

#ifdef STM32F10X_HD
    ADC1_2                 = 18,     /**< ADC1 and ADC2 global Interrupt                                    */
    USB_HP_CAN1_TX         = 19,     /**< USB Device High Priority or CAN1 TX Interrupts                    */
    USB_LP_CAN1_RX0        = 20,     /**< USB Device Low Priority or CAN1 RX0 Interrupts                    */
    CAN1_RX1               = 21,     /**< CAN1 RX1 Interrupt                                                */
    CAN1_SCE               = 22,     /**< CAN1 SCE Interrupt                                                */
    EXTI9_5                = 23,     /**< External Line[9:5] Interrupts                                     */
    TIM1_BRK               = 24,     /**< TIM1 Break Interrupt                                              */
    TIM1_UP                = 25,     /**< TIM1 Update Interrupt                                             */
    TIM1_TRG_COM           = 26,     /**< TIM1 Trigger and Commutation Interrupt                            */
    TIM1_CC                = 27,     /**< TIM1 Capture Compare Interrupt                                    */
    TIM2                   = 28,     /**< TIM2 global Interrupt                                             */
    TIM3                   = 29,     /**< TIM3 global Interrupt                                             */
    TIM4                   = 30,     /**< TIM4 global Interrupt                                             */
    I2C1_EV                = 31,     /**< I2C1 Event Interrupt                                              */
    I2C1_ER                = 32,     /**< I2C1 Error Interrupt                                              */
    I2C2_EV                = 33,     /**< I2C2 Event Interrupt                                              */
    I2C2_ER                = 34,     /**< I2C2 Error Interrupt                                              */
    SPI1                   = 35,     /**< SPI1 global Interrupt                                             */
    SPI2                   = 36,     /**< SPI2 global Interrupt                                             */
    USART1                 = 37,     /**< USART1 global Interrupt                                           */
    USART2                 = 38,     /**< USART2 global Interrupt                                           */
    USART3                 = 39,     /**< USART3 global Interrupt                                           */
    EXTI15_10              = 40,     /**< External Line[15:10] Interrupts                                   */
    RTCAlarm               = 41,     /**< RTC Alarm through EXTI Line Interrupt                             */
    USBWakeUp              = 42,     /**< USB Device WakeUp from suspend through EXTI Line Interrupt        */
    TIM8_BRK               = 43,     /**< TIM8 Break Interrupt                                              */
    TIM8_UP                = 44,     /**< TIM8 Update Interrupt                                             */
    TIM8_TRG_COM           = 45,     /**< TIM8 Trigger and Commutation Interrupt                            */
    TIM8_CC                = 46,     /**< TIM8 Capture Compare Interrupt                                    */
    ADC3                   = 47,     /**< ADC3 global Interrupt                                             */
    FSMC                   = 48,     /**< FSMC global Interrupt                                             */
    SDIO                   = 49,     /**< SDIO global Interrupt                                             */
    TIM5                   = 50,     /**< TIM5 global Interrupt                                             */
    SPI3                   = 51,     /**< SPI3 global Interrupt                                             */
    UART4                  = 52,     /**< UART4 global Interrupt                                            */
    UART5                  = 53,     /**< UART5 global Interrupt                                            */
    TIM6                   = 54,     /**< TIM6 global Interrupt                                             */
    TIM7                   = 55,     /**< TIM7 global Interrupt                                             */
    DMA2_Channel1          = 56,     /**< DMA2 Channel 1 global Interrupt                                   */
    DMA2_Channel2          = 57,     /**< DMA2 Channel 2 global Interrupt                                   */
    DMA2_Channel3          = 58,     /**< DMA2 Channel 3 global Interrupt                                   */
    DMA2_Channel4_5        = 59      /**< DMA2 Channel 4 and Channel 5 global Interrupt                     */
#endif /* STM32F10X_HD                                                                                      */  

#ifdef STM32F10X_XL
    ADC1_2                 = 18,     /**< ADC1 and ADC2 global Interrupt                                    */
    USB_HP_CAN1_TX         = 19,     /**< USB Device High Priority or CAN1 TX Interrupts                    */
    USB_LP_CAN1_RX0        = 20,     /**< USB Device Low Priority or CAN1 RX0 Interrupts                    */
    CAN1_RX1               = 21,     /**< CAN1 RX1 Interrupt                                                */
    CAN1_SCE               = 22,     /**< CAN1 SCE Interrupt                                                */
    EXTI9_5                = 23,     /**< External Line[9:5] Interrupts                                     */
    TIM1_BRK_TIM9          = 24,     /**< TIM1 Break Interrupt and TIM9 global Interrupt                    */
    TIM1_UP_TIM10          = 25,     /**< TIM1 Update Interrupt and TIM10 global Interrupt                  */
    TIM1_TRG_COM_TIM11     = 26,     /**< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
    TIM1_CC                = 27,     /**< TIM1 Capture Compare Interrupt                                    */
    TIM2                   = 28,     /**< TIM2 global Interrupt                                             */
    TIM3                   = 29,     /**< TIM3 global Interrupt                                             */
    TIM4                   = 30,     /**< TIM4 global Interrupt                                             */
    I2C1_EV                = 31,     /**< I2C1 Event Interrupt                                              */
    I2C1_ER                = 32,     /**< I2C1 Error Interrupt                                              */
    I2C2_EV                = 33,     /**< I2C2 Event Interrupt                                              */
    I2C2_ER                = 34,     /**< I2C2 Error Interrupt                                              */
    SPI1                   = 35,     /**< SPI1 global Interrupt                                             */
    SPI2                   = 36,     /**< SPI2 global Interrupt                                             */
    USART1                 = 37,     /**< USART1 global Interrupt                                           */
    USART2                 = 38,     /**< USART2 global Interrupt                                           */
    USART3                 = 39,     /**< USART3 global Interrupt                                           */
    EXTI15_10              = 40,     /**< External Line[15:10] Interrupts                                   */
    RTCAlarm               = 41,     /**< RTC Alarm through EXTI Line Interrupt                             */
    USBWakeUp              = 42,     /**< USB Device WakeUp from suspend through EXTI Line Interrupt        */
    TIM8_BRK_TIM12         = 43,     /**< TIM8 Break Interrupt and TIM12 global Interrupt                   */
    TIM8_UP_TIM13          = 44,     /**< TIM8 Update Interrupt and TIM13 global Interrupt                  */
    TIM8_TRG_COM_TIM14     = 45,     /**< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
    TIM8_CC                = 46,     /**< TIM8 Capture Compare Interrupt                                    */
    ADC3                   = 47,     /**< ADC3 global Interrupt                                             */
    FSMC                   = 48,     /**< FSMC global Interrupt                                             */
    SDIO                   = 49,     /**< SDIO global Interrupt                                             */
    TIM5                   = 50,     /**< TIM5 global Interrupt                                             */
    SPI3                   = 51,     /**< SPI3 global Interrupt                                             */
    UART4                  = 52,     /**< UART4 global Interrupt                                            */
    UART5                  = 53,     /**< UART5 global Interrupt                                            */
    TIM6                   = 54,     /**< TIM6 global Interrupt                                             */
    TIM7                   = 55,     /**< TIM7 global Interrupt                                             */
    DMA2_Channel1          = 56,     /**< DMA2 Channel 1 global Interrupt                                   */
    DMA2_Channel2          = 57,     /**< DMA2 Channel 2 global Interrupt                                   */
    DMA2_Channel3          = 58,     /**< DMA2 Channel 3 global Interrupt                                   */
    DMA2_Channel4_5        = 59      /**< DMA2 Channel 4 and Channel 5 global Interrupt                     */
#endif /* STM32F10X_XL                                                                                      */  

#ifdef STM32F10X_CL
    ADC1_2                 = 18,     /**< ADC1 and ADC2 global Interrupt                                    */
    CAN1_TX                = 19,     /**< USB Device High Priority or CAN1 TX Interrupts                    */
    CAN1_RX0               = 20,     /**< USB Device Low Priority or CAN1 RX0 Interrupts                    */
    CAN1_RX1               = 21,     /**< CAN1 RX1 Interrupt                                                */
    CAN1_SCE               = 22,     /**< CAN1 SCE Interrupt                                                */
    EXTI9_5                = 23,     /**< External Line[9:5] Interrupts                                     */
    TIM1_BRK               = 24,     /**< TIM1 Break Interrupt                                              */
    TIM1_UP                = 25,     /**< TIM1 Update Interrupt                                             */
    TIM1_TRG_COM           = 26,     /**< TIM1 Trigger and Commutation Interrupt                            */
    TIM1_CC                = 27,     /**< TIM1 Capture Compare Interrupt                                    */
    TIM2                   = 28,     /**< TIM2 global Interrupt                                             */
    TIM3                   = 29,     /**< TIM3 global Interrupt                                             */
    TIM4                   = 30,     /**< TIM4 global Interrupt                                             */
    I2C1_EV                = 31,     /**< I2C1 Event Interrupt                                              */
    I2C1_ER                = 32,     /**< I2C1 Error Interrupt                                              */
    I2C2_EV                = 33,     /**< I2C2 Event Interrupt                                              */
    I2C2_ER                = 34,     /**< I2C2 Error Interrupt                                              */
    SPI1                   = 35,     /**< SPI1 global Interrupt                                             */
    SPI2                   = 36,     /**< SPI2 global Interrupt                                             */
    USART1                 = 37,     /**< USART1 global Interrupt                                           */
    USART2                 = 38,     /**< USART2 global Interrupt                                           */
    USART3                 = 39,     /**< USART3 global Interrupt                                           */
    EXTI15_10              = 40,     /**< External Line[15:10] Interrupts                                   */
    RTCAlarm               = 41,     /**< RTC Alarm through EXTI Line Interrupt                             */
    OTG_FS_WKUP            = 42,     /**< USB OTG FS WakeUp from suspend through EXTI Line Interrupt        */
    TIM5                   = 50,     /**< TIM5 global Interrupt                                             */
    SPI3                   = 51,     /**< SPI3 global Interrupt                                             */
    UART4                  = 52,     /**< UART4 global Interrupt                                            */
    UART5                  = 53,     /**< UART5 global Interrupt                                            */
    TIM6                   = 54,     /**< TIM6 global Interrupt                                             */
    TIM7                   = 55,     /**< TIM7 global Interrupt                                             */
    DMA2_Channel1          = 56,     /**< DMA2 Channel 1 global Interrupt                                   */
    DMA2_Channel2          = 57,     /**< DMA2 Channel 2 global Interrupt                                   */
    DMA2_Channel3          = 58,     /**< DMA2 Channel 3 global Interrupt                                   */
    DMA2_Channel4          = 59,     /**< DMA2 Channel 4 global Interrupt                                   */
    DMA2_Channel5          = 60,     /**< DMA2 Channel 5 global Interrupt                                   */
    ETH                    = 61,     /**< Ethernet global Interrupt                                         */
    ETH_WKUP               = 62,     /**< Ethernet Wakeup through EXTI line Interrupt                       */
    CAN2_TX                = 63,     /**< CAN2 TX Interrupt                                                 */
    CAN2_RX0               = 64,     /**< CAN2 RX0 Interrupt                                                */
    CAN2_RX1               = 65,     /**< CAN2 RX1 Interrupt                                                */
    CAN2_SCE               = 66,     /**< CAN2 SCE Interrupt                                                */
    OTG_FS                 = 67      /**< USB OTG FS global Interrupt                                       */
#endif /* STM32F10X_CL                                                                                      */     
  };
}


////////////////////  CoreFunctions  ////////////////////


struct CoreFunctions
{
  static inline void enable_irq()        { __asm volatile ("cpsie i"); } //< global interrupt enable
  static inline void disable_irq()       { __asm volatile ("cpsid i"); } //< global interrupt disable

  static inline void enable_fault_irq()  { __asm volatile ("cpsie f"); }
  static inline void disable_fault_irq() { __asm volatile ("cpsid f"); }

  static inline void nop()               { __asm volatile ("nop"); }
  static inline void wfi()               { __asm volatile ("wfi"); }
  static inline void wfe()               { __asm volatile ("wfe"); }
  static inline void sev()               { __asm volatile ("sev"); }
  static inline void isb()               { __asm volatile ("isb"); }
  static inline void dsb()               { __asm volatile ("dsb"); }
  static inline void dmb()               { __asm volatile ("dmb"); }
  static inline void clrex()             { __asm volatile ("clrex"); }
};


////////////////////  Core  ////////////////////


class Core : public CoreRegister,
             public PeripheralRegister,
             public CoreSetup,
             public CoreFunctions
{
public:

  /* Initialize the hardware. */
  /* NOTE: this function is called BEFORE ANY static object constructors are called! */
  static void InitHW(void);

  static void nop(unsigned value) {
    while(value-- > 0 ) { CoreFunctions::nop(); }
  }

  /* make sure clock_frequency is set correctly in cppcore_setup.hpp */
  static_assert(clock_frequency >= 0, "invalid clock frequency (maybe you forgot to set it in cppcore_setup.hpp ?)");

#if 0
  struct CriticalSection
  {
    CriticalSection () { disable_irq(); }
    ~CriticalSection() { enable_irq(); }
  };
#endif

private:
  static inline void InitClocks();
};

#endif // CORE_HPP_INCLUDED
