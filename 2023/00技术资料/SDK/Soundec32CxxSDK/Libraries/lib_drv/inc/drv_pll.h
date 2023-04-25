/** 
***********************************************************
 * 
 * @file name	: drv_pll.h
 * @author	: nick
 * @version	: Vxx.xx
 * @date		: 2020-02-24
 * @brief		: PLL Driver
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_PLL_H__
#define __DRV_PLL_H__

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "hal_pll.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/** 
 * @brief			PLL initialization entry.
 * @param[in]	pllOut         Clock parameters to be configured @ref PLLout_list_t
 * @return		Get system clock setting status.
 * @code
 * 	pll_init(PLL_OUT_VALUE)
 */
hal_status_t pll_init(PLLout_list_t pllOut);

//extern uint32_t get_sys_clk_khz(void);
//extern uint32_t get_peripheral_clk_khz(peri_clock_source_t peri);

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  /* __DRV_PLL_H__ */
