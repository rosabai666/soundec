/**
  ******************************************************************************
  * @file    usbd_ioreq.c
  * @brief   This file provides the IO requests APIs for control endpoints.
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

#include "usb_hal.h"
#include "usb_req.h"
#include "usbd_config.h"

#if 1//(USB_ENABLE)
#ifdef USB_DEV_KEYSIGHT_TEST_MODE
extern uint32_t usbd_test_mode_flag;
#endif
#if(SUPPORT_UAC_SELF_ADAPTION)
bool enumFlag = 0;
#endif
#if(TEST_DETECT_PC)
bool detectPcStarted = 0;
#endif
static void usb_get_descriptor(usb_device_t *pdev, usb_setup_req_t *req, uint8_t epnum)
{
    uint16_t len = 0;
    uint8_t *pbuf;

#if(SUPPORT_UAC_SELF_ADAPTION)
    enumFlag = 1;
#endif

    switch (req->wValue >> 8)
    {
#if (USB_DEV_LPM_ENABLED == 1U)
    case DESC_TYPE_BOS:
        pbuf = pdev->pDesc->GetBOSDescriptor(pdev->devSpeed, &len);
        break;
#endif
    case DESC_TYPE_DEV:
        pbuf = pdev->pDesc->GetDeviceDescriptor(pdev->devSpeed, &len);
        break;

    case DESC_TYPE_CONFIG:
        if(pdev->devSpeed == USB_SPEED_HIGH )
        {
            pbuf    = (uint8_t *)pdev->pClass[epnum]->GetHSConfigDescriptor(&len);
            pbuf[1] = DESC_TYPE_CONFIG;
        }
        else
        {
            pbuf    = (uint8_t *)pdev->pClass[epnum]->GetFSConfigDescriptor(&len);
            pbuf[1] = DESC_TYPE_CONFIG;
        }
#if(TEST_DETECT_PC)
        if(detectPcStarted == 0)
        {
        	detectPcStarted = 1;
        	if(req->wLength == 9)
        		uart_printf("phone\n");
        	else
        		uart_printf("pc\n");
        }
#endif
        break;

    case DESC_TYPE_STRING:
        switch ((uint8_t)(req->wValue))
        {
        case USB_DEV_IDX_LANGID_STR:
            pbuf = pdev->pDesc->GetLangIDStrDescriptor(pdev->devSpeed, &len);
            break;

        case USB_DEV_IDX_MFC_STR:
            pbuf = pdev->pDesc->GetManufacturerStrDescriptor(pdev->devSpeed, &len);
            break;

        case USB_DEV_IDX_PRODUCT_STR:
            pbuf = pdev->pDesc->GetProductStrDescriptor(pdev->devSpeed, &len);
            break;

        case USB_DEV_IDX_SERIAL_STR:
            pbuf = pdev->pDesc->GetSerialStrDescriptor(pdev->devSpeed, &len);
            break;

        case USB_DEV_IDX_CONFIG_STR:
            pbuf = pdev->pDesc->GetConfigurationStrDescriptor(pdev->devSpeed, &len);
            break;

        case USB_DEV_IDX_INTERFACE_STR:
            pbuf = pdev->pDesc->GetInterfaceStrDescriptor(pdev->devSpeed, &len);
            break;
        case USB_DEV_IDX_HEADPHONE:
            pbuf = pdev->pDesc->GetHeadphoneStrDescriptor(pdev->devSpeed, &len);
            break;
        case USB_DEV_IDX_MICROPHONE:
            pbuf = pdev->pDesc->GetMicrophoneStrDescriptor(pdev->devSpeed, &len);
            break;
        case USB_DEV_IDX_OTHER_STR:
            pbuf = pdev->pDesc->GetProfileStrDescriptor(pdev->devSpeed, &len);
//			usb_ctl_send_data(pdev, NULL, 0);
            break;

        default:
#if (USB_DEV_SUPPORT_USER_STRING == 1U)
            pbuf = pdev->pDesc->GetUsrStrDescriptor(pdev->devSpeed, (req->wValue), &len);
            break;
#endif
            len = MIN(len, req->wLength);
            usb_ctl_send_data (pdev, 0, len);
            return;

        }
        break;
    case DESC_TYPE_DEV_Q:
        if((pdev->devSpeed >= USB_SPEED_HIGH)||(pdev->devSpeed <= USB_SPEED_LOW))
        {
            pbuf = (uint8_t *)pdev->pClass[epnum]->GetDeviceQualifierDescriptor(&len);
        }
        else
        {
            usb_ctl_error(pdev, req);
            return;
        }
        break;
    case DESC_TYPE_OS_CONFIG:
        if(pdev->devSpeed >= USB_SPEED_HIGH )
        {
            pbuf   = (uint8_t *)pdev->pClass[epnum]->GetOtherSpeedConfigDescriptor(&len);
            pbuf[1] = DESC_TYPE_OS_CONFIG;
            break;
        }
        else
        {
            usb_ctl_error(pdev, req);
            return;
        }
    case USB_DEV_IDX_NULL_CONGFIG:
        usb_ctl_send_data (pdev, 0, len);
        return;

    default:
        usb_ctl_error(pdev, req);
        return;
    }

    if((len != 0U) && (req->wLength != 0U))
    {

        len = MIN(len, req->wLength);

        usb_ctl_send_data (pdev, pbuf, len);
    }

    if(req->wLength == 0U)
    {
        usb_ctl_send_status(pdev);
    }
}

static void usb_set_address(usb_device_t *pdev,  usb_setup_req_t *req)
{
    uint8_t devAddr;

    if ((req->wIndex == 0U) && (req->wLength == 0U) && (req->wValue < 128U))
    {
        devAddr = (uint8_t)(req->wValue) & 0x7FU;

        if (pdev->devState == USB_DEV_STATE_CONFIGURED)
        {
            usb_ctl_error(pdev, req);
        }
        else
        {
            pdev->devAddress = devAddr;
            hal_usb_set_address(pdev, devAddr);
            usb_ctl_send_status(pdev);

            if (devAddr != 0U)
            {
                pdev->devState = USB_DEV_STATE_ADDRESSED;
            }
            else
            {
                pdev->devState = USB_DEV_STATE_DEFAULT;
            }
        }
    }
    else
    {
        usb_ctl_error(pdev, req);
    }
}

static void usb_set_config(usb_device_t *pdev, usb_setup_req_t *req)
{
    static uint8_t cfgidx;
    cfgidx = (uint8_t)(req->wValue);

#if xxxxxerr
    if (cfgidx > USB_MAX_DEV_NUM_CONFIGURATION)
    {
        usb_ctl_error(pdev, req);
    }
    else
#endif
    {
        switch (pdev->devState)
        {
        case USB_DEV_STATE_ADDRESSED:
            if (cfgidx)
            {
                pdev->devConfig = cfgidx;
                pdev->devState = USB_DEV_STATE_CONFIGURED;
                if(usb_dev_set_class_cfg(pdev, cfgidx) == USBD_FAIL)
                {
                    usb_ctl_error(pdev, req);
                    return;
                }
                usb_ctl_send_status(pdev);
            }
            else
            {
                usb_ctl_send_status(pdev);
            }
            break;

        case USB_DEV_STATE_CONFIGURED:
            if (cfgidx == 0U)
            {
                pdev->devState = USB_DEV_STATE_ADDRESSED;
                pdev->devConfig = cfgidx;
                usb_dev_clr_class_cfg(pdev, cfgidx);
                usb_ctl_send_status(pdev);
            }
            else if (cfgidx != pdev->devConfig)
            {
                /* Clear old configuration */
                usb_dev_clr_class_cfg(pdev, (uint8_t)pdev->devConfig);

                /* set new configuration */
                pdev->devConfig = cfgidx;
                if(usb_dev_set_class_cfg(pdev, cfgidx) == USBD_FAIL)
                {
                    usb_ctl_error(pdev, req);
                    return;
                }
                usb_ctl_send_status(pdev);
            }
            else
            {
                usb_ctl_send_status(pdev);
            }
            break;

        default:
            usb_ctl_error(pdev, req);
            usb_dev_clr_class_cfg(pdev, cfgidx);
            break;
        }
    }
}


