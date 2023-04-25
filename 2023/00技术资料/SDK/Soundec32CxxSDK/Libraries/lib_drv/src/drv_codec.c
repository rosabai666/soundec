/** 
***********************************************************
 * 
 * @file name	: drv_codec.c
 * @author		: zhangkun
 * @version		: V00.11
 * @date		: 2022-02-12
 * @brief		: Dolphin Codec Driver Code
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

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "drv_codec.h"
#include "hal_gpio.h"
#include "snc8600.h"
#include "hal_clock.h"
#include "hal_interrupt.h"
#include "hal_codec_fifo.h"
#include "hal_sysctrl.h"
#include "SaveRestoreVectorRegisters.h"
#include "hal_sysctrl.h"
#include "app_dmac_test.h"
#include "dsp_eq.h"
#include "altotool.h"
#include "drv_codec_cfg.h"
#if (AUDIO_MANAGER)
#include "audio_manager.h"
#endif
#if SUPPORT_PLAY_TONE
#include "playTone.h"
#endif
#include "usbd_config.h"

extern int systemCrystal;

#if SUPPORT_ALG
#include "speech.h"
#endif

#if(DMA_TEST_CODEC_ADC_ENABLE)
#include "hal_dmac.h"
#endif
#if CODEC_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

#if (SUPPORT_ALTOTOOL_CODEDC)
extern codec_adc_cfgs_t gAdcCfg;
extern codec_dac_cfgs_t gDacCfg;
#endif

#if (AUDIO_MANAGER)
auDevInfos_t codecDevInfos[CODEC_CALLBACK_NUM]; //= {0};
#else
codec_isr_cb_func g_codec_cb_table[CODEC_CALLBACK_NUM] ;//= {0};
#endif
/**
 * @brief This function is used to initialize the Codec clk settings
 */
drv_status_t codec_clk_init(void)
{
	/*clock src init*/
#if (CODEC_SLAVE_FRAC_SRC == CODEC_SLAVE_FRAC_SRC_DOL)
	hal_sysctrl_set_codec_slave_frac_src(HAL_SYSCTRL_CODEC_SLAVE_FRAC_SRC_DOL);
#else
	hal_sysctrl_set_codec_slave_frac_src(HAL_SYSCTRL_CODEC_SLAVE_FRAC_SRC_SNC);
#endif

	/*ADC and DAC clock use delicated fractinal divider*/
#if (CODEC_SLAVE_FRAC_SHARE == CODEC_SLAVE_FRAC_SHARE_DEDICATED)
	hal_sysctrl_set_codec_slave_frac_share(HAL_SYSCTRL_CODEC_SLAVE_FRAC_SHARE_DEDICATE);
#else
	hal_sysctrl_set_codec_slave_frac_share(HAL_SYSCTRL_CODEC_SLAVE_FRAC_ShARE_ONE);
#endif

	hal_sysctrl_set_clock_gate(hal_sysctrl_get_clock_gate() | (1 << HAL_SYSCTRL_CLKGT_CODEC));

	/*MCLK input:12MHz or 13MHz*/
	if (hal_get_system_crystal() == SYSTEM_CRYSTAL_24MHZ)
		hal_codec_set_crystal(HAL_CODEC_CRYSTAL_12MHZ);
	else
		hal_codec_set_crystal(HAL_CODEC_CRYSTAL_13MHZ);

	return DRV_STATUS_OK;
}

drv_status_t codec_init(void)
{
	codec_clk_init();
	codec_set_operation_mode(CODEC_OP_MODE_SHUTDOWN);

	return DRV_STATUS_OK;
}
#if (AUDIO_MANAGER)


#if (CODEC_ADC_ENABLE)
drv_status_t codec_adc_init(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	if(codecDevInfos[CODEC_CALLBACK_ADC].initialized)
		return DRV_STATUS_OK;

	codecDevInfos[CODEC_CALLBACK_ADC].initialized = true;

	codecDevInfos[CODEC_CALLBACK_ADC].cbArg = capture->attr->pStream;
	codecDevInfos[CODEC_CALLBACK_ADC].bitSlot = capture->format->bitSlot;
	codecDevInfos[CODEC_CALLBACK_ADC].wordSize = capture->attr->formatCap->wordSize;
	codecDevInfos[CODEC_CALLBACK_ADC].channel = capture->format->channlesPerFrame;
	codecDevInfos[CODEC_CALLBACK_ADC].sampleRate = capture->format->sampleRateHz;
	codecDevInfos[CODEC_CALLBACK_ADC].calSwGainCb = capture->attr->calSwGainCb;
	codecDevInfos[CODEC_CALLBACK_ADC].doEqCb= capture->attr->doEqCb;
	codecDevInfos[CODEC_CALLBACK_ADC].dataLen = CODEC_FIFO_AF_LEVEL*codecDevInfos[CODEC_CALLBACK_ADC].channel*BIT_SLOT_32;

	codec_set_operation_mode(CODEC_OP_MODE_VOICE_OPEN);
#if (SUPPORT_ALTOTOOL_CODEDC)
	hal_codec_set_adc_data_mode(gAdcCfg.gCfg[ADC_12].wordLength);
#else
	hal_codec_set_adc_data_mode(1);
#endif	

#if CODEC_ADC12_ENABLE
	adc12_init();
#endif

#if CODEC_ADC34_ENABLE
	adc3456_common_init();
	adc34_init();
#endif

#if CODEC_ADC56_ENABLE
	adc56_init();
#endif

#if CODEC_ADC78_ENABLE
	adc789a_common_init();
	adc78_init();
#endif

#if CODEC_ADC9A_ENABLE
	adc9a_init();
#endif

#if(SUPPORT_USB_MIC)
	codec_set_adc_samplerate(MIC_DEFAULT_FREQ);
#endif

	codec_register_cb(CODEC_CALLBACK_ADC, capture->attr->isrCb);

	codec_adc_start();

	return DRV_STATUS_OK;
}
#endif

drv_status_t codec_dac_init(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	if(codecDevInfos[CODEC_CALLBACK_DAC].initialized)
		return DRV_STATUS_OK;

	codecDevInfos[CODEC_CALLBACK_DAC].initialized = true;

	codecDevInfos[CODEC_CALLBACK_DAC].cbArg = playback->attr->pStream;
	codecDevInfos[CODEC_CALLBACK_DAC].bitSlot = playback->format->bitSlot;
	codecDevInfos[CODEC_CALLBACK_DAC].wordSize = playback->attr->formatCap->wordSize;
	codecDevInfos[CODEC_CALLBACK_DAC].channel = playback->format->channlesPerFrame;
	codecDevInfos[CODEC_CALLBACK_DAC].calSwGainCb = playback->attr->calSwGainCb;
	codecDevInfos[CODEC_CALLBACK_DAC].doEqCb = playback->attr->doEqCb;
	codecDevInfos[CODEC_CALLBACK_DAC].useSwGain = playback->attr->useSwGain;
	codecDevInfos[CODEC_CALLBACK_DAC].dataLen = CODEC_FIFO_AE_LEVEL*codecDevInfos[CODEC_CALLBACK_DAC].channel*BIT_SLOT_32;
	((auStream_t *)codecDevInfos[CODEC_CALLBACK_DAC].cbArg)->var.swgain[0] = 0x7fffffff;
	((auStream_t *)codecDevInfos[CODEC_CALLBACK_DAC].cbArg)->var.swgain[1] = 0x7fffffff;

	codec_set_operation_mode(CODEC_OP_MODE_PLAYBACK_OPEN);

	g_dac_config.sample_rate = playback->format->sampleRateHz;

	codec_config_dac(&g_dac_config);
	
//#if(SUPPORT_USB_SPK)
//	codec_set_dac_samplerate(SPK_DEFAULT_FREQ);
//#endif

	hal_codec_fifo_init(HAL_CODEC_FIFO_DAC, CODEC_FIFO_AE_LEVEL);

#if CODEC_USE_ONE_IRQ
	hal_codec_unmask_interrupt(CODEC_INT_CHANNEL, CODEC_INT_SRC);	//Unmask a FIFO interrupt
#else
	hal_codec_unmask_interrupt(CODEC_DAC_INT_CHANNEL, CODEC_DAC_INT_SRC);	//Unmask a FIFO interrupt
#endif

	hal_codec_unmask_sc_irq();

	codec_register_cb(CODEC_CALLBACK_DAC, playback->attr->isrCb);

	uart_printf("DAC init.\r\n");

	return DRV_STATUS_OK;
}

drv_status_t adc12_cfg_init(codec_adc12_config_t *config, auSlotSize_t bitSlot)
{
	codec_set_operation_mode(CODEC_OP_MODE_VOICE_OPEN);

#if (SUPPORT_ALTOTOOL_CODEDC)
	hal_codec_set_adc_data_mode(gAdcCfg.gCfg[ADC_12].wordLength);
#else
	hal_codec_set_adc_data_mode(1);
#endif

	codec_config_adc12(config);

	hal_codec_fifo_init(HAL_CODEC_FIFO_ADC12, CODEC_FIFO_AF_LEVEL);

#if !CODEC_USE_ONE_IRQ
	hal_codec_unmask_interrupt(CODEC_ADC_INT_CHANNEL, CODEC_ADC_INT_SRC);	//Unmask a FIFO interrupt
#endif

	return DRV_STATUS_OK;
}
drv_status_t codec_adc12_init(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	if(codecDevInfos[CODEC_CALLBACK_ADC12].initialized)
		return DRV_STATUS_OK;

	codecDevInfos[CODEC_CALLBACK_ADC12].initialized = true;
	codecDevInfos[CODEC_CALLBACK_ADC12].bitSlot = capture->format->bitSlot;
	codecDevInfos[CODEC_CALLBACK_ADC12].channel = capture->format->channlesPerFrame;
	codecDevInfos[CODEC_CALLBACK_ADC12].wordSize = capture->attr->formatCap->wordSize;
	codecDevInfos[CODEC_CALLBACK_ADC12].cbArg = capture->attr->pStream;
	codecDevInfos[CODEC_CALLBACK_ADC12].calSwGainCb = capture->attr->calSwGainCb;
	codecDevInfos[CODEC_CALLBACK_ADC12].doEqCb = capture->attr->doEqCb;
	codecDevInfos[CODEC_CALLBACK_ADC12].useSwGain = capture->attr->useSwGain;
	codecDevInfos[CODEC_CALLBACK_ADC12].dataLen = CODEC_FIFO_AF_LEVEL*codecDevInfos[CODEC_CALLBACK_ADC12].channel*BIT_SLOT_32;

	codec_adc12_config_t *config = &g_adc12_config;
	config->sample_rate = capture->format->sampleRateHz;
	adc12_cfg_init(config, codecDevInfos[CODEC_CALLBACK_ADC12].bitSlot-2);

//	uart_printf("ADC12 init.\r\n");

#if !CODEC_ADC_ENABLE
	codec_register_cb(CODEC_CALLBACK_ADC12, capture->attr->isrCb);
#endif

	return DRV_STATUS_OK;
}

drv_status_t codec_adc34_init(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	if(codecDevInfos[CODEC_CALLBACK_ADC34].initialized)
		return DRV_STATUS_OK;

	codecDevInfos[CODEC_CALLBACK_ADC34].initialized = true;

	if(!codecDevInfos[CODEC_CALLBACK_ADC12].initialized)
	{
		g_adc12_config.sample_rate = capture->format->sampleRateHz;
		adc12_cfg_init(&g_adc12_config, capture->format->bitSlot-2);
	}
	
	codecDevInfos[CODEC_CALLBACK_ADC34].cbArg = capture->attr->pStream;
	codecDevInfos[CODEC_CALLBACK_ADC34].bitSlot = capture->format->bitSlot;
	codecDevInfos[CODEC_CALLBACK_ADC34].wordSize = capture->attr->formatCap->wordSize;
	codecDevInfos[CODEC_CALLBACK_ADC34].channel = capture->format->channlesPerFrame;
	codecDevInfos[CODEC_CALLBACK_ADC34].calSwGainCb = capture->attr->calSwGainCb;
	codecDevInfos[CODEC_CALLBACK_ADC34].doEqCb = capture->attr->doEqCb;
	codecDevInfos[CODEC_CALLBACK_ADC34].useSwGain = capture->attr->useSwGain;
	codecDevInfos[CODEC_CALLBACK_ADC34].dataLen = CODEC_FIFO_AF_LEVEL*codecDevInfos[CODEC_CALLBACK_ADC34].channel*BIT_SLOT_32;

	if(!codecDevInfos[CODEC_CALLBACK_ADC56].initialized)
	{
		codec_adc3456_config_t *config = &g_adc3456_config;
		config->sample_rate = capture->format->sampleRateHz;

		codec_config_adc3456(config);
	}

	codec_config_adc34(&g_adc34_config);
	hal_codec_fifo_init(HAL_CODEC_FIFO_ADC34, CODEC_FIFO_AF_LEVEL);

#if !CODEC_ADC_ENABLE
	codec_register_cb(CODEC_CALLBACK_ADC34, capture->attr->isrCb);
#endif

	if(!codecDevInfos[CODEC_CALLBACK_ADC12].initialized)
	{
		codec_adc12_start();
		codecDevInfos[CODEC_CALLBACK_ADC12].initialized = true;
	}

//	uart_printf("ADC34 init.\r\n");

	return DRV_STATUS_OK;
}

drv_status_t codec_adc56_init(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	if(codecDevInfos[CODEC_CALLBACK_ADC56].initialized)
		return DRV_STATUS_OK;

	codecDevInfos[CODEC_CALLBACK_ADC56].initialized = true;
	
	if(!codecDevInfos[CODEC_CALLBACK_ADC12].initialized)
	{
		g_adc12_config.sample_rate = capture->format->sampleRateHz;
		adc12_cfg_init(&g_adc12_config, capture->format->bitSlot-2);
	}
	
	codecDevInfos[CODEC_CALLBACK_ADC56].cbArg = capture->attr->pStream;
	codecDevInfos[CODEC_CALLBACK_ADC56].bitSlot = capture->format->bitSlot;	
	codecDevInfos[CODEC_CALLBACK_ADC56].wordSize = capture->attr->formatCap->wordSize;
	codecDevInfos[CODEC_CALLBACK_ADC56].channel = capture->format->channlesPerFrame;
	codecDevInfos[CODEC_CALLBACK_ADC56].calSwGainCb = capture->attr->calSwGainCb;
	codecDevInfos[CODEC_CALLBACK_ADC56].doEqCb = capture->attr->doEqCb;
	codecDevInfos[CODEC_CALLBACK_ADC56].useSwGain = capture->attr->useSwGain;
	codecDevInfos[CODEC_CALLBACK_ADC56].dataLen = CODEC_FIFO_AF_LEVEL*codecDevInfos[CODEC_CALLBACK_ADC56].channel*BIT_SLOT_32;

	if(!codecDevInfos[CODEC_CALLBACK_ADC34].initialized)
	{
		codec_adc3456_config_t *config = &g_adc3456_config;
		config->sample_rate = capture->format->sampleRateHz;

		codec_config_adc3456(config);
	}

	codec_config_adc56(&g_adc56_config);
	hal_codec_fifo_init(HAL_CODEC_FIFO_ADC56, CODEC_FIFO_AF_LEVEL);

#if !CODEC_ADC_ENABLE
	codec_register_cb(CODEC_CALLBACK_ADC56, capture->attr->isrCb);
#endif

	if(!codecDevInfos[CODEC_CALLBACK_ADC12].initialized)
	{
		codec_adc12_start();
		codecDevInfos[CODEC_CALLBACK_ADC12].initialized = true;
	}

	return DRV_STATUS_OK;
}

drv_status_t codec_adc78_init(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	if(codecDevInfos[CODEC_CALLBACK_ADC78].initialized)
		return DRV_STATUS_OK;

	codecDevInfos[CODEC_CALLBACK_ADC78].initialized = true;

	if(!codecDevInfos[CODEC_CALLBACK_ADC12].initialized)
	{
		g_adc12_config.sample_rate = capture->format->sampleRateHz;
		adc12_cfg_init(&g_adc12_config, capture->format->bitSlot-2);
	}

	codecDevInfos[CODEC_CALLBACK_ADC78].cbArg = capture->attr->pStream;
	codecDevInfos[CODEC_CALLBACK_ADC78].bitSlot = capture->format->bitSlot;
	codecDevInfos[CODEC_CALLBACK_ADC78].wordSize = capture->attr->formatCap->wordSize;
	codecDevInfos[CODEC_CALLBACK_ADC78].channel = capture->format->channlesPerFrame;
	codecDevInfos[CODEC_CALLBACK_ADC78].calSwGainCb = capture->attr->calSwGainCb;
	codecDevInfos[CODEC_CALLBACK_ADC78].doEqCb = capture->attr->doEqCb;
	codecDevInfos[CODEC_CALLBACK_ADC78].useSwGain = capture->attr->useSwGain;
	codecDevInfos[CODEC_CALLBACK_ADC78].dataLen = CODEC_FIFO_AF_LEVEL*codecDevInfos[CODEC_CALLBACK_ADC78].channel*BIT_SLOT_32;

	if(!codecDevInfos[CODEC_CALLBACK_ADC78].initialized)
	{
		codec_adc789a_config_t *config = &g_adc789a_config;
		config->sample_rate = capture->format->sampleRateHz;

		codec_config_adc789a(config);
	}

	codec_config_adc78(&g_adc78_config);
	hal_codec_fifo_init(HAL_CODEC_FIFO_ADC78, CODEC_FIFO_AF_LEVEL);

#if !CODEC_ADC_ENABLE
	codec_register_cb(CODEC_CALLBACK_ADC78, capture->attr->isrCb);
#endif

	if(!codecDevInfos[CODEC_CALLBACK_ADC12].initialized)
	{
		codec_adc12_start();
		codecDevInfos[CODEC_CALLBACK_ADC12].initialized = true;
	}

	return DRV_STATUS_OK;
}

drv_status_t codec_adc9a_init(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	if(codecDevInfos[CODEC_CALLBACK_ADC9A].initialized)
		return DRV_STATUS_OK;

	codecDevInfos[CODEC_CALLBACK_ADC9A].initialized = true;

	if(!codecDevInfos[CODEC_CALLBACK_ADC12].initialized)
	{
		g_adc12_config.sample_rate = capture->format->sampleRateHz;
		adc12_cfg_init(&g_adc12_config, capture->format->bitSlot-2);
	}

	codecDevInfos[CODEC_CALLBACK_ADC9A].cbArg = capture->attr->pStream;
	codecDevInfos[CODEC_CALLBACK_ADC9A].bitSlot = capture->format->bitSlot;
	codecDevInfos[CODEC_CALLBACK_ADC9A].wordSize = capture->attr->formatCap->wordSize;
	codecDevInfos[CODEC_CALLBACK_ADC9A].channel = capture->format->channlesPerFrame;
	codecDevInfos[CODEC_CALLBACK_ADC9A].calSwGainCb = capture->attr->calSwGainCb;
	codecDevInfos[CODEC_CALLBACK_ADC9A].doEqCb = capture->attr->doEqCb;
	codecDevInfos[CODEC_CALLBACK_ADC9A].useSwGain = capture->attr->useSwGain;
	codecDevInfos[CODEC_CALLBACK_ADC9A].dataLen = CODEC_FIFO_AF_LEVEL*codecDevInfos[CODEC_CALLBACK_ADC9A].channel*BIT_SLOT_32;

	if(!codecDevInfos[CODEC_CALLBACK_ADC9A].initialized)
	{
		codec_adc789a_config_t *config = &g_adc789a_config;
		config->sample_rate = capture->format->sampleRateHz;

		codec_config_adc789a(config);
	}

	codec_config_adc9a(&g_adc9a_config);
	hal_codec_fifo_init(HAL_CODEC_FIFO_ADC9A, CODEC_FIFO_AF_LEVEL);

#if !CODEC_ADC_ENABLE
	codec_register_cb(CODEC_CALLBACK_ADC9A, capture->attr->isrCb);
#endif

	if(!codecDevInfos[CODEC_CALLBACK_ADC12].initialized)
	{
		codec_adc12_start();
		codecDevInfos[CODEC_CALLBACK_ADC12].initialized = true;
	}

	return DRV_STATUS_OK;
}

