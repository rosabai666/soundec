/**
***********************************************************
 *
 * @file name   : Altotool_codec.c
 * @author      : nick
 * @version     : V0.1
 * @date        : 2022-03-00
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
#include "altotool_codec.h"
#include "altotool.h"
#include "drv_codec.h"
#include "userFlash.h"

#if (SUPPORT_ALTOTOOL_CODEDC)
void save_adc_data(void)
{
#if(FLASH_SAVE_BANK_ENABLE)
	user_flash_region_t region = UFLASH_REGION_CODEC_ADC_PARAM;
	user_flash_save_data(region, (uint32_t*)&(gAdcCfg));
#endif	//FLASH_SAVE_BANK_ENABLE
}


void switch_adc_enable(uint8_t channel)
{
    uint8_t en = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc chx%d enable = %d \r\n",channel, en);
	gAdcCfg.chxCfg[channel].en = en;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
			codec_adc12_enable(en, CHANNEL_L);
			break;
		case SUB_MODULE_AMIC2:
			codec_adc12_enable(en, CHANNEL_R);
			break;
		case SUB_MODULE_DMIC3:
			codec_adc34_enable(en, CHANNEL_L);
			break;
		case SUB_MODULE_DMIC4:
			codec_adc34_enable(en, CHANNEL_R);
			break;
		case SUB_MODULE_DMIC5:
			codec_adc56_enable(en, CHANNEL_L);
			break;
		case SUB_MODULE_DMIC6:
			codec_adc56_enable(en, CHANNEL_R);
			break;
		default:
			break;
	}
}

void switch_adc_mute(uint8_t channel)
{
    uint8_t mute = altotoolPara.rxPkt.data[COMM_DATA_POS];
    uart_printf("adc chx%d mute = %d\r\n",channel, mute);
    gAdcCfg.chxCfg[channel].mute = mute;

}	

void switch_adc_hpf(uint8_t channel)
{
    uint8_t en = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc chx%d hpf enable = %d\r\n",channel, en);
	gAdcCfg.pairCfg[channel/2].hpf = en;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
			codec_enable_adc12_hpf(en);
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
			codec_enable_adc34_hpf(en);
			break;
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			codec_enable_adc56_hpf(en);
			break;
		default:
			break;
	}
}	
void switch_adc_mixrec(uint8_t channel)
{
    uint8_t en = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc chx%d mixrec enable = %d \r\n",channel, en);
	gAdcCfg.pairCfg[ADC_12].mixrec = en;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
			codec_select_adc12_mixer_mode(en);
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
			codec_select_adc34_mixer_mode(en);
			break;
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			codec_select_adc56_mixer_mode(en);
			break;
		default:
			break;
	}
}	
void switch_adc_aias(uint8_t channel)
{
    uint8_t en = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc%d aias %d \r\n",channel, en);

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
			gAdcCfg.gCfg[ADC_12].aias = en;
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			gAdcCfg.gCfg[ADC_3456].aias = en;
			break;
		default:
			break;
	}
}	


void set_adc12_analog_gain(uint8_t channel)
{

    uint8_t gain = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc chx%d gain = %d \r\n",channel, gain);
	gAdcCfg.chxCfg[channel].gim = gain;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
			codec_set_adc12_gain(gain, 1, CHANNEL_L);
			break;
		case SUB_MODULE_AMIC2:
			codec_set_adc12_gain(gain, 1, CHANNEL_R);
			break;
		default:
			break;
	}
}	

void set_adc123456_digital_gain(uint8_t channel)
{
    int8_t gain = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc chx%d gain = %d \r\n",channel, gain);
	gAdcCfg.chxCfg[channel].gid = gain;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
			codec_set_adc12_gain(gain, 0, CHANNEL_L);
			break;
		case SUB_MODULE_AMIC2:
			codec_set_adc12_gain(gain, 0, CHANNEL_R);
			break;
		case SUB_MODULE_DMIC3:
			codec_set_adc34_gain(gain, 0, CHANNEL_L);
			break;
		case SUB_MODULE_DMIC4:
			codec_set_adc34_gain(gain, 0, CHANNEL_R);
			break;
		case SUB_MODULE_DMIC5:
			codec_set_adc56_gain(gain, 0, CHANNEL_L);
			break;
		case SUB_MODULE_DMIC6:
			codec_set_adc56_gain(gain, 0, CHANNEL_R);
			break;
		default:
			break;
	}
}	


void set_adc_sample_rate(uint8_t channel)
{
   	U32_t fs ;
	
	fs.byte[0] = altotoolPara.rxPkt.data[COMM_DATA_POS +0];
	fs.byte[1] = altotoolPara.rxPkt.data[COMM_DATA_POS +1];
	fs.byte[2] = altotoolPara.rxPkt.data[COMM_DATA_POS +2];
	fs.byte[3] = altotoolPara.rxPkt.data[COMM_DATA_POS +3];
	uart_printf("fs : %d  \r\n",fs.dWords);

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
		{
			gAdcCfg.gCfg[ADC_12].sr = fs.dWords;
			codec_set_adc12_samplerate(fs.dWords);
			codec_set_adc3456_samplerate(fs.dWords);
		}
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
		{
			gAdcCfg.gCfg[ADC_3456].sr = fs.dWords;
			codec_set_adc12_samplerate(fs.dWords);
			codec_set_adc3456_samplerate(fs.dWords);
		}
			break;
		default:
			break;
	}
}	

void set_adc_word_length(uint8_t channel)
{
    uint8_t wordLength = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc%d word lenth %d \r\n",channel ,wordLength);

	gAdcCfg.gCfg[ADC_12].wordLength = wordLength;
	gAdcCfg.gCfg[ADC_3456].wordLength = wordLength;
	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			codec_set_adc_word_length(wordLength);
			break;
		default:
			break;
	}
}	

void set_adc_aiadc(uint8_t channel)
{
    uint8_t sel = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc chx%d aiadc = %d \r\n",channel, sel);
	gAdcCfg.chxCfg[channel].aiadc = sel;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
			codec_set_adc1_aiadc(sel);
			break;
		case SUB_MODULE_AMIC2:
			codec_set_adc2_aiadc(sel);
			break;
		case SUB_MODULE_DMIC3:
			codec_set_adc3_aiadc(sel);
			break;
		case SUB_MODULE_DMIC4:
			codec_set_adc4_aiadc(sel);
			break;
		case SUB_MODULE_DMIC5:
			codec_set_adc5_aiadc(sel);
			break;
		case SUB_MODULE_DMIC6:
			codec_set_adc6_aiadc(sel);
			break;
		default:
			break;
	}
}	

void set_adc_mixadc(uint8_t channel)
{
    uint8_t sel = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc chx%d mixadc = %d \r\n",channel, sel);

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
			gAdcCfg.gCfg[ADC_12].mixadc1 = sel;
			codec_select_dac_mixadc1_input(sel);
			break;
		case SUB_MODULE_AMIC2:
			gAdcCfg.gCfg[ADC_3456].mixadc1 = sel;
			codec_select_dac_mixadc2_input(sel);
			break;
		default:
			break;
	}
}	

void set_adc_wnf(uint8_t channel)
{
    uint8_t mode = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc chx%d wnf = %d \r\n",channel, mode);
	gAdcCfg.pairCfg[channel/2].wnf = mode;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
			codec_set_adc12_wnf_mode(mode);
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
			codec_set_adc34_wnf_mode(mode);
			break;
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			codec_set_adc56_wnf_mode(mode);
			break;
		default:
			break;
	}
}	

void switch_adc_hardware_agc_enable(uint8_t channel)
{
    uint8_t en = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc chx%d adc agc enable = %d \r\n",channel, en);
	gAdcCfg.pairCfg[channel/2].agcCfg.agcEn = en;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
			codec_adc12_agc_enable(en);
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
			codec_adc34_agc_enable(en);
			break;
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			codec_adc56_agc_enable(en);
			break;
		default:
			break;
	}
}	


void switch_adc_agc_stereo_enable(uint8_t channel)
{
    uint8_t en = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc%d agc  %d \r\n",en);
	gAdcCfg.pairCfg[channel/2].agcCfg.stereo = en;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
			codec_set_adc12_agc_stereo(en);
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
			codec_set_adc34_agc_stereo(en);
			break;
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			codec_set_adc56_agc_stereo(en);
			break;
		default:
			break;
	}
}	

void switch_adc_agc_noise_gate_enable(uint8_t channel)
{
    uint8_t en = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc%d niose gate enable %d \r\n",channel ,en);
	gAdcCfg.pairCfg[channel/2].agcCfg.ngEn = en;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
			codec_adc12_agc_noise_gate_enable(en);
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
			codec_adc34_agc_noise_gate_enable(en);
			break;
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			codec_adc56_agc_noise_gate_enable(en);
			break;
		default:
			break;
	}
}	


void switch_adc_agc_snr_optimzation(uint8_t channel)
{
    uint8_t en = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc%d snr byte = %d \r\n", channel, en);
	gAdcCfg.pairCfg[channel/2].agcCfg.snrOptEn = en;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
			codec_adc12_agc_snr_optimzation(en);  	//only adc12 have snr optimzation
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			uart_printf("DMIC3456 does not support SNR OPTIMZATION.\r\n");
			break;
		default:
			break;
	}
}	


void set_adc_agc_target(uint8_t channel)
{
    uint8_t target = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc%d agc target  %d \r\n", channel, target);
	gAdcCfg.pairCfg[channel/2].agcCfg.target = target;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
			codec_set_adc12_agc_target(target);
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
			codec_set_adc34_agc_target(target);
			break;
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			codec_set_adc56_agc_target(target);
			break;
		default:
			break;
	}
}	

void set_adc_agc_hold_time(uint8_t channel)
{
    uint8_t holdTime = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("adc%d agc hold time %d \r\n", channel, holdTime);
	gAdcCfg.pairCfg[channel/2].agcCfg.holdTime = holdTime;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
			codec_set_adc12_agc_hold_time(holdTime);
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
			codec_set_adc34_agc_hold_time(holdTime);
			break;
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			codec_set_adc56_agc_hold_time(holdTime);
			break;
		default:
			break;
	}
}	


void set_adc_agc_attack_time(uint8_t channel)
{
    uint8_t attackTime = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("byte = %d \r\n",attackTime);
	gAdcCfg.pairCfg[channel/2].agcCfg.atkTime = attackTime;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
			codec_set_adc12_agc_attack_time(attackTime);
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
			codec_set_adc34_agc_attack_time(attackTime);
			break;
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			codec_set_adc56_agc_attack_time(attackTime);
			break;
		default:
			break;
	}
}	

void set_adc_agc_decay_time(uint8_t channel)
{
    uint8_t decayTime = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("byte = %d \r\n",decayTime);
	gAdcCfg.pairCfg[channel/2].agcCfg.dcyTime = decayTime;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
			codec_set_adc12_agc_decay_time(decayTime);
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
			codec_set_adc34_agc_decay_time(decayTime);
			break;
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			codec_set_adc56_agc_decay_time(decayTime);
			break;
		default:
			break;
	}
}	

void set_adc_agc_noise_gate_thres(uint8_t channel)
{
    uint8_t threshold = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("byte = %d \r\n", threshold);
	gAdcCfg.pairCfg[channel/2].agcCfg.ngThr = threshold;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
			codec_set_adc12_agc_noise_gate_thres(threshold);
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
			codec_set_adc34_agc_noise_gate_thres(threshold);
			break;
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			codec_set_adc56_agc_noise_gate_thres(threshold);
			break;
		default:
			break;
	}
}	

void set_adc_agc_gain_min(uint8_t channel)
{
    uint8_t minGain = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("byte = %d \r\n", minGain);
	gAdcCfg.pairCfg[channel/2].agcCfg.minGain = minGain;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
			codec_set_adc12_agc_min_gain(minGain);
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
			codec_set_adc34_agc_min_gain(minGain);
			break;
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			codec_set_adc56_agc_min_gain(minGain);
			break;
		default:
			break;
	}
}	

void set_adc_agc_gain_max(uint8_t channel)
{
    uint8_t maxGain = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("byte = %d \r\n", maxGain);
	gAdcCfg.pairCfg[channel/2].agcCfg.maxGain = maxGain;

	switch(channel)
	{
		case SUB_MODULE_AMIC1:
		case SUB_MODULE_AMIC2:
			codec_set_adc12_agc_max_gain(maxGain);
			break;
		case SUB_MODULE_DMIC3:
		case SUB_MODULE_DMIC4:
			codec_set_adc34_agc_max_gain(maxGain);
			break;
		case SUB_MODULE_DMIC5:
		case SUB_MODULE_DMIC6:
			codec_set_adc56_agc_max_gain(maxGain);
			break;
		default:
			break;
	}
}	


void adc_config_setting(uint8_t cmd ,uint8_t channel)
{
  uart_printf("cmd : %d channel : %d \r\n",cmd,channel);
  
  switch(cmd)
  {
	  case CMD_ADC_ENABLE://				= 0x80, /*adc enable / disable */
		  switch_adc_enable(channel);
		  break;
	  case CMD_ADC_MUTE://					= 0x81,
		  switch_adc_mute(channel);
		  break;
	  case CMD_ADC_HPF://					= 0x82,
		  switch_adc_hpf(channel);
		  break;
	  case CMD_ADC_MIXREC://				= 0x83,
		  switch_adc_mixrec(channel);
		  break;
	  case CMD_ADC_AIAS://					= 0x84,
		  switch_adc_aias(channel);
		  break;
	  case CMD_ADC12_GAIN://				= 0x85,
		  set_adc12_analog_gain(channel);
		  break;
	  case CMD_ADC1234556_GAIN://			= 0x86,
		  set_adc123456_digital_gain(channel);
		  break;
	  case CMD_ADC_SSR://					= 0x87,/*set sample rate*/
		  set_adc_sample_rate(channel);
		  break;
	  case CMD_ADC_SWL://					= 0x88,/*set word length*/
		  set_adc_word_length(channel);
		  break;
	  case CMD_ADC_AIADC:// 				= 0x89,
		  set_adc_aiadc(channel);
		  break;
	  case CMD_ADC_MIXADC://				= 0x8A,
		  set_adc_mixadc(channel);
		  break;
	  case CMD_ADC_SNF://					= 0x8B,
		  set_adc_wnf(channel);
		  break;
	  case CMD_ADC_AGC_ENABLE://			= 0x8C,
		  switch_adc_hardware_agc_enable(channel);
		  break;
	  case CMD_ADC_AGC_STEREO://			= 0x8D,
		  switch_adc_agc_stereo_enable(channel);
		  break;
	  case CMD_ADC_AGC_NOISE_GATE://		= 0x8E,
		  switch_adc_agc_noise_gate_enable(channel);
		  break;
	  case CMD_ADC_AGC_SNR_OPT://			= 0x8F,/*agc snr optimization*/
		  switch_adc_agc_snr_optimzation(channel);
		  break;
	  case CMD_ADC_AGC_TARGET://			= 0x90,
		  set_adc_agc_target(channel);
		  break;
	  case CMD_ADC_AGC_HOLD_TIME:// 		= 0x91,
		  set_adc_agc_hold_time(channel);
		  break;
	  case CMD_ADC_AGC_ATTACK_TIME://		= 0x92,
		  set_adc_agc_attack_time(channel);
		  break;
	  case CMD_ADC_AGC_NOISE_GATE_TIME://	= 0x93,/*set agc max/min gain*/
		  set_adc_agc_noise_gate_thres(channel);
		  break;
	  case CMD_ADC_AGC_MIN_GAIN://			= 0x94,/*set agc max/min gain*/
		  set_adc_agc_gain_min(channel);
		  break;
	  case CMD_ADC_AGC_MAX_GAIN://			= 0x95,/*set agc max/min gain*/
		  set_adc_agc_gain_max(channel);
		  break;
	  case CMD_ADC_AGC_DECAY_TIME://		= 0x96,/*set agc max/min gain*/
		  set_adc_agc_decay_time(channel);
		  break;
	  case CMD_SAVE_ADC_PARAM:
		  save_adc_data();
		  break;
	  default:
	  	  break;
  }
}


