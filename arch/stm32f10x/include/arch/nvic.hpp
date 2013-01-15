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

#ifndef NVIC_HPP_INCLUDED
#define NVIC_HPP_INCLUDED

#include "../../../common/arm-cortex/nvic.hpp"

struct NvicSetup {
#if defined(STM32F10X_LD) || defined(STM32F10X_MD)
  static constexpr int irq_channels = 43;
#elif defined(STM32F10X_LD_VL) || defined(STM32F10X_MD_VL)
  static constexpr int irq_channels = 56;
#elif defined(STM32F10X_HD) || defined(STM32F10X_XL)
  static constexpr int irq_channels = 60;
#elif defined(STM32F10X_CL)
  static constexpr int irq_channels = 68;
#endif
};

typedef Irq<0>   IrqWWDG;                   /**< Window WatchDog Interrupt                   */
typedef Irq<1>   IrqPVD;                    /**< PVD through EXTI Line detection Interrupt   */
typedef Irq<2>   IrqTAMPER;                 /**< Tamper Interrupt                            */
typedef Irq<3>   IrqRTC;                    /**< RTC global Interrupt                        */
typedef Irq<4>   IrqFLASH;                  /**< FLASH global Interrupt                      */
typedef Irq<5>   IrqRCC;                    /**< RCC global Interrupt                        */
typedef Irq<6>   IrqEXTI0;                  /**< EXTI Line0 Interrupt                        */
typedef Irq<7>   IrqEXTI1;                  /**< EXTI Line1 Interrupt                        */
typedef Irq<8>   IrqEXTI2;                  /**< EXTI Line2 Interrupt                        */
typedef Irq<9>   IrqEXTI3;                  /**< EXTI Line3 Interrupt                        */
typedef Irq<10>  IrqEXTI4;                  /**< EXTI Line4 Interrupt                        */
typedef Irq<11>  IrqDMA1_Channel1;          /**< DMA1 Channel 1 global Interrupt             */
typedef Irq<12>  IrqDMA1_Channel2;          /**< DMA1 Channel 2 global Interrupt             */
typedef Irq<13>  IrqDMA1_Channel3;          /**< DMA1 Channel 3 global Interrupt             */
typedef Irq<14>  IrqDMA1_Channel4;          /**< DMA1 Channel 4 global Interrupt             */
typedef Irq<15>  IrqDMA1_Channel5;          /**< DMA1 Channel 5 global Interrupt             */
typedef Irq<16>  IrqDMA1_Channel6;          /**< DMA1 Channel 6 global Interrupt             */
typedef Irq<17>  IrqDMA1_Channel7;          /**< DMA1 Channel 7 global Interrupt             */

#ifdef STM32F10X_LD
typedef Irq<18>  IrqADC1_2;                 /**< ADC1 and ADC2 global Interrupt                               */
typedef Irq<19>  IrqUSB_HP_CAN1_TX;         /**< USB Device High Priority or CAN1 TX Interrupts               */
typedef Irq<20>  IrqUSB_LP_CAN1_RX0;        /**< USB Device Low Priority or CAN1 RX0 Interrupts               */
typedef Irq<21>  IrqCAN1_RX1;               /**< CAN1 RX1 Interrupt                                           */
typedef Irq<22>  IrqCAN1_SCE;               /**< CAN1 SCE Interrupt                                           */
typedef Irq<23>  IrqEXTI9_5;                /**< External Line[9:5] Interrupts                                */
typedef Irq<24>  IrqTIM1_BRK;               /**< TIM1 Break Interrupt                                         */
typedef Irq<25>  IrqTIM1_UP;                /**< TIM1 Update Interrupt                                        */
typedef Irq<26>  IrqTIM1_TRG_COM;           /**< TIM1 Trigger and Commutation Interrupt                       */
typedef Irq<27>  IrqTIM1_CC;                /**< TIM1 Capture Compare Interrupt                               */
typedef Irq<28>  IrqTIM2;                   /**< TIM2 global Interrupt                                        */
typedef Irq<29>  IrqTIM3;                   /**< TIM3 global Interrupt                                        */
typedef Irq<31>  IrqI2C1_EV;                /**< I2C1 Event Interrupt                                         */
typedef Irq<32>  IrqI2C1_ER;                /**< I2C1 Error Interrupt                                         */
typedef Irq<35>  IrqSPI1;                   /**< SPI1 global Interrupt                                        */
typedef Irq<37>  IrqUSART1;                 /**< USART1 global Interrupt                                      */
typedef Irq<38>  IrqUSART2;                 /**< USART2 global Interrupt                                      */
typedef Irq<40>  IrqEXTI15_10;              /**< External Line[15:10] Interrupts                              */
typedef Irq<41>  IrqRTCAlarm;               /**< RTC Alarm through EXTI Line Interrupt                        */
typedef Irq<42>  IrqUSBWakeUp;              /**< USB Device WakeUp from suspend through EXTI Line Interrupt   */
#endif // STM32F10X_LD

