/** 
***********************************************************
 * 
 * @file name	: debug_tools.c
 * @author		: yen.yu
 * @version		: Vxx.xx
 * @date		: 2022-09-01
 * @brief		: Functions for debug tools
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
 * and Soundec Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 * 
***********************************************************/

/* Includes ------------------------------------------------------------------*/
#include <common.h>
#include"debug_tools.h"
#if (MEMORY_SPACE_MONITOR || CPU_USAGE_DEBUG)
/* Private typedef -----------------------------------------------------------*/
/**
	pPLL   cpu count
	100MHz	35396
	120MHz	42848
	160MHz	57396
	190MHz	68044
**/
#define CPU_COUNT_10MS			(358*SYS_CLK_KHZ/1000)//PLL:190MHZ

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint32_t cpu_total_count;
uint32_t cpu_max_count;
uint32_t temp_count;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void memory_spce_monitor(void)
{
#if 1
	// Assuming no memory fragments exist
	uint8_t stackTest;
	uint32_t* heapTest = malloc(1);
	uint32_t dramFreeSize = (uint32_t)&stackTest - (uint32_t)heapTest;

	if(heapTest != NULL)
	{
		uart_printf("=========Memory free size:%.2fKB, stackTest:0x%x, heapTest:0x%x============\n", (float)dramFreeSize/1024, &stackTest, heapTest);
		free(heapTest);
	}
#else
	uint32_t *testBuf[200];
	int count = 0;
	while(count < 200)
	{
		testBuf[count] = malloc(1024);
		if(testBuf[count] == NULL)
			break;
		count++;
	}
	uart_printf("\n=========Memory free size:%dKB============\n", count);
	delay(10);
	while(count >= 0)
	{
		free(testBuf[count]);
		testBuf[count] = 0;
		count--;
	}
#endif
}

//Suggest called from 10ms polling task
void cpu_usage_polling(void)
{
	if(cpu_max_count < cpu_total_count)
		cpu_max_count = cpu_total_count;
	cpu_total_count = 0;
}

void cpu_usage_print(void)
{
	uart_printf("cpu usage:%.2f%\n", (float)(CPU_COUNT_10MS - cpu_max_count)*100/CPU_COUNT_10MS);
//	uart_printf("cpu_max_count:%d\n", cpu_max_count);
	cpu_max_count = 0;
	cpu_total_count = 0;
	
#if MEMORY_SPACE_MONITOR
	static bool printFlag = true;
	if(printFlag)
	{
		printFlag = false;
		memory_spce_monitor();
	}
#endif
}

uint8_t get_cpu_usage(void)
{
	return (uint8_t)((CPU_COUNT_10MS - cpu_max_count)*100/CPU_COUNT_10MS);
}

#endif//(MEMORY_SPACE_MONITOR || CPU_USAGE_DEBUG)