/**************************************ADC**********************************************/


/**************************************DAC**********************************************/
void save_dac_data(void)
{
#if(FLASH_SAVE_BANK_ENABLE)
	user_flash_region_t region = UFLASH_REGION_CODEC_DAC_PARAM;
	user_flash_save_data(region, (uint32_t*)&(gDacCfg));
#endif	//FLASH_SAVE_BANK_ENABLE
}


void switch_dac_enable(uint8_t channel)
{
    uint8_t en = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("dac enable = %d \r\n",en);
	gDacCfg.enable = en;

	codec_dac_enable(en);
}

void switch_dac_mute(uint8_t channel)
{
    uint8_t mute = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("dac channel=%d, mute = %d \r\n",channel,mute);
	gDacCfg.chxCfg[channel].mute = mute;

}

void switch_dac_mix(uint8_t channel)
{
    uint8_t en = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("dac mixrec = %d \r\n",en);
	gDacCfg.mix = en;

	codec_dac_mix_enable(en);
}


void switch_dac_aias(uint8_t channel)
{
    uint8_t en = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("dac aias en = %d \r\n",en);
	gDacCfg.aias = en;

    if(channel == SUB_DAC1 )
    {
    }
    else
    {
    }
}

void set_dac_word_width(uint8_t channel)
{
    uint8_t mode = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("dac word length mode = %d \r\n",mode);
	gDacCfg.wordLength = mode;

	//can not config
	uart_printf("codec does not support config dac wordwidth.\r\n");
}