#ifdef STM32F10X_LD_VL
typedef Irq<18>  IrqADC1;                   /**< ADC1 global Interrupt                              */
typedef Irq<23>  IrqEXTI9_5;                /**< External Line[9:5] Interrupts                      */
typedef Irq<24>  IrqTIM1_BRK_TIM15;         /**< TIM1 Break and TIM15 Interrupts                    */
typedef Irq<25>  IrqTIM1_UP_TIM16;          /**< TIM1 Update and TIM16 Interrupts                   */
typedef Irq<26>  IrqTIM1_TRG_COM_TIM17;     /**< TIM1 Trigger and Commutation and TIM17 Interrupt   */
typedef Irq<27>  IrqTIM1_CC;                /**< TIM1 Capture Compare Interrupt                     */
typedef Irq<28>  IrqTIM2;                   /**< TIM2 global Interrupt                              */
typedef Irq<29>  IrqTIM3;                   /**< TIM3 global Interrupt                              */
typedef Irq<31>  IrqI2C1_EV;                /**< I2C1 Event Interrupt                               */
typedef Irq<32>  IrqI2C1_ER;                /**< I2C1 Error Interrupt                               */
typedef Irq<35>  IrqSPI1;                   /**< SPI1 global Interrupt                              */
typedef Irq<37>  IrqUSART1;                 /**< USART1 global Interrupt                            */
typedef Irq<38>  IrqUSART2;                 /**< USART2 global Interrupt                            */
typedef Irq<40>  IrqEXTI15_10;              /**< External Line[15:10] Interrupts                    */
typedef Irq<41>  IrqRTCAlarm;               /**< RTC Alarm through EXTI Line Interrupt              */
typedef Irq<42>  IrqCEC;                    /**< HDMI-CEC Interrupt                                 */
typedef Irq<54>  IrqTIM6_DAC;               /**< TIM6 and DAC underrun Interrupt                    */
typedef Irq<55>  IrqTIM7;                   /**< TIM7 Interrupt                                     */
#endif // STM32F10X_LD_VL

#ifdef STM32F10X_MD
typedef Irq<18>  IrqADC1_2;                 /**< ADC1 and ADC2 global Interrupt                               */
typedef Irq<19>  IrqUSB_HP_CAN1_TX;         /**< USB Device High Priority or CAN1 TX Interrupts               */
typedef Irq<20>  IrqUSB_LP_CAN1_RX0;        /**< USB Device Low Priority or CAN1 RX0 Interrupts               */
typedef Irq<21>  IrqCAN1_RX1;               /**< CAN1 RX1 Interrupt                                           */
typedef Irq<22>  IrqCAN1_SCE;               /**< CAN1 SCE Interrupt                                           */
typedef Irq<23>  IrqEXTI9_5;                /**< External Line[9:5] Interrupts                                */
typedef Irq<24>  IrqTIM1_BRK;               /**< TIM1 Break Interrupt                                         */
typedef Irq<25>  IrqTIM1_UP;                /**< TIM1 Update Interrupt                                        */
typedef Irq<26>  IrqTIM1_TRG_COM;           /**< TIM1 Trigger and Commutation Interrupt                       */
typedef Irq<27>  IrqTIM1_CC;                /**< TIM1 Capture Compare Interrupt                               */
typedef Irq<28>  IrqTIM2;                   /**< TIM2 global Interrupt                                        */
typedef Irq<29>  IrqTIM3;                   /**< TIM3 global Interrupt                                        */
typedef Irq<30>  IrqTIM4;                   /**< TIM4 global Interrupt                                        */
typedef Irq<31>  IrqI2C1_EV;                /**< I2C1 Event Interrupt                                         */
typedef Irq<32>  IrqI2C1_ER;                /**< I2C1 Error Interrupt                                         */
typedef Irq<33>  IrqI2C2_EV;                /**< I2C2 Event Interrupt                                         */
typedef Irq<34>  IrqI2C2_ER;                /**< I2C2 Error Interrupt                                         */
typedef Irq<35>  IrqSPI1;                   /**< SPI1 global Interrupt                                        */
typedef Irq<36>  IrqSPI2;                   /**< SPI2 global Interrupt                                        */
typedef Irq<37>  IrqUSART1;                 /**< USART1 global Interrupt                                      */
typedef Irq<38>  IrqUSART2;                 /**< USART2 global Interrupt                                      */
typedef Irq<39>  IrqUSART3;                 /**< USART3 global Interrupt                                      */
typedef Irq<40>  IrqEXTI15_10;              /**< External Line[15:10] Interrupts                              */
typedef Irq<41>  IrqRTCAlarm;               /**< RTC Alarm through EXTI Line Interrupt                        */
typedef Irq<42>  IrqUSBWakeUp;              /**< USB Device WakeUp from suspend through EXTI Line Interrupt   */
#endif // STM32F10X_MD

