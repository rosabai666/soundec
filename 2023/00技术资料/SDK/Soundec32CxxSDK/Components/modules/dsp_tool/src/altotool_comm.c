/**
***********************************************************
 *
 * @file name   : Altotool_comm.c
 * @author      : nick
 * @version     : V0.1
 * @date        : 2022-03-10
 * @brief       : alto tool codec config
 *
***********************************************************
 * @attention
 *
 * Copyright (c) 2019 Soundec Co., Ltd.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user
 * who uses the software, his/her employer (which must be your employer
 * and Sourced Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/
#include "altotool.h"
#include "altotool_comm.h"
#include "agc.h"
#include "drc.h"
#include "drv_codec.h"
#ifdef SUPPORT_ALTOTOOL_COMM

void set_volume_control(uint8_t channel ,uint8_t cmd)
{
	if(cmd == VOL_CTRL_SETTING)
	{
    uint8_t gain = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("cmd:%d ,channel:%d ,byte: %d \r\n", cmd, channel, gain);

		switch(channel)
		{
			case VOL_AMIC1:
				codec_set_adc12_gain(gain, 0, CHANNEL_L);
				break;
			case VOL_AMIC2:
				codec_set_adc12_gain(gain, 0, CHANNEL_R);
				break;
			case VOL_DMIC3:
				codec_set_adc34_gain(gain, 0, CHANNEL_L);
				break;
			case VOL_DMIC4:
				codec_set_adc34_gain(gain, 0, CHANNEL_R);
				break;
			case VOL_DMIC5: 
				codec_set_adc56_gain(gain, 0, CHANNEL_L);
				break;
			case VOL_DMIC6:
				codec_set_adc56_gain(gain, 0, CHANNEL_R);
				break;
			case VOL_DAC1:
				codec_set_dac_gain(gain, 0, CHANNEL_L);
				break;
			case VOL_DAC2:
				codec_set_dac_gain(gain, 0, CHANNEL_R);
				break;
			default:
				break;
		}

	}
	else
	{
	   uint8_t enable = altotoolPara.rxPkt.data[COMM_DATA_POS];
	   uart_printf("channel:%d ,byte: %d \r\n", channel, enable);
	   gAlgCfg.volCtrlEn = enable;
	   switch(channel)
	   {
		   case VOL_AMIC1:
			   break;
		   case VOL_AMIC2:
			   break;
		   case VOL_DMIC3:
			   break;
		   case VOL_DMIC4:
			   break;
		   case VOL_DMIC5: 
			   break;
		   case VOL_DMIC6:
			   break;
		   case VOL_DAC1:
			   break;
		   case VOL_DAC2:
			   break;
		   default:
			   break;
	   }
	}

}




/***************************************MIC VOL*****************************************/

/***************************************SOFT AGC****************************************/
uint8_t agcSw = 0;
extern audio_agc_param agc_param;
extern audio_agc mic_agc;
void set_soft_agc_config(uint8_t cmd )
{
    switch(cmd)
    {
		case SOFT_AGC_SWITCH:
			agcSw = altotoolPara.rxPkt.data[COMM_DATA_POS];
			gAlgCfg.agcEn = agcSw;
			uart_printf("%d \r\n", agcSw);
			break;
		case SOFT_AGC_SAMPLE_RATE:
			gAlgCfg.agc.sr = *(uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
			agc_param.sample_rate = parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
			agc_param.delta_attack	= 1.0 / ((agc_param.sample_rate/1000.0)*agc_param.attack_time);
			agc_param.delta_release = 1.0 / ((agc_param.sample_rate/1000.0)*agc_param.release_time);
			free(mic_agc.agc_in_buff);
			free(mic_agc.agc_out_buff);
			mic_agc.agc_in_buff = NULL;
			mic_agc.agc_out_buff = NULL;
			agc_param.delay_size = agc_param.sample_rate * agc_param.hold_time;
			mic_agc.agc_in_buff = (int32_t *)malloc(agc_param.delay_size*sizeof(int32_t));
			mic_agc.agc_out_buff = (int32_t *)malloc(agc_param.delay_size*sizeof(int32_t));
			uart_printf("%f %f %f %f \r\n", agc_param.sample_rate, agc_param.delta_attack, agc_param.delta_release, agc_param.delay_size);
			break;
		case SOFT_AGC_TARGET:
			gAlgCfg.agc.target =  byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			agc_param.target_level	= scl_antilog10f(parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS])/20.0);
			uart_printf("%f \r\n", agc_param.target_level);
			break;
		case SOFT_AGC_MIN_GAIN:
			gAlgCfg.agc.minGain = byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			agc_param.min_gain = scl_antilog10f(parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS])/20.0);
			uart_printf("%f \r\n", agc_param.min_gain);
			break;
		case SOFT_AGC_MAX:
			gAlgCfg.agc.maxGain = byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			agc_param.max_gain = scl_antilog10f(parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS])/20.0);
			uart_printf("%f \r\n", agc_param.max_gain);
			break;
		case SOFT_AGC_HOLDTIME:
			gAlgCfg.agc.holdTime= byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			agc_param.hold_time = parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
			free(mic_agc.agc_in_buff);
			free(mic_agc.agc_out_buff);
			mic_agc.agc_in_buff = NULL;
			mic_agc.agc_out_buff = NULL;
			agc_param.delay_size = agc_param.sample_rate * agc_param.hold_time;
			mic_agc.agc_in_buff = (int32_t *)malloc(agc_param.delay_size*sizeof(int32_t));
			mic_agc.agc_out_buff = (int32_t *)malloc(agc_param.delay_size*sizeof(int32_t));
			uart_printf("%f %f \r\n", agc_param.hold_time, agc_param.delay_size);
			break;
		case SOFT_AGC_ATTACK_TIME:
			gAlgCfg.agc.atkTime=byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			agc_param.attack_time = parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
			agc_param.delta_attack	= 1.0 / (agc_param.sample_rate*agc_param.attack_time);
			uart_printf("%f %f \r\n", agc_param.attack_time, agc_param.delta_attack);
			break;
		case SOFT_AGC_RELEASE_TIME:
			gAlgCfg.agc.rlsTime= byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			agc_param.release_time = parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
			agc_param.delta_release = 1.0 / (agc_param.sample_rate*agc_param.release_time);
			uart_printf("%f %f \r\n", agc_param.release_time, agc_param.delta_release);
			break;
		case SOFT_AGC_THRESHOLD:
			gAlgCfg.agc.thr= byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			agc_param.detect_level = scl_antilog10f(parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS])/20.0);
			uart_printf("%f \r\n", agc_param.detect_level);
			break;
		default:
			break;
    }
}