drv_status_t codec_dac_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
#if CODEC_USE_ONE_IRQ
	hal_interrupt_disable_irq(CODEC_IRQ);
#else
	hal_interrupt_disable_irq(CODEC_DAC_IRQ);
#endif

	playback->attr->isrCb = NULL;	
	codec_dac_stop();

	return DRV_STATUS_OK;
}

#if (CODEC_ADC_ENABLE)
drv_status_t codec_adc_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
#if !CODEC_USE_ONE_IRQ
	hal_interrupt_disable_irq(CODEC_ADC_IRQ);
#endif

	codec_adc_stop();

	return DRV_STATUS_OK;
}
#endif

drv_status_t codec_adc12_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
#if !CODEC_USE_ONE_IRQ
	hal_interrupt_disable_irq(CODEC_ADC_IRQ);
#endif

	capture->attr->isrCb = NULL;
	codec_adc12_stop();

	return DRV_STATUS_OK;
}

drv_status_t codec_adc34_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	capture->attr->isrCb = NULL;
	codec_adc34_stop();

	return DRV_STATUS_OK;
}

drv_status_t codec_adc56_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	capture->attr->isrCb = NULL;
	codec_adc56_stop();

	return DRV_STATUS_OK;
}

drv_status_t codec_adc78_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	capture->attr->isrCb = NULL;
	codec_adc78_stop();

	return DRV_STATUS_OK;
}

drv_status_t codec_adc9a_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	capture->attr->isrCb = NULL;
	codec_adc9a_stop();

	return DRV_STATUS_OK;
}

drv_status_t codec_dac_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	codec_dac_stop();

	codec_dac_init(capture, playback);
	
	return DRV_STATUS_OK;
}

#if (CODEC_ADC_ENABLE)
drv_status_t codec_adc_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	codec_adc_stop();

	codec_adc_init(capture, playback);

	return DRV_STATUS_OK;
}
#endif

drv_status_t codec_adc12_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	codec_adc12_stop();

	codec_adc12_init(capture, playback);

	return DRV_STATUS_OK;
}

drv_status_t codec_adc34_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	codec_adc34_stop();

	codec_adc34_init(capture, playback);

	return DRV_STATUS_OK;
}

drv_status_t codec_adc56_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	codec_adc56_stop();

	codec_adc56_init(capture, playback);

	return DRV_STATUS_OK;
}

drv_status_t codec_adc78_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	codec_adc78_stop();

	codec_adc78_init(capture, playback);

	return DRV_STATUS_OK;
}

drv_status_t codec_adc9a_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	codec_adc9a_stop();

	codec_adc9a_init(capture, playback);

	return DRV_STATUS_OK;
}

#if (CODEC_ADC_ENABLE)
/**
 * @brief This function is used to initialize Codec ADC12 audio interface
 * @param[in] fs The sampling rate is set. This parameter can only be one of the array #codec_fs_table
 * @return
 * 	@retval 0 Codec ADC12 is successfully initialized.
 */
drv_status_t adc12_init(void)
{
	codec_adc12_config_t *config = &g_adc12_config;

	config->sample_rate = codecDevInfos[CODEC_CALLBACK_ADC].sampleRate;

	codec_config_adc12(config);

	hal_codec_fifo_init(HAL_CODEC_FIFO_ADC12, CODEC_FIFO_AF_LEVEL);

#if !CODEC_USE_ONE_IRQ
	hal_codec_unmask_interrupt(CODEC_ADC_INT_CHANNEL, CODEC_ADC_INT_SRC);	//Unmask a FIFO interrupt
#endif

	return DRV_STATUS_OK;
}

/**
 * @brief This function is used to initialize Codec ADC3456 common setting.
 * @param[in] fs The sampling rate is set. This parameter can only be one of the array #codec_fs_table[]
 * @return
 * 	@retval 0 Codec ADC3456 common setting is successfully initialized.
 */
drv_status_t adc3456_common_init(void)
{
	codec_adc3456_config_t *config = &g_adc3456_config;

	config->sample_rate = codecDevInfos[CODEC_CALLBACK_ADC].sampleRate;

	codec_config_adc3456(config);

	return DRV_STATUS_OK;
}

/**
 * @brief This function is used to initialize Codec ADC34 audio interface
 * @note This function should be called after calling #codec_adc3456_init()
 * @return
 * 	@retval 0 Codec ADC34 is successfully initialized.
 */
drv_status_t adc34_init(void)
{
	codec_config_adc34(&g_adc34_config);
	hal_codec_fifo_init(HAL_CODEC_FIFO_ADC34, CODEC_FIFO_AF_LEVEL);

	codec_adc34_start();

	return DRV_STATUS_OK;
}

/**
 * @brief This function is used to initialize Codec ADC56 audio interface
 * @note This function should be called after calling #codec_adc3456_init()
 * @return
 * 	@retval 0 Codec ADC56 is successfully initialized.
 */
drv_status_t adc56_init(void)
{
	codec_config_adc56(&g_adc56_config);
	hal_codec_fifo_init(HAL_CODEC_FIFO_ADC56, CODEC_FIFO_AF_LEVEL);

	codec_adc56_start();

	return DRV_STATUS_OK;
}

/**
 * @brief This function is used to initialize Codec ADC789a common setting.
 * @param[in] fs The sampling rate is set. This parameter can only be one of the array #codec_fs_table[]
 * @return
 * 	@retval 0 Codec ADC789a common setting is successfully initialized.
 */
drv_status_t adc789a_common_init(void)
{
	codec_adc789a_config_t *config = &g_adc789a_config;
	config->sample_rate = codecDevInfos[CODEC_CALLBACK_ADC].sampleRate;

	codec_config_adc789a(config);

	return DRV_STATUS_OK;
}

/**
 * @brief This function is used to initialize Codec ADC78 audio interface
 * @note This function should be called after calling #codec_adc789a_init()
 * @return
 * 	@retval 0 Codec ADC78 is successfully initialized.
 */
drv_status_t adc78_init(void)
{
	codec_config_adc78(&g_adc78_config);
	hal_codec_fifo_init(HAL_CODEC_FIFO_ADC78, CODEC_FIFO_AF_LEVEL);

	codec_adc78_start();

	return DRV_STATUS_OK;
}

/**
 * @brief This function is used to initialize Codec ADC9a audio interface
 * @note This function should be called after calling #codec_adc789a_init()
 * @return
 * 	@retval 0 Codec ADC9a is successfully initialized.
 */
drv_status_t adc9a_init(void)
{
	codec_config_adc9a(&g_adc9a_config);
	hal_codec_fifo_init(HAL_CODEC_FIFO_ADC9A, CODEC_FIFO_AF_LEVEL);

	codec_adc9a_start();

	return DRV_STATUS_OK;
}
#endif

auSlotSize_t codec_reset_adc_bitslot(auSlotSize_t bitslot)
{
	//auSlotSize_t slotSize;

	hal_codec_set_adc_data_mode(1);
//	uart_printf("adc setBitSlot:%d\n", bitslot);

	return BIT_SLOT_32;
}

auSlotSize_t codec_reset_dac_bitslot(auSlotSize_t bitslot)
{
	return BIT_SLOT_32;
}
#if (SUPPORT_ALTOTOOL)
void codec_adc_mute_channel(auChannel_t channel, int8_t mode, int32_t *adcData)
{
	if(mode == CODEC_ADC_DATA_MODE_16BIT)
	{
		int16_t *adcData16 =  (int16_t *)adcData;
		adcData16[channel] = 0;
	}else
	{
		adcData[channel] = 0;
	}
}

void codec_adc_mute_channels(int32_t *adcData)
{
	for(int i= ADC_1; i<=ADC_10; i++)
	{
		if(gAdcCfg.chxCfg[i].mute)
		{
			 int32_t *startData = adcData+CODEC_FIFO_AF_LEVEL*(i%2==0? i:i-1) +i%2;
			 for(int j=0; j<CODEC_FIFO_AF_LEVEL;j++)
			 {
			 	*(startData + 2*j) = 0;
			 }
		}
	}
}
#endif
void code_adc_read_data(void)
{
#if (CODEC_ADC_ENABLE)
	int32_t adcData[CODEC_ADC_NUM*CODEC_FIFO_AF_LEVEL] = {0};
	int32_t temp = 0;
#else
	int32_t adc12Data[2*CODEC_FIFO_AF_LEVEL] = {0};
#if CODEC_ADC34_ENABLE
	int32_t adc34Data[2*CODEC_FIFO_AF_LEVEL] = {0};
#endif
#if CODEC_ADC56_ENABLE
	int32_t adc56Data[2*CODEC_FIFO_AF_LEVEL] = {0};
#endif
#if CODEC_ADC78_ENABLE
	int32_t adc78Data[2*CODEC_FIFO_AF_LEVEL] = {0};
#endif
#if CODEC_ADC9A_ENABLE
	int32_t adc9aData[2*CODEC_FIFO_AF_LEVEL] = {0};
#endif
#endif

	uint8_t adcCount = 0;

#if (CODEC_ADC_ENABLE)
	switch(codecDevInfos[CODEC_CALLBACK_ADC].channel)
	{
		case 1:
			while(adcCount < CODEC_FIFO_AF_LEVEL)
			{
				hal_codec_read_adc12_data_24bit(&adcData[adcCount], &temp);

				adcCount += 1;
			}
			break;
		case 2:
			while(adcCount < CODEC_FIFO_AF_LEVEL * 2)
			{
				hal_codec_read_adc12_data_24bit(&adcData[adcCount], &adcData[adcCount+1]);
#if (CODEC_ADC12_ENABLE)// gui tool function
				if (codecDevInfos[CODEC_CALLBACK_ADC].doEqCb)
					codecDevInfos[CODEC_CALLBACK_ADC].doEqCb(adcData+adcCount, ((auStream_t *)codecDevInfos[CODEC_CALLBACK_ADC].cbArg)->format->sampleRateHz);
#endif
				adcCount += 2;
			}
			break;
		case 3:
			while(adcCount < CODEC_FIFO_AF_LEVEL * 3)
			{
				hal_codec_read_adc12_data_24bit(&adcData[adcCount], &adcData[adcCount+1]);
				hal_codec_read_adc34_data_24bit(&adcData[adcCount+2], &temp);

				adcCount += 3;
			}
			break;
		case 4:
			while(adcCount < CODEC_FIFO_AF_LEVEL * 4)
			{
				hal_codec_read_adc12_data_24bit(&adcData[adcCount], &adcData[adcCount+1]);
				hal_codec_read_adc34_data_24bit(&adcData[adcCount+2], &adcData[adcCount+3]);
				adcCount += 4;
			}
			break;
		case 5:
			while(adcCount < CODEC_FIFO_AF_LEVEL * 5)
			{
				hal_codec_read_adc12_data_24bit(&adcData[adcCount], &adcData[adcCount+1]);
				hal_codec_read_adc34_data_24bit(&adcData[adcCount+2], &adcData[adcCount+3]);
				hal_codec_read_adc56_data_24bit(&adcData[adcCount+4], &temp);
				adcCount += 5;
			}
			break;
		case 6:
			while(adcCount < CODEC_FIFO_AF_LEVEL * 6)
			{
				hal_codec_read_adc12_data_24bit(&adcData[adcCount], &adcData[adcCount+1]);
				hal_codec_read_adc34_data_24bit(&adcData[adcCount+2], &adcData[adcCount+3]);
				hal_codec_read_adc56_data_24bit(&adcData[adcCount+4], &adcData[adcCount+5]);
				adcCount += 6;
			}
			break;
		case 7:
			while(adcCount < CODEC_FIFO_AF_LEVEL * 7)
			{
				hal_codec_read_adc12_data_24bit(&adcData[adcCount], &adcData[adcCount+1]);
				hal_codec_read_adc34_data_24bit(&adcData[adcCount+2], &adcData[adcCount+3]);
				hal_codec_read_adc56_data_24bit(&adcData[adcCount+4], &adcData[adcCount+5]);
				hal_codec_read_adc78_data_24bit(&adcData[adcCount+6], &temp);
				adcCount += 7;
			}
			break;
		case 8:
			while(adcCount < CODEC_FIFO_AF_LEVEL * 8)
			{
				hal_codec_read_adc12_data_24bit(&adcData[adcCount], &adcData[adcCount+1]);
				hal_codec_read_adc34_data_24bit(&adcData[adcCount+2], &adcData[adcCount+3]);
				hal_codec_read_adc56_data_24bit(&adcData[adcCount+4], &adcData[adcCount+5]);
				hal_codec_read_adc78_data_24bit(&adcData[adcCount+6], &adcData[adcCount+7]);
				adcCount += 8;
			}
			break;
		case 9:
			while(adcCount < CODEC_FIFO_AF_LEVEL * 9)
			{
				hal_codec_read_adc12_data_24bit(&adcData[adcCount], &adcData[adcCount+1]);
				hal_codec_read_adc34_data_24bit(&adcData[adcCount+2], &adcData[adcCount+3]);
				hal_codec_read_adc56_data_24bit(&adcData[adcCount+4], &adcData[adcCount+5]);
				hal_codec_read_adc78_data_24bit(&adcData[adcCount+6], &adcData[adcCount+7]);
				hal_codec_read_adc9a_data_24bit(&adcData[adcCount+8], &temp);
				adcCount += 9;
			}
			break;
		case 10:
			while(adcCount < CODEC_FIFO_AF_LEVEL * 10)
			{
				hal_codec_read_adc12_data_24bit(&adcData[adcCount], &adcData[adcCount+1]);
				hal_codec_read_adc34_data_24bit(&adcData[adcCount+2], &adcData[adcCount+3]);
				hal_codec_read_adc56_data_24bit(&adcData[adcCount+4], &adcData[adcCount+5]);
				hal_codec_read_adc78_data_24bit(&adcData[adcCount+6], &adcData[adcCount+7]);
				hal_codec_read_adc9a_data_24bit(&adcData[adcCount+8], &adcData[adcCount+9]);
				adcCount += 10;
			}
			break;
		default:
			uart_printf("Adc config error in xxx_session_desc_xxx.c\n");
			break;
	}
#if (SUPPORT_ALTOTOOL)
	codec_adc_mute_channels(&adcData[0]);
#endif
	if (codecDevInfos[CODEC_CALLBACK_ADC].isrCb)
		codecDevInfos[CODEC_CALLBACK_ADC].isrCb(codecDevInfos[CODEC_CALLBACK_ADC].cbArg,
		&adcData[0],
		&codecDevInfos[CODEC_CALLBACK_ADC].dataLen,
		BIT_SLOT_32);
#else
	while(adcCount < 2 * CODEC_FIFO_AF_LEVEL)
	{
		hal_codec_read_adc12_data_24bit(&adc12Data[adcCount], &adc12Data[adcCount+1]);
#if (SUPPORT_ALTOTOOL_CODEDC)
		if(gAdcCfg.chxCfg[ADC_1].mute)
			adc12Data[adcCount] = 0;
		if(gAdcCfg.chxCfg[ADC_2].mute)
			adc12Data[adcCount + 1] = 0;
#endif
#if (CODEC_ADC12_ENABLE)
		if (codecDevInfos[CODEC_CALLBACK_ADC12].useSwGain && codecDevInfos[CODEC_CALLBACK_ADC12].calSwGainCb)
			codecDevInfos[CODEC_CALLBACK_ADC12].calSwGainCb(((auStream_t *)codecDevInfos[CODEC_CALLBACK_ADC12].cbArg)->var.swgain,
															(int32_t*)adc12Data+adcCount,
															(int32_t*)adc12Data+adcCount);

		if (codecDevInfos[CODEC_CALLBACK_ADC12].doEqCb)
			codecDevInfos[CODEC_CALLBACK_ADC12].doEqCb(adc12Data+adcCount, ((auStream_t *)codecDevInfos[CODEC_CALLBACK_ADC12].cbArg)->format->sampleRateHz);
#endif

#if (CODEC_ADC34_ENABLE)
		hal_codec_read_adc34_data_24bit(&adc34Data[adcCount], &adc34Data[adcCount+1]);
#if (SUPPORT_ALTOTOOL_CODEDC)
		if(gAdcCfg.chxCfg[ADC_3].mute)
			adc34Data[adcCount] = 0;
		if(gAdcCfg.chxCfg[ADC_4].mute)
			adc34Data[adcCount + 1] = 0;
#endif
		if (codecDevInfos[CODEC_CALLBACK_ADC34].useSwGain && codecDevInfos[CODEC_CALLBACK_ADC34].calSwGainCb)
			codecDevInfos[CODEC_CALLBACK_ADC34].calSwGainCb(((auStream_t *)codecDevInfos[CODEC_CALLBACK_ADC34].cbArg)->var.swgain,
															(int32_t*)adc34Data+adcCount,
															(int32_t*)adc34Data+adcCount);

		if (codecDevInfos[CODEC_CALLBACK_ADC34].doEqCb)
			codecDevInfos[CODEC_CALLBACK_ADC34].doEqCb(adc34Data+adcCount, ((auStream_t *)codecDevInfos[CODEC_CALLBACK_ADC34].cbArg)->format->sampleRateHz);
#endif

#if (CODEC_ADC56_ENABLE)
		hal_codec_read_adc56_data_24bit(&adc56Data[adcCount], &adc56Data[adcCount+1]);
#if (SUPPORT_ALTOTOOL_CODEDC)
		if(gAdcCfg.chxCfg[ADC_5].mute)
			adc56Data[adcCount] = 0;
		if(gAdcCfg.chxCfg[ADC_6].mute)
			adc56Data[adcCount + 1] = 0;
#endif
		if (codecDevInfos[CODEC_CALLBACK_ADC56].useSwGain && codecDevInfos[CODEC_CALLBACK_ADC56].calSwGainCb)
			codecDevInfos[CODEC_CALLBACK_ADC56].calSwGainCb(((auStream_t *)codecDevInfos[CODEC_CALLBACK_ADC56].cbArg)->var.swgain,
															(int32_t*)adc56Data+adcCount,
															(int32_t*)adc56Data+adcCount);

		if (codecDevInfos[CODEC_CALLBACK_ADC56].doEqCb)
			codecDevInfos[CODEC_CALLBACK_ADC56].doEqCb(adc56Data+adcCount, ((auStream_t *)codecDevInfos[CODEC_CALLBACK_ADC56].cbArg)->format->sampleRateHz);
#endif

#if (CODEC_ADC78_ENABLE)
		hal_codec_read_adc78_data_24bit(&adc78Data[adcCount], &adc78Data[adcCount+1]);
#if (SUPPORT_ALTOTOOL_CODEDC)
		if(gAdcCfg.chxCfg[ADC_7].mute)
			adc78Data[adcCount] = 0;
		if(gAdcCfg.chxCfg[ADC_8].mute)
			adc78Data[adcCount + 1] = 0;
#endif
		if (codecDevInfos[CODEC_CALLBACK_ADC78].useSwGain && codecDevInfos[CODEC_CALLBACK_ADC78].calSwGainCb)
			codecDevInfos[CODEC_CALLBACK_ADC78].calSwGainCb(((auStream_t *)codecDevInfos[CODEC_CALLBACK_ADC78].cbArg)->var.swgain,
															(int32_t*)adc78Data+adcCount,
															(int32_t*)adc78Data+adcCount);

		if (codecDevInfos[CODEC_CALLBACK_ADC78].doEqCb)
			codecDevInfos[CODEC_CALLBACK_ADC78].doEqCb(adc78Data+adcCount, ((auStream_t *)codecDevInfos[CODEC_CALLBACK_ADC78].cbArg)->format->sampleRateHz);
#endif

#if (CODEC_ADC9A_ENABLE)
		hal_codec_read_adc9a_data_24bit(&adc9aData[adcCount], &adc9aData[adcCount+1]);
#if (SUPPORT_ALTOTOOL_CODEDC)
		if(gAdcCfg.chxCfg[ADC_9].mute)
			adc9aData[adcCount] = 0;
		if(gAdcCfg.chxCfg[ADC_10].mute)
			adc9aData[adcCount + 1] = 0;
#endif
		if (codecDevInfos[CODEC_CALLBACK_ADC9A].useSwGain && codecDevInfos[CODEC_CALLBACK_ADC9A].calSwGainCb)
			codecDevInfos[CODEC_CALLBACK_ADC9A].calSwGainCb(((auStream_t *)codecDevInfos[CODEC_CALLBACK_ADC9A].cbArg)->var.swgain,
															(int32_t*)adc9aData+adcCount,
															(int32_t*)adc9aData+adcCount);

		if (codecDevInfos[CODEC_CALLBACK_ADC9A].doEqCb)
			codecDevInfos[CODEC_CALLBACK_ADC9A].doEqCb(adc9aData+adcCount, ((auStream_t *)codecDevInfos[CODEC_CALLBACK_ADC9A].cbArg)->format->sampleRateHz);
#endif

		adcCount += 2;
	}

#if (CODEC_ADC12_ENABLE)
	if (codecDevInfos[CODEC_CALLBACK_ADC12].isrCb)
		codecDevInfos[CODEC_CALLBACK_ADC12].isrCb(codecDevInfos[CODEC_CALLBACK_ADC12].cbArg,
		&adc12Data[0],
		&codecDevInfos[CODEC_CALLBACK_ADC12].dataLen,
		BIT_SLOT_32);
#endif

#if (CODEC_ADC34_ENABLE)
	if (codecDevInfos[CODEC_CALLBACK_ADC34].isrCb)
		codecDevInfos[CODEC_CALLBACK_ADC34].isrCb(codecDevInfos[CODEC_CALLBACK_ADC34].cbArg,
		&adc34Data[0],
		&codecDevInfos[CODEC_CALLBACK_ADC34].dataLen,
		BIT_SLOT_32);
#endif

#if (CODEC_ADC56_ENABLE)
	if (codecDevInfos[CODEC_CALLBACK_ADC56].isrCb)
		codecDevInfos[CODEC_CALLBACK_ADC56].isrCb(codecDevInfos[CODEC_CALLBACK_ADC56].cbArg,
		&adc56Data[0],
		&codecDevInfos[CODEC_CALLBACK_ADC56].dataLen,
		BIT_SLOT_32);
#endif


#if (CODEC_ADC78_ENABLE)
	if (codecDevInfos[CODEC_CALLBACK_ADC78].isrCb)
		codecDevInfos[CODEC_CALLBACK_ADC78].isrCb(codecDevInfos[CODEC_CALLBACK_ADC78].cbArg,
		&adc78Data[0],
		&codecDevInfos[CODEC_CALLBACK_ADC78].dataLen,
		BIT_SLOT_32);
#endif

#if (CODEC_ADC9A_ENABLE)
	if (codecDevInfos[CODEC_CALLBACK_ADC9A].isrCb)
		codecDevInfos[CODEC_CALLBACK_ADC9A].isrCb(codecDevInfos[CODEC_CALLBACK_ADC9A].cbArg,
		&adc9aData[0],
		&codecDevInfos[CODEC_CALLBACK_ADC9A].dataLen,
		BIT_SLOT_32);
#endif

#endif
}

