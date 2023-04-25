/** 
***********************************************************
 * 
 * @file name	: hal_codec_reg.h
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-02-24
 * @brief		: Register descriptions for Dolphin Codec
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

#ifndef __REG_CODEC_H__
#define __REG_CODEC_H__

//#include "boardConfig.h"
#include "types.h"
#include "hal.h"

typedef uint32_t hal_codec_reg_type_t;

/** 
 * @brief This struct defiens the registers of the Dolphin Codec
 */
typedef struct {
	 __I uint32_t SR;				/* Offset: 0x00*4, Global Status Register 		*/
	 __I uint32_t SR_PUN;			/* Offset: 0x01*4, Status Register for anti PuN 	*/
	 __I uint32_t SR_AIAS;			/* Offset: 0x02*4, Status Register for AIAS 		*/
	 __I uint32_t SR_WT;			/* Offset: 0x03*4, Status Register for Whisper Trigger */
	__IO uint32_t ICR;				/* Offset: 0x04*4, Interrupt Control Register */
	__IO uint32_t IMR;				/* Offset: 0x05*4, Interrupt Mask Register */
	__IO uint32_t IFR;				/* Offset: 0x06*4, Interrupt Flag Register */
	__IO uint32_t CR_ViC;			/* Offset: 0x07*4, Control Register for the ViC */
	__IO uint32_t CR_CK;			/* Offset: 0x08*4, Clock Control Register */
	__IO uint32_t AICR_DAC;			/* Offset: 0x09*4, DAC Audio Interface Control Register */
	__IO uint32_t AICR_ADC12;		/* Offset: 0x0a*4, ADC12 Digital Filter Audio Interface Control Register */
	__IO uint32_t AICR_SB_ADC12;	/* Offset: 0x0b*4, ADC12 Digital Filter Audio Interface Control Register2 */
	__IO uint32_t AICR_ADC3456;		/* Offset: 0x0c*4, ADC3456 Digital Filter Audio Interface Control Register */
	__IO uint32_t AICR_SB_ADC3456;	/* Offset: 0x0d*4, ADC3456 Digital Filter Audio Interface Control Register2 */
	__IO uint32_t FCR_DAC;			/* Offset: 0x0e*4, DAC Frequency Control Register */
	__IO uint32_t SFCR_DAC_MSB;		/* Offset: 0x0f*4, DAC Sample Frequency Fine Control Register(MSB) */
	__IO uint32_t SFCR_DAC_LSB;		/* Offset: 0x10*4, DAC Sample Frequency Fine Control Register(LSB) */
	__IO uint32_t CR_ADC12_WNF;		/* Offset: 0x11*4, ADC12 Wind Noise Filter Control Register */
	__IO uint32_t CR_ADC3456_WNF;	/* Offset: 0x12*4, ADC3456 Wind Noise Filter Control Register */
	__IO uint32_t FCR_ADC12;		/* Offset: 0x13*4, ADC12 Frequency Control Register */
	__IO uint32_t SFCR_ADC12_MSB;	/* Offset: 0x14*4, ADC12 Sample Frequency Fine Control Register(MSB) */
	__IO uint32_t SFCR_ADC12_LSB;	/* Offset: 0x15*4, ADC12 Sample Frequency Fine Control Register(LSB) */
	__IO uint32_t FCR_ADC3456;		/* Offset: 0x16*4, ADC3456 Frequency Control Register */
	__IO uint32_t SFCR_ADC3456_MSB;	/* Offset: 0x17*4, ADC3456 Sample Frequency Fine Control Register(MSB) */
	__IO uint32_t SFCR_ADC3456_LSB;	/* Offset: 0x18*4, ADC3456 Sample Frequency Fine Control Register(LSB) */
	__IO uint32_t CR_DMIC12;		/* Offset: 0x19*4, Digital Microphone 1 and 2 Register */
	__IO uint32_t CR_DMIC34;		/* Offset: 0x1a*4, Digital Microphone 3 and 4 Register */
	__IO uint32_t CR_DMIC56;		/* Offset: 0x1b*4, Digital Microphone 5 and 6 Register */
	__IO uint32_t CR_LO;			/* Offset: 0x1c*4, Differential Line-Out Register */
	__IO uint32_t CR_HP;			/* Offset: 0x1d*4, Headphone Control Register */
	__IO uint32_t GCR_HPL;			/* Offset: 0x1e*4, Headphone Left Channel Gain Control Register */
	__IO uint32_t GCR_HPR;			/* Offset: 0x1f*4, Headphone Right Channel Gain Control Register */
	__IO uint32_t CR_MIC1;			/* Offset: 0x20*4, Microphone 1 Control Register */
	__IO uint32_t CR_MIC2;			/* Offset: 0x21*4, Microphone 2 Control Register */
	__IO uint32_t GCR_MIC12;		/* Offset: 0x22*4, Microphone 1 and 2 Gain Control Register */
	__IO uint32_t CR_DAC;			/* Offset: 0x23*4, DAC Control Register */
	__IO uint32_t CR_ADC12;			/* Offset: 0x24*4, ADC 1 and 2 Control Register */
	__IO uint32_t CR_ADC34;			/* Offset: 0x25*4, ADC 3 and 4 Control Register */
	__IO uint32_t CR_ADC56;			/* Offset: 0x26*4, ADC 5 and 6 Control Register */
	__IO uint32_t MIX_0;			/* Offset: 0x27*4, Digital Mixer Control Register 0 -- AIDACx_SEL */
	__IO uint32_t MIX_1;			/* Offset: 0x28*4, Digital Mixer Control Register 1 -- MIXDACx_SEL, MIXADCx_SEL */
	__IO uint32_t MIX_2;			/* Offset: 0x29*4, Digital Mixer Control Register 2 -- AIADC12_SEL, MIX_RECx */
	__IO uint32_t MIX_3;			/* Offset: 0x2a*4, Digital Mixer Control Register 3 -- AIADC34_SEL */
	__IO uint32_t MIX_4;			/* Offset: 0x2b*4, Digital Mixer Control Register 4 -- AIADC56_SEL */
	__IO uint32_t GCR_DACL;			/* Offset: 0x2c*4, DAC Left Channel Digital Gain Control Register */
	__IO uint32_t GCR_DACR;			/* Offset: 0x2d*4, DAC Right Channel Digital Gain Control Register */
	__IO uint32_t GCR_ADC1;			/* Offset: 0x2e*4, ADC1 Digital Gain Control Register */
	__IO uint32_t GCR_ADC2;			/* Offset: 0x2f*4, ADC2 Digital Gain Control Register */
	__IO uint32_t GCR_ADC3;			/* Offset: 0x30*4, ADC3 Digital Gain Control Register */
	__IO uint32_t GCR_ADC4;			/* Offset: 0x31*4, ADC4 Digital Gain Control Register */
	__IO uint32_t GCR_ADC5;			/* Offset: 0x32*4, ADC5 Digital Gain Control Register */
	__IO uint32_t GCR_ADC6;			/* Offset: 0x33*4, ADC6 Digital Gain Control Register */
	__IO uint32_t GCR_MIXDACL;		/* Offset: 0x34*4, DAC Left Channel Digital Mixer Gain Control Register */
	__IO uint32_t GCR_MIXDACR;		/* Offset: 0x35*4, DAC Right Channel Digital Mixer Gain Control Register */
	__IO uint32_t GCR_MIXADCL;		/* Offset: 0x36*4, ADC Left Channel Digital Mixer Gain Control Register */
	__IO uint32_t GCR_MIXADCR;		/* Offset: 0x37*4, ADC Right Channel Digital Mixer Gain Control Register */
	__IO uint32_t CR_DAC_AGC;		/* Offset: 0x38*4, DAC Soft Clipping DRC Control Register */
	__IO uint32_t DAC_AGC_0;		/* Offset: 0x39*4, DAC Left Channel DRC Threshold */
	__IO uint32_t DAC_AGC_1;		/* Offset: 0x3a*4, DAC Left Channel DRC Compression Rate */
	__IO uint32_t DAC_AGC_2;		/* Offset: 0x3b*4, DAC Right Channel DRC Threshold */
	__IO uint32_t DAC_AGC_3;		/* Offset: 0x3c*4, DAC Right Channel DRC Compression Rate */
	__IO uint32_t ADC12_AGC_0;		/* Offset: 0x3d*4, ADC12 AGC Control Register 0 */
	__IO uint32_t ADC12_AGC_1;		/* Offset: 0x3e*4, ADC12 AGC Control Register 1 */
	__IO uint32_t ADC12_AGC_2;		/* Offset: 0x3f*4, ADC12 AGC Control Register 2 */
	__IO uint32_t ADC12_AGC_3;		/* Offset: 0x40*4, ADC12 AGC Control Register 3 */
	__IO uint32_t ADC12_AGC_4;		/* Offset: 0x41*4, ADC12 AGC Control Register 4 */
	__IO uint32_t ADC34_AGC_0;		/* Offset: 0x42*4, ADC34 AGC Control Register 0 */
	__IO uint32_t ADC34_AGC_1;		/* Offset: 0x43*4, ADC34 AGC Control Register 1 */
	__IO uint32_t ADC34_AGC_2;		/* Offset: 0x44*4, ADC34 AGC Control Register 2 */
	__IO uint32_t ADC34_AGC_3;		/* Offset: 0x45*4, ADC34 AGC Control Register 3 */
	__IO uint32_t ADC34_AGC_4;		/* Offset: 0x46*4, ADC34 AGC Control Register 4 */
	__IO uint32_t ADC56_AGC_0;		/* Offset: 0x47*4, ADC56 AGC Control Register 0 */
	__IO uint32_t ADC56_AGC_1;		/* Offset: 0x48*4, ADC56 AGC Control Register 1 */
	__IO uint32_t ADC56_AGC_2;		/* Offset: 0x49*4, ADC56 AGC Control Register 2 */
	__IO uint32_t ADC56_AGC_3;		/* Offset: 0x4a*4, ADC56 AGC Control Register 3 */
	__IO uint32_t ADC56_AGC_4;		/* Offset: 0x4b*4, ADC56 AGC Control Register 4 */
	__IO uint32_t CR_WT;			/* Offset: 0x4c*4, Whisper Trigger Control Register 1 */
	__IO uint32_t CR_WT_2;			/* Offset: 0x4d*4, Whisper Trigger Control Register 2 */
	__IO uint32_t CR_WT_3;			/* Offset: 0x4e*4, Whisper Trigger Control Register 3 */
	__IO uint32_t CR_WT_4;			/* Offset: 0x4f*4, Whisper Trigger Control Register 4, Reserved */
	__IO uint32_t TR_DIG0;			/* Offset: 0x50*4, Test Mode Digital Control Register 0 */
	__IO uint32_t TR_DIG1;			/* Offset: 0x51*4, Test Mode Digital Control Register 1, Reserved */
	__IO uint32_t TR_DIG2;			/* Offset: 0x52*4, Test Mode Digital Control Register 2, Reserved */
	__IO uint32_t TR_DIG3;			/* Offset: 0x53*4, Test Mode Digital Control Register 3, Reserved */
	__IO uint32_t TR_DIG4;			/* Offset: 0x54*4, Test Mode Digital Control Register 4, Reserved */
	__IO uint32_t TR_DIG5;			/* Offset: 0x55*4, Test Mode Digital Control Register 5, Reserved */
	__IO uint32_t TR_DIG6;			/* Offset: 0x56*4, Test Mode Digital Control Register 6, Reserved */
	__IO uint32_t TR_ANA0;			/* Offset: 0x57*4, Test Mode Analog Control Register 0, Reserved */
	__IO uint32_t TR_ANA1;			/* Offset: 0x58*4, Test Mode Analog Control Register 1, Reserved */
	__IO uint32_t TR_ANA2;			/* Offset: 0x59*4, Test Mode Analog Control Register 2, Reserved */
	__IO uint32_t TR_ANA3;			/* Offset: 0x5a*4, Test Mode Analog Control Register 3, Reserved */
	__IO uint32_t TR_ANA4;			/* Offset: 0x5b*4, Test Mode Analog Control Register 4, Reserved */
	__IO uint32_t TR_ANA5;			/* Offset: 0x5c*4, Test Mode Analog Control Register 5, Reserved */
	__IO uint32_t TR_ANA6;			/* Offset: 0x5d*4, Test Mode Analog Control Register 6, Reserved */
	__IO uint32_t TR_ANA7;			/* Offset: 0x5e*4, Test Mode Analog Control Register 7, Reserved */
	 __I uint32_t SR_TR_ANA0;		/* Offset: 0x5f*4, Test Mode Analog Status Register 0, Reserved */
} hal_codec_t;

