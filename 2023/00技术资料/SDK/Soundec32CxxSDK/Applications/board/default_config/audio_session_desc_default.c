/** 
***********************************************************
 * 
 * @file name	: audio_session_desc_default.c
 * @author		: yen.yu
 * @version		: Vxx.xx
 * @date		: 2022-03-23
 * @brief		: Interface of audio stream process
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
#if D_CONFIG_DEFAULT
#include "audio_def.h"
#include "audio_hw_if.h"
#include "audio_session.h"
#include "audio_manager.h"
#include "usbd_config.h"

#if (AUDIO_MANAGER)
/* Private typedef -----------------------------------------------------------*/

/* Private macro ------------------------------------------------------------*/
static uint32_t mems12_onFifoReady(uint32_t frameLen, uint8_t pickFifoNum, int32_t **pickFifo, uint8_t pokeFifoNum, int32_t **pokeFifo);
static uint32_t spk_onFifoReady(uint32_t frameLen, uint8_t pickFifoNum, int32_t **pickFifo, uint8_t pokeFifoNum, int32_t **pokeFifo);

/* Private variables ---------------------------------------------------------*/
#define SESSION0_FRAME_SIZE					960 								// points
#define SESSION0_BUFFER_MULTIPLE			2 									// Buffer size = SESSION0_FRAME_SIZE * SESSION0_BUFFER_MULTIPLE

#define SESSION1_FRAME_SIZE					960 								// points
#define SESSION1_BUFFER_MULTIPLE			2