static void usb_get_config(usb_device_t *pdev, usb_setup_req_t *req)
{

    if (req->wLength != 1U)
    {
        usb_ctl_error(pdev, req);
    }
    else
    {
        switch (pdev->devState)
        {
        case USB_DEV_STATE_DEFAULT:
        case USB_DEV_STATE_ADDRESSED:
            pdev->devDefaultConfig = 0U;
            *(uint32_t*)send_buff = pdev->devDefaultConfig;
            usb_ctl_send_data (pdev, (uint8_t *)send_buff, 1U);
            break;

        case USB_DEV_STATE_CONFIGURED:
            *(uint32_t*)send_buff = pdev->devConfig;
            usb_ctl_send_data (pdev, (uint8_t *)send_buff, 1U);
            break;

        default:
            usb_ctl_error(pdev, req);
            break;
        }
    }
}


/**
* @brief  usb_get_status
*         Handle Get Status request
* @param  pdev: device instance
* @param  req: usb request
* @retval status
*/
static void usb_get_status(usb_device_t *pdev, usb_setup_req_t *req)
{
    switch (pdev->devState)
    {
    case USB_DEV_STATE_DEFAULT:
    case USB_DEV_STATE_ADDRESSED:
    case USB_DEV_STATE_CONFIGURED:
        if(req->wLength != 0x2U)
        {
            usb_ctl_error(pdev, req);
            break;
        }

#if ( USB_DEV_SELF_POWERED == 1U)
        pdev->devConfigStatus = USB_CONFIG_SELF_POWERED;
#else
        pdev->devConfigStatus = 0U;
#endif

        if (pdev->devRemoteWakeup)
        {
            pdev->devConfigStatus |= USB_CONFIG_REMOTE_WAKEUP;
        }
        *(uint32_t*)send_buff = pdev->devConfigStatus;

        usb_ctl_send_data (pdev, (uint8_t *)send_buff, 2U);
        break;

    default :
        usb_ctl_error(pdev, req);
        break;
    }
}

