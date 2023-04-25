/** 
***********************************************************
 *
 * @file name	: drv_rtc.h
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-02-21
 * @brief		: Real Time Clock Driver Header File
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
#ifndef __DRV_RTC_H__
#define __DRV_RTC_H__

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "boardConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#if RTC_ENABLE == 1
typedef void (*rtc_callback_t)(void);

/** 
 * @brief	This struct defines the datetime settings
 */
typedef struct {
    uint32_t year;
    uint32_t month;
    uint32_t day;
    uint32_t hour;
    uint32_t minute;
    uint32_t second;
} rtc_datetime_t;

/** 
 * @brief	This struct defines the datetime settings for alarm
 */
typedef struct {
    uint32_t month;
    uint32_t day;
    uint32_t hour;
    uint32_t minute;
} rtc_alarm_config_t;

/** 
 * @brief	This struct defines the configuration settings for watch dog
 */
typedef struct {
    uint32_t clk_sel;	/* The source clock of Watch-Dog, it can only be one of the type #hal_rtc_wd_clock_t */
    uint32_t divider;	/* The clock divider of the APB bus when the source clock is divided APB bus. The acture divider is (#divide+1)*2 */
    uint32_t reload;	/* Watch-Dog reload value. When the clock of Watch-Dog is 32.768kHz(whether the source clock is 32.768kHz crystal or not), the unit is 1/16 second. */
    uint32_t reset_type;/* The reset control type, this parameter can only be one of the type #hal_rtc_wd_rst_ctrl_t */
} rtc_wd_config_t;


/* Private functions ---------------------------------------------------------*/
/** 
 * @brief	This function sets the datetime of RTC
 * @param[in]	datetime    The datetime is set.
 * @return	The return value of function #hal_rtc_set_datetime
 */
int rtc_set_datetime(rtc_datetime_t *datetime);

/** 
 * @brief	This function gets the datetime of RTC
 * @param[in]	datetime    The datetime is got.
 * @return	The return value of function #hal_rtc_get_datetime
 */
int rtc_get_datetime(rtc_datetime_t *datetime);

/** 
 * @brief	This function is used to initialize the alarm of RTC
 * @param[in]	config      The configuration information of Alarm
 * @param[in]	cb          The callback function of Alarm interrupt
 * @return
 * 	@retval -1 Config alarm datetime failed
 * 	@retval  0 Alarm is successfully initialized.
 */
int rtc_alarm_init(rtc_alarm_config_t *config, rtc_callback_t cb);

/** 
 * @brief	This function is used to deinitialize the alarm of RTC
 * @return
 * 	@retval 0 Alarm is successfully deinitialized.
 */
int rtc_alarm_deinit(void);

/** 
 * @brief	This function is used to get the alarm datetime of RTC
 * @param[out]	datetime      The returned datetime of Alarm
 * @return
 * 	@retval  0 The datetime is successfully gotten.
 */
int rtc_alarm_get_datetime(rtc_alarm_config_t *datetime);

/** 
 * @brief	This function is used to initialize the sample interrupt of RTC
 * @return
 * 	@retval 0 Sample interrupt is successfully initialized.
 */
int rtc_sample_int_init(uint16_t count, rtc_callback_t cb);

/** 
 * @brief	This function is used to deinitialize the sample interrupt of RTC
 * @return
 * 	@retval 0 Sample interrupt is successfully deinitialized.
 */
int rtc_sample_int_deinit(void);

/** 
 * @brief	This function is used to initialize the minute interrupt of RTC
 * @return
 * 	@retval 0 Minute interrupt is successfully initialized.
 */
int rtc_minute_int_init(rtc_callback_t cb);

/** 
 * @brief	This function is used to deinitialize the minute interrupt of RTC
 * @return
 * 	@retval 0 Minute interrupt is successfully deinitialized.
 */
int rtc_minute_int_deinit(void);

/** 
 * @brief	This function is used to initialize the second interrupt of RTC
 * @return
 * 	@retval 0 Second interrupt is successfully initialized.
 */
int rtc_second_int_init(rtc_callback_t cb);

/** 
 * @brief	This function is used to deinitialize the minute interrupt of RTC
 * @return
 * 	@retval 0 Second interrupt is successfully deinitialized.
 */
int rtc_second_int_deinit(void);

/** 
 * @brief	This function is used to configure the Watch Dog
 * @param[in]	config      The configuration information of Watch Dog
 * @param[in]	cb          The callback function of Watch Dog interrupt
 * @return
 * 	@retval 0 Watch Dog is successfully configured.
 */
int watch_dog_config(rtc_wd_config_t *config, rtc_callback_t cb);

/** 
 * @brief	This function is used to initialize the Watch Dog
 * @note	This initialization use 32.768kHz crystal as the default clock, the reset signal will be generated after the second timeout.
 * @param[in] reload The timeout value is set. The unit is 1/16 s.
 * @param[in] cb The callback function of the Watch-Dog interrupt.
 * @return
 * 	@retval 0 Watch Dog is successfully initialized.
 */
int watch_dog_init(uint32_t reload, rtc_callback_t cb);

/** 
 * @brief	This function is used to deinitialize the Watch Dog
 * @return
 * 	@retval 0 Watch Dog is successfully deinitialized.
 */
int watch_dog_deinit(void);

/** 
 * @brief	This function is used to pause the Watch Dog
 * @return
 * 	@retval 0 Watch Dog is successfully paused.
 */
int watch_dog_pause(void);

/** 
 * @brief	This function is used to resume the Watch Dog from pause state
 * @return
 * 	@retval 0 Watch Dog is successfully resumed.
 */
int watch_dog_resume(void);

/**
 * @brief	This function is used to initialize the RTC
 * @param[in]	datetime    The datetime is set.
 * @return	None
 */
void rtc_init(rtc_datetime_t datetime);
void rtc_deinit(void);
#ifdef __cplusplus
}
#endif

#endif
#endif  /* __DRV_RTC_H__ */


