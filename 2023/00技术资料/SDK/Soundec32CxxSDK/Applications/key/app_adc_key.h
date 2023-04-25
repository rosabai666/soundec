/** 
***********************************************************
 * 
 * @file name	: app_adc_key.h
 * @author		: RandyFan
 * @version		: V02.02
 * @date		: 2022-02-18
 * @brief		: SAR ADC Key Header File
 * 
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
***********************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEY_BOARD_H__
#define __KEY_BOARD_H__

/* Includes ------------------------------------------------------------------*/
#include "drv_saradc.h"
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name KEY_PARAM
 * @brief Key parameters
 * @{
 */
#define KEY_NULL						0xFF
#define KEY_PARSE_THRES					80
#define KEY_SHORT_PRESS_THRES			8	//8*10 = 80 ms
#define KEY_NULL_THRES					50	//50*10 = 500 ms
/** @} KEY_PARAM */

typedef enum{
	GPIO_KEY_LOW = 0,
	GPIO_KEY_HIGH,
	ADC_KEY,
}key_type_t;


/**
 * @enum key_t
 * @brief Define key numbers.
 */
typedef enum{
	KEY_VOL_UP = 0,
	KEY_VOL_DOWN,
	KEY_PLAY_PAUSE,
	KEY_NEXT,
	KEY_PREV,
	KEY_MUTE,
	//...
	KEY_NUM
}key_t;

/**
 * @enum key_state_t
 * @brief Define key states.
 */
typedef enum{
	KEY_S0_NULL = 0,
	KEY_S1_PRESS_CNT,
	KEY_S2_LONG_CNT,
	KEY_S3_LONG_PRESS,
	KEY_S4_NULL_CNT,
	KEY_S5_WAIT_RLS,
}key_state_t;

typedef enum{
	KEY_INTERMEDIATE_STATE_NONE = 0,
	KEY_INTERMEDIATE_STATE_SHORT,
	KEY_INTERMEDIATE_STATE_LONG,
	KEY_INTERMEDIATE_STATE_LONG_JUSTIFY_FAIL,
	KEY_INTERMEDIATE_STATE_DOUBLE_SHORT_JUSTIFY_FAIL,
}key_intermediate_t;

/**
 * @enum key_event_t
 * @brief Define key simple events
 */
typedef enum{
	KEY_EVENT_NONE = 0,
	KEY_EVENT_SHORT,
	KEY_EVENT_LONG,
	KEY_EVENT_DOUBLE,
	KEY_EVENT_RELEASE,
}key_event_t;

/**
 * @struct key_handle_t
 * @brief key process handler
 */
typedef struct{
	uint8_t curKeyState;
	uint8_t lastKey;
	uint16_t keyCnt;
	uint8_t shortKeyCnt;
	bool rlsFlag;
	bool longFlag;
}key_handle_t;



typedef enum{
	SHORT_FUNC_IMMEDIATE = 0,
	SHORT_FUNC_NEED_JUSTIFY,
}key_short_func_types_t;

typedef enum{
	LONG_FUNC_OFF = 0,
	LONG_FUNC_SINGLE,
	LONG_FUNC_MULTIPLE,
}key_long_func_types_t;

typedef enum{
	DOUBLE_FUNC_OFF = 0,
	DOUBLE_FUNC_ON,
}key_double_func_types_t;

typedef union{
	uint8_t func;
	struct{
		uint8_t shortFunc : 1;
		uint8_t longFunc : 2;
		uint8_t doubleFunc : 1;
		uint8_t rev : 4;
	}funcBits;
}key_func_t;

typedef struct{
	uint8_t keyType;
	uint16_t keyVal;
	uint16_t longKeyThr;
	uint16_t longPressThr;
	key_func_t keyFunc;
}key_cfg_t;

typedef enum{
	HID_NULL 		= 0,
	HID_VOL_UP 		= 0x01,
	HID_VOL_DOWN	= 0x02,
	HID_PLAYPAUSE	= 0x04,
	HID_NEXT		= 0x08,
	HID_PREV		= 0x10,
}hid_t;

#if(AUX_ADC_ENABLE)
/**
 * @brief This function inits ADC keys
 */
void key_init(void);

/**
 * @brief Key 10ms polling task
 */
void key_polling_10ms(void);
#endif	//AUX_ADC_ENABLE

#ifndef JTAG_DEBUG_ENABLE
/**
 * @brief This function set io to general function
 */
void set_jtag_io_general_function(void);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* __KEY_BOARD_H__ */









