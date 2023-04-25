/** 
***********************************************************
 * 
 * @file name	: snc8600.h
 * @author		: yen.yu
 * @version		: Vxx.xx
 * @date		: 2022-03-23
 * @brief		: hal define
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SNC8600_H__
#define __SNC8600_H__

/* Includes ------------------------------------------------------------------*/
#include "types.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup Regs Register Structures
 * @brief Packing On-Chip Peripherals' Registers
 */

#define CHIP_SNC8600						0
#define CHIP_SNC8600A						1


//#define HAL_CACHE_MODULE_ENABLED
//#define HAL_I2S_DMA_ENABLE

#define HAL_DMAC_I2S0_RX_HW_HS	(2)
#define HAL_DMAC_I2S0_TX_HW_HS	(3)
#define HAL_DMAC_I2S1_RX_HW_HS	(4)
#define HAL_DMAC_I2S1_TX_HW_HS	(5)
#define HAL_DMAC_I2S2_RX_HW_HS	(6)
#define HAL_DMAC_I2S2_TX_HW_HS	(7)

#define SFC_BASE  				(0x75001000)
#define BQ_BASE					(0x75002000)
#define CODEC_DATA_BASE			(0x75003000)
#define DMAC_BASE				(0x75006000)

#define CODEC_BASE				(0x76001000)
#define CODEC2_BASE				(0x76002000)

#define HAL_I2S0_RX_FIFO_ADDR	(0x770001C0)
#define HAL_I2S0_TX_FIFO_ADDR	(0x770001C8)
#define HAL_I2S1_RX_FIFO_ADDR	(0x770011C0)
#define HAL_I2S1_TX_FIFO_ADDR	(0x770011C8)
#define HAL_I2S2_RX_FIFO_ADDR	(0x770021C0)
#define HAL_I2S2_TX_FIFO_ADDR	(0x770021C8)

#define	I2C1_BASE				(0x77003000)
#define	I2C2_BASE				(0x77004000)
#define	RTC_BASE				(0x77005000)
#define GPIO_BASE				(0x77006000)
#define EFUSE_BASE				(0x77008000)
#define SARADC_BASE				(0x77009000)
#define UART_BASE				(0x7700a000)
#define SYSCTRL_BASE			(0x7700b000)
#define PWM_BASE				(0x7700c000)
#define OSC32K768_BASE 			(0x7700f000)

#define CODEC_CLOCK_BASE		(CODEC_DATA_BASE + 0x40)
#define CODEC_FIFO_BASE			(CODEC_DATA_BASE + 0x80)


typedef enum IRQn {
	/* Level 4 interrupts */
    USB_IRQn			= 0,	//USB
    USB_DMA_IRQn		= 1,	//USB DMA
    ADC12_IRQn			= 2,	//Codec ADC12
    ADC34_IRQn			= 3,	//Codec ADC34
    ADC56_IRQn			= 4,	//Codec ADC56
    ADC78_IRQn			= 5,	//Codec ADC78
    ADC9A_IRQn			= 6,	//Codec ADC9A
    DAC_IRQn			= 7,	//Codec DAC
    EXT_SW0_IRQn		= 8,	//External Software interrupt 0
    I2S1_IRQn			= 9,	//I2S0
    I2S2_IRQn			= 10,	//I2S1
    I2S3_IRQn			= 11,	//I2S2
    DMAC_IRQn			= 12,	//DMA controller
    PMU_IRQn			= 13,	//PMU
    CODEC_IRQn			= 14,	//Codec AIAS and HP short-circuit interrupt
    CODEC_WT_IRQn		= 15,	//Codec Whisper Trigger
	
	/* Level 3 interrupts */
    I2C1_IRQn			= 16,	//I2C0
    I2C2_IRQn			= 17,	//I2C1
    UART_IRQn			= 18,	//UART
    TIMER0_IRQn			= 19,	//System timer0, Cannot be remapped!!!!!
    GPIO_678_IRQn		= 20,	//GPIO 6/7/8
	
	/* Level 2 interrupts */
    RTC_IRQn			= 21,	//RTC
    WATCH_DOG_IRQn		= 22,	//Watch-dog
    GPIO_9ABCDEF_IRQn	= 23,	//GPIO 9/10/11/12/13/14/15
	
	/* Level 1 interrupts */
    USB_SLEEP_IRQn		= 24,	//USB Sleep
    SW_IRQn				= 25,	//System Software, Cannot be remapped!!!!!
    TIMER1_IRQn			= 26,	//System timer1, Cannot be remapped!!!!!
    TIMER2_IRQn			= 27,	//System timer2, Cannot be remapped!!!!!
    SARADC_IRQn			= 28,	//Aux-ADC
    PWM_IRQn			= 29,	//PWM
    EXT_SW1_IRQn		= 30,	//External Software interrupt 1
    PROFILING_IRQn		= 31,	//System Profiling, Cannot be remapped!!!!!
    IRQ_NUMBER      	= 32
} IRQn_Type;

typedef IRQn_Type hal_interrupt_irq_t;

#ifdef __cplusplus
}
#endif

#endif  /* __SNC8600_H__ */

