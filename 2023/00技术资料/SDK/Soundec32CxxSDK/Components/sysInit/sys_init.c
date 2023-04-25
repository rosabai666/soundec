/** 
***********************************************************
 * 
 * @file name	: sys_init.c
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-02-20
 * @brief		: System Initialize 
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
 * and Soundec Co., Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 * 
***********************************************************/

#include <snc8600.h>
//#include <drv_cache.h>
#include "common.h"
#include "sys_init.h"
#include "hal_sysctrl.h"
#include "hal_interrupt.h"
#include "drv_pll.h"
#include "task.h"
#include "drv_pmu.h"
#include "app_adc_key.h"


#ifdef __cplusplus
extern "C" {
#endif

//static uint32_t peripheral_clock_khz[PERI_CLOCK_MAX];
uint32_t gSysLastCycleCount = 0;


/** 
 * @brief	A method calculate the time difference,first clear cycle count where the time count start
 * @code
 * 	calculate_time_start()
 * @endcode
 */
void calculate_time_clear(void)
{
	gSysLastCycleCount = xthal_get_ccount();
}

/** 
 * @brief	A method calculate the time difference,should be called where the time count end
 * @note	{description}
 * @param[in]	timeUnit 
 * 	@arg @ref 0 unit is machine cycles, uart_printf("%fus", (float)getTime/190);
 * 	@arg @ref 1 unit is us
 * @par Example:
 * @code
 * 	calculate_time_get(0);
 * @endcode
 */
uint32_t calculate_time_get(uint8_t timeUnit)
{
	uint32_t newCycleCount = xthal_get_ccount();
	uint32_t cycleDiff = (newCycleCount > gSysLastCycleCount)?(newCycleCount - gSysLastCycleCount):(0xffffffff - gSysLastCycleCount + newCycleCount);

	if(timeUnit)
	{
		return cycleDiff/(SYS_CLK_KHZ/1000);
	}
	else
	{
		return cycleDiff;
	}
}

void app_pm_init(void)
{
	uint32_t ClkGate = 0;
	
#ifndef HANDLE_USBD_SUSPEND_MODE2
	//CLRBIT(ClkGate, HAL_SYSCTRL_CLKGT_SFC);
#endif
	SETBIT(ClkGate, HAL_SYSCTRL_CLKGT_BUS);
	SETBIT(ClkGate, HAL_SYSCTRL_CLKGT_HBUS2);
	SETBIT(ClkGate, HAL_SYSCTRL_CLKGT_HBUS3);
	SETBIT(ClkGate, HAL_SYSCTRL_CLKGT_GPIO);
#ifndef HANDLE_USBD_SUSPEND_MODE2
	//CLRBIT(ClkGate, HAL_SYSCTRL_CLKGT_SPIF);
#endif
	hal_sysctrl_set_clock_gate(ClkGate);

#ifndef JTAG_DEBUG_ENABLE
    set_jtag_io_general_function();
#endif

	for (size_t i = 0; i < 16; i++)
	{
		hal_gpio_pull_up((gpio_pin_t)i);
		hal_gpio_set_direction((gpio_pin_t)i, GPIO_DIRECTION_INPUT);
	}
}


/**
* @brief       This function is used to do system initialization, eg: system clock, hardware and logging port.
*/
void system_init(void)
{
#ifdef HAL_CACHE_MODULE_ENABLED
	cache_init();
	cache_deinit();
#else
	xthal_set_cacheattr(0x22222222);
	XT_WSR_MEMCTL(0);
#endif
#if (CONFIG_VDD_IO_18)
	pmu_set_regulator(DMU_iLR2, 0);
#endif

	app_pm_init();

	hal_set_system_crystal(SYSTEM_CRYSTAL);

	hal_interrupt_init();

	pll_init(PLL_OUT_VALUE);
	
	hal_init_peripheral_clk_khz();
	
	task_init();
}

#ifdef __cplusplus
}
#endif