/**
 * @brief This struct defines the parallel audio data of the Codec 
 */
//#if (CODEC_ADC_DATA_MODE == CODEC_ADC_DATA_MODE_24BIT)
typedef struct {
	__I  int32_t adc1_do;		/* Offset: 0x00, ADC1 channel audio data parallel output */
	__I  int32_t adc2_do;		/* Offset: 0x04, ADC2 channel audio data parallel output */
	__I  int32_t adc3_do;		/* Offset: 0x08, ADC3 channel audio data parallel output */
	__I  int32_t adc4_do;		/* Offset: 0x0c, ADC4 channel audio data parallel output */
	__I  int32_t adc5_do;		/* Offset: 0x10, ADC5 channel audio data parallel output */
	__I  int32_t adc6_do;		/* Offset: 0x14, ADC6 channel audio data parallel output */
	__I  int32_t adc7_do;		/* Offset: 0x18, ADC7 channel audio data parallel output */
	__I  int32_t adc8_do;		/* Offset: 0x1c, ADC8 channel audio data parallel output */
	__I  int32_t adc9_do;		/* Offset: 0x20, ADC9 channel audio data parallel output */
	__I  int32_t adc10_do;		/* Offset: 0x24, ADC10 channel audio data parallel output */
	__I  int32_t reserved[2];
	__IO int32_t dac_dil;		/* Offset: 0x30, DAC L-channel audio data parallel input */
	__IO int32_t dac_dir;		/* Offset: 0x34, DAC R-channel audio data parallel input */
} hal_codec_data_t;
//#elif (CODEC_ADC_DATA_MODE == CODEC_ADC_DATA_MODE_16BIT)
//typedef struct {
//	__I  int32_t adc12_do;		/* Offset: 0x00, ADC12 channel audio data parallel output */
//	__I  int32_t adc34_do;		/* Offset: 0x04, ADC34 channel audio data parallel output */
//	__I  int32_t adc56_do;		/* Offset: 0x08, ADC56 channel audio data parallel output */
//	__I  int32_t adc78_do;		/* Offset: 0x0C, ADC78 channel audio data parallel output */
//	__I  int32_t adc9a_do;		/* Offset: 0x10, ADC9a channel audio data parallel output */
//	__I  int32_t reserved[7];
//	__IO int32_t dac_dil;		/* Offset: 0x30, DAC L-channel audio data parallel input */
//	__IO int32_t dac_dir;		/* Offset: 0x34, DAC R-channel audio data parallel input */
//} hal_codec_data_t;
//#endif

/* 0x00: SR */
#define HAL_CODEC_PON_ACK_SHIFT						    7
#define HAL_CODEC_PON_ACK_MASK						    (0x01 << HAL_CODEC_PON_ACK_SHIFT)
#define HAL_CODEC_IRQ_PENDING_SHIFT					    6
#define HAL_CODEC_IRQ_PENDING_MASK					    (0x01 << HAL_CODEC_IRQ_PENDING_SHIFT)

/* 0x01: SR PuN */
#define HAL_CODEC_DAC_SMUTE_STATUS_SHIFT				7
#define HAL_CODEC_DAC_SMUTE_STATUS_MASK					(0x01 << HAL_CODEC_DAC_SMUTE_STATUS_SHIFT)
#define HAL_CODEC_ADC12_SMUTE_STATUS_SHIFT				5
#define HAL_CODEC_ADC12_SMUTE_STATUS_MASK				(0x01 << HAL_CODEC_ADC12_SMUTE_STATUS_SHIFT)
#define HAL_CODEC_ADC3456_SMUTE_STATUS_SHIFT			4
#define HAL_CODEC_ADC3456_SMUTE_STATUS_MASK				(0x01 << HAL_CODEC_ADC3456_SMUTE_STATUS_SHIFT)
#define HAL_CODEC_DAC_STANDBY_STATUS_SHIFT				3
#define HAL_CODEC_DAC_STANDBY_STATUS_MASK				(0x01 << HAL_CODEC_DAC_STANDBY_STATUS_SHIFT)

/* 0x02: SR AIAS */
#define HAL_CODEC_DAC_AIAS_LOCKED_STATUS_SHIFT			7
#define HAL_CODEC_DAC_AIAS_LOCKED_STATUS_MASK			(0x01 << HAL_CODEC_DAC_AIAS_LOCKED_STATUS_SHIFT)
#define HAL_CODEC_ADC12_AIAS_LOCKED_STATUS_SHIFT		5
#define HAL_CODEC_ADC12_AIAS_LOCKED_STATUS_MASK			(0x01 << HAL_CODEC_ADC12_AIAS_LOCKED_STATUS_SHIFT)
#define HAL_CODEC_ADC3456_AIAS_LOCKED_STATUS_SHIFT		4
#define HAL_CODEC_ADC3456_AIAS_LOCKED_STATUS_MASK		(0x01 << HAL_CODEC_ADC3456_AIAS_LOCKED_STATUS_SHIFT)
#define HAL_CODEC_DAC_AIAS_UNKNOWN_FS_SHIFT				3
#define HAL_CODEC_DAC_AIAS_UNKNOWN_FS_MASK				(0x01 << HAL_CODEC_DAC_AIAS_UNKNOWN_FS_SHIFT)
#define HAL_CODEC_ADC12_AIAS_UNKNOWN_FS_SHIFT			1
#define HAL_CODEC_ADC12_AIAS_UNKNOWN_FS_MASK			(0x01 << HAL_CODEC_ADC12_AIAS_UNKNOWN_FS_SHIFT)
#define HAL_CODEC_ADC3456_AIAS_UNKNOWN_FS_SHIFT			0
#define HAL_CODEC_ADC3456_AIAS_UNKNOWN_FS_MASK			(0x01 << HAL_CODEC_ADC3456_AIAS_UNKNOWN_FS_SHIFT)

/* 0x03: SR WT */
#define HAL_CODEC_IRQ_WT_PENDING_SHIFT					7
#define HAL_CODEC_IRQ_WT_PENDING_MASK					(0x01 << HAL_CODEC_IRQ_WT_PENDING_SHIFT)

/* 0x04: ICR -- Interrupt Control Register */
#define HAL_CODEC_INT_FORM_SHIFT						6
#define HAL_CODEC_INT_FORM_MASK							(0x03 << HAL_CODEC_INT_FORM_SHIFT)
#define HAL_CODEC_WT_INT_FORM_SHIFT						4
#define HAL_CODEC_WT_INT_FORM_MASK						(0x03 << HAL_CODEC_WT_INT_FORM_SHIFT)

/* 0x05: IMR -- Interrupt Mask Register */
#define HAL_CODEC_AIAS_UNLOCK_MASK_SHIFT				7
#define HAL_CODEC_AIAS_UNLOCK_MASK_MASK					(0x01 << HAL_CODEC_AIAS_UNLOCK_MASK_SHIFT)
#define HAL_CODEC_WT_DETECT_MASK_SHIFT					6
#define HAL_CODEC_WT_DETECT_MASK_MASK					(0x01 << HAL_CODEC_WT_DETECT_MASK_SHIFT)
#define HAL_CODEC_SHORT_CIRCUIT_MASK_SHIFT				4
#define HAL_CODEC_SHORT_CIRCUIT_MASK_MASK				(0x01 << HAL_CODEC_SHORT_CIRCUIT_MASK_SHIFT)