#ifdef STM32F10X_MD_VL
typedef Irq<18>  IrqADC1;                   /**< ADC1 global Interrupt                              */
typedef Irq<23>  IrqEXTI9_5;                /**< External Line[9:5] Interrupts                      */
typedef Irq<24>  IrqTIM1_BRK_TIM15;         /**< TIM1 Break and TIM15 Interrupts                    */
typedef Irq<25>  IrqTIM1_UP_TIM16;          /**< TIM1 Update and TIM16 Interrupts                   */
typedef Irq<26>  IrqTIM1_TRG_COM_TIM17;     /**< TIM1 Trigger and Commutation and TIM17 Interrupt   */
typedef Irq<27>  IrqTIM1_CC;                /**< TIM1 Capture Compare Interrupt                     */
typedef Irq<28>  IrqTIM2;                   /**< TIM2 global Interrupt                              */
typedef Irq<29>  IrqTIM3;                   /**< TIM3 global Interrupt                              */
typedef Irq<30>  IrqTIM4;                   /**< TIM4 global Interrupt                              */
typedef Irq<31>  IrqI2C1_EV;                /**< I2C1 Event Interrupt                               */
typedef Irq<32>  IrqI2C1_ER;                /**< I2C1 Error Interrupt                               */
typedef Irq<33>  IrqI2C2_EV;                /**< I2C2 Event Interrupt                               */
typedef Irq<34>  IrqI2C2_ER;                /**< I2C2 Error Interrupt                               */
typedef Irq<35>  IrqSPI1;                   /**< SPI1 global Interrupt                              */
typedef Irq<36>  IrqSPI2;                   /**< SPI2 global Interrupt                              */
typedef Irq<37>  IrqUSART1;                 /**< USART1 global Interrupt                            */
typedef Irq<38>  IrqUSART2;                 /**< USART2 global Interrupt                            */
typedef Irq<39>  IrqUSART3;                 /**< USART3 global Interrupt                            */
typedef Irq<40>  IrqEXTI15_10;              /**< External Line[15:10] Interrupts                    */
typedef Irq<41>  IrqRTCAlarm;               /**< RTC Alarm through EXTI Line Interrupt              */
typedef Irq<42>  IrqCEC;                    /**< HDMI-CEC Interrupt                                 */
typedef Irq<54>  IrqTIM6_DAC;               /**< TIM6 and DAC underrun Interrupt                    */
typedef Irq<55>  IrqTIM7;                   /**< TIM7 Interrupt                                     */
#endif // STM32F10X_MD_VL

