/**
***********************************************************
 *
 * @file name   : altotool.c
 * @author      : nick
 * @version     : V0.1
 * @date        : 2022-03-06
 * @brief       : This routine used to implement the communication with
 *                Mic EQ tool. Default use uart interface. The protocol format
 *			      is compatible with USB HID
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
#include "sys_init.h"
#include "drv_uart.h"
#include "hal_uart.h"
#include "userFlash.h"
#include "altotool.h"
#include "dsp_eq.h"
#include "hid_parser.h"
#include "drv_codec.h"
#include "equalizer.h"
#include "Altotool_codec.h"
#include "Altotool_i2s.h"
#include "altotool_comm.h"
#include "Altotool_eq.h"
#include "altotool_debug.h"
#include "usbd_config.h"

#if (SUPPORT_ALTOTOOL)

#define HID_COMMUNICATION_PROTOCOL_SUPPORT_DEBUG

extern void populateAdcParameters(void);
extern void populateDacParameters(void);
extern void populateAlgoParameters(void);
extern void populateI2SParameters(void);
extern void resetI2SParameters(void);
extern void resetAdcParameters(void);


#if(TOOL_DATA_STRUCT_DEMO)
codec_adc_cfgs_t gAdcCfg = {
	.gCfg[ADC_12] = {
		.aias = false,
		.wordLength = HAL_CODEC_ADWL_24BIT,
		.mixadc1 = HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
		.mixadc2 = HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
		.sr = SAMPLING_RATE_48000,
	},
	.gCfg[ADC_3456] = {
		.aias = false,
		.wordLength = HAL_CODEC_ADWL_24BIT,
		.mixadc1 = HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
		.mixadc2 = HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
		.sr = SAMPLING_RATE_48000,
	},
	.chxCfg[ADC_1] = {
		.en = true,
		.mute = false,
		.gim = HAL_CODEC_GIM_0DB,
		.gid = 0,
		.aiadc = HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	},
	.chxCfg[ADC_2] = {
		.en = true,
		.mute = false,
		.gim = HAL_CODEC_GIM_0DB,
		.gid = 0,
		.aiadc = HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	},
	.chxCfg[ADC_3] = {
		.en = false,
		.mute = false,
		.gim = 0,
		.gid = 12,
		.aiadc = HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	},
	.chxCfg[ADC_4] = {
		.en = false,
		.mute = false,
		.gim = 0,
		.gid = 12,
		.aiadc = HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	},
	.chxCfg[ADC_5] = {
		.en = false,
		.mute = false,
		.gim = 0,
		.gid = 12,
		.aiadc = HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	},
	.chxCfg[ADC_6] = {
		.en = false,
		.mute = false,
		.gim = 0,
		.gid = 12,
		.aiadc = HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	},
	.pairCfg[ADC_12] = {
		.hpf = false,
		.mixrec = HAL_CODEC_ADC_MIXER_MODE_RECORD_ONLY,
		.wnf = HAL_CODEC_WNF_OFF,
		.agcCfg = {
			.agcEn = false,
			.stereo = false,
			.ngEn = false,
			.snrOptEn = false,
			.target = HAL_CODEC_AGC_TARGET_MAX_MINUS_19P5DB,
			.holdTime = HAL_CODEC_AGC_HOLD_TIME_128MS,
			.atkTime = HAL_CODEC_AGC_ATTACK_TIME_160MS,
			.dcyTime = HAL_CODEC_AGC_DECAY_TIME_160MS,
			.ngThr = HAL_CODEC_AGC_NG_THR_MIN,
			.minGain = HAL_CODEC_AGC_GAIN_21DB,
			.maxGain = HAL_CODEC_AGC_GAIN_42P5DB,
		},
	},
	.pairCfg[ADC_34] = {
		.hpf = false,
		.mixrec = HAL_CODEC_ADC_MIXER_MODE_RECORD_ONLY,
		.wnf = HAL_CODEC_WNF_OFF,
		.agcCfg = {
			.agcEn = false,
			.stereo = false,
			.ngEn = false,
			.snrOptEn = false,
			.target = HAL_CODEC_AGC_TARGET_MAX_MINUS_19P5DB,
			.holdTime = HAL_CODEC_AGC_HOLD_TIME_128MS,
			.atkTime = HAL_CODEC_AGC_ATTACK_TIME_160MS,
			.dcyTime = HAL_CODEC_AGC_DECAY_TIME_160MS,
			.ngThr = HAL_CODEC_AGC_NG_THR_MIN,
			.minGain = HAL_CODEC_AGC_GAIN_21DB,
			.maxGain = HAL_CODEC_AGC_GAIN_42P5DB,
		},
	},
	.pairCfg[ADC_56] = {
		.hpf = false,
		.mixrec = HAL_CODEC_ADC_MIXER_MODE_RECORD_ONLY,
		.wnf = HAL_CODEC_WNF_OFF,
		.agcCfg = {
			.agcEn = false,
			.stereo = false,
			.ngEn = false,
			.snrOptEn = false,
			.target = HAL_CODEC_AGC_TARGET_MAX_MINUS_19P5DB,
			.holdTime = HAL_CODEC_AGC_HOLD_TIME_128MS,
			.atkTime = HAL_CODEC_AGC_ATTACK_TIME_160MS,
			.dcyTime = HAL_CODEC_AGC_DECAY_TIME_160MS,
			.ngThr = HAL_CODEC_AGC_NG_THR_MIN,
			.minGain = HAL_CODEC_AGC_GAIN_21DB,
			.maxGain = HAL_CODEC_AGC_GAIN_42P5DB,
		},
	},
};

codec_dac_cfgs_t gDacCfg = {
	.aias = false,
	.mix = HAL_CODEC_PLAYBACK_DAC_ONLY,
	.drcEn = false,
	.wordLength = HAL_CODEC_ADWL_24BIT,
	.sr = SAMPLING_RATE_48000,
	.enable = true,
	.chxCfg[DAC_L] = {
		.mute = false,
		.god = 0,
		.gom = 0,
		.gomix = 0,
		.gimix = 0,
		.mixdac = HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
		.aidac = HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
		.drcThr = 0,
		.drcComp = HAL_CODEC_DRC_RATE_1,
	},
	.chxCfg[DAC_R] = {
		.mute = false,
		.god = 0,
		.gom = 0,
		.gomix = 0,
		.gimix = 0,
		.mixdac = HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
		.aidac = HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
		.drcThr = 0,
		.drcComp = HAL_CODEC_DRC_RATE_1,
	},
};

i2s_cfgs_t gI2sCfg = {
	.chxCfg[I2S_1] = {
		.en = true,
		.txEn = true,
		.rxEn = true,
		.isMaster = true,
		.mute = false,
		.format = I2S_STANDARD_PHILIPS,
		.dataWidth = I2S_FIFO_DATALENGTH_32B,
		.frameLength = I2S_FRAMELENGTH_32B,
		.sr = SAMPLING_RATE_48000,
	},
	.chxCfg[I2S_2] = {
		.en = true,
		.txEn = true,
		.rxEn = true,
		.isMaster = true,
		.mute = false,
		.format = I2S_STANDARD_PHILIPS,
		.dataWidth = I2S_FIFO_DATALENGTH_32B,
		.frameLength = I2S_FRAMELENGTH_32B,
		.sr = SAMPLING_RATE_48000,
	},
	.chxCfg[I2S_3] = {
		.en = true,
		.txEn = true,
		.rxEn = true,
		.isMaster = true,
		.mute = false,
		.format = I2S_STANDARD_PHILIPS,
		.dataWidth = I2S_FIFO_DATALENGTH_32B,
		.frameLength = I2S_FRAMELENGTH_32B,
		.sr = SAMPLING_RATE_48000,
	},
};

alg_cfgs_t gAlgCfg = {
	.volCtrlEn = false,
	.agcEn = false,
	.drcEn = false,
	.aecEn = false,
	.dereverbEn = false,
	.ncEn = false,
	.bfEn = false,
	.doaEn = false,
	.spkEqEn = false,
	.micEqEn = false,
	.referenceEn = false,

	.vol[ADC_1]=0,
	.vol[ADC_2]=0,
	.vol[ADC_3]=0,
	.vol[ADC_4]=0,
	.vol[ADC_5]=0,
	.vol[ADC_6]=0,
	.vol[6+DAC_L]=0,
	.vol[6+DAC_R]=0,

	.agc = {
		.sr = SAMPLING_RATE_48000,
		.target = 0,
		.minGain = -1,
		.maxGain = 20,
		.holdTime = 10,
		.atkTime = 100,
		.rlsTime = 100,
		.thr = -40,
	},
	.drc= {
		.sr = SAMPLING_RATE_48000,
		.ratio = 1,
		.atkTime = 0,
		.rlsTime = 0,
		.kneeWidth = 0,
		.thr = 0,
	},
	.aec = {
		.depth = 1,
		.convergenceRate = 0.05,
	},
	.dereverb = {
		.rate = 0,
		.str = 0,
	},
	.ncDepth = 0.5,
	.epDn = {
		.vol = 12,
		.depth = 1.5,
	},
};

const eq_chx_t spkEqBase = {
	.eqNum =   1,
	.stereo =  false,
	.bypass =  false,
	.gainBeforeEq =    0,
	.gainBeforeEq_R =	0,
	.gainBeforeEqType =    0,
	.band[0] = {												  
		.bandEn =	 true,											  
		.eqType =	 GCORE_TYPE_BIQUAD_FILTER_HIGH_PASS, 											  
		.qbw = 	 1, 											  
		.fc =		 20,											  
		.gain =	 0, 											  
	 }, 														  
	.band[1] = {												  
		 .bandEn =	 true,											  
		 .eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		 .qbw = 	 1, 											  
		 .fc =		 120,											  
		 .gain =	 0, 											  
	 }, 														  
	.band[2] = {												  
		.bandEn =	 true,											  
		.eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		.qbw = 	 0.2, 											  
		.fc =		 150,											  
		.gain =	 0, 											  
	 }, 														  
	.band[3] = {												  
		 .bandEn =	 true,											  
		 .eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		 .qbw = 	 0.5, 											  
		 .fc =		 220,											  
		 .gain =	 0, 											  
	 }, 															  
	.band[4] = {												  
		.bandEn =	 true,											  
		.eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		.qbw = 	 0.2, 											  
		.fc =		 300,											  
		.gain =	 0, 											  
	 }, 														  
	.band[5] = {												  
		 .bandEn =	 true,											  
		 .eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		 .qbw = 	 2, 											  
		 .fc =		 800,											  
		 .gain =	 0, 											  
	 }, 																									  
	.band[6] = {												  
		.bandEn =	 true,											  
		.eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		.qbw = 	 0.2, 											  
		.fc =		 1000,											  
		.gain =	 0, 											  
	 }, 														  
	.band[7] = {												  
		 .bandEn =	 true,											  
		 .eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		 .qbw = 	 2, 											  
		 .fc =		 1500,											  
		 .gain =	 0, 											  
	 }, 	
	.band[8] = {												  
		.bandEn =	 true,											  
		.eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		.qbw =	 0.2, 											  
		.fc =		 4000,											  
		.gain =  0, 											  
	 }, 														  
	.band[9] = {												  
		 .bandEn =	 true,											  
		 .eqType =	 GCORE_TYPE_BIQUAD_FILTER_LOW_PASS, 											  
		 .qbw = 	 1, 											  
		 .fc =		 8000,											  
		 .gain =	 0, 											  
	 },
	.bandStereoR[0] = {												  
		.bandEn =	 true,											  
		.eqType =	 GCORE_TYPE_BIQUAD_FILTER_HIGH_PASS, 											  
		.qbw = 	 1, 											  
		.fc =		 20,											  
		.gain =	 0, 											  
	 }, 														  
	.bandStereoR[1] = {												  
		 .bandEn =	 true,											  
		 .eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		 .qbw = 	 1, 											  
		 .fc =		 120,											  
		 .gain =	 0, 											  
	 }, 														  
	.bandStereoR[2] = {												  
		.bandEn =	 true,											  
		.eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		.qbw = 	 0.2, 											  
		.fc =		 150,											  
		.gain =	 0, 											  
	 }, 														  
	.bandStereoR[3] = {												  
		 .bandEn =	 true,											  
		 .eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		 .qbw = 	 0.5, 											  
		 .fc =		 220,											  
		 .gain =	 0, 											  
	 }, 															  
	.bandStereoR[4] = {												  
		.bandEn =	 true,											  
		.eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		.qbw = 	 0.2, 											  
		.fc =		 300,											  
		.gain =	 0, 											  
	 }, 														  
	.bandStereoR[5] = {												  
		 .bandEn =	 true,											  
		 .eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		 .qbw = 	 2, 											  
		 .fc =		 800,											  
		 .gain =	 0, 											  
	 }, 																									  
	.bandStereoR[6] = {												  
		.bandEn =	 true,											  
		.eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		.qbw = 	 0.2, 											  
		.fc =		 1000,											  
		.gain =	 0, 											  
	 }, 														  
	.bandStereoR[7] = {												  
		 .bandEn =	 true,											  
		 .eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		 .qbw = 	 2, 											  
		 .fc =		 1500,											  
		 .gain =	 0, 											  
	 }, 	
	.bandStereoR[8] = {												  
		.bandEn =	 true,											  
		.eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		.qbw =	 0.2, 											  
		.fc =		 4000,											  
		.gain =  0, 											  
	 }, 														  
	.bandStereoR[9] = {												  
		 .bandEn =	 true,											  
		 .eqType =	 GCORE_TYPE_BIQUAD_FILTER_LOW_PASS, 											  
		 .qbw = 	 1, 											  
		 .fc =		 8000,											  
		 .gain =	 0, 											  
	 },
};

const eq_chx_t micEqBase = {
	.eqNum =   1,
	.stereo =  false,
	.bypass =  false,
	.gainBeforeEq =    0,
	.gainBeforeEq_R =	0,
	.gainBeforeEqType =    0,
	.band[0] = {												  
		.bandEn =	 true,											  
		.eqType =	 GCORE_TYPE_BIQUAD_FILTER_HIGH_PASS, 											  
		.qbw = 	 1, 											  
		.fc =		 20,											  
		.gain =	 0, 											  
	 }, 														  
	.band[1] = {												  
		 .bandEn =	 true,											  
		 .eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		 .qbw = 	 1, 											  
		 .fc =		 120,											  
		 .gain =	 0, 											  
	 }, 														  
	.band[2] = {												  
		.bandEn =	 true,											  
		.eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		.qbw = 	 0.2, 											  
		.fc =		 150,											  
		.gain =	 0, 											  
	 }, 														  
	.band[3] = {												  
		 .bandEn =	 true,											  
		 .eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		 .qbw = 	 0.5, 											  
		 .fc =		 220,											  
		 .gain =	 0, 											  
	 }, 															  
	.band[4] = {												  
		.bandEn =	 true,											  
		.eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		.qbw = 	 0.2, 											  
		.fc =		 300,											  
		.gain =	 0, 											  
	 }, 														  
	.band[5] = {												  
		 .bandEn =	 true,											  
		 .eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		 .qbw = 	 2, 											  
		 .fc =		 800,											  
		 .gain =	 0, 											  
	 }, 																									  
	.band[6] = {												  
		.bandEn =	 true,											  
		.eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		.qbw = 	 0.2, 											  
		.fc =		 1000,											  
		.gain =	 0, 											  
	 }, 														  
	.band[7] = {												  
		 .bandEn =	 true,											  
		 .eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		 .qbw = 	 2, 											  
		 .fc =		 1500,											  
		 .gain =	 0, 											  
	 }, 	
	.band[8] = {												  
		.bandEn =	 true,											  
		.eqType =	 GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING, 											  
		.qbw =	 0.2, 											  
		.fc =		 4000,											  
		.gain =  0, 											  
	 }, 														  
	.band[9] = {												  
		 .bandEn =	 true,											  
		 .eqType =	 GCORE_TYPE_BIQUAD_FILTER_LOW_PASS, 											  
		 .qbw = 	 1, 											  
		 .fc =		 8000,											  
		 .gain =	 0, 											  
	 }
};
															  

#endif

comm_para_t altotoolPara;
const U8 COMM_sopStr[COMM_SOP_STR_LEN] = COMM_SOP_STR;
const char COMM_devType[COMM_DEVICE_TYPE_STR_LEN] = COMM_DEVICE_TYPE_STR;

U8 protV[4] = {     /*data structure versiom*/
	ALTOTOOL_PROTOCLOL_VER&0xff,              /*Protocol Major Version*/
	(ALTOTOOL_PROTOCLOL_VER >> 8  )&0xff,     /*Protocol Minor Version*/
	(ALTOTOOL_PROTOCLOL_VER >> 16 )&0xff,     /*Protocol Draft Version*/
	(ALTOTOOL_PROTOCLOL_VER >> 24 )&0xff,
};

