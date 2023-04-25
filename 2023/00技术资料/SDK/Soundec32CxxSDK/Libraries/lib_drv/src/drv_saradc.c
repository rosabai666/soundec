/** 
***********************************************************
 * 
 * @file name	: drv_saradc.c
 * @author		: RandyFan
 * @version		: V02.01
 * @date		: 2022-02-18
 * @brief		: SAR ADC Driver
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
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "hal_saradc.h"
#include "drv_saradc.h"
#include "snc8600.h"
#include "common.h"
#include "SaveRestoreVectorRegisters.h"
#include "hal_sysctrl.h"
#if(AUX_ADC_ENABLE)
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static saradc_config_t adcCfg = {
	.resolution = SARADC_RESOLUTION_12_BIT,
#if(AUX_ADC_USE_CH0)
	.channel = SARADC_SLOW_CHANNEL,	/* adc0-->SLOW_CHANNEL , adc1-->FAST_CHANNEL  */
#else
	.channel = SARADC_FAST_CHANNEL,
#endif
	.vinMode = SARADC_SINGLE_ENDED,
	.sampleRate = FS_20KHZ,
#if(AUX_ADC_INTR_ENABLE)
	.cb = NULL
#endif
};

/* Private function prototypes -----------------------------------------------*/
static void aux_adc_config(void);
/* Private functions ---------------------------------------------------------*/
#if(AUX_ADC_INTR_ENABLE)
/**
 * @brief This function is SAR ADC's interrupt service routine
 */
void aux_adc_isr(void* arg)
{
	SaveVectors(SARADC_IRQn);

    uint8_t adc_ch = hal_saradc_get_channel_mode();//adc_config.Channel_sel;

    hal_saradc_clr_irq();

    uint16_t val = hal_saradc_get_val();

    if(adcCfg.cb)
    {
    	adcCfg.cb(val);
    }

//	static uint32_t cnt = 0;
//	cnt++;
//	if(cnt == 2000)
//	{
//		cnt = 0;
//		uart_printf("ch%d: %d\n",adc_ch, val);
//	}

    RestoreVectors(SARADC_IRQn);
}
#endif	//AUX_ADC_INTR_ENABLE


/**
 * @brief This function inits and configs SAR ADC
 */
void aux_adc_init(void)
{
	uint32_t clkGate = hal_sysctrl_get_clock_gate();
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_ADC);
	hal_sysctrl_set_clock_gate(clkGate);

    //int32_t count = 0 ;

#if(AUX_ADC_INTR_ENABLE)
    hal_interrupt_disable_irq(SARADC_IRQn);
    hal_interrupt_register_isr_handler(SARADC_IRQn, aux_adc_isr);
#endif

    /*adc init*/
	hal_saradc_disable();
	aux_adc_config();
    hal_saradc_enable();

    ///Must Start up in the order of Power_down->Standby->Power_up
    hal_saradc_set_workmode(SARADC_POWER_DOWN);
	delay(2);
    while(hal_saradc_get_ldo_ready());	///<Check Ldo before enter Standby
    hal_saradc_set_workmode(SARADC_STANDBY);

	delay(1);
    while(hal_saradc_get_calibration_status());
    uint8_t cal = hal_saradc_get_calibration();
    hal_saradc_set_calibration(cal);	///<Do Calibration before enter Power_on
    hal_saradc_set_workmode(SARADC_POWER_ON);

#if(AUX_ADC_INTR_ENABLE)
    hal_interrupt_enable_irq(SARADC_IRQn);
    hal_saradc_enable_intr();
#endif

#ifdef HANDLE_AO_ADC_KEY_WAKEUP_FROM_USBD_SUSPEND
//TBD
#endif
}

/**
 * @brief This function configs SAR ADC's resolution, channel, Vinput mode and sample rate
 */
void aux_adc_config(void)
{
	hal_saradc_set_res(adcCfg.resolution);
	hal_saradc_set_channel_mode(adcCfg.channel);
	hal_saradc_set_vin_mode(adcCfg.vinMode);
	hal_saradc_set_fs(adcCfg.sampleRate);
}

uint16_t aux_adc_get_val(void)
{
	uint8_t eoc_timeout = 0;
	while((hal_saradc_get_eoc_status()==0)&&(eoc_timeout < 5)){
		eoc_timeout ++;
	}
	return hal_saradc_get_val();
}

#if(AUX_ADC_INTR_ENABLE)
/**
 * @brief This function registers SAR ADC's isr callback
 * @param[in] cb callback
 */
void aux_adc_register_cb(saradc_callback_t cb)
{
	adcCfg.cb = cb;
}
#endif	//AUX_ADC_INTR_ENABLE

#endif	//AUX_ADC_ENABLE

