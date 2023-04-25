/**
***********************************************************
 *
 * @file name   : altotool_debug.h
 * @author      : nick
 * @version     : V0.1
 * @date        : 2022-03-10
 * @brief       : alto tool DEBUG config
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
 * and Sourced Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/


#ifndef _ALTOTOOL_DEBUG_
#define _ALTOTOOL_DEBUG_
#include <stdio.h>
#include "math.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "common.h"
#if (SUPPORT_ALTOTOOL_CODEDC)

extern uint32_t gSysDebugLevel;
extern uint32_t gSysDebugModule;


typedef enum{
	CMD_DEBUG_MODULE		  = 0x80,
	CMD_DEBUG_LEVEL 		  = 0x81,
	CMD_DEBUG_MODULE_INFO	  = 0x82,
	CMD_DEBUG_REGISTER_WRITE  = 0x83,
	CMD_DEBUG_REGISTER_READ   = 0x84,
	CMD_DEBUG_CUSTOM_CMD	  = 0x85,
 
}debug_t;


#define SYS_FATAL    0
#define SYS_ERROR    1
#define SYS_WARNING  2
#define SYS_INFO     3
#define SYS_DEBUG    4

#define DEBUG_USB        1<<0
#define DEBUG_CODEC      1<<1
#define DEBUG_I2S        1<<2
#define DEBUG_I2C        1<<3
#define DEBUG_AUDIO_SYNC 1<<4
#define DEBUG_PROTOCOL   1<<5

#define DEBUG_USB_MODULE_INFO         1<<0
#define DEBUG_CODEC_MODULE_INFO       1<<1
#define DEBUG_I2S_MODULE_INFO         1<<2
#define DEBUG_I2C_MODULE_INFO         1<<3
#define DEBUG_PROTOCOL_MODULE_INFO    1<<4


int hid_sys_debug_if(uint8_t module_id ,uint8_t debug_level ,const char *fmt, ...);




void set_debug_config(uint8_t cmd);

#endif/* SUPPORT_ALTOTOOL_CODEDC */

#endif /* _ALTOTOOL_DEBUG_ */


