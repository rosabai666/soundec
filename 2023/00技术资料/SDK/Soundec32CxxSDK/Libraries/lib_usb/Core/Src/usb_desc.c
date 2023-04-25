/**
  ******************************************************************************
 *
  * @file    Src/usbd_desc.c
  * @brief   This file provides the USBD descriptors and string formating method.
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


#include "usb_dev.h"
#include "usb_desc.h"
#include "usbd_config.h"
#include "boardConfig.h"

#if (USB_ENABLE)

#define USB_VID                      	0x3327

#define USB_MANUFACTURER_STRING      	"Soundec"

#define USB_CONFIGURATION_HS_STRING  	"AUDIO Config"
#define USB_INTERFACE_HS_STRING      	"AUDIO Interface"
#define USB_CONFIGURATION_FS_STRING  	"AUDIO Config"
#define USB_INTERFACE_FS_STRING      	"AUDIO Interface"

#define USB_DEV_BOS_STRING               	""
#define USB_DEV_OTHER_STRING                "usb headset"
#define USB_DEV_HEADPHONE_STRING            "Headphone"
#define USB_DEV_MICROPHONE_STRING           "Microphone"

#define USB_LANGID_STRING            	0x409
#define USB_SOFT_VERSION				CONFIG_PRODUCT_VERSION

#if(SUPPORT_USB_USR_STR)
#define USB_USR_STR						"User String"
#endif

//#pragma data_alignment=4

/* USB Standard Device Descriptor for uac 20*/
static uint8_t UsbDeviceDescUac[USB_LEN_DEV_DESC] __attribute__ ((aligned (4))) =
{
    0x12,                       /* bLength */
    DESC_TYPE_DEV,       /* bDescriptorType */
    0x01,                       /* bcdUSB */
    0x02,                       /* changed to USB version 2.01 in order to support LPM L1 suspend resume test of USBCV3.0*/
    0x00,                       /* bDeviceClass */
    0x00,                       /* bDeviceSubClass */
    0x00,                       /* bDeviceProtocol */
    USB_MAX_EP0_SIZE,           /* bMaxPacketSize */
    LOBYTE(USB_VID),            /* idVendor */
    HIBYTE(USB_VID),            /* idVendor */
    LOBYTE(USB_PID),            /* idVendor */
    HIBYTE(USB_PID),            /* idVendor */
    LOBYTE(USB_SOFT_VERSION),   /* bcdDevice rel. 2.00 */
    HIBYTE(USB_SOFT_VERSION),
    USB_DEV_IDX_MFC_STR,           /* Index of manufacturer string */
    USB_DEV_IDX_PRODUCT_STR,       /* Index of product string */
    USB_DEV_IDX_SERIAL_STR,        /* Index of serial number string */
    USB_MAX_DEV_NUM_CONFIGURATION  /* bNumConfigurations */
}; /* USB_DeviceDescriptor */

/* USB Standard Device Descriptor */
static uint8_t UsbDeviceLangIDDesc[USB_LEN_LANGID_STR_DESC] __attribute__ ((aligned (4))) =
{
    USB_LEN_LANGID_STR_DESC,
    DESC_TYPE_STRING,
    LOBYTE(USB_LANGID_STRING),
    HIBYTE(USB_LANGID_STRING),
};

static uint8_t UsbDeviceStringSerial[USB_SIZ_STRING_SERIAL] __attribute__ ((aligned (4))) =
{
    USB_SIZ_STRING_SERIAL,
    DESC_TYPE_STRING,
};

static uint8_t UsbDeviceStringDesc[USB_DEV_MAX_STR_DESC_SIZ] __attribute__ ((aligned (4)));

/**
  * @brief  Convert Hex 32Bits value into char
  * @param  value: value to convert
  * @param  pbuf: pointer to the buffer
  * @param  len: buffer length
  * @retval None
  */
