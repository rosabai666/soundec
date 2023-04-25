/** 
***********************************************************
 * 
 * @file name	: hal_pll.c
 * @author	: Rosa.Bai
 * @version	: Vxx.xx
 * @date		: 2019-12-04
 * @brief		: Driver of PLL
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_PLL_H__
#define __HAL_PLL_H__

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "hal.h"
#ifdef __cplusplus
extern "C" {
#endif

#define SYS_CLK_KHZ				hal_get_sys_clk_khz()
#define SYS_CLK					(SYS_CLK_KHZ*1000)

// peri @ref peri_clock_source_t
#define PERI_CLK_KHZ(peri)		hal_get_peripheral_clk_khz(peri)

// peri @ref peri_clock_source_t
#define PERI_CLK_HZ(peri)		(hal_get_peripheral_clk_khz(peri)*1000)

/* Private typedef -----------------------------------------------------------*/

#define SYSTEM_CRYSTAL_24MHZ				0
#define SYSTEM_CRYSTAL_26MHZ				1

typedef enum _Pllout_list
{
	PLLOUT_20MHZ = 0U,
	PLLOUT_30MHZ = 1U,
	PLLOUT_40MHZ = 2U,
	PLLOUT_50MHZ = 3U,
	PLLOUT_60MHZ = 4U,
	PLLOUT_70MHZ = 5U,
	PLLOUT_80MHZ = 6U,
	PLLOUT_90MHZ = 7U,
	PLLOUT_100MHZ = 8U,
	PLLOUT_110MHZ = 9U,
	PLLOUT_120MHZ = 10U,
	PLLOUT_130MHZ = 11U,
	PLLOUT_140MHZ = 12U,
	PLLOUT_150MHZ = 13U,
	PLLOUT_160MHZ = 14U,
	PLLOUT_170MHZ = 15U,
	PLLOUT_180MHZ = 16U,
	PLLOUT_190MHZ = 17U,
	PLLOUT_200MHZ = 18U,
	PLLOUT_210MHZ = 19U,
	PLLOUT_220MHZ = 20U,
	PLLOUT_OFF = 21,
	PLLOUT_24MHZ_BYPASS = PLLOUT_OFF,	// here is sysclock 24MHz
	PLLOUT_RANGE = PLLOUT_OFF,
} PLLout_list_t;

typedef enum{
	PERI_CLOCK_AHB,				/* no divided */
	PERI_CLOCK_APB2,			/* 2 divided, PWM, Uart, I2C, */
	PERI_CLOCK_APB1,			/* 4 devided */
	PERI_CLOCK_FLASH,			/* 2 devided */
	PERI_CLOCK_CODEC,			/* 12MHz */
	PERI_CLOCK_USB, 			/* 24MHz */
	PERI_CLOCK_MAX
}peri_clock_source_t;
// Register structure of PLL
// Structure for pll control and data registers
// Direct access registers
typedef struct
{
	uint32_t REG_PLL_SET;				// 0x00, Default:0x00000000
	uint32_t REG_PLL_PARAMCFG;			// 0x04, Default:0x000D0230
	uint32_t REG_PLL_SEL_CFG;			// 0x08, Default:0x00000000
	uint32_t REG_PLL_LOCK;				// 0x0C, Default:0x00000000
} hal_pll_reg_t;


/* Private macro -------------------------------------------------------------*/
// The base address for the PLL registers
#define PLL_BASE_ADDRESS    		0x7700D000
#define PLL_REG   					((volatile hal_pll_reg_t *)PLL_BASE_ADDRESS)

/* Private variables ---------------------------------------------------------*/

/**
 * @brief	system clock select parameter.
*/
#define CLK_24MHZ_MODE 			0
#define CLK_PLL_MODE			1

//PLL parameter configuration register
#define PLL_DIV_SHIF			20
#define PLL_DIV_MASK			(0x01 << PLL_DIV_SHIF)
#define PLL_BYPASS_SHIF			19
#define PLL_BYPASS_MASK			(0x01 << PLL_BYPASS_SHIF)
#define PLL_PD_SHIF				18
#define PLL_PD_MASK				(0x01 << PLL_PD_SHIF)
#define PLL_DIV_RANGE_SHIF		15
#define PLL_DIV_RANGE_MASK		(0x07 << PLL_DIV_RANGE_SHIF)
#define PLL_DIVF_SHIF			8
#define PLL_DIVF_MASK			(0x7F << PLL_DIVF_SHIF)
#define PLL_DIVQ_SHIF			5
#define PLL_DIVQ_MASK			(0x07 << PLL_DIVQ_SHIF)
#define PLL_DIVR_SHIF			0
#define PLL_DIVR_MASK			(0x1F << PLL_DIVR_SHIF)
/* Private function prototypes -----------------------------------------------*/
/** 
 * @brief			Shut down PLL
 * @note			Before shut down PLL, make sure that system clock has been set to CLK_24MHZ_MODE.
 * @param[in]	None.
 * @return		@ref hal_status_t
 */
extern hal_status_t hal_pll_off(void);

/** 
 * @brief			This routine used to switch the PLL output the sepcified frequence.
 * @param[in]	pll		PLL frequency output
 * 	@arg @ref PLLout_list_t, list frequency that supported by PLL
 * @param[in]	Div         Divide the pll output frequency by two; 0 ,disale; 1 , enable. 
 * @return		None
 */
extern hal_status_t hal_pllout_switch(PLLout_list_t pll , bool Div);

/** 
 * @brief	 	config pll main parameters.
 * @note	 	none.
 * @param[in]	pll         User configures system clock parameters.
 * @param[in]	Div        select divide the pll output frequency by two.
 * @return	pll parameters config status .
 * 	@HAL_STATUS_OK           config ok.
 * 	@HAL_STATUS_FAIL         config failure.
 * 	@HAL_STATUS_TIMEOUT  config timeout.
 * @code
 * 	hal_pll_set_new()
 *    hal_pllout_switch()
 */
extern hal_status_t hal_pll_set_new(PLLout_list_t pll, bool Div);

extern const uint32_t sysClkMap[PLLOUT_RANGE+1];

uint32_t get_sysclk_delayus_ticks(uint32_t delayus);
uint32_t get_sysclk_delayms_ticks(uint32_t delayms);
hal_status_t hal_set_sys_clk(PLLout_list_t pll, bool Div);
uint32_t hal_get_sys_clk_khz(void);
void hal_init_peripheral_clk_khz(void);
uint32_t hal_get_peripheral_clk_khz(peri_clock_source_t peri);
void hal_set_system_crystal(int val);
int hal_get_system_crystal();

/* Private functions ---------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif  /* __HAL_PLL_H__ */
