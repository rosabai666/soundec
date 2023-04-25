/**
***********************************************************
 *
 * @file name   : Altotool_i2s.h
 * @author      : nick
 * @version     : V0.1
 * @date        : 2022-03-10
 * @brief       : alto tool codec config
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


#include "drv_i2s.h"
#include "altotool.h"
#include "altotool_i2s.h"

#if (SUPPORT_ALTOTOOL_I2S)

void switch_i2s_enable(uint8_t channel )
{
    uint8_t en = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("i2s chx%d enable = %d \r\n",channel, en);

#if	(I2S_ENABLE)
    i2s_enable(channel, en);
#endif
}

void switch_i2s_tx_enable(uint8_t channel )
{
    uint8_t en = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("i2s chx%d tx enable = %d \r\n",channel, en);

#if	(I2S_ENABLE)
	i2s_tx_enable(channel, en);
#endif
}


void switch_i2s_rx_enable(uint8_t channel )
{
    uint8_t en = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("i2s chx%d rx enable = %d \r\n",channel, en);

#if	(I2S_ENABLE)
	i2s_rx_enable(channel, en);
#endif
}

void set_i2s_mode(uint8_t channel )
{
    uint8_t byte = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("byte = %d",byte);

#if	(I2S_ENABLE)
	i2s_set_mode(channel, byte);
#endif
}

void set_i2s_format(uint8_t channel )
{
    uint8_t byte = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("byte = %d \r\n",byte);

#if	(I2S_ENABLE)
	i2s_set_format(channel, byte);
#endif
}


void set_i2s_sample_rate(uint8_t channel )
{
   	U32_t fs ;
	fs.byte[0] = altotoolPara.rxPkt.data[COMM_DATA_POS + 0];
	fs.byte[1] = altotoolPara.rxPkt.data[COMM_DATA_POS + 1];
	fs.byte[2] = altotoolPara.rxPkt.data[COMM_DATA_POS + 2];
	fs.byte[3] = altotoolPara.rxPkt.data[COMM_DATA_POS + 3];
	uart_printf("fs : %d  \r\n",fs.dWords);

#if	(I2S_ENABLE)
	i2s_set_sample_rate(channel, fs.dWords);
#endif
}

void set_i2s_word_length(uint8_t channel)
{
    uint8_t byte = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("byte = %d \r\n",byte);

#if	(I2S_ENABLE)
  	i2s_set_word_length(channel, byte);
#endif
}

void set_i2s_frame_length(uint8_t channel )
{
    uint8_t byte = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("byte = %d \r\n",byte);

#if	(I2S_ENABLE)
	i2s_set_frame_length(channel, byte);
#endif
}

void set_i2s_mute(uint8_t channel)
{
    uint8_t byte = altotoolPara.rxPkt.data[COMM_DATA_POS];
	uart_printf("byte = %d",byte);

#if	(I2S_ENABLE)
	i2s_set_mute(channel, byte);
#endif
}

void save_i2s_data(void)
{
#if(FLASH_SAVE_BANK_ENABLE)
	user_flash_region_t region = UFLASH_REGION_I2S_PARAM;
	uart_printf("start save i2s\n");
	user_flash_save_data(region, (uint32_t*)&(gI2sCfg));
#endif
}

void i2s_config_setting(uint8_t cmd , uint8_t channel)
{
	uart_printf("cmd:%d ,channel:%d \r\n",cmd,channel);
	
	switch(cmd)
	{
		case CMD_I2S_ENABLE:
			switch_i2s_enable(channel );
			break;
		case CMD_I2S_TX_EN:
			switch_i2s_tx_enable(channel );
			break;
		case CMD_I2S_RX_EN:
			switch_i2s_rx_enable(channel);
			break;
		case CMD_I2S_MODE:
			set_i2s_mode(channel);
			break;
		case CMD_I2S_FORMATE:
			set_i2s_format(channel);
			break;
		case CMD_I2S_SAMPLE:
			set_i2s_sample_rate(channel);
			break;
		case CMD_I2S_WORD_LENGTH:
			set_i2s_word_length(channel);
			break;
		case CMD_I2S_FRAME_LENGTH: /*frame length*/
			set_i2s_frame_length(channel );
			break;
		case CMD_I2S_MUTE:
			set_i2s_mute(channel );
			break;
		case CMD_I2S_SAVE_PARAM:
			save_i2s_data();
			break;
		default:
			break;
	}
}

#endif/* SUPPORT_ALTOTOOL_I2S */



