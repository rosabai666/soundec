/** 
***********************************************************
 * 
 * @file name	: hal_uart.h
 * @author		: Gaoshuai
 * @version		: Vxx.xx
 * @date		: 2022-01-10
 * @brief		: uart config paramers
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_UART_H__
#define __HAL_UART_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "types.h"
#include "hal.h"


#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/

/**
 * @brief	main baudrate support list
 */
typedef enum
{
	BAUDRATE_9600 	 = 9600,
	BAUDRATE_19200   = 19200,
	BAUDRATE_38400	 = 38400,
	BAUDRATE_57600	 = 57600,
	BAUDRATE_115200  = 115200,
	BAUDRATE_921600  = 921600,
	BAUDRATE_1843200 = 1843200,
	BAUDRATE_2M      = 2000000,
	BAUDRATE_3M      = 3000000
} hal_uart_baudrate_t;


/**
 * @brief  uart interrupt enable/disable list
 */
typedef enum
{
	TURN_ON = 0,
	TURN_OFF = 1
} uart_int_enable_t;
/** 
 * @brief	 UART Init Structure definition 
 * @details	UART - Register Layout Typedef        offset and reset value
 */
typedef struct
{
	 __IO  uint32_t UART_DLBL;           /*0x00: SEL = 1;Baud rate setting lower eight bits register*/ 
     __IO  uint32_t UART_DLBH;           /*0x04: SEL = 1; Baud rate setting upper eight bits register*/
	 __IO  uint32_t UART_IIR;            /*0x08: Interrupt identification register*/
	 __IO  uint32_t UART_LCR;            /*0x0C: Row control register*/
	 __O   uint32_t UART_MCR;            /*0x10: Modem control register */
	 __I   uint32_t UART_LSR;			 /*0x14: Interrupt identification register*/
	 __I   uint32_t UART_MSR;			 /*0x18: Modem status register*/
} uart_reg_t;

/**
 * @brief	   UART base pointer
 */
#define  UART          ((uart_reg_t *)UART_BASE)

#define  UART_RXFIFO   UART->UART_DLBL   /*0x00: SEL = 0;Receive FIFO*/
#define  UART_TXFIFO   UART->UART_DLBL   /*0x00: SEL = 0;Transmit FIFO*/
#define  UART_IER      UART->UART_DLBH   /*0x04: SEL = 0;Interrupt enable register*/
#define  UART_FCR      UART->UART_IIR    /*0x08: IFIFO control register*/

typedef struct _uart_cfg
{
  uint32_t baudRate;            /*!< This member configures the UART communication baud rate.The baud rate is computed using the following formula:
                                                       - Divider = ((Frequency) / (16 * (uart_cfg_t->UART_BaudRate))) */

  uint32_t dataLength;          /*!< Specifies the number of data bits transmitted or received in a frame.
                                                       This parameter can be a value of @ref UART_data_Length */

  uint32_t stopBits;            /*!< Specifies the number of stop bits transmitted.
                                                       This parameter can be a value of @ref UART_Stop_Bits */

  uint32_t parityEnable;        /*!< Specifies the parity mode.
                                                       This parameter can be a value of @ref UART_Parity_ENABLE */

  uint32_t paritySelect;       /*!< Select the parity is no or even.
                                                       This parameter can be a value of @ref UART_Parity_Select */

  uint32_t breakControl;        /*!< Specifies the Break contrl.
								     This parameter can be a value of @ref UART_Break*/

  uint32_t rxFifoLevel;       /*!< Specifies the number of data bits received in a frame.This parameter can be a value of
								     @ref UART_Receiver_FIFO_Interrupt_TriggerLevel */

  uint32_t txFifoLevel;        /*!< Specifies the number of data bits received in a frame.This parameter can be a value of
									@ref UART_Transmitter_FIFO_Interrupt_TriggerLevel */
  uint32_t inteConfig;    	/* uart interrupt configuration*/
} uart_cfg_t;


