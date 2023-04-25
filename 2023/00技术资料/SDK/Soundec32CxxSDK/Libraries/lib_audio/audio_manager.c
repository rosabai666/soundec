/** 
***********************************************************
 * 
 * @file name	: audio_manager.c
 * @author		: Rosa.Bai
 * @version		: V0.2
 * @date		: 2022-02-09
 * @brief		: Use audioManager manage audioRoute
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
#include <xtensa/xtruntime.h>
#include <xtensa/config/core.h>
#include "hal_interrupt.h"
#include "audio_def.h"
#include "audio_session.h"
#include "audio_hw_if.h"
#include "audio_fifo_manager.h"
#include "audio_stream.h"
#include "audio_manager.h"

#if (AUDIO_MANAGER)
/* Private typedef ----------------------------------------------------------*/

/* Private macro -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
auManagerHandler_t audioManager;

auSession_t *auSession_get_session(auSessionId_t id)
{
	return &audioManager.session[id];
}

auManagerSt_t auManager_open_session(auSessionId_t sessionID)
{
	if(sessionID > AUDIO_SESSION_MAX)
		return AUDIO_MNGR_FAIL;
		
	auManagerSt_t st = AUDIO_MNGR_SUCCESS;
	uint8_t max = 0, cnt = 0, role = 0;
	auStream_t **stream = NULL;
	auSession_t *session = auSession_get_session(sessionID);

	for (role = 0; role<2; role++)
	{
		if (role == STREAM_CAPTURE)
		{
			stream = session->capture;
			max = session->captureNum;
		}
		else
		{
			stream = session->playback;
			max = session->playbackNum;
		}
		
		for (cnt = 0; cnt<max; cnt++)
		{

			//if (!stream[cnt]->var.useSwGain)
			{
				if (stream[cnt]->ops->setSampleRate)
				{
					stream[cnt]->ops->setSampleRate(stream[cnt]->format->sampleRateHz);
				}
			}
			
			if (stream[cnt]->device && stream[cnt]->device->ops->start) stream[cnt]->device->ops->start();
		}
	}
	
	if (session->ops && session->ops->open) session->ops->open();
	
	AU_MNGR_DEBUG("auManager_open_session %d ok.\n", sessionID);	
	
	return st;
}

auManagerSt_t auManager_close_session(auSessionId_t sessionID)
{
	auManagerSt_t st = AUDIO_MNGR_SUCCESS;
	uint8_t max = 0, cnt = 0, role = 0;
	auStream_t **stream = NULL;
	auSession_t *session = auSession_get_session(sessionID);

	for (role = 0; role<2; role++)
	{
		if (role == STREAM_CAPTURE)
		{
			stream = session->capture;
			max = session->captureNum;
		}
		else
		{
			stream = session->playback;
			max = session->playbackNum;
		}
		
		for (cnt = 0; cnt<max; cnt++)
		{
			if (stream[cnt]->device && stream[cnt]->device->ops->stop) stream[cnt]->device->ops->stop();
		}
	}
	
	if (session->ops && session->ops->close) session->ops->close();
	
	AU_MNGR_DEBUG("auManager_close_session %d ok.\n", sessionID);	
	
	return st;
}


void auManager_fifo_callback_isr(void *arg)
{
	auProcFifoCb_t *procFifo = (auProcFifoCb_t *)arg;
	SaveVectors(procFifo->irqNum);
	if(procFifo->irqNum == SW_IRQn)
		hal_interrupt_clear_irq(procFifo->irqNum);
	else
		hal_interrupt_irq_clear_force_trigger(hal_interrupt_get_intr_src(procFifo->irqNum));

//	AU_MNGR_DEBUG("irq:%d,num:%d,dly:%d\n", procFifo->irqNum, procFifo->pickFifoNum, procFifo->noLatency);
	uint8_t i=0, pickNum = 0;

	if (!procFifo->noLatency)
	{
		for (i = 0; i<procFifo->pickFifoNum; i++)
		{
			pickNum += procFifo->pickFifo[i]->fuc->is_enough(procFifo->pickFifo[i])?1:(fifo_get_playing_state(procFifo->pickFifo[i])?0:1);
		}
	}

//	AU_MNGR_DEBUG("pickNum:%d\n", pickNum);
	if (pickNum == procFifo->pickFifoNum || procFifo->noLatency)
	{
		uint32_t popLen = 0;
		uint32_t pushLen = 0;

		for (i = 0; i<procFifo->pickFifoNum; i++)
		{
			if(procFifo->pickFifo[i]->fuc->is_enough(procFifo->pickFifo[i]))
			{
				procFifo->pickAddrList[i] = (int32_t *)procFifo->pickFifo[i]->fuc->get_fifo_head(procFifo->pickFifo[i]);
				popLen = procFifo->pickFifo[i]->fuc->get_fifo_thresholdLevel(procFifo->pickFifo[i]);
				procFifo->pickFifo[i]->fuc->pop_skip(procFifo->pickFifo[i], &popLen);
			}
			else
			{
				procFifo->pickAddrList[i] = NULL;
			}
		}
		
		for (i = 0; i<procFifo->pokeFifoNum; i++)
		{
			procFifo->pokeAddrList[i] = (int32_t *)procFifo->pokeFifo[i]->fuc->get_fifo_tail(procFifo->pokeFifo[i]);
		}
//		AU_MNGR_DEBUG("popLen:%d\n", popLen);
		pushLen = procFifo->pickFifo[0]->fifo_ready_cb(popLen, procFifo->pickFifoNum, procFifo->pickAddrList, procFifo->pokeFifoNum, procFifo->pokeAddrList);
		for (i = 0; i<procFifo->pokeFifoNum; i++)
		{
			procFifo->pokeFifo[i]->fuc->push_skip(procFifo->pokeFifo[i], pushLen);
		}
//		AU_MNGR_DEBUG("pushLen:%d\n", pushLen);
	}
	
	RestoreVectors(procFifo->irqNum);
}

#if(SUPPORT_RESAMPLER)
extern void session0In_isr(void *arg);
#endif
void auManager_register_fifoready_isr(xtos_handler isr, auProcFifoCb_t *event)
{
	xtos_interrupt_disable(event->irqNum);
	xtos_interrupt_clear(event->irqNum);
#if(SUPPORT_RESAMPLER)
	xtos_set_interrupt_handler(event->irqNum, session0In_isr, event, NULL);
#else
	xtos_set_interrupt_handler(event->irqNum, auManager_fifo_callback_isr, event, NULL);
#endif
	xtos_interrupt_enable(event->irqNum);
}

// Return all of reused stream
uint8_t proc_fifo_reuse(auManagerHandler_t *auMngr, auFifo_t **fifo, auDeviceId_t devId, auSessionId_t curSession, auSessionId_t sessionID)	//uint8_t strmNum, uint8_t strmMax)
{
	if (sessionID >= AUDIO_SESSION_MAX)	return 0;

	if(proc_fifo_reuse(auMngr, fifo, devId, curSession, sessionID+1))
		return 1;

	uint8_t cnt = 0, strmNum = 0;//, reuseNum = 0;
	//auFifo_t *tmpFifo = NULL;
	auStream_t **tmpStream;

	strmNum = auMngr->session[sessionID].captureNum;
	tmpStream = auMngr->session[sessionID].capture;

	for (cnt = 0; cnt<strmNum; cnt++)
	{
		if ((tmpStream[cnt]->devId == devId) && tmpStream[cnt]->fifo) /*Fifo size fixed at begining. so no need to compare fifo size*/
		{
			*fifo = tmpStream[cnt]->fifo;
//			AU_MNGR_DEBUG("stream fifo reuse dev[%s], fifo[%08x].\n", tmpStream[cnt]->device->name, tmpStream[cnt]->fifo);
			return 1;
		}
	}
	return 0;
}

