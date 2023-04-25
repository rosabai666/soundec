/**
***********************************************************
 *
 * @file name	: usb_hal.h
 * @author		: nick
 * @version		: Vxx.xx
 * @date		: 2020-07-09
 * @brief		:
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
#ifndef __USB_HAL_CONF_H__
#define __USB_HAL_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "usb_def.h"

#if 1//(USB_ENABLE)

#define USB_MASK_INTERRUPT(__INSTANCE__, __INTERRUPT__)     ((__INSTANCE__)->GINTMSK &= ~(__INTERRUPT__))
#define USB_UNMASK_INTERRUPT(__INSTANCE__, __INTERRUPT__)   ((__INSTANCE__)->GINTMSK |= (__INTERRUPT__))

#define CLEAR_IN_EP_INTR(__EPNUM__, __INTERRUPT__)          (USBx_INEP(__EPNUM__)->DIEPINT = (__INTERRUPT__))
#define CLEAR_OUT_EP_INTR(__EPNUM__, __INTERRUPT__)         (USBx_OUTEP(__EPNUM__)->DOEPINT = (__INTERRUPT__))

/********************  Bit definition for USB_OTG_DIEPINT register  ********************/
#define USB_OTG_DIEPINT_XFRC_Pos                 (0U)
#define USB_OTG_DIEPINT_XFRC_Msk                 (0x1UL << USB_OTG_DIEPINT_XFRC_Pos) /*!< 0x00000001 */
#define USB_OTG_DIEPINT_XFRC                     USB_OTG_DIEPINT_XFRC_Msk      /*!< Transfer completed interrupt */
#define USB_OTG_DIEPINT_EPDISD_Pos               (1U)
#define USB_OTG_DIEPINT_EPDISD_Msk               (0x1UL << USB_OTG_DIEPINT_EPDISD_Pos) /*!< 0x00000002 */
#define USB_OTG_DIEPINT_EPDISD                   USB_OTG_DIEPINT_EPDISD_Msk    /*!< Endpoint disabled interrupt */
#define USB_OTG_DIEPINT_AHBERR_Pos               (2U)
#define USB_OTG_DIEPINT_AHBERR_Msk               (0x1UL << USB_OTG_DIEPINT_AHBERR_Pos) /*!< 0x00000004 */
#define USB_OTG_DIEPINT_AHBERR                   USB_OTG_DIEPINT_AHBERR_Msk   /*!< AHB Error (AHBErr) during an IN transaction */
#define USB_OTG_DIEPINT_TOC_Pos                  (3U)
#define USB_OTG_DIEPINT_TOC_Msk                  (0x1UL << USB_OTG_DIEPINT_TOC_Pos) /*!< 0x00000008 */
#define USB_OTG_DIEPINT_TOC                      USB_OTG_DIEPINT_TOC_Msk       /*!< Timeout condition */
#define USB_OTG_DIEPINT_ITTXFE_Pos               (4U)
#define USB_OTG_DIEPINT_ITTXFE_Msk               (0x1UL << USB_OTG_DIEPINT_ITTXFE_Pos) /*!< 0x00000010 */
#define USB_OTG_DIEPINT_ITTXFE                   USB_OTG_DIEPINT_ITTXFE_Msk    /*!< IN token received when TxFIFO is empty */
#define USB_OTG_DIEPINT_INEPNM_Pos               (5U)
#define USB_OTG_DIEPINT_INEPNM_Msk               (0x1UL << USB_OTG_DIEPINT_INEPNM_Pos) /*!< 0x00000004 */
#define USB_OTG_DIEPINT_INEPNM                   USB_OTG_DIEPINT_INEPNM_Msk   /*!< IN token received with EP mismatch */
#define USB_OTG_DIEPINT_INEPNE_Pos               (6U)
#define USB_OTG_DIEPINT_INEPNE_Msk               (0x1UL << USB_OTG_DIEPINT_INEPNE_Pos) /*!< 0x00000040 */
#define USB_OTG_DIEPINT_INEPNE                   USB_OTG_DIEPINT_INEPNE_Msk    /*!< IN endpoint NAK effective */
#define USB_OTG_DIEPINT_TXFE_Pos                 (7U)
#define USB_OTG_DIEPINT_TXFE_Msk                 (0x1UL << USB_OTG_DIEPINT_TXFE_Pos) /*!< 0x00000080 */
#define USB_OTG_DIEPINT_TXFE                     USB_OTG_DIEPINT_TXFE_Msk      /*!< Transmit FIFO empty */
#define USB_OTG_DIEPINT_TXFIFOUDRN_Pos           (8U)
#define USB_OTG_DIEPINT_TXFIFOUDRN_Msk           (0x1UL << USB_OTG_DIEPINT_TXFIFOUDRN_Pos) /*!< 0x00000100 */
#define USB_OTG_DIEPINT_TXFIFOUDRN               USB_OTG_DIEPINT_TXFIFOUDRN_Msk /*!< Transmit Fifo Underrun */
#define USB_OTG_DIEPINT_BNA_Pos                  (9U)
#define USB_OTG_DIEPINT_BNA_Msk                  (0x1UL << USB_OTG_DIEPINT_BNA_Pos) /*!< 0x00000200 */
#define USB_OTG_DIEPINT_BNA                      USB_OTG_DIEPINT_BNA_Msk       /*!< Buffer not available interrupt */
#define USB_OTG_DIEPINT_PKTDRPSTS_Pos            (11U)
#define USB_OTG_DIEPINT_PKTDRPSTS_Msk            (0x1UL << USB_OTG_DIEPINT_PKTDRPSTS_Pos) /*!< 0x00000800 */
#define USB_OTG_DIEPINT_PKTDRPSTS                USB_OTG_DIEPINT_PKTDRPSTS_Msk /*!< Packet dropped status */
#define USB_OTG_DIEPINT_BERR_Pos                 (12U)
#define USB_OTG_DIEPINT_BERR_Msk                 (0x1UL << USB_OTG_DIEPINT_BERR_Pos) /*!< 0x00001000 */
#define USB_OTG_DIEPINT_BERR                     USB_OTG_DIEPINT_BERR_Msk      /*!< Babble error interrupt */
#define USB_OTG_DIEPINT_NAK_Pos                  (13U)
#define USB_OTG_DIEPINT_NAK_Msk                  (0x1UL << USB_OTG_DIEPINT_NAK_Pos) /*!< 0x00002000 */
#define USB_OTG_DIEPINT_NAK                      USB_OTG_DIEPINT_NAK_Msk       /*!< NAK interrupt */


