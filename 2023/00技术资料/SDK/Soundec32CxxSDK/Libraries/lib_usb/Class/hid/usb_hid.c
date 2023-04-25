/**
***********************************************************
 *
 * @file name	: usb_hid.c
 * @author	: Nick
 * @version	: Vxx.xx
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
#include "usb_hid.h"
#include "usb_req.h"
#include "uac.h"
#include "boardConfig.h"
#include "hid_parser.h"
#include "usb_hw.h"
#include "usb_hal.h"

#if (USB_ENABLE)
#define AO_WAKEUP_DO_HID_KEY_DELAY               11
#define AO_WAKEUP_DO_HID_PRESSED_KEY_DELAY       7
#define AO_WAKEUP_DO_HID_RELEASE_KEY_DELAY       4
#if SUPPORT_ALTOTOOL
#define USB_DEV_HIDRXBUF_NUM	                     12
#else
#define USB_DEV_HIDRXBUF_NUM	                     6//12
#endif
#define USB_DEV_HIDRXBUF_SIZE	                     64
#define USB_DEV_HIDTXBUF_SIZE	                     64

#define REPORT_HIDKEY_IN                         0x01
#define REPORTIN_ID                              0x02
#define REPORTOUT_ID                             0x01

__attribute__((aligned(4))) static struct
{
    uint32_t status;
    uint32_t buff;
}  hidCPrxSts[USB_DEV_HIDRXBUF_NUM];

static uint8_t hid_commp_buffer[64] __attribute__ ((aligned (4)));
__attribute__((aligned(8))) static uint8_t hidCPrx[64*USB_DEV_HIDRXBUF_NUM];

uint8_t hid_key_buffer[3] = {REPORT_HIDKEY_IN,0,0};
uint8_t hid_system_normal_work = 0;
int hidrx_dmaPtr = 0;
static uint8_t hid_commp_data_in = 0;
uint8_t hid_commp_data_in_blocksize = 0;
uint8_t blocksize_record = 0;

extern usb_device_t gUsbDevice;

#ifdef HANDLE_AO_ADC_KEY_WAKEUP_FROM_USB_DEV_SUSPEND
/*AO wakeup from usbd suspend, handle adc key event*/
typedef struct{
    int16_t  record[2];
    uint8_t  state[2];
    uint8_t  ao_adc_key;
    uint8_t  key_release;
}hid_key_t;
hid_key_t hid_key ;

/*******************End*************************/
#endif


#ifdef HANDLE_AO_ADC_KEY_WAKEUP_FROM_USB_DEV_SUSPEND
typedef enum{
    KEY_FUNC_NULL    = 0,
    KEY_FUNC_RECORD  = 1,
    KEY_FUNC_PRESSED = 2,
    KEY_FUNC_RELEASE = 3
}key_func_t;

void set_always_on_adc_key_status(uint8_t state)
{
    hid_key.ao_adc_key = state;
}

uint8_t get_always_on_adc_key_status(void)
{
    return hid_key.ao_adc_key ;
}

/*handler adc key-ao  wakeup record*/
uint8_t transmit_hid_record_key(key_func_t key)
{
    uint8_t ret = 0;
    uint8_t index = key - KEY_FUNC_PRESSED ;
    if(hid_key.state[index])
    {
        uart_printf("hid_key.record[%d] = %d \r\n",index,hid_key.record[index]);

        usb_dev_handler_t *phpcd_hid = usb_dev_Device.pData;//
        usb_device_t *pdev_hid = phpcd_hid->pData;

        //uart_printf("sent key = %d \r\n",hid_key.record[index]);

        set_hid_key_buffer(hid_key.record[index]);

        usb_pdd_setEndpoint_nak_status(AUDIO_HID_IN_EP,0);

        usb_pdd_Transmit (pdev_hid,AUDIO_HID_IN_EP,hid_key_buffer,HID_ISO_IN_ENDP_PACKET_SIZE);

        hid_key.record[index] = KEY_FUNC_NULL;
        hid_key.state[index]  = KEY_FUNC_NULL;
        ret = 1;
    }
    return ret;
}
void store_hid_key(uint8_t key)
{

    if(key){
        hid_key.record[0] = key ;
        hid_key.state[0]  = KEY_FUNC_RECORD ;
    }else{
        hid_key.record[1] = key ;
        hid_key.state[1]  = KEY_FUNC_RECORD ;
    }

}

