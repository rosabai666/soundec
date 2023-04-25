/**
***********************************************************
 *
 * @file name	: drv_gpio.h
 * @author		: RandyFan
 * @version		: V02.01
 * @date		: 2022-02-16
 * @brief		: GPIO Driver Header
 *
***********************************************************
 * @attention
 *
 * Copyright (c) 2022 Soundec Co., Ltd.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user
 * who uses the software, his/her employer (which must be your employer
 * and Soundec Co., Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/

#ifndef __DRV_GPIO_H__
#define __DRV_GPIO_H__

#include "boardConfig.h"
#include "hal_gpio.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*gpio_cb_t)(uint8_t pin);

/**
 * @struct gpio_intr_cfg_t
 * @brief Gpio interrupt configuration
 */
typedef struct{
	uint8_t pin;
	uint8_t isrTrigger;
	gpio_cb_t cb;
}gpio_intr_cfg_t;

/**
 * @brief Config gpio interrupt
 * @param[in] cfg
 * @return config ok or not
 */
bool gpio_cfg_intr(gpio_intr_cfg_t cfg);

/**
 * @brief Enable gpio interrupt
 * @param[in] intr interrupt source
 */
void gpio_enable_intr(gpio_intr_t intr);

#ifdef __cplusplus
}
#endif
#endif /* __DRV_GPIO_H__ */