typedef struct __uart_handle_t
{
	uart_reg_t * instance;		// base address of uart
	uart_cfg_t configs;

} uart_handle_t;

/* Private macro -------------------------------------------------------------*/

/** 
 * @brief	ModemStatusInterruptEnable : Modem state change interrupt enable
 */
#define HAL_UART_IER_MSTENABLE_MASK             (0x8)
#define HAL_UART_IER_MSTENABLE_SHIFT            (3U)
#define HAL_UART_IER_MSTENABLE(x)               (((uint32_t)(((uint32_t)(x))<<HAL_UART_IER_MSTENABLE_SHIFT))&HAL_UART_IER_MSTENABLE_MASK)

/** 
 * @brief	ReceiverLineStatusInterruptEnable : Receive data status interrupt enable
 */
#define HAL_UART_IER_RLSIENABLE_MASK            (0x4)
#define HAL_UART_IER_RLSIENABLE_SHIFT           (2U)
#define HAL_UART_IER_RLSIENABLE(x)              ((uint32_t)(((uint32_t)(x))<<HAL_UART_IER_RLSIENABLE_SHIFT))&HAL_UART_IER_RLSIENABLE_MASK)

/** 
 * @brief	TransmitterHoldingRegisterEmptyinterruptEnable : Transmit FIFO trigger interrupt enable
 */
#define HAL_UART_IER_THREIENABLE_MASK           (0x2)
#define HAL_UART_IER_THREIENABLE_SHIFT          (1U)
#define HAL_UART_IER_THREIENABLE(x)             (((uint32_t)(((uint32_t)(x))<<HAL_UART_IER_THREIENABLE_SHIFT))&HAL_UART_IER_THREIENABLE_MASK)

/** 
 * @brief	ReceivedDataavailableinterruptEnable : Receive FIFO trigger interrupt enable
 */
#define HAL_UART_IER_RDLIENABLE_MASK            (0x1)
#define HAL_UART_IER_RDLIENABLE_SHIFT           (0U)
#define HAL_UART_IER_RDLIENABLE(x)              (((uint32_t)(((uint32_t)(x))<<HAL_UART_IER_RDLIENABLE_SHIFT))&HAL_UART_IER_RDLIENABLE_MASK)



/** 
 * @brief	Interrupt SourceNumber
 */
#define HAL_UART_IIR_ISR_SOURCE_NUM_MASK         (0xE)
#define HAL_UART_IIR_ISR_SOURCE_NUM_SHIFT        (1U)
#define HAL_UART_IIR_ISR_SOURCE_NUM(x)           (((uint32_t)(((uint32_t)(x))<<HAL_UART_IIR_ISR_SOURCE_NUM_SHIFT))&HAL_UART_IIR_ISR_SOURCE_NUM_MASK)


/** 
 * @brief	UartInterrupt Flag
 */
#define HAL_UART_IIR_ISR_FLAG_MASK               (0x1)
#define HAL_UART_IIR_ISR_FLAG_SHIFT              (0U)
#define HAL_UART_IIR_ISR_FLAG(x)                 (((uint32_t)(((uint32_t)(x))<<HAL_UART_IIR_ISR_FLAG_SHIFT))&HAL_UART_IIR_ISR_FLAG_MASK)


/*ReceiverFIFOInterrupt TriggerLevel*/
/** 
 * @brief	ModemStatusInterruptEnable : Modem state change interrupt enable
 */

#define HAL_UART_FCR_R_FIFO_ISR_T_MASK           (0xC0)
#define HAL_UART_FCR_R_FIFO_ISR_T_SHIFT          (6U)
#define HAL_UART_FCR_R_FIFO_ISR_T(x)             (((uint32_t)(((uint32_t)(x))<<HAL_UART_FCR_R_FIFO_ISR_T_SHIFT))&HAL_UART_FCR_R_FIFO_ISR_T_MASK)