/* 0x06: IFR -- Interrupt Flag Register */
#define HAL_CODEC_AIAS_UNLOCK_FLAG_SHIFT				7
#define HAL_CODEC_AIAS_UNLOCK_FLAG_MASK					(0x01 << HAL_CODEC_AIAS_UNLOCK_FLAG_SHIFT)
#define HAL_CODEC_WT_DETECT_FLAG_SHIFT					6
#define HAL_CODEC_WT_DETECT_FLAG_MASK					(0x01 << HAL_CODEC_WT_DETECT_FLAG_SHIFT)
#define HAL_CODEC_SHORT_CIRCUIT_FLAG_SHIFT				4
#define HAL_CODEC_SHORT_CIRCUIT_FLAG_MASK				(0x01 << HAL_CODEC_SHORT_CIRCUIT_FLAG_SHIFT)

/* 0x07: CR_ViC */
#define HAL_CODEC_ANALOG_DAC_SLEEP_SHIFT				3
#define HAL_CODEC_ANALOG_DAC_SLEEP_MASK					(0x01 << HAL_CODEC_ANALOG_DAC_SLEEP_SHIFT)
#define HAL_CODEC_ANALOG_ADC_SLEEP_SHIFT				2
#define HAL_CODEC_ANALOG_ADC_SLEEP_MASK					(0x01 << HAL_CODEC_ANALOG_ADC_SLEEP_SHIFT)
#define HAL_CODEC_ANALOG_STANDBY_SHIFT					1
#define HAL_CODEC_ANALOG_STANDBY_MASK					(0x01 << HAL_CODEC_ANALOG_STANDBY_SHIFT)
#define HAL_CODEC_DIGITAL_STANDBY_SHIFT					0
#define HAL_CODEC_DIGITAL_STANDBY_MASK					(0x01 << HAL_CODEC_DIGITAL_STANDBY_SHIFT)

/* 0x08: CR_CK */
#define HAL_CODEC_MCLK_SHIFT							0
#define HAL_CODEC_MCLK_MASK								(0x03 << HAL_CODEC_MCLK_SHIFT)

/* 0x09: AICR_DAC */
#define HAL_CODEC_DAC_WORD_LENGTH_SHIFT					6
#define HAL_CODEC_DAC_WORD_LENGTH_MASK					(0x03 << HAL_CODEC_DAC_WORD_LENGTH_SHIFT)
#define HAL_CODEC_DAC_MASTER_SLAVE_SEL_SHIFT			5
#define HAL_CODEC_DAC_MASTER_SLAVE_SEL_MASK				(0x01 << HAL_CODEC_DAC_MASTER_SLAVE_SEL_SHIFT)
#define HAL_CODEC_DAC_AUDIO_IF_ACTIVATION_SHIFT			4
#define HAL_CODEC_DAC_AUDIO_IF_ACTIVATION_MASK			(0x01 << HAL_CODEC_DAC_AUDIO_IF_ACTIVATION_SHIFT)
#define HAL_CODEC_DAC_PAIRING_SHIFT						2
#define HAL_CODEC_DAC_PAIRING_MASK						(0x01 << HAL_CODEC_DAC_PAIRING_SHIFT)
#define HAL_CODEC_DAC_AUDIO_IF_SHIFT					0
#define HAL_CODEC_DAC_AUDIO_IF_MASK						(0x03 << HAL_CODEC_DAC_AUDIO_IF_SHIFT)
#define HAL_CODEC_DAC_AUDIO_IF_PARALLEL_MODE			(0x00 << HAL_CODEC_DAC_AUDIO_IF_SHIFT)

/* 0x0a: AICR_ADC12 */
#define HAL_CODEC_ADC12_WORD_LENGTH_SHIFT				6
#define HAL_CODEC_ADC12_WORD_LENGTH_MASK				(0x03 << HAL_CODEC_ADC12_WORD_LENGTH_SHIFT)
#define HAL_CODEC_ADC12_MASTER_SLAVE_SEL_SHIFT			5
#define HAL_CODEC_ADC12_MASTER_SLAVE_SEL_MASK			(0x01 << HAL_CODEC_ADC12_MASTER_SLAVE_SEL_SHIFT)
#define HAL_CODEC_ADC12_PAIRING_SHIFT					2
#define HAL_CODEC_ADC12_PAIRING_MASK					(0x01 << HAL_CODEC_ADC12_PAIRING_SHIFT)
#define HAL_CODEC_ADC12_AUDIO_IF_SHIFT					0
#define HAL_CODEC_ADC12_AUDIO_IF_MASK					(0x03 << HAL_CODEC_ADC12_AUDIO_IF_SHIFT)
#define HAL_CODEC_ADC12_AUDIO_IF_PARALLEL_MODE			(0x00 << HAL_CODEC_ADC12_AUDIO_IF_SHIFT)

/* 0x0b: AICR_SB_ADC12 */
#define HAL_CODEC_ADC12_AUDIO_IF_ACTIVATION_SHIFT		0
#define HAL_CODEC_ADC12_AUDIO_IF_ACTIVATION_MASK		(0x01 << HAL_CODEC_ADC12_AUDIO_IF_ACTIVATION_SHIFT)

/* 0x0c: AICR_ADC3456 */
#define HAL_CODEC_ADC3456_WORD_LENGTH_SHIFT				6
#define HAL_CODEC_ADC3456_WORD_LENGTH_MASK				(0x03 << HAL_CODEC_ADC3456_WORD_LENGTH_SHIFT)
#define HAL_CODEC_ADC3456_MASTER_SLAVE_SEL_SHIFT		5
#define HAL_CODEC_ADC3456_MASTER_SLAVE_SEL_MASK			(0x01 << HAL_CODEC_ADC3456_MASTER_SLAVE_SEL_SHIFT)
#define HAL_CODEC_ADC3456_PAIRING_SHIFT					2
#define HAL_CODEC_ADC3456_PAIRING_MASK					(0x01 << HAL_CODEC_ADC3456_PAIRING_SHIFT)
#define HAL_CODEC_ADC3456_AUDIO_IF_SHIFT				0
#define HAL_CODEC_ADC3456_AUDIO_IF_MASK					(0x03 << HAL_CODEC_ADC3456_AUDIO_IF_SHIFT)
#define HAL_CODEC_ADC3456_AUDIO_IF_PARALLEL_MODE		(0x00 << HAL_CODEC_ADC3456_AUDIO_IF_SHIFT)

/* 0x0d: AICR_SB_ADC3456 */
#define HAL_CODEC_ADC56_AUDIO_IF_ACTIVATION_SHIFT		1
#define HAL_CODEC_ADC56_AUDIO_IF_ACTIVATION_MASK		(0x01 << HAL_CODEC_ADC56_AUDIO_IF_ACTIVATION_SHIFT)
#define HAL_CODEC_ADC34_AUDIO_IF_ACTIVATION_SHIFT		0
#define HAL_CODEC_ADC34_AUDIO_IF_ACTIVATION_MASK		(0x01 << HAL_CODEC_ADC34_AUDIO_IF_ACTIVATION_SHIFT)

/* 0x0e: FCR_DAC */
#define HAL_CODEC_DAC_FREQ_SHIFT						0
#define HAL_CODEC_DAC_FREQ_MASK							(0x0f << HAL_CODEC_DAC_FREQ_SHIFT)

/* 0x0f: DAC_FS_ADJ MSB */
#define HAL_CODEC_DAC_FREQ_TUNE_VALID_SHIFT				7
#define HAL_CODEC_DAC_FREQ_TUNE_VALID_MASK				(0x01 << HAL_CODEC_DAC_FREQ_TUNE_VALID_SHIFT)
#define HAL_CODEC_DAC_FREQ_TUNE_MSB_SHIFT				0
#define HAL_CODEC_DAC_FREQ_TUNE_MSB_MASK				(0x7f << HAL_CODEC_DAC_FREQ_TUNE_MSB_SHIFT)

/* 0x10: DAC_FS_ADJ LSB */
#define HAL_CODEC_DAC_FREQ_TUNE_LSB_SHIFT				0
#define HAL_CODEC_DAC_FREQ_TUNE_LSB_MASK				(0xff << HAL_CODEC_DAC_FREQ_TUNE_LSB_SHIFT)

/* 0x11: CR_ADC12_WNF */
#define HAL_CODEC_WNF_ADC12_SHIFT						0
#define HAL_CODEC_WNF_ADC12_MASK						(0x03 << HAL_CODEC_WNF_ADC12_SHIFT)

/* 0x12: CR_ADC3456_WNF */
#define HAL_CODEC_WNF_ADC56_SHIFT						2
#define HAL_CODEC_WNF_ADC56_MASK						(0x03 << HAL_CODEC_WNF_ADC56_SHIFT)
#define HAL_CODEC_WNF_ADC34_SHIFT						0
#define HAL_CODEC_WNF_ADC34_MASK						(0x03 << HAL_CODEC_WNF_ADC34_SHIFT)

/* 0x13: FCR_ADC12 */
#define HAL_CODEC_ADC12_HPF_EN_SHIFT					4
#define HAL_CODEC_ADC12_HPF_EN_MASK						(0x01 << HAL_CODEC_ADC12_HPF_EN_SHIFT)
#define HAL_CODEC_ADC12_FREQ_SHIFT						0
#define HAL_CODEC_ADC12_FREQ_MASK						(0x0f << HAL_CODEC_ADC12_FREQ_SHIFT)

/* 0x14: ADC12_FS_ADJ MSB */
#define HAL_CODEC_ADC12_FREQ_TUNE_VALID_SHIFT			7
#define HAL_CODEC_ADC12_FREQ_TUNE_VALID_MASK			(0x01 << HAL_CODEC_ADC12_FREQ_TUNE_VALID_SHIFT)
#define HAL_CODEC_ADC12_FREQ_TUNE_MSB_SHIFT				0
#define HAL_CODEC_ADC12_FREQ_TUNE_MSB_MASK				(0x7f << HAL_CODEC_ADC12_FREQ_TUNE_MSB_SHIFT)

