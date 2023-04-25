/** 
***********************************************************
 * 
 * @file name	: drv_saradc.h
 * @author		: RandyFan
 * @version		: V02.01
 * @date		: 2022-02-18
 * @brief		: SARADC MODULE DRIVER
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
 * and Soundec Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 * 
***********************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_SARADC_H__
#define __DRV_SARADC_H__

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <xtensa/xtruntime.h>
#include <xtensa/config/core.h>
#include "boardConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
#if(AUX_ADC_INTR_ENABLE)
typedef void (*saradc_callback_t)(uint16_t val);
#endif

/**
 * @struct saradc_config_t
 * @brief Configuration of SAR ADC
 */
typedef struct
{
	uint8_t	resolution;
	uint8_t channel;    	///<Input mode, fast channel mode, Min sampling time (ns) and Additional clk cycles The sampling time and clk cycles are less. Slow channel can effectively protect accuracy
	uint8_t vinMode;
	uint32_t sampleRate;
#if(AUX_ADC_INTR_ENABLE)
	saradc_callback_t cb;
#endif
}saradc_config_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
 * @brief This function inits and configs SAR ADC
 */
void aux_adc_init(void);
#if(AUX_ADC_INTR_ENABLE)
/**
 * @brief This function registers SAR ADC's isr callback
 * @param[in] cb callback
 */
void aux_adc_register_cb(saradc_callback_t cb);
#endif
uint16_t aux_adc_get_val(void);

#ifdef __cplusplus
}
#endif

#endif  /* __DRV_SARADC_H__ */