#if (!CODEC_USE_ONE_IRQ)
void codec_adc_isr_handler(void *arg)
{
	SaveVectors(CODEC_ADC_IRQ);

	hal_interrupt_clear_irq(CODEC_ADC_IRQ);

	if (hal_codec_get_interrupt_status(CODEC_ADC_INT_CHANNEL) & (1 << CODEC_ADC_INT_SRC))
	{
		code_adc_read_data();

		hal_codec_clear_interrupt(CODEC_ADC_INT_CHANNEL, CODEC_ADC_INT_SRC);
	}

	RestoreVectors(CODEC_ADC_IRQ);
}

extern volatile uint32_t feedSampleCnt;
void codec_dac_isr_handler(void *arg)
{
	int32_t dacData[2*CODEC_FIFO_AE_LEVEL] = {0};
	uint8_t dacCount = 0;

	/* When ROK of RAR/ESR/iLR1/iLR2 is not all 1, this means that the power is off,
	 * shutdown the codec firstly, to avoid popup noise.
	 * bit3: iLR2 ROK
	 * bit2: iLR1 ROK
	 * bit1: ESR ROK
	 * bit0: RAR ROK
	 */
	if ((read_reg(0x7700f040) & 0xf) != 0x0f)
	{
		hal_codec_analog_standby(true);
		hal_codec_analog_dac_sleep(true);
		hal_codec_digital_standby(true);
		hal_codec_analog_adc_sleep(true);
	}

	SaveVectors(CODEC_DAC_IRQ);

	hal_interrupt_clear_irq(CODEC_DAC_IRQ);

	if (hal_codec_get_interrupt_status(CODEC_DAC_INT_CHANNEL) & (1 << CODEC_DAC_INT_SRC))
	{
		uint32_t len = codecDevInfos[CODEC_CALLBACK_DAC].dataLen;

		if(codecDevInfos[CODEC_CALLBACK_DAC].isrCb)
			codecDevInfos[CODEC_CALLBACK_DAC].isrCb(codecDevInfos[CODEC_CALLBACK_DAC].cbArg,
													&dacData[0],
													&len,
													BIT_SLOT_32);

		while(dacCount < 2*CODEC_FIFO_AE_LEVEL)
		{
#if (SUPPORT_PLAY_TONE && D_DAC_PLAY_TONE_ENABLE)
			int32_t toneData;
			float fadeScale = 0;
#if (PREEMPTIVE_MODE == 1)
			float fadeScaleTone = 0;
			int readOffset = PlayToneGetData(PLAY_TONE_DAC, &toneData, &fadeScale, &fadeScaleTone, g_dac_config.sample_rate);
#else
			int readOffset = PlayToneGetData(PLAY_TONE_DAC, &toneData, &fadeScale, g_dac_config.sample_rate);
#endif
			if(readOffset >= 0)
			{
				if(fadeScale > 0)
				{
					dacData[dacCount] = fadeScale*dacData[dacCount];
					dacData[dacCount+1] = fadeScale*dacData[dacCount+1];
				}
				else
				{
				    #if (PREEMPTIVE_MODE == 1)
				    if (fadeScaleTone != 0)
	                {
					#if D_PLAY_TONE_OUTPUT_MIX
						dacData[dacCount] = toneData * fadeScaleTone + dacData[dacCount];
						dacData[dacCount+1] = toneData * fadeScaleTone + dacData[dacCount+1];
					#else
	                    dacData[dacCount] = toneData * fadeScaleTone;
	                    dacData[dacCount+1] = toneData * fadeScaleTone;
					#endif
	                }
	                else
	                #endif
	                {
	                #if D_PLAY_TONE_OUTPUT_MIX
	                    dacData[dacCount] = toneData + dacData[dacCount];
	                    dacData[dacCount+1] = toneData + dacData[dacCount+1];
	                #else
	                    dacData[dacCount] = toneData;
	                    dacData[dacCount+1] = toneData;
	                #endif
	                }
				}
			}
#endif
#ifndef TEST_I2S_DAC_SYNC
			if (codecDevInfos[CODEC_CALLBACK_DAC].useSwGain && codecDevInfos[CODEC_CALLBACK_DAC].calSwGainCb)
				codecDevInfos[CODEC_CALLBACK_DAC].calSwGainCb(((auStream_t *)codecDevInfos[CODEC_CALLBACK_DAC].cbArg)->var.swgain,
															   (int32_t*)dacData+dacCount,
															   (int32_t*)dacData+dacCount);

			if (codecDevInfos[CODEC_CALLBACK_DAC].doEqCb)
				codecDevInfos[CODEC_CALLBACK_DAC].doEqCb(dacData+dacCount, ((auStream_t *)codecDevInfos[CODEC_CALLBACK_DAC].cbArg)->format->sampleRateHz);
#endif
#if (SUPPORT_ALTOTOOL_CODEDC)
			bool muteL = gDacCfg.chxCfg[DAC_L].mute;
			bool muteR = gDacCfg.chxCfg[DAC_R].mute;
			hal_codec_write_dac_data(muteL ? 0: dacData[dacCount],muteR ? 0 : dacData[dacCount + 1]);
#else
			hal_codec_write_dac_data(dacData[dacCount], dacData[dacCount+1]);
#endif
			dacCount += 2;
		}

		hal_codec_clear_interrupt(CODEC_DAC_INT_CHANNEL, CODEC_DAC_INT_SRC);
	}

	RestoreVectors(CODEC_DAC_IRQ);
}

/**
 * @brief This function is the ISR of Short-Circuit Detection
 */
void codec_dac_sc_isr_handler(void *arg)
{
	SaveVectors(CODEC_DAC_SC_IRQ);

	hal_interrupt_clear_irq(CODEC_DAC_SC_IRQ);

	if (hal_codec_get_sc_irq_flag())
	{
		codec_dac_stop();
		codec_dac_restart();
	}

	hal_codec_clear_sc_irq_flag();

	RestoreVectors(CODEC_DAC_SC_IRQ);
}


#else
void codec_isr_handler(void *arg)
{
	int32_t dacData[2*CODEC_FIFO_AE_LEVEL] = {0};

	uint8_t dacCount = 0;

	SaveVectors(CODEC_IRQ);

	hal_interrupt_clear_irq(CODEC_IRQ);

	if (hal_codec_get_interrupt_status(CODEC_ADC_INT_CHANNEL) & (1 << CODEC_ADC_INT_SRC))
	{
		code_adc_read_data();

		uint32_t len = CODEC_FIFO_AE_LEVEL*codecDevInfos[CODEC_CALLBACK_DAC].channel*BIT_SLOT_32;
		if(codecDevInfos[CODEC_CALLBACK_DAC].isrCb)
				codecDevInfos[CODEC_CALLBACK_DAC].isrCb(codecDevInfos[CODEC_CALLBACK_DAC].cbArg,
				&dacData[0],
				&len,
				BIT_SLOT_32);

		while(dacCount < 2*CODEC_FIFO_AE_LEVEL)
		{
			hal_codec_write_dac_data(dacData[dacCount], dacData[dacCount+1]);
			dacCount += 2;
		}

		hal_codec_clear_interrupt(CODEC_INT_CHANNEL, CODEC_INT_SRC);
	}
	RestoreVectors(CODEC_IRQ);
}
#endif

#else
/** 
 * @brief This function is used to stop the Codec
 */
void codec_deinit(void)
{
#if CODEC_USE_ONE_IRQ
	hal_interrupt_disable_irq(CODEC_IRQ);
#else
	hal_interrupt_disable_irq(CODEC_ADC_IRQ);
	hal_interrupt_disable_irq(CODEC_DAC_IRQ);
#endif

	codec_adc12_stop();
	codec_adc34_stop();
	codec_adc56_stop();
	codec_adc78_stop();
	codec_adc9a_stop();

	codec_dac_stop();

	hal_sysctrl_set_clock_gate(hal_sysctrl_get_clock_gate() & (~(1 << HAL_SYSCTRL_CLKGT_CODEC)));
}

/**
 * @brief This function is used to initialize the Codec
 */
drv_status_t codec_init(void)
{
	codec_clk_init();

	codec_adc_init();

	codec_dac_init();

	return DRV_STATUS_OK;
}

/**
 * @brief This function is used to initialize the Codec dac
 */
drv_status_t codec_dac_init(void)
{
	codec_set_operation_mode(CODEC_OP_MODE_PLAYBACK_OPEN);

	codec_config_dac(&g_dac_config);

	hal_codec_fifo_init(HAL_CODEC_FIFO_DAC, CODEC_FIFO_AE_LEVEL);
#if CODEC_USE_ONE_IRQ
	hal_codec_unmask_interrupt(CODEC_INT_CHANNEL, CODEC_INT_SRC);	//Unmask a FIFO interrupt
#else
	hal_codec_unmask_interrupt(CODEC_DAC_INT_CHANNEL, CODEC_DAC_INT_SRC);	//Unmask a FIFO interrupt
#endif

	codec_dac_start();
	codec_register_cb(CODEC_CALLBACK_DAC, app_codec_dac_callback);

	return DRV_STATUS_OK;
}

/**
 * @brief This function is used to initialize the Codec adc
 */
drv_status_t codec_adc_init(void)
{
	codec_set_operation_mode(CODEC_OP_MODE_VOICE_OPEN);
#if (SUPPORT_ALTOTOOL_CODEDC)
	hal_codec_set_adc_data_mode(gAdcCfg.gCfg[ADC_12].wordLength);
#else
	hal_codec_set_adc_data_mode(HAL_CODEC_ADWL_24BIT);
#endif

#if CODEC_ADC12_ENABLE
	codec_adc12_init();		/* ADC12 initialize */
#endif

#if (CODEC_ADC34_ENABLE || CODEC_ADC56_ENABLE)
	codec_adc3456_init();	/* ADC34&56 common initialize */

#endif

#if CODEC_ADC34_ENABLE
	codec_adc34_init();
#endif

#if CODEC_ADC56_ENABLE
	codec_adc56_init();
#endif

#if (CODEC_ADC78_ENABLE || CODEC_ADC9A_ENABLE)
	codec_adc789a_init();	/* ADC78&9a common initialize */
#endif

#if CODEC_ADC78_ENABLE
	codec_adc78_init();

#endif

#if CODEC_ADC9A_ENABLE
	codec_adc9a_init();

#endif

	return DRV_STATUS_OK;
}

/**
 * @brief This function is used to initialize Codec ADC12 audio interface
 * @param[in] fs The sampling rate is set. This parameter can only be one of the array #codec_fs_table
 * @return
 * 	@retval 0 Codec ADC12 is successfully initialized.
 */
drv_status_t codec_adc12_init(void)
{
	codec_adc12_config_t *config = &g_adc12_config;

	codec_register_cb(CODEC_CALLBACK_ADC, app_codec_adc_callback);

	codec_config_adc12(config);

	hal_codec_fifo_init(HAL_CODEC_FIFO_ADC12, CODEC_FIFO_AF_LEVEL);

#if !CODEC_USE_ONE_IRQ
	hal_codec_unmask_interrupt(CODEC_ADC_INT_CHANNEL, CODEC_ADC_INT_SRC);	//Unmask a FIFO interrupt
#endif

	codec_adc12_start();

	return DRV_STATUS_OK;
}

/**
 * @brief This function is used to initialize Codec ADC3456 common setting.
 * @param[in] fs The sampling rate is set. This parameter can only be one of the array #codec_fs_table[]
 * @return
 * 	@retval 0 Codec ADC3456 common setting is successfully initialized.
 */
drv_status_t codec_adc3456_init(void)
{
	codec_adc3456_config_t *config = &g_adc3456_config;

	codec_config_adc3456(config);

	return DRV_STATUS_OK;
}

/**
 * @brief This function is used to initialize Codec ADC34 audio interface
 * @note This function should be called after calling #codec_adc3456_init()
 * @return
 * 	@retval 0 Codec ADC34 is successfully initialized.
 */
drv_status_t codec_adc34_init(void)
{
	codec_config_adc34(&g_adc34_config);

	hal_codec_fifo_init(HAL_CODEC_FIFO_ADC34, CODEC_FIFO_AF_LEVEL);

	codec_adc34_start();

	return DRV_STATUS_OK;
}

/**
 * @brief This function is used to initialize Codec ADC56 audio interface
 * @note This function should be called after calling #codec_adc3456_init()
 * @return
 * 	@retval 0 Codec ADC56 is successfully initialized.
 */
drv_status_t codec_adc56_init(void)
{
	codec_config_adc56(&g_adc56_config);

	hal_codec_fifo_init(HAL_CODEC_FIFO_ADC56, CODEC_FIFO_AF_LEVEL);

	codec_adc56_start();

	return DRV_STATUS_OK;
}

/**
 * @brief This function is used to initialize Codec ADC789a common setting.
 * @param[in] fs The sampling rate is set. This parameter can only be one of the array #codec_fs_table[]
 * @return
 * 	@retval 0 Codec ADC789a common setting is successfully initialized.
 */
drv_status_t codec_adc789a_init(void)
{
	codec_adc789a_config_t *config = &g_adc789a_config;

	codec_config_adc789a(config);

	return DRV_STATUS_OK;
}

/**
 * @brief This function is used to initialize Codec ADC78 audio interface
 * @note This function should be called after calling #codec_adc789a_init()
 * @return
 * 	@retval 0 Codec ADC78 is successfully initialized.
 */
drv_status_t codec_adc78_init(void)
{
	codec_config_adc78(&g_adc78_config);

	hal_codec_fifo_init(HAL_CODEC_FIFO_ADC78, CODEC_FIFO_AF_LEVEL);

	codec_adc78_start();

	return DRV_STATUS_OK;
}

/**
 * @brief This function is used to initialize Codec ADC9a audio interface
 * @note This function should be called after calling #codec_adc789a_init()
 * @return
 * 	@retval 0 Codec ADC9a is successfully initialized.
 */
drv_status_t codec_adc9a_init(void)
{
	codec_config_adc9a(&g_adc9a_config);

	hal_codec_fifo_init(HAL_CODEC_FIFO_ADC9A, CODEC_FIFO_AF_LEVEL);

	codec_adc9a_start();

	return DRV_STATUS_OK;
}

