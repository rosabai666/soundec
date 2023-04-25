/** 
***********************************************************
 * 
 * @file name	: hal.h
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-02-20
 * @brief		: HAL Header File
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_H__
#define __HAL_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @brief	This enum defines the status of HAL
 */
typedef enum
{
	HAL_STATUS_OK = 0,
	HAL_STATUS_FAIL = -1,
	HAL_STATUS_TIMEOUT = -2,
	HAL_STATUS_BUSY = -3,
	HAL_STATUS_ERROR = -4,

	HAL_STATUS_ERROR_CACHE_SIZE = -5,
	HAL_STATUS_ERROR_CACHE_REGION_ADDRESS = -6,
	HAL_STATUS_ERROR_CACHE_REGION_SIZE = -7,
} hal_status_t;

extern uint32_t hal_get_lib_version(void);

#ifdef __cplusplus
}
#endif

#endif