/* 0x15: ADC12_FS_ADJ LSB */
#define HAL_CODEC_ADC12_FREQ_TUNE_LSB_SHIFT				0
#define HAL_CODEC_ADC12_FREQ_TUNE_LSB_MASK				(0xff << HAL_CODEC_ADC12_FREQ_TUNE_LSB_SHIFT)

/* 0x16: FCR_ADC3456 */
#define HAL_CODEC_ADC56_HPF_EN_SHIFT					5
#define HAL_CODEC_ADC56_HPF_EN_MASK						(0x01 << HAL_CODEC_ADC56_HPF_EN_SHIFT)
#define HAL_CODEC_ADC34_HPF_EN_SHIFT					4
#define HAL_CODEC_ADC34_HPF_EN_MASK						(0x01 << HAL_CODEC_ADC34_HPF_EN_SHIFT)
#define HAL_CODEC_ADC3456_FREQ_SHIFT					0
#define HAL_CODEC_ADC3456_FREQ_MASK						(0x0f << HAL_CODEC_ADC3456_FREQ_SHIFT)

/* 0x17: ADC3456_FS_ADJ MSB */
#define HAL_CODEC_ADC3456_FREQ_TUNE_VALID_SHIFT			7
#define HAL_CODEC_ADC3456_FREQ_TUNE_VALID_MASK			(0x01 << HAL_CODEC_ADC3456_FREQ_TUNE_VALID_SHIFT)
#define HAL_CODEC_ADC3456_FREQ_TUNE_MSB_SHIFT			0
#define HAL_CODEC_ADC3456_FREQ_TUNE_MSB_MASK			(0x7f << HAL_CODEC_ADC3456_FREQ_TUNE_MSB_SHIFT)

/* 0x18: ADC3456_FS_ADJ LSB */
#define HAL_CODEC_ADC3456_FREQ_TUNE_LSB_SHIFT			0
#define HAL_CODEC_ADC3456_FREQ_TUNE_LSB_MASK			(0xff << HAL_CODEC_ADC3456_FREQ_TUNE_LSB_SHIFT)

/* 0x19: CR_DMIC12 */
#define HAL_CODEC_DMIC1_STANDBY_SHIFT					7
#define HAL_CODEC_DMIC1_STANDBY_MASK					(0x01 << HAL_CODEC_DMIC1_STANDBY_SHIFT)
#define HAL_CODEC_DMIC2_STANDBY_SHIFT					6
#define HAL_CODEC_DMIC2_STANDBY_MASK					(0x01 << HAL_CODEC_DMIC2_STANDBY_SHIFT)
#define HAL_CODEC_DMIC12_RATE_SHIFT						4
#define HAL_CODEC_DMIC12_RATE_MASK						(0x03 << HAL_CODEC_DMIC12_RATE_SHIFT)
#define HAL_CODEC_ADC_DMIC_SEL1_SHIFT					2
#define HAL_CODEC_ADC_DMIC_SEL1_MASK					(0x03 << HAL_CODEC_ADC_DMIC_SEL1_SHIFT)
#define HAL_CODEC_ADC_DMIC_SEL2_SHIFT					0
#define HAL_CODEC_ADC_DMIC_SEL2_MASK					(0x03 << HAL_CODEC_ADC_DMIC_SEL2_SHIFT)

/* 0x1a: CR_DMIC34 */
#define HAL_CODEC_DMIC3_STANDBY_SHIFT					7
#define HAL_CODEC_DMIC3_STANDBY_MASK					(0x01 << HAL_CODEC_DMIC3_STANDBY_SHIFT)
#define HAL_CODEC_DMIC4_STANDBY_SHIFT					6
#define HAL_CODEC_DMIC4_STANDBY_MASK					(0x01 << HAL_CODEC_DMIC4_STANDBY_SHIFT)
#define HAL_CODEC_DMIC3456_RATE_SHIFT					4
#define HAL_CODEC_DMIC3456_RATE_MASK					(0x03 << HAL_CODEC_DMIC3456_RATE_SHIFT)
#define HAL_CODEC_ADC_DMIC_SEL3_SHIFT					2
#define HAL_CODEC_ADC_DMIC_SEL3_MASK					(0x03 << HAL_CODEC_ADC_DMIC_SEL3_SHIFT)
#define HAL_CODEC_ADC_DMIC_SEL4_SHIFT					0
#define HAL_CODEC_ADC_DMIC_SEL4_MASK					(0x03 << HAL_CODEC_ADC_DMIC_SEL4_SHIFT)

/* 0x1b: CR_DMIC56 */
#define HAL_CODEC_DMIC5_STANDBY_SHIFT					7
#define HAL_CODEC_DMIC5_STANDBY_MASK					(0x01 << HAL_CODEC_DMIC5_STANDBY_SHIFT)
#define HAL_CODEC_DMIC6_STANDBY_SHIFT					6
#define HAL_CODEC_DMIC6_STANDBY_MASK					(0x01 << HAL_CODEC_DMIC6_STANDBY_SHIFT)
#define HAL_CODEC_ADC_DMIC_SEL5_SHIFT					2
#define HAL_CODEC_ADC_DMIC_SEL5_MASK					(0x03 << HAL_CODEC_ADC_DMIC_SEL5_SHIFT)
#define HAL_CODEC_ADC_DMIC_SEL6_SHIFT					0
#define HAL_CODEC_ADC_DMIC_SEL6_MASK					(0x03 << HAL_CODEC_ADC_DMIC_SEL6_SHIFT)

/* 0x1c: CR_LO */
#define HAL_CODEC_LO_MUTE_SHIFT							7
#define HAL_CODEC_LO_MUTE_MASK							(0x01 << HAL_CODEC_LO_MUTE_SHIFT)
#define HAL_CODEC_LO_STANDBY_SHIFT						4
#define HAL_CODEC_LO_STANDBY_MASK						(0x01 << HAL_CODEC_LO_STANDBY_SHIFT)
#define HAL_CODEC_LO_SEL_SHIFT							0
#define HAL_CODEC_LO_SEL_MASK							(0x07 << HAL_CODEC_LO_SEL_SHIFT)

/* 0x1d: CR_HP */
#define HAL_CODEC_HP_MUTE_SHIFT							7
#define HAL_CODEC_HP_MUTE_MASK							(0x01 << HAL_CODEC_HP_MUTE_SHIFT)
#define HAL_CODEC_HP_STANDBY_SHIFT						4
#define HAL_CODEC_HP_STANDBY_MASK						(0x01 << HAL_CODEC_HP_STANDBY_SHIFT)
#define HAL_CODEC_HP_SEL_SHIFT							0
#define HAL_CODEC_HP_SEL_MASK							(0x07 << HAL_CODEC_HP_SEL_SHIFT)

/* 0x1e ~ 0x1f: GCR_HPL/GCR_HPR */
#define HAL_CODEC_GO_SHIFT								0
#define HAL_CODEC_GO_MASK								(0x1f << HAL_CODEC_GO_SHIFT)
#define HAL_CODEC_GO(x)									(((12-x) & HAL_CODEC_GO_MASK) << HAL_CODEC_GO_SHIFT)

/* 0x20: CR_MIC1 */
#define HAL_CODEC_MICBIAS1_V_SHIFT						7
#define HAL_CODEC_MICBIAS1_V_MASK						(0x01 << HAL_CODEC_MICBIAS1_V_SHIFT)
#define HAL_CODEC_MICDIFF1_SHIFT						6
#define HAL_CODEC_MICDIFF1_MASK							(0x01 << HAL_CODEC_MICDIFF1_SHIFT)
#define HAL_CODEC_MICBIAS1_STANDBY_SHIFT				5
#define HAL_CODEC_MICBIAS1_STANDBY_MASK					(0x01 << HAL_CODEC_MICBIAS1_STANDBY_SHIFT)
#define HAL_CODEC_MICCAP1_SHIFT							0
#define HAL_CODEC_MICCAP1_MASK							(0x01 << HAL_CODEC_MICCAP1_SHIFT)

/* 0x21: CR_MIC2 */
#define HAL_CODEC_MICBIAS2_V_SHIFT						7
#define HAL_CODEC_MICBIAS2_V_MASK						(0x01 << HAL_CODEC_MICBIAS2_V_SHIFT)
#define HAL_CODEC_MICDIFF2_SHIFT						6
#define HAL_CODEC_MICDIFF2_MASK							(0x01 << HAL_CODEC_MICDIFF2_SHIFT)
#define HAL_CODEC_MICBIAS2_STANDBY_SHIFT				5
#define HAL_CODEC_MICBIAS2_STANDBY_MASK					(0x01 << HAL_CODEC_MICBIAS2_STANDBY_SHIFT)
#define HAL_CODEC_MICCAP2_SHIFT							0
#define HAL_CODEC_MICCAP2_MASK							(0x01 << HAL_CODEC_MICCAP2_SHIFT)

/* 0x22: GCR_MIC12 */
#define HAL_CODEC_GIM2_SHIFT							4
#define HAL_CODEC_GIM2_MASK								(0x0f << HAL_CODEC_GIM2_SHIFT)
#define HAL_CODEC_GIM1_SHIFT							0
#define HAL_CODEC_GIM1_MASK								(0x0f << HAL_CODEC_GIM1_SHIFT)

/* 0x23: CR_DAC */
#define HAL_CODEC_DAC_SOFT_MUTE_SHIFT					7
#define HAL_CODEC_DAC_SOFT_MUTE_MASK					(0x01 << HAL_CODEC_DAC_SOFT_MUTE_SHIFT)
#define HAL_CODEC_DAC_LEFT_ONLY_SHIFT					5
#define HAL_CODEC_DAC_LEFT_ONLY_MASK					(0x01 << HAL_CODEC_DAC_LEFT_ONLY_SHIFT)
#define HAL_CODEC_DAC_STANDBY_SHIFT						4
#define HAL_CODEC_DAC_STANDBY_MASK						(0x01 << HAL_CODEC_DAC_STANDBY_SHIFT)

