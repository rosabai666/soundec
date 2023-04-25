/**
***********************************************************
 *
 * @file name	: uac.h
 * @author		: Nick
 * @version		: Vxx.xx
 * @date		: 2020-07-09
 * @brief		: usbd audio if
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
#ifndef __USB_AUDIO_H
#define __USB_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include  "usb_req.h"
#include  "usbd_config.h"
#include  "common.h"
#include  "boardConfig.h"

#define USBSPKER       0
#define USBMIC         1

/* Frequency */
#ifndef UAC_FREQ_DEFAULT
#if(SUPPORT_USB_SPK)
#define UAC_FREQ_DEFAULT       (SPK_DEFAULT_FREQ)
#else
#define UAC_FREQ_DEFAULT       (48000)
#endif
#endif

#ifndef UAC_FREQ_MAX
#define UAC_FREQ_MAX           96000U
#endif

/* See UAC Spec 1.0 p.77 */
#define USB_DEV_SPK_VOL_MAX          0x0000U
#define USB_DEV_SPK_VOL_MIN          0xC400U
#define USB_DEV_SPK_VOL_STEP         0x0080U
#define USB_DEV_SPK_VOL_DEF          0x0000U

#define SPKER_LEFT_FRORNT                            1
#define SPKER_RIGHT_FRONT                            2

#define USB_DEV_MIC_VOL_MAX                        0x1600U
#define USB_DEV_MIC_VOL_MIN                        0xDE80U //0xDB00U
#define USB_DEV_MIC_VOL_STEP                       0x0080U
#define USB_DEV_MIC_VOL_DEF                        0x1600U

#define USB_DEV_SLIDETONE_VOL_MAX                  0x0C00U
#define USB_DEV_SLIDETONE_VOL_MIN                  0xDD80U
#define USB_DEV_SLIDETONE_VOL_STEP                 0x0019U
#define USB_DEV_SLIDETONE_VOL_DEF                  0x0C00U



/* Input endpoint is for feedback. See USB 1.1 Spec, 5.10.4.2 Feedback. */
#define AUDIO_IN_PACKET                               3U

/* Number of sub-packets in the audio transfer buffer. You can modify this value but always make sure
  that it is an even number and higher than 3 */
#define AUDIO_OUT_PACKET_NUM                          8U

/* Total size of the audio transfer buffer */
#define AUDIO_TOTAL_BUF_SIZE                          ((uint16_t)((UAC_FREQ_MAX / 1000U + 1) * 2U * 4U * AUDIO_OUT_PACKET_NUM))

#if (USB_ENABLE)
typedef struct
{
    uint8_t cmd;                    /* bRequest */
    uint8_t req_type;               /* bmRequest */
    uint8_t cs;                     /* wValue (high byte): Control Selector */
    uint8_t cn;                     /* wValue (low byte): Control Number */
    uint8_t unit;                   /* wIndex: Feature Unit ID, Extension Unit ID, or Interface, Endpoint */
    uint8_t len;                    /* wLength */
    uint8_t data[USB_MAX_EP0_SIZE]; /* Data */
}uac_ctrl_t;

typedef struct
{
    uint32_t	  freq;
    uint8_t       dwidth;
    uint32_t      curPacket_Size;
    bool          trans_flag;
}uac_info_t;


typedef struct
{
    uint32_t     alt_setting;
    uint32_t     freq;
    uint32_t     bit_depth;
    int16_t      vol;
    uint8_t      mute;
    int16_t      mixer_vol;
    uint16_t     intfNum;
    uac_ctrl_t   control;
}uac_handle_t;


typedef struct
{
    int8_t  (*Init)         (uint32_t  AudioFreq, uint32_t Volume, uint32_t options);
    int8_t  (*DeInit)       (uint32_t options);
    int8_t  (*AudioCmd)     (uint16_t* pbuf, uint32_t size, uint8_t cmd);
    int8_t  (*VolumeCtl)    (uint8_t vol, uint8_t index,uint8_t ch);
    int8_t  (*MuteCtl)      (uint8_t cmd, uint8_t index);
    int8_t  (*PeriodicTC)   (uint8_t cmd);
    int8_t  (*GetState)     (void);
}usb_audio_interface_t;

#if (AUDIO_MANAGER)
typedef struct{
	bool 										initialized;
	bool 										enable;
	auStrmFormat_t 								format;
	uint8_t										role;
	uint8_t										vol;
	uint8_t										mute;
	auTransmitCompleteCB_t 						isrCb;						// 0:tx,1:rx
	auUpdateInfoCB_t							updateInfoCb;
	void 										*streamHandler;
	uint8_t										*buff;
	uint32_t									len;
	
}usbDevInfos_t;
#endif

uint8_t uac_register_interface  (usb_device_t   *pdev, usb_audio_interface_t *fops);
uint8_t uac_init(usb_device_t* pdev, uint8_t cfgidx);
uint8_t uac_de_init(usb_device_t* pdev, uint8_t cfgidx);
uint8_t uac_setup(usb_device_t* pdev, usb_setup_req_t* req);
uint8_t* uac_get_cfg_desc(uint16_t* length);
uint8_t* uac_get_device_qualifier_desc(uint16_t* length);
uint8_t uac_feedback(usb_device_t* pdev, uint8_t epnum);
uint8_t uac_data_out(usb_device_t* pdev, uint8_t epnum);
uint8_t uac_ep0_rx_ready(usb_device_t* pdev);
uint8_t uac_ep0_tx_ready(usb_device_t* pdev);
uint8_t uac_sof(usb_device_t* pdev);
uint8_t uac_iso_in_incomplete(usb_device_t* pdev, uint8_t epnum);
uint8_t uac_iso_out_incomplete(usb_device_t* pdev, uint8_t epnum);
uint8_t uac_mic_in(usb_device_t* pdev,uint8_t epnum);
uint32_t usb_dev_get_speaker_audio_sample(void);
uint32_t usb_dev_get_mic_audio_sample(void);
void usb_dev_dectect_play_state_polling(void);

#endif

#ifdef __cplusplus
}
#endif

#endif  /* __USB_AUDIO_H */

