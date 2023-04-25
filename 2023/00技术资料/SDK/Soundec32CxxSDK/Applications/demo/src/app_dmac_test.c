/**
***********************************************************
 *
 * @file name	: app_dmac_test.c
 * @author		: RandyFan
 * @version		: V02.02.demo
 * @date		: 2022-01-04
 * @brief		: DMAC test file
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
 * and Soundec Co., Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/

#include <stdlib.h>
#include "drv_dmac.h"
#include "app_dmac_test.h"
#include "drv_i2s.h"
#include "hal_gpio.h"
#include "drv_flash.h"
#include "drv_uart.h"
#include "hal_codec.h"

#if(DMA_ENABLE)
#if(DMA_TEST_MODE_ENABLE)
int32_t test_1khz_48k[96] = {
	0x00000000,
	0x10B53500,
	0x21210385,
	0x30FBA882,
	0x3FFFFFFF,
	0x4DEBD900,
	0x5A829499,
	0x658C7E27,
	0x6EDA122E,
	0x7641B327,
	0x7BA3981F,
	0x7EE78182,
	0x7FFFFFFF,
	0x7EE78182,
	0x7BA3981F,
	0x7641B327,
	0x6EDA122E,
	0x658C7E27,
	0x5A829499,
	0x4DEBD900,
	0x3FFFFFFF,
	0x30FBA882,
	0x21210385,
	0x10B53500,
	0x00000000,
	0xEF4ACB00,
	0xDEDEFC7B,
	0xCF04577E,
	0xC0000001,
	0xB2142700,
	0xA57D6B67,
	0x9A7381D9,
	0x9125EDD2,
	0x89BE4CD9,
	0x845C67E1,
	0x81187E7E,
	0x80000001,
	0x81187E7E,
	0x845C67E1,
	0x89BE4CD9,
	0x9125EDD2,
	0x9A7381D9,
	0xA57D6B67,
	0xB2142700,
	0xC0000001,
	0xCF04577E,
	0xDEDEFC7B,
	0xEF4ACB00,
	0x00000000,
	0x10B5150F,
	0x2120FB82,
	0x30FBC54C,
	0x3FFFFFFF,
	0x4DEBE4FD,
	0x5A827999,
	0x658C9A2C,
	0x6ED9EBA0,
	0x7641AF3B,
	0x7BA3751C,
	0x7EE7AA4A,
	0x7FFFFFFF,
	0x7EE7AA4A,
	0x7BA3751C,
	0x7641AF3B,
	0x6ED9EBA0,
	0x658C9A2C,
	0x5A827999,
	0x4DEBE4FD,
	0x3FFFFFFF,
	0x30FBC54D,
	0x2120FB83,
	0x10B5150F,
	0x00000000,
	0xEF4AEAF1,
	0xDEDF047E,
	0xCF043AB4,
	0xC0000001,
	0xB2141B03,
	0xA57D8667,
	0x9A7365D4,
	0x91261460,
	0x89BE50C5,
	0x845C8AE4,
	0x811855B6,
	0x80000001,
	0x811855B6,
	0x845C8AE4,
	0x89BE50C5,
	0x91261460,
	0x9A7365D4,
	0xA57D8667,
	0xB2141B03,
	0xC0000001,
	0xCF043AB3,
	0xDEDF047D,
	0xEF4AEAF1,
};

int32_t testDmacSrc0[96] = {0};
int32_t testDmacSrc1[96] = {0};
int32_t testDmacDst0[96] = {0};
int32_t testDmacDst1[96] = {0};
uint8_t testString[61] = {'t', 'e','s', 't', 'D', 'M', 'A','\n'};

#if(APP_DMAC_TEST_TYPE == APP_DMAC_TEST_MEMORY_TO_MEMORY)
void dmac_cb_close(uint8_t chx)
{
	dmac_close_channel(chx);

	uint8_t i = 0;
	for(i=0; i<96; i++)
	{
		uart_printf("test[%d]=0x%08x\n", i, testDmacDst0[i]);
	}
}

void dmac_cb_open_new_test2(uint8_t chx)
{
	dmac_close_channel(chx);

	gDmacChx[chx].cfg.srcAddr = 0xA0091000;
	gDmacChx[chx].cfg.dstAddr = (uint32_t)testDmacDst0;;
	gDmacChx[chx].cb[DMAC_INTERRUPT_BLOCK] = dmac_cb_close;
	dmac_open_channel(chx, CHX_MEM_TO_MEM);
}

void dmac_cb_open_new_test1(uint8_t chx)
{
	dmac_close_channel(chx);

	gDmacChx[chx].cfg.srcAddr = 0xA0090000;
	gDmacChx[chx].cfg.dstAddr = 0xA0091000;
	gDmacChx[chx].cb[DMAC_INTERRUPT_BLOCK] = dmac_cb_open_new_test2;
	dmac_open_channel(chx, CHX_MEM_TO_MEM);
}
void app_dmac_test_m_to_m(void)
{
	dmac_init();
	uart_printf("**************************\n");

	uint8_t i = 0;

	//generate random test data
	for(i=0;i<96;i++)
	{
		testDmacSrc0[i] = (rand() & (~0xFF)) | (i);
		uart_printf("test0[%d]: %x\n", i, testDmacSrc0[i]);
	}
	/*test ram 2 ram--------------------------------------------------*/
	int8_t chx = dmac_get_idle_channel();
	if(chx == -1)
	{
		uart_printf("no idle chx\n");
		return;
	}
	else
	{
		uart_printf("chx:%d\n", chx);
	}

	gDmacChx[chx].cfg.chPriority = 1;
	gDmacChx[chx].cfg.blockSize = 96;
	gDmacChx[chx].cfg.srcAddr = (uint32_t)testDmacSrc0;
	gDmacChx[chx].cfg.dstAddr = (uint32_t)testDmacDst0;
	gDmacChx[chx].cfg.intrBlockEn = 1;

	/*two ways to register cb----------*/
