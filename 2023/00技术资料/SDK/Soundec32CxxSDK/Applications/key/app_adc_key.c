/** 
***********************************************************
 * 
 * @file name	: app_adc_key.c
 * @author		: RandyFan
 * @version		: V02.02
 * @date		: 2022-02-18
 * @brief		: SAR ADC Key
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

/* Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include "app_adc_key.h"
#include "types.h"
#include "usb_hid.h"
#include "drv_gpio.h"
#if SUPPORT_PLAY_TONE
#include "playTone.h"
#endif

#if(APP_ADC_KEY_ENABLE)
/* Private typedef ----------------------------------------------------------*/
/* Private macro -----------------------------------------------------------*/
#define ADC_KEY_DEBUG		1
#if(ADC_KEY_DEBUG)
#define KEY_DEBUG(...)		uart_printf(__VA_ARGS__)
#else
#define KEY_DEBUG(...)
#endif

/* Private function prototypes -------------------------------------------------*/
static uint8_t key_parse(void);
static uint8_t key_process(uint8_t key);
static void key_event_handler(uint8_t keyEvent);

/* Private variables ---------------------------------------------------------*/
key_cfg_t keyCfg[] = { //KEY_NUM
	//Key 0
	{
		.keyType = ADC_KEY,
		.keyVal = 1650,
		.longKeyThr = 30,	//300ms
		.longPressThr = 4,	//40ms
		.keyFunc = {
			.funcBits = {
				.shortFunc = SHORT_FUNC_IMMEDIATE,
				.longFunc = LONG_FUNC_MULTIPLE,
				.doubleFunc = DOUBLE_FUNC_OFF,
			},
		},
	},

	//Key 1
	{
		.keyType = ADC_KEY,
		.keyVal = 100,
		.longKeyThr = 30,
		.longPressThr = 4,
		.keyFunc = {
			.funcBits = {
				.shortFunc = SHORT_FUNC_IMMEDIATE,
				.longFunc = LONG_FUNC_MULTIPLE,
				.doubleFunc = DOUBLE_FUNC_OFF,
			},
		},
	},

	//Key 2
	{
		.keyType = ADC_KEY,
		.keyVal = 860,
		.longKeyThr = 30,
		.longPressThr = 4,
		.keyFunc = {
			.funcBits = {
				.shortFunc = SHORT_FUNC_IMMEDIATE,
				.longFunc = LONG_FUNC_OFF,
				.doubleFunc = DOUBLE_FUNC_OFF,
			},
		},
	},

	//Key 3
	{
		.keyType = ADC_KEY,
		.keyVal = 2630,
		.longKeyThr = 100,
		.longPressThr = 4,
		.keyFunc = {
			.funcBits = {
				.shortFunc = SHORT_FUNC_IMMEDIATE,
				.longFunc = LONG_FUNC_SINGLE,
				.doubleFunc = DOUBLE_FUNC_OFF,
			},
		},
	},

//	//Key 4
	{
		.keyType = ADC_KEY,
		.keyVal = 3480,
		.longKeyThr = 100,
		.longPressThr = 4,
		.keyFunc = {
			.funcBits = {
				.shortFunc = SHORT_FUNC_NEED_JUSTIFY,
				.longFunc = LONG_FUNC_MULTIPLE,
				.doubleFunc = DOUBLE_FUNC_OFF,
			},
		},
	},
#if (APP_GPIO_KEY_ENABLE)
	//Key 5
	{
		.keyType = GPIO_KEY_LOW,
		.keyVal = GPIO_5,//2000,
		.longKeyThr = 100,
		.longPressThr = 4,
		.keyFunc = {
			.funcBits = {
				.shortFunc = SHORT_FUNC_NEED_JUSTIFY,
				.longFunc = LONG_FUNC_SINGLE,
				.doubleFunc = DOUBLE_FUNC_ON,
			},
		},
	},
#endif
};

