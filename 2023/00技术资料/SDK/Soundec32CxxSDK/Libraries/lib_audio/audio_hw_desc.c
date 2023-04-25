/** 
***********************************************************
 * 
 * @file name	: audio_hw_desc.c
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2022-02-11
 * @brief		: Audio hardware device controller and register interface.
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

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "audio_def.h"
#include "audio_hw_if.h"
#include "audio_session.h"
#include "boardConfig.h"
#include "audio_stream.h"

#if (SUPPORT_DSP_EQ)
#include "dsp_eq.h"
#endif

#if (USB_ENABLE)
#include "drv_usb.h"
#include "uac.h"
#include "uac_if.h"
#endif

#if (CODEC_ENABLE)
#include "drv_codec.h"
#endif

#if (I2S_ENABLE)
#include "drv_i2s.h"
#endif

#if (AUDIO_MANAGER)

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern int8_t auRxCompleteCB(void *arg, void *data, uint32_t *len, auSlotSize_t bitSlot);
extern int8_t auTxCompleteCB(void *arg, void *data, uint32_t *len, auSlotSize_t bitSlot);

void auStream_cal_gain(int32_t *gain, int32_t *__restrict in, int32_t *__restrict out);
void auStream_update_info(void *arg, auCtrlPath_t path, auInfoType_t type, void *info);


/* Private functions ---------------------------------------------------------*/
#if (CODEC_ADC_ENABLE)
static auStreamOps_t auDrvTransOps_adc_in={
	codec_set_adc_samplerate,	/*codec_set_adc9a_sample_rate,*/
	NULL,
	codec_reset_adc_bitslot,
	codec_set_adc_gain,			/*codec_adc_set_gain,,*/
	NULL,
	codec_set_adc_mute,			/*codec_adc_set_mute,*/
	auStream_detect_capture_signal,
	NULL,
};

static auDriverOps_t auDrvOps_adc_in={
	codec_adc_init,				/*codec_adc_init,*/
	codec_adc_deinit,				/*codec_adc_de_init,*/
	codec_adc_reinit,				/*codec_adc_re_init,*/
	codec_adc_start,				/*codec_adc_start,*/
	codec_adc_stop,					/*codec_adc_stop,*/
	NULL,							/*codec_adc_sleep,*/
	NULL,							/*codec_adc12_wakeup,*/
};

auDriverState_t auDrvSt_adc_in={
	.isInitialized = FALSE,
	codec_adc_get_mute,				/*codec_adc_get_mute,*/
	NULL,							/*codec_adc_get_play_status,*/
	codec_get_adc_samplerate,		/*codec_adc_get_sample_rate,*/
	NULL,
	codec_get_adc_gain, 			/*codec_adc_get_gain,*/
};
	
auDevFormatCapacity_t adcFormatCap = {
	.sampleRateHzMax 	= SAMPLING_RATE_192000,
	.channlesNumMax		= 2,
	.wordSize			= BIT_SLOT_32,
	.frameLevel			= 8,
	.frameLength		= 16,
};

auDevAttr_t auDrvAttr_adc = {
	.formatCap			= &adcFormatCap,
	.isrCb 				= auRxCompleteCB,
	.updateInfoCb		= auStream_update_info,
	.calSwGainCb		= auStream_cal_gain,
#if (SUPPORT_DSP_EQ)
	.doEqCb				= mic_equ_do_process,
#endif
	.enable 			= FALSE,
	.useSwGain 			= FALSE,
	.useDMA 			= FALSE,
	.dmaChannel 		= DMAC_CHX_NULL,
	.ops 				= &auDrvTransOps_adc_in,
	.pStream 			= NULL,
	.pUserData 			= NULL,
};

auDriverConfig_t auDrvCfg_adc = 
{
	.format 			= NULL,
	.attr 				= &auDrvAttr_adc,
};
#endif

#if (CODEC_ADC12_ENABLE)
static auStreamOps_t auDrvTransOps_mems12_in={
	codec_set_adc12_samplerate,	/*codec_set_adc9a_sample_rate,*/
	codec_tune_adc12_sample_rate,
	codec_reset_adc_bitslot,
	codec_set_adc12_gain,			/*codec_adc12_set_gain,,*/
	NULL,
	codec_set_adc12_mute,			/*codec_adc12_set_mute,*/
	auStream_detect_capture_signal,
	NULL,
};

static auDriverOps_t auDrvOps_mems12_in={
	codec_adc12_init,				/*codec_adc12_init,*/
	codec_adc12_deinit,				/*codec_adc12_de_init,*/
	codec_adc12_reinit,				/*codec_adc12_re_init,*/
	codec_adc12_start,				/*codec_adc12_start,*/
	codec_adc12_stop,				/*codec_adc12_stop,*/
	NULL,							/*codec_adc12_sleep,*/
	NULL,							/*codec_adc12_wakeup,*/
};

