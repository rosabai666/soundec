/** 
***********************************************************
 * 
 * @file name	: audio_stream.c
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2022-02-18
 * @brief		: Audio stream build and process
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
#include "audio_fifo_manager.h"
#include "audio_stream.h"
#include "audio_session.h"
#include "audio_manager.h"

#if (AUDIO_MANAGER)
/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -------------------------------------------------*/
void auStream_set_samplerate(auSessionId_t id, auCtrlPath_t path, auStrmFormat_t *format);
void auStream_set_mute(auSessionId_t id, auCtrlPath_t path, uint8_t *mute);
void auStream_set_volume(auSessionId_t id, auCtrlPath_t path, uint8_t *volL, uint8_t *volR);

/* Private functions ---------------------------------------------------------*/
auManagerSt_t auStream_create_fifo(auFifo_t **stream, auStrmFormat_t *format, void *sData, auDeviceId_t deviceId)
{
	if(format == NULL)		return AUDIO_MNGR_FAIL;
		
//	uart_printf("auStream_create_fifo frameLength:%d, fifoSizeInByte:%d ms, sData:%x\n", format->frameLength, format->frameLength, sData);
	*stream = creat_fifo(format, sData, deviceId);
	if (*stream) 
		return AUDIO_MNGR_SUCCESS;
	else 
		return AUDIO_MNGR_FAIL;
}

auManagerSt_t auStream_create_fifo_share(auFifo_t **stream, auFifo_t *shareFifo, auStrmFormat_t *format, void *sData, auDeviceId_t deviceId)
{
	if(format == NULL)		return AUDIO_MNGR_FAIL;
	
	uart_printf("=share fifo:%08x, frameLength:%d, sData:%x \n", shareFifo, format->frameLength, sData);
	*stream = creat_fifo_share(shareFifo, sData, deviceId);
	uart_printf("=share success:%08x \n", *stream);
	if (*stream) 
		return AUDIO_MNGR_SUCCESS;
	else 
		return AUDIO_MNGR_FAIL;
}

auManagerSt_t auStream_register_event_on_fifo(auFifo_t *stream, auFifoEvent_t *param)
{
	if(param == NULL)		return AUDIO_MNGR_FAIL;

	register_fifo_event(stream, param);
	return AUDIO_MNGR_SUCCESS;
}

auManagerSt_t auStream_reset_fifo(void *stream)
{
//	return AUDIO_MNGR_SUCCESS;
	auFifo_t *auFifo = ((auStream_t*)stream)->fifo;
	if (auFifo->fuc->reset)		auFifo->fuc->reset(auFifo, *((auStream_t*)stream)->format);
	return AUDIO_MNGR_SUCCESS;
}


void auStream_cal_gain(int32_t *gain, int32_t *__restrict in, int32_t *__restrict out)
{
//	uart_printf("%x\n", *gain);
	ae_f32x2 *__restrict pa = (ae_f32x2*)gain;
	ae_f32x2 *__restrict pb = (ae_f32x2*)in;
	ae_f32x2 *__restrict pc = (ae_f32x2*)out;
	ae_f32x2 d0, d1, d;

	AE_LF32X2_IP(d0, pa, 0);
	AE_LF32X2_IP(d1, pb, 0);
	d = AE_MULFP32X2RAS(d0, d1);
	AE_SF32X2_IP(d, pc, 0);
}

static uint8_t auStream_find_session(auStream_t *arg, auSessionId_t sID)
{
	if (arg == NULL)  return AUDIO_MNGR_FAIL;
	uint8_t streamNum = 0;
	auStream_t **stream = NULL;
	uint32_t num = 0;//, sessionId = 0;

	auSession_t *session = auSession_get_session(sID);//&audioManager.session[id];
	if (arg->streamRole == STREAM_PLAYBACK)
	{
		streamNum = session->playbackNum;
		stream = session->playback;
	}
	else
	{
		streamNum = session->captureNum;
		stream = session->capture;
	}
	
	for(num=0; num<streamNum; num++)
	{
		if (stream[num]->devId == arg->devId)
			return AUDIO_MNGR_SUCCESS;
	}

	return AUDIO_MNGR_FAIL;
}

