/** 
***********************************************************
 * 
 * @file name	: Hid_CommProtocol.h
 * @author		: nick
 * @version		: Vxx.xx
 * @date		: 2021-02-02
 * @brief		: usbd hid communication protocol
 * 
***********************************************************
 * @attention
 * 
 * Copyright (c) 2021 Soundec Co., Ltd. 
 * All rights reserved.
 * 
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user
 * who uses the software, his/her employer (which must be your employer
 * and Soundec Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 * 
***********************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HID_COMMPROTOCOL_H__
#define __HID_COMMPROTOCOL_H__

/* Includes ------------------------------------------------------------------*/

#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif

#if (USB_ENABLE)

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


extern void hid_CommProtocol_host_data_out( uint8_t *pBuf, uint32_t len ) ;
extern uint8_t hid_CommProtocol_filling_device_in_data(uint8_t * __restrict hid_commpbuff);
extern	void hid_CommProtocol_host_data_printf(void);

extern int8_t hid_CommProtocol_rx_output(U8 *revData) ;
extern int8_t hid_CommProtocol_tx_input(U8 *sentData ,uint32_t tx_len);
extern void hid_CommProtocol_set_device_data_idle(uint8_t enable) ;
void hid_CommProtocol_active_system_dfu_event(uint8_t *data) ;
int8_t hid_CommProtocol_rx_output_all(U8 *revData,U8 *length);


#ifdef __cplusplus
}
#endif

#endif

#endif  /* __HID_COMMPROTOCOL_H__ */