auDriverState_t auDrvSt_mems12_in={
	.isInitialized = FALSE,
	codec_adc12_get_mute,			/*codec_adc12_get_mute,*/
	NULL,							/*codec_adc12_get_play_status,*/
	codec_get_adc12_samplerate,		/*codec_adc12_get_sample_rate,*/
	NULL,
	codec_get_adc12_gain, 			/*codec_adc12_get_gain,*/
};
	
auDevFormatCapacity_t mems12FormatCap = {
	.sampleRateHzMax 	= SAMPLING_RATE_192000,
	.channlesNumMax		= 2,
	.wordSize			= BIT_SLOT_32,
	.frameLevel			= 8,
	.frameLength		= 16,
};

auDevAttr_t auDrvAttr_mems12 = {
	.formatCap 			= &mems12FormatCap,
	.isrCb 				= auRxCompleteCB,
	.updateInfoCb		= auStream_update_info,
	.calSwGainCb		= auStream_cal_gain,
#if (SUPPORT_DSP_EQ)
	.doEqCb				= mic_equ_do_process,
#endif	
	.enable 			= FALSE,
	.useSwGain 			= FALSE,
	.useDMA 			= TRUE,
#if(DMA_ENABLE)
	.dmaChannel 		= DMA_CHX_ADC_TO_MEM,
#else
	.dmaChannel 		= DMAC_CHX_NULL,
#endif
	.ops 				= &auDrvTransOps_mems12_in,
	.pStream 			= NULL,
	.pUserData 			= NULL,
};

auDriverConfig_t auDrvCfg_mems12 = 
{
	.format 			= NULL,
	.attr 				= &auDrvAttr_mems12,
};
#endif

#if (CODEC_ADC34_ENABLE)
static auStreamOps_t auDrvTransOps_mems34_in={
	codec_set_adc3456_samplerate,	/*codec_set_adc9a_sample_rate,*/
	codec_tune_adc3456_sample_rate,
	codec_reset_adc_bitslot,
	codec_set_adc34_gain,			/*codec_adc34_set_gain,,*/
	NULL,
	codec_set_adc34_mute,			/*codec_adc34_set_mute,*/
	auStream_detect_capture_signal,
	NULL,
};

static auDriverOps_t auDrvOps_mems34_in={
	codec_adc34_init,				/*codec_adc34_init,*/
	codec_adc34_deinit,				/*codec_adc34_de_init,*/
	codec_adc34_reinit,				/*codec_adc34_re_init,*/
	codec_adc34_start,				/*codec_adc34_start,*/
	codec_adc34_stop,				/*codec_adc34_stop,*/
	NULL,							/*codec_adc34_sleep,*/
	NULL,							/*codec_adc34_wakeup,*/
};
	
auDriverState_t auDrvSt_mems34_in={
	.isInitialized = FALSE,
	codec_adc34_get_mute,			/*codec_adc34_get_mute,*/
	NULL,							/*codec_adc34_get_play_status,*/
	codec_get_adc3456_samplerate,	/*codec_adc34_get_sample_rate,*/
	NULL,
	codec_get_adc34_gain,			/*codec_adc34_get_gain,*/
};

auDevFormatCapacity_t mems34FormatCap = {
	.sampleRateHzMax 	= SAMPLING_RATE_192000,
	.channlesNumMax		= 2,
	.wordSize			= BIT_SLOT_32,
	.frameLevel			= 8,
	.frameLength		= 16,
};

auDevAttr_t auDrvAttr_mems34 = {
	.formatCap 			= &mems34FormatCap,
	.isrCb 				= auRxCompleteCB,
	.updateInfoCb		= auStream_update_info,
	.calSwGainCb		= auStream_cal_gain,
#if (SUPPORT_DSP_EQ)
	.doEqCb				= mic_equ_do_process,
#endif
	.useDMA 			= FALSE,
	.useSwGain 			= FALSE,
	.dmaChannel 		= DMAC_CHX_NULL,
	.enable 			= FALSE,
	.ops 				= &auDrvTransOps_mems34_in,
	.pStream 			= NULL,
	.pUserData 			= NULL,
};

auDriverConfig_t auDrvCfg_mems34 = 
{
	.format 			= NULL,
	.attr 				= &auDrvAttr_mems34,
};
#endif

