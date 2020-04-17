/**************************************************************************//**
 * @file     helper_irq.c
 * @brief    Create centralied overridable IRQ & Fault handler table
 * @date     17. April 2020
 ******************************************************************************/
/*
 * Copyright (c) 2020 Milosch Meriac <milosch@meriac.com>. All rights reserved.
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
#include <stdint.h>
#include <Helper/helper_irq.h>

/* Have a centrally maintenable function for aliasing IRQs */
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

/*
 * Point all fault handlers by default to OnUnknownFault
 */

void OnUnknownFault(void)
{
    /* Spin and do nothing, useful for detecting
     * spurious interrupts while debugging */
    for(;;);
}

void OnNonMaskableInt(void) ALIAS(OnUnknownFault);
void OnHardFault(void) ALIAS(OnUnknownFault);
void OnMemoryManagement(void) ALIAS(OnUnknownFault);
void OnBusFault(void) ALIAS(OnUnknownFault);
void OnUsageFault(void) ALIAS(OnUnknownFault);
void OnSVCall(void) ALIAS(OnUnknownFault);
void OnDebugMonitor(void) ALIAS(OnUnknownFault);
void OnPendSV(void) ALIAS(OnUnknownFault);
void OnSysTick(void) ALIAS(OnUnknownFault);

/*
 * Point all interrupt handler by default to OnUnknownIrq
 */

static void OnUnknownIrq(void)
{
    /* Spin and do nothing, useful for detecting
     * spurious interrupts while debugging */
    for(;;);
}

void OnIrq_WWDG(void) ALIAS(OnUnknownIrq);
void OnIrq_PVD(void) ALIAS(OnUnknownIrq);
void OnIrq_TAMPER(void) ALIAS(OnUnknownIrq);
void OnIrq_RTC(void) ALIAS(OnUnknownIrq);
void OnIrq_FLASH(void) ALIAS(OnUnknownIrq);
void OnIrq_RCC(void) ALIAS(OnUnknownIrq);
void OnIrq_EXTI0(void) ALIAS(OnUnknownIrq);
void OnIrq_EXTI1(void) ALIAS(OnUnknownIrq);
void OnIrq_EXTI2(void) ALIAS(OnUnknownIrq);
void OnIrq_EXTI3(void) ALIAS(OnUnknownIrq);
void OnIrq_EXTI4(void) ALIAS(OnUnknownIrq);
void OnIrq_DMA1_Channel1(void) ALIAS(OnUnknownIrq);
void OnIrq_DMA1_Channel2(void) ALIAS(OnUnknownIrq);
void OnIrq_DMA1_Channel3(void) ALIAS(OnUnknownIrq);
void OnIrq_DMA1_Channel4(void) ALIAS(OnUnknownIrq);
void OnIrq_DMA1_Channel5(void) ALIAS(OnUnknownIrq);
void OnIrq_DMA1_Channel6(void) ALIAS(OnUnknownIrq);
void OnIrq_DMA1_Channel7(void) ALIAS(OnUnknownIrq);
void OnIrq_ADC1_2(void) ALIAS(OnUnknownIrq);
void OnIrq_USB_HP_CAN1_TX(void) ALIAS(OnUnknownIrq);
void OnIrq_USB_LP_CAN1_RX0(void) ALIAS(OnUnknownIrq);
void OnIrq_CAN1_RX1(void) ALIAS(OnUnknownIrq);
void OnIrq_CAN1_SCE(void) ALIAS(OnUnknownIrq);
void OnIrq_EXTI9_5(void) ALIAS(OnUnknownIrq);
void OnIrq_TIM1_BRK(void) ALIAS(OnUnknownIrq);
void OnIrq_TIM1_UP(void) ALIAS(OnUnknownIrq);
void OnIrq_TIM1_TRG_COM(void) ALIAS(OnUnknownIrq);
void OnIrq_TIM1_CC(void) ALIAS(OnUnknownIrq);
void OnIrq_TIM2(void) ALIAS(OnUnknownIrq);
void OnIrq_TIM3(void) ALIAS(OnUnknownIrq);
void OnIrq_TIM4(void) ALIAS(OnUnknownIrq);
void OnIrq_I2C1_EV(void) ALIAS(OnUnknownIrq);
void OnIrq_I2C1_ER(void) ALIAS(OnUnknownIrq);
void OnIrq_I2C2_EV(void) ALIAS(OnUnknownIrq);
void OnIrq_I2C2_ER(void) ALIAS(OnUnknownIrq);
void OnIrq_SPI1(void) ALIAS(OnUnknownIrq);
void OnIrq_SPI2(void) ALIAS(OnUnknownIrq);
void OnIrq_USART1(void) ALIAS(OnUnknownIrq);
void OnIrq_USART2(void) ALIAS(OnUnknownIrq);
void OnIrq_USART3(void) ALIAS(OnUnknownIrq);
void OnIrq_EXTI15_10(void) ALIAS(OnUnknownIrq);
void OnIrq_RTC_Alarm(void) ALIAS(OnUnknownIrq);
void OnIrq_USBWakeUp (void) ALIAS(OnUnknownIrq);

/* stack end address declared in linker script (minimal.ld etc.) */
extern const uint32_t StackTop;

