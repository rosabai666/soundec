/**
***********************************************************
 *
 * @file name	: audio_fifo_manager.c
 * @author		: ye.yu
 * @version		: Vxx.xx
 * @date		: 2022-01-19
 * @brief		: A fifo manager for audio stream usage
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
#include "types.h"
#include "common.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "snc8600.h"
#include "audio_fifo_manager.h"
#include "app_dmac_test.h"
#include "hal_interrupt.h"
#include "audio_session.h"
#include "audio_manager.h"

#if AUDIO_MANAGER
/* Private typedef -----------------------------------------------------------*/
#define D_SHARE_FIFO_MAX				5

#define FIFO_DEBUG						0
#if(FIFO_DEBUG)
#define AU_FIFO_DEBUG(...)			uart_printf(__VA_ARGS__)
#define AU_FIFO_WARN(...)			uart_printf(__VA_ARGS__)
#define AU_FIFO_ERROR(...)			uart_printf(__VA_ARGS__)
#else
#define AU_FIFO_DEBUG(...)
#define AU_FIFO_WARN(...)			uart_printf(__VA_ARGS__)
#define AU_FIFO_ERROR(...)			uart_printf(__VA_ARGS__)
#endif

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


typedef struct
{
	void*							fifo;		  // Fifo pointer to store data
	auSlotSize_t					slotSize;	  // bit-width #auSlotSize_t
	uint32_t						fifoSize;	  // fifo size in bytes
	uint32_t						frameLength;  // frames length,unit is ms
	uint32_t						frameSize;	  // frame size
	uint32_t						channels;	  // Number of channels
	uint32_t						sampleRate;	  // Current sampling rate for accessing audio data
	uint32_t						sampleRateMax;// The maximum sampling rate for accessing audio data
	uint32_t						thresholdTime;// The threshold length in terms of time,unit is ms
	uint32_t						thresholdLevel;// Must <= fifoSize, used to set a treshould level which indicate FiFo is ready;when fifo size upto threshouldLen, generate a event for CB_functions
	uint32_t						tailPtr;	  // write offset,value range is 0-fifosize.
	uint32_t						headPtr;	  // read offset,value range is 0-fifosize.
	bool							enough;		  // If the readable data length is larger than the threshold, the fifo is enough.
	bool							full;		  // If the writeable data length is 0, the fifo is full.
	bool 							empty;		  // If the readeable data length is 0, the fifo is empty.
	bool							useShareBuf;  // Share memory space with other fifo.
	bool 							channelCopy;  // true:Mono input dual channel output
	uint8_t 						shareFifoNum; // Number of fifos that share the same memory.
	auPcmDataFormat_t				pcmFormat;	  // The format of stored data #auPcmDataFormat_t.
	fifo_handle_t*					shareFifoHandle[D_SHARE_FIFO_MAX]; // Stored the handle to the shared fifo.
	void *							sData;		  // The audio session handle of the fifo.
	auDeviceId_t 					deviceId;	  // Device id of the fifo
	IRQn_Type						irq;		  // Interrupt Number when the amount of written data reaches the threshold,the interruption is triggered.
	bool							isPlaying;	  // Audio devices playback state.
	bool 							isrBusy;
}fifo_elem_t;

void fifo_finish_isr(fifo_handle_t *p_fifo)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;
	p_elem->isrBusy = 0;
}

/**
 * @brief	Get the audio session handle of the fifo.
 * @param[in]	p_fifo      Handle of the data
 * @return	The audio session handle of the fifo.
 */
uint32_t fifo_get_sdata(fifo_handle_t *p_fifo)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;

	return (uint32_t)(p_elem->sData);
}
/**
 * @brief	Get the length of the remaining stored data in the fifo.
 * @param[in]	p_fifo      Handle of the data
 * @return	The length of the remaining stored data in the fifo.
 */
uint32_t fifo_get_free_len(fifo_handle_t *p_fifo)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;

	if(p_elem->headPtr < p_elem->tailPtr)
		return (p_elem->headPtr + p_elem->fifoSize - p_elem->tailPtr);
	if(p_elem->headPtr > p_elem->tailPtr)
		return (p_elem->headPtr - p_elem->tailPtr);
	else if(p_elem->full)
		return 0;
	else
		return p_elem->fifoSize;
}
/**
 * @brief	Get the length of data stored in the fifo.
 * @param[in]	p_fifo      Handle of the data
 * @return	The length of data stored in the fifo.
 */
uint32_t fifo_get_data_len(fifo_handle_t* p_fifo)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;

	if(!p_elem->full)
	{
		if(p_elem->tailPtr > p_elem->headPtr)
			return (p_elem->tailPtr - p_elem->headPtr);
		else if(p_elem->tailPtr == p_elem->headPtr)
			return 0;
		else
			return (p_elem->fifoSize + p_elem->tailPtr - p_elem->headPtr);
	}
	else
		return p_elem->fifoSize;
}
/**
 * @brief	Get fifo size parameter of the fifo.
 * @param[in]	p_fifo      Handle of the data
 * @return	Fifo size.
 */
uint32_t fifo_get_fifo_size(fifo_handle_t* p_fifo)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;

	return p_elem->fifoSize;
}
/**
 * @brief	Get thresholdLevel parameter of the fifo.
 * @param[in]	p_fifo      Handle of the data
 * @return	thresholdLevel.
 */
uint32_t fifo_get_thresholdLevel(fifo_handle_t* p_fifo)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;

	if(p_elem->pcmFormat == PCM_PLANAR_FORMAT)
		return 2*p_elem->thresholdLevel;
	else
		return p_elem->thresholdLevel;
}
/**
 * @brief	Get empty parameter of the fifo.
 * @param[in]	p_fifo      Handle of the data
 * @return	Empty parameter.
 * 	@retval false Fifo is not empty.
 * 	@retval true Fifo is empty
 */
bool fifo_is_empty(fifo_handle_t* p_fifo)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;

	return p_elem->empty;
}
/**
 * @brief	Get full parameter of the fifo.
 * @param[in]	p_fifo      Handle of the data
 * @return	Full parameter.
 * 	@retval false Fifo is not full.
 * 	@retval true Fifo is full
 */
bool fifo_is_full(fifo_handle_t* p_fifo)
{
	return (fifo_get_free_len(p_fifo) == 0);
}
/**
 * @brief	Get enough parameter of the fifo.
 * @param[in]	p_fifo      Handle of the data
 * @return	Enough parameter.
 * 	@retval false Fifo is not enough.
 * 	@retval true Fifo is enough
 */
bool fifo_is_enough(fifo_handle_t* p_fifo)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;

	return p_elem->enough;
}
/**
 * @brief	Get the number of fifos that are shared.
 * @param[in]	p_fifo      Handle of the fifo
 * 	@arg @ref xx {description if necessary}
 * @return	The number of fifos that are shared.
 * 	@retval 0 Not be shared.
 * 	@retval N There is N fifo are shared.
 */
uint8_t fifo_get_share_num(fifo_handle_t* p_fifo)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;

	return p_elem->shareFifoNum;
}
/**
 * @brief	Set audio device playing state into fifo manager.
 * @param[in]	p_fifo      Handle of the fifo.
 * @param[in]	isPlaying   The playback state to set.
 * 	@arg @ref xx {description if necessary}
 * @return	None
 */
void fifo_set_playing_state(fifo_handle_t* p_fifo, bool isPlaying)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;

	p_elem->isPlaying = isPlaying;
//	AU_FIFO_DEBUG("set playing:%d\n", isPlaying);
}
/**
 * @brief	Get audio device playback state.
 * @param[in]	p_fifo      Handle of the fifo.
 * @return	playing state
 * 	@retval true Device is playing.
 * 	@retval false Device has stoped playing.
 */
bool fifo_get_playing_state(fifo_handle_t* p_fifo)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;

	return p_elem->isPlaying;
}
/**
 * @brief	Called by fifo_add_data when pcmFormat is PCM_PLANAR_FORMAT.
 * @note	The patameter pcmFormat is configured in auSessionDesc.
 * @param[in]	p_fifo      Handle of the fifo.
 * @param[in]	data        The pointer to store data.
 * @param[in]	sizeInByte  Length of the data to be write,the unit is the number of bytes.
 * @param[in]	bitWidth    The bit-width of the data to be read.
 * @return	The result of the function execution.
 * 	@retval FIFO_INVALID_PTR The data parameter is null.
 * 	@retval FIFO_NULL Handle of fifo is error.
 * 	@retval FIFO_BITSLOT_ERROR The bit-width format used to read the data is incorrect.
 * 	@retval FIFO_EMPTY There is not enough data.
 * 	@retval FIFO_SUCCESS Write data success.
 */
