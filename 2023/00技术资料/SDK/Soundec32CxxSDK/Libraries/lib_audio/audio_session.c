/**
***********************************************************
 *
 * @file name	: audio_session.c
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2022-02-09
 * @brief		: This file used for audio session management, 2 sessions for
 * general use
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
#include "audio_def.h"
#include "audio_hw_if.h"
#include "audio_session.h"
#include "uac.h"
#include "drv_codec_cfg.h"

#if (AUDIO_MANAGER)
/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
auSession_t *auSession_get_session(auSessionId_t id);

/* Private functions ---------------------------------------------------------*/
codec_adc_vol_t sidetone_vol_table[84] =
{
	{HAL_CODEC_GIM_20DB,	0x00000000},
	{HAL_CODEC_GIM_20DB,	0xffffffff},
	{HAL_CODEC_GIM_18DB,	0x00000000},
	{HAL_CODEC_GIM_18DB,	0xffffffff},
	{HAL_CODEC_GIM_16DB,	0x00000000},
	{HAL_CODEC_GIM_16DB,	0xffffffff},
	{HAL_CODEC_GIM_14DB,	0x00000000},
	{HAL_CODEC_GIM_14DB,	0xffffffff},
	{HAL_CODEC_GIM_12DB,	0x00000000},
	{HAL_CODEC_GIM_12DB,	0xffffffff},
	{HAL_CODEC_GIM_10DB,	0x00000000},
	{HAL_CODEC_GIM_10DB,	0xffffffff},
	{HAL_CODEC_GIM_8DB,		0x00000000},
	{HAL_CODEC_GIM_8DB,		0xffffffff},
	{HAL_CODEC_GIM_6DB,		0x00000000},
	{HAL_CODEC_GIM_6DB,		0xffffffff},
	{HAL_CODEC_GIM_4DB,		0x00000000},
	{HAL_CODEC_GIM_4DB,		0xffffffff},
	{HAL_CODEC_GIM_2DB,		0x00000000},
	{HAL_CODEC_GIM_2DB,		0xffffffff},
	{HAL_CODEC_GIM_0DB,		0x00000000},
	{HAL_CODEC_GIM_0DB,		0xffffffff},	//-1
	{HAL_CODEC_GIM_M2DB,	0x00000000},	//-2

	{HAL_CODEC_GIM_M2DB,	0xffffffff},	//
	{HAL_CODEC_GIM_M2DB,	0xfffffffe},	//
	{HAL_CODEC_GIM_M2DB,	0xfffffffd},	//
	{HAL_CODEC_GIM_M2DB,	0xfffffffc},	//
	{HAL_CODEC_GIM_M2DB,	0xfffffffb},	//
	{HAL_CODEC_GIM_M2DB,	0xfffffffa},	//
	{HAL_CODEC_GIM_M2DB,	0xfffffff9},	//
	{HAL_CODEC_GIM_M2DB,	0xfffffff8},	//
	{HAL_CODEC_GIM_M2DB,	0xfffffff7},	//
	{HAL_CODEC_GIM_M2DB,	0xfffffff6},	//
	{HAL_CODEC_GIM_M2DB,	0xfffffff5},	//
	{HAL_CODEC_GIM_M2DB,	0xfffffff4},	//
	{HAL_CODEC_GIM_M2DB,	0xfffffff3},	//
	{HAL_CODEC_GIM_M2DB,	0xfffffff2},	//
	{HAL_CODEC_GIM_M2DB,	0xfffffff1},	//
	{HAL_CODEC_GIM_M2DB,	0xfffffff0},	//

	{HAL_CODEC_GIM_M2DB,	0xffffffef},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffee},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffed},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffec},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffeb},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffea},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffe9},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffe8},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffe7},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffe6},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffe5},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffe4},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffe3},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffe2},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffe1},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffe0},	//

	{HAL_CODEC_GIM_M2DB,	0xffffffdf},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffde},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffdd},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffdc},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffdb},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffda},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffd9},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffd8},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffd7},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffd6},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffd5},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffd4},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffd3},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffd2},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffd1},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffd0},	//

	{HAL_CODEC_GIM_M2DB,	0xffffffcf},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffce},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffcd},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffcc},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffcb},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffca},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffc9},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffc8},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffc7},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffc6},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffc5},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffc4},	//
	{HAL_CODEC_GIM_M2DB,	0xffffffc3},	//
};