void set_dac_digital_gain(uint8_t channel)
{
    int8_t gain = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("dac chx%d god = %d \r\n",channel, gain);
	gDacCfg.chxCfg[channel].god = gain;

   if(channel == SUB_DAC1 )
   {
	   codec_set_dac_gain(gain, 0, CHANNEL_L);
   }
   else
   {
	   codec_set_dac_gain(gain, 0, CHANNEL_R);
   }
}
void set_dac_analog_gain(uint8_t channel)
{
    int8_t gain = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("dac chx%d gom = %d \r\n",channel, gain);
	gDacCfg.chxCfg[channel].gom = gain;

   if(channel == SUB_DAC1 )
   {
	  codec_set_dac_gain(gain, 1, CHANNEL_L);
   }
   else
   {
	  codec_set_dac_gain(gain, 1, CHANNEL_R);
   }
}
void set_dac_gomix_gain(uint8_t channel)
{
    int8_t gain = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("dac chx%d gomix = %d \r\n",channel, gain);
	gDacCfg.chxCfg[channel].gomix = gain;

   if(channel == SUB_DAC1 )
   {
	   codec_set_dac_golmix_gain(gain);
   }
   else
   {
	   codec_set_dac_gormix_gain(gain);
   }
}

void set_dac_gimix_gain(uint8_t channel)
{
    int8_t gain = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("dac chx%d gimix = %d \r\n",channel, gain);
	gDacCfg.chxCfg[channel].gimix = gain;

   if(channel == SUB_DAC1 )
   {
	   codec_set_dac_gilmix_gain(gain);
   }
   else
   {
	   codec_set_dac_girmix_gain(gain);
   }
}


