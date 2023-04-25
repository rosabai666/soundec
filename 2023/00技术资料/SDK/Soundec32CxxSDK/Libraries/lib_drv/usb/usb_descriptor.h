/**
***********************************************************
 *
 * @file name	: usb_descriptor.h
 * @author	: nick
 * @version	: Vxx.xx
 * @date	: 2020-09-22
 * @brief	: usbd audio Configuration Descriptor
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
#ifndef __USBDDESCRIPTOR_H__
#define __USBDDESCRIPTOR_H__

/* Includes ------------------------------------------------------------------*/
#include "uac.h"
#include "usb_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (USB_ENABLE)
#define HID_KEYBOARD_DESC_SIZE    68 + 15 + 15
#define HID_COMMP_DESC_SIZE   (50)
#define DESC_SIZE_HID_KEYBOARD_REPORT (sizeof(g_UsbDeviceHidKeyboardReportDescriptor))

extern uint8_t g_UsbDeviceHidKeyboardReportDescriptor[HID_KEYBOARD_DESC_SIZE];
extern uint8_t g_UsbDeviceHidCommPReportDescriptor[HID_COMMP_DESC_SIZE];
extern uint8_t uac_CfgDesc[USB_AUDIO_CONFIG_DESC_SIZ];
#if(TEST_USB_SWITCH || SUPPORT_UAC_SELF_ADAPTION)
extern uint8_t uac_fullCfgDesc[USB_FS_AUDIO_CONFIG_DESC_SIZ];
#endif
extern uint8_t uac_OtherSpeed_CfgDesc[USB_AUDIO_OTHERSPEED_CONFIG_DESC_SIZ];
extern uint8_t uac_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC];

#if(SUPPORT_UAC20)
#define GET_RANGE_SPKER_FUATURE_LEN    8
#define GET_RANGE_MIC_FUATURE_LEN    8
#define GET_RANGE_MIXER_FUATURE_LEN    8
extern uint8_t uac20_CfgDesc[USB_AUDIO20_CONFIG_DESC_SIZ];
extern uint8_t usbd_audio_spker_sample_rate_default[];
extern uint8_t usbd_audio_spker_sample_rate_list[];
extern uint8_t usbd_audio_mic_sample_rate_default[];
extern  uint8_t usbd_audio_mic_sample_rate_list[];
extern  uint8_t get_spker_range_feature_unitid[GET_RANGE_SPKER_FUATURE_LEN];
extern  uint8_t get_mic_range_feature_unitid[GET_RANGE_MIC_FUATURE_LEN];
extern  uint8_t get_mixer_range_feature_unitid[GET_RANGE_MIXER_FUATURE_LEN];
#endif // SUPPORT_UAC20

#ifdef __cplusplus
}
#endif

#endif

#endif  /* __USBDDESCRIPTOR_H__ */

