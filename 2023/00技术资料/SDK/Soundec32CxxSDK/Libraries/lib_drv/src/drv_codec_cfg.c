/**
***********************************************************
 *
 * @file name	: drv_codec_cfg.c
 * @author		: zhangkun
 * @version		: V00.11
 * @date		: 2022-02-12
 * @brief		: Dolphin Codec Driver Code Cfg
 *
***********************************************************
 * @attention
 *
 * Copyright (c) 2020 Soundec Co., Ltd.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user
 * who uses the software, his/her employer (which must be your employer
 * and Soundec Co., Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/
#include "boardConfig.h"

#if CODEC_ENABLE

/* Includes ------------------------------------------------------------------*/
#include "drv_codec_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif
hal_codec_adc_agc_config_t g_adc12_agc_config = {
	.stereo					= false,
	.target					= HAL_CODEC_AGC_TARGET_MAX_MINUS_19P5DB,
	.snr_optimisation_enable= false,
	.noise_gate_enable		= false,
	.noise_gate_threshold	= HAL_CODEC_AGC_NG_THR_MIN,
	.hold_time				= HAL_CODEC_AGC_HOLD_TIME_128MS,
	.attack_time			= HAL_CODEC_AGC_ATTACK_TIME_160MS,
	.decay_time				= HAL_CODEC_AGC_DECAY_TIME_160MS,
	.max_gain				= HAL_CODEC_AGC_GAIN_42P5DB,
	.min_gain				= HAL_CODEC_AGC_GAIN_21DB,
};

hal_codec_adc_agc_config_t g_adc34_agc_config = {
	.stereo					= false,
	.target					= HAL_CODEC_AGC_TARGET_MAX_MINUS_19P5DB,
	.snr_optimisation_enable= false,
	.noise_gate_enable		= false,
	.noise_gate_threshold	= HAL_CODEC_AGC_NG_THR_MIN,
	.hold_time				= HAL_CODEC_AGC_HOLD_TIME_128MS,
	.attack_time			= HAL_CODEC_AGC_ATTACK_TIME_160MS,
	.decay_time				= HAL_CODEC_AGC_DECAY_TIME_160MS,
	.max_gain				= HAL_CODEC_AGC_GAIN_42P5DB,
	.min_gain				= HAL_CODEC_AGC_GAIN_21DB,
};

hal_codec_adc_agc_config_t g_adc56_agc_config = {
	.stereo					= false,
	.target					= HAL_CODEC_AGC_TARGET_MAX_MINUS_19P5DB,
	.snr_optimisation_enable= false,
	.noise_gate_enable		= false,
	.noise_gate_threshold	= HAL_CODEC_AGC_NG_THR_MIN,
	.hold_time				= HAL_CODEC_AGC_HOLD_TIME_128MS,
	.attack_time			= HAL_CODEC_AGC_ATTACK_TIME_160MS,
	.decay_time				= HAL_CODEC_AGC_DECAY_TIME_160MS,
	.max_gain				= HAL_CODEC_AGC_GAIN_42P5DB,
	.min_gain				= HAL_CODEC_AGC_GAIN_21DB,
};

hal_codec_adc_agc_config_t g_adc78_agc_config = {
	.stereo					= false,
	.target					= HAL_CODEC_AGC_TARGET_MAX_MINUS_19P5DB,
	.snr_optimisation_enable= false,
	.noise_gate_enable		= false,
	.noise_gate_threshold	= HAL_CODEC_AGC_NG_THR_MIN,
	.hold_time				= HAL_CODEC_AGC_HOLD_TIME_128MS,
	.attack_time			= HAL_CODEC_AGC_ATTACK_TIME_160MS,
	.decay_time				= HAL_CODEC_AGC_DECAY_TIME_160MS,
	.max_gain				= HAL_CODEC_AGC_GAIN_42P5DB,
	.min_gain				= HAL_CODEC_AGC_GAIN_21DB,
};

hal_codec_adc_agc_config_t g_adc9a_agc_config = {
	.stereo					= false,
	.target					= HAL_CODEC_AGC_TARGET_MAX_MINUS_19P5DB,
	.snr_optimisation_enable= false,
	.noise_gate_enable		= false,
	.noise_gate_threshold	= HAL_CODEC_AGC_NG_THR_MIN,
	.hold_time				= HAL_CODEC_AGC_HOLD_TIME_128MS,
	.attack_time			= HAL_CODEC_AGC_ATTACK_TIME_160MS,
	.decay_time				= HAL_CODEC_AGC_DECAY_TIME_160MS,
	.max_gain				= HAL_CODEC_AGC_GAIN_42P5DB,
	.min_gain				= HAL_CODEC_AGC_GAIN_21DB,
};

codec_dac_config_t g_dac_config = {
	.master_slave 			= CODEC_DEFAULT_MODE,
	.sample_rate 			= CONFIG_CODEC_FREQUENCY,
	.is_hp					= true,
	.drc_enable				= false,
	.left_only 				= false,

	.aidacl_sel 			= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	.aidacr_sel				= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
#if SUPPORT_SPK_SIDETONE	
	.dac_mix				= HAL_CODEC_PLAYBACK_DAC_MIXED_ADC,
#else
	.dac_mix				= HAL_CODEC_PLAYBACK_DAC_ONLY,
#endif
	.mixdacl_sel			= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	.mixdacr_sel			= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	.mixadc1_sel			= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	.mixadc2_sel			= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,

	.godl					= 0,
	.godr					= 0,
	.gomixl					= 0,
	.gomixr					= 0,
	.gimixl					= 0,
	.gimixr					= 0,

	.lthres					= 0,
	.rthres					= 0,
	.lcomp					= HAL_CODEC_DRC_RATE_1,
	.rcomp					= HAL_CODEC_DRC_RATE_1,

	.hp_sel					= 0,
#ifdef SUPPORT_SPEAKER_EQ
	.gol					= 5,
	.gor					= 5,
#else
	.gol					= 0,
	.gor					= 0,
#endif
};