void set_dac_mixdac(uint8_t channel)
{
    uint8_t sel = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("dac chx%d mixdac = %d \r\n",channel, sel);
	gDacCfg.chxCfg[channel].mixdac = sel;

    if(channel == SUB_DAC1 )
    {
    	codec_set_dac_mixdacl(sel);
    }
    else
    {
    	codec_set_dac_mixdacr(sel);
    }
}

void set_dac_aidac(uint8_t channel)
{
    uint8_t sel = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("dac chx%d aidac = %d \r\n", channel, sel);
	gDacCfg.chxCfg[channel].aidac = sel;

    if(channel == SUB_DAC1)
    {
    	codec_set_dac_aidacl(sel);
    }
    else
    {
    	codec_set_dac_aidacr(sel);
    }
}

void switch_dac_drc_enbale(uint8_t channel)
{
    uint8_t en = altotoolPara.rxPkt.data[0];
	uart_printf("dac drc enable = %d \r\n",en);
	gDacCfg.drcEn = en;

	codec_dac_drc_enbale(en);
}

void set_dac_drc_thres(uint8_t channel)
{
    uint8_t thres = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("dac chx%d thres = %d \r\n",channel, thres);
	gDacCfg.chxCfg[channel].drcThr = thres;

   if(channel == SUB_DAC1)
   {
	  codec_set_dac_drc_lthres(thres);
   }
   else
   {
	  codec_set_dac_drc_rthres(thres);
   }
}

