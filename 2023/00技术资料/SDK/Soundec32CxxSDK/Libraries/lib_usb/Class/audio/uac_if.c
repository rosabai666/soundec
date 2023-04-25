/**
 ******************************************************************************
 *
 * @file    Src/usbd_audio_if.c
 * @brief   USB Device Audio interface file.
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
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "uac_if.h"
//#include "drv_codec.h"
#include "hal_codec.h"

#if (USB_ENABLE)

#if(AUDIO_MANAGER)
#include "audio_manager.h"
extern usbDevInfos_t usbDevInfo[2];	
#endif


/**
 * @brief  Initializes the AUDIO media low layer.
 * @param  AudioFreq: Audio frequency used to play the audio stream.
 * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
 * @param  options: Reserved for future use
 * @retval Result of the operation: USBD_OK if all operations are OK else
 * USBD_FAIL
 */
static int8_t Audio_Init(uint32_t AudioFreq, uint32_t Volume, uint32_t options)
{
#if (AUDIO_MANAGER)
    if(options == AUDIO_CONTROL_REQ_SPKER_VOL)
	{
		usbDevInfo[USBSPKER].updateInfoCb(usbDevInfo[USBSPKER].streamHandler, PLAYBACK_PATH, INFO_VOL_STEREO, &Volume);
    }
    else if(options == AUDIO_CONTROL_REQ_MIC_VOL)
    {
		usbDevInfo[USBMIC].updateInfoCb(usbDevInfo[USBMIC].streamHandler, CAPTURE_PATH, INFO_VOL_STEREO, &Volume);
    }
    else if(options == AUDIO_CONTROL_REQ_SIDETONE_VOL)
    {
		usbDevInfo[USBMIC].updateInfoCb(usbDevInfo[USBMIC].streamHandler, SIDETONE_PATH, INFO_VOL_STEREO, &Volume);
    }
    else
    {
    }
#endif
    return 0;
}

/**
 * @brief  De-Initializes the AUDIO media low layer.
 * @param  options: Reserved for future use
 * @retval Result of the operation: USBD_OK if all operations are OK else
 * USBD_FAIL
 */
static int8_t Audio_DeInit(uint32_t options)
{
    return 0;
}

/**
 * @brief  Handles AUDIO command.
 * @param  pbuf: Pointer to buffer of data to be sent
 * @param  size: Number of data to be sent (in bytes)
 * @param  cmd: Command opcode
 * @retval Result of the operation: USBD_OK if all operations are OK else
 * USBD_FAIL
 */
static int8_t Audio_PlaybackCmd(uint16_t* pbuf, uint32_t size, uint8_t cmd)
{
    return 0;
}

/**
 * @brief  Controls AUDIO Volume.
 * @param  vol: Volume level (0..100)
 * @retval Result of the operation: USBD_OK if all operations are OK else
 * USBD_FAIL
 */
static int8_t Audio_VolumeCtl(uint8_t vol, uint8_t index,uint8_t ch)
{
    //if(vol > 31) vol = 0;
#ifdef CONGIFG_USB_DEV_VOLUME_FIT_USBH
#if (AUDIO_MANAGER)
//    uart_printf("Volume(%d,%d,%d)\r\n", vol, index, ch);
	if(index == AUDIO_CONTROL_REQ_SPKER_VOL) 
	{
		if(ch == SPKER_LEFT_FRORNT){
			/*spker left front gain*/
			usbDevInfo[USBSPKER].updateInfoCb(usbDevInfo[USBSPKER].streamHandler, PLAYBACK_PATH, INFO_VOL_LEFT, &vol);
		}else if(ch == SPKER_RIGHT_FRONT){
			/*spker right front gain*/
			usbDevInfo[USBSPKER].updateInfoCb(usbDevInfo[USBSPKER].streamHandler, PLAYBACK_PATH, INFO_VOL_RIGHT, &vol);
		}else{
			usbDevInfo[USBSPKER].updateInfoCb(usbDevInfo[USBSPKER].streamHandler, PLAYBACK_PATH, INFO_VOL_STEREO, &vol);
		}
	}
	else if(index == AUDIO_CONTROL_REQ_MIC_VOL)
	{
		usbDevInfo[USBMIC].updateInfoCb(usbDevInfo[USBMIC].streamHandler, CAPTURE_PATH, INFO_VOL_STEREO, &vol);
	}
	else if(index == AUDIO_CONTROL_REQ_SIDETONE_VOL)
	{
		usbDevInfo[USBMIC].updateInfoCb(usbDevInfo[USBMIC].streamHandler, SIDETONE_PATH, INFO_VOL_STEREO, &vol);
	}
	else
	{
	}
#else
	set_codec_volume((int8_t)vol,index, ch);
#endif
#endif
    return 0;
}

/**
 * @brief  Controls AUDIO Mute.
 * @param  cmd: Command opcode
 * @retval Result of the operation: USBD_OK if all operations are OK else
 * USBD_FAIL
 */
static int8_t Audio_MuteCtl(uint8_t cmd, uint8_t index)
{
#ifdef CONGIFG_USB_DEV_VOLUME_FIT_USBH
#if (AUDIO_MANAGER)
	if(index == AUDIO_CONTROL_REQ_SPKER_VOL)
	{
		usbDevInfo[USBSPKER].updateInfoCb(usbDevInfo[USBSPKER].streamHandler, PLAYBACK_PATH, INFO_MUTE, &cmd);
	}
	else if(index == AUDIO_CONTROL_REQ_MIC_VOL)
	{
		usbDevInfo[USBMIC].updateInfoCb(usbDevInfo[USBMIC].streamHandler, CAPTURE_PATH, INFO_MUTE, &cmd);
	}
	else if(index == AUDIO_CONTROL_REQ_SIDETONE_VOL)
	{
		// sidetone
		usbDevInfo[USBMIC].updateInfoCb(usbDevInfo[USBMIC].streamHandler, SIDETONE_PATH, INFO_MUTE, &cmd);
	}
	else
	{}
#else
    set_codec_mute(cmd,index);
#endif
#endif
    return 0;
}

/**
 * @brief  Audio_PeriodicTC
 * @param  cmd: Command opcode
 * @retval Result of the operation: USBD_OK if all operations are OK else
 * USBD_FAIL
 */
static int8_t Audio_PeriodicTC(uint8_t cmd)
{
    return 0;
}

/**
 * @brief  Gets AUDIO State.
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else
 * USBD_FAIL
 */
static int8_t Audio_GetState(void)
{
    return 0;
}


usb_audio_interface_t uac_fops =
{
    Audio_Init,
    Audio_DeInit,
    Audio_PlaybackCmd,
    Audio_VolumeCtl,
    Audio_MuteCtl,
    Audio_PeriodicTC,
    Audio_GetState,
};
#endif
