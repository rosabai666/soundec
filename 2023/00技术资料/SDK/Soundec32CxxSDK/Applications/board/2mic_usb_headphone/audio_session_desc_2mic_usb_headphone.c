/** 
***********************************************************
 * 
 * @file name	: audio_session_desc_2mic_usb_headphone.c
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
#if D_CONFIG_2MIC_USB_HEADPHONE
#include "audio_def.h"
#include "audio_hw_if.h"
#include "audio_session.h"
#include "audio_manager.h"
#include "speech.h"

#if (AUDIO_MANAGER)
/* Private typedef -----------------------------------------------------------*/
#define SESSION0_FRAME_SIZE					480 								// points
#define SESSION0_BUFFER_MULTIPLE			2 									// Buffer size = SESSION0_FRAME_SIZE * SESSION0_BUFFER_MULTIPLE

#define SESSION1_FRAME_SIZE					480 								// points
#define SESSION1_BUFFER_MULTIPLE			2
/* Private macro ------------------------------------------------------------*/
static uint32_t headphone_onFifoReady(uint32_t frameLen, uint8_t pickFifoNum, int32_t **pickFifo, uint8_t pokeFifoNum, int32_t **pokeFifo);

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
//--------Mems12---------------
#if (CODEC_ADC12_ENABLE)
auCaptureIf_t mems12_in 	= {
	.capture = {
		.devId 				= AUDIO_DEVICE_MEMS12_IN,							//device id
		.type 				= STREAM_VOICE,										//reserved
	},
	.format = {
		.channlesPerFrame	= 2,												//channels
		.frameLength		= SESSION0_FRAME_SIZE * SESSION0_BUFFER_MULTIPLE,	//points
#if (SUPPORT_ALG_2MIC_HEADSET)
		.pcmFormat			= PCM_PLANAR_FORMAT,								//data format:LL...LL RR...RR
#else
		.pcmFormat			= PCM_PACKED_FORMAT,								//data format:LR LR LR
#endif
		.bitSlot 			= BIT_SLOT_32,										//bit width for data push & pop, support 16bit/32bit
		.sampleRateHz 		= SAMPLING_RATE_48000,								//ADC sample rate
		.sampleRateHzMax	= SAMPLING_RATE_48000,								//reserved
	},
};
#endif

//-----------------------
#if (SUPPORT_USB_SPK)
auCaptureIf_t usb_playback 	= {
	.capture = {
		.devId = AUDIO_DEVICE_USB,												//device id
		.type = STREAM_VOICE,													//reserved
	},
	.format = {
		.channlesPerFrame	= 2,												//channels
		.frameLength		= SESSION1_FRAME_SIZE * SESSION1_BUFFER_MULTIPLE,	//points
		.pcmFormat			= PCM_PACKED_FORMAT,								//data format:LR LR LR
		.bitSlot 			= BIT_SLOT_32,										//bit width for data push & pop, support 16bit/32bit
		.sampleRateHz 		= SAMPLING_RATE_48000,								//USB play sample rate
		.sampleRateHzMax 	= SAMPLING_RATE_48000,								//reserved
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
		.frameLength		= SESSION1_FRAME_SIZE * SESSION1_BUFFER_MULTIPLE,	//points
		.pcmFormat			= PCM_PACKED_FORMAT,								//data format:LR LR LR
		.bitSlot 			= BIT_SLOT_32,										//bit width for data push & pop, support 16bit/32bit
		.sampleRateHz 		= SAMPLING_RATE_48000,								//DAC sample rate
		.sampleRateHzMax 	= SAMPLING_RATE_48000,								//reserved
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
		.frameLength		= SESSION0_FRAME_SIZE * SESSION0_BUFFER_MULTIPLE,	//points
		.pcmFormat			= PCM_PACKED_FORMAT,								//data format:LR LR LR
		.bitSlot 			= BIT_SLOT_32,										//bit width for data push & pop, support 16bit/32bit
		.sampleRateHz 		= SAMPLING_RATE_48000,								//USB record sample rate
		.sampleRateHzMax 	= SAMPLING_RATE_48000,								//reserved
	},
#if (SUPPORT_ALG_2MIC_HEADSET)
	.channelCopy = TRUE,														//two channels output the same data
#else
	.channelCopy = FALSE,														//two channels output different data
#endif
	.fifoId		= 0,															//0~N,N<playbackNum, share fifo use the same fifoId
};
#endif
//----------Session AUDIO_SESSION_USB_RECORD-------------
auCaptureIf_t *session0StreamIn[]=
{
	&mems12_in,
};

