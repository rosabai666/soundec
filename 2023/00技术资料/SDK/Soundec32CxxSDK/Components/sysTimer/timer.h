/** 
***********************************************************
 * 
 * @file name	: timer.h
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2022-01-20
 * @brief		: This is the implement of timer modulles
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
#ifndef __TIMERTICKER_H__
#define __TIMERTICKER_H__

/* Includes ------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* Private macro -------------------------------------------------------------*/
#define TIMER0		0
#define TIMER1		1
#define TIMER2		2

/* Private typedef -----------------------------------------------------------*/
typedef void (*timer_isr_cb)(void);

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
extern void timern_init(uint8_t timer_n, uint32_t period, bool repeat, timer_isr_cb isr);

#ifdef __cplusplus
}
#endif

#endif  /* __TIMERTICKER_H__ */

