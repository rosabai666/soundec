/** 
***********************************************************
 * 
 * @file name	: hal_sysctrl.h
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-09-18
 * @brief		: Header file for system control module
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


#ifndef __HAL_SYSCTRL_H__
#define __HAL_SYSCTRL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

typedef struct {
	__IO uint32_t DivParamSet;		/* Offset: 0x00 */
	__IO uint32_t ClkCfgFlash;		/* Offset: 0x04 */
	__IO uint32_t ClkCfgAHB;		/* Offset: 0x08 */
	__IO uint32_t ClkGTEn;			/* Offset: 0x0c */
	__IO uint32_t JTCKGTEn;			/* Offset: 0x10 */
	__IO uint32_t Protiry;			/* Offset: 0x14 */
	__IO uint32_t ProrityCfg;		/* Offset: 0x18 */
	__IO uint32_t DefaultPorta1;	/* Offset: 0x1c */
	__IO uint32_t DefaultPorta2;	/* Offset: 0x20 */
	__IO uint32_t SysCtrl1;			/* Offset: 0x24 */
	__IO uint32_t IntRemapReg1;		/* Offset: 0x28 */
	__IO uint32_t IntRemapReg2;		/* Offset: 0x2c */
	__IO uint32_t IntRemapReg3;		/* Offset: 0x30 */
	__IO uint32_t IntRemapReg4;		/* Offset: 0x34 */
	__IO uint32_t IntRemapReg5;		/* Offset: 0x38 */
	__IO uint32_t IntRemapReg6;		/* Offset: 0x3c */
	__IO uint32_t IntRemapReg7;		/* Offset: 0x40 */
	__IO uint32_t Counter32k;		/* Offset: 0x44 */
	__IO uint32_t MbistHifi3Config1;/* Offset: 0x48 */
	__IO uint32_t MbistHifi3Config2;/* Offset: 0x4c */
	__IO uint32_t MbistHifi3Status1;/* Offset: 0x50 */
	__IO uint32_t MbistHifi3Status2;/* Offset: 0x54 */
	__IO uint32_t reserved[2];		/* Offset: 0x58 */
	__IO uint32_t FwEncCtrl1;		/* Offset: 0x60 */
	__IO uint32_t FwEncCtrl2;		/* Offset: 0x64 */
	__IO uint32_t SoftwareInt;		/* Offset: 0x68 */
	__IO uint32_t IntForce;			/* Offset: 0x6c */ 
} hal_sysctrl_t;

typedef enum {
	HAL_SYSCTRL_CODEC_SLAVE_FRAC_SRC_DOL = 0,	/* Dolphin */
	HAL_SYSCTRL_CODEC_SLAVE_FRAC_SRC_SNC,		/* Soundec */
	HAL_SYSCTRL_CODEC_SLAVE_FRAC_SRC_NUM
} hal_sysctrl_codec_slave_frac_src_t;

typedef enum {
	HAL_SYSCTRL_CODEC_SLAVE_FRAC_SHARE_DEDICATE = 0,	/* All slave ADCs and DAC use Dedicated fractional divider */
	HAL_SYSCTRL_CODEC_SLAVE_FRAC_ShARE_ONE,				/* All slave ADCs and DAC share fractional divider with ADC3456 */
	HAL_SYSCTRL_CODEC_SLAVE_FRAC_SHARE_NUM
} hal_sysctrl_codec_slave_frac_share_t;

