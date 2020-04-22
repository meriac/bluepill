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
static ADC_HandleTypeDef g_adc;

static const GPIO_InitItem g_gpio_init[] = {
    {GPIOA, {GPIO_PIN_2, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},
    {GPIOA, {GPIO_PIN_3, GPIO_MODE_AF_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_HIGH}},
    {GPIOB, {GPIO_PIN_0, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},
    {GPIOB, {GPIO_PIN_1, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},
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

static void init_adc(void)
{
    static const ADC_InitTypeDef adc_init = {
        ADC_DATAALIGN_RIGHT,
        ADC_SCAN_DISABLE,
        DISABLE,
        1,
        DISABLE,
        1,
        ADC_SOFTWARE_START
    };

    /* Enable Clocks */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_ADC1_CLK_ENABLE();

        /* Pre-initialize ADC */
    g_adc.Instance = ADC1;
    g_adc.Init = adc_init;
    HAL_ADC_Init(&g_adc);
};


static int adc_read(int channel)
{
    ADC_ChannelConfTypeDef adcc;

    switch(channel)
    {
        case 0: 
            adcc.Channel = ADC_CHANNEL_8;
            break;
        case 1:
            adcc.Channel = ADC_CHANNEL_9;
            break;
        default:
            return -1;
    }
    adcc.Rank = 1;
    adcc.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
 
    if (HAL_ADC_ConfigChannel(&g_adc, &adcc) != HAL_OK)
        return -2;

    if (HAL_ADC_Start(&g_adc) != HAL_OK)
        return -3;

    if (HAL_ADC_PollForConversion(&g_adc, 1000000) != HAL_OK)
        return -4;

    return HAL_ADC_GetValue(&g_adc);
}

static void init_clocks(void)
{
    static const RCC_OscInitTypeDef rcc_osc_init = {
        RCC_OSCILLATORTYPE_HSE,
        RCC_HSE_ON,
        RCC_HSE_PREDIV_DIV1,
        0,0,0,0,
        {
            RCC_PLL_ON,
            RCC_PLLSOURCE_HSE,
            RCC_PLL_MUL9
        }
    };

    static const RCC_ClkInitTypeDef rcc_clk_init = {
        (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2),
        RCC_SYSCLKSOURCE_PLLCLK,
        RCC_SYSCLK_DIV1,
        RCC_HCLK_DIV2,
        RCC_HCLK_DIV1,
    };

    HAL_RCC_OscConfig((RCC_OscInitTypeDef*)&rcc_osc_init);
    HAL_RCC_ClockConfig((RCC_ClkInitTypeDef*)&rcc_clk_init, FLASH_LATENCY_2);

    __HAL_RCC_PWR_CLK_ENABLE();

    SystemCoreClockUpdate();
}

static void init_usart(void)
{
    int i;
 
    static const USART_InitTypeDef usart_init = {
        115200*2,
        USART_WORDLENGTH_8B,
        USART_STOPBITS_1,
        USART_PARITY_NONE,
        USART_MODE_TX_RX,
        USART_POLARITY_LOW,
        USART_PHASE_1EDGE,
        USART_LASTBIT_ENABLE
    };

    /* reconfigure clocks */
    init_clocks();

    /* Enable Clocks */
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* Initialize Pins */
    for(i=0;i<GPIO_INIT_COUNT;i++)
        HAL_GPIO_Init(g_gpio_init[i].gpio, (GPIO_InitTypeDef*)&g_gpio_init[i].pin);

    /* Enable USART2 */
    __HAL_RCC_USART2_FORCE_RESET( );
    __HAL_RCC_USART2_RELEASE_RESET( );
    __HAL_RCC_USART2_CLK_ENABLE();

    /* Pre-initialize UART */
    g_usart.Instance = USART2;
    g_usart.Init = usart_init;
    HAL_USART_Init(&g_usart);
}

/* Main application entry - called by OnReset handler,
 * performing memory initalizations in helper_init.c */
void main_entry(void)
{
    uint32_t counter;

    /* Init hardware */
    init_usart();
    init_adc();
 
    /* Sleep forever */
    counter = 0;
    while(1)
    {
        Blink(3);

        tfp_printf("ADC0: %d\n\r",adc_read(0));
        tfp_printf("ADC1: %d\n\r",adc_read(1));

        tfp_printf("%08u: Hello world (@%uMHz)!\n\r",counter++,SystemCoreClock/1000000);
    }
}
