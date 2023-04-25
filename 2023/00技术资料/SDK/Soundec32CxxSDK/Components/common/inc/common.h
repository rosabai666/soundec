/** 
***********************************************************
 *
 * @file name		: common.h
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2020-02-17
 * @brief		: Common definations used whole of project based on SDK
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
 * and Soundec Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_H__
#define __COMMON_H__

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "types.h"
#include "sys_init.h"
#include "boardConfig.h"
#include "userFlash.h"
#include "SaveRestoreVectorRegisters.h"
#include "audio_def.h"
#include "hal_common.h"
#include"debug_tools.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private variables ---------------------------------------------------------*/
//Todo: Should move to board_config.h
typedef enum
{
	DRV_STATUS_OK = 0,
	DRV_STATUS_FAIL = -1,
	DRV_STATUS_TIMEOUT = -2,
	DRV_STATUS_BUSY = -3,
}drv_status_t;

/* Private function prototypes -----------------------------------------------*/
extern void delay(uint32_t ms);
extern int uart_printf(const char *fmt, ...);

/* Private functions ---------------------------------------------------------*/
void delay(uint32_t ms);
void delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif  /* __COMMON_H__ */


