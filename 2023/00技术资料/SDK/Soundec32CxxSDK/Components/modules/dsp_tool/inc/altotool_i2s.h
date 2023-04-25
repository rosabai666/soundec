/**
***********************************************************
 *
 * @file name   : Altotool_i2s.h
 * @author      : nick
 * @version     : V0.1
 * @date        : 2022-03-10
 * @brief       : alto tool codec config
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

#ifndef _ALTOTOOL_I2S_
#define _ALTOTOOL_I2S_
#include <stdio.h>
#include "math.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "common.h"

#if (SUPPORT_ALTOTOOL_I2S)

typedef enum{
	SUB_I2S1                       = 0x00,
	SUB_I2S2                       = 0x01,
	SUB_I2S3                       = 0x02
}sub_i2s_t;


typedef enum{
	CMD_I2S_ENABLE                 = 0x80,
	CMD_I2S_TX_EN                  = 0x81,
	CMD_I2S_RX_EN                  = 0x82,
	CMD_I2S_MODE                   = 0x83,
	CMD_I2S_FORMATE                = 0x84,
	CMD_I2S_SAMPLE                 = 0x85,
	CMD_I2S_WORD_LENGTH            = 0x86,
	CMD_I2S_FRAME_LENGTH           = 0x87, /*frame length*/
	CMD_I2S_MUTE                   = 0x88,
	CMD_I2S_SAVE_PARAM       = 0x89,

}i2s_cmd_t;

void i2s_config_setting(uint8_t cmd , uint8_t channel);

#endif/* SUPPORT_ALTOTOOL_I2S */

#endif /* _ALTOTOOL_I2S_ */


