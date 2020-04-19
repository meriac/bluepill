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

static TIM_HandleTypeDef g_pwm;
static USART_HandleTypeDef g_usart;

static const GPIO_InitItem g_gpio_init[] = {
    {GPIOA, {GPIO_PIN_8, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},
    {GPIOA, {GPIO_PIN_2, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},
    {GPIOA, {GPIO_PIN_3, GPIO_MODE_AF_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_HIGH}},
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

static void pwm_update(uint32_t ms)
{
    if(ms>1000)
        ms=1000;

    TIM1->CCR1 = 1000+ms;
}

static void pwm_init(void)
{
    TIM_OC_InitTypeDef sConfigOC;
    TIM_MasterConfigTypeDef sMasterConfig;

    __HAL_RCC_TIM1_CLK_ENABLE();

    memset(&g_pwm, 0, sizeof(g_pwm));
    g_pwm.Instance = TIM1;
    g_pwm.Init.Prescaler = SystemCoreClock/1000000;
    g_pwm.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_pwm.Init.Period = 1000000/50;
    g_pwm.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&g_pwm);
 
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&g_pwm, &sMasterConfig);

    memset(&sConfigOC, 0, sizeof(sConfigOC));
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 1000;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(&g_pwm, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&g_pwm, TIM_CHANNEL_1); 
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

static void init(void)
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
    /* init PWM */
    pwm_init();

    /* Enable Clocks */
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
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
    int i;
    uint32_t counter;

    /* Init hardware */
    init();
 
    /* Sleep forever */
    counter = 0;
    while(1)
    {
        Blink(3);
        tfp_printf("%08u: Hello world (@%uMHz)!\n\r",counter++,SystemCoreClock/1000000);

        for(i=25;i<120;i++)
        {
            pwm_update(i);
            HAL_Delay(4);
        }
        for(i=120;i>25;i--)
        {
            pwm_update(i);
            HAL_Delay(6);
        }
    }
}
