/** 
***********************************************************
 * 
 * @file name	: drv_i2s.h
 * @author		: zhangkun
 * @version		: V01.01
 * @date		: 2022-02-12
 * @brief		: I2S Driver
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
 * and Soundec Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 * 
***********************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_I2S_H__
#define __DRV_I2S_H__

/* Includes ------------------------------------------------------------------*/
#include "hal_i2s.h"
#include "common.h"
#include "audio_hw_if.h"

#if (I2S_ENABLE == ENABLE)

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/


typedef void (*i2s_isr_cb_func)(int32_t *data, uint32_t len);

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#if (AUDIO_MANAGER)

typedef struct{
	bool initialized;
	bool enable;
	auTransmitCompleteCB_t isrCb[2];	// 0:tx,1:rx
	void *rxCbArg;
	void *txCbArg;
	auSlotSize_t bitSlot;
	auSlotSize_t wordSize;
	uint8_t channel;
	uint32_t samplingRate;
}i2sDevInfos_t;

drv_status_t i2s1_init(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t i2s2_init(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t i2s3_init(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t i2s1_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t i2s2_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t i2s3_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t i2s1_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t i2s2_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t i2s3_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t i2s1_start(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t i2s2_start(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t i2s3_start(auDriverConfig_t *capture, auDriverConfig_t *playback);
drv_status_t i2s_register_cb(uint8_t i2s_index, auTransmitCompleteCB_t rx_func, auTransmitCompleteCB_t tx_func);
drv_status_t i2s_it_start(uint8_t i2sx, hal_interrupt_irq_t irq_number, auTransmitCompleteCB_t rx_func, auTransmitCompleteCB_t tx_func, hal_interrupt_isr_t isr_func);
#else
drv_status_t i2s_register_cb(uint8_t i2s_index, i2s_isr_cb_func rx_func ,i2s_isr_cb_func tx_func);
drv_status_t i2s_it_start(uint8_t i2sx, hal_interrupt_irq_t irq_number, i2s_isr_cb_func rx_func, i2s_isr_cb_func tx_func, hal_interrupt_isr_t isr_func);
extern uint32_t app_audio_mic_get_data(int32_t *data);
extern void app_i2s1_rx_callback(int32_t *data, uint32_t len);
extern void app_i2s2_rx_callback(int32_t *data, uint32_t len);
extern void app_i2s3_rx_callback(int32_t *data, uint32_t len);
extern void app_i2s1_tx_callback(int32_t *data, uint32_t len);
extern void app_i2s2_tx_callback(int32_t *data, uint32_t len);
extern void app_i2s3_tx_callback(int32_t *data, uint32_t len);
#endif
drv_status_t i2s_set_audio_freq(uint8_t i2sx, uint32_t audioFreq);
drv_status_t i2s_deinit(uint8_t i2sx);
drv_status_t i2s_reinit(uint8_t i2sx);
drv_status_t i2s_init(void);
void i2s1_set_audio_freq(uint32_t audioFreq);
void i2s2_set_audio_freq(uint32_t audioFreq);
void i2s3_set_audio_freq(uint32_t audioFreq);
uint32_t i2s1_get_audio_freq(void);
uint32_t i2s2_get_audio_freq(void);
uint32_t i2s3_get_audio_freq(void);
drv_status_t i2s_enable(uint8_t i2sx, uint8_t enable);
drv_status_t i2s_tx_enable(uint8_t i2sx, uint8_t enable);
drv_status_t i2s_rx_enable(uint8_t i2sx, uint8_t enable);
drv_status_t i2s_set_mode(uint8_t i2sx, uint8_t mode);
drv_status_t i2s_set_format(uint8_t i2sx, uint8_t format);
drv_status_t i2s_set_sample_rate(uint8_t i2sx, uint32_t sampleRate);
drv_status_t i2s_set_word_length(uint8_t i2sx, uint8_t wordLength);
drv_status_t i2s_set_frame_length(uint8_t i2sx, uint8_t frameLength);
drv_status_t i2s_set_mute(uint8_t i2sx, uint8_t mute);
void i2s_dectect_play_state_polling(void);

#ifdef __cplusplus
}
#endif

#endif

#endif  /* __DRV_I2S_H__ */