static void usb_set_feature(usb_device_t *pdev, usb_setup_req_t *req)

{
    pdev->devLpmConfig = USB_DEV_STATE_SUSPENDED ;
    if (req->wValue == USB_FEATURE_REMOTE_WAKEUP)
    {
        pdev->devRemoteWakeup = 1U;
        usb_ctl_send_status(pdev);
    }else{
#ifdef USB_DEV_KEYSIGHT_TEST_MODE
        usb_ctl_send_status(pdev);
        extern usb_dev_status_t enter_usb_dev_TEST_mode(uint32_t test_mode);
        enter_usb_dev_TEST_mode(HIBYTE(req->wIndex)); /*if ack err,please use polling usb_pdd_keysight_test*/
        usbd_test_mode_flag = HIBYTE(req->wIndex);
#endif
    }

}

static void usb_clear_feature(usb_device_t *pdev, usb_setup_req_t *req)
{
    switch (pdev->devState)
    {
    case USB_DEV_STATE_DEFAULT:
    case USB_DEV_STATE_ADDRESSED:
    case USB_DEV_STATE_CONFIGURED:
        if (req->wValue == USB_FEATURE_REMOTE_WAKEUP)
        {
            pdev->devRemoteWakeup = 0U;
            usb_ctl_send_status(pdev);
        }
        break;

    default :
        usb_ctl_error(pdev, req);
        break;
    }
    uart_printf("!!\n");
}