void set_dac_drc_comprate(uint8_t channel)
{
    uint8_t rate = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("dac chx%d comp rate = %d \r\n",channel, rate);
	gDacCfg.chxCfg[channel].drcComp = rate;

   if(channel == SUB_DAC1)
   {
	   codec_set_dac_drc_lcomprate(rate);
   }
   else
   {
	   codec_set_dac_drc_rcomprate(rate);
   }
}

void dac_config_setting(uint8_t cmd , uint8_t channel)
{
	uart_printf("cmd:%d ,channel:%d  \r\n",cmd,channel);
	
	switch(cmd)
	{
		case CMD_DAC_ENABLE://			   = 0x80,
			switch_dac_enable(channel);
			break;
		case CMD_DAC_MUTE://			   = 0x81,
			switch_dac_mute(channel);
			break;
		case CMD_DAC_MIX:// 			   = 0x82,
			switch_dac_mix(channel);
			break;
		case CMD_DAC_AIAS://			   = 0x83,
			switch_dac_aias(channel);
			break;
		case CMD_DAC_SDWW://			   = 0x85, /*set dac word witch*/
			set_dac_word_width(channel);
			break;
		case CMD_DAC_DIGITAL_GAIN://	   = 0x86, /*GOD*/
			set_dac_digital_gain(channel);
			break;
		case CMD_DAC_ANALOG_GAIN ://	   = 0x87, /*GOM*/
			set_dac_analog_gain(channel);
			break;
		case CMD_DAC_GOMIX://			   = 0x88, /*GOMIX*/
			set_dac_gomix_gain(channel);
			break;
		case CMD_DAC_GIMIX://			   = 0x89, /*GOM*/
			set_dac_gimix_gain(channel);
			break;
		case CMD_DAC_MIXDAC://			   = 0x8A,
			set_dac_mixdac(channel);
			break;
		case CMD_DAC_AIDAC://			   = 0x8B,
			set_dac_aidac(channel);
			break;
		case CMD_DAC_DRC_ENABLE://		   = 0x8C,/*switch drc*/
			switch_dac_drc_enbale(channel);
			break;
		case CMD_DAC_THRES://			   = 0x8D,/*set drc thres*/
			set_dac_drc_thres(channel);
			break;
		case CMD_DAC_COMP_RATE://		   = 0x8E,/*set dac hardware drc:drc comprate*/
			set_dac_drc_comprate(channel);
			break;
		case CMD_SAVE_DAC_PARAM:
			save_dac_data();
			break;
		default:
		   break;
    }
}


#endif /*SUPPORT_ALTOTOOL_CODEDC*/