ALIGN(4) int32_t spk_vol_table[64] =
{
	0x7fffffff,	//0db
	0x72148280,	//-1db
	0x65ac8c00,	//-2db
	0x5a9df780,	//-3db
	0x50c33580,	//-4db
	0x47facd00,	//-5db
	0x4026e700,	//-6db
	0x392cedc0,	//-7db
	0x32f52d00,	//-8db
	0x2d6a8640,	//-9db
	0x287a2700,	//-10db
	0x24134700,	//-11db
	0x2026f300,	//-12db
	0x1ca7d760,	//-13db
	0x198a1360,	//-14db
	0x16c31100,	//-15db
	0x144960a0,	//-16db
	0x12149a40,	//-17db
	0x101d3f20,	//-18db
	0x0e5ca160,	//-19db
	0x0cccccd0,	//-20db
	0x0b687360,	//-21db
	0x0a2adac0,	//-22db
	0x090fcc10,	//-23db
	0x08138560,	//-24db
	0x0732ae20,	//-25db
	0x066a4a48,	//-26db
	0x05b7b150,	//-27db
	0x05188490,	//-28db
	0x048aa6f8,	//-29db
	0x040c3718,	//-30db
	0x039b8714,	//-31db
	0x03371848,	//-32db
	0x02dd9594,	//-33db
	0x028dceb0,	//-34db
	0x0246b4e4,	//-35db
	0x02075678,	//-36db
	0x01cedc38,	//-37db
	0x019c8656,	//-38db
	0x016fa9b4,	//-39db
	0x0147ae16,	//-40db
	0x01240b90,	//-41db
	0x01044912,	//-42db
	0x00e7fac9,	//-43db
	0x00cec086,	//-44db
	0x00b8449d,	//-45db
	0x00a43aa5,	//-46db
	0x00925e88,	//-47db
	0x008273a5,	//-48db
	0x007443e5,	//-49db
	0x00679f1b,	//-50db
	0x005c5a51,	//-51db
	0x00524f3a,	//-52db
	0x00495bc0,	//-53db
	0x00416178,	//-54dbs
	0x003a454a,	//-55db
	0x0033ef0d,	//-56db
	0x002e4938,	//-57db
	0x002940a0,	//-58db
	0x0024c42c,	//-59db
	0x0020c49b,	//-60db
	0x001d345b,	//-61db
	0x001a074e,	//-62db
	0x00000000,	//0
};

int8_t curSidetoneVol = 0xff;
uint8_t sideToneMute = 0;

void set_sidetone_mute(uint8_t mode)
{
	static uint8_t curMode = 0xff;

	if (curMode == mode)
		return;

	curMode = mode;

	if (mode)
	{
		hal_codec_select_dac_mixer_mode(HAL_CODEC_PLAYBACK_DAC_ONLY);
	}
	else
	{
		if(g_dac_config.dac_mix == HAL_CODEC_PLAYBACK_DAC_MIXED_ADC)
		{
			hal_codec_select_dac_mixer_mode(HAL_CODEC_PLAYBACK_DAC_MIXED_ADC);
		}
	}
}


void app_codec_set_sidetone_mute(uint8_t mode)
{
	sideToneMute = mode;

	if((curSidetoneVol == -128) && (mode == 0))
		return;
	set_sidetone_mute(mode);
}

void app_codec_set_sidetone_volume(int8_t vol)
{
	if(vol == curSidetoneVol)
		return;

	curSidetoneVol = vol;

	if(vol == -128)
	{
		set_sidetone_mute(1);
	}
	else
	{
		if(sideToneMute == 0)
		{
			set_sidetone_mute(0);
		}

		int volume = 12 - vol;
#if (CODEC_ADC12_ENABLE)
		codec_set_adc12_gain(sidetone_vol_table[volume].digital_gain, 0, CHANNEL_L);
		codec_set_adc12_gain(sidetone_vol_table[volume].digital_gain, 0, CHANNEL_R);
#if (ADC12_IS_AMIC)
		codec_set_adc12_gain(sidetone_vol_table[volume].analog_gain, 1, CHANNEL_L);
		codec_set_adc12_gain(sidetone_vol_table[volume].analog_gain, 1, CHANNEL_R);
#endif
#endif
	}
}

