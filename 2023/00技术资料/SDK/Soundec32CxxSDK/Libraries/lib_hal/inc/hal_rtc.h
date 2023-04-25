/** 
***********************************************************
 * @attention
 * 
 * Copyright (c) 2019 Soundec Co., Ltd. 
 * All rights reserved.
 * 
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user
 * who uses the software, his/her employer (which must be your employer
 * and Soundec Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 * 
***********************************************************
 * 
 * @file name   : hal_rtc.h
 * @author      : chenjb
 * @version     : V01.00
 * @date        : 2019-11-15
 * @brief       : RTC & Watch Dog driver header file
 * 
***********************************************************/



#ifndef __HAL_RTC_H__
#define __HAL_RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal.h"
#include "types.h"

#if 1//RTC_ENABLE == 1
#define HAL_RTC_CFG_CHK_VALUE			(0xaaaaaaaa)

#define HAL_RTC_YEAR_MIN				(2008)
#define HAL_RTC_YEAR_MAX				(2047)
#define HAL_RTC_MONTH_MIN				(1)
#define HAL_RTC_MONTH_MAX				(12)
#define HAL_RTC_DAY_MIN					(1)
#define HAL_RTC_HOUR_MAX				(23)
#define HAL_RTC_MINUTE_MAX				(59)
#define HAL_RTC_SECOND_MAX				(59)
                                		
/* RTC_STA_YMD */               		
#define HAL_RTC_YEAR_SHIFT				(9)
#define HAL_RTC_YEAR_MASK				(0x7ff << HAL_RTC_YEAR_SHIFT)
#define HAL_RTC_MONTH_SHIFT				(5)
#define HAL_RTC_MONTH_MASK				(0xf << HAL_RTC_MONTH_SHIFT)
#define HAL_RTC_DAY_SHIFT				(0)
#define HAL_RTC_DAY_MASK				(0x1f << HAL_RTC_DAY_SHIFT)
                                		
/* RTC_STA_HMS */               		
#define HAL_RTC_HOUR_SHIFT				(12)
#define HAL_RTC_HOUR_MASK				(0x1f << HAL_RTC_HOUR_SHIFT)
#define HAL_RTC_MIN_SHIFT				(6)
#define HAL_RTC_MIN_MASK				(0x3f << HAL_RTC_MIN_SHIFT)
#define HAL_RTC_SEC_SHIFT				(0)
#define HAL_RTC_SEC_MASK				(0x3f << HAL_RTC_SEC_SHIFT)

/* RTC_ALARM */
#define HAL_RTC_ALARM_MONTH_SHIFT		(28)
#define HAL_RTC_ALARM_MONTH_MASK		(0xf << HAL_RTC_ALARM_MONTH_SHIFT)
#define HAL_RTC_ALARM_DAY_SHIFT			(23)
#define HAL_RTC_ALARM_DAY_MASK			(0x1f << HAL_RTC_ALARM_DAY_SHIFT)
#define HAL_RTC_ALARM_HOUR_SHIFT		(6)
#define HAL_RTC_ALARM_HOUR_MASK			(0x1f << HAL_RTC_ALARM_HOUR_SHIFT)
#define HAL_RTC_ALARM_MINUTE_SHIFT		(0)
#define HAL_RTC_ALARM_MINUTE_MASK		(0x3f << HAL_RTC_ALARM_MINUTE_SHIFT)