#ifdef STM32F10X_HD
typedef Irq<18>  IrqADC1_2;                 /**< ADC1 and ADC2 global Interrupt                               */
typedef Irq<19>  IrqUSB_HP_CAN1_TX;         /**< USB Device High Priority or CAN1 TX Interrupts               */
typedef Irq<20>  IrqUSB_LP_CAN1_RX0;        /**< USB Device Low Priority or CAN1 RX0 Interrupts               */
typedef Irq<21>  IrqCAN1_RX1;               /**< CAN1 RX1 Interrupt                                           */
typedef Irq<22>  IrqCAN1_SCE;               /**< CAN1 SCE Interrupt                                           */
typedef Irq<23>  IrqEXTI9_5;                /**< External Line[9:5] Interrupts                                */
typedef Irq<24>  IrqTIM1_BRK;               /**< TIM1 Break Interrupt                                         */
typedef Irq<25>  IrqTIM1_UP;                /**< TIM1 Update Interrupt                                        */
typedef Irq<26>  IrqTIM1_TRG_COM;           /**< TIM1 Trigger and Commutation Interrupt                       */
typedef Irq<27>  IrqTIM1_CC;                /**< TIM1 Capture Compare Interrupt                               */
typedef Irq<28>  IrqTIM2;                   /**< TIM2 global Interrupt                                        */
typedef Irq<29>  IrqTIM3;                   /**< TIM3 global Interrupt                                        */
typedef Irq<30>  IrqTIM4;                   /**< TIM4 global Interrupt                                        */
typedef Irq<31>  IrqI2C1_EV;                /**< I2C1 Event Interrupt                                         */
typedef Irq<32>  IrqI2C1_ER;                /**< I2C1 Error Interrupt                                         */
typedef Irq<33>  IrqI2C2_EV;                /**< I2C2 Event Interrupt                                         */
typedef Irq<34>  IrqI2C2_ER;                /**< I2C2 Error Interrupt                                         */
typedef Irq<35>  IrqSPI1;                   /**< SPI1 global Interrupt                                        */
typedef Irq<36>  IrqSPI2;                   /**< SPI2 global Interrupt                                        */
typedef Irq<37>  IrqUSART1;                 /**< USART1 global Interrupt                                      */
typedef Irq<38>  IrqUSART2;                 /**< USART2 global Interrupt                                      */
typedef Irq<39>  IrqUSART3;                 /**< USART3 global Interrupt                                      */
typedef Irq<40>  IrqEXTI15_10;              /**< External Line[15:10] Interrupts                              */
typedef Irq<41>  IrqRTCAlarm;               /**< RTC Alarm through EXTI Line Interrupt                        */
typedef Irq<42>  IrqUSBWakeUp;              /**< USB Device WakeUp from suspend through EXTI Line Interrupt   */
typedef Irq<43>  IrqTIM8_BRK;               /**< TIM8 Break Interrupt                                         */
typedef Irq<44>  IrqTIM8_UP;                /**< TIM8 Update Interrupt                                        */
typedef Irq<45>  IrqTIM8_TRG_COM;           /**< TIM8 Trigger and Commutation Interrupt                       */
typedef Irq<46>  IrqTIM8_CC;                /**< TIM8 Capture Compare Interrupt                               */
typedef Irq<47>  IrqADC3;                   /**< ADC3 global Interrupt                                        */
typedef Irq<48>  IrqFSMC;                   /**< FSMC global Interrupt                                        */
typedef Irq<49>  IrqSDIO;                   /**< SDIO global Interrupt                                        */
typedef Irq<50>  IrqTIM5;                   /**< TIM5 global Interrupt                                        */
typedef Irq<51>  IrqSPI3;                   /**< SPI3 global Interrupt                                        */
typedef Irq<52>  IrqUART4;                  /**< UART4 global Interrupt                                       */
typedef Irq<53>  IrqUART5;                  /**< UART5 global Interrupt                                       */
typedef Irq<54>  IrqTIM6;                   /**< TIM6 global Interrupt                                        */
typedef Irq<55>  IrqTIM7;                   /**< TIM7 global Interrupt                                        */
typedef Irq<56>  IrqDMA2_Channel1;          /**< DMA2 Channel 1 global Interrupt                              */
typedef Irq<57>  IrqDMA2_Channel2;          /**< DMA2 Channel 2 global Interrupt                              */
typedef Irq<58>  IrqDMA2_Channel3;          /**< DMA2 Channel 3 global Interrupt                              */
typedef Irq<59>  IrqDMA2_Channel4_5;        /**< DMA2 Channel 4 and Channel 5 global Interrupt                */
#endif // STM32F10X_HD

