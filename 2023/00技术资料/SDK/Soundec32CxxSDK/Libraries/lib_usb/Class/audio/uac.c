/**
***********************************************************
 *
 * @file name	: usbd_audio.c
 * @author	: Nick
 * @version	: Vxx.xx
 * @date	: 2020-07-09
 * @brief	: usbd audio if
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
#include "uac.h"
#include "usb_req.h"
#include "usb_def.h"
#include "usb_hw.h"
#include "usbd_config.h"
#include "usb_hal.h"
#include "hal_interrupt.h"
#include "drv_codec.h"
#include "usb_descriptor.h"
#include "drv_usb.h"
#include "app_adc_key.h"
#include "uac_vol.h"
#if(AUDIO_MANAGER)
#include "audio_manager.h"
#endif
#if(SUPPORT_RESAMPLER)
#include "resampler.h"
extern auManagerHandler_t audioManager;
#endif
#if (SUPPORT_PLAY_TONE && D_USB_PLAY_TONE_ENABLE)
#include "playTone.h"
static void uac_get_tone_data(uint8_t* pMicBuff, uint32_t len);
#endif

uint8_t send_buff[128] __attribute__ ((aligned (4)));
#if (USB_ENABLE)
#if(AUDIO_MANAGER)
extern usbDevInfos_t usbDevInfo[2];
#define USBSPK_HNDL		usbDevInfo[USBSPKER]
#define USBMIC_HNDL		usbDevInfo[USBMIC]
#endif

/*WARNING: First disable app_codec_dac_callback!*/
//#define USB_DATA_IN_CHECK

// clang-format off
#if (SUPPORT_ALG_2MIC_HEADSET || SUPPORT_ALG_2MIC_MEETING_BOX)
#define USB_SPKTXBUF_NUM	8
#else
#if (SPK_FS_192K || MIC_FS_192K)
#define USB_SPKTXBUF_NUM	32
#else
#define USB_SPKTXBUF_NUM	8
#endif
#endif
#define SUPPORT_FREQ_LIST   11
uint32_t frep_list[SUPPORT_FREQ_LIST] = {SAMPLING_RATE_384000, SAMPLING_RATE_352800, SAMPLING_RATE_192000, SAMPLING_RATE_176400, SAMPLING_RATE_96000, SAMPLING_RATE_88200, SAMPLING_RATE_48000, SAMPLING_RATE_44100, SAMPLING_RATE_32000, SAMPLING_RATE_16000, SAMPLING_RATE_8000};

__attribute__((aligned(4))) static struct
{
    uint32_t status;
    uint32_t buff;
}spkTxSts[USB_SPKTXBUF_NUM];

int dmaPtr = 0;
int feedbackTimes = 0;
uac_info_t audio_info[2];

uint8_t sidetone_mute = 0;
uint16_t audio_res[3] = {0};
uint16_t audio_vol[4] = {0};    // spk l spk r mic sidetone
uint8_t spkIsrCount = 0;
bool spkPlaying = true;
uint8_t micIsrCount = 0;
bool micRecording = true;
static uint32_t mltip_len = 576;

static uint8_t	micBuff[1024] __attribute__ ((aligned (4)));
#if ((SPK_FS_192K || SPK_FS_176P4K) &&SPK_AUDIO_24BIT)
#define SPKTXBUFF                  800//1024
#else
#define SPKTXBUFF                  608
#endif
__attribute__((aligned(8))) static uint8_t spkTxBuf[SPKTXBUFF*USB_SPKTXBUF_NUM];


static uint8_t fs44100_handle = 0;

uac_handle_t audioClassData[USB_FUN_NUM_INTERFACES];
#if(AUDIO_MANAGER == 0)
extern volatile uint32_t usb_feedback_data;
#endif

#if(SUPPORT_USB_INSERT_DETECTION)
bool insFlag = 0;
uint32_t insCnt = 0;
#if(SUPPORT_UAC20)
bool setCurFlag = 0;
#endif
#endif


uint32_t srSwCnt = 0;
volatile uint32_t curSr = 48000;
volatile bool srChangeFlag = 0;
volatile uint32_t feedbackVal = 0;
uint32_t feedbackMax = 0;
uint32_t feedbackMin = 0;
volatile uint32_t feedSr = 0;
#ifdef SUPPORT_FEEDBACK_TUNE
volatile int8_t tuneDir = 0;
#endif

//#if(UAC2.0)
//#define FEEDBACK_PACK_SIZE		4
//#else
#define FEEDBACK_PACK_SIZE		3
//#endif

void uac_tune(uint32_t dataLen, uint32_t fifoSize)
{
#ifdef SUPPORT_FEEDBACK_TUNE
	uint32_t sr = 0;
	if(curSr && !srChangeFlag)
	{
		if(tuneDir != -1)
		{
			if(dataLen <= (fifoSize>>3))
			{
				tuneDir = 1;
				uart_printf("!");
			}
			else if(dataLen >= (fifoSize - (fifoSize >> 3)))
			{
				tuneDir = -1;
				uart_printf("~");
			}
			else
				tuneDir = 0;
		}
	}
#endif
}

#if(SUPPORT_USB_FEEDBACK)

void set_feedback_thres(uint32_t sr)
{
//	feedbackMin = sr - sr/8;
//	feedbackMax = sr + sr/4;

	feedbackMin = sr - 3000;
	feedbackMax = sr + 3000;
}
#endif

/**
  * @brief  uac_feedback
  *         handle feedback
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
uint8_t uac_feedback(usb_device_t* pdev, uint8_t epnum)
{
#if(SUPPORT_USB_FEEDBACK)
	static uint32_t sr = 0;
	static uint8_t cnt = 0;
	if(srChangeFlag)
	{
		cnt++;
		if(cnt >= 3)
		{
			cnt = 0;
			srChangeFlag = 0;
			sr = feedSr;
		}

		feedbackVal = 0;

	}
	else
	{
#ifdef SUPPORT_FEEDBACK_TUNE
		static uint8_t oldDir = 0;
		if(tuneDir == oldDir)
			oldDir = 60;
		else
			oldDir = 0;
		if(tuneDir == 1)
		{
			sr += 60;
			sr += oldDir;
		}
		else if(tuneDir == -1)
		{
			sr -= 60;
			sr -= oldDir;
		}

		oldDir = tuneDir;
		tuneDir = 0;
#endif
		feedbackVal = ((sr/1000)<<14) | ((sr%1000)<<4);
	}

#endif
	hal_usb_transmit(pdev, AUDIO_FB_EP, (uint8_t*)&feedbackVal, FEEDBACK_PACK_SIZE);

    return USBD_OK;
}

/**
 * @brief  uac_req_get_current
 *         Handles the GET_CUR Audio control request.
 * @param  pdev: instance
 * @param  req: setup class request
 * @retval status
 */
static void uac_req_get_current(usb_device_t* pdev, usb_setup_req_t* req)
{
    uac_handle_t* haudio;
    haudio = ((uac_handle_t*)pdev->pClassData);
    if (haudio == NULL )
        return;

    if ((req->bmRequest & 0x1f) == AUDIO_CONTROL_REQ)
    {
        switch (HIBYTE(req->wValue))
        {

        case AUDIO_CONTROL_REQ_FU_MUTE:
        {
            uac_handle_t* haudio_ct;
			switch(HIBYTE(req->wIndex))
			{
	            case AUDIO_CONTROL_REQ_SIDETONE_VOL:
	                *(uint8_t*)send_buff = sidetone_mute;
	                break;
				case AUDIO_CONTROL_REQ_MIC_VOL:
	                haudio_ct = ((uac_handle_t*)pdev->pClassData) + INTF_AUDIO_STREAM_MIC ;
	                *(uint8_t*)send_buff = haudio_ct->mute;
					break;
	            case AUDIO_CONTROL_REQ_SPKER_VOL:
				case USB_DEV_SPKER_VOLUME_RANGE_ID:
	                pdev->irqPriority = 1;
	                haudio_ct = ((uac_handle_t*)pdev->pClassData) + INTF_AUDIO_STREAM_SPK ;
	                *(uint8_t*)send_buff = haudio_ct->mute;
					break;
				case AUDIO_CONTROL_REQ_MIXER:
	                *(uint8_t*)send_buff = 0;
					break;
	            case 0x0D:
	                *(uint8_t*)send_buff = sidetone_mute;
	                break;
				default:
				    break;
			}
            /* Current mute state */
            usb_ctl_send_data(pdev, (uint8_t*)send_buff, 1);
        };
            break;
        case AUDIO_CONTROL_REQ_FU_VOL:
        {
            uac_handle_t* haudio_ct;
            switch(HIBYTE(req->wIndex))
            {
	            case AUDIO_CONTROL_REQ_SIDETONE_VOL:
	                *(uint16_t*)send_buff =audio_vol[3];
					break;
	            case AUDIO_CONTROL_REQ_MIC_VOL:
	                haudio_ct = ((uac_handle_t*)pdev->pClassData) + INTF_AUDIO_STREAM_MIC;
	                *(uint16_t*)send_buff = audio_vol[2];
					break;
				case AUDIO_CONTROL_REQ_SPKER_VOL:
				case USB_DEV_SPKER_VOLUME_RANGE_ID:
	                haudio_ct = ((uac_handle_t*)pdev->pClassData) + INTF_AUDIO_STREAM_SPK;
	                if(LOBYTE(req->wValue) == SPKER_LEFT_FRORNT)
	                    *(uint16_t*)send_buff = audio_vol[0];
	                else
	                    *(uint16_t*)send_buff = audio_vol[1];
					break;
	            case AUDIO_CONTROL_REQ_MIXER:
	                *(uint16_t*)send_buff = 0;
					break;
	            case 0x0D:
	            	*(uint16_t*)send_buff = audio_vol[3];
	            	break;
	            default:
					break;
            }

            usb_ctl_send_data(pdev, (uint8_t*)send_buff, 2);
        };
            break;
        case AUDIO_AUTOMATIC_GAIN_CTRL:
        {
            *(uint16_t*)send_buff = 0;
            usb_ctl_send_data(pdev, (uint8_t*)send_buff, 1);
        };
            break;
        case 0x03:
        case 0x04:
        {
            *(uint16_t*)send_buff = 0;
            /* Current volume. See UAC Spec 1.0 p.77 */
            usb_ctl_send_data(pdev, (uint8_t*)send_buff, 2);
        };
            break;
        }
    }
    else if ((req->bmRequest & 0x1f) == AUDIO_STREAMING_REQ)
    {
        if (HIBYTE(req->wValue) == AUDIO_STREAMING_REQ_FREQ_CTRL)
        {
            /* Current frequency */
            uac_handle_t* haudio;
            haudio = ((uac_handle_t*)pdev->pClassData) + ((LOBYTE(req->wIndex) == 1)?1:2);

            uint32_t freq __attribute__((aligned(4))) = haudio->freq;
            *(uint32_t*)send_buff = freq;
            //uart_printf("frep: %d " , haudio->freq);
            usb_ctl_send_data(pdev, (uint8_t*)send_buff, 3);
        }
    }
}