U8 softwareV[3] = {      /*Device Software version*/
	CONFIG_PRODUCT_VERSION&0xff,
	(CONFIG_PRODUCT_VERSION >>8 )&0xff,
	0,                                           /*Project config type*/
};
U8 hardwareV[3] = {  /*Device hardware version*/
	BOARD_VER &0xff,
	(BOARD_VER >>8 )&0xff,
	CONFIG_CHIP_TYPE,                              /*chip type*/
};

float byteToFloat(U32 bytes)
{
	U32_t u32 ={.dWords = bytes};
	float fData;
	floatLongType fd1;
	fd1.ldata = 0;
	fd1.ldata = u32.byte[3];
	fd1.ldata = (fd1.ldata<<8)|u32.byte[2];
	fd1.ldata = (fd1.ldata<<8)|u32.byte[1];
	fd1.ldata = (fd1.ldata<<8)|u32.byte[0];

	fData = fd1.fdata;
	return fData;
}

U8 altotool_checksum(U8 *data, U16 len)
{
	U16 i = 0;
	U8 checkSum = 0;
	while(i++<len) checkSum+=*data++;
	checkSum = ~checkSum +1;
	return checkSum;
}

void micEqProfile_sendData(U8 *pdata, U16 len)
{
    #ifdef HID_COMMUNICATION_PROTOCOL_SUPPORT_DEBUG
	hid_CommProtocol_tx_input(pdata, len);
    #endif
	//uart_tx_array(pdata, len);
}


