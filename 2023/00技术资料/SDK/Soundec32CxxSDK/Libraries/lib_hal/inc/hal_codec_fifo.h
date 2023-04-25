/** 
***********************************************************
 * 
 * @file name	: hal_codec_fifo.h
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-09-16
 * @brief		: HAL Codec Fifo Driver Header File
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


#ifndef __HAL_CODEC_FIFO_H__
#define __HAL_CODEC_FIFO_H__

#ifdef	__cplusplus
extern "C"
{
#endif

#include "snc8600.h"
//#include "boardConfig.h"
#define HAL_CODEC_FIFO_AE_LEVEL					8		/*Almost empty level*/
#define HAL_CODEC_FIFO_AF_LEVEL					1		/*Almost empty level*/

#define HAL_CODEC_FIFO_STS_ERR_SHIFT			0
#define HAL_CODEC_FIFO_STS_ERR_MASK				(1 << HAL_CODEC_FIFO_STS_ERR_SHIFT)
#define HAL_CODEC_FIFO_STS_FULL_SHIFT			1
#define HAL_CODEC_FIFO_STS_FULL_MASK			(1 << HAL_CODEC_FIFO_STS_FULL_SHIFT)
#define HAL_CODEC_FIFO_STS_AF_SHIFT				2
#define HAL_CODEC_FIFO_STS_AF_MASK				(1 << HAL_CODEC_FIFO_STS_AF_SHIFT)
#define HAL_CODEC_FIFO_STS_HF_SHIFT				3
#define HAL_CODEC_FIFO_STS_HF_MASK				(1 << HAL_CODEC_FIFO_STS_HF_SHIFT)
#define HAL_CODEC_FIFO_STS_AE_SHIFT				4
#define HAL_CODEC_FIFO_STS_AE_MASK				(1 << HAL_CODEC_FIFO_STS_AE_SHIFT)
#define HAL_CODEC_FIFO_STS_EMPTY_SHIFT			5
#define HAL_CODEC_FIFO_STS_EMPTY_MASK			(1 << HAL_CODEC_FIFO_STS_EMPTY_SHIFT)
#define HAL_CODEC_FIFO_STS_WS_SHIFT				6
#define HAL_CODEC_FIFO_STS_WS_MASK				(1 << HAL_CODEC_FIFO_STS_WS_SHIFT)
#define HAL_CODEC_FIFO_STS_WORD_CNT_SHIFT 		7
#define HAL_CODEC_FIFO_STS_WORD_CNT_MASK		(0x1f << HAL_CODEC_FIFO_STS_WORD_CNT_SHIFT)
#define HAL_CODEC_FIFO_STS_CLR_CMPLT_SHIFT 		12
#define HAL_CODEC_FIFO_STS_CLR_CMPLT_MASK		(0x1 << HAL_CODEC_FIFO_STS_CLR_CMPLT_SHIFT)
#define HAL_CODEC_FIFO_STS_FIFO_EMPTY_SHIFT 	13
#define HAL_CODEC_FIFO_STS_FIFO_EMPTY_MASK		(1 << HAL_CODEC_FIFO_STS_FIFO_EMPTY_SHIFT)
#define HAL_CODEC_FIFO_STS_FIFO_WORD_CNT_SHIFT 	14
#define HAL_CODEC_FIFO_STS_FIFO_WORD_CNT_MASK	(0x1f << HAL_CODEC_FIFO_STS_FIFO_WORD_CNT_SHIFT)

#define HAL_CODEC_FIFO_INT_MASK_ERROR_SHFIT		0
#define HAL_CODEC_FIFO_INT_MASK_ERROR_MASK		(1 << HAL_CODEC_FIFO_INT_MASK_ERROR_SHFIT)
#define HAL_CODEC_FIFO_INT_MASK_FULL_SHFIT		1
#define HAL_CODEC_FIFO_INT_MASK_FULL_MASK		(1 << HAL_CODEC_FIFO_INT_MASK_FULL_SHFIT)
#define HAL_CODEC_FIFO_INT_MASK_AF_SHFIT		2
#define HAL_CODEC_FIFO_INT_MASK_AF_MASK			(1 << HAL_CODEC_FIFO_INT_MASK_AF_SHFIT)
#define HAL_CODEC_FIFO_INT_MASK_HF_SHFIT		3
#define HAL_CODEC_FIFO_INT_MASK_HF_MASK			(1 << HAL_CODEC_FIFO_INT_MASK_HF_SHFIT)
#define HAL_CODEC_FIFO_INT_MASK_AE_SHFIT		4
#define HAL_CODEC_FIFO_INT_MASK_AE_MASK			(1 << HAL_CODEC_FIFO_INT_MASK_AE_SHFIT)
#define HAL_CODEC_FIFO_INT_MASK_EMPTY_SHFIT		5
#define HAL_CODEC_FIFO_INT_MASK_EMPTY_MASK		(1 << HAL_CODEC_FIFO_INT_MASK_EMPTY_SHFIT)
#define HAL_CODEC_FIFO_INT_MASK_WS_SHFIT		6
#define HAL_CODEC_FIFO_INT_MASK_WS_MASK			(1 << HAL_CODEC_FIFO_INT_MASK_WS_SHFIT)