/**
 * @brief  uac_req_get_max
 *         Handles the GET_MAX Audio control request.
 * @param  pdev: instance
 * @param  req: setup class request
 * @retval status
 */
static void uac_req_get_max(usb_device_t* pdev, usb_setup_req_t* req)
{
    if(( pdev == NULL)||(req == NULL))
		return;

    if ((req->bmRequest & 0x1f) == AUDIO_CONTROL_REQ)
    {
        switch (HIBYTE(req->wValue))
        {
        case AUDIO_CONTROL_REQ_FU_VOL:
        {
            int16_t vol_max = 0;
			switch(HIBYTE(req->wIndex))
			{
	            case AUDIO_CONTROL_REQ_SPKER_VOL:
	                if(LOBYTE(req->wValue) == SPKER_LEFT_FRORNT)
	                    vol_max = USB_DEV_SPK_VOL_MAX;
	                else if(LOBYTE(req->wValue) == SPKER_RIGHT_FRONT)
	                    vol_max = USB_DEV_SPK_VOL_MAX;
					break;
	            case AUDIO_CONTROL_REQ_MIC_VOL:
	                vol_max = USB_DEV_MIC_VOL_MAX;
					break;
	            case AUDIO_CONTROL_REQ_SIDETONE_VOL:
	                vol_max = USB_DEV_SLIDETONE_VOL_MAX;
					break;
	            default:
	                break;
			}
            *(uint16_t*)send_buff = vol_max;
            usb_ctl_send_data(pdev, (uint8_t*)send_buff, 2);
        };
            break;
        }
    }
}

/**
 * @brief  uac_req_get_min
 *         Handles the GET_MIN Audio control request.
 * @param  pdev: instance
 * @param  req: setup class request
 * @retval status
 */
static void uac_req_get_min(usb_device_t* pdev, usb_setup_req_t* req)
{
    if(( pdev == NULL)||(req == NULL))
		return;

    if ((req->bmRequest & 0x1f) == AUDIO_CONTROL_REQ)
    {
        switch (HIBYTE(req->wValue))
        {
        case AUDIO_CONTROL_REQ_FU_VOL:
        {
            int16_t vol_min = 0;
            switch(HIBYTE(req->wIndex))
            {
	            case AUDIO_CONTROL_REQ_SPKER_VOL:
	                if(LOBYTE(req->wValue) == SPKER_LEFT_FRORNT)
	                    vol_min = USB_DEV_SPK_VOL_MIN;
	                else if(LOBYTE(req->wValue) == SPKER_RIGHT_FRONT)
	                    vol_min = USB_DEV_SPK_VOL_MIN;
					break;
	            case AUDIO_CONTROL_REQ_MIC_VOL:
	                vol_min = USB_DEV_MIC_VOL_MIN;
					break;
	            case AUDIO_CONTROL_REQ_SIDETONE_VOL:
	                vol_min = USB_DEV_SLIDETONE_VOL_MIN;
					break;
	            default:
					break;
            }

            *(uint16_t*)send_buff = vol_min;
            usb_ctl_send_data(pdev, (uint8_t*)send_buff, 2);
        };
            break;
        }
    }
}

/**
 * @brief  uac_req_get_res
 *         Handles the GET_RES Audio control request.
 * @param  pdev: instance
 * @param  req: setup class request
 * @retval status
 */
static void uac_req_get_res(usb_device_t* pdev, usb_setup_req_t* req)
{
    if(( pdev == NULL)||(req == NULL))
        return;

    if ((req->bmRequest & 0x1f) == AUDIO_CONTROL_REQ)
    {
        switch (HIBYTE(req->wValue))
        {
        case AUDIO_CONTROL_REQ_FU_VOL:
        {
            int16_t vol_res =0;
			switch(HIBYTE(req->wIndex))
			{
	            case AUDIO_CONTROL_REQ_SPKER_VOL:
	            {
	                if(LOBYTE(req->wValue) == SPKER_LEFT_FRORNT)
	                    vol_res = audio_res[1]?audio_res[1] : USB_DEV_SPK_VOL_STEP;
	                else if(LOBYTE(req->wValue) == SPKER_RIGHT_FRONT)
	                    vol_res = audio_res[1]?audio_res[1] : USB_DEV_SPK_VOL_STEP;
	            }
				break;
	            case AUDIO_CONTROL_REQ_MIC_VOL:
	                vol_res = audio_res[2]? audio_res[2] : USB_DEV_MIC_VOL_STEP;
					break;
	            case AUDIO_CONTROL_REQ_SIDETONE_VOL:
	                vol_res = audio_res[0] ? audio_res[0] : USB_DEV_SLIDETONE_VOL_STEP;
					break;
	            default:
					break;
			}
            *(uint16_t*)send_buff = vol_res;
            usb_ctl_send_data(pdev, (uint8_t*)send_buff, 2);
        };
            break;
        }
    }
}


/**
  * @brief  AUDIO_Req_SetCurrent
  *         Handles the SET_CUR Audio control request.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
#if(SUPPORT_UAC20)
static void uac20_req_set_current_l2(usb_device_t* pdev, usb_setup_req_t* req)
{
    uac_handle_t	 *haudio;
    haudio = (uac_handle_t *) pdev->pClassData;

    if (LOBYTE(req->wIndex) == 0x03)
    {
        send_buff[0] = 1;
        send_buff[1] = 0;
        send_buff[2] = 0;
        usb_ctl_send_data(pdev, (uint8_t*)send_buff, 3);
        return;
    }

    else if ((req->bmRequest & 0x1f) == AUDIO_CONTROL_REQ)
    {
        switch (HIBYTE(req->wValue))
        {
        case AUDIO_CONTROL_REQ_FU_MUTE:
        {
            usb_ctl_prepare_rx(pdev, haudio->control.data, req->wLength);

            haudio->control.cmd = AUDIO_REQ_SET_CUR;
            haudio->control.req_type = AUDIO_CONTROL_REQ;
            haudio->control.cs = HIBYTE(req->wValue);
            haudio->control.unit = HIBYTE(req->wIndex);
        };
            break;
        case AUDIO_CONTROL_REQ_FU_VOL:
        {
            usb_ctl_prepare_rx(pdev, haudio->control.data, req->wLength);

            haudio->control.cmd = AUDIO_REQ_SET_CUR;
            haudio->control.req_type = AUDIO_CONTROL_REQ;
            haudio->control.cs =  HIBYTE(req->wValue);
            haudio->control.unit = HIBYTE(req->wIndex);
        };
            break;
        }
    }
    else if ((req->bmRequest & 0x1f) == AUDIO_STREAMING_REQ)
    {
        if (HIBYTE(req->wValue) == AUDIO_STREAMING_REQ_FREQ_CTRL)
        {
            usb_ctl_prepare_rx(pdev, haudio->control.data, req->wLength);

            haudio->control.cmd = AUDIO_REQ_SET_CUR;
            haudio->control.req_type = AUDIO_STREAMING_REQ;
            haudio->control.cs = AUDIO_STREAMING_REQ_FREQ_CTRL;
            haudio->control.unit = HIBYTE(req->wIndex);
        }
    }
    pdev->irqPriority = 1;
    usb_ctl_send_data(pdev, NULL, 0);
}
#endif

static void uac10_req_set_current(usb_device_t* pdev, usb_setup_req_t* req)
{
    if(( pdev == NULL)||(req == NULL)) return ;
    uac_handle_t	 *haudio;
    haudio = (uac_handle_t *) pdev->pClassData;

    if (LOBYTE(req->wIndex) == 0x03)
    {
        send_buff[0] = 1;
        send_buff[1] = 0;
        send_buff[2] = 0;
        usb_ctl_send_data(pdev, (uint8_t*)send_buff, 3);
        return;
    }
    else if ((req->bmRequest & 0x1f) == AUDIO_CONTROL_REQ)
    {
        switch (HIBYTE(req->wValue))
        {
        case AUDIO_CONTROL_REQ_FU_MUTE:
        {
            usb_ctl_prepare_rx(pdev, haudio->control.data, req->wLength);

            haudio->control.cmd = AUDIO_REQ_SET_CUR;
            haudio->control.req_type = AUDIO_CONTROL_REQ;
            haudio->control.cs = AUDIO_CONTROL_REQ_FU_MUTE;
            haudio->control.unit = HIBYTE(req->wIndex);
        };
            break;
        case AUDIO_CONTROL_REQ_FU_VOL:
        {
            usb_ctl_prepare_rx(pdev, haudio->control.data, req->wLength);
            haudio->control.cmd = AUDIO_REQ_SET_CUR;
            haudio->control.req_type = AUDIO_CONTROL_REQ;
            if(req->bRequest == AUDIO_REQ_SET_RES)
			{
                haudio->control.cs = AUDIO_CONTROL_REQ_FU_RES;
            }
			else
			{
                haudio->control.cs = AUDIO_CONTROL_REQ_FU_VOL;
            }
            haudio->control.cn = LOBYTE(req->wValue);
            haudio->control.unit = HIBYTE(req->wIndex);
        };
            break;
        }
    }
    else if ((req->bmRequest & 0x1f) == AUDIO_STREAMING_REQ)
    {

        if (HIBYTE(req->wValue) == AUDIO_STREAMING_REQ_FREQ_CTRL)
        {
            usb_ctl_prepare_rx(pdev, haudio->control.data, req->wLength);

            haudio->control.cmd = AUDIO_REQ_SET_CUR;
            haudio->control.req_type = AUDIO_STREAMING_REQ;
            haudio->control.cs = AUDIO_STREAMING_REQ_FREQ_CTRL;
            haudio->control.unit = LOBYTE(req->wIndex)&0x7 ;

        }
    }
    usb_ctl_send_data(pdev, NULL, 0);
}




/**
 * @brief  Stop playing and reset buffer pointers
 * @param  pdev: instance
 */
