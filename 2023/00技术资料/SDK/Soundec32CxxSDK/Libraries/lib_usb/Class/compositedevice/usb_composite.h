/**
***********************************************************
 *
 * @file name	: usb_composite.h
 * @author	: nick
 * @version	: Vxx.xx
 * @date		: 2020-12-07
 * @brief		: USBDevice Composite
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
#ifndef __USB_DEV_COMPOSITE_H__
#define __USB_DEV_COMPOSITE_H__

#include "usb_def.h"

#ifdef __cplusplus
extern "C" {
#endif
#if (USB_ENABLE)
extern usb_dev_class_cb_t  usbDevComposite[USB_FUN_MAX_NUM_INTERFACES];
#endif


#ifdef __cplusplus
}
#endif

#endif  /* __USB_DEV_COMPOSITE_H__ */


