/** 
***********************************************************
 * 
 * @file name	: dsp_eq_para.c
 * @author	:     nick
 * @version	:     Vxx.xx
 * @date		: 2022-03-12
 * @brief		: dsp_eq_para
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
#include "dsp_eq_para.h"

#if (SUPPORT_DSP_EQ) 

#if (SUPPORT_SPKER_EQ)
Gcore_DspBiquadParams_t spker_eqLParam[EQ_TAP_NUM] = 
{
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_HIGH_PASS,
		.fc_Hz = 20,
		.qbw = 1,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 120,
		.qbw = 1 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 150,
		.qbw = 0.2 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 220,
		.qbw = 0.5,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 300,
		.qbw = 0.2,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 800,
		.qbw = 2 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 1000,
		.qbw = 0.2 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 1500,
		.qbw = 2 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 4000,
		.qbw = 0.2 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_LOW_PASS,
		.fc_Hz = 8000,
		.qbw = 1,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	
};

#if (SUPPORT_SPKER_STEREO_EQ)
Gcore_DspBiquadParams_t spker_eqRParam[EQ_TAP_NUM] = 
{
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_HIGH_PASS,
		.fc_Hz = 20,
		.qbw = 1,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 120,
		.qbw = 1 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 150,
		.qbw = 0.2 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 220,
		.qbw = 0.5,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 300,
		.qbw = 0.2,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 800,
		.qbw = 2 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 1000,
		.qbw = 0.2 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 1500,
		.qbw = 2 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 4000,
		.qbw = 0.2 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_LOW_PASS,
		.fc_Hz = 8000,
		.qbw = 1,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	
};
#endif
#endif

#if (SUPPORT_MIC_EQ)
Gcore_DspBiquadParams_t mic_eqParam[EQ_TAP_NUM] = 
{
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_HIGH_PASS,
		.fc_Hz = 20,
		.qbw = 1,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 120,
		.qbw = 1 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 150,
		.qbw = 0.2 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 220,
		.qbw = 0.5,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 300,
		.qbw = 0.2,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 800,
		.qbw = 2 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 1000,
		.qbw = 0.2 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 1500,
		.qbw = 2 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING,
		.fc_Hz = 4000,
		.qbw = 0.2 ,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	{
		.type = GCORE_TYPE_BIQUAD_FILTER_LOW_PASS,
		.fc_Hz = 8000,
		.qbw = 1,
		.gain_dB = 0,
		.shift_bits = BIQ_SHIFT,
	},
	
};
#endif

#endif