static void uac_out_stop_reset(usb_device_t* pdev, uint16_t intf)
{
	curSr = 0;

    uac_handle_t* haudio;
    haudio = ((uac_handle_t*)pdev->pClassData) + intf;
    if(( pdev == NULL)||(haudio == NULL)) return ;

#if (SUPPORT_USB_SPK == 0)
    intf = INTF_AUDIO_STREAM_MIC ;
#endif

    if (intf == INTF_AUDIO_STREAM_SPK)
    {
        hal_usb_flush_ep(pdev, AUDIO_OUT_EP);

        if (pdev->epOut[AUDIO_OUT_EP & 0xFU].isUsed)
        {
            hal_usb_close_ep(pdev, AUDIO_OUT_EP);
            pdev->epOut[AUDIO_OUT_EP & 0xFU].isUsed = 0U;
        }
#if(SUPPORT_USB_FEEDBACK)
        hal_usb_flush_ep(pdev, AUDIO_FB_EP);
        if (pdev->epIn[AUDIO_FB_EP & 0xFU].isUsed)
        {
            hal_usb_close_ep(pdev, AUDIO_FB_EP);
            pdev->epIn[AUDIO_FB_EP & 0xFU].isUsed = 0U;
        }
#endif
    }
    else if (intf == INTF_AUDIO_STREAM_MIC)
    {
        hal_usb_flush_ep(pdev, AUDIO_IN_EP);
        pdev->epIn[AUDIO_IN_EP & 0xFU].isUsed = 0U;
    }

}


/**
 * @brief  Restart playing with new parameters
 * @param  pdev: instance
 */
static void uac_out_restart(usb_device_t* pdev, uint16_t intf)
{
    uint8_t index = intf-INTF_AUDIO_STREAM_SPK;
#if (SUPPORT_USB_SPK == 0)
    intf = INTF_AUDIO_STREAM_MIC ;
#endif

    uac_handle_t* haudio;
    haudio = ((uac_handle_t*)pdev->pClassData) + intf;
    if(( pdev == NULL)||(haudio == NULL)) return ;

    if (is_usb_dev_UAC20 == UAC_20)
	{
		if(audio_info[index].dwidth == 24 )
			audio_info[index].curPacket_Size = audio_info[index].freq*6/8000U;
		else
        	audio_info[index].curPacket_Size = (audio_info[index].dwidth == 16)? (audio_info[index].freq / 2000U):(audio_info[index].freq/1000U);
    }
	else
        audio_info[index].curPacket_Size = (audio_info[index].freq / 1000U) * (audio_info[index].dwidth/8) * 2U ;


    if (intf == INTF_AUDIO_STREAM_SPK)
    {
        audio_info[index].curPacket_Size += (audio_info[index].dwidth/8) * 3 * 2U;
        
#if (AUDIO_MANAGER == 0)
        usb_spk_reset_feedback();
        audio_spk_buf_reset();
#endif
        /* Open EP OUT */
        hal_usb_open_ep(pdev, AUDIO_OUT_EP, USB_DEV_EP_TYPE_ISOC, audio_info[USBSPKER].curPacket_Size);
        pdev->epOut[AUDIO_OUT_EP & 0xFU].isUsed = 1U;
        hal_usb_flush_ep(pdev, AUDIO_OUT_EP);

#if(SUPPORT_USB_FEEDBACK)
        hal_usb_open_ep(pdev, AUDIO_FB_EP, USB_DEV_EP_TYPE_ISOC, AUDIO_IN_PACKET);
        pdev->epIn[AUDIO_FB_EP & 0xFU].isUsed = 1U;
        hal_usb_flush_ep(pdev, AUDIO_FB_EP);

//        feedbackVal = ((USBSPK_HNDL.format.sampleRateHz/1000)<<14) | ((USBSPK_HNDL.format.sampleRateHz%1000)<<4);
        feedbackVal = ((curSr/1000)<<14) | ((curSr%1000)<<4);
        hal_usb_transmit(pdev, AUDIO_FB_EP, (uint8_t*)&feedbackVal, FEEDBACK_PACK_SIZE);
#endif
        hal_usb_ep_receive(pdev, AUDIO_OUT_EP, (uint8_t*)spkTxSts, audio_info[USBSPKER].curPacket_Size, 0);

    }
    else if ((intf == INTF_AUDIO_STREAM_MIC)||(intf == 2))
    {
        //int i;
        audio_info[USBMIC].curPacket_Size = audio_info[USBMIC].curPacket_Size *USB_MIC_CHANNELS / 2;
        uint32_t len = audio_info[USBMIC].curPacket_Size;
        struct scDMA *pDma = &scDMA_EPIN[2];
        fs44100_handle = 0;

        if (is_usb_dev_UAC20 == UAC_20)
		{
            audio_info[USBMIC].trans_flag = TRUE ;
            hal_usb_flush_ep(pdev, AUDIO_IN_EP);
            switch(audio_info[USBMIC].freq)
            {
            case 44100:
            	len = (audio_info[USBMIC].dwidth == 16) ? 24 : 36;
            	len = len*USB_MIC_CHANNELS / 2;
            	break;
            case 88200:
            	len = (audio_info[USBMIC].dwidth == 16) ? 66 : 72;
            	len = len*USB_MIC_CHANNELS / 2;
            	break;
            case 176400:
            	len = (audio_info[USBMIC].dwidth == 16) ? 132 : 138;
            	len = len*USB_MIC_CHANNELS / 2;
            	break;
            default:
            	break;
            }

            if(pdev->epIn[AUDIO_IN_EP & 0xFU].isUsed == 0U)
            {
                hal_usb_open_ep(pdev, AUDIO_IN_EP, USB_DEV_EP_TYPE_ISOC, len + 60);
                pdev->epIn[AUDIO_IN_EP & 0xFU].isUsed = 1U;
                hal_usb_flush_ep(pdev, AUDIO_IN_EP);
            }
        }else{

            hal_usb_flush_ep(pdev, AUDIO_IN_EP);
            if ((haudio->freq == 192000)&&(haudio->bit_depth == 24))
            {
                len = 576;

                hal_usb_open_ep(pdev, AUDIO_IN_EP, USB_DEV_EP_TYPE_ISOC, len+60);
                pdev->epIn[AUDIO_IN_EP & 0xFU].isUsed = 1U;
                hal_usb_flush_ep(pdev, AUDIO_IN_EP);

                pDma->bufferPtr = (uint32_t)micBuff;
                pDma->bufferSts = len;
                pDma->bufferSts |= USB_SCDMA_L;
                pDma->bufferSts |= USB_SCDMA_IOC;
                pDma->bufferSts |= USB_SCDMA_MTRF;
            }

           else if ((haudio->freq == SAMPLING_RATE_176400)&&(haudio->bit_depth == 24))
            {
                len = mltip_len;
				mltip_len = 480;
                hal_usb_open_ep(pdev, AUDIO_IN_EP, USB_DEV_EP_TYPE_ISOC, len+60);
                pdev->epIn[AUDIO_IN_EP & 0xFU].isUsed = 1U;
                hal_usb_flush_ep(pdev, AUDIO_IN_EP);

                pDma->bufferPtr = (uint32_t)micBuff;
                pDma->bufferSts = len;
                pDma->bufferSts |= USB_SCDMA_L;
                pDma->bufferSts |= USB_SCDMA_IOC;
                pDma->bufferSts |= USB_SCDMA_MTRF;
            }
            else
            {
                hal_usb_open_ep(pdev, AUDIO_IN_EP, USB_DEV_EP_TYPE_ISOC, len + 60);
                pdev->epIn[AUDIO_IN_EP & 0xFU].isUsed = 1U;
                hal_usb_flush_ep(pdev, AUDIO_IN_EP);

                pDma->bufferPtr = (uint32_t)micBuff;
                pDma->bufferSts = len;
                pDma->bufferSts |= USB_SCDMA_L;
                pDma->bufferSts |= USB_SCDMA_IOC;
            }
        }
	    uint32_t len_record = len ;


		micIsrCount++;
#if(AUDIO_MANAGER)
		USBMIC_HNDL.format.bitSlot = audio_info[USBMIC].dwidth>>3;
		if (USBMIC_HNDL.enable && USBMIC_HNDL.isrCb)
		{
			memset(micBuff, 0, len);
			USBMIC_HNDL.isrCb(USBMIC_HNDL.streamHandler, micBuff, &len, USBMIC_HNDL.format.bitSlot);
#if (SUPPORT_PLAY_TONE && D_USB_PLAY_TONE_ENABLE)
			uac_get_tone_data(micBuff, len);
#endif

		}
			
#endif

		if (is_usb_dev_UAC20 == UAC_20)
		{
			hal_usb_transmit(pdev, AUDIO_IN_EP, (uint8_t*)micBuff, len);
		}
		else
		{
    		if(len_record != len)
		    {
				pDma->bufferSts = (pDma->bufferSts&0xFFFFF000)|len;
		    }
            hal_usb_transmit(pdev, AUDIO_IN_EP, (uint8_t*)pDma, len);
		}
    }
}

int8_t vol_to_pencentage(int16_t vol, uint8_t index)
{
	int16_t getUsbVol = 0;

	if(index == AUDIO_CONTROL_REQ_SPKER_VOL )
	{
		if(vol == (int16_t)USB_DEV_SPK_VOL_MIN)
		{
			return -63;
		}
	}
	else if(index == AUDIO_CONTROL_REQ_MIC_VOL)
	{
		if(vol == (int16_t)USB_DEV_MIC_VOL_MIN)
		{
			return -128;
		}
	}
	else if(index == AUDIO_CONTROL_REQ_SIDETONE_VOL)
	{
		if(vol == (int16_t)USB_DEV_SLIDETONE_VOL_MIN)
		{
			return -128;
		}
	}
	else if(index == AUDIO_CONTROL_REQ_MIXER)
	{
		getUsbVol = 0;
	}
	else if(index == 0x0D)
	{
		if(vol == (int16_t)USB_DEV_SLIDETONE_VOL_MIN)
		{
			return -128;
		}
	}


	if (vol >= 0)
	{
		vol += 23;
	}
	else
	{
		vol -= 3;
	}

	getUsbVol = vol / 256;
	return (int8_t)getUsbVol;
}

void uac_dma_desc_init()
{
    int i;
    for (i = 0; i < USB_SPKTXBUF_NUM; i++)
    {
        spkTxSts[i].status = USB_SCDMA_IOC|audio_info[USBSPKER].curPacket_Size;
        spkTxSts[i].buff = (uint32_t)(spkTxBuf+SPKTXBUFF*i);
    }

    spkTxSts[USB_SPKTXBUF_NUM - 1].status |= USB_SCDMA_L;
    dmaPtr = 0;
}

