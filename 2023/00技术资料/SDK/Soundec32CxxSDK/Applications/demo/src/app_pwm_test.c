/** 
***********************************************************
 * 
 * @file name	: app_pwm_test.c
 * @author		: Kevin
 * @version		: Vxx.xx
 * @date		: 2020-03-06
 * @brief		: PWM Test Application
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

#include <xtensa/tie/xt_hifi2.h>
#include "hal_gpio.h"
#include "drv_pwm.h"
#include "common.h"
#include <xtensa/config/core.h>
#include <xtensa/xtruntime.h>
#include "app_pwm_test.h"

#if PWMX_ENABLE == 1
static void app_pwm_gpio(void)
{
	hal_gpio_set_pinmux_function(GPIO_12, GPIO_12_FUNC_PWM0);
	hal_gpio_set_pinmux_function(GPIO_13, GPIO_13_FUNC_PWM1);
	hal_gpio_set_pinmux_function(GPIO_14, GPIO_14_FUNC_PWM2);
	hal_gpio_set_pinmux_function(GPIO_15, GPIO_15_FUNC_PWM3);
}

void app_pwm_normal_test(void)
{
	pwm_chennel[PWM0].configs.pwmMode = PWM_MODE;
	pwm_chennel[PWM0].configs.dutyCycle = 500;
	pwm_chennel[PWM0].configs.clock_divider = 1000;	/* APB_CLOCK/80 */
	pwm_chennel[PWM0].configs.period = 1000; //
#if (PWM_INT_ENABLE == 1)
	pwm_int_register_callback(PWM0,PWM0_INT, pwm1_callback);
	pwm_int_config(PWM0);
#endif
	pwm_init(PWM0);
}

void app_pwm_gpio_mode_test(void)
{
	pwm_chennel[PWM1].configs.pwmMode = PWM_GPIO_MODE;
	pwm_chennel[PWM1].configs.gpioDirSel = PWM_GPIO_OUT;
	pwm_chennel[PWM1].configs.clock_divider = 1000;	/* APB_CLOCK/80 */
#if (PWM_INT_ENABLE == 1)
	pwm_int_register_callback(PWM1,PWM1_INT, pwm2_callback);
	pwm_int_config(PWM1);
#endif
	pwm_init(PWM1);
}

void app_pwm_timer1_mode_test(void)
{
	pwm_chennel[PWM2].configs.pwmMode = PWM_TIMER1_MODE;
	pwm_chennel[PWM2].configs.matchOut = PWM_OUT_LOW_LEVEL;
	pwm_chennel[PWM2].configs.clock_divider = 1000;	/* APB_CLOCK/80 */
	pwm_chennel[PWM2].configs.period = 4000; //timer mode 40ms hight level
#if (PWM_INT_ENABLE == 1)
	pwm_int_register_callback(PWM2,PWM2_INT, pwm3_callback);
	pwm_int_config(PWM2);
#endif
	pwm_init(PWM2);
}

void app_pwm_timer2_mode_test(void)
{
	pwm_chennel[PWM3].configs.pwmMode = PWM_TIMER2_MODE;
	pwm_chennel[PWM3].configs.matchOut = PWM_OUT_REVERSE_LEVEL;
	pwm_chennel[PWM3].configs.clock_divider = 1000;	/* APB_CLOCK/80 */
	pwm_chennel[PWM3].configs.period = 4000; //timer mode 40ms hight level
#if (PWM_INT_ENABLE == 1)
	pwm_int_register_callback(PWM3,PWM3_INT, pwm4_callback);
	pwm_int_config(PWM3);
#endif
	pwm_init(PWM3);
}

void app_pwm_test_task(void)
{
	app_pwm_gpio();
#if(PWM_NORMAL_ENABLE == 1)
	app_pwm_normal_test();
#endif
#if(PWM_GPIO_MODE_ENABLE == 1)
	app_pwm_gpio_mode_test();
#endif

#if(PWM_TIMER1_MODE_ENABLE == 1)
	app_pwm_timer1_mode_test();
#endif

#if(PWM_TIMER2_MODE_ENABLE == 1)
	app_pwm_timer2_mode_test();
#endif

}


uint32_t int_counter = 0;
void pwm1_callback(void)
{
	uint32_t arry[4] = {500,600,700,800};
	int_counter++;
	uart_printf("PWM0 ISR \r\n");
	for(int i = 0; i < 4; i++)
	{
		pwm_change_duty_cycle(PWM0, arry[i]);
	}
}

void pwm2_callback(void)
{
	uint32_t arry[4] = {0xaa,0x55,0x36,0x63};
	uart_printf("PWM1 ISR \r\n");
	for(int i = 0; i < 4;i ++)
	{
		pwm_change_duty_cycle(PWM1, arry[i]);
	}
}

void pwm3_callback(void)
{
	uart_printf("PWM2 ISR \r\n");
}

void pwm4_callback(void)
{
	uart_printf("PWM3 ISR \r\n");
}

#endif
