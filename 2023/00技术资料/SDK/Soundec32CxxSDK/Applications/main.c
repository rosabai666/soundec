/**
***********************************************************
 *
 * @file name	: main.c
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2022-02-11
 * @brief		: main.c
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
#include "audio_manager.h"
#include "app_adc_key.h"
#include "task.h"
#include "drv_uart.h"
#include "drv_usb.h"
#include "drv_codec.h"
#include "drv_flash.h"
#include "drv_dmac.h"
#include "drv_i2s.h"
#include "drv_i2c.h"
#if(TEST_I2C_SLAVE)
#include "app_i2c_test.h"
#endif
#include "hal_interrupt.h"
#include "usb_hal.h"
#if UART_RX_INT_ENABLE
#include "uart_parser.h"
#endif
#if (SUPPORT_ALG_2MIC_HEADSET || SUPPORT_ALG_2MIC_MEETING_BOX)
#include "speech.h"
#endif
#ifdef SUPPORT_ALTOTOOL
#include "Altotool.h"
#endif
#if(SUPPORT_RESAMPLER)
#include "resampler.h"
#endif
#if SUPPORT_PLAY_TONE
#include "playTone.h"
#endif

#if (SUPPORT_ALTOTOOL)
#include "altotool.h"
#endif

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
#if SUPPORT_AMP_AW88759
extern void aw87559_init(void);
#endif

extern auDriverConfig_t auDrvCfg_usb_speaker;
extern auDriverConfig_t auDrvCfg_usb_mic;
void peripheral_init(void)
{
#if (UART_ENABLE)
	uart_init();
#if UART_RX_INT_ENABLE
	uart_rx_int_init();
#endif
#endif

#if (FLASH_ENABLE)
	flash_init();
#if(FLASH_SAVE_BANK_ENABLE)
	user_flash_init();
#endif
#if FLASH_WRITE_PROTECT_ENABLE
	user_flash_check_start_flag();
#endif
#endif

#if SUPPORT_AMP_AW88759
	aw87559_init();
#endif

#if (SUPPORT_ALTOTOOL)
	altotool_profile_param_init();
#endif

#if (APP_ADC_KEY_ENABLE)
	key_init();
#endif


#if(DMA_ENABLE)
	dmac_init();
#endif

#if (I2C_ENABLE)
	i2c_init();

#if(TEST_I2C_SLAVE)
	app_i2c_test_slave();
#endif
#endif
#if SUPPORT_PLAY_TONE
	PlayToneInit();
#endif
#if (CODEC_ENABLE)
	codec_init();
#endif

#if(AUDIO_MANAGER)
#if (SUPPORT_ALG_2MIC_HEADSET || SUPPORT_ALG_2MIC_MEETING_BOX)
	FrtEndProcInit();
#endif

#if(SUPPORT_RESAMPLER)
	rsr_init();
#endif

	audioManager_init();
	auManager_open_session(AUDIO_SESSION_0);
	auManager_open_session(AUDIO_SESSION_1);
 #if (D_CONFIG_2MIC_BT_HEADPHONE)
	usb_init(&auDrvCfg_usb_speaker,&auDrvCfg_usb_mic);
#endif
#else
 #if (USB_ENABLE)
	altochip_usb_init(USB_SPEED_HIGH, UAC_20);
#endif

#endif

}

void show_version(void)
{
	uart_printf("\n------------------------\n");
#if (SUPPORT_ALTOTOOL)
	uart_printf("GUI DSP : V%01x.%01x.%01x \n", ALTOTOOL_PROTOCLOL_VER&0Xff, (ALTOTOOL_PROTOCLOL_VER>>8)&0Xff,ALTOTOOL_PROTOCLOL_VER>>16);
#endif
	uart_printf("HAL lib : V%d.%02d \n", hal_get_lib_version()>>8, hal_get_lib_version()&0xff);
#if (USB_ENABLE)
	uart_printf("USB lib : V%d.%02d \n", usb_get_lib_version()>>8, usb_get_lib_version()&0xff);
#endif
	uart_printf("%s : V%01X.%02X \n", PROJECT_NAME, CONFIG_PRODUCT_VERSION>>12,CONFIG_PRODUCT_VERSION&0x00ff);
	uart_printf("------------------------\n");
}

int main(void)
{
	system_init();
	peripheral_init();
	show_version();

	while(1)
	{
#if CPU_USAGE_DEBUG
		cpu_total_count++;
#endif
		task_process();

#if (SUPPORT_ALTOTOOL)
		altotool_polling();
#endif

#if (!SUPPORT_ALTOTOOL && !CPU_USAGE_DEBUG)
		XT_WAITI(0);
#endif
	}
	return 1;
}


