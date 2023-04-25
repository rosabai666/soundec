/** 
***********************************************************
 * @attention
 * 
 * Copyright (c) 2022 Soundec Co., Ltd.
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
 * @file name   : hal_gpio.h
 * @author      : RandyFan
 * @version     : V02.01
 * @date        : 2022-02-16
 * @brief       : HAL GPIO Driver Header File
 * 
***********************************************************/

#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

#include "types.h"
#include "snc8600.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_PINMUX_MODE_NUM 	3
#define GPIO_CB_NUM				(10)

#define GPIO_DIRECTION_INPUT	(0)
#define GPIO_DIRECTION_OUTPUT	(1)

#define GPIO_DATA_LOW			(0)
#define GPIO_DATA_HIGH			(1)

#define GPIO_PIN_PULL_UP		(0)
#define GPIO_PIN_PULL_DOWN		(1)

/**
 * @name GPIO_REG
 * @brief Read/Write GPIO Registers
 * @{
 */
#define GPIO_REG						((volatile gpio_regs_t *)GPIO_BASE)
#define GPIO_Read(reg)					(GPIO_REG->reg)
#define GPIO_Write(reg, val)			(GPIO_REG->reg = (val))
/** @} GPIO_REG */

/** 
 * @brief	This enum defines the GPIO pin number
 */
typedef enum {
    GPIO_0  = 0,    /*!< GPIO pin0. */
    GPIO_1  = 1,    /*!< GPIO pin1. */
    GPIO_2  = 2,    /*!< GPIO pin2. */
    GPIO_3  = 3,    /*!< GPIO pin3. */
    GPIO_4  = 4,    /*!< GPIO pin4. */
    GPIO_5  = 5,    /*!< GPIO pin5. */
    GPIO_6  = 6,    /*!< GPIO pin6. */
    GPIO_7  = 7,    /*!< GPIO pin7. */
    GPIO_8  = 8,    /*!< GPIO pin8. */
    GPIO_9  = 9,    /*!< GPIO pin9. */
    GPIO_10 = 10,   /*!< GPIO pin10. */
    GPIO_11 = 11,   /*!< GPIO pin11. */
    GPIO_12 = 12,   /*!< GPIO pin12. */
    GPIO_13 = 13,   /*!< GPIO pin13. */
    GPIO_14 = 14,   /*!< GPIO pin14. */
    GPIO_15 = 15,   /*!< GPIO pin15. */
    GPIO_16 = 16,   /*!< GPIO pin16. */
    GPIO_NUM        /*!< The total number of GPIO pins */
}gpio_pin_t;

/**
 * @name GPIO_PINMUX
 * @brief Define GPIO pinmux functions
 * @{
 */
#define GPIO_0_FUNC_GPIO			(0)
#define GPIO_0_FUNC_JTCK			(1)
#define GPIO_0_FUNC_I2S1_CLK		(2)

#define GPIO_1_FUNC_GPIO			(0)
#define GPIO_1_FUNC_JTDI			(1)
#define GPIO_1_FUNC_I2S1_WS			(2)

#define GPIO_2_FUNC_GPIO			(0)
#define GPIO_2_FUNC_JTMS			(1)
#define GPIO_2_FUNC_I2S1_SDI		(2)

#define GPIO_3_FUNC_GPIO			(0)
#define GPIO_3_FUNC_JTRST			(1)
#define GPIO_3_FUNC_I2S1_SDO		(2)

#define GPIO_4_FUNC_GPIO			(0)
#define GPIO_4_FUNC_DMIC_IN4		(1)
#define GPIO_4_FUNC_I2S2_CLK		(2)

#define GPIO_5_FUNC_GPIO			(0)
#define GPIO_5_FUNC_DMIC_IN5		(1)
#define GPIO_5_FUNC_I2S2_WS			(2)

#define GPIO_6_FUNC_GPIO			(0)
#define GPIO_6_FUNC_DMIC_CLK41		(1)
#define GPIO_6_FUNC_I2S2_SDI		(2)

#define GPIO_7_FUNC_GPIO			(0)
#define GPIO_7_FUNC_DMIC_CLK51		(1)
#define GPIO_7_FUNC_I2S2_SDO		(2)

#define GPIO_8_FUNC_GPIO			(0)
#define GPIO_8_FUNC_NULL			(1)
#define GPIO_8_FUNC_DMIC_CLK11		(2)

#define GPIO_9_FUNC_GPIO			(0)
#define GPIO_9_FUNC_JTDO			(1)
#define GPIO_9_FUNC_DMIC_IN1		(2)

#define GPIO_10_FUNC_GPIO			(0)
#define GPIO_10_FUNC_I2C2_SCl		(1)
#define GPIO_10_FUNC_NULL			(2)

#define GPIO_11_FUNC_GPIO			(0)
#define GPIO_11_FUNC_I2C2_SDA		(1)
#define GPIO_11_FUNC_32K768			(2)

#define GPIO_12_FUNC_GPIO			(0)
#define GPIO_12_FUNC_I2S3_CLK		(1)
#define GPIO_12_FUNC_PWM0			(2)

#define GPIO_13_FUNC_GPIO			(0)
#define GPIO_13_FUNC_I2S3_WS		(1)
#define GPIO_13_FUNC_PWM1			(2)

#define GPIO_14_FUNC_GPIO			(0)
#define GPIO_14_FUNC_I2S3_SDI		(1)
#define GPIO_14_FUNC_PWM2			(2)

#define GPIO_15_FUNC_GPIO			(0)
#define GPIO_15_FUNC_I2S3_SDO		(1)
#define GPIO_15_FUNC_PWM3			(2)

