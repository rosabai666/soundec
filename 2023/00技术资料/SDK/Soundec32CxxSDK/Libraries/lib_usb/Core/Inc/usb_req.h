
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DEV_IOREQ_H
#define __USB_DEV_IOREQ_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usb_def.h"
#include "usb_dev.h"
#if 1//(USB_ENABLE)

usb_dev_status_t usb_std_dev_request (usb_device_t  *pdev, usb_setup_req_t  *req, uint8_t epnum);
usb_dev_status_t usb_std_interface_request (usb_device_t  *pdev, usb_setup_req_t  *req, uint8_t epnum);
usb_dev_status_t usb_std_ep_request  (usb_device_t  *pdev, usb_setup_req_t  *req, uint8_t epnum);
void             usb_parse_setup_request (usb_setup_req_t *req, uint8_t *pdata);
void             usb_ctl_error  (usb_device_t  *pdev, usb_setup_req_t *req);
usb_dev_status_t usb_ctl_send_data (usb_device_t *pdev, uint8_t *pbuf, uint16_t len);
usb_dev_status_t usb_ctl_continue_send_data (usb_device_t  *pdev, uint8_t *pbuf, uint16_t len);
usb_dev_status_t usb_ctl_prepare_rx (usb_device_t *pdev, uint8_t *pbuf, uint16_t len);
usb_dev_status_t usb_ctl_continue_rx (usb_device_t  *pdev, uint8_t *pbuf, uint16_t len);
usb_dev_status_t usb_ctl_send_status (usb_device_t  *pdev);
usb_dev_status_t usb_ctl_receive_status (usb_device_t  *pdev);

#endif

#ifdef __cplusplus
}
#endif

#endif /* __USB_DEV_IOREQ_H */