static key_handle_t keyHandle;

#if SUPPORT_USB_HID
extern void isHid_key(uint32_t key);
#endif

/* Private functions ---------------------------------------------------------*/
void key_event_handler(uint8_t keyEvent)
{
	switch(keyHandle.lastKey)
	{
	case KEY_VOL_UP:
		switch(keyEvent)
		{
		case KEY_EVENT_SHORT:
		case KEY_EVENT_LONG:	//identical function of short and long
			uart_printf("vol up\n");
		#if SUPPORT_PLAY_TONE
			if(keyEvent == KEY_EVENT_SHORT)
			{
				PlayToneSwitch(SOUNDEC_WAV_VOL_UP);
			}
		#endif
			#if SUPPORT_USB_HID
			is_hid_key(HID_VOL_UP);
			#endif
			break;
		case KEY_EVENT_RELEASE:
			#if SUPPORT_USB_HID
//			uart_printf("rls\n");
			is_hid_key(HID_NULL);
			#endif
			break;
		default:
			#if SUPPORT_USB_HID
//			uart_printf("n");
			is_hid_key(HID_NULL);
			#endif
			break;
		}
		break;

	case KEY_VOL_DOWN:
		switch(keyEvent)
		{
		case KEY_EVENT_SHORT:
		case KEY_EVENT_LONG:
			uart_printf("vol down\n");
			#if SUPPORT_PLAY_TONE
			if(keyEvent == KEY_EVENT_SHORT)
			{
				PlayToneSwitch(SOUNDEC_WAV_VOL_DOWN);
			}
			#endif
			#if SUPPORT_USB_HID
			is_hid_key(HID_VOL_DOWN);
			#endif
			break;
		case KEY_EVENT_RELEASE:
//			uart_printf("rls\n");
			#if SUPPORT_USB_HID
			is_hid_key(HID_NULL);
			#endif
			break;
		default:
			#if SUPPORT_USB_HID
			is_hid_key(HID_NULL);
			#endif
			break;
		}
		break;

	case KEY_PLAY_PAUSE:
		switch(keyEvent)
		{
		case KEY_EVENT_SHORT:	//no long, only play/pause once
			uart_printf("play pause\n");
			#if SUPPORT_USB_HID
			is_hid_key(HID_PLAYPAUSE);
			#endif
#if SUPPORT_PLAY_TONE
			PlayToneSwitch(SOUNDEC_WAV_PLAY);
#endif
			break;
		case KEY_EVENT_RELEASE:
//			uart_printf("rls\n");
			#if SUPPORT_USB_HID
			is_hid_key(HID_NULL);
			#endif
			break;
		default:
//			uart_printf("n");
			break;
		}
		break;

	case KEY_NEXT:
		switch(keyEvent)
		{
		case KEY_EVENT_SHORT:	//no long, only play/pause once
			uart_printf("NEXT\n");
			#if SUPPORT_USB_HID
			is_hid_key(HID_NEXT);
			#endif
#if SUPPORT_PLAY_TONE
			PlayToneSwitch(SOUNDEC_WAV_NEXT);
#endif
			break;
		case KEY_EVENT_RELEASE:
//			uart_printf("rls\n");
			#if SUPPORT_USB_HID
			is_hid_key(HID_NULL);
			#endif
			break;
		default:
//			uart_printf("n");
			break;
		}
		break;

	case KEY_PREV:
		switch(keyEvent)
		{
		case KEY_EVENT_SHORT:	//no long, only play/pause once
			uart_printf("PREV\n");
			#if SUPPORT_USB_HID
			is_hid_key(HID_PREV);
			#endif
#if SUPPORT_PLAY_TONE
			PlayToneSwitch(SOUNDEC_WAV_PREV);
#endif
			break;
		case KEY_EVENT_RELEASE:
//			uart_printf("rls\n");
			#if SUPPORT_USB_HID
			is_hid_key(HID_NULL);
			#endif
			break;
		default:
//			uart_printf("n");
			break;
		}
		break;

//	case KEY_MUTE:
//		switch(keyEvent)
//		{
//		case KEY_EVENT_SHORT:
//			uart_printf("Key 5 single short\n");
//#if SUPPORT_PLAY_TONE
//			PlayToneSwitch(SOUNDEC_WAV_MUTE);
//#endif
//			break;
//		case KEY_EVENT_LONG:
//			uart_printf("Key 5 long that only excuted once\n");
//			break;
//		case KEY_EVENT_DOUBLE:
//			uart_printf("Key 5 double\n");	//double press func
//			break;
//		case KEY_EVENT_RELEASE:
//			uart_printf("rls\n");
//			break;
//		default:
//			uart_printf("n");
//			break;
//		}
//		break;
	default:
		break;
	}
}