/**
  * @brief  Initializes the USB Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */
usb_dev_status_t hal_usb_init(usb_device_t *pdev);

/**
  * @brief  De-Initializes the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */
usb_dev_status_t hal_usb_de_init(usb_device_t *pdev);

/**
  * @brief  Starts the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */
usb_dev_status_t hal_usb_start(usb_device_t *pdev);

/**
  * @brief  Stops the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */
usb_dev_status_t hal_usb_stop(usb_device_t *pdev);

/**
  * @brief  Opens an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @param  ep_type: Endpoint Type
  * @param  ep_mps: Endpoint Max Packet Size
  * @retval USBD Status
  */
usb_dev_status_t hal_usb_open_ep(usb_device_t *pdev, uint8_t ep_addr,  uint8_t ep_type, uint16_t ep_mps);

/**
  * @brief  Closes an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
usb_dev_status_t hal_usb_close_ep(usb_device_t *pdev, uint8_t ep_addr);

/**
  * @brief  Flushes an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
usb_dev_status_t hal_usb_flush_ep(usb_device_t *pdev, uint8_t ep_addr);

/**
  * @brief  Sets a Stall condition on an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
usb_dev_status_t hal_usb_stall_ep(usb_device_t *pdev, uint8_t ep_addr);

/**
  * @brief  Clears a Stall condition on an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
usb_dev_status_t hal_usb_clear_stall_ep(usb_device_t *pdev, uint8_t ep_addr);

/**
  * @brief  Returns Stall condition.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval Stall (1: Yes, 0: No)
  */
uint8_t hal_usb_is_stall_ep(usb_device_t *pdev, uint8_t ep_addr);

/**
  * @brief  Assigns a USB address to the device.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
usb_dev_status_t hal_usb_set_address(usb_device_t *pdev, uint8_t devAddr);

/**
  * @brief  Transmits data over an endpoint.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @param  pbuf: Pointer to data to be sent
  * @param  size: Data size
  * @retval USBD Status
  */
usb_dev_status_t hal_usb_transmit(usb_device_t *pdev, uint8_t ep_addr, uint8_t *pbuf, uint16_t size);

/**
  * @brief  reception from an endpoint.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @param  pbuf: Pointer to data to be received
  * @param  size: Data size
  * @retval USBD Status
  */
usb_dev_status_t hal_usb_ep_receive(usb_device_t *pdev, uint8_t ep_addr, uint8_t *pbuf,  uint16_t size, uint32_t frmNum);
usb_dev_status_t hal_usb_suspend(usb_device_t  *pdev);
usb_dev_status_t hal_usb_sof(usb_device_t  *pdev);
usb_dev_status_t hal_usb_device_disconnected(usb_device_t  *pdev);
void  hal_usb_set_ep_nak_status(uint32_t endpoint, uint32_t sc);
void usb_isr_handler(void *arg);
usb_dev_status_t hal_usb_set_rx_fiFo(usb_dev_handler_t *pUsbHandler, uint16_t size);
usb_dev_status_t hal_usb_set_tx_fiFo(usb_dev_handler_t *pUsbHandler, uint8_t fifo, uint16_t size);
usb_dev_status_t hal_usb_ep_close(usb_dev_handler_t *pUsbHandler, uint8_t ep_addr);
usb_dev_status_t hal_usb_ep_clr_stall(usb_dev_handler_t *pUsbHandler, uint8_t ep_addr);
usb_dev_status_t hal_usb_ep_set_stall(usb_dev_handler_t *pUsbHandler, uint8_t ep_addr);
void hal_delay(__IO uint32_t Delay);

uint32_t usb_get_lib_version(void);
uint32_t hal_usb_get_speed_init_count(void);
void hal_usb_set_speed_init_count_zero(void);

#endif

#ifdef __cplusplus
}
#endif

#endif /* __USB_HAL_CONF_H__ */

