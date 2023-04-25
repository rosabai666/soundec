/** 
***********************************************************
 * 
 * @file name	: debug_tools.h
 * @author		: yen.yu
 * @version		: Vxx.xx
 * @date		: 2022-09-01
 * @brief		: Interface for debug tools
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
#ifndef __DEBUG_TOOLS_H__
#define __DEBUG_TOOLS_H__

/* Includes ------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/

/* public variables ---------------------------------------------------------*/
extern uint32_t cpu_total_count;

/* public functions ---------------------------------------------------------*/
void memory_spce_monitor(void);
void cpu_usage_polling(void);
void cpu_usage_print(void);
uint8_t get_cpu_usage(void);

#ifdef __cplusplus
}
#endif

#endif  /* __DEBUG_TOOLS_H__ */