typedef enum {
	HAL_SYSCTRL_CLKGT_BUS = 0,			/* Not used */
	HAL_SYSCTRL_CLKGT_USB,				/* USB AHB clock enable signal */
	HAL_SYSCTRL_CLKGT_PUSB,				/* USB APB clock enable signal */
	HAL_SYSCTRL_CLKGT_SFC,				/* SPI Flash Controller AHB clock enable signal */
	HAL_SYSCTRL_CLKGT_ROM,				/* ROM block and AHB clock enable signal */
	HAL_SYSCTRL_CLKGT_DMAC,				/* DMA block and AHB clock enable signal */
	HAL_SYSCTRL_CLKGT_BQ,				/* BQ block and AHB clock enable signal */
	HAL_SYSCTRL_CLKGT_HBUS2,			/* HBUS clock enable signal of APB bridge 1 */
	HAL_SYSCTRL_CLKGT_CODEC,			/* Codec APB clock enable signal */
	HAL_SYSCTRL_CLKGT_MAESTRO,			/* Maestro APB clock enable signal */
	HAL_SYSCTRL_CLKGT_HBUS3,			/* Not used */
	HAL_SYSCTRL_CLKGT_I2S1,				/* I2S 0 APB clock enable signal */
	HAL_SYSCTRL_CLKGT_I2S2,				/* I2S 1 APB clock enable signal */
	HAL_SYSCTRL_CLKGT_I2S3,				/* I2S 2 APB clock enable signal */
	HAL_SYSCTRL_CLKGT_I2C1,				/* I2C 1 APB clock enable signal */
	HAL_SYSCTRL_CLKGT_I2C2,				/* I2C 2 APB clock enable signal */
	HAL_SYSCTRL_CLKGT_UART,				/* UART block and APB clock enable signal */
	HAL_SYSCTRL_CLKGT_ADC,				/* Extra ADC APB clock enable signal */
	HAL_SYSCTRL_CLKGT_EFUSE,			/* eFuse block and APB clock enable signal */
	HAL_SYSCTRL_CLKGT_GPIO,				/* GPIO block and APB clock enable signal */
	HAL_SYSCTRL_CLKGT_PWM,				/* PWM block and APB clock enable signal */
	HAL_SYSCTRL_CLKGT_GPIOAO,			/* GPIO Always On block and APB clock enable signal */
	HAL_SYSCTRL_CLKGT_RTC,				/* RTC APB clock enable signal */
	HAL_SYSCTRL_CLKGT_SPIF,				/* Flash controller block clock enable signal */
	HAL_SYSCTRL_CLKGT_USBPHY,			/* USB PHY 24M clock enable signal */
	HAL_SYSCTRL_CLKGT_NUM,
	
	HAL_SYSCTRL_CLKGT_CODEC12M = HAL_SYSCTRL_CLKGT_NUM,			/* Not used. Codec 12MHz clock enable signal. */
	HAL_SYSCTRL_CLKGT_ADC12_SBCLK,		/* Not used. ADC12 slave bit clock enable signal. */
	HAL_SYSCTRL_CLKGT_ADC3456_SBCLK,	/* Not used. ADC3456 slave bit clock enable signal. */
	HAL_SYSCTRL_CLKGT_DAC_SBCLK,		/* Not used. DAC slave bit clock enable signal. */
} hal_sysctrl_clk_gate_t;

#define HAL_SYSCTRL_32KCAL_EN_SHIFT			8
#define HAL_SYSCTRL_32KCAL_EN_MASK			(0x01 << HAL_SYSCTRL_32KCAL_EN_SHIFT)
#define HAL_SYSCTRL_SHARE_FRAC_DIV_SHIFT	3
#define HAL_SYSCTRL_SHARE_FRAC_DIV_MASK		(0x07 << HAL_SYSCTRL_SHARE_FRAC_DIV_SHIFT)
#define HAL_SYSCTRL_FRAC_DIV_SRC_SHIFT		1
#define HAL_SYSCTRL_FRAC_DIV_SRC_MASK		(0x01 << HAL_SYSCTRL_FRAC_DIV_SRC_SHIFT)

#define HAL_SYSCTRL_32K_COUNTER_SHIFT		0
#define HAL_SYSCTRL_32K_COUNTER_MASK	(	0x3ff << HAL_SYSCTRL_32K_COUNTER_SHIFT)

void hal_sysctrl_32k_cal_enable(void);

void hal_sysctrl_32k_cal_disable(void);

bool hal_sysctrl_get_32k_cal_enable_status(void);

void hal_sysctrl_get_32k_counter(uint32_t *count);

void hal_sysctrl_set_codec_slave_frac_src(hal_sysctrl_codec_slave_frac_src_t src);

void hal_sysctrl_set_codec_slave_frac_share(hal_sysctrl_codec_slave_frac_share_t src);

/** 
 * @brief This function is used to set the clock gate
 * @param[in] clk_gate The clock gate is set. Each bit is descripted at enum hal_sysctrl_clk_gate_t
 */
void hal_sysctrl_set_clock_gate(uint32_t clk_gate);
uint32_t hal_sysctrl_get_clock_gate(void);
uint32_t hal_sysctrl_get_clock_gate(void);

#ifdef __cplusplus
}
#endif
#endif /* __HAL_SYSCTRL_H__ */
