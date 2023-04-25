/** 
***********************************************************
 * 
 * @file name	: drv_i2s.c
 * @author		: zhangkun
 * @version		: V01.01
 * @date		: 2022-02-12
 * @brief		: I2S Driver moldule
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


/* Includes ------------------------------------------------------------------*/
#include "drv_i2s.h"
#include "boardConfig.h"
#include "hal_gpio.h"
#include "SaveRestoreVectorRegisters.h"
#include "drv_dmac.h"
#include "hal_sysctrl.h"
#include "i2s_sample_rate_detect.h"
#if(AUDIO_MANAGER)
#include "audio_manager.h"
#endif
#if SUPPORT_PLAY_TONE
#include "playTone.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if (I2S_ENABLE == ENABLE)
#if(AUDIO_MANAGER == 0)
static i2s_isr_cb_func g_i2s_cb_table[3][2] ;//= {0};
#else
static i2sDevInfos_t i2sDevInfos[I2S_INDEX_MAX];// = {0};
#endif
/* Private function prototypes -----------------------------------------------*/
#if I2S1_ENABLE
static void i2s1_isr_handler(void *arg);
#endif
#if I2S2_ENABLE
static void i2s2_isr_handler(void *arg);
#endif
#if I2S3_ENABLE
static void i2s3_isr_handler(void *arg);
#endif
/* Private functions ---------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#if (SUPPORT_ALTOTOOL_I2S)
extern i2s_cfgs_t gI2sCfg;
extern void overrideI2SCfg(i2s_cfg_t *i2s_cfg, int index);
#endif
uint8_t i2sIsrCount[3] = {0, 0, 0};
bool i2sWorking[3] = {true, true, true};


#if (I2S_ENABLE)
#if(AUDIO_MANAGER == 0)
/** 
 * @brief	I2S GPIO Config.
 * @param[in]	void  
 * @return	None
 */
void i2s_gpio_init(void)
{
	/**I2S0 GPIO Configuration
	GPIO_DATA0  ------> I2S1_SCLK
	GPIO_DATA1  ------> I2S1_LRCLK
	GPIO_DATA2	------> I2S1_SDI
	GPIO_DATA3	------> I2S1_SDO
	*/
#if (I2S1_ENABLE == ENABLE)
	hal_gpio_set_pinmux_function(GPIO_0, 2);
    hal_gpio_set_pinmux_function(GPIO_1, 2);
    hal_gpio_set_pinmux_function(GPIO_2, 2);
    hal_gpio_set_pinmux_function(GPIO_3, 2);
#endif

	/**I2S1 GPIO Configuration
	GPIO_DATA4  ------> I2S2_SCLK
	GPIO_DATA5  ------> I2S2_LRCLK
	GPIO_DATA6	------> I2S2_SDI
	GPIO_DATA7	------> I2S2_SDO
	*/
#if (I2S2_ENABLE == ENABLE)
	hal_gpio_set_pinmux_function(GPIO_4, 2);
	hal_gpio_set_pinmux_function(GPIO_5, 2);
	hal_gpio_set_pinmux_function(GPIO_6, 2);
	hal_gpio_set_pinmux_function(GPIO_7, 2);
#endif

	/**I2S2 GPIO Configuration
	GPIO_DATA12  ------> I2S3_SCLK
	GPIO_DATA13  ------> I2S3_LRCLK
	GPIO_DATA14	------> I2S3_SDI
	GPIO_DATA15	------> I2S3_SDO
	*/
#if (I2S3_ENABLE == ENABLE)
	hal_gpio_set_pinmux_function(GPIO_12, 1);
	hal_gpio_set_pinmux_function(GPIO_13, 1);
	hal_gpio_set_pinmux_function(GPIO_14, 1);
	hal_gpio_set_pinmux_function(GPIO_15, 1);
#endif
}

/** 
 * @brief	Initializes the I2S according to the specified paraemeters in the I2S_InitTyeDef.
 * @param[in]	void  
 * @return	DRV status
 */
drv_status_t i2s_init(void)
{
	i2s_cfg_t i2s_cfg;

	i2s_gpio_init();

	i2s_cfg.mode 	 	 = I2S_MODE_MASTER;
	i2s_cfg.standard	 = I2S_STANDARD_PHILIPS;
	i2s_cfg.audioFreq 	 = SAMPLING_RATE_48000;
	i2s_cfg.txDataLength = I2S_FIFO_DATALENGTH_32B;
	i2s_cfg.rxDataLength = I2S_FIFO_DATALENGTH_32B;
	i2s_cfg.frameLength  = I2S_FRAMELENGTH_32B;
	i2s_cfg.rxFifoLevel  = I2S_RX_FIFO_LEVEL;
	i2s_cfg.txFifolevel  = I2S_TX_FIFO_LEVEL;
	i2s_cfg.txEnable 	 = ENABLE;
	i2s_cfg.rxEnable 	 = ENABLE;

#if (I2S1_ENABLE == ENABLE)
	overrideI2SCfg(&i2s_cfg, I2S_1);

	hal_i2s_init(I2S_INDEX_1, &i2s_cfg);

#if (SUPPORT_ALTOTOOL_I2S)
	if(gI2sCfg.chxCfg[I2S_1].en)
		hal_i2s_start(I2S_INDEX_1);
#else
	hal_i2s_start(I2S_INDEX_1);
#endif

#ifndef I2S_DMA_ENABLE
	i2s_it_start(I2S_INDEX_1, I2S1_IRQn, app_i2s1_rx_callback, app_i2s1_tx_callback, i2s1_isr_handler);
#endif
#endif

#if (I2S2_ENABLE == ENABLE)
	overrideI2SCfg(&i2s_cfg, I2S_2);

#if (SUPPORT_ALTOTOOL_I2S)
	if(gI2sCfg.chxCfg[I2S_2].en)
		hal_i2s_start(I2S_INDEX_2);
#else
	hal_i2s_start(I2S_INDEX_2);
#endif

#ifndef I2S_DMA_ENABLE
	i2s_it_start(I2S_INDEX_2, I2S2_IRQn, app_i2s2_rx_callback, app_i2s2_tx_callback, i2s2_isr_handler);
#endif
#endif

#if (I2S3_ENABLE == ENABLE)
	overrideI2SCfg(&i2s_cfg, I2S_3);

#if (SUPPORT_ALTOTOOL_I2S)
	if(gI2sCfg.chxCfg[I2S_3].en)
		hal_i2s_start(I2S_INDEX_3);
#else
	hal_i2s_start(I2S_INDEX_3);
#endif

#ifndef I2S_DMA_ENABLE
	i2s_it_start(I2S_INDEX_3, I2S3_IRQn, app_i2s3_rx_callback, app_i2s3_tx_callback, i2s3_isr_handler);
#endif
#endif

	return DRV_STATUS_OK;
}