/**
 * @brief This function inits ADC keys
 */
void key_init(void)
{
	memset(&keyHandle, 0, sizeof(key_handle_t));
	aux_adc_init();
#if(AUX_ADC_INTR_ENABLE)
//	aux_adc_register_cb();
#endif
#if (APP_GPIO_KEY_ENABLE)
	for(uint8_t i=0;i<KEY_NUM;i++)
	{
		if(keyCfg[i].keyType != ADC_KEY)
		{
			hal_gpio_set_pinmux_function(keyCfg[i].keyVal, 0);
			hal_gpio_set_direction(keyCfg[i].keyVal, 0);
			if(keyCfg[i].keyType == GPIO_KEY_LOW)
				hal_gpio_pull_up(keyCfg[i].keyVal);
			else
				hal_gpio_pull_down(keyCfg[i].keyVal);
		}
	}
#endif
}

/**
 * @brief Key 10ms polling task
 */
void key_polling_10ms(void)
{
	uint8_t curKey = key_parse();
	uint8_t keyEvent = key_process(curKey);
	key_event_handler(keyEvent);
}

/**
 * @brief This function parses ADC data to key number
 * @return key number
 */
uint8_t key_parse(void)
{
	
	//uint8_t adcKey = KEY_NULL;
	uint16_t val = aux_adc_get_val();
	
	for(uint8_t i = 0;i < sizeof(keyCfg)/sizeof(key_cfg_t);i++)
	{
		//uart_printf("adc value = %d key num = %d\n",val, sizeof(keyCfg)/sizeof(key_cfg_t));
		if(keyCfg[i].keyType == ADC_KEY)
		{
			if(abs(val - keyCfg[i].keyVal) < KEY_PARSE_THRES)
				return i;
		}
#if (APP_GPIO_KEY_ENABLE)
		else
		{
			if(hal_gpio_get_input(keyCfg[i].keyVal) == keyCfg[i].keyType)
				return i;
		}
#endif
	}
	
	return KEY_NULL;
}


/**
 * @brief This function process keys' press and release
 * @param[in] key key number
 * @return key event
 */