void auStream_set_volume(auSessionId_t id, auCtrlPath_t path, uint8_t *volL, uint8_t *volR)
{
	auSession_t *session = auSession_get_session(id);//&audioManager.session[id];
	//auPolicyParam_t *policy = ((auSession_t *)session)->sessionPolicy;
	uint8_t num = 0, cnt = 0, tmpVolL = 0, tmpVolR = 0;
	auStream_t **stream;

	if (path == CAPTURE_PATH)
	{
		num = session->captureNum;
		stream = session->capture;

        //uart_printf("v[CAP %d]:%d, %d\n:", num, stream[0]->var.swgain[CHANNEL_L], stream[0]->var.swgain[CHANNEL_R]);

		for(cnt=0; cnt<num; cnt++)
		{
			if (stream[cnt]->var.useSwGain)
			{
				tmpVolL = (USB_DEV_MIC_VOL_MAX >> 8) - *volL;
				if(tmpVolL <= 63)
				{
					stream[cnt]->var.swgain[CHANNEL_L] = spk_vol_table[tmpVolL];
					stream[cnt]->var.swgain[CHANNEL_R] = spk_vol_table[tmpVolL];
				}
				else
				{
					stream[cnt]->var.swgain[CHANNEL_L] = 0;
					stream[cnt]->var.swgain[CHANNEL_R] = 0;
				}
			}
			else
			{
				stream[cnt]->var.swgain[CHANNEL_L] = 0x7fffffff;	// No sw gain table for mic volume, default to 0dB
				stream[cnt]->var.swgain[CHANNEL_R] = 0x7fffffff;	// No sw gain table for mic volume, default to 0dB

				if (volL)
				{
					tmpVolL = (USB_DEV_MIC_VOL_MAX >> 8) - *volL;
					if(tmpVolL <= 63)
						stream[cnt]->var.dgain[CHANNEL_L] = MIC_DGAIN_MAX - tmpVolL;
					else
						stream[cnt]->var.dgain[CHANNEL_L] = -64;
				}
				if (volR)
				{
					tmpVolR = (USB_DEV_MIC_VOL_MAX >> 8) - *volR;
					if(tmpVolR <= 63)
						stream[cnt]->var.dgain[CHANNEL_R] = MIC_DGAIN_MAX - tmpVolR;
					else
						stream[cnt]->var.dgain[CHANNEL_R] = -64;
				}

				if (stream[cnt]->ops->setGain)
				{
					stream[cnt]->ops->setGain(stream[cnt]->var.dgain[CHANNEL_L], 0, CHANNEL_L);
					stream[cnt]->ops->setGain(stream[cnt]->var.dgain[CHANNEL_R], 0, CHANNEL_R);
				}
			}
		}
	}
	else if (path == PLAYBACK_PATH)
	{
		num = session->playbackNum;
		stream = session->playback;

		for(cnt=0; cnt<num; cnt++)
		{
			if (stream[cnt]->var.useSwGain)
			{

				if (volL)
				{
					tmpVolL = -*volL;
					stream[cnt]->var.swgain[CHANNEL_L] = spk_vol_table[tmpVolL];
				}
				if (volR)
				{
					tmpVolR = -*volR;
					stream[cnt]->var.swgain[CHANNEL_R] = spk_vol_table[tmpVolR];
				}
//				uart_printf("v[PLAY %d]:%d, %d\n:", num, tmpVolL, tmpVolR);
//				uart_printf("sg:%x, %x\n", stream[cnt]->var.swgain[CHANNEL_L],

			}
			else
			{
//				uart_printf("hg=0dB");
				stream[cnt]->var.swgain[CHANNEL_L] = 0x7fffffff;	// No sw gain table for mic volume, default to 0dB
				stream[cnt]->var.swgain[CHANNEL_R] = 0x7fffffff;	// No sw gain table for mic volume, default to 0dB

				/*stream[cnt]->var.again[CHANNEL_L] = 1;//HAL_CODEC_GIM_0DB;
				stream[cnt]->var.again[CHANNEL_R] = 1;//HAL_CODEC_GIM_0DB;

				stream[cnt]->var.dgain[CHANNEL_L] = 0x00000000;
				stream[cnt]->var.dgain[CHANNEL_R] = 0x00000000;
				if (stream[cnt]->ops->setGain)
				{
					stream[cnt]->ops->setGain(stream[cnt]->var.again[CHANNEL_L], 1, CHANNEL_L);
					stream[cnt]->ops->setGain(stream[cnt]->var.again[CHANNEL_R], 1, CHANNEL_R);

					stream[cnt]->ops->setGain(stream[cnt]->var.dgain[CHANNEL_L], 0, CHANNEL_L);
					stream[cnt]->ops->setGain(stream[cnt]->var.dgain[CHANNEL_R], 0, CHANNEL_R);
				}
				*/

			}
		}
	}
	else
	{
#if (SUPPORT_SPK_SIDETONE && CODEC_ENABLE)
		app_codec_set_sidetone_volume(*volL);
#endif

	}
}
void auStream_set_mute(auSessionId_t id, auCtrlPath_t path, uint8_t *mute)
{
//	uart_printf("session[%d]set mute:%d.\n", id, *mute);

	auSession_t *session = auSession_get_session(id);//&audioManager.session[id];
	//auPolicyParam_t *policy = ((auSession_t *)session)->sessionPolicy;
	uint8_t num = 0, cnt = 0;
	auStream_t **stream;

	if (path == CAPTURE_PATH)
	{
//		uart_printf("mute[CAP]:");
//		policy->captureVol.mute = *mute;
		num = session->captureNum;
		stream = session->capture;
	}
	else if (path == PLAYBACK_PATH)
	{
//		uart_printf("mute[PLAY]:");
//		policy->playbackVol.mute = *mute;
		num = session->playbackNum;
		stream = session->playback;
	}
	else
	{
#if (SUPPORT_SPK_SIDETONE && CODEC_ENABLE)
		app_codec_set_sidetone_mute(*mute);
#endif
	}

	for(cnt=0; cnt<num; cnt++)
	{
		stream[cnt]->var.mute = *mute;
		if (stream[cnt]->ops->setMute)
		{
			stream[cnt]->ops->setMute(*mute);
		}
	}
}