/** 
 * @brief	Register i2s interrupt and callback, enable interrupt.
 * @param[in]	i2sx    i2s index    
 * @return	DRV status
 * 
 */
drv_status_t i2s_it_start(uint8_t i2sx, hal_interrupt_irq_t irq_number, i2s_isr_cb_func rx_func, i2s_isr_cb_func tx_func, hal_interrupt_isr_t isr_func)
{
	i2s_register_cb(i2sx, rx_func, tx_func);
	hal_interrupt_register_isr_handler(irq_number, isr_func);
	hal_interrupt_enable_irq(irq_number);

	return DRV_STATUS_OK;
}

#else
#if (I2S1_ENABLE == ENABLE)
/** 
 * @brief	Initializes the I2S according to the specified paraemeters in the I2S_InitTyeDef.
 * @param[in]	void  
 * @return	DRV status
 */
drv_status_t i2s1_init(auDriverConfig_t *rxCfg, auDriverConfig_t *txCfg)
{
	if(i2sDevInfos[I2S_INDEX_1].initialized)
		return DRV_STATUS_OK;

	i2sDevInfos[I2S_INDEX_1].initialized = true;

	hal_sysctrl_set_clock_gate(hal_sysctrl_get_clock_gate() | (1 << HAL_SYSCTRL_CLKGT_I2S1));

	hal_gpio_set_pinmux_function(GPIO_0, 2);
    hal_gpio_set_pinmux_function(GPIO_1, 2);
    hal_gpio_set_pinmux_function(GPIO_2, 2);
    hal_gpio_set_pinmux_function(GPIO_3, 2);

	i2s_cfg_t i2s_cfg;
	auTransmitCompleteCB_t rxCB = NULL;
	auTransmitCompleteCB_t txCB = NULL;

	memset(&i2s_cfg, 0, sizeof(i2s_cfg_t));

	i2s_cfg.mode 	 	 = I2S_MODE_MASTER;
	i2s_cfg.standard	 = I2S_STANDARD_PHILIPS;
	i2s_cfg.frameLength  = I2S_FRAMELENGTH_32B;
	i2s_cfg.rxFifoLevel  = I2S_RX_FIFO_LEVEL;
	i2s_cfg.txFifolevel  = I2S_TX_FIFO_LEVEL;

	if(txCfg->format != NULL)
	{
		i2s_cfg.txEnable     = txCfg->attr->enable;
		i2s_cfg.audioFreq 	 = txCfg->format->sampleRateHz;
		i2s_cfg.txDataLength = I2S_FIFO_WORD_LENGTH;
		i2sDevInfos[I2S_INDEX_1].txCbArg = txCfg->attr->pStream;
		i2sDevInfos[I2S_INDEX_1].channel = txCfg->format->channlesPerFrame;
		i2sDevInfos[I2S_INDEX_1].bitSlot = txCfg->format->bitSlot;
		i2sDevInfos[I2S_INDEX_1].wordSize = txCfg->attr->formatCap->wordSize;
		i2sDevInfos[I2S_INDEX_1].samplingRate = txCfg->format->sampleRateHz;

		txCB = txCfg->attr->isrCb;

		if (txCfg->attr->useDMA)
		{
			txCB = NULL;
#ifdef I2S_DMA_ENABLE
			hal_i2s_tx_dma_init(I2S_INDEX_1);
#endif
		}
	}

	if(rxCfg->format != NULL)
	{
		i2s_cfg.rxEnable     = rxCfg->attr->enable;
		i2s_cfg.audioFreq 	 = rxCfg->format->sampleRateHz;
		i2s_cfg.rxDataLength = I2S_FIFO_WORD_LENGTH;
		i2sDevInfos[I2S_INDEX_1].rxCbArg = rxCfg->attr->pStream;
		i2sDevInfos[I2S_INDEX_1].channel = rxCfg->format->channlesPerFrame;
		i2sDevInfos[I2S_INDEX_1].bitSlot = rxCfg->format->bitSlot;
		i2sDevInfos[I2S_INDEX_1].wordSize = rxCfg->attr->formatCap->wordSize;
		i2sDevInfos[I2S_INDEX_1].samplingRate = rxCfg->format->sampleRateHz;

		rxCB = rxCfg->attr->isrCb;

		if (rxCfg->attr->useDMA)
		{
			rxCB = NULL;
#ifdef I2S_DMA_ENABLE
			hal_i2s_rx_dma_init(I2S_INDEX_1);
#endif
		}
	}

#ifdef I2S_DMA_ENABLE
		i2sDevInfos[I2S_INDEX_1].enable = FALSE;
#else
		i2sDevInfos[I2S_INDEX_1].enable = TRUE;
#endif

#if (SUPPORT_ALTOTOOL_I2S)
	overrideI2SCfg(&i2s_cfg,I2S_1);
#endif

	hal_i2s_init(I2S_INDEX_1, &i2s_cfg);

	i2s_it_start(I2S_INDEX_1, I2S1_IRQn, rxCB, txCB, i2s1_isr_handler);

	return DRV_STATUS_OK;
}