/**
* @brief  usb_std_dev_request
*         Handle usb standard requests
* @param  pdev: device instance
* @param  req: usb request
* @retval status
*/
usb_dev_status_t  usb_std_dev_request (usb_device_t *pdev, usb_setup_req_t  *req, uint8_t epnum)
{
    usb_dev_status_t ret = USBD_OK;
    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
    case USB_REQ_TYPE_CLASS:
    case USB_REQ_TYPE_VENDOR:
        pdev->pClass[epnum]->Setup(pdev, req);
        break;

    case USB_REQ_TYPE_STANDARD:

        switch (req->bRequest)
        {
        case USB_REQ_GET_DESCRIPTOR:

            usb_get_descriptor (pdev, req, epnum);
            break;

        case USB_REQ_SET_ADDRESS:
            usb_set_address (pdev, req);
            break;

        case USB_REQ_SET_CONFIGURATION:
            usb_set_config (pdev, req);
            break;

        case USB_REQ_GET_CONFIGURATION:
            usb_get_config (pdev, req);
            break;

        case USB_REQ_GET_STATUS:
            usb_get_status (pdev, req);
            break;


        case USB_REQ_SET_FEATURE:
            usb_set_feature (pdev, req);
            break;

        case USB_REQ_CLEAR_FEATURE:
            usb_clear_feature (pdev, req);
            break;

        default:
            usb_ctl_error(pdev, req);
            break;
        }
        break;

    default:
        usb_ctl_error(pdev, req);
        break;
    }

    return ret;
}

/**
* @brief  usb_std_interface_request
*         Handle standard usb interface requests
* @param  pdev: device instance
* @param  req: usb request
* @retval status
*/
usb_dev_status_t  usb_std_interface_request(usb_device_t *pdev, usb_setup_req_t  *req, uint8_t epnum)
{
    usb_dev_status_t ret = USBD_OK;

    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
    case USB_REQ_TYPE_CLASS:
    case USB_REQ_TYPE_VENDOR:
    case USB_REQ_TYPE_STANDARD:
        switch (pdev->devState)
        {
        case USB_DEV_STATE_DEFAULT:
        case USB_DEV_STATE_ADDRESSED:
        case USB_DEV_STATE_CONFIGURED:

            if (LOBYTE(req->wIndex) <= usb_interface_num)
            {
                ret = (usb_dev_status_t)pdev->pClass[epnum]->Setup (pdev, req);

                if ((req->wLength == 0U) && (ret == USBD_OK))
                {
                    usb_ctl_send_status(pdev);
                }
            }
            else
            {
                usb_ctl_error(pdev, req);
            }
            break;

        default:
            usb_ctl_error(pdev, req);
            break;
        }
        break;

    default:
        usb_ctl_error(pdev, req);
        break;
    }

    return USBD_OK;
}