/** 
 * @brief	TransmitterFIFOInterrupt TriggerLevel
 */
#define HAL_UART_FCR_T_FIFO_ISR_T_MASK           (0x30)
#define HAL_UART_FCR_T_FIFO_ISR_T_SHIFT          (4U)
#define HAL_UART_FCR_T_FIFO_ISR_T(x)             (((uint32_t)(((uint32_t)(x))<<HAL_UART_FCR_T_FIFO_ISR_T_SHIFT))&HAL_UART_FCR_T_FIFO_ISR_T_MASK)

/** 
 * @brief	ClearTransmitterFIFO
 */
#define HAL_UART_FCR_CLEAR_TRANSMITTER_FIFO_MASK  (0x4)
#define HAL_UART_FCR_CLEAR_TRANSMITTER_FIFO_SHIFT (2U)
#define HAL_UART_FCR_CLEAR_TRANSMITTER_FIFO(x)    (((uint32_t)(((uint32_t)(x))<<HAL_UART_FCR_CLEAR_TRANSMITTER_FIFO_SHIFT))&HAL_UART_FCR_CLEAR_TRANSMITTER_FIFO_MASK)

/** 
 * @brief	ClearReceiverFIFO
 */
#define HAL_UART_FCR_CLEAR_RECEIVER_FIFO_MASK     (0x2)
#define HAL_UART_FCR_CLEAR_RECEIVER_FIFO_SHIFT    (1U)
#define HAL_UART_FCR_CLEAR_RECEIVER_FIFO(x)       (((uint32_t)(((uint32_t)(x))<<HAL_UART_FCR_CLEAR_RECEIVER_FIFO_SHIFT))&HAL_UART_FCR_CLEAR_RECEIVER_FIFO_MASK)


/** 
 * @brief	FIFOEnable
 */
#define HAL_UART_FCR_FIFO_ENABLE_MASK             (0x1)
#define HAL_UART_FCR_FIFO_ENABLE_SHIFT            (0U)
#define HAL_UART_FCR_FIFO_ENABLE(x)               (((uint32_t)(((uint32_t)(x))<<HAL_UART_FCR_FIFO_ENABLE_SHIFT))&HAL_UART_FCR_FIFO_ENABLE_MASK)

/** 
 * @brief	Divisor Latch Access Bit: 1, Divisor Latch is visited ;0,ordinary register is accessed
 */

#define HAL_UART_LCR_SELECT_MASK                  (0x80)
#define HAL_UART_LCR_SELECT_SHIFT                 (7U)
#define HAL_UART_LCR_SELECT(x)                    (((uint32_t)(((uint32_t)(x))<<HAL_UART_LCR_SELECT_SHIFT))&HAL_UART_LCR_SELECT_MASK)

/** 
 * @brief	out_BreakControl Bit
 */
#define HAL_UART_LCR_OUT_BREAKCONTROL_BIT_MASK     (0x40)
#define HAL_UART_LCR_OUT_BREAKCONTROL_BIT_SHIFT    (6U)
#define HAL_UART_LCR_OUT_BREAKCONTROL_BIT(x)       (((uint32_t)(((uint32_t)(x))<<HAL_UART_LCR_OUT_BREAKCONTROL_BIT_SHIFT))&HAL_UART_LCR_OUT_BREAKCONTROL_BIT_MASK)

/** 
 * @brief	StickParityBit
 */
#define HAL_UART_LCR_PARITY_BIT_MASK               (0x20)
#define HAL_UART_LCR_PARITY_BIT_SHIFT              (5U)
#define HAL_UART_LCR_PARITY_BIT(x)                 (((uint32_t)(((uint32_t)(x))<<HAL_UART_LCR_PARITY_BIT_SHIFT))&HAL_UART_LCR_PARITY_BIT_MASK)

/** 
 * @brief	EvenParitySelect
 */
