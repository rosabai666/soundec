/** 
***********************************************************
 * 
 * @file name	: drv_pwm.h
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-02-19
 * @brief		: PWM Driver Header File
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
#ifndef __DRV_PWM_H__
#define __DRV_PWM_H__

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "hal_pwm.h"
#include "boardConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#if PWMX_ENABLE == 1
/* Private typedef -----------------------------------------------------------*/

#define PWM_MODE						0x00
#define PWM_GPIO_MODE					0x01
#define PWM_TIMER1_MODE					0x02 //count a
#define PWM_TIMER2_MODE					0x03 //cycle count

#define PWM_GPIO_OUT					0x00 //GPIO is selected output
#define PWM_GPIO_IN						0x08 //input is selected input

#define PWM_OUT_LOW_LEVEL				0x00
#define PWM_OUT_HIGHT_LEVEL				0x10
#define PWM_OUT_KEPP_LEVEL				0x20
#define PWM_OUT_REVERSE_LEVEL			0x30
/* 
 * @brief This enum defines the PWM device number 
 */
typedef enum {
	PWM0 = 0,
	PWM1,
	PWM2,
	PWM3,
	PWM_NUM
} pwm_t;


typedef enum
{
	PWM0_INT = 0,
	PWM1_INT = 1,
	PWM2_INT = 2,
	PWM3_INT = 3,
	PWM_INT_NUM
} pwm_interrupt_type_t;

typedef void(*pwm_int_callback) (void);

typedef struct
{
	pwm_config_t configs;
	pwm_int_callback callback[PWM_INT_NUM];
} pwm_chennel_t;

extern pwm_chennel_t pwm_chennel[PWM_NUM];
/* Private function prototypes -----------------------------------------------*/
/** 
 * @brief	This function initialize the specified PWM device
 * @note	None
 * @param[in]	pwm         The specified PWM device needs to be operated, this parameter can only be a value of type pwm_t
 * @param[in]	config      The configuration of the specified PWM.
 * @return	None
 */
void pwm_init(pwm_t pwmx);

/** 
 * @brief	This function de-initialize the specified PWM device
 * @note	None
 * @param[in]	pwm         The specified PWM device needs to be operated, this parameter can only be a value of type pwm_t
 * @return	None
 */
void pwm_deinit(pwm_t pwmx);

/**
 * @brief		Register a user i2c callback function
 * @note
 * @param[in]	callbackID  ID of the callback to be registered
 * @param[in]	pCallback   Pointer to the callback function.
 *
 * @return	None
 */
void pwm_int_register_callback(pwm_t pwmx,pwm_interrupt_type_t callbackID, pwm_int_callback pCallback);

/** 
 * @brief	This function changes the duty cycle of the specified PWM device
 * @note	None
 * @param[in]	pwm         The specified PWM device needs to be operated, this parameter can only be a value of type pwm_t
 * @param[in]	duty_cycle  The duty cycle needs to be change to.
 * @return	None
 */
void pwm_change_duty_cycle(pwm_t pwmx, uint32_t duty_cycle);

void pwm_int_config(pwm_t pwmx);
/**
 * @brief	This function get PWM data register value.
 * @note	None
 * @param[in]	sel         The specified PWM device needs to be operated, this parameter can only be a value of type hal_pwm_sel_t
 * @return	PWM receive data
 */
uint8_t pwm_get_data(pwm_t pwmx);
#ifdef __cplusplus
}
#endif

#endif
#endif  /* __DRV_PWM_H__ */


