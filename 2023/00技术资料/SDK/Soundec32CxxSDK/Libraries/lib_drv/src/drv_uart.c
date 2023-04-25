/**
***********************************************************
 *
 * @file name	: drv_uart.c
 * @author		: Gaoshuai
 * @version		: Vxx.xx
 * @date		: 2022-01-17
 * @brief		: uart driver module
 *
***********************************************************
 * @attention
 *
 * Copyright (c) 2022 Soundec Co., Ltd.
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
#include "common.h"
#include "hal.h"
#include "hal_uart.h"
#include "hal_interrupt.h"
#include "drv_uart.h"
#include "hal_sysctrl.h"
#include "SaveRestoreVectorRegisters.h"

/* Private typedef -------------------------------------------------------------*/
/* Private macro --------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------------*/

#if UART_ENABLE == 1

#if (UART_TX_INT_ENABLE==1 || UART_RECEIVE ==1 || UART_RX_INT_ENABLE == 1)
// notice : rx and tx use same ram,it may be cause error 
// if you use rx_int and tx_int in the same time
uart_queue_t uqueue[1] ;//{0};
#endif

#if (UART_TX_INT_ENABLE==1 || UART_RX_INT_ENABLE == 1)
uart_int_callback uart_callback[5] = {0};
static void uart_isr_handler(void *arg);
extern void timeout_callback(void);
#endif

#if UART_TX_INT_ENABLE == 1
extern void tx_callback(void);
#endif

#if UART_RX_INT_ENABLE  == 1
extern void rx_callback(void);
#endif
//extern void modem_callback(void);
//extern void rx_line_stat_callback(void);

/* Private functions -----------------------------------------------------------*/
/**
 * @brief	The user initialized the Uart module
 			Parameters in uart_handel_t and uart_cfg_t
 * @param	uart  UART handeler
 * @return	hal status
 */
void uart_init(void)
{

	uint32_t clkGate = hal_sysctrl_get_clock_gate();
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_UART);
	hal_sysctrl_set_clock_gate(clkGate);
	uart_cfg_t config;
	config.baudRate 	= UART_BUARD_RATE;
	config.dataLength   = UART_DATALENGRTH_8BIT;
	config.stopBits 	= UART_STOPBITS_1BIT;
	config.parityEnable = UART_PARITY_DISABLE;
	config.paritySelect = UART_PARITY_NO;			//UART_Parity_Even;
	config.breakControl = UART_BREAK_NORMAL;
	config.rxFifoLevel  = UART_R_FIFO_LEVEL_8B;	//UART_R_FIFO_Level_1B;
	config.txFifoLevel  = UART_T_FIFO_LEVEL_8B;	//UART_T_FIFO_Level_0B;
	config.inteConfig   = UART_ISR_ALL_DISABLE;
	hal_uart_init(&config);

}

/**
 * @brief	Deinitailizes the uart module
 * 	 The UART clock is disabled.
 * @return	None
 */
void uart_deinit(void)
{
	uint32_t clkGate = hal_sysctrl_get_clock_gate();
	CLRBIT(clkGate, HAL_SYSCTRL_CLKGT_UART);
	hal_sysctrl_set_clock_gate(clkGate);
}

/**
 * @brief	Reinitailizes the uart module
 * 	 The UART clock is enabled and reconfiguration parameter.
 * @return	None
 */
void uart_reinit(void)
{
	uint32_t clkGate = hal_sysctrl_get_clock_gate();
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_UART);
	hal_sysctrl_set_clock_gate(clkGate);
	uart_init();
}

/**
 * @brief	 The transmit FIFO is used to store the data to be transmitted. The user only needs to write data to this register,
                and the UART will perform data transmission according to the relevant settings of transmission control. The status
                of this FIFO has related status register flags, including LSR [6] and LSR [5]
 * @param[in]	ch   Sent BYTE.
 * @return	None
 */
void uart_tx_byte(uint8_t ch)
{
	/* Waiting to send data register is empty */
    while (hal_uart_get_status(UART_FLAG_TXE) != HAL_STATUS_OK);
    hal_uart_tx_data(ch);
}

/**
 * @brief	 Send multiple bytes
 * @note	NULL
 * @param[in]	array   Sent BYTE array.
 * @param[in]	num    Sent BYTE number.
 * @return	None
 */
void uart_tx_array(uint8_t *array, uint16_t num)
{
    while(num)
    {
    	if(hal_uart_get_status(UART_FLAG_TXE) == HAL_STATUS_OK)
    	{
    		/* Send a byte of data to the UART*/
    		hal_uart_tx_data(*array);
    		array++;
    		num--;
    	}
    }
	/* wait transmitte complete */
    while(hal_uart_get_status(UART_FLAG_TC) != HAL_STATUS_OK);
}