#define HAL_UART_LCR_EVEN_PARITY_SELECT_MASK        (0x10)
#define HAL_UART_LCR_EVEN_PARITY_SELECT_SHIFT       (4U)
#define HAL_UART_LCR_EVEN_PARITY_SELECT(x)          (((uint32_t)(((uint32_t)(x))<<HAL_UART_LCR_EVEN_PARITY_SELECT_SHIFT))&HAL_UART_LCR_EVEN_PARITY_SELECT_MASK)

/** 
 * @brief	ParityEnable
 */
#define HAL_UART_LCR_PARITY_ENABLE_MASK             (0x8)
#define HAL_UART_LCR_PARITY_ENABLE_SHIFT            (3U)
#define HAL_UART_LCR_PARITY_ENABLE(x)               (((uint32_t)(((uint32_t)(x))<<HAL_UART_LCR_PARITY_ENABLE_SHIFT))&HAL_UART_LCR_PARITY_ENABLE_MASK)

/** 
 * @brief	NumberOf Generated StopBits
 */
#define HAL_UART_LCR_NUM_STOP_BITS_MASK             (0x4)
#define HAL_UART_LCR_NUM_STOP_BITS_SHIFT            (2U)
#define HAL_UART_LCR_NUM_STOP_BITS(x)               (((uint32_t)(((uint32_t)(x))<<HAL_UART_LCR_NUM_STOP_BITS_SHIFT))&HAL_UART_LCR_NUM_STOP_BITS_MASK)

/** 
 * @brief NumberOfBits
 */
#define HAL_UART_LCR_NUM_BITS_MASK                  (0x3)
#define HAL_UART_LCR_NUM_BITS_SHIFT                 (0U)
#define HAL_UART_LCR_NUM_BITS(x)                    (((uint32_t)(((uint32_t)(x))<<HAL_UART_LCR_NUM_BITS_SHIFT))&HAL_UART_LCR_NUM_BITS_MASK)

/** 
 * @brief	IrdaMode
 */
#define HAL_UART_MCR_IRDA_MODE_MASK                  (0x40)
#define HAL_UART_MCR_IRDA_MODE_SHIFT                 (6U)
#define HAL_UART_MCR_IRDA_MODE(x)                    (((uint32_t)(((uint32_t)(x))<<HAL_UART_MCR_IRDA_MODE_SHIFT))&HAL_UART_MCR_IRDA_MODE_MASK)

/** 
 * @brief	AutoFlowControl
 */
#define HAL_UART_MCR_AUTO_FLOW_CONTROL_MASK          (0x20)
#define HAL_UART_MCR_AUTO_FLOW_CONTROL_SHIFT         (5U)
#define HAL_UART_MCR_AUTO_FLOW_CONTROL(x)            (((uint32_t)(((uint32_t)(x))<<HAL_UART_MCR_AUTO_FLOW_CONTROL_SHIFT))&HAL_UART_MCR_AUTO_FLOW_CONTROL_MASK)

/** 
 * @brief	LoopbackMode
 */
#define HAL_UART_MCR_LOOP_BACK_MODE_MASK             (0x10)
#define HAL_UART_MCR_LOOP_BACK_MODE_SHIFT            (4U)
#define HAL_UART_MCR_LOOP_BACK_MODE(x)               (((uint32_t)(((uint32_t)(x))<<HAL_UART_MCR_LOOP_BACK_MODE_SHIFT))&HAL_UART_MCR_LOOP_BACK_MODE_MASK)

#define HAL_UART_MCR_OUT2_MASK                       (0x8)
#define HAL_UART_MCR_OUT2_SHIFT                      (3U)
#define HAL_UART_MCR_OUT2(x)                         (((uint32_t)(((uint32_t)(x))<<HAL_UART_MCR_OUT2_SHIFT))&HAL_UART_MCR_OUT2_MASK)