#ifdef STM32F10X_XL
typedef Irq<18>  IrqADC1_2;                 /**< ADC1 and ADC2 global Interrupt                                      */
typedef Irq<19>  IrqUSB_HP_CAN1_TX;         /**< USB Device High Priority or CAN1 TX Interrupts                      */
typedef Irq<20>  IrqUSB_LP_CAN1_RX0;        /**< USB Device Low Priority or CAN1 RX0 Interrupts                      */
typedef Irq<21>  IrqCAN1_RX1;               /**< CAN1 RX1 Interrupt                                                  */
typedef Irq<22>  IrqCAN1_SCE;               /**< CAN1 SCE Interrupt                                                  */
typedef Irq<23>  IrqEXTI9_5;                /**< External Line[9:5] Interrupts                                       */
typedef Irq<24>  IrqTIM1_BRK_TIM9;          /**< TIM1 Break Interrupt and TIM9 global Interrupt                      */
typedef Irq<25>  IrqTIM1_UP_TIM10;          /**< TIM1 Update Interrupt and TIM10 global Interrupt                    */
typedef Irq<26>  IrqTIM1_TRG_COM_TIM11;     /**< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt   */
typedef Irq<27>  IrqTIM1_CC;                /**< TIM1 Capture Compare Interrupt                                      */
typedef Irq<28>  IrqTIM2;                   /**< TIM2 global Interrupt                                               */
typedef Irq<29>  IrqTIM3;                   /**< TIM3 global Interrupt                                               */
typedef Irq<30>  IrqTIM4;                   /**< TIM4 global Interrupt                                               */
typedef Irq<31>  IrqI2C1_EV;                /**< I2C1 Event Interrupt                                                */
typedef Irq<32>  IrqI2C1_ER;                /**< I2C1 Error Interrupt                                                */
typedef Irq<33>  IrqI2C2_EV;                /**< I2C2 Event Interrupt                                                */
typedef Irq<34>  IrqI2C2_ER;                /**< I2C2 Error Interrupt                                                */
typedef Irq<35>  IrqSPI1;                   /**< SPI1 global Interrupt                                               */
typedef Irq<36>  IrqSPI2;                   /**< SPI2 global Interrupt                                               */
typedef Irq<37>  IrqUSART1;                 /**< USART1 global Interrupt                                             */
typedef Irq<38>  IrqUSART2;                 /**< USART2 global Interrupt                                             */
typedef Irq<39>  IrqUSART3;                 /**< USART3 global Interrupt                                             */
typedef Irq<40>  IrqEXTI15_10;              /**< External Line[15:10] Interrupts                                     */
typedef Irq<41>  IrqRTCAlarm;               /**< RTC Alarm through EXTI Line Interrupt                               */
typedef Irq<42>  IrqUSBWakeUp;              /**< USB Device WakeUp from suspend through EXTI Line Interrupt          */
typedef Irq<43>  IrqTIM8_BRK_TIM12;         /**< TIM8 Break Interrupt and TIM12 global Interrupt                     */
typedef Irq<44>  IrqTIM8_UP_TIM13;          /**< TIM8 Update Interrupt and TIM13 global Interrupt                    */
typedef Irq<45>  IrqTIM8_TRG_COM_TIM14;     /**< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt   */
typedef Irq<46>  IrqTIM8_CC;                /**< TIM8 Capture Compare Interrupt                                      */
typedef Irq<47>  IrqADC3;                   /**< ADC3 global Interrupt                                               */
typedef Irq<48>  IrqFSMC;                   /**< FSMC global Interrupt                                               */
typedef Irq<49>  IrqSDIO;                   /**< SDIO global Interrupt                                               */
typedef Irq<50>  IrqTIM5;                   /**< TIM5 global Interrupt                                               */
typedef Irq<51>  IrqSPI3;                   /**< SPI3 global Interrupt                                               */
typedef Irq<52>  IrqUART4;                  /**< UART4 global Interrupt                                              */
typedef Irq<53>  IrqUART5;                  /**< UART5 global Interrupt                                              */
typedef Irq<54>  IrqTIM6;                   /**< TIM6 global Interrupt                                               */
typedef Irq<55>  IrqTIM7;                   /**< TIM7 global Interrupt                                               */
typedef Irq<56>  IrqDMA2_Channel1;          /**< DMA2 Channel 1 global Interrupt                                     */
typedef Irq<57>  IrqDMA2_Channel2;          /**< DMA2 Channel 2 global Interrupt                                     */
typedef Irq<58>  IrqDMA2_Channel3;          /**< DMA2 Channel 3 global Interrupt                                     */
typedef Irq<59>  IrqDMA2_Channel4_5;        /**< DMA2 Channel 4 and Channel 5 global Interrupt                       */
#endif // STM32F10X_XL