/***************************************SOFT AGC****************************************/

/***************************************SOFT DRC****************************************/


uint8_t drcSw = 0;
extern audio_drc_param drc_param;
void set_soft_drc_config(uint8_t cmd )
{
	switch(cmd)
	{
		case SOFT_DRC_SWITCH:
			drcSw = altotoolPara.rxPkt.data[COMM_DATA_POS];
			gAlgCfg.drcEn = drcSw;
			uart_printf("%d \r\n", drcSw);
			break;
		case SOFT_DRC_SAMPLE_RATE:
			drc_param.sample_rate = parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
			gAlgCfg.drc.sr = *(uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
			uart_printf("%f \r\n",drc_param.sample_rate);
			break;
		case SOFT_DRC_RATIO:
			gAlgCfg.drc.ratio= byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			drc_param.ratio = parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
			uart_printf("%f \r\n",drc_param.ratio);
			break;
		case SOFT_DRC_ATTACK_TIME:
			gAlgCfg.drc.atkTime= byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			drc_param.attack_time = parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
			drc_param.delta_attack  = scl_antilognf((-scl_log10f(9))/(drc_param.sample_rate * drc_param.attack_time /1000.0));
			uart_printf("%f \r\n", drc_param.delta_attack);
			break;
		case SOFT_DRC_RELEASE_TIME:
			gAlgCfg.drc.rlsTime= byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			drc_param.release_time = parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
			drc_param.delta_release = scl_antilognf((-scl_log10f(9))/(drc_param.sample_rate * drc_param.release_time /1000.0));
			uart_printf("%f \r\n", drc_param.delta_release);
			break;
		case SOFT_DRC_KNEE_WIDTH:
			gAlgCfg.drc.kneeWidth= byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			drc_param.knee_width = parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
			uart_printf("%f \r\n", drc_param.knee_width);
			break;
		case SOFT_DRC_THRESHOLD:
			gAlgCfg.drc.thr= byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			drc_param.threshold = parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
			uart_printf("%f \r\n", drc_param.threshold);
			break;
		default:
		break;
	}
}


/***************************************SOFT AGC****************************************/
/***************************************AEC*********************************************/

#if (SUPPORT_ALG_2MIC_MEETING_BOX)
uint8_t aecSw = 0;
float key_post_val = 5.0;
float convergence_rate = 0.2;
#endif
void set_aec_config(uint8_t cmd)
{
	//uint8_t byte = 0;
	//U32_t data ;//= {.DWORD = 0};
	switch(cmd)
	{
       case AEC_ENABLE:
#if (SUPPORT_ALG_2MIC_MEETING_BOX)
    	aecSw = altotoolPara.rxPkt.data[COMM_DATA_POS];
	   	gAlgCfg.aecEn = aecSw;
		uart_printf("%d \r\n", aec_sw);
#endif
	   	break;
	   case AEC_DEPTH:
#if (SUPPORT_ALG_2MIC_MEETING_BOX)
	   	gAlgCfg.aec.depth = byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
	   	key_post_val = parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
		uart_printf("%d \r\n", key_post_val);
#endif
	   	break;
	   case AEC_CONVERGENCE_RATE:
#if (SUPPORT_ALG_2MIC_MEETING_BOX)
	   	gAlgCfg.aec.convergenceRate = byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
	   	convergence_rate = parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
		uart_printf("%f \r\n", convergence_rate);
#endif
	   	break;
	   default:
	   	break;
	}
}


/***************************************AEC*********************************************/
/***************************************DEREVERB****************************************/

void set_dereverb_config(uint8_t cmd)
{
    uint8_t byte = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("%d \r\n",byte);
	switch(cmd)
	{
		case DEREVERB_ENABLE:
			gAlgCfg.dereverbEn = byte;
			break;
		case DEREVERB_RATE:
			gAlgCfg.dereverb.rate = byte;
			break;
		case DEREVERB_STRENGTH:
			gAlgCfg.dereverb.str = byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			break;
		default:
			break;
	}
}


/***************************************DEREVERB****************************************/

uint8_t encSw = 0;
float key_ANS_val = 1.0;
void set_noise_config(uint8_t cmd)
{
	if(cmd == NOISE_CANCELLER_ENABLE )
	{
		encSw = altotoolPara.rxPkt.data[COMM_DATA_POS];
		gAlgCfg.ncEn = encSw;
		uart_printf("%d \r\n", encSw);
	}
	else
	{
		key_ANS_val = parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
		gAlgCfg.ncDepth= byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
		uart_printf("%f \r\n",key_ANS_val);
	}

}




/***************************************DEREVERB****************************************/
void save_alg_data(void)
{
#if(FLASH_SAVE_BANK_ENABLE)
	user_flash_region_t region = UFLASH_REGION_ALG_PARAM;
	user_flash_save_data(region, (uint32_t*)&(gAlgCfg));
#endif	//FLASH_SAVE_BANK_ENABLE
}

#if SUPPORT_ALG_2MIC_HEADSET
extern float key_sound_val;
extern float RefAmp_Gain;
uint8_t dnSw = 0;
#endif

void set_earophone_config(uint8_t cmd)
{
//	uint8_t byte = altotoolPara.rxPkt.data[COMM_DATA_POS];
//	uart_printf("%d \r\n",byte);
#if SUPPORT_ALG_2MIC_HEADSET
	U32_t tmp;
#endif
	switch(cmd)
	{
		case DENOISE_ENABLE:
#if SUPPORT_ALG_2MIC_HEADSET
			gAlgCfg.doaEn = altotoolPara.rxPkt.data[COMM_DATA_POS];
			uart_printf("dnSw %f \r\n", gAlgCfg.doaEn);
#endif
			break;
		case DEPTH:
#if SUPPORT_ALG_2MIC_HEADSET
			gAlgCfg.epDn.depth = byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			RefAmp_Gain = parsePacket(&altotoolPara.rxPkt.data[COMM_DATA_POS]) * 10;
			gAlgCfg.epDn.depth = RefAmp_Gain;
			uart_printf("RefAmp_Gain %f \r\n", RefAmp_Gain);
#endif
			break;
		case VOL:
#if SUPPORT_ALG_2MIC_HEADSET
			gAlgCfg.epDn.vol = byteToFloat(*((uint32_t *)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			tmp.byte[0] = altotoolPara.rxPkt.data[COMM_DATA_POS];
			tmp.byte[1] = altotoolPara.rxPkt.data[COMM_DATA_POS+1];
			tmp.byte[2] = altotoolPara.rxPkt.data[COMM_DATA_POS+2];
			tmp.byte[3] = altotoolPara.rxPkt.data[COMM_DATA_POS+3];
			key_sound_val = tmp.dWords;
			gAlgCfg.epDn.vol = key_sound_val;
			uart_printf("key_sound_val: %f, algcfg vol:%f \r\n", key_sound_val,gAlgCfg.epDn.vol);
#endif
			break;
		default:
			break;
	}
}


void set_oth_config(uint8_t cmd)
{
	uint8_t byte = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("%d \r\n",byte);

	switch(cmd)
	{
		case OTH_BEAMFORMING:
			gAlgCfg.bfEn = byte;
			break;
		case OTH_DOA:
			gAlgCfg.doaEn = byte;
			break;
		case OTH_SPKER_EQ:
			gAlgCfg.spkEqEn = byte;
			break;
		case OTH_MIC_EQ:
			gAlgCfg.micEqEn = byte;
			break;
		case OTH_SIGNAL_REF:
			gAlgCfg.referenceEn = byte;
			break;
		case SAVE_ALG_PARAM:
			save_alg_data();
			break;
		default:
			break;
	}
}


#endif /*SUPPORT_ALTOTOOL_COMM*/

