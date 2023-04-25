/** 
***********************************************************
 * 
 * @file name	: drv_uart.h
 * @author	: nick
 * @version	: V01.00
 * @date		: 2020-02-19
 * @brief		: uart driver 
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_UART_H__
#define __DRV_UART_H__

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <xtensa/xtruntime.h>
#include <xtensa/config/core.h>
#include <stdarg.h>
#include <common.h>
#include "hal_uart.h"


#ifdef __cplusplus
extern "C" {
#endif
#if UART_ENABLE == 1

typedef struct
{
	uint8_t  pdata[UART_QUEUE_SIZE];
	uint16_t size;
	uint16_t head, tail;
} uart_queue_t;
/* Private typedef -----------------------------------------------------------*/
/*uart callback id enumeration definition*/
typedef enum
{
	UART_MODEM_CALLBACK_ID           = 0x00,
	UART_TX_CALLBACK_ID 			 = 0x01,
	UART_RX_CALLBACK_ID 		     = 0x02,
	UART_RX_LINE_STATUS_CALLBACK_ID  = 0x03,
	UART_TIMEOUT_CALLBACK_ID         = 0x04,

} hal_uart_interrupt_type_t;
/* Private macro -------------------------------------------------------------*/
#define LOG_SIZE_MAX (256)

/* pointer to an uart callback funtion*/
typedef void (*uart_int_callback) (void);
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

int uart_printf(const char *format, ...);

void uart_init(void);
void uart_deinit(void);
void uart_reinit(void);

void uart_tx_array(uint8_t *array, uint16_t num);
void uart_tx_byte(uint8_t ch);
void uart_tx_string(char *str);
uint8_t uart_rx_byte(void);
void uart_rx_array(uint16_t num);
void uart_out_queue(void);
void uart_check_queue(void);

void uart_receive_interrupt(void);
void uart_transmit_interrupt(void);

void uart_interrupt_init(void);
void uart_transmit_interrupt_config(uint8_t *txBuffer, uint16_t num);
void uart_receive_interrupt_config(uint16_t num);
void uart_register_callback(hal_uart_interrupt_type_t callbacID, uart_int_callback pCallback);

void uart_clean_rx_fifo(void);
void uart_clean_tx_fifo(void);

uart_queue_t *uart_get_rx_buffter_ptr(void);
/* Private functions ---------------------------------------------------------*/

#endif
#ifdef __cplusplus
}
#endif

#endif  /* __DRV_UART_H__ */