/** 
 * @brief	DeInitializes the I2S peripheral
 * @param[in]	i2sx	i2s index	 
 * @return	DRV status
 * 
 */
drv_status_t i2s1_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	hal_i2s_deinit(I2S_INDEX_1);

	return DRV_STATUS_OK;
}

/** 
 * @brief		Restart. 
 * @param[in]	i2sx	i2s index 
 * @return	DRV status
 */
drv_status_t i2s1_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{	
	hal_i2s_reinit(I2S_INDEX_1);
	hal_i2s_start(I2S_INDEX_1);

	return DRV_STATUS_OK;
}

void i2s1_set_audio_freq(uint32_t audioFreq)
{
	hal_i2s_set_audio_freq(I2S_INDEX_1, audioFreq);
	hal_i2s_start(I2S_INDEX_1);
//	uart_printf("audioFreq = %d\r\n.\r\n", audioFreq);
}

uint32_t i2s1_get_audio_freq(void)
{
	return hal_i2s_get_audio_freq(I2S_INDEX_1);
}

drv_status_t i2s1_start(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	uart_printf("i2s1_start.\n");
#if (SUPPORT_ALTOTOOL_I2S)
	if(gI2sCfg.chxCfg[I2S_1].en)
		hal_i2s_start(I2S_INDEX_1);
#else
	hal_i2s_start(I2S_INDEX_1);
#endif
#ifdef I2S_DMA_ENABLE
	dmac_open_channel(1, CHX_MEM_TO_I2S);
#endif
	if (i2sDevInfos[I2S_INDEX_1].enable)
		hal_interrupt_enable_irq(I2S1_IRQn);
	return DRV_STATUS_OK;
}
#endif

#if (I2S2_ENABLE == ENABLE)
/** 
 * @brief	Initializes the I2S according to the specified paraemeters in the I2S_InitTyeDef.
 * @param[in]	void  
 * @return	DRV status
 */
drv_status_t i2s2_init(auDriverConfig_t *rxCfg, auDriverConfig_t *txCfg)
{
	if(i2sDevInfos[I2S_INDEX_2].initialized)
		return DRV_STATUS_OK;

	i2sDevInfos[I2S_INDEX_2].initialized = true;

	hal_sysctrl_set_clock_gate(hal_sysctrl_get_clock_gate() | (1 << HAL_SYSCTRL_CLKGT_I2S2));

	hal_gpio_set_pinmux_function(GPIO_4, 2);
    hal_gpio_set_pinmux_function(GPIO_5, 2);
    hal_gpio_set_pinmux_function(GPIO_6, 2);
    hal_gpio_set_pinmux_function(GPIO_7, 2);

	i2s_cfg_t i2s_cfg;
	auTransmitCompleteCB_t rxCB = NULL;
	auTransmitCompleteCB_t txCB = NULL;

	memset(&i2s_cfg, 0, sizeof(i2s_cfg_t));

	i2s_cfg.mode 	 	 = I2S_MODE_MASTER;
	i2s_cfg.standard	 = I2S_STANDARD_PHILIPS;
	i2s_cfg.frameLength  = I2S_FRAMELENGTH_32B;
	i2s_cfg.rxFifoLevel  = I2S_RX_FIFO_LEVEL;
	i2s_cfg.txFifolevel  = I2S_TX_FIFO_LEVEL;

	if(txCfg->format != NULL)
	{
		i2s_cfg.txEnable     = txCfg->attr->enable;
		i2s_cfg.audioFreq 	 = txCfg->format->sampleRateHz;
		i2s_cfg.txDataLength = I2S_FIFO_WORD_LENGTH;
		i2sDevInfos[I2S_INDEX_2].txCbArg = txCfg->attr->pStream;
		i2sDevInfos[I2S_INDEX_2].channel = txCfg->format->channlesPerFrame;
		i2sDevInfos[I2S_INDEX_2].bitSlot = txCfg->format->bitSlot;
		i2sDevInfos[I2S_INDEX_2].wordSize = txCfg->attr->formatCap->wordSize;
		i2sDevInfos[I2S_INDEX_2].samplingRate = txCfg->format->sampleRateHz;

		txCB = txCfg->attr->isrCb;

		if (txCfg->attr->useDMA)
		{
			txCB = NULL;
#ifdef I2S_DMA_ENABLE
			hal_i2s_tx_dma_init(I2S_INDEX_2);
#endif
		}
	}

	if(rxCfg->format != NULL)
	{
		i2s_cfg.rxEnable     = rxCfg->attr->enable;
		i2s_cfg.audioFreq 	 = rxCfg->format->sampleRateHz;
		i2s_cfg.rxDataLength = I2S_FIFO_WORD_LENGTH;
		i2sDevInfos[I2S_INDEX_2].rxCbArg = rxCfg->attr->pStream;
		i2sDevInfos[I2S_INDEX_2].channel = rxCfg->format->channlesPerFrame;
		i2sDevInfos[I2S_INDEX_2].bitSlot = rxCfg->format->bitSlot;
		i2sDevInfos[I2S_INDEX_2].wordSize = rxCfg->attr->formatCap->wordSize;
		i2sDevInfos[I2S_INDEX_2].samplingRate = rxCfg->format->sampleRateHz;

		rxCB = rxCfg->attr->isrCb;

		if (rxCfg->attr->useDMA)
		{
			rxCB = NULL;
#ifdef I2S_DMA_ENABLE
			hal_i2s_rx_dma_init(I2S_INDEX_2);
#endif
		}
	}

#ifdef I2S_DMA_ENABLE
		i2sDevInfos[I2S_INDEX_2].enable = FALSE;
#else
		i2sDevInfos[I2S_INDEX_2].enable = TRUE;
#endif

#if (SUPPORT_ALTOTOOL_I2S)
	overrideI2SCfg(&i2s_cfg,I2S_2 );
#endif

	hal_i2s_init(I2S_INDEX_2, &i2s_cfg);

	i2s_it_start(I2S_INDEX_2, I2S2_IRQn, rxCB, txCB, i2s2_isr_handler);
	
	return DRV_STATUS_OK;
}