/**
  * @brief  uac_init
  *         Initialize the AUDIO interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
uint8_t uac_init(usb_device_t* pdev, uint8_t cfgidx)
{
    if (pdev == NULL )
		return 0;

    memset(audio_info,0,sizeof(audio_info));

#if(AUDIO_MANAGER)
	audio_info[USBSPKER].freq	= USBSPK_HNDL.format.sampleRateHz;
	audio_info[USBSPKER].dwidth = USBSPK_HNDL.format.bitSlot*8;
	audio_info[USBSPKER].curPacket_Size = (audio_info[USBSPKER].dwidth/8)* (audio_info[USBSPKER].freq/1000)*2;

	audio_info[USBMIC].freq   = USBMIC_HNDL.format.sampleRateHz;
	audio_info[USBMIC].dwidth = USBMIC_HNDL.format.bitSlot*8;
	audio_info[USBMIC].curPacket_Size = (audio_info[USBMIC].dwidth/8)* (audio_info[USBMIC].freq/1000)*USB_MIC_CHANNELS;

#else // AUDIO MANAGER
    audio_info[USBSPKER].freq   = USB_SPK_SAMPLING_RATE_HZ;
#if SPK_AUDIO_24BIT
    audio_info[USBSPKER].dwidth = BITWIDTH_24;
#else
    audio_info[USBSPKER].dwidth = BITWIDTH_16;
#endif
    audio_info[USBSPKER].curPacket_Size = (audio_info[USBSPKER].dwidth/8)* (audio_info[USBSPKER].freq/1000)*2;


    audio_info[USBMIC].freq   = CONFIG_USB_MIC_FREQUENCY;
#if MIC_AUDIO_24BIT
    audio_info[USBMIC].dwidth = BITWIDTH_24;
#else
    audio_info[USBMIC].dwidth = BITWIDTH_16;
#endif
    audio_info[USBMIC].curPacket_Size = (audio_info[USBMIC].dwidth/8)* (audio_info[USBMIC].freq/1000)*USB_MIC_CHANNELS;
#endif // AUDIO_MANAGER

    uac_dma_desc_init();

    uac_handle_t* haudio;
    /* Allocate Audio structure */
    pdev->pClassData = audioClassData;
    memset(pdev->pClassData, 0, sizeof(audioClassData));

    if (pdev->pClassData == NULL)
    {
        return USBD_FAIL;
    }
    else
    {
        int i = 0;
        for (; i < USB_FUN_NUM_INTERFACES; i++)
        {
            haudio = ((uac_handle_t*)pdev->pClassData) + i;
            haudio->intfNum     = i;
            haudio->alt_setting = 0U;
            haudio->freq        = UAC_FREQ_DEFAULT;
            haudio->bit_depth   = 16U;
            if(i==1)
            {
                haudio->vol         = USB_DEV_SPK_VOL_DEF;
                audio_vol[0]  = audio_vol[1] = haudio->vol ;

                haudio->mixer_vol	= USB_DEV_SLIDETONE_VOL_DEF;
                audio_vol[3]   = haudio->mixer_vol ;
            }
			else if(i==2)
            {
                haudio->vol    = USB_DEV_MIC_VOL_DEF;
                audio_vol[2]   = haudio->vol ;
            }else
            {
            }
            haudio->mute        = 0U;
            if ((i == 1)&&(((usb_audio_interface_t*)pdev->pUserData)->Init(haudio->freq, vol_to_pencentage(USB_DEV_SPK_VOL_DEF,0x06), 0x06) != 0))
            {
                return USBD_FAIL;
            }

            if ((i == 1)&&(((usb_audio_interface_t*)pdev->pUserData)->Init(haudio->freq, vol_to_pencentage(USB_DEV_SLIDETONE_VOL_DEF,0x07), 0x07) != 0))
            {
                return USBD_FAIL;
            }

//            if ((i == 1)&&(((usb_audio_interface_t*)pdev->pUserData)->Init(haudio->freq, vol_to_pencentage(USB_DEV_SLIDETONE_VOL_DEF,0x0D), 0x0D) != 0))
//            {
//                return USBD_FAIL;
//            }

            if ((i == 2)&&(((usb_audio_interface_t*)pdev->pUserData)->Init(haudio->freq, vol_to_pencentage(USB_DEV_MIC_VOL_DEF,0x05), 0x05) != 0))
            {
                return USBD_FAIL;
            }
        }
    }

    return USBD_OK;
}

