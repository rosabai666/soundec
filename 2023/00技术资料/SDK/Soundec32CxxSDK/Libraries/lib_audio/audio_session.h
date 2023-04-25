/** 
***********************************************************
 * 
 * @file name	: audio_session.h
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2022-02-10
 * @brief		: Include file of audio session
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
#ifndef __AUDIO_SESSION_H__
#define __AUDIO_SESSION_H__

/* Includes ------------------------------------------------------------------*/
#include "audio_def.h"
#include "audio_hw_if.h"
#include "audio_stream.h"
#include "audio_fifo_manager.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
typedef enum{
	AUDIO_SESSION_0 = 0,	
	AUDIO_SESSION_1,
	AUDIO_SESSION_MAX,
	
}auSessionId_t;

typedef struct{
	bool interruptUnmixedSession;		// Whether could interrupt the session that currently playback.
	bool syncSamplerate;
	bool procBypass;
	
	auSlotSize_t bitSlot;				// bitwidth of valid bits in store format
	uint32_t sampleRate;
	uint32_t maxSampleRate;
	uint32_t maxLatencyInFrames;		// Define the max latency within a full session: e.g. CaptureA mix with CaptureB and then to PlaybackC; Fifo ready of CaptureA is later than CaptureB; this maxLatency is the max frames that CaptureB will wait for CaptureA

}auPolicyParam_t;

typedef struct{
	void (*open)				();
	void (*close)				();
	void (*fadeIn)				();					// increasement with step 1 
	void (*fadeOut)				();					// decrement with step 1
	void (*tuneSampleRate)		(uint8_t dir);		// dir = 1: increasement with step 1; dir = 0:decrement with step 1;
	void (*tuneVol)				(uint8_t dir);		// dir = 1: increasement with step 1; dir = 0:decrement with step 1;
	void (*setVol)				(auSessionId_t id, uint8_t *volL, uint8_t *volR);
	void (*calGain)				(int32_t *gain, int32_t *__restrict in, int32_t *__restrict out);
	void (*setMute)				(auMute_t mute);
	
}auSessionDescOps_t;

typedef struct{
	auDeviceId_t 				devId;
	auStreamType_t 				type;
	
}auDeviceIf_t;

typedef struct{
	auDeviceIf_t 				capture;
	auStrmFormat_t 				format;		/*Format of fifo, from session_if.c*/

}auStreamIf_t,auCaptureIf_t;

typedef struct{
	auDeviceIf_t 				playback;
	auStrmFormat_t 				format;		/*Format of fifo, from session_if.c*/
	bool 						channelCopy; 				//true:Mono to double channel data
	uint8_t						fifoId;		/* 0~N,N<playbackNum, share fifo use the same fifoId */
}auPlaybackIf_t;

typedef struct{
	auSessionId_t 				id;

	auPolicyParam_t				*sessionPolicy;
	
	uint8_t 					captureNum;
	auCaptureIf_t 				**capture;

	auProcIf_t 					*procIf;
	
	uint8_t 					playbackNum;
	auPlaybackIf_t 				**playback;
	
	auSessionDescOps_t 				*ops;
	
}auSessionDesc_t;

typedef struct{
	auSessionId_t 				id;

	auPolicyParam_t				*sessionPolicy;
	
	uint8_t 					captureNum;
	auStream_t					**capture;

	auProcOnStream_t			*proc;			/* processor for all of captures fifo or capture->proc_fifo*/

	uint8_t 					playbackNum;
	auStream_t					**playback;		/* for playback stream, fifo(stream) reuse with: capture->fifo or session->proc->result*/

	auSessionDescOps_t 				*ops;
	
}auSession_t;

extern auSessionDesc_t auSessionDesc[AUDIO_SESSION_MAX];

void auSession_suspend(auSession_t* pSession, bool suspend);

#ifdef __cplusplus
}
#endif


#endif  /* __AUDIO_SESSION_H__ */