/* Private function prototypes -------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
//--------Mems12---------------
#if (CODEC_ADC_ENABLE)
auCaptureIf_t adc_in 	= {
	.capture = {
		.devId = AUDIO_DEVICE_ADC_IN,											//device id
		.type = STREAM_MUSIC,													//reserved
	},
	
	.format = {
		.channlesPerFrame	= 2,												//channels
		.frameLength		= SESSION0_FRAME_SIZE*SESSION0_BUFFER_MULTIPLE,		//points
		.pcmFormat			= PCM_PACKED_FORMAT,								//data format:LR LR LR
		.bitSlot 			= BIT_SLOT_32,										//bit width for data push & pop, support 16bit/32bit
		.sampleRateHz 		= SAMPLING_RATE_48000,								//ADC sample rate
		.sampleRateHzMax	= SAMPLING_RATE_192000,								//reserved
	},
};
#endif

#if (I2S1_ENABLE)
//--------I2S1-In---------------
auCaptureIf_t i2s1_in 	= {
	.capture = {
		.devId = AUDIO_DEVICE_I2S1,												//device id
		.type = STREAM_MUSIC,													//reserved
	},
	.format = {
		.channlesPerFrame	= 2,												//channels
		.frameLength		= SESSION1_FRAME_SIZE*SESSION1_BUFFER_MULTIPLE,		//points
		.pcmFormat			= PCM_PACKED_FORMAT,								//data format:LR LR LR
		.bitSlot 			= BIT_SLOT_32,										//bit width for data push & pop, support 16bit/32bit
		.sampleRateHz 		= SAMPLING_RATE_48000,								//I2S sample rate
		.sampleRateHzMax 	= SAMPLING_RATE_48000,								//reserved
	},
};
#endif

//-----------------------
#if (SUPPORT_USB_SPK)
auCaptureIf_t usb_playback 	= {
	.capture = {
		.devId = AUDIO_DEVICE_USB,												//device id
		.type = STREAM_MUSIC,													//reserved
	},
	.format = {
		.channlesPerFrame	= 2,												//channels
		.frameLength		= SESSION1_FRAME_SIZE*SESSION1_BUFFER_MULTIPLE,		//points
		.pcmFormat			= PCM_PACKED_FORMAT,								//data format:LR LR LR
		.bitSlot 			= BIT_SLOT_32,										//bit width for data push & pop, support 16bit/32bit
		.sampleRateHz 		= SAMPLING_RATE_48000,								//USB play sample rate
		.sampleRateHzMax 	= SAMPLING_RATE_192000,								//reserved
	},
};
#endif

//-----------------------
#if (CODEC_DAC_ENABLE)
auPlaybackIf_t dac_out 	= {
	.playback = {
		.devId = AUDIO_DEVICE_CODEC_OUT,										//device id
		.type = STREAM_VOICE,													//reserved
	},
	.format = {
		.channlesPerFrame	= 2,												//channels
		.frameLength		= SESSION1_FRAME_SIZE*SESSION1_BUFFER_MULTIPLE,		//points
		.pcmFormat			= PCM_PACKED_FORMAT,								//data format:LR LR LR
		.bitSlot 			= BIT_SLOT_32,										//bit width for data push & pop, support 16bit/32bit
		.sampleRateHz 		= SAMPLING_RATE_48000,								//DAC sample rate
		.sampleRateHzMax 	= SAMPLING_RATE_192000,								//reserved
	},	
	.channelCopy = FALSE,														//two channels output different data
	.fifoId		= 0,															//0~N,N<playbackNum, share fifo use the same fifoId
};
#endif
		
#if (I2S1_ENABLE)
//-----------------------
auPlaybackIf_t i2s1_out	= {
	.playback = {
		.devId = AUDIO_DEVICE_I2S1,												//device id
		.type = STREAM_MUSIC,													//reserved
	},
	.format = {
		.channlesPerFrame	= 2,												//channels
		.frameLength		= SESSION0_FRAME_SIZE*SESSION0_BUFFER_MULTIPLE,		//points
		.pcmFormat			= PCM_PACKED_FORMAT,								//data format:LR LR LR
		.bitSlot 			= BIT_SLOT_32,										//bit width for data push & pop, support 16bit/32bit
		.sampleRateHz 		= SAMPLING_RATE_48000,								//I2S sample rate
		.sampleRateHzMax 	= SAMPLING_RATE_192000,								//reserved
	},	
	.channelCopy = FALSE,														//two channels output different data
	.fifoId		= 0,															//0~N,N<playbackNum, share fifo use the same fifoId
};
#endif
//-----------------------
#if (SUPPORT_USB_MIC)
auPlaybackIf_t usb_record 	= {
	.playback = {
		.devId = AUDIO_DEVICE_USB,												//device id
		.type = STREAM_MUSIC,													//reserved
	},
	.format = {
		.channlesPerFrame	= 2,												//channels
		.frameLength		= SESSION0_FRAME_SIZE*SESSION0_BUFFER_MULTIPLE,		//points
		.pcmFormat			= PCM_PACKED_FORMAT,								//data format:LR LR LR
		.bitSlot 			= BIT_SLOT_32,										//bit width for data push & pop, support 16bit/32bit
		.sampleRateHz 		= SAMPLING_RATE_48000,								//USB record sample rate
		.sampleRateHzMax 	= SAMPLING_RATE_192000,								//reserved
	},
	.channelCopy = FALSE,														//two channels output different data
	.fifoId		= 0,															//0~N,N<playbackNum, share fifo use the same fifoId
};
#endif

auCaptureIf_t *session1StreamIn[]=
{
#if (SUPPORT_USB_SPK)
	&usb_playback,
#endif
};
auPlaybackIf_t *session1StreamOut[] = {
	&dac_out,
};

auPolicyParam_t session0Policy ={
	.interruptUnmixedSession = FALSE,											//reserved
	.syncSamplerate = TRUE,														//reserved
	.procBypass = FALSE,														//reserved

	.bitSlot = BIT_SLOT_32,														//reserved
	.sampleRate = 48000,														//reserved
	.maxSampleRate = 192000,													//reserved
	.maxLatencyInFrames = 0,													//reserved
};

auProcIf_t session0Proc = {
	.procEvent = {
		.frameThreshold = SESSION0_FRAME_SIZE,									//trigger soft interrupt threshold
		.onFifoReady = mems12_onFifoReady,										//soft interrupt process function
		.channelCopy = FALSE,													//reserved
		.irqNum = SW_IRQn,														//interrupt type
	},
	.outputFormat = {
		.channlesPerFrame	= 2,												//channels
		.frameLength		= SESSION0_FRAME_SIZE*SESSION0_BUFFER_MULTIPLE,		//points
		.pcmFormat			= PCM_PACKED_FORMAT,								//data format:LR LR LR
		.bitSlot 			= BIT_SLOT_32,										//bit width for data push & pop, support 16bit/32bit
		.sampleRateHz 		= SAMPLING_RATE_48000,								//reserved
		.sampleRateHzMax 	= SAMPLING_RATE_192000,								//reserved
	},
};

auProcIf_t session1Proc = {
	.procEvent = {
		.frameThreshold = SESSION1_FRAME_SIZE,									//trigger soft interrupt threshold
		.onFifoReady = spk_onFifoReady,										//soft interrupt process function
		.channelCopy = FALSE,													//reserved
		.irqNum = EXT_SW1_IRQn,														//interrupt type
	},
	.outputFormat = {
		.channlesPerFrame	= 2,												//channels
		.frameLength		= SESSION1_FRAME_SIZE*SESSION1_BUFFER_MULTIPLE,		//points
		.pcmFormat			= PCM_PACKED_FORMAT,								//data format:LR LR LR
		.bitSlot 			= BIT_SLOT_32,										//bit width for data push & pop, support 16bit/32bit
		.sampleRateHz 		= SAMPLING_RATE_48000,								//reserved
		.sampleRateHzMax 	= SAMPLING_RATE_192000,								//reserved
	},
};
		
auCaptureIf_t *session0StreamIn[] = {
	&adc_in,
};
	
auPlaybackIf_t *session0StreamOut[] = {
#if (SUPPORT_USB_MIC)
	&usb_record,
#endif
};

//-----------------------
auSessionDesc_t auSessionDesc[AUDIO_SESSION_MAX] = {
	{
		.id 				= AUDIO_SESSION_0,
		.sessionPolicy 		= &session0Policy,									//reserved
		.captureNum 		= 1,
		.capture 			= session0StreamIn,
		.procIf 			= NULL,
		.playbackNum 		= 1,
		.playback 			= session0StreamOut,
	},
	{
		.id 				= AUDIO_SESSION_1,
		.sessionPolicy 		= &session0Policy,									//reserved
		.captureNum 		= 1,
		.capture 			= session1StreamIn,
		.procIf 			= NULL,
		.playbackNum 		= 1,
		.playback 			= session1StreamOut,
	},
};

/**
 * @brief		This function is used to do algorithm processing.
 * @note		none
 * @param[in]	frameLen	 the length of each pickFifo, equal to frameThreshold*BIT_SLOT_32*channlesPerFrame
 * @param[in]	pickFifoNum  number of capture, equal to captureNum
 * @param[in]	pickFifo     capture fifo
 * @param[in]	pokeFifoNum  number of playback, equal to playbackNum
 * @param[in]	pokeFifo     playback fifo
 * @return		the length of each pokeFifo
 */
static uint32_t mems12_onFifoReady(uint32_t frameLen, uint8_t pickFifoNum, int32_t **pickFifo, uint8_t pokeFifoNum, int32_t **pokeFifo)
{
	memcpy(pokeFifo[0], pickFifo[0], frameLen);
	return frameLen;
}
static uint32_t spk_onFifoReady(uint32_t frameLen, uint8_t pickFifoNum, int32_t **pickFifo, uint8_t pokeFifoNum, int32_t **pokeFifo)
{
	memcpy(pokeFifo[0], pickFifo[0], frameLen);
	return frameLen;
}
#endif
#endif


