/**
***********************************************************
 *
 * @file name   : Altotool.h
 * @author      : nick
 * @version     : V0.1
 * @date        : 2022-03-00
 * @brief       : Structures defined in protocols
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
 * and Sourced Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/

#ifndef __EQCOMM_H__
#define __EQCOMM_H__

#include <stdio.h>
#include "math.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "common.h"
#include "boardConfig.h"

#if (SUPPORT_ALTOTOOL)

#define TOOL_DATA_STRUCT_DEMO			   1

#define ALTOTOOL_PROTOCLOL_VER              0x010002     /*ALTOTOOL_PROTOCLOL_VER*/    

#define COMM_DEVICE_TYPE_STR				"ALTO EQ V1.0.0     "
#define COMM_DEVICE_TYPE_STR_LEN			20

#define COMM_EQ_BANDS_NUM					10

#define COMM_SOP_STR						{0xAA,0x53,0x4E,0x44,0x43}
#define COMM_SOP_STR_LEN					5


#define COMM_SOP_PKT_LEN					(COMM_SOP_STR_LEN+5)	
#define COMM_PARAM_LEN_MAX				     332 //SPK EQ LENGTH
#define COMM_PKT_LEN_MAX					(COMM_SOP_PKT_LEN+COMM_PARAM_LEN_MAX)

#define COMM_DATA_POS                       10

typedef union{
	U8 data[COMM_PKT_LEN_MAX];
}CommTuneData_t;

typedef void (*pktParseCB2_t)(U8 *data);

typedef union
{
	float fdata;
	U32 ldata;
}floatLongType;

typedef enum {
	MODULE_START_LINK	         = 0x00,
	MODULE_LINK_PARAM			 = 0x10,
	MODULE_ADC                   = 0x30, 
	MODULE_DAC                   = 0x31,  
	MODULE_I2S                   = 0x32,
	MODULE_EQ_SPK                = 0x40,
	MODULE_EQ_MIC                = 0x41,
	MODULE_VOL                   = 0x50, /**/
	MODULE_AGC                   = 0x51, 
	MODULE_DRC                   = 0x52,
	MODULE_AEC                   = 0x53,
	MODULE_DEV                   = 0x54, /*dereverb config*/
	MODULE_DEN                   = 0x55, /*noise canceller*/
	MODULE_OTH                   = 0x56, /*algorithm setting*/
	MODULE_EARPHONE				 = 0X57,
	MODULE_DEBUG                 = 0x70, /*debug setting*/
	MODULE_MAX                   = 0xF0,

}major_module_t;

typedef enum {
	CMD_DLINK_START                = 0x81,
	CMD_DLINK_READ_PVERSION,
	CMD_DLINK_READ_HSVERSION,
	CMD_DLINK_COMPLETE,
	CMD_DLINK_READ_ID,
	CMD_DLINK_KEEP_DLINKING,
	CMD_DLINK_CONFIG_TYPE,
	CMD_DLINK_DSP_STATUS           = 0x88,
}db_info_t;   /*alto chip device base info sncy */


typedef enum{
	CMD_LINK_PARAM_START = 0x80,
	CMD_LINK_PARAM_ADC,
	CMD_LINK_PARAM_DAC,
	CMD_LINK_PARAM_I2S,
	CMD_LINK_PARAM_SPK_EQ_NUMS,
	CMD_LINK_PARAM_SPK_EQ,
	CMD_LINK_PARAM_MIC_EQ_NUMS,
	CMD_LINK_PARAM_MIC_EQ,
	CMD_LINK_PARAM_ALG,
	CMD_LINK_PARAM_ALL,
	CMD_LINK_PARAM_END,
}cmd_link_param_t;


typedef enum{
	COMM_STATE_IDLE = 0,
	COMM_STATE_SOP,
	COMM_STATE_MODULE,
	COMM_STATE_REV,
	COMM_STATE_CMD,
	COMM_STATE_DATALEN_H,
	COMM_STATE_DATALEN_L,
	COMM_STATE_DATAS,
	COMM_STATE_CHECKSUM_ALL,
	COMM_STATE_PROC_PKT,
}comm_st_t;


typedef struct{
	bool isInited;
	bool isBusy;
	bool linked;
	bool loading;
	comm_st_t procState;
	const U8 *sopStr;
	const char *devTypeStr;
	U8 module;
	U8 rev_ch;
	U8 cmd;
	U32 Handshake;

	CommTuneData_t txPkt;
	U16 txLen;
	CommTuneData_t rxPkt;
	U16 rxLen;
	bool isEqReady[2];

	pktParseCB2_t pktParseCB;
}comm_para_t;

extern comm_para_t altotoolPara;