#define GPIO_16_FUNC_GPIO			(0)
#define GPIO_16_FUNC_RST			(1)
#define GPIO_16_FUNC_NULL			(2)
/** @} GPIO_PINMUX */


/**
 * @enum gpio_isr_trigger_t
 * @brief This enum defines the interrupt trigger type of GPIO.
 */
typedef enum {
	GPIO_TRIGGER_RISING_EDGE = 0,			/*!< GPIO interrupt trigger type: Rising edge */
	GPIO_TRIGGER_FALLING_EDGE,				/*!< GPIO interrupt trigger type: Falling edge */
	GPIO_TRIGGER_HIGH_LEVEL,				/*!< GPIO interrupt trigger type: High level */
	GPIO_TRIGGER_LOW_LEVEL,					/*!< GPIO interrupt trigger type: Low level */
	GPIO_TRIGGER_NUM						/*!< The number of GPIO interrupt trigger type */
}gpio_isr_trigger_t;

/**
 * @enum gpio_intr_t
 * @brief This enum defines the interrupt source of GPIO.
 */
typedef enum{
	GPIO_678_INTR = 0,
	GPIO_9ABCDEF,
}gpio_intr_t;

/**
 * @struct gpio_regs_t
 * @brief Packing Registers of GPIO
 * @ingroup Regs
 */
typedef struct {
	__IO uint32_t DBCLK_DIV;	/*!< Offset:0x00 */
	__IO uint32_t reserved[3];	/*!< Offset:0x04 */
	__IO uint32_t PORT_DIR;			/*!< Offset:0x10 : GPIO direction register */
	__IO uint32_t PORT_SEL;	/*!< Offset:0x14 : Pin Multiplex Function selection. 0:Special, 1:General */
	__IO uint32_t PORT_DATA;			/*!< Offset:0x18 : GPIO input or output value. 0:low, 1:high */
	__IO uint32_t PORT_INTRSEL;		/*!< Offset:0x1c : GPIO interrupt enable register, available when GPIO set to input */
	__IO uint32_t PORT_INTRLEL;	/*!< Offset:0x20 : GPIO interrupt type: 0:Edge, 1:Level */
	__IO uint32_t PORT_INTRPOL;		/*!< Offset:0x24 : GPIO interrupt type: 0:High, 1:Low */
	__IO uint32_t PORT_INTRCLR;		/*!< Offset:0x28 : GPIO interrupt clear register, write 1 to clear interrupt */
	__IO uint32_t PORT_2FUNC;		/*!< Offset:0x2c : Special function selection register */
	__IO uint32_t PORT_INTRSTS;		/*!< Offset:0x30 : Interrupt status */
}gpio_regs_t;


/**
 * @brief	This function pulls down the GPIO
 * @param[in]	gpioPin    The target GPIO needs to be operated.
 */
void hal_gpio_pull_down(gpio_pin_t gpioPin);

/**
 * @brief	This function pulls up the GPIO
 * @param[in]	gpioPin    The target GPIO needs to be operated.
 */
void hal_gpio_pull_up(gpio_pin_t gpioPin);

/** 
 * @brief	This function Sets the direction of the GPIO
 * @note	None
 * @param[in]	gpioPin    The target GPIO needs to be operated.
 * @param[in]	dir         The direction of GPIO needs to be set, this parameter can only be a value of type hal_gpio_direction_t
 * @return	None
 */
void hal_gpio_set_direction(gpio_pin_t gpioPin, bool dir);

/** 
 * @brief	This function sets the multiplexing of the pin. 
 * @note	The pin multiplex with GPIO and peripherals. The pin multiplexers define in hal_pinmux.h
 * @param[in]	gpioPin    The target GPIO needs to be operated.
 * @param[in]	func 	The function to be set for the pin. Defined in hal_pinmux.h
 * @return	None
 */
void hal_gpio_set_pinmux_function(gpio_pin_t gpioPin, uint8_t func);

/** 
 * @brief	This function sets the output data of the target GPIO
 * @note	None
 * @param[in]	gpioPin    The target GPIO to be operated.
 * @param[in]	data   The output data of the GPIO.
 * 	@arg HAL_GPIO_DATA_LOW 	GPIO outputs low
 *	@arg HAL_GPIO_DATA_HIGH	GPIO outputs high
 * @return	None
 */
void hal_gpio_set_output(gpio_pin_t gpioPin, bool gpio_data);

/** 
 * @brief	This function gets the input data of the target GPIO
 * @note	None
 * @param[in]	gpioPin    The target GPIO to be operated.
 * @return	The input data of the GPIO.
 */
bool hal_gpio_get_input(gpio_pin_t gpioPin);

/** 
 * @brief	This function toggles the output data of the target GPIO
 * @note	None
 * @param[in]	gpioPin    The target GPIO to be operated.
 * @return	None
 */
void hal_gpio_toggle_pin(gpio_pin_t gpioPin);

/** 
 * @brief	This function configure the target GPIO as interrupt
 * @note	Only some GPIO pin can support interrupt feature. GPIO[6...15]
 * @param[in]	gpioPin    The target GPIO to be operated.
 * @param[in]	trigger The trigger type of the GPIO, this parameter can only be a value of type hal_gpio_isr_trigger_t
 * @return	None
 */
void hal_gpio_config_isr(gpio_pin_t gpioPin, gpio_isr_trigger_t trigger);

/**
 * @brief	This function toggles the trigger level of the target GPIO
 * @note	None
 * @param[in]	gpioPin    The target GPIO to be operated.
 * @return	None
 */
void hal_gpio_toggle_isr_level(gpio_pin_t gpioPin);


#ifdef __cplusplus
}
#endif


#endif /* __HAL_GPIO_H__ */