//	dmac_register_channel(chx, DMAC_INTERRUPT_BLOCK, dmac_cb_close);
	gDmacChx[chx].cb[DMAC_INTERRUPT_BLOCK] = dmac_cb_close;

	dmac_open_channel(chx, CHX_MEM_TO_MEM);

	/*test ram 2 flash 2 flash 2 ram-------------------------------------------*/
	chx = dmac_get_idle_channel();
	if(chx == -1)
	{
		uart_printf("no idle chx\n");
		return;
	}
	else
	{
		uart_printf("chx:%d\n", chx);
	}

	gDmacChx[chx].cfg.chPriority = 0;
	gDmacChx[chx].cfg.blockSize = 96;
	gDmacChx[chx].cfg.srcAddr = (uint32_t)testDmacSrc0;
	gDmacChx[chx].cfg.dstAddr = 0xA0090000;
	gDmacChx[chx].cfg.intrBlockEn = 1;
	gDmacChx[chx].cb[DMAC_INTERRUPT_BLOCK] = dmac_cb_open_new_test1;

	dmac_open_channel(chx, CHX_MEM_TO_MEM);
}

#endif

#if(APP_DMAC_TEST_TYPE == APP_DMAC_TEST_MEMORY_TO_I2S)
void app_dmac_test_i2s_init(void)
{
	hal_gpio_set_pinmux_function(HAL_GPIO_0, 2);
	hal_gpio_set_pinmux_function(HAL_GPIO_1, 2);
	hal_gpio_set_pinmux_function(HAL_GPIO_2, 2);
	hal_gpio_set_pinmux_function(HAL_GPIO_3, 2);

	hal_gpio_set_pinmux_function(HAL_GPIO_4, 2);
	hal_gpio_set_pinmux_function(HAL_GPIO_5, 2);
	hal_gpio_set_pinmux_function(HAL_GPIO_6, 2);
	hal_gpio_set_pinmux_function(HAL_GPIO_7, 2);

	hal_gpio_set_pinmux_function(HAL_GPIO_12, 1);
	hal_gpio_set_pinmux_function(HAL_GPIO_13, 1);
	hal_gpio_set_pinmux_function(HAL_GPIO_14, 1);
	hal_gpio_set_pinmux_function(HAL_GPIO_15, 1);

	i2s_cfg_t i2s_cfg;

	i2s_cfg.mode 	 	 = I2S_MODE_MASTER,
	i2s_cfg.standard	 = I2S_STANDARD_PHILIPS,
	i2s_cfg.audioFreq 	 = APP_DMAC_TEST_I2S_SAMPLE_RATE,
	i2s_cfg.txDataLength = I2S_FIFO_DATALENGTH_32B,
	i2s_cfg.rxDataLength = I2S_FIFO_DATALENGTH_32B,
#if(APP_DMAC_TEST_I2S_SAMPLE_RATE == 192000)
	i2s_cfg.frameLength  = I2S_FRAMELENGTH_24B,
#else
	i2s_cfg.frameLength  = I2S_FRAMELENGTH_32B,
#endif
	i2s_cfg.rxFifoLevel  = 7,
	i2s_cfg.txFifolevel  = 7,
	hal_i2s_init(I2S_INDEX_1, &i2s_cfg);

	I2S1_DMA->I2S_RRXDMA = 0x1;
	I2S1_DMA->I2S_RTXDMA = 0x1;
	I2S1_DMA->I2S_RRXDMA = 0;
	I2S1_DMA->I2S_RTXDMA = 0;

	i2s_cfg_t i2s_cfg2;

	i2s_cfg2.mode 	 	 = I2S_MODE_SLAVE,
	i2s_cfg2.standard	 = I2S_STANDARD_PHILIPS,
	i2s_cfg2.audioFreq 	 = APP_DMAC_TEST_I2S_SAMPLE_RATE,
	i2s_cfg2.txDataLength = I2S_FIFO_DATALENGTH_32B,
	i2s_cfg2.rxDataLength = I2S_FIFO_DATALENGTH_32B,
#if(APP_DMAC_TEST_I2S_SAMPLE_RATE == 192000)
	i2s_cfg2.frameLength  = I2S_FRAMELENGTH_24B,
#else
	i2s_cfg2.frameLength  = I2S_FRAMELENGTH_32B,
#endif
	i2s_cfg2.rxFifoLevel  = 7,
	i2s_cfg2.txFifolevel  = 7,
	hal_i2s_init(I2S_INDEX_2, &i2s_cfg2);

	I2S2_DMA->I2S_RRXDMA = 0x1;
	I2S2_DMA->I2S_RTXDMA = 0x1;
	I2S2_DMA->I2S_RRXDMA = 0;
	I2S2_DMA->I2S_RTXDMA = 0;

#if(DMA_TEST_I2S_USB_ENABLE)
	i2s_cfg_t i2s_cfg3;

	i2s_cfg3.mode 	 	 = I2S_MODE_MASTER,
	i2s_cfg3.standard	 = I2S_STANDARD_PHILIPS,
	i2s_cfg3.audioFreq 	 = SAMPLING_RATE_48000,
	i2s_cfg3.txDataLength = I2S_FIFO_DATALENGTH_32B,
	i2s_cfg3.rxDataLength = I2S_FIFO_DATALENGTH_32B,
	i2s_cfg3.frameLength  = I2S_FRAMELENGTH_32B,
	i2s_cfg3.rxFifoLevel  = 7,
	i2s_cfg3.txFifolevel  = 7,
	hal_i2s_init(I2S_INDEX_3, &i2s_cfg3);

	I2S3_DMA->I2S_RRXDMA = 0x1;
	I2S3_DMA->I2S_RTXDMA = 0x1;
	I2S3_DMA->I2S_RRXDMA = 0;
	I2S3_DMA->I2S_RTXDMA = 0;
#endif
}

