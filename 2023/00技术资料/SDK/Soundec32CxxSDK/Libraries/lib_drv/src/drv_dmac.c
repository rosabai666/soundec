/** 
***********************************************************
 * 
 * @file name	: drv_dmac.c
 * @author		: RandyFan
 * @version		: V02.02.demo
 * @date		: 2022-01-04
 * @brief		: DMAC Driver
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


/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "drv_dmac.h"
#include "hal_dmac.h"
#include "hal_interrupt.h"
#include "snc8600.h"
#include "common.h"
#include "SaveRestoreVectorRegisters.h"
#include "hal_sysctrl.h"
#ifdef __cplusplus
extern "C" {
#endif

#if(DMA_ENABLE)

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define DMAC_DEBUG(...)		uart_printf(__VA_ARGS__)
/* Private variables ---------------------------------------------------------*/
static bool dmacIsInited = false;
dmac_channel_t gDmacChx[DMAC_CHANNEL_NUM] = {0};
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
 * @brief	This function is the interrupt handler of the DMA Controller.
 * @note	None
 * @return	None
 */
void dmac_isr(void* arg)
{
	SaveVectors(DMAC_IRQn);

	uint8_t intrBlock = hal_dmac_get_intr_block();
	uint8_t intrSrcTran = hal_dmac_get_intr_src_tran();
	uint8_t intrDstTran = hal_dmac_get_intr_dst_tran();

	uint8_t i = DMAC_CHANNEL_NUM;

	while(i--)
	{
		if(gDmacChx[i].isOpen == true)
		{
			if(intrBlock & (0x01 << i))
			{
//				DMAC_DEBUG("Bk:%d\n", i);
				if (gDmacChx[i].cb[DMAC_INTERRUPT_BLOCK])
					gDmacChx[i].cb[DMAC_INTERRUPT_BLOCK](i);
				hal_dmac_clr_intr_block(i);
			}
			if(intrSrcTran & (0x01 << i))
			{
				if (gDmacChx[i].cb[DMAC_INTERRUPT_SRC_TRAN])
					gDmacChx[i].cb[DMAC_INTERRUPT_SRC_TRAN](i);
				hal_dmac_clr_intr_src_tran(i);
			}
			if(intrDstTran & (0x01 << i))
			{
				if (gDmacChx[i].cb[DMAC_INTERRUPT_DST_TRAN])
					gDmacChx[i].cb[DMAC_INTERRUPT_DST_TRAN](i);
				hal_dmac_clr_intr_dst_tran(i);
			}
		}
	}
	RestoreVectors(DMAC_IRQn);
}

/** 
 * @brief	This function is used to initialize the DMA Controller. It will enable DMA, 
 * @note	This function should be called before calling dmac_open_channel
 * @return	None
 */
void dmac_init(void)
{
	uint32_t clkGate = hal_sysctrl_get_clock_gate();
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_DMAC);
	hal_sysctrl_set_clock_gate(clkGate);
	if (!dmacIsInited)
	{
		dmacIsInited = true;
		hal_dmac_enable_global();
		hal_dmac_clear_all_interrupts();
		hal_interrupt_register_isr_handler(DMAC_IRQn, dmac_isr);
		hal_interrupt_enable_irq(DMAC_IRQn);
	}
}

/** 
 * @brief	This function deinitialize the DMA Controller
 * @note	None
 * @return	None
 */
void dmac_deinit(void)
{
	if (dmacIsInited)
	{
		dmacIsInited = false;
		hal_dmac_disable_global();
		hal_dmac_clear_all_interrupts();
		hal_interrupt_disable_irq(DMAC_IRQn);
	}
}

/**
 * @brief	This function gets a current idle channel
 * @return	an idle channel, or -1 indicating no idle channel
 */
int8_t dmac_get_idle_channel(void)
{
	uint8_t chx = hal_dmac_get_channel_st();
	uint8_t i = 0;
	for(i=0; i<DMAC_CHANNEL_NUM; i++)
	{
		if(gDmacChx[i].isOpen == false && (chx & (0x1 << i)) == 0)
		{
			gDmacChx[i].isOpen = true;
			return i;
		}
	}
	return -1;
}

/**
 * @brief	This function register the channel callback function.
 * @note	None
 * @param[in]	chx     The target channel needs to be operated.
 * @param[in]	type     Transfer type
 * @param[in]	callback    The callback function of the specified channel.
 * @return	None
 */
void dmac_register_channel(dmac_channel_num_t chx, dmac_intr_type_t type, dmac_channel_callback_t callback)
{
	ASSERT(chx < DMAC_CHANNEL_NUM);
	ASSERT(type < DMAC_INTERRUPT_NUM);
	ASSERT(callback);

	gDmacChx[chx].cb[type] = callback;
}

/**
 * @brief	This function unregister the channel information and callback function.
 * @note	None
 * @param[in]	chx     The target channel needs to be operated.
 * @return	None
 */
void dmac_unregister_channel(dmac_channel_num_t chx)
{
	ASSERT(chx < DMAC_CHANNEL_NUM);

	uint8_t i = 0;
	for(i=0; i< DMAC_INTERRUPT_NUM; i++)
	{
		gDmacChx[chx].cb[i] = NULL;
	}
}

/**
 * @brief	This function is used to initialize the specified channel configuration of DMA controller
 * @note	None
 * @param[in]	chx     The target channel needs to be operated.
 * @param[in]	fun      The channel's function
 * @return	None
 *
 * @pre		Called after dmac_init
 */
void dmac_open_channel(dmac_channel_num_t chx, dmac_chx_fun_t fun)
{
	ASSERT(chx < DMAC_CHANNEL_NUM);

	hal_dmac_disable_channel(chx);
	switch(fun)
	{
	case CHX_MEM_TO_MEM:
		hal_dmac_config_channel_mem_to_mem(chx, &(gDmacChx[chx].cfg));
		break;
	case CHX_MEM_TO_I2S:
		hal_dmac_config_channel_mem_to_i2s(chx, &(gDmacChx[chx].cfg));
		break;
	case CHX_MEM_TO_PER:
		hal_dmac_config_channel_mem_to_per_soft(chx, &(gDmacChx[chx].cfg));
		break;
	default:
		break;
	}

	gDmacChx[chx].isOpen = true;
	hal_dmac_enable_channel(chx);
}

/**
 * @brief	This function is used to reopen the specified channel configuration of DMA controller
 * @note	None
 * @param[in]	chx     The target channel needs to be operated.
 * @param[in] srcAddr	new Source Address
 * @param[in] dstAddr	new Destination Address
 * @param[in] block		new Block Size
 * @return	None
 *
 * @pre		Called after dmac_init
 */
void dmac_reopen_channel(dmac_channel_num_t chx, uint32_t srcAddr, uint32_t dstAddr, uint16_t block)
{
	hal_dmac_disable_channel(chx);
	if(gDmacChx[chx].isOpen == true)
	{
		hal_dmac_recfg_channel(chx, srcAddr, dstAddr, block);
		hal_dmac_enable_channel(chx);
	}
}

/**
 * @brief	This function is used to close the specified channel configuration of DMA controller
 * @note	None
 * @param[in]	chx     The target channel needs to be operated.
 * @return	None
 */
void dmac_close_channel(dmac_channel_num_t chx)
{
	dmac_unregister_channel(chx);
	hal_dmac_disable_channel(chx);
	gDmacChx[chx].isOpen = false;
}

#endif	//DMA_ENABLE

#ifdef __cplusplus
}
#endif


