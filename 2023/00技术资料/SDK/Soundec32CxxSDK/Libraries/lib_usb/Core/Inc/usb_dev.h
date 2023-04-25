/**
  ******************************************************************************
 *
  * @file    usb_dev.h
  * @brief   Header file for usbd_core.c file
 * @version		: Vxx.xx
 * @date		: 2020-07-09
 * @brief		:
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DEV_CORE_H
#define __USB_DEV_CORE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usb_def.h"

#if 1//(USB_ENABLE)

usb_dev_status_t usb_dev_init(usb_device_t *pdev, usb_dev_get_desc_t *pdesc);
usb_dev_status_t usb_dev_deinit(usb_device_t *pdev);
usb_dev_status_t usb_dev_start (usb_device_t *pdev);
usb_dev_status_t usb_dev_stop  (usb_device_t *pdev);
usb_dev_status_t usb_dev_register_class(usb_device_t *pdev, usb_dev_class_cb_t *pclass);

usb_dev_status_t usb_dev_run_testmode (usb_device_t  *pdev);
usb_dev_status_t usb_dev_set_class_cfg(usb_device_t  *pdev, uint8_t cfgidx);
usb_dev_status_t usb_dev_clr_class_cfg(usb_device_t  *pdev, uint8_t cfgidx);

usb_dev_status_t usb_dev_setup_stage(usb_device_t *pdev, uint8_t *psetup, uint8_t epnum);
usb_dev_status_t usb_dev_data_out_stage(usb_device_t *pdev, uint8_t epnum, uint8_t *pdata);
usb_dev_status_t usb_dev_data_in_stage(usb_device_t *pdev, uint8_t epnum, uint8_t *pdata);
usb_dev_status_t usb_dev_resume(usb_device_t  *pdev);

usb_dev_status_t usb_dev_enter_hibernation(void);
usb_dev_status_t usb_dev_hib_restore_common(void);

#endif
#ifdef __cplusplus
}
#endif

#endif /* __USB_DEV_CORE_H */



