/**
***********************************************************
 *
 * @file name   : Altotool_comm.h
 * @author      : nick
 * @version     : V0.1
 * @date        : 2022-03-10
 * @brief       : alto tool codec config
 *
***********************************************************
 * @attention
 *
 * Copyright (c) 2019 Soundec Co., Ltd.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user
 * who uses the software, his/her employer (which must be your employer
 * and Sourced Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/

#ifndef _ALTOTOOL_COMM_
#define _ALTOTOOL_COMM_
#include <stdio.h>
#include "math.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "types.h"

#if (SUPPORT_ALTOTOOL_COMM)


typedef enum{
	VOL_AMIC1                      = 0x00, 
	VOL_AMIC2                      = 0x01, 
	VOL_DMIC3                      = 0x02, 
	VOL_DMIC4                      = 0x03, 
	VOL_DMIC5                      = 0x04, 
	VOL_DMIC6                      = 0x05, 
	VOL_DAC1                       = 0x06, 
	VOL_DAC2                       = 0x07, 
}vol_ctl_t;

#define VOL_CTRL_ENABLE            0x80
#define VOL_CTRL_SETTING           0x81
#define NOISE_CANCELLER_VOL    0x82


typedef enum{
	SOFT_AGC_SWITCH                =  0x80,
	SOFT_AGC_SAMPLE_RATE           =  0x81,
	SOFT_AGC_TARGET                =  0x82,
	SOFT_AGC_MIN_GAIN              =  0x83,
	SOFT_AGC_MAX                   =  0x84,
	SOFT_AGC_HOLDTIME              =  0x85,
	SOFT_AGC_ATTACK_TIME           =  0x86,
	SOFT_AGC_RELEASE_TIME          =  0x87,
	SOFT_AGC_THRESHOLD             =  0x88,
}soft_agc_t;

typedef enum{
	SOFT_DRC_SWITCH                = 0x80,
	SOFT_DRC_SAMPLE_RATE           = 0x81,
	SOFT_DRC_RATIO                 = 0x82,
	SOFT_DRC_ATTACK_TIME           = 0x83,
	SOFT_DRC_RELEASE_TIME          = 0x84,
	SOFT_DRC_KNEE_WIDTH            = 0x85,
	SOFT_DRC_THRESHOLD             = 0x86,
}soft_drc_t;

typedef enum{
	AEC_ENABLE                    = 0X80,
	AEC_DEPTH                     = 0X81,
	AEC_CONVERGENCE_RATE          = 0X82, 
}aec_t;


typedef enum{
	DEREVERB_ENABLE               = 0x80,
	DEREVERB_RATE                 = 0x81,
	DEREVERB_STRENGTH             = 0x82,
}dereverb_t;


#define NOISE_CANCELLER_ENABLE     0x80
#define NOISE_CANCELLER_DEPTH      0x81
	
typedef enum{
	OTH_BEAMFORMING               = 0x80,
	OTH_DOA                       = 0x81,
	OTH_SPKER_EQ                  = 0x82,
	OTH_MIC_EQ                    = 0x83,
	OTH_SIGNAL_REF                = 0x84,

	SAVE_ALG_PARAM				  = 0x85,
}oth_t;

#define DENOISE_ENABLE     0x80
#define DEPTH    		   0x81
#define VOL       		   0x82


void set_volume_control(uint8_t channel ,uint8_t cmd);
void set_soft_agc_config(uint8_t cmd );
void set_soft_drc_config(uint8_t cmd );
void set_aec_config(uint8_t cmd);
void set_dereverb_config(uint8_t cmd);
void set_noise_config(uint8_t cmd);
void set_oth_config(uint8_t cmd);
void set_earophone_config(uint8_t cmd);
#endif/*SUPPORT_ALTOTOOL_COMM*/


#endif /* _ALTOTOOL_COMM_ */