/**
* @brief  usb_std_ep_request
*         Handle standard usb endpoint requests
* @param  pdev: device instance
* @param  req: usb request
* @retval status
*/
usb_dev_status_t  usb_std_ep_request(usb_device_t *pdev, usb_setup_req_t  *req, uint8_t epnum)
{

    uint8_t   ep_addr;
    usb_dev_status_t ret = USBD_OK;
    usb_dev_endpoint_t   *pep;
    ep_addr  = LOBYTE(req->wIndex);

    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {

    case USB_REQ_TYPE_CLASS:
    case USB_REQ_TYPE_VENDOR:
        pdev->pClass[epnum]->Setup (pdev, req);
        break;

    case USB_REQ_TYPE_STANDARD:
        /* Check if it is a class request */
        if ((req->bmRequest & 0x60U) == 0x20U)
        {
            ret = (usb_dev_status_t)pdev->pClass[epnum]->Setup (pdev, req);

            return ret;
        }

        switch (req->bRequest)
        {

        case USB_REQ_SET_FEATURE :

            switch (pdev->devState)
            {
            case USB_DEV_STATE_ADDRESSED:
                if ((ep_addr != 0x00U) && (ep_addr != 0x80U))
                {
                    hal_usb_stall_ep(pdev, ep_addr);
                    hal_usb_stall_ep(pdev, 0x80U);
                }
                else
                {
                    usb_ctl_error(pdev, req);
                }
                break;

            case USB_DEV_STATE_CONFIGURED:
                if (req->wValue == USB_FEATURE_EP_HALT)
                {
                    if ((ep_addr != 0x00U) && (ep_addr != 0x80U) && (req->wLength == 0x00U))
                    {
                        hal_usb_stall_ep(pdev, ep_addr);
                    }
                }
                usb_ctl_send_status(pdev);

                break;

            default:
                usb_ctl_error(pdev, req);
                break;
            }
            break;

        case USB_REQ_CLEAR_FEATURE :

            switch (pdev->devState)
            {
            case USB_DEV_STATE_ADDRESSED:
                if ((ep_addr != 0x00U) && (ep_addr != 0x80U))
                {
                    hal_usb_stall_ep(pdev, ep_addr);
                    hal_usb_stall_ep(pdev, 0x80U);
                }
                else
                {
                    usb_ctl_error(pdev, req);
                }
                break;

            case USB_DEV_STATE_CONFIGURED:
                if (req->wValue == USB_FEATURE_EP_HALT)
                {
                    if ((ep_addr & 0x7FU) != 0x00U)
                    {
                        hal_usb_clear_stall_ep(pdev, ep_addr);
                    }
                    usb_ctl_send_status(pdev);
                }
                break;

            default:
                usb_ctl_error(pdev, req);
                break;
            }
            break;

        case USB_REQ_GET_STATUS:
            switch (pdev->devState)
            {
            case USB_DEV_STATE_ADDRESSED:
                if ((ep_addr != 0x00U) && (ep_addr != 0x80U))
                {
                    usb_ctl_error(pdev, req);
                    break;
                }
                pep = ((ep_addr & 0x80U) == 0x80U) ? &pdev->epIn[ep_addr & 0x7FU]:\
                        &pdev->epOut[ep_addr & 0x7FU];

                pep->status = 0x0000U;
                *(uint32_t*)send_buff = pep->status;
                usb_ctl_send_data (pdev, (uint8_t *)send_buff, 2U);
                break;

            case USB_DEV_STATE_CONFIGURED:
                if((ep_addr & 0x80U) == 0x80U)
                {
                    if (pdev->epIn[ep_addr & 0xFU].isUsed == 0U)
                    {
                        usb_ctl_error(pdev, req);
                        break;
                    }
                }
                else
                {
                    if (pdev->epOut[ep_addr & 0xFU].isUsed == 0U)
                    {
                        usb_ctl_error(pdev, req);
                        break;
                    }
                }

                pep = ((ep_addr & 0x80U) == 0x80U) ? &pdev->epIn[ep_addr & 0x7FU]:\
                        &pdev->epOut[ep_addr & 0x7FU];

                if ((ep_addr == 0x00U) || (ep_addr == 0x80U))
                {
                    pep->status = 0x0000U;
                }
                else if(hal_usb_is_stall_ep(pdev, ep_addr))
                {
                    pep->status = 0x0001U;
                }
                else
                {
                    pep->status = 0x0000U;
                }

                *(uint32_t*)send_buff = pep->status;
                usb_ctl_send_data (pdev, (uint8_t *)send_buff, 2U);
                break;

            default:
                usb_ctl_error(pdev, req);
                break;
            }
            break;

        default:
            usb_ctl_error(pdev, req);
            break;
        }
        break;

    default:
        usb_ctl_error(pdev, req);
        break;
    }

    return ret;
}

/**
* @brief  usb_parse_setup_request
*         Copy buffer into setup structure
* @param  pdev: device instance
* @param  req: usb request
* @retval None
*/
void usb_parse_setup_request(usb_setup_req_t *req, uint8_t *pdata)
{
    req->bmRequest     = *(uint8_t *)  (pdata);
    req->bRequest      = *(uint8_t *)  (pdata +  1);
    req->wValue        = SWAPBYTE      (pdata +  2);
    req->wIndex        = SWAPBYTE      (pdata +  4);
    req->wLength       = SWAPBYTE      (pdata +  6);
}

/**
* @brief  usb_ctl_error
*         Handle USB low level Error
* @param  pdev: device instance
* @param  req: usb request
* @retval None
*/

