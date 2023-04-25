/**
***********************************************************
 *
 * @file name   : Altotool_codec.h
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

#ifndef _ALTOTOOL_CODEC_
#define _ALTOTOOL_CODEC_
#include <stdio.h>
#include "math.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "common.h"

#if (SUPPORT_ALTOTOOL_CODEDC)

typedef enum{
	SUB_MODULE_AMIC1             = 0x00,
	SUB_MODULE_AMIC2             = 0x01,
	SUB_MODULE_DMIC3,
	SUB_MODULE_DMIC4,
	SUB_MODULE_DMIC5,
	SUB_MODULE_DMIC6,

}sub_adc_ch_t;

typedef enum{
	CMD_ADC_ENABLE                = 0x80, /*adc enable / disable */
	CMD_ADC_MUTE                  = 0x81,
	CMD_ADC_HPF                   = 0x82,
	CMD_ADC_MIXREC                = 0x83,
	CMD_ADC_AIAS                  = 0x84,
	CMD_ADC12_GAIN                = 0x85,
	CMD_ADC1234556_GAIN           = 0x86,
	CMD_ADC_SSR                   = 0x87,/*set sample rate*/
	CMD_ADC_SWL                   = 0x88,/*set word length*/
	CMD_ADC_AIADC                 = 0x89,
	CMD_ADC_MIXADC                = 0x8A,
	CMD_ADC_SNF                   = 0x8B,
	CMD_ADC_AGC_ENABLE            = 0x8C,
	CMD_ADC_AGC_STEREO            = 0x8D,
	CMD_ADC_AGC_NOISE_GATE        = 0x8E,
	CMD_ADC_AGC_SNR_OPT           = 0x8F,/*agc snr optimization*/
    CMD_ADC_AGC_TARGET            = 0x90,
    CMD_ADC_AGC_HOLD_TIME         = 0x91,
	CMD_ADC_AGC_ATTACK_TIME       = 0x92,
	CMD_ADC_AGC_DECAY_TIME	      = 0x93,/*set agc max/min gain*/
    CMD_ADC_AGC_NOISE_GATE_TIME   = 0x94,/*set agc max/min gain*/
    CMD_ADC_AGC_MIN_GAIN          = 0x95,/*set agc max/min gain*/
	CMD_ADC_AGC_MAX_GAIN	      = 0x96,/*set agc max/min gain*/

	CMD_SAVE_ADC_PARAM			  = 0x97,
}adc_cmd_t;

typedef enum {
	ADC_SWITCH  =  1 << 0,
	ADC_MUTE    =  1 << 1,
	ADC_HPF     =  1 << 2,
	ADC_MIXREC  =  1 << 3,
	ADC_AIAS    =  1 << 4,
	ADC_ENABLE  =  1 << 5, /*adc enable / disable */
}adc_swh_t;

#define GIM_ADC12                  0
#define GID_ADC3456                1


typedef enum{
	SUB_DAC1                      = 0x00,
	SUB_DAC2                      = 0x01,
}sub_dac_t;


typedef enum{

	CMD_DAC_ENABLE                 = 0x80,
	CMD_DAC_MUTE                   = 0x81,
	CMD_DAC_MIX                    = 0x82,
	CMD_DAC_AIAS                   = 0x83,
	CMD_DAC_SDSR                   = 0x84, /*set dac saple rate*/
	CMD_DAC_SDWW                   = 0x85, /*set dac word witch*/
	CMD_DAC_DIGITAL_GAIN           = 0x86, /*GOD*/
	CMD_DAC_ANALOG_GAIN            = 0x87, /*GOM*/
	CMD_DAC_GOMIX                  = 0x88, /*GOMIX*/
	CMD_DAC_GIMIX                  = 0x89, /*GOM*/
	CMD_DAC_MIXDAC                 = 0x8A,
	CMD_DAC_AIDAC                  = 0x8B,
	CMD_DAC_DRC_ENABLE             = 0x8C,/*switch drc*/
	CMD_DAC_THRES                  = 0x8D,/*set drc thres*/
	CMD_DAC_COMP_RATE              = 0x8E,/*set dac hardware drc:drc comprate*/

	CMD_SAVE_DAC_PARAM			   = 0x8F,
}dac_cmd_t;

typedef enum{
	DAC_GOD_GAIN	               = 0x01,
	DAC_GOM_GAIN	               = 0x02,
	DAC_GOMIX_GAIN	               = 0x03,
	DAC_GIMIX		               = 0x04,
}dac_gain_t;

void adc_config_setting(uint8_t cmd ,uint8_t channel);
void dac_config_setting(uint8_t cmd , uint8_t channel);
#endif /*SUPPORT_ALTOTOOL_CODEDC*/

#endif /* _ALTOTOOL_CODEC_ */
