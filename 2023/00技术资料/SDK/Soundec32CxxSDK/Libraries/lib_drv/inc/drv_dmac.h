/** 
***********************************************************
 * 
 * @file name	: drv_dmac.h
 * @author		: RandyFan
 * @version		: V02.02.demo
 * @date		: 2022-01-04
 * @brief		: DMAC Driver Header File
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_DMAC_H__
#define __DRV_DMAC_H__

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "hal_dmac.h"
#include "boardConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#if(DMA_ENABLE)

/* Private typedef -----------------------------------------------------------*/
typedef void (*dmac_channel_callback_t)(uint8_t chx);

/**
 * @enum dmac_chx_fun_t
 * @brief Define DMA function type
 */
typedef enum{
	CHX_MEM_TO_MEM = 0,
	CHX_MEM_TO_I2S,
	CHX_MEM_TO_PER,
}dmac_chx_fun_t;

/**
 * @struct dmac_channel_t
 * @brief DMA channel configuration
 */
typedef struct{
	bool isOpen;
	dmac_channel_cfg_t cfg;
	dmac_channel_callback_t cb[DMAC_INTERRUPT_NUM];
}dmac_channel_t;
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern dmac_channel_t gDmacChx[DMAC_CHANNEL_NUM];
/* Private function prototypes -----------------------------------------------*/
/** 
 * @brief	This function is used to initialize the DMA Controller. It will enable DMA, 
 * @note	This function should be called before calling dmac_open_channel
 * @return	None
 */
void dmac_init(void);

/** 
 * @brief	This function deinitialize the DMA Controller
 * @note	None
 * @return	None
 */
void dmac_deinit(void);

/**
 * @brief	This function gets a current idle channel
 * @return	an idle channel, or -1 indicating no idle channel
 */
int8_t dmac_get_idle_channel(void);

/**
 * @brief	This function is used to initialize the specified channel configuration of DMA controller
 * @note	None
 * @param[in]	channel     The target channel needs to be operated.
 * @param[in]	fun      The channel's function
 * @return	None
 *
 * @pre		Called after dmac_init
 */
void dmac_open_channel(dmac_channel_num_t _chx, dmac_chx_fun_t fun);

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
void dmac_reopen_channel(dmac_channel_num_t chx, uint32_t srcAddr, uint32_t dstAddr, uint16_t block);

/**
 * @brief	This function is used to close the specified channel configuration of DMA controller
 * @note	None
 * @param[in]	chx     The target channel needs to be operated.
 * @return	None
 */
void dmac_close_channel(dmac_channel_num_t _chx);

#endif	//DMA_ENABLE

#ifdef __cplusplus
}
#endif

#endif  /* __DRV_DMAC_H__ */


