/** 
***********************************************************
 * 
 * @file name	: drv_rtc.c
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-02-21
 * @brief		: Real Time Clock Driver Code
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

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "hal.h"
#include "hal_rtc.h"
#include "hal_interrupt.h"
#include "drv_rtc_internal.h"
#include "hal_sysctrl.h"

#if RTC_ENABLE == 1
extern void app_rtc_minute_int_cb(void);
extern void app_rtc_second_int_cb(void);
extern void app_rtc_sample_int_cb(void);
extern void app_wt_int_cb(void);
extern void app_rtc_alarm_int_cb(void);

static rtc_isr_handler_t g_rtc_isr_table[RTC_ISR_NUM] = 
{
	/* Alarm */
	{
		hal_rtc_get_alarm_interrupt_status,
		hal_rtc_clear_alarm_interrupt,
		NULL
	},
	/* Sample */
	{
		hal_rtc_get_sample_interrupt_status,
		hal_rtc_clear_sample_interrupt,
		NULL
	},
	/* Minute */
	{
		hal_rtc_get_minute_interrupt_status,
		hal_rtc_clear_minute_interrupt,
		NULL
	},
	/* Second */
	{
		hal_rtc_get_second_interrupt_status,
		hal_rtc_clear_second_interrupt,
		NULL
	},
};

static rtc_callback_t g_wt_isr = NULL;

static void watch_dog_isr_handler(void *arg);
static void rtc_isr_handler(void *arg);

/** 
 * @brief	This function sets the datetime of RTC
 * @param[in]	datetime    The datetime is set.
 * @return	The return value of function #hal_rtc_set_datetime
 */
int rtc_set_datetime(rtc_datetime_t *datetime)
{
	return hal_rtc_set_datetime(datetime->year, datetime->month, datetime->day, \
		datetime->hour, datetime->minute, datetime->second);
}

/** 
 * @brief	This function gets the datetime of RTC
 * @param[in]	datetime    The datetime is got.
 * @return	The return value of function #hal_rtc_get_datetime
 */
int rtc_get_datetime(rtc_datetime_t *datetime)
{
	return hal_rtc_get_datetime(&datetime->year, &datetime->month, &datetime->day, \
		&datetime->hour, &datetime->minute, &datetime->second);
}

/** 
 * @brief	This function is used to initialize the alarm of RTC
 * @param[in]	config      The configuration information of Alarm
 * @param[in]	cb          The callback function of Alarm interrupt
 * @return	
 * 	@retval -1 Config alarm datetime failed
 * 	@retval  0 Alarm is successfully initialized.
 */
int rtc_alarm_init(rtc_alarm_config_t *config, rtc_callback_t cb)
{
	hal_status_t status;
	
	ASSERT(config);

	status = hal_rtc_config_alarm_datetime(config->month, config->day, config->hour, config->minute);
	if (HAL_STATUS_OK != status)
		return -1;
	
	g_rtc_isr_table[RTC_ISR_ALARM].cb = cb;
	
	hal_rtc_enable_alarm_interrupt();
	hal_interrupt_register_isr_handler(RTC_IRQn, rtc_isr_handler);
	hal_interrupt_enable_irq(RTC_IRQn);
	
	return 0;
}

/** 
 * @brief	This function is used to get the alarm datetime of RTC
 * @param[out]	datetime      The returned datetime of Alarm
 * @return	
 * 	@retval  0 The datetime is successfully gotten.
 */
int rtc_alarm_get_datetime(rtc_alarm_config_t *datetime)
{
	ASSERT(datetime);
	
	return hal_rtc_get_alarm_datetime(&datetime->month, &datetime->day, &datetime->hour, &datetime->minute);
}

/** 
 * @brief	This function is used to deinitialize the alarm of RTC
 * @return	
 * 	@retval 0 Alarm is successfully deinitialized.
 */
int rtc_alarm_deinit(void)
{
	g_rtc_isr_table[RTC_ISR_ALARM].cb = NULL;
	hal_rtc_disable_alarm_interrupt();
	
	return 0;
}

