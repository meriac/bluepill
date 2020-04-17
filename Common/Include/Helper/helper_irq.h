/**************************************************************************//**
 * @file     helper_irq.h
 * @brief    Create centralied overridable IRQ & Fault handler table
 * @date     17. April 2020
 ******************************************************************************/
/*
 * Copyright (c) 2018 Milosch Meriac <milosch@meriac.com>. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __HELPER_IRQ_H__
#define __HELPER_IRQ_H__

/*
 * Special case: reset never returns, allows compiler to optimize
 * */
extern void OnReset(void) __attribute__ ((noreturn));

/*
 * Generic Arm Architecture Fault Handlers
 */
extern void OnReset(void);
extern void OnNonMaskableInt(void);
extern void OnHardFault(void);
extern void OnMemoryManagement(void);
extern void OnBusFault(void);
extern void OnUsageFault(void);
extern void OnSVCall(void);
extern void OnDebugMonitor(void);
extern void OnPendSV(void);
extern void OnSysTick(void);

/*
 * Vendor-specific IRQ handlers
 */
extern void OnIrq_WWDG(void);
extern void OnIrq_PVD(void);
extern void OnIrq_TAMPER(void);
extern void OnIrq_RTC(void);
extern void OnIrq_FLASH(void);
extern void OnIrq_RCC(void);
extern void OnIrq_EXTI0(void);
extern void OnIrq_EXTI1(void);
extern void OnIrq_EXTI2(void);
extern void OnIrq_EXTI3(void);
extern void OnIrq_EXTI4(void);
extern void OnIrq_DMA1_Channel1(void);
extern void OnIrq_DMA1_Channel2(void);
extern void OnIrq_DMA1_Channel3(void);
extern void OnIrq_DMA1_Channel4(void);
extern void OnIrq_DMA1_Channel5(void);
extern void OnIrq_DMA1_Channel6(void);
extern void OnIrq_DMA1_Channel7(void);
extern void OnIrq_ADC1_2(void);
extern void OnIrq_USB_HP_CAN1_TX(void);
extern void OnIrq_USB_LP_CAN1_RX0(void);
extern void OnIrq_CAN1_RX1(void);
extern void OnIrq_CAN1_SCE(void);
extern void OnIrq_EXTI9_5(void);
extern void OnIrq_TIM1_BRK(void);
extern void OnIrq_TIM1_UP(void);
extern void OnIrq_TIM1_TRG_COM(void);
extern void OnIrq_TIM1_CC(void);
extern void OnIrq_TIM2(void);
extern void OnIrq_TIM3(void);
extern void OnIrq_TIM4(void);
extern void OnIrq_I2C1_EV(void);
extern void OnIrq_I2C1_ER(void);
extern void OnIrq_I2C2_EV(void);
extern void OnIrq_I2C2_ER(void);
extern void OnIrq_SPI1(void);
extern void OnIrq_SPI2(void);
extern void OnIrq_USART1(void);
extern void OnIrq_USART2(void);
extern void OnIrq_USART3(void);
extern void OnIrq_EXTI15_10(void);
extern void OnIrq_RTC_Alarm(void);
extern void OnIrq_USBWakeUp (void);

#endif/*__HELPER_IRQ_H__*/
