/**
***********************************************************
 *
 * @file name   : Altotool_eq.h
 * @author      : nick
 * @version     : V0.1
 * @date        : 2022-03-10
 * @brief       : alto tool eq config
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

#ifndef _ALTOTOOL_EQ_
#define _ALTOTOOL_EQ_

#include <stdio.h>
#include "math.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "boardConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SUPPORT_SPK_EQ_NUM		1

#define EQ_NUM    10
#define EQ_COEFF   6
#define EQ_PARA    4
 
enum{
	 EQ_SPKER     	          =  0x00,  
	 EQ_MIC                   =  0x01,
	 EQ_MODULE_MAX,
};

enum{
	OP_EQ_SPKER_LCH				   =  0x00,  
	OP_EQ_SPKER_RCH,  
	OP_EQ_MIC,
	OP_EQ_MAX,
};

 typedef enum{
	  CMD_EQ_NUM			      =  0x80,
	  CMD_EQ_MONO_STEREO_SWITCH   =  0x81,
	  CMD_EQ_BYBASS,
	  CMD_EQ_GAIN,
	  CMD_EQ_RESET,
	  CMD_EQ_SAVE,       /*0x85*/
	  CMD_EQ_SWITCH_L,
	  CMD_EQ_COEFF_L,    /*0x87*/
	  CMD_EQ_PARA_L,
	  CMD_EQ_REQUEST_L,  /*0x89*/
	  CMD_EQ_SWITCH_R,
	  CMD_EQ_COEFF_R,
	  CMD_EQ_PARA_R,     /*0x0C*/
	  CMD_EQ_REQUEST_R,
	  CMD_EQ_MAX,
  }eq_cmd_t;

typedef struct{
	bool bandEn;
	uint8_t eqType;
	uint8_t rev[2];
	float qbw;
	float fc;
	float gain;
}eq_band_t;

typedef struct{
	uint8_t eqNum;
	bool stereo;
	bool bypass;
	int8_t gainBeforeEq;
	int8_t gainBeforeEq_R;
	uint8_t gainBeforeEqType;   //0-mono, 1-stereoL, 2-stereoR
	uint8_t reserved[2]; //for padding
	eq_band_t band[EQ_NUM];
#if (SUPPORT_SPKER_STEREO_EQ)
	eq_band_t bandStereoR[EQ_NUM];
#endif	//SUPPORT_SPKER_STEREO_EQ
}eq_chx_t;

typedef struct{
#if (SUPPORT_SPKER_EQ)
	eq_chx_t spkEq;
#endif	//SUPPORT_SPKER_EQ
#if (SUPPORT_MIC_EQ)
	eq_chx_t micEq;
#endif	//SUPPORT_MIC_EQ
}eq_t;

#if(SUPPORT_DSP_EQ)
extern eq_t gEqCfg;

void altotool_eq_init(void);
uint8_t set_altotool_spker_eq(uint8_t ch);
uint8_t set_altotool_mic_eq(uint8_t ch);
void set_spker_eq_para(uint8_t eqModule , uint8_t ch );
void get_spker_eq_para(uint8_t eqModule , uint8_t ch );

#endif	//SUPPORT_DSP_EQ

#ifdef __cplusplus
}
#endif

#endif /* _ALTOTOOL_EQ_ */