/**
 * @brief	uart transmitter string
 * @note	null
 * @param[in] 	*str transmitted *str
 * @return	None
 */
void uart_tx_string(char *str)
{
    unsigned int k=0;
    do
    {
        uart_tx_byte(*(str + k));
        k++;
    } while(*(str + k)!='\0');
  /* wait transmitte complete */
    while(hal_uart_get_status(UART_FLAG_TC) != HAL_STATUS_OK);
}

#if (UART_RECEIVE == 1)
/**
 * @brief  The receive FIFO is used to store the data received by the UART.
 * 		   The status of this FIFO is reflected by the interrupt identification register.
           The user can get the data received by the UART by reading the RXFIFO.
 * @note  None.
 * @return	None
 */
uint8_t uart_rx_byte(void)
{
	uint8_t data;
	while (hal_uart_get_status(UART_FLAG_RXNE) != HAL_STATUS_OK);
	data = hal_uart_rx_data();
	return data;
}
/**
 * @brief	 Receive multiple bytes
 * @note	 NULL
 * @param	num    Receive BYTE number.
 * @return	None
 */
void uart_rx_array(uint16_t num)
{
	uart_queue_t *queue = uqueue;

	queue->head = queue->tail =  0;
	if(num > queue->size)
		num = queue->size;

	while(num > 0)
	{
		if(hal_uart_get_status(UART_FLAG_RXNE) == HAL_STATUS_OK)
		{
			queue->pdata[queue->tail] = hal_uart_rx_data();
			queue->tail++;
			num--;
		}
	}
}

/**
 * @brief	 Transmit data in the queue.
 * @note	 NULL
 * @param 	 None
 * @return	None
 */
void uart_out_queue(void)
{
	uart_queue_t *queue = uqueue;
	uint8_t arr[500];// notice size
	int j = 0;
	while(queue->head < queue->size)
	{
		arr[j] = queue->pdata[queue->head];
		queue->head++;
		j++;
	}
	uart_tx_array(arr, queue->size);
}

/**
 * @brief	Check data in the queue.
 * @note	 NULL
 * @param 	 None
 * @return	None
 */
void uart_check_queue(void)
{
	uart_queue_t *queue = uqueue;
	int i = 0;
	while(queue->head < queue->size)
	{
		i++;
		if(queue->pdata[queue->head] != i)
			uart_printf("error %d rx %d\r\n",i, queue->pdata[queue->head]);
		queue->head++;
	}
}
#endif
/********************************INTERRUPT FUNCTION************************************/
#if (UART_TX_INT_ENABLE == 1 || UART_RX_INT_ENABLE == 1)
/**
 * @brief  Initialize interrupt of uart.
 * @code
 * 	hal_uart_interrupt_init()
 */
void uart_interrupt_init(void)
{
    hal_interrupt_register_isr_handler(UART_IRQn, uart_isr_handler);
    hal_interrupt_enable_irq(UART_IRQn);
    uart_register_callback(UART_TIMEOUT_CALLBACK_ID, timeout_callback);
}

#if UART_RX_INT_ENABLE
/**
 * @brief	Register the rx callback funtion and Enable uart interrupt.
 * @note    null
 * @param[in]	num    receive data length
 * @return	None
 */
void uart_receive_interrupt_config(uint16_t num)
{
	uart_queue_t *queue = uqueue;

	queue->head = queue->tail =  0;
	queue->size = num;

	uart_register_callback(UART_RX_CALLBACK_ID, rx_callback);
	hal_uart_int_config(HAL_UART_IER_RDLIENABLE_MASK,TURN_ON);
}
#endif

#if UART_TX_INT_ENABLE
/**
 * @brief	Register the tx callback funtion and Enable uart interrupt.
 * @note    null
 * @param[in]	txBuffer	Point to sent data.
 * @param[in]	num		transmit data length
 * @return	None
 */
void uart_transmit_interrupt_config(uint8_t *txBuffer, uint16_t num)
{
	uart_queue_t *queue = uqueue;

	queue->head = queue->tail =  0;
	if(num > queue->size)
		num = queue->size;

	while(num)
	{
		queue->pdata[queue->tail] = *txBuffer;
		txBuffer++;
		queue->tail++;
		num--;
	}
	uart_register_callback(UART_TX_CALLBACK_ID, tx_callback);
	hal_uart_int_config(HAL_UART_IER_THREIENABLE_MASK,TURN_ON);//enable uart tx interrupt
}
#endif