/**
  * @brief  uac_de_init
  *         DeInitialize the AUDIO layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
uint8_t uac_de_init(usb_device_t* pdev, uint8_t cfgidx)
{
    if (pdev == NULL ) return 0;
    hal_usb_flush_ep(pdev, AUDIO_OUT_EP);
    hal_usb_flush_ep(pdev, AUDIO_FB_EP);

    hal_usb_close_ep(pdev, AUDIO_OUT_EP);
    pdev->epOut[AUDIO_OUT_EP & 0xFU].isUsed = 0U;

    hal_usb_close_ep(pdev, AUDIO_FB_EP);
    pdev->epIn[AUDIO_FB_EP & 0xFU].isUsed = 0U;

    if (pdev->pClassData != NULL)
	{
        ((usb_audio_interface_t*)pdev->pUserData)->DeInit(0U);
    }

    return USBD_OK;
}

#if(SUPPORT_UAC20)
static void uac20_req_set_current_l1(usb_device_t* pdev, usb_setup_req_t* req)
{
    uac_handle_t	 *haudio;
    haudio = (uac_handle_t *) pdev->pClassData;
    //uint8_t temp = 0x01;
    uint32_t len = 0;

    switch(req->bmRequest)
	{
    case USB_DEVICE_AUDIO_GET_REQUEST_INTERFACE:
        len = LOBYTE(req->wLength);
		switch(HIBYTE(req->wIndex))
		{
	        case MIC_CLOCK_SELECTOR_BCK_ID:
	            send_buff[0]= MIC_CLOCK_SELECTOR_BICK_INDEX;
				break;
	        case SPKER_CLOCK_SELECTOR_BCK_ID:
	            send_buff[0] = SPKER_CLOCK_SELECTOR_BICK_INDEX;
				break;
	        case SPKER_CLOCK_SOURCE_BCLK_ID:
	        {
	            if(LOBYTE(req->wLength) == SPKER_CLOCK_BICK_NUM)
	            {
	                send_buff[0] = SPKER_CLOCK_BICK_NUM;
	            }
	            else if(LOBYTE(req->wLength) == SPKER_SAMPLING_FREQUENCY_LEN)
	            {
	                *(uint32_t*)send_buff = audio_info[USBSPKER].freq;
	            }
				else{}

	            pdev->irqPriority = 1;
	            hal_usb_flush_ep(pdev, 0x80U);
				break;
	        }
	        case MIC_CLOCK_SOURCE_BCLK_ID:
	            if(req->wLength == MIC_CLOCK_BICK_NUM)
	            {
	                send_buff[0] = MIC_CLOCK_BICK_NUM;
	            }
	            else if(LOBYTE(req->wLength) == MIC_SAMPLING_FREQUENCY_LEN)
	            {
	                *(uint32_t*)send_buff = audio_info[USBMIC].freq ;
	            }

	            pdev->irqPriority = 1;
				break;
	        case USB_DEV_SPKER_VOLUME_RANGE_ID:
			case USB_DEV_MIC_VOLUME_RANGE_ID:
			case 0x0D:
		        {
		            uac_req_get_current(pdev, req);
		            return;
		        }
			default:
				break;
        }
        usb_ctl_send_data(pdev, (uint8_t*)send_buff,len);
        return;
        break;
    case USB_DEVICE_AUDIO_SET_REQUEST_INTERFACE:
		switch(HIBYTE(req->wIndex))
		{
	        case SPKER_CLOCK_SOURCE_BCLK_ID:
	        {
	            usb_ctl_prepare_rx(pdev, haudio->control.data, req->wLength);
	            haudio->control.cmd = AUDIO_REQ_SET_CUR;
	            haudio->control.req_type = AUDIO_STREAMING_REQ;
	            haudio->control.cs = AUDIO_SPKER_REQ_FREQ_CTRL;
	            haudio->control.unit = req->wIndex;
	            pdev->irqPriority = 1;
				break;
	        }
	        case MIC_CLOCK_SOURCE_BCLK_ID:
	        {
	            usb_ctl_prepare_rx(pdev, haudio->control.data, req->wLength);
	            haudio->control.cmd = AUDIO_REQ_SET_CUR;
	            haudio->control.req_type = AUDIO_STREAMING_REQ;
	            haudio->control.cs = AUDIO_MIC_REQ_FREQ_CTRL;
	            haudio->control.unit = req->wIndex;
	            pdev->irqPriority = 1;
				break;
	        }
			case USB_DEV_SPKER_VOLUME_RANGE_ID:
			case USB_DEV_MIC_VOLUME_RANGE_ID:
			case 0x0D:
	        {
	            uac20_req_set_current_l2(pdev, req);
	            pdev->irqPriority = 1;
	            return;
	        }
			default:
				break;
		}
        usb_ctl_send_data(pdev, NULL, 0);
        break;
    default:
        break;
    }
}

static void uac20_req_rang(usb_device_t* pdev, usb_setup_req_t* req)
{
    uint16_t len;

    switch(req->bmRequest)
	{
    case USB_DEVICE_AUDIO_GET_REQUEST_INTERFACE:
        len = req->wLength;
		switch(HIBYTE(req->wIndex))
		{
	        case MIC_CLOCK_SELECTOR_BCK_ID:
	        case SPKER_CLOCK_SELECTOR_BCK_ID:
	           break;
	        case SPKER_CLOCK_SOURCE_BCLK_ID:
	        {
				switch(LOBYTE(req->wLength))
				{
		            case UAC_SPKER_SAMPLE_RATE_NUM:
		            {
		                send_buff[0] = USB_SPKER_SUPPORT_SAMPLE_RATE_LIST_LEN;
		                send_buff[1] = USB_SPKER_SUPPORT_SAMPLE_RATE_LIST_LEN >> 8;
						break;
		            }
#if(SUPPORT_USB_SPK)
		            case USB_SPKER_SUPPORT_SAMPLE_RATE_LIST_LEN:
		            {
		                len =  MIN(USB_SPKER_SUPPORT_SAMPLE_RATE_LIST_LEN, LOBYTE(req->wLength));
		                memcpy(send_buff, usbd_audio_spker_sample_rate_list, len);
						break;
		            }
#endif
#if(USB_SPKER_SUPPORT_SAMPLE_RATE_LIST_LEN != 0x0e)
					case 0x0e:
		            {
		                len =  MIN(USB_SPKER_SUPPORT_SAMPLE_RATE_LIST_LEN, LOBYTE(req->wLength));
		                memcpy(send_buff,usbd_audio_spker_sample_rate_default,len);
						break;
		            }
#endif
					default:
					{
		                len =  USB_SPKER_SUPPORT_SAMPLE_RATE_LIST_LEN;
		                memcpy(send_buff, usbd_audio_spker_sample_rate_list,len);
						break;
		            }
				}
				break;
	        }
#if(SUPPORT_USB_MIC)
	        case MIC_CLOCK_SOURCE_BCLK_ID:
	        {
				switch(LOBYTE(req->wLength))
				{
		            case UAC_MIC_SAMPLE_RATE_NUM:
					{
		                send_buff[0] = UAC_MIC_SUPPORT_SAMPLE_RATE_LIST_LEN;
		                send_buff[1] = UAC_MIC_SUPPORT_SAMPLE_RATE_LIST_LEN >> 8;
						break;
		            }
		            case UAC_MIC_SUPPORT_SAMPLE_RATE_LIST_LEN:
		            {
		                len = MIN(UAC_MIC_SUPPORT_SAMPLE_RATE_LIST_LEN,LOBYTE(req->wLength));
		                memcpy(send_buff,usbd_audio_mic_sample_rate_list,len);
						break;
		            }
#if(UAC_MIC_SUPPORT_SAMPLE_RATE_LIST_LEN != 0x0e)
					case 0x0e:
					{
		                len = MIN(UAC_MIC_SUPPORT_SAMPLE_RATE_LIST_LEN,LOBYTE(req->wLength));
		                memcpy(send_buff,usbd_audio_mic_sample_rate_default,len);
						break;
		            }
#endif
					default:
					{
		                len =  UAC_MIC_SUPPORT_SAMPLE_RATE_LIST_LEN ;
		                memcpy(send_buff,usbd_audio_mic_sample_rate_list,len);
						break;
		            }
				}
				break;
	        }
#endif
			case USB_DEV_SPKER_VOLUME_RANGE_ID:
			{
	            if((LOBYTE(req->wValue)== USB_DEV_SPKER_VOLUME_RANGE_LEFT_ID)||(LOBYTE(req->wValue)== USB_DEV_SPKER_VOLUME_RANGE_RIGHT_ID))
	            {
	                len =  MIN(GET_RANGE_SPKER_FUATURE_LEN,LOBYTE(req->wLength));
	                memcpy(send_buff,get_spker_range_feature_unitid,len);
	            }
				break;
	        }
			case USB_DEV_MIC_VOLUME_RANGE_ID:
	        {
	            len =  MIN(GET_RANGE_MIC_FUATURE_LEN,LOBYTE(req->wLength));
	            memcpy(send_buff,get_mic_range_feature_unitid,len);
				break;

	        }
			case USB_DEV_MIXER_VOLUME_RANGE_ID:
	        {
	            len =  MIN(GET_RANGE_MIXER_FUATURE_LEN,LOBYTE(req->wLength));
	            memcpy(send_buff,get_mixer_range_feature_unitid,len);
				break;
	        }

			case 0x0D:
	        {
	            len =  MIN(GET_RANGE_MIXER_FUATURE_LEN,LOBYTE(req->wLength));
	            memcpy(send_buff,get_mixer_range_feature_unitid,len);
				break;
	        }
			default:
				break;
		 }
	    usb_ctl_send_data(pdev, (uint8_t*)send_buff,len);
	    break;
	    case USB_DEVICE_AUDIO_SET_REQUEST_INTERFACE:
	        break;
	    default:
        break;
    }
}
#endif


/**
  * @brief  uac_setup
  *         Handle the AUDIO specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
uint8_t uac_setup(usb_device_t* pdev, usb_setup_req_t* req)
{
    uac_handle_t* haudio;
    uint16_t len;
    uint8_t* pbuf;
    //uint16_t status_info = 0U;
    uint8_t ret = USBD_OK;

    switch (req->bmRequest & USB_REQ_TYPE_MASK)
	{
    case USB_REQ_TYPE_CLASS:
#if(SUPPORT_UAC20)
        if (is_usb_dev_UAC20 == UAC_20)
        {
            switch (req->bRequest)
            {
            case AUDIO_REQ_CUR:
                uac20_req_set_current_l1(pdev, req);
#if(SUPPORT_USB_INSERT_DETECTION && SUPPORT_UAC20)
				setCurFlag = 1;
#endif
                break;
            case USB_DEV_HID_BDESCRIPTORTYPE:
                usb_ctl_send_data(pdev, NULL, 0);
                break;
            case AUDIO_REQ_RANGE:
                uac20_req_rang(pdev, req);
                break;
            default:
                usb_ctl_error(pdev, req);
                ret = USBD_FAIL;
                break;
            }
        }
		else /// UAC10
#endif
		{
            switch (req->bRequest)
            {
            case AUDIO_REQ_GET_CUR:
                uac_req_get_current(pdev, req);
                break;

            case AUDIO_REQ_GET_MAX:
                uac_req_get_max(pdev, req);
                break;

            case AUDIO_REQ_GET_MIN:
                uac_req_get_min(pdev, req);
                break;

            case AUDIO_REQ_GET_RES:
                uac_req_get_res(pdev, req);
                break;

            case AUDIO_REQ_SET_CUR:
                uac10_req_set_current(pdev, req);
                break;
            case 0x0A:   /*HID*/
            case 0x0B:   /*HID*/
                usb_ctl_send_data(pdev, NULL, 0);
                break;

            case AUDIO_REQ_SET_RES:
                uac10_req_set_current(pdev, req);
                break;
            case AUDIO_REQ_SET_MIN:
            case AUDIO_REQ_SET_MAX:
                usb_ctl_send_data(pdev, NULL, 0);
                break;

            default:
                usb_ctl_error(pdev, req);
                ret = USBD_FAIL;
                break;
            }
        }
        break;
    case USB_REQ_TYPE_STANDARD:
        switch (req->bRequest)
		{
        case USB_REQ_GET_STATUS:
            if (pdev->devState == USB_DEV_STATE_CONFIGURED)
			{
                *(uint16_t*)send_buff = 0;
                usb_ctl_send_data(pdev, (uint8_t*)send_buff, 2U);
            }
			else
			{
                usb_ctl_error(pdev, req);
                ret = USBD_FAIL;
            }
            break;

        case USB_REQ_GET_DESCRIPTOR:
            if ((req->wValue >> 8) == AUDIO_DESCRIPTOR_TYPE)
			{
#if(SUPPORT_UAC20)
                if (is_usb_dev_UAC20 == UAC_20)
                {
                    pbuf = uac20_CfgDesc + 18;
                    len = MIN(USB_AUDIO20_CONFIG_DESC_SIZ, req->wLength);
                }
                else
#endif
                {
                    pbuf = uac_CfgDesc + 18;
                    len = MIN(USB_AUDIO_CONFIG_DESC_SIZ, req->wLength);
                }
                usb_ctl_send_data(pdev, pbuf, len);
            }else if ((req->wValue >> 8) == 0x22) /*HID*/ {
#if SUPPORT_USB_HID
                pbuf = g_UsbDeviceHidKeyboardReportDescriptor;
                len = MIN(HID_KEYBOARD_DESC_SIZE, req->wLength);

                usb_ctl_send_data(pdev, pbuf, len);
#endif
            }
            break;

        case USB_REQ_GET_INTERFACE:
            if (pdev->devState == USB_DEV_STATE_CONFIGURED)
			{
                if (HIBYTE(req->wIndex) < USB_FUN_NUM_INTERFACES)
				{
                    haudio = ((uac_handle_t*)pdev->pClassData) + HIBYTE(req->wIndex);
                    *(uint32_t*)send_buff = haudio->alt_setting;
                    usb_ctl_send_data(pdev, (uint8_t*)send_buff, 1U);
                }
				else
				{
                    usb_ctl_error(pdev, req);
                    ret = USBD_FAIL;
                }
            }
			else
			{
                usb_ctl_error(pdev, req);
                ret = USBD_FAIL;
            }
            break;

        case USB_REQ_SET_INTERFACE:
            if (pdev->devState == USB_DEV_STATE_CONFIGURED)
            {
                if (req->wIndex < USB_FUN_NUM_INTERFACES)
                {
#if(SUPPORT_USB_INSERT_DETECTION)
                	insFlag = 1;
#endif
                    haudio = ((uac_handle_t*)pdev->pClassData)+ LOBYTE(req->wIndex);
                    pdev->devLpmConfig = USB_DEV_STATE_SUSPENDED ;

                    if (haudio->alt_setting != (uint8_t)(req->wValue))
                    {

                        haudio->alt_setting = (uint8_t)(req->wValue);
                        if (haudio->alt_setting == 0U)
                        {
                            uac_out_stop_reset(pdev, haudio->intfNum);
                        }
                        else if (haudio->alt_setting == 1U)
                        {
							if (is_usb_dev_UAC20 == UAC_10)
	                            haudio->bit_depth = (2 + SUPPORT_USB_24BIT_ONLY)*8;
							else
								haudio->bit_depth = 16U;
                        }
                        else if (haudio->alt_setting == 2U)
                        {
                            haudio->bit_depth = 24U;
                        }
                        else if ((haudio->alt_setting == 3U)||(haudio->alt_setting == 4U))
                        {
                            if (is_usb_dev_UAC20 == UAC_20)
                                haudio->bit_depth = 32;
                            else
                                haudio->bit_depth = 24U;
                        }

                        audio_info[LOBYTE(req->wIndex)-INTF_AUDIO_STREAM_SPK].dwidth = haudio->bit_depth;
                        if (is_usb_dev_UAC20 == UAC_20)
						{
                            if(haudio->alt_setting)
                                uac_out_restart(pdev, haudio->intfNum);
                        }
						else
						{
                            if((MULT_FS_SUPPORT == 0)&&(haudio->alt_setting))
                            {
                                uac_out_restart(pdev, haudio->intfNum);
                            }
                        }
                    }

                }
                else
                {
                    usb_ctl_error(pdev, req);
                    ret = USBD_FAIL;
                }
            }
            else
            {
                usb_ctl_error(pdev, req);
                ret = USBD_FAIL;
            }
            break;

        default:
            usb_ctl_error(pdev, req);
            ret = USBD_FAIL;
            break;
        }
        break;
    default:
        usb_ctl_error(pdev, req);
        ret = USBD_FAIL;
        break;
    }

    return ret;
}

