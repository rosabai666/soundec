/** 
***********************************************************
 * 
 * @file name	: playTone.h
 * @author		: yingen.yu
 * @version		: V0.1
 * @date		: 2021-10-28
 * @brief		: declare for playTone
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLAYTONE_H__
#define __PLAYTONE_H__

/* Includes ------------------------------------------------------------------*/
#if D_USE_DC_WAV
#include "DC_wav.h"
#elif D_USE_WAV_8K8BIT
#include "wav_8k8bit.h"
#elif D_USE_SOUNDEC_WAV
#include "soundec_wav.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	PLAY_TONE_DAC,
	PLAY_TONE_I2S,
	PLAY_TONE_USB,
	PLAY_TONE_MAX,
}eToneOutputSrc;
/* Public functions ---------------------------------------------------------*/
void PlayToneInit();
#if (PREEMPTIVE_MODE == 1)
int PlayToneResetFor2nd(void);
int PlayToneDetectFor2nd(void);
#endif
int PlayToneSwitch(U8 playId);
int PlayToneString(uint8_t* strTab, uint8_t len);
#if (PREEMPTIVE_MODE == 1)
int PlayToneGetData(eToneOutputSrc src, int32_t* readBuf, float* fadeScale, float* fadeScaleTone, uint32_t samplerate);
#else
int PlayToneGetData(eToneOutputSrc src, int32_t* readBuf, float* fadeScale, uint32_t samplerate);
#endif

int PlayToneLoadDataProc(void);

#ifdef __cplusplus
}
#endif

#endif  /* __PLAYTONE_H__ */