uint32_t fifo_add_data_rebuild_sort(fifo_handle_t *p_fifo, void *data, uint32_t sizeInByte, auSlotSize_t bitWidth)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;
	uint32_t needFifoSize = sizeInByte*p_elem->slotSize/bitWidth;
    //ae_int32x2 *__restrict pSrc = (ae_int32x2 *)data;
    //ae_int32x2 *__restrict pDst = (ae_int32x2 *)(p_elem->fifo + p_elem->tailPtr);
    //ae_int32x2 temp;
    //ae_valign align;
	size_t sampleCnt;
	int i, j;

	if (data == NULL)										return FIFO_INVALID_PTR;
	if (p_elem->fifo == NULL)								return FIFO_NULL;
	if (bitWidth > BIT_SLOT_32 || bitWidth < BIT_SLOT_8)	return FIFO_BITSLOT_ERROR;

	if(fifo_get_free_len(p_fifo) <= needFifoSize && needFifoSize > 0 && !p_elem->full)
	{
	//	AU_FIFO_WARN("add size:%d free:%d p_elem->tailPtr:%d, %d fullch0=====\n\n", sizeInByte, fifo_get_free_len(p_fifo), p_elem->tailPtr, p_elem->headPtr);
		p_elem->full = true;
	}

	//fifo is full
	if (fifo_get_free_len(p_fifo) < needFifoSize)
	{
		p_elem->headPtr = (p_elem->tailPtr + needFifoSize)%p_elem->fifoSize;
	}

	if(p_elem->useShareBuf)
	{
		p_elem->tailPtr = p_elem->tailPtr + sizeInByte;
		if(p_elem->tailPtr >= p_elem->fifoSize)
			p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;

		if (!p_elem->enough && p_elem->thresholdLevel < fifo_get_data_len(p_fifo))
		{
			p_elem->enough = true;
//			AU_FIFO_DEBUG("useShareBuf enough:%d \n", p_elem->enough);
			if(p_fifo->fifo_ready_cb != NULL)
			{
				//p_fifo->fifo_ready_cb(p_fifo, &p_elem->fifo[p_elem->headPtr], p_elem->thresholdLevel);
				if(p_elem->irq == SW_IRQn)
					hal_interrupt_irq_software_trigger(p_elem->irq);
				else
					hal_interrupt_irq_force_trigger(hal_interrupt_get_intr_src(p_elem->irq));
			}
		}
		return FIFO_SUCCESS;
	}
	else if(p_elem->shareFifoNum > 0)
	{
		for(i = 0; i < p_elem->shareFifoNum; i++)
		{
			if(p_elem->shareFifoHandle[i] != NULL)
			{
				p_elem->shareFifoHandle[i]->fuc->push(p_elem->shareFifoHandle[i], data, sizeInByte, bitWidth);
			}
		}
	}

	sampleCnt = needFifoSize/(p_elem->frameSize);
	if(p_elem->slotSize == BIT_SLOT_32)
	{
		if(bitWidth == BIT_SLOT_8)
		{
			for (i = 0; i < sampleCnt; i++)
			{
				for(j = 0; j < p_elem->channels; j++)
				{
					((uint32_t *)p_elem->fifo)[(p_elem->tailPtr + p_elem->thresholdLevel*j)/BIT_SLOT_32] = ((uint8_t *)data)[i*p_elem->channels + j] << 24;
				}
				p_elem->tailPtr = p_elem->tailPtr + BIT_SLOT_32;
				if(p_elem->tailPtr >= p_elem->fifoSize)
					p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_16)
		{
			for (i = 0; i < sampleCnt; i++)
			{
				for(j = 0; j < p_elem->channels; j++)
				{
					((uint32_t *)p_elem->fifo)[(p_elem->tailPtr + p_elem->thresholdLevel*j)/BIT_SLOT_32] = ((uint16_t *)data)[i*p_elem->channels + j] << 16;
				}
				p_elem->tailPtr = p_elem->tailPtr + BIT_SLOT_32;
				if(p_elem->tailPtr >= p_elem->fifoSize)
					p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_24)
		{
			for (i = 0; i < sampleCnt; i++)
			{
				for(j = 0; j < p_elem->channels; j++)
				{
					((uint8_t *)p_elem->fifo)[p_elem->tailPtr + p_elem->thresholdLevel*j] = 0;
					((uint8_t *)p_elem->fifo)[p_elem->tailPtr + p_elem->thresholdLevel*j + 1] = ((uint8_t *)data)[(i*p_elem->channels + j)*BIT_SLOT_24];
					((uint8_t *)p_elem->fifo)[p_elem->tailPtr + p_elem->thresholdLevel*j + 2] = ((uint8_t *)data)[(i*p_elem->channels + j)*BIT_SLOT_24 + 1];
					((uint8_t *)p_elem->fifo)[p_elem->tailPtr + p_elem->thresholdLevel*j + 3] = ((uint8_t *)data)[(i*p_elem->channels + j)*BIT_SLOT_24 + 2];
				}
				p_elem->tailPtr = p_elem->tailPtr + BIT_SLOT_32;
				if(p_elem->tailPtr >= p_elem->fifoSize)
					p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_32)
		{
			//verified OK
			for (i = 0; i < sampleCnt; i++)
			{
				for(j = 0; j < p_elem->channels; j++)
				{
					((uint32_t *)p_elem->fifo)[(p_elem->tailPtr + p_elem->thresholdLevel*j)/BIT_SLOT_32] = ((uint32_t *)data)[i*p_elem->channels + j];
				}
				p_elem->tailPtr = p_elem->tailPtr + BIT_SLOT_32;
				if(p_elem->tailPtr >= p_elem->fifoSize)
					p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;
			}
		}
	}
	else if(p_elem->slotSize == BIT_SLOT_16)
	{
		if(bitWidth == BIT_SLOT_8)
		{
			for (i = 0; i < sampleCnt; i++)
			{
				for(j = 0; j < p_elem->channels; j++)
				{
					((uint16_t *)p_elem->fifo)[(p_elem->tailPtr + p_elem->thresholdLevel*j)/BIT_SLOT_16] = ((uint8_t *)data)[i*p_elem->channels + j] << 8;
				}
				p_elem->tailPtr = p_elem->tailPtr + BIT_SLOT_16;
				if(p_elem->tailPtr >= p_elem->fifoSize)
					p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_16)
		{
			for (i = 0; i < sampleCnt; i++)
			{
				for(j = 0; j < p_elem->channels; j++)
				{
					((uint16_t *)p_elem->fifo)[(p_elem->tailPtr + p_elem->thresholdLevel*j)/BIT_SLOT_16] = ((uint16_t *)data)[i*p_elem->channels + j];
				}
				p_elem->tailPtr = p_elem->tailPtr + BIT_SLOT_16;
				if(p_elem->tailPtr >= p_elem->fifoSize)
					p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_24)
		{
			for (i = 0; i < sampleCnt; i++)
			{
				for(j = 0; j < p_elem->channels; j++)
				{
					((uint16_t *)p_elem->fifo)[(p_elem->tailPtr + p_elem->thresholdLevel*j)/BIT_SLOT_16] = ((uint8_t *)data)[(i*p_elem->channels + j)*BIT_SLOT_24 + 1]
																				+ (((uint8_t *)data)[(i*p_elem->channels + j)*BIT_SLOT_24 + 2] << 8);
				}
				p_elem->tailPtr = p_elem->tailPtr + BIT_SLOT_16;
				if(p_elem->tailPtr >= p_elem->fifoSize)
					p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_32)
		{
			for (i = 0; i < sampleCnt; i++)
			{
				for(j = 0; j < p_elem->channels; j++)
				{
					((uint16_t *)p_elem->fifo)[(p_elem->tailPtr + p_elem->thresholdLevel*j)/BIT_SLOT_16] = (int16_t)(((int32_t *)data)[i*p_elem->channels + j] >> 16);
				}
				p_elem->tailPtr = p_elem->tailPtr + BIT_SLOT_16;
				if(p_elem->tailPtr >= p_elem->fifoSize)
					p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;
			}
		}
	}

	if (!p_elem->enough && (p_elem->tailPtr%p_elem->thresholdLevel < needFifoSize/2) && fifo_get_data_len(p_fifo) >= p_elem->thresholdLevel)
	{
		p_elem->enough = true;
		p_elem->tailPtr += p_elem->thresholdLevel*(p_elem->channels - 1);
		if(p_elem->tailPtr >= p_elem->fifoSize)
			p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;
		if(p_fifo->fifo_ready_cb != NULL)
		{
			//p_fifo->fifo_ready_cb(p_fifo, &p_elem->fifo[p_elem->headPtr], p_elem->thresholdLevel);
			//AU_FIFO_DEBUG("trigger irq:%d.\n", p_elem->irq);
			if(p_elem->irq == SW_IRQn)
				hal_interrupt_irq_software_trigger(p_elem->irq);
			else
				hal_interrupt_irq_force_trigger(hal_interrupt_get_intr_src(p_elem->irq));
		}
	}
	else if(p_elem->thresholdLevel > 0 && (p_elem->tailPtr%p_elem->thresholdLevel < needFifoSize/2) && fifo_get_data_len(p_fifo) >= p_elem->thresholdLevel)
	{
		p_elem->tailPtr += p_elem->thresholdLevel*(p_elem->channels - 1);
		if(p_elem->tailPtr >= p_elem->fifoSize)
			p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;
		if(p_fifo->fifo_ready_cb != NULL)
		{
			//p_fifo->fifo_ready_cb(p_fifo, &p_elem->fifo[p_elem->headPtr], p_elem->thresholdLevel);
			//AU_FIFO_DEBUG("trigger irq:%d.\n", p_elem->irq);
			if(p_elem->irq == SW_IRQn)
				hal_interrupt_irq_software_trigger(p_elem->irq);
			else
				hal_interrupt_irq_force_trigger(hal_interrupt_get_intr_src(p_elem->irq));
		}
	}

	return FIFO_SUCCESS;
}
/**
 * @brief	Used to write data into the fifo.
 * @note	{description}
 * @param[in]	p_fifo      Handle of the fifo.
 * @param[in]	data        The pointer to store data.
 * @param[in]	sizeInByte  Length of the data to be write,the unit is the number of bytes.
 * @param[in]	bitWidth    The bit-width of the data to be read.
 * @return	The result of the function execution.
 * 	@retval FIFO_INVALID_PTR The data parameter is null.
 * 	@retval FIFO_NULL Handle of fifo is error.
 * 	@retval FIFO_BITSLOT_ERROR The bit-width format used to read the data is incorrect.
 * 	@retval FIFO_EMPTY There is not enough data.
 * 	@retval FIFO_SUCCESS Write data success.
 */
uint32_t fifo_add_data(fifo_handle_t *p_fifo, uint8_t *__restrict data, uint32_t sizeInByte, auSlotSize_t bitWidth)
{
//	AU_FIFO_DEBUG("%d,", sizeInByte);
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;
	if(p_elem->pcmFormat == PCM_PLANAR_FORMAT)
	{
		return fifo_add_data_rebuild_sort(p_fifo, data, sizeInByte, bitWidth);
	}
	uint32_t needFifoSize = 0;//sizeInByte*p_elem->slotSize/bitWidth;
    ae_p16x2s * __restrict 	pSrc16 = (ae_p16x2s *)data;
    ae_f24x2 * __restrict 	pSrc24 = (ae_f24x2 *)data;
    ae_int32x2 * __restrict pSrc32 = (ae_int32x2 *)data;
    ae_f24x2 * __restrict 	pDst24 = (ae_f24x2 *)(p_elem->fifo + p_elem->tailPtr);
    ae_int32x2 * __restrict pDst32 = (ae_int32x2 *)(p_elem->fifo + p_elem->tailPtr);

    ae_f24x2 	temp24;
    ae_int32x2 	temp32;
    ae_valign 	align;
    ae_valign 	align32;

	int i;//, j;
	size_t elems, sample;
	static uint32_t old=0;

	if (data == NULL)										return FIFO_INVALID_PTR;
	if (p_elem->fifo == NULL)								return FIFO_NULL;
	if (bitWidth > BIT_SLOT_32 || bitWidth < BIT_SLOT_8)	return FIFO_BITSLOT_ERROR;

	elems = sizeInByte/bitWidth;
    /* Ensure the number of the elements is even number */
    elems -= elems & 1;
	needFifoSize = elems * p_elem->slotSize;
	if(fifo_get_free_len(p_fifo) <= needFifoSize)
	{
		if(needFifoSize > 0 && !p_elem->full)
		{
		//	AU_FIFO_DEBUG("add size:%d free:%d p_elem->tailPtr:%d, %d fullch0=====\n\n", sizeInByte, fifo_get_free_len(p_fifo), p_elem->tailPtr, p_elem->headPtr);
			//if (old != sizeInByte)
			//	AU_FIFO_WARN("add %d!\n", sizeInByte);
			old = sizeInByte;
			p_elem->full = true;
		}
		else
		{
		//	p_elem->headPtr = (p_elem->tailPtr + needFifoSize)%p_elem->fifoSize;
			p_elem->headPtr = 0;
			p_elem->tailPtr = 0;
			p_elem->full = false;
			p_elem->enough = false;
		}
	}

	if(p_elem->useShareBuf)
	{
		p_elem->tailPtr = p_elem->tailPtr + needFifoSize;
		if(p_elem->tailPtr >= p_elem->fifoSize)
			p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;

		if (!p_elem->enough && p_elem->thresholdLevel < fifo_get_data_len(p_fifo))
		{
			p_elem->enough = true;
//			AU_FIFO_DEBUG("useShareBuf enough:%d \n", p_elem->enough);
			if(p_fifo->fifo_ready_cb != NULL)
			{
				//p_fifo->fifo_ready_cb(p_fifo, &p_elem->fifo[p_elem->headPtr], p_elem->thresholdLevel);
				//AU_FIFO_DEBUG("trigger irq:%d.\n", p_elem->irq);
				if(p_elem->irq == SW_IRQn)
					hal_interrupt_irq_software_trigger(p_elem->irq);
				else
					hal_interrupt_irq_force_trigger(hal_interrupt_get_intr_src(p_elem->irq));
			}
		}
		return FIFO_SUCCESS;
	}
	else if(p_elem->shareFifoNum > 0)
	{
		for(i = 0; i < p_elem->shareFifoNum; i++)
		{
			if(p_elem->shareFifoHandle[i] != NULL)
			{
				p_elem->shareFifoHandle[i]->fuc->push(p_elem->shareFifoHandle[i], data, sizeInByte, bitWidth);
			}
		}
	}

	align = AE_LA64_PP(pSrc24);
    align32 = AE_LA64_PP(pSrc32);
    /* Initialize the circular buffer boundaries */
    AE_SETCBEGIN0((uint8_t*)p_elem->fifo);
    AE_SETCEND0((uint8_t*)p_elem->fifo + p_elem->fifoSize);

	if (p_elem->slotSize == BIT_SLOT_32)
	{
		sample = elems >> 3;
		if(BIT_SLOT_24 == bitWidth)
		{
			for (i=0; i<sample; i++)
			{
				AE_LA24X2_IP(temp24, align, pSrc24);
				AE_S32X2F24_XC(temp24, pDst24, 8);

				AE_LA24X2_IP(temp24, align, pSrc24);
				AE_S32X2F24_XC(temp24, pDst24, 8);

				AE_LA24X2_IP(temp24, align, pSrc24);
				AE_S32X2F24_XC(temp24, pDst24, 8);

				AE_LA24X2_IP(temp24, align, pSrc24);
				AE_S32X2F24_XC(temp24, pDst24, 8);
			}
			//sample = (elems % 8)>>1;
			sample = (elems >> 1) - ((elems >> 3) << 2);
			for (i = 0; i<sample; i++)
			{
				AE_LA24X2_IP(temp24, align, pSrc24);
				AE_S32X2F24_XC(temp24, pDst24, 8);
			}
		//	p_elem->tailPtr = (p_elem->tailPtr + needFifoSize) % p_elem->fifoSize;
			p_elem->tailPtr = (p_elem->tailPtr + needFifoSize);
			if(p_elem->tailPtr >= p_elem->fifoSize)
				p_elem->tailPtr -= p_elem->fifoSize;
		}
		else if(BIT_SLOT_32 == bitWidth)
		{
			align = AE_ZALIGN64();	//zero alignment reg
			for (i=0; i<sample; i++)
			{
				AE_LA32X2_IP(temp32, align32, pSrc32);
				AE_S32X2_XC(temp32, pDst32, 8);

				AE_LA32X2_IP(temp32, align32, pSrc32);
				AE_S32X2_XC(temp32, pDst32, 8);

				AE_LA32X2_IP(temp32, align32, pSrc32);
				AE_S32X2_XC(temp32, pDst32, 8);

				AE_LA32X2_IP(temp32, align32, pSrc32);
				AE_S32X2_XC(temp32, pDst32, 8);
			}
//			sample = (elems % 8)>>1;
			sample = (elems >> 1) - ((elems >> 3) << 2);
			for (i=0; i<sample; i++)
			{
				AE_LA32X2_IP(temp32, align32, pSrc32);
				AE_S32X2_XC(temp32, pDst32, 8);
			}
		//	p_elem->tailPtr = (p_elem->tailPtr + needFifoSize) % p_elem->fifoSize;
			p_elem->tailPtr = (p_elem->tailPtr + needFifoSize);
			if(p_elem->tailPtr >= p_elem->fifoSize)
				p_elem->tailPtr -= p_elem->fifoSize;
		}
  		else if(BIT_SLOT_8 == bitWidth)
		{
			for (i = 0; i < elems; i++)
			{
				((uint32_t *)p_elem->fifo)[p_elem->tailPtr/BIT_SLOT_32] = ((uint8_t *)data)[i] << 24;
				p_elem->tailPtr = (p_elem->tailPtr + BIT_SLOT_32)%p_elem->fifoSize;
			}
		}
		else if(BIT_SLOT_16 == bitWidth)
		{
			pSrc16--;
	        for(i = 0; i < sample; i++)
	        {
	            AE_LP16X2F_IU(temp24, pSrc16, 4);
	            AE_S32X2F24_XC(temp24, pDst24, 8);

	            AE_LP16X2F_IU(temp24, pSrc16, 4);
	            AE_S32X2F24_XC(temp24, pDst24, 8);

				AE_LP16X2F_IU(temp24, pSrc16, 4);
				AE_S32X2F24_XC(temp24, pDst24, 8);

				AE_LP16X2F_IU(temp24, pSrc16, 4);
				AE_S32X2F24_XC(temp24, pDst24, 8);
			}
			sample = (elems % 8)>>1;
			for (i = 0; i < sample; i++)
			{
				AE_LP16X2F_IU(temp24, pSrc16, 4);
				AE_S32X2F24_XC(temp24, pDst24, 8);
			}
			p_elem->tailPtr = (p_elem->tailPtr + needFifoSize)%p_elem->fifoSize;
		}
	}
	else if (p_elem->slotSize == BIT_SLOT_16)
	{
		if(bitWidth == BIT_SLOT_16)
		{
			for (i = 0; i < elems; i++)
			{
				((uint16_t *)p_elem->fifo)[p_elem->tailPtr/BIT_SLOT_16] = ((uint16_t *)data)[i];
				p_elem->tailPtr = p_elem->tailPtr + BIT_SLOT_16;
				if(p_elem->tailPtr >= p_elem->fifoSize)
					p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_24)
		{
			for (i = 0; i < elems; i++)
			{
				((uint16_t *)p_elem->fifo)[p_elem->tailPtr/BIT_SLOT_16] = ((uint8_t *)data)[i*3 + 1] + (((uint8_t *)data)[i*3 + 2] << 8);
				p_elem->tailPtr = p_elem->tailPtr + BIT_SLOT_16;
				if(p_elem->tailPtr >= p_elem->fifoSize)
					p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_32)
		{
			for (i = 0; i < elems; i++)
			{
				((uint16_t *)p_elem->fifo)[p_elem->tailPtr/BIT_SLOT_16] = ((uint32_t *)data)[i] >> 16;
				p_elem->tailPtr = p_elem->tailPtr + BIT_SLOT_16;
				if(p_elem->tailPtr >= p_elem->fifoSize)
					p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_24)
		{
			for (i = 0; i < elems; i++)
			{
				((uint16_t *)p_elem->fifo)[p_elem->tailPtr/BIT_SLOT_16] = ((uint8_t *)data)[i]<<8;
				p_elem->tailPtr = p_elem->tailPtr + BIT_SLOT_16;
				if(p_elem->tailPtr >= p_elem->fifoSize)
					p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize;
			}
		}
	}

	if (!p_elem->enough && p_elem->thresholdLevel < fifo_get_data_len(p_fifo))
	{
		p_elem->enough = true;
		if(p_fifo->fifo_ready_cb != NULL)
		{
			if(p_elem->irq == SW_IRQn)
				hal_interrupt_irq_software_trigger(p_elem->irq);
			else
				hal_interrupt_irq_force_trigger(hal_interrupt_get_intr_src(p_elem->irq));
		}
	}
#if(SUPPORT_RESAMPLER)
	else if(!p_elem->isrBusy && fifo_get_data_len(p_fifo)>=p_elem->thresholdLevel)
	{
		if(p_fifo->fifo_ready_cb)
		{
			hal_interrupt_irq_software_trigger(p_elem->irq);
			p_elem->isrBusy = 1;
		}
	}
#else
	else if(p_elem->thresholdLevel > 0 && (p_elem->tailPtr%p_elem->thresholdLevel < needFifoSize) && fifo_get_data_len(p_fifo) >= p_elem->thresholdLevel)
	{
		if(p_fifo->fifo_ready_cb != NULL)
		{
			if(p_elem->irq == SW_IRQn)
				hal_interrupt_irq_software_trigger(p_elem->irq);
			else
				hal_interrupt_irq_force_trigger(hal_interrupt_get_intr_src(p_elem->irq));
		}
	}
#endif


	return FIFO_SUCCESS;
}
/**
 * @brief	Called by fifo_get_data when channelCopy is true.
 * @note	The patameter channelCopy is configured in auSessionDesc.Audio data is copied from p_elem->fifo to "data"(@param[out]).
 * @param[in]	p_fifo      Handle of the fifo.
 * @param[out]	data        The pointer to store data.
 * @param[in]	sizeInByte  Length of the data to be read,the unit is the number of bytes.
 * @param[in]	bitWidth    The bit-width of the data to be read.
 * @return	The result of the function execution.
 * 	@retval FIFO_INVALID_PTR The data parameter is null.
 * 	@retval FIFO_NULL Handle of fifo is error.
 * 	@retval FIFO_BITSLOT_ERROR The bit-width format used to read the data is incorrect.
 * 	@retval FIFO_EMPTY There is not enough data.
 * 	@retval FIFO_SUCCESS Read such length of data success.
 */
uint32_t fifo_get_data_change_channel(fifo_handle_t *p_fifo, void* __restrict data, uint32_t *sizeInByte, auSlotSize_t bitWidth)
{
	fifo_elem_t *p_elem 	= (fifo_elem_t*)p_fifo->elem;
	uint32_t needFifoSize 	= *sizeInByte*p_elem->slotSize/(bitWidth*2);
	uint32_t dataLen 		= fifo_get_data_len(p_fifo);
	uint32_t	*headPtr;
	bool		*enough;
	bool		*full;
	size_t i;

	if (data == NULL)										return FIFO_INVALID_PTR;
	if (p_elem->fifo == NULL)								return FIFO_NULL;
	if (bitWidth > BIT_SLOT_32 || bitWidth < BIT_SLOT_8)	return FIFO_BITSLOT_ERROR;

	headPtr = &p_elem->headPtr;
	enough = &p_elem->enough;
	full = &p_elem->full;
	if (!*enough || dataLen < 2 )
	{
		//data[0] = data[1] = 0;
		*sizeInByte = 0;
		return FIFO_EMPTY;
	}

	if(dataLen < needFifoSize)
	{
		*sizeInByte = dataLen*bitWidth*2/p_elem->slotSize;
	}

	size_t cnt = *sizeInByte/(bitWidth*2);
	if(BIT_SLOT_32 == p_elem->slotSize)
	{
		if(bitWidth == BIT_SLOT_8)
		{
			for (i = 0; i < cnt; i++)
			{
				((uint8_t *)data)[i*2] = ((uint32_t *)p_elem->fifo)[*headPtr/BIT_SLOT_32] >> 24;
				((uint8_t *)data)[i*2 + 1] = ((uint32_t *)p_elem->fifo)[*headPtr/BIT_SLOT_32] >> 24;
				*headPtr = *headPtr + BIT_SLOT_32;
				if(*headPtr >= p_elem->fifoSize)
					*headPtr = *headPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_16)
		{
			for (i = 0; i < cnt; i++)
			{
				((uint16_t *)data)[i*2] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16 + 1];
				((uint16_t *)data)[i*2 + 1] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16 + 1];
				*headPtr = *headPtr + BIT_SLOT_32;
				if(*headPtr >= p_elem->fifoSize)
					*headPtr = *headPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_24)
		{
			for (i = 0; i < cnt; i++)
			{
				((uint8_t *)data)[i*BIT_SLOT_24*2] = ((uint8_t *)p_elem->fifo)[*headPtr + 1];
				((uint8_t *)data)[i*BIT_SLOT_24*2 + 1] = ((uint8_t *)p_elem->fifo)[*headPtr + 2];
				((uint8_t *)data)[i*BIT_SLOT_24*2 + 2] = ((uint8_t *)p_elem->fifo)[*headPtr + 3];
				((uint8_t *)data)[i*BIT_SLOT_24*2 + BIT_SLOT_24] = ((uint8_t *)p_elem->fifo)[*headPtr + 1];
				((uint8_t *)data)[i*BIT_SLOT_24*2 + BIT_SLOT_24 + 1] = ((uint8_t *)p_elem->fifo)[*headPtr + 2];
				((uint8_t *)data)[i*BIT_SLOT_24*2 + BIT_SLOT_24 + 2] = ((uint8_t *)p_elem->fifo)[*headPtr + 3];
				*headPtr = *headPtr + BIT_SLOT_32;
				if(*headPtr >= p_elem->fifoSize)
					*headPtr = *headPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_32)
		{
			for (i = 0; i < cnt; i++)
			{
				((uint32_t *)data)[i*2] = ((uint32_t *)p_elem->fifo)[*headPtr/BIT_SLOT_32];
				((uint32_t *)data)[i*2 + 1] = ((uint32_t *)p_elem->fifo)[*headPtr/BIT_SLOT_32];
				*headPtr = *headPtr + BIT_SLOT_32;
				if(*headPtr >= p_elem->fifoSize)
					*headPtr = *headPtr - p_elem->fifoSize;
			}
		}
	}
	else if(BIT_SLOT_16 == p_elem->slotSize)
	{
		if(bitWidth == BIT_SLOT_8)
		{
			for (i = 0; i < cnt; i++)
			{
				((uint8_t *)data)[i*2] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16] >> 8;
				((uint8_t *)data)[i*2 + 1] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16] >> 8;
				*headPtr = *headPtr + BIT_SLOT_16;
				if(*headPtr >= p_elem->fifoSize)
					*headPtr = *headPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_16)
		{
			for (i = 0; i < cnt; i++)
			{
				((uint16_t *)data)[i*2] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16];
				((uint16_t *)data)[i*2 + 1] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16];
				*headPtr = *headPtr + BIT_SLOT_16;
				if(*headPtr >= p_elem->fifoSize)
					*headPtr = *headPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_24)
		{
			for (i = 0; i < cnt; i++)
			{
				((uint8_t *)data)[i*BIT_SLOT_24*2] = 0;
				((uint8_t *)data)[i*BIT_SLOT_24*2 + 1] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16] & 0xff;
				((uint8_t *)data)[i*BIT_SLOT_24*2 + 2] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16] >> 8;
				((uint8_t *)data)[i*BIT_SLOT_24*2 + BIT_SLOT_24] = 0;
				((uint8_t *)data)[i*BIT_SLOT_24*2 + BIT_SLOT_24 + 1] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16] & 0xff;
				((uint8_t *)data)[i*BIT_SLOT_24*2 + BIT_SLOT_24 + 2] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16] >> 8;
				*headPtr = *headPtr + BIT_SLOT_16;
				if(*headPtr >= p_elem->fifoSize)
					*headPtr = *headPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_32)
		{
			for (i = 0; i < cnt; i++)
			{
				((uint32_t *)data)[i*2] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16] << 16;
				((uint32_t *)data)[i*2 + 1] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16] << 16;
				*headPtr = *headPtr + BIT_SLOT_16;
				if(*headPtr >= p_elem->fifoSize)
					*headPtr = *headPtr - p_elem->fifoSize;
			}
		}
	}

	if(*full && *sizeInByte > 0)
		*full = false;
//	AU_FIFO_DEBUG("pop %d headptr:%d\n", *size, p_elem->headPtr);

	return FIFO_SUCCESS;
}
/**
 * @brief	Read data from the fifo.
 * @note	Audio data is copied from p_elem->fifo to "data"(@param[out]).
 * @param[in]	p_fifo      Handle of the fifo.
 * @param[out]	data        The pointer to store data.
 * @param[in]	sizeInByte  Length of the data to be read,the unit is the number of bytes.
 * @param[in]	bitWidth    The bit-width of the data to be read.
 * @return	The result of the function execution.
 * 	@retval FIFO_INVALID_PTR The data parameter is null.
 * 	@retval FIFO_NULL Handle of fifo is error.
 * 	@retval FIFO_BITSLOT_ERROR The bit-width format used to read the data is incorrect.
 * 	@retval FIFO_EMPTY There is not enough data.
 * 	@retval FIFO_SUCCESS Read such length of data success.
 */
uint32_t fifo_get_data(fifo_handle_t *p_fifo, uint8_t *__restrict data, uint32_t *sizeInByte, auSlotSize_t bitWidth)
{
	fifo_elem_t *p_elem 	= (fifo_elem_t*)p_fifo->elem;
	uint32_t 	needFifoSize 	= *sizeInByte*p_elem->slotSize/bitWidth;
	uint32_t 	dataLen 		= fifo_get_data_len(p_fifo);
	uint32_t	*headPtr;
	bool		*enough;
	bool		*full;
	bool 		*empty;
	size_t 		i;

	if(p_elem->channelCopy)
	{
		return fifo_get_data_change_channel(p_fifo, data, sizeInByte, bitWidth);
	}
	if (data == NULL)										return FIFO_INVALID_PTR;
	if (p_elem->fifo == NULL)								return FIFO_NULL;
	if (bitWidth > BIT_SLOT_32 || bitWidth < BIT_SLOT_8)	return FIFO_BITSLOT_ERROR;

	headPtr	= &p_elem->headPtr;
	enough 	= &p_elem->enough;
	full 	= &p_elem->full;
	empty 	= &p_elem->empty;
	if (!*enough)
	{
		return FIFO_EMPTY;
	}

    ae_f24x2 temp24;
    ae_p16x2s *__restrict pDst16 = (ae_p16x2s *)data;
    ae_f24x2  *__restrict pDst24 = (ae_f24x2 *)data;
    ae_f24x2 *__restrict pSrc24 = (ae_f24x2 *)(p_elem->fifo + *headPtr);
	ae_f32x2  temp32;
	ae_f32 *__restrict pSrc32 = (ae_f32 *)(p_elem->fifo + *headPtr);
	ae_f32 *__restrict pDst32 = (ae_f32 *)data;
    ae_valign align;
	size_t sample = 0;

	size_t elems;

	if(dataLen < needFifoSize)
	{
//		elems = dataLen*bitWidth/p_elem->slotSize;
		p_elem->enough = 0;
		p_elem->empty = 1;
		p_elem->headPtr = 0;
		p_elem->tailPtr = 0;
		return FIFO_EMPTY;
	}
	else
	{
		elems = *sizeInByte/bitWidth;
		p_elem->empty = 0;
	}


//	size_t elems = *sizeInByte/bitWidth;
    /* Ensure the number of the elements is even number */
    elems -= elems & 1;

    align = AE_ZALIGN64();	//zero alignment reg
	/* Initialize the circular buffer boundaries */
	AE_SETCBEGIN0(p_elem->fifo);
	AE_SETCEND0(p_elem->fifo + p_elem->fifoSize);
	if(p_elem->slotSize == BIT_SLOT_32)
	{
		if(BIT_SLOT_24 == bitWidth)
		{
			sample = elems >> 3;
			for(i = 0 ; i < sample ; i++)
	        {
	            AE_L32X2F24_XC(temp24, pSrc24, 8);
	            AE_SA24X2_IP(temp24, align, pDst24);

	            AE_L32X2F24_XC(temp24, pSrc24, 8);
	            AE_SA24X2_IP(temp24, align, pDst24);

	            AE_L32X2F24_XC(temp24, pSrc24, 8);
	            AE_SA24X2_IP(temp24, align, pDst24);

	            AE_L32X2F24_XC(temp24, pSrc24, 8);
	            AE_SA24X2_IP(temp24, align, pDst24);
	        }
			//sample = (elems % 8)>>1;
			sample = (elems >> 1) - ((elems >> 3) << 2);
			for (i = 0; i < sample; i++)
			{
				AE_L32X2F24_XC(temp24, pSrc24, 8);
				AE_SA24X2_IP(temp24, align, pDst24);
			}
			AE_SA64POS_FP(align, pDst24); //finalize the stream
		//	*headPtr = (*headPtr + elems*p_elem->slotSize)%p_elem->fifoSize;
			*headPtr = (*headPtr + elems*p_elem->slotSize);
			if(*headPtr >= p_elem->fifoSize)
				*headPtr -= p_elem->fifoSize;
		}
		else if(BIT_SLOT_32 == bitWidth)
		{
			sample = elems >> 2;
			for(i = 0 ; i < sample ; i++)
			{
				AE_L32_XC(temp32, pSrc32, 4);
				AE_S32_L_IP(temp32, pDst32, 4);

				AE_L32_XC(temp32, pSrc32, 4);
				AE_S32_L_IP(temp32, pDst32, 4);

				AE_L32_XC(temp32, pSrc32, 4);
				AE_S32_L_IP(temp32, pDst32, 4);

				AE_L32_XC(temp32, pSrc32, 4);
				AE_S32_L_IP(temp32, pDst32, 4);
			}

		//	sample = elems % 4;
			sample = elems - ((elems >> 2) << 2);
			for (i = 0; i < sample; i++)
			{
				AE_L32_XC(temp32, pSrc32, 4);
				AE_S32_L_IP(temp32, pDst32, 4);
			}
			AE_SA64POS_FP(align, pDst24); //finalize the stream
		//	*headPtr = (*headPtr + elems*p_elem->slotSize)%p_elem->fifoSize;
			*headPtr = (*headPtr + elems*p_elem->slotSize);
			if(*headPtr >= p_elem->fifoSize)
				*headPtr -= p_elem->fifoSize;
		}
		else if(BIT_SLOT_8 == bitWidth)
		{
			for (i = 0; i < elems; i++)
			{
				((uint8_t *)data)[i] = ((uint32_t *)p_elem->fifo)[*headPtr/p_elem->slotSize] >> 24;
				*headPtr = (*headPtr + p_elem->slotSize)%p_elem->fifoSize;
			}
		}
		else if(BIT_SLOT_16 == bitWidth)
		{
		    pDst16--;
			sample = elems >> 3;
			for(i = 0; i<sample; i++)
			{
				AE_L32X2F24_XC(temp24, pSrc24, 8);
				AE_SP16X2F_IU(temp24, pDst16, 4);

				AE_L32X2F24_XC(temp24, pSrc24, 8);
				AE_SP16X2F_IU(temp24, pDst16, 4);

				AE_L32X2F24_XC(temp24, pSrc24, 8);
				AE_SP16X2F_IU(temp24, pDst16, 4);

				AE_L32X2F24_XC(temp24, pSrc24, 8);
				AE_SP16X2F_IU(temp24, pDst16, 4);
			}
			sample = (elems%8)>>1;
		    for (i = 0; i < sample; i++)
		    {
		        AE_L32X2F24_XC(temp24, pSrc24, 8);
		        AE_SP16X2F_IU(temp24, pDst16, 4);
		    }
			AE_SA64POS_FP(align, pDst24); //finalize the stream
			*headPtr = (*headPtr + elems*p_elem->slotSize)%p_elem->fifoSize;
		}

	}
	else if(p_elem->slotSize == BIT_SLOT_16)
	{
		if(bitWidth == BIT_SLOT_8)
		{
			for (i = 0; i < elems; i++)
			{
				((uint8_t *)data)[i] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16] >> 8;
				*headPtr = *headPtr + BIT_SLOT_16;
				if(*headPtr >= p_elem->fifoSize)
					*headPtr = *headPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_16)
		{
			for (i = 0; i < elems; i++)
			{
				((uint16_t *)data)[i] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16];
				*headPtr = *headPtr + BIT_SLOT_16;
				if(*headPtr >= p_elem->fifoSize)
					*headPtr = *headPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_24)
		{
			for (i = 0; i < elems; i++)
			{
				((uint8_t *)data)[i*BIT_SLOT_24] = 0;
				((uint8_t *)data)[i*BIT_SLOT_24 + 1] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16] & 0xff;
				((uint8_t *)data)[i*BIT_SLOT_24 + 2] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16] >> 8;
				*headPtr = *headPtr + BIT_SLOT_16;
				if(*headPtr >= p_elem->fifoSize)
					*headPtr = *headPtr - p_elem->fifoSize;
			}
		}
		else if(bitWidth == BIT_SLOT_32)
		{
			for (i = 0; i < elems; i++)
			{
				((uint32_t *)data)[i] = ((uint16_t *)p_elem->fifo)[*headPtr/BIT_SLOT_16] << 16;
				*headPtr = *headPtr + BIT_SLOT_16;
				if(*headPtr >= p_elem->fifoSize)
					*headPtr = *headPtr - p_elem->fifoSize;
			}
		}
	}

	if(*full && *sizeInByte > 0)
		*full = false;

	return FIFO_SUCCESS;
}
/**
 * @brief	This function is used to synchronize the fifo state when data is write from the fifo elsewhere.
 * @note	{description}
 * @param[in]	p_fifo      Handle of fifo.
 * @param[in]	len         Length of data to be read out.
 * @return	The result of the function execution.
 * 	@retval FIFO_NULL Handle of fifo is error.
 * 	@retval FIFO_BITSLOT_ERROR The bit-width format used to write the data is incorrect.
 * 	@retval FIFO_FULL The fifo is full and fifo will be clear.
 * 	@retval FIFO_SUCCESS Write data success.
 */