static void int_to_unicode (uint32_t value, uint8_t *pbuf, uint8_t len)
{
    uint8_t idx = 0;

    for( idx = 0 ; idx < len ; idx ++)
    {
        if( ((value >> 28)) < 0xA )
        {
            pbuf[ 2* idx] = (value >> 28) + '0';
        }
        else
        {
            pbuf[2* idx] = (value >> 28) + 'A' - 10;
        }

        value = value << 4;

        pbuf[ 2* idx + 1] = 0;
    }
}

/**
  * @brief  Create the serial number string descriptor
  * @param  None
  * @retval None
  */
static void get_serial_num(void)
{
    uint32_t deviceserial0 = 0x12340000, deviceserial1 = 0xABCDEF00, deviceserial2 = 0x00005678;
#if 0
    deviceserial0 = get_param_usb_sn();
    deviceserial1 = (get_param_usb_sn()&0xffff)*((get_param_usb_sn()>>16)&0xffff);
    deviceserial2 = (get_param_usb_sn()>>1);
#endif

    deviceserial0 += deviceserial2;

    if (deviceserial0 != 0)
    {
        int_to_unicode (deviceserial0, (uint8_t*)&UsbDeviceStringSerial[2],8);
        int_to_unicode (deviceserial1, (uint8_t*)&UsbDeviceStringSerial[18],4);
    }
}

/**
  * @brief  Returns the device descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *usb_device_descriptor(uint8_t speed, uint16_t *length)
{
    /* Prevent unused argument(s) compilation warning */
//    UNUSED(speed);
	if(is_usb_dev_UAC20 == UAC_20)
		UsbDeviceDescUac[3] = 0x02;
	else
		UsbDeviceDescUac[3] = 0x01;

    *length = sizeof(UsbDeviceDescUac);
    return (uint8_t*)UsbDeviceDescUac;
}

/**
  * @brief  Returns the LangID string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *usb_lang_id_str_descriptor(uint8_t speed, uint16_t *length)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(speed);

    *length = sizeof(UsbDeviceLangIDDesc);
    return (uint8_t*)UsbDeviceLangIDDesc;
}

static uint8_t usb_get_len(uint8_t *buf)
{
    uint8_t  len = 0U;

    while (*buf != '\0')
    {
        len++;
        buf++;
    }
    return len;
}

/**
  * @brief  usb_get_string
  *         Convert Ascii string into unicode one
  * @param  desc : descriptor buffer
  * @param  unicode : Formatted string buffer (unicode)
  * @param  len : descriptor length
  * @retval None
  */
static void usb_get_string(uint8_t *desc, uint8_t *unicode, uint16_t *len)
{
    uint8_t idx = 0U;

    if (desc != NULL)
    {
        *len = (uint16_t)usb_get_len(desc) * 2U + 2U;
        unicode[idx++] = *(uint8_t *)(void *)len;
        unicode[idx++] = DESC_TYPE_STRING;

        while (*desc != '\0')
        {
            unicode[idx++] = *desc++;
            unicode[idx++] =  0U;
        }
    }
}

/**
  * @brief  Returns the product string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *usb_product_str_descriptor(uint8_t speed, uint16_t *length)
{
    usb_get_string((uint8_t *)USB_PRODUCT_STRING, UsbDeviceStringDesc, length);
    return UsbDeviceStringDesc;
}

/**
  * @brief  Returns the manufacturer string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *usb_manufacturer_str_descriptor(uint8_t speed, uint16_t *length)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(speed);

    usb_get_string((uint8_t *)USB_MANUFACTURER_STRING, UsbDeviceStringDesc, length);
    return UsbDeviceStringDesc;
}

/**
  * @brief  Returns the serial number string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *usb_serial_str_descriptor(uint8_t speed, uint16_t *length)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(speed);

    *length = USB_SIZ_STRING_SERIAL;

    /* Update the serial number string descriptor with the data from the unique ID*/
    get_serial_num();

    return (uint8_t*)UsbDeviceStringSerial;
}

