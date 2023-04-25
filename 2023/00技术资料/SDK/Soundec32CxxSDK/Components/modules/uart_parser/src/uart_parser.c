/*
 *  uart_parser.c
 *  Created on: 2022.12.05
 *  Author: Ken Bu
 */
#include "drv_uart.h"
#include "boardConfig.h"
#include  "types.h"
#include "usb_def.h"
#include "userFlash.h"
#include "drv_usb.h"
#include "playTone.h"

//command format: MSB   LSB   command length data1 data2... checksum(MSB+LSB+command+length+data1+data2+...)//
//index            0      1       2       3       4    5        6                                            //
//value            0XFA  0XAF     1       2       1    1      0xae                                           //
#if UART_ENABLE == 1
#if UART_RX_INT_ENABLE

#define UART_SOP_MSB 0XFA
#define UART_SOP_LSB 0XAF
#define COMMAND_1    1
#define COMMAND_2    2
#define INSTRUCTION_MIN_LENGTH 6

void uart_rx_int_init(void)
{
	uart_interrupt_init();
	uart_receive_interrupt_config(UART_QUEUE_SIZE);
}

void rx_callback(void)
{
	uart_receive_interrupt();
}

void timeout_callback(void)
{
	uart_receive_interrupt();
}

//command format: MSB   LSB   command length data1 data2... checksum(MSB+LSB+command+length+data1+data2+...)//
//index            0      1       2       3       4    5        6                                            //
//value            0XFA  0XAF     1       2       1    1      0xae
bool check_checksum(uint8_t      *point_temp)
{

	uint16_t sum = 0;
	uint16_t count;

	for(count = 0; count < point_temp[3];count++)
	{
		sum += point_temp[4+count];
	}

	sum = sum + point_temp[0] + point_temp[1] + point_temp[2] + point_temp[3];

	uart_printf("check sum = %x\n",(uint8_t)sum);
	if(point_temp[4+point_temp[3]] == (uint8_t)sum)
		return TRUE;
	else
		return FALSE;

}
void reset_rx_int_rcv(uart_queue_t *reset_queue)
{
	reset_queue->tail = 0;
	reset_queue->head = 0;
    memset(&(reset_queue->pdata[0]),0,reset_queue->tail);
	hal_uart_clean_rx_fifo();
}

#if(TEST_USB_SWITCH)
extern volatile bool usbSwitchFlag;
#endif
void uart_rx_data_parser(void)
{
    uint32_t count = 0;
    uart_queue_t *data_queue = NULL;

	hal_uart_int_config(HAL_UART_IER_RDLIENABLE_MASK,TURN_OFF);
	data_queue = uart_get_rx_buffter_ptr();

	if(data_queue->tail < INSTRUCTION_MIN_LENGTH-1)// data not enough,so return;
	{
 	   hal_uart_int_config(HAL_UART_IER_RDLIENABLE_MASK,TURN_ON);
 	   return;
	}
uart_parser:
    if(data_queue->pdata[count] == UART_SOP_MSB)
	{
	    uart_printf("0\n");
		if(!check_checksum(&data_queue->pdata[count]))
		{
            reset_rx_int_rcv(data_queue);
			hal_uart_int_config(HAL_UART_IER_RDLIENABLE_MASK,TURN_ON);
			return;
		}
        count++;
		count++;
		switch(data_queue->pdata[count])//Command
		{
		    case COMMAND_1:
//		        uart_printf("1\n");
#if(TEST_USB_SWITCH)
#if FLASH_ENABLE
				sysParam.usbMode++;
				if(sysParam.usbMode >= 0x04)
					sysParam.usbMode = 0x01;
				user_flash_region_t region = UFLASH_REGION_SYS_PARAM;
				user_flash_save_data(region, (uint32_t*)&sysParam);
				switch(sysParam.usbMode)
				{
				case 0x01:
					is_usb_dev_UAC20 = UAC_10;
					Usbd_Speed_Cfg = USB_SPEED_FULL;
					uart_printf("switch to UAC1.0, full speed\n");
					break;
				case 0x02:
					is_usb_dev_UAC20 = UAC_10;
					Usbd_Speed_Cfg = USB_SPEED_HIGH;
					uart_printf("switch to UAC1.0, high speed\n");
					break;
				case 0x03:
					is_usb_dev_UAC20 = UAC_20;
					Usbd_Speed_Cfg = USB_SPEED_HIGH;
					uart_printf("switch to UAC2.0, high speed\n");
					break;
				default:
					is_usb_dev_UAC20 = UAC_10;
					Usbd_Speed_Cfg = USB_SPEED_FULL;
					uart_printf("switch to UAC1.0, full speed\n");
					break;
				}
				altochip_usb_deinit();
				usbSwitchFlag = 1;
#endif
#endif
				break;
#if (SUPPORT_PLAY_TONE && TEST_PLAY_TONE)
			case 2:
			   	PlayToneSwitch(SOUNDEC_WAV_BT_CONNECTED);
   				break;
			case 3:
			   	PlayToneSwitch(SOUNDEC_WAV_BT_DISCONNECTED);
   				break;
			case 4:
			   	PlayToneSwitch(SOUNDEC_WAV_MUTE);
   				break;
			case 5:
			   	PlayToneSwitch(SOUNDEC_WAV_NEXT);
   				break;
			case 6:
			   	PlayToneSwitch(SOUNDEC_WAV_PAUSE);
   				break;
			case 7:
			   	PlayToneSwitch(SOUNDEC_WAV_PLAY);
   				break;
			case 8:
			   	PlayToneSwitch(SOUNDEC_WAV_POWER_OFF);
   				break;
			case 9:
			   	PlayToneSwitch(SOUNDEC_WAV_POWER_ON);
   				break;
			case 10:
			   	PlayToneSwitch(SOUNDEC_WAV_PREV);
   				break;
			case 11:
			   	PlayToneSwitch(SOUNDEC_WAV_SW_MODE);
   				break;
			case 12:
			   	PlayToneSwitch(SOUNDEC_WAV_VOL_DOWN);
   				break;
			case 13:
			   	PlayToneSwitch(SOUNDEC_WAV_VOL_UP);
   				break;
#endif
			default:
			    break;
		}
		reset_rx_int_rcv(data_queue);

	}
	count++;
    if(count < data_queue->tail)
	{
	     goto uart_parser;
    }

	if(count >= UART_QUEUE_SIZE/2)
	{
		uart_printf("count = %d tail = %d queue size =%d\n",count,data_queue->tail,data_queue->size);
		reset_rx_int_rcv(data_queue);
	}

	hal_uart_int_config(HAL_UART_IER_RDLIENABLE_MASK,TURN_ON);
}
#endif

#endif