uint32_t fifo_push_skip(fifo_handle_t *p_fifo, uint32_t len)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;
	uint32_t needFifoSize = len;
	int i;

	if (p_elem->fifo == NULL)								return FIFO_NULL;

	if(fifo_get_free_len(p_fifo) < needFifoSize && needFifoSize > 0 && !p_elem->full)
	{
	//	AU_FIFO_WARN("push_skip:%d free:%d tailPtr:%d, %d===\n", len, fifo_get_free_len(p_fifo), p_elem->tailPtr, p_elem->headPtr);
		p_elem->full = false;
		p_elem->headPtr = 0;
		p_elem->tailPtr = 0;
		return FIFO_FULL;
	}
	//fifo is full
	if (fifo_get_free_len(p_fifo) < needFifoSize)
	{
		p_elem->headPtr = (p_elem->tailPtr + needFifoSize)%p_elem->fifoSize;
	}

	p_elem->tailPtr = p_elem->tailPtr + needFifoSize;
	if(p_elem->tailPtr >= p_elem->fifoSize)
		p_elem->tailPtr = p_elem->tailPtr - p_elem->fifoSize ;

	if (!p_elem->enough && p_elem->thresholdLevel <= fifo_get_data_len(p_fifo))
	{
		p_elem->enough = true;
		if(p_fifo->fifo_ready_cb != NULL)
		{
			//p_fifo->fifo_ready_cb(p_fifo, &p_elem->fifo[p_elem->headPtr], p_elem->thresholdLevel);
			//AU_FIFO_DEBUG("trigger irq:%d.\n", p_elem->irq);
			if(p_elem->irq == SW_IRQn)
				hal_interrupt_irq_software_trigger(p_elem->irq);
			else
				hal_interrupt_irq_force_trigger(hal_interrupt_get_intr_src(p_elem->irq));
		}
	}
	else if(p_elem->thresholdLevel > 0 && (p_elem->tailPtr%p_elem->thresholdLevel < needFifoSize) && fifo_get_data_len(p_fifo) >= p_elem->thresholdLevel)
	{
		if(p_fifo->fifo_ready_cb != NULL)
		{
				if(p_elem->irq == SW_IRQn)
					hal_interrupt_irq_software_trigger(p_elem->irq);
				else
					hal_interrupt_irq_force_trigger(hal_interrupt_get_intr_src(p_elem->irq));
		}
	}

	if(p_elem->shareFifoNum > 0)
	{
		for(i = 0; i < p_elem->shareFifoNum; i++)
		{
			if(p_elem->shareFifoHandle[i] != NULL)
			{
				p_elem->shareFifoHandle[i]->fuc->push_skip(p_elem->shareFifoHandle[i], len);
			}
		}
	}
	return FIFO_SUCCESS;
}