#if (CODEC_ADC56_ENABLE)
static auStreamOps_t auDrvTransOps_mems56_in={
	codec_set_adc3456_samplerate,	/*codec_set_adc9a_sample_rate,*/
	codec_tune_adc3456_sample_rate,
	codec_reset_adc_bitslot,
	codec_set_adc56_gain,			/*codec_adc56_set_gain,,*/
	NULL,
	codec_set_adc56_mute,			/*codec_adc56_set_mute,*/
	auStream_detect_capture_signal,
	NULL,
};

static auDriverOps_t auDrvOps_mems56_in={
	codec_adc56_init,				/*codec_adc56_init,*/
	codec_adc56_deinit,				/*codec_adc56_de_init,*/
	codec_adc56_reinit,				/*codec_adc56_re_init,*/
	codec_adc56_start,				/*codec_adc56_start,*/
	codec_adc56_stop,				/*codec_adc56_stop,*/
	NULL,							/*codec_adc56_sleep,*/
	NULL,							/*codec_adc56_wakeup,*/
};

auDriverState_t auDrvSt_mems56_in={
	.isInitialized = FALSE,
	codec_adc56_get_mute,			/*codec_adc56_get_mute,*/
	NULL,							/*codec_adc56_get_play_status,*/
	codec_get_adc3456_samplerate,	/*codec_adc56_get_sample_rate,*/
	NULL,
	codec_get_adc56_gain,			/*codec_adc56_get_gain,*/
};
	
auDevFormatCapacity_t mems56FormatCap = {
	.sampleRateHzMax 	= SAMPLING_RATE_192000,
	.channlesNumMax		= 2,
	.wordSize			= BIT_SLOT_32,
	.frameLevel			= 8,
	.frameLength		= 16,
};

auDevAttr_t auDrvAttr_mems56 = {
	.formatCap 			= &mems56FormatCap,
	.isrCb 				= auRxCompleteCB,
	.updateInfoCb		= auStream_update_info,
	.calSwGainCb		= auStream_cal_gain,
#if (SUPPORT_DSP_EQ)
	.doEqCb				= mic_equ_do_process,
#endif	
	.useDMA 			= FALSE,
	.useSwGain 			= FALSE,
	.dmaChannel 		= DMAC_CHX_NULL,
	.enable 			= FALSE,
	.ops 				= &auDrvTransOps_mems56_in,
	.pStream 			= NULL,
	.pUserData 			= NULL,
};

auDriverConfig_t auDrvCfg_mems56 = 
{
	.format 			= NULL,
	.attr 				= &auDrvAttr_mems56,
};
#endif

#if (CODEC_ADC78_ENABLE)
static auStreamOps_t auDrvTransOps_mems78_in={
	codec_set_adc789a_samplerate,	/*codec_set_adc9a_sample_rate,*/
	codec_tune_adc789a_sample_rate,
	codec_reset_adc_bitslot,
	codec_set_adc78_gain,			/*codec_adc78_set_gain,,*/
	NULL,
	codec_set_adc78_mute,			/*codec_adc78_set_mute,*/
	auStream_detect_capture_signal,
	NULL,
};

static auDriverOps_t auDrvOps_mems78_in={
	codec_adc78_init,				/*codec_adc78_init,*/
	codec_adc78_deinit,				/*codec_adc78_de_init,*/
	codec_adc78_reinit,				/*codec_adc78_re_init,*/
	codec_adc78_start,				/*codec_adc78_start,*/
	codec_adc78_stop,				/*codec_adc78_stop,*/
	NULL,							/*codec_adc78_sleep,*/
	NULL,							/*codec_adc78_wakeup,*/
};
	
auDriverState_t auDrvSt_mems78_in={
	.isInitialized = FALSE,
	codec_adc78_get_mute,			/*codec_adc78_get_mute,*/
	NULL,							/*codec_adc78_get_play_status,*/
	codec_get_adc789a_samplerate,	/*codec_adc78_get_sample_rate,*/
	NULL,
	codec_get_adc56_gain,			/*codec_adc78_get_gain,*/
};
	
auDevFormatCapacity_t mems78FormatCap = {
	.sampleRateHzMax 	= SAMPLING_RATE_192000,
	.channlesNumMax		= 2,
	.wordSize			= BIT_SLOT_32,
	.frameLevel			= 8,
	.frameLength		= 16,
};
	
auDevAttr_t auDrvAttr_mems78 = {
	.formatCap 			= &mems78FormatCap,
	.isrCb 				= auRxCompleteCB,
	.updateInfoCb		= auStream_update_info,
	.calSwGainCb		= auStream_cal_gain,
#if (SUPPORT_DSP_EQ)
	.doEqCb				= mic_equ_do_process,
#endif	
	.useDMA 			= FALSE,
	.useSwGain 			= FALSE,
	.dmaChannel 		= DMAC_CHX_NULL,
	.enable 			= FALSE,
	.ops 				= &auDrvTransOps_mems78_in,
	.pStream 			= NULL,
	.pUserData 			= NULL,
};
	