auPlaybackIf_t *session0StreamOut[] = {
	&usb_record,
};

auPolicyParam_t session0Policy ={
	.interruptUnmixedSession = FALSE,											//reserved
	.syncSamplerate = TRUE,														//reserved
	.procBypass = FALSE,														//reserved

	.bitSlot = BIT_SLOT_32,														//reserved
	.sampleRate = 48000,														//reserved
	.maxSampleRate = 48000,														//reserved
	.maxLatencyInFrames = 0,													//reserved
};

auProcIf_t session0Proc = {
	.procEvent = {
		.frameThreshold = SESSION0_FRAME_SIZE,									//trigger soft interrupt threshold
		.onFifoReady = headphone_onFifoReady,									//soft interrupt process function
		.channelCopy = FALSE,													//reserved
		.irqNum = SW_IRQn,														//interrupt type
	},
	.outputFormat = {
#if (SUPPORT_ALG_2MIC_HEADSET)
		.channlesPerFrame	= 1,												//output channels
#else
		.channlesPerFrame	= 2,												//output channels
#endif
		.frameLength		= SESSION0_FRAME_SIZE * SESSION0_BUFFER_MULTIPLE,	//points
		.pcmFormat			= PCM_PACKED_FORMAT,								//data format:LR LR LR
		.bitSlot 			= BIT_SLOT_32,										//bit width for data push & pop, support 16bit/32bit
		.sampleRateHz 		= SAMPLING_RATE_48000,								//reserved
		.sampleRateHzMax 	= SAMPLING_RATE_48000,								//reserved
	},
};
		
//-----------Session AUDIO_SESSION_DAC_SIDETONE------------
auCaptureIf_t *session1StreamIn[] = {
	&usb_playback,
};

auPlaybackIf_t *session1StreamOut[] = {
	&dac_out,
};

//-----------------------
auSessionDesc_t auSessionDesc[AUDIO_SESSION_MAX] = {
	{
		.id = AUDIO_SESSION_0,
		.sessionPolicy = &session0Policy,										//reserved
		.captureNum = 1,
		.capture = session0StreamIn,

		.procIf = &session0Proc,
		
		.playbackNum = 1,
		.playback = session0StreamOut,
	},	
	{
		.id = AUDIO_SESSION_1,
		.sessionPolicy = &session0Policy,
		.captureNum = 1,
		.capture = session1StreamIn,

		.procIf = NULL,

		.playbackNum = 1,
		.playback = session1StreamOut,
	},
};

#if (SUPPORT_ALG_2MIC_HEADSET)
float key_post_val = 1.5;
float key_sound_val = 12;
float RefAmp_Gain = 1.5;
float Denoise_deep = 1.5;
float Add_Noise_Amp = 5000;
float denoise_remain = 0.1;
#endif

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
static uint32_t headphone_onFifoReady(uint32_t frameLen, uint8_t pickFifoNum, int32_t **pickFifo, uint8_t pokeFifoNum, int32_t **pokeFifo)
{
#if (SUPPORT_ALG_2MIC_HEADSET)
	FrtEndProc_new(pickFifo[0] + frameLen/8, pickFifo[0] + frameLen/8, pickFifo[0], pokeFifo[0]);
	return frameLen/2;	// should divide channlesPerFrame defined in routeProc; Depends on the length of proc result
#else
	memcpy(pokeFifo[0], pickFifo[0], frameLen);
	return frameLen;
#endif
}
#endif

#endif
