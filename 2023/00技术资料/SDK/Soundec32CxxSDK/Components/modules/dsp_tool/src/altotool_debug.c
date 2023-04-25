/**
***********************************************************
 *
 * @file name   : altotool_debug.c
 * @author      : nick
 * @version     : V0.1
 * @date        : 2022-03-10
 * @brief       : alto tool DEBUG config
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
 * and Sourced Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/

#include "altotool.h"
#include "altotool_debug.h"
#include "hid_parser.h"
#if (SUPPORT_ALTOTOOL_CODEDC)

#define SYS_DEBUG_INFO_SIZE_MAX (1024)

uint32_t gSysDebugLevel = 0;
uint32_t gSysDebugModule = 0;

 
void set_debug_module(void)
{
	uint8_t byte = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("set = %d \r\n",byte);
	gSysDebugModule = byte;
}

void set_debug_level(void)
{
	uint8_t byte = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("%d \r\n",byte);
	gSysDebugLevel = byte;
}

void set_debug_modulel_info(void)
{
	uint8_t byte = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("%d \r\n",byte);

	switch(byte)
	{
		case DEBUG_USB_MODULE_INFO://		   1<<0
			uart_printf("usb module information !\r\n");
			break;
		case DEBUG_CODEC_MODULE_INFO://	   1<<1
			uart_printf("codec module information !\r\n");
			break;
		case DEBUG_I2S_MODULE_INFO://		   1<<2
			uart_printf("i2s module information !\r\n");
			break;
		case DEBUG_I2C_MODULE_INFO://		   1<<3
			uart_printf("i2c module information !\r\n");
			break;
		case DEBUG_PROTOCOL_MODULE_INFO://    1<<4
			uart_printf("protocol module information !\r\n");
			break;
		default:
		break;

	}
}


void set_debug_register_write(void)
{
	U32_t base_addr ;
	base_addr.byte[0] = altotoolPara.rxPkt.data[COMM_DATA_POS+0];
	base_addr.byte[1] = altotoolPara.rxPkt.data[COMM_DATA_POS+1];
	base_addr.byte[2] = altotoolPara.rxPkt.data[COMM_DATA_POS+2];
	base_addr.byte[3] = altotoolPara.rxPkt.data[COMM_DATA_POS+3];


	U32_t offset ;
	offset.byte[0] = altotoolPara.rxPkt.data[COMM_DATA_POS+4];
	offset.byte[1] = altotoolPara.rxPkt.data[COMM_DATA_POS+5];
	offset.byte[2] = altotoolPara.rxPkt.data[COMM_DATA_POS+6];
	offset.byte[3] = altotoolPara.rxPkt.data[COMM_DATA_POS+7];

	uint32_t reg = base_addr.dWords + offset.dWords ;


	U32_t data ;
	data.byte[0] = altotoolPara.rxPkt.data[COMM_DATA_POS+8];
	data.byte[1] = altotoolPara.rxPkt.data[COMM_DATA_POS+9];
	data.byte[2] = altotoolPara.rxPkt.data[COMM_DATA_POS+10];
	data.byte[3] = altotoolPara.rxPkt.data[COMM_DATA_POS+11];

	uart_printf("0x%08x , 0x%08x \r\n",reg ,data.dWords);
	//write_reg32(reg,data.dWords);

}

void set_debug_register_read(void)
{
	U32_t base_addr ;
	base_addr.byte[0] = altotoolPara.rxPkt.data[COMM_DATA_POS+0];
	base_addr.byte[1] = altotoolPara.rxPkt.data[COMM_DATA_POS+1];
	base_addr.byte[2] = altotoolPara.rxPkt.data[COMM_DATA_POS+2];
	base_addr.byte[3] = altotoolPara.rxPkt.data[COMM_DATA_POS+3];


	U32_t offset ;
	offset.byte[0] = altotoolPara.rxPkt.data[COMM_DATA_POS+4];
	offset.byte[1] = altotoolPara.rxPkt.data[COMM_DATA_POS+5];
	offset.byte[2] = altotoolPara.rxPkt.data[COMM_DATA_POS+6];
	offset.byte[3] = altotoolPara.rxPkt.data[COMM_DATA_POS+7];

	uint32_t reg = base_addr.dWords + offset.dWords ;

	uart_printf("0x%08x \r\n",reg);
//uint32_t date = read_reg32(reg);
}

void set_debug_custom_cmd(void)
{

}


void set_debug_config(uint8_t cmd)
{
	switch(cmd)
	{
		case CMD_DEBUG_MODULE://		   = 0x00,
			set_debug_module();
			break;
		case CMD_DEBUG_LEVEL://		       = 0x01,
			set_debug_level();
			break;
		case CMD_DEBUG_MODULE_INFO://      = 0x02,
			set_debug_modulel_info();
			break;
		case CMD_DEBUG_REGISTER_WRITE://   = 0x03,
			set_debug_register_write();
			break;
		case CMD_DEBUG_REGISTER_READ://	   = 0x04,
			set_debug_register_read();
			break;
		case CMD_DEBUG_CUSTOM_CMD://	   = 0x05,
			set_debug_custom_cmd();
			break;
		default:
			break;
	}
}



uint8_t StrDesc[1024];
static uint16_t getLen(uint8_t *buf)
{
    uint16_t  len = 0U;

    while (*buf != '\0')
    {
        len++;
        buf++;
    }

    return len;
}

void getString(uint8_t *desc, uint8_t *unicode, uint16_t *len)
{
  uint8_t idx = 0U;

  if (desc != NULL)
  {
    *len = (uint16_t)getLen(desc) * 2U + 2U ;
    unicode[idx++] = 0x66;
    unicode[idx++] = 0x88;

    while (*desc != '\0')
    {
      unicode[idx++] = *desc++;
      unicode[idx++] =  0U;
    }
  }
}


int hid_sys_debug_if(uint8_t module_id ,uint8_t debug_level , const char *fmt, ...)
{
	//uint8_t ret = (gSysDebugLevel >= debug_level)&&(gSysDebugModule&module_id) ;
	//if(ret == 0)return 0;

    va_list list;
    int32_t log_size;
    char buffer[SYS_DEBUG_INFO_SIZE_MAX];
	uint16_t length = 0;
    va_start(list, fmt);
    log_size = vsnprintf(buffer, SYS_DEBUG_INFO_SIZE_MAX, fmt, list);
    if (log_size < 0) {
        va_end(list);
        return 0;
    }
    if ((uint32_t)log_size >= sizeof(buffer)) {
        log_size = sizeof(buffer) - 1;
		 va_end(list);
        return 0;
    }
	getString((uint8_t *)buffer, StrDesc, &length);
	hid_CommProtocol_tx_input(StrDesc, length);
	
    va_end(list);
    return log_size;
}


#endif/* SUPPORT_ALTOTOOL_CODEDC */