auDriverConfig_t auDrvCfg_mems78 = 
{
	.format 			= NULL,
	.attr 				= &auDrvAttr_mems78,
};
#endif

#if (CODEC_ADC9A_ENABLE)
static auStreamOps_t auDrvTransOps_mems9a_in={
	codec_set_adc789a_samplerate,	/*codec_set_adc9a_sample_rate,*/
	codec_tune_adc789a_sample_rate,
	codec_reset_adc_bitslot,
	codec_set_adc9a_gain,			/*codec_adc9a_set_gain,,*/
	NULL,
	codec_set_adc9a_mute,			/*codec_adc9a_set_mute,*/
	auStream_detect_capture_signal,
};

static auDriverOps_t auDrvOps_mems9a_in={
	codec_adc9a_init,				/*codec_adc9a_init,*/
	codec_adc9a_deinit,				/*codec_adc9a_de_init,*/
	codec_adc9a_reinit,				/*codec_adc9a_re_init,*/
	codec_adc9a_start,				/*codec_adc9a_start,*/
	codec_adc9a_stop,				/*codec_adc9a_stop,*/
	NULL,							/*codec_adc9a_sleep,*/
	NULL,							/*codec_adc9a_wakeup,*/
};

auDriverState_t auDrvSt_mems9a_in={
	.isInitialized = FALSE,
	codec_adc9a_get_mute,			/*codec_adc9a_get_mute,*/
	NULL,							/*codec_adc9a_get_play_status,*/
	codec_get_adc789a_samplerate,	/*codec_adc9a_get_sample_rate,*/
	NULL,
	codec_get_adc9a_gain,			/*codec_adc9a_get_gain,*/
};
	
auDevFormatCapacity_t mems9aFormatCap = {
	.sampleRateHzMax 	= SAMPLING_RATE_192000,
	.channlesNumMax		= 2,
	.wordSize			= BIT_SLOT_32,
	.frameLevel			= 8,
	.frameLength		= 16,
};
	
auDevAttr_t auDrvAttr_mems9a = {
	.formatCap 			= &mems9aFormatCap,
	.isrCb 	 			= auRxCompleteCB,
	.updateInfoCb		= auStream_update_info,
	.calSwGainCb		= auStream_cal_gain,
#if (SUPPORT_DSP_EQ)
	.doEqCb				= mic_equ_do_process,
#endif	
	.useDMA 			= FALSE,
	.useSwGain 			= FALSE,
	.enable 			= FALSE,
	.dmaChannel			= DMAC_CHX_NULL,
	.ops	 			= &auDrvTransOps_mems9a_in,
	.pStream 			= NULL,
	.pUserData 			= NULL,
};
	
auDriverConfig_t auDrvCfg_mems9a = 
{
	.format 			= NULL,
	.attr 				= &auDrvAttr_mems9a,
};
#endif

#if (CODEC_DAC_ENABLE)
static auStreamOps_t auDrvTransOps_dac={
	codec_set_dac_samplerate,	/*codec_set_dac_sample_rate,*/
	codec_tune_dac_sample_rate,
	codec_reset_dac_bitslot,
	codec_set_dac_gain,			/*codec_dac_set_gain,,*/
	auStream_cal_gain,
	codec_set_dac_mute,			/*codec_dac_set_mute,*/
	NULL,
	auStream_detect_playback_signal,
};

static auDriverOps_t auDrvOps_dac={
	codec_dac_init,				/*codec_dac_init,*/
	codec_dac_deinit,			/*codec_dac_de_init,*/
	codec_dac_reinit,			/*codec_dac_re_init,*/
	codec_dac_start,			/*codec_dac_start,*/
	codec_dac_stop,				/*codec_dac_stop,*/
	NULL,/*codec_dac_sleep,*/
	NULL,/*codec_dac_wakeup,*/
};

auDriverState_t auDrvSt_dac={
	.isInitialized = FALSE,
	codec_dac_get_mute,			/*codec_dac_get_mute,*/
	NULL,						/*codec_dac_get_play_status,*/
	NULL,
	codec_get_dac_samplerate,	/*codec_dac_get_sample_rate,*/
	codec_get_dac_gain,			/*codec_dac_get_gain,*/
};
	
auDevFormatCapacity_t dacFormatCap = {
	.sampleRateHzMax 	= SAMPLING_RATE_192000,
	.channlesNumMax		= 2,
	.wordSize			= BIT_SLOT_32,
	.frameLevel			= 8,
	.frameLength		= 16,
};
	