#define HAL_UART_MCR_OUT1_MASK                       (0x4)
#define HAL_UART_MCR_OUT1_SHIFT                      (2U)
#define HAL_UART_MCR_OUT1(x)                         (((uint32_t)(((uint32_t)(x))<<HAL_UART_MCR_OUT1_SHIFT))&HAL_UART_MCR_OUT1_MASK)

/** 
 * @brief	RequestToSendSignal Control
 */
#define HAL_UART_MCR_SENDSIGNAL_CONTROL_MASK         (0x2)
#define HAL_UART_MCR_SENDSIGNAL_CONTROL_SHIFT        (1U)
#define HAL_UART_MCR_SENDSIGNAL_CONTROL(x)           (((uint32_t)(((uint32_t)(x))<<HAL_UART_MCR_SENDSIGNAL_CONTROL_SHIFT))&HAL_UART_MCR_SENDSIGNAL_CONTROL_MASK)

/** 
 * @brief	DataTerminalReadSignal Control
 */
#define HAL_UART_MCR_READSIGNAL_CONTROL_MASK         (0x1)
#define HAL_UART_MCR_READSIGNAL_CONTROL_SHIFT        (0U)
#define HAL_UART_MCR_READSIGNAL_CONTROL(x)           (((uint32_t)(((uint32_t)(x))<<HAL_UART_MCR_READSIGNAL_CONTROL_SHIFT))&HAL_UART_MCR_READSIGNAL_CONTROL_MASK)

/** 
 * @brief	Divisorlow
 */
#define HAL_UART_DLBL_DIVISOR_LOW_MASK                (0xFF)
#define HAL_UART_DLBL_DIVISOR_LOW_SHIFT               (0U)
#define HAL_UART_DLBL_DIVISOR_LOW(x)                  ((uint32_t)(((uint32_t)(x))<<HAL_UART_DLBL_DIVISOR_LOW_SHIFT))&HAL_UART_DLBL_DIVISOR_LOW_MASK)

/** 
 * @brief	Divisorhigh
 */
#define HAL_UART_DLBL_DIVISOR_HIGH_MASK               (0xFF)
#define HAL_UART_DLBL_DIVISOR_HIGH_SHIFT              (0U)
#define HAL_UART_DLBL_DIVISOR_HIGH(x)                 (((uint32_t)(((uint32_t)(x))<<HAL_UART_DLBL_DIVISOR_HIGH_SHIFT))&HAL_UART_DLBL_DIVISOR_HIGH_MASK)

#define HAL_UART_RXFIFO_MASK                          (0xFF)
#define HAL_UART_RXFIFO_SHIFT                         (0U)
#define HAL_UART_RXFIFO(x)                            (((uint32_t)(((uint32_t)(x))<<HAL_UART_RXFIFO_SHIFT))&HAL_UART_RXFIFO_MASK)

#define HAL_UART_TXFIFO_MASK                           (0xFF)
#define HAL_UART_TXFIFO_SHIFT                          (0U)
#define HAL_UART_TXFIFO(x)                             (((uint32_t)(((uint32_t)(x))<<HAL_UART_TXFIFO_SHIFT))&HAL_UART_TXFIFO_MASK)

/** 
* @brief	UART_data_Length 
*/
#define UART_DATALENGRTH_5BIT                  ((uint32_t)0x00)
#define UART_DATALENGRTH_6BIT                  ((uint32_t)0x01)
#define UART_DATALENGRTH_7BIT                  ((uint32_t)0x02)
#define UART_DATALENGRTH_8BIT                  ((uint32_t)0x03)
                               
/** 
 * @brief	UART_Stop_Bits 
 */  
#define UART_STOPBITS_1BIT                    ((uint32_t)0x00)
#define UART_STOPBIT_OTHER                    ((uint32_t)0x04)

/** 
* @brief	UART_Parity_ENABLE 
*/ 
#define UART_PARITY_DISABLE                   ((uint32_t)0x00)
#define UART_PARITY_ENABLE                    ((uint32_t)0x08)