/** 
 * @brief	This function is used to initialize the sample interrupt of RTC
 * @return	
 * 	@retval 0 Sample interrupt is successfully initialized.
 */
int rtc_sample_int_init(uint16_t count, rtc_callback_t cb)
{
	g_rtc_isr_table[RTC_ISR_SAMPLE].cb = cb;
	hal_rtc_config_sample_count(count);
	hal_rtc_enable_sample_interrupt();
	hal_interrupt_register_isr_handler(RTC_IRQn, rtc_isr_handler);
	hal_interrupt_enable_irq(RTC_IRQn);

	return 0;
}

/** 
 * @brief	This function is used to deinitialize the sample interrupt of RTC
 * @return	
 * 	@retval 0 Sample interrupt is successfully deinitialized.
 */
int rtc_sample_int_deinit(void)
{
	g_rtc_isr_table[RTC_ISR_SAMPLE].cb = NULL;
	hal_rtc_disable_sample_interrupt();
	
	return 0;
}

/** 
 * @brief	This function is used to initialize the minute interrupt of RTC
 * @return	
 * 	@retval 0 Minute interrupt is successfully initialized.
 */
int rtc_minute_int_init(rtc_callback_t cb)
{
	g_rtc_isr_table[RTC_ISR_MINUTE].cb = cb;

	hal_rtc_enable_minute_interrupt();
	hal_interrupt_register_isr_handler(RTC_IRQn, rtc_isr_handler);
	hal_interrupt_enable_irq(RTC_IRQn);

	return 0;
}

/** 
 * @brief	This function is used to deinitialize the minute interrupt of RTC
 * @return	
 * 	@retval 0 Minute interrupt is successfully deinitialized.
 */
int rtc_minute_int_deinit(void)
{
	g_rtc_isr_table[RTC_ISR_MINUTE].cb = NULL;
	hal_rtc_disable_minute_interrupt();
	
	return 0;
}

/** 
 * @brief	This function is used to initialize the second interrupt of RTC
 * @return	
 * 	@retval 0 Second interrupt is successfully initialized.
 */
int rtc_second_int_init(rtc_callback_t cb)
{
	g_rtc_isr_table[RTC_ISR_SECOND].cb = cb;

	hal_rtc_enable_second_interrupt();
	hal_interrupt_register_isr_handler(RTC_IRQn, rtc_isr_handler);
	hal_interrupt_enable_irq(RTC_IRQn);

	return 0;
}

/** 
 * @brief	This function is used to deinitialize the minute interrupt of RTC
 * @return	
 * 	@retval 0 Second interrupt is successfully deinitialized.
 */
int rtc_second_int_deinit(void)
{
	g_rtc_isr_table[RTC_ISR_SECOND].cb = NULL;
	hal_rtc_disable_second_interrupt();
	
	return 0;
}

/** 
 * @brief	This function is used to configure the Watch Dog
 * @param[in]	config      The configuration information of Watch Dog
 * @param[in]	cb          The callback function of Watch Dog interrupt
 * @return	
 * 	@retval 0 Watch Dog is successfully configured.
 */
int watch_dog_config(rtc_wd_config_t *config, rtc_callback_t cb)
{
	hal_rtc_config_watch_dog(config->clk_sel, config->divider, config->reset_type, config->reload);
	g_wt_isr = cb;
	hal_rtc_enable_watch_dog();

	return 0;
}

/** 
 * @brief	This function is used to initialize the Watch Dog
 * @note	This initialization use 32.768kHz crystal as the default clock, the reset signal will be generated after the second timeout.
 * @param[in] reload The timeout value is set. The unit is 1/16 s.
 * @param[in] cb The callback function of the Watch-Dog interrupt.
 * @return	
 * 	@retval 0 Watch Dog is successfully initialized.
 */
int watch_dog_init(uint32_t reload, rtc_callback_t cb)
{
	rtc_wd_config_t config;

	config.clk_sel = HAL_RTC_WD_CLK_32K;
	config.divider = 0;		/* Invalid when clk_sel = HAL_RTC_WD_CLK_32K */
	config.reset_type = HAL_RTC_WD_RST_SECOND_TIMEOUT;
	config.reload = reload;	/* reload/16 (s) */

	watch_dog_config(&config, cb);
	hal_interrupt_register_isr_handler(WATCH_DOG_IRQn, watch_dog_isr_handler);
	hal_interrupt_enable_irq(WATCH_DOG_IRQn);
	
	return 0;
}