/* 0x24: CR_ADC12 */
#define HAL_CODEC_ADC12_SOFT_MUTE_SHIFT					7
#define HAL_CODEC_ADC12_SOFT_MUTE_MASK					(0x01 << HAL_CODEC_ADC12_SOFT_MUTE_SHIFT)
#define HAL_CODEC_ADC2_STANDBY_SHIFT					5
#define HAL_CODEC_ADC2_STANDBY_MASK						(0x01 << HAL_CODEC_ADC2_STANDBY_SHIFT)
#define HAL_CODEC_ADC1_STANDBY_SHIFT					4
#define HAL_CODEC_ADC1_STANDBY_MASK						(0x01 << HAL_CODEC_ADC1_STANDBY_SHIFT)
#define HAL_CODEC_ADC12_POWER_MODE_SHIFT				3
#define HAL_CODEC_ADC12_POWER_MODE_MASK					(0x01 << HAL_CODEC_ADC12_POWER_MODE_SHIFT)

/* 0x25: CR_ADC34 */
#define HAL_CODEC_ADC34_SOFT_MUTE_SHIFT					7
#define HAL_CODEC_ADC34_SOFT_MUTE_MASK					(0x01 << HAL_CODEC_ADC34_SOFT_MUTE_SHIFT)

/* 0x26: CR_ADC56 */
#define HAL_CODEC_ADC56_SOFT_MUTE_SHIFT					7
#define HAL_CODEC_ADC56_SOFT_MUTE_MASK					(0x01 << HAL_CODEC_ADC56_SOFT_MUTE_SHIFT)

/* 0x27: MIX_0 */
#define HAL_CODEC_AIDACL_SEL_SHIFT						6
#define HAL_CODEC_AIDACL_SEL_MASK						(0x03 << HAL_CODEC_AIDACL_SEL_SHIFT)
#define HAL_CODEC_AIDACR_SEL_SHIFT						4
#define HAL_CODEC_AIDACR_SEL_MASK						(0x03 << HAL_CODEC_AIDACR_SEL_SHIFT)
#define HAL_CODEC_DAC_MIX_SHIFT							0
#define HAL_CODEC_DAC_MIX_MASK							(0x01 << HAL_CODEC_DAC_MIX_SHIFT)

/* 0x28: MIX_1 */
#define HAL_CODEC_MIXDACL_SEL_SHIFT						6
#define HAL_CODEC_MIXDACL_SEL_MASK						(0x03 << HAL_CODEC_MIXDACL_SEL_SHIFT)
#define HAL_CODEC_MIXDACR_SEL_SHIFT						4
#define HAL_CODEC_MIXDACR_SEL_MASK						(0x03 << HAL_CODEC_MIXDACR_SEL_SHIFT)
#define HAL_CODEC_MIXADC1_SEL_SHIFT						2
#define HAL_CODEC_MIXADC1_SEL_MASK						(0x03 << HAL_CODEC_MIXADC1_SEL_SHIFT)
#define HAL_CODEC_MIXADC2_SEL_SHIFT						0
#define HAL_CODEC_MIXADC2_SEL_MASK						(0x03 << HAL_CODEC_MIXADC2_SEL_SHIFT)

/* 0x29: MIX_2 */
#define HAL_CODEC_AIADC1_SEL_SHIFT						6
#define HAL_CODEC_AIADC1_SEL_MASK						(0x03 << HAL_CODEC_AIADC1_SEL_SHIFT)
#define HAL_CODEC_AIADC2_SEL_SHIFT						4
#define HAL_CODEC_AIADC2_SEL_MASK						(0x03 << HAL_CODEC_AIADC2_SEL_SHIFT)
#define HAL_CODEC_MIX_REC3_SEL_SHIFT					2
#define HAL_CODEC_MIX_REC3_SEL_MASK						(0x01 << HAL_CODEC_MIX_REC3_SEL_SHIFT)
#define HAL_CODEC_MIX_REC2_SEL_SHIFT					1
#define HAL_CODEC_MIX_REC2_SEL_MASK						(0x01 << HAL_CODEC_MIX_REC2_SEL_SHIFT)
#define HAL_CODEC_MIX_REC_SEL_SHIFT						0
#define HAL_CODEC_MIX_REC_SEL_MASK						(0x01 << HAL_CODEC_MIX_REC_SEL_SHIFT)

/* 0x2a: MIX_3 */
#define HAL_CODEC_AIADC3_SEL_SHIFT						2
#define HAL_CODEC_AIADC3_SEL_MASK						(0x03 << HAL_CODEC_AIADC3_SEL_SHIFT)
#define HAL_CODEC_AIADC4_SEL_SHIFT						0
#define HAL_CODEC_AIADC4_SEL_MASK						(0x03 << HAL_CODEC_AIADC4_SEL_SHIFT)

/* 0x2b: MIX_4 */
#define HAL_CODEC_AIADC5_SEL_SHIFT						2
#define HAL_CODEC_AIADC5_SEL_MASK						(0x03 << HAL_CODEC_AIADC5_SEL_SHIFT)
#define HAL_CODEC_AIADC6_SEL_SHIFT						0
#define HAL_CODEC_AIADC6_SEL_MASK						(0x03 << HAL_CODEC_AIADC6_SEL_SHIFT)

/* 0x2c ~ 0x2d: GODL/R */
#define HAL_CODEC_GOD_SHIFT								0
#define HAL_CODEC_GOD_MASK								(0x3f << HAL_CODEC_GOD_SHIFT)
#define HAL_CODEC_GOD(x)								(((uint8_t)(((int8_t)-x) & HAL_CODEC_GOD_MASK)) << HAL_CODEC_GOD_SHIFT)

/* 0x2e ~ 0x33: GIDx */
#define HAL_CODEC_GID_SHIFT								0
#define HAL_CODEC_GID_MASK								(0x7f << HAL_CODEC_GID_SHIFT)
#define HAL_CODEC_GID(x)								(((uint8_t)(((int8_t)x) & HAL_CODEC_GID_MASK)) << HAL_CODEC_GID_SHIFT)

/* 0x34 ~ 0x35: GCR_MIXDACL/R */
#define HAL_CODEC_GOMIX_SHIFT							0
#define HAL_CODEC_GOMIX_MASK							(0x1f << HAL_CODEC_GOMIX_SHIFT)
#define HAL_CODEC_GOMIX(x)								(((uint8_t)(((int8_t)-x) & HAL_CODEC_GOMIX_MASK)) << HAL_CODEC_GOMIX_SHIFT)

/* 0x36 ~ 0x37: GCR_MIXADCL/R */
#define HAL_CODEC_GIMIX_SHIFT							0
#define HAL_CODEC_GIMIX_MASK							(0x1f << HAL_CODEC_GIMIX_SHIFT)
#define HAL_CODEC_GIMIX(x)								(((uint8_t)(((int8_t)-x) & HAL_CODEC_GIMIX_MASK)) << HAL_CODEC_GIMIX_SHIFT)

/* 0x38: CR_DAC_AGC */
#define HAL_CODEC_DAC_AGC_EN_SHIFT						7
#define HAL_CODEC_DAC_AGC_EN_MASK						(0x01 << HAL_CODEC_DAC_AGC_EN_SHIFT)

/* 0x39/0x3b: DAC_AGC_THRES */
#define HAL_CODEC_DAC_AGC_THRES_SHIFT					0
#define HAL_CODEC_DAC_AGC_THRES_MASK					(0x1f << HAL_CODEC_DAC_AGC_THRES_SHIFT)
#define HAL_CODEC_DAC_AGC_THRES(x)						(((uint8_t)(((int8_t)-x) & HAL_CODEC_DAC_AGC_THRES_MASK)) << HAL_CODEC_DAC_AGC_THRES_SHIFT)

/* 0x3a/0x3c: DAC_AGC_COMP */
#define HAL_CODEC_DAC_AGC_COMP_SHIFT					0
#define HAL_CODEC_DAC_AGC_COMP_MASK						(0x07 << HAL_CODEC_DAC_AGC_COMP_SHIFT)

/* 0x3d: ADC12_AGC_0 */
#define HAL_CODEC_ADC12_AGC_STEREO_SHIFT				6
#define HAL_CODEC_ADC12_AGC_STEREO_MASK					(0x01 << HAL_CODEC_ADC12_AGC_STEREO_SHIFT)
#define HAL_CODEC_ADC12_AGC_TARGET_SHIFT				2
#define HAL_CODEC_ADC12_AGC_TARGET_MASK					(0x0f << HAL_CODEC_ADC12_AGC_TARGET_SHIFT)
#define HAL_CODEC_ADC12_AGC_EN_SHIFT					1
#define HAL_CODEC_ADC12_AGC_EN_MASK						(0x01 << HAL_CODEC_ADC12_AGC_EN_SHIFT)
#define HAL_CODEC_ADC12_AGC_SNR_OPT_EN_SHIFT			0
#define HAL_CODEC_ADC12_AGC_SNR_OPT_EN_MASK				(0x01 << HAL_CODEC_ADC12_AGC_SNR_OPT_EN_SHIFT)

/* 0x3e: ADC12_AGC_1 */
#define HAL_CODEC_ADC12_AGC_NG_EN_SHIFT					7
#define HAL_CODEC_ADC12_AGC_NG_EN_MASK					(0x01 << HAL_CODEC_ADC12_AGC_NG_EN_SHIFT)
#define HAL_CODEC_ADC12_AGC_NG_THRES_SHIFT				4
#define HAL_CODEC_ADC12_AGC_NG_THRES_MASK				(0x07 << HAL_CODEC_ADC12_AGC_NG_THRES_SHIFT)
#define HAL_CODEC_ADC12_AGC_HOLD_TIME_SHIFT				0
#define HAL_CODEC_ADC12_AGC_HOLD_TIME_MASK				(0x0f << HAL_CODEC_ADC12_AGC_HOLD_TIME_SHIFT)

/* 0x3f: ADC12_AGC_2 */
#define HAL_CODEC_ADC12_AGC_ATTACH_TIME_SHIFT			4
#define HAL_CODEC_ADC12_AGC_ATTACH_TIME_MASK			(0x0f << HAL_CODEC_ADC12_AGC_ATTACH_TIME_SHIFT)
#define HAL_CODEC_ADC12_AGC_DECAY_TIME_SHIFT			0
#define HAL_CODEC_ADC12_AGC_DECAY_TIME_MASK				(0x0F << HAL_CODEC_ADC12_AGC_DECAY_TIME_SHIFT)

