/** 
***********************************************************
 * 
 * @file name	: speech.h
 * @author		: yen.yu
 * @version		: Vxx.xx
 * @date		: 2022-03-22
 * @brief		: header file for alg
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
#ifndef __SPEECH_H__
#define __SPEECH_H__

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "boardConfig.h"

#ifdef __cplusplus
extern "C" {
#endif


#if SUPPORT_ALG_2MIC_MEETING_BOX

void FrtEndProcInit(void);
void FrtEndProcFini(void);
void FrtEndProc( int32_t* mic_in,  int32_t* ref_in, int32_t* sig_out);
void AECProc(void);
#elif SUPPORT_ALG_4MIC_MEETING_BOX

void FrtEndProcInit(void);
void FrtEndProcFini(void);
void FrtEndProc( int32_t* mic_in,  int32_t* ref_in, int32_t* sig_out);
void AECProc(void);
void FrtEndProc_new(int32_t* mic_in0,int32_t* mic_in1,int32_t* mic_in2,int32_t* mic_in3, int32_t* ref_in, int32_t* sig_out);
#elif SUPPORT_ALG_2MIC_HEADSET

void FrtEndProcInit(void);
void FrtEndProcFini(void);
void FrtEndProc( int32_t* mic_in,int32_t* sig_out);
void FrtEndProc_new( int32_t* mic_in0,int32_t* mic_in1,int32_t* mic_in2,int32_t* sig_out);
void AECProc(void);
void AECProc2(void);
void NoiseEs4(float* spec_magn,float* noise_es);
void NoiseEs3(float* spec_magn,float* noise_es);
void SpecSubtractiveG(float* spec, float* Noise, int data_len, float* spec_G, float* previousEstimateStsa);
#elif SUPPORT_ALG_2MIC_HEADSET_YG
void FrtEndProcInit(void);
void FrtEndProcFini(void);
void FrtEndProc( int_32* mic_in,int_32* ref_in,   int_32* sig_out);
void AECProc(void);
void AECProc2(void);
void NoiseEs4(float* spec_magn,float* noise_es);
void NoiseEs3(float* spec_magn,float* noise_es);
void SpecSubtractiveG(float* spec, float* Noise, int data_len, float* spec_G, float* previousEstimateStsa);


#elif SUPPORT_ALG_2MIC_littlebee

// 全向模式
/*
 * 注： NS_Level = 0	 --->不降噪
 *      NS_Level  分别为 1，2，3，4，5，6时降噪等级逐渐提升
*/
void FrtEndProcInit(void);
void FrtEndProcFini(void);
void FrtEndProc( int* mic_in,  int* sig_out, int NS_Level, float key_sound_val);

// 前指向模式
void FrtEndProcInit_DMA(void);
void FrtEndProcFini_DMA(void);
void FrtEndProc_DMA( int* mic_in,  int* sig_out, int NS_Level, float key_sound_val);

#endif


#ifdef __cplusplus
}
#endif

#endif  /* __SPEECH_H__ */

