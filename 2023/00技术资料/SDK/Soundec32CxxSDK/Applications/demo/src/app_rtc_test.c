/** 
***********************************************************
 * 
 * @file name	: app_rtc_test.c
 * @author		: Kevin
 * @version		: Vxx.xx
 * @date		: 2020-03-06
 * @brief		: RTC and Watch-Dog Test Application
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

#include "drv_rtc.h"
#include "drv_uart.h"

#if RTC_ENABLE == 1
void app_rtc_print_datetime(void)
{
	rtc_datetime_t datetime;
	rtc_get_datetime(&datetime);
	uart_printf("The current time is %d/%02d/%02d %02d:%02d:%02d\r\n", datetime.year, datetime.month, datetime.day, datetime.hour, datetime.minute, datetime.second);
}

void app_rtc_minute_int_cb(void)
{
	uart_printf("RTC Minute Interrupt Trigger \r\n");
	app_rtc_print_datetime();
}

void app_rtc_second_int_cb(void)
{
	//uart_printf("RTC Second Interrupt Trigger \r\n");
	app_rtc_print_datetime();
}

void app_rtc_sample_int_cb(void)
{
	uart_printf("RTC Sample Interrupt Trigger \r\n");
}

void app_wt_int_cb(void)
{
	uart_printf("Watch dog Interrupt Trigger \r\n");
//	while(1);	//Loop here forever to make the system "hang up".
}

void app_rtc_alarm_int_cb(void)
{
	rtc_alarm_config_t time;
	
	rtc_alarm_get_datetime(&time);
	
	uart_printf("RTC Alarm Interrupt Trigger %02d/%02d %02d:%02d\r\n", time.month, time.day, time.hour, time.minute);
}

void app_rtc_test_task(void)
{
	static int count = 0;
	rtc_datetime_t datetime;
//	rtc_alarm_config_t alarm;

	datetime.year = 2024;
	datetime.month = 2;
	datetime.day = 29;
	datetime.hour = 23;
	datetime.minute = 59;
	datetime.second = 50;

	rtc_init(datetime);

}

#endif