/* 0x40: ADC12_AGC_3 */
#define HAL_CODEC_ADC12_AGC_MAX_GAIN_SHIFT				0
#define HAL_CODEC_ADC12_AGC_MAX_GAIN_MASK				(0x1f << HAL_CODEC_ADC12_AGC_MAX_GAIN_SHIFT)

/* 0x41: ADC12_AGC_4 */
#define HAL_CODEC_ADC12_AGC_MIN_GAIN_SHIFT				0
#define HAL_CODEC_ADC12_AGC_MIN_GAIN_MASK				(0x1f << HAL_CODEC_ADC12_AGC_MIN_GAIN_SHIFT)

/* 0x42: ADC34_AGC_0 */
#define HAL_CODEC_ADC34_AGC_STEREO_SHIFT				6
#define HAL_CODEC_ADC34_AGC_STEREO_MASK					(0x01 << HAL_CODEC_ADC34_AGC_STEREO_SHIFT)
#define HAL_CODEC_ADC34_AGC_TARGET_SHIFT				2
#define HAL_CODEC_ADC34_AGC_TARGET_MASK					(0x0f << HAL_CODEC_ADC34_AGC_TARGET_SHIFT)
#define HAL_CODEC_ADC34_AGC_EN_SHIFT					1
#define HAL_CODEC_ADC34_AGC_EN_MASK						(0x01 << HAL_CODEC_ADC34_AGC_EN_SHIFT)

/* 0x43: ADC34_AGC_1 */
#define HAL_CODEC_ADC34_AGC_NG_EN_SHIFT					7
#define HAL_CODEC_ADC34_AGC_NG_EN_MASK					(0x01 << HAL_CODEC_ADC34_AGC_NG_EN_SHIFT)
#define HAL_CODEC_ADC34_AGC_NG_THRES_SHIFT				4
#define HAL_CODEC_ADC34_AGC_NG_THRES_MASK				(0x07 << HAL_CODEC_ADC34_AGC_NG_THRES_SHIFT)
#define HAL_CODEC_ADC34_AGC_HOLD_TIME_SHIFT				0
#define HAL_CODEC_ADC34_AGC_HOLD_TIME_MASK				(0x0f << HAL_CODEC_ADC34_AGC_HOLD_TIME_SHIFT)

/* 0x44: ADC34_AGC_2 */
#define HAL_CODEC_ADC34_AGC_ATTACH_TIME_SHIFT			4
#define HAL_CODEC_ADC34_AGC_ATTACH_TIME_MASK			(0x0f << HAL_CODEC_ADC34_AGC_ATTACH_TIME_SHIFT)
#define HAL_CODEC_ADC34_AGC_DECAY_TIME_SHIFT			0
#define HAL_CODEC_ADC34_AGC_DECAY_TIME_MASK				(0x0F << HAL_CODEC_ADC34_AGC_DECAY_TIME_SHIFT)

/* 0x45: ADC34_AGC_3 */
#define HAL_CODEC_ADC34_AGC_MAX_GAIN_SHIFT				0
#define HAL_CODEC_ADC34_AGC_MAX_GAIN_MASK				(0x1f << HAL_CODEC_ADC34_AGC_MAX_GAIN_SHIFT)

/* 0x46: ADC34_AGC_4 */
#define HAL_CODEC_ADC34_AGC_MIN_GAIN_SHIFT				0
#define HAL_CODEC_ADC34_AGC_MIN_GAIN_MASK				(0x1f << HAL_CODEC_ADC34_AGC_MIN_GAIN_SHIFT)

/* 0x47: ADC56_AGC_0 */
#define HAL_CODEC_ADC56_AGC_STEREO_SHIFT				6
#define HAL_CODEC_ADC56_AGC_STEREO_MASK					(0x01 << HAL_CODEC_ADC56_AGC_STEREO_SHIFT)
#define HAL_CODEC_ADC56_AGC_TARGET_SHIFT				2
#define HAL_CODEC_ADC56_AGC_TARGET_MASK					(0x0f << HAL_CODEC_ADC56_AGC_TARGET_SHIFT)
#define HAL_CODEC_ADC56_AGC_EN_SHIFT					1
#define HAL_CODEC_ADC56_AGC_EN_MASK						(0x01 << HAL_CODEC_ADC56_AGC_EN_SHIFT)

/* 0x48: ADC56_AGC_1 */
#define HAL_CODEC_ADC56_AGC_NG_EN_SHIFT					7
#define HAL_CODEC_ADC56_AGC_NG_EN_MASK					(0x01 << HAL_CODEC_ADC56_AGC_NG_EN_SHIFT)
#define HAL_CODEC_ADC56_AGC_NG_THRES_SHIFT				4
#define HAL_CODEC_ADC56_AGC_NG_THRES_MASK				(0x07 << HAL_CODEC_ADC56_AGC_NG_THRES_SHIFT)
#define HAL_CODEC_ADC56_AGC_HOLD_TIME_SHIFT				0
#define HAL_CODEC_ADC56_AGC_HOLD_TIME_MASK				(0x0f << HAL_CODEC_ADC56_AGC_HOLD_TIME_SHIFT)

/* 0x49: ADC56_AGC_2 */
#define HAL_CODEC_ADC56_AGC_ATTACH_TIME_SHIFT			4
#define HAL_CODEC_ADC56_AGC_ATTACH_TIME_MASK			(0x0f << HAL_CODEC_ADC56_AGC_ATTACH_TIME_SHIFT)
#define HAL_CODEC_ADC56_AGC_DECAY_TIME_SHIFT			0
#define HAL_CODEC_ADC56_AGC_DECAY_TIME_MASK				(0x0F << HAL_CODEC_ADC56_AGC_DECAY_TIME_SHIFT)

/* 0x4a: ADC56_AGC_3 */
#define HAL_CODEC_ADC56_AGC_MAX_GAIN_SHIFT				0
#define HAL_CODEC_ADC56_AGC_MAX_GAIN_MASK				(0x1f << HAL_CODEC_ADC34_AGC_MAX_GAIN_SHIFT)

/* 0x4b: ADC56_AGC_4 */
#define HAL_CODEC_ADC56_AGC_MIN_GAIN_SHIFT				0
#define HAL_CODEC_ADC56_AGC_MIN_GAIN_MASK				(0x1f << HAL_CODEC_ADC56_AGC_MIN_GAIN_SHIFT)

/* 0x4c: CR_WT */
#define HAL_CODEC_WT_EN_SHIFT							7
#define HAL_CODEC_WT_EN_MASK							(0x01 << HAL_CODEC_WT_EN_SHIFT)
#define HAL_CODEC_WT_ONLY_SHIFT							6
#define HAL_CODEC_WT_ONLY_MASK							(0x01 << HAL_CODEC_WT_ONLY_SHIFT)
#define HAL_CODEC_WT_RESTART_SHIFT						0
#define HAL_CODEC_WT_RESTART_MASK						(0x01 << HAL_CODEC_WT_RESTART_SHIFT)

/* 0x4d: CR_WT_2 */
#define HAL_CODEC_WT_CHANNEL_SEL_SHIFT					5
#define HAL_CODEC_WT_CHANNEL_SEL_MASK					(0x03 << HAL_CODEC_WT_CHANNEL_SEL_SHIFT)
#define HAL_CODEC_WT_ZCD_EN_SHIFT						4
#define HAL_CODEC_WT_ZCD_EN_MASK						(0x01 << HAL_CODEC_WT_ZCD_EN_SHIFT)
#define HAL_CODEC_WT_DMIC_FREQ_SHIFT					0
#define HAL_CODEC_WT_DMIC_FREQ_MASK						(0x07 << HAL_CODEC_WT_DMIC_FREQ_SHIFT)

/* 0x4e: CR_WT_3 */
#define HAL_CODEC_WT_POWER_SENSITIVITY_SHIFT			0
#define HAL_CODEC_WT_POWER_SENSITIVITY_MASK				(0x1F << HAL_CODEC_WT_POWER_SENSITIVITY_SHIFT)

/* 0x50: TR_DIG0 */
#define HAL_CODEC_FAST_ON_SHIFT							0
#define HAL_CODEC_FAST_ON_MASK							(0x01 << HAL_CODEC_FAST_ON_SHIFT)

/** 
 * @brief	This enum defines the MCLK frequency
 */
typedef enum {
	HAL_CODEC_CRYSTAL_12MHZ = 0x00,	/* 12 MHz */
	HAL_CODEC_CRYSTAL_13MHZ = 0x02,	/* 13 MHz */
} hal_codec_crystal_t;

/** 
 * @brief This enum defines the audio data word length of the codec for DSP protocol 
 */ 
typedef enum {
	HAL_CODEC_ADWL_16BIT = 0,
	HAL_CODEC_ADWL_24BIT,
} hal_codec_adwl_t;

/** 
 * @brief This enum defines the supported sample rate of the Codec 
 */
typedef enum {
	HAL_CODEC_FS_8KHZ = 0,	/* 8000 */
	HAL_CODEC_FS_11P025KHZ,	/* 11025 */
	HAL_CODEC_FS_12KHZ,		/* 12000 */
	HAL_CODEC_FS_16KHZ,		/* 16000 */
	HAL_CODEC_FS_22P05KHZ,	/* 22050 */
	HAL_CODEC_FS_24KHZ,		/* 24000 */
	HAL_CODEC_FS_32KHZ,		/* 32000 */
	HAL_CODEC_FS_44P1KHZ,	/* 44100 */
	HAL_CODEC_FS_48KHZ,		/* 48000 */
	HAL_CODEC_FS_88P2KHZ,	/* 88200 */
	HAL_CODEC_FS_96KHZ,		/* 96000 */
	HAL_CODEC_FS_176P4KHZ,	/* 176400 */
	HAL_CODEC_FS_192KHZ,	/* 192000 */
	HAL_CODEC_FS_NUM
} hal_codec_fs_t;