auDevAttr_t auDrvAttr_dac = {
	.formatCap 			= &dacFormatCap,
	.isrCb 	 			= auTxCompleteCB,
	.updateInfoCb		= auStream_update_info,
	.calSwGainCb		= auStream_cal_gain,
#if (SUPPORT_DSP_EQ)
	.doEqCb				= spker_equ_do_process,
#endif	
	.useDMA 			= FALSE,
	.useSwGain 			= TRUE,
	.dmaChannel 		= DMAC_CHX_NULL,
	.enable 			= FALSE,
	.ops 				= &auDrvTransOps_dac,
	.pStream 			= NULL,
	.pUserData 			= NULL,
};
	
auDriverConfig_t auDrvCfg_dac = 
{
	.format 			= NULL,
	.attr 				= &auDrvAttr_dac,
};
#endif

#if (I2S_ENABLE)
auDevFormatCapacity_t i2sFormatCap = {
	.sampleRateHzMax 	= SAMPLING_RATE_192000,
	.channlesNumMax		= 2,
	.wordSize			= BIT_SLOT_32,
	.frameLevel			= 8,
	.frameLength		= 16,
};
#endif

#if (I2S1_ENABLE)
static auStreamOps_t auDrvTransOps_i2s1={
	i2s1_set_audio_freq,/*i2s1_set_sample_rate,*/
	NULL,
	NULL,
	NULL,/*i2s1_set_gain,,*/
	auStream_cal_gain,
	NULL,/*i2s1_set_mute,*/
	auStream_detect_capture_signal,
	auStream_detect_playback_signal,
};

static auDriverOps_t auDrvOps_i2s1={
	i2s1_init,
	i2s1_deinit,/*i2s1_de_init,*/
	i2s1_reinit,/*i2s1_re_init,*/
	i2s1_start,/*i2s1_start,*/
	NULL,/*i2s1_stop,*/
	NULL,/*i2s1_sleep,*/
	NULL,/*i2s1_wakeup,*/
};

auDriverState_t auDrvSt_i2s1={
	.isInitialized = FALSE,
	NULL,/*i2s1_get_mute,*/
	NULL,/*i2s1_get_play_status,*/
	i2s1_get_audio_freq,/*get_sample_rate,*/
	i2s1_get_audio_freq,/*get_sample_rate,*/
	NULL,/*i2s1_get_gain,*/
};
	
auDevAttr_t auDrvAttr_i2s1_rx = {
	.formatCap 			= &i2sFormatCap,
	.isrCb 	 			= auRxCompleteCB,
	.updateInfoCb		= auStream_update_info,
	.calSwGainCb		= auStream_cal_gain,
#if (SUPPORT_DSP_EQ)
	.doEqCb				= mic_equ_do_process,
#endif	
	.useDMA 			= FALSE,
	.useSwGain 			= TRUE,
	.dmaChannel 		= DMAC_CHX_NULL,
	.enable 			= FALSE,
	.ops 				= &auDrvTransOps_i2s1,
	.pStream 			= NULL,
	.pUserData 			= NULL,
};
	
auDriverConfig_t auDrvCfg_i2s1_rx = 
{
	.format 			= NULL,
	.attr 				= &auDrvAttr_i2s1_rx,
};
	
auDevAttr_t auDrvAttr_i2s1_tx = {
	.formatCap 			= &i2sFormatCap,
	.isrCb 	 			= auTxCompleteCB,
	.updateInfoCb		= auStream_update_info,
	.calSwGainCb		= auStream_cal_gain,
#if (SUPPORT_DSP_EQ)
	.doEqCb				= spker_equ_do_process,
#endif	
	.useDMA 			= FALSE,
	.useSwGain 			= TRUE,
	.dmaChannel 		= DMAC_CHX_NULL,
	.enable 			= FALSE,
	.ops		 		= &auDrvTransOps_i2s1,
	.pStream 			= NULL,
	.pUserData 			= NULL,
};
		
auDriverConfig_t auDrvCfg_i2s1_tx = 
{
	.format 			= NULL,
	.attr 				= &auDrvAttr_i2s1_tx,
};
#endif

#if (I2S2_ENABLE)
static auStreamOps_t auDrvTransOps_i2s2={
	i2s2_set_audio_freq,/*i2s2_set_sample_rate,*/
	NULL,
	NULL,/*i2s2_wakeup,*/
	NULL,/*i2s2_set_gain,,*/
	auStream_cal_gain,
	NULL,/*i2s2_set_mute,*/
	auStream_detect_capture_signal,
	auStream_detect_playback_signal,
};

static auDriverOps_t auDrvOps_i2s2={
	i2s2_init,
	i2s2_deinit,/*i2s2_de_init,*/
	i2s2_reinit,/*i2s2_re_init,*/
	i2s2_start,/*i2s2_start,*/
	NULL,/*i2s2_stop,*/
	NULL,/*i2s2_sleep,*/
	NULL,/*i2s2_wakeup,*/
};

