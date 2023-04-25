/** 
***********************************************************
 * 
 * @file name	: drv_pmu.h
 * @author	: Rosa.Bai
 * @version	: V01.00
 * @date		: 2020-02-24
 * @brief		: Power manage unit
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
#include "hal_maestro.h"
#include "hal_pll.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum{
	PMU_WAKEUP_SRC_AO = 0,
	PMU_WAKEUP_SRC_USB,
	PMU_WAKEUP_SRC_NULL,
}pmu_wakeup_src_t;

typedef struct
{
	U32 DBCLK_DIV;	// 0x000	0x00000000	ȥë�̲���ʱ�ӷ�Ƶ�����üĴ���	
}gpio_ao_t;

typedef struct
{
	U32 PORT_DIR;		// 	0x77007110	0x00000001	�˿���������������üĴ���	
	U32 PORT_SEL;		// 	0x77007114	0x00000001	�˿�ͨ����;ѡ�����üĴ���,����Ϊ0��ʾ��Ӧ����Ϊ���	
	U32 PORT_DATA;		// 	0x77007118	0x00000000	�˿�ͨ����;�������üĴ���,	����Ϊ0��ʾ��������ר����;
	U32 PORT_INTRSEL;	// 	0x7700711C	0x00000000	ͨ����;����ʱ�������üĴ���,����Ϊ1ʱ����Ӧ���ж�Դ�ܹ�����GPIO_AO�ж�
	U32 PORT_INTRLEL;	// 	0x77007120	0x00000000	�˿��жϴ����������üĴ���1 , 0: ѡ���ش����ж�
	U32 PORT_INTRPOL;	// 	0x77007124	0x00000000	�˿��жϴ����������üĴ���2 , 0: High
	U32 PORT_INTRCLR;	// 	0x77007128	0x00000000	�˿��ж�������üĴ���,����Ϊ1��ʾ������ж�λ�������ж�
	U32 PORT_INTRREG;	// 	0x7700712C	0x00000000	�˿��ж�״̬�Ĵ���,ֻ�����ж�״̬�Ĵ���
}gpio_ao_port_t;

/* Private macro -------------------------------------------------------------*/
#define GPIO_AO_BASE					0x77007000
#define GPIO_AO_PORT_BASE				0x77007110

#define GPIO_AO_REG   					((volatile gpio_ao_t *)GPIO_AO_BASE)
#define GPIO_AO_PORT_REG   				((volatile gpio_ao_port_t *)GPIO_AO_PORT_BASE)
/*
  * @brief a macro that reads a DLC register
  * @param reg the register that is read
*/
#define pmu_ao_read(reg)     			(GPIO_AO_REG->reg)
#define pmu_aoport_read(reg)     		(GPIO_AO_PORT_REG->reg)
/*
 * @brief a macro that writes a DLC register
 * @param reg the register that is read
 * @param value the value that is written
*/
#define pmu_ao_write(reg, value) 		(GPIO_AO_REG->reg = (value))
#define pmu_aoport_write(reg, value)	(GPIO_AO_PORT_REG->reg = (value))


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

extern char *DMU_Unit[DMU_MAX];
//extern dmu_offset_t pmuAdjMode;
extern void pmu_init(void);
extern U8 pmu_get_pll(void);
extern void pmu_set_pll(U8 val);
extern void pmu_tune_pll(U8 dir);
extern void pmu_tune_regulator(dmu_offset_t dmu_index, U8 dir);
extern void pmu_set_regulator(dmu_offset_t dmu_index, U8 val);
extern maestro_status_t pmu_set_rar(U8 val);
extern int pmu_test_task(void);
extern void pmu_set_maestro_mode(maestro_mode_t icuMode);
extern maestro_status_t pmu_set_icu_mode(maestro_mode_t mode);
extern U8 get_dmuX_current_value(dmu_offset_t dmu_index);
extern U8 get_dmuX_max_value(dmu_offset_t dmu_index);
extern void set_dmuX_current_value(dmu_offset_t dmu_index, U8 val);
extern void set_dmuX_max_value(dmu_offset_t dmu_index, U8 val);
void pmu_enable_all_interrupts(void);
void pmu_set_wiu_ifr(wiu_offset_t wiu_index, WIU_IFR_t valIFR);
void pmu_enter_es1_lowpower_mode(void);
void pmu_enter_es2_lowpower_mode(void);
void pmu_enter_es2_lowpower_mode_usb(void);
maestro_status_t pmu_set_regulator_hz(void);
extern pmu_wakeup_src_t pmu_get_wakeup_source(void);
