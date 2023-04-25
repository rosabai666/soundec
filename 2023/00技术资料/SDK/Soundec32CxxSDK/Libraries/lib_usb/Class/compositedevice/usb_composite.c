/**
***********************************************************
 *
 * @file name	: Usbd_composite.c
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

#include"usb_composite.h"
#include "usb_hid.h"
#include "uac.h"

#if (USB_ENABLE)
usb_dev_class_cb_t usbDevComposite[USB_FUN_MAX_NUM_INTERFACES] =
{
    {
        uac_init,
        uac_de_init,
        uac_setup,
        uac_ep0_tx_ready,
        uac_ep0_rx_ready,
        NULL,
        NULL,
        uac_sof,
        NULL,
        NULL,
        uac_get_cfg_desc,
        uac_get_cfg_desc,
        uac_get_cfg_desc,
        uac_get_device_qualifier_desc,
    },

    #if SUPPORT_USB_SPK
    {
        uac_init,
        uac_de_init,
        NULL,
        NULL,
        NULL,
        uac_feedback,
        uac_data_out,
        NULL,
        uac_iso_in_incomplete,
        uac_iso_out_incomplete,
        NULL,
        NULL,
        NULL,
        NULL,
    },
    #endif

    #if SUPPORT_USB_MIC
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        uac_mic_in,
        NULL, /*EP0_TxSent*/
        NULL, /*SOF */
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },

    #endif
    #if SUPPORT_USB_HID
    {
        usb_dev_hid_init,
        usb_dev_hid_de_init,
        NULL,
        NULL, /*EP0_TxSent*/
        usb_dev_hid_ep0_rx_ready,
        usb_dev_hid_data_in,
        usb_dev_hid_data_out,
        NULL, /*SOF */
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    }

    #endif
};
#endif