/* RTC_CTR */
#define HAL_RTC_CFG_STATE_SHIFT			(31)
#define HAL_RTC_CFG_STATE_MASK			(0x1 << HAL_RTC_CFG_STATE_SHIFT)
#define HAL_RTC_WD_RST_CTRL_SHIFT		(25)
#define HAL_RTC_WD_RST_CTRL_MASK		(0x1 << HAL_RTC_WD_RST_CTRL_SHIFT)
#define HAL_RTC_WD_CLK_SEL_SHIFT		(24)
#define HAL_RTC_WD_CLK_SEL_MASK			(0X1 << HAL_RTC_WD_CLK_SEL_SHIFT)
#define HAL_RTC_WD_CLK_DIVIDED_SHIFT	(8)
#define HAL_RTC_WD_CLK_DIVIDED_MASK		(0xffff << HAL_RTC_WD_CLK_DIVIDED_SHIFT)
#define HAL_RTC_WD_PAUSE_CFG_SHIFT		(5)
#define HAL_RTC_WD_PAUSE_CFG_MASD		(0x1 << HAL_RTC_WD_PAUSE_CFG_SHIFT)
#define HAL_RTC_WD_PAUSE_SHIFT			(4)
#define HAL_RTC_WD_PAUSE_MASK			(0x1 << HAL_RTC_WD_PAUSE_SHIFT)
#define HAL_RTC_WD_EN_CFG_SHIFT			(3)
#define HAL_RTC_WD_EN_CFG_MASK			(0x1 << HAL_RTC_WD_EN_CFG_SHIFT)
#define HAL_RTC_WD_EN_SHIFT				(2)
#define HAL_RTC_WD_EN_MASK				(0x1 << HAL_RTC_WD_EN_SHIFT)
#define HAL_RTC_SAMPLE_EN_CFG_SHIFT		(1)
#define HAL_RTC_SAMPLE_EN_CFG_MASK		(0x1 << HAL_RTC_SAMPLE_EN_CFG_SHIFT)
#define HAL_RTC_SAMPLE_EN_SHIFT			(0)
#define HAL_RTC_SAMPLE_EN_MASK			(0x1 << HAL_RTC_SAMPLE_EN_SHIFT)

/* RTC_INT_EN */
#define HAL_RTC_WD_RESET_EN_SHIFT		(5)
#define HAL_RTC_WD_RESET_EN_MASK		(0x1 << HAL_RTC_WD_RESET_EN_SHIFT)
#define HAL_RTC_ALARM_INT_EN_SHIFT		(4)
#define HAL_RTC_ALARM_INT_EN_MASK		(0x1 << HAL_RTC_ALARM_INT_EN_SHIFT)
#define HAL_RTC_MIN_INT_EN_SHIFT		(3)
#define HAL_RTC_MIN_INT_EN_MASK			(0x1 << HAL_RTC_MIN_INT_EN_SHIFT)
#define HAL_RTC_SEC_INT_EN_SHIFT		(2)
#define HAL_RTC_SEC_INT_EN_MASK			(0x1 << HAL_RTC_SEC_INT_EN_SHIFT)
#define HAL_RTC_WD_INT_EN_SHIFT			(1)
#define HAL_RTC_WD_INT_EN_MASK			(0x1 << HAL_RTC_WD_INT_EN_SHIFT)
#define HAL_RTC_SAMPLE_INT_EN_SHIFT		(0)
#define HAL_RTC_SAMPLE_INT_EN_MASK		(0x1 << HAL_RTC_SAMPLE_INT_EN_SHIFT)

/* RTC_INT_STS */
#define HAL_RTC_ALARM_INT_FLAG_SHIFT	(4)
#define HAL_RTC_ALARM_INT_FLAG_MASK		(0x1 << HAL_RTC_ALARM_INT_FLAG_SHIFT)
#define HAL_RTC_MIN_INT_FLAG_SHIFT		(3)
#define HAL_RTC_MIN_INT_FLAG_MASK		(0x1 << HAL_RTC_MIN_INT_FLAG_SHIFT)
#define HAL_RTC_SEC_INT_FLAG_SHIFT		(2)
#define HAL_RTC_SEC_INT_FLAG_MASK		(0x1 << HAL_RTC_SEC_INT_FLAG_SHIFT)
#define HAL_RTC_WD_INT_FLAG_SHIFT		(1)
#define HAL_RTC_WD_INT_FLAG_MASK		(0x1 << HAL_RTC_WD_INT_FLAG_SHIFT)
#define HAL_RTC_SAMPLE_INT_FLAG_SHIFT	(0)
#define HAL_RTC_SAMPLE_INT_FLAG_MASK	(0x1 << HAL_RTC_SAMPLE_INT_FLAG_SHIFT)

/* RTC_SAMPLE */
#define HAL_RTC_SAMPLE_RELOAD_SHIFT		(16)
#define HAL_RTC_SAMPLE_RELOAD_MASK		(0xffff << HAL_RTC_SAMPLE_RELOAD_SHIFT)
#define HAL_RTC_SAMPLE_SYNC_SHIFT		(0)
#define HAL_RTC_SAMPLE_SYNC_MASK		(0xffff << HAL_RTC_SAMPLE_SYNC_SHIFT)

