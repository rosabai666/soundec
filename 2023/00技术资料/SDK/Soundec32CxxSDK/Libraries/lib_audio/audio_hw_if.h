/** 
***********************************************************
 * 
 * @file name	: audio_hw_if.h
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2022-02-10
 * @brief		: Inlude the hw configurations of audio 
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
#ifndef __AUDIO_HW_IF_H__
#define __AUDIO_HW_IF_H__

/* Includes ----------------------------------------------------------------*/
#include "common.h"
#include "audio_def.h"
#if (USB_ENABLE)	
#include "usb_def.h"
#endif
//#include "audio_stream.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef ----------------------------------------------------------*/

/* Private macro -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
typedef enum{
#if (CODEC_ADC_ENABLE)
	AUDIO_DEVICE_ADC_IN,
#endif

#if (CODEC_ADC12_ENABLE)
	AUDIO_DEVICE_MEMS12_IN,
#endif
#if (CODEC_ADC34_ENABLE)
	AUDIO_DEVICE_MEMS34_IN,
#endif
#if (CODEC_ADC56_ENABLE)
	AUDIO_DEVICE_MEMS56_IN,
#endif
#if (CODEC_ADC78_ENABLE)
	AUDIO_DEVICE_MEMS78_IN,
#endif
#if (CODEC_ADC9A_ENABLE)
	AUDIO_DEVICE_MEMS9A_IN,
#endif

#if (CODEC_DAC_ENABLE)
	AUDIO_DEVICE_CODEC_OUT,
#endif

#if (I2S1_ENABLE)
	AUDIO_DEVICE_I2S1,
#endif
#if (I2S2_ENABLE)
	AUDIO_DEVICE_I2S2,
#endif
#if (I2S3_ENABLE)
	AUDIO_DEVICE_I2S3,
#endif

#if (USB_ENABLE)	
	AUDIO_DEVICE_USB,
#endif

	//AUDIO_DEVICE_BUILD_IN,
	AUDIO_DEVICE_MAX,
	AUDIO_DEVICE_NULL = AUDIO_DEVICE_MAX,
	
}auDeviceId_t;

typedef struct{
	void 						(*setSampleRate)(uint32_t sampleRate);
	void 						(*tuneSampleRate)(uint32_t dataLen, uint32_t fifoSize);//(float s, int32_t dir);
	auSlotSize_t				(*transforBitSlot)(auSlotSize_t bitSlot);
	drv_status_t				(*setGain)(int8_t gain, uint8_t isAgain, auChannel_t lr);
	void 						(*setSwGain)(int32_t *gain, int32_t *__restrict in, int32_t *__restrict out);
	drv_status_t				(*setMute)(bool mute);
	auStreamDetect_t			(*detCapture)(int32_t *micDat, uint32_t datThr, uint32_t *cal, auStrmFormat_t *format);
	auStreamDetect_t 			(*detPlayback)(int32_t *spkDat, uint32_t datThr, uint32_t *cal, auStrmFormat_t *format);

}auStreamOps_t;

typedef struct{
	uint32_t					sampleRateHzMax;			// Hz
	auSlotSize_t				wordSize;					// bitwidth of store format: 16bit,24bit,32bit
	uint8_t						frameLevel;
	uint8_t						frameLength;
	uint8_t 					channlesNumMax;				// 0 is error

}auDevFormatCapacity_t;

typedef union{
#if (USB_ENABLE)	
	uac_spec_t					uacSpec;
#endif
#if (CODEC_ENABLE)
	hal_codec_ai_mode_t			codecRole;
#endif
	uint8_t						masterMode;

}auDevRole_t;

typedef struct{
	auDevFormatCapacity_t		*formatCap;
	auTransmitCompleteCB_t 		isrCb;
	auUpdateInfoCB_t			updateInfoCb;
	auCalSwGainCB_t				calSwGainCb;
	auDoEqCB_t					doEqCb;
	bool						useSwGain;
	bool 						enable;						/* Default is 0: unenable, if enable = 1: register callback and enable corresponding isr or DMA. */
	bool 						useDMA;
	dmac_channel_num_t 			dmaChannel;
	auStreamOps_t				*ops;
	void 						*pStream;
	void 						*pUserData;					/* Reserved for multi-stream in one route */
	
}auDevAttr_t;

typedef struct{
	auStrmFormat_t 				*format;
	auDevAttr_t					*attr;

}auDriverConfig_t;	

typedef struct{	
	bool 						isInitialized;
	auMute_t 					(*get_mute)();
	auPlayStatus_t 				(*getPlayStatus)();
	uint32_t 					(*getCaptureSampleRate)();
	uint32_t 					(*getPlaybackSampleRate)();
	int8_t 						(*getGain)(uint8_t isAgain, auChannel_t lr);

}auDriverState_t;

typedef struct{
	auDevRole_t					role;
	auDriverConfig_t			*rxCfgs;
	auDriverConfig_t			*txCfgs;
	
}auDriverParam_t;

typedef struct{
	drv_status_t 				(*init)(auDriverConfig_t *capture, auDriverConfig_t *playback);
	drv_status_t 				(*deInit)(auDriverConfig_t *capture, auDriverConfig_t *playback);
	drv_status_t 				(*reInit)(auDriverConfig_t *capture, auDriverConfig_t *playback);
	drv_status_t 				(*start)();
	drv_status_t 				(*stop)();
	drv_status_t 				(*lowPowerMode)();
	drv_status_t 				(*wakeup)();

}auDriverOps_t;

typedef struct{
	auDeviceId_t 				id; 	/*Index of device list*/
	TBool						load;	/*If this device has been allocated to any stream, set this load flag to 1*/
	const char					*name;
	auDriverConfig_t			*rxCfgs;
	auDriverConfig_t			*txCfgs;
	auDriverState_t				*state;
	auDriverOps_t				*ops;
//	void 						*userData;

}audioDevIf_t;

typedef struct{
	bool 						initialized;
	bool 						enable;
	auDevRole_t 				role;
	auSlotSize_t 				bitSlot;
	auSlotSize_t 				wordSize;
	uint8_t 					channel;
	uint32_t 					sampleRate;
	uint8_t 					useSwGain;
	auCalSwGainCB_t 			calSwGainCb;
	auTransmitCompleteCB_t 		isrCb;
	auDevFormatCapacity_t 		*formatCap;
	void 						*cbArg;
	auDoEqCB_t 					doEqCb;
	uint32_t                    dataLen;
}auDevInfos_t;


extern audioDevIf_t auDeviceIfs[AUDIO_DEVICE_MAX];
extern auManagerSt_t auDevive_init(void);


#ifdef __cplusplus
}
#endif

#endif  /* __AUDIO_HW_IF_H__ */

             




