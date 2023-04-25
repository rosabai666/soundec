/**
***********************************************************
 *
 * @file name	: usb_hid.h
 * @author		: Nick
 * @version		: Vxx.xx
 * @date		: 2020-12-10
 * @brief		: usbd hid
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

/* Includes ------------------------------------------------------------------*/
#include  "usb_req.h"
#include  "usbd_config.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_HID_H
#define __USB_HID_H

#ifdef __cplusplus
extern "C" {
#endif

#if (USB_ENABLE)
#define HID_EPIN_ADDR                 AUDIO_HID_IN_EP
#define HID_EPIN_SIZE                 0x04U

#define USB_HID_CONFIG_DESC_SIZ       34U
#define USB_HID_DESC_SIZ              9U
#define HID_MOUSE_REPORT_DESC_SIZE    74U

#define HID_DESCRIPTOR_TYPE           0x21U
#define HID_REPORT_DESC               0x22U

#define HID_REQ_SET_PROTOCOL          0x0BU
#define HID_REQ_GET_PROTOCOL          0x03U

#define HID_REQ_SET_IDLE              0x0AU
#define HID_REQ_GET_IDLE              0x02U

#define HID_REQ_SET_REPORT            0x09U
#define HID_REQ_GET_REPORT            0x01U

typedef enum
{
    HID_IDLE = 0,
    HID_BUSY,
}usb_hid_state_t;


typedef struct
{
    uint32_t             Protocol;
    uint32_t             IdleState;
    uint32_t             AltSetting;
    usb_hid_state_t     state;
}usb_dev_hid_handle_t;

void is_hid_key(uint32_t key);

uint8_t usb_dev_hid_init (usb_device_t *pdev, uint8_t cfgidx);
uint8_t usb_dev_hid_de_init (usb_device_t *pdev,uint8_t cfgidx);
uint8_t usb_dev_hid_data_in (usb_device_t *pdev, uint8_t epnum);
uint8_t usb_dev_hid_data_out (usb_device_t *pdev,uint8_t epnum);
uint8_t usb_dev_hid_ep0_rx_ready(usb_device_t *pdev);
uint8_t usb_dev_hid_send_report (usb_device_t *pdev,uint8_t *report, uint16_t len);
#ifdef HANDLE_AO_ADC_KEY_WAKEUP_FROM_USB_DEV_SUSPEND
void set_always_on_adc_key_status(uint8_t state);
#endif
uint32_t set_hid_key_buffer(uint32_t key);
void do_hid_key_release(void);
void usb_dev_hid_ep_clean_nak(uint8_t ep_enable);
uint8_t usb_dev_hid_protocol_data_in_timer(void);
#endif
#ifdef __cplusplus
}
#endif

#endif