auDriverState_t auDrvSt_i2s2={
	.isInitialized = FALSE,
	NULL,/*get_mute,*/
	NULL,/*get_play_status,*/
	i2s2_get_audio_freq,/*get_sample_rate,*/
	i2s2_get_audio_freq,/*get_sample_rate,*/
	NULL,/*get_gain,*/
};
	
auDevAttr_t auDrvAttr_i2s2_rx = {
	.formatCap 			= &i2sFormatCap,
	.isrCb 				= auRxCompleteCB,
	.updateInfoCb		= auStream_update_info,
	.calSwGainCb		= auStream_cal_gain,
#if (SUPPORT_DSP_EQ)
	.doEqCb				= mic_equ_do_process,
#endif
	.enable 			= FALSE,
	.useSwGain 			= TRUE,
	.useDMA 			= FALSE,
#if(DMA_ENABLE)
	.dmaChannel			= DMAC_CHX_NULL,
#endif
	.ops 				= &auDrvTransOps_i2s2,
	.pStream 			= NULL,
	.pUserData 			= NULL,
};
	
auDriverConfig_t auDrvCfg_i2s2_rx = 
{
	.format 			= NULL,
	.attr 				= &auDrvAttr_i2s2_rx,
};

auDevAttr_t auDrvAttr_i2s2_tx = {
	.formatCap 			= &i2sFormatCap,
	.isrCb 				= auTxCompleteCB,
	.updateInfoCb		= auStream_update_info,
	.calSwGainCb		= auStream_cal_gain,
#if (SUPPORT_DSP_EQ)
	.doEqCb				= spker_equ_do_process,
#endif
	.enable 			= FALSE,
	.useSwGain 			= TRUE,
	.useDMA 			= FALSE,
#if(DMA_ENABLE)
	.dmaChannel 		= DMA_CHX_MEM_TO_I2S1,
#endif
	.ops 				= &auDrvTransOps_i2s2,
	.pStream 			= NULL,
	.pUserData 			= NULL,
};
	
auDriverConfig_t auDrvCfg_i2s2_tx = 
{
	.format 			= NULL,
	.attr 				= &auDrvAttr_i2s2_tx,
};

#endif

#if (I2S3_ENABLE)
static auDriverOps_t auDrvOps_i2s3={
	i2s3_init,
	i2s3_deinit,/*i2s3_de_init,*/
	i2s3_reinit,/*i2s3_re_init,*/
	i2s3_start,/*i2s3_start,*/
	NULL,/*i2s3_stop,*/
	NULL,/*i2s3_sleep,*/
	NULL,/*i2s3_wakeup,*/
};

static auStreamOps_t auDrvTxOps_i2s3={
	i2s3_set_audio_freq,/*i2s3_set_sample_rate,*/
	NULL,/*tuneSampleRate*/
	NULL,/*transforBitSlot*/
	NULL,/*i2s3_set_gain,,*/
	auStream_cal_gain,
	NULL,/*i2s3_set_mute,*/
	auStream_detect_capture_signal,
	auStream_detect_playback_signal,
};

auDriverState_t auDrvSt_i2s3={
	.isInitialized = FALSE,
	NULL,/*get_mute,*/
	NULL,/*get_play_status,*/
	i2s3_get_audio_freq,/*get_sample_rate,*/
	i2s3_get_audio_freq,/*get_sample_rate,*/
	NULL,/*get_gain,*/
};
	
auDevAttr_t auDrvAttr_i2s3_rx = {
	.formatCap 			= &i2sFormatCap,
	.isrCb 	 			= auRxCompleteCB,
	.updateInfoCb		= auStream_update_info,
	.calSwGainCb		= auStream_cal_gain,
#if (SUPPORT_DSP_EQ)
	.doEqCb				= mic_equ_do_process,
#endif
	.enable 			= FALSE,
	.useDMA 			= FALSE,
	.useSwGain 			= TRUE,
	.dmaChannel 		= DMAC_CHX_NULL,
	.ops 				= &auDrvTxOps_i2s3,
	.pStream 			= NULL,
	.pUserData 			= NULL,
};
	
auDriverConfig_t auDrvCfg_i2s3_rx = 
{
	.format 			= NULL,
	.attr 				= &auDrvAttr_i2s3_rx,
};