#endif

#if(APP_DMAC_TEST_TYPE == APP_DMAC_TEST_MEMORY_TO_I2S)
void dmac_cb_i2s_tx(uint8_t chx)
{
	dmac_reopen_channel(chx, gDmacChx[chx].cfg.srcAddr, 0);
}

static uint16_t correctCnt0 = 0;
static uint16_t correctCnt1 = 0;
void dmac_cb_i2s_rx_examine_data(uint8_t chx)
{
	uint8_t i = 0;
	uint8_t p = 0;
	uint8_t j = 0;

	uint32_t* testDst;
	uint32_t* testSrc;
	uint16_t* correctCnt;

	if(gDmacChx[chx].cfg.i2sSel == 0)
	{
		testDst = (uint32_t*)&testDmacDst0[0];
		testSrc = (uint32_t*)&testDmacSrc1[0];
		correctCnt = &correctCnt0;
	}
	else
	{
		testDst = (uint32_t*)&testDmacDst1[0];
		testSrc = (uint32_t*)&testDmacSrc0[0];
		correctCnt = &correctCnt1;
	}

	for(i=0;i<96;i++)
	{
#if(APP_DMAC_TEST_I2S_SAMPLE_RATE == 192000)
		j = (testDst[i] >> 8)&0xFF;
#else
		j = testDst[i] & 0xFF;
#endif
		if(j)
		{
//			if(j == 0x01 && i && testDst[i-1])
//			{
//				p = 0;
//				j = i-1;
//			}
//			else
//			{
				p = j;
				j = i;
//			}
			break;
		}
	}
	if(i >= 95 || j >= 95)
	{
		dmac_reopen_channel(chx, 0, gDmacChx[chx].cfg.dstAddr);
		return;
	}
	uint8_t errCnt = 0;
	for(i=j;i<96;i++)
	{
		if(testDst[i] != testSrc[p])
		{
//			uart_printf("e%d:%x, %x\n", gDmacChx[chx].cfg.i2sSel, testDst[i], testSrc[p]);
			errCnt++;
		}
		p++;
		if(p >= 96)
			p = 0;
	}
	if(errCnt == 0)
	{
		(*correctCnt)++;
		if((*correctCnt) == 0xFFF)
		{
			uart_printf("ok\n");
			(*correctCnt) = 0;
		}
	}
	else
	{
		(*correctCnt) = 0;
	}

	dmac_reopen_channel(chx, 0, gDmacChx[chx].cfg.dstAddr);
}

