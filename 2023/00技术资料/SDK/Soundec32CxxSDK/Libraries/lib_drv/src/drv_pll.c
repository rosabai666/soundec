/** 
***********************************************************
 * 
 * @file name	: drv_pll.c
 * @author	: nick
 * @version	: V01.00
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

/* Includes ------------------------------------------------------------------*/
#include "drv_pll.h"

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
//static uint32_t sys_clock_khz = 0;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/** 
 * @brief			PLL initialization entry.
 * @param[in]	pllOut         Clock parameters to be configured @ref PLLout_list_t
 * @return		Get system clock setting status.
 * @code
 * 	pll_init(PLL_OUT_VALUE)
 */
hal_status_t pll_init(PLLout_list_t pllOut)
{
	//hal_status_t pllStatus = HAL_STATUS_OK;

	return hal_set_sys_clk(pllOut, false);
}