/**
  * @brief  uac_get_cfg_desc
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
uint8_t* uac_get_cfg_desc(uint16_t* length)
{
#if(SUPPORT_UAC20)
    if (is_usb_dev_UAC20 == UAC_20)
    {
        *length = sizeof(uac20_CfgDesc);
        return uac20_CfgDesc;
    }
    else
#endif
	{
#if(TEST_USB_SWITCH || SUPPORT_UAC_SELF_ADAPTION)
		if(Usbd_Speed_Cfg == USB_SPEED_HIGH)
        {
        	*length = sizeof(uac_CfgDesc);
        	return uac_CfgDesc;
		}
		else
		{
			*length = sizeof(uac_fullCfgDesc);
        	return uac_fullCfgDesc;
		}
#else
    	*length = sizeof(uac_CfgDesc);
    	return uac_CfgDesc;
#endif
    }

}

/**
  * @brief  uac_get_other_speed_cfg_desc
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer

uint8_t* uac_get_other_speed_cfg_desc(uint16_t* length)
{
    if (is_usb_dev_UAC20)
    {
        *length = sizeof(uac20_CfgDesc);
        return uac20_CfgDesc;
    }
    else{
        *length = sizeof(uac_CfgDesc);
        return uac_CfgDesc;
    }//#endif
}
*/



/**
  * @brief  uac_sof
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
uint8_t uac_sof(usb_device_t* pdev)
{
#if(SUPPORT_USB_INSERT_DETECTION)
	insCnt++;
#endif
    return USBD_OK;
}


/**
  * @brief  uac_iso_in_incomplete
  *         handle data ISO IN Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
uint8_t uac_iso_in_incomplete(usb_device_t* pdev, uint8_t epnum)
{
    return USBD_OK;
}

/**
  * @brief  uac_iso_out_incomplete
  *         handle data ISO OUT Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */

uint8_t uac_iso_out_incomplete(usb_device_t* pdev, uint8_t epnum)
{
    return USBD_OK;
}

/**
  * @brief  uac_dma_restart
  *         handle DMA Restart
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
void uac_dma_restart(usb_device_t* pdev, uint32_t epnum)
{
    if (epnum == AUDIO_OUT_EP)
    {
        uac_dma_desc_init();
        hal_usb_ep_receive(pdev, AUDIO_OUT_EP, (uint8_t*)spkTxSts, audio_info[USBSPKER].curPacket_Size, 0);
    }
}


#if SUPPORT_ALG_2MIC_MEETING_BOX
extern short usb_talking;
#endif
/**
  * @brief  uac_mic_in
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
uint8_t uac_mic_in(usb_device_t* pdev,uint8_t epnum)
{
    if (pdev == NULL ) return 0;

    if (epnum == (AUDIO_IN_EP & 0xf))
	{
        struct scDMA *pDma = &scDMA_EPIN[2];

        //int i;
        //int16_t *pDst = (int16_t *)micBuff;
        uint32_t len = audio_info[USBMIC].curPacket_Size;
        bool flag = 0;

        uac_handle_t* haudio;
        haudio = ((uac_handle_t*)pdev->pClassData) + INTF_AUDIO_STREAM_MIC ;

        fs44100_handle++;
        if (is_usb_dev_UAC20 == UAC_20)
		{
        	switch(audio_info[USBMIC].freq)
        	{
        	case 44100:
        		if(fs44100_handle < 80)
        		{
        			 if(audio_info[USBMIC].dwidth == 16 )
						len = (audio_info[USBMIC].trans_flag) ? 24:20 ;
					else
						len = (audio_info[USBMIC].trans_flag) ? 36:30 ;
        			 audio_info[USBMIC].trans_flag = !audio_info[USBMIC].trans_flag;
        		}
        		else
        		{
        			fs44100_handle = 0;
        			len = (audio_info[USBMIC].dwidth == 16) ? 24 : 36;
        			audio_info[USBMIC].trans_flag = 1;
        		}
                len = len*USB_MIC_CHANNELS / 2;
                break;
        	case 88200:
        		if(fs44100_handle < 40)
        		{
        			len = (audio_info[USBMIC].dwidth == 16) ? 44 : 66;
        		}
        		else
        		{
        			len = (audio_info[USBMIC].dwidth == 16) ? 48 : 72;
        			fs44100_handle = 0;
        		}
        		len = len*USB_MIC_CHANNELS / 2;
        		break;
        	case 176400:
        		if(fs44100_handle < 20)
        		{
        			len = (audio_info[USBMIC].dwidth == 16) ? 88 : 132;
        		}
        		else
        		{
        			len = (audio_info[USBMIC].dwidth == 16) ? 92 : 138;
        			fs44100_handle = 0;
        		}
        		len = len*USB_MIC_CHANNELS / 2;
        		break;
        	default:
        		break;

        	}
        }
		else // UAC 10
		{
            switch(haudio->freq)
            {
            case 44100:
            	if(fs44100_handle >= 10)
            	{
            		len += (haudio->bit_depth >> 2); //byte*2channel, *2/8 = /4
                    fs44100_handle = 0;
            	}
            	break;
            case 88200:
            	if(fs44100_handle >= 10)
            	{
            		len += (haudio->bit_depth >> 2) << 1;
                    fs44100_handle = 0;
            	}
            	break;
            case 176400:
				if(haudio->bit_depth == 24)
				{
					mltip_len = (mltip_len == 576)?480:576;
					len = mltip_len;
					if(fs44100_handle >= 19 && len == 480)
					{
						len = 504;
						fs44100_handle = 0;
					}

					flag = 1;
            	}
				else if(fs44100_handle >= 10)
            	{
                    len += 16;
                    fs44100_handle = 0;
            	}
				break;
            case 192000:
            	if(haudio->bit_depth == 24)
            	{
                    len = 576;
                    flag = 1;
            	}
            	break;
            default:

            	break;
            }

            pDma->bufferPtr = (uint32_t)micBuff;
            pDma->bufferSts = len;
            pDma->bufferSts |= USB_SCDMA_L;
            pDma->bufferSts |= USB_SCDMA_IOC;
            if(flag)
            {
                pDma->bufferSts |= USB_SCDMA_MTRF;
                pDma->bufferSts |= USB_SCDMA_SP;
            }
        }


	    uint32_t len_record = len ;

		micIsrCount++;
#if(AUDIO_MANAGER)
		if (USBMIC_HNDL.enable && USBMIC_HNDL.isrCb)
		{
			memset(micBuff, 0, len);
			USBMIC_HNDL.isrCb(USBMIC_HNDL.streamHandler, micBuff, &len, USBMIC_HNDL.format.bitSlot);
#if (SUPPORT_PLAY_TONE && D_USB_PLAY_TONE_ENABLE)
			uac_get_tone_data(micBuff, len);
#endif

		}
#endif
        if (is_usb_dev_UAC20 == UAC_20)
		{
			hal_usb_transmit(pdev, AUDIO_IN_EP, (uint8_t*)micBuff, len);
        }
		else
		{

			if(len_record != len)
			{
				pDma->bufferSts = (pDma->bufferSts&0xFFFFF000)|len;
			}
            hal_usb_transmit(pdev, AUDIO_IN_EP, (uint8_t*)pDma, len);
		}
    }
#if SUPPORT_ALG_2MIC_MEETING_BOX
	usb_talking = 1;
#endif
    return USBD_OK;
}

/**
  * @brief  uac_data_out
  *         handle Data Out Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
uint8_t uac_data_out(usb_device_t* pdev,  uint8_t epnum)
{
    int dmaIndex = dmaPtr;
    if (pdev == NULL ) return 0;

    if (epnum == AUDIO_OUT_EP)
    {
        if ((spkTxSts[dmaIndex].status & USB_SCDMA_BS_Msk) == 0)
        {
            int i;
            for (i = dmaIndex; i < USB_SPKTXBUF_NUM; i++)
            {
                /* Find the first "DMA_DONE" item from the list */
                if ((spkTxSts[i].status & USB_SCDMA_BS_Msk) == (2 << USB_SCDMA_BS_Pos))
                {
                    dmaIndex = i;
                    goto FIND_DMA_DONE;
                }
            }

            for (i = 0; i < dmaIndex; i++)
            {
                if ((spkTxSts[i].status & USB_SCDMA_BS_Msk) == (2 << USB_SCDMA_BS_Pos))
                {
                    dmaIndex = i;
                    break;
                }
            }
FIND_DMA_DONE:
            if( dmaIndex == USB_SPKTXBUF_NUM)
            {
                dmaIndex = 0;
            }
        }
        int bytes = audio_info[USBSPKER].curPacket_Size - (spkTxSts[dmaIndex].status & 0x7ff);
        if(bytes > 0)
		{
			spkIsrCount++;
#if (AUDIO_MANAGER)
			USBSPK_HNDL.format.bitSlot = audio_info[USBSPKER].dwidth>>3;
			if(!srSwCnt)
			{
				if (USBSPK_HNDL.enable && USBSPK_HNDL.isrCb)
	 				USBSPK_HNDL.isrCb(USBSPK_HNDL.streamHandler, (void*)spkTxSts[dmaIndex].buff, (uint32_t *)&bytes, USBSPK_HNDL.format.bitSlot);
			}
			else
			{
				srSwCnt--;
				memset((void*)spkTxSts[dmaIndex].buff, 0, bytes);
				if (USBSPK_HNDL.enable && USBSPK_HNDL.isrCb)
	 				USBSPK_HNDL.isrCb(USBSPK_HNDL.streamHandler, (void*)spkTxSts[dmaIndex].buff, (uint32_t *)&bytes, USBSPK_HNDL.format.bitSlot);
			}
#else
	//		uart_printf("spkCallback\n");
            usb_spk_recv((uint8_t*)spkTxSts[dmaIndex].buff, bytes, audio_info[USBSPKER].dwidth);
#endif
        }

        spkTxSts[dmaIndex].status = USB_SCDMA_IOC|audio_info[USBSPKER].curPacket_Size;
        if (dmaIndex >= USB_SPKTXBUF_NUM - 1)
        {
            spkTxSts[dmaIndex].status |= USB_SCDMA_L;
            dmaPtr = 0;
        }
        else
            dmaPtr++;
    }

    return USBD_OK;
}


uint8_t effective_host_freq(uint32_t freq)
{
    int ret = 1;
    int i;
    for( i = 0 ; i < SUPPORT_FREQ_LIST ; i++)
    {
        if(freq == frep_list[i])
        {
            ret = 0;
            break;
        }
    }
    return ret;
}