#if(DMA_TEST_I2S_USB_ENABLE)
int32_t testDmacSpkFifo[NUM_OF_AUDIO_SPK_FIFO_ELEMENTS] = {0};
uint32_t DmacFifoRdPtr = 0;
uint32_t DmacFifoWrPtr = 0;
bool DmacFifoHasEnoughData = 0;
uint8_t DmacI2sUsbTxChx = 0;
uint8_t DmacI2sUsbRxChx = 0;
bool DmacI2sUsbStart = 0;

void dmac_cb_i2s_tx_usb_data(uint8_t chx)
{
	audioSpkRdPtr += APP_DMAC_TEST_I2S_USB_BLOCK;
	if(audioSpkRdPtr >= NUM_OF_AUDIO_SPK_FIFO_ELEMENTS)
		audioSpkRdPtr = 0;
	gDmacChx[chx].cfg.srcAddr = (uint32_t)&audioSpeakerBuffer[audioSpkRdPtr];
	dmac_reopen_channel(chx, gDmacChx[chx].cfg.srcAddr, 0);
}

void dmac_cb_i2s_rx_usb_data(uint8_t chx)
{
	if(DmacFifoHasEnoughData == 0)
	{
		DmacFifoHasEnoughData = 1;
	}
	DmacFifoWrPtr += APP_DMAC_TEST_I2S_USB_BLOCK;
	if(DmacFifoWrPtr >= NUM_OF_AUDIO_SPK_FIFO_ELEMENTS)
		DmacFifoWrPtr = 0;
	gDmacChx[chx].cfg.dstAddr = (uint32_t)&testDmacSpkFifo[DmacFifoWrPtr];
	dmac_reopen_channel(chx, 0, gDmacChx[chx].cfg.dstAddr);
}

uint32_t dma_spk_buf_get_storedElems(void)
{
	return (DmacFifoWrPtr + NUM_OF_AUDIO_SPK_FIFO_ELEMENTS - DmacFifoRdPtr) % NUM_OF_AUDIO_SPK_FIFO_ELEMENTS;
}