//////////////////////////////////////////////////
void buildSopPacket(U8 **pkt, U16 *pktLen)
{
	memcpy(*pkt, COMM_sopStr, COMM_SOP_STR_LEN);
	*pkt += COMM_SOP_STR_LEN;
	*((*pkt)++) = altotoolPara.module;
	*((*pkt)++) = altotoolPara.rev_ch;
	*((*pkt)++) = altotoolPara.cmd & 0x7f;

	*pktLen += (COMM_SOP_STR_LEN + 3);
}

void buildAckPackect(U8 *pkt, U16 *pktLen, U8 *data, U16 datLen)
{
	U8 *pTxBuf = pkt;

	*pktLen = 0;
	buildSopPacket(&pTxBuf, pktLen);

	*pTxBuf++ = (U8)(datLen);
	*pTxBuf++ = (U8)(datLen  >> 8);
	*pktLen  += 2 ;
	if(datLen && (data != NULL))
	{
		memcpy(pTxBuf, data, datLen);
		*pktLen += datLen;
		pTxBuf  += datLen;
	}
	*pTxBuf = altotool_checksum(pkt, *pktLen);
	*pktLen += 1;
}



uint8_t alto_device_base_info_link(uint8_t cmd)
{
	U8 *pkt = altotoolPara.txPkt.data;
    uint8_t ret = 0;
    U16 type;
    //U8 status[2];

	switch(cmd)
	{
		case CMD_DLINK_START:
			break;
		case CMD_DLINK_READ_PVERSION:
			buildAckPackect(pkt, &altotoolPara.txLen, protV, 3);
			//micEqProfile_sendData(altotoolPara.txPkt.data, altotoolPara.txLen);
			ret = 1;
			break;
		case CMD_DLINK_READ_HSVERSION:
		{
			U8 hsVersion[6];
			getHSVersion(hsVersion);
			buildAckPackect(pkt, &altotoolPara.txLen, hsVersion, sizeof(hsVersion));
			//micEqProfile_sendData(altotoolPara.txPkt.data, altotoolPara.txLen);
			ret = 1;
		}
			break;
		case CMD_DLINK_COMPLETE:
			break;
		case CMD_DLINK_READ_ID:
			break;
		case CMD_DLINK_KEEP_DLINKING:
			break;
		case CMD_DLINK_CONFIG_TYPE:
			type = getProductConfigType();   
			uart_printf("get config type:%d\n", type);
			buildAckPackect(pkt, &altotoolPara.txLen, (U8 *)(&type),2);
			uart_printf("get cfg type,  cmd len:%d\n", altotoolPara.txLen);
			ret = 1;
			break;
		case CMD_DLINK_DSP_STATUS:
 #if (MEMORY_SPACE_MONITOR && CPU_USAGE_DEBUG)
			status[0] = get_cpu_usage();
			status[1] = 30;
			ret = 1;
			buildAckPackect(pkt, &altotoolPara.txLen, status,2);
			uart_printf("get DSP status,  cmd len:%d\n", altotoolPara.txLen);
#endif
			break;
		default:
			break;
	}
	if(ret)
	{
		micEqProfile_sendData(altotoolPara.txPkt.data, altotoolPara.txLen);
	}
	return ret;
}
uint8_t alto_device_module_info_link(uint8_t cmd)
{
	U8 *pkt = altotoolPara.txPkt.data;
	uint8_t status = 0 , ret = 1;
	uint16_t txLen = 0;

	switch(cmd)
	{
	case CMD_LINK_PARAM_START:
		ret = 0;
		break;
#if (SUPPORT_ALTOTOOL_CODEDC)
	case CMD_LINK_PARAM_ADC:
		gAdcCfg.chxCfg[ADC_1].amic = gAdcCfg.chxCfg[ADC_2].amic = ADC12_IS_AMIC;
		buildAckPackect(pkt, &altotoolPara.txLen, (U8*)&gAdcCfg, sizeof(gAdcCfg));
		break;
	case CMD_LINK_PARAM_DAC:
		buildAckPackect(pkt, &altotoolPara.txLen, (U8*)&gDacCfg, sizeof(gDacCfg));
		break;
#endif
#ifdef SUPPORT_ALTOTOOL_I2S
	case CMD_LINK_PARAM_I2S:
		buildAckPackect(pkt, &altotoolPara.txLen, (U8*)&gI2sCfg, sizeof(gI2sCfg));
			break;
#endif
#if (SUPPORT_DSP_EQ)
#if (SUPPORT_SPKER_EQ)
	case CMD_LINK_PARAM_SPK_EQ_NUMS:
		status = SUPPORT_SPKER_EQ;
		buildAckPackect(pkt, &altotoolPara.txLen, &status, 1);
		break;
	case CMD_LINK_PARAM_SPK_EQ:
		gEqCfg.spkEq.eqNum = SUPPORT_SPKER_EQ;
		buildAckPackect(pkt, &txLen, (U8*)&(gEqCfg.spkEq), sizeof(gEqCfg.spkEq));
		uart_printf("altotoolPara.txLen = %d , spkeq sizeof:%d\r\n",txLen,sizeof(gEqCfg.spkEq));
		altotoolPara.txLen = txLen;
		break;
#endif
#if (SUPPORT_MIC_EQ)
	case CMD_LINK_PARAM_MIC_EQ_NUMS:
		status = SUPPORT_MIC_EQ;
		gEqCfg.micEq.eqNum = SUPPORT_MIC_EQ;
		buildAckPackect(pkt, &altotoolPara.txLen, &status, 1);
		break;
	case CMD_LINK_PARAM_MIC_EQ:
		buildAckPackect(pkt, &txLen, (U8*)&(gEqCfg.micEq), sizeof(gEqCfg.micEq));
		altotoolPara.txLen = txLen;
			break;
#endif
#endif
#ifdef SUPPORT_ALTOTOOL_COMM
		case CMD_LINK_PARAM_ALG:
		buildAckPackect(pkt, &txLen, (U8*)&gAlgCfg, sizeof(gAlgCfg));
		altotoolPara.txLen = txLen;
		break;
	case CMD_LINK_PARAM_ALL:
	{
		U8 msg[400];
		int len1 = sizeof(gAdcCfg);
		int len2 = sizeof(gDacCfg);
		int len3 = sizeof(gI2sCfg);
		int len4 = sizeof(gAlgCfg);
		int len = len1 + len2 + len3 + len4;
		memcpy(msg,&gAdcCfg, len1);
		memcpy(&msg[len1], &gDacCfg, len2);
		memcpy(&msg[len1+len2], &gI2sCfg, len3);
		memcpy(&msg[len1+len2+len3],&gAlgCfg, len4);
		buildAckPackect(pkt, &txLen, msg, len);
		uart_printf("send all parameter len:%d\n", len);
		altotoolPara.txLen = txLen;
		break;
	}
#endif
		default:
		buildAckPackect(pkt, &altotoolPara.txLen, &status, 1);
		break;
	}

	if(altotoolPara.txLen == 0)return 0;
	if(ret)
	{
		micEqProfile_sendData(altotoolPara.txPkt.data, altotoolPara.txLen);
	}
	return ret;
}

