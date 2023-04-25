/** 
***********************************************************
 * 
 * @file name	: hal_osc32k768.h
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-02-23
 * @brief		: HAL 32.768k Oscillator driver header file
 * 
***********************************************************
 * @attention
 * 
 * Copyright (c) 2020 Soundec Co., Ltd. 
 * All rights reserved.
 * 
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user
 * who uses the software, his/her employer (which must be your employer
 * and Soundec Co., Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 * 
***********************************************************/


#ifndef __HAL_OSC32K768_H__
#define __HAL_OSC32K768_H__

#include "snc8600.h"
#include "hal.h"

#define HAL_OSC32K768_MODE_SHIFT		0
#define HAL_OSC32K768_MODE_MASK			(0x03 << HAL_OSC32K768_MODE_SHIFT)

#define HAL_OSC32K768_TRIM_SHIFT		2
#define HAL_OSC32K768_TRIM_MASK			(0x3f << HAL_OSC32K768_TRIM_SHIFT)

#define HAL_OSC32K768_DELAY_CYCLE_SHIFT	8
#define HAL_OSC32K768_DELAY_CYCLE_MASK	(0xff << HAL_OSC32K768_DELAY_CYCLE_SHIFT)

/* 
 * @brief This enum defines the operation mode of 32.768kHz Oscillator 
 */
typedef enum {
	HAL_OSC32K768_MODE_SHUTDOWN = 1,	/* Shutdown mode, no clock generated and only leakage consumption */
	HAL_OSC32K768_MODE_STANDBY,			/* Standby mode, no clock generated, internal biasing function ON, quick start available */
	HAL_OSC32K768_MODE_NORMAL,			/* Normal mode, clock generated */
	HAL_OSC32K768_MODE_NUM
} hal_osc32k768_mode_t;

/* 
 * @brief This structure defines the registers of 32.768kHz Oscillator 
 */
typedef struct {
	uint32_t reg;
} hal_osc32k768_t;

/** 
 * @brief	This function is used to set the power mode of OSC32K768.
 * @param[in]	mode        The power mode is set. This parameter only can be one of the value type #hal_osc32k768_mode_t
 * @return	
 * 	@retval HAL_STATUS_OK The power mode is successfully set.
 */
hal_status_t hal_osc32k768_set_mode(hal_osc32k768_mode_t mode);

/**
 * @brief 	This function is used to set the delay cycles.
 * @note 	When the 32.768k Oscillator is set to normal, the clock will be output after delaying #cycle cycles.
 *			The purpose is to make the Oscillator output stable 32.768k clock, ignore the unstable clock.
 * @param[in] 	cycle 		The delay cycles will be set.
 * @return	
 * 	@retval HAL_STATUS_OK The delay cycle is successfully set.
 */
hal_status_t hal_osc32k768_set_delay_cycle(uint32_t cycle);

/**
 * @brief 	This function is used to trim the output clock of OSC32K768.
 * @param[in] 	trim 		The trimming will be set.
 * @return	
 * 	@retval HAL_STATUS_OK The trimming is successfully set.
 */
hal_status_t hal_osc32k768_set_trim(uint32_t trim);


#endif /* __HAL_OSC32K768_H__ */
