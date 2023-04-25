/**
***********************************************************
 *
 * @file name	: drv_usb.h
 * @author		: nick
 * @version		: Vxx.xx
 * @date		: 2021-06-24
 * @brief		: usbd init
 *
***********************************************************
 * @attention
 *
 * Copyright (c) 2021 Soundec Co., Ltd.
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
#ifndef __USB_DEV_INIT_H__
#define __USB_DEV_INIT_H__

/* Includes ------------------------------------------------------------------*/
#include "audio_hw_if.h"
#include "boardConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


#if (USB_ENABLE)
#if(AUDIO_MANAGER)
drv_status_t usb_init(auDriverConfig_t *spkCfg, auDriverConfig_t *micCfg);
drv_status_t usb_reinit(auDriverConfig_t *spkCfg, auDriverConfig_t *micCfg);
drv_status_t usb_deinit(auDriverConfig_t *spkCfg, auDriverConfig_t *micCfg);
auSlotSize_t usb_transform_bitslot(auSlotSize_t bitslot);
#endif
void usb_speed_init_error_detect(void);
int altochip_usb_init(uint8_t speed, uac_spec_t uac);
int altochip_usb_deinit(void);
#endif

void usb_insert_detection(void);

#define UNPLUG_DEVICE_THRES				(3 - 1) //3s
#define DETECT_PC_DELAY					(1 - 1) //1s
#define DETECT_PC_THRES					(2 - 2) //2s
#define UAC_SELF_ADAPTION_THRES			(2 - 1) //2s

typedef enum{
	DETECT_PC_STATUS_UNDETECTED = -2,
	DETECT_PC_STATUS_DETECTING = -1,
	DETECT_PC_STATUS_MOBILE_PHONE = 0,
	DETECT_PC_STATUS_PC = 1,
}detectPcSt_t;

#ifdef __cplusplus
}
#endif

#endif  /* __USB_DEV_INIT_H__ */