void altotool_ack(void)
{
	U8 *pkt = altotoolPara.txPkt.data;
	uint8_t status = 0;

	buildAckPackect(pkt, &altotoolPara.txLen, &status, 1);
	micEqProfile_sendData(altotoolPara.txPkt.data, altotoolPara.txLen);
}

float parsePacket(U8 *data)
{
	U32_t tmp;
	U8 pos = 0;

	tmp.byte[0] = data[pos++];
	tmp.byte[1] = data[pos++];
	tmp.byte[2] = data[pos++];
	tmp.byte[3] = data[pos++];

	return byteToFloat(tmp.dWords);
}


void altotool_cmd_handle(void)
{
    if(altotoolPara.procState != COMM_STATE_PROC_PKT)return ;
    uint8_t module  =  altotoolPara.module;
	uint8_t channel =  altotoolPara.rev_ch;;
	uint8_t cmd     =  altotoolPara.cmd;
    uint8_t ret = 0;
    uart_printf("rec cmd:%x for module:%x\n", cmd,module);
	switch(module)
	{
		case MODULE_START_LINK:
			ret = alto_device_base_info_link(cmd);
			break;
		case MODULE_LINK_PARAM:
			ret = alto_device_module_info_link(cmd);
			break;
#if (SUPPORT_ALTOTOOL_CODEDC)
		case MODULE_ADC:
			adc_config_setting(cmd , channel);
			break;
		case MODULE_DAC:
			dac_config_setting(cmd, channel);
			break;
#endif
#ifdef SUPPORT_ALTOTOOL_I2S
		case MODULE_I2S:
			i2s_config_setting(cmd, channel);
			break;
#endif
#if (SUPPORT_DSP_EQ)
		case MODULE_EQ_SPK:
			ret = set_altotool_spker_eq(channel);
			break;
		case MODULE_EQ_MIC:
			ret = set_altotool_mic_eq(channel);
			break;
#endif
#ifdef SUPPORT_ALTOTOOL_COMM
		case MODULE_VOL:
			set_volume_control(channel,cmd);
			break;
		case MODULE_AGC:
			set_soft_agc_config(cmd);
			break;
		case MODULE_DRC:
			set_soft_drc_config(cmd);
			break;
		case MODULE_AEC:
			set_aec_config(cmd);
			break;
		case MODULE_DEV:
			set_dereverb_config(cmd);
			break;
		case MODULE_DEN:
			set_noise_config(cmd);
			break;
		case MODULE_OTH:
			set_oth_config(cmd);
			break;
		case MODULE_EARPHONE:
			set_earophone_config(cmd);
			break;
#endif
#if (SUPPORT_ALTOTOOL_CODEDC)
		case MODULE_DEBUG:
			set_debug_config(cmd);
			break;
#endif
		default:
			break;
	}

	altotoolPara.procState = COMM_STATE_IDLE;
	if(ret == 0)
	{
		altotool_ack();
	}
}