/* RTC_WD_CNT */
#define HAL_RTC_WD_RELOAD_SHIFT			(16)
#define HAL_RTC_WD_RELOAD_MASK			(0xff << HAL_RTC_WD_RELOAD_SHIFT)
#define HAL_RTC_WD_SYNC_SHIFT			(0)
#define HAL_RTC_WDE_SYNC_MASK			(0xff << HAL_RTC_WD_SYNC_SHIFT)


/** 
 * @brief This enum defines the watch dog reset control type. 
 */
typedef enum {
	HAL_RTC_WD_RST_FIRST_TIMEOUT = 0,	/*!< Reset after the first timeout */
	HAL_RTC_WD_RST_SECOND_TIMEOUT,		/*!< Reset after the second timeout */
	HAL_RTC_WD_RST_CTRL_NUM
} hal_rtc_wd_rst_ctrl_t;

/** 
 * @brief	This enum defines the source clock of the Watch-Dog
 */
typedef enum {
	HAL_RTC_WD_CLK_32K = 0,		/*!< Select 32.768kHz crystal as the watch dog clock */
	HAL_RTC_WD_CLK_BUS,			/*!< Select the bus divided clock as the watch dog clock */
	HAL_RTC_WD_CLK_NUM
} hal_rtc_wd_clock_t;

/** 
 * @brief	This struct defines the registers of the RTC module
 */
typedef struct {
	__IO uint32_t RTC_STA_YMD;				/*!< Offset: 0x00, Year/Month/Day register */
	__IO uint32_t RTC_STA_HMS;				/*!< Offset: 0x04, Hour/Minute/Second register */
	__IO uint32_t RTC_ALARM;			/*!< Offset: 0x08, Alarm register */
	__IO uint32_t RTC_CTR;				/*!< Offset: 0x0c, Control register */
	__IO uint32_t RTC_INT_EN;			/*!< Offset: 0x10, Interrupt enable register */
	__IO uint32_t RTC_INT_STS;		/*!< Offset: 0x14, Interrupt Status register */
	__IO uint32_t RTC_SAMP;			/*!< Offset: 0x18, Sample regtster */
	__IO uint32_t RTC_WD_CNT;			/*!< Offset: 0x1c, Watch Dog Counter register */
		 uint32_t reserved;			/*!< Offset: 0x20 */
	__IO uint32_t RTC_CFG_CHK;			/*!< Offset: 0x24, Datetime setting confirm register */
} hal_rtc_t;


/** 
 * @brief	This function checks whether the given year is leap year or not
 * @param[in]	y           The year is checked.
 * @return	Leap year or not
 * 	@retval true It's a leap year.
 * 	@retval false It'a not a leap year
 */
bool hal_rtc_get_leap_year(uint32_t y);

/** 
 * @brief	This function get the days of the month in given year
 * @param[in]	year        Year
 * @param[in]	month       Month
 * @return	The days of the month
 */
uint32_t hal_rtc_get_max_day(uint32_t year, uint32_t month);

/** 
 * @brief	This function sets the datetime of RTC
 * @param[in]	year        The year to be set.
 * @param[in]	month       The month to be set.
 * @param[in]	day         The day to be set.
 * @param[in]	hour        The hour to be set.
 * @param[in]	min         The minute to be set.
 * @param[in]	sec         The second to be set.
 * @return	
 * 	@retval HAL_STATUS_OK The datetime is successfully set. 
 * 	@retval HAL_STATUS_TIMEOUT Set the datetime timeout.
 */
hal_status_t hal_rtc_set_datetime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t min, uint32_t sec);

/** 
 * @brief	This function gets the date time from RTC
 * @param[out]	year        Year get from RTC
 * @param[out]	month       Month get from RTC
 * @param[out]	day         Day get from RTC
 * @param[out]	hour        Hour get from RTC
 * @param[out]	min         Minute get from RTC
 * @param[out]	sec         Second get from RTC
 * @return	
 * 	@retval HAL_STATUS_OK The date time is successfully get from RTC
 */
hal_status_t hal_rtc_get_datetime(uint32_t *year, uint32_t *month, uint32_t *day, uint32_t *hour, uint32_t *min, uint32_t *sec);