#if (!CODEC_USE_ONE_IRQ)
void codec_adc_isr_handler(void *arg)
{

	int32_t channel = CODEC_ADC_NUM;
	int32_t adcData[CODEC_ADC_NUM] = {0};

	SaveVectors(CODEC_ADC_IRQ);

	hal_interrupt_clear_irq(CODEC_ADC_IRQ);

	if (hal_codec_get_interrupt_status(CODEC_ADC_INT_CHANNEL) & (1 << CODEC_ADC_INT_SRC))
	{
		if(hal_codec_get_adc_data_mode() == CODEC_ADC_DATA_MODE_16BIT)
		{
#if CODEC_ADC12_ENABLE
			hal_codec_read_adc12_data_16bit(&adcData[0]);
#if (SUPPORT_ALTOTOOL)
			if(gAdcCfg.chxCfg[ADC_1].mute)
			{
				codec_adc_mute_channel(CHANNEL_L,CODEC_ADC_DATA_MODE_16BIT,adcData)
			}
			if (gAdcCfg.chxCfg[ADC_2].mute)
			{
				codec_adc_mute_channel(CHANNEL_R,CODEC_ADC_DATA_MODE_16BIT,adcData)
			}
#endif
#endif

#if CODEC_ADC34_ENABLE
			hal_codec_read_adc34_data_16bit(&adcData[1]);
#if (SUPPORT_ALTOTOOL)
			if(gAdcCfg.chxCfg[ADC_3].mute)
			{
				codec_adc_mute_channel(CHANNEL_L,CODEC_ADC_DATA_MODE_16BIT,&adcData[1])
			}
                        if (gAdcCfg.chxCfg[ADC_4].mute)
                        {
                        	codec_adc_mute_channel(CHANNEL_R,CODEC_ADC_DATA_MODE_16BIT,&adcData[1])
                        }
#endif
#endif

#if CODEC_ADC56_ENABLE
			hal_codec_read_adc56_data_16bit(&adcData[2]);
#if (SUPPORT_ALTOTOOL)
			if(gAdcCfg.chxCfg[ADC_5].mute)
			{
				codec_adc_mute_channel(CHANNEL_L,CODEC_ADC_DATA_MODE_16BIT,&adcData[2])
			}
			if (gAdcCfg.chxCfg[ADC_6].mute)
			{
				codec_adc_mute_channel(CHANNEL_R,CODEC_ADC_DATA_MODE_16BIT,&adcData[2])
			}
#endif
#endif

#if CODEC_ADC78_ENABLE
			hal_codec_read_adc78_data_16bit(&adcData[3]);
#if (SUPPORT_ALTOTOOL)
			if(gAdcCfg.chxCfg[ADC_7].mute)
			{
				codec_adc_mute_channel(CHANNEL_L,CODEC_ADC_DATA_MODE_16BIT,&adcData[3]);
			}
			if (gAdcCfg.chxCfg[ADC_8].mute)
			{
				codec_adc_mute_channel(CHANNEL_R,CODEC_ADC_DATA_MODE_16BIT,&adcData[3]);
			}
#endif
#endif

#if CODEC_ADC9A_ENABLE
			hal_codec_read_adc9a_data_16bit(&adcData[4]);
#if (SUPPORT_ALTOTOOL)
			if(gAdcCfg.chxCfg[ADC_9].mute)
			{
				codec_adc_mute_channel(CHANNEL_L,CODEC_ADC_DATA_MODE_16BIT,&adcData[4]);
			}
			if (gAdcCfg.chxCfg[ADC_10].mute)
			{
				codec_adc_mute_channel(CHANNEL_R,CODEC_ADC_DATA_MODE_16BIT,&adcData[4]);
			}
#endif
#endif
		}
		else
		{
#if CODEC_ADC12_ENABLE
			hal_codec_read_adc12_data_24bit(&adcData[0], &adcData[1]);
#if (SUPPORT_ALTOTOOL)
			if(gAdcCfg.chxCfg[ADC_1].mute)
			{
				codec_adc_mute_channel(CHANNEL_L,CODEC_ADC_DATA_MODE_24BIT,&adcData[0]);
			}
			if (gAdcCfg.chxCfg[ADC_2].mute)
			{
				codec_adc_mute_channel(CHANNEL_R,CODEC_ADC_DATA_MODE_24BIT,&adcData[0]);
			}
#endif
#endif

#if CODEC_ADC34_ENABLE
			hal_codec_read_adc34_data_24bit(&adcData[2], &adcData[3]);
#if (SUPPORT_ALTOTOOL)
			if(gAdcCfg.chxCfg[ADC_3].mute)
			{
				codec_adc_mute_channel(CHANNEL_L,CODEC_ADC_DATA_MODE_24BIT,&adcData[2]);
			}
			if (gAdcCfg.chxCfg[ADC_4].mute)
			{
				codec_adc_mute_channel(CHANNEL_R,CODEC_ADC_DATA_MODE_24BIT,&adcData[2]);
			}
#endif
#endif

#if CODEC_ADC56_ENABLE
			hal_codec_read_adc56_data_24bit(&adcData[4], &adcData[5]);
#if (SUPPORT_ALTOTOOL)
			if(gAdcCfg.chxCfg[ADC_5].mute)
			{
				codec_adc_mute_channel(CHANNEL_L,CODEC_ADC_DATA_MODE_24BIT,&adcData[4]);
			}
			if (gAdcCfg.chxCfg[ADC_6].mute)
			{
				codec_adc_mute_channel(CHANNEL_R,CODEC_ADC_DATA_MODE_24BIT,&adcData[4]);
			}
#endif
#endif

#if CODEC_ADC78_ENABLE
			hal_codec_read_adc78_data_24bit(&adcData[6], &adcData[7]);
#if (SUPPORT_ALTOTOOL)
			if(gAdcCfg.chxCfg[ADC_7].mute)
			{
				codec_adc_mute_channel(CHANNEL_L,CODEC_ADC_DATA_MODE_24BIT,&adcData[6]);
			}
			if (gAdcCfg.chxCfg[ADC_8].mute)
			{
				codec_adc_mute_channel(CHANNEL_R,CODEC_ADC_DATA_MODE_24BIT,&adcData[6]);
			}
#endif
#endif

#if CODEC_ADC9A_ENABLE
			hal_codec_read_adc9a_data_24bit(&adcData[8], &adcData[9]);
#if (SUPPORT_ALTOTOOL)
			if(gAdcCfg.chxCfg[ADC_9].mute)
			{
				codec_adc_mute_channel(CHANNEL_L,CODEC_ADC_DATA_MODE_24BIT,&adcData[8]);
			}
			if (gAdcCfg.chxCfg[ADC_10].mute)
			{
				codec_adc_mute_channel(CHANNEL_R,CODEC_ADC_DATA_MODE_24BIT,&adcData[8]);
			}
#endif
#endif
		}

		if (g_codec_cb_table[CODEC_CALLBACK_ADC])
			g_codec_cb_table[CODEC_CALLBACK_ADC](adcData, &channel);

		hal_codec_clear_interrupt(CODEC_ADC_INT_CHANNEL, CODEC_ADC_INT_SRC);
	}

	RestoreVectors(CODEC_ADC_IRQ);
}

void codec_dac_isr_handler(void *arg)
{
	int32_t dil = 0;
	int32_t dir = 0;

	/* When ROK of RAR/ESR/iLR1/iLR2 is not all 1, this means that the power is off,
	 * shutdown the codec firstly, to avoid popup noise.
	 * bit3: iLR2 ROK
	 * bit2: iLR1 ROK
	 * bit1: ESR ROK
	 * bit0: RAR ROK
	 */
	if ((read_reg(0x7700f040) & 0xf) != 0x0f)
	{
		hal_codec_analog_standby(true);
		hal_codec_analog_dac_sleep(true);
		hal_codec_digital_standby(true);
		hal_codec_analog_adc_sleep(true);
	}

	SaveVectors(CODEC_DAC_IRQ);

	hal_interrupt_clear_irq(CODEC_DAC_IRQ);

	if (hal_codec_get_interrupt_status(CODEC_DAC_INT_CHANNEL) & (1 << CODEC_DAC_INT_SRC))
	{
		if (g_codec_cb_table[CODEC_CALLBACK_DAC])
			g_codec_cb_table[CODEC_CALLBACK_DAC](&dil, &dir);

		hal_codec_write_dac_data(dil, dir);

		hal_codec_clear_interrupt(CODEC_DAC_INT_CHANNEL, CODEC_DAC_INT_SRC);
	}

	RestoreVectors(CODEC_DAC_IRQ);
}
#else
#if(DMA_TEST_CODEC_ADC_ENABLE)
extern void dmac_adc_cb(uint8_t chx);
#endif

/**
 * @brief This function is the ISR of Codec
 */
void codec_isr_handler(void *arg)
{
	int32_t channel = CODEC_ADC_NUM;
	ALIGN(8) int32_t adcData[CODEC_ADC_NUM] = {0};

	int32_t dil = 0;
	int32_t dir = 0;

	/* When ROK of RAR/ESR/iLR1/iLR2 is not all 1, this means that the power is off,
	 * shutdown the codec firstly, to avoid popup noise.
	 * bit3: iLR2 ROK
	 * bit2: iLR1 ROK
	 * bit1: ESR ROK
	 * bit0: RAR ROK
	 */
	if ((read_reg(0x7700f040) & 0xf) != 0x0f)
	{
		hal_codec_analog_standby(true);
		hal_codec_analog_dac_sleep(true);
		hal_codec_digital_standby(true);
		hal_codec_analog_adc_sleep(true);
	}

	SaveVectors(CODEC_IRQ);
	hal_interrupt_clear_irq(CODEC_IRQ);

	if (hal_codec_get_interrupt_status(CODEC_INT_CHANNEL) & (1 << CODEC_INT_SRC))
	{
#if (DMA_TEST_CODEC_ADC_ENABLE)
		dmac_adc_cb(5);
#else
		if(hal_codec_get_adc_data_mode() == CODEC_ADC_DATA_MODE_16BIT)
		{
#if CODEC_ADC12_ENABLE
			hal_codec_read_adc12_data_16bit(&adcData[0]);
#endif

#if CODEC_ADC34_ENABLE
			hal_codec_read_adc34_data_16bit(&adcData[1]);
#endif

#if CODEC_ADC56_ENABLE
			hal_codec_read_adc56_data_16bit(&adcData[2]);
#endif

#if CODEC_ADC78_ENABLE
			hal_codec_read_adc78_data_16bit(&adcData[3]);
#endif

#if CODEC_ADC9A_ENABLE
			hal_codec_read_adc9a_data_16bit(&adcData[4]);
#endif
		}
		else
		{
#if CODEC_ADC12_ENABLE
			hal_codec_read_adc12_data_24bit(&adcData[0], &adcData[1]);
#endif

#if CODEC_ADC34_ENABLE
			hal_codec_read_adc34_data_24bit(&adcData[2], &adcData[3]);
#endif

#if CODEC_ADC56_ENABLE
			hal_codec_read_adc56_data_24bit(&adcData[4], &adcData[5]);
#endif

#if CODEC_ADC78_ENABLE
			hal_codec_read_adc78_data_24bit(&adcData[6], &adcData[7]);
#endif

#if CODEC_ADC9A_ENABLE
			hal_codec_read_adc9a_data_24bit(&adcData[8], &adcData[9]);
#endif
		}
#endif

	if (g_codec_cb_table[CODEC_CALLBACK_DAC])
		g_codec_cb_table[CODEC_CALLBACK_DAC](&dil, &dir);

	hal_codec_write_dac_data(dil, dir);

#if(!DMA_TEST_CODEC_ADC_ENABLE)
	if (g_codec_cb_table[CODEC_CALLBACK_ADC])
		g_codec_cb_table[CODEC_CALLBACK_ADC](adcData, &channel);
#endif

	hal_codec_clear_interrupt(CODEC_INT_CHANNEL, CODEC_INT_SRC);
	}
	RestoreVectors(CODEC_IRQ);
}
#endif
#endif

/** 
 * @brief This function is used to configure the codec DAC audio interface
 * @param[in] config DAC configuration information
 * @return
 * 	@retval DRV_STATUS_OK DAC is successfully configured.
 */
