/** 
***********************************************************
 * 
 * @file name	: boardConfig.h
 * @author		: yen.yu
 * @version		: Vxx.xx
 * @date		: 2022-03-21
 * @brief		: config for user
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
#ifndef __BOARDCONFIG_H__
#define __BOARDCONFIG_H__

#define BOARD_VER 0x0101
#define D_CONFIG_2MIC_MEETING_BOX					0
#define D_CONFIG_2MIC_USB_HEADPHONE					0
#define D_CONFIG_2MIC_BT_HEADPHONE					0

#if D_CONFIG_2MIC_MEETING_BOX
#include "user_config_2mic_meetingbox.h"
#if SUPPORT_ALTOTOOL
#define PROJECT_NAME								"2mic_meeting_box_gui"
#else
#define PROJECT_NAME								"2mic_meeting_box"
#endif

#elif D_CONFIG_2MIC_USB_HEADPHONE
#include "user_config_2mic_usb_headphone.h"
#if SUPPORT_ALTOTOOL
#define PROJECT_NAME								"2mic_usb_headphone_gui"
#else
#define PROJECT_NAME								"2mic_usb_headphone"
#endif

#elif D_CONFIG_2MIC_BT_HEADPHONE
#include "user_config_2mic_bt_headphone.h"
#if SUPPORT_ALTOTOOL
#define PROJECT_NAME								"2mic_bt_headphone_gui"
#else
#define PROJECT_NAME								"2mic_bt_headphone"
#endif

#else
#define D_CONFIG_DEFAULT							1
#include "user_config_default.h"
#if SUPPORT_ALTOTOOL
#define PROJECT_NAME								"Soundec_default_gui"
#else
#define PROJECT_NAME								"Soundec_default"
#endif
#endif


#endif  /* __BOARDCONFIG_H__ */


