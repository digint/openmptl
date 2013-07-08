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

#ifndef NVIC_HPP_INCLUDED
#define NVIC_HPP_INCLUDED

#include "../../../../common/nvic.hpp"

namespace mptl { namespace irq {

using wwdg               = irq_channel< 0>;  /**< Window Watchdog interrupt                                                */
using pvd                = irq_channel< 1>;  /**< PVD through EXTI line detection interrupt                                */
using tamp_stamp         = irq_channel< 2>;  /**< Tamper and TimeStamp interrupts through the EXTI line                    */
using rtc_wkup           = irq_channel< 3>;  /**< RTC Wakeup interrupt through the EXTI line                               */
using flash              = irq_channel< 4>;  /**< Flash global interrupt                                                   */
using rcc                = irq_channel< 5>;  /**< RCC global interrupt                                                     */
using exti0              = irq_channel< 6>;  /**< EXTI Line0 interrupt                                                     */
using exti1              = irq_channel< 7>;  /**< EXTI Line1 interrupt                                                     */
using exti2              = irq_channel< 8>;  /**< EXTI Line2 interrupt                                                     */
using exti3              = irq_channel< 9>;  /**< EXTI Line3 interrupt                                                     */
using exti4              = irq_channel<10>;  /**< EXTI Line4 interrupt                                                     */
using dma1_stream0       = irq_channel<11>;  /**< DMA1 Stream0 global interrupt                                            */
using dma1_stream1       = irq_channel<12>;  /**< DMA1 Stream1 global interrupt                                            */
using dma1_stream2       = irq_channel<13>;  /**< DMA1 Stream2 global interrupt                                            */
using dma1_stream3       = irq_channel<14>;  /**< DMA1 Stream3 global interrupt                                            */
using dma1_stream4       = irq_channel<15>;  /**< DMA1 Stream4 global interrupt                                            */
using dma1_stream5       = irq_channel<16>;  /**< DMA1 Stream5 global interrupt                                            */
using dma1_stream6       = irq_channel<17>;  /**< DMA1 Stream6 global interrupt                                            */
using adc                = irq_channel<18>;  /**< ADC1 global interrupt / ADC2 global interrupts / ADC3 global interrupts  */
using can1_tx            = irq_channel<19>;  /**< CAN1 TX interrupts                                                       */
using can1_rx0           = irq_channel<20>;  /**< CAN1 RX0 interrupts                                                      */
using can1_rx1           = irq_channel<21>;  /**< CAN1 RX1 interrupts                                                      */
using can1_sce           = irq_channel<22>;  /**< CAN1 SCE interrupt                                                       */
using exti9_5            = irq_channel<23>;  /**< EXTI Line[9:5] interrupts                                                */
using tim1_brk_tim9      = irq_channel<24>;  /**< TIM1 Break interrupt and TIM9 global interrupt                           */
using tim1_up_tim10      = irq_channel<25>;  /**< TIM1 Update interrupt and TIM10 global interrupt                         */
using tim1_trg_com_tim11 = irq_channel<26>;  /**< TIM1 Trigger and Commutation interrupts and TIM11 global interrupt       */
using tim1_cc            = irq_channel<27>;  /**< TIM1 Capture Compare interrupt                                           */
using tim2               = irq_channel<28>;  /**< TIM2 global interrupt                                                    */
using tim3               = irq_channel<29>;  /**< TIM3 global interrupt                                                    */
using tim4               = irq_channel<30>;  /**< TIM4 global interrupt                                                    */
using i2c1_ev            = irq_channel<31>;  /**< I2C1 event interrupt                                                     */
using i2c1_er            = irq_channel<32>;  /**< I2C1 error interrupt                                                     */
using i2c2_ev            = irq_channel<33>;  /**< I2C2 event interrupt                                                     */
using i2c2_er            = irq_channel<34>;  /**< I2C2 error interrupt                                                     */
using spi1               = irq_channel<35>;  /**< SPI1 global interrupt                                                    */
using spi2               = irq_channel<36>;  /**< SPI2 global interrupt                                                    */
using usart1             = irq_channel<37>;  /**< USART1 global interrupt                                                  */
using usart2             = irq_channel<38>;  /**< USART2 global interrupt                                                  */
using usart3             = irq_channel<39>;  /**< USART3 global interrupt                                                  */
using exti15_10          = irq_channel<40>;  /**< EXTI Line[15:10] interrupts                                              */
using rtc_alarm          = irq_channel<41>;  /**< RTC Alarms (A and B) through EXTI line interrupt                         */
using otg_fs_wkup        = irq_channel<42>;  /**< USB On-The-Go FS Wakeup through EXTI line interrupt                      */
using tim8_brk_tim12     = irq_channel<43>;  /**< TIM8 Break interrupt and TIM12 global interrupt                          */
using tim8_up_tim13      = irq_channel<44>;  /**< TIM8 Update interrupt and TIM13 global interrupt                         */
using tim8_trg_com_tim14 = irq_channel<45>;  /**< TIM8 Trigger and Commutation interrupts and TIM14 global interrupt       */
using tim8_cc            = irq_channel<46>;  /**< TIM8 Capture Compare interrupt                                           */
using dma1_stream7       = irq_channel<47>;  /**< DMA1 Stream7 global interrupt                                            */
using fsmc               = irq_channel<48>;  /**< FSMC global interrupt                                                    */
using sdio               = irq_channel<49>;  /**< SDIO global interrupt                                                    */
using tim5               = irq_channel<50>;  /**< TIM5 global interrupt                                                    */
using spi3               = irq_channel<51>;  /**< SPI3 global interrupt                                                    */
using uart4              = irq_channel<52>;  /**< UART4 global interrupt                                                   */
using uart5              = irq_channel<53>;  /**< UART5 global interrupt                                                   */
using tim6_dac           = irq_channel<54>;  /**< TIM6 global interrupt, DAC1 and DAC2 underrun error interrupt            */
using tim7               = irq_channel<55>;  /**< TIM7 global interrupt                                                    */
using dma2_stream0       = irq_channel<56>;  /**< DMA2 Stream0 global interrupt                                            */
using dma2_stream1       = irq_channel<57>;  /**< DMA2 Stream1 global interrupt                                            */
using dma2_stream2       = irq_channel<58>;  /**< DMA2 Stream2 global interrupt                                            */
using dma2_stream3       = irq_channel<59>;  /**< DMA2 Stream3 global interrupt                                            */
using dma2_stream4       = irq_channel<60>;  /**< DMA2 Stream4 global interrupt                                            */
using eth                = irq_channel<61>;  /**< Ethernet global interrupt                                                */
using eth_wkup           = irq_channel<62>;  /**< Ethernet Wakeup through EXTI line interrupt                              */
using can2_tx            = irq_channel<63>;  /**< CAN2 TX interrupts                                                       */
using can2_rx0           = irq_channel<64>;  /**< CAN2 RX0 interrupts                                                      */
using can2_rx1           = irq_channel<65>;  /**< CAN2 RX1 interrupts                                                      */
using can2_sce           = irq_channel<66>;  /**< CAN2 SCE interrupt                                                       */
using otg_fs             = irq_channel<67>;  /**< USB On The Go FS global interrupt                                        */
using dma2_stream5       = irq_channel<68>;  /**< DMA2 Stream5 global interrupt                                            */
using dma2_stream6       = irq_channel<69>;  /**< DMA2 Stream6 global interrupt                                            */
using dma2_stream7       = irq_channel<70>;  /**< DMA2 Stream7 global interrupt                                            */
using usart6             = irq_channel<71>;  /**< USART6 global interrupt                                                  */
using i2c3_ev            = irq_channel<72>;  /**< I2C3 event interrupt                                                     */
using i2c3_er            = irq_channel<73>;  /**< I2C3 error interrupt                                                     */
using otg_hs_ep1_out     = irq_channel<74>;  /**< USB On The Go HS End Point 1 Out global interrupt                        */
using otg_hs_ep1_in      = irq_channel<75>;  /**< USB On The Go HS End Point 1 In global interrupt                         */
using otg_hs_wkup        = irq_channel<76>;  /**< USB On The Go HS Wakeup through EXTI interrupt                           */
using otg_hs             = irq_channel<77>;  /**< USB On The Go HS global interrupt                                        */
using dcmi               = irq_channel<78>;  /**< DCMI global interrupt                                                    */
using cryp               = irq_channel<79>;  /**< CRYP crypto global interrupt                                             */
using hash_rng           = irq_channel<80>;  /**< Hash and Rng global interrupt                                            */
using fpu                = irq_channel<81>;  /**<  FPU global interrupt                                                    */


template<unsigned usart_no> class usart;
template<> class usart<1> : public usart1 { };
template<> class usart<2> : public usart2 { };
template<> class usart<3> : public usart3 { };
template<> class usart<6> : public usart6 { };

template<unsigned usart_no> class spi;
template<> class spi<1> : public spi1 { };
template<> class spi<2> : public spi2 { };
template<> class spi<3> : public spi3 { };

static constexpr int numof_interrupt_channels = 82;

} } // namespace mptl::irq

#endif // NVIC_HPP_INCLUDED