drv_status_t codec_config_dac(codec_dac_config_t *config)
{
#if (CODEC_MASTER == 0) && (CODEC_SLAVE_FRAC_SHARE == CODEC_SLAVE_FRAC_SHARE_DEDICATED)
	if (config->master_slave == CODEC_MODE_SLAVE)
		hal_clock_set_slave_dac_samplerate(config->sample_rate);
#endif

	hal_codec_mute_dac();

	hal_codec_set_dac_audioif_parallel();
	hal_codec_set_dac_master_slave_mode(config->master_slave);
	hal_codec_set_dac_sample_rate(codec_samplerate_transform(config->sample_rate));

	/* Select Audio output paths */
	hal_codec_set_dac_left_channel_only(config->left_only);
	hal_codec_select_dac_aidacl_input(config->aidacl_sel);
	hal_codec_select_dac_aidacr_input(config->aidacr_sel);
	hal_codec_select_dac_mixer_mode(config->dac_mix);
	hal_codec_select_dac_mixdacl_input(config->mixdacl_sel);
	hal_codec_select_dac_mixdacr_input(config->mixdacr_sel);
	hal_codec_select_dac_mixadc1_input(config->mixadc1_sel);
	hal_codec_select_dac_mixadc2_input(config->mixadc2_sel);

	/* Set Gains */
	hal_codec_set_dac_godl(config->godl);
	hal_codec_set_dac_godr(config->godr);
	hal_codec_set_dac_gomixl(config->gomixl);
	hal_codec_set_dac_gomixr(config->gomixr);

	if (config->dac_mix == HAL_CODEC_PLAYBACK_DAC_MIXED_ADC) {
		hal_codec_set_dac_gimixl(config->gimixl);
		hal_codec_set_dac_gimixr(config->gimixr);
	}
	hal_codec_set_hp_gol(config->gol);
	hal_codec_set_hp_gor(config->gor);
	hal_codec_select_hp_amp_input(config->hp_sel);

#if (CODEC_SPK_DRC_ENABLE)
	/* Configure DAC DRC */
	hal_codec_set_dac_drc_lthres(config->lthres);
	hal_codec_set_dac_drc_rthres(config->rthres);
	hal_codec_set_dac_drc_lcomp(config->lcomp);
	hal_codec_set_dac_drc_rcomp(config->rcomp);

	hal_codec_enable_dac_drc(config->drc_enable);
#endif

	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to set the operating mode of the Codec ADC12
 * @param[in] mode The operation mode is set. This parameter can only be one of the type #codec_op_mode_t.
 * @return	
 * 	@retval DRV_STATUS_OK The operating mode is successfully set.
 */
drv_status_t codec_set_adc12_low_power_mode(void)
{
	hal_codec_set_adc12_power_mode(HAL_CODEC_ADC12_LOW_POWER_MODE);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc12_wake_up_mode(void)
{
	hal_codec_set_adc12_power_mode(HAL_CODEC_ADC12_LOW_POWER_MODE);

	return DRV_STATUS_OK;
}


/** 
 * @brief 	This function is used to mute the DAC interface
 */
drv_status_t codec_set_dac_mute(bool enable)
{
//	uart_printf("dacMute\n");
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.chxCfg[DAC_L].mute = enable;
	gDacCfg.chxCfg[DAC_R].mute = enable;
#endif
	if(enable)
	{
		hal_codec_mute_hp();
		hal_codec_mute_dac();
	}
	else
	{
		hal_codec_unmute_hp();
		hal_codec_unmute_dac();
	}
	return DRV_STATUS_OK;
}

#if (CODEC_ADC_ENABLE)
drv_status_t codec_set_adc_mute(bool enable)
{
#if (CODEC_ADC12_ENABLE)
	codec_set_adc12_mute(enable);
#endif
#if (CODEC_ADC34_ENABLE)
	codec_set_adc34_mute(enable);
#endif

#if (CODEC_ADC56_ENABLE)
	codec_set_adc56_mute(enable);
#endif

#if (CODEC_ADC78_ENABLE)
	codec_set_adc78_mute(enable);
#endif

#if (CODEC_ADC9A_ENABLE)
	codec_set_adc9a_mute(enable);
#endif

	return DRV_STATUS_OK;
}
#endif
/** 
 * @brief 	This function is used to mute the ADC12 interface
 */
drv_status_t codec_set_adc12_mute(bool enable)
{
//	uart_printf("adc12 set mute:%d\n", enable);
	if(enable)
	{
		hal_codec_mute_adc12();
	}
	else
	{
		hal_codec_unmute_adc12();
	}

#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.chxCfg[ADC_1].mute = enable;
	gAdcCfg.chxCfg[ADC_2].mute = enable;
#endif
	return DRV_STATUS_OK;
}

/** 
 * @brief 	This function is used to mute the ADC34 interface
 */
drv_status_t codec_set_adc34_mute(bool enable)
{
	if(enable)
	{
		hal_codec_mute_adc34();
	}
	else
	{
		hal_codec_unmute_adc34();
	}
	
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.chxCfg[ADC_3].mute = enable;
	gAdcCfg.chxCfg[ADC_4].mute = enable;
#endif
	return DRV_STATUS_OK;
}

/** 
 * @brief 	This function is used to mute the ADC56 interface
 */
drv_status_t codec_set_adc56_mute(bool enable)
{
//	uart_printf("adc56Mute\n");
	if(enable)
	{
		hal_codec_mute_adc56();
	}
	else
	{
		hal_codec_unmute_adc56();
	}

#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.chxCfg[ADC_5].mute = enable;
	gAdcCfg.chxCfg[ADC_6].mute = enable;
#endif
	return DRV_STATUS_OK;
}

/** 
 * @brief 	This function is used to mute the ADC78 interface
 */
drv_status_t codec_set_adc78_mute(bool enable)
{
	if(enable)
	{
		hal_codec_mute_adc78();
	}
	else
	{
		hal_codec_unmute_adc78();
	}
	return DRV_STATUS_OK;
}

/** 
 * @brief 	This function is used to mute the ADC9a interface
 */
drv_status_t codec_set_adc9a_mute(bool enable)
{
	if(enable)
	{
		hal_codec_mute_adc9a();
	}
	else
	{
		hal_codec_unmute_adc9a();
	}
	return DRV_STATUS_OK;
}

auMute_t codec_dac_get_mute(void)
{
	return hal_codec_get_dac_mute();
}

#if (CODEC_ADC_ENABLE)
auMute_t codec_adc_get_mute(void)
{
	return hal_codec_get_adc12_mute();
}
#endif

auMute_t codec_adc12_get_mute(void)
{
	return hal_codec_get_adc12_mute();
}

auMute_t codec_adc34_get_mute(void)
{
	return hal_codec_get_adc34_mute();
}

auMute_t codec_adc56_get_mute(void)
{
	return hal_codec_get_adc56_mute();
}

auMute_t codec_adc78_get_mute(void)
{
	return hal_codec_get_adc78_mute();
}

auMute_t codec_adc9a_get_mute(void)
{
	return hal_codec_get_adc9a_mute();
}

drv_status_t codec_set_dac_gain(int8_t gain, uint8_t isAgain, auChannel_t lr)
{
	uart_printf("dac set %sGain:%d\n", isAgain?"A":"D", gain);
	if(isAgain == TRUE)
	{
		if(lr == CHANNEL_L)
		{
			hal_codec_set_hp_gol(gain);
#if (SUPPORT_ALTOTOOL_CODEDC)
			gAlgCfg.vol[6+DAC_L] = gain;
			gDacCfg.chxCfg[DAC_L].gom = gain;
#endif
		}
		else
		{
			hal_codec_set_hp_gor(gain);
#if (SUPPORT_ALTOTOOL_CODEDC)
			gAlgCfg.vol[6+DAC_R] = gain;
			gDacCfg.chxCfg[DAC_R].gom = gain;
#endif
		}
	}
	else
	{
		if(lr == CHANNEL_L)
		{
			hal_codec_set_dac_godl(gain);
#if (SUPPORT_ALTOTOOL_CODEDC)
			gAlgCfg.vol[6+DAC_L] = gain;
			gDacCfg.chxCfg[DAC_L].god = gain;
#endif
		}
		else
		{
			hal_codec_set_dac_godr(gain);
#if (SUPPORT_ALTOTOOL_CODEDC)
			gAlgCfg.vol[6+DAC_R] = gain;
			gDacCfg.chxCfg[DAC_R].god = gain;
#endif
		}
	}
	return DRV_STATUS_OK;
}

#if (CODEC_ADC_ENABLE)
drv_status_t codec_set_adc_gain(int8_t gain, uint8_t isAgain, auChannel_t lr)
{
#if (CODEC_ADC12_ENABLE)
	codec_set_adc12_gain(gain, isAgain, lr);
#endif

#if (CODEC_ADC34_ENABLE)
	codec_set_adc34_gain(gain, isAgain, lr);
#endif

#if (CODEC_ADC56_ENABLE)
	codec_set_adc56_gain(gain, isAgain, lr);
#endif

#if (CODEC_ADC78_ENABLE)
	codec_set_adc78_gain(gain, isAgain, lr);
#endif

#if (CODEC_ADC9A_ENABLE)
	codec_set_adc9a_gain(gain, isAgain, lr);
#endif

	return DRV_STATUS_OK;
}
#endif
drv_status_t codec_set_adc12_gain(int8_t gain, uint8_t isAgain, auChannel_t lr)
{
//	uart_printf("adc12 set %sGain:%d\n", isAgain?"A":"D", gain);
	if (g_adc12_config.is_amic) //Analog Microphone
	{
		if(isAgain == TRUE)
		{
			if(lr == CHANNEL_L)
			{
				g_adc12_config.amic.gim1 = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
				gAlgCfg.vol[ADC_1] = gain;
				gAdcCfg.chxCfg[ADC_1].gim = gain;
#endif
				hal_codec_set_amic1_analog_gain((hal_codec_gim_t)gain);
			}
			else
			{
				g_adc12_config.amic.gim2 = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
				gAlgCfg.vol[ADC_2] = gain;
				gAdcCfg.chxCfg[ADC_2].gim = gain;
#endif
				hal_codec_set_amic2_analog_gain((hal_codec_gim_t)gain);
			}
		}
		else
		{
			if(lr == CHANNEL_L)
			{
				g_adc12_config.amic.gid1 = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
				gAlgCfg.vol[ADC_1] = gain;
				gAdcCfg.chxCfg[ADC_1].gid = gain;
#endif
				hal_codec_set_adc1_digital_gain(gain);
			}
			else
			{
				g_adc12_config.amic.gid2 = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
				gAlgCfg.vol[ADC_2] = gain;
				gAdcCfg.chxCfg[ADC_2].gid = gain;
#endif
				hal_codec_set_adc2_digital_gain(gain);
			}
		}
	}
	else
	{
		if(lr == CHANNEL_L)
		{
			g_adc12_config.dmic.gid1 = gain;	
			
#if (SUPPORT_ALTOTOOL_CODEDC)
			gAlgCfg.vol[ADC_1] = gain;
#endif
			hal_codec_set_adc1_digital_gain(gain);
		}
		else
		{
			g_adc12_config.dmic.gid2 = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
			gAlgCfg.vol[ADC_2] = gain;
#endif
			hal_codec_set_adc2_digital_gain(gain);
		}
	}
	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc34_gain(int8_t gain, uint8_t isAgain, auChannel_t lr)
{
	if(isAgain == FALSE)
	{
		if(lr == CHANNEL_L)
		{
			g_adc34_config.gid3 = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
			gAlgCfg.vol[ADC_3] = gain;
			gAdcCfg.chxCfg[ADC_3].gid = gain;
#endif
			hal_codec_set_adc3_digital_gain(gain);
		}
		else
		{
			g_adc34_config.gid4 = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
			gAlgCfg.vol[ADC_4] = gain;
			gAdcCfg.chxCfg[ADC_4].gid = gain;
#endif
			hal_codec_set_adc4_digital_gain(gain);
		}
	}

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc56_gain(int8_t gain, uint8_t isAgain, auChannel_t lr)
{
	if(isAgain == FALSE)
	{
		if(lr == CHANNEL_L)
		{
			g_adc56_config.gid5 = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
			gAlgCfg.vol[ADC_5] = gain;
			gAdcCfg.chxCfg[ADC_5].gid = gain;
#endif
			hal_codec_set_adc5_digital_gain(gain);
		}
		else
		{
			g_adc56_config.gid6 = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
			gAlgCfg.vol[ADC_6] = gain;
			gAdcCfg.chxCfg[ADC_6].gid = gain;
#endif
			hal_codec_set_adc6_digital_gain(gain);
		}
	}

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc78_gain(int8_t gain, uint8_t isAgain, auChannel_t lr)
{
	if(isAgain == FALSE)
	{
		if(lr == CHANNEL_L)
		{
			hal_codec_set_adc7_digital_gain(gain);
#if (SUPPORT_ALTOTOOL_CODEDC)
			gAlgCfg.vol[ADC_7] = gain;
#endif
		}else
		{
			hal_codec_set_adc8_digital_gain(gain);
#if (SUPPORT_ALTOTOOL_CODEDC)
			gAlgCfg.vol[ADC_8] = gain;
#endif
		}
	}

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc9a_gain(int8_t gain, uint8_t isAgain, auChannel_t lr)
{
	if(isAgain == FALSE)
	{
		if(lr == CHANNEL_L)
		{
			hal_codec_set_adc9_digital_gain(gain);
#if (SUPPORT_ALTOTOOL_CODEDC)
			gAlgCfg.vol[ADC_9] = gain;
#endif
		}
		else
		{
			hal_codec_set_adc10_digital_gain(gain);
#if (SUPPORT_ALTOTOOL_CODEDC)
			gAlgCfg.vol[ADC_10] = gain;
#endif
		}
	}

	return DRV_STATUS_OK;
}

int8_t codec_get_dac_gain(uint8_t isAgain, auChannel_t lr)
{
	if(isAgain == TRUE)
	{
		if(lr == CHANNEL_L)
			return hal_codec_get_hp_gol();
		else
			return hal_codec_get_hp_gor();
	}
	else
	{
		if(lr == CHANNEL_L)
			return hal_codec_get_dac_godl();
		else
			return hal_codec_get_dac_godr();
	}	
}

#if (CODEC_ADC_ENABLE)
int8_t codec_get_adc_gain(uint8_t isAgain, auChannel_t lr)
{
	if(isAgain == TRUE)
	{
		if(lr == CHANNEL_L)
			return hal_codec_get_amic1_analog_gain();
		else
			return hal_codec_get_amic2_analog_gain();
	}
	else
	{
		if(lr == CHANNEL_L)
			return hal_codec_get_adc1_digital_gain();
		else
			return hal_codec_get_adc2_digital_gain();
	}

	return DRV_STATUS_OK;
}
#endif

int8_t codec_get_adc12_gain(uint8_t isAgain, auChannel_t lr)
{
	if (g_adc12_config.is_amic) //Analog Microphone
	{
		if(isAgain == TRUE)
		{
			if(lr == CHANNEL_L)
				return hal_codec_get_amic1_analog_gain();
			else
				return hal_codec_get_amic2_analog_gain();
		}
		else
		{
			if(lr == CHANNEL_L)
				return hal_codec_get_adc1_digital_gain();
			else
				return hal_codec_get_adc2_digital_gain();
		}
	}
	else
	{
		if(lr == CHANNEL_L)
			return hal_codec_get_adc1_digital_gain();
		else
			return hal_codec_get_adc2_digital_gain();
	}

}

int8_t codec_get_adc34_gain(uint8_t isAgain, auChannel_t lr)
{
	if(lr == CHANNEL_L)
		return hal_codec_get_adc3_digital_gain();
	else
		return hal_codec_get_adc4_digital_gain();
}

int8_t codec_get_adc56_gain(uint8_t isAgain, auChannel_t lr)
{
	if(lr == CHANNEL_L)
		return hal_codec_get_adc5_digital_gain();
	else
		return hal_codec_get_adc6_digital_gain();
}

int8_t codec_get_adc78_gain(uint8_t isAgain, auChannel_t lr)
{
	if(lr == CHANNEL_L)
		return hal_codec_get_adc7_digital_gain();
	else
		return hal_codec_get_adc8_digital_gain();
}

int8_t codec_get_adc9a_gain(uint8_t isAgain, auChannel_t lr)
{
	if(lr == CHANNEL_L)
		return hal_codec_get_adc9_digital_gain();
	else
		return hal_codec_get_adc10_digital_gain();
}

drv_status_t codec_enable_adc12_hpf(uint8_t enable)
{
	g_adc12_config.hpf_enable = enable;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_12].hpf = enable;
#endif

	hal_codec_enable_adc12_hpf(enable);

	return DRV_STATUS_OK;
}

drv_status_t codec_enable_adc34_hpf(uint8_t enable)
{
	g_adc34_config.hpf_enable = enable;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_34].hpf = enable;
#endif

	hal_codec_enable_adc34_hpf(enable);

	return DRV_STATUS_OK;
}

drv_status_t codec_enable_adc56_hpf(uint8_t enable)
{
	g_adc56_config.hpf_enable = enable;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_56].hpf = enable;
#endif

	hal_codec_enable_adc56_hpf(enable);

	return DRV_STATUS_OK;
}

drv_status_t codec_select_adc12_mixer_mode(uint8_t enable)
{
	g_adc12_config.mix_rec = enable;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_12].mixrec = enable;
#endif
	
	hal_codec_select_adc12_mixer_mode(enable);
	return DRV_STATUS_OK;
}

drv_status_t codec_select_adc34_mixer_mode(uint8_t enable)
{
	g_adc12_config.mix_rec = enable;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_34].mixrec = enable;
#endif
	
	hal_codec_select_adc34_mixer_mode(enable);
	return DRV_STATUS_OK;
}

drv_status_t codec_select_adc56_mixer_mode(uint8_t enable)
{
	g_adc12_config.mix_rec = enable;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_56].mixrec = enable;
#endif
	
	hal_codec_select_adc56_mixer_mode(enable);
	return DRV_STATUS_OK;
}