auDevAttr_t auDrvAttr_i2s3_tx = {
	.formatCap 			= &i2sFormatCap,
	.isrCb 	 			= auTxCompleteCB,
	.updateInfoCb		= auStream_update_info,
	.calSwGainCb		= auStream_cal_gain,
#if (SUPPORT_DSP_EQ)
	.doEqCb				= spker_equ_do_process,
#endif	
	.enable 			= FALSE,
	.useDMA 			= FALSE,
	.useSwGain 			= TRUE,
	.dmaChannel 		= DMAC_CHX_NULL,
	.ops 				= &auDrvTxOps_i2s3,
	.pStream 			= NULL,
	.pUserData 			= NULL,
};
		
auDriverConfig_t auDrvCfg_i2s3_tx = 
{
	.format 			= NULL,
	.attr 				= &auDrvAttr_i2s3_tx,
};
#endif

#if (USB_ENABLE)
auDriverOps_t auDrvOps_usb={
	usb_init,
	usb_deinit,/*usb_deinit,*/
	usb_reinit,/*usb_reinit,*/
	NULL,/*usb_start,*/
	NULL,/*usb_stop,*/
	NULL,/*usb_sleep,*/
	NULL,/*usb_wakeup,*/
};

#if(SUPPORT_USB_FEEDBACK)
extern void uac_tune(uint32_t dataLen, uint32_t fifoSize);
#endif
auStreamOps_t auDrvRxOps_usb={
	NULL,/*usb_set_sample_rate,*/
#if(SUPPORT_USB_FEEDBACK)
	uac_tune,
#else
	NULL,
#endif
	usb_transform_bitslot,
	NULL,/*usb_set_gain,*/
	auStream_cal_gain,/*auStream_cal_gain,*/
	NULL,/*usb_set_mute,*/
	auStream_detect_capture_signal,
	auStream_detect_playback_signal,
};

auStreamOps_t auDrvTxOps_usb={
	NULL,/*usb_set_sample_rate,*/
	NULL,
	usb_transform_bitslot,
	NULL,/*usb_set_gain,*/
	auStream_cal_gain,/*auStream_cal_gain,*/
	NULL,/*usb_set_mute,*/
	auStream_detect_capture_signal,
	auStream_detect_playback_signal,
};

auDriverState_t auDrvSt_usb={
	.isInitialized = FALSE,
	NULL,/*usb_get_mute,*/
	NULL,/*usb_get_play_status,*/
	usb_dev_get_speaker_audio_sample,/*usb_get_sample_rate,*/
	usb_dev_get_mic_audio_sample,
	NULL,/*usb_get_gain,*/
};

auDevFormatCapacity_t usbMicFormatCap = {
	.sampleRateHzMax 	= SAMPLING_RATE_192000,
	.channlesNumMax		= 2,
	.wordSize			= BIT_SLOT_32,
	.frameLevel			= 8,
	.frameLength		= 20,
};
	
auDevAttr_t auDrvAttr_usb_mic = {
	.formatCap 			= &usbMicFormatCap,
	.isrCb 	 			= auTxCompleteCB,
	.updateInfoCb		= auStream_update_info,
	.calSwGainCb		= auStream_cal_gain,
#if (SUPPORT_DSP_EQ)
	.doEqCb				= spker_equ_do_process,
#endif
	.enable 			= FALSE,
	.useDMA 			= FALSE,
	.useSwGain 			= TRUE,
	.dmaChannel 		= DMAC_CHX_NULL,
	.ops 				= &auDrvTxOps_usb,
	.pStream 			= NULL,
	.pUserData 			= NULL,
};
	
auDriverConfig_t auDrvCfg_usb_mic = 
{
	.format 			= NULL,
	.attr 				= &auDrvAttr_usb_mic,
};

auDevFormatCapacity_t usbSpkrFormatCap = {
	.sampleRateHzMax 	= SAMPLING_RATE_192000,
	.channlesNumMax		= 2,
	.wordSize			= BIT_SLOT_32,
	.frameLevel			= 8,
	.frameLength		= 16,
};

auDevAttr_t auDrvAttr_usb_speaker = {
//	.role = {.uacSpec		= UAC_10,},
	.formatCap 			= &usbSpkrFormatCap,
	.isrCb 	 			= auRxCompleteCB,
	.updateInfoCb		= auStream_update_info,
	.calSwGainCb		= auStream_cal_gain,
#if (SUPPORT_DSP_EQ)
	.doEqCb				= spker_equ_do_process,
#endif
	.enable 			= FALSE,
	.useSwGain 			= TRUE,
	.useDMA 			= FALSE,
	.dmaChannel 		= DMAC_CHX_NULL,
	.ops 				= &auDrvRxOps_usb,
	.pStream 			= NULL,
	.pUserData 			= NULL,
};
	
auDriverConfig_t auDrvCfg_usb_speaker = 
{
	.format 			= NULL,
	.attr 				= &auDrvAttr_usb_speaker,
};
#endif