void altotool_pkt_parse(U8 *rxData)
{
	static U16 pos = 0;
	static U16_t revLen ={.words = 0};
	U8 checksum = 0;

	switch(altotoolPara.procState)
	{
		case COMM_STATE_IDLE:
		case COMM_STATE_SOP:
			if(altotoolPara.sopStr[pos] != *rxData){
				pos = 0;
				revLen.words = 0;
				return;
			}else{
				altotoolPara.procState = COMM_STATE_SOP;
				altotoolPara.rxPkt.data[pos++] = *rxData;
				if(pos == COMM_SOP_STR_LEN)
				{
					altotoolPara.procState = COMM_STATE_MODULE;
				}
			}
			break;

		case COMM_STATE_MODULE:
			altotoolPara.rxPkt.data[pos++] = *rxData;
			altotoolPara.module = *rxData;
			altotoolPara.procState = COMM_STATE_REV;
			break;

		case COMM_STATE_REV:
			altotoolPara.rxPkt.data[pos++] = *rxData;
			altotoolPara.rev_ch = *rxData;
			altotoolPara.procState = COMM_STATE_CMD;
			break;

		case COMM_STATE_CMD:
			altotoolPara.rxPkt.data[pos++] = *rxData;
			altotoolPara.cmd = *rxData;
			altotoolPara.procState = COMM_STATE_DATALEN_H;
			break;

		case COMM_STATE_DATALEN_H:
			altotoolPara.rxPkt.data[pos++] = *rxData;
			revLen.byte[1] = *rxData;
			altotoolPara.procState = COMM_STATE_DATALEN_L;
			break;

		case COMM_STATE_DATALEN_L:
			altotoolPara.rxPkt.data[pos++] = *rxData;
			revLen.byte[0] = *rxData;
			if (revLen.words == 0){
				altotoolPara.procState = COMM_STATE_CHECKSUM_ALL;
			}
			else
			{
				altotoolPara.procState = COMM_STATE_DATAS;
			}
			altotoolPara.rxLen = revLen.words;
			break;

		case COMM_STATE_DATAS:
			altotoolPara.rxPkt.data[pos++] = *rxData;
			if ((pos-COMM_SOP_PKT_LEN) == (revLen.words))
				altotoolPara.procState = COMM_STATE_CHECKSUM_ALL;
			break;

		case COMM_STATE_CHECKSUM_ALL:
			checksum = altotool_checksum(&altotoolPara.rxPkt.data[0], COMM_SOP_PKT_LEN+revLen.words);
			//UART_Printf("get_checksum = %02x.cal checksum = %02x\r\n", *rxData, checksum);
			if (*rxData == checksum)
			{
				altotoolPara.rxPkt.data[pos++] = *rxData;
				altotoolPara.Handshake = 0;
				altotoolPara.procState = COMM_STATE_PROC_PKT;
				altotool_cmd_handle();
			}
			else
			{
				altotoolPara.procState = COMM_STATE_IDLE;
				// error
			}
			pos = 0;
			break;

		default:
			break;
	}

}