drv_status_t codec_set_adc_word_length(uint8_t wordLength)
{
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.gCfg[ADC_12].wordLength = wordLength;
	gAdcCfg.gCfg[ADC_3456].wordLength = wordLength;
#endif
	hal_codec_set_adc_data_mode(wordLength);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc1_aiadc(uint8_t sel)
{
	g_adc12_config.aiadc1_sel = sel;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.chxCfg[ADC_1].aiadc = sel;
#endif
	hal_codec_select_adc1_output(sel);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc2_aiadc(uint8_t sel)
{
	g_adc12_config.aiadc2_sel = sel;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.chxCfg[ADC_2].aiadc = sel;
#endif
	hal_codec_select_adc2_output(sel);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc3_aiadc(uint8_t sel)
{
	g_adc34_config.aiadc3_sel = sel;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.chxCfg[ADC_3].aiadc = sel;
#endif
	hal_codec_select_adc3_output(sel);
	
	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc4_aiadc(uint8_t sel)
{
	g_adc34_config.aiadc4_sel = sel;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.chxCfg[ADC_4].aiadc = sel;
#endif
	hal_codec_select_adc4_output(sel);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc5_aiadc(uint8_t sel)
{
	g_adc56_config.aiadc5_sel = sel;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.chxCfg[ADC_5].aiadc = sel;
#endif
	hal_codec_select_adc5_output(sel);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc6_aiadc(uint8_t sel)
{
	g_adc56_config.aiadc6_sel = sel;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.chxCfg[ADC_6].aiadc = sel;
#endif
	hal_codec_select_adc6_output(sel);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc12_wnf_mode(uint8_t mode)
{
	g_adc12_config.wnf = mode;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_12].wnf = mode;
#endif
	hal_codec_set_adc12_wnf_mode(mode);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc34_wnf_mode(uint8_t mode)
{
	g_adc34_config.wnf = mode;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_34].wnf = mode;
#endif
	hal_codec_set_adc34_wnf_mode(mode);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc56_wnf_mode(uint8_t mode)
{
	g_adc56_config.wnf = mode;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_56].wnf = mode;
#endif
	hal_codec_set_adc56_wnf_mode(mode);

	return DRV_STATUS_OK;
}

drv_status_t codec_adc12_agc_enable(uint8_t enable)
{
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_12].agcCfg.agcEn = enable;
#endif
	hal_codec_enable_adc12_agc(enable);
	
	return DRV_STATUS_OK;
}

drv_status_t codec_adc34_agc_enable(uint8_t enable)
{
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_34].agcCfg.agcEn = enable;
#endif
	hal_codec_enable_adc34_agc(enable);
	
	return DRV_STATUS_OK;
}

drv_status_t codec_adc56_agc_enable(uint8_t enable)
{
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_56].agcCfg.agcEn = enable;
#endif
	hal_codec_enable_adc56_agc(enable);
	
	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc12_agc_stereo(uint8_t enable)
{
	g_adc12_agc_config.stereo = enable;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_12].agcCfg.stereo = enable;
#endif

	hal_codec_set_adc12_agc_stereo(enable);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc12_agc_target(uint8_t target)
{
	g_adc12_agc_config.target = target;
#if (SUPPORT_ALTOTOOL_CODEDC)
		gAdcCfg.pairCfg[ADC_12].agcCfg.target = target;
#endif

	hal_codec_set_adc12_agc_target(target);

	return DRV_STATUS_OK;
}

drv_status_t codec_adc12_agc_noise_gate_enable(uint8_t enable)
{
	g_adc12_agc_config.noise_gate_enable = enable;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_12].agcCfg.ngEn = enable;
#endif

	hal_codec_adc12_agc_noise_gate_enable(enable);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc12_agc_noise_gate_thres(uint8_t threshold)
{
	g_adc12_agc_config.noise_gate_threshold = threshold;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_12].agcCfg.ngThr = threshold;
#endif
	hal_codec_set_adc12_agc_noise_gate_thres(threshold);

	return DRV_STATUS_OK;
}

drv_status_t codec_adc12_agc_snr_optimzation(uint8_t enable)
{
	g_adc12_agc_config.snr_optimisation_enable = enable;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_12].agcCfg.snrOptEn = enable;
#endif
	hal_codec_adc12_agc_snr_optimzation(enable);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc12_agc_hold_time(uint8_t holdTime)
{
	g_adc12_agc_config.hold_time = holdTime;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_12].agcCfg.holdTime = holdTime;
#endif
	hal_codec_set_adc12_agc_hold_time(holdTime);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc12_agc_attack_time(uint8_t attackTime)
{
	g_adc12_agc_config.attack_time = attackTime;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_12].agcCfg.atkTime = attackTime;
#endif

	hal_codec_set_adc12_agc_attack_time(attackTime);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc12_agc_decay_time(uint8_t decayTime)
{
	g_adc12_agc_config.decay_time = decayTime;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_12].agcCfg.dcyTime = decayTime;
#endif
	hal_codec_set_adc12_agc_decay_time(decayTime);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc12_agc_min_gain(uint8_t gain)
{
	g_adc12_agc_config.min_gain = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_12].agcCfg.minGain = gain;
#endif
	hal_codec_set_adc12_agc_min_gain(gain);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc12_agc_max_gain(uint8_t gain)
{
	g_adc12_agc_config.max_gain = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_12].agcCfg.maxGain= gain;
#endif

	hal_codec_set_adc12_agc_max_gain(gain);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc34_agc_stereo(uint8_t enable)
{
	g_adc34_agc_config.stereo = enable;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_34].agcCfg.stereo = enable;
#endif

	hal_codec_set_adc34_agc_stereo(enable);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc34_agc_target(uint8_t target)
{
	g_adc34_agc_config.target = target;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_34].agcCfg.target = target;
#endif
	hal_codec_set_adc34_agc_target(target);

	return DRV_STATUS_OK;
}

drv_status_t codec_adc34_agc_noise_gate_enable(uint8_t enable)
{
	g_adc34_agc_config.noise_gate_enable = enable;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_34].agcCfg.ngEn = enable;
#endif

	hal_codec_adc34_agc_noise_gate_enable(enable);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc34_agc_noise_gate_thres(uint8_t threshold)
{
	g_adc34_agc_config.noise_gate_threshold = threshold;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_34].agcCfg.ngThr = threshold;
#endif
	hal_codec_set_adc34_agc_noise_gate_thres(threshold);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc34_agc_hold_time(uint8_t holdTime)
{
	g_adc34_agc_config.hold_time = holdTime;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_34].agcCfg.holdTime = holdTime;
#endif
	hal_codec_set_adc34_agc_hold_time(holdTime);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc34_agc_attack_time(uint8_t attackTime)
{
	g_adc34_agc_config.attack_time = attackTime;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_34].agcCfg.atkTime = attackTime;
#endif

	hal_codec_set_adc34_agc_attack_time(attackTime);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc34_agc_decay_time(uint8_t decayTime)
{
	g_adc34_agc_config.decay_time = decayTime;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_34].agcCfg.dcyTime = decayTime;
#endif

	hal_codec_set_adc34_agc_decay_time(decayTime);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc34_agc_min_gain(uint8_t gain)
{
	g_adc34_agc_config.min_gain = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_34].agcCfg.minGain = gain;
#endif
	hal_codec_set_adc34_agc_min_gain(gain);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc34_agc_max_gain(uint8_t gain)
{
	g_adc34_agc_config.max_gain = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_34].agcCfg.maxGain = gain;
#endif

	hal_codec_set_adc34_agc_max_gain(gain);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc56_agc_stereo(uint8_t enable)
{
	g_adc56_agc_config.stereo = enable;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_56].agcCfg.stereo = enable;
#endif

	hal_codec_set_adc56_agc_stereo(enable);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc56_agc_target(uint8_t target)
{
	g_adc56_agc_config.target = target;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_56].agcCfg.target = target;
#endif
	hal_codec_set_adc56_agc_target(target);

	return DRV_STATUS_OK;
}

drv_status_t codec_adc56_agc_noise_gate_enable(uint8_t enable)
{
	g_adc56_agc_config.noise_gate_enable = enable;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_56].agcCfg.ngEn = enable;
#endif

	hal_codec_adc56_agc_noise_gate_enable(enable);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc56_agc_noise_gate_thres(uint8_t threshold)
{
	g_adc56_agc_config.noise_gate_threshold = threshold;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_56].agcCfg.ngThr = threshold;
#endif

	hal_codec_set_adc56_agc_noise_gate_thres(threshold);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc56_agc_hold_time(uint8_t holdTime)
{
	g_adc56_agc_config.hold_time = holdTime;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_56].agcCfg.holdTime = holdTime;
#endif
	hal_codec_set_adc56_agc_hold_time(holdTime);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc56_agc_attack_time(uint8_t attackTime)
{
	g_adc56_agc_config.attack_time = attackTime;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_56].agcCfg.atkTime = attackTime;
#endif

	hal_codec_set_adc56_agc_attack_time(attackTime);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc56_agc_decay_time(uint8_t decayTime)
{
	g_adc56_agc_config.decay_time = decayTime;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_56].agcCfg.dcyTime = decayTime;
#endif
	hal_codec_set_adc56_agc_decay_time(decayTime);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc56_agc_min_gain(uint8_t gain)
{
	g_adc56_agc_config.min_gain = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_56].agcCfg.minGain = gain;
#endif

	hal_codec_set_adc56_agc_min_gain(gain);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_adc56_agc_max_gain(uint8_t gain)
{
	g_adc56_agc_config.max_gain = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.pairCfg[ADC_56].agcCfg.maxGain = gain;
#endif

	hal_codec_set_adc56_agc_max_gain(gain);

	return DRV_STATUS_OK;
}

drv_status_t codec_dac_drc_enbale(uint8_t enable)
{
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.drcEn = enable;
#endif

	hal_codec_enable_dac_drc(enable);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_dac_drc_lthres(uint8_t threshold)
{
	g_dac_config.lthres = threshold;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.chxCfg[DAC_L].drcThr = threshold;
#endif

	hal_codec_set_dac_drc_lthres(threshold);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_dac_drc_rthres(uint8_t threshold)
{
	g_dac_config.rthres = threshold;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.chxCfg[DAC_R].drcThr = threshold;
#endif

	hal_codec_set_dac_drc_rthres(threshold);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_dac_drc_lcomprate(uint8_t comprate)
{
	g_dac_config.lcomp = comprate;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.chxCfg[DAC_L].drcComp = comprate;
#endif

	hal_codec_set_dac_drc_lcomp(comprate);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_dac_drc_rcomprate(uint8_t comprate)
{
	g_dac_config.rcomp = comprate;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.chxCfg[DAC_R].drcComp = comprate;
#endif

	hal_codec_set_dac_drc_rcomp(comprate);

	return DRV_STATUS_OK;
}

drv_status_t codec_dac_enable(uint8_t enable)
{
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.enable = enable;
#endif
	hal_codec_activate_dac(enable);
	hal_codec_activate_dac_audio_interface(enable);
	hal_codec_activate_hp(enable);
	
	return DRV_STATUS_OK;
}

drv_status_t codec_dac_mix_enable(uint8_t enable)
{
	g_dac_config.dac_mix = enable;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.mix = enable;
#endif
	hal_codec_select_dac_mixer_mode(enable);
	
	return DRV_STATUS_OK;
}

drv_status_t codec_set_dac_golmix_gain(uint8_t gain)
{
	g_dac_config.gomixl = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.chxCfg[DAC_L].gomix = gain;
#endif
	hal_codec_set_dac_gomixl(gain);
	
	return DRV_STATUS_OK;
}

drv_status_t codec_set_dac_gormix_gain(uint8_t gain)
{
	g_dac_config.gomixr = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.chxCfg[DAC_R].gomix = gain;
#endif

	hal_codec_set_dac_gomixr(gain);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_dac_gilmix_gain(uint8_t gain)
{
	g_dac_config.gimixl = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.chxCfg[DAC_L].gimix = gain;
#endif
	hal_codec_set_dac_gimixl(gain);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_dac_girmix_gain(uint8_t gain)
{
	g_dac_config.gimixr = gain;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.chxCfg[DAC_R].gimix = gain;
#endif
	hal_codec_set_dac_gimixr(gain);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_dac_mixdacl(uint8_t sel)
{
	g_dac_config.aidacl_sel = sel;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.chxCfg[DAC_L].mixdac = sel;
#endif
	hal_codec_select_dac_mixdacl_input(sel);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_dac_mixdacr(uint8_t sel)
{
	g_dac_config.aidacr_sel = sel;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.chxCfg[DAC_R].mixdac = sel;
#endif

	hal_codec_select_dac_mixdacr_input(sel);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_dac_aidacl(uint8_t sel)
{
	g_dac_config.aidacl_sel = sel;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.chxCfg[DAC_L].aidac = sel;
#endif

	hal_codec_select_dac_aidacl_input(sel);

	return DRV_STATUS_OK;
}

drv_status_t codec_set_dac_aidacr(uint8_t sel)
{
	g_dac_config.aidacr_sel = sel;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gDacCfg.chxCfg[DAC_R].aidac = sel;
#endif

	hal_codec_select_dac_aidacr_input(sel);

	return DRV_STATUS_OK;
}

drv_status_t codec_select_dac_mixadc1_input(uint8_t sel)
{
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.gCfg[ADC_12].mixadc1 = sel;
#endif
	hal_codec_select_dac_mixadc1_input(sel);

	return DRV_STATUS_OK;
}

drv_status_t codec_select_dac_mixadc2_input(uint8_t sel)
{
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.gCfg[ADC_12].mixadc2 = sel;
#endif

	hal_codec_select_dac_mixadc2_input(sel);

	return DRV_STATUS_OK;
}
#if (SUPPORT_ALTOTOOL)
drv_status_t codec_adc12_enable(uint8_t enable, auChannel_t lr)
{
	if(lr == CHANNEL_L)
	{
		if (g_adc12_config.is_amic)
		{
			hal_codec_activate_dmic1(false);
			hal_codec_activate_dmic2(false);
#if	(ADC_MICBIAS)
			hal_codec_activate_amic1_bias(true);
			hal_codec_activate_amic2_bias(true);
#endif
		hal_codec_activate_adc1(enable);
		}
		else
		{
			hal_codec_activate_amic1_bias(false);
			hal_codec_activate_amic2_bias(false);
			hal_codec_activate_dmic1(enable);
		}		
#if (SUPPORT_ALTOTOOL_CODEDC)
		gAdcCfg.chxCfg[ADC_1].en = enable;
#endif
	}
	else
	{
		if (g_adc12_config.is_amic)
		{
			hal_codec_activate_dmic1(false);
			hal_codec_activate_dmic2(false);
#if	(ADC_MICBIAS)
			hal_codec_activate_amic1_bias(true);
			hal_codec_activate_amic2_bias(true);
#endif
		hal_codec_activate_adc2(enable);
		}
		else
		{
			hal_codec_activate_amic1_bias(false);
			hal_codec_activate_amic2_bias(false);	
			hal_codec_activate_dmic2(enable);
		}
#if (SUPPORT_ALTOTOOL_CODEDC)
		gAdcCfg.chxCfg[ADC_2].en = enable;
#endif
	}

	return DRV_STATUS_OK;
}
#endif
drv_status_t codec_adc34_enable(uint8_t enable, auChannel_t lr)
{
	if(lr == CHANNEL_L)
	{
		hal_codec_activate_dmic3(enable);
#if (SUPPORT_ALTOTOOL_CODEDC)
		gAdcCfg.chxCfg[ADC_3].en = enable;
#endif
	}
	else
	{
		hal_codec_activate_dmic4(enable);
#if (SUPPORT_ALTOTOOL_CODEDC)
		gAdcCfg.chxCfg[ADC_4].en = enable;
#endif
	}

	return DRV_STATUS_OK;
}

drv_status_t codec_adc56_enable(uint8_t enable, auChannel_t lr)
{
	if(lr == CHANNEL_L)
	{
		hal_codec_activate_dmic5(enable);
#if (SUPPORT_ALTOTOOL_CODEDC)
		gAdcCfg.chxCfg[ADC_5].en = enable;
#endif
	}
	else
	{
		hal_codec_activate_dmic6(enable);
#if (SUPPORT_ALTOTOOL_CODEDC)
		gAdcCfg.chxCfg[ADC_6].en = enable;
#endif
	}

	return DRV_STATUS_OK;
}


/** 
 * @brief This function is used to configure the Codec ADC12 audio interface
 * @param[in] config The configuration information of ADC
 * @return	
 * 	@retval 0 ADC12 is successfully configured.
 */
drv_status_t codec_config_adc12(codec_adc12_config_t *config)
{


	hal_codec_mute_adc12();

	if (!config->is_amic)
	{
		hal_gpio_set_pinmux_function(GPIO_8, GPIO_8_FUNC_DMIC_CLK11);
		hal_gpio_set_pinmux_function(GPIO_9, GPIO_9_FUNC_DMIC_IN1);
	}

	hal_codec_set_adc12_audioif_parallel();
	hal_codec_set_adc12_master_slave_mode(config->master_slave);
	hal_codec_set_adc12_sample_rate(codec_samplerate_transform(config->sample_rate));

#if (CODEC_MASTER == 0) && (CODEC_SLAVE_FRAC_SHARE == CODEC_SLAVE_FRAC_SHARE_DEDICATED)
	if (config->master_slave == CODEC_MODE_SLAVE)
		hal_clock_set_slave_adc12_samplerate(config->sample_rate);
#endif
	
	hal_codec_select_adc1_output(config->aiadc1_sel);
	hal_codec_select_adc2_output(config->aiadc2_sel);
	hal_codec_select_adc12_mixer_mode(config->mix_rec);
	hal_codec_set_adc12_power_mode(config->adc12_mode);

	if (config->is_amic) //Analog Microphone
	{
		hal_codec_disable_pga_lf_drift();
		hal_codec_set_amic1_capcouple(config->amic.capcouple1);
		hal_codec_set_amic2_capcouple(config->amic.capcouple2);
		
		hal_codec_select_adc1_data_provided(HAL_CODEC_ADC_DATA_PROVIEDE_ANALOG_ADC);
		hal_codec_select_adc2_data_provided(HAL_CODEC_ADC_DATA_PROVIEDE_ANALOG_ADC);
		if (config->adc12_mode == HAL_CODEC_ADC12_NORMAL_MODE) {
			hal_codec_set_dmic12_clock(HAL_CODEC_DMIC_RATIO_12);
		} else {
			hal_codec_set_dmic12_clock(HAL_CODEC_DMIC_RATIO_4);
		}
		
		hal_codec_set_amic1_input_mode(config->amic.micdiff1);
		hal_codec_set_amic1_analog_gain(config->amic.gim1);
		hal_codec_set_adc1_digital_gain(config->amic.gid1);

		hal_codec_set_amic2_input_mode(config->amic.micdiff2);
		hal_codec_set_amic2_analog_gain(config->amic.gim2);
		hal_codec_set_adc2_digital_gain(config->amic.gid2);
		
		hal_codec_set_amic1_bias_state_in_power_down(config->amic.micbias1_v);
		hal_codec_set_amic2_bias_state_in_power_down(config->amic.micbias2_v);
	} 
	else //Digital Microphone
	{
		hal_codec_set_dmic12_clock(config->dmic.clock);	
		hal_codec_set_adc1_digital_gain(config->dmic.gid1);
		hal_codec_set_adc2_digital_gain(config->dmic.gid2);
		hal_codec_select_adc1_data_provided(config->dmic.adc_dmic_sel1);
		hal_codec_select_adc2_data_provided(config->dmic.adc_dmic_sel2);
	}

	/* ADC12 AGC config */
#if (CODEC_MIC_AGC_ENABLE)
	hal_codec_config_adc12_agc(config->agc_config);
	hal_codec_enable_adc12_agc(config->agc_enable);
#endif

	hal_codec_enable_adc12_hpf(config->hpf_enable);
	hal_codec_set_adc12_wnf_mode(config->wnf);

	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to configure the commom part of the Codec ADC3456 audio interface
 * @param[in] config The configuration information of ADC3456
 * @return	
 * 	@retval 0 The common part of ADC3456 is successfully configured.
 */
drv_status_t codec_config_adc3456(codec_adc3456_config_t *config)
{
	ASSERT(config);

#if (SUPPORT_ALTOTOOL_CODEDC)
	config->sample_rate = gAdcCfg.gCfg[ADC_34].sr;
#endif
	
	/**********************ADC3456 Configuration***************************/
	hal_codec_set_adc3456_audioif_parallel();
	hal_codec_set_adc3456_master_slave_mode(config->master_slave);
	hal_codec_set_adc3456_sample_rate(codec_samplerate_transform(config->sample_rate));

#if (CODEC_MASTER == 0)
	if (config->master_slave == CODEC_MODE_SLAVE)
		hal_clock_set_slave_adc3456_samplerate(config->sample_rate);
#endif

	hal_codec_set_dmic3456_clock(config->dmic_clock);

	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to configure the Codec ADC34 audio interface
 * @param[in] config The configuration information of ADC34
 * @return	
 * 	@retval 0 ADC34 is successfully configured.
 */
drv_status_t codec_config_adc34(codec_adc34_config_t *config)
{
	ASSERT(config);



	hal_codec_mute_adc34();
	
	/**********************ADC34 Configuration***************************/
	hal_codec_select_adc3_data_provided(config->adc_dmic_sel3);
	hal_codec_select_adc4_data_provided(config->adc_dmic_sel4);
	hal_codec_select_adc3_output(config->aiadc3_sel);
	hal_codec_select_adc4_output(config->aiadc4_sel);
	hal_codec_set_adc3_digital_gain(config->gid3);
	hal_codec_set_adc4_digital_gain(config->gid4);

#if (CODEC_MIC_AGC_ENABLE)
	hal_codec_config_adc34_agc(config->agc_config);
	hal_codec_enable_adc34_agc(config->agc_enable);
#endif

	hal_codec_enable_adc34_hpf(config->hpf_enable);
	hal_codec_set_adc34_wnf_mode(config->wnf);

	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to configure the Codec ADC56 audio interface
 * @param[in] config The configuration information of ADC56
 * @return	
 * 	@retval 0 ADC56 is successfully configured.
 */
drv_status_t codec_config_adc56(codec_adc56_config_t *config)
{
	ASSERT(config);

#if (SUPPORT_ALTOTOOL_CODEDC)
	config->gid5 = gAdcCfg.chxCfg[ADC_5].gid;
	config->gid6 = gAdcCfg.chxCfg[ADC_6].gid;
	config->aiadc5_sel = gAdcCfg.chxCfg[ADC_5].aiadc;
	config->aiadc6_sel = gAdcCfg.chxCfg[ADC_6].aiadc;
	config->hpf_enable = gAdcCfg.pairCfg[ADC_56].hpf;
	config->wnf = gAdcCfg.pairCfg[ADC_56].wnf;
	config->mix_rec = gAdcCfg.pairCfg[ADC_34].mixrec;

	config->agc_enable = gAdcCfg.pairCfg[ADC_56].agcCfg.agcEn;
	config->agc_config->stereo = gAdcCfg.pairCfg[ADC_56].agcCfg.stereo;	
	config->agc_config->target = gAdcCfg.pairCfg[ADC_56].agcCfg.target;	
	config->agc_config->snr_optimisation_enable = gAdcCfg.pairCfg[ADC_56].agcCfg.snrOptEn;	
	config->agc_config->noise_gate_enable = gAdcCfg.pairCfg[ADC_56].agcCfg.ngEn;	
	config->agc_config->noise_gate_threshold = gAdcCfg.pairCfg[ADC_56].agcCfg.ngThr;	
	config->agc_config->hold_time = gAdcCfg.pairCfg[ADC_56].agcCfg.holdTime;	
	config->agc_config->attack_time = gAdcCfg.pairCfg[ADC_56].agcCfg.atkTime;	
	config->agc_config->decay_time = gAdcCfg.pairCfg[ADC_56].agcCfg.dcyTime;	
	config->agc_config->max_gain = gAdcCfg.pairCfg[ADC_56].agcCfg.maxGain;
	config->agc_config->min_gain = gAdcCfg.pairCfg[ADC_56].agcCfg.minGain;	
#endif

	hal_codec_mute_adc56();
	
	/**********************ADC56 Configuration***************************/
	hal_codec_select_adc5_data_provided(config->adc_dmic_sel5);
	hal_codec_select_adc6_data_provided(config->adc_dmic_sel6);
	hal_codec_select_adc5_output(config->aiadc5_sel);
	hal_codec_select_adc6_output(config->aiadc6_sel);
	hal_codec_set_adc5_digital_gain(config->gid5);
	hal_codec_set_adc6_digital_gain(config->gid6);

#if (CODEC_MIC_AGC_ENABLE)
	hal_codec_config_adc56_agc(config->agc_config);
	hal_codec_enable_adc56_agc(config->agc_enable);
#endif

	hal_codec_enable_adc56_hpf(config->hpf_enable);
	hal_codec_set_adc56_wnf_mode(config->wnf);

	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to configure the commom part of the Codec ADC789a audio interface
 * @param[in] config The configuration information of ADC789a
 * @return	
 * 	@retval 0 The common part of ADC789a is successfully configured.
 */
drv_status_t codec_config_adc789a(codec_adc789a_config_t *config)
{
	ASSERT(config);
	
	/**********************ADC789a Configuration***************************/
	hal_codec_set_adc789a_audioif_parallel();
	hal_codec_set_adc789a_master_slave_mode(config->master_slave);
	hal_codec_set_adc789a_sample_rate(codec_samplerate_transform(config->sample_rate));

#if (CODEC_MASTER == 0) && (CODEC_SLAVE_FRAC_SHARE == CODEC_SLAVE_FRAC_SHARE_DEDICATED)
	if (config->master_slave == CODEC_MODE_SLAVE)
		hal_clock_set_slave_adc789a_samplerate(config->sample_rate);
#endif

	hal_codec_set_dmic789a_clock(config->dmic_clock);

	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to configure the Codec ADC78 audio interface
 * @param[in] config The configuration information of ADC78
 * @return	
 * 	@retval 0 ADC78 is successfully configured.
 */
drv_status_t codec_config_adc78(codec_adc78_config_t *config)
{
	ASSERT(config);

	hal_gpio_set_pinmux_function(GPIO_4, GPIO_4_FUNC_DMIC_IN4);
	hal_gpio_set_pinmux_function(GPIO_6, GPIO_6_FUNC_DMIC_CLK41);

	hal_codec_mute_adc78();
	
	/**********************ADC78 Configuration***************************/
	hal_codec_select_adc7_data_provided(config->adc_dmic_sel7);
	hal_codec_select_adc8_data_provided(config->adc_dmic_sel8);
	hal_codec_select_adc7_output(config->aiadc7_sel);
	hal_codec_select_adc8_output(config->aiadc8_sel);
	hal_codec_set_adc7_digital_gain(config->gid7);
	hal_codec_set_adc8_digital_gain(config->gid8);

#if (CODEC_MIC_AGC_ENABLE)
	hal_codec_config_adc78_agc(config->agc_config);
#endif

	hal_codec_enable_adc78_hpf(config->hpf_enable);
	hal_codec_set_adc78_wnf_mode(config->wnf);

	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to configure the Codec ADC9a audio interface
 * @param[in] config The configuration information of ADC9a
 * @return	
 * 	@retval 0 ADC9a is successfully configured.
 */
drv_status_t codec_config_adc9a(codec_adc9a_config_t *config)
{
	ASSERT(config);

	hal_gpio_set_pinmux_function(GPIO_5, GPIO_5_FUNC_DMIC_IN5);
	hal_gpio_set_pinmux_function(GPIO_7, GPIO_7_FUNC_DMIC_CLK51);

	hal_codec_mute_adc9a();
	
	/**********************ADC9a Configuration***************************/
	hal_codec_select_adc9_data_provided(config->adc_dmic_sel9);
	hal_codec_select_adc10_data_provided(config->adc_dmic_sel10);
	hal_codec_select_adc9_output(config->aiadc9_sel);
	hal_codec_select_adc10_output(config->aiadc10_sel);
	hal_codec_set_adc9_digital_gain(config->gid9);
	hal_codec_set_adc10_digital_gain(config->gid10);

#if (CODEC_MIC_AGC_ENABLE)
	hal_codec_config_adc9a_agc(config->agc_config);
#endif

	hal_codec_enable_adc9a_hpf(config->hpf_enable);
	hal_codec_set_adc9a_wnf_mode(config->wnf);

	return DRV_STATUS_OK;
}


/** 
 * @brief This function is used to set the operating mode of the Codec
 * @param[in] mode The operation mode is set. This parameter can only be one of the type #codec_op_mode_t.
 * @return	
 * 	@retval 0 The operating mode is successfully set.
 */
drv_status_t codec_set_operation_mode(codec_op_mode_t mode)
{
	//uint32_t cycle;
	
	switch (mode)
	{
	case CODEC_OP_MODE_VOICE_OPEN:
//		hal_codec_analog_dac_sleep((false));
		hal_codec_analog_adc_sleep(false);
		hal_codec_analog_standby(false);
		hal_codec_digital_standby(false);
		//delay(150);
		break;
	case CODEC_OP_MODE_VOICE_CLOSE:
		hal_codec_analog_adc_sleep(true);
		break;
	case CODEC_OP_MODE_PLAYBACK_OPEN:
		hal_codec_analog_dac_sleep(false);
		hal_codec_analog_standby(false);
		hal_codec_digital_standby(false);
		delay(150);
		break;
	case CODEC_OP_MODE_WT:
		hal_codec_analog_adc_sleep(true);
		hal_codec_analog_dac_sleep(true);
		hal_codec_analog_standby(true);
		hal_codec_digital_standby(false);
		break;
	case CODEC_OP_MODE_SHUTDOWN:
	default:
		hal_codec_analog_adc_sleep(true);
		hal_codec_analog_dac_sleep(true);
		hal_codec_analog_standby(true);
		hal_codec_digital_standby(true);
		delay(150);
		break;
	}

	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to start the DAC audio interface
 * @return	
 * 	@retval 0 DAC is successfully started.
 */
drv_status_t codec_dac_start(void)
{
#if (SUPPORT_ALTOTOOL_CODEDC)
	codec_dac_enable(gDacCfg.enable);
#else
	hal_codec_activate_dac(true);
	hal_codec_activate_dac_audio_interface(true);
	hal_codec_activate_hp(true);
#endif

#if (SUPPORT_ALTOTOOL_CODEDC)
	codec_set_dac_mute(gDacCfg.chxCfg[DAC_L].mute&&gDacCfg.chxCfg[DAC_R].mute);
#else
	hal_codec_unmute_dac();
	hal_codec_unmute_hp();
#endif
	
#if CODEC_USE_ONE_IRQ
	hal_interrupt_register_isr_handler(CODEC_IRQ, codec_isr_handler);
	hal_interrupt_enable_irq(CODEC_IRQ);
#else
	hal_interrupt_register_isr_handler(CODEC_DAC_IRQ, codec_dac_isr_handler);
	hal_interrupt_enable_irq(CODEC_DAC_IRQ);

	hal_interrupt_register_isr_handler(CODEC_DAC_SC_IRQ, codec_dac_sc_isr_handler);
	hal_interrupt_enable_irq(CODEC_DAC_SC_IRQ);
#endif

	uart_printf("DAC start.\r\n");
	
	return DRV_STATUS_OK;
}

drv_status_t codec_dac_restart(void)
{
	hal_codec_activate_dac(true);
	hal_codec_activate_dac_audio_interface(true);
	hal_codec_activate_hp(true);

	hal_codec_unmute_dac();
	hal_codec_unmute_hp();

	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to stop the DAC audio interface
 * @return	
 * 	@retval 0 DAC is successfully stopped.
 */
drv_status_t codec_dac_stop(void)
{
	hal_codec_mute_dac();
	hal_codec_activate_hp(false);
	hal_codec_mute_hp();
	hal_codec_activate_dac(false);
	hal_codec_activate_dac_audio_interface(false);

	return DRV_STATUS_OK;
}

drv_status_t codec_adc_start(void)
{
#if(CODEC_ADC12_ENABLE)
	codec_adc12_start();
#endif
#if(CODEC_ADC34_ENABLE)
	codec_adc34_start();
#endif
#if(CODEC_ADC56_ENABLE)
	codec_adc56_start();
#endif
#if(CODEC_ADC78_ENABLE)
	codec_adc78_start();
#endif
#if(CODEC_ADC9A_ENABLE)
	codec_adc9a_start();
#endif
	return DRV_STATUS_OK;
}

#if (CODEC_ADC_ENABLE)
drv_status_t codec_adc_stop(void)
{
#if CODEC_ADC12_ENABLE
	codec_adc12_stop();
#endif

#if CODEC_ADC34_ENABLE
	codec_adc34_stop();
#endif

#if CODEC_ADC56_ENABLE
	codec_adc56_stop();
#endif

#if CODEC_ADC78_ENABLE
	codec_adc78_stop();
#endif

#if CODEC_ADC9A_ENABLE
	codec_adc9a_stop();
#endif

	return DRV_STATUS_OK;
}
#endif

drv_status_t codec_adc12_start(void)
{
	if (g_adc12_config.is_amic)
	{
		/* Disable DMIC1 and DMIC2 */
		hal_codec_activate_dmic1(false);
		hal_codec_activate_dmic2(false);
		
#if	(ADC_MICBIAS)
		hal_codec_activate_amic1_bias(true);
		hal_codec_activate_amic2_bias(true);
#endif

#if (SUPPORT_ALTOTOOL_CODEDC)
		hal_codec_activate_adc1(gAdcCfg.chxCfg[ADC_1].en);
		hal_codec_activate_adc2(gAdcCfg.chxCfg[ADC_2].en);
#else
		hal_codec_activate_adc1(true);
		hal_codec_activate_adc2(true);
#endif
	}
	else
	{
		hal_codec_activate_amic1_bias(false);
		hal_codec_activate_amic2_bias(false);
		
		hal_codec_activate_dmic1(true);
		hal_codec_activate_dmic2(true);
	}

	/* Activate ADC12 */
	hal_codec_activate_adc12_audio_interface(true);
#if (SUPPORT_ALTOTOOL_CODEDC)
	if((gAdcCfg.chxCfg[ADC_1].mute == false)||(gAdcCfg.chxCfg[ADC_2].mute == false))
		hal_codec_unmute_adc12();
#else
	hal_codec_unmute_adc12();
#endif

#if !CODEC_USE_ONE_IRQ
	hal_interrupt_register_isr_handler(CODEC_ADC_IRQ, codec_adc_isr_handler);
	hal_interrupt_enable_irq(CODEC_ADC_IRQ);
#endif

//	uart_printf("ADC start.\r\n");
	
	return DRV_STATUS_OK;
}


/** 
 * @brief This function is used to stop the ADC12 audio interface
 * @return	
 * 	@retval 0 ADC12 is successfully stopped.
 */
drv_status_t codec_adc12_stop(void)
{
	hal_codec_mute_adc12();
#if	(ADC_MICBIAS)
	hal_codec_activate_amic1_bias(false);
	hal_codec_activate_amic2_bias(false);
#endif

	hal_codec_activate_adc1(false);
	hal_codec_activate_adc2(false);
	
	/* Disable DMIC1 and DMIC2 */
	hal_codec_activate_dmic1(false);
	hal_codec_activate_dmic2(false);
		
	/* Deactivate ADC12 */
	hal_codec_mute_adc12();
	hal_codec_activate_adc12_audio_interface(false);

	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to start the ADC34 audio interface
 * @return	
 * 	@retval 0 ADC34 is successfully started.
 */
drv_status_t codec_adc34_start(void)
{
	/* Activate ADC34 */
#if (SUPPORT_ALTOTOOL_CODEDC)
	hal_codec_activate_dmic3(gAdcCfg.chxCfg[ADC_3].en);
	hal_codec_activate_dmic4(gAdcCfg.chxCfg[ADC_4].en);
#else
	hal_codec_activate_dmic3(true);
	hal_codec_activate_dmic4(true);
#endif
	
	hal_codec_activate_adc34_audio_interface(true);
#if (SUPPORT_ALTOTOOL_CODEDC)
	if((gAdcCfg.chxCfg[ADC_3].mute == false)||(gAdcCfg.chxCfg[ADC_4].mute == false))
		hal_codec_unmute_adc34();
#else
		hal_codec_unmute_adc34();
#endif


	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to stop the ADC34 audio interface
 * @return	
 * 	@retval 0 ADC34 is successfully stopped.
 */
drv_status_t codec_adc34_stop(void)
{
	/* Deactivate ADC34 */
	hal_codec_mute_adc34();
	hal_codec_activate_dmic3(false);
	hal_codec_activate_dmic4(false);
	
	hal_codec_activate_adc34_audio_interface(false);

	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to start the ADC56 audio interface
 * @return	
 * 	@retval 0 ADC56 is successfully started.
 */
drv_status_t codec_adc56_start(void)
{
	/* Activate ADC56 */
#if (SUPPORT_ALTOTOOL_CODEDC)
	hal_codec_activate_dmic5(gAdcCfg.chxCfg[ADC_5].en);
	hal_codec_activate_dmic6(gAdcCfg.chxCfg[ADC_6].en);
#else
	hal_codec_activate_dmic5(true);
	hal_codec_activate_dmic6(true);
#endif

	hal_codec_activate_adc56_audio_interface(true);

#if (SUPPORT_ALTOTOOL_CODEDC)
	if((gAdcCfg.chxCfg[ADC_5].mute == false)||(gAdcCfg.chxCfg[ADC_6].mute == false))
		hal_codec_unmute_adc56();
#else
	hal_codec_unmute_adc56();
#endif


	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to stop the ADC56 audio interface
 * @return	
 * 	@retval 0 ADC56 is successfully stopped.
 */
drv_status_t codec_adc56_stop(void)
{
	/* Deactivate ADC56 */
	hal_codec_activate_dmic5(false);
	hal_codec_activate_dmic6(false);
	hal_codec_mute_adc56();
	hal_codec_activate_adc56_audio_interface(false);

	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to start the ADC78 audio interface
 * @return	
 * 	@retval 0 ADC78 is successfully started.
 */
drv_status_t codec_adc78_start(void)
{
	/* Activate ADC78 */
	hal_codec_activate_dmic7(true);
	hal_codec_activate_dmic8(true);
	
	hal_codec_activate_adc78_audio_interface(true);
	hal_codec_unmute_adc78();

	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to stop the ADC78 audio interface
 * @return	
 * 	@retval 0 ADC78 is successfully stopped.
 */
drv_status_t codec_adc78_stop(void)
{
	/* Deactivate ADC78 */
	hal_codec_mute_adc78();
	hal_codec_activate_dmic7(false);
	hal_codec_activate_dmic8(false);
	
	hal_codec_activate_adc78_audio_interface(false);

	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to start the ADC9a audio interface
 * @return	
 * 	@retval 0 ADC9a is successfully started.
 */
drv_status_t codec_adc9a_start(void)
{
	/* Activate ADC9a */
	hal_codec_activate_dmic9(true);
	hal_codec_activate_dmic10(true);
	
	hal_codec_activate_adc9a_audio_interface(true);
	hal_codec_unmute_adc9a();

	return DRV_STATUS_OK;
}

/** 
 * @brief This function is used to stop the ADC9a audio interface
 * @return	
 * 	@retval 0 ADC9a is successfully stopped.
 */
drv_status_t codec_adc9a_stop(void)
{
	/* Deactivate ADC9a */
	hal_codec_activate_dmic9(false);
	hal_codec_activate_dmic10(false);
	hal_codec_mute_adc9a();
	hal_codec_activate_adc9a_audio_interface(false);

	return DRV_STATUS_OK;
}

#if (CODEC_MIC_AGC_ENABLE)
/** 
 * @brief This function is used to enable or disable the AGC function of Microphones
 * @param[in] enable true:Enable AGC; false:Disable AGC
 */
void codec_mic_agc_enable(bool enable)
{
	hal_codec_enable_adc12_agc(enable);
	hal_codec_enable_adc34_agc(enable);
	hal_codec_enable_adc56_agc(enable);
	hal_codec_enable_adc78_agc(enable);
	hal_codec_enable_adc9a_agc(enable);
}
#endif

void codec_dac_left_input_enable(void)
{
	hal_codec_select_dac_aidacl_input(g_dac_config.aidacl_sel);
}

void codec_dac_right_input_enable(void)
{
	hal_codec_select_dac_aidacr_input(g_dac_config.aidacr_sel);
}

void codec_adc1_output_enable(void)
{
	hal_codec_select_adc1_output(g_adc12_config.aiadc1_sel);
}

void codec_adc2_output_enable(void)
{
	hal_codec_select_adc2_output(g_adc12_config.aiadc2_sel);
}

void codec_adc3_output_enable(void)
{
	hal_codec_select_adc3_output(g_adc34_config.aiadc3_sel);
}

void codec_adc4_output_enable(void)
{
	hal_codec_select_adc4_output(g_adc34_config.aiadc4_sel);
}

void codec_adc5_output_enable(void)
{
	hal_codec_select_adc5_output(g_adc56_config.aiadc5_sel);
}

void codec_adc6_output_enable(void)
{
	hal_codec_select_adc6_output(g_adc56_config.aiadc6_sel);
}

void codec_adc7_output_enable(void)
{
	hal_codec_select_adc7_output(g_adc78_config.aiadc7_sel);
}

void codec_adc8_output_enable(void)
{
	hal_codec_select_adc8_output(g_adc78_config.aiadc8_sel);
}

void codec_adc9_output_enable(void)
{
	hal_codec_select_adc9_output(g_adc9a_config.aiadc9_sel);
}

void codec_adc10_output_enable(void)
{
	hal_codec_select_adc10_output(g_adc9a_config.aiadc10_sel);
}

void codec_dac_left_input_disable(void)
{
	hal_codec_select_dac_aidacl_input(HAL_CODEC_CHANNEL_SEL_ZERO_INPUTS);
}

void codec_dac_right_input_disable(void)
{
	hal_codec_select_dac_aidacr_input(HAL_CODEC_CHANNEL_SEL_ZERO_INPUTS);
}

void codec_adc1_output_disable(void)
{
	hal_codec_select_adc1_output(HAL_CODEC_CHANNEL_SEL_ZERO_INPUTS);
}

void codec_adc2_output_disable(void)
{
	hal_codec_select_adc2_output(HAL_CODEC_CHANNEL_SEL_ZERO_INPUTS);
}

void codec_adc3_output_disable(void)
{
	hal_codec_select_adc3_output(HAL_CODEC_CHANNEL_SEL_ZERO_INPUTS);
}

void codec_adc4_output_disable(void)
{
	hal_codec_select_adc4_output(HAL_CODEC_CHANNEL_SEL_ZERO_INPUTS);
}

void codec_adc5_output_disable(void)
{
	hal_codec_select_adc5_output(HAL_CODEC_CHANNEL_SEL_ZERO_INPUTS);
}

void codec_adc6_output_disable(void)
{
	hal_codec_select_adc6_output(HAL_CODEC_CHANNEL_SEL_ZERO_INPUTS);
}

void codec_adc7_output_disable(void)
{
	hal_codec_select_adc7_output(HAL_CODEC_CHANNEL_SEL_ZERO_INPUTS);
}

void codec_adc8_output_disable(void)
{
	hal_codec_select_adc8_output(HAL_CODEC_CHANNEL_SEL_ZERO_INPUTS);
}

void codec_adc9_output_disable(void)
{
	hal_codec_select_adc9_output(HAL_CODEC_CHANNEL_SEL_ZERO_INPUTS);
}

void codec_adc10_output_disable(void)
{
	hal_codec_select_adc10_output(HAL_CODEC_CHANNEL_SEL_ZERO_INPUTS);
}


#ifdef CONFIG_CODEC_ENABLE_WT
/** 
 * @brief This function is used to initialize and enable the Whisper Trigger
 */
void codec_wt_init(void)
{
	hal_codec_set_wt_dmic_clock(HAL_CODEC_WT_DMIC_RATIO_16);
	hal_codec_select_wt_channel(HAL_CODEC_WT_CHANNEL_LEFT);
	hal_codec_wt_only(true);
	hal_codec_set_wt_sensitivity(31);
	hal_codec_unmask_wt_irq();
	hal_codec_enable_wt(true);
}

/** 
 * @brief This function is used to disable the Whisper Trigger
 */
void codec_wt_deinit(void)
{
	hal_codec_wt_only(false);
	hal_codec_enable_wt(false);
}
#endif

#ifdef CONFIG_CODEC_ENABLE_AIAS_IRQ
/** 
 * @brief This function is used to enable the AIAS Unlock interrupt
 */
void codec_enable_aias_unlock_irq(void)
{
	hal_codec_unmask_aiasunlock_irq();
}
#endif

/** 
 * @brief This function is used to set DAC sampling rate
 * @param[in] fs The sampling rate is set.
 */
void codec_set_dac_samplerate(uint32_t fs)
{
//	uart_printf("dac_samplerate:%d\n", fs);
	codec_dac_config_t *config = &g_dac_config;

	config->sample_rate = fs;
	hal_codec_set_dac_sample_rate(codec_samplerate_transform(config->sample_rate));
	
#if (CODEC_MASTER == 0)
	if (config->master_slave == CODEC_MODE_SLAVE)
		hal_clock_set_slave_dac_samplerate(fs);
#endif
}

#if (CODEC_ADC_ENABLE)
void codec_set_adc_samplerate(uint32_t fs)
{
//	uart_printf("adc_samplerate:%d\n", fs);
#if (CODEC_ADC12_ENABLE)
	codec_set_adc12_samplerate(fs);
#endif

#if (CODEC_ADC34_ENABLE || CODEC_ADC56_ENABLE)
	codec_set_adc3456_samplerate(fs);
#endif

#if (CODEC_ADC78_ENABLE || CODEC_ADC9A_ENABLE)
	codec_set_adc789a_samplerate(fs);
#endif
}
#endif

/** 
 * @brief This function is used to set ADC12 sampling rate
 * @param[in] fs The sampling rate is set.
 */
void codec_set_adc12_samplerate(uint32_t fs)
{
//	uart_printf("adc12_samplerate:%d\n", fs);

	codec_adc12_config_t *config = &g_adc12_config;
#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.gCfg[ADC_12].sr = fs;
#endif
	config->sample_rate = fs;
	hal_codec_set_adc12_sample_rate(codec_samplerate_transform(config->sample_rate));
	
#if (CODEC_MASTER == 0)
	if (config->master_slave == CODEC_MODE_SLAVE)
		hal_clock_set_slave_adc12_samplerate(fs);
#endif
}

/** 
 * @brief This function is used to set ADC3456 sampling rate
 * @param[in] fs The sampling rate is set.
 */
void codec_set_adc3456_samplerate(uint32_t fs)
{
	codec_adc3456_config_t *config = &g_adc3456_config;

#if (SUPPORT_ALTOTOOL_CODEDC)
	gAdcCfg.gCfg[ADC_3456].sr = fs;
#endif

	config->sample_rate = fs;
	hal_codec_set_adc3456_sample_rate(codec_samplerate_transform(config->sample_rate));
	
#if (CODEC_MASTER == 0)
	if (config->master_slave == CODEC_MODE_SLAVE)
		hal_clock_set_slave_adc3456_samplerate(fs);
#endif
}

/** 
 * @brief This function is used to set ADC789a sampling rate
 * @param[in] fs The sampling rate is set.
 */
void codec_set_adc789a_samplerate(uint32_t fs)
{
	codec_adc789a_config_t *config = &g_adc789a_config;

	config->sample_rate = fs;
	hal_codec_set_adc789a_sample_rate(codec_samplerate_transform(config->sample_rate));
	
#if (CODEC_MASTER == 0)
	if (config->master_slave == CODEC_MODE_SLAVE)
		hal_clock_set_slave_adc789a_samplerate(fs);
#endif
}


/** 
 * @brief This function is used to get DAC sampling rate.
 * @return	The returned DAC sampling rate.
 */
uint32_t codec_get_dac_samplerate(void)
{
	return g_dac_config.sample_rate;
}

#if (CODEC_ADC_ENABLE)
uint32_t codec_get_adc_samplerate(void)
{
	return codec_get_adc12_samplerate();
}
#endif

/** 
 * @brief This function is used to get ADC12 sampling rate.
 * @return	The returned ADC12 sampling rate.
 */
uint32_t codec_get_adc12_samplerate(void)
{
	return g_adc12_config.sample_rate;
}

/** 
 * @brief This function is used to get ADC3456 sampling rate.
 * @return	The returned ADC3456 sampling rate.
 */
uint32_t codec_get_adc3456_samplerate(void)
{
	return g_adc3456_config.sample_rate;
}

/** 
 * @brief This function is used to get ADC789a sampling rate.
 * @return	The returned ADC789a sampling rate.
 */
uint32_t codec_get_adc789a_samplerate(void)
{
	return g_adc789a_config.sample_rate;
}

/** 
 * @brief This function is used to set the headphone left analog gain.
 * @param[in] gain The analog gain is set. The range is [-19, 12] by step of 1 dB.
 */
void codec_set_hp_left_analog_gain(int8_t gain)
{
	g_dac_config.gol = gain;
	hal_codec_set_hp_gol(gain);
}

/** 
 * @brief This function is used to set the headphone right analog gain.
 * @param[in] gain The analog gain is set. The range is [-19, 12] by step of 1 dB.
 */
void codec_set_hp_right_analog_gain(int8_t gain)
{
	g_dac_config.gor = gain;
	hal_codec_set_hp_gor(gain);
}

/** 
 * @brief This function is used to set DAC left digital gain.
 * @param[in] gain The digital gain is set. The range is [-31, 32] by step of 1 dB.
 */
void codec_set_dac_left_digital_gain(int8_t gain)
{
	g_dac_config.godl = gain;
	hal_codec_set_dac_godl(gain);
}

/** 
 * @brief This function is used to set DAC right digital gain.
 * @param[in] gain The digital gain is set. The range is [-31, 32] by step of 1 dB.
 */
void codec_set_dac_right_digital_gain(int8_t gain)
{
	g_dac_config.godr = gain;
	hal_codec_set_dac_godr(gain);
}

/** 
 * @brief This function is used to set ADC1 analog gain.
 * @param[in] gain The analog gain is set. This parameter can only be one of the type #codec_gim_t
 */
void codec_set_adc1_analog_gain(codec_gim_t gain)
{
	g_adc12_config.amic.gim1 = gain;
	hal_codec_set_amic1_analog_gain((hal_codec_gim_t)gain);
}

/** 
 * @brief This function is used to set ADC2 analog gain.
 * @param[in] gain The analog gain is set. This parameter can only be one of the type #codec_gim_t
 */
void codec_set_adc2_analog_gain(codec_gim_t gain)
{
	g_adc12_config.amic.gim2 = gain;
	hal_codec_set_amic2_analog_gain((hal_codec_gim_t)gain);
}

/** 
 * @brief This function is used to set ADC1 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc1_digital_gain(int8_t gain)
{
	if (g_adc12_config.is_amic) //Analog Microphone
	{
		g_adc12_config.amic.gid1 = gain;
	}
	else
	{
		g_adc12_config.dmic.gid1 = gain;
	}
	hal_codec_set_adc1_digital_gain(gain);
}

/** 
 * @brief This function is used to set ADC2 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc2_digital_gain(int8_t gain)
{
	if (g_adc12_config.is_amic) //Analog Microphone
	{
		g_adc12_config.amic.gid2 = gain;
	}
	else
	{
		g_adc12_config.dmic.gid2 = gain;
	}
	hal_codec_set_adc2_digital_gain(gain);
}

/** 
 * @brief This function is used to set ADC3 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc3_digital_gain(int8_t gain)
{
	g_adc34_config.gid3 = gain;
	hal_codec_set_adc3_digital_gain(gain);
}

/** 
 * @brief This function is used to set ADC4 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc4_digital_gain(int8_t gain)
{
	g_adc34_config.gid4 = gain;
	hal_codec_set_adc4_digital_gain(gain);
}

/** 
 * @brief This function is used to set ADC5 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc5_digital_gain(int8_t gain)
{
	g_adc56_config.gid5 = gain;
	hal_codec_set_adc5_digital_gain(gain);
}

/** 
 * @brief This function is used to set ADC6 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc6_digital_gain(int8_t gain)
{
	g_adc56_config.gid6 = gain;
	hal_codec_set_adc6_digital_gain(gain);
}

/** 
 * @brief This function is used to set ADC7 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc7_digital_gain(int8_t gain)
{
	g_adc78_config.gid7 = gain;
	hal_codec_set_adc7_digital_gain(gain);
}

/** 
 * @brief This function is used to set ADC8 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc8_digital_gain(int8_t gain)
{
	g_adc78_config.gid8 = gain;
	hal_codec_set_adc8_digital_gain(gain);
}

/** 
 * @brief This function is used to set ADC9 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc9_digital_gain(int8_t gain)
{
	g_adc9a_config.gid9 = gain;
	hal_codec_set_adc9_digital_gain(gain);
}

/** 
 * @brief This function is used to set ADC10 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc10_digital_gain(int8_t gain)
{
	g_adc9a_config.gid10 = gain;
	hal_codec_set_adc10_digital_gain(gain);
}

/** 
 * @brief This function is used to get ADC1/ADC2 digital or analog attribute.
 */
bool codec_get_adc12_attribute(void)
{
	return (g_adc12_config.is_amic);
}

/**
 * @brief This function is used to calculate the sample rate tuning control word
 * @note Note that the difference between fs and fs0 must not exceed +/-3% to ensure correct audio performances.
 * @param[in] s The proportion the sample rate want to be tuned, must not exceed +/-3%
 * @param[in] fs0 The standard sample rate.
 * @return The sample rate tuning control word
 */
static uint16_t codec_calc_master_sample_rate_adjusted_value(float s, uint32_t fs0)
{
	/* Fix Me, Should use Cadence floating point intrinsics to implement it
	 * The result should be rounded to the closest signed decimal value
	 */
	/* The equation is fs = (1+s)*fs0. Where fs is the sampling rate you want to get */

	return ((uint16_t)((int16_t)(128.0f * 12000000 * (1/(1+s)-1) / fs0)) & 0x7fff);
}

extern volatile uint32_t curSr;
void codec_dac_aias(float s, int32_t dir)
{
	static int32_t oldDir = 0xff;
#ifdef TEST_I2S_DAC_SYNC

	static uint32_t sr = 0;

	if (oldDir == dir && sr == curSr)
		return;

	oldDir = dir;
	sr = curSr;

	switch(curSr)
	{
	case 44100:
		s = 0.001360;
		break;
	case 48000:
		s = 0.000833333;
		break;
	case 88200:
		s = 0.001247165;
		break;
//	case 96000:
//		s = 0.001;
//		break;
	case 176000:
		s = 0.00367045;
	case 192000:
		s = 0.0090625;
		break;
	default:
		s=0;
		break;
	}

	if(dir > 0)
	{
		s += DAC_TUNE_STEP;

	}
	else if(dir < 0)
	{
		s -= DAC_TUNE_STEP;
	}
#else
	if (oldDir == dir)
		return;

	oldDir = dir;

	if(dir < 0)
	{
		s = -s;
	}
	else if(dir == 0)
	{
		s = 0;
	}
#endif

#if (CODEC_MASTER)
	uint16_t adj = codec_calc_master_sample_rate_adjusted_value(s, g_dac_config.sample_rate);
	hal_codec_tune_dac_sample_rate_valid(false);
	hal_codec_tune_dac_sample_rate(adj);
	hal_codec_tune_dac_sample_rate_valid(true);
#else
	uint32_t fs = (uint32_t)((1 + s) * g_dac_config.sample_rate);
	hal_clock_set_slave_dac_samplerate(fs);
#endif
}


/**
 * @brief This function is used to adjust the codec ADC12 sample rate
 * @param[in] s The proportion the sample rate want to be tuned, must not exceed +/-3%
 */
void codec_adc12_aias(float s, int32_t dir)
{
	static int32_t oldDir = 0xff;
	if (oldDir == dir)
		return;

	oldDir = dir;

	if(dir < 0)
	{
		s = -s;
	}
	else if(dir == 0)
	{
		s = 0;
	}

#if (CODEC_MASTER)
	uint16_t adj = codec_calc_master_sample_rate_adjusted_value(s, g_adc12_config.sample_rate);
	hal_codec_tune_adc12_sample_rate_valid(false);	//Set validation to be false before tuning sampling rate
	hal_codec_tune_adc12_sample_rate(adj);			//Tuning sampling rate.
	hal_codec_tune_adc12_sample_rate_valid(true); 	//Validate the sampling rate tuning.
#else
	uint32_t fs = (uint32_t)((1 + s) * g_adc12_config.sample_rate);
	hal_clock_set_slave_adc12_samplerate(fs);
#endif
}


/**
 * @brief This function is used to adjust the codec ADC3456 sample rate
 * @param[in] s The proportion the sample rate want to be tuned, must not exceed +/-3%
 */
void codec_adc3456_aias(float s, int32_t dir)
{
	static int32_t oldDir = 0xff;
	if (oldDir == dir)
		return;

	oldDir = dir;

	if(dir < 0)
	{
		s = -s;
	}
	else if(dir == 0)
	{
		s = 0;
	}

#if (CODEC_MASTER)
	uint16_t adj = codec_calc_master_sample_rate_adjusted_value(s, g_adc3456_config.sample_rate);
	hal_codec_tune_adc3456_sample_rate_valid(false);	//Set validation to be false before tuning sampling rate
	hal_codec_tune_adc3456_sample_rate(adj);			//Tuning sampling rate.
	hal_codec_tune_adc3456_sample_rate_valid(true); 	//Validate the sampling rate tuning.
#else
	uint32_t fs = (uint32_t)((1 + s) * g_adc3456_config.sample_rate);
	hal_clock_set_slave_adc3456_samplerate(fs);
#endif
}

/**
 * @brief This function is used to adjust the codec ADC789a sample rate
 * @param[in] s The proportion the sample rate want to be tuned, must not exceed +/-3%
 */
void codec_adc789a_aias(float s, int32_t dir)
{
	static int32_t oldDir = 0xff;
	if (oldDir == dir)
		return;

	oldDir = dir;

	if(dir < 0)
	{
		s = -s;
	}
	else if(dir == 0)
	{
		s = 0;
	}

#if (CODEC_MASTER)
	uint16_t adj = codec_calc_master_sample_rate_adjusted_value(s, g_adc3456_config.sample_rate);
	hal_codec_tune_adc789a_sample_rate_valid(false);	//Set validation to be false before tuning sampling rate
	hal_codec_tune_adc789a_sample_rate(adj);			//Tuning sampling rate.
	hal_codec_tune_adc789a_sample_rate_valid(true); 	//Validate the sampling rate tuning.
#else
	uint32_t fs = (uint32_t)((1 + s) * g_adc3456_config.sample_rate);
	hal_clock_set_slave_adc789a_samplerate(fs);
#endif
}

void codec_tune_dac_sample_rate(uint32_t dataLen, uint32_t fifoSize)
{
#if (DAC_AIAS)
	if (dataLen <= (fifoSize >> 3))
	{
		codec_dac_aias(DAC_TUNE_STEP, TUNE_DOWN);
	}

	/* Buffer becomes bigger, samplerate should be up */
	if (dataLen >= ((fifoSize >> 3) * 7))
	{
		codec_dac_aias(DAC_TUNE_STEP, TUNE_UP);
	}

	/* Buffer becomes normal, samplerate should be change to standard one */
	if ((dataLen >= ((fifoSize >> 4) * 7)) && (dataLen <= ((fifoSize >> 4) * 9)))
	{
		codec_dac_aias(DAC_TUNE_STEP, NO_TUNE);
	}
#endif
}

#if (CODEC_ADC_ENABLE)
void codec_tune_adc_sample_rate(uint32_t dataLen, uint32_t fifoSize)
{
#if (ADCX_AIAS)
	if (dataLen <= (fifoSize >> 3))
	{
		codec_adc12_aias(ADC_TUNE_STEP, TUNE_UP);
#if (CODEC_ADC34_ENABLE || CODEC_ADC56_ENABLE)
		codec_adc3456_aias(ADC_TUNE_STEP, TUNE_UP);
#endif
#if (CODEC_ADC78_ENABLE || CODEC_ADC9A_ENABLE)
		codec_adc789a_aias(ADC_TUNE_STEP, TUNE_UP);
#endif
	}
	
	/* Buffer becomes bigger, samplerate should be up */
	if (dataLen >= ((fifoSize >> 3) * 7))
	{
		codec_adc12_aias(ADC_TUNE_STEP, TUNE_DOWN);
#if (CODEC_ADC34_ENABLE || CODEC_ADC56_ENABLE)
		codec_adc3456_aias(ADC_TUNE_STEP, TUNE_DOWN);
#endif
#if (CODEC_ADC78_ENABLE || CODEC_ADC9A_ENABLE)
		codec_adc789a_aias(ADC_TUNE_STEP, TUNE_DOWN);
#endif
	}
	
	/* Buffer becomes normal, samplerate should be change to standard one */
	if ((dataLen >= ((fifoSize >> 4) * 7)) && (dataLen <= ((fifoSize >> 4) * 9)))
	{
		codec_adc12_aias(ADC_TUNE_STEP, NO_TUNE);
#if (CODEC_ADC34_ENABLE || CODEC_ADC56_ENABLE)
		codec_adc3456_aias(ADC_TUNE_STEP, NO_TUNE);
#endif
#if (CODEC_ADC78_ENABLE || CODEC_ADC9A_ENABLE)
		codec_adc789a_aias(ADC_TUNE_STEP, NO_TUNE);
#endif	
	}
#endif
}
#endif

void codec_tune_adc12_sample_rate(uint32_t dataLen, uint32_t fifoSize)
{
#if (ADC12_AIAS)
	if (dataLen <= (fifoSize >> 3))
	{
		codec_adc12_aias(ADC_TUNE_STEP, TUNE_UP);
	}
	
	/* Buffer becomes bigger, samplerate should be up */
	if (dataLen >= ((fifoSize >> 3) * 7))
	{
		codec_adc12_aias(ADC_TUNE_STEP, TUNE_DOWN);
	}
	
	/* Buffer becomes normal, samplerate should be change to standard one */
	if ((dataLen >= ((fifoSize >> 4) * 7)) && (dataLen <= ((fifoSize >> 4) * 9)))
	{
		codec_adc12_aias(ADC_TUNE_STEP, NO_TUNE);
	}
#endif
}

void codec_tune_adc3456_sample_rate(uint32_t dataLen, uint32_t fifoSize)
{
#if (ADC3456_AIAS)
	if (dataLen <= (fifoSize >> 3))
	{
#if (!CODEC_ADC12_ENABLE)
		codec_adc12_aias(ADC_TUNE_STEP, TUNE_UP);
#endif
		codec_adc3456_aias(ADC_TUNE_STEP, TUNE_UP);
	}
	
	/* Buffer becomes bigger, samplerate should be up */
	if (dataLen >= ((fifoSize >> 3) * 7))
	{
#if (!CODEC_ADC12_ENABLE)
		codec_adc12_aias(ADC_TUNE_STEP, TUNE_DOWN);
#endif
		codec_adc3456_aias(ADC_TUNE_STEP, TUNE_DOWN);
	}
	
	/* Buffer becomes normal, samplerate should be change to standard one */
	if ((dataLen >= ((fifoSize >> 4) * 7)) && (dataLen <= ((fifoSize >> 4) * 9)))
	{
#if (!CODEC_ADC12_ENABLE)
		codec_adc12_aias(ADC_TUNE_STEP, NO_TUNE);
#endif
		codec_adc3456_aias(ADC_TUNE_STEP, NO_TUNE);
	}
#endif
}

void codec_tune_adc789a_sample_rate(uint32_t dataLen, uint32_t fifoSize)
{
#if (ADC789a_AIAS)
	if (dataLen <= (fifoSize >> 3))
	{
#if (!CODEC_ADC12_ENABLE)
		codec_adc12_aias(ADC_TUNE_STEP, TUNE_UP);
#endif
		codec_adc789a_aias(ADC_TUNE_STEP, TUNE_UP);
	}
	
	/* Buffer becomes bigger, samplerate should be up */
	if (dataLen >= ((fifoSize >> 3) * 7))
	{
#if (!CODEC_ADC12_ENABLE)
		codec_adc12_aias(ADC_TUNE_STEP, TUNE_DOWN);
#endif
		codec_adc789a_aias(ADC_TUNE_STEP, TUNE_DOWN);
	}
	
	/* Buffer becomes normal, samplerate should be change to standard one */
	if ((dataLen >= ((fifoSize >> 4) * 7)) && (dataLen <= ((fifoSize >> 4) * 9)))
	{
#if (!CODEC_ADC12_ENABLE)
		codec_adc12_aias(ADC_TUNE_STEP, NO_TUNE);
#endif
		codec_adc789a_aias(ADC_TUNE_STEP, NO_TUNE);
	}
#endif
}
/** 
 * @brief This function is used to register the callback function of the ISRs. The callback function can handle the Codec datas.
 * @param[in] index The location the callback function is registered. This parameter can only be one of the type #codec_isr_cb_t
 * @param[in] cb The callback function is registered.
 * @return	
 * 	@retval 0 The callback function is successfully registered
 */
#if(AUDIO_MANAGER)
drv_status_t codec_register_cb(codec_isr_cb_t index, auTransmitCompleteCB_t cb)
{
	ASSERT(index < CODEC_CALLBACK_NUM);

	codecDevInfos[index].isrCb = cb;

	return DRV_STATUS_OK;
}
#else
int codec_register_cb(codec_isr_cb_t index, codec_isr_cb_func cb)
{
	ASSERT(index < CODEC_CALLBACK_NUM);
	
	g_codec_cb_table[index] = cb;

	return 0;
}
#endif


#ifdef __cplusplus
}
#endif
#endif

