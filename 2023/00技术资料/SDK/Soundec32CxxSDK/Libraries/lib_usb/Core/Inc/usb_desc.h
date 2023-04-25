/**
  ******************************************************************************
 *
 * @file    Inc/usb_desc.h
 * @brief   Header for usbd_desc.c module
 * @version	 : Vxx.xx
 * @date	 : 2020-07-09
 * @brief	 :
 *
  ******************************************************************************
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

#ifndef __USB_DEV_DESC_H
#define __USB_DEV_DESC_H

#include "usb_def.h"

#if (USB_ENABLE)
#ifdef USE_FLASHUID_AS_USB_SN
#define  USB_SIZ_STRING_SERIAL       0x42
#else
#define  USB_SIZ_STRING_SERIAL       0x1A
#endif

#if(SUPPORT_USB_USR_STR)
#define USB_USR_STR_IDX				0xFF
#endif

extern usb_dev_get_desc_t gUsbDescFun;

#endif

#endif /* __USB_DEV_DESC_H */