/** 
 * @brief	DeInitializes the I2S peripheral
 * @param[in]	i2sx	i2s index	 
 * @return	DRV status
 * 
 */
drv_status_t i2s2_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	hal_i2s_deinit(I2S_INDEX_2);

	return DRV_STATUS_OK;
}

/** 
 * @brief		Restart. 
 * @param[in]	i2sx	i2s index 
 * @return	DRV status
 */
drv_status_t i2s2_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{	
	hal_i2s_reinit(I2S_INDEX_2);
	hal_i2s_start(I2S_INDEX_2);

	return DRV_STATUS_OK;
}
void i2s2_set_audio_freq(uint32_t audioFreq)
{
	hal_i2s_set_audio_freq(I2S_INDEX_2, audioFreq);
	hal_i2s_start(I2S_INDEX_2);
}

uint32_t i2s2_get_audio_freq(void)
{
	return hal_i2s_get_audio_freq(I2S_INDEX_2);
}

drv_status_t i2s2_start(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	uart_printf("i2s2_start.\n");
#if (SUPPORT_ALTOTOOL_I2S)
	if(gI2sCfg.chxCfg[I2S_2].en)
		hal_i2s_start(I2S_INDEX_2);
#else
	hal_i2s_start(I2S_INDEX_2);
#endif

#ifdef I2S_DMA_ENABLE
	dmac_open_channel(1, CHX_MEM_TO_I2S);
#endif
	if (i2sDevInfos[I2S_INDEX_2].enable)
		hal_interrupt_enable_irq(I2S2_IRQn);
	return DRV_STATUS_OK;
}
#endif

#if (I2S3_ENABLE == ENABLE)
/** 
 * @brief	Initializes the I2S according to the specified paraemeters in the I2S_InitTyeDef.
 * @param[in]	void  
 * @return	DRV status
 */
