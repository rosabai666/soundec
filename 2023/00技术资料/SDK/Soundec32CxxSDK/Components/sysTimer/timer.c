/** 
***********************************************************
 * 
 * @file name	: timer.c
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2022-01-20
 * @brief		: This is the implement of timer modulles
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
#include <xtensa/xtruntime.h>
#include <xtensa/config/core.h>
#include <stdint.h>
#include "types.h"
#include "sys_init.h"
#include "timer.h"
#include "drv_uart.h"
/* Private function prototypes -----------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define TIMER_MAX	3

/* Private typedef -----------------------------------------------------------*/
typedef struct{
	uint8_t timern;	// 0: TIMER0; 1:TIMER1;2:TIMER2
	IRQn_Type irqn;
	bool repeat;
	uint32_t period;
	uint32_t tick;
	timer_isr_cb isr_cb;
}timer_param_t;

typedef struct
{
	uint8_t index;	// 
	timer_param_t param[TIMER_MAX];
}timer_t;

/* Private variables ---------------------------------------------------------*/
static timer_t timer={
	0,
	{
		{TIMER0, TIMER0_IRQn, 0, 0, 0, NULL},
		{TIMER1, TIMER1_IRQn, 0, 0, 0, NULL},
		{TIMER2, TIMER2_IRQn, 0, 0, 0, NULL},
	},
};

/* Private functions ---------------------------------------------------------*/
void timer_isr_handler(void *arg)
{
	uint8_t idx = *(uint8_t *)arg;
	uint32_t old_compare, diff;

	xtos_interrupt_clear(timer.param[idx].irqn);
	do{
		old_compare = xthal_get_ccompare(timer.param[idx].timern);
		if (timer.param[idx].repeat == true)
			xthal_set_ccompare(timer.param[idx].timern, old_compare + timer.param[idx].period);
		diff = xthal_get_ccount() - old_compare;
	} while(diff > timer.param[idx].period);

	timer.param[idx].tick++;
	if(timer.param[idx].isr_cb)
		timer.param[idx].isr_cb();
}

void timer_init(void)
{
	int i = 0;
	for (i = 0;i <ARRAY_SIZE(timer.param); i++)
	{
		if (timer.param[i].period && timer.param[i].isr_cb)
		{
			timer.index = i;
			// Disable timer interrupt 1st, in order to timern is re-init.
			xtos_interrupt_disable(timer.param[i].irqn);
			xtos_interrupt_clear(timer.param[i].irqn);
			// Set and enable timern interrupt.
			xthal_set_ccompare(timer.param[i].timern, xthal_get_ccount() + timer.param[i].period);
			xtos_set_interrupt_handler(timer.param[i].irqn, timer_isr_handler, &timer.index, NULL);
			xtos_interrupt_enable(timer.param[i].irqn);
		}
	}
}

void timern_init(uint8_t timer_n, uint32_t period, bool repeat, timer_isr_cb isr_cb)
{
//[Todo]:	Timer2 is used for system task, need tobe protected.
	ASSERT(timer_n<TIMER_MAX);
	timer.param[timer_n].period = period;
	timer.param[timer_n].repeat = repeat;
	timer.param[timer_n].isr_cb = isr_cb;
	timer_init();
}



