/** 
***********************************************************
 * 
 * @file name	: task.h
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2022-01-20
 * @brief		:  
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
#ifndef __TASK_H__
#define __TASK_H__

/* Includes ------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* Private macro -------------------------------------------------------------*/
#define TASK_NUM 	4

/* Private typedef -----------------------------------------------------------*/
typedef int (*task_hook_t)(void);

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern U32 get_sys_tick(void);
extern void task_register(uint8_t index, int itvl_ms, task_hook_t task);
extern void task_process(void);
extern void task_init(void);

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  /* __TASK_H__ */