void auStream_set_samplerate(auSessionId_t id, auCtrlPath_t path, auStrmFormat_t *format)
{
	if (format == NULL)  return;
//	uart_printf("session[%d]set SPR, bslot:%d, ch:%d, spr:%d.\n", id, format->bitSlot, format->channlesPerFrame, format->sampleRateHz);

	auSession_t *session = auSession_get_session(id);//&audioManager.session[id];
	auPolicyParam_t *policy = ((auSession_t *)session)->sessionPolicy;

	policy->sampleRate = format->sampleRateHz;
	policy->bitSlot = format->bitSlot;

	uint8_t num = 0, cnt = 0;
	auStream_t **stream;
	if (path == CAPTURE_PATH)
	{
//		uart_printf("Set[CAP]:");
		num = session->captureNum;
		stream = session->capture;
	}
	else if (path == PLAYBACK_PATH)
	{
//		uart_printf("Set[PLAY]:");
		num = session->playbackNum;
		stream = session->playback;
	}
	else
	{
//		uart_printf("Set path[%d] error.\n", path);
		return;
	}

	for(cnt=0; cnt<num; cnt++)
	{
		if (stream[cnt]->ops->setSampleRate)
		{
			stream[cnt]->ops->setSampleRate(format->sampleRateHz);
			if (stream[cnt]->ops->transforBitSlot)
				stream[cnt]->formatCap->wordSize = stream[cnt]->ops->transforBitSlot(format->bitSlot);
			else
				stream[cnt]->formatCap->wordSize = format->bitSlot;
		}
		stream[cnt]->format->bitSlot = format->bitSlot;
		stream[cnt]->format->channlesPerFrame = format->channlesPerFrame;
		stream[cnt]->format->sampleRateHz = format->sampleRateHz;
		auStream_reset_fifo(stream[cnt]);
//		uart_printf("stream [%d][%s], wslot:%d\n", cnt, stream[cnt]->device->name, stream[cnt]->formatCap->wordSize);
	}
}

void auSession_suspend(auSession_t* pSession, bool suspend)
{
	uint8_t cNum = pSession->captureNum;
	uint8_t pNum = pSession->playbackNum;
	uint8_t i = 0;
	for(i=0;i<cNum;i++)
	{
		pSession->capture[i]->var.suspend = suspend;
		pSession->capture[i]->fifo->fuc->reset(pSession->capture[i]->fifo, *pSession->capture[i]->format);
	}
	for(i=0;i<pNum;i++)
	{
		pSession->playback[i]->var.suspend = suspend;
		pSession->playback[i]->fifo->fuc->reset(pSession->playback[i]->fifo, *pSession->playback[i]->format);
	}
	uart_printf("suspend %d\n", suspend);
}

#endif