codec_adc12_config_t g_adc12_config = {
	.is_amic				= ADC12_IS_AMIC,
	.agc_enable				= false,
	.hpf_enable				= true,

	.master_slave			= CODEC_DEFAULT_MODE,
	.sample_rate 			= CONFIG_CODEC_FREQUENCY,

	.amic.micdiff1			= HAL_CODEC_AMIC_DIFFERENTIAL_INPUT,
	.amic.micbias1_v		= HAL_CODEC_BIAS_PULL_DOWN,
	.amic.gim1				= (MIC_AGAIN_MAX/2 + 1),
	.amic.gid1				= MIC_DGAIN_MAX,
	.amic.capcouple1		= true,

	.amic.micdiff2			= HAL_CODEC_AMIC_DIFFERENTIAL_INPUT,
	.amic.micbias2_v		= HAL_CODEC_BIAS_PULL_DOWN,
	.amic.gim2				= (MIC_AGAIN_MAX/2 + 1),
	.amic.gid2				= MIC_DGAIN_MAX,
	.amic.capcouple2		= true,

	.dmic.clock				= HAL_CODEC_DMIC_RATIO_4,
	.dmic.gid1				= MIC_DGAIN_MAX,
	.dmic.gid2				= MIC_DGAIN_MAX,
	.dmic.adc_dmic_sel1 	= HAL_CODEC_ADC_DATA_PROVIEDE_DMIC_SELF_CHANNEL,	/* Should be changed according to #is_amic setting */
	.dmic.adc_dmic_sel2 	= HAL_CODEC_ADC_DATA_PROVIEDE_DMIC_SELF_CHANNEL,	/* Should be changed according to #is_amic setting */

	.aiadc1_sel				= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	.aiadc2_sel				= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	.mix_rec				= HAL_CODEC_ADC_MIXER_MODE_RECORD_ONLY,
	.adc12_mode				= HAL_CODEC_ADC12_NORMAL_MODE,
	.wnf					= HAL_CODEC_WNF_OFF,

	.agc_config				= &g_adc12_agc_config,
};

codec_adc3456_config_t g_adc3456_config = {
	.master_slave			= CODEC_DEFAULT_MODE,
	.sample_rate 			= CONFIG_CODEC_FREQUENCY,
	.dmic_clock 			= HAL_CODEC_DMIC_RATIO_4,
};

codec_adc34_config_t g_adc34_config = {
	.agc_enable				= false,
	.hpf_enable				= true,
	.adc_dmic_sel3			= HAL_CODEC_DMIC_SELF_CHANNEL,
	.adc_dmic_sel4	 		= HAL_CODEC_DMIC_SELF_CHANNEL,
	.aiadc3_sel				= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	.aiadc4_sel			 	= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	.mix_rec				= HAL_CODEC_ADC_MIXER_MODE_RECORD_ONLY,
	.gid3			 		= 12,
	.gid4			 		= 12,
	.wnf					= HAL_CODEC_WNF_OFF,
	.agc_config				= &g_adc34_agc_config,
};

codec_adc56_config_t g_adc56_config = {
	.agc_enable				= false,
	.hpf_enable				= true,
	.adc_dmic_sel5	 		= HAL_CODEC_DMIC_SELF_CHANNEL,
	.adc_dmic_sel6	 		= HAL_CODEC_DMIC_SELF_CHANNEL,
	.aiadc5_sel				= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	.aiadc6_sel			 	= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	.mix_rec 				= HAL_CODEC_ADC_MIXER_MODE_RECORD_ONLY,
	.gid5			 		= 12,
	.gid6			 		= 12,
	.wnf					= HAL_CODEC_WNF_OFF,
	.agc_config				= &g_adc56_agc_config,
};

codec_adc789a_config_t g_adc789a_config = {
	.master_slave			= CODEC_DEFAULT_MODE,
	.sample_rate 			= CONFIG_CODEC_FREQUENCY,
	.dmic_clock 			= HAL_CODEC_DMIC_RATIO_4,
};

codec_adc78_config_t g_adc78_config = {
	.agc_enable				= false,
	.hpf_enable				= true,
	.adc_dmic_sel7			= HAL_CODEC_DMIC_SELF_CHANNEL,
	.adc_dmic_sel8	 		= HAL_CODEC_DMIC_SELF_CHANNEL,
	.aiadc7_sel				= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	.aiadc8_sel			 	= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	.gid7			 		= 12,
	.gid8			 		= 12,
	.wnf					= HAL_CODEC_WNF_OFF,
	.agc_config				= &g_adc78_agc_config,
};

codec_adc9a_config_t g_adc9a_config = {
	.agc_enable				= false,
	.hpf_enable				= true,
	.adc_dmic_sel9	 		= HAL_CODEC_DMIC_SELF_CHANNEL,
	.adc_dmic_sel10	 		= HAL_CODEC_DMIC_SELF_CHANNEL,
	.aiadc9_sel				= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	.aiadc10_sel			= HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS,
	.gid9			 		= 12,
	.gid10			 		= 12,
	.wnf					= HAL_CODEC_WNF_OFF,
	.agc_config				= &g_adc9a_agc_config,
};


#ifdef __cplusplus
}
#endif
#endif



