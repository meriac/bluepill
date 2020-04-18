/**************************************************************************//**
 * @file     common.h
 * @brief    Top-level include file for Baremetal-C course
 * @date     12. August 2018
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
#ifndef __COMMON_H__
#define __COMMON_H__

#include <stm32f1xx.h>
#include <stdbool.h>
#include <string.h>
#include <helper.h>

#ifdef  USE_HAL
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_rcc_ex.h>

typedef struct {
    GPIO_TypeDef  *gpio;
    GPIO_InitTypeDef pin;
} GPIO_InitItem;

#endif/*USE_HAL*/

#endif/*__NRF_H__*/