void codec_cb_get_data_fr_dma(int32_t* __restrict data)
{
	ae_int32x2 *__restrict pDst = (ae_int32x2 *)data;
	ae_int32x2 *__restrict pSrc = (ae_int32x2 *)&testDmacSpkFifo[DmacFifoRdPtr];
	ae_int32x2 temp;

	if (!DmacFifoHasEnoughData || dma_spk_buf_get_storedElems() < 2)
	{
		data[0] = data[1] = 0;
		return;
	}

	AE_L32X2_IP(temp, pSrc, 0);
	AE_S32X2_IP(temp, pDst, 0);

	DmacFifoRdPtr += 2;
	if (DmacFifoRdPtr >= NUM_OF_AUDIO_SPK_FIFO_ELEMENTS)
		DmacFifoRdPtr = 0;
}
#endif

void app_dmac_test_m_to_i2s(void)
{
	/*test random data tx\rx ---------------------------------------*/
	uint16_t i = 0;

	//generate random test data
	for(i=0;i<96;i++)
	{
#if(APP_DMAC_TEST_I2S_SAMPLE_RATE == 192000)
		testDmacSrc0[i] = ((rand() & (~0xFF)) | i) << 8;
		testDmacSrc1[i] = ((rand() & (~0xFF)) | i) << 8;
#else
		testDmacSrc0[i] = (rand() & (~0xFF)) | i;
		testDmacSrc1[i] = (rand() & (~0xFF)) | i;
#endif
		uart_printf("test0[%d]: %x\n", i, testDmacSrc0[i]);
		uart_printf("test1[%d]: %x\n", i, testDmacSrc1[i]);
	}
	app_dmac_test_i2s_init();

	uint8_t chx1 = dmac_get_idle_channel();
	uint8_t chx2 = dmac_get_idle_channel();
	uint8_t chx3 = dmac_get_idle_channel();
	uint8_t chx4 = dmac_get_idle_channel();

	//i2s0 tx
	gDmacChx[chx1].cfg.trfType = DMAC_MEMORY_TO_PERIPHERAL;
	gDmacChx[chx1].cfg.chPriority = 0;
	gDmacChx[chx1].cfg.blockSize = 96;
	gDmacChx[chx1].cfg.srcAddr = (uint32_t)testDmacSrc0;
	gDmacChx[chx1].cfg.i2sSel = 0; //i2s0
	gDmacChx[chx1].cfg.intrBlockEn = 1;
	gDmacChx[chx1].cb[DMAC_INTERRUPT_BLOCK] = dmac_cb_i2s_tx;

	//i2s0 rx
	gDmacChx[chx2].cfg.trfType = DMAC_PERIPHERAL_TO_MEMORY;
	gDmacChx[chx2].cfg.chPriority = 0;
	gDmacChx[chx2].cfg.blockSize = 96;
	gDmacChx[chx2].cfg.dstAddr = (uint32_t)testDmacDst0;
	gDmacChx[chx2].cfg.i2sSel = 0; //i2s0
	gDmacChx[chx2].cfg.intrBlockEn = 1;
	gDmacChx[chx2].cb[DMAC_INTERRUPT_BLOCK] = dmac_cb_i2s_rx_examine_data;


	//i2s1 tx
	gDmacChx[chx3].cfg.trfType = DMAC_MEMORY_TO_PERIPHERAL;
	gDmacChx[chx3].cfg.chPriority = 0;
	gDmacChx[chx3].cfg.blockSize = 96;
	gDmacChx[chx3].cfg.srcAddr = (uint32_t)testDmacSrc1;
	gDmacChx[chx3].cfg.i2sSel = 1; //i2s1
	gDmacChx[chx3].cfg.intrBlockEn = 1;
	gDmacChx[chx3].cb[DMAC_INTERRUPT_BLOCK] = dmac_cb_i2s_tx;

	//i2s1 rx
	gDmacChx[chx4].cfg.trfType = DMAC_PERIPHERAL_TO_MEMORY;
	gDmacChx[chx4].cfg.chPriority = 0;
	gDmacChx[chx4].cfg.blockSize = 96;
	gDmacChx[chx4].cfg.dstAddr = (uint32_t)testDmacDst1;
	gDmacChx[chx4].cfg.i2sSel = 1; //i2s1
	gDmacChx[chx4].cfg.intrBlockEn = 1;
	gDmacChx[chx4].cb[DMAC_INTERRUPT_BLOCK] = dmac_cb_i2s_rx_examine_data;

	dmac_open_channel(chx1, CHX_MEM_TO_I2S);
	dmac_open_channel(chx2, CHX_MEM_TO_I2S);
	dmac_open_channel(chx3, CHX_MEM_TO_I2S);
	dmac_open_channel(chx4, CHX_MEM_TO_I2S);
}