void altotool_profile_receiver(void)
{
	U8 revData[64] = {0};
	U8 length;
	if (hid_CommProtocol_rx_output_all((U8 *)(&revData),&length) == HAL_STATUS_OK)
	{
	    for(int i=0; i<length; i++)
			altotoolPara.pktParseCB(&revData[i]);
	}
}

#if SUPPORT_ALG_2MIC_HEADSET
extern float key_sound_val;
extern float RefAmp_Gain;
#endif

void altotool_profile_param_init(void)
{
#if SUPPORT_ALG_2MIC_HEADSET
	U32_t tmp;
#endif
	uart_printf("alto_tool_profile_param_init. \r\n");
	memset(&altotoolPara, 0, sizeof(comm_para_t));
	altotoolPara.procState = COMM_STATE_IDLE;
	altotoolPara.sopStr = COMM_sopStr;
	altotoolPara.devTypeStr = COMM_devType;
	altotoolPara.linked = FALSE;
	altotoolPara.isBusy = FALSE;
	altotoolPara.module = 0;
	altotoolPara.rev_ch = 0;
	altotoolPara.pktParseCB = altotool_pkt_parse;
	resetAdcParameters();
	resetI2SParameters();

#if(SUPPORT_ALTOTOOL_CODEDC && FLASH_SAVE_BANK_ENABLE)
	user_flash_region_t adcRegion = UFLASH_REGION_CODEC_ADC_PARAM;
	if(user_flash_load_data(adcRegion, (uint32_t*)&(gAdcCfg)) == 0)
	{
    	uart_printf("restore adc para ok!\r\n");
    	populateAdcParameters();
    
	}

	user_flash_region_t dacRegion = UFLASH_REGION_CODEC_DAC_PARAM;
	if(user_flash_load_data(dacRegion, (uint32_t*)&(gDacCfg)) == 0)
	{
    	uart_printf("restore dac para ok!\r\n");
    	populateDacParameters();
	}
#endif	//SUPPORT_ALTOTOOL_CODEDC

#if(SUPPORT_ALTOTOOL_COMM && FLASH_SAVE_BANK_ENABLE)
	user_flash_region_t algRegion = UFLASH_REGION_ALG_PARAM;
	if(user_flash_load_data(algRegion, (uint32_t*)&(gAlgCfg)) == 0)
	{
    	uart_printf("restore alg para ok!\r\n");
    	populateAlgoParameters();
	}

#if SUPPORT_ALG_2MIC_HEADSET
	uint8_t *p = (uint8_t *)(& gAlgCfg.epDn.vol);
	tmp.byte[0] = *p;
	tmp.byte[1] = *(p+1);
	tmp.byte[2] = *(p+2);
	tmp.byte[3] = *(p+3);
	key_sound_val = tmp.dWords;			
	RefAmp_Gain = gAlgCfg.epDn.depth *10;

	uart_printf("key_sound_val %f \r\n", key_sound_val);
	uart_printf("RefAmp_Gain %f \r\n", RefAmp_Gain);
#endif
#if(SUPPORT_ALTOTOOL_I2S && FLASH_SAVE_BANK_ENABLE)
	user_flash_region_t i2sRegion = UFLASH_REGION_I2S_PARAM;
	if(user_flash_load_data(i2sRegion, (uint32_t*)&(gI2sCfg)) == 0)
	{
    	uart_printf("restore i2s para ok!\r\n");
    	populateI2SParameters();
	}
#endif
#endif

#if (SUPPORT_DSP_EQ)
    altotool_eq_init();
#endif
}

