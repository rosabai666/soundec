/** 
***********************************************************
 * 
 * @file name	: sample_auto_adaptive.c
 * @author		: xxxx
 * @version		: Vxx.xx
 * @date		: 2021-07-28
 * @brief		: FS AUTO ADAPTIVE FUNCTION
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

/* Includes ------------------------------------------------------------------*/
#include "i2s_sample_rate_detect.h"
#include "drv_codec.h"
#include "stdlib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t sampleRateList[FS_SUPPORT_NUM] = {8000, 16000, 44100, 48000, 88200, 96000};
i2sSrDet_t sampleRateDet;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void i2s_sr_detect_count(void)
{
	sampleRateDet.count++;
}

void sample_rate_switch(uint32_t sampleRate)
{
	if(sampleRate == sampleRateDet.recordSampleRate) 
	{
		sampleRateDet.checkCount = 0;
		if(sampleRateDet.unmuteCount)
		{
		    sampleRateDet.unmuteCount--;
			if(codec_dac_get_mute() && sampleRateDet.unmuteCount == ACTION_POINT)
			{
				sampleRateDet.unmuteCount = 0;
			    codec_set_dac_mute(FALSE);
			}
		}
		return ;
	}
	
	if(++sampleRateDet.checkCount < CHECK_NUM)
	{
		if(sampleRateDet.checkCount > DETECT_SAMPLE_DEBOUNCE_MUTE_COUNT)
		{
		    codec_set_dac_mute(TRUE);
			sampleRateDet.unmuteCount = CHECK_NUM;
		}
		return;
	}
	
	codec_set_dac_mute(FALSE);

	sampleRateDet.recordSampleRate = sampleRate ;
	uart_printf("recordSampleRate = %d.\r\n", sampleRateDet.recordSampleRate);
#if (D_CONFIG_2MIC_BT_HEADPHONE)
	codec_set_dac_samplerate(sampleRateDet.recordSampleRate);
	//codec_set_adc12_samplerate(sampleRateDet.recordSampleRate);
#endif

}

void i2s_sr_detect_process_ms(void)
{
   uint32_t sample_count =  sampleRateDet.count;
   sampleRateDet.count = 0;
   for( uint8_t i = 0; i < FS_SUPPORT_NUM; i ++)
   {
     if(sample_count - sampleRateList[i]/1000 == 0)
	 {
    	 sampleRateDet.curSampleRate = sampleRateList[i] ;
    	 sample_rate_switch(sampleRateDet.curSampleRate);
		 break;
	 }
   }
}

uint32_t get_i2s_sample_rate(void)
{
	return sampleRateDet.recordSampleRate;
}