#if(DMA_TEST_I2S_USB_ENABLE)
void dmac_test_i2s_usb(void)
{
	uart_printf("start dma i2s usb test\n");
	uint8_t chx1 = dmac_get_idle_channel();
	uint8_t chx2 = dmac_get_idle_channel();

	DmacI2sUsbTxChx = chx1;
	DmacI2sUsbRxChx = chx2;

	//i2s2 tx
	gDmacChx[chx1].cfg.trfType = DMAC_MEMORY_TO_PERIPHERAL;
	gDmacChx[chx1].cfg.chPriority = 0;
	gDmacChx[chx1].cfg.blockSize = APP_DMAC_TEST_I2S_USB_BLOCK;
	gDmacChx[chx1].cfg.srcAddr = (uint32_t)audioSpeakerBuffer;
	gDmacChx[chx1].cfg.i2sSel = 2; //i2s2
	gDmacChx[chx1].cfg.intrBlockEn = 1;
	gDmacChx[chx1].cb[DMAC_INTERRUPT_BLOCK] = dmac_cb_i2s_tx_usb_data;

	//i2s2 rx
	gDmacChx[chx2].cfg.trfType = DMAC_PERIPHERAL_TO_MEMORY;
	gDmacChx[chx2].cfg.chPriority = 0;
	gDmacChx[chx2].cfg.blockSize = APP_DMAC_TEST_I2S_USB_BLOCK;
	gDmacChx[chx2].cfg.dstAddr = (uint32_t)testDmacSpkFifo;
	gDmacChx[chx2].cfg.i2sSel = 2; //i2s2
	gDmacChx[chx2].cfg.intrBlockEn = 1;
	gDmacChx[chx2].cb[DMAC_INTERRUPT_BLOCK] = dmac_cb_i2s_rx_usb_data;

	dmac_open_channel(chx1, CHX_MEM_TO_I2S);
	dmac_open_channel(chx2, CHX_MEM_TO_I2S);
	DmacI2sUsbStart = 1;
}
#endif

#endif

#if(APP_DMAC_TEST_TYPE == APP_DMAC_TEST_MEMORY_TO_ADC)

#endif

#if(APP_DMAC_TEST_TYPE == APP_DMAC_TEST_MEMORY_TO_UART)
void dmac_cb_close(uint8_t chx)
{
	dmac_close_channel(chx);
	uart_printf("end DMA\n");
}

void dmac_cb_dstTran(uint8_t chx)
{
	uart_printf("\n");
	hal_dmac_soft_hs_set_dst(chx);
}