/**
  * @brief  uac_ep0_rx_ready
  *         handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
uint8_t uac_ep0_rx_ready(usb_device_t* pdev)
{
    if ( pdev == NULL ) return 0 ;
    uac_handle_t* haudio;
    haudio = ((uac_handle_t*)pdev->pClassData);

    if (haudio->control.cmd == AUDIO_REQ_SET_CUR)
    {
        if (haudio->control.req_type == AUDIO_CONTROL_REQ)
        {
            switch (haudio->control.cs)
            {
            case AUDIO_CONTROL_REQ_FU_RES:  		/* Volume Control */
            {
                if(haudio->control.unit == AUDIO_CONTROL_REQ_SIDETONE_VOL ){
                    audio_res[0] = *(uint32_t*)scDMA_OUT[0].bufferPtr&0xffff;
                }else{
                    if(haudio->control.unit == AUDIO_CONTROL_REQ_SPKER_VOL) {
                        audio_res[1]= *(uint32_t*)scDMA_OUT[0].bufferPtr&0xffff;
                    }else{
                        audio_res[2]= *(uint32_t*)scDMA_OUT[0].bufferPtr&0xffff;
                    }
                }
            };
                break;
            case AUDIO_CONTROL_REQ_FU_VOL:  		/* Volume Control */
            {
                uac_handle_t* haudio_ct;
                if(haudio->control.unit == AUDIO_CONTROL_REQ_SIDETONE_VOL || haudio->control.unit == 0x0D){

                    haudio->vol = *(uint32_t*)scDMA_OUT[0].bufferPtr&0xffff;
                    audio_vol[3] = haudio->vol ;
                }
                else
                {
                    haudio_ct = ((uac_handle_t*)pdev->pClassData) + ((haudio->control.unit == AUDIO_CONTROL_REQ_MIC_VOL)?2:1);
					haudio->control.unit = (haudio->control.unit == AUDIO_CONTROL_REQ_MIC_VOL)?5:6 ;
                    haudio->vol = *(uint32_t*)scDMA_OUT[0].bufferPtr&0xffff;

					if(haudio->control.unit == AUDIO_CONTROL_REQ_SPKER_VOL)
					{
	                    if(haudio->control.cn == 1)
	                        audio_vol[0] = haudio->vol;
                        else if(haudio->control.cn == 2)
                            audio_vol[1] = haudio->vol ;
                        else
                            audio_vol[0] = audio_vol[1] = haudio->vol;
                    }
                    else
                    {
                        audio_vol[2] = haudio->vol;
                    }
                    haudio_ct->vol = haudio->vol;
                }

#ifdef CONGIFG_USB_DEV_VOLUME_FIT_USBH
#if (AUDIO_MANAGER)
				switch (haudio->control.unit)
				{
					case AUDIO_CONTROL_REQ_SPKER_VOL:
						USBSPK_HNDL.vol = vol_to_pencentage(haudio->vol, haudio->control.unit);
						if(haudio->control.cn == SPKER_LEFT_FRORNT){
							/*spker left front gain*/
							if (USBSPK_HNDL.updateInfoCb)
								USBSPK_HNDL.updateInfoCb(USBSPK_HNDL.streamHandler, PLAYBACK_PATH, INFO_VOL_LEFT, &USBSPK_HNDL.vol);
						}else if(haudio->control.cn == SPKER_RIGHT_FRONT){
							/*spker right front gain*/
							if (USBSPK_HNDL.updateInfoCb)
								USBSPK_HNDL.updateInfoCb(USBSPK_HNDL.streamHandler, PLAYBACK_PATH, INFO_VOL_RIGHT, &USBSPK_HNDL.vol);
						}else{
							if (USBSPK_HNDL.updateInfoCb)
								USBSPK_HNDL.updateInfoCb(USBSPK_HNDL.streamHandler, PLAYBACK_PATH, INFO_VOL_STEREO, &USBSPK_HNDL.vol);
						}
						break;

					case AUDIO_CONTROL_REQ_MIC_VOL:
						USBMIC_HNDL.vol = vol_to_pencentage(haudio->vol, haudio->control.unit);
						if (USBMIC_HNDL.updateInfoCb)
						{
							USBMIC_HNDL.updateInfoCb(USBMIC_HNDL.streamHandler, CAPTURE_PATH, INFO_VOL_STEREO, &USBMIC_HNDL.vol);
						}
						break;

					case AUDIO_CONTROL_REQ_SIDETONE_VOL:
					case 0x0D:
						if (USBMIC_HNDL.updateInfoCb)
						{
							uint8_t sideVol = vol_to_pencentage(haudio->vol, haudio->control.unit);
							USBMIC_HNDL.updateInfoCb(USBMIC_HNDL.streamHandler, SIDETONE_PATH, INFO_VOL_STEREO, &sideVol);
						}
						break;
				}
#else
				if(((usb_audio_interface_t*)pdev->pUserData)->VolumeCtl != NULL)
                {
                    ((usb_audio_interface_t*)pdev->pUserData)->VolumeCtl(vol_to_pencentage(haudio->vol, haudio->control.unit),
                                                                  haudio->control.unit,
                                                                  haudio->control.cn);
                }
#endif
#endif
            };
                break;

            case AUDIO_CONTROL_REQ_FU_MUTE:    		/* Mute Control */
            {
                uac_handle_t* haudio_ct;

                if(haudio->control.unit == AUDIO_CONTROL_REQ_SIDETONE_VOL || haudio->control.unit == 0x0D)
				{
                    sidetone_mute = haudio->mute = *(uint32_t*)scDMA_OUT[0].bufferPtr&0xff;
                    USBMIC_HNDL.updateInfoCb(USBMIC_HNDL.streamHandler, SIDETONE_PATH, INFO_MUTE, &sidetone_mute);
                    break;
                }
				else
				{
                    haudio_ct = ((uac_handle_t*)pdev->pClassData) + ((haudio->control.unit == AUDIO_CONTROL_REQ_MIC_VOL)?2:1);
					haudio->control.unit = (haudio->control.unit == AUDIO_CONTROL_REQ_MIC_VOL)?5:6 ;

                    haudio->mute = *(uint32_t*)scDMA_OUT[0].bufferPtr&0x1;
                    haudio_ct->mute = haudio->mute;
                }
#if (AUDIO_MANAGER)
				if(haudio->control.unit == AUDIO_CONTROL_REQ_SPKER_VOL)
				{
					USBSPK_HNDL.mute = haudio->mute;
					USBSPK_HNDL.updateInfoCb(USBSPK_HNDL.streamHandler, PLAYBACK_PATH, INFO_MUTE, &USBSPK_HNDL.mute);
				}
				else if(haudio->control.unit == AUDIO_CONTROL_REQ_MIC_VOL)
				{
					USBMIC_HNDL.mute = haudio->mute;
					USBMIC_HNDL.updateInfoCb(USBMIC_HNDL.streamHandler, CAPTURE_PATH, INFO_MUTE, &USBMIC_HNDL.mute);
				}
				else if(haudio->control.unit == AUDIO_CONTROL_REQ_SIDETONE_VOL)
				{
					// sidetone
//					USBMIC_HNDL.updateInfoCb(USBMIC_HNDL.streamHandler, SIDETONE_PATH, INFO_MUTE, &USBMIC_HNDL.mute);
				}
				else
				{}
#else
                if(((usb_audio_interface_t*)pdev->pUserData)->MuteCtl != NULL)
                {
                    ((usb_audio_interface_t*)pdev->pUserData)->MuteCtl(haudio->mute,haudio->control.unit);
                }
#endif
            };
                break;
            }

        }
        else if (haudio->control.req_type == AUDIO_STREAMING_REQ)
        {
            /* Frequency Control */
            uint8_t index = haudio->control.cs;

            if (is_usb_dev_UAC20 == UAC_20)
                index = haudio->control.cs;
            else
                index = haudio->control.unit;

            if ((index == INTF_AUDIO_STREAM_SPK)||(index== INTF_AUDIO_STREAM_MIC))
            {
                audio_info[index-INTF_AUDIO_STREAM_SPK].freq = *(uint32_t*)scDMA_OUT[0].bufferPtr&0xffffff;
                uac_handle_t* haudio_ep;
                haudio_ep = ((uac_handle_t*)pdev->pClassData) + index;
                haudio_ep->freq = audio_info[index-INTF_AUDIO_STREAM_SPK].freq;

                if (is_usb_dev_UAC20 == UAC_20)
                {
                    if(effective_host_freq(audio_info[index-INTF_AUDIO_STREAM_SPK].freq))
					{
                        haudio_ep->freq = audio_info[index-INTF_AUDIO_STREAM_SPK].freq = SAMPLING_RATE_48000;
//                        uart_printf("Set default 48k!\r\n");
                        uart_printf("fr: %d\n", audio_info[index-INTF_AUDIO_STREAM_SPK].freq);
                        system_reset();
                    }
                }

                uac_out_restart(pdev, index);//haudio->control.unit

#if(AUDIO_MANAGER)
                if(index == INTF_AUDIO_STREAM_SPK)
                {	// DAC
					USBSPK_HNDL.format.bitSlot = audio_info[USBSPKER].dwidth/8;
					USBSPK_HNDL.format.sampleRateHz = audio_info[USBSPKER].freq;

					if(curSr != audio_info[USBSPKER].freq)
					{
						srChangeFlag = 1;
						if(is_usb_dev_UAC20 == UAC_20)
							srSwCnt = 400;
						else
							srSwCnt = 50;
					}
					curSr = audio_info[USBSPKER].freq;

#ifdef TEST_USB_I2S_SYNC
					set_feedback_thres(audio_info[USBSPKER].freq);

					switch(curSr)
					{
					case 44100:
						feedSr = 44140;
						break;
					case 48000:
						feedSr = 48000;
						break;
					case 88200:
						feedSr = 88300;
						break;
					case 96000:
						feedSr = 96096;
						break;
					case 176000:
						feedSr = 176640;
						break;
					case 192000:
						feedSr = 193740;
//						feedSr = 193480;
						break;
					default:
						feedSr = curSr;
						break;
					}
#endif

#if(SUPPORT_RESAMPLER)
					auSession_suspend(&audioManager.session[0], 1);
					rsrArg.inSr = curSr;
					rsrArg.outSr = 48000;
					rsrArg.flag = 1;
					auStream_t*p = USBSPK_HNDL.streamHandler;
					fifo_set_thresholdLevel(p->fifo, audio_info[USBSPKER].freq, 1);
#endif
					if (USBSPK_HNDL.enable && USBSPK_HNDL.updateInfoCb)
						USBSPK_HNDL.updateInfoCb(USBSPK_HNDL.streamHandler, PLAYBACK_PATH, INFO_FORMAT, &USBSPK_HNDL.format);

                }
				else
				{
					USBMIC_HNDL.format.bitSlot = audio_info[USBMIC].dwidth/8;
					USBMIC_HNDL.format.sampleRateHz = audio_info[USBMIC].freq;

					if (USBMIC_HNDL.enable && USBMIC_HNDL.updateInfoCb)
						USBMIC_HNDL.updateInfoCb(USBMIC_HNDL.streamHandler, CAPTURE_PATH, INFO_FORMAT, &USBMIC_HNDL.format);
				}
#else
#if (CODEC_ENABLE)
                if(index == 0x01)
                {
                	codec_set_dac_samplerate(haudio_ep->freq);
                }
                else
                {
                    codec_set_adc12_samplerate(haudio_ep->freq);
                }
#endif

#endif

            }
        }

        haudio->control.req_type = 0U;
        haudio->control.cs       = 0U;
        haudio->control.cn       = 0U;
        haudio->control.cmd      = 0U;
        haudio->control.len      = 0U;
        haudio->control.unit     = 0U;
        pdev->irqPriority        = 0U;
    }

    return USBD_OK;
}

