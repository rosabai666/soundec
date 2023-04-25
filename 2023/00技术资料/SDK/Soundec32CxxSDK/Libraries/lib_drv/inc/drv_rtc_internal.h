/** 
***********************************************************
 *
 * @file name	: drv_rtc_internal.h
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-02-21
 * @brief		: Real Time Clock Driver Internal Header File
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
#ifndef __DRV_RTC_INTERNAL_H__
#define __DRV_RTC_INTERNAL_H__

/* Includes ------------------------------------------------------------------*/
#include "hal_rtc.h"
#include "drv_rtc.h"


#ifdef __cplusplus
extern "C" {
#endif
#if RTC_ENABLE == 1
/* Private typedef -----------------------------------------------------------*/
typedef bool (*rtc_get_status_t)(void);
typedef hal_status_t (*rtc_clear_int_t)(void);

/** 
 * @brief	This enum defines the ISR of RTC
 */
typedef enum {
    RTC_ISR_ALARM = 0,
    RTC_ISR_SAMPLE,
    RTC_ISR_MINUTE,
    RTC_ISR_SECOND,
    RTC_ISR_NUM
} rtc_isr_t;

/** 
 * @brief	This struct defines the ISR handler
 */
typedef struct {
    rtc_get_status_t get_status;	/* Get interrupt status */
    rtc_clear_int_t clear_int;		/* Clear interrupt */
    rtc_callback_t cb;				/* Callback function */
} rtc_isr_handler_t;


#ifdef __cplusplus
}
#endif

#endif
#endif  /* __DRV_RTC_INTERNAL_H__ */