/* declare vector table */
__attribute__ ((section(".vtor")))
const void* VectorTable[] = {
    &StackTop,          /* CPU will automatically       *
                         * set stack its pointer        *
                         * to this value                */

    OnReset,            /* -15: Reset_IRQn              */
    OnNonMaskableInt,   /* -14: NonMaskableInt_IRQn     */
    OnHardFault,        /* -13: HardFault_IRQn          */
    OnMemoryManagement, /* -12: MemoryManagement_IRQn   */
    OnBusFault,         /* -11: BusFault_IRQn           */
    OnUsageFault,       /* -10: UsageFault_IRQn         */
    OnUnknownFault,     /*  -9:                         */
    OnUnknownFault,     /*  -8:                         */
    OnUnknownFault,     /*  -7:                         */
    OnUnknownFault,     /*  -6:                         */
    OnSVCall,           /*  -5: SVCall_IRQn             */
    OnDebugMonitor,     /*  -4: DebugMonitor_IRQn       */
    OnUnknownFault,     /*  -3:                         */
    OnPendSV,           /*  -2: PendSV_IRQn             */
    OnSysTick,          /*  -1: SysTick_IRQn            */


    OnIrq_WWDG,             /*   0: Window WatchDog Interrupt                                  */
    OnIrq_PVD,              /*   1: PVD through EXTI Line detection Interrupt                  */
    OnIrq_TAMPER,           /*   2: Tamper Interrupt                                           */
    OnIrq_RTC,              /*   3: RTC global Interrupt                                       */
    OnIrq_FLASH,            /*   4: FLASH global Interrupt                                     */
    OnIrq_RCC,              /*   5: RCC global Interrupt                                       */
    OnIrq_EXTI0,            /*   6: EXTI Line0 Interrupt                                       */
    OnIrq_EXTI1,            /*   7: EXTI Line1 Interrupt                                       */
    OnIrq_EXTI2,            /*   8: EXTI Line2 Interrupt                                       */
    OnIrq_EXTI3,            /*   9: EXTI Line3 Interrupt                                       */
    OnIrq_EXTI4,            /*  10: EXTI Line4 Interrupt                                       */
    OnIrq_DMA1_Channel1,    /*  11: DMA1 Channel 1 global Interrupt                            */
    OnIrq_DMA1_Channel2,    /*  12: DMA1 Channel 2 global Interrupt                            */
    OnIrq_DMA1_Channel3,    /*  13: DMA1 Channel 3 global Interrupt                            */
    OnIrq_DMA1_Channel4,    /*  14: DMA1 Channel 4 global Interrupt                            */
    OnIrq_DMA1_Channel5,    /*  15: DMA1 Channel 5 global Interrupt                            */
    OnIrq_DMA1_Channel6,    /*  16: DMA1 Channel 6 global Interrupt                            */
    OnIrq_DMA1_Channel7,    /*  17: DMA1 Channel 7 global Interrupt                            */
    OnIrq_ADC1_2,           /*  18: ADC1 and ADC2 global Interrupt                             */
    OnIrq_USB_HP_CAN1_TX,   /*  19: USB Device High Priority or CAN1 TX Interrupts             */
    OnIrq_USB_LP_CAN1_RX0,  /*  20: USB Device Low Priority or CAN1 RX0 Interrupts             */
    OnIrq_CAN1_RX1,         /*  21: CAN1 RX1 Interrupt                                         */
    OnIrq_CAN1_SCE,         /*  22: CAN1 SCE Interrupt                                         */
    OnIrq_EXTI9_5,          /*  23: External Line[9:5] Interrupts                              */
    OnIrq_TIM1_BRK,         /*  24: TIM1 Break Interrupt                                       */
    OnIrq_TIM1_UP,          /*  25: TIM1 Update Interrupt                                      */
    OnIrq_TIM1_TRG_COM,     /*  26: TIM1 Trigger and Commutation Interrupt                     */
    OnIrq_TIM1_CC,          /*  27: TIM1 Capture Compare Interrupt                             */
    OnIrq_TIM2,             /*  28: TIM2 global Interrupt                                      */
    OnIrq_TIM3,             /*  29: TIM3 global Interrupt                                      */
    OnIrq_TIM4,             /*  30: TIM4 global Interrupt                                      */
    OnIrq_I2C1_EV,          /*  31: I2C1 Event Interrupt                                       */
    OnIrq_I2C1_ER,          /*  32: I2C1 Error Interrupt                                       */
    OnIrq_I2C2_EV,          /*  33: I2C2 Event Interrupt                                       */
    OnIrq_I2C2_ER,          /*  34: I2C2 Error Interrupt                                       */
    OnIrq_SPI1,             /*  35: SPI1 global Interrupt                                      */
    OnIrq_SPI2,             /*  36: SPI2 global Interrupt                                      */
    OnIrq_USART1,           /*  37: USART1 global Interrupt                                    */
    OnIrq_USART2,           /*  38: USART2 global Interrupt                                    */
    OnIrq_USART3,           /*  39: USART3 global Interrupt                                    */
    OnIrq_EXTI15_10,        /*  40: External Line[15:10] Interrupts                            */
    OnIrq_RTC_Alarm,        /*  41: RTC Alarm through EXTI Line Interrupt                      */
    OnIrq_USBWakeUp         /*  42: USB Device WakeUp from suspend through EXTI Line Interrupt */
};