#if UART_RX_INT_ENABLE
/**
 * @brief	This function is interrupt mode receive data.
 * @note    null
 * @return	None
 */
void uart_receive_interrupt(void)
{
	uart_queue_t *queue = uqueue;

	while(hal_uart_get_status(UART_FLAG_RXNE) == HAL_STATUS_OK && queue->tail < queue->size)
	{
		queue->pdata[queue->tail] = hal_uart_rx_data();
		queue->tail++;
	}
}

/**
 * @brief	This function is return rx buffer ptr.
 * @note    null
 * @return	rx buffer ptr
 */
uart_queue_t *uart_get_rx_buffter_ptr(void)
{
	return uqueue;
}

#endif

#if UART_TX_INT_ENABLE
/**
 * @brief	This function is interrupt mode send data.
 * @note    null
 * @return	None
 */
void uart_transmit_interrupt(void)
{
	uart_queue_t *queue = uqueue;

	while(queue->head < queue->size)
	{
		if(hal_uart_get_status(UART_FLAG_TXE) == HAL_STATUS_OK)
		{
		    hal_uart_tx_data(queue->pdata[queue->head]);
		   	queue->head++;
		}
	}
}
#endif
/**
 * @brief	Register a user UART callback function
 *
 * @param	uart        uart handle
 * @param	callbacID   ID of the callback to be registered.
 			this parameter can be one of following values:
 			@arg UART_TX_CALLBACK_ID
    		@arg UART_RX_CALLBACK_ID
    		@arg UART_RX_LINE_STATUS_CALLBACK_ID
    		@arg UART_TIMEOUT_CALLBACK_ID
    		@arg UART_MODEM_CALLBACK_ID
 * @paramc	pCallback  pointer to the callback function
 * @return	None
 */
void uart_register_callback(hal_uart_interrupt_type_t callbacID, uart_int_callback pCallback)
{
	ASSERT(callbacID < 0x05);
	uart_callback[callbacID] = pCallback;
}

/***********************INTERRUPT HANDLER*********************/

/*
 * @brief	 interrupt service function
 * @note   There are four maskable interrupt sources in the UART module.
                 It should be noted that the timeout interrupt of the receiving module
                 is always valid, and no configuration is required.
 * @code
 * 	uart_isr_handler()
 */
static void uart_isr_handler(void *arg)
{
	SaveVectors(UART_IRQn);
	uint8_t stat = hal_uart_get_int_status();
	switch(stat)
	{
		case UART_LINE_STATUS:
			if(uart_callback[UART_RX_LINE_STATUS_CALLBACK_ID])
				uart_callback[UART_RX_LINE_STATUS_CALLBACK_ID]();
			hal_uart_int_config(HAL_UART_IER_RLSIENABLE_MASK,TURN_OFF);//disable receiver data status interrupt
			break;
		case UART_RX_FIFO_ISR:
			if(uart_callback[UART_RX_CALLBACK_ID])
				uart_callback[UART_RX_CALLBACK_ID]();
			break;
		case UART_TX_FIFO_ISR:
			if(uart_callback[UART_TX_CALLBACK_ID])
				uart_callback[UART_TX_CALLBACK_ID]();
			hal_uart_int_config(HAL_UART_IER_THREIENABLE_MASK,TURN_OFF);//diable tx fifo interrupt
			break;
		case UART_RX_TIMEOUT:
			if(uart_callback[UART_TIMEOUT_CALLBACK_ID])
				uart_callback[UART_TIMEOUT_CALLBACK_ID]();
			 break;
		case UART_MODEM_STATUS:
			if(uart_callback[UART_MODEM_CALLBACK_ID])
				uart_callback[UART_MODEM_CALLBACK_ID]();
			hal_uart_int_config(HAL_UART_IER_MSTENABLE_MASK,TURN_OFF);//disable modem status change interrupt
			 break;
		default:
			break;
	}
	RestoreVectors(UART_IRQn);
}
#endif

/**
 * @brief	Redirect the printf function.
 * @note    null
 * @param[in]
 * @return	None
 */
int uart_printf(const char *fmt, ...)
{
    va_list list;
    int32_t log_size;
    char buffer[LOG_SIZE_MAX];

    va_start(list, fmt);
    log_size = vsnprintf(buffer, LOG_SIZE_MAX, fmt, list);
    if (log_size < 0) {
        va_end(list);
        return 0;
    }
    if ((uint32_t)log_size >= sizeof(buffer)) {
        log_size = sizeof(buffer) - 1;
		 va_end(list);
        return 0;
    }
	uart_tx_string(buffer);
    va_end(list);
    return log_size;
}
#else
int uart_printf(const char *fmt, ...)
{
    return 0;
}
#endif
