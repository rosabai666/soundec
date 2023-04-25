/**
***********************************************************
 *
 * @file name	: drv_codec_cfg.h
 * @author		: zhangkun
 * @version		: V00.10
 * @date		: 2022-03-25
 * @brief		: Dolphin Codec Driver Header File
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_CODEC_CFG_H__
#define __DRV_CODEC_CFG_H__

/* Includes ------------------------------------------------------------------*/
#include "drv_codec.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define MIC_DGAIN_MAX				6//dB
#define MIC_AGAIN_MAX				6//dB
/* Private variables ---------------------------------------------------------*/
extern hal_codec_adc_agc_config_t g_adc12_agc_config;
extern hal_codec_adc_agc_config_t g_adc34_agc_config;
extern hal_codec_adc_agc_config_t g_adc56_agc_config;
extern hal_codec_adc_agc_config_t g_adc78_agc_config;
extern hal_codec_adc_agc_config_t g_adc9a_agc_config;
extern codec_dac_config_t g_dac_config;
extern codec_adc12_config_t g_adc12_config;
extern codec_adc3456_config_t g_adc3456_config;
extern codec_adc34_config_t g_adc34_config;
extern codec_adc56_config_t g_adc56_config;
extern codec_adc789a_config_t g_adc789a_config;
extern codec_adc78_config_t g_adc78_config;
extern codec_adc9a_config_t g_adc9a_config;

#ifdef __cplusplus
}
#endif

#endif  /* __DRV_CODEC_CFG_H__ */