/**
 * @brief	This function is used to synchronize the fifo state when data is read from the fifo elsewhere.
 * @note	Should be called before reading the data
 * @param[in]	p_fifo      Handle of fifo
 * @param[in/out]	len         Length of data to be read out,if there is not enough data,the length will be set to 0.
 * @return	The result of the function execution.
 * 	@retval FIFO_NULL Handle of fifo is error.
 * 	@retval FIFO_BITSLOT_ERROR The bit-width format used to read the data is incorrect.
 * 	@retval FIFO_EMPTY There is not enough data.
 * 	@retval FIFO_SUCCESS Read such length of data success.
 */
uint32_t fifo_pop_skip(fifo_handle_t *p_fifo, uint32_t *len)
{
	fifo_elem_t *p_elem 	= (fifo_elem_t*)p_fifo->elem;
	uint32_t needFifoSize 	= *len;

	if (p_elem->fifo == NULL)								return FIFO_NULL;
	if (!p_elem->enough || fifo_get_data_len(p_fifo) < 2 || fifo_get_data_len(p_fifo) < needFifoSize)
	{
		*len = 0;
		p_elem->enough = 0;
		return FIFO_EMPTY;
	}

//	if(fifo_get_data_len(p_fifo) < needFifoSize)
//		*len = fifo_get_data_len(p_fifo)*bitWidth/p_elem->slotSize;
	p_elem->headPtr = p_elem->headPtr + needFifoSize;
	if(p_elem->headPtr >= p_elem->fifoSize)
		p_elem->headPtr = p_elem->headPtr - p_elem->fifoSize;

	if(p_elem->full && needFifoSize > 0)
		p_elem->full = false;
	if(p_elem->thresholdLevel > fifo_get_data_len(p_fifo))
		p_elem->enough = 0;

	return FIFO_SUCCESS;
}