drv_status_t i2s3_init(auDriverConfig_t *rxCfg, auDriverConfig_t *txCfg)
{
	if(i2sDevInfos[I2S_INDEX_3].initialized)
		return DRV_STATUS_OK;

	i2sDevInfos[I2S_INDEX_3].initialized = true;

	hal_sysctrl_set_clock_gate(hal_sysctrl_get_clock_gate() | (1 << HAL_SYSCTRL_CLKGT_I2S3));

	hal_gpio_set_pinmux_function(GPIO_12, 1);
	hal_gpio_set_pinmux_function(GPIO_13, 1);
	hal_gpio_set_pinmux_function(GPIO_14, 1);
	hal_gpio_set_pinmux_function(GPIO_15, 1);

	i2s_cfg_t i2s_cfg;
	auTransmitCompleteCB_t rxCB = NULL;
	auTransmitCompleteCB_t txCB = NULL;

	memset(&i2s_cfg, 0, sizeof(i2s_cfg_t));

	i2s_cfg.mode 	 	 = I2S_MODE_SLAVE;
	i2s_cfg.standard	 = I2S_STANDARD_PHILIPS;
	i2s_cfg.frameLength  = I2S_FRAMELENGTH_32B;
	i2s_cfg.rxFifoLevel  = I2S_RX_FIFO_LEVEL;
	i2s_cfg.txFifolevel  = I2S_TX_FIFO_LEVEL;

	if(txCfg->format != NULL)
	{
		i2s_cfg.txEnable     = txCfg->attr->enable;
		i2s_cfg.audioFreq 	 = txCfg->format->sampleRateHz;
		i2s_cfg.txDataLength = I2S_FIFO_WORD_LENGTH;
		i2sDevInfos[I2S_INDEX_3].txCbArg = txCfg->attr->pStream;
		i2sDevInfos[I2S_INDEX_3].channel = txCfg->format->channlesPerFrame;
		i2sDevInfos[I2S_INDEX_3].bitSlot = txCfg->format->bitSlot;
		i2sDevInfos[I2S_INDEX_3].wordSize = txCfg->attr->formatCap->wordSize;
		i2sDevInfos[I2S_INDEX_3].samplingRate = txCfg->format->sampleRateHz;

		txCB = txCfg->attr->isrCb;

		if (txCfg->attr->useDMA)
		{
			txCB = NULL;
#ifdef I2S_DMA_ENABLE
			hal_i2s_tx_dma_init(I2S_INDEX_3);
#endif
		}
	}

	if(rxCfg->format != NULL)
	{
		i2s_cfg.rxEnable     = rxCfg->attr->enable;
		i2s_cfg.audioFreq 	 = rxCfg->format->sampleRateHz;
		i2s_cfg.rxDataLength = I2S_FIFO_WORD_LENGTH;
		i2sDevInfos[I2S_INDEX_3].rxCbArg = rxCfg->attr->pStream;
		i2sDevInfos[I2S_INDEX_3].channel = rxCfg->format->channlesPerFrame;
		i2sDevInfos[I2S_INDEX_3].bitSlot = rxCfg->format->bitSlot;
		i2sDevInfos[I2S_INDEX_3].wordSize = rxCfg->attr->formatCap->wordSize;
		i2sDevInfos[I2S_INDEX_3].samplingRate = rxCfg->format->sampleRateHz;

		rxCB = rxCfg->attr->isrCb;

		if (rxCfg->attr->useDMA)
		{
			rxCB = NULL;
#ifdef I2S_DMA_ENABLE
			hal_i2s_rx_dma_init(I2S_INDEX_3);
#endif
		}
	}

#ifdef I2S_DMA_ENABLE
		i2sDevInfos[I2S_INDEX_3].enable = FALSE;
#else
		i2sDevInfos[I2S_INDEX_3].enable = TRUE;
#endif

#if (SUPPORT_ALTOTOOL_I2S)
	overrideI2SCfg(&i2s_cfg, I2S_3);
#endif
	
	hal_i2s_init(I2S_INDEX_3, &i2s_cfg);

	i2s_it_start(I2S_INDEX_3, I2S3_IRQn, rxCB, txCB, i2s3_isr_handler);

	return DRV_STATUS_OK;
}

/** 
 * @brief	DeInitializes the I2S peripheral
 * @param[in]	i2sx	i2s index	 
 * @return	DRV status
 * 
 */
drv_status_t i2s3_deinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{
	hal_i2s_deinit(I2S_INDEX_3);

	return DRV_STATUS_OK;
}

/** 
 * @brief		Restart. 
 * @param[in]	i2sx	i2s index 
 * @return	DRV status
 */
drv_status_t i2s3_reinit(auDriverConfig_t *capture, auDriverConfig_t *playback)
{	
	hal_i2s_reinit(I2S_INDEX_3);
	hal_i2s_start(I2S_INDEX_3);

	return DRV_STATUS_OK;
}

void i2s3_set_audio_freq(uint32_t audioFreq)
{
	hal_i2s_set_audio_freq(I2S_INDEX_3, audioFreq);
	hal_i2s_start(I2S_INDEX_3);
}

uint32_t i2s3_get_audio_freq(void)
{
	return hal_i2s_get_audio_freq(I2S_INDEX_3);
}
	
drv_status_t i2s3_start(auDriverConfig_t *capture, auDriverConfig_t *playback)
{	
	uart_printf("i2s3_start.\n");
#if (SUPPORT_ALTOTOOL_I2S)
	if(gI2sCfg.chxCfg[I2S_3].en)
		hal_i2s_start(I2S_INDEX_3);
#else
	hal_i2s_start(I2S_INDEX_3);
#endif

#ifdef I2S_DMA_ENABLE
	dmac_open_channel(1, CHX_MEM_TO_I2S);
#endif
	if (i2sDevInfos[I2S_INDEX_3].enable)
		hal_interrupt_enable_irq(I2S3_IRQn);
	return DRV_STATUS_OK;
}
#endif

/** 
 * @brief	Register i2s interrupt and callback, enable interrupt.
 * @param[in]	i2sx    i2s index    
 * @return	DRV status
 * 
 */
drv_status_t i2s_it_start(uint8_t i2sx, hal_interrupt_irq_t irq_number, auTransmitCompleteCB_t rx_func, auTransmitCompleteCB_t tx_func, hal_interrupt_isr_t isr_func)
{
	i2s_register_cb(i2sx, rx_func, tx_func);
	hal_interrupt_register_isr_handler(irq_number, isr_func);

	return DRV_STATUS_OK;
}

#endif

/** 
 * @brief	DeInitializes the I2S peripheral
 * @param[in]	i2sx    i2s index    
 * @return	DRV status
 * 
 */
drv_status_t i2s_deinit(uint8_t i2sx)
{
	hal_i2s_deinit(i2sx);

	return DRV_STATUS_OK;
}

/** 
 * @brief	 	Restart. 
 * @param[in]	i2sx    i2s index 
 * @return	DRV status
 */
drv_status_t i2s_reinit(uint8_t i2sx)
{	
	hal_i2s_reinit(i2sx);
	hal_i2s_start(i2sx);

	return DRV_STATUS_OK;
}