void do_hid_key_release(void)
{
    if((hid_key.key_release) && (get_always_on_adc_key_status()))
    {

        //uart_printf("hid_key.key_release = %d .\r\n",hid_key.key_release);
        hid_key.key_release--;
        if(hid_key.key_release == AO_WAKEUP_DO_HID_RELEASE_KEY_DELAY ){
            if(transmit_hid_record_key(KEY_FUNC_RELEASE))
            {
                uart_printf("KEY_FUNC_RELEASE .\r\n");
                set_always_on_adc_key_status(0);
            }
        }else if(hid_key.key_release == AO_WAKEUP_DO_HID_PRESSED_KEY_DELAY ){
            if(transmit_hid_record_key(KEY_FUNC_PRESSED))
            {
                uart_printf("KEY_FUNC_PRESSED .\r\n");
            }
        }
    }
}
#endif

void usb_dev_hidrx_buffer_init(void)
{
    int i;

    for (i = 0; i < USB_DEV_HIDRXBUF_NUM; i++)
    {
        hidCPrxSts[i].status = USB_SCDMA_IOC|USB_DEV_HIDRXBUF_SIZE ;
        hidCPrxSts[i].buff = (uint32_t)(hidCPrx+USB_DEV_HIDRXBUF_SIZE*i);
    }

    hidCPrxSts[USB_DEV_HIDRXBUF_NUM - 1].status |= USB_SCDMA_L;
}

/**
  * @brief  usb_dev_hid_init
  *         Initialize the HID interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
uint8_t  usb_dev_hid_init (usb_device_t *pdev, uint8_t cfgidx)
{

    usb_dev_hidrx_buffer_init(); //DMA RX BUFFER

    /* Open EP IN */
    hal_usb_open_ep(pdev, AUDIO_HID_OUT_EP, USB_DEV_EP_TYPE_INTR, USB_DEV_HIDRXBUF_SIZE);
    pdev->epOut[AUDIO_HID_OUT_EP & 0xFU].isUsed = 1U;
    hal_usb_flush_ep(pdev, AUDIO_HID_OUT_EP);
    hal_usb_ep_receive(pdev, AUDIO_HID_OUT_EP, (uint8_t*)hidCPrxSts, USB_DEV_HIDRXBUF_SIZE, 0);

    /*hid in*/
    hal_usb_open_ep(pdev, AUDIO_HID_IN_EP, USB_DEV_EP_TYPE_INTR, /*HID_ISO_IN_ENDP_PACKET_SIZE*/ 64);
    pdev->epIn[AUDIO_HID_IN_EP & 0xFU].isUsed = 1U;
    hal_usb_flush_ep(pdev, AUDIO_HID_IN_EP);
    hid_system_normal_work = 1;

#ifdef HANDLE_AO_ADC_KEY_WAKEUP_FROM_USB_DEV_SUSPEND
    //uart_printf("DDDD = %d \r\n",get_always_on_adc_key_status());
    if(get_always_on_adc_key_status())
    {
        if(hid_key.state[0])
        {
            hid_key.key_release = AO_WAKEUP_DO_HID_KEY_DELAY;
            //usb_pdd_Transmit (pdev,AUDIO_HID_IN_EP,hid_key_buffer,HID_ISO_IN_ENDP_PACKET_SIZE);
        }
    }else
#endif
    {
        hal_usb_transmit (pdev, AUDIO_HID_IN_EP, hid_key_buffer, HID_ISO_IN_ENDP_PACKET_SIZE);
    }

    if (pdev->pClassData == NULL)
    {
        return USBD_FAIL;
    }

    ((usb_dev_hid_handle_t *)pdev->pClassData)->state = HID_IDLE;

    return USBD_OK;
}