/**
 * @brief	Return the reading offset of the fifo.
 * @param[in]	p_fifo      Handle of the fifo.
 * @return	Return the reading offset of the fifo, value range is 0-fifosize.
 */
uint32_t fifo_get_head(fifo_handle_t *p_fifo) //return readPtr
{
	fifo_elem_t *p_elem 	= (fifo_elem_t*)p_fifo->elem;

	return (uint32_t)((uint8_t *)p_elem->fifo + p_elem->headPtr);
}
/**
 * @brief	Return the writing offset of the fifo.
 * @param[in]	p_fifo      Handle of the fifo.
 * @return	Return the writing offset of the fifo, value range is 0-fifosize.
 */
uint32_t fifo_get_tail(fifo_handle_t *p_fifo) //return writePtr
{
	fifo_elem_t *p_elem 	= (fifo_elem_t*)p_fifo->elem;

	return (uint32_t)((uint8_t *)p_elem->fifo + p_elem->tailPtr);
}

/**
 * @brief	Flush the data
 * @note	Reserved
 * @param[in]	p_fifo      Handle of the data
 * @return	None
 */
void fifo_flush(fifo_handle_t *p_fifo)
{

}
/**
 * @brief	Inset some data into the fifo.
 * @note	Reserved
 * @param[in]	p_fifo      Handle of fifo
 * @param[in]	pos         The position of the data inseted
 * @param[in]	len         The length of the data inseted
 * @return	None
 */