/** 
 * @brief	This function configures the alarm
 * @note	When the RTC time reaches the datetime set by alarm, it will trigger an alarm interrupt.
 * @param[in]	month       The month of the alarm
 * @param[in]	day         The day of the alarm
 * @param[in]	hour        The hour of the alarm
 * @param[in]	min         The minute of the alarm
 * @return
 * 	@retval HAL_STATUS_OK The alarm is successfully configured. 
 * 	@retval HAL_STATUS_TIMEOUT Configure the alarm timeout.
 */
hal_status_t hal_rtc_config_alarm_datetime(uint32_t month, uint32_t day, uint32_t hour, uint32_t min);

/** 
 * @brief	This function gets the alarm
 * @param[out]	month       The returned month of the alarm
 * @param[out]	day         The returned day of the alarm 
 * @param[out]	hour        The returned hour of the alarm
 * @param[out]	min         The returned minute of the alarm
 * @return
 * 	@retval HAL_STATUS_OK The alarm is successfully gotten. 
 */
hal_status_t hal_rtc_get_alarm_datetime(uint32_t *month, uint32_t *day, uint32_t *hour, uint32_t *min);

/** 
 * @brief	This function enalbes Alarm interrupt
 * @return	
 * 	@retval HAL_STATUS_OK Alarm is successfully enabled.
 */
hal_status_t hal_rtc_enable_alarm_interrupt(void);

/** 
 * @brief	This function disalbes Alarm interrupt
 * @return	
 * 	@retval HAL_STATUS_OK Alarm is successfully disabled.
 */
hal_status_t hal_rtc_disable_alarm_interrupt(void);

/** 
 * @brief	This function clears the interrupt of the Alarm
 * @return	
 * 	@retval HAL_STATUS_OK The interrupt of the Alarm is successfully cleared.
 */
hal_status_t hal_rtc_clear_alarm_interrupt(void);

/** 
 * @brief	This function checks whether there is an alarm interrupt or not
 * @return	
 * 	@retval ture There is an alarm interrupt.
 * 	@retval false There is no alarm interrupt.
 */
bool hal_rtc_get_alarm_interrupt_status(void);

/** 
 * @brief	This function configures the sample counter. The unit of sample counter is 1/32768 second.
 * @param[in]	count       The sample counter is configured.
 * 	@arg @ref xx {description if necessary}
 * @return
 * 	@retval HAL_STATUS_OK The sample counter is successfully configured.
 */
hal_status_t hal_rtc_config_sample_count(uint16_t count);

/** 
 * @brief	This function enables the sample interrupt
 * @note	Enable the sample interrupt, when the sample counter decrease to 1, it will trigger a sample interrupt
 * @return	
 * 	@retval HAL_STATUS_OK The sample interrupt is successfully enabled.
 */
hal_status_t hal_rtc_enable_sample_interrupt(void);

/** 
 * @brief	This function disables the sample interrupt
 * @return	
 * 	@retval HAL_STATUS_OK The sample interrupt is successfully disabled.
 */
hal_status_t hal_rtc_disable_sample_interrupt(void);

/** 
 * @brief	This function clears the sample interrupt
 * @return	
 * 	@retval HAL_STATUS_OK The sample interrupt is successfully cleared.
 */
hal_status_t hal_rtc_clear_sample_interrupt(void);

/** 
 * @brief	This function checks whether there is a sample interrupt or not
 * @return	
 * 	@retval ture There is a sample interrupt.
 * 	@retval false There is no sample interrupt.
 */
bool hal_rtc_get_sample_interrupt_status(void);

/** 
 * @brief	This function enables the minute interrupt
 * @note	Enable the minute interrupt, when the second of RTC changes to 0, it will trigger a minute interrupt
 * @return	
 * 	@retval HAL_STATUS_OK The minute interrupt is successfully enabled.
 */
hal_status_t hal_rtc_enable_minute_interrupt(void);

/** 
 * @brief	This function disables the minute interrupt
 * @return	
 * 	@retval HAL_STATUS_OK The minute interrupt is successfully disabled.
 */
hal_status_t hal_rtc_disable_minute_interrupt(void);

/** 
 * @brief	This function clears the minute interrupt
 * @return	
 * 	@retval HAL_STATUS_OK The minute interrupt is successfully cleared.
 */
hal_status_t hal_rtc_clear_minute_interrupt(void);