void app_dmac_test_uart(void)
{
	U8 i = 0;
	for(i=8;i<8+26;i++)
	{
		testString[i] = 0x41+i-8;
		testString[i+26] = 0x61+i-8;
	}

	testString[60] = '\n';

	uint8_t chx = dmac_get_idle_channel();

	gDmacChx[chx].cfg.trfType = DMAC_MEMORY_TO_PERIPHERAL;
	gDmacChx[chx].cfg.chPriority = 0;
	gDmacChx[chx].cfg.blockSize = 61;
	gDmacChx[chx].cfg.srcAddr = (uint32_t)testString;
	gDmacChx[chx].cfg.dstAddr = 0x7700A000;
	gDmacChx[chx].cfg.intrBlockEn = 1;
	gDmacChx[chx].cfg.perCfg.srcMsize = DMAC_MSIZE_8;
	gDmacChx[chx].cfg.perCfg.dstMsize = DMAC_MSIZE_8;
	gDmacChx[chx].cfg.perCfg.srcTrfWidth = DMAC_TR_WIDTH_8BIT;
	gDmacChx[chx].cfg.perCfg.dstTrfWidth = DMAC_TR_WIDTH_8BIT;
	gDmacChx[chx].cfg.perCfg.intrDstTranEn = 1;

	gDmacChx[chx].cb[DMAC_INTERRUPT_BLOCK] = dmac_cb_close;
	gDmacChx[chx].cb[DMAC_INTERRUPT_DST_TRAN] = dmac_cb_dstTran;

	dmac_open_channel(chx, CHX_MEM_TO_PER);

	hal_dmac_soft_hs_set_dst(chx);
}
#endif

void app_dmac_test_main(void)
{
	dmac_init();
#if(APP_DMAC_TEST_TYPE == APP_DMAC_TEST_MEMORY_TO_MEMORY)
	app_dmac_test_m_to_m();
#endif
#if(APP_DMAC_TEST_TYPE == APP_DMAC_TEST_MEMORY_TO_I2S)
	app_dmac_test_m_to_i2s();
#endif
#if(APP_DMAC_TEST_TYPE == APP_DMAC_TEST_MEMORY_TO_SFC)
	app_dmac_test_sfc();
#endif
#if(APP_DMAC_TEST_TYPE == APP_DMAC_TEST_MEMORY_TO_UART)
	app_dmac_test_uart();
#endif
}
#endif	//DMA_TEST_MODE_ENABLE

#if(AUDIO_MANAGER)
typedef struct{
	fifo_handle_t *pFifoHandle;
	uint32_t fifoAddr;
	uint32_t * ptrTail;
	uint32_t * ptrHead;
	uint32_t maxLen;
	uint32_t frameSize;
	fifo_dmac_callback_t fifoCb;
}fifo_dma_cfg_t;

fifo_dma_cfg_t fifoDmaAdcCfg;
fifo_dma_cfg_t fifoDmaI2S1Cfg;
static volatile bool started = 0;
static uint32_t cnt = 0;

bool resetPtr = 1;
bool dmacAdcChxIsOpen = 0;
void dmac_adc_get_and_push_data(uint8_t chx)
{
	if(dmacAdcChxIsOpen)
	{
		uint32_t size = 8;
		if(started)
		{
			if(resetPtr)
			{
				resetPtr = 0;
			}
			else if(*fifoDmaAdcCfg.ptrTail == 0)
			{
				dmac_reopen_channel(chx, CODEC_DATA_BASE, fifoDmaAdcCfg.fifoAddr, 0);
				resetPtr = 1;
				return;
			}

			*fifoDmaAdcCfg.ptrTail += (gDmacChx[chx].cfg.blockSize * 4);

			if(*fifoDmaAdcCfg.ptrTail >= fifoDmaAdcCfg.maxLen)
			{
				dmac_reopen_channel(chx, CODEC_DATA_BASE, fifoDmaAdcCfg.fifoAddr, 0);
				*fifoDmaAdcCfg.ptrTail = 0;
				resetPtr = 1;
			}
			else
			{
				dmac_reopen_channel(chx, CODEC_DATA_BASE, 0, 0);
			}

			cnt+= (gDmacChx[chx].cfg.blockSize * 4);
			if(cnt >= fifoDmaAdcCfg.frameSize)
			{
				cnt = 0;
				fifoDmaAdcCfg.fifoCb(fifoDmaAdcCfg.pFifoHandle);
			}
		}
		else
		{
			started = 1;

			dmac_open_channel(chx, CHX_MEM_TO_MEM);
		}
	}
	else
	{
		int32_t trashSpace[10];
#if CODEC_ADC12_ENABLE
		hal_codec_read_adc12_data_24bit(&trashSpace[0], &trashSpace[1]);
#endif

#if CODEC_ADC34_ENABLE
		hal_codec_read_adc34_data_24bit(&trashSpace[2], &trashSpace[3]);
#endif

#if CODEC_ADC56_ENABLE
		hal_codec_read_adc56_data_24bit(&trashSpace[4], &trashSpace[5]);
#endif

#if CODEC_ADC78_ENABLE
		hal_codec_read_adc78_data_24bit(&trashSpace[6], &trashSpace[7]);
#endif

#if CODEC_ADC9A_ENABLE
		hal_codec_read_adc9a_data_24bit(&trashSpace[8], &trashSpace[9]);
#endif
	}
}