void fifo_insert(fifo_handle_t *p_fifo, uint32_t pos, uint32_t len)
{

}
/**
 * @brief	Used to jump to the specified position
 * @note	{description}
 * @param[in]	p_fifo      Handle of fifo
 * @param[in]	pos         Position of the fifo needs to jump
 * @param[in]	len         reserved
 * @return	None
 */
void fifo_skip(fifo_handle_t *p_fifo, uint32_t pos, uint32_t len)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;
	p_elem->tailPtr = pos;
}
/**
 * @brief	Used to reset the fifo when the samplerate changes
 * @note	Some parameters are reset
 * @param[in]	p_fifo      Handle of fifo.
 * @param[in]	streamFormat New stream format for fifo reset.
 * @return	None
 */
void fifo_reset(fifo_handle_t *p_fifo, auStrmFormat_t streamFormat)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;

	p_elem->tailPtr = 0;
	p_elem->headPtr = 0;
	p_elem->enough = false;
	p_elem->full = false;
	p_elem->sampleRate = streamFormat.sampleRateHz;
#if 0 // thresholdLevel and fifosize are fixed,no need to reset
	p_elem->thresholdLevel = p_elem->thresholdTime * p_elem->frameSize * p_elem->sampleRate / 1000U;
	p_elem->fifoSize = p_elem->frameLength * p_elem->frameSize * p_elem->sampleRate / 1000U;
	if(p_elem->thresholdLevel > 0)
	{
		if(p_elem->thresholdLevel%(p_elem->frameSize) != 0)
		{
			p_elem->thresholdLevel = p_elem->thresholdLevel - p_elem->thresholdLevel%(p_elem->frameSize);
			AU_FIFO_DEBUG("fifo_reset thresholdLevel and frameSize alignment!!\n");
		}
		if(p_elem->fifoSize%p_elem->thresholdLevel != 0)
		{
			p_elem->fifoSize = p_elem->fifoSize - (p_elem->fifoSize%p_elem->thresholdLevel);
			AU_FIFO_DEBUG("fifo_reset fifoSize and thresholdLevel alignment!!\n");
		}
	}
	else
	{
		auSession_t* pSession = (auSession_t *)p_elem->sData;
		uint32_t capThresholdLv = fifo_get_thresholdLevel(pSession->capture[0]->fifo);
		if(capThresholdLv > 0 && p_elem->fifoSize%capThresholdLv != 0)
		{
			p_elem->fifoSize = p_elem->fifoSize - (p_elem->fifoSize%capThresholdLv);
			AU_FIFO_DEBUG("fifo_reset fifoSize and capThresholdLv alignment!!\n");
		}
	}
//	memset(p_elem->fifo, 0, p_elem->fifoSize);//Removed to reduce time-consuming for USB interrupts
#endif
//	AU_FIFO_DEBUG("fifo_reset sampleRate:%d, fifo:%x, thresholdLevel:%d, fifoSize:%d\n", p_elem->sampleRate, p_elem->fifo, p_elem->thresholdLevel, p_elem->fifoSize);
}
/**
 * @brief	Used to fifo initialization
 * @param[in]	p_fifo      Handle of fifo
 * @return	None
 */
void fifo_init(fifo_handle_t *p_fifo)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;
	p_elem->tailPtr = 0;
	p_elem->headPtr = 0;
	p_elem->enough = false;
	p_elem->full  = false;
	p_elem->isPlaying = true;
	memset(p_elem->fifo, 0, p_elem->fifoSize);
}
/**
 * @brief	Used to reinit fifo
 * @note	Unsupport re-initialization,due to memory fragmentation
 * @param[in]	p_fifo      Handle of re-initialization fifo
 */
void fifo_reinit(fifo_handle_t *p_fifo)
{
//	free(p_fifo);
//	p_fifo = creat_fifo();

}
/**
 * @brief	Register the shared fifo into the original fifo
 * @note	{description}
 * @param[in]	p_srcFifo   The original fifo with separate memory space.
 * @param[in]	p_shareFifo The shared fifo using shared memory space.
 * @return	{description}
 * 	@retval FIFO_NULL failed
 * 	@retval FIFO_SUCCESS success
 */
uint32_t fifo_add_share_fifo(fifo_handle_t *p_srcFifo, fifo_handle_t *p_shareFifo)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_srcFifo->elem;

	if(p_elem->shareFifoNum < D_SHARE_FIFO_MAX)
	{
		p_elem->shareFifoHandle[p_elem->shareFifoNum] = p_shareFifo;
		p_elem->shareFifoNum++;
//		AU_FIFO_DEBUG("fifo_add_share_fifo %x, p_elem->useShareBuf:%d\n", p_shareFifo, p_elem->useShareBuf);
		return FIFO_SUCCESS;
	}
	return FIFO_NULL;
}
/**
 * @brief	Interface of controlling fifo
 */
