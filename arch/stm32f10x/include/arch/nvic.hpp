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
#include "../../../common/arm-cortex/vector_table.hpp"

namespace Irq
{
  typedef IrqChannel<0>   WWDG;                   /**< Window WatchDog Interrupt                   */
  typedef IrqChannel<1>   PVD;                    /**< PVD through EXTI Line detection Interrupt   */
  typedef IrqChannel<2>   TAMPER;                 /**< Tamper Interrupt                            */
  typedef IrqChannel<3>   RTC;                    /**< RTC global Interrupt                        */
  typedef IrqChannel<4>   FLASH;                  /**< FLASH global Interrupt                      */
  typedef IrqChannel<5>   RCC;                    /**< RCC global Interrupt                        */
  typedef IrqChannel<6>   EXTI0;                  /**< EXTI Line0 Interrupt                        */
  typedef IrqChannel<7>   EXTI1;                  /**< EXTI Line1 Interrupt                        */
  typedef IrqChannel<8>   EXTI2;                  /**< EXTI Line2 Interrupt                        */
  typedef IrqChannel<9>   EXTI3;                  /**< EXTI Line3 Interrupt                        */
  typedef IrqChannel<10>  EXTI4;                  /**< EXTI Line4 Interrupt                        */
  typedef IrqChannel<11>  DMA1_Channel1;          /**< DMA1 Channel 1 global Interrupt             */
  typedef IrqChannel<12>  DMA1_Channel2;          /**< DMA1 Channel 2 global Interrupt             */
  typedef IrqChannel<13>  DMA1_Channel3;          /**< DMA1 Channel 3 global Interrupt             */
  typedef IrqChannel<14>  DMA1_Channel4;          /**< DMA1 Channel 4 global Interrupt             */
  typedef IrqChannel<15>  DMA1_Channel5;          /**< DMA1 Channel 5 global Interrupt             */
  typedef IrqChannel<16>  DMA1_Channel6;          /**< DMA1 Channel 6 global Interrupt             */
  typedef IrqChannel<17>  DMA1_Channel7;          /**< DMA1 Channel 7 global Interrupt             */

#ifdef STM32F10X_LD
  typedef IrqChannel<18>  ADC1_2;                 /**< ADC1 and ADC2 global Interrupt                               */
  typedef IrqChannel<19>  USB_HP_CAN1_TX;         /**< USB Device High Priority or CAN1 TX Interrupts               */
  typedef IrqChannel<20>  USB_LP_CAN1_RX0;        /**< USB Device Low Priority or CAN1 RX0 Interrupts               */
  typedef IrqChannel<21>  CAN1_RX1;               /**< CAN1 RX1 Interrupt                                           */
  typedef IrqChannel<22>  CAN1_SCE;               /**< CAN1 SCE Interrupt                                           */
  typedef IrqChannel<23>  EXTI9_5;                /**< External Line[9:5] Interrupts                                */
  typedef IrqChannel<24>  TIM1_BRK;               /**< TIM1 Break Interrupt                                         */
  typedef IrqChannel<25>  TIM1_UP;                /**< TIM1 Update Interrupt                                        */
  typedef IrqChannel<26>  TIM1_TRG_COM;           /**< TIM1 Trigger and Commutation Interrupt                       */
  typedef IrqChannel<27>  TIM1_CC;                /**< TIM1 Capture Compare Interrupt                               */
  typedef IrqChannel<28>  TIM2;                   /**< TIM2 global Interrupt                                        */
  typedef IrqChannel<29>  TIM3;                   /**< TIM3 global Interrupt                                        */
  typedef IrqChannel<31>  I2C1_EV;                /**< I2C1 Event Interrupt                                         */
  typedef IrqChannel<32>  I2C1_ER;                /**< I2C1 Error Interrupt                                         */
  typedef IrqChannel<35>  SPI1;                   /**< SPI1 global Interrupt                                        */
  typedef IrqChannel<37>  USART1;                 /**< USART1 global Interrupt                                      */
  typedef IrqChannel<38>  USART2;                 /**< USART2 global Interrupt                                      */
  typedef IrqChannel<40>  EXTI15_10;              /**< External Line[15:10] Interrupts                              */
  typedef IrqChannel<41>  RTCAlarm;               /**< RTC Alarm through EXTI Line Interrupt                        */
  typedef IrqChannel<42>  USBWakeUp;              /**< USB Device WakeUp from suspend through EXTI Line Interrupt   */
#define NVIC_NUMOF_INTERRUPTS  43
#endif // STM32F10X_LD

#ifdef STM32F10X_LD_VL
  typedef IrqChannel<18>  ADC1;                   /**< ADC1 global Interrupt                              */
  typedef IrqChannel<23>  EXTI9_5;                /**< External Line[9:5] Interrupts                      */
  typedef IrqChannel<24>  TIM1_BRK_TIM15;         /**< TIM1 Break and TIM15 Interrupts                    */
  typedef IrqChannel<25>  TIM1_UP_TIM16;          /**< TIM1 Update and TIM16 Interrupts                   */
  typedef IrqChannel<26>  TIM1_TRG_COM_TIM17;     /**< TIM1 Trigger and Commutation and TIM17 Interrupt   */
  typedef IrqChannel<27>  TIM1_CC;                /**< TIM1 Capture Compare Interrupt                     */
  typedef IrqChannel<28>  TIM2;                   /**< TIM2 global Interrupt                              */
  typedef IrqChannel<29>  TIM3;                   /**< TIM3 global Interrupt                              */
  typedef IrqChannel<31>  I2C1_EV;                /**< I2C1 Event Interrupt                               */
  typedef IrqChannel<32>  I2C1_ER;                /**< I2C1 Error Interrupt                               */
  typedef IrqChannel<35>  SPI1;                   /**< SPI1 global Interrupt                              */
  typedef IrqChannel<37>  USART1;                 /**< USART1 global Interrupt                            */
  typedef IrqChannel<38>  USART2;                 /**< USART2 global Interrupt                            */
  typedef IrqChannel<40>  EXTI15_10;              /**< External Line[15:10] Interrupts                    */
  typedef IrqChannel<41>  RTCAlarm;               /**< RTC Alarm through EXTI Line Interrupt              */
  typedef IrqChannel<42>  CEC;                    /**< HDMI-CEC Interrupt                                 */
  typedef IrqChannel<54>  TIM6_DAC;               /**< TIM6 and DAC underrun Interrupt                    */
  typedef IrqChannel<55>  TIM7;                   /**< TIM7 Interrupt                                     */
#define NVIC_NUMOF_INTERRUPTS  56
#endif // STM32F10X_LD_VL

#ifdef STM32F10X_MD
  typedef IrqChannel<18>  ADC1_2;                 /**< ADC1 and ADC2 global Interrupt                               */
  typedef IrqChannel<19>  USB_HP_CAN1_TX;         /**< USB Device High Priority or CAN1 TX Interrupts               */
  typedef IrqChannel<20>  USB_LP_CAN1_RX0;        /**< USB Device Low Priority or CAN1 RX0 Interrupts               */
  typedef IrqChannel<21>  CAN1_RX1;               /**< CAN1 RX1 Interrupt                                           */
  typedef IrqChannel<22>  CAN1_SCE;               /**< CAN1 SCE Interrupt                                           */
  typedef IrqChannel<23>  EXTI9_5;                /**< External Line[9:5] Interrupts                                */
  typedef IrqChannel<24>  TIM1_BRK;               /**< TIM1 Break Interrupt                                         */
  typedef IrqChannel<25>  TIM1_UP;                /**< TIM1 Update Interrupt                                        */
  typedef IrqChannel<26>  TIM1_TRG_COM;           /**< TIM1 Trigger and Commutation Interrupt                       */
  typedef IrqChannel<27>  TIM1_CC;                /**< TIM1 Capture Compare Interrupt                               */
  typedef IrqChannel<28>  TIM2;                   /**< TIM2 global Interrupt                                        */
  typedef IrqChannel<29>  TIM3;                   /**< TIM3 global Interrupt                                        */
  typedef IrqChannel<30>  TIM4;                   /**< TIM4 global Interrupt                                        */
  typedef IrqChannel<31>  I2C1_EV;                /**< I2C1 Event Interrupt                                         */
  typedef IrqChannel<32>  I2C1_ER;                /**< I2C1 Error Interrupt                                         */
  typedef IrqChannel<33>  I2C2_EV;                /**< I2C2 Event Interrupt                                         */
  typedef IrqChannel<34>  I2C2_ER;                /**< I2C2 Error Interrupt                                         */
  typedef IrqChannel<35>  SPI1;                   /**< SPI1 global Interrupt                                        */
  typedef IrqChannel<36>  SPI2;                   /**< SPI2 global Interrupt                                        */
  typedef IrqChannel<37>  USART1;                 /**< USART1 global Interrupt                                      */
  typedef IrqChannel<38>  USART2;                 /**< USART2 global Interrupt                                      */
  typedef IrqChannel<39>  USART3;                 /**< USART3 global Interrupt                                      */
  typedef IrqChannel<40>  EXTI15_10;              /**< External Line[15:10] Interrupts                              */
  typedef IrqChannel<41>  RTCAlarm;               /**< RTC Alarm through EXTI Line Interrupt                        */
  typedef IrqChannel<42>  USBWakeUp;              /**< USB Device WakeUp from suspend through EXTI Line Interrupt   */
#define NVIC_NUMOF_INTERRUPTS  43
#endif // STM32F10X_MD

#ifdef STM32F10X_MD_VL
  typedef IrqChannel<18>  ADC1;                   /**< ADC1 global Interrupt                              */
  typedef IrqChannel<23>  EXTI9_5;                /**< External Line[9:5] Interrupts                      */
  typedef IrqChannel<24>  TIM1_BRK_TIM15;         /**< TIM1 Break and TIM15 Interrupts                    */
  typedef IrqChannel<25>  TIM1_UP_TIM16;          /**< TIM1 Update and TIM16 Interrupts                   */
  typedef IrqChannel<26>  TIM1_TRG_COM_TIM17;     /**< TIM1 Trigger and Commutation and TIM17 Interrupt   */
  typedef IrqChannel<27>  TIM1_CC;                /**< TIM1 Capture Compare Interrupt                     */
  typedef IrqChannel<28>  TIM2;                   /**< TIM2 global Interrupt                              */
  typedef IrqChannel<29>  TIM3;                   /**< TIM3 global Interrupt                              */
  typedef IrqChannel<30>  TIM4;                   /**< TIM4 global Interrupt                              */
  typedef IrqChannel<31>  I2C1_EV;                /**< I2C1 Event Interrupt                               */
  typedef IrqChannel<32>  I2C1_ER;                /**< I2C1 Error Interrupt                               */
  typedef IrqChannel<33>  I2C2_EV;                /**< I2C2 Event Interrupt                               */
  typedef IrqChannel<34>  I2C2_ER;                /**< I2C2 Error Interrupt                               */
  typedef IrqChannel<35>  SPI1;                   /**< SPI1 global Interrupt                              */
  typedef IrqChannel<36>  SPI2;                   /**< SPI2 global Interrupt                              */
  typedef IrqChannel<37>  USART1;                 /**< USART1 global Interrupt                            */
  typedef IrqChannel<38>  USART2;                 /**< USART2 global Interrupt                            */
  typedef IrqChannel<39>  USART3;                 /**< USART3 global Interrupt                            */
  typedef IrqChannel<40>  EXTI15_10;              /**< External Line[15:10] Interrupts                    */
  typedef IrqChannel<41>  RTCAlarm;               /**< RTC Alarm through EXTI Line Interrupt              */
  typedef IrqChannel<42>  CEC;                    /**< HDMI-CEC Interrupt                                 */
  typedef IrqChannel<54>  TIM6_DAC;               /**< TIM6 and DAC underrun Interrupt                    */
  typedef IrqChannel<55>  TIM7;                   /**< TIM7 Interrupt                                     */
#define NVIC_NUMOF_INTERRUPTS  56
#endif // STM32F10X_MD_VL

#ifdef STM32F10X_HD
  typedef IrqChannel<18>  ADC1_2;                 /**< ADC1 and ADC2 global Interrupt                               */
  typedef IrqChannel<19>  USB_HP_CAN1_TX;         /**< USB Device High Priority or CAN1 TX Interrupts               */
  typedef IrqChannel<20>  USB_LP_CAN1_RX0;        /**< USB Device Low Priority or CAN1 RX0 Interrupts               */
  typedef IrqChannel<21>  CAN1_RX1;               /**< CAN1 RX1 Interrupt                                           */
  typedef IrqChannel<22>  CAN1_SCE;               /**< CAN1 SCE Interrupt                                           */
  typedef IrqChannel<23>  EXTI9_5;                /**< External Line[9:5] Interrupts                                */
  typedef IrqChannel<24>  TIM1_BRK;               /**< TIM1 Break Interrupt                                         */
  typedef IrqChannel<25>  TIM1_UP;                /**< TIM1 Update Interrupt                                        */
  typedef IrqChannel<26>  TIM1_TRG_COM;           /**< TIM1 Trigger and Commutation Interrupt                       */
  typedef IrqChannel<27>  TIM1_CC;                /**< TIM1 Capture Compare Interrupt                               */
  typedef IrqChannel<28>  TIM2;                   /**< TIM2 global Interrupt                                        */
  typedef IrqChannel<29>  TIM3;                   /**< TIM3 global Interrupt                                        */
  typedef IrqChannel<30>  TIM4;                   /**< TIM4 global Interrupt                                        */
  typedef IrqChannel<31>  I2C1_EV;                /**< I2C1 Event Interrupt                                         */
  typedef IrqChannel<32>  I2C1_ER;                /**< I2C1 Error Interrupt                                         */
  typedef IrqChannel<33>  I2C2_EV;                /**< I2C2 Event Interrupt                                         */
  typedef IrqChannel<34>  I2C2_ER;                /**< I2C2 Error Interrupt                                         */
  typedef IrqChannel<35>  SPI1;                   /**< SPI1 global Interrupt                                        */
  typedef IrqChannel<36>  SPI2;                   /**< SPI2 global Interrupt                                        */
  typedef IrqChannel<37>  USART1;                 /**< USART1 global Interrupt                                      */
  typedef IrqChannel<38>  USART2;                 /**< USART2 global Interrupt                                      */
  typedef IrqChannel<39>  USART3;                 /**< USART3 global Interrupt                                      */
  typedef IrqChannel<40>  EXTI15_10;              /**< External Line[15:10] Interrupts                              */
  typedef IrqChannel<41>  RTCAlarm;               /**< RTC Alarm through EXTI Line Interrupt                        */
  typedef IrqChannel<42>  USBWakeUp;              /**< USB Device WakeUp from suspend through EXTI Line Interrupt   */
  typedef IrqChannel<43>  TIM8_BRK;               /**< TIM8 Break Interrupt                                         */
  typedef IrqChannel<44>  TIM8_UP;                /**< TIM8 Update Interrupt                                        */
  typedef IrqChannel<45>  TIM8_TRG_COM;           /**< TIM8 Trigger and Commutation Interrupt                       */
  typedef IrqChannel<46>  TIM8_CC;                /**< TIM8 Capture Compare Interrupt                               */
  typedef IrqChannel<47>  ADC3;                   /**< ADC3 global Interrupt                                        */
  typedef IrqChannel<48>  FSMC;                   /**< FSMC global Interrupt                                        */
  typedef IrqChannel<49>  SDIO;                   /**< SDIO global Interrupt                                        */
  typedef IrqChannel<50>  TIM5;                   /**< TIM5 global Interrupt                                        */
  typedef IrqChannel<51>  SPI3;                   /**< SPI3 global Interrupt                                        */
  typedef IrqChannel<52>  UART4;                  /**< UART4 global Interrupt                                       */
  typedef IrqChannel<53>  UART5;                  /**< UART5 global Interrupt                                       */
  typedef IrqChannel<54>  TIM6;                   /**< TIM6 global Interrupt                                        */
  typedef IrqChannel<55>  TIM7;                   /**< TIM7 global Interrupt                                        */
  typedef IrqChannel<56>  DMA2_Channel1;          /**< DMA2 Channel 1 global Interrupt                              */
  typedef IrqChannel<57>  DMA2_Channel2;          /**< DMA2 Channel 2 global Interrupt                              */
  typedef IrqChannel<58>  DMA2_Channel3;          /**< DMA2 Channel 3 global Interrupt                              */
  typedef IrqChannel<59>  DMA2_Channel4_5;        /**< DMA2 Channel 4 and Channel 5 global Interrupt                */
#define NVIC_NUMOF_INTERRUPTS  60
#endif // STM32F10X_HD

#ifdef STM32F10X_XL
  typedef IrqChannel<18>  ADC1_2;                 /**< ADC1 and ADC2 global Interrupt                                      */
  typedef IrqChannel<19>  USB_HP_CAN1_TX;         /**< USB Device High Priority or CAN1 TX Interrupts                      */
  typedef IrqChannel<20>  USB_LP_CAN1_RX0;        /**< USB Device Low Priority or CAN1 RX0 Interrupts                      */
  typedef IrqChannel<21>  CAN1_RX1;               /**< CAN1 RX1 Interrupt                                                  */
  typedef IrqChannel<22>  CAN1_SCE;               /**< CAN1 SCE Interrupt                                                  */
  typedef IrqChannel<23>  EXTI9_5;                /**< External Line[9:5] Interrupts                                       */
  typedef IrqChannel<24>  TIM1_BRK_TIM9;          /**< TIM1 Break Interrupt and TIM9 global Interrupt                      */
  typedef IrqChannel<25>  TIM1_UP_TIM10;          /**< TIM1 Update Interrupt and TIM10 global Interrupt                    */
  typedef IrqChannel<26>  TIM1_TRG_COM_TIM11;     /**< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt   */
  typedef IrqChannel<27>  TIM1_CC;                /**< TIM1 Capture Compare Interrupt                                      */
  typedef IrqChannel<28>  TIM2;                   /**< TIM2 global Interrupt                                               */
  typedef IrqChannel<29>  TIM3;                   /**< TIM3 global Interrupt                                               */
  typedef IrqChannel<30>  TIM4;                   /**< TIM4 global Interrupt                                               */
  typedef IrqChannel<31>  I2C1_EV;                /**< I2C1 Event Interrupt                                                */
  typedef IrqChannel<32>  I2C1_ER;                /**< I2C1 Error Interrupt                                                */
  typedef IrqChannel<33>  I2C2_EV;                /**< I2C2 Event Interrupt                                                */
  typedef IrqChannel<34>  I2C2_ER;                /**< I2C2 Error Interrupt                                                */
  typedef IrqChannel<35>  SPI1;                   /**< SPI1 global Interrupt                                               */
  typedef IrqChannel<36>  SPI2;                   /**< SPI2 global Interrupt                                               */
  typedef IrqChannel<37>  USART1;                 /**< USART1 global Interrupt                                             */
  typedef IrqChannel<38>  USART2;                 /**< USART2 global Interrupt                                             */
  typedef IrqChannel<39>  USART3;                 /**< USART3 global Interrupt                                             */
  typedef IrqChannel<40>  EXTI15_10;              /**< External Line[15:10] Interrupts                                     */
  typedef IrqChannel<41>  RTCAlarm;               /**< RTC Alarm through EXTI Line Interrupt                               */
  typedef IrqChannel<42>  USBWakeUp;              /**< USB Device WakeUp from suspend through EXTI Line Interrupt          */
  typedef IrqChannel<43>  TIM8_BRK_TIM12;         /**< TIM8 Break Interrupt and TIM12 global Interrupt                     */
  typedef IrqChannel<44>  TIM8_UP_TIM13;          /**< TIM8 Update Interrupt and TIM13 global Interrupt                    */
  typedef IrqChannel<45>  TIM8_TRG_COM_TIM14;     /**< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt   */
  typedef IrqChannel<46>  TIM8_CC;                /**< TIM8 Capture Compare Interrupt                                      */
  typedef IrqChannel<47>  ADC3;                   /**< ADC3 global Interrupt                                               */
  typedef IrqChannel<48>  FSMC;                   /**< FSMC global Interrupt                                               */
  typedef IrqChannel<49>  SDIO;                   /**< SDIO global Interrupt                                               */
  typedef IrqChannel<50>  TIM5;                   /**< TIM5 global Interrupt                                               */
  typedef IrqChannel<51>  SPI3;                   /**< SPI3 global Interrupt                                               */
  typedef IrqChannel<52>  UART4;                  /**< UART4 global Interrupt                                              */
  typedef IrqChannel<53>  UART5;                  /**< UART5 global Interrupt                                              */
  typedef IrqChannel<54>  TIM6;                   /**< TIM6 global Interrupt                                               */
  typedef IrqChannel<55>  TIM7;                   /**< TIM7 global Interrupt                                               */
  typedef IrqChannel<56>  DMA2_Channel1;          /**< DMA2 Channel 1 global Interrupt                                     */
  typedef IrqChannel<57>  DMA2_Channel2;          /**< DMA2 Channel 2 global Interrupt                                     */
  typedef IrqChannel<58>  DMA2_Channel3;          /**< DMA2 Channel 3 global Interrupt                                     */
  typedef IrqChannel<59>  DMA2_Channel4_5;        /**< DMA2 Channel 4 and Channel 5 global Interrupt                       */
#define NVIC_NUMOF_INTERRUPTS  60
#endif // STM32F10X_XL

#ifdef STM32F10X_CL
  typedef IrqChannel<18>  ADC1_2;                 /**< ADC1 and ADC2 global Interrupt                               */
  typedef IrqChannel<19>  CAN1_TX;                /**< USB Device High Priority or CAN1 TX Interrupts               */
  typedef IrqChannel<20>  CAN1_RX0;               /**< USB Device Low Priority or CAN1 RX0 Interrupts               */
  typedef IrqChannel<21>  CAN1_RX1;               /**< CAN1 RX1 Interrupt                                           */
  typedef IrqChannel<22>  CAN1_SCE;               /**< CAN1 SCE Interrupt                                           */
  typedef IrqChannel<23>  EXTI9_5;                /**< External Line[9:5] Interrupts                                */
  typedef IrqChannel<24>  TIM1_BRK;               /**< TIM1 Break Interrupt                                         */
  typedef IrqChannel<25>  TIM1_UP;                /**< TIM1 Update Interrupt                                        */
  typedef IrqChannel<26>  TIM1_TRG_COM;           /**< TIM1 Trigger and Commutation Interrupt                       */
  typedef IrqChannel<27>  TIM1_CC;                /**< TIM1 Capture Compare Interrupt                               */
  typedef IrqChannel<28>  TIM2;                   /**< TIM2 global Interrupt                                        */
  typedef IrqChannel<29>  TIM3;                   /**< TIM3 global Interrupt                                        */
  typedef IrqChannel<30>  TIM4;                   /**< TIM4 global Interrupt                                        */
  typedef IrqChannel<31>  I2C1_EV;                /**< I2C1 Event Interrupt                                         */
  typedef IrqChannel<32>  I2C1_ER;                /**< I2C1 Error Interrupt                                         */
  typedef IrqChannel<33>  I2C2_EV;                /**< I2C2 Event Interrupt                                         */
  typedef IrqChannel<34>  I2C2_ER;                /**< I2C2 Error Interrupt                                         */
  typedef IrqChannel<35>  SPI1;                   /**< SPI1 global Interrupt                                        */
  typedef IrqChannel<36>  SPI2;                   /**< SPI2 global Interrupt                                        */
  typedef IrqChannel<37>  USART1;                 /**< USART1 global Interrupt                                      */
  typedef IrqChannel<38>  USART2;                 /**< USART2 global Interrupt                                      */
  typedef IrqChannel<39>  USART3;                 /**< USART3 global Interrupt                                      */
  typedef IrqChannel<40>  EXTI15_10;              /**< External Line[15:10] Interrupts                              */
  typedef IrqChannel<41>  RTCAlarm;               /**< RTC Alarm through EXTI Line Interrupt                        */
  typedef IrqChannel<42>  OTG_FS_WKUP;            /**< USB OTG FS WakeUp from suspend through EXTI Line Interrupt   */
  typedef IrqChannel<50>  TIM5;                   /**< TIM5 global Interrupt                                        */
  typedef IrqChannel<51>  SPI3;                   /**< SPI3 global Interrupt                                        */
  typedef IrqChannel<52>  UART4;                  /**< UART4 global Interrupt                                       */
  typedef IrqChannel<53>  UART5;                  /**< UART5 global Interrupt                                       */
  typedef IrqChannel<54>  TIM6;                   /**< TIM6 global Interrupt                                        */
  typedef IrqChannel<55>  TIM7;                   /**< TIM7 global Interrupt                                        */
  typedef IrqChannel<56>  DMA2_Channel1;          /**< DMA2 Channel 1 global Interrupt                              */
  typedef IrqChannel<57>  DMA2_Channel2;          /**< DMA2 Channel 2 global Interrupt                              */
  typedef IrqChannel<58>  DMA2_Channel3;          /**< DMA2 Channel 3 global Interrupt                              */
  typedef IrqChannel<59>  DMA2_Channel4;          /**< DMA2 Channel 4 global Interrupt                              */
  typedef IrqChannel<60>  DMA2_Channel5;          /**< DMA2 Channel 5 global Interrupt                              */
  typedef IrqChannel<61>  ETH;                    /**< Ethernet global Interrupt                                    */
  typedef IrqChannel<62>  ETH_WKUP;               /**< Ethernet Wakeup through EXTI line Interrupt                  */
  typedef IrqChannel<63>  CAN2_TX;                /**< CAN2 TX Interrupt                                            */
  typedef IrqChannel<64>  CAN2_RX0;               /**< CAN2 RX0 Interrupt                                           */
  typedef IrqChannel<65>  CAN2_RX1;               /**< CAN2 RX1 Interrupt                                           */
  typedef IrqChannel<66>  CAN2_SCE;               /**< CAN2 SCE Interrupt                                           */
  typedef IrqChannel<67>  OTG_FS;                 /**< USB OTG FS global Interrupt                                  */
#define NVIC_NUMOF_INTERRUPTS  68
#endif // STM32F10X_CL
} // namespace Irq


/**
 * VectorTable: Provides a static vector table.
 *
 * Instantiate this class somewhere in your starup code to puts the
 * table to the ".isr_vector" section.
 */
template<const uint32_t *stack_top>
struct VectorTable : VectorTableBuilder<NVIC_NUMOF_INTERRUPTS, stack_top>
{ };

#undef NVIC_NUMOF_INTERRUPTS

#endif // NVIC_HPP_INCLUDED
