/** 
***********************************************************
 * 
 * @file name	: drv_pwm.c
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-02-19
 * @brief		: PWM Driver File
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

/* Includes ------------------------------------------------------------------*/
#include "hal.h"
#include "hal_pwm.h"
#include "drv_pwm.h"
#include "hal_sysctrl.h"

#if PWMX_ENABLE == 1
#ifdef __cplusplus
extern "C" {
#endif
pwm_chennel_t pwm_chennel[PWM_NUM] = {0};



/* Private functions ---------------------------------------------------------*/
/** 
 * @brief	This function initialize the specified PWM device
 * @note	None
 * @param[in]	pwm         The specified PWM device needs to be operated, this parameter can only be a value of type pwm_t
 * @param[in]	config      The configuration of the specified PWM.
 * @return	None
 */
void pwm_init(pwm_t pwmx)
{
	uint32_t clkGate = hal_sysctrl_get_clock_gate();
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_PWM);
	hal_sysctrl_set_clock_gate(clkGate);
#if(PWMX_ENABLE == 1)


	hal_pwm_control_mode((hal_pwm_sel_t)pwmx, &(pwm_chennel[pwmx].configs));
	hal_pwm_set_clock_divider((hal_pwm_sel_t)pwmx, &(pwm_chennel[pwmx].configs));
	hal_pwm_set_period((hal_pwm_sel_t)pwmx, &(pwm_chennel[pwmx].configs));
#if(PWM_GPIO_MODE_ENABLE == 0)
	hal_pwm_set_duty_cycle((hal_pwm_sel_t)pwmx, pwm_chennel[pwmx].configs.dutyCycle);
#endif
	hal_pwm_enable((hal_pwm_sel_t)pwmx);
#endif
}

/** 
 * @brief	This function de-initialize the specified PWM device
 * @note	None
 * @param[in]	pwm         The specified PWM device needs to be operated, this parameter can only be a value of type pwm_t
 * @return	None
 */
void pwm_deinit(pwm_t pwmx)
{
	hal_pwm_disable((hal_pwm_sel_t)pwmx);

	uint32_t clkGate = hal_sysctrl_get_clock_gate();
	CLRBIT(clkGate, HAL_SYSCTRL_CLKGT_PWM);
	hal_sysctrl_set_clock_gate(clkGate);
}

/**
 * @brief		Register a user i2c callback function
 * @note
 * @param[in]	callbackID  ID of the callback to be registered
 * @param[in]	pCallback   Pointer to the callback function.
 *
 * @return	None
 */
void pwm_int_register_callback(pwm_t pwmx,pwm_interrupt_type_t callbackID, pwm_int_callback pCallback)
{
	ASSERT(callbackID < PWM_INT_NUM);
	pwm_chennel[pwmx].callback[callbackID] = pCallback;
}

/**
 * @brief	This function is interrupt handle of PWM.
 * @return	None
 */
static void pwm_isr_handler(void *arg)
{
	uint8_t regValue = hal_pwm_get_int_status();
	switch(regValue)
	{
	case 0x01:
		if(pwm_chennel[HAL_PWM0].callback[PWM0_INT])
			pwm_chennel[HAL_PWM0].callback[PWM0_INT]();
		hal_pwm_interrupt_config(HAL_PWM0, PWM_INTE_DISABLE);// enable pwm interrupt
		hal_pwm_interrupt_clear(HAL_PWM0);
		break;
	case 0x02:
		hal_pwm_interrupt_clear(HAL_PWM1);
		if(pwm_chennel[HAL_PWM1].callback[PWM1_INT])
			pwm_chennel[HAL_PWM1].callback[PWM1_INT]();
		hal_pwm_interrupt_config(HAL_PWM1, PWM_INTE_DISABLE);// enable pwm interrupt
		break;
	case 0x04:
		hal_pwm_interrupt_clear(HAL_PWM2);
		if(pwm_chennel[HAL_PWM2].callback[PWM2_INT])
			pwm_chennel[HAL_PWM2].callback[PWM2_INT]();
		hal_pwm_interrupt_config(HAL_PWM2, PWM_INTE_DISABLE);// enable pwm interrupt
		break;
	case 0x08:
		hal_pwm_interrupt_clear(HAL_PWM3);
		if(pwm_chennel[HAL_PWM3].callback[PWM3_INT])
			pwm_chennel[HAL_PWM3].callback[PWM3_INT]();
		hal_pwm_interrupt_config(HAL_PWM3, PWM_INTE_DISABLE);// enable pwm interrupt
		break;
	default:
		break;
	}

}

/**
 * @brief	This function initialize the pwm interrupt.
 * @note	None
 * @param[in]	pwm         The specified PWM device needs to be operated, this parameter can only be a value of type pwm_t
 * @return	None
 */
void pwm_int_config(pwm_t pwmx)
{
	hal_pwm_interrupt_config((hal_pwm_sel_t) pwmx, PWM_INTE_ENABLE);// enable pwm interrupt
	hal_interrupt_register_isr_handler(PWM_IRQn, pwm_isr_handler);
	hal_interrupt_enable_irq(PWM_IRQn);
}
/** 
 * @brief	This function changes the duty cycle of the specified PWM device
 * @note	None
 * @param[in]	pwm         The specified PWM device needs to be operated, this parameter can only be a value of type pwm_t
 * @param[in]	duty_cycle  The duty cycle needs to be change to.
 * @return	None
 */
void pwm_change_duty_cycle(pwm_t pwmx, uint32_t duty_cycle)
{
	hal_pwm_set_duty_cycle((hal_pwm_sel_t)pwmx, duty_cycle);
}

/**
 * @brief	This function get PWM data register value.
 * @note	None
 * @param[in]	sel         The specified PWM device needs to be operated, this parameter can only be a value of type hal_pwm_sel_t
 * @return	PWM receive data
 */
uint8_t pwm_get_data(pwm_t pwmx)
{
	return hal_pwm_get_data((hal_pwm_sel_t)pwmx);
}

#ifdef __cplusplus
}
#endif

#endif