/**
  * @brief  usb_dev_hid_de_init
  *         DeInitialize the HID layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
uint8_t  usb_dev_hid_de_init (usb_device_t *pdev,uint8_t cfgidx)
{
    /* Close HID EPs */
    hal_usb_flush_ep(pdev, AUDIO_HID_IN_EP);
    hid_system_normal_work = 0;

    /* Close EP OUT */
    if (pdev->epOut[AUDIO_HID_IN_EP & 0xFU].isUsed)
    {
        hal_usb_close_ep(pdev, AUDIO_HID_IN_EP);
        pdev->epOut[AUDIO_HID_IN_EP & 0xFU].isUsed = 0U;
    }

    hal_usb_flush_ep(pdev, AUDIO_HID_OUT_EP);

    /* Close EP OUT */
    if (pdev->epOut[AUDIO_HID_OUT_EP & 0xFU].isUsed)
    {
        hal_usb_close_ep(pdev, AUDIO_HID_OUT_EP);
        pdev->epOut[AUDIO_HID_OUT_EP & 0xFU].isUsed = 0U;
    }

    /**
  */

    return USBD_OK;
}

uint32_t set_hid_key_buffer(uint32_t key)
{
    uint32_t hid_key = ((key << 8)|REPORT_HIDKEY_IN);

    hid_key_buffer[0] = hid_key&0xff;
    hid_key_buffer[1] = (hid_key>>8)&0xff;
    hid_key_buffer[2] = (hid_key>>16)&0xff;
    return 1;
}


/*
VOL +         = 0x01 ;
VOL -          = 0x02 ;
PLAYPAUSE  = 0x04
*/

void is_hid_key(uint32_t key)
{
    if(hid_system_normal_work == 0){

#ifdef HANDLE_AO_ADC_KEY_WAKEUP_FROM_USB_DEV_SUSPEND
        uart_printf("key = %d \r\n",key);

        store_hid_key(key);
#endif
        return;
    }

#ifdef HANDLE_AO_ADC_KEY_WAKEUP_FROM_USB_DEV_SUSPEND
    if(get_always_on_adc_key_status())
    {
        if(hid_key.state[1])return ;
    }
#endif
    usb_dev_handler_t *phUsbHid = gUsbDevice.pData;
    usb_device_t *pdev_hid = phUsbHid->pData;

    //uart_printf("sent key 1 = %d \r\n",key);

    set_hid_key_buffer(key);
    hal_usb_set_ep_nak_status(AUDIO_HID_IN_EP, 0);

    hal_usb_transmit(pdev_hid, AUDIO_HID_IN_EP, hid_key_buffer, HID_ISO_IN_ENDP_PACKET_SIZE);
}


/**
  * @brief  usb_dev_hid_send_report
  *         Send HID Report
  * @param  pdev: device instance
  * @param  buff: pointer to report
  * @retval status
  */
uint8_t usb_dev_hid_send_report(usb_device_t  *pdev, uint8_t *report, uint16_t len)
{
    usb_dev_hid_handle_t     *hhid = (usb_dev_hid_handle_t*)pdev->pClassData;

    if (pdev->devState == USB_DEV_STATE_CONFIGURED )
    {
        //if(hhid->state == HID_IDLE)
        {
            hhid->state = HID_BUSY;
            hal_usb_transmit(pdev,AUDIO_HID_IN_EP,report,len);
        }
    }
    return USBD_OK;
}

uint8_t usb_dev_hid_protocol_send_report	   (usb_device_t	*pdev, uint8_t *report, uint16_t len)
{
    usb_dev_hid_handle_t *cp_hid = (usb_dev_hid_handle_t*)pdev->pClassData;

    if (pdev->devState == USB_DEV_STATE_CONFIGURED )
    {
        if(cp_hid->state == HID_IDLE)
        {
            cp_hid->state = HID_BUSY;
            hal_usb_transmit(pdev, AUDIO_HID_IN_EP, report,len);
        }
    }
    return USBD_OK;
}


void usb_dev_hid_ep_clean_nak(uint8_t ep_enable)
{
    hid_commp_data_in = ep_enable;

    usb_dev_handler_t *phUsbHidCommp = gUsbDevice.pData;
    usb_device_t *pdev_hid_commp = phUsbHidCommp->pData;
    hal_usb_set_ep_nak_status(AUDIO_HID_IN_EP, ((~ep_enable)&0x1));

    if(hid_commp_data_in )
	{
        memset(hid_commp_buffer,0,64);
		blocksize_record = hid_commp_data_in_blocksize = hid_CommProtocol_filling_device_in_data(hid_commp_buffer);
        hal_usb_transmit(pdev_hid_commp, AUDIO_HID_IN_EP, hid_commp_buffer, 64);
		hid_commp_data_in_blocksize --;
    }
}