audioDevIf_t auDeviceIfs[AUDIO_DEVICE_MAX] = {
#if (CODEC_ADC_ENABLE)
	{
		.id = AUDIO_DEVICE_ADC_IN,
		.load = FALSE,
		.name = "adc",
		.rxCfgs = &auDrvCfg_adc,
		.txCfgs = NULL,
		.state = &auDrvSt_adc_in,
		.ops = &auDrvOps_adc_in,
		/*.userData = NULL;*/
	},
#endif
#if (CODEC_ADC12_ENABLE)
	{
		.id = AUDIO_DEVICE_MEMS12_IN,
		.load = FALSE,
		.name = "adc12",
		.rxCfgs = &auDrvCfg_mems12,
		.txCfgs = NULL,
		.state = &auDrvSt_mems12_in,
		.ops = &auDrvOps_mems12_in,
		/*.userData = NULL;*/
	},
#endif
#if (CODEC_ADC34_ENABLE)
	{
		.id = AUDIO_DEVICE_MEMS34_IN,
		.load = FALSE,
		.name = "adc34",
		.rxCfgs = &auDrvCfg_mems34,
		.txCfgs = NULL,
		.state = &auDrvSt_mems34_in,
		.ops = &auDrvOps_mems34_in,
		/*.userData = NULL;*/
	},
#endif
#if (CODEC_ADC56_ENABLE)
	{
		.id = AUDIO_DEVICE_MEMS56_IN,
		.name = "adc56",
		.load = FALSE,
		.rxCfgs = &auDrvCfg_mems56,
		.txCfgs = NULL,
		.state = &auDrvSt_mems56_in,
		.ops = &auDrvOps_mems56_in,
		/*.userData = NULL;*/
	},
#endif
#if (CODEC_ADC78_ENABLE)
	{
		.id = AUDIO_DEVICE_MEMS78_IN,
		.name = "adc78",
		.load = FALSE,
		.rxCfgs = &auDrvCfg_mems78,
		.txCfgs = NULL,
		.state = &auDrvSt_mems78_in,
		.ops = &auDrvOps_mems78_in,
		/*.userData = NULL;*/
	},
#endif
#if (CODEC_ADC9A_ENABLE)
	{
		.id = AUDIO_DEVICE_MEMS9A_IN,
		.name = "adc9a",
		.load = FALSE,
		.rxCfgs = &auDrvCfg_mems9a,
		.txCfgs = NULL,
		.state = &auDrvSt_mems9a_in,
		.ops = &auDrvOps_mems9a_in,
		/*.userData = NULL;*/
	},
#endif
//#endif

#if (CODEC_DAC_ENABLE)
	{
		.id = AUDIO_DEVICE_CODEC_OUT,
		.name = "codec",
		.load = FALSE,
		.rxCfgs = NULL,
		.txCfgs = &auDrvCfg_dac,
		.state = &auDrvSt_dac,
		.ops = &auDrvOps_dac,
		/*.userData = NULL;*/
	},
#endif


#if (I2S_ENABLE)
#if (I2S1_ENABLE)
	{
		.id = AUDIO_DEVICE_I2S1,
		.name = "i2s1",
		.load = FALSE,
		.rxCfgs = &auDrvCfg_i2s1_rx,
		.txCfgs = &auDrvCfg_i2s1_tx,
		.state = &auDrvSt_i2s1,
		.ops = &auDrvOps_i2s1,
		/*.userData = NULL;*/
	},
#endif
#if (I2S2_ENABLE)
	{
		.id = AUDIO_DEVICE_I2S2,
		.name = "i2s2",
		.load = FALSE,
		.rxCfgs = &auDrvCfg_i2s2_rx,
		.txCfgs = &auDrvCfg_i2s2_tx,
		.state = &auDrvSt_i2s2,
		.ops = &auDrvOps_i2s2,
		/*.userData = NULL;*/
	},
#endif
#if (I2S3_ENABLE)
	{
		.id = AUDIO_DEVICE_I2S3,
		.name = "i2s3",
		.load = FALSE,
		.rxCfgs = &auDrvCfg_i2s3_rx,
		.txCfgs = &auDrvCfg_i2s3_tx,
		.state = &auDrvSt_i2s3,
		.ops = &auDrvOps_i2s3,
		/*.userData = NULL;*/
	},
#endif
#endif

#if (USB_ENABLE == 1)
	{
		.id = AUDIO_DEVICE_USB,
		.name = "usb",
		.load = FALSE,
		.rxCfgs = &auDrvCfg_usb_speaker,
		.txCfgs = &auDrvCfg_usb_mic,
		.state = &auDrvSt_usb,
		.ops = &auDrvOps_usb,
		/*.userData = NULL;*/
	},
#endif
};

#endif