/** 
* @brief	UART_Parity_Select
*/
#define UART_PARITY_NO                        ((uint32_t)0x00)
#define UART_PARITY_EVEN                      ((uint32_t)0x10)

/** 
* @brief	UART_StickParityBit
*/
#define UART_STICKPARITY_DIABLE               ((uint32_t)0x00)
#define UART_STICKPARITY_ENABLE               ((uint32_t)0x20)

/** 
* @brief	UART_Break
*/
#define UART_BREAK_NORMAL                     ((uint32_t)0x00)
#define UART_BREAK_0                          ((uint32_t)0x40)

/** 
* @brief	 UART_Divisor Latch
*/
#define UART_SEL_NO_CALL                      ((uint32_t)0x00)
#define UART_SEL_CALL                      	  ((uint32_t)0x80)

/** 
* @brief	UART_Receiver_FIFO_Interrupt_TriggerLevel
*/
#define UART_R_FIFO_LEVEL_1B                  ((uint32_t)0x00)
#define UART_R_FIFO_LEVEL_4B                  ((uint32_t)0x40)
#define UART_R_FIFO_LEVEL_8B                  ((uint32_t)0x80)
#define UART_R_FIFO_LEVEL_14B                 ((uint32_t)0xC0)

/** 
* @brief	UART_Transmitter_FIFO_Interrupt_TriggerLevel
*/
#define UART_T_FIFO_LEVEL_0B                  ((uint32_t)0x00)
#define UART_T_FIFO_LEVEL_2B                  ((uint32_t)0x10)
#define UART_T_FIFO_LEVEL_4B                  ((uint32_t)0x20)
#define UART_T_FIFO_LEVEL_8B                  ((uint32_t)0x30)
 
/** 
 * @brief	UART_ClearTransmitterFIFO
 */

#define UART_ClEAR_TFIFO_NOT                  ((uint32_t)0x00)
#define UART_ClEAR_TFIFO                      ((uint32_t)0x04)

/** 
 * @brief	UART_ClearReceiverFIFO
 */

#define UART_ClEAR_RFIFO_NOT                  ((uint32_t)0x00)
#define UART_ClEAR_RFIFO                      ((uint32_t)0x02)

/** 
 * @brief	 UART flag in the UART_LSR register
 */
#define UART_FLAG_RE                          ((uint32_t)0x80)
#define UART_FLAG_TC                          ((uint32_t)0x40)
#define UART_FLAG_TXE                         ((uint32_t)0x20)
#define UART_FLAG_BREAK           			  ((uint32_t)0x10)
#define UART_FLAG_FE            			  ((uint32_t)0x08)
#define UART_FLAG_PE           			      ((uint32_t)0x04)
#define UART_FLAG_ORE           			  ((uint32_t)0x02)
#define UART_FLAG_RXNE                        ((uint32_t)0x01)




/**
 * @brief	 UART interrupt flag in the UART_IIR register
 */
#define UART_ISR_ALL_ENABLE   0x0F
#define UART_ISR_ALL_DISABLE  0x00

#define UART_LINE_STATUS   	  0x06
#define UART_RX_FIFO_ISR      0x04
#define UART_TX_FIFO_ISR      0x02
#define UART_RX_TIMEOUT       0x0C
#define UART_MODEM_STATUS     0x00




/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
hal_status_t hal_uart_init(uart_cfg_t* config);

void hal_uart_tx_data( uint8_t Data);
uint8_t hal_uart_rx_data(void);

hal_status_t hal_uart_get_status(uint32_t uartFlag);
void hal_uart_int_config(uint32_t uart_it, uart_int_enable_t status);
uint8_t hal_uart_get_int_status(void);

void hal_uart_clean_tx_fifo(void);
void hal_uart_clean_rx_fifo(void);


#ifdef __cplusplus
}
#endif

#endif  /* __HAL_UART_H__ */