void auStream_update_info(void *arg, auCtrlPath_t path, auInfoType_t type, void *info)
{
	uint32_t  sessionId = 0;//num = 0,

	if (info == NULL)	return;
	auStream_t *stream = (auStream_t *)arg;
//	uart_printf("dev[%d][%s][%x]upd [%d] info[%x]\n", stream->devId, stream->device->name, stream, type, info);

	switch(type)
	{
		case INFO_FORMAT:
			{
				auStrmFormat_t *format = ((auStrmFormat_t *)info);
				if (stream->format->sampleRateHz == format->sampleRateHz)  return;
				stream->format->bitSlot = format->bitSlot;
				stream->format->channlesPerFrame = format->channlesPerFrame;
				stream->format->sampleRateHz = format->sampleRateHz;
				if (stream->ops->transforBitSlot)
					stream->formatCap->wordSize = stream->ops->transforBitSlot(format->bitSlot);
				else
					stream->formatCap->wordSize = format->bitSlot;

				for (sessionId = 0; sessionId<AUDIO_SESSION_MAX; sessionId++)
				{
					if (auStream_find_session((auStream_t *)arg, sessionId) == AUDIO_MNGR_SUCCESS)
					{						
						auStream_set_samplerate(sessionId, CAPTURE_PATH, format);
#if(!SUPPORT_RESAMPLER)
						auStream_set_samplerate(sessionId, PLAYBACK_PATH, format);
#endif
					}
				}
			}
			break;

		case INFO_VOL_LEFT:
			for (sessionId = 0; sessionId<AUDIO_SESSION_MAX; sessionId++)
			{
				if (auStream_find_session((auStream_t *)arg, sessionId) == AUDIO_MNGR_SUCCESS)
				{
					auStream_set_volume(sessionId, path, (uint8_t *)info, NULL);
				}
			}
			break;

		case INFO_VOL_RIGHT:
			for (sessionId = 0; sessionId<AUDIO_SESSION_MAX; sessionId++)
			{
				if (auStream_find_session((auStream_t *)arg, sessionId) == AUDIO_MNGR_SUCCESS)
				{
					auStream_set_volume(sessionId, path, NULL, (uint8_t *)info);
				}
			}
			break;
	
		case INFO_VOL_STEREO:
			for (sessionId = 0; sessionId<AUDIO_SESSION_MAX; sessionId++)
			{
				if (auStream_find_session((auStream_t *)arg, sessionId) == AUDIO_MNGR_SUCCESS)
				{
					auStream_set_volume(sessionId, path, (uint8_t *)info, (uint8_t *)info);
				}
			}
			break;

		case INFO_MUTE:
			for (sessionId = 0; sessionId<AUDIO_SESSION_MAX; sessionId++)
			{
				if (auStream_find_session((auStream_t *)arg, sessionId) == AUDIO_MNGR_SUCCESS)
				{
					auStream_set_mute(sessionId, path, (uint8_t *)info);
				}
			}
			break;
	}
}

auStreamDetect_t auStream_detect_playback_signal(int32_t *spkDat, uint32_t datThr, uint32_t *cal, auStrmFormat_t *format)
{
	bool playing = 0;

#if 0	//TBD, joint with usb mute and vol down to 0 mute
	if(volSpkMuteFlag || usbSpkMuteFlag)
	{
		playing = false;
		return;
	}
#endif
	for(uint8_t i=0; i<format->channlesPerFrame; i++)
	{
		if(spkDat[i]!=0)
		{
			if(format->bitSlot == BIT_SLOT_16)
			{
				if(spkDat[i]!=65536 && spkDat[i]!=-65536)
				{
					playing = 1;
					break;
				}
			}
			else
			{
				playing = 1;
				break;
			}
		}
	}

	if(playing)
	{
		*cal = 0;
		return AUDIO_STREAM_ACTIVE;
	}
	else
	{
		if(*cal == 0xFFFFFFFF)
		{
			return AUDIO_STREAM_IDLE;
		}
		else
		{
			*cal = *cal + 1;
			if(*cal >= 2 * format->sampleRateHz / format->channlesPerFrame)
			{
				*cal = 0xFFFFFFFF;
				return AUDIO_STREAM_IDLE;
			}
			return AUDIO_STREAM_ACTIVE;
		}
	}
}

auStreamDetect_t auStream_detect_capture_signal(int32_t *micDat, uint32_t datThr, uint32_t *cal, auStrmFormat_t *format)
{
	bool playing = 0;

#if 0	//TBD, joint with usb mute and vol down to 0 mute
	if(volMicMuteFlag || usbMicMuteFlag)
	{
		playing = false;
		return;
	}
#endif

	for(uint8_t i=0; i<format->channlesPerFrame; i++)
	{
		if(micDat[i] > datThr)
		{
			playing = 1;
			break;
		}
	}

	if(playing)
	{
		*cal = 0;
		return AUDIO_STREAM_ACTIVE;
	}
	else
	{
		if(*cal == 0xFFFFFFFF)
		{
			return AUDIO_STREAM_IDLE;
		}
		else
		{
			*cal = *cal + 1;
			if(*cal >= 2 * format->sampleRateHz / format->channlesPerFrame)
			{
				*cal = 0xFFFFFFFF;
				return AUDIO_STREAM_IDLE;
			}
			return AUDIO_STREAM_ACTIVE;
		}
	}
}

#endif

