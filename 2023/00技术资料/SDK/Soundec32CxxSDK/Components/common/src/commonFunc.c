/**
***********************************************************
 *
 * @file name   : commFunc.c
 * @author      : Rosa.Bai
 * @version     : V0.1
 * @date        : 2020-02-16
 * @brief       : This file make a collection for the routines which used in
 *		  the whole project.
 *
***********************************************************
 * @attention
 *
 * Copyright (c) 2019 Soundec Co., Ltd.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user
 * who uses the software, his/her employer (which must be your employer
 * and Sourced Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/
/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include <xtensa/config/core.h>
#include <xtensa/xtruntime.h>

#if 0
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define isL		'L'
#define isB		'B'


/**
 * @fn		getEndian
 * @brief	[description]
 * @param	[description]
 * @arg         sysEndian.dWords	[A 32 bit data]
 * @return	['L' is little endian; 'B' is big endian]
 */
#define getEndian 							((U8)sysEndian.dWords)

/* Private variables ---------------------------------------------------------*/
static U32_t sysEndian = {isL, 0, 0, isB};
#endif
/**
 * @brief	Delay fuction, Unit: Millisecond(ms)
 * @param
 *		@arg ms 	The delay time
 * @return	None
 */
void delay(uint32_t ms)
{
   // int i = 0;
    uint32_t delay;

    uint32_t cycle;

    cycle = xthal_get_ccount();

    delay = SYS_CLK_KHZ*ms;
    while(xthal_get_ccount() - cycle <= delay);
}

/**
 * @brief	Delay fuction, Unit: Microsecond(us)
 * @param
 *		@arg ms 	The delay time
 * @return	None
 */
void delay_us(uint32_t us)
{
   // int i = 0;
    uint32_t delay;

    uint32_t cycle;

    cycle = xthal_get_ccount();

    delay = (SYS_CLK_KHZ*us)/1000;
    while(xthal_get_ccount() - cycle <= delay);
}


