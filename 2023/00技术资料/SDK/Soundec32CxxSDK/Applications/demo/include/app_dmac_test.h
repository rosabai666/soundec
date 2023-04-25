/**
***********************************************************
 *
 * @file name	: app_dmac_test.h
 * @author		: RandyFan
 * @version		: V02.02.demo
 * @date		: 2022-01-04
 * @brief		: DMAC test header file
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
 * and Soundec Co., Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/

#ifndef __APP_DMAC_TEST_H__
#define __APP_DMAC_TEST_H__

#include "snc8600.h"
#include "boardConfig.h"
#include "audio_fifo_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

#if(DMA_ENABLE)
#if(DMA_TEST_MODE_ENABLE)
#define APP_DMAC_TEST_NONE			0
#define APP_DMAC_TEST_MEMORY_TO_MEMORY		1
#define APP_DMAC_TEST_MEMORY_TO_I2S		2
#define APP_DMAC_TEST_MEMORY_TO_ADC		3
#define APP_DMAC_TEST_MEMORY_TO_UART	4
#if(DMA_TEST_I2S_USB_ENABLE)
#define APP_DMAC_TEST_TYPE		(APP_DMAC_TEST_MEMORY_TO_I2S)
#else
#if(DMA_TEST_CODEC_ADC_ENABLE)
#define APP_DMAC_TEST_TYPE		(APP_DMAC_TEST_MEMORY_TO_ADC)
#else
#define APP_DMAC_TEST_TYPE		(APP_DMAC_TEST_MEMORY_TO_MEMORY)
#endif
#endif

#if(APP_DMAC_TEST_TYPE == APP_DMAC_TEST_MEMORY_TO_I2S)
#define APP_DMAC_TEST_I2S_SAMPLE_RATE	(192000U)
#define APP_DMAC_TEST_I2S_USB_BLOCK		(96U)
#endif

void app_dmac_test_main(void);
#if(DMA_TEST_I2S_USB_ENABLE)
void codec_cb_get_data_fr_dma(int32_t* __restrict data);
extern uint8_t DmacI2sUsbTxChx;
extern uint8_t DmacI2sUsbRxChx;
extern bool DmacI2sUsbStart;
#endif	//DMA_TEST_I2S_USB_ENABLE

#endif	//DMA_TEST_MODE_ENABLE

#if(AUDIO_MANAGER)
void fifo_dmac_register_channel(fifo_handle_t *p_fifo, dmac_channel_num_t chx, fifo_dmac_param_t dmacParam, fifo_dmac_callback_t callback);
void dmac_adc_get_and_push_data(uint8_t chx);
#endif	//AUDIO_MANAGER


#endif	//DMA_ENABLE

#ifdef __cplusplus
}
#endif

#endif /* __APP_DMAC_TEST_H__ */