uint8_t usb_dev_hid_protocol_data_in_interrupt (usb_device_t *pdev,  uint8_t epnum)
{
        return USBD_OK;
}
uint8_t usb_dev_hid_protocol_data_in_timer(void)
{
        usb_device_t *pdev = &gUsbDevice;
	if((hid_commp_data_in)&&(hid_commp_data_in_blocksize > 0)) {
		((usb_dev_hid_handle_t *)pdev->pClassData)->state = HID_IDLE;
	}
	else
    {
        ((usb_dev_hid_handle_t *)pdev->pClassData)->state = HID_BUSY;
		hid_CommProtocol_set_device_data_idle(0);
        hid_commp_data_in = 0 ;
        blocksize_record = 0;
		return USBD_OK;
    }

    uint16_t data_pos  = (blocksize_record - hid_commp_data_in_blocksize)*64;
	if(data_pos >= 960)
	{
		blocksize_record = hid_commp_data_in_blocksize = 0;
		return USBD_OK;
	}
	uart_printf("data_pos = %d \r\n", data_pos);
    usb_dev_hid_protocol_send_report(pdev, &hid_commp_buffer[data_pos], 64);
	if(hid_commp_data_in_blocksize > 0)
		hid_commp_data_in_blocksize --;
    return USBD_OK;
}

/**
  * @brief  usb_dev_hid_data_in
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */

///bool hid_comm_start = false;
uint8_t  usb_dev_hid_data_in (usb_device_t *pdev, uint8_t epnum)
{
    /* Ensure that the FIFO is empty before a new transfer, this condition could
   be caused by  a new transfer before the end of the previous transfer */
    usb_dev_hid_protocol_data_in_interrupt(pdev, epnum);
    return USBD_OK;
    // if(!set_hid_key_buffer(HID_key))return USBD_OK;

}

uint8_t  usb_dev_hid_data_out (usb_device_t *pdev,uint8_t epnum)
{
    int dmaIndex = hidrx_dmaPtr;

    if (epnum == AUDIO_HID_OUT_EP)
    {
    	((usb_dev_hid_handle_t *)pdev->pClassData)->state = HID_BUSY;
        if ((hidCPrxSts[dmaIndex].status & USB_SCDMA_BS_Msk) == 0)
        {
            int i;
            for (i = dmaIndex; i < USB_DEV_HIDRXBUF_NUM; i++)
            {
                /* Find the first "DMA_DONE" item from the list */
                if ((hidCPrxSts[i].status & USB_SCDMA_BS_Msk) == (2 << USB_SCDMA_BS_Pos))
                {
                    dmaIndex = i;
                    goto FIND_DMA_DONE;
                }
            }

            for (i = 0; i < dmaIndex; i++)
            {
                /* Find the first "DMA_DONE" item from the list */
                if ((hidCPrxSts[i].status & USB_SCDMA_BS_Msk) == (2 << USB_SCDMA_BS_Pos))
                {
                    dmaIndex = i;
                    break;
                }
            }
FIND_DMA_DONE:
            if( dmaIndex == USB_DEV_HIDRXBUF_NUM){
                dmaIndex = 0;
            }
        }

        int bytes = ((uint8_t*)hidCPrxSts[dmaIndex].buff)[1];
        if(bytes > 0)
            hid_CommProtocol_host_data_out((uint8_t*)hidCPrxSts[dmaIndex].buff, bytes + 2 );

        hidCPrxSts[dmaIndex].status = USB_SCDMA_IOC|USB_DEV_HIDRXBUF_SIZE;

        if ((dmaIndex >= USB_DEV_HIDRXBUF_NUM - 1)||(hidrx_dmaPtr >= USB_DEV_HIDRXBUF_NUM - 1))
        {
            hidCPrxSts[dmaIndex].status |= USB_SCDMA_L;
            hidrx_dmaPtr = 0;
        }
        else
            hidrx_dmaPtr++;
    }

    hal_usb_ep_receive(pdev, AUDIO_HID_OUT_EP, (uint8_t*)hidCPrxSts, USB_DEV_HIDRXBUF_SIZE, 0);

    return USBD_OK;
}

uint8_t  usb_dev_hid_ep0_rx_ready(usb_device_t *pdev)
{
    return USBD_OK;
}
#endif