/** 
 * @brief	 	Set i2s sample rate. 
 * @param[in]	i2sx    i2s index 
 * @param[in]	audioFreq    sample rate
 * @return	DRV status
 */
drv_status_t i2s_set_audio_freq(uint8_t i2sx, uint32_t audioFreq)
{
	hal_i2s_set_audio_freq(i2sx, audioFreq);
	hal_i2s_start(i2sx);

	return DRV_STATUS_OK;
}

/** 
 * @brief		This function handles I2S interrupt status
 * @param[in]	void
 * @return  	None
 */
void i2s_isr_status_handler(uint8_t i2sx)
{
	if(i2sx == I2S_INDEX_1)
	{	
	 	SaveVectors(I2S1_IRQn);
	}
	else if(i2sx == I2S_INDEX_2)
	{
		SaveVectors(I2S2_IRQn);
	}
	else
	{
		SaveVectors(I2S3_IRQn);
	}

	 ALIGN(8) int32_t txData[I2S_TX_FIFO_LEVEL*2] = {0};
	 ALIGN(8) int32_t rxData[I2S_RX_FIFO_LEVEL*2] = {0};

	 uint8_t txCount = 0;
	 uint8_t rxCount = 0;
	 uint32_t len = 0;

	 /* I2S in mode Transmitter -----------empty interrupt*/
	if(hal_i2s_tx_ready(i2sx))
	{	
#if (AUDIO_MANAGER == 0)
		app_audio_mic_get_data(txData);
#else
		len = I2S_TX_FIFO_LEVEL*i2sDevInfos[i2sx].channel*BIT_SLOT_32;
		if (i2sDevInfos[i2sx].isrCb[0])
			i2sDevInfos[i2sx].isrCb[0](i2sDevInfos[i2sx].txCbArg, 
										&txData[0], 
										&len,
										BIT_SLOT_32);
#endif

#if (SUPPORT_ALTOTOOL_I2S)
		if(gI2sCfg.chxCfg[i2sx].mute)
			memset(txData, 0, I2S_TX_FIFO_LEVEL*2*4);
#endif

		while(txCount < I2S_TX_FIFO_LEVEL*2)
		{
#if (SUPPORT_PLAY_TONE && D_I2S_PLAY_TONE_ENABLE)
			int32_t toneData;
			float fadeScale = 0;
#if (PREEMPTIVE_MODE == 1)
			float fadeScaleTone = 0;
#if I2S_SAMPLE_RATE_DETECT
            extern	uint32_t get_i2s_sample_rate(void);
			i2sDevInfos[i2sx].samplingRate = get_i2s_sample_rate();
#endif
			int readOffset = PlayToneGetData(PLAY_TONE_I2S, &toneData, &fadeScale, &fadeScaleTone, i2sDevInfos[i2sx].samplingRate);
#else
			int readOffset = PlayToneGetData(PLAY_TONE_I2S, &toneData, &fadeScale, i2sDevInfos[i2sx].samplingRate);
#endif
			if(readOffset >= 0)
			{
				//play tone Fade in-out
				if(fadeScale > 0)
				{
					txData[txCount] = fadeScale*txData[txCount];
					txData[txCount + 1] = fadeScale*txData[txCount + 1];
				}
				else
				{
	    #if (PREEMPTIVE_MODE == 1)
					if (fadeScaleTone != 0)
					{
					#if D_PLAY_TONE_OUTPUT_MIX
						txData[txCount] = toneData * fadeScaleTone + txData[txCount];
						txData[txCount + 1] = toneData * fadeScaleTone + txData[txCount + 1];
					#else
						txData[txCount] = toneData * fadeScaleTone;
						txData[txCount + 1] = toneData * fadeScaleTone;
					#endif
					}
					else
        #endif
					{
					#if D_PLAY_TONE_OUTPUT_MIX
						txData[txCount] = toneData + txData[txCount];
						txData[txCount + 1] = toneData + txData[txCount + 1];
					#else
						txData[txCount] = toneData;
						txData[txCount + 1] = toneData;
					#endif
					}
				}
			}
#endif

#if (D_CONFIG_2MIC_BT_HEADPHONE)
			txData[txCount] = txData[txCount] >> 16;
			txData[txCount+1] = txData[txCount+1] >> 16;
#endif

			hal_i2s_tx_data(i2sx, &txData[txCount]);
			txCount += 2;
		}
	}

	/* I2S in mode Receiver -----------full interrupt*/
	if(hal_i2s_rx_ready(i2sx))
	{
		while(rxCount < I2S_RX_FIFO_LEVEL*2)
		{
			hal_i2s_rx_data(i2sx, &rxData[rxCount]);
#if (I2S_SAMPLE_RATE_DETECT)
			i2s_sr_detect_count();
#endif
#if (D_CONFIG_2MIC_BT_HEADPHONE)
			rxData[rxCount] = rxData[rxCount] << 8;
			rxData[rxCount+1] = rxData[rxCount+1] << 8;
#endif
			rxCount += 2;
		}

#if (SUPPORT_ALTOTOOL_I2S)
		if(gI2sCfg.chxCfg[i2sx].mute)
			memset(rxData, 0, I2S_RX_FIFO_LEVEL*2*4);
#endif

#if (AUDIO_MANAGER == 0)
 		audio_spk_add_data(rxData);
#else
		len = I2S_RX_FIFO_LEVEL*i2sDevInfos[i2sx].channel*BIT_SLOT_32;
		if (i2sDevInfos[i2sx].isrCb[1])
			i2sDevInfos[i2sx].isrCb[1](i2sDevInfos[i2sx].rxCbArg, 
										&rxData[0], 
										&len,
										BIT_SLOT_32);
#endif

		if(i2sx == I2S_INDEX_1)
		{
			i2sIsrCount[I2S_INDEX_1]++;
		}
		else if(i2sx == I2S_INDEX_2)
		{
			i2sIsrCount[I2S_INDEX_2]++;
		}
		else if(i2sx == I2S_INDEX_3)
		{
			i2sIsrCount[I2S_INDEX_3]++;
		}
	}
	
	/* I2S TX Overrun errror interrupt occurred--------------------*/
	if(hal_i2s_tx_overrun(i2sx))
	{
		hal_i2s_clean_tx_overrun_status(i2sx);
	}

	/* I2S RX Overrun errror interrupt occurred--------------------*/
	if(hal_i2s_rx_overrun(i2sx))
	{
		hal_i2s_clean_rx_overrun_status(i2sx);
	}

	if(i2sx == I2S_INDEX_1)
	{	
	 	RestoreVectors(I2S1_IRQn);
	}
	else if(i2sx == I2S_INDEX_2)
	{
		RestoreVectors(I2S2_IRQn);
	}
	else
	{
		RestoreVectors(I2S3_IRQn);
	}
}