uint8_t key_process(uint8_t key)
{
	switch(keyHandle.curKeyState)
	{
	case KEY_S0_NULL:
		if(keyHandle.rlsFlag)
		{
			keyHandle.rlsFlag = 0;
			return KEY_EVENT_RELEASE;
		}
		keyHandle.longFlag = 0;
		keyHandle.shortKeyCnt = 0;
		keyHandle.lastKey = key;
		if(key != KEY_NULL)
		{
//			KEY_DEBUG("p");
			keyHandle.curKeyState = KEY_S1_PRESS_CNT;
		}
		return KEY_EVENT_NONE;

	case KEY_S1_PRESS_CNT:
		if(key == keyHandle.lastKey)
		{
			keyHandle.keyCnt++;
			if(keyHandle.keyCnt == KEY_SHORT_PRESS_THRES)
			{
				keyHandle.keyCnt = 0;
				switch(keyCfg[key].keyFunc.func & 0x0F)
				{
				case 0b0000:
				case 0b0001:
					keyHandle.curKeyState = KEY_S5_WAIT_RLS;
					return KEY_EVENT_SHORT;
				case 0b0010:
				case 0b0100:
					keyHandle.curKeyState = KEY_S2_LONG_CNT;
					return KEY_EVENT_SHORT;
				case 0b1000:
				case 0b1010:
				case 0b1100:
					keyHandle.shortKeyCnt++;
					if(keyHandle.shortKeyCnt >= 2)
					{
						keyHandle.curKeyState = KEY_S5_WAIT_RLS;
						return KEY_EVENT_DOUBLE;
					}
					else
					{
						keyHandle.curKeyState = KEY_S2_LONG_CNT;
						return KEY_EVENT_SHORT;
					}
				case 0b1001:
				case 0b1011:
				case 0b1101:
					keyHandle.shortKeyCnt++;
					if(keyHandle.shortKeyCnt >= 2)
					{
						keyHandle.curKeyState = KEY_S5_WAIT_RLS;
						return KEY_EVENT_DOUBLE;
					}
					else
					{
						keyHandle.curKeyState = KEY_S2_LONG_CNT;
						return KEY_EVENT_NONE;
					}
				default://0b011, 0b101
					keyHandle.curKeyState = KEY_S2_LONG_CNT;
					return KEY_EVENT_NONE;
				}
			}
		}
		else
		{
			keyHandle.keyCnt = 0;
//			keyHandle.lastKey = key;
			keyHandle.curKeyState = KEY_S0_NULL;
			if(keyCfg[keyHandle.lastKey].keyFunc.funcBits.doubleFunc == DOUBLE_FUNC_ON && keyHandle.shortKeyCnt >= 1)
				return KEY_EVENT_SHORT;
		}
		return KEY_EVENT_NONE;

	case KEY_S2_LONG_CNT:
		if(key == keyHandle.lastKey)
		{
			keyHandle.keyCnt++;
			if(keyHandle.keyCnt == keyCfg[key].longKeyThr)
			{
				keyHandle.keyCnt = 0;

				switch(keyCfg[key].keyFunc.func & 0x0F)
				{
				case 0b0010:
				case 0b0011:
					keyHandle.curKeyState = KEY_S5_WAIT_RLS;
					return KEY_EVENT_LONG;
				case 0b0100:
				case 0b0101:
					keyHandle.curKeyState = KEY_S3_LONG_PRESS;
					return KEY_EVENT_LONG;
				case 0b1000:
				case 0b1001:
					keyHandle.longFlag = 1;
					keyHandle.curKeyState = KEY_S5_WAIT_RLS;
					return KEY_EVENT_SHORT;
				default:
					keyHandle.longFlag = 1;
					keyHandle.curKeyState = KEY_S5_WAIT_RLS;
					return KEY_EVENT_LONG;
				}
			}
		}
		else
		{
			keyHandle.keyCnt = 0;
			if(key == KEY_NULL && keyCfg[keyHandle.lastKey].keyFunc.funcBits.doubleFunc == DOUBLE_FUNC_ON)
			{
				keyHandle.curKeyState = KEY_S4_NULL_CNT;
			}
			else if(keyCfg[keyHandle.lastKey].keyFunc.funcBits.shortFunc == SHORT_FUNC_NEED_JUSTIFY)
			{
				keyHandle.curKeyState = KEY_S5_WAIT_RLS;

					return KEY_EVENT_SHORT;
			}
			else
			{
				keyHandle.curKeyState = KEY_S0_NULL;
				return KEY_EVENT_RELEASE;
			}
		}
		return KEY_EVENT_NONE;

	case KEY_S3_LONG_PRESS:
		if(key == keyHandle.lastKey)
		{
			keyHandle.keyCnt++;
			if(keyHandle.keyCnt == keyCfg[key].longPressThr)
			{
				keyHandle.keyCnt = 0;
				return KEY_EVENT_LONG;
			}
		}
		else
		{
			keyHandle.keyCnt = 0;
			keyHandle.curKeyState = KEY_S0_NULL;
			return KEY_EVENT_RELEASE;
		}
		return KEY_EVENT_NONE;

	case KEY_S4_NULL_CNT:
//		if(key != keyHandle.lastKey)
//		{
//			if(key == KEY_NULL && keyHandle.keyCnt < KEY_NULL_THRES)
//				keyHandle.keyCnt++;
//			else
//			{
//				keyHandle.keyCnt = 0;
//				keyHandle.curKeyState = KEY_S0_NULL;
//				if(keyCfg[keyHandle.lastKey].keyFunc.funcBits.doubleFunc == DOUBLE_FUNC_ON && keyHandle.shortKeyCnt < 2 && keyHandle.longFlag == 0)
//				{
//					keyHandle.rlsFlag = 1;
//					return KEY_EVENT_SHORT;
//				}
//				else
//				{
//					keyHandle.longFlag = 0;
//					return KEY_EVENT_RELEASE;
//				}
//			}
//		}
//		else
//		{
//			keyHandle.keyCnt = 0;
//			keyHandle.curKeyState = KEY_S1_PRESS_CNT;
//		}
//		return KEY_EVENT_NONE;
		if(key == KEY_NULL)
		{
			keyHandle.keyCnt++;
			if(keyHandle.keyCnt == KEY_NULL_THRES)
			{
				keyHandle.keyCnt = 0;
				keyHandle.curKeyState = KEY_S0_NULL;
				if(keyCfg[keyHandle.lastKey].keyFunc.funcBits.doubleFunc == DOUBLE_FUNC_ON && keyHandle.shortKeyCnt < 2 && keyHandle.longFlag == 0)
				{
					keyHandle.rlsFlag = 1;
					return KEY_EVENT_SHORT;
				}
				else
				{
					keyHandle.longFlag = 0;
					return KEY_EVENT_RELEASE;
				}

			}
		}
		else
		{
			keyHandle.keyCnt = 0;
			if(key == keyHandle.lastKey)
			{
				keyHandle.curKeyState = KEY_S1_PRESS_CNT;
			}
			else
			{
				keyHandle.curKeyState = KEY_S0_NULL;
				if(keyCfg[keyHandle.lastKey].keyFunc.funcBits.doubleFunc == DOUBLE_FUNC_ON && keyHandle.shortKeyCnt < 2 && keyHandle.longFlag == 0)
				{
					keyHandle.rlsFlag = 1;
					return KEY_EVENT_SHORT;
				}
				else
				{
					keyHandle.longFlag = 0;
					return KEY_EVENT_RELEASE;
				}
			}
		}
		return KEY_EVENT_NONE;

	case KEY_S5_WAIT_RLS:
		if(key == keyHandle.lastKey)
			return KEY_EVENT_NONE;
		else
		{
			if(keyCfg[keyHandle.lastKey].keyFunc.funcBits.doubleFunc == DOUBLE_FUNC_ON && keyHandle.shortKeyCnt < 2)
			{
				keyHandle.curKeyState = KEY_S4_NULL_CNT;
				return KEY_EVENT_NONE;
			}
			else
			{
				keyHandle.curKeyState = KEY_S0_NULL;
				return KEY_EVENT_RELEASE;
			}
		}

	default:
		return KEY_EVENT_NONE;
	}
}
#endif	//APP_ADC_KEY_ENABLE

#ifndef JTAG_DEBUG_ENABLE
void set_jtag_io_general_function(void)
{
    hal_gpio_set_pinmux_function(GPIO_0, 0);
    hal_gpio_set_pinmux_function(GPIO_1, 0);
    hal_gpio_set_pinmux_function(GPIO_2, 0);
    hal_gpio_set_pinmux_function(GPIO_3, 0);
}
#endif


