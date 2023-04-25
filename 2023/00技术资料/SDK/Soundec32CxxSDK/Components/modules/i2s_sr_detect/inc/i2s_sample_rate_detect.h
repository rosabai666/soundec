/** 
***********************************************************
 * 
 * @file name	: sample_auto_adaptive.h
 * @author	: xxxx
 * @version	: Vxx.xx
 * @date		: 2021-07-28
 * @brief		: FS AUTO ADAPTIVE FUNCTION
 * 
***********************************************************
 * @attention
 * 
 * Copyright (c) 2021 Soundec Co., Ltd. 
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
#ifndef __SAMPLE_AUTO_ADAPTIVE_H__
#define __SAMPLE_AUTO_ADAPTIVE_H__

/* Includes ------------------------------------------------------------------*/
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
typedef struct {
  uint32_t count;
  uint32_t curSampleRate;
  uint32_t recordSampleRate;
  uint8_t checkCount;
  uint8_t unmuteCount;
}i2sSrDet_t;

/* Private macro -------------------------------------------------------------*/
#define CHECK_NUM       250//6
#define FS_SUPPORT_NUM  6
#define DETECT_SAMPLE_DEBOUNCE_MUTE_COUNT 5
#define ACTION_POINT 1

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void i2s_sr_detect_process_ms(void) ;
void i2s_sr_detect_count(void) ;
uint32_t get_i2s_sample_rate(void);
uint32_t get_i2s_sample(void);

#ifdef __cplusplus
}
#endif

#endif  /* __SAMPLE_AUTO_ADAPTIVE_H__ */