/**
  * @brief  Returns the configuration string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *usb_config_str_descriptor(uint8_t speed, uint16_t *length)
{
    if(speed == USB_SPEED_HIGH)
    {
        usb_get_string((uint8_t *)USB_CONFIGURATION_HS_STRING, UsbDeviceStringDesc, length);
    }
    else
    {
        usb_get_string((uint8_t *)USB_CONFIGURATION_FS_STRING, UsbDeviceStringDesc, length);
    }
    return UsbDeviceStringDesc;
}

/**
  * @brief  Returns the interface string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *usb_interface_str_descriptor(uint8_t speed, uint16_t *length)
{
    if(speed == USB_SPEED_HIGH)
    {
        usb_get_string((uint8_t *)USB_INTERFACE_HS_STRING, UsbDeviceStringDesc, length);
    }
    else
    {
        usb_get_string((uint8_t *)USB_INTERFACE_FS_STRING, UsbDeviceStringDesc, length);
    }
    return UsbDeviceStringDesc;
}

uint8_t *usb_headphone_str_descriptor(uint8_t speed, uint16_t *length)
{
    usb_get_string((uint8_t *)USB_DEV_HEADPHONE_STRING, UsbDeviceStringDesc, length);
    return UsbDeviceStringDesc;
}

uint8_t *usb_microphone_str_descriptor(uint8_t speed, uint16_t *length)
{
    usb_get_string((uint8_t *)USB_DEV_MICROPHONE_STRING, UsbDeviceStringDesc, length);
    return UsbDeviceStringDesc;
}

uint8_t *usb_profile_str_descriptor(uint8_t speed, uint16_t *length)
{
    usb_get_string((uint8_t *)USB_DEV_OTHER_STRING, UsbDeviceStringDesc, length);
    return UsbDeviceStringDesc;
}

#if (USB_DEV_LPM_ENABLED == 1)
/*{0x5,0xf,0xc,0x0,0x1,0x7,0x10,0x2,0x0,0x0,0x0,0x0};*/

/* BOS descriptor */
static uint8_t UsbDeviceBOSDesc[12] __attribute__ ((aligned (4))) =
{
    0x5,
    DESC_TYPE_BOS,
    0xC,
    0x0,
    0x1,  /* 1 device capability */
    /* device capability*/
    0x7,
    USB_DEVICE_CAPABITY_TYPE,
    0x2,
    0x0, /*LPM capability bit set */
    0x0,
    0x0,
    0x0
};

/**
  * @brief  Returns the BOS descriptor.
  *         return the BOS descriptor
  * @param  speed : current device speed
  * @param  length : pointer to data length variable
  * @retval pointer to descriptor buffer
  */
uint8_t *usb_bos_descriptor(uint8_t speed, uint16_t *length)
{
    *length = sizeof(UsbDeviceBOSDesc);
    return (uint8_t*)UsbDeviceBOSDesc;
}
#endif

#if (USB_DEV_SUPPORT_USER_STRING == 1U)
uint8_t *usb_usr_str_descriptor(uint8_t speed, uint8_t idx, uint16_t *length)
{
#if(SUPPORT_USB_USR_STR)
	switch(idx)
	{
	case USB_USR_STR_IDX:
		usb_get_string((uint8_t *)USB_USR_STR, UsbDeviceStringDesc, length);
		uart_printf("ok\n");
		break;
	default:
		break;
	}
#endif
	return UsbDeviceStringDesc;
}
#endif

usb_dev_get_desc_t gUsbDescFun =
{
    usb_device_descriptor,
    usb_lang_id_str_descriptor,
    usb_manufacturer_str_descriptor,
    usb_product_str_descriptor,
    usb_serial_str_descriptor,
	usb_config_str_descriptor,
    usb_interface_str_descriptor,
    usb_profile_str_descriptor,
	usb_headphone_str_descriptor,
	usb_microphone_str_descriptor,
    #if (USB_DEV_LPM_ENABLED == 1U)
	usb_bos_descriptor,
    #endif
	#if (USB_DEV_SUPPORT_USER_STRING == 1U)
	usb_usr_str_descriptor,
	#endif
};

#endif