void altotool_polling(void)
{
	altotool_cmd_handle();
	altotool_profile_receiver();
}

U16 getProductConfigType()
{
	U16 type = DSP_GENERAL;
#if ADC12_IS_AMIC
#if D_CONFIG_2MIC_MEETING_BOX
	type = USB_CONFERENCE_PHONE_AMIC;
#elif D_CONFIG_2MIC_USB_HEADPHONE
    type = USB_NC_HEADSET_AMIC;
#elif D_CONFIG_2MIC_BT_HEADPHONE
	type = BT_NC_HEADSET_AMIC;
#elif D_CONFIG_DEFAULT
	type = USB_HEADSET_AMIC;
#endif
#else
#if D_CONFIG_2MIC_USB_HEADPHONE
    type = USB_NC_HEADSET_DMIC;
#elif D_CONFIG_2MIC_BT_HEADPHONE
	type = BT_NC_HEADSET_DMIC;
#elif D_CONFIG_DEFAULT
	type = USB_HEADSET_DMIC;
#endif
#endif

	return type;
}
void getHSVersion(U8 *hsVersion)
{
	U16 type = getProductConfigType();
	softwareV[2] = (U8)type;
	memcpy(hsVersion,softwareV, 3);
	memcpy(hsVersion+3, hardwareV, 3);
}
#endif /*SUPPORT_ALTOTOOL*/

