/**
***********************************************************
 *
 * @file name	: app_i2s_test.c
 * @author		: zhangkun
 * @version		: V01.01
 * @date		: 2022-02-12
 * @brief		: I2S Test Application
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

#include <stdio.h>
#include <string.h>
#include <xtensa/config/core.h>
#include "hal_i2s.h"
#include "boardConfig.h"
#include <xtensa/tie/xt_hifi2.h>
#include <xtensa/tie/xt_FP.h>
#include "hal_gpio.h"
#include "audio_hw_if.h"

#if (I2S_ENABLE == ENABLE)

int8_t app_i2s1_rx_callback(void *arg, void *data, uint32_t len);
int8_t app_i2s2_rx_callback(void *arg, void *data, uint32_t len);
int8_t app_i2s3_rx_callback(void *arg, void *data, uint32_t len);
int8_t app_i2s1_tx_callback(void *arg, void *data, uint32_t len);
int8_t app_i2s2_tx_callback(void *arg, void *data, uint32_t len);
int8_t app_i2s3_tx_callback(void *arg, void *data, uint32_t len);

//auStrmFormat_t txFormat = {SAMPLING_RATE_48000, 2, BIT_SLOT_32};
//auStrmFormat_t rxFormat = {SAMPLING_RATE_48000, 2, BIT_SLOT_32};

int32_t gtestData[I2S_TX_FIFO_LEVEL*2];// = {0x11112222, 0x22221111, 0x33334444, 0x44443333, 0x55556666, 0x66665555, 0x77778888, 0x88887777, 0x99990000, 0x00009999};
uint32_t gcheck_count = 0;

void check_rx_data(int32_t *rx_data, uint8_t index)
{
	uint8_t count = 0;

	for(int i = index;i < I2S_TX_FIFO_LEVEL*2;i++)
	{ 
		if(rx_data[i-index] == gtestData[i])
		{
			count++;
		}
	}

	for(int i = 0;i < index;i++)
	{
		if(rx_data[I2S_TX_FIFO_LEVEL*2-i-1] == gtestData[index-i-1])
		{
			count++;
		}
	}

	if(count != I2S_TX_FIFO_LEVEL*2)
	{
		gcheck_count++;
		if(gcheck_count == 160000)
		{
			gcheck_count = 0;
		}
	}
}

void check_all(int32_t *data)
{
	int8_t index = -1;
	for(int i = 0;i < I2S_TX_FIFO_LEVEL*2;i++)
	{
		if(data[0] == gtestData[i])
		{
			index = i;
			break;
		}
	}

	if(index == -1)
	{
		gcheck_count++;
	}
	else
	{
		check_rx_data(data, index);
	}
}

#if(I2S1_ENABLE == ENABLE)
int8_t app_i2s1_tx_callback(void *arg, void *data, uint32_t len)
{
	ALIGN(8) int32_t txData[I2S_TX_FIFO_LEVEL*2];// = {0x11112222, 0x22221111, 0x33334444, 0x44443333, 0x55556666, 0x66665555, 0x77778888, 0x88887777, 0x99990000, 0x00009999};
	uint8_t txCount = 0;

//	uart_printf("arg = %d.\r\n", *(int *)arg);

	int32_t *temp = (int32_t *)data;

	while(txCount < I2S_TX_FIFO_LEVEL*2)
	{
		*temp++ = txData[txCount++];
	};

	return 0;
}
int8_t app_i2s1_rx_callback(void *arg, void *data, uint32_t len)
{
	ALIGN(8) int32_t rxData[I2S_RX_FIFO_LEVEL*2];
	uint8_t rxCount = 0;

	int32_t *temp = (int32_t *)data;

	while(rxCount < len)
	{
		rxData[rxCount++] = *temp++;
	};

	check_all(rxData);

	return 0;
}
#endif

#if (I2S2_ENABLE == ENABLE)
int8_t app_i2s2_tx_callback(void *arg, void *data, uint32_t len)
{
	ALIGN(8) int32_t txData[I2S_TX_FIFO_LEVEL*2];// = {0x11112222, 0x22221111, 0x33334444, 0x44443333, 0x55556666, 0x66665555, 0x77778888, 0x88887777, 0x99990000, 0x00009999};
	uint8_t txCount = 0;

	int32_t *temp = (int32_t *)data;

	while(txCount < I2S_TX_FIFO_LEVEL*2)
	{
		*temp++ = txData[txCount++];
	};


	return 0;
}
int8_t app_i2s2_rx_callback(void *arg, void *data, uint32_t len)
{
	ALIGN(8) int32_t rxData[I2S_RX_FIFO_LEVEL*2];
	uint8_t rxCount = 0;

	int32_t *temp = (int32_t *)data;

	while(rxCount < len)
	{
		rxData[rxCount++] = *temp++;
	};


	check_all(rxData);

	return 0;
}
#endif


#if (I2S3_ENABLE == ENABLE)
int8_t app_i2s3_tx_callback(void *arg, void *data, uint32_t len)
{
	ALIGN(8) int32_t txData[I2S_TX_FIFO_LEVEL*2];// = {0x11112222, 0x22221111, 0x33334444, 0x44443333, 0x55556666, 0x66665555, 0x77778888, 0x88887777, 0x99990000, 0x00009999};
	uint8_t txCount = 0;

	int32_t *temp = (int32_t *)data;

	while(txCount < I2S_TX_FIFO_LEVEL*2)
	{
		*temp++ = txData[txCount++];
	};


	return 0;
}
int8_t app_i2s3_rx_callback(void *arg, void *data, uint32_t len)
{
	ALIGN(8) int32_t rxData[I2S_RX_FIFO_LEVEL*2];
	uint8_t rxCount = 0;
	
	int32_t *temp = (int32_t *)data;

	while(rxCount < len)
	{
		rxData[rxCount++] = *temp++;
	};


	check_all(rxData);

	return 0;
}
#endif

#endif

