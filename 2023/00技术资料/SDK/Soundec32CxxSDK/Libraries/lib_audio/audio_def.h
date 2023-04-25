/** 
***********************************************************
 * 
 * @file name	: audio_def.h
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2022-02-11
 * @brief		: General define of audio system
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
#ifndef __AUDIO_DEF_H__
#define __AUDIO_DEF_H__

/* Includes -----------------------------------------------------------------*/
#include "types.h"
#include <string.h>
#include <stdlib.h>
#include "assert.h"
#include <xtensa/xtruntime.h>
#include <xtensa/config/core.h>
#include "snc8600.h"
#include "boardConfig.h"
#include "hal_dmac.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/

/* Private macro ------------------------------------------------------------*/

#define AU_MNGR_DEBUG(...)						uart_printf(__VA_ARGS__)

#if (DMA_ENABLE)
#define AU_DMAC_I2S_OUT							(APP_DMAC_TEST_MEMORY_TO_I2S)

//#define APP_DMAC_TEST_TYPE		(APP_DMAC_TEST_MEMORY_TO_ADC)

//#define APP_DMAC_TEST_TYPE		(APP_DMAC_TEST_MEMORY_TO_MEMORY)

#endif
	
#define SAMPLING_RATE_8000        			(8000)
#define SAMPLING_RATE_16000        			(16000)
#define SAMPLING_RATE_32000        			(32000)
#define SAMPLING_RATE_44100        			(44100)
#define SAMPLING_RATE_48000        			(48000)
#define SAMPLING_RATE_88200        			(88200)
#define SAMPLING_RATE_96000        			(96000)
#define SAMPLING_RATE_176400       			(176400)
#define SAMPLING_RATE_192000       			(192000)
#define SAMPLING_RATE_352800       			(352800)
#define SAMPLING_RATE_384000       			(384000)

/* Private variables ----------------------------------------------------------*/

/* Private function prototypes --------------------------------------------------*/

/* Private functions ----------------------------------------------------------*/
typedef enum{
	UNMUTE = 0,
	MUTE = 1,
		
}auMute_t;

typedef enum{
	PLAY_STATUS_PLAYING,
	PLAY_STATUS_STOPED,
	PLAY_STATUS_PAUSED,
	
}auPlayStatus_t;
	
typedef enum{
	AUDIO_STREAM_ACTIVE = 0,
	AUDIO_STREAM_IDLE,
	
}auStreamDetect_t;

typedef enum{
	STREAM_MUSIC,
	STREAM_VOICE,
	STREAM_BUILDIN,
	STREAM_MIXER,
	STREAM_MAX,
	
}auStreamType_t;

typedef enum{
	AUDIO_MNGR_SUCCESS 				= 0x00,
	AUDIO_MNGR_FAIL					= 0x01,
	AUDIO_MANAGER_NOT_EXIST 		= 0x02,
	AUDIO_STREAM_NOT_EXIST		 	= 0x04,
	AUDIO_SESSION_NOT_EXIST			= 0x08,
	AUDIO_DEVICE_NOT_EXIST			= 0x10,
	AUDIO_SESSION_CAPTURE_NOT_EXIST	= 0x20,
	AUDIO_SESSION_PLAYBACK_NOT_EXIST	= 0x40,
	AUDIO_SESSION_OPS_NOT_EXIST		= 0x80,
	
}auManagerSt_t;

typedef enum
{
	BIT_SLOT_8 = 1,
	BIT_SLOT_16 = 2,
	BIT_SLOT_24 = 3,
	BIT_SLOT_32 = 4,
	
 }auSlotSize_t;

typedef enum
{
	PCM_PACKED_FORMAT,		// LR LR LR
	PCM_PLANAR_FORMAT,		// LL...LL RR...RR
	PCM_FORMAT_MAX,
	
}auPcmDataFormat_t;

typedef enum{
	/*0x00:noFifo, 0x01:exclusive; 0x02: reused fifo, 0x04: shared fifo*/
	FIFO_NULL = 0x00,
	FIFO_NEW = 0x01,
	FIFO_REUSED = 0x02,
	FIFO_SHARED = 0x03,

}auFifoState_t;

typedef enum{
	CAPTURE_PATH = 0,
	PLAYBACK_PATH,
	SIDETONE_PATH,
	
}auCtrlPath_t;

typedef enum{
	INFO_FORMAT = 0,
	INFO_VOL_LEFT,
	INFO_VOL_RIGHT,
	INFO_VOL_STEREO,
	INFO_MUTE,
	
}auInfoType_t;

typedef enum{
	AUDIO_PLAY,
	AUDIO_STOP,		// after stop, should release player source and route map.
	AUDIO_PAUSE,	// after pause, keep player source and route map.
	AUDIO_RESUME,	
	AUDIO_RECORD,
	
}auPlayerCtrl_t;

typedef enum{
	AU_TP_MUSIC,
	AU_TP_TONE,
	AU_TP_VOICE,

	AU_TP_MAX,
	
}auType_t;

/*
* @brief This enum defines the audio interface of the channel
*/
typedef enum {
	CHANNEL_L = 0,
	CHANNEL_R,
	CHANNEL_STEREO,
	
}auChannel_t;


typedef int8_t(*auTransmitCompleteCB_t)			(void *arg, void *data, uint32_t *len, auSlotSize_t bitSlot);
typedef uint32_t(*fifo_ready_cb_t) 				(uint32_t frameLen, uint8_t pickFifoNum, int32_t **pickFifo, uint8_t pokeFifoNum, int32_t **pokeFifo);	/*fifo_handle_t*/
typedef void (*auUpdateInfoCB_t)				(void *arg, auCtrlPath_t path, auInfoType_t type, void *info);
typedef void (*xtos_handler)					(void *arg);
typedef void (*auCalSwGainCB_t)					(int32_t *gain, int32_t *__restrict in, int32_t *__restrict out);
typedef void (*auDoEqCB_t) 						(int32_t *data ,uint32_t sampleRate);

typedef struct{
	uint8_t 				mute;
	uint8_t 				volumeMax[CHANNEL_STEREO];
	uint8_t 				volumeMin[CHANNEL_STEREO];
	uint8_t 				volume[CHANNEL_STEREO];
	uint8_t 				step;

}auVolume_t;

typedef struct{
	uint32_t 				frameThreshold;				// unit is ms, frameThreshold must <= frameLength
	fifo_ready_cb_t 		onFifoReady;
	IRQn_Type				irqNum;
	bool 					channelCopy; 				//true:Mono to double channel data
}auFifoEvent_t;


// size of frame in byte = channlesPerFrame*bitSlot(bit width in byte)
typedef struct{
	uint8_t 				channlesPerFrame;			// 0 is error
	uint32_t				frameLength;				// unit is number of points,must be divisible by 4
	auPcmDataFormat_t		pcmFormat;
	auSlotSize_t			bitSlot;					// bitwidth for data push & pop
	uint32_t				sampleRateHz;				// Hz
	uint32_t				sampleRateHzMax;			// Hz
	
}auStrmFormat_t;


#ifdef __cplusplus
}
#endif

#endif  /* __AUDIO_DEF_H__ */



