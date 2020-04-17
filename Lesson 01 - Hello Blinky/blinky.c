/**************************************************************************//**
 * @file     linker_magic.c
 * @brief    Demonstrate linker initializing SRAM upon boot
 * @date     16. August 2018
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
#include <common.h>

static void Wait()
{
    volatile int i;

    /* wait ~0.5s */
    for(i=0; i<200000; i++);
}

static void Blink(int count)
{
    int i;

    for(i=0; i<count; i++)
    {
        GPIOC->BSRR = GPIO_BSRR_BR13;
        Wait();
        GPIOC->BSRR = GPIO_BSRR_BS13;
        Wait();
    }
    Wait();
    Wait();
}

/* Main application entry - called by OnReset handler,
 * performing memory initalizations in helper_init.c */
void main_entry(void)
{
    /* Enable GPIO port C */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH = GPIO_CRH_MODE13_0;
    GPIOC->ODR = GPIO_ODR_ODR13;
 
    /* Sleep forever */
    while(1)
        Blink(3);
}