uint8_t auManager_load_fifo(auManagerHandler_t *auMngr, auSessionId_t sessionID)
{
	if (sessionID >= AUDIO_SESSION_MAX)	return 0;

	auManager_load_fifo(auMngr, sessionID+1);
	
	uint8_t cnt = 0;
	auFifo_t *tmpFifo = NULL;
	uint8_t capNum = auMngr->session[sessionID].captureNum;
	uint8_t playNum = auMngr->session[sessionID].playbackNum;
	auProcOnStream_t *procOnStream = auMngr->session[sessionID].proc;
	auStream_t *playbackStream = NULL, *captureStream = NULL;

	for (cnt = 0; cnt<capNum; cnt++)
	{
		captureStream = auMngr->session[sessionID].capture[cnt];
		if (proc_fifo_reuse(auMngr, &tmpFifo, captureStream->devId, sessionID, 0) == 0)
		{
			if (procOnStream)
			{
				if (auStream_create_fifo(&captureStream->fifo, captureStream->format,  &auMngr->session[sessionID], captureStream->devId) == AUDIO_MNGR_SUCCESS)
				{
					captureStream->fifoState = FIFO_NEW;
					procOnStream->cbEvents.pickFifo[cnt] = captureStream->fifo;
					if (procOnStream->event)   auStream_register_event_on_fifo(captureStream->fifo, procOnStream->event);
#if (DMA_ENABLE)
					if (captureStream->dmaChannel != DMAC_CHX_NULL)
					{
						fifo_register_dmac_input(captureStream->fifo, captureStream->dmaChannel);
						AU_MNGR_DEBUG("DMAC chx%d input inited\n", captureStream->dmaChannel);
					}
#endif
				}
				else
				{
					AU_MNGR_DEBUG("Fifo get Fail!!!\n");
				}
			}
			else
			{
				if (auStream_create_fifo(&captureStream->fifo, captureStream->format, &auMngr->session[sessionID], captureStream->devId) == AUDIO_MNGR_SUCCESS)
				{
					captureStream->fifoState = FIFO_NEW;
#if (DMA_ENABLE)
					if (captureStream->dmaChannel != DMAC_CHX_NULL)
					{
						fifo_register_dmac_input(captureStream->fifo, captureStream->dmaChannel);
						AU_MNGR_DEBUG("DMAC chx%d input inited\n", captureStream->dmaChannel);
					}
#endif
				}
				else
					AU_MNGR_DEBUG("Fifo get Fail!!!\n");
			}
		}				
		else
		{
			if (procOnStream)
			{
				if (auStream_create_fifo_share(&captureStream->fifo, tmpFifo, captureStream->format, &auMngr->session[sessionID], captureStream->devId) == AUDIO_MNGR_SUCCESS)
				{
					captureStream->fifoState = FIFO_SHARED;
					procOnStream->cbEvents.pickFifo[cnt] = captureStream->fifo;
					if (procOnStream->event)   auStream_register_event_on_fifo(captureStream->fifo, procOnStream->event);
				}
				else
				{
					AU_MNGR_DEBUG("Fifo get Fail!!!\n");
				}
			}
			else
			{
				captureStream->fifoState = FIFO_REUSED;
				captureStream->fifo = tmpFifo;
			}
		}
	}

	if(procOnStream)
	{
		if (auStream_create_fifo(&procOnStream->result, procOnStream->format, &auMngr->session[sessionID], AUDIO_DEVICE_NULL) == AUDIO_MNGR_SUCCESS)
		{
			procOnStream->cbEvents.irqNum = procOnStream->event->irqNum;
			procOnStream->cbEvents.noLatency = FALSE;
			procOnStream->cbEvents.pickFifoNum = capNum;
//			procOnStream->cbEvents.pokeFifo[0] = procOnStream->result;
			auManager_register_fifoready_isr(auManager_fifo_callback_isr, &procOnStream->cbEvents);
			AU_MNGR_DEBUG("result playNum:%d, pokefifo:%x\n", playNum, procOnStream->cbEvents.pokeFifo[0]);
		}
		else
		{
			AU_MNGR_DEBUG("Fifo get Fail!!!\n");
		}
	}

	for (cnt = 0; cnt<playNum; cnt++)
	{
		// according to playNum set share fifo for playbackstream;
		playbackStream = auMngr->session[sessionID].playback[cnt];
		if(procOnStream)
		{
			// ProcOnStream exists means more than one capture streams in the current session. 
			// They will foce to mixed through procOnStream->event->onfifoready, and save the result into procOnStream->result
			if (playbackStream->fifoId == 0 && procOnStream->cbEvents.pokeFifo[playbackStream->fifoId] == NULL)	
			{
				playbackStream->fifo = procOnStream->result;
				procOnStream->cbEvents.pokeFifo[playbackStream->fifoId] = playbackStream->fifo;
				procOnStream->cbEvents.pokeFifoNum = 1;
				AU_MNGR_DEBUG("playback[%d] pokefifo[%d]=%x\n", cnt, playbackStream->fifoId, procOnStream->cbEvents.pokeFifo[playbackStream->fifoId]);
			}
			else if(playbackStream->fifoId < playNum && procOnStream->cbEvents.pokeFifo[playbackStream->fifoId] == NULL)
			{
				playbackStream->format = procOnStream->format;
				if (auStream_create_fifo(&playbackStream->fifo, procOnStream->format, &auMngr->session[sessionID], AUDIO_DEVICE_NULL) == AUDIO_MNGR_SUCCESS)
				{
					if((playbackStream->fifoId + 1) > procOnStream->cbEvents.pokeFifoNum)
						procOnStream->cbEvents.pokeFifoNum = playbackStream->fifoId + 1;
					procOnStream->cbEvents.pokeFifo[playbackStream->fifoId] = playbackStream->fifo;
					auManager_register_fifoready_isr(auManager_fifo_callback_isr, &procOnStream->cbEvents);
					AU_MNGR_DEBUG("creat playback[%d]  pokefifo[%d]=%x\n", cnt, playbackStream->fifoId, procOnStream->cbEvents.pokeFifo[playbackStream->fifoId]);
				}
				else
				{
					AU_MNGR_DEBUG("Fifo get Fail!!!\n");
				}
			}
			else
			{
				AU_MNGR_DEBUG("share playback[%d] from pokefifo[%d]=%x\n", cnt, playbackStream->fifoId, procOnStream->cbEvents.pokeFifo[playbackStream->fifoId]);
				if (auStream_create_fifo_share(&playbackStream->fifo, procOnStream->cbEvents.pokeFifo[playbackStream->fifoId], playbackStream->format, &auMngr->session[sessionID], AUDIO_DEVICE_NULL) == AUDIO_MNGR_SUCCESS)
				{
					playbackStream->fifoState = FIFO_SHARED;
					//if (playbackStream->event)	 auStream_register_event_on_fifo(playbackStream->fifo, playbackStream->event);
					AU_MNGR_DEBUG("share ok playback[%d]  pokefifo[%d]=%x\n", cnt, playbackStream->fifoId, playbackStream->fifo);
				}
				else
				{
					AU_MNGR_DEBUG("Fifo get Fail!!!\n");
				}
			}
			
		}
		else
		{
			// No procOnStream also means only one capture stream in the current session.
			if (cnt == 0 && captureStream->fifoState != FIFO_REUSED)			
			{
				// In this case(no procOnStream function, also means only one captureStream exists)
				playbackStream->fifo = captureStream->fifo;
			}
			else
			{
				playbackStream->format = captureStream->format;
				if (auStream_create_fifo_share(&playbackStream->fifo, captureStream->fifo, playbackStream->format, &auMngr->session[sessionID], AUDIO_DEVICE_NULL)  == AUDIO_MNGR_SUCCESS)
				{
					playbackStream->fifoState = FIFO_SHARED;
	//				if (playbackStream->event)	 auStream_register_event_on_fifo(playbackStream->fifo, playbackStream->event);
				}
				else
				{
					AU_MNGR_DEBUG("Fifo get Fail!!!\n");
				}
				
			}

#if (DMA_ENABLE)
			if (playbackStream->dmaChannel != DMAC_CHX_NULL)
				fifo_register_dmac_output(playbackStream->fifo, playbackStream->dmaChannel);
#endif
		}
		
	    if (playbackStream->event)	 auStream_register_event_on_fifo(playbackStream->fifo, playbackStream->event);
	}
	
	return 1;
}

