/**
***********************************************************
 *
 * @file name	: audio_fifo_manager.h
 * @author		:
 * @version		: Vxx.xx
 * @date		: 2022-01-19
 * @brief		: Include file of fifo magager
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
#ifndef __AUDIO_FIFO_MANAGER_H__
#define __AUDIO_FIFO_MANAGER_H__

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "audio_def.h"
#include "audio_hw_if.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
#define FIFO_SUCCESS				0
#define FIFO_MAX_LENGTH_ERROR		1
#define FIFO_MALLOC_ERROR			2
#define FIFO_FULL					4
#define FIFO_EMPTY					8
#define FIFO_NULL					16
#define FIFO_INVALID_PTR			32
#define FIFO_BITSLOT_ERROR			64


typedef enum
{
	FIFO_OFFSET_NONE = 0,	//freelen == buflen
	FIFO_OFFSET_HALF,		//freelen < buflen/2
	FIFO_OFFSET_FULL,		//freelen == 0
	FIFO_OFFSET_UNKNOWN,	//freelen > buflen/2 && freelen < buflen

}fifoOffset_t;

struct fifo_handle_st;

typedef struct {
	void (*init) 					(struct fifo_handle_st *p_fifo);	/*len in byte*/
	void (*reset)					(struct fifo_handle_st *p_fifo, auStrmFormat_t streamFormat);
	void (*skip)					(struct fifo_handle_st *p_fifo, uint32_t pos, uint32_t len);
	void (*insert)					(struct fifo_handle_st *p_fifo, uint32_t pos, uint32_t len);

	uint32_t (*push)				(struct fifo_handle_st *p_fifo, uint8_t *__restrict data, uint32_t sizeInByte, auSlotSize_t bitWidth);
	uint32_t (*pop) 				(struct fifo_handle_st *p_fifo, uint8_t *data, uint32_t *len, auSlotSize_t bitWidth);
	uint32_t (*push_skip)			(struct fifo_handle_st *p_fifo, uint32_t len);
	uint32_t (*pop_skip) 			(struct fifo_handle_st *p_fifo, uint32_t *len);
	uint32_t (*get_fifo_head)		(struct fifo_handle_st *p_fifo);//return readPtr
	uint32_t (*get_fifo_tail)		(struct fifo_handle_st *p_fifo);//return writePtr

//	uint32_t (*get_fifo)			(struct fifo_handle_st *p_fifo);
	bool (*is_empty)				(struct fifo_handle_st *p_fifo);
	bool (*is_full) 				(struct fifo_handle_st *p_fifo);
	bool (*is_enough)				(struct fifo_handle_st *p_fifo);

	uint32_t (*get_free_len)		(struct fifo_handle_st *p_fifo);
	uint32_t (*get_data_len)		(struct fifo_handle_st *p_fifo);
	uint32_t (*get_fifo_size)		(struct fifo_handle_st *p_fifo);
	uint32_t (*get_fifo_thresholdLevel)		(struct fifo_handle_st *p_fifo);
	uint8_t (*get_fifo_share)		(struct fifo_handle_st *p_fifo);
	fifoOffset_t (*get_status)		(struct fifo_handle_st *p_fifo);
	uint32_t (*add_share_fifo)		(struct fifo_handle_st *p_srcFifo, struct fifo_handle_st *p_shareFifo);

	uint32_t (*get_sdata)				(struct fifo_handle_st *p_fifo);

	void (*set_playing_state) 		(struct fifo_handle_st *p_fifo, bool isPlaying);
	bool (*get_playing_state) 		(struct fifo_handle_st *p_fifo);

}fifo_ctrl_t;

typedef struct fifo_handle_st{
	void *elem;
	fifo_ctrl_t *fuc;
	fifo_ready_cb_t	fifo_ready_cb;
}fifo_handle_t;

typedef struct
{
	void*							pFifo;
	uint32_t						maxLen;
	uint32_t						blockSize;
	uint32_t						threshouldLevel;	// Must <= maxLen, used to set a treshould level which indicate FiFo is ready;when fifo size upto threshouldLen, generate a event for CB_functions
	uint32_t*						pTailPtr;	// write
	uint32_t*						pHeadPtr;	// read
}fifo_dmac_param_t;

typedef struct{
	dmac_channel_num_t fifoInDmacCh;
	dmac_channel_num_t fifoOut0DmacCh;
}fifo_dma_ch_cfg_t;


typedef void(*fifo_dmac_callback_t) (fifo_handle_t *p_fifo);
/**
 * @brief	This function is used to update fifo parameters after fifo  created.
 * @note	The parameter of thresholdLevel is assigned here.
 * @param[in]	p_fifo      Handle of created fifo.
 * @param[in]	fifoParam   Fifo parameters to update.
 * @return	None
 */