fifo_ctrl_t fifo_func={
	fifo_init,
	fifo_reset,
	fifo_skip,
	fifo_insert,

	fifo_add_data,
	fifo_get_data,
	fifo_push_skip,
	fifo_pop_skip,
	fifo_get_head,
	fifo_get_tail,

//	fifo_get,
	fifo_is_empty,
	fifo_is_full,
	fifo_is_enough,
	fifo_get_free_len,
	fifo_get_data_len,
	fifo_get_fifo_size,
	fifo_get_thresholdLevel,
	fifo_get_share_num,
	NULL,
	fifo_add_share_fifo,

	fifo_get_sdata,
	fifo_set_playing_state,
	fifo_get_playing_state,
};

#if (DMA_ENABLE)

#include "drv_dmac.h"
typedef struct{
	fifo_handle_t *pFifoHandle;
	uint32_t fifoAddr;
	uint32_t * ptrTail;
	uint32_t * ptrHead;
	uint32_t maxLen;
	uint32_t frameSize;
	fifo_dmac_callback_t fifoCb;
}fifo_dma_cfg_t;

fifo_dma_cfg_t fifoDmaAdcCfg;
fifo_dma_cfg_t fifoDmaI2S1Cfg;
bool dmacAdcChxIsOpen = 0;

void dmac_cb_i2s_tx(uint8_t chx)
{
	*fifoDmaI2S1Cfg.ptrHead += (gDmacChx[chx].cfg.blockSize * 4);
	if(*fifoDmaI2S1Cfg.ptrHead >= fifoDmaI2S1Cfg.maxLen)
	{
		*fifoDmaI2S1Cfg.ptrHead = 0;
		gDmacChx[chx].cfg.srcAddr = fifoDmaI2S1Cfg.fifoAddr;
		dmac_reopen_channel(chx, gDmacChx[chx].cfg.srcAddr, 0, 0);
	}
	else
	{
		dmac_reopen_channel(chx, 0, 0, 0);
	}
}

void fifo_dmac_register_channel(fifo_handle_t *p_fifo, dmac_channel_num_t chx, fifo_dmac_param_t dmacParam, fifo_dmac_callback_t callback)
{
	switch(chx)
	{
	case DMA_CHX_MEM_TO_I2S1:
		gDmacChx[chx].cfg.trfType = DMAC_MEMORY_TO_PERIPHERAL;
		gDmacChx[chx].cfg.chPriority = 0;
		gDmacChx[chx].cfg.blockSize = dmacParam.blockSize;
		gDmacChx[chx].cfg.srcAddr = (uint32_t)dmacParam.pFifo;
		gDmacChx[chx].cfg.i2sSel = 0;
		gDmacChx[chx].cfg.intrBlockEn = 1;
		gDmacChx[chx].cb[DMAC_INTERRUPT_BLOCK] = dmac_cb_i2s_tx;

		fifoDmaI2S1Cfg.fifoAddr = (uint32_t)dmacParam.pFifo;
		fifoDmaI2S1Cfg.pFifoHandle = p_fifo;
		fifoDmaI2S1Cfg.ptrHead = dmacParam.pHeadPtr;
		fifoDmaI2S1Cfg.maxLen = dmacParam.maxLen;
		fifoDmaI2S1Cfg.fifoCb = callback;

		dmac_open_channel(chx, CHX_MEM_TO_I2S);
		break;
	case DMA_CHX_ADC_TO_MEM:
		gDmacChx[chx].cfg.trfType = DMAC_MEMORY_TO_MEMORY;
		gDmacChx[chx].cfg.chPriority = 0;
		gDmacChx[chx].cfg.blockSize = 2;
		fifoDmaAdcCfg.frameSize = dmacParam.blockSize;
		gDmacChx[chx].cfg.srcAddr = CODEC_DATA_BASE;
		gDmacChx[chx].cfg.dstAddr = (uint32_t)dmacParam.pFifo;
		gDmacChx[chx].cfg.intrBlockEn = 0;

		fifoDmaAdcCfg.fifoAddr = (uint32_t)dmacParam.pFifo;
		fifoDmaAdcCfg.pFifoHandle = p_fifo;
		fifoDmaAdcCfg.ptrTail = dmacParam.pTailPtr;
		fifoDmaAdcCfg.maxLen = dmacParam.maxLen;
		fifoDmaAdcCfg.fifoCb = callback;
		dmacAdcChxIsOpen = 1;
		break;
	default:
		break;
	}
}

/**
 * @brief	The callback of DMAC push data.
 * @param[in]	p_fifo      handle to the fifo using DMAC
 * @return	None
 */
void fifo_push_dmac_callback(fifo_handle_t *p_fifo)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;
	auSlotSize_t bitWidth = BIT_SLOT_32;
	uint32_t blockSize = p_elem->frameSize;
	uint32_t needFifoSize = blockSize*p_elem->slotSize/bitWidth;

	if (p_elem->fifo == NULL)						return;

	if(fifo_get_free_len(p_fifo) < needFifoSize && needFifoSize > 0 && !p_elem->full)
	{
		AU_FIFO_WARN("add size:%d free:%d p_elem->tailPtr:%d, %d fullch0=====\n\n", blockSize, fifo_get_free_len(p_fifo), p_elem->tailPtr, p_elem->headPtr);
		p_elem->full = true;
	}
	//fifo is full
	if (fifo_get_free_len(p_fifo) < needFifoSize)
	{
		p_elem->headPtr = (p_elem->tailPtr + needFifoSize)%p_elem->fifoSize;
	}

	if (!p_elem->enough && p_elem->thresholdLevel < fifo_get_data_len(p_fifo))
	{
		p_elem->enough = true;
		if(p_fifo->fifo_ready_cb != NULL)
		{
//			p_fifo->fifo_ready_cb(p_fifo, &p_elem->fifo[p_elem->headPtr], p_elem->thresholdLevel);
			//AU_FIFO_DEBUG("trigger irq:%d.\n", p_elem->irq);
			if(p_elem->irq == SW_IRQn)
				hal_interrupt_irq_software_trigger(p_elem->irq);
			else
				hal_interrupt_irq_force_trigger(hal_interrupt_get_intr_src(p_elem->irq));
		}
	}
}
/**
 * @brief	The callback of DMAC pop data.
 * @note	{description}
 * @param[in]	p_fifo      handle to the fifo using DMAC
 * @return	None
 */
void fifo_pop_dmac_callback(fifo_handle_t *p_fifo)
{
	fifo_elem_t *p_elem 	= (fifo_elem_t*)p_fifo->elem;
	auSlotSize_t bitWidth = BIT_SLOT_32;
	uint32_t blockSize = p_elem->frameSize;
	uint32_t needFifoSize 	= blockSize*p_elem->slotSize/bitWidth;
	bool		*full;
	if (p_elem->fifo == NULL)		return;

	if(p_elem->full && needFifoSize > 0)
		p_elem->full = false;
}
/**
 * @brief	Update the fifo input using DMAC
 * @note	{description}
 * @param[in]	p_fifo      handle to the fifo using DMAC
 * @param[in]	dmaCh       channel id of damc #dmac_channel_num_t
 * @return	None
 */
void fifo_register_dmac_input(fifo_handle_t *p_fifo, dmac_channel_num_t dmaCh)
{
	fifo_dmac_param_t dmacParam;
	fifo_elem_t *p_elem 		= (fifo_elem_t*)p_fifo->elem;
	dmacParam.pFifo 			= p_elem->fifo;
	dmacParam.maxLen 			= p_elem->fifoSize;
	dmacParam.blockSize			= p_elem->frameSize;
	dmacParam.threshouldLevel 	= p_elem->thresholdLevel;
	dmacParam.pTailPtr 			= &p_elem->tailPtr;
	dmacParam.pHeadPtr 			= &p_elem->headPtr;
	fifo_dmac_register_channel(p_fifo, dmaCh, dmacParam, fifo_push_dmac_callback);

//	AU_FIFO_DEBUG("dma in register:%d \n", dmaCh);
}
/**
 * @brief	Update the fifo output using DMAC
 * @note	{description}
 * @param[in]	p_fifo      handle to the fifo using DMAC
 * @param[in]	dmaCh       channel id of damc #dmac_channel_num_t
 * @return	None
 */
void fifo_register_dmac_output(fifo_handle_t *p_fifo, dmac_channel_num_t dmaCh)
{
	fifo_dmac_param_t dmacParam;
	fifo_elem_t *p_elem 		= (fifo_elem_t*)p_fifo->elem;
	dmacParam.pFifo 			= p_elem->fifo;
	dmacParam.maxLen 			= p_elem->fifoSize;
	dmacParam.blockSize 		= p_elem->frameSize;
	dmacParam.threshouldLevel 	= p_elem->thresholdLevel;
	dmacParam.pTailPtr 			= &p_elem->tailPtr;
	dmacParam.pHeadPtr 			= &p_elem->headPtr;
	fifo_dmac_register_channel(p_fifo, dmaCh, dmacParam, fifo_pop_dmac_callback);

//	AU_FIFO_DEBUG("dma out register:%d \n", dmaCh);
}
#endif
/**
 * @brief	This function is used to update fifo parameters after fifo  created.
 * @note	The parameter of thresholdLevel is assigned here.
 * @param[in]	p_fifo      Handle of created fifo.
 * @param[in]	fifoParam   Fifo parameters to update.
 * @return	None
 */