#ifdef STM32F10X_CL
typedef Irq<18>  IrqADC1_2;                 /**< ADC1 and ADC2 global Interrupt                               */
typedef Irq<19>  IrqCAN1_TX;                /**< USB Device High Priority or CAN1 TX Interrupts               */
typedef Irq<20>  IrqCAN1_RX0;               /**< USB Device Low Priority or CAN1 RX0 Interrupts               */
typedef Irq<21>  IrqCAN1_RX1;               /**< CAN1 RX1 Interrupt                                           */
typedef Irq<22>  IrqCAN1_SCE;               /**< CAN1 SCE Interrupt                                           */
typedef Irq<23>  IrqEXTI9_5;                /**< External Line[9:5] Interrupts                                */
typedef Irq<24>  IrqTIM1_BRK;               /**< TIM1 Break Interrupt                                         */
typedef Irq<25>  IrqTIM1_UP;                /**< TIM1 Update Interrupt                                        */
typedef Irq<26>  IrqTIM1_TRG_COM;           /**< TIM1 Trigger and Commutation Interrupt                       */
typedef Irq<27>  IrqTIM1_CC;                /**< TIM1 Capture Compare Interrupt                               */
typedef Irq<28>  IrqTIM2;                   /**< TIM2 global Interrupt                                        */
typedef Irq<29>  IrqTIM3;                   /**< TIM3 global Interrupt                                        */
typedef Irq<30>  IrqTIM4;                   /**< TIM4 global Interrupt                                        */
typedef Irq<31>  IrqI2C1_EV;                /**< I2C1 Event Interrupt                                         */
typedef Irq<32>  IrqI2C1_ER;                /**< I2C1 Error Interrupt                                         */
typedef Irq<33>  IrqI2C2_EV;                /**< I2C2 Event Interrupt                                         */
typedef Irq<34>  IrqI2C2_ER;                /**< I2C2 Error Interrupt                                         */
typedef Irq<35>  IrqSPI1;                   /**< SPI1 global Interrupt                                        */
typedef Irq<36>  IrqSPI2;                   /**< SPI2 global Interrupt                                        */
typedef Irq<37>  IrqUSART1;                 /**< USART1 global Interrupt                                      */
typedef Irq<38>  IrqUSART2;                 /**< USART2 global Interrupt                                      */
typedef Irq<39>  IrqUSART3;                 /**< USART3 global Interrupt                                      */
typedef Irq<40>  IrqEXTI15_10;              /**< External Line[15:10] Interrupts                              */
typedef Irq<41>  IrqRTCAlarm;               /**< RTC Alarm through EXTI Line Interrupt                        */
typedef Irq<42>  IrqOTG_FS_WKUP;            /**< USB OTG FS WakeUp from suspend through EXTI Line Interrupt   */
typedef Irq<50>  IrqTIM5;                   /**< TIM5 global Interrupt                                        */
typedef Irq<51>  IrqSPI3;                   /**< SPI3 global Interrupt                                        */
typedef Irq<52>  IrqUART4;                  /**< UART4 global Interrupt                                       */
typedef Irq<53>  IrqUART5;                  /**< UART5 global Interrupt                                       */
typedef Irq<54>  IrqTIM6;                   /**< TIM6 global Interrupt                                        */
typedef Irq<55>  IrqTIM7;                   /**< TIM7 global Interrupt                                        */
typedef Irq<56>  IrqDMA2_Channel1;          /**< DMA2 Channel 1 global Interrupt                              */
typedef Irq<57>  IrqDMA2_Channel2;          /**< DMA2 Channel 2 global Interrupt                              */
typedef Irq<58>  IrqDMA2_Channel3;          /**< DMA2 Channel 3 global Interrupt                              */
typedef Irq<59>  IrqDMA2_Channel4;          /**< DMA2 Channel 4 global Interrupt                              */
typedef Irq<60>  IrqDMA2_Channel5;          /**< DMA2 Channel 5 global Interrupt                              */
typedef Irq<61>  IrqETH;                    /**< Ethernet global Interrupt                                    */
typedef Irq<62>  IrqETH_WKUP;               /**< Ethernet Wakeup through EXTI line Interrupt                  */
typedef Irq<63>  IrqCAN2_TX;                /**< CAN2 TX Interrupt                                            */
typedef Irq<64>  IrqCAN2_RX0;               /**< CAN2 RX0 Interrupt                                           */
typedef Irq<65>  IrqCAN2_RX1;               /**< CAN2 RX1 Interrupt                                           */
typedef Irq<66>  IrqCAN2_SCE;               /**< CAN2 SCE Interrupt                                           */
typedef Irq<67>  IrqOTG_FS;                 /**< USB OTG FS global Interrupt                                  */
#endif // STM32F10X_CL

#endif // NVIC_HPP_INCLUDED