//#if(TOOL_DATA_STRUCT_DEMO)
typedef enum{
	ADC_12 = 0,
	ADC_3456 = 1,
	ADC_34 = 1,
	ADC_56 = 2,
	ADC_1 = 0,
	ADC_2 = 1,
	ADC_3 = 2,
	ADC_4 = 3,
	ADC_5 = 4,
	ADC_6 = 5,
	ADC_7 = 6,
	ADC_8 = 7,
	ADC_9 = 8,
	ADC_10 = 9,

	DAC_L = 0,
	DAC_R = 1,

	I2S_1 = 0,
	I2S_2 = 1,
	I2S_3 = 2,
}tool_num_t;

typedef struct{
	bool aias;
	uint8_t wordLength;
	uint8_t mixadc1;//only adc12
	uint8_t mixadc2;//only adc12
	uint32_t sr;
}codec_adc_12_3456_t;

typedef struct{
	bool en;
	bool mute;
	bool amic; //0-dmic 1-amic for adc12 only	
	int8_t gim;//only adc12
	int8_t gid;
	uint8_t aiadc;
}codec_adc_chx_cfg_t;

typedef struct{
	bool agcEn;
	bool stereo;
	bool ngEn;
	bool snrOptEn;
	uint8_t target;
	uint8_t holdTime;
	uint8_t atkTime;
	uint8_t dcyTime;
	uint8_t ngThr;
	uint8_t minGain;
	uint8_t maxGain;
	uint8_t rev;
}codec_adc_agc_cfg_t;

typedef struct{
	bool hpf;;
	bool mixrec;//only adc12
	uint8_t wnf;
	uint8_t reserved;
	codec_adc_agc_cfg_t agcCfg;
}codec_adc_pair_cfg_t;

typedef struct{
	codec_adc_12_3456_t gCfg[2];
	codec_adc_chx_cfg_t chxCfg[6];
	codec_adc_pair_cfg_t pairCfg[3];
}codec_adc_cfgs_t;

typedef struct{
	bool mute;
	int8_t god;
	int8_t gom;
	int8_t gomix;
	int8_t gimix;
	uint8_t mixdac;
	uint8_t aidac;
	uint8_t drcThr;
	uint8_t drcComp;
	uint8_t rev[3];
}codec_dac_chx_cfg_t;

typedef struct{
	bool aias;
	bool mix;
	bool drcEn;
	uint8_t wordLength;
	uint32_t sr;
	bool enable;
	uint8_t rev[3];

	codec_dac_chx_cfg_t chxCfg[2];
}codec_dac_cfgs_t;

typedef struct{
	bool en;
	bool txEn;
	bool rxEn;
	bool isMaster;
	bool mute;
	uint8_t format;
	uint8_t dataWidth;
	uint8_t frameLength;//
	uint32_t sr;
}i2s_chx_cfg_t;

typedef struct{
	i2s_chx_cfg_t chxCfg[3];
}i2s_cfgs_t;

typedef struct{
	uint32_t sr;
	float target;
	float minGain;
	float maxGain;
	float holdTime;
	float atkTime;
	float rlsTime;
	float thr;
}alg_agc_t;
typedef enum{
	DSP_GENERAL = 1,
	USB_HEADSET_AMIC = 2,
	USB_CONFERENCE_PHONE_AMIC = 3,
	BT_NC_HEADSET_AMIC = 4,
	USB_NC_HEADSET_AMIC = 5,
	USB_HEADSET_DMIC = 6,
	BT_NC_HEADSET_DMIC = 8,
	USB_NC_HEADSET_DMIC = 9,	
}product_config_t;

typedef struct{
	uint32_t sr;
	float ratio;
	float atkTime;
	float rlsTime;
	float kneeWidth;
	float thr;
}alg_drc_t;

typedef struct{
	float depth;
	float convergenceRate;
}alg_aec_t;

typedef struct{
	uint8_t rate;
	uint8_t rev[3];
	float str;
}alg_dereverb_t;

typedef struct{
	float vol;
	float depth;
}alg_ep_dn_t;

typedef struct{
	bool volCtrlEn;
	bool agcEn;
	bool drcEn;
	bool aecEn;
	bool dereverbEn;
	bool ncEn;
	bool bfEn;
	bool doaEn;
	bool spkEqEn;
	bool micEqEn;
	bool referenceEn;
	uint8_t rev;
	int8_t vol[12];
	alg_agc_t agc;
	alg_drc_t drc;
	alg_aec_t aec;
	alg_dereverb_t dereverb;
	float ncDepth;
	alg_ep_dn_t epDn;
}alg_cfgs_t;

extern codec_adc_cfgs_t gAdcCfg;
extern codec_dac_cfgs_t gDacCfg;
extern i2s_cfgs_t gI2sCfg;
extern alg_cfgs_t gAlgCfg;
//#endif



void altotool_profile_param_init(void);
void altotool_polling(void);

float byteToFloat(U32 bytes);
float parsePacket(U8 *data);
uint16_t getProductConfigType();
void getHSVersion(U8 *hsVersion);
void dump_buffer(U8* buffer);
#endif /*SUPPORT_ALTOTOOL*/

#endif