void register_fifo_event(fifo_handle_t *p_fifo, auFifoEvent_t *fifoParam)
{
	if (fifoParam && p_fifo != NULL)
	{
		fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;
		p_elem->irq = fifoParam->irqNum;
		p_elem->thresholdTime = (fifoParam->frameThreshold * 1000U) / p_elem->sampleRate;
		p_elem->channelCopy = fifoParam->channelCopy;
		p_elem->thresholdLevel = fifoParam->frameThreshold*p_elem->frameSize;
		if(fifoParam->frameThreshold != 0 && p_elem->frameLength%fifoParam->frameThreshold != 0)
		{
			p_elem->frameLength = p_elem->frameLength - (p_elem->frameLength%fifoParam->frameThreshold);
			p_elem->fifoSize = p_elem->frameLength * p_elem->frameSize * p_elem->sampleRate / 1000U;
			AU_FIFO_DEBUG("register_fifo_event fifo size aligment!!.\n");
		}
		if (p_elem->thresholdLevel >= p_elem->fifoSize)	AU_FIFO_DEBUG("thresholdLevel larger than fifoSize!!.\n");
		if(p_elem->pcmFormat == PCM_PLANAR_FORMAT)
			p_elem->thresholdLevel = p_elem->thresholdLevel/2;
		p_fifo->fifo_ready_cb = fifoParam->onFifoReady;
		if(fifoParam->frameThreshold == 0)
		{
			auSession_t* pSession = (auSession_t *)p_elem->sData;

			uint32_t capThresholdLv = fifo_get_thresholdLevel(pSession->capture[0]->fifo);
			if(capThresholdLv > 0 && p_elem->fifoSize%capThresholdLv != 0)
			{
				p_elem->fifoSize = p_elem->fifoSize - (p_elem->fifoSize%capThresholdLv);
				AU_FIFO_DEBUG("register_fifo_event fifoSize and capThresholdLv alignment!!\n");
			}
			p_elem->enough = TRUE;
		}
		AU_FIFO_DEBUG("register_fifo_event threshold:%d, thresholdLevel:%d, enough:%d, sampleRate:%d, thresholdTime:%d, p_fifo->fifo_ready_cb:%x\n",
		fifoParam->frameThreshold, p_elem->thresholdLevel, p_elem->enough, p_elem->sampleRate, p_elem->thresholdTime, p_fifo->fifo_ready_cb);
	}
}

/**
 * @brief	This function is used to creat a new FIFO
 * @note	none
 * @param[in]	streamFormat Steam format for creat fifo
 * @param[in]	sData       Audio session handle of new fifo
 * @param[in]	deviceId    Device id of new fifo
 * @return	Fifo handle
 * 	@retval NULL Creat fifo failed
 * 	@retval Other values Creat fifo success
 */
 fifo_handle_t *creat_fifo(auStrmFormat_t *streamFormat, void *sData, auDeviceId_t deviceId)
{
	fifo_handle_t *p = NULL;
	if(streamFormat->bitSlot != BIT_SLOT_16 && streamFormat->bitSlot != BIT_SLOT_32)
	{
		AU_FIFO_ERROR("creat_fifo , bitSlot is illegal:%d.\n", streamFormat->bitSlot);
		streamFormat->bitSlot = BIT_SLOT_32;
	}
	uint32_t fifoSizeInByte = streamFormat->frameLength * streamFormat->channlesPerFrame * streamFormat->bitSlot;

//	AU_FIFO_DEBUG("creat_fifo: len=%d, bit_slot=%d, ch:%d, spr=%d, fifoSize=%d.\n", streamFormat->frameLength, streamFormat->bitSlot, streamFormat->channlesPerFrame, streamFormat->sampleRateHzMax, fifoSizeInByte);
	if (fifoSizeInByte)
	{
		p = (fifo_handle_t *)malloc(sizeof(fifo_handle_t));
		if (p != NULL)
		{
			memset(p, 0, sizeof(fifo_handle_t));
			p->elem = malloc(sizeof(fifo_elem_t));
			if (p->elem == NULL) return NULL;

			fifo_elem_t *p_elem = (fifo_elem_t*)p->elem;

			memset(p_elem, 0, sizeof(fifo_elem_t));

			p_elem->fifo = (int8_t*)malloc(fifoSizeInByte);
			if (p_elem->fifo == NULL) return NULL;
			memset(p_elem->fifo, 0, fifoSizeInByte);
			p_elem->frameLength = streamFormat->frameLength;
			p_elem->frameSize = streamFormat->bitSlot * streamFormat->channlesPerFrame;
			p_elem->fifoSize = p_elem->frameLength * streamFormat->channlesPerFrame * streamFormat->bitSlot;

			p_elem->channels = streamFormat->channlesPerFrame;
			p_elem->slotSize = streamFormat->bitSlot;
			p_elem->pcmFormat = streamFormat->pcmFormat;
			p_elem->sampleRate = streamFormat->sampleRateHz;
			p_elem->sampleRateMax = streamFormat->sampleRateHzMax;
			p_elem->thresholdLevel = p_elem->fifoSize/2;
			p_elem->thresholdTime = streamFormat->frameLength/2;
			p_elem->deviceId = deviceId;
			p_elem->sData = sData;

			p->fuc = &fifo_func;

			// init
			fifo_init(p);

			AU_FIFO_DEBUG("creat_fifo success:%x, Fifo:%x, fifoSize:%d, slotSize:%d, thresholdTime:%d, pcmFormat:%d, channelCopy:%d, fifo_ready_cb:%x, thresholdLevel:%d.\n",
			p, p_elem->fifo, p_elem->fifoSize, p_elem->slotSize, p_elem->thresholdTime, p_elem->pcmFormat, p_elem->channelCopy, p->fifo_ready_cb, p_elem->thresholdLevel);
		}
		else
		{
			AU_FIFO_ERROR("creat_fifo fail.\n");
		}
	}

	return p;
}
/**
 * @brief	This function is used to creat share fifo.
 * @note	If different devices use the same audio data,we can use share fifo,means sharred fifo memory.
 * @param[in]	p_fifo      The shared fifo handle.
 * @param[in]	sData       Audio session handle of new fifo
 * @param[in]	deviceId    Device id of new fifo
 * @return	The newly created fifo handly.
 * 	@retval NULL Creat fifo failed
 * 	@retval Other values Creat fifo success
 */
fifo_handle_t *creat_fifo_share(fifo_handle_t *p_fifo, void *sData, auDeviceId_t deviceId)
{
	if(p_fifo->elem != NULL)
	{
		fifo_elem_t *srcFifoElem = p_fifo->elem;
		fifo_handle_t *p = NULL;
		p = (fifo_handle_t *)malloc(sizeof(fifo_handle_t));
		if (p != NULL)
		{
			memset(p, 0, sizeof(fifo_handle_t));
			p->elem = malloc(sizeof(fifo_elem_t));
			if (p->elem == NULL) return NULL;
			memset(p->elem, 0, sizeof(fifo_elem_t));
			//memcpy(p->elem, srcFifoElem, sizeof(fifo_handle_t));
			fifo_elem_t *p_elem = (fifo_elem_t*)p->elem;
			p_elem->fifo = srcFifoElem->fifo;
			p_elem->slotSize = srcFifoElem->slotSize;
			p_elem->fifoSize = srcFifoElem->fifoSize;
			p_elem->frameLength = srcFifoElem->frameLength;
			p_elem->frameSize = srcFifoElem->frameSize;
			p_elem->tailPtr = srcFifoElem->tailPtr;
			p_elem->headPtr = srcFifoElem->headPtr;
			p_elem->enough = srcFifoElem->enough;
			p_elem->full = srcFifoElem->full;
			p_elem->sampleRate = srcFifoElem->sampleRate;
			p_elem->useShareBuf = true;
			p_elem->thresholdLevel = srcFifoElem->thresholdLevel;
			p_elem->thresholdTime = srcFifoElem->thresholdTime;
			p_elem->deviceId = deviceId;
			p_elem->sData = sData;

			p->fuc = &fifo_func;

			fifo_add_share_fifo(p_fifo, p);
//			AU_FIFO_DEBUG("add_share_fifo fifo:%x, %x, shareFifoNum:%d\n", p_elem->fifo, p, srcFifoElem->shareFifoNum);
		}
		return p;
	}
	return NULL;
}

void fifo_set_thresholdLevel(fifo_handle_t *p_fifo, uint32_t freq, uint8_t timeThres)
{
	fifo_elem_t *p_elem = (fifo_elem_t*)p_fifo->elem;

	p_elem->thresholdLevel = timeThres * 8 * (freq / 1000);
}


extern auManagerHandler_t audioManager;
/**
 * @brief	This function is used to display free fifo size in bytes
 * @note    None
 * @param   None
 * @return	None
 */
void display_fifo_information(void)
{
#if FIFO_DEBUG
    uint32_t session_count = 0;
    uint32_t stream_count = 0;
    for(session_count = 0;session_count < AUDIO_SESSION_MAX;session_count++)
    {
       for(stream_count = 0;stream_count < audioManager.session[session_count].captureNum;stream_count++)
       {
           AU_FIFO_DEBUG("session[%d]  capture[%d] fifo free:%d , fito total:%d , fifo addr:%8x\n",session_count,stream_count,fifo_get_free_len(audioManager.session[session_count].capture[stream_count]->fifo),
           ((fifo_elem_t *)(audioManager.session[session_count].capture[stream_count]->fifo->elem))->fifoSize,((fifo_elem_t *)(audioManager.session[session_count].capture[stream_count]->fifo->elem))->fifo);
       }

       for(stream_count = 0;stream_count < audioManager.session[session_count].playbackNum;stream_count++)
       {
           AU_FIFO_DEBUG("session[%d] playback[%d] fifo data:%d , fito total:%d , fifo addr:%8x\n",session_count,stream_count,fifo_get_data_len(audioManager.session[session_count].playback[stream_count]->fifo),
           ((fifo_elem_t *)(audioManager.session[session_count].playback[stream_count]->fifo->elem))->fifoSize,((fifo_elem_t *)(audioManager.session[session_count].playback[stream_count]->fifo->elem))->fifo);
       }
    }
#endif
}


#endif