#if (I2S1_ENABLE == ENABLE) 
/** 
 * @brief		This function handles I2S interrupt
 * @param[in]	void
 * @return  	None
 */
static void i2s1_isr_handler(void *arg)
{
	 i2s_isr_status_handler(I2S_INDEX_1);
}
#endif

#if (I2S2_ENABLE == ENABLE)
static void i2s2_isr_handler(void *arg)
{
	 i2s_isr_status_handler(I2S_INDEX_2);
}
#endif 

#if (I2S3_ENABLE == ENABLE)
static void i2s3_isr_handler(void *arg)
{
	 i2s_isr_status_handler(I2S_INDEX_3);	
}
#endif

/** 
 * @brief This function is used to register the callback function of the ISRs. The callback function can handle the i2s datas.
 * @param[in] index The location the callback function is registered. This parameter can only be one of the type #I2S_INDEX_X
 * @param[in] cb The callback function is registered.
 * @return	
 * 	@retval DRV_STATUS_OK The callback function is successfully registered
 */
#if(AUDIO_MANAGER == 0)
drv_status_t i2s_register_cb(uint8_t i2s_index, i2s_isr_cb_func rx_func, i2s_isr_cb_func tx_func)
{
	ASSERT(i2s_index < 3);
	
	g_i2s_cb_table[i2s_index][0] = tx_func; 
	g_i2s_cb_table[i2s_index][1] = rx_func;

	return DRV_STATUS_OK;
}
#else
drv_status_t i2s_register_cb(uint8_t i2s_index, auTransmitCompleteCB_t rx_func, auTransmitCompleteCB_t tx_func)
{
	ASSERT(i2s_index < 3);

	i2sDevInfos[i2s_index].isrCb[0] = tx_func; 
	i2sDevInfos[i2s_index].isrCb[1] = rx_func;

	return DRV_STATUS_OK;
}
#endif

drv_status_t i2s_enable(uint8_t i2sx, uint8_t enable)
{
	hal_i2s_enable(i2sx, enable);
#if (SUPPORT_ALTOTOOL_I2S)
	gI2sCfg.chxCfg[i2sx].en = enable;
#endif

	return DRV_STATUS_OK;
}

drv_status_t i2s_tx_enable(uint8_t i2sx, uint8_t enable)
{
	hal_i2s_tx_enable(i2sx, enable);
#if (SUPPORT_ALTOTOOL_I2S)
	gI2sCfg.chxCfg[i2sx].txEn = enable;
#endif

	return DRV_STATUS_OK;
}

drv_status_t i2s_rx_enable(uint8_t i2sx, uint8_t enable)
{
	hal_i2s_rx_enable(i2sx, enable);
#if (SUPPORT_ALTOTOOL_I2S)
	gI2sCfg.chxCfg[i2sx].rxEn = enable;
#endif

	return DRV_STATUS_OK;
}

drv_status_t i2s_set_mode(uint8_t i2sx, uint8_t mode)
{
	hal_i2s_set_mode(i2sx, mode);
#if (SUPPORT_ALTOTOOL_I2S)
	gI2sCfg.chxCfg[i2sx].isMaster = mode;
#endif

	return DRV_STATUS_OK;
}

drv_status_t i2s_set_format(uint8_t i2sx, uint8_t format)
{
	hal_i2s_set_format(i2sx, format);
#if (SUPPORT_ALTOTOOL_I2S)
	gI2sCfg.chxCfg[i2sx].format = format;
#endif

	return DRV_STATUS_OK;
}

drv_status_t i2s_set_sample_rate(uint8_t i2sx, uint32_t sampleRate)
{
	hal_i2s_set_sample_rate(i2sx, sampleRate);
#if (SUPPORT_ALTOTOOL_I2S)
	gI2sCfg.chxCfg[i2sx].sr = sampleRate;
#endif

	return DRV_STATUS_OK;
}

drv_status_t i2s_set_word_length(uint8_t i2sx, uint8_t wordLength)
{
	hal_i2s_set_word_length(i2sx, wordLength);
#if (SUPPORT_ALTOTOOL_I2S)
	gI2sCfg.chxCfg[i2sx].dataWidth = wordLength;
#endif

	return DRV_STATUS_OK;
}