void dmac_cb_i2s_tx(uint8_t chx)
{
	*fifoDmaI2S1Cfg.ptrHead += (gDmacChx[chx].cfg.blockSize * 4);
	if(*fifoDmaI2S1Cfg.ptrHead >= fifoDmaI2S1Cfg.maxLen)
	{
		*fifoDmaI2S1Cfg.ptrHead = 0;
		gDmacChx[chx].cfg.srcAddr = fifoDmaI2S1Cfg.fifoAddr;
		dmac_reopen_channel(chx, gDmacChx[chx].cfg.srcAddr, 0, 0);
	}
	else
	{
		dmac_reopen_channel(chx, 0, 0, 0);
	}
}
void fifo_dmac_register_channel(fifo_handle_t *p_fifo, dmac_channel_num_t chx, fifo_dmac_param_t dmacParam, fifo_dmac_callback_t callback)
{
	switch(chx)
	{
	case DMA_CHX_MEM_TO_I2S1:
		gDmacChx[chx].cfg.trfType = DMAC_MEMORY_TO_PERIPHERAL;
		gDmacChx[chx].cfg.chPriority = 0;
		gDmacChx[chx].cfg.blockSize = dmacParam.blockSize;
		gDmacChx[chx].cfg.srcAddr = (uint32_t)dmacParam.pFifo;
		gDmacChx[chx].cfg.i2sSel = 0;
		gDmacChx[chx].cfg.intrBlockEn = 1;
		gDmacChx[chx].cb[DMAC_INTERRUPT_BLOCK] = dmac_cb_i2s_tx;

		fifoDmaI2S1Cfg.fifoAddr = (uint32_t)dmacParam.pFifo;
		fifoDmaI2S1Cfg.pFifoHandle = p_fifo;
		fifoDmaI2S1Cfg.ptrHead = dmacParam.pHeadPtr;
		fifoDmaI2S1Cfg.maxLen = dmacParam.maxLen;
		fifoDmaI2S1Cfg.fifoCb = callback;

		dmac_open_channel(chx, CHX_MEM_TO_I2S);
		break;
	case DMA_CHX_ADC_TO_MEM:
		gDmacChx[chx].cfg.trfType = DMAC_MEMORY_TO_MEMORY;
		gDmacChx[chx].cfg.chPriority = 0;
		gDmacChx[chx].cfg.blockSize = 2;
		fifoDmaAdcCfg.frameSize = dmacParam.blockSize;
		gDmacChx[chx].cfg.srcAddr = CODEC_DATA_BASE;
		gDmacChx[chx].cfg.dstAddr = (uint32_t)dmacParam.pFifo;
		gDmacChx[chx].cfg.intrBlockEn = 0;

		fifoDmaAdcCfg.fifoAddr = (uint32_t)dmacParam.pFifo;
		fifoDmaAdcCfg.pFifoHandle = p_fifo;
		fifoDmaAdcCfg.ptrTail = dmacParam.pTailPtr;
		fifoDmaAdcCfg.maxLen = dmacParam.maxLen;
		fifoDmaAdcCfg.fifoCb = callback;
		dmacAdcChxIsOpen = 1;
		break;
	default:
		break;
	}
}

void fifo_dmac_unregister_channel(dmac_channel_num_t chx)
{
	switch(chx)
	{
	case DMA_CHX_ADC_TO_MEM:
		dmac_close_channel(5);
		dmacAdcChxIsOpen = 0;
		break;
	default:
		break;
	}

}
#endif	//AUDIO_MANAGER
#endif	//DMA_ENABLE


