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

#ifndef ARCH_NVIC_HPP_INCLUDED
#define ARCH_NVIC_HPP_INCLUDED

#include "../../../../common/nvic.hpp"

namespace mptl { namespace irq {

using wwdg                   = irq_channel<0>;   /**< Window WatchDog Interrupt                   */
using pvd                    = irq_channel<1>;   /**< PVD through EXTI Line detection Interrupt   */
using tamper                 = irq_channel<2>;   /**< Tamper Interrupt                            */
using rtc                    = irq_channel<3>;   /**< RTC global Interrupt                        */
using flash                  = irq_channel<4>;   /**< FLASH global Interrupt                      */
using rcc                    = irq_channel<5>;   /**< RCC global Interrupt                        */
using exti0                  = irq_channel<6>;   /**< EXTI Line0 Interrupt                        */
using exti1                  = irq_channel<7>;   /**< EXTI Line1 Interrupt                        */
using exti2                  = irq_channel<8>;   /**< EXTI Line2 Interrupt                        */
using exti3                  = irq_channel<9>;   /**< EXTI Line3 Interrupt                        */
using exti4                  = irq_channel<10>;  /**< EXTI Line4 Interrupt                        */
using dma1_channel1          = irq_channel<11>;  /**< DMA1 Channel 1 global Interrupt             */
using dma1_channel2          = irq_channel<12>;  /**< DMA1 Channel 2 global Interrupt             */
using dma1_channel3          = irq_channel<13>;  /**< DMA1 Channel 3 global Interrupt             */
using dma1_channel4          = irq_channel<14>;  /**< DMA1 Channel 4 global Interrupt             */
using dma1_channel5          = irq_channel<15>;  /**< DMA1 Channel 5 global Interrupt             */
using dma1_channel6          = irq_channel<16>;  /**< DMA1 Channel 6 global Interrupt             */
using dma1_channel7          = irq_channel<17>;  /**< DMA1 Channel 7 global Interrupt             */

#ifdef STM32F10X_LD
using adc1_2                 = irq_channel<18>;  /**< ADC1 and ADC2 global Interrupt                               */
using usb_hp_can1_tx         = irq_channel<19>;  /**< USB Device High Priority or CAN1 TX Interrupts               */
using usb_lp_can1_rx0        = irq_channel<20>;  /**< USB Device Low Priority or CAN1 RX0 Interrupts               */
using can1_rx1               = irq_channel<21>;  /**< CAN1 RX1 Interrupt                                           */
using can1_sce               = irq_channel<22>;  /**< CAN1 SCE Interrupt                                           */
using exti9_5                = irq_channel<23>;  /**< External Line[9:5] Interrupts                                */
using tim1_brk               = irq_channel<24>;  /**< TIM1 Break Interrupt                                         */
using tim1_up                = irq_channel<25>;  /**< TIM1 Update Interrupt                                        */
using tim1_trg_com           = irq_channel<26>;  /**< TIM1 Trigger and Commutation Interrupt                       */
using tim1_cc                = irq_channel<27>;  /**< TIM1 Capture Compare Interrupt                               */
using tim2                   = irq_channel<28>;  /**< TIM2 global Interrupt                                        */
using tim3                   = irq_channel<29>;  /**< TIM3 global Interrupt                                        */
using i2c1_ev                = irq_channel<31>;  /**< I2C1 Event Interrupt                                         */
using i2c1_er                = irq_channel<32>;  /**< I2C1 Error Interrupt                                         */
using spi1                   = irq_channel<35>;  /**< SPI1 global Interrupt                                        */
using usart1                 = irq_channel<37>;  /**< USART1 global Interrupt                                      */
using usart2                 = irq_channel<38>;  /**< USART2 global Interrupt                                      */
using exti15_10              = irq_channel<40>;  /**< External Line[15:10] Interrupts                              */
using rtc_alarm              = irq_channel<41>;  /**< RTC Alarm through EXTI Line Interrupt                        */
using usb_wakeup             = irq_channel<42>;  /**< USB Device WakeUp from suspend through EXTI Line Interrupt   */
static constexpr int numof_interrupt_channels = 43;
#endif // STM32F10X_LD

#ifdef STM32F10X_LD_VL
using adc1                   = irq_channel<18>;  /**< ADC1 global Interrupt                              */
using exti9_5                = irq_channel<23>;  /**< External Line[9:5] Interrupts                      */
using tim1_brk_tim15         = irq_channel<24>;  /**< TIM1 Break and TIM15 Interrupts                    */
using tim1_up_tim16          = irq_channel<25>;  /**< TIM1 Update and TIM16 Interrupts                   */
using tim1_trg_com_tim17     = irq_channel<26>;  /**< TIM1 Trigger and Commutation and TIM17 Interrupt   */
using tim1_cc                = irq_channel<27>;  /**< TIM1 Capture Compare Interrupt                     */
using tim2                   = irq_channel<28>;  /**< TIM2 global Interrupt                              */
using tim3                   = irq_channel<29>;  /**< TIM3 global Interrupt                              */
using i2c1_ev                = irq_channel<31>;  /**< I2C1 Event Interrupt                               */
using i2c1_er                = irq_channel<32>;  /**< I2C1 Error Interrupt                               */
using spi1                   = irq_channel<35>;  /**< SPI1 global Interrupt                              */
using usart1                 = irq_channel<37>;  /**< USART1 global Interrupt                            */
using usart2                 = irq_channel<38>;  /**< USART2 global Interrupt                            */
using exti15_10              = irq_channel<40>;  /**< External Line[15:10] Interrupts                    */
using rtc_alarm              = irq_channel<41>;  /**< RTC Alarm through EXTI Line Interrupt              */
using cec                    = irq_channel<42>;  /**< HDMI-CEC Interrupt                                 */
using tim6_dac               = irq_channel<54>;  /**< TIM6 and DAC underrun Interrupt                    */
using tim7                   = irq_channel<55>;  /**< TIM7 Interrupt                                     */
static constexpr int numof_interrupt_channels = 56;
#endif // STM32F10X_LD_VL

#ifdef STM32F10X_MD
using adc1_2                 = irq_channel<18>;  /**< ADC1 and ADC2 global Interrupt                               */
using usb_hp_can1_tx         = irq_channel<19>;  /**< USB Device High Priority or CAN1 TX Interrupts               */
using usb_lp_can1_rx0        = irq_channel<20>;  /**< USB Device Low Priority or CAN1 RX0 Interrupts               */
using can1_rx1               = irq_channel<21>;  /**< CAN1 RX1 Interrupt                                           */
using can1_sce               = irq_channel<22>;  /**< CAN1 SCE Interrupt                                           */
using exti9_5                = irq_channel<23>;  /**< External Line[9:5] Interrupts                                */
using tim1_brk               = irq_channel<24>;  /**< TIM1 Break Interrupt                                         */
using tim1_up                = irq_channel<25>;  /**< TIM1 Update Interrupt                                        */
using tim1_trg_com           = irq_channel<26>;  /**< TIM1 Trigger and Commutation Interrupt                       */
using tim1_cc                = irq_channel<27>;  /**< TIM1 Capture Compare Interrupt                               */
using tim2                   = irq_channel<28>;  /**< TIM2 global Interrupt                                        */
using tim3                   = irq_channel<29>;  /**< TIM3 global Interrupt                                        */
using tim4                   = irq_channel<30>;  /**< TIM4 global Interrupt                                        */
using i2c1_ev                = irq_channel<31>;  /**< I2C1 Event Interrupt                                         */
using i2c1_er                = irq_channel<32>;  /**< I2C1 Error Interrupt                                         */
using i2c2_ev                = irq_channel<33>;  /**< I2C2 Event Interrupt                                         */
using i2c2_er                = irq_channel<34>;  /**< I2C2 Error Interrupt                                         */
using spi1                   = irq_channel<35>;  /**< SPI1 global Interrupt                                        */
using spi2                   = irq_channel<36>;  /**< SPI2 global Interrupt                                        */
using usart1                 = irq_channel<37>;  /**< USART1 global Interrupt                                      */
using usart2                 = irq_channel<38>;  /**< USART2 global Interrupt                                      */
using usart3                 = irq_channel<39>;  /**< USART3 global Interrupt                                      */
using exti15_10              = irq_channel<40>;  /**< External Line[15:10] Interrupts                              */
using rtc_alarm              = irq_channel<41>;  /**< RTC Alarm through EXTI Line Interrupt                        */
using usb_wakeup             = irq_channel<42>;  /**< USB Device WakeUp from suspend through EXTI Line Interrupt   */
static constexpr int numof_interrupt_channels = 43;
#endif // STM32F10X_MD

#ifdef STM32F10X_MD_VL
using adc1                   = irq_channel<18>;  /**< ADC1 global Interrupt                              */
using exti9_5                = irq_channel<23>;  /**< External Line[9:5] Interrupts                      */
using tim1_brk_tim15         = irq_channel<24>;  /**< TIM1 Break and TIM15 Interrupts                    */
using tim1_up_tim16          = irq_channel<25>;  /**< TIM1 Update and TIM16 Interrupts                   */
using tim1_trg_com_tim17     = irq_channel<26>;  /**< TIM1 Trigger and Commutation and TIM17 Interrupt   */
using tim1_cc                = irq_channel<27>;  /**< TIM1 Capture Compare Interrupt                     */
using tim2                   = irq_channel<28>;  /**< TIM2 global Interrupt                              */
using tim3                   = irq_channel<29>;  /**< TIM3 global Interrupt                              */
using tim4                   = irq_channel<30>;  /**< TIM4 global Interrupt                              */
using i2c1_ev                = irq_channel<31>;  /**< I2C1 Event Interrupt                               */
using i2c1_er                = irq_channel<32>;  /**< I2C1 Error Interrupt                               */
using i2c2_ev                = irq_channel<33>;  /**< I2C2 Event Interrupt                               */
using i2c2_er                = irq_channel<34>;  /**< I2C2 Error Interrupt                               */
using spi1                   = irq_channel<35>;  /**< SPI1 global Interrupt                              */
using spi2                   = irq_channel<36>;  /**< SPI2 global Interrupt                              */
using usart1                 = irq_channel<37>;  /**< USART1 global Interrupt                            */
using usart2                 = irq_channel<38>;  /**< USART2 global Interrupt                            */
using usart3                 = irq_channel<39>;  /**< USART3 global Interrupt                            */
using exti15_10              = irq_channel<40>;  /**< External Line[15:10] Interrupts                    */
using rtc_alarm              = irq_channel<41>;  /**< RTC Alarm through EXTI Line Interrupt              */
using cec                    = irq_channel<42>;  /**< HDMI-CEC Interrupt                                 */
using tim6_dac               = irq_channel<54>;  /**< TIM6 and DAC underrun Interrupt                    */
using tim7                   = irq_channel<55>;  /**< TIM7 Interrupt                                     */
static constexpr int numof_interrupt_channels = 56;
#endif // STM32F10X_MD_VL

#ifdef STM32F10X_HD
using adc1_2                 = irq_channel<18>;  /**< ADC1 and ADC2 global Interrupt                               */
using usb_hp_can1_tx         = irq_channel<19>;  /**< USB Device High Priority or CAN1 TX Interrupts               */
using usb_lp_can1_rx0        = irq_channel<20>;  /**< USB Device Low Priority or CAN1 RX0 Interrupts               */
using can1_rx1               = irq_channel<21>;  /**< CAN1 RX1 Interrupt                                           */
using can1_sce               = irq_channel<22>;  /**< CAN1 SCE Interrupt                                           */
using exti9_5                = irq_channel<23>;  /**< External Line[9:5] Interrupts                                */
using tim1_brk               = irq_channel<24>;  /**< TIM1 Break Interrupt                                         */
using tim1_up                = irq_channel<25>;  /**< TIM1 Update Interrupt                                        */
using tim1_trg_com           = irq_channel<26>;  /**< TIM1 Trigger and Commutation Interrupt                       */
using tim1_cc                = irq_channel<27>;  /**< TIM1 Capture Compare Interrupt                               */
using tim2                   = irq_channel<28>;  /**< TIM2 global Interrupt                                        */
using tim3                   = irq_channel<29>;  /**< TIM3 global Interrupt                                        */
using tim4                   = irq_channel<30>;  /**< TIM4 global Interrupt                                        */
using i2c1_ev                = irq_channel<31>;  /**< I2C1 Event Interrupt                                         */
using i2c1_er                = irq_channel<32>;  /**< I2C1 Error Interrupt                                         */
using i2c2_ev                = irq_channel<33>;  /**< I2C2 Event Interrupt                                         */
using i2c2_er                = irq_channel<34>;  /**< I2C2 Error Interrupt                                         */
using spi1                   = irq_channel<35>;  /**< SPI1 global Interrupt                                        */
using spi2                   = irq_channel<36>;  /**< SPI2 global Interrupt                                        */
using usart1                 = irq_channel<37>;  /**< USART1 global Interrupt                                      */
using usart2                 = irq_channel<38>;  /**< USART2 global Interrupt                                      */
using usart3                 = irq_channel<39>;  /**< USART3 global Interrupt                                      */
using exti15_10              = irq_channel<40>;  /**< External Line[15:10] Interrupts                              */
using rtc_alarm              = irq_channel<41>;  /**< RTC Alarm through EXTI Line Interrupt                        */
using usb_wakeup             = irq_channel<42>;  /**< USB Device WakeUp from suspend through EXTI Line Interrupt   */
using tim8_brk               = irq_channel<43>;  /**< TIM8 Break Interrupt                                         */
using tim8_up                = irq_channel<44>;  /**< TIM8 Update Interrupt                                        */
using tim8_trg_com           = irq_channel<45>;  /**< TIM8 Trigger and Commutation Interrupt                       */
using tim8_cc                = irq_channel<46>;  /**< TIM8 Capture Compare Interrupt                               */
using adc3                   = irq_channel<47>;  /**< ADC3 global Interrupt                                        */
using fsmc                   = irq_channel<48>;  /**< FSMC global Interrupt                                        */
using sdio                   = irq_channel<49>;  /**< SDIO global Interrupt                                        */
using tim5                   = irq_channel<50>;  /**< TIM5 global Interrupt                                        */
using spi3                   = irq_channel<51>;  /**< SPI3 global Interrupt                                        */
using uart4                  = irq_channel<52>;  /**< UART4 global Interrupt                                       */
using uart5                  = irq_channel<53>;  /**< UART5 global Interrupt                                       */
using tim6                   = irq_channel<54>;  /**< TIM6 global Interrupt                                        */
using tim7                   = irq_channel<55>;  /**< TIM7 global Interrupt                                        */
using dma2_channel1          = irq_channel<56>;  /**< DMA2 Channel 1 global Interrupt                              */
using dma2_channel2          = irq_channel<57>;  /**< DMA2 Channel 2 global Interrupt                              */
using dma2_channel3          = irq_channel<58>;  /**< DMA2 Channel 3 global Interrupt                              */
using dma2_channel4_5        = irq_channel<59>;  /**< DMA2 Channel 4 and Channel 5 global Interrupt                */
static constexpr int numof_interrupt_channels = 60;
#endif // STM32F10X_HD

#ifdef STM32F10X_XL
using adc1_2                 = irq_channel<18>;  /**< ADC1 and ADC2 global Interrupt                                      */
using usb_hp_can1_tx         = irq_channel<19>;  /**< USB Device High Priority or CAN1 TX Interrupts                      */
using usb_lp_can1_rx0        = irq_channel<20>;  /**< USB Device Low Priority or CAN1 RX0 Interrupts                      */
using can1_rx1               = irq_channel<21>;  /**< CAN1 RX1 Interrupt                                                  */
using can1_sce               = irq_channel<22>;  /**< CAN1 SCE Interrupt                                                  */
using exti9_5                = irq_channel<23>;  /**< External Line[9:5] Interrupts                                       */
using tim1_brk_tim9          = irq_channel<24>;  /**< TIM1 Break Interrupt and TIM9 global Interrupt                      */
using tim1_up_tim10          = irq_channel<25>;  /**< TIM1 Update Interrupt and TIM10 global Interrupt                    */
using tim1_trg_com_tim11     = irq_channel<26>;  /**< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt   */
using tim1_cc                = irq_channel<27>;  /**< TIM1 Capture Compare Interrupt                                      */
using tim2                   = irq_channel<28>;  /**< TIM2 global Interrupt                                               */
using tim3                   = irq_channel<29>;  /**< TIM3 global Interrupt                                               */
using tim4                   = irq_channel<30>;  /**< TIM4 global Interrupt                                               */
using i2c1_ev                = irq_channel<31>;  /**< I2C1 Event Interrupt                                                */
using i2c1_er                = irq_channel<32>;  /**< I2C1 Error Interrupt                                                */
using i2c2_ev                = irq_channel<33>;  /**< I2C2 Event Interrupt                                                */
using i2c2_er                = irq_channel<34>;  /**< I2C2 Error Interrupt                                                */
using spi1                   = irq_channel<35>;  /**< SPI1 global Interrupt                                               */
using spi2                   = irq_channel<36>;  /**< SPI2 global Interrupt                                               */
using usart1                 = irq_channel<37>;  /**< USART1 global Interrupt                                             */
using usart2                 = irq_channel<38>;  /**< USART2 global Interrupt                                             */
using usart3                 = irq_channel<39>;  /**< USART3 global Interrupt                                             */
using exti15_10              = irq_channel<40>;  /**< External Line[15:10] Interrupts                                     */
using rtc_alarm              = irq_channel<41>;  /**< RTC Alarm through EXTI Line Interrupt                               */
using usb_wakeup             = irq_channel<42>;  /**< USB Device WakeUp from suspend through EXTI Line Interrupt          */
using tim8_brk_tim12         = irq_channel<43>;  /**< TIM8 Break Interrupt and TIM12 global Interrupt                     */
using tim8_up_tim13          = irq_channel<44>;  /**< TIM8 Update Interrupt and TIM13 global Interrupt                    */
using tim8_trg_com_tim14     = irq_channel<45>;  /**< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt   */
using tim8_cc                = irq_channel<46>;  /**< TIM8 Capture Compare Interrupt                                      */
using adc3                   = irq_channel<47>;  /**< ADC3 global Interrupt                                               */
using fsmc                   = irq_channel<48>;  /**< FSMC global Interrupt                                               */
using sdio                   = irq_channel<49>;  /**< SDIO global Interrupt                                               */
using tim5                   = irq_channel<50>;  /**< TIM5 global Interrupt                                               */
using spi3                   = irq_channel<51>;  /**< SPI3 global Interrupt                                               */
using uart4                  = irq_channel<52>;  /**< UART4 global Interrupt                                              */
using uart5                  = irq_channel<53>;  /**< UART5 global Interrupt                                              */
using tim6                   = irq_channel<54>;  /**< TIM6 global Interrupt                                               */
using tim7                   = irq_channel<55>;  /**< TIM7 global Interrupt                                               */
using dma2_channel1          = irq_channel<56>;  /**< DMA2 Channel 1 global Interrupt                                     */
using dma2_channel2          = irq_channel<57>;  /**< DMA2 Channel 2 global Interrupt                                     */
using dma2_channel3          = irq_channel<58>;  /**< DMA2 Channel 3 global Interrupt                                     */
using dma2_channel4_5        = irq_channel<59>;  /**< DMA2 Channel 4 and Channel 5 global Interrupt                       */
static constexpr int numof_interrupt_channels = 60;
#endif // STM32F10X_XL

#ifdef STM32F10X_CL
using adc1_2                 = irq_channel<18>;  /**< ADC1 and ADC2 global Interrupt                               */
using can1_tx                = irq_channel<19>;  /**< USB Device High Priority or CAN1 TX Interrupts               */
using can1_rx0               = irq_channel<20>;  /**< USB Device Low Priority or CAN1 RX0 Interrupts               */
using can1_rx1               = irq_channel<21>;  /**< CAN1 RX1 Interrupt                                           */
using can1_sce               = irq_channel<22>;  /**< CAN1 SCE Interrupt                                           */
using exti9_5                = irq_channel<23>;  /**< External Line[9:5] Interrupts                                */
using tim1_brk               = irq_channel<24>;  /**< TIM1 Break Interrupt                                         */
using tim1_up                = irq_channel<25>;  /**< TIM1 Update Interrupt                                        */
using tim1_trg_com           = irq_channel<26>;  /**< TIM1 Trigger and Commutation Interrupt                       */
using tim1_cc                = irq_channel<27>;  /**< TIM1 Capture Compare Interrupt                               */
using tim2                   = irq_channel<28>;  /**< TIM2 global Interrupt                                        */
using tim3                   = irq_channel<29>;  /**< TIM3 global Interrupt                                        */
using tim4                   = irq_channel<30>;  /**< TIM4 global Interrupt                                        */
using i2c1_ev                = irq_channel<31>;  /**< I2C1 Event Interrupt                                         */
using i2c1_er                = irq_channel<32>;  /**< I2C1 Error Interrupt                                         */
using i2c2_ev                = irq_channel<33>;  /**< I2C2 Event Interrupt                                         */
using i2c2_er                = irq_channel<34>;  /**< I2C2 Error Interrupt                                         */
using spi1                   = irq_channel<35>;  /**< SPI1 global Interrupt                                        */
using spi2                   = irq_channel<36>;  /**< SPI2 global Interrupt                                        */
using usart1                 = irq_channel<37>;  /**< USART1 global Interrupt                                      */
using usart2                 = irq_channel<38>;  /**< USART2 global Interrupt                                      */
using usart3                 = irq_channel<39>;  /**< USART3 global Interrupt                                      */
using exti15_10              = irq_channel<40>;  /**< External Line[15:10] Interrupts                              */
using rtc_alarm              = irq_channel<41>;  /**< RTC Alarm through EXTI Line Interrupt                        */
using otg_fs_wkup            = irq_channel<42>;  /**< USB OTG FS WakeUp from suspend through EXTI Line Interrupt   */
using tim5                   = irq_channel<50>;  /**< TIM5 global Interrupt                                        */
using spi3                   = irq_channel<51>;  /**< SPI3 global Interrupt                                        */
using uart4                  = irq_channel<52>;  /**< UART4 global Interrupt                                       */
using uart5                  = irq_channel<53>;  /**< UART5 global Interrupt                                       */
using tim6                   = irq_channel<54>;  /**< TIM6 global Interrupt                                        */
using tim7                   = irq_channel<55>;  /**< TIM7 global Interrupt                                        */
using dma2_channel1          = irq_channel<56>;  /**< DMA2 Channel 1 global Interrupt                              */
using dma2_channel2          = irq_channel<57>;  /**< DMA2 Channel 2 global Interrupt                              */
using dma2_channel3          = irq_channel<58>;  /**< DMA2 Channel 3 global Interrupt                              */
using dma2_channel4          = irq_channel<59>;  /**< DMA2 Channel 4 global Interrupt                              */
using dma2_channel5          = irq_channel<60>;  /**< DMA2 Channel 5 global Interrupt                              */
using eth                    = irq_channel<61>;  /**< Ethernet global Interrupt                                    */
using eth_wkup               = irq_channel<62>;  /**< Ethernet Wakeup through EXTI line Interrupt                  */
using can2_tx                = irq_channel<63>;  /**< CAN2 TX Interrupt                                            */
using can2_rx0               = irq_channel<64>;  /**< CAN2 RX0 Interrupt                                           */
using can2_rx1               = irq_channel<65>;  /**< CAN2 RX1 Interrupt                                           */
using can2_sce               = irq_channel<66>;  /**< CAN2 SCE Interrupt                                           */
using otg_fs                 = irq_channel<67>;  /**< USB OTG FS global Interrupt                                  */
static constexpr int numof_interrupt_channels = 68;
#endif // STM32F10X_CL


template<unsigned usart_no> class usart;
template<> class usart<1> : public usart1 { };
template<> class usart<2> : public usart2 { };
template<> class usart<3> : public usart3 { };

template<unsigned usart_no> class spi;
template<> class spi<1> : public spi1 { };
template<> class spi<2> : public spi2 { };
//  template<> class spi<3> : public spi3 { };

} } // namespace mptl::irq

#endif // ARCH_NVIC_HPP_INCLUDED
