/** 
***********************************************************
 * 
 * @file name	: hal_interrupt.h
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-02-20
 * @brief		: HAL interrupt header file
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


#ifndef __HAL_INTERRUPT_H__
#define __HAL_INTERRUPT_H__

#include "snc8600.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 	This enum defines the hardware interrupt sources
 */
typedef enum {
	INTR_SRC_USB 				= 0,
	INTR_SRC_USB_DMA 			= 1,
	INTR_SRC_ADC12				= 2,
	INTR_SRC_ADC34				= 3,
	INTR_SRC_ADC56				= 4,
	INTR_SRC_ADC78				= 5,
	INTR_SRC_ADC9A				= 6,
	INTR_SRC_DAC				= 7,
	INTR_SRC_EXT_SW0			= 8,
	INTR_SRC_I2S1				= 9,
	INTR_SRC_I2S2				= 10,
	INTR_SRC_I2S3				= 11,
	INTR_SRC_DMAC				= 12,
	INTR_SRC_PMU				= 13,
	INTR_SRC_CODEC				= 14,
	INTR_SRC_CODEC_WT			= 15,
	INTR_SRC_I2C1				= 16,
	INTR_SRC_I2C2				= 17,
	INTR_SRC_UART				= 18,
	INTR_SRC_GPIO_678			= 19,
	INTR_SRC_RTC				= 20,
	INTR_SRC_WATCH_DOG			= 21,
	INTR_SRC_GPIO_9ABCDEF		= 22,
	INTR_SRC_USB_SLEEP			= 23,
	INTR_SRC_SARADC				= 24,
	INTR_SRC_PWM				= 25,
	INTR_SRC_EXT_SW1			= 26,
	INTR_SRC_EXT_SW2			= 27,
	INTR_SRC_EXT_SW3			= 28,
	INTR_SRC_EXT_SW4			= 29,
	INTR_SRC_NUM				= 30,
} hal_interrupt_intr_src_t;


/** 
 * @brief  This defines the callback function prototype.
 *          User should register a callback (an ISR) for each interrupt IRQ in use.
 *          The callback function is called in the interrupt ISR after the interrupt IRQ is triggered.
 *          For more details about the callback, please refer to #hal_interrupt_register_isr_handler().
 */
typedef void (*hal_interrupt_isr_t)(void *arg);


/*****************************************************************************
* Functions
*****************************************************************************/

/** 
 * @brief	This function is used to register a callback (an ISR) for interrupt IRQ.
 * @note	None
 * @param[in]	irq_number  It is the interrupt IRQ number.
 * @param[in]	isr_handler It is the interrupt IRQ's ISR.
 * @return	None
 */
void hal_interrupt_register_isr_handler(hal_interrupt_irq_t irq_number, hal_interrupt_isr_t isr_handler);


/**
 * @brief This function initializes the interrupt IRQ.
 * @return	None
 */
void hal_interrupt_init(void);

/**
 * @brief 	This function enables a device-specific interrupt in the interrupt interrupt controller.
 * @param[in] 	irq_number 	It is the interrupt IRQ number.
 * @return	None
 */
void hal_interrupt_enable_irq(hal_interrupt_irq_t irq_number);

/**
 * @brief 	This function disables a device-specific interrupt in the interrupt interrupt controller.
 * @param[in] 	irq_number 	It is the interrupt IRQ number.
 * @return	None
 */
void hal_interrupt_disable_irq(hal_interrupt_irq_t irq_number);

/**
 * @brief 	This function is used to clear a device-specific interrupt in the interrupt interrupt controller.
 * @param[in] 	irq_number 	It is the interrupt IRQ number.
 * @return	None
 */
void hal_interrupt_clear_irq(hal_interrupt_irq_t irq_number);

/**
 * @brief 	This function saves the current IRQ settings in a temporary variable, and then disables the IRQ by setting the IRQ mask.
 * @note	It should be used in conjunction with #hal_interrupt_restore_interrupt_mask() to protect the critical resources.
 * @param[out] 	mask 	It's used to store the current IRQ settings, upon the return of this function.
 * @return	None
 */
void hal_interrupt_save_and_set_interrupt_mask(uint32_t *mask);

/**
 * @brief 	This function restores the IRQ settings as specified in the mask. 
 * @note	It should be used in conjunction with #hal_interrupt_save_and_set_interrupt_mask() to protect critical resources.
 * @param[in] 	mask 	It is an unsigned integer to specify the IRQ settings.
 * @return	None
 */
void hal_interrupt_restore_interrupt_mask(uint32_t mask);


/**
 * @brief 	This function is for trigger DSP SW IRQ
 * @param[in] 	irq_number 	It is IRQ number.
 * @return	None
 */
void hal_interrupt_irq_software_trigger(hal_interrupt_irq_t irq_number);

/**
 * @brief 	This function force to trigger a specified interrupt by software
 * @note If call #hal_interrupt_irq_force_trigger to trigger an interrupt, it should call #hal_interrupt_irq_clear_force_trigger in the IRQ handler to clear the force trigger flag
 * @param[in] irq The interrupt will be triggered, it should be one of the value #hal_interrupt_intr_src_t
 * @return	None
 */
void hal_interrupt_irq_force_trigger(int irq);

/**
 * @brief	This function clears the specified force interrupt trigger flag
 * @note If call #hal_interrupt_irq_force_trigger to trigger an interrupt, it should call #hal_interrupt_irq_clear_force_trigger in the IRQ handler to clear the force trigger flag
 * @param[in] irq The interrupt trigger flag will be cleared, it should be one of the value #hal_interrupt_intr_src_t
 * @return	None
 */
void hal_interrupt_irq_clear_force_trigger(int irq);

int hal_interrupt_get_intr_src(IRQn_Type irqNum);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_INTERRUPT_H__ */
