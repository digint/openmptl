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
  typedef IrqChannel< 0>  WWDG;                /**< Window Watchdog interrupt                                                */
  typedef IrqChannel< 1>  PVD;                 /**< PVD through EXTI line detection interrupt                                */
  typedef IrqChannel< 2>  TAMP_STAMP;          /**< Tamper and TimeStamp interrupts through the EXTI line                    */
  typedef IrqChannel< 3>  RTC_WKUP;            /**< RTC Wakeup interrupt through the EXTI line                               */
  typedef IrqChannel< 4>  FLASH;               /**< Flash global interrupt                                                   */
  typedef IrqChannel< 5>  RCC;                 /**< RCC global interrupt                                                     */
  typedef IrqChannel< 6>  EXTI0;               /**< EXTI Line0 interrupt                                                     */
  typedef IrqChannel< 7>  EXTI1;               /**< EXTI Line1 interrupt                                                     */
  typedef IrqChannel< 8>  EXTI2;               /**< EXTI Line2 interrupt                                                     */
  typedef IrqChannel< 9>  EXTI3;               /**< EXTI Line3 interrupt                                                     */
  typedef IrqChannel<10>  EXTI4;               /**< EXTI Line4 interrupt                                                     */
  typedef IrqChannel<11>  DMA1_Stream0;        /**< DMA1 Stream0 global interrupt                                            */
  typedef IrqChannel<12>  DMA1_Stream1;        /**< DMA1 Stream1 global interrupt                                            */
  typedef IrqChannel<13>  DMA1_Stream2;        /**< DMA1 Stream2 global interrupt                                            */
  typedef IrqChannel<14>  DMA1_Stream3;        /**< DMA1 Stream3 global interrupt                                            */
  typedef IrqChannel<15>  DMA1_Stream4;        /**< DMA1 Stream4 global interrupt                                            */
  typedef IrqChannel<16>  DMA1_Stream5;        /**< DMA1 Stream5 global interrupt                                            */
  typedef IrqChannel<17>  DMA1_Stream6;        /**< DMA1 Stream6 global interrupt                                            */
  typedef IrqChannel<18>  ADC;                 /**< ADC1 global interrupt / ADC2 global interrupts / ADC3 global interrupts  */
  typedef IrqChannel<19>  CAN1_TX;             /**< CAN1 TX interrupts                                                       */
  typedef IrqChannel<20>  CAN1_RX0;            /**< CAN1 RX0 interrupts                                                      */
  typedef IrqChannel<21>  CAN1_RX1;            /**< CAN1 RX1 interrupts                                                      */
  typedef IrqChannel<22>  CAN1_SCE;            /**< CAN1 SCE interrupt                                                       */
  typedef IrqChannel<23>  EXTI9_5;             /**< EXTI Line[9:5] interrupts                                                */
  typedef IrqChannel<24>  TIM1_BRK_TIM9;       /**< TIM1 Break interrupt and TIM9 global interrupt                           */
  typedef IrqChannel<25>  TIM1_UP_TIM10;       /**< TIM1 Update interrupt and TIM10 global interrupt                         */
  typedef IrqChannel<26>  TIM1_TRG_COM_TIM11;  /**< TIM1 Trigger and Commutation interrupts and TIM11 global interrupt       */
  typedef IrqChannel<27>  TIM1_CC;             /**< TIM1 Capture Compare interrupt                                           */
  typedef IrqChannel<28>  TIM2;                /**< TIM2 global interrupt                                                    */
  typedef IrqChannel<29>  TIM3;                /**< TIM3 global interrupt                                                    */
  typedef IrqChannel<30>  TIM4;                /**< TIM4 global interrupt                                                    */
  typedef IrqChannel<31>  I2C1_EV;             /**< I2C1 event interrupt                                                     */
  typedef IrqChannel<32>  I2C1_ER;             /**< I2C1 error interrupt                                                     */
  typedef IrqChannel<33>  I2C2_EV;             /**< I2C2 event interrupt                                                     */
  typedef IrqChannel<34>  I2C2_ER;             /**< I2C2 error interrupt                                                     */
  typedef IrqChannel<35>  SPI1;                /**< SPI1 global interrupt                                                    */
  typedef IrqChannel<36>  SPI2;                /**< SPI2 global interrupt                                                    */
  typedef IrqChannel<37>  USART1;              /**< USART1 global interrupt                                                  */
  typedef IrqChannel<38>  USART2;              /**< USART2 global interrupt                                                  */
  typedef IrqChannel<39>  USART3;              /**< USART3 global interrupt                                                  */
  typedef IrqChannel<40>  EXTI15_10;           /**< EXTI Line[15:10] interrupts                                              */
  typedef IrqChannel<41>  RTC_Alarm;           /**< RTC Alarms (A and B) through EXTI line interrupt                         */
  typedef IrqChannel<42>  OTG_FS_WKUP;         /**< USB On-The-Go FS Wakeup through EXTI line interrupt                      */
  typedef IrqChannel<43>  TIM8_BRK_TIM12;      /**< TIM8 Break interrupt and TIM12 global interrupt                          */
  typedef IrqChannel<44>  TIM8_UP_TIM13;       /**< TIM8 Update interrupt and TIM13 global interrupt                         */
  typedef IrqChannel<45>  TIM8_TRG_COM_TIM14;  /**< TIM8 Trigger and Commutation interrupts and TIM14 global interrupt       */
  typedef IrqChannel<46>  TIM8_CC;             /**< TIM8 Capture Compare interrupt                                           */
  typedef IrqChannel<47>  DMA1_Stream7;        /**< DMA1 Stream7 global interrupt                                            */
  typedef IrqChannel<48>  FSMC;                /**< FSMC global interrupt                                                    */
  typedef IrqChannel<49>  SDIO;                /**< SDIO global interrupt                                                    */
  typedef IrqChannel<50>  TIM5;                /**< TIM5 global interrupt                                                    */
  typedef IrqChannel<51>  SPI3;                /**< SPI3 global interrupt                                                    */
  typedef IrqChannel<52>  UART4;               /**< UART4 global interrupt                                                   */
  typedef IrqChannel<53>  UART5;               /**< UART5 global interrupt                                                   */
  typedef IrqChannel<54>  TIM6_DAC;            /**< TIM6 global interrupt, DAC1 and DAC2 underrun error interrupt            */
  typedef IrqChannel<55>  TIM7;                /**< TIM7 global interrupt                                                    */
  typedef IrqChannel<56>  DMA2_Stream0;        /**< DMA2 Stream0 global interrupt                                            */
  typedef IrqChannel<57>  DMA2_Stream1;        /**< DMA2 Stream1 global interrupt                                            */
  typedef IrqChannel<58>  DMA2_Stream2;        /**< DMA2 Stream2 global interrupt                                            */
  typedef IrqChannel<59>  DMA2_Stream3;        /**< DMA2 Stream3 global interrupt                                            */
  typedef IrqChannel<60>  DMA2_Stream4;        /**< DMA2 Stream4 global interrupt                                            */
  typedef IrqChannel<61>  ETH;                 /**< Ethernet global interrupt                                                */
  typedef IrqChannel<62>  ETH_WKUP;            /**< Ethernet Wakeup through EXTI line interrupt                              */
  typedef IrqChannel<63>  CAN2_TX;             /**< CAN2 TX interrupts                                                       */
  typedef IrqChannel<64>  CAN2_RX0;            /**< CAN2 RX0 interrupts                                                      */
  typedef IrqChannel<65>  CAN2_RX1;            /**< CAN2 RX1 interrupts                                                      */
  typedef IrqChannel<66>  CAN2_SCE;            /**< CAN2 SCE interrupt                                                       */
  typedef IrqChannel<67>  OTG_FS;              /**< USB On The Go FS global interrupt                                        */
  typedef IrqChannel<68>  DMA2_Stream5;        /**< DMA2 Stream5 global interrupt                                            */
  typedef IrqChannel<69>  DMA2_Stream6;        /**< DMA2 Stream6 global interrupt                                            */
  typedef IrqChannel<70>  DMA2_Stream7;        /**< DMA2 Stream7 global interrupt                                            */
  typedef IrqChannel<71>  USART6;              /**< USART6 global interrupt                                                  */
  typedef IrqChannel<72>  I2C3_EV;             /**< I2C3 event interrupt                                                     */
  typedef IrqChannel<73>  I2C3_ER;             /**< I2C3 error interrupt                                                     */
  typedef IrqChannel<74>  OTG_HS_EP1_OUT;      /**< USB On The Go HS End Point 1 Out global interrupt                        */
  typedef IrqChannel<75>  OTG_HS_EP1_IN;       /**< USB On The Go HS End Point 1 In global interrupt                         */
  typedef IrqChannel<76>  OTG_HS_WKUP;         /**< USB On The Go HS Wakeup through EXTI interrupt                           */
  typedef IrqChannel<77>  OTG_HS;              /**< USB On The Go HS global interrupt                                        */
  typedef IrqChannel<78>  DCMI;                /**< DCMI global interrupt                                                    */
  typedef IrqChannel<79>  CRYP;                /**< CRYP crypto global interrupt                                             */
  typedef IrqChannel<80>  HASH_RNG;            /**< Hash and Rng global interrupt                                            */
  typedef IrqChannel<81>  FPU;                 /**<  FPU global interrupt                                                    */
}


/**
 * VectorTable: Provides a static vector table.
 *
 * Instantiate this class somewhere in your starup code to puts the
 * table to the ".isr_vector" section.
 */
template<const uint32_t *stack_top>
struct VectorTable : VectorTableBuilder<82, stack_top>
{ };


#endif // NVIC_HPP_INCLUDED