void register_fifo_event(fifo_handle_t *p_fifo, auFifoEvent_t *fifoParam);
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
fifo_handle_t *creat_fifo(auStrmFormat_t *streamFormat, void *sData, auDeviceId_t deviceId);
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
fifo_handle_t *creat_fifo_share(fifo_handle_t *p_fifo, void *sData, auDeviceId_t deviceId);


#if (DMA_ENABLE)
/**
 * @brief	Update the fifo input using DMAC
 * @note	{description}
 * @param[in]	p_fifo      handle to the fifo using DMAC
 * @param[in]	dmaCh       channel id of damc #dmac_channel_num_t
 * @return	None
 */
void fifo_register_dmac_input(fifo_handle_t *p_fifo, dmac_channel_num_t dmaCh);
/**
 * @brief	Update the fifo output using DMAC
 * @note	{description}
 * @param[in]	p_fifo      handle to the fifo using DMAC
 * @param[in]	dmaCh       channel id of damc #dmac_channel_num_t
 * @return	None
 */
void fifo_register_dmac_output(fifo_handle_t *p_fifo, dmac_channel_num_t dmaCh);
#endif
/**
 * @brief	Get the length of the remaining stored data in the fifo.
 * @param[in]	p_fifo      Handle of the data
 * @return	The length of the remaining stored data in the fifo.
 */
uint32_t fifo_get_free_len(fifo_handle_t *p_fifo);
/**
 * @brief	Get the length of data stored in the fifo.
 * @param[in]	p_fifo      Handle of the data
 * @return	The length of data stored in the fifo.
 */
uint32_t fifo_get_data_len(fifo_handle_t* p_fifo);
/**
 * @brief	Get fifo size parameter of the fifo.
 * @param[in]	p_fifo      Handle of the data
 * @return	Fifo size.
 */
uint32_t fifo_get_fifo_size(fifo_handle_t* p_fifo);
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
uint32_t fifo_add_data(fifo_handle_t *p_fifo, uint8_t *__restrict data, uint32_t sizeInByte, auSlotSize_t bitWidth);
/**
 * @brief	Read data from the fifo.
 * @note	{description}
 * @param[in]	p_fifo      Handle of the fifo.
 * @param[in]	data        The pointer to store data.
 * @param[in]	sizeInByte  Length of the data to be read,the unit is the number of bytes.
 * @param[in]	bitWidth    The bit-width of the data to be read.
 * @return	The result of the function execution.
 * 	@retval FIFO_INVALID_PTR The data parameter is null.
 * 	@retval FIFO_NULL Handle of fifo is error.
 * 	@retval FIFO_BITSLOT_ERROR The bit-width format used to read the data is incorrect.
 * 	@retval FIFO_EMPTY There is not enough data.
 * 	@retval FIFO_SUCCESS Read such length of data success.
 */
uint32_t fifo_get_data(fifo_handle_t *p_fifo, uint8_t *__restrict data, uint32_t *sizeInByte, auSlotSize_t bitWidth);
/**
 * @brief	Set audio device playing state into fifo manager.
 * @param[in]	p_fifo      Handle of the fifo.
 * @param[in]	isPlaying   The playback state to set.
 * 	@arg @ref xx {description if necessary}
 * @return	None
 */
void fifo_set_playing_state(fifo_handle_t* p_fifo, bool isPlaying);
/**
 * @brief	Get audio device playback state.
 * @param[in]	p_fifo      Handle of the fifo.
 * @return	playing state
 * 	@retval true Device is playing.
 * 	@retval false Device has stoped playing.
 */
bool fifo_get_playing_state(fifo_handle_t* p_fifo);
/**
 * @brief	Get enough parameter of the fifo.
 * @param[in]	p_fifo      Handle of the data
 * @return	Enough parameter.
 * 	@retval false Fifo is not enough.
 * 	@retval true Fifo is enough
 */
bool fifo_is_enough(fifo_handle_t* p_fifo);
/**
 * @brief	Get empty parameter of the fifo.
 * @param[in]	p_fifo      Handle of the data
 * @return	Empty parameter.
 * 	@retval false Fifo is not empty.
 * 	@retval true Fifo is empty
 */
bool fifo_is_empty(fifo_handle_t* p_fifo);

/**
 * @brief	This function is used to display free fifo size in bytes
 * @note    None
 * @param   None
 * @return	None
 */
void display_fifo_information(void);

uint32_t fifo_get_thresholdLevel(fifo_handle_t* p_fifo);
void fifo_set_thresholdLevel(fifo_handle_t *p_fifo, uint32_t freq, uint8_t timeThres);
bool fifo_is_enough(fifo_handle_t* p_fifo);
void fifo_finish_isr(fifo_handle_t *p_fifo);
#ifdef __cplusplus
}
#endif

#endif  /* __FIFO_MANAGER_H__ */




