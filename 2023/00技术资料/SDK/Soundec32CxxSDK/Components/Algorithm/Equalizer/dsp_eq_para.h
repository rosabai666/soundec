/** 
***********************************************************
 * 
 * @file name	: dsp_eq_para.h
 * @author	:     nick
 * @version	:     Vxx.xx
 * @date		: 2022-03-12
 * @brief		: spker eq
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
/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "equalizer.h"

#ifndef _DSP_EQ_PARA_
#define _DSP_EQ_PARA_

#define EQ_TAP_NUM		         	10
#define BIQ_SHIFT					4
#if (SUPPORT_DSP_EQ) 

#if (SUPPORT_SPKER_EQ)
extern Gcore_DspBiquadParams_t spker_eqLParam[EQ_TAP_NUM]; 
#if (SUPPORT_SPKER_STEREO_EQ)
extern  Gcore_DspBiquadParams_t spker_eqRParam[EQ_TAP_NUM];
#endif
#endif

#if (SUPPORT_MIC_EQ)
extern Gcore_DspBiquadParams_t mic_eqParam[EQ_TAP_NUM];
#endif

#endif

#endif /* _DSP_EQ_PARA_ */
