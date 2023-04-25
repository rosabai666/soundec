/** 
***********************************************************
 * 
 * @file name	: dsp_eq.h
 * @author		: nick
 * @version		: Vxx.xx
 * @date		: 2021-1-12
 * @brief		: dsp eq
 * 
***********************************************************
 * @attention
 * 
 * Copyright (c) 2021 Soundec Co., Ltd. 
 * All rights reserved.
 * 
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user
 * who uses the software, his/her employer (which must be your employer
 * and Soundec Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 * 
***********************************************************/

#ifndef __DSP_EQ__
#define __DSP_EQ__

#include "altotool_eq.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
	EQ_DISABLE = 0,
	EQ_POPS,
	EQ_JAZZ,
	EQ_ROCK,
	EQ_VOCAL,
	EQ_CLASSIC,
};

#define MAX_EQ_FS_HZ          20100.0f
#define MINI_EQ_FS_HZ         10.0f
#define MAX_EQ_GAIN           40.0f     
#define MINI_EQ_GAIN          -40.0f          
#define MAX_EQ_TYPE           7
#define MINI_EQ_TYPE          0        
#define MAX_EQ_QBW            16.0f    
#define MINI_EQ_QBW           0.0f


	
/* Private variables ---------------------------------------------------------*/

#if (SUPPORT_DSP_EQ)
void spker_eqLch_init(void);
#if (SUPPORT_SPKER_STEREO_EQ)
void spker_eqRch_init(void);
#endif	//SUPPORT_SPKER_STEREO_EQ
void eq_module_reset(uint8_t eqModule);
void eq_module_channel_switch(uint8_t eqModule ,uint8_t channel ,uint8_t onoff);
uint8_t eq_para_update(uint8_t eqModule ,uint8_t index ,eq_band_t para);
void spker_equ_do_process(int32_t *data ,uint32_t sampleRate);
void mic_equ_do_process(int32_t *data , uint32_t sampleRate);
void eq_module_init(void);
#endif	//SUPPORT_DSP_EQ

#ifdef __cplusplus
}
#endif


#endif /* __DSP_EQ__ */
