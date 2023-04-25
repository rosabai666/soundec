/*
 * app_uart_test.c
 *
 *  Created on: 2022.01.18
 *      Author: User
 */
#include "drv_uart.h"
#include "app_uart_test.h"

#if UART_ENABLE == 1

uint8_t Buffer[100] = {0};
void uart_initail(void)
{
	uart_init();
#if UART_TX_INT_ENABLE || UART_RX_INT_ENABLE
	uart_interrupt_init();
#endif
	for(int i = 0; i < 200; i++)
	{
		Buffer[i] = i+1;
	}
}

#if(UART_RECEIVE == 1)
void app_uart_rx(void)
{
	uart_rx_array(100);
	uart_check_queue();
}

void app_uart_tx(void)
{

	uart_rx_array(500);
	uart_out_queue();//sent data
}
#endif

void app_uart_test_case(void)
{
//	app_uart_rx();
//	app_uart_tx();
//	app_uart_tx_it();
//	app_uart_rx_it();
}

#if (UART_RECEIVE == 1)
void rx_tx_byte(void)
{
	while(1)
	{
		uart_tx_byte(uart_rx_byte());
	}
}
#endif

void rx_tx_bulk(void)
{
	uint8_t arry[100];
//	uart_rx_arry(arry,  20);
	uart_tx_array(arry, 20);
}

#if UART_TX_INT_ENABLE == 1
void app_uart_tx_it(void)
{
	uart_transmit_interrupt_config(Buffer,200);
}

void tx_callback(void)
{
//	uart_printf("tx fifo interrupt successful !\r\n");
//	uart_tx_byte(0x55);
	uart_transmit_interrupt();
}
#endif

#if UART_RX_INT_ENABLE == 1
void app_uart_rx_it(void)
{
	uart_receive_interrupt_config(500);
}

//void rx_callback(void)
//{
//	uart_receive_interrupt();
//}

//void timeout_callback(void)
//{
//	uart_receive_interrupt();
//	uart_out_queue();//sent data
//}
#endif

#endif