uint8_t load_stream(uint8_t strmNum, auSessionDesc_t *router, auStreamRole_t strmRole, auStream_t **strms, uint8_t strmMax, audioDevIf_t *devs)
{
	if (strmNum >= strmMax)		return AUDIO_MNGR_FAIL;
	
	load_stream(strmNum+1, router, strmRole, strms, strmMax, devs);
	
	if(strmRole == STREAM_CAPTURE)	
	{
		strms[strmNum] = (auStream_t *)malloc(sizeof(auStream_t));
		if (strms[strmNum] == NULL) {
			AU_MNGR_DEBUG("malloc fail!\n");
			return AUDIO_MNGR_FAIL;
		}
		memset(strms[strmNum], 0, sizeof(auStream_t));
		
		// Load stream info
		strms[strmNum]->devId 			= router->capture[strmNum]->capture.devId;
		strms[strmNum]->var.useSwGain	= devs[strms[strmNum]->devId].rxCfgs->attr->useSwGain;
		strms[strmNum]->formatCap		= devs[strms[strmNum]->devId].rxCfgs->attr->formatCap;	/*Default use information in hw_if.c, this format is output of device*/
		strms[strmNum]->format			= &router->capture[strmNum]->format;	/*Default use information in hw_if.c, this format is output of device*/
		strms[strmNum]->event			= NULL;
		strms[strmNum]->ops				= devs[strms[strmNum]->devId].rxCfgs->attr->ops;
		strms[strmNum]->dmaChannel		= devs[strms[strmNum]->devId].rxCfgs->attr->dmaChannel;
		strms[strmNum]->streamRole		= strmRole;
		
		// Load device
		strms[strmNum]->device 							= &devs[router->capture[strmNum]->capture.devId];
		strms[strmNum]->device->rxCfgs->attr->enable 	= TRUE;
		if(strms[strmNum]->device->rxCfgs->attr->pStream == NULL) // 20230209 ye.yu added if(),if a shared fifo exists,the device can only bind the shared fifo.
		{
			strms[strmNum]->device->rxCfgs->attr->pStream 	= strms[strmNum];
			strms[strmNum]->device->rxCfgs->format			= strms[strmNum]->format;
		}
		strms[strmNum]->device->load 					= TRUE;
	}
	else
	{
		strms[strmNum] = (auStream_t *)malloc(sizeof(auStream_t));
		if (strms[strmNum] == NULL){
			AU_MNGR_DEBUG("malloc fail!\n");
			return AUDIO_MNGR_FAIL;
		}
		memset(strms[strmNum], 0, sizeof(auStream_t));
		
		// Load stream info
		strms[strmNum]->devId 			= router->playback[strmNum]->playback.devId;
		strms[strmNum]->var.useSwGain	= devs[strms[strmNum]->devId].txCfgs->attr->useSwGain;
		strms[strmNum]->formatCap		= devs[strms[strmNum]->devId].txCfgs->attr->formatCap;	/*Default use information in hw_if.c, this format is output of device*/
		strms[strmNum]->format 			= &router->playback[strmNum]->format;//NULL;
		strms[strmNum]->ops				= devs[strms[strmNum]->devId].txCfgs->attr->ops;
		strms[strmNum]->dmaChannel		= devs[strms[strmNum]->devId].txCfgs->attr->dmaChannel;
		strms[strmNum]->streamRole		= strmRole;
		strms[strmNum]->event			= (auFifoEvent_t *)malloc(sizeof(auFifoEvent_t));
		strms[strmNum]->fifoId			= router->playback[strmNum]->fifoId;
		
		if (strms[strmNum]->event == NULL){
			AU_MNGR_DEBUG("malloc event fail!\n");
			return AUDIO_MNGR_FAIL;
		}
		memset(strms[strmNum]->event, 0, sizeof(auFifoEvent_t));
		strms[strmNum]->event->channelCopy = router->playback[strmNum]->channelCopy;

		// Load device
		strms[strmNum]->device 							= &devs[router->playback[strmNum]->playback.devId];
		strms[strmNum]->device->txCfgs->attr->enable 	= TRUE;
		strms[strmNum]->device->txCfgs->attr->pStream 	= strms[strmNum];
		strms[strmNum]->device->txCfgs->format			= strms[strmNum]->format;
		strms[strmNum]->device->load 					= TRUE;
	}
	
	return AUDIO_MNGR_SUCCESS;
}