/** 
 * @brief	This function is used to deinitialize the Watch Dog
 * @return	
 * 	@retval 0 Watch Dog is successfully deinitialized.
 */
int watch_dog_deinit(void)
{
	hal_rtc_disable_watch_dog();
	hal_interrupt_disable_irq(WATCH_DOG_IRQn);
	
	return 0;
}

/** 
 * @brief	This function is used to pause the Watch Dog
 * @return	
 * 	@retval 0 Watch Dog is successfully paused.
 */
int watch_dog_pause(void)
{
	hal_rtc_pause_watch_dog();
	
	return 0;
}

/** 
 * @brief	This function is used to resume the Watch Dog from pause state
 * @return	
 * 	@retval 0 Watch Dog is successfully resumed.
 */
int watch_dog_resume(void)
{
	hal_rtc_resume_watch_dog();
	
	return 0;
}

/** 
 * @brief	This function is the ISR of the RTC
 */
static void rtc_isr_handler(void *arg)
{
	int i = 0;

	for (; i < RTC_ISR_NUM; i++) {
		/* If there is an interrupt triggerred, handle it */
		if (g_rtc_isr_table[i].get_status())
		{
			g_rtc_isr_table[i].clear_int();	/* Clear interrupt */

			/* Call the callback function */
			if (g_rtc_isr_table[i].cb)
				g_rtc_isr_table[i].cb();
		}
	}
}

/** 
 * @brief	This function is the ISR of the Watch-Dog
 */
static void watch_dog_isr_handler(void *arg)
{
	/* If there is an interrupt triggerred, handle it */
	if (hal_rtc_get_watch_dog_interrupt_status())
	{
		hal_rtc_clear_watch_dog_interrupt();	/* Clear interrupt */
		if (g_wt_isr) g_wt_isr();
	}
}

/**
 * @brief	This function is used to initialize the RTC
 * @param[in]	datetime    The datetime is set.
 * @return	None
 */
void rtc_init(rtc_datetime_t datetime)
{
	rtc_alarm_config_t alarm;
	uint32_t clkGate = hal_sysctrl_get_clock_gate();
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_RTC);
	hal_sysctrl_set_clock_gate(clkGate);
#if	RTC_ENABLE
	rtc_set_datetime(&datetime);
#if RTC_ALARM_INT_ENABLE
	alarm.month = 3;
	alarm.day = 1;
	alarm.hour = 0;
	alarm.minute = 01;
	if (rtc_alarm_init(&alarm, app_rtc_alarm_int_cb) !=0)
		uart_printf("RTC Alarm initialized failed\r\n");
#endif
#if RTC_MIN_INT_ENABLE
	rtc_minute_int_init(app_rtc_minute_int_cb);
#endif
#if RTC_SEC_INT_ENABLE
	rtc_second_int_init(app_rtc_second_int_cb);
#endif
#if RTC_SAMP_INT_ENABLE
	rtc_sample_int_init(32768, app_rtc_sample_int_cb);
#endif
#if RTC_WD_INT_ENABLE
	watch_dog_init(64, app_wt_int_cb);//about 4s produces an  interrupt
#endif
#endif
}

/**
 * @brief	This function is used to deinitialize the RTC
 * @param[in]	None
 * @return	None
 */
void rtc_deinit(void)
{
	rtc_alarm_deinit();
	rtc_sample_int_deinit();
	rtc_minute_int_deinit();
	rtc_second_int_deinit();
	watch_dog_deinit();
	uint32_t clkGate = hal_sysctrl_get_clock_gate();
	CLRBIT(clkGate, HAL_SYSCTRL_CLKGT_RTC);
	hal_sysctrl_set_clock_gate(clkGate);
}

#ifdef __cplusplus
}
#endif

#endif

