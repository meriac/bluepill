/**************************************************************************//**
 * @file     linker_magic.c
 * @brief    Demonstrate linker initializing SRAM upon boot
 * @date     18. April 2020
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
#include <common.h>

static USART_HandleTypeDef g_usart;

static const GPIO_InitItem g_gpio_init[] = {
    {GPIOA, {GPIO_PIN_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},
    {GPIOA, {GPIO_PIN_10, GPIO_MODE_AF_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_HIGH}},
    {GPIOC, {GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW}}
};
#define GPIO_INIT_COUNT ((int)(sizeof(g_gpio_init)/sizeof(g_gpio_init[0])))

static void Blink(int count)
{
    int i;

    for(i=0; i<count; i++)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        HAL_Delay(50);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        HAL_Delay(200);
    }
    HAL_Delay(500);
 }

void tfp_putchar(char data)
{
    HAL_USART_Transmit(&g_usart, (uint8_t*)&data, sizeof(data), HAL_MAX_DELAY);
}

static void init(void)
{
    int i;
 
    const USART_InitTypeDef usart_init = {
        115200*2,
        USART_WORDLENGTH_8B,
        USART_STOPBITS_1,
        USART_PARITY_NONE,
        USART_MODE_TX_RX,
        USART_POLARITY_LOW,
        USART_PHASE_1EDGE,
        USART_LASTBIT_ENABLE
    };

    /* Enable Clocks */
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* Initialize Pins */
    for(i=0;i<GPIO_INIT_COUNT;i++)
        HAL_GPIO_Init(g_gpio_init[i].gpio, (GPIO_InitTypeDef*)&g_gpio_init[i].pin);

    /* Enable USART1 */
    __HAL_RCC_USART1_FORCE_RESET( );
    __HAL_RCC_USART1_RELEASE_RESET( );
    __HAL_RCC_USART1_CLK_ENABLE();

    /* Pre-initialize UART */
    g_usart.Instance = USART1;
    g_usart.Init = usart_init;
    HAL_USART_Init(&g_usart);
} 

/* Main application entry - called by OnReset handler,
 * performing memory initalizations in helper_init.c */
void main_entry(void)
{
    uint32_t counter;

    /* Init hardware */
    init();
 
    /* Sleep forever */
    counter = 0;
    while(1)
    {
        Blink(3);

        tfp_printf("%08u: Hello world!\n\r",counter++);
    }
}