drv_status_t i2s_set_frame_length(uint8_t i2sx, uint8_t frameLength)
{
	hal_i2s_set_frame_length(i2sx, frameLength);
#if (SUPPORT_ALTOTOOL_I2S)
	gI2sCfg.chxCfg[i2sx].frameLength = frameLength;
#endif

	return DRV_STATUS_OK;
}

drv_status_t i2s_set_mute(uint8_t i2sx, uint8_t mute)
{
#if (SUPPORT_ALTOTOOL_I2S)
	gI2sCfg.chxCfg[i2sx].mute = mute;
#endif

	return DRV_STATUS_OK;
}

uint8_t i2s_get_packages_per_ms(uint8_t i2sId)
{
	if(i2sId == I2S_INDEX_1)
	{
		return i2sDevInfos[I2S_INDEX_1].samplingRate/(I2S_TX_FIFO_LEVEL*1000);//I2S_TX_FIFO_LEVEL and I2S_RX_FIFO_LEVEL mast be the same
	}
	else if(i2sId == I2S_INDEX_2)
	{
		return i2sDevInfos[I2S_INDEX_2].samplingRate/(I2S_TX_FIFO_LEVEL*1000);
	}
	else if(i2sId == I2S_INDEX_3)
	{
		return i2sDevInfos[I2S_INDEX_3].samplingRate/(I2S_TX_FIFO_LEVEL*1000);
	}
	return 0;
}


void i2s_dectect_play_state_polling(void)
{
	static uint8_t timeCount = 0;

	if(++timeCount < 3)
		return;
	if(i2sWorking[I2S_INDEX_1] || i2sIsrCount[I2S_INDEX_1] != 0)
	{
		auStream_t *stream = (auStream_t *)i2sDevInfos[I2S_INDEX_1].rxCbArg;
		
		if(stream != NULL && i2sIsrCount[I2S_INDEX_1] < (timeCount - 1)*i2s_get_packages_per_ms(I2S_INDEX_1) && i2sWorking[I2S_INDEX_1])
		{
			i2sWorking[I2S_INDEX_1] = false;
			stream->fifo->fuc->set_playing_state(stream->fifo, i2sWorking[I2S_INDEX_1]);
			uart_printf("i2s1 stop\n");
		}
		else if(stream != NULL && i2sIsrCount[I2S_INDEX_1] >= (timeCount - 1)*i2s_get_packages_per_ms(I2S_INDEX_1) && !i2sWorking[I2S_INDEX_1])
		{
			i2sWorking[I2S_INDEX_1] = true;
			stream->fifo->fuc->set_playing_state(stream->fifo, i2sWorking[I2S_INDEX_1]);
			uart_printf("i2s1 play\n");
		}
		i2sIsrCount[I2S_INDEX_1] = 0;
	}
	if(i2sWorking[I2S_INDEX_2] || i2sIsrCount[I2S_INDEX_2] != 0)
	{
		auStream_t *stream = (auStream_t *)i2sDevInfos[I2S_INDEX_2].rxCbArg;
		
		if(stream != NULL && i2sIsrCount[I2S_INDEX_2] < (timeCount - 1)*i2s_get_packages_per_ms(I2S_INDEX_2) && i2sWorking[I2S_INDEX_2])
		{
			i2sWorking[I2S_INDEX_2] = false;
			stream->fifo->fuc->set_playing_state(stream->fifo, i2sWorking[I2S_INDEX_2]);
			uart_printf("i2s2 stop\n");
		}
		else if(stream != NULL && i2sIsrCount[I2S_INDEX_2] >= (timeCount - 1)*i2s_get_packages_per_ms(I2S_INDEX_2) && !i2sWorking[I2S_INDEX_2])
		{
			i2sWorking[I2S_INDEX_2] = true;
			stream->fifo->fuc->set_playing_state(stream->fifo, i2sWorking[I2S_INDEX_2]);
			uart_printf("i2s2 play\n");
		}
		i2sIsrCount[I2S_INDEX_2] = 0;
	}
	if(i2sWorking[I2S_INDEX_3] || i2sIsrCount[I2S_INDEX_3] != 0)
	{
		auStream_t *stream = (auStream_t *)i2sDevInfos[I2S_INDEX_3].rxCbArg;
		
		if(stream != NULL && i2sIsrCount[I2S_INDEX_3] < (timeCount - 1)*i2s_get_packages_per_ms(I2S_INDEX_3) && i2sWorking[I2S_INDEX_3])
		{
			i2sWorking[I2S_INDEX_3] = false;
			stream->fifo->fuc->set_playing_state(stream->fifo, i2sWorking[I2S_INDEX_3]);
			uart_printf("i2s3 stop\n");
		}
		else if(stream != NULL && i2sIsrCount[I2S_INDEX_3] >= (timeCount - 1)*i2s_get_packages_per_ms(I2S_INDEX_3) && !i2sWorking[I2S_INDEX_3])
		{
			i2sWorking[I2S_INDEX_3] = true;
			stream->fifo->fuc->set_playing_state(stream->fifo, i2sWorking[I2S_INDEX_3]);
			uart_printf("i2s3 play\n");
		}
		i2sIsrCount[I2S_INDEX_3] = 0;
	}

	timeCount = 0;
}


#ifdef __cplusplus
}
#endif

#endif
#endif