#define HAL_CODEC_FIFO_CTRL_INIT_SHIFT			0
#define HAL_CODEC_FIFO_CTRL_INIT_MASK			(1 << HAL_CODEC_FIFO_CTRL_INIT_SHIFT)
#define HAL_CODEC_FIFO_CTRL_CLR_SHIFT			1
#define HAL_CODEC_FIFO_CTRL_CLR_MASK			(1 << HAL_CODEC_FIFO_CTRL_CLR_SHIFT)
#define HAL_CODEC_FIFO_CTRL_AE_LEVEL_SHIFT		2
#define HAL_CODEC_FIFO_CTRL_AE_LEVEL_MASK		(0x1f << HAL_CODEC_FIFO_CTRL_AE_LEVEL_SHIFT)
#define HAL_CODEC_FIFO_CTRL_AF_LEVEL_SHIFT		7
#define HAL_CODEC_FIFO_CTRL_AF_LEVEL_MASK		(0x1f << HAL_CODEC_FIFO_CTRL_AF_LEVEL_SHIFT)

#define HAL_CODEC_FIFO_DEPTH					(16)

typedef enum {
	HAL_CODEC_FIFO_DAC = 0,
	HAL_CODEC_FIFO_ADC12,
	HAL_CODEC_FIFO_ADC34,
	HAL_CODEC_FIFO_ADC56,
	HAL_CODEC_FIFO_ADC78,
	HAL_CODEC_FIFO_ADC9A,
	HAL_CODEC_FIFO_NUM
} hal_codec_fifo_chan_t;

typedef struct {
	__IO uint32_t status;
	__IO uint32_t int_mask;
	__IO uint32_t ctrl;
	__I	 uint32_t reserved;
} hal_codec_fifo_reg_t;

typedef enum {
	HAL_CODEC_INT_ERROR = 0,
	HAL_CODEC_INT_FULL,
	HAL_CODEC_INT_AF,
	HAL_CODEC_INT_HF,
	HAL_CODEC_INT_AE,
	HAL_CODEC_INT_EMPTY,
	HAL_CODEC_INT_WS,
	HAL_CODEC_INT_NUM
} hal_codec_intr_type_t;
	
typedef struct {
	hal_codec_fifo_reg_t fifo[HAL_CODEC_FIFO_NUM];
	__IO uint32_t bist0_ctrl;
	__I  uint32_t bist0_sts;
	__IO uint32_t bist1_ctrl;
	__I  uint32_t bist1_sts;
	__I  uint32_t resved[3];
	__IO uint32_t adc_data_mode;
} hal_codec_fifo_t;

void hal_codec_fifo_set_ae_level(hal_codec_fifo_chan_t chan, uint8_t ae_level);
void hal_codec_fifo_set_af_level(hal_codec_fifo_chan_t chan, uint8_t ae_level);
void hal_codec_fifo_init(hal_codec_fifo_chan_t chan, uint8_t levle);
bool hal_codec_fifo_is_almost_empty(hal_codec_fifo_chan_t chan);
uint32_t hal_codec_fifo_get_word_count(hal_codec_fifo_chan_t chan);
void hal_codec_mask_interrupt(hal_codec_fifo_chan_t chan, hal_codec_intr_type_t intr);
void hal_codec_unmask_interrupt(hal_codec_fifo_chan_t chan, hal_codec_intr_type_t intr);
void hal_codec_clear_interrupt(hal_codec_fifo_chan_t chan, hal_codec_intr_type_t intr);
uint32_t hal_codec_get_interrupt_status(hal_codec_fifo_chan_t chan);
void hal_codec_set_adc_data_mode(uint32_t mode);
uint32_t hal_codec_get_adc_data_mode(void);
void hal_codec_set_adc_word_length(uint32_t wordLength);

#ifdef	__cplusplus
}
#endif
#endif /* __HAL_CODEC_FIFO_H__ */