/**
 * @brief This enum defines the Wind Noise Filter modes 
 */
typedef enum {
	HAL_CODEC_WNF_OFF = 0,	/* Wind Noise filter disabled. */
	HAL_CODEC_WNF_MODE_1,	/* Enable Wind Noise filter mode 1. */
	HAL_CODEC_WNF_MODE_2,	/* Enable Wind Noise filter mode 2. */
	HAL_CODEC_WNF_MODE_3,	/* Enable Wind Noise filter mode 3. */
	HAL_CODEC_WNF_MODE_NUM
} hal_codec_wnf_mode_t;

/**
 * @brief This enum defines the clock of the digtal microphone 
 */
typedef enum {
	HAL_CODEC_DMIC_RATIO_4 = 0, 	/* MCLK/4  */
	HAL_CODEC_DMIC_RATIO_5,			/* MCLK/5  */
	HAL_CODEC_DMIC_RATIO_12,		/* MCLK/12 */
	HAL_CODEC_DMIC_RATIO_16,		/* MCLK/16 */
	HAL_CODEC_DMIC_RATIO_MAX
} hal_codec_dmic_clock_t;

/**
 * @brief This enum defines the clock of the digtal mocrophone when whisper trigger is enalbe and no voice is detected
 */
typedef enum {
	HAL_CODEC_WT_DMIC_RATIO_4 = 0, 	/* MCLK/4  */
	HAL_CODEC_WT_DMIC_RATIO_5,		/* MCLK/5  */
	HAL_CODEC_WT_DMIC_RATIO_12,		/* MCLK/12, default value */
	HAL_CODEC_WT_DMIC_RATIO_16 = 6, /* MCLK/16 */
} hal_codec_wt_dmic_clock_t;

/** 
 * @brief This enum defines ADC12/DMIC12 selection
 */
typedef enum {
	HAL_CODEC_ADC_DATA_PROVIEDE_ANALOG_ADC = 0,				/* Analog ADC data */
	HAL_CODEC_ADC_DATA_PROVIEDE_DMIC_SELF_CHANNEL,			/* Digital microphone interface data */
	HAL_CODEC_ADC_DATA_PROVIEDE_DMIC_TWO_CHANNEL_AVERAGE,	/* Average between digital mocrophone interface 1&2 data */
} hal_codec_adc_data_provided_t;

/** 
 * @brief This enum defines the analog microphone input mode selections 
 */
typedef enum {
	HAL_CODEC_AMIC_SINGLE_ENDED_INPUT = 0,	/* Single-ended input */
	HAL_CODEC_AMIC_DIFFERENTIAL_INPUT,		/* Differential input */
} hal_codec_amic_input_mode_t;

/** 
 * @brief This enum defines MICBIAS output state when MICBIAS in power-down mode
 */
typedef enum {
	HAL_CODEC_BIAS_PULL_DOWN = 0,	/* MICBIAS output pulled down to 0 */
	HAL_CODEC_BIAS_IMPENDANCE,		/* MICBIAS output in high impedance state */
} hal_codec_amic_bias_output_state_t;

/** 
 * @brief This enum defines the power mode of ADC12
 */
typedef enum {
	HAL_CODEC_ADC12_NORMAL_MODE = 0,	/* Normal mode */
	HAL_CODEC_ADC12_LOW_POWER_MODE,		/* Low power mode */
} hal_codec_adc12_power_mode_t;

/** 
 * @brief This enum defines the selection of audio data provided for DMIC3456
 */
typedef enum {
	HAL_CODEC_DMIC_SELF_CHANNEL = 0x01,			/* The data is provided from the channel itself */
	HAL_CODEC_DMIC_TWO_CHANNEL_AVERAGE = 0x02,	/* The data is provided from the average between two channels */
} hal_codec_dmic_data_provided_t;

/** 
 * @brief This enum defines the mixer mode on DAC path
 */
typedef enum {
	HAL_CODEC_PLAYBACK_DAC_ONLY = 0,			/* Playback DAC only */
	HAL_CODEC_PLAYBACK_DAC_MIXED_ADC,			/* Playback DAC + ADC */
} hal_codec_dac_mixer_mode_t;

/** 
 * @brief This enum defines the mixer mode on ADC12 path
 */
typedef enum {
	HAL_CODEC_ADC_MIXER_MODE_RECORD_ONLY = 0,	/* Record input only */
	HAL_CODEC_ADC_MIXER_MODE_RECORD_MIXED_DAC,	/* Record input + DAC */
} hal_codec_adc_mixer_mode_t;

/** 
 * @brief This struct defines the channel input selection for the DAC and ADCs
 */
typedef enum {
	HAL_CODEC_CHANNEL_SEL_NORMAL_INPUTS = 0,	/* Self Channel */
	HAL_CODEC_CHANNEL_SEL_CROSS_INPUTS,			/* The other channel */
	HAL_CODEC_CHANNEL_SEL_MIXED_INPUTS,			/* Average of the two channels */
	HAL_CODEC_CHANNEL_SEL_ZERO_INPUTS,			/* No inputs */
	HAL_CODEC_CHANNEL_SEL_MAX,
} hal_codec_channel_sel_t;

/**
 * @brief This enum defines the audio interface of the Codec 
 */
typedef enum {
	HAL_CODEC_AUDIO_INTERFACE_MASTER = 0,	/* Master Mode */
	HAL_CODEC_AUDIO_INTERFACE_SLAVE,		/* Slave Mode */
} hal_codec_ai_mode_t;

/** 
 * @brief This enum defines the proccessed channel selection of the whisper trigger
 */
typedef enum {
	HAL_CODEC_WT_CHANNEL_LEFT = 0,	/* Left channel */
	HAL_CODEC_WT_CHANNEL_RIGHT,		/* Right channel */
	HAL_CODEC_WT_CHANNEL_AVERAGE,	/* Average between left and right channels */
	HAL_CODEC_WT_CHANNEL_NUM,
} hal_codec_wt_channel_sel_t;

/** 
 * @brief This enum defines the DRC compression rate for DAC
 */
typedef enum {
	HAL_CODEC_DRC_RATE_1 = 0,	/* 1 */
	HAL_CODEC_DRC_RATE_2,		/* 2 */
	HAL_CODEC_DRC_RATE_4,		/* 4 */
	HAL_CODEC_DRC_RATE_8,		/* 8 */
	HAL_CODEC_DRC_RATE_16,		/* 16 */
	HAL_CODEC_DRC_RATE_32,		/* 32 */
	HAL_CODEC_DRC_RATE_MAX,
} hal_codec_dac_drc_comp_t;

/** 
 * @brief This enum defines the analog microphone gain value
 */
typedef enum {
	HAL_CODEC_GIM_M2DB = 0,		/* -2 dB */
	HAL_CODEC_GIM_0DB,			/*  0 dB, default */
	HAL_CODEC_GIM_2DB,			/*  2 dB */
	HAL_CODEC_GIM_4DB,			/*  4 dB */
	HAL_CODEC_GIM_6DB,			/*  6 dB */
	HAL_CODEC_GIM_8DB,			/*  8 dB */
	HAL_CODEC_GIM_10DB,			/* 10 dB */
	HAL_CODEC_GIM_12DB,			/* 12 dB */
	HAL_CODEC_GIM_14DB,			/* 14 dB */
	HAL_CODEC_GIM_16DB,			/* 16 dB */
	HAL_CODEC_GIM_18DB,			/* 18 dB */
	HAL_CODEC_GIM_20DB,			/* 20 dB */
	HAL_CODEC_GIM_22DB,			/* 22 dB */
	HAL_CODEC_GIM_24DB,			/* 24 dB */
	HAL_CODEC_GIM_NUM
} hal_codec_gim_t;

/** 
 * @brief This enum defines the target output level of ADCs
 */
typedef enum {
	HAL_CODEC_AGC_TARGET_MAX = 0,			/* MAX_TARGET */
	HAL_CODEC_AGC_TARGET_MAX_MINUS_1P5DB,	/* MAX_TARGET - 1.5dB */
	HAL_CODEC_AGC_TARGET_MAX_MINUS_3DB,		/* MAX_TARGET - 3dB */
	HAL_CODEC_AGC_TARGET_MAX_MINUS_4P5DB,	/* MAX_TARGET - 4.5dB */
	HAL_CODEC_AGC_TARGET_MAX_MINUS_6DB,		/* MAX_TARGET - 6dB */
	HAL_CODEC_AGC_TARGET_MAX_MINUS_7P5DB,	/* MAX_TARGET - 7.5dB */
	HAL_CODEC_AGC_TARGET_MAX_MINUS_9DB,		/* MAX_TARGET - 9dB */
	HAL_CODEC_AGC_TARGET_MAX_MINUS_10P5DB,	/* MAX_TARGET - 10.5dB */
	HAL_CODEC_AGC_TARGET_MAX_MINUS_12DB,	/* MAX_TARGET - 12dB */
	HAL_CODEC_AGC_TARGET_MAX_MINUS_13P5DB,	/* MAX_TARGET - 13.5dB */
	HAL_CODEC_AGC_TARGET_MAX_MINUS_15DB,	/* MAX_TARGET - 15dB */
	HAL_CODEC_AGC_TARGET_MAX_MINUS_16P5DB,	/* MAX_TARGET - 16.5dB */
	HAL_CODEC_AGC_TARGET_MAX_MINUS_18DB,	/* MAX_TARGET - 18dB */
	HAL_CODEC_AGC_TARGET_MAX_MINUS_19P5DB,	/* MAX_TARGET - 19.5dB (default) */
	HAL_CODEC_AGC_TARGET_MAX_MINUS_21DB,	/* MAX_TARGET - 21dB */
	HAL_CODEC_AGC_TARGET_MAX_MINUS_22P5DB,	/* MAX_TARGET - 22.5dB */
	HAL_CODEC_AGC_TARGET_NUM
} hal_codec_agc_target_t;

/** 
 * @brief This enum defines the Noise Gate Threshold value for ADCs
 */
