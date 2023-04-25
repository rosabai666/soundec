/** 
***********************************************************
 * 
 * @file name	: task.c
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2022-01-20
 * @brief		: This file used to implement a simple task schedule controller
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
#include <stdint.h>
#include "types.h"
#include "task.h"
#include "timer.h"
#include "sys_init.h"

/* Private macro -------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
//Default Tasks
int task_1ms(void);
int task_10ms(void);
int task_100ms(void);
int task_1s(void);


/* Private typedef -----------------------------------------------------------*/
typedef struct
{
    int run;
    int cnt;
    int itvl_ms;
    task_hook_t task; 
}task_t;

typedef struct 
{
	uint8_t active;
	uint32_t tick;
	task_t task[TASK_NUM];
}tasks_t;

/* Private variables ---------------------------------------------------------*/
static tasks_t tasks;

/* Private functions ---------------------------------------------------------*/
/** 
 * @brief			Provide a time tick with 1ms interval.
 * @note			This tick counter increases to 0xFFFFFED8 and then back to 0 automatically.
 *				Effectived after calling @ref sysPolling_init();
 * @param[in]	None.
 * @return		Current tick counters
 * 
 * @pre			Call @ref sysPolling_init() before use.
 */
U32 get_sys_tick(void)
{
	return tasks.tick;
}

void task_timer_handler(void)
{
    int i ;

	tasks.tick ++;
    for (i=0; i<TASK_NUM; i++)
    {
    	tasks.task[i].cnt--;
		if (!tasks.task[i].cnt)
		{
			tasks.task[i].cnt = tasks.task[i].itvl_ms;
			tasks.task[i].run = 1;
		}
	}
}

void task_register(uint8_t index, int itvl_ms, task_hook_t task)
{
	ASSERT(index<TASK_NUM);
	tasks.task[index].run = 0;
	tasks.task[index].cnt = itvl_ms;
	tasks.task[index].itvl_ms = itvl_ms;
	tasks.task[index].task = task;
}

void task_process(void)
{
	if (tasks.task[tasks.active].run)
	{
		if (tasks.task[tasks.active].task)
		{
			tasks.task[tasks.active].task();
			tasks.task[tasks.active].run = 0;
		}
	}
	if(++tasks.active >= TASK_NUM)
		tasks.active = 0;
} 

/** 
 * @brief		Timer #2 used for system time base of polling. Configured as 1ms interval.
 *				Other longer interval is generated based on this time, such as 10ms, 50ms, 100ms and 1s
 * @note		Don't add any other process in the routine.
 * @param[in]	None.
 * @return		None.
 */
void task_init(void)
{
	tasks.active = 0;
	timern_init(TIMER2, SYS_CLK_KHZ, true, task_timer_handler);
	task_register(0, 1, task_1ms);
	task_register(1, 10, task_10ms);
	task_register(2, 100, task_100ms);
	task_register(3, 1000, task_1s);

}

