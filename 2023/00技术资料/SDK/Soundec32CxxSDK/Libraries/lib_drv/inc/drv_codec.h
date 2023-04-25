/** 
***********************************************************
 * 
 * @file name	: drv_codec.h
 * @author		: zhangkun
 * @version		: V00.20
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
#ifndef __DRV_CODEC_H__
#define __DRV_CODEC_H__

/* Includes ------------------------------------------------------------------*/
#include "hal_codec.h"
#include "hal_codec_fifo.h"
#include "audio_hw_if.h"


#ifdef __cplusplus
extern "C" {
#endif

#define CODEC_ADC_NUM					10

#define CODEC_USE_ONE_IRQ				0

#define CODEC_ADC_ONE_CB				0

#if CODEC_USE_ONE_IRQ
#define CODEC_IRQ						DAC_IRQn
#define CODEC_INT_CHANNEL				HAL_CODEC_FIFO_DAC
#define CODEC_INT_SRC					HAL_CODEC_INT_AE
#else
#define CODEC_DAC_IRQ					DAC_IRQn
#define CODEC_DAC_INT_CHANNEL			HAL_CODEC_FIFO_DAC
#define CODEC_DAC_INT_SRC				HAL_CODEC_INT_AE

#define CODEC_ADC_IRQ					ADC12_IRQn
#define CODEC_ADC_INT_CHANNEL			HAL_CODEC_FIFO_ADC12
#define CODEC_ADC_INT_SRC				HAL_CODEC_INT_AF

#define CODEC_DAC_SC_IRQ				CODEC_IRQn
#endif

#define ADC_TUNE_STEP 					0.0004166667
#define DAC_TUNE_STEP 					0.004166667

#define TUNE_UP							1
#define TUNE_DOWN						-1
#define NO_TUNE							0	

#define CODEC_ADC_DATA_MODE_16BIT			0
#define CODEC_ADC_DATA_MODE_24BIT 			1
	
#define CODEC_SLAVE_FRAC_SRC_DOL			0		/* Use Dolphin's Fractional Divider */
#define CODEC_SLAVE_FRAC_SRC_SNC			1		/* Use Soundec's Fractional Divider */
#define CODEC_SLAVE_FRAC_SHARE_DEDICATED	0		/* All ADCs and DAC use the dedicated Fractional Divider */
#define CODEC_SLAVE_FRAC_SHARE_ONE			1		/* All ADCs and DAC share the Fractional Divider with ADC3456 */

#if (CODEC_MASTER)
#define CODEC_DEFAULT_MODE	HAL_CODEC_AUDIO_INTERFACE_MASTER
#else
#define CODEC_DEFAULT_MODE	HAL_CODEC_AUDIO_INTERFACE_SLAVE
#endif

typedef hal_codec_fs_t codec_fs_t;

typedef void (*codec_isr_cb_func)(int32_t *l, int32_t *r);

/** 
 * @brief This enum defines the operating mode of the Codec
 */
typedef enum {
	CODEC_OP_MODE_PLAYBACK_OPEN = 0,		/* Playback Open mode */
	CODEC_OP_MODE_VOICE_OPEN,				/* Voice Open mode */
	CODEC_OP_MODE_VOICE_CLOSE,				/* Voice Close mode */
	CODEC_OP_MODE_SHUTDOWN,			        /* Shutdown mode, turn off playback and record both  */
	CODEC_OP_MODE_WT,
	CODEC_OP_MODE_NUM,
} codec_op_mode_t;

/** 
 * @brief This enum defines the ISR callback number of the Codec
 */
typedef enum {
	CODEC_CALLBACK_DAC = 0,
	CODEC_CALLBACK_ADC,
#if(AUDIO_MANAGER)
	CODEC_CALLBACK_ADC12,
	CODEC_CALLBACK_ADC34,
	CODEC_CALLBACK_ADC56,
	CODEC_CALLBACK_ADC78,
	CODEC_CALLBACK_ADC9A,
#endif
	CODEC_CALLBACK_AIAS_SC,
	CODEC_CALLBACK_WT,
	CODEC_CALLBACK_NUM,
} codec_isr_cb_t;

/* 
* @brief This enum defines the audio interface of the Codec 
*/
typedef enum {
	CODEC_MODE_MASTER = 0,
	CODEC_MODE_SLAVE,
} codec_mode_t;

/** 
 * @brief This enum defines the analog microphone gain value
 */
typedef enum {
	CODEC_GIM_M2DB = 0,		/* -2 dB */
	CODEC_GIM_0DB,			/*  0 dB, default */
	CODEC_GIM_2DB,			/*  2 dB */
	CODEC_GIM_4DB,			/*  4 dB */
	CODEC_GIM_6DB,			/*  6 dB */
	CODEC_GIM_8DB,			/*  8 dB */
	CODEC_GIM_10DB,			/* 10 dB */
	CODEC_GIM_12DB,			/* 12 dB */
	CODEC_GIM_14DB,			/* 14 dB */
	CODEC_GIM_16DB,			/* 16 dB */
	CODEC_GIM_18DB,			/* 18 dB */
	CODEC_GIM_20DB,			/* 20 dB */
	CODEC_GIM_22DB,			/* 22 dB */
	CODEC_GIM_24DB,			/* 24 dB */
	CODEC_GIM_NUM
} codec_gim_t;


/** 
 * @brief This struct defines the configuration information of DAC
 */
typedef struct {
	hal_codec_ai_mode_t master_slave;       /* DAC audio interface is master or slave */
	uint32_t sample_rate;			    	/* DAC sample rate */
	bool is_hp;								/* Indicate whether it's headphone or line-out. true:Headphone; false:line-out. */
	bool drc_enable;						/* Enable or disable DRC */
	bool left_only;				            /* Deactivation of analog right channel of DAC and headphone output amplifier */
	hal_codec_channel_sel_t aidacl_sel;     /* DAC left channel input selection. 00: Normal inputs 01: Cross inputs 10: Mixed inputs 11: 0 inputs */
	hal_codec_channel_sel_t aidacr_sel;     /* DAC right channel input selection. 00: Normal inputs 01: Cross inputs 10: Mixed inputs 11: 0 inputs */
	hal_codec_dac_mixer_mode_t dac_mix;     /* Only Playback or Playbak+ADC */
	hal_codec_channel_sel_t mixdacl_sel;    /* Mixer left channel input selection on DAC path */
	hal_codec_channel_sel_t mixdacr_sel;	/* Mixer right channel input selection on DAC path */
	hal_codec_channel_sel_t mixadc1_sel;	/* Mixer channel 1 input selection on ADC path */
	hal_codec_channel_sel_t mixadc2_sel;	/* Mixer channel 2 input selection on ADC path */
	int8_t 	godl;							/* Left channel DAC digital gain, -31dB ~ 32dB by step of 1 dB */
	int8_t 	godr;							/* Right channel DAC digital gain, -31dB ~ 32dB by step of 1 dB */
	int8_t 	gomixl;							/* Mixer gain for DAC left path, 0dB ~ -31dB by step of -1 dB */
	int8_t 	gomixr;							/* Mixer gain for DAC right path, 0dB ~ -31dB by step of -1 dB */
	int8_t 	gimixl;							/* Mixer gain for left input path, 0dB ~ -31dB by step of -1 dB */
	int8_t 	gimixr;							/* Mixer gain for right input path, 0dB ~ -31dB by step of -1 dB */
	int8_t 	lthres;							/* DRC compression threshold for left channel DAC, [full scale] ~ [full scale - 31dB] by step of -1 dB */
	int8_t 	rthres;							/* DRC compression threshold for right channel DAC, [full scale] ~ [full scale - 31dB] by step of -1 dB */
	hal_codec_dac_drc_comp_t lcomp;			/* DRC compression rate for left channel DAC, 1, 2, 4 ~ 32 by step of x2 */
	hal_codec_dac_drc_comp_t rcomp;			/* DRC compression rate for right channel DAC, 1, 2, 4 ~ 32 by step of x2 */

	uint8_t hp_sel;					/* Headphone Output Amplifier input selection */
	int8_t  gol;					/* Left channel HP amplifier gain */
	int8_t  gor;					/* Right channel HP amplifier gain */
} codec_dac_config_t;

/** 
 * @brief This struct defines the configuration information of analog microphone
 */
typedef struct {
	hal_codec_amic_input_mode_t micdiff1;			/* Analog microphone input mode */
	hal_codec_amic_bias_output_state_t	micbias1_v;	/* MICBIAS output state when MICBIAS in power-down mode */
	int8_t	gim1;									/* Analog microphone gain */	
	int8_t 	gid1;									/* ADC1 digital gain, range: -64 ~ 63 dB, step: 1dB */
	bool capcouple1;								/* Capacitance couple. True: Have capacitance; False: Capacitance-less */

	hal_codec_amic_input_mode_t micdiff2;			/* Analog microphone input mode */
	hal_codec_amic_bias_output_state_t	micbias2_v;	/* MICBIAS output state when MICBIAS in power-down mode */
	int8_t	gim2;									/* Analog microphone gain */
	int8_t 	gid2;									/* ADC2 digital gain, range: -64 ~ 63 dB, step: 1dB */
	bool capcouple2;								/* Capacitance couple. True: Have capacitance; False: Capacitance-less */
} codec_amic12_config_t;

/** 
 * @brief This struct defines the configuration information of digital microphone
 */
typedef struct {
	hal_codec_dmic_clock_t	clock;					/* DMIC clock */

	int8_t 	gid1;									/* DMIC1 digital gain, range: -64 ~ 63 dB, step: 1dB */
	int8_t 	gid2;									/* DMIC2 digital gain, range: -64 ~ 63 dB, step: 1dB */
	
	hal_codec_adc_data_provided_t adc_dmic_sel1;	/* Select audio data provided to left channel DMIC12 digital filters (channel 1), DMIC1 or (DMIC1+DMIC2)/2 */
	hal_codec_adc_data_provided_t adc_dmic_sel2;	/* Select audio data provided to right channel DMIC12 digital filters (channel 2), DMIC1 or (DMIC1+DMIC2)/2 */
} codec_dmic12_config_t;

/** 
 * @brief This struct defines the configuration information of ADC12
 */
typedef struct {
	hal_codec_ai_mode_t master_slave;			/* Master or slave mode */
	uint32_t sample_rate;						/* ADC12 sample rate */
	
	hal_codec_channel_sel_t aiadc1_sel;			/* Mixer channel 1 output selection on ADC path, 0:ADC1, 1:ADC2, 2:(ADC1+ADC2)/2, 3:None */
	hal_codec_channel_sel_t aiadc2_sel;			/* Mixer channel 2 output selection on ADC path, 0:ADC2, 1:ADC1, 2:(ADC1+ADC2)/2, 3:None */
	hal_codec_adc_mixer_mode_t mix_rec;			/* ADC12 mixed with DAC or not */	
	hal_codec_adc12_power_mode_t adc12_mode;	/* ADC1 and ADC2 power mode, 0: Normal mode, 1: Low power mode */
	hal_codec_wnf_mode_t wnf;					/* ADC12 wind noise filter */
	
	bool	agc_enable;							/* ADC12 AGC Enable or not */
	bool	hpf_enable;							/* ADC12 HPF Enable or not */
	bool 	is_amic;							/* Indicates whether it's analog microphone or not; true:analog microphone; false:digital microphone. */

	/* ADC12 can be as either analog microphone or digital microphone */
	codec_amic12_config_t amic;					/* Analog microphone specified config */
	codec_dmic12_config_t dmic;					/* Digital microphone specified config */

	hal_codec_adc_agc_config_t *agc_config;		/* ADC12 AGC configuration information */
} codec_adc12_config_t;

/** 
 * @brief This struct defines the common configuration information of ADC34 and ADC56
 */
typedef struct {
	hal_codec_ai_mode_t     master_slave;			/* Master or slave mode */
	uint32_t          		sample_rate;			/* ADC3456 sample rate */
	hal_codec_dmic_clock_t  dmic_clock;				/* DMIC3456 clock(ratio) */
} codec_adc3456_config_t;

/** 
 * @brief This struct defines the configuration information of ADC34
 */
typedef struct {
	hal_codec_dmic_data_provided_t adc_dmic_sel3;	/* Select audio data provided to left channel ADC34 digital filters (channel 3), DMIC3 or (DMIC3+DMIC4)/2 */
	hal_codec_dmic_data_provided_t adc_dmic_sel4;	/* Select audio data provided to right channel ADC34 digital filters (channel 4), DMIC4 or (DMIC3+DMIC4)/2 */
	hal_codec_channel_sel_t aiadc3_sel;				/* Mixer channel 3 output selection on ADC path, 0:DMIC3, 1:DMIC4, 2:(DMIC3+DMIC4)/2, 3:None */
	hal_codec_channel_sel_t aiadc4_sel;				/* Mixer channel 4 output selection on ADC path, 0:DMIC4, 1:DMIC3, 2:(DMIC3+DMIC4)/2, 3:None */
	hal_codec_adc_mixer_mode_t mix_rec;				/* ADC34 mixed with DAC or not */	
	hal_codec_wnf_mode_t wnf;						/* ADC34 wind noise filter */
	int8_t 	gid3;									/* ADC3 digital gain, range: -64 ~ 63 dB, step: 1dB */
	int8_t 	gid4;									/* ADC4 digital gain, range: -64 ~ 63 dB, step: 1dB */
	bool	agc_enable;								/* ADC34 AGC Enable or not */
	bool	hpf_enable;								/* ADC34 HPF Enable or not */
	
	hal_codec_adc_agc_config_t *agc_config;		/* ADC12 AGC configuration information */
} codec_adc34_config_t;

/** 
 * @brief This struct defines the configuration information of ADC56
 */
typedef struct {
	hal_codec_dmic_data_provided_t adc_dmic_sel5;	/* Select audio data provided to left channel ADC56 digital filters (channel 5), DMIC5 or (DMIC5+DMIC6)/2 */
	hal_codec_dmic_data_provided_t adc_dmic_sel6;	/* Select audio data provided to left channel ADC56 digital filters (channel 6), DMIC6 or (DMIC5+DMIC6)/2 */
	hal_codec_channel_sel_t aiadc5_sel;				/* Mixer channel 5 output selection on ADC path, 0:DMIC5, 1:DMIC6, 2:(DMIC5+DMIC6)/2, 3:None */
	hal_codec_channel_sel_t aiadc6_sel;				/* Mixer channel 6 output selection on ADC path, 0:DMIC6, 1:DMIC5, 2:(DMIC5+DMIC6)/2, 3:None */
	hal_codec_adc_mixer_mode_t mix_rec;				/* ADC56 mixed with DAC or not */	
	hal_codec_wnf_mode_t wnf;						/* ADC56 wind noise filter */
	int8_t 	gid5;									/* ADC5 digital gain, range: -64 ~ 63 dB, step: 1dB */
	int8_t 	gid6;									/* ADC6 digital gain, range: -64 ~ 63 dB, step: 1dB */
	bool	agc_enable;								/* ADC56 AGC Enable */
	bool	hpf_enable;								/* ADC56 HPF Enable or not */
	
	hal_codec_adc_agc_config_t *agc_config;			/* ADC56 AGC configuration information */
} codec_adc56_config_t;

/** 
 * @brief This struct defines the common configuration information of ADC78 and ADC9a
 */
typedef struct {
	hal_codec_ai_mode_t     master_slave;			/* Master or slave mode */
	uint32_t          		sample_rate;			/* ADC789a sample rate */
	hal_codec_dmic_clock_t  dmic_clock;				/* DMIC789a clock(ratio) */
} codec_adc789a_config_t;

/** 
 * @brief This struct defines the configuration information of ADC78
 */
typedef struct {
	hal_codec_dmic_data_provided_t adc_dmic_sel7;	/* Select audio data provided to left channel ADC78 digital filters (channel 7), DMIC7 or (DMIC7+DMIC8)/2 */
	hal_codec_dmic_data_provided_t adc_dmic_sel8;	/* Select audio data provided to left channel ADC78 digital filters (channel 8), DMIC8 or (DMIC7+DMIC8)/2 */
	hal_codec_channel_sel_t aiadc7_sel;				/* Mixer channel 7 output selection on ADC path, 0:DMIC7, 1:DMIC8, 2:(DMIC7+DMIC8)/2, 3:None */
	hal_codec_channel_sel_t aiadc8_sel;				/* Mixer channel 8 output selection on ADC path, 0:DMIC8, 1:DMIC7, 2:(DMIC7+DMIC8)/2, 3:None */
	hal_codec_wnf_mode_t wnf;						/* ADC78 wind noise filter */
	int8_t 	gid7;									/* ADC7 digital gain, range: -64 ~ 63 dB, step: 1dB */
	int8_t 	gid8;									/* ADC8 digital gain, range: -64 ~ 63 dB, step: 1dB */
	bool	agc_enable;								/* ADC78 AGC Enable or not */
	bool	hpf_enable;								/* ADC78 HPF Enable or not */
	
	hal_codec_adc_agc_config_t *agc_config;			/* ADC78 AGC configuration information */
} codec_adc78_config_t;

/** 
 * @brief This struct defines the configuration information of ADC9a
 */
typedef struct {
	hal_codec_dmic_data_provided_t adc_dmic_sel9;	/* Select audio data provided to left channel ADC9a digital filters (channel 9), DMIC9 or (DMIC9+DMIC10)/2 */
	hal_codec_dmic_data_provided_t adc_dmic_sel10;	/* Select audio data provided to left channel ADC9a digital filters (channel 6), DMIC6 or (DMIC9+DMIC10)/2 */
	hal_codec_channel_sel_t aiadc9_sel;				/* Mixer channel 9 output selection on ADC path, 0:DMIC9, 1:DMIC6, 2:(DMIC9+DMIC10)/2, 3:None */
	hal_codec_channel_sel_t aiadc10_sel;			/* Mixer channel 10 output selection on ADC path, 0:DMIC10, 1:DMIC5, 2:(DMIC9+DMIC10)/2, 3:None */
	hal_codec_wnf_mode_t wnf;						/* ADC9a wind noise filter */
	int8_t 	gid9;									/* ADC9 digital gain, range: -64 ~ 63 dB, step: 1dB */
	int8_t 	gid10;									/* ADC10 digital gain, range: -64 ~ 63 dB, step: 1dB */
	bool	agc_enable;								/* ADC9a AGC Enable */
	bool	hpf_enable;								/* ADC9a HPF Enable or not */
	
	hal_codec_adc_agc_config_t *agc_config;			/* ADC9a AGC configuration information */
} codec_adc9a_config_t;

/** 
 * @brief This function is used to initialize the Codec clk settings
 */
drv_status_t codec_clk_init(void);


/** 
 * @brief This function is used to configure the codec DAC audio interface
 * @param[in] config DAC configuration information
 * @return
 * 	@retval 0 DAC is successfully configured.
 */
drv_status_t codec_config_dac(codec_dac_config_t *config);

drv_status_t codec_set_dac_mute(bool enable);
drv_status_t codec_set_adc12_mute(bool enable);
drv_status_t codec_set_adc34_mute(bool enable);
drv_status_t codec_set_adc56_mute(bool enable);
drv_status_t codec_set_adc78_mute(bool enable);
drv_status_t codec_set_adc9a_mute(bool enable);

/** 
 * @brief This function is used to configure the Codec ADC12 audio interface
 * @param[in] config The configuration information of ADC
 * @return	
 * 	@retval 0 ADC12 is successfully configured.
 */
drv_status_t codec_config_adc12(codec_adc12_config_t *config);

/** 
 * @brief This function is used to configure the commom part of the Codec ADC3456 audio interface
 * @param[in] config The configuration information of ADC3456
 * @return	
 * 	@retval 0 The common part of ADC3456 is successfully configured.
 */
drv_status_t codec_config_adc3456(codec_adc3456_config_t *config);

/** 
 * @brief This function is used to configure the Codec ADC34 audio interface
 * @param[in] config The configuration information of ADC34
 * @return	
 * 	@retval 0 ADC34 is successfully configured.
 */
drv_status_t codec_config_adc34(codec_adc34_config_t *config);

/** 
 * @brief This function is used to configure the Codec ADC56 audio interface
 * @param[in] config The configuration information of ADC56
 * @return	
 * 	@retval 0 ADC56 is successfully configured.
 */
drv_status_t codec_config_adc56(codec_adc56_config_t *config);

/** 
 * @brief This function is used to configure the commom part of the Codec ADC789a audio interface
 * @param[in] config The configuration information of ADC789a
 * @return	
 * 	@retval 0 The common part of ADC789a is successfully configured.
 */
drv_status_t codec_config_adc789a(codec_adc789a_config_t *config);

/** 
 * @brief This function is used to configure the Codec ADC78 audio interface
 * @param[in] config The configuration information of ADC78
 * @return	
 * 	@retval 0 ADC78 is successfully configured.
 */
drv_status_t codec_config_adc78(codec_adc78_config_t *config);

/** 
 * @brief This function is used to configure the Codec ADC9a audio interface
 * @param[in] config The configuration information of ADC9a
 * @return	
 * 	@retval 0 ADC9a is successfully configured.
 */
drv_status_t codec_config_adc9a(codec_adc9a_config_t *config);

/** 
 * @brief This function is used to set the operating mode of the Codec
 * @param[in] mode The operation mode is set. This parameter can only be one of the type #codec_op_mode_t.
 * @return	
 * 	@retval 0 The operating mode is successfully set.
 */
drv_status_t codec_set_operation_mode(codec_op_mode_t mode);

/** 
 * @brief This function is used to start the DAC audio interface
 * @return	
 * 	@retval 0 DAC is successfully started.
 */
drv_status_t codec_dac_start(void);

/** 
 * @brief This function is used to stop the DAC audio interface
 * @return	
 * 	@retval 0 DAC is successfully stopped.
 */
drv_status_t codec_dac_stop(void);

/** 
 * @brief This function is used to start the DAC audio interface
 * @return	
 * 	@retval 0 ADC12 is successfully started.
 */
drv_status_t codec_adc12_start(void);

/** 
 * @brief This function is used to stop the ADC12 audio interface
 * @return	
 * 	@retval 0 ADC12 is successfully stopped.
 */
drv_status_t codec_adc12_stop(void);

/** 
 * @brief This function is used to start the ADC34 audio interface
 * @return	
 * 	@retval 0 ADC34 is successfully started.
 */
drv_status_t codec_adc34_start(void);

/** 
 * @brief This function is used to stop the ADC34 audio interface
 * @return	
 * 	@retval 0 ADC34 is successfully stopped.
 */
drv_status_t codec_adc34_stop(void);

/** 
 * @brief This function is used to start the ADC56 audio interface
 * @return	
 * 	@retval 0 ADC56 is successfully started.
 */
drv_status_t codec_adc56_start(void);

/** 
 * @brief This function is used to stop the ADC56 audio interface
 * @return	
 * 	@retval 0 ADC56 is successfully stopped.
 */
drv_status_t codec_adc56_stop(void);

/** 
 * @brief This function is used to start the ADC78 audio interface
 * @return	
 * 	@retval 0 ADC78 is successfully started.
 */
drv_status_t codec_adc78_start(void);

/** 
 * @brief This function is used to stop the ADC78 audio interface
 * @return	
 * 	@retval 0 ADC78 is successfully stopped.
 */
drv_status_t codec_adc78_stop(void);

/** 
 * @brief This function is used to start the ADC9a audio interface
 * @return	
 * 	@retval 0 ADC9a is successfully started.
 */
drv_status_t codec_adc9a_start(void);

/** 
 * @brief This function is used to stop the ADC9a audio interface
 * @return	
 * 	@retval 0 ADC9a is successfully stopped.
 */
drv_status_t codec_adc9a_stop(void);

#if (CODEC_MIC_AGC_ENABLE)
/** 
 * @brief This function is used to enable or disable the AGC function of Microphones
 * @param[in] enable true:Enable AGC; false:Disable AGC
 */
void codec_mic_agc_enable(bool enable);
#endif

void codec_dac_left_input_enable(void);

void codec_dac_right_input_enable(void);

void codec_adc1_output_enable(void);

void codec_adc2_output_enable(void);

void codec_adc3_output_enable(void);

void codec_adc4_output_enable(void);

void codec_adc5_output_enable(void);

void codec_adc6_output_enable(void);

void codec_adc7_output_enable(void);

void codec_adc8_output_enable(void);

void codec_adc9_output_enable(void);

void codec_adc10_output_enable(void);

void codec_dac_left_input_disable(void);

void codec_dac_right_input_disable(void);

void codec_adc1_output_disable(void);

void codec_adc2_output_disable(void);

void codec_adc3_output_disable(void);

void codec_adc4_output_disable(void);

void codec_adc5_output_disable(void);

void codec_adc6_output_disable(void);

void codec_adc7_output_disable(void);

void codec_adc8_output_disable(void);

void codec_adc9_output_disable(void);

void codec_adc10_output_disable(void);

/** 
 * @brief This function is used to initialize and enable the Whisper Trigger
 */
void codec_wt_init(void);

/** 
 * @brief This function is used to disable the Whisper Trigger
 */
void codec_wt_deinit(void);

/** 
 * @brief This function is used to enable the AIAS Unlock interrupt
 */
void codec_enable_aias_unlock_irq(void);

/** 
 * @brief This function is used to adjust the codec DAC sample rate
 * @param[in] s The proportion the sample rate want to be tuned, must not exceed +/-3%
 */
//void codec_tune_dac_sample_rate(int32_t dir);
void codec_tune_dac_sample_rate(uint32_t dataLen, uint32_t fifoSize);//(float s, int32_t dir);

/** 
 * @brief This function is used to set DAC sampling rate
 * @param[in] fs The sampling rate is set.
 */
void codec_set_dac_samplerate(uint32_t fs);

/** 
 * @brief This function is used to set ADC12 sampling rate
 * @param[in] fs The sampling rate is set.
 */
void codec_set_adc12_samplerate(uint32_t fs);

/** 
 * @brief This function is used to set ADC3456 sampling rate
 * @param[in] fs The sampling rate is set.
 */
void codec_set_adc3456_samplerate(uint32_t fs);

/** 
 * @brief This function is used to set ADC789a sampling rate
 * @param[in] fs The sampling rate is set.
 */
void codec_set_adc789a_samplerate(uint32_t fs);

/** 
 * @brief This function is used to get DAC sampling rate.
 * @return	The returned DAC sampling rate.
 */
uint32_t codec_get_dac_samplerate(void);

/** 
 * @brief This function is used to get ADC12 sampling rate.
 * @return	The returned ADC12 sampling rate.
 */
uint32_t codec_get_adc12_samplerate(void);

/** 
 * @brief This function is used to get ADC3456 sampling rate.
 * @return	The returned ADC3456 sampling rate.
 */
uint32_t codec_get_adc3456_samplerate(void);

/** 
 * @brief This function is used to get ADC789a sampling rate.
 * @return	The returned ADC789a sampling rate.
 */
uint32_t codec_get_adc789a_samplerate(void);

/** 
 * @brief This function is used to set the headphone left analog gain.
 * @param[in] gain The analog gain is set. The range is [-19, 12] by step of 1 dB.
 */
void codec_set_hp_left_analog_gain(int8_t gain);

/** 
 * @brief This function is used to set the headphone right analog gain.
 * @param[in] gain The analog gain is set. The range is [-19, 12] by step of 1 dB.
 */
void codec_set_hp_right_analog_gain(int8_t gain);

/** 
 * @brief This function is used to set DAC left digital gain.
 * @param[in] gain The digital gain is set. The range is [-31, 32] by step of 1 dB.
 */
void codec_set_dac_left_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set DAC right digital gain.
 * @param[in] gain The digital gain is set. The range is [-31, 32] by step of 1 dB.
 */
void codec_set_dac_right_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set ADC1 analog gain.
 * @param[in] gain The analog gain is set. This parameter can only be one of the type #codec_gim_t
 */
void codec_set_adc1_analog_gain(codec_gim_t gain);

/** 
 * @brief This function is used to set ADC2 analog gain.
 * @param[in] gain The analog gain is set. This parameter can only be one of the type #codec_gim_t
 */
void codec_set_adc2_analog_gain(codec_gim_t gain);

/** 
 * @brief This function is used to set ADC1 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc1_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set ADC2 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc2_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set ADC3 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc3_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set ADC4 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc4_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set ADC5 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc5_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set ADC6 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc6_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set ADC7 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc7_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set ADC8 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc8_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set ADC9 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc9_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set ADC10 digital gain.
 * @param[in] gain The digital gain is set. The range is [-64, 63] by step of 1dB
 */
void codec_set_adc10_digital_gain(int8_t gain);

/** 
 * @brief This function is used to adjust the codec DAC sample rate
 * @param[in] s The proportion the sample rate want to be tuned, must not exceed +/-3%
 */
void codec_tune_dac_sample_rate(uint32_t dataLen, uint32_t fifoSize);

/** 
 * @brief This function is used to adjust the codec ADC12 sample rate
 * @param[in] s The proportion the sample rate want to be tuned, must not exceed +/-3%
 */
void codec_tune_adc12_sample_rate(uint32_t dataLen, uint32_t fifoSize);

/** 
 * @brief This function is used to adjust the codec ADC3456 sample rate
 * @param[in] s The proportion the sample rate want to be tuned, must not exceed +/-3%
 */
void codec_tune_adc3456_sample_rate(uint32_t dataLen, uint32_t fifoSize);

/** 
 * @brief This function is used to set the data mode (16 or 24 bits)
 */
void codec_set_adc_data_mode(void);

#if(AUDIO_MANAGER == 0)
/**
 * @brief This function is used to initialize Codec DAC audio interface
 * @param[in] fs The sampling rate is set. This parameter can only be one of the array #codec_fs_table
 * @return
 * 	@retval 0 Codec DAC is successfully initialized.
 */
drv_status_t codec_dac_init(void);
/** 
 * @brief This function is used to initialize Codec ADC audio interface
 * @param[in] fs The sampling rate is set. This parameter can only be one of the array #codec_fs_table
 * @return
 * 	@retval 0 Codec DAC is successfully initialized.
 */

drv_status_t codec_adc_init(void);

drv_status_t codec_register_cb(codec_isr_cb_t index, codec_isr_cb_func cb);
#else
//#if (CODEC_ADC_ENABLE == 0)
drv_status_t codec_adc12_init(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_adc34_init(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_adc56_init(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_adc78_init(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_adc9a_init(auDriverConfig_t *capture, auDriverConfig_t *playback);

drv_status_t codec_adc12_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_adc34_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_adc56_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_adc78_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_adc9a_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback);

drv_status_t codec_adc12_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_adc34_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_adc56_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_adc78_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_adc9a_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback);

drv_status_t codec_dac_init(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_dac_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_dac_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
auMute_t codec_dac_get_mute(void);

auSlotSize_t codec_reset_adc_bitslot(auSlotSize_t bitslot);
auSlotSize_t codec_reset_dac_bitslot(auSlotSize_t bitslot);
drv_status_t codec_set_adc12_low_power_mode(void);
drv_status_t codec_set_adc12_wake_up_mode(void);
auMute_t codec_adc12_get_mute(void);
auMute_t codec_adc34_get_mute(void);
auMute_t codec_adc56_get_mute(void);
auMute_t codec_adc78_get_mute(void);
auMute_t codec_adc9a_get_mute(void);

int8_t codec_get_dac_gain(uint8_t isAgain, auChannel_t lr);
int8_t codec_get_adc12_gain(uint8_t isAgain, auChannel_t lr);
int8_t codec_get_adc34_gain(uint8_t isAgain, auChannel_t lr);
int8_t codec_get_adc56_gain(uint8_t isAgain, auChannel_t lr);
int8_t codec_get_adc78_gain(uint8_t isAgain, auChannel_t lr);
int8_t codec_get_adc9a_gain(uint8_t isAgain, auChannel_t lr);

drv_status_t codec_set_adc78_gain(int8_t gain, uint8_t isAgain, auChannel_t lr);
drv_status_t codec_set_adc9a_gain(int8_t gain, uint8_t isAgain, auChannel_t lr);

#if (CODEC_ADC_ENABLE)
drv_status_t adc12_init(void);
drv_status_t adc34_init(void);
drv_status_t adc56_init(void);
drv_status_t adc78_init(void);
drv_status_t adc9a_init(void);
drv_status_t adc3456_common_init(void);
drv_status_t adc789a_common_init(void);

drv_status_t codec_adc_init(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_adc_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_adc_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t codec_adc_start(void);
drv_status_t codec_adc_stop(void);

drv_status_t codec_set_adc_gain(int8_t gain, uint8_t isAgain, auChannel_t lr);
drv_status_t codec_set_adc_mute(bool enable);
void codec_set_adc_samplerate(uint32_t fs);
int8_t codec_get_adc_gain(uint8_t isAgain, auChannel_t lr);
auMute_t codec_adc_get_mute(void);
uint32_t codec_get_adc_samplerate(void);
#endif	
bool hal_codec_get_sc_irq_flag(void);
void hal_codec_unmask_sc_irq(void);
drv_status_t codec_dac_restart(void);
void hal_codec_clear_sc_irq_flag(void);

/**
 * @brief This function is used to register the callback function of the ISRs. The callback function can handle the Codec datas.
 * @param[in] index The location the callback function is registered. This parameter can only be one of the type #codec_isr_cb_t
 * @param[in] cb The callback function is registered.
 * @return
 * 	@retval 0 The callback function is successfully registered
 */

drv_status_t codec_register_cb(codec_isr_cb_t index, auTransmitCompleteCB_t cb);
#endif

#if(AUDIO_MANAGER == 0)
/** 
 * @brief This function is used to initialize Codec ADC12 audio interface
 * @param[in] fs The sampling rate is set. This parameter can only be one of the array #codec_fs_table
 * @return
 * 	@retval 0 Codec ADC12 is successfully initialized.
 */
drv_status_t codec_adc12_init(void);

/** 
 * @brief This function is used to initialize Codec ADC3456 common setting.
 * @param[in] fs The sampling rate is set. This parameter can only be one of the array #codec_fs_table[]
 * @return
 * 	@retval 0 Codec ADC3456 common setting is successfully initialized.
 */
drv_status_t codec_adc3456_init(void);

/** 
 * @brief This function is used to initialize Codec ADC34 audio interface
 * @note This function should be called after calling #codec_adc3456_init()
 * @return
 * 	@retval 0 Codec ADC34 is successfully initialized.
 */
drv_status_t codec_adc34_init(void);

/** 
 * @brief This function is used to initialize Codec ADC56 audio interface
 * @note This function should be called after calling #codec_adc3456_init()
 * @return
 * 	@retval 0 Codec ADC56 is successfully initialized.
 */
drv_status_t codec_adc56_init(void);

/** 
 * @brief This function is used to initialize Codec ADC789a common setting.
 * @param[in] fs The sampling rate is set. This parameter can only be one of the array #codec_fs_table[]
 * @return
 * 	@retval 0 Codec ADC789a common setting is successfully initialized.
 */
drv_status_t codec_adc789a_init(void);

/** 
 * @brief This function is used to initialize Codec ADC78 audio interface
 * @note This function should be called after calling #codec_adc789a_init()
 * @return
 * 	@retval 0 Codec ADC78 is successfully initialized.
 */
drv_status_t codec_adc78_init(void);

/** 
 * @brief This function is used to initialize Codec ADC9a audio interface
 * @note This function should be called after calling #codec_adc789a_init()
 * @return
 * 	@retval 0 Codec ADC9a is successfully initialized.
 */
drv_status_t codec_adc9a_init(void);
#endif

drv_status_t codec_set_adc12_mute(bool enable);

#if CODEC_USE_ONE_IRQ
/** 
 * @brief This function is the ISR of Codec
 */
void codec_isr_handler(void *arg);

#else
void codec_dac_isr_handler(void *arg);
void codec_adc_isr_handler(void *arg);
#endif

/** 
 * @brief This function is the ISR of AIAS-Unlock and Short-Circuit Detection
 */
void codec_aias_sc_irq_handler(void *arg);

#ifdef CONFIG_CODEC_ENABLE_WT
/** 
 * @brief This function is the ISR of Whisper Trigger Detection
 */
void codec_wt_irq_handler(void *arg);
#endif

drv_status_t codec_enable_adc12_hpf(uint8_t enable);
drv_status_t codec_enable_adc34_hpf(uint8_t enable);
drv_status_t codec_enable_adc56_hpf(uint8_t enable);
drv_status_t codec_select_adc12_mixer_mode(uint8_t enable);
drv_status_t codec_select_adc34_mixer_mode(uint8_t enable);
drv_status_t codec_select_adc56_mixer_mode(uint8_t enable);
drv_status_t codec_set_adc_word_length(uint8_t wordLength);
drv_status_t codec_set_adc1_aiadc(uint8_t sel);
drv_status_t codec_set_adc2_aiadc(uint8_t sel);
drv_status_t codec_set_adc3_aiadc(uint8_t sel);
drv_status_t codec_set_adc4_aiadc(uint8_t sel);
drv_status_t codec_set_adc5_aiadc(uint8_t sel);
drv_status_t codec_set_adc6_aiadc(uint8_t sel);
drv_status_t codec_set_adc12_wnf_mode(uint8_t mode);
drv_status_t codec_set_adc34_wnf_mode(uint8_t mode);
drv_status_t codec_set_adc56_wnf_mode(uint8_t mode);
drv_status_t codec_adc12_agc_enable(uint8_t enable);
drv_status_t codec_adc34_agc_enable(uint8_t enable);
drv_status_t codec_adc56_agc_enable(uint8_t enable);
drv_status_t codec_set_adc12_agc_stereo(uint8_t enable);
drv_status_t codec_set_adc34_agc_stereo(uint8_t enable);
drv_status_t codec_set_adc56_agc_stereo(uint8_t enable);
drv_status_t codec_set_adc12_agc_target(uint8_t target);
drv_status_t codec_set_adc34_agc_target(uint8_t target);
drv_status_t codec_set_adc56_agc_target(uint8_t target);
drv_status_t codec_adc12_agc_noise_gate_enable(uint8_t enable);
drv_status_t codec_adc34_agc_noise_gate_enable(uint8_t enable);
drv_status_t codec_adc56_agc_noise_gate_enable(uint8_t enable);
drv_status_t codec_set_adc12_agc_noise_gate_thres(uint8_t threshold);
drv_status_t codec_set_adc34_agc_noise_gate_thres(uint8_t threshold);
drv_status_t codec_set_adc56_agc_noise_gate_thres(uint8_t threshold);
drv_status_t codec_adc12_agc_snr_optimzation(uint8_t enable);
drv_status_t codec_set_adc12_agc_hold_time(uint8_t holdTime);
drv_status_t codec_set_adc34_agc_hold_time(uint8_t holdTime);
drv_status_t codec_set_adc56_agc_hold_time(uint8_t holdTime);
drv_status_t codec_set_adc12_agc_attack_time(uint8_t attackTime);
drv_status_t codec_set_adc34_agc_attack_time(uint8_t attackTime);
drv_status_t codec_set_adc56_agc_attack_time(uint8_t attackTime);
drv_status_t codec_set_adc12_agc_decay_time(uint8_t decayTime);
drv_status_t codec_set_adc34_agc_decay_time(uint8_t decayTime);
drv_status_t codec_set_adc56_agc_decay_time(uint8_t decayTime);
drv_status_t codec_set_adc12_agc_min_gain(uint8_t gain);
drv_status_t codec_set_adc34_agc_min_gain(uint8_t gain);
drv_status_t codec_set_adc56_agc_min_gain(uint8_t gain);
drv_status_t codec_set_adc12_agc_max_gain(uint8_t gain);
drv_status_t codec_set_adc34_agc_max_gain(uint8_t gain);
drv_status_t codec_set_adc56_agc_max_gain(uint8_t gain);
drv_status_t codec_dac_drc_enbale(uint8_t enable);
drv_status_t codec_set_dac_drc_lthres(uint8_t threshold);
drv_status_t codec_set_dac_drc_rthres(uint8_t threshold);
drv_status_t codec_set_dac_drc_lcomprate(uint8_t comprate);
drv_status_t codec_set_dac_drc_rcomprate(uint8_t comprate);
drv_status_t codec_dac_enable(uint8_t enable);
drv_status_t codec_dac_mix_enable(uint8_t enable);
drv_status_t codec_set_dac_golmix_gain(uint8_t gain);
drv_status_t codec_set_dac_gormix_gain(uint8_t gain);
drv_status_t codec_set_dac_gilmix_gain(uint8_t gain);
drv_status_t codec_set_dac_girmix_gain(uint8_t gain);
drv_status_t codec_set_dac_mixdacl(uint8_t sel);
drv_status_t codec_set_dac_mixdacr(uint8_t sel);
drv_status_t codec_set_dac_aidacl(uint8_t sel);
drv_status_t codec_set_dac_aidacr(uint8_t sel);
drv_status_t codec_select_dac_mixadc1_input(uint8_t sel);
drv_status_t codec_select_dac_mixadc2_input(uint8_t sel);
drv_status_t codec_adc12_enable(uint8_t enable, auChannel_t lr);
drv_status_t codec_adc34_enable(uint8_t enable, auChannel_t lr);
drv_status_t codec_adc56_enable(uint8_t enable, auChannel_t lr);

drv_status_t codec_set_adc12_gain(int8_t gain, uint8_t isAgain, auChannel_t lr);
drv_status_t codec_set_adc34_gain(int8_t gain, uint8_t isAgain, auChannel_t lr);
drv_status_t codec_set_adc56_gain(int8_t gain, uint8_t isAgain, auChannel_t lr);
drv_status_t codec_set_dac_gain(int8_t gain, uint8_t isAgain, auChannel_t lr);
void codec_adc12_aias(float s, int32_t dir);
void codec_adc3456_aias(float s, int32_t dir);
void codec_adc789a_aias(float s, int32_t dir);
void codec_tune_adc_sample_rate(uint32_t dataLen, uint32_t fifoSize);
void codec_tune_adc789a_sample_rate(uint32_t dataLen, uint32_t fifoSize);
drv_status_t codec_init(void);


#ifdef __cplusplus
}
#endif

#endif  /* __DRV_CODEC_H__ */


