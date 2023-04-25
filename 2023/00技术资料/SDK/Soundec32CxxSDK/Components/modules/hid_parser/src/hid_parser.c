/**
***********************************************************
 *
 * @file name	: hid_parser.c
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

/* Includes ------------------------------------------------------------------*/
#include "hid_parser.h"
#include <string.h>
#include "sys_init.h"
#include "drv_flash.h"
#include "usb_hid.h"

#if (USB_ENABLE)
/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define HID_DFU_ACTIVE_KEY   16
#define HID_COMMP_DATAIN_SIZE  64
/* Private variables ---------------------------------------------------------*/
uint8_t dfu_active_key[HID_DFU_ACTIVE_KEY] = {0x01,0x0F,0xCD,0x55,0xAA,0xCC,0x32,0xAA,0x55,0x33,0x04,0x20,0x09,0x36,0x00,0x91} ;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

typedef enum{
 COMMP_DATA_IN_DISABLE = 0,
 COMMP_DATA_IN_ENABLE  = 1,
}cp_data_in_t;

typedef struct{
	uint8_t rx_buffer[64] ;
	uint8_t rx_count;
	uint8_t rx_len;
	uint8_t rx_state;
#if SUPPORT_ALTOTOOL
	uint8_t tx_buffer[1024];
#else
	uint8_t tx_buffer[64];
#endif
	uint32_t tx_count ;
	uint32_t tx_len;
	uint8_t tx_state;
}hid_commp_protocol_t;
hid_commp_protocol_t hid_commp;

void hid_CommProtocol_active_system_dfu_event(uint8_t *data);

/*when need data in ,frist make endpoint nak sclear . then do filling_device_in_data*/
void hid_CommProtocol_set_device_data_in_start(uint8_t enable)
{
	if(hid_commp.tx_state == enable)return;

	hid_commp.tx_state = enable;


	usb_dev_hid_ep_clean_nak(enable);
}
void hid_CommProtocol_set_device_data_idle(uint8_t enable)
{
	hid_commp.tx_state = enable;
}


void hid_CommProtocol_host_data_printf(void)
{
	return ;
	int i;//,len;
	if(hid_commp.rx_len == 0)return;
	for(i=0;i < hid_commp.rx_len;i++)
	{
		uart_printf("0x%02x ",hid_commp.rx_buffer[i]);
	}
	hid_commp.rx_len = 0;
}
// uint8_t hid_commp_buff[1024] ;

/*Get data from micEqProfile_sendData*/
int8_t hid_CommProtocol_tx_input(U8 *sentData ,uint32_t tx_len)
{
   hid_commp.tx_len   = tx_len ;
   hid_commp.tx_count = 0 ;
   memcpy(hid_commp.tx_buffer,sentData,tx_len);

   if(hid_commp.tx_state == 0)
   {
	 hid_CommProtocol_set_device_data_in_start(1);
   }
   return 0;
}

/*transmit date to host*/
uint8_t hid_CommProtocol_filling_device_in_data(uint8_t * __restrict hid_commpbuff)
{
	uint8_t i;
	if(hid_commp.tx_len == 0)return 0;

	if(hid_commp.tx_len >= 1000)
	{
		uart_printf("hid_commp.tx_len too long!");
	}

	int m = 0;
	for(i = 0; i < hid_commp.tx_len ; i++ , m++)
	{
	    if(i%(HID_COMMP_DATAIN_SIZE - 2) == 0)
	    {
			hid_commpbuff[m] = 0x02 ;
			m++;
			if(hid_commp.tx_len - i> HID_COMMP_DATAIN_SIZE - 2 ){
				hid_commpbuff[m] = HID_COMMP_DATAIN_SIZE - 2;
			}else{
				hid_commpbuff[m] = hid_commp.tx_len - i;
			}
			m ++;
		}
		hid_commpbuff[m] = hid_commp.tx_buffer[i] ;
		hid_commp.tx_count ++;
	}


	hid_commp.tx_count = 0;
	hid_commp.tx_len = 0;

	return (m/HID_COMMP_DATAIN_SIZE + 1);
}


/****************************************************************/

/*Get data form host*/
extern void checksum_check(void) ;
void hid_CommProtocol_host_data_out( uint8_t *pBuf, uint32_t len )
{
	if(hid_commp.rx_len != 0 ) {
		return;
	}

	memcpy(hid_commp.rx_buffer,pBuf,len) ;
	hid_commp.rx_len = len;
	hid_commp.rx_count = 2;
	hid_CommProtocol_active_system_dfu_event(pBuf);

}

/*sent data to micEqProfile_polling_receiver*/
int8_t hid_CommProtocol_rx_output(U8 *revData)
{
	int8_t ret = -1;
	if( hid_commp.rx_count < hid_commp.rx_len  )
	{
	    *revData = hid_commp.rx_buffer[hid_commp.rx_count] ;
		hid_commp.rx_count ++;
		ret = 0;
	}else{
		 hid_commp.rx_len = 0;
	}
	return ret;
}
int8_t hid_CommProtocol_rx_output_all(U8 *revData,U8 *length)
{
	int8_t ret = -1;
	if( hid_commp.rx_count ==2 && hid_commp.rx_len >hid_commp.rx_count  )
	{
	       memcpy(revData,hid_commp.rx_buffer+2, hid_commp.rx_len-2);
		*length = hid_commp.rx_len -2;
		hid_commp.rx_len = 0;
		ret = 0;
	}else{
	}
	return ret;
}
U8 checksum(uint8_t *data ,uint8_t len)
{
	U8 i ,sum = 0;
	for(i = 0; i < len ; i++)
	{
		sum += data[i] ;
	}
	sum= ~sum +1 ;
	return sum;
}

void hid_CommProtocol_active_system_dfu_event(uint8_t *data)
{
	if((data[1] == HID_DFU_ACTIVE_KEY -1)&&(checksum(data,(HID_DFU_ACTIVE_KEY -1)) == data[15] ))
	{
		uint32_t *Src   = (uint32_t *)data;
		uint32_t *Dakey = (uint32_t *)dfu_active_key;
		uint8_t i = 0;

		for(i = 0; i < 4 ; i ++)
		{
			if(Src[i] != Dakey[i])
				break;
		}

		if( i == 4)
		{
			hid_commp.rx_len = 0;
#if(FLASH_ENABLE)
			flash_active_dfu();
#endif
			system_reset();
		}
	}
}
#endif