typedef enum {
	HAL_CODEC_AGC_NG_THR_MIN = 0,	/* MIN_NG_THR (default) */
	HAL_CODEC_AGC_NG_THR_MIN_06DB,	/* MIN_NG_THR + 6dB */
	HAL_CODEC_AGC_NG_THR_MIN_12DB,	/* MIN_NG_THR + 12dB */
	HAL_CODEC_AGC_NG_THR_MIN_18DB,	/* MIN_NG_THR + 18dB */
	HAL_CODEC_AGC_NG_THR_MIN_24DB,	/* MIN_NG_THR + 24dB */
	HAL_CODEC_AGC_NG_THR_MIN_30DB,	/* MIN_NG_THR + 30dB */
	HAL_CODEC_AGC_NG_THR_MIN_36DB,	/* MIN_NG_THR + 36dB */
	HAL_CODEC_AGC_NG_THR_MIN_42DB,	/* MIN_NG_THR + 42dB */
	HAL_CODEC_AGC_NG_THR_NUM
} hal_codec_agc_ng_threshold_t;

/** 
 * @brief This enum defines the hold time before starting AGC adjusting to the TARGET value for ADCs
 */
typedef enum {
	HAL_CODEC_AGC_HOLD_TIME_0MS = 0,	/* 0 ms */
	HAL_CODEC_AGC_HOLD_TIME_2MS,		/* 2 ms */
	HAL_CODEC_AGC_HOLD_TIME_4MS,		/* 4 ms */
	HAL_CODEC_AGC_HOLD_TIME_8MS,		/* 8 ms */
	HAL_CODEC_AGC_HOLD_TIME_16MS,		/* 16 ms */
	HAL_CODEC_AGC_HOLD_TIME_32MS,		/* 32 ms */
	HAL_CODEC_AGC_HOLD_TIME_64MS,		/* 64 ms */
	HAL_CODEC_AGC_HOLD_TIME_128MS,		/* 128 ms */
	HAL_CODEC_AGC_HOLD_TIME_256MS,		/* 256 ms */
	HAL_CODEC_AGC_HOLD_TIME_512MS,		/* 512 ms */
	HAL_CODEC_AGC_HOLD_TIME_1P024S,		/* 1.024 s */
	HAL_CODEC_AGC_HOLD_TIME_2P048S,		/* 2.048 s */
	HAL_CODEC_AGC_HOLD_TIME_4P096S,		/* 4.096 s */
	HAL_CODEC_AGC_HOLD_TIME_8P192S,		/* 8.192 s */
	HAL_CODEC_AGC_HOLD_TIME_16P384S,	/* 16.384 s */
	HAL_CODEC_AGC_HOLD_TIME_32P768S,	/* 32.768 s */
	HAL_CODEC_AGC_HOLD_TIME_NUM
} hal_codec_agc_hold_time_t;

/** 
 * @brief This enum defines the attack time - gain ramp down for ADCs
 */
typedef enum {
	HAL_CODEC_AGC_ATTACK_TIME_32MS = 0,	/* 32 ms */
	HAL_CODEC_AGC_ATTACK_TIME_64MS,		/* 64 ms */
	HAL_CODEC_AGC_ATTACK_TIME_96MS,		/* 96 ms */
	HAL_CODEC_AGC_ATTACK_TIME_128MS,	/* 128 ms */
	HAL_CODEC_AGC_ATTACK_TIME_160MS,	/* 160 ms */
	HAL_CODEC_AGC_ATTACK_TIME_192MS,	/* 192 ms */
	HAL_CODEC_AGC_ATTACK_TIME_224MS,	/* 224 ms */
	HAL_CODEC_AGC_ATTACK_TIME_256MS,	/* 256 ms */
	HAL_CODEC_AGC_ATTACK_TIME_288MS,	/* 288 ms */
	HAL_CODEC_AGC_ATTACK_TIME_320MS,	/* 320 ms */
	HAL_CODEC_AGC_ATTACK_TIME_352MS,	/* 352 ms */
	HAL_CODEC_AGC_ATTACK_TIME_384MS,	/* 384 ms */
	HAL_CODEC_AGC_ATTACK_TIME_416MS,	/* 416 ms */
	HAL_CODEC_AGC_ATTACK_TIME_448MS,	/* 448 ms */
	HAL_CODEC_AGC_ATTACK_TIME_480MS,	/* 480 ms */
	HAL_CODEC_AGC_ATTACK_TIME_512MS,	/* 512 ms */
	HAL_CODEC_AGC_ATTACH_TIME_NUM
} hal_codec_agc_attack_time_t;

/** 
 * @brief This enum defines the decay time - gain ramp up for ADCs
 */
typedef enum {
	HAL_CODEC_AGC_DECAY_TIME_32MS = 0,	/* 32 ms */ 
	HAL_CODEC_AGC_DECAY_TIME_64MS,      /* 64 ms */ 
	HAL_CODEC_AGC_DECAY_TIME_96MS,      /* 96 ms */ 
	HAL_CODEC_AGC_DECAY_TIME_128MS,     /* 128 ms */
	HAL_CODEC_AGC_DECAY_TIME_160MS,     /* 160 ms */
	HAL_CODEC_AGC_DECAY_TIME_192MS,     /* 192 ms */
	HAL_CODEC_AGC_DECAY_TIME_224MS,     /* 224 ms */
	HAL_CODEC_AGC_DECAY_TIME_256MS,     /* 256 ms */
	HAL_CODEC_AGC_DECAY_TIME_288MS,     /* 288 ms */
	HAL_CODEC_AGC_DECAY_TIME_320MS,     /* 320 ms */
	HAL_CODEC_AGC_DECAY_TIME_352MS,     /* 352 ms */
	HAL_CODEC_AGC_DECAY_TIME_384MS,     /* 384 ms */
	HAL_CODEC_AGC_DECAY_TIME_416MS,     /* 416 ms */
	HAL_CODEC_AGC_DECAY_TIME_448MS,     /* 448 ms */
	HAL_CODEC_AGC_DECAY_TIME_480MS,     /* 480 ms */
	HAL_CODEC_AGC_DECAY_TIME_512MS,     /* 512 ms */
	HAL_CODEC_AGC_DECAY_TIME_NUM
} hal_codec_agc_decay_time_t;

/** 
 * @brief This enum defines the maximum gain value to apply to the ADCs
 *
 * @details	{description}
 */
typedef enum {
	HAL_CODEC_AGC_GAIN_0DB = 0,			/* 0 dB */
	HAL_CODEC_AGC_GAIN_1P5DB,			/* 1.5 dB */
	HAL_CODEC_AGC_GAIN_3DB,				/* 3 dB */
	HAL_CODEC_AGC_GAIN_4P5DB,			/* 4.5 dB */
	HAL_CODEC_AGC_GAIN_6DB,				/* 6 dB */
	HAL_CODEC_AGC_GAIN_7P5DB,			/* 7.5 dB */
	HAL_CODEC_AGC_GAIN_9DB,				/* 9 dB */
	HAL_CODEC_AGC_GAIN_10P5DB,			/* 10.5 dB */
	HAL_CODEC_AGC_GAIN_12DB,			/* 12 dB */
	HAL_CODEC_AGC_GAIN_13P5DB,			/* 13.5 dB */
	HAL_CODEC_AGC_GAIN_15DB,			/* 15 dB */
	HAL_CODEC_AGC_GAIN_16P5DB,			/* 16.5 dB */
	HAL_CODEC_AGC_GAIN_18DB,			/* 18 dB */
	HAL_CODEC_AGC_GAIN_19P5DB,			/* 19.5 dB */
	HAL_CODEC_AGC_GAIN_21DB,			/* 21 dB */
	HAL_CODEC_AGC_GAIN_22P5DB,			/* 22.5 dB */
	HAL_CODEC_AGC_GAIN_23DB,			/* 23 dB */
	HAL_CODEC_AGC_GAIN_24P5DB = 0x13,	/* 24.5 dB */
	HAL_CODEC_AGC_GAIN_26DB,			/* 26 dB */
	HAL_CODEC_AGC_GAIN_27P5DB,			/* 27.5 dB */
	HAL_CODEC_AGC_GAIN_29DB,			/* 29 dB */
	HAL_CODEC_AGC_GAIN_30P5DB,			/* 30.5 dB */
	HAL_CODEC_AGC_GAIN_32DB,			/* 32 dB */
	HAL_CODEC_AGC_GAIN_33P5DB,			/* 33.5 dB */
	HAL_CODEC_AGC_GAIN_35DB,			/* 35 dB */
	HAL_CODEC_AGC_GAIN_36P5DB,			/* 36.5 dB */
	HAL_CODEC_AGC_GAIN_38DB,			/* 38 dB */
	HAL_CODEC_AGC_GAIN_39P5DB,			/* 39.5 dB */
	HAL_CODEC_AGC_GAIN_41DB,			/* 41 dB */
	HAL_CODEC_AGC_GAIN_42P5DB,			/* 42.5 dB */
	HAL_CODEC_AGC_GAIN_NUM
} hal_codec_agc_gain_t;

/** 
 * @brief This struct defines the configuration information of AGC
 *
 * @details	{description}
 */
typedef struct {
	bool 						    stereo;					/* Stereo Link */
	hal_codec_agc_target_t 			target;					/* Target output level */
	bool    					    snr_optimisation_enable;/* Selection of the SNR optimisation system */
	bool    					    noise_gate_enable;		/* Selection of the Noise Gate system */
	hal_codec_agc_ng_threshold_t 	noise_gate_threshold;	/* Noise Gate Threshold */
	hal_codec_agc_hold_time_t 		hold_time;				/* Hold time */
	hal_codec_agc_attack_time_t 	attack_time;			/* Attack time */
	hal_codec_agc_decay_time_t 		decay_time;				/* Decay time */
	hal_codec_agc_gain_t 		    max_gain;				/* Maximum Gain */
	hal_codec_agc_gain_t 		    min_gain;				/* Minimum Gain */
} hal_codec_adc_agc_config_t;


#endif /* __REG_CODEC_H__ */