/**
  * @brief  uac_ep0_tx_ready
  *         handle EP0 TRx Ready event
  * @param  pdev: device instance
  * @retval status
  */


uint8_t uac_ep0_tx_ready(usb_device_t* pdev)
{
    /* Only OUT control data are processed */
    return USBD_OK;
}


/**
* @brief  uac_get_device_qualifier_desc
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t* uac_get_device_qualifier_desc(uint16_t* length)
{
    *length = sizeof(uac_DeviceQualifierDesc);
    return uac_DeviceQualifierDesc;
}

/**
* @brief  uac_register_interface
* @param  fops: Audio interface callback
* @retval status
*/
uint8_t uac_register_interface(usb_device_t* pdev, usb_audio_interface_t* fops)
{
    if (fops != NULL)
    {
        pdev->pUserData = fops;
    }
    return USBD_OK;
}

uint8_t usb_dev_get_mic_audio_info(void)
{
    uint8_t ret = 0;
    if((audio_info[USBMIC].freq == SAMPLING_RATE_192000 || audio_info[USBMIC].freq == 176400)&&(audio_info[USBMIC].dwidth == 24))
	{
        ret = 1;
    }

    return ret ;
}

uint32_t usb_dev_get_speaker_audio_sample(void)
{
    return audio_info[USBSPKER].freq;
}

uint32_t usb_dev_get_mic_audio_sample(void)
{
    return audio_info[USBMIC].freq;
}

uint8_t usb_dev_get_spk_packages_per_ms(void)
{
	if(is_usb_dev_UAC20 == UAC_20)
	{
		return 8;
	}
	else
	{
		if(audio_info[USBSPKER].freq > SAMPLING_RATE_96000)
			return 2;
		else
			return 1;
	}
}

uint8_t usb_dev_get_mic_packages_per_ms(void)
{
	if(is_usb_dev_UAC20 == UAC_20)
	{
		return 8;
	}
	else
	{
		if(audio_info[USBMIC].freq > SAMPLING_RATE_96000)
			return 2;
		else
			return 1;
	}
}

void usb_dev_dectect_play_state_polling(void)
{
	static uint8_t timeCount = 0;

	if(++timeCount < 3)
		return;
	if(spkPlaying || spkIsrCount != 0)
	{
		if(USBSPK_HNDL.streamHandler == NULL)
			return;
		auStream_t *stream = (auStream_t *)USBSPK_HNDL.streamHandler;

		if(spkIsrCount < (timeCount - 1)*usb_dev_get_spk_packages_per_ms() && spkPlaying)
		{
			spkPlaying = false;
		//	uart_printf("0spk:%d,%d", spkIsrCount, usb_dev_get_spk_packages_per_ms());
			stream->fifo->fuc->set_playing_state(stream->fifo, spkPlaying);
		}
		else if(spkIsrCount >= (timeCount - 1)*usb_dev_get_spk_packages_per_ms() && !spkPlaying)
		{
			spkPlaying = true;
		//	uart_printf("1spk:%d,%d", spkIsrCount, usb_dev_get_spk_packages_per_ms());
			stream->fifo->fuc->set_playing_state(stream->fifo, spkPlaying);
		}
		spkIsrCount = 0;
	}
	if(micRecording || micIsrCount != 0)
	{
		if(USBMIC_HNDL.streamHandler == NULL)
			return;
		auStream_t *stream = (auStream_t *)USBMIC_HNDL.streamHandler;

		if(micIsrCount < (timeCount - 1)*usb_dev_get_mic_packages_per_ms() && micRecording)
		{
			micRecording = false;
			stream->fifo->fuc->set_playing_state(stream->fifo, micRecording);
		//	uart_printf("usbmic stop:%d, %d\n", micIsrCount, usb_dev_get_mic_packages_per_ms());
		}
		else if(micIsrCount >= (timeCount - 1)*usb_dev_get_mic_packages_per_ms() && !micRecording)
		{
			micRecording = true;
			stream->fifo->fuc->set_playing_state(stream->fifo, micRecording);
		//	uart_printf("usbmic play:%d, %d\n", micIsrCount, usb_dev_get_mic_packages_per_ms());
		}
		micIsrCount = 0;
	}
	timeCount = 0;
}

#if (SUPPORT_PLAY_TONE && D_USB_PLAY_TONE_ENABLE)
void uac_get_tone_data(uint8_t* pMicBuff, uint32_t len)
{
	int32_t toneData;
	float fadeScale = 0;
	int32_t micL, micR;
	
	for(int i = 0; i < len/(USBMIC_HNDL.format.bitSlot*2); i++)
	{
#if (PREEMPTIVE_MODE == 1)
		float fadeScaleTone = 0;
		int readOffset = PlayToneGetData(PLAY_TONE_USB, &toneData, &fadeScale, &fadeScaleTone, USBMIC_HNDL.format.sampleRateHz);
#else
		int readOffset = PlayToneGetData(PLAY_TONE_USB, &toneData, &fadeScale, USBMIC_HNDL.format.sampleRateHz);
#endif
		if(readOffset >= 0)
		{
			if(USBMIC_HNDL.format.bitSlot == BIT_SLOT_16)
			{
				micL = (int32_t)(pMicBuff[4*i]<<16) | ((int32_t)(pMicBuff[4*i + 1]) << 24);
				micR = (int32_t)(pMicBuff[4*i + 2]<<16) | ((int32_t)(pMicBuff[4*i + 3]) << 24);
			}
			else if(USBMIC_HNDL.format.bitSlot == BIT_SLOT_24)
			{
				micL = (int32_t)(pMicBuff[6*i] << 8) | ((int32_t)(pMicBuff[6*i + 1]) << 16) | ((int32_t)(pMicBuff[6*i + 2]) << 24);
				micR = (int32_t)(pMicBuff[6*i + 3] << 8) | ((int32_t)(pMicBuff[6*i + 4]) << 16) | ((int32_t)(pMicBuff[6*i + 5]) << 24);
			}
			else if(USBMIC_HNDL.format.bitSlot == BIT_SLOT_32)
			{
				micL = (int32_t)(pMicBuff[8*i]) | ((int32_t)(pMicBuff[8*i + 1]) << 8) | ((int32_t)(pMicBuff[8*i + 2]) << 16) | ((int32_t)(pMicBuff[8*i + 3]) << 24);
				micR = (int32_t)(pMicBuff[8*i + 4]) | ((int32_t)(pMicBuff[8*i + 5]) << 8) | ((int32_t)(pMicBuff[8*i + 6]) << 16) | ((int32_t)(pMicBuff[8*i + 7]) << 24);
			}
			else if(USBMIC_HNDL.format.bitSlot == BIT_SLOT_8)
			{
				micL = (int32_t)(pMicBuff[2*i] << 24);
				micR = (int32_t)(pMicBuff[2*i + 1] << 24);
			}
				
			if(fadeScale > 0)
			{
				micL = fadeScale*micL;
				micR = fadeScale*micR;
			}
			else
			{
#if (PREEMPTIVE_MODE == 1)
				if (fadeScaleTone != 0)
				{
#if D_PLAY_TONE_OUTPUT_MIX
					micL = toneData * fadeScaleTone + micL;
					micR = toneData * fadeScaleTone + micR;
#else
					micL = toneData * fadeScaleTone;
					micR = toneData * fadeScaleTone;
#endif
				}
				else
#endif
				{
#if D_PLAY_TONE_OUTPUT_MIX
					micL = toneData + micL;
					micR = toneData + micR;
#else
					micL = toneData;
					micR = toneData;
#endif
				}
			}
				
			if(USBMIC_HNDL.format.bitSlot == BIT_SLOT_16)
			{
				pMicBuff[4*i]	  = (micL >> 16)&0xff;
				pMicBuff[4*i + 1] = (micL >> 24)&0xff;
				pMicBuff[4*i + 2] = (micR >> 16)&0xff;
				pMicBuff[4*i + 3] = (micR >> 24)&0xff;
			}
			else if(USBMIC_HNDL.format.bitSlot == BIT_SLOT_24)
			{
				pMicBuff[6*i]	  = (micL >>  8)&0xff;
				pMicBuff[6*i + 1] = (micL >> 16)&0xff;
				pMicBuff[6*i + 2] = (micL >> 24)&0xff;
				pMicBuff[6*i + 3] = (micR >>  8)&0xff;
				pMicBuff[6*i + 4] = (micR >> 16)&0xff;
				pMicBuff[6*i + 5] = (micR >> 24)&0xff;
			}
			else if(USBMIC_HNDL.format.bitSlot == BIT_SLOT_32)
			{
				pMicBuff[8*i]	  = (micL	   )&0xff;
				pMicBuff[8*i + 1] = (micL >>  8)&0xff;
				pMicBuff[8*i + 2] = (micL >> 16)&0xff;
				pMicBuff[8*i + 3] = (micL >> 24)&0xff;
				pMicBuff[8*i + 4] = (micR	   )&0xff;
				pMicBuff[8*i + 5] = (micR >>  8)&0xff;
				pMicBuff[8*i + 6] = (micR >> 16)&0xff;
				pMicBuff[8*i + 7] = (micR >> 24)&0xff;
			}
			else if(USBMIC_HNDL.format.bitSlot == BIT_SLOT_8)
			{
				pMicBuff[2*i]	  = (micL >> 24)&0xff;
				pMicBuff[2*i + 1] = (micR >> 24)&0xff;
			}
		}
	}
}
#endif

#endif
