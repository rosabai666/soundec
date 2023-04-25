/** 
***********************************************************
 * 
 * @file name	: hal_pwm.h
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-02-18
 * @brief		: HAL PWM Driver Header File
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_PWM_H__
#define __HAL_PWM_H__

/* Includes ------------------------------------------------------------------*/
#include "types.h"
//#include "common.h"

#if 1//PWMX_ENABLE == 1
#ifdef __cplusplus
extern "C" {
#endif

#define HAL_PWM_CLOCK_DIVIDER_MAX	(0xfff)
#define HAL_PWM_PERIOD_MAX			(0xffff)
#define HAL_PWM_DUTY_CYCLE_MAX		(0xffff)

#define PWM_INTE_ENABLE				0
#define PWM_INTE_DISABLE			1


/** 
 * @brief This enum defines the number of the PWM device. 
 */
typedef enum {
	HAL_PWM0 = 0,
	HAL_PWM1,
	HAL_PWM2,
	HAL_PWM3,
	HAL_PWM_NUM
} hal_pwm_sel_t;

/** 
 * @brief This enum defines the registers of the specified PWMn device. 
 */
typedef struct {
	__IO uint32_t PWM_CTRL;		/* Offset: 0x00+(0x20*n), Control Register */
	__IO uint32_t PWM_DIV;		/* Offset: 0x04+(0x20*n), Clock Divider Register */
	__IO uint32_t PWM_PERIOD;	/* Offset: 0x08+(0x20*n), Period Register */
	__IO uint32_t PWM_DATA;		/* Offset: 0x0c+(0x20*n), Data Register, is the duty cycle of the high level */
	__I  uint32_t PWM_CNT;		/* Offset: 0x10+(0x20*n), Counter Register */
	__I  uint32_t PWM_STATUS;	/* Offset: 0x14+(0x20*n), Status Register */
	uint32_t reserved[2];
} hal_pwm_reg_t;

/** 
 * @brief This enum defines the registers of all PWM devices. 
 */
typedef struct {
	hal_pwm_reg_t PWM[HAL_PWM_NUM];
	__IO uint32_t PWM_INTMASK;	/* Offset: 0x80, Interrupt Mask Register, 1:mask; 0:unmask */
	__IO uint32_t PWM_INT;/* Offset: 0x84, Interrupt Register */
	__IO uint32_t PWM_ENABLE;	/* Offset: 0x88, Enable Register */
} hal_pwm_t;

/*
 * @brief This struct defines the PWM configuration information
 */
typedef struct {
	uint32_t pwmMode;    //Configure the PWM working mode.
	uint32_t gpioDirSel; //Configure the GPIO mode input/output direction
	uint32_t matchOut;   //timer out mode.
	uint32_t clock_divider;		/* APB Bus Clock Divider. (1)The division number should be set to even. (2)If div = 0, use the APB clock. */
	uint32_t period;			/* The PWM period counter, the unit is the PWM clock divided from the APB bus. The range is [2, 0x10000) */
	uint32_t dutyCycle;		/* The PWM duty cycle, the unit is the PWM clock divided from the APB bus. The range is [1, 0x10000) */
} pwm_config_t;
/**
 * @brief	PWM module initialize
 * @note	None.
 * @param	regValue   Set pwm module parameter.
 * @param	sel      Select PWM channel.
 * 	@arg @ref PWM0,PWM1,PWM2,PWM3
 * @return
 */
void hal_pwm_control_mode(hal_pwm_sel_t sel, pwm_config_t *config);
/** 
 * @brief	This function sets the clock divider of the specified PWM from the APB bus
 * @note	None
 * @param[in]	sel 	The specified PWM device needs to be operated, this parameter can only be a value of type hal_pwm_sel_t
 * @param[in]	div		The divided factor. (1)The division number should be set to even. (2)If div = 0, use the APB clock.
 * @return	None
 */
void hal_pwm_set_clock_divider(hal_pwm_sel_t sel, pwm_config_t *config);

/** 
 * @brief	This function sets the period of the specified PWM.
 * @note	None
 * @param[in]	sel         The specified PWM device needs to be operated, this parameter can only be a value of type hal_pwm_sel_t
 * @param[in]	period		The PWM period counter, the unit is the PWM clock divided from the APB bus. The range is [2, 0x10000)
 * @return	None
 */
void hal_pwm_set_period(hal_pwm_sel_t sel, pwm_config_t *config);

/** 
 * @brief	This function sets the duty cycle of the specified PWM.
 * @note	None
 * @param[in]	sel         The specified PWM device needs to be operated, this parameter can only be a value of type hal_pwm_sel_t
 * @param[in]	duty_cycle	The PWM duty cycle, the unit is the PWM clock divided from the APB bus. The range is [1, 0x10000)
 * @return	None
 */
void hal_pwm_set_duty_cycle(hal_pwm_sel_t sel, uint32_t duty_cycle);

/**
 * @brief	This function get PWM data register value.
 * @note	None
 * @param[in]	sel         The specified PWM device needs to be operated, this parameter can only be a value of type hal_pwm_sel_t
 * @return	PWM receive data
 */
uint8_t hal_pwm_get_data(hal_pwm_sel_t sel);

/**
 * @brief	This function get PWM status.
 * @note	None
 * @param[in]	sel         The specified PWM device needs to be operated, this parameter can only be a value of type hal_pwm_sel_t
 * @return	the PWM status
 */
uint8_t hal_pwm_get_status(hal_pwm_sel_t sel);

/** 
 * @brief	This function enables the specified PWM.
 * @note	None
 * @param[in]	sel         The specified PWM device needs to be operated, this parameter can only be a value of type hal_pwm_sel_t
 * @return	None
 */
void hal_pwm_enable(hal_pwm_sel_t sel);

/** 
 * @brief	This function disables the specified PWM.
 * @note	None
 * @param[in]	sel         The specified PWM device needs to be operated, this parameter can only be a value of type hal_pwm_sel_t
 * @return	None
 */
void hal_pwm_disable(hal_pwm_sel_t sel);

/**
 * @brief	This function enable/disable the PWM interrupt.
 * @note	None
 * @param[in]	sel     The specified PWM device needs to be operated, this parameter can only
 * 					     be a value of type hal_pwm_sel_t.
 * @param[in]   enable 	the parameter can be of PWM_INTE_ENABLE or 	PWM_INTE_DISABLL
 * @return	None
 */
void hal_pwm_interrupt_config(hal_pwm_sel_t sel, uint8_t enable);

/**
 * @brief	This function clear interrupt of the PWM.
 * @note	None
 * @param[in]	sel         The specified PWM device needs to be operated, this parameter can only be a value of type hal_pwm_sel_t
 * @return	None
 */
void hal_pwm_interrupt_clear(hal_pwm_sel_t sel);

/**
 * @brief	This function get PWM interrupt status.
 * @note	None
 * @return	the PWM interrupt status
 */
uint8_t hal_pwm_get_int_status(void);
#ifdef __cplusplus
}
#endif

#endif

#endif /* __HAL_PWM_H__ */

