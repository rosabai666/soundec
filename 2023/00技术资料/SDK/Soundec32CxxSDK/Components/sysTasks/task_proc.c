/**
***********************************************************
 *
 * @file name	: task_proc.c
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2022-02-17
 * @brief		: Task process here
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
#include <common.h>
#include <stdint.h>
#include "types.h"
#include "app_adc_key.h"
#include "task.h"
#include "drv_uart.h"
#if(AUDIO_MANAGER)
#include "audio_stream.h"
#include "audio_manager.h"
#endif
#include "hal_gpio.h"
#ifdef SUPPORT_ALTOTOOL
#include "Altotool.h"
#endif
#include "i2s_sample_rate_detect.h"
#include "uac.h"
#include "drv_i2s.h"
#include "drv_i2c.h"
#include "drv_codec.h"
#include "usb_hid.h"
#if UART_RX_INT_ENABLE
#include "uart_parser.h"
#endif
#if(TEST_I2C_MASTER)
#include "app_i2c_test.h"
#endif
#if(TEST_USB_SWITCH || SUPPORT_UAC_SELF_ADAPTION)
#include "usb_def.h"
#include "usbphy.h"
#endif
#include "drv_usb.h"
#if(SUPPORT_RESAMPLER)
#include "resampler.h"
extern auManagerHandler_t audioManager;
#endif
#if SUPPORT_PLAY_TONE
#include "playTone.h"
#endif
#if AUDIO_MANAGER
#include "audio_fifo_manager.h"
#endif


/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
#ifdef TEST_AU_MANAGER
extern uint32_t rxLen;
extern uint32_t txLen;
extern auStream_t *rxStream;
extern auStream_t *txStream;
#endif

/* Private functions ---------------------------------------------------------*/
int task_1ms(void)
{
#if (I2S_SAMPLE_RATE_DETECT)
	i2s_sr_detect_process_ms();
#endif
#if (USB_ENABLE)
	usb_dev_dectect_play_state_polling();
#endif
#if (I2S_ENABLE)
//	i2s_dectect_play_state_polling();
#endif
#if (I2S_ENABLE)
	usb_speed_init_error_detect();
#endif
	return 0;
}

int task_10ms(void)
{
//	uart_printf("task_10ms.\n");

#if (APP_ADC_KEY_ENABLE)
	key_polling_10ms();
#endif
#if CPU_USAGE_DEBUG
	cpu_usage_polling();
#endif
#if UART_RX_INT_ENABLE
	uart_rx_data_parser();
#endif
#if SUPPORT_PLAY_TONE
	PlayToneLoadDataProc();
#if (PREEMPTIVE_MODE == 1)
    PlayToneDetectFor2nd();
#endif
#endif
#if(I2C_ENABLE)
	i2c_polling();
#endif

#if (SUPPORT_USB_HID&&SUPPORT_ALTOTOOL)
	usb_dev_hid_protocol_data_in_timer();
#endif
	return 0;
}

int task_100ms(void)
{
//	uart_printf("task_100ms.\n");
#if(FLASH_SAVE_BANK_ENABLE)
	user_flash_polling_100ms();
#endif	//FLASH_ENABLE

#ifdef HANDLE_AO_ADC_KEY_WAKEUP_FROM_USBD_SUSPEND
	do_hid_key_release();
#endif

#if(SUPPORT_RESAMPLER)
	if(rsrArg.flag)
	{
		rsrArg.flag = 0;
		rsr_initParam();

		delay(5);
		auSession_suspend(&audioManager.session[0], 0);
	}
#endif
	return 0;
}

int task_1s(void)
{

//	uart_printf("task_1s\n");
#if (USB_ENABLE)
	usb_insert_detection();
#endif

#if(TEST_I2C_MASTER)
	static uint8_t cnt = 0;
	if(cnt < 2)
		cnt++;
	else
	app_i2c_test_master();
#endif

#ifdef TEST_AU_MANAGER
	uint8_t sessionId = 0;
	for (sessionId = 0; sessionId<AUDIO_SESSION_MAX; sessionId++)
	{
		uart_printf("---Session %d Info---\n", sessionId);
		uint8_t max = 0, cnt = 0;
		auStream_t *stream = NULL;
		max = auSession_get_session(sessionId)->captureNum;
		for (cnt = 0; cnt<max; cnt++)
		{
			stream = auSession_get_session(sessionId)->capture[cnt];
			uart_printf("rxLen = %d, = %d.\n", stream->var._1sLen, stream->var.pushPopSlot);
			uart_printf("cap %s use %sgain, sgainL:%x, againL:%x, dgainL:%x\n", stream->device->name, stream->var.useSwGain?"S":"H", stream->var.swgain[CHANNEL_L], stream->var.again[CHANNEL_L], stream->var.dgain[CHANNEL_L]);
			uart_printf("bslot:%x, wsize:%x, spr:%d\n", stream->format->bitSlot, stream->formatCap->wordSize, stream->format->sampleRateHz);
			stream->var._1sLen = 0;
			stream->var.pushPopSlot = 0;
		}
		max = auSession_get_session(sessionId)->playbackNum;
		for (cnt = 0; cnt<max; cnt++)
		{
			stream = auSession_get_session(sessionId)->playback[cnt];
			uart_printf("txLen = %d, = %d.\n", stream->var._1sLen, stream->var.pushPopSlot);
			uart_printf("play %s use %sgain, sgainL:%x, againL:%x, dgainL:%x\n", stream->device->name, stream->var.useSwGain?"S":"H", stream->var.swgain[CHANNEL_L], stream->var.again[CHANNEL_L], stream->var.dgain[CHANNEL_L]);
			uart_printf("bslot:%x, wsize:%x, spr:%d\n", stream->format->bitSlot, stream->formatCap->wordSize, stream->format->sampleRateHz);
			stream->var._1sLen = 0;
			stream->var.pushPopSlot = 0;
		}
	}
#endif
#if CPU_USAGE_DEBUG
	cpu_usage_print();
#endif

#if AUDIO_MANAGER
	display_fifo_information();
#endif

	return 0;
}