/** 
 * @brief	This function checks whether there is a minute interrupt or not
 * @return	
 * 	@retval ture There is a minute interrupt.
 * 	@retval false There is no minute interrupt.
 */
bool hal_rtc_get_minute_interrupt_status(void);

/** 
 * @brief	This function enables the second interrupt
 * @note	Enable the second interrupt, it will trigger a minute interrupt every second
 * @return	
 * 	@retval HAL_STATUS_OK The second interrupt is successfully enabled.
 */
hal_status_t hal_rtc_enable_second_interrupt(void);

/** 
 * @brief	This function disables the second interrupt
 * @return	
 * 	@retval HAL_STATUS_OK The second interrupt is successfully disabled.
 */
hal_status_t hal_rtc_disable_second_interrupt(void);

/** 
 * @brief	This function clears the second interrupt
 * @return	
 * 	@retval HAL_STATUS_OK The second interrupt is successfully cleared.
 */
hal_status_t hal_rtc_clear_second_interrupt(void);

/** 
 * @brief	This function checks whether there is a second interrupt or not
 * @return	
 * 	@retval ture There is a second interrupt.
 * 	@retval false There is no second interrupt.
 */
bool hal_rtc_get_second_interrupt_status(void);

/** 
 * @brief	This function configures the Watch-Dog
 * @note	{description}
 * @param[in]	clk_sel     Select source clock of Watch-Dog, this parameter can only be one of the type #hal_rtc_wd_clock_t
 * @param[in]	divide      The clock divider of the APB bus when the source clock is divided APB bus. The acture divider is (#divide+1)*2
 * @param[in]	reset_ctrl  The reset control type, this parameter can only be one of the type #hal_rtc_wd_rst_ctrl_t
 * @param[in]	reload      Watch-Dog reload value. When the clock of Watch-Dog is 32.768kHz(whether the source clock is 32.768kHz crystal or not), the unit is 1/16 second.
 * @return	
 * 	@retval HAL_STATUS_OK The Watch-Dog is successfully configured.
 */
hal_status_t hal_rtc_config_watch_dog(hal_rtc_wd_clock_t clk_sel, uint32_t divide, hal_rtc_wd_rst_ctrl_t reset_ctrl, uint8_t reload);

/** 
 * @brief	This function enables the Watch-Dog and interrupt
 * @note	Enable the Watch-Dog interrupt, it will trigger a Watch-Dog interrupt when the counter decrease to 0.
 * @return	
 * 	@retval HAL_STATUS_OK The Watch-Dog interrupt is successfully enabled.
 */
hal_status_t hal_rtc_enable_watch_dog(void);

/** 
 * @brief	This function disables the Watch-Dog and interrupt
 * @return	
 * 	@retval HAL_STATUS_OK The Watch-Dog interrupt is successfully disabled.
 */
hal_status_t hal_rtc_disable_watch_dog(void);

/** 
 * @brief	This function is used to feed the Watch-Dog by resetting the reload counter
 * @return	
 * 	@retval HAL_STATUS_OK The Watch-Dog is successfully feeded.
 */
hal_status_t hal_rtc_feed_watch_dog(uint8_t reload);

/** 
 * @brief	This function is used to pause the Watch-Dog.
 * @return	
 * 	@retval HAL_STATUS_OK The Watch-Dog is successfully paused.
 */
hal_status_t hal_rtc_pause_watch_dog(void);

/** 
 * @brief	This function is used to resume the Watch-Dog.
 * @return	
 * 	@retval HAL_STATUS_OK The Watch-Dog is successfully resumed.
 */
hal_status_t hal_rtc_resume_watch_dog(void);

/** 
 * @brief	This function is used to clear the Watch-Dog interrupt.
 * @return	
 * 	@retval HAL_STATUS_OK The Watch-Dog interupt is successfully cleared.
 */
hal_status_t hal_rtc_clear_watch_dog_interrupt(void);

/** 
 * @brief	This function checks whether there is a Watch-Dog interrupt or not
 * @return	
 * 	@retval ture There is a Watch-Dog interrupt.
 * 	@retval false There is no Watch-Dog interrupt.
 */
bool hal_rtc_get_watch_dog_interrupt_status(void);

#endif
#ifdef __cplusplus
}
#endif

#endif /* __HAL_RTC_H__ */
