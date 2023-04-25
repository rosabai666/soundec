/** 
***********************************************************
 * 
 * @file name	: audio_hw_if.c
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2022-02-11
 * @brief		: Audio hardware device controller and register interface.
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

/* Includes -----------------------------------------------------------------*/
#include "common.h"
#include "audio_def.h"
#include "audio_hw_if.h"
#include "audio_manager.h"
#include "drv_codec.h"

#if (AUDIO_MANAGER)
/* Private typedef -----------------------------------------------------------*/

/* Private macro ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
#ifdef TEST_AU_MANAGER
uint32_t rxPushSlot = 0;
uint32_t txPopSlot = 0;
auStream_t *rxStream = NULL;
auStream_t *txStream = NULL;
#endif /*TEST_AU_MANAGER*/

int8_t auRxCompleteCB(void *arg, void *data, uint32_t *len, auSlotSize_t bitSlot)
{
//	if (arg == NULL)  return AUDIO_MNGR_FAIL;
	auStream_t *stream = (auStream_t *)arg;
	
	if(!stream->var.suspend)
	{
#ifdef TEST_AU_MANAGER
		stream->var.pushPopSlot = bitSlot;
		stream->var._1sLen += *len;
#endif
		fifo_add_data(stream->fifo, data, *len, bitSlot);
		auSession_t *pSession = (auSession_t *)stream->fifo->fuc->get_sdata(stream->fifo);
		if(fifo_is_enough(stream->fifo) || fifo_is_empty(stream->fifo))
		{
			if(pSession->proc)
			{
				if (stream->ops->tuneSampleRate && fifo_get_playing_state(pSession->playback[0]->fifo))
					stream->ops->tuneSampleRate(fifo_get_data_len(pSession->playback[0]->fifo), fifo_get_fifo_size(pSession->playback[0]->fifo));
			}
			else
			{
				if (stream->ops->tuneSampleRate && fifo_get_playing_state(stream->fifo))
					stream->ops->tuneSampleRate(fifo_get_data_len(stream->fifo), fifo_get_fifo_size(stream->fifo));
			}
		}

	}
	return AUDIO_MNGR_FAIL;
}

/* Len: in Bytes*/
int8_t auTxCompleteCB(void *arg, void *data, uint32_t *len, auSlotSize_t bitSlot)
{
//	if (arg == NULL)  return AUDIO_MNGR_FAIL;
	auStream_t *stream = (auStream_t *)arg;

	if(!stream->var.suspend)
	{
#ifdef TEST_AU_MANAGER
		stream->var.pushPopSlot = bitSlot;
		stream->var._1sLen += *len;
#endif
		fifo_get_data(stream->fifo, data, len, bitSlot);

		if(fifo_is_enough(stream->fifo) || fifo_is_empty(stream->fifo))
		{
			if (stream->ops->tuneSampleRate)
				stream->ops->tuneSampleRate(fifo_get_data_len(stream->fifo), fifo_get_fifo_size(stream->fifo));
		}

	}
	return AUDIO_MNGR_FAIL;
}

auManagerSt_t auDevive_init(void)
{
	auDeviceId_t i = 0;

	for (i = 0; i<AUDIO_DEVICE_MAX; i++)
	{
		if (auDeviceIfs[i].load)
		{
			uart_printf("init device [%s].\n", auDeviceIfs[i].name);
			auDeviceIfs[i].ops->init(auDeviceIfs[i].rxCfgs, auDeviceIfs[i].txCfgs);	
//			uart_printf("init ok [%s].\n", auDeviceIfs[i].name);
		}
	}
	return AUDIO_MNGR_SUCCESS;
}
#endif

