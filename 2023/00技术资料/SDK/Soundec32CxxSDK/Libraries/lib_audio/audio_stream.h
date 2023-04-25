/** 
***********************************************************
 * 
 * @file name	: audio_stream.h
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2022-02-18
 * @brief		: Audio stream related defines.
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
#ifndef __AUDIO_STREAM_H__
#define __AUDIO_STREAM_H__

/* Includes ------------------------------------------------------------------*/
#include "audio_def.h"
#include "audio_hw_if.h"
#include "audio_fifo_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
typedef fifo_handle_t		auFifo_t;

typedef struct{
	auFifoEvent_t			procEvent;				// Informations that pickup from capture FiFo
	auStrmFormat_t 			outputFormat;			// Stream output format of processer routine, which decided the result fifo.
	
}auProcIf_t;

typedef struct{
	bool noLatency;
	uint8_t pickFifoNum;
	uint8_t pokeFifoNum;
	auFifo_t **pickFifo;
	int32_t **pickAddrList;
	auFifo_t **pokeFifo;
	int32_t **pokeAddrList;
	IRQn_Type irqNum;
	
}auProcFifoCb_t;

/*
	Event is pickup information of processor routine.
	Format is the store information of processer result.
	Result is the fifo of processer output (Allocated when auManager initial fifo)
*/
typedef struct{
	auFifoEvent_t			*event;			/*Register a event to fifo which need to be processed*/
	auStrmFormat_t 			*format;		/*Set format for the result fifo of onfifoready event*/
	auFifo_t				*result;		/*Result of onfifoready in event, NULL if onfifoready == NULL*/
	auProcFifoCb_t 			cbEvents;

}auProcOnStream_t;

typedef struct{
	uint8_t 				mute;
	uint8_t 				useSwGain;		/*configure from hw_desc.c*/
	ALIGN(8) int32_t 		again[2];	
	ALIGN(8) int32_t 		dgain[2];	
	ALIGN(8) int32_t 		swgain[2];	
	bool  					suspend;;
#ifdef TEST_AU_MANAGER
	uint32_t				_1sLen;
	auSlotSize_t			pushPopSlot;
#endif	
	
}auCtrlVar_t;

typedef enum{
	STREAM_PLAYBACK = 0,
	STREAM_CAPTURE,
	
}auStreamRole_t;

typedef struct{
	auDeviceId_t 			devId;
	auStreamRole_t			streamRole;
	auCtrlVar_t				var;
	audioDevIf_t			*device;
	auDevFormatCapacity_t	*formatCap;
	auStrmFormat_t			*format;
	auStreamOps_t			*ops;
	auFifoEvent_t			*event;			/*Register a event to fifo which need to be processed*/
	auFifo_t				*fifo;			/*Result of onfifoready event; For capture inputstream, onfifoready=rxcb*/
	uint8_t 				fifoId;			/*Just for playback stream output fifo, share fifo use the same fifoId */
	auFifoState_t			fifoState;		/*0x00:noFifo, 0x01:exclusive; 0x02: reused fifo, 0x04: shared fifo*/
	dmac_channel_num_t		dmaChannel;
//	auProcOnStream_t		*proc;			/*processor on fifo: register event on fifo(devId), use format creat a result fifo for proc results*/

}auStream_t;


typedef struct {
	int32_t analog_gain;
	int32_t digital_gain;
	
}codec_adc_vol_t;



/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
auManagerSt_t auStream_create_fifo(auFifo_t **stream, auStrmFormat_t *format, void *sData, auDeviceId_t deviceId);
auManagerSt_t auStream_create_fifo_share(auFifo_t **stream, auFifo_t *shareFifo, auStrmFormat_t *format, void *sData, auDeviceId_t deviceId);
auManagerSt_t auStream_register_event_on_fifo(auFifo_t *stream, auFifoEvent_t *param);
auManagerSt_t auStream_reset_fifo(void *stream);

void auStream_cal_gain(int32_t *gain, int32_t *__restrict in, int32_t *__restrict out);
void auStream_update_info(void *arg, auCtrlPath_t path, auInfoType_t type, void *info);
auStreamDetect_t auStream_detect_capture_signal(int32_t *micDat, uint32_t datThr, uint32_t *cal, auStrmFormat_t *format);
auStreamDetect_t auStream_detect_playback_signal(int32_t *micDat, uint32_t datThr, uint32_t *cal, auStrmFormat_t *format);


#ifdef __cplusplus
}
#endif

#endif  /* __AUDIO_STREAM_H__ */