uint8_t auManager_load_session(auManagerHandler_t *auMngr, auSessionId_t rnum, auSessionDesc_t *router, audioDevIf_t *devs)
{
	if (rnum >= AUDIO_SESSION_MAX)  return 0;
	auManager_load_session(auMngr, rnum+1, router, devs);
	
	auStream_t **stream = NULL;
	uint8_t strmNum = 0;
	
	auMngr->session[rnum].id = router[rnum].id;
	auMngr->session[rnum].ops = router[rnum].ops;
	auMngr->session[rnum].sessionPolicy = router[rnum].sessionPolicy;

	if (router[rnum].procIf)
	{
		auMngr->session[rnum].proc = (auProcOnStream_t *)malloc(sizeof(auProcOnStream_t));

		if (auMngr->session[rnum].proc == NULL){
			AU_MNGR_DEBUG("malloc fail!\n");
			return 0;
		}
		memset(auMngr->session[rnum].proc, 0, sizeof(auProcOnStream_t));
		auMngr->session[rnum].proc->event = &router[rnum].procIf->procEvent;
		auMngr->session[rnum].proc->format = &router[rnum].procIf->outputFormat;

		auMngr->session[rnum].proc->cbEvents.pickFifo = (auFifo_t **)malloc(sizeof(auFifo_t *)*router[rnum].captureNum);
		memset(auMngr->session[rnum].proc->cbEvents.pickFifo, 0, sizeof(auFifo_t *)*router[rnum].captureNum);
		auMngr->session[rnum].proc->cbEvents.pickAddrList = (int32_t **)malloc(sizeof(uint32_t *)*(router[rnum].captureNum));
		memset(auMngr->session[rnum].proc->cbEvents.pickAddrList, 0, sizeof(uint32_t *)*(router[rnum].captureNum));
		auMngr->session[rnum].proc->cbEvents.pokeFifo = (auFifo_t **)malloc(sizeof(auFifo_t *)*router[rnum].playbackNum);
		memset(auMngr->session[rnum].proc->cbEvents.pokeFifo, 0, sizeof(auFifo_t *)*router[rnum].playbackNum);
		auMngr->session[rnum].proc->cbEvents.pokeAddrList = (int32_t **)malloc(sizeof(uint32_t *)*(router[rnum].playbackNum));
		memset(auMngr->session[rnum].proc->cbEvents.pokeAddrList, 0, sizeof(uint32_t *)*(router[rnum].playbackNum));
	}
	
	if (router[rnum].capture)
	{
		strmNum = router[rnum].captureNum;
		auMngr->session[rnum].captureNum = strmNum;
		auMngr->session[rnum].capture = (auStream_t **)malloc(sizeof(auStream_t*)*strmNum);
		if (auMngr->session[rnum].capture == NULL) {
			AU_MNGR_DEBUG("malloc fail!\n");
			return 0;
		}
		stream = (auStream_t **)auMngr->session[rnum].capture;
		memset(stream, 0, sizeof(auStream_t*)*strmNum);
		load_stream(0, &router[rnum], STREAM_CAPTURE, stream, strmNum, devs);
	}
	
	if (router[rnum].playback)
	{
		strmNum = router[rnum].playbackNum;
		auMngr->session[rnum].playbackNum = strmNum;
		auMngr->session[rnum].playback = (auStream_t **)malloc(sizeof(auStream_t*)*strmNum);
		if (auMngr->session[rnum].playback == NULL) {
			AU_MNGR_DEBUG("malloc fail!\n");
			return 0;
		}
		stream = (auStream_t **)auMngr->session[rnum].playback;
		memset(stream, 0, sizeof(auStream_t*)*strmNum);
		load_stream(0, &router[rnum], STREAM_PLAYBACK, stream, strmNum, devs);
	}
	
	return 1;
}

void audioManager_init(void)
{
	memset(&audioManager, 0, sizeof(auManagerHandler_t));

	auManager_load_session(&audioManager, 0, auSessionDesc, auDeviceIfs);
	auManager_load_fifo(&audioManager, 0);
	auDevive_init();
}

#endif