void usb_ctl_error(usb_device_t *pdev, usb_setup_req_t *req)
{
    hal_usb_stall_ep(pdev, 0x80U);
    hal_usb_stall_ep(pdev, 0U);
}

/**
* @brief  usb_ctl_send_data
*         send data on the ctl pipe
* @param  pdev: device instance
* @param  buff: pointer to data buffer
* @param  len: length of data to be sent
* @retval status
*/
usb_dev_status_t usb_ctl_send_data (usb_device_t *pdev, uint8_t *pbuf, uint16_t len)
{
    if (pdev == NULL )
		return USBD_FAIL;

    pdev->ep0State = USB_DEV_EP0_DATA_IN;
    pdev->epIn[0].totalLen = len;
#if 0
    pdev->epIn[0].remLen   = len;
#else
    pdev->epIn[0].remLen   = 0;
#endif
    /* Start the transfer */
    hal_usb_transmit(pdev, 0x00U, pbuf, len);

    return USBD_OK;
}

/**
* @brief  usb_ctl_continue_send_data
*         continue sending data on the ctl pipe
* @param  pdev: device instance
* @param  buff: pointer to data buffer
* @param  len: length of data to be sent
* @retval status
*/
usb_dev_status_t usb_ctl_continue_send_data (usb_device_t *pdev, uint8_t *pbuf, uint16_t len)
{
    if (pdev == NULL )return USBD_FAIL ;
    /* Start the next transfer */
    hal_usb_transmit(pdev, 0x00U, pbuf, len);

    return USBD_OK;
}

/**
* @brief  usb_ctl_prepare_rx
*         receive data on the ctl pipe
* @param  pdev: device instance
* @param  buff: pointer to data buffer
* @param  len: length of data to be received
* @retval status
*/
usb_dev_status_t usb_ctl_prepare_rx (usb_device_t *pdev, uint8_t *pbuf,  uint16_t len)
{
    if (pdev == NULL )return USBD_FAIL ;
    /* Set EP0 State */
    pdev->ep0State = USB_DEV_EP0_DATA_OUT;
    pdev->epOut[0].totalLen = len;
#if 1
    pdev->epOut[0].remLen   = len;
#else
    pdev->epOut[0].remLen   = 0;
#endif
    /* Start the transfer */
    hal_usb_ep_receive(pdev, 0U, pbuf, len, 0);

    return USBD_OK;
}

/**
* @brief  usb_ctl_continue_rx
*         continue receive data on the ctl pipe
* @param  pdev: device instance
* @param  buff: pointer to data buffer
* @param  len: length of data to be received
* @retval status
*/
usb_dev_status_t usb_ctl_continue_rx (usb_device_t *pdev, uint8_t *pbuf, uint16_t len)
{
    if (pdev == NULL )return USBD_FAIL ;
    hal_usb_ep_receive(pdev, 0U, pbuf, len, 0);

    return USBD_OK;
}

/**
* @brief  usb_ctl_send_status
*         send zero lzngth packet on the ctl pipe
* @param  pdev: device instance
* @retval status
*/
usb_dev_status_t usb_ctl_send_status (usb_device_t *pdev)
{
    if (pdev == NULL )return USBD_FAIL ;
    /* Set EP0 State */
    pdev->ep0State = USB_DEV_EP0_STATUS_IN;

    /* Start the transfer */
    hal_usb_transmit(pdev, 0x00U, NULL, 0U);

    return USBD_OK;
}

/**
* @brief  usb_ctl_receive_status
*         receive zero lzngth packet on the ctl pipe
* @param  pdev: device instance
* @retval status
*/
usb_dev_status_t usb_ctl_receive_status (usb_device_t *pdev)
{
    if (pdev == NULL )return USBD_FAIL ;
    /* Set EP0 State */
    pdev->ep0State = USB_DEV_EP0_STATUS_OUT;

    /* Start the transfer */
    hal_usb_ep_receive(pdev, 0U, NULL, 0U, 0);

    return USBD_OK;
}
#endif

