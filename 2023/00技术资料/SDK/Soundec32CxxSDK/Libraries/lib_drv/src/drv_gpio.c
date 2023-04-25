/**
***********************************************************
 *
 * @file name	: drv_gpio.c
 * @author		: RandyFan
 * @version		: V02.01
 * @date		: 2022-02-16
 * @brief		: GPIO Driver
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

#include "drv_gpio.h"
#include "SaveRestoreVectorRegisters.h"
#include "hal_interrupt.h"
#include "common.h"

gpio_cb_t gpioCb[GPIO_CB_NUM] = {NULL};

/**
 * @brief Interrupt service routine of gpio 678
 */
void hal_gpio_678_isr(void *arg)
{
	SaveVectors(GPIO_678_IRQn);

	uint32_t intrSts = GPIO_Read(PORT_INTRSTS);
	hal_interrupt_clear_irq(GPIO_678_IRQn);

	uint8_t i = 6;
	while(i < 9)
	{
		if(intrSts & (1 << i))
		{
			GPIO_Write(PORT_INTRCLR, (0x01 << i));
			if(gpioCb[i-6])
				gpioCb[i-6](i);
		}
		i++;
	}
	RestoreVectors(GPIO_678_IRQn);
}

/**
 * @brief Interrupt service routine of gpio 9ABCDEF
 */
void hal_gpio_9ABCDEF_isr(void *arg)
{
	SaveVectors(GPIO_9ABCDEF_IRQn);

	uint32_t intrSts = GPIO_Read(PORT_INTRSTS);
	hal_interrupt_clear_irq(GPIO_9ABCDEF_IRQn);

	uint8_t i = 9;
	while(i < 0x10)
	{
		if(intrSts & (1 << i))
		{
			GPIO_Write(PORT_INTRCLR, (0x01 << i));
			if(gpioCb[i-6])
				gpioCb[i-6](i);
		}
		i++;
	}
	RestoreVectors(GPIO_9ABCDEF_IRQn);
}

/**
 * @brief Config gpio interrupt
 * @param[in] cfg
 * @return config ok or not
 */
bool gpio_cfg_intr(gpio_intr_cfg_t cfg)
{
	ASSERT(cfg.pin >= GPIO_6 && cfg.pin < GPIO_NUM);
	ASSERT(cfg.isrTrigger < GPIO_TRIGGER_NUM);
	ASSERT(cfg.cb);

	if(!(GPIO_Read(PORT_DIR) & (1 << cfg.pin)))
	{
		return false;
	}
	hal_gpio_config_isr(cfg.pin, cfg.isrTrigger);
	gpioCb[cfg.pin - 6] = cfg.cb;
	return true;
}

/**
 * @brief Enable gpio interrupt
 * @param[in] intr interrupt source
 */
void gpio_enable_intr(gpio_intr_t intr)
{
	if(intr == GPIO_678_INTR)
	{
		hal_interrupt_register_isr_handler(GPIO_678_IRQn, hal_gpio_678_isr);
		hal_interrupt_enable_irq(GPIO_678_IRQn);
	}
	else
	{
		hal_interrupt_register_isr_handler(GPIO_9ABCDEF_IRQn, hal_gpio_9ABCDEF_isr);
		hal_interrupt_enable_irq(GPIO_9ABCDEF_IRQn);
	}
}


