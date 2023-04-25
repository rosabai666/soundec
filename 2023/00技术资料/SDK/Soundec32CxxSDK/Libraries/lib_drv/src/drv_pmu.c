/** 
***********************************************************
 * 
 * @file name   : drv_pmu.c
 * @author      : Rosa.Bai
 * @version	  : V01.00
 * @date		  : 2020-02-24
 * @brief		  : Power manage unit
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

#include <stdio.h>
#include "common.h"
#include "hal.h"
#include "hal_gpio.h"
#include "hal_maestro.h"
#include "hal_interrupt.h"
#include "hal_sysctrl.h"
#include "drv_pmu.h"
#include "drv_uart.h"

//DLC_PCTRL_st regPICL;
//DLC_IMR_st regIMR;    /*Register reset value: 0x3000063F*/
//maestro_state_st gMaestroState = { { { 0 } }, 0, 0, 0, 0, 0, 0 };

WIU_IFR_t wiuFlag;
dmu_offset_t pmuAdjMode = DMU_RAR;
maestro_mode_t maestroMode = MAESTRO_MODE_ON_NF_NV;
ICU_cgo_steps_t pll_step_width = ICU_CGO_STEP_1;
enum{
	CURR_VAL = 0,
	MAX_VAL,
	VAL_NUM,
};
U8 dmuRegulatorVal[DMU_MAX][VAL_NUM] ;//= {0x1F};/*default value & max value*/

char *DMU_Unit[DMU_MAX]={
	"RAR",
	"eSR",
	"iRL1",
	"iRL2",
	"OPM",
	"PLL",
};

maestro_status_t pmu_set_icu_mode(maestro_mode_t mode);

static void dmu_init_regulator_value(void)
{
	U8 dmu_unit = DMU_RAR;

	for (dmu_unit=DMU_RAR; dmu_unit<DMU_MAX; dmu_unit++)
	{
		if (dmu_unit == DMU_RAR)
			dmuRegulatorVal[dmu_unit][MAX_VAL] = 0x1F;
		else if (dmu_unit == DMU_OPM)
			dmuRegulatorVal[dmu_unit][MAX_VAL] = 1;
		else
			dmuRegulatorVal[dmu_unit][MAX_VAL] = 0x0F;
		dmuRegulatorVal[dmu_unit][CURR_VAL] = dmuRegulatorVal[dmu_unit][MAX_VAL];
	}
}

U8 get_dmuX_current_value(dmu_offset_t dmu_index)
{
	return dmuRegulatorVal[dmu_index][CURR_VAL];
}

U8 get_dmuX_max_value(dmu_offset_t dmu_index)
{
	return dmuRegulatorVal[dmu_index][MAX_VAL];
}

void set_dmuX_current_value(dmu_offset_t dmu_index, U8 val)
{
	dmuRegulatorVal[dmu_index][CURR_VAL] = val;
}

void set_dmuX_max_value(dmu_offset_t dmu_index, U8 val)
{
	dmuRegulatorVal[dmu_index][MAX_VAL] = val;
}

//////////////////////////////////////////////////////////////////////
////////////////////////////Applications////////////////////////////////
//////////////////////////////////////////////////////////////////////
/* ****** Interrupt ****** */
void maestro_handle_wiu_interrupt(void *arg)
{
/*Sequence
    All the sequences are hardware configured defining the number of operations and the operations to
    perform.

    To ensure the SoC remains always in a functional state, the order of the sequence will depend on
    the current mode and the requested mode. It respects the following rules.
    1. The mode/sub-mode number1 of the DMU connected to power regulator is always greater than
    or equal to the mode/sub-mode number of the DMU connected to clock generator (refer to
    table 2.5).
    2. If the current mode of the island is greater than the requested mode, the island mode is
    changed before DMU connected to regulator or clock generator modes/sub-modes, otherwise
    it is changed after.
    3. In case of handling forward body biasing generator set in BBON mode, if the body biasing
    order associated to the current mode is lower than the one associated to the requested mode,
    the body biasing generator order is applied first, otherwise last.
    4. In case of handling reverse body biasing generator set in BBON mode, if the body biasing
    order associated to the current mode is higher than the one associated to the requested mode,
    the body biasing generator order is applied first, otherwise last.

    SCU takes control of the PICL bus once a sequence must be performed.
    In case PICL bus is not available, the sequence start is delayed until the bus is unlocked.
    A start of SCU sequence locks the PICL bus for the SCU. The PICL bus is unlocked when SCU sequence ends.

    32 sequences triggered by events controlled by the WIU.
*/
	//pmu_set_icu_mode(MAESTRO_MODE_ON_NF_NV);		//on:current:25.9mA
	hal_interrupt_clear_irq(PMU_IRQn);
	//U32 intr = MAESTRO_Read(DLC_SR);
#if 1//def SUPPORT_UART_TEST
	//uart_printf("DLC_SR:%2x!\n", intr);
#endif
//	intr = MAESTRO_Read(DLC_IFR);
#if 1//def  SUPPORT_UART_TEST
	//uart_printf("DLC_IFR:%2x!\n", intr);
#endif
//	MAESTRO_Write(DLC_IFR, intr);
}
/**
 * @fn         pmu_set_wiu_interrupt_all
 * @brief      [set WIU interrupt enable or disable]
 * @author   Rosa.Bai
 * @date      2019-11-04
 * @param    [description]
 *        @arg   maskBits        [Refer to WIU_ISPMR_t; 1: mask all of WIU interrupt;  0: enable all of WIU interrupt]
 *							WIU_IRQ_7 (bit 7) downto WIU_IRQ_0 (bit 0)masks, active high.
 *							WIU_IRQ_15 (bit 7) downto WIU_IRQ_8 (bit 0)masks, active high.
 * @return	   None
 */
void pmu_set_wiu_interrupt(WIU_ISPMR_t maskBits)
{
	hal_maestro_set_wiu_interrupt(WIU_0, maskBits);
}

void pmu_set_wiu_ifr(wiu_offset_t wiu_index, WIU_IFR_t valIFR)
{
  // Disable WIU Interrupt
	hal_maestro_set_wiu_register(wiu_index, WIU_IFR_0, valIFR.ifrVal&0x00FF);/*WIU_IRQ_7 (bit 7) downto WIU_IRQ_0 (bit 0)masks, active high.*/
	hal_maestro_set_wiu_register(wiu_index, WIU_IFR_1, (valIFR.ifrVal&0xFF00)>>8);/*WIU_IRQ_15 (bit 7) downto WIU_IRQ_8 (bit 0)masks, active high.*/
}

void pmu_enable_all_interrupts(void)
{
  /*
	The SCU sequence which will be executed can be configured for each interrupt source by setting
	the sequence number in the WIU interrupt control registers (SEQ_SEL_IRQ field of WIU_ICR_* registers).
  */
	WIU_ISPMR_t wiuIspmr;
	wiuIspmr.ispmrVal = 0;
	hal_maestro_mask_wiu_interrupt();
	hal_maestro_set_wiu_interrupt(WIU_0, wiuIspmr);
}

void pmu_set_maestro_mode(maestro_mode_t icuMode)
{
	//uart_printf("set maestro mode : %d \n", icuMode);
	if (icuMode == MAESTRO_MODE_EXT_FOFF_NV)
	{
		//Set_System_Clk(CLK_24MHZ_MODE);
		//Set_24MHzOff();
		//if (wait_PLL_Stable())
		if (hal_pll_off() == HAL_STATUS_OK)
		{
			if (setIcuModeUntilStable(ICU_0, icuMode) == MAESTRO_OK)
			{
				//Pllout_start(PLLOUT_200MHZ);
				hal_pll_set_new(PLL_OUT_VALUE, false);

				// Resume sys state.
#if UART_ENABLE
				uart_init();
#endif
				//uart_printf("setIcuModeUntilStable\n");
				//uart_printf("Maestro wakeup type:%d,%d\n", pmu_aoport_read(PORT_INTRREG),read_reg32(0x7700712C));
				pmu_aoport_write(PORT_INTRCLR, 1);/*Clear AO interrupt flag*/
				hal_maestro_clear_wiu_interrupt();
//				while(1)
//				{
//					uart_printf("Maestro wakeup type:%d,%d\n", pmu_aoport_read(PORT_INTRREG),read_reg32(0x7700712C));
//				}
				write_reg32(0x7700f000, 0x89898989);
			}
			else
			{
				//write_reg32(0x7700f000, 0x66666667);
				uart_printf("setIcuModeNotStable\n");
			}
		}
		else
		{
			//write_reg32(0x7700f000, 0x66666666);
			uart_printf("hal_pll_off fail\n");
		}
	}
	else
	{
		//write_reg32(0x7700f000, 0x66666668);
		uart_printf("icuMode = MAESTRO_MODE_ON_NF_NV\n");
	}
}

U8 pmu_get_pll(void)
{
	return hal_maestro_get_icu_cgo();
}

/*  Set PLL directly */
void pmu_set_pll(U8 val)
{
	// 1. Set PLL directly,
//	hal_maestro_set_dmu_cgno(DMU_PLL, 0x00);   //MIN
//	hal_maestro_set_dmu_cgno(DMU_PLL, 0x1F);   //MAX
//	hal_maestro_set_dmu_cgno(DMU_PLL, 0x0F);   //Middle
	hal_maestro_set_dmu_cgno(DMU_PLL, val);
	dmuRegulatorVal[DMU_PLL][CURR_VAL] = pmu_get_pll();
}

/*
 * 	Adjust PLL step by step
 *	1 Increase with step 1,2,3,4 auto
 *	2 Decrease with step 1,2,3,4 auto
 * */
void pmu_tune_pll(U8 dir)
{
	hal_maestro_set_icu_cgo(dir, pll_step_width);
	dmuRegulatorVal[DMU_PLL][CURR_VAL] = pmu_get_pll();
}

maestro_status_t pmu_set_rar(U8 val)
{
	if (val>dmuRegulatorVal[DMU_RAR][1])  return MAESTRO_ERROR_INCORRECT_VAL;
	dmuRegulatorVal[DMU_RAR][0] = val;
	hal_maestro_set_dmu_rno(DMU_RAR, val);
	return MAESTRO_OK;
}

maestro_status_t pmu_set_icu_mode(maestro_mode_t mode)
{
#if 1//def  SUPPORT_UART_TEST
	uart_printf("set icu mode:%d\n",mode);
#endif
	maestroMode = mode;
	//maestro_status_t icumtSt = hal_maestro_set_icu_mode(ICU_0, mode);

	return MAESTRO_OK;
}

void pmu_set_regulator(dmu_offset_t dmu_index, U8 val)
{
	dmuRegulatorVal[dmu_index][CURR_VAL] = val;
	hal_maestro_set_dmu_rno(dmu_index, val);
}

void pmu_tune_regulator(dmu_offset_t dmu_index, U8 dir)
{
	U8 value = dmuRegulatorVal[dmu_index][CURR_VAL];
	U8 max = dmuRegulatorVal[dmu_index][MAX_VAL];

	if (dir && value<max)
		value++;
	else if (!dir && value)
		value--;
	else
		return;

	if (dmu_index < DMU_OPM)
	{
		hal_maestro_set_dmu_rno(dmu_index, value);
		dmuRegulatorVal[dmu_index][CURR_VAL] = value;
	}
	else if (dmu_index == DMU_PLL)
	{
		pmu_tune_pll(dir);
		dmuRegulatorVal[DMU_PLL][CURR_VAL] = pmu_get_pll();
	}
#if 1//def  SUPPORT_UART_TEST
	uart_printf("dmu mode[%s], value:%d\n", DMU_Unit[dmu_index], dmuRegulatorVal[dmu_index][CURR_VAL]);
#endif
}

maestro_status_t pmu_set_regulator_hz(void)
{
	hal_maestro_set_dmu_manual_mode(DMU_eSR);
	hal_maestro_set_dmu_manual_mode(DMU_iLR1);
	hal_maestro_set_dmu_manual_mode(DMU_iLR2);

	return MAESTRO_OK;
}

void pmu_init_ao(void)
{
	//WIU_ISPMR_t intMask;
	U16 retry = 0xFFF;
	
	MAESTRO_Write(DLC_IMR, 0);//0xFFFF); mask dlc interrupt
	hal_interrupt_disable_irq(PMU_IRQn);
	
	pmu_ao_write(DBCLK_DIV, 7);
	pmu_aoport_write(PORT_DIR, 1);
	pmu_aoport_write(PORT_SEL, 1);
	pmu_aoport_write(PORT_INTRSEL, 1);
	pmu_aoport_write(PORT_INTRLEL, 0);
	pmu_aoport_write(PORT_INTRPOL, 1);
	pmu_aoport_write(PORT_INTRCLR, 1);
#if 1//def  SUPPORT_UART_TEST
	while(pmu_aoport_read(PORT_INTRREG)&&--retry);
	//uart_printf("retryCnt:%d, flag:%d\n", retry, pmu_aoport_read(PORT_INTRREG));
#endif
	hal_interrupt_clear_irq(PMU_IRQn);
	pmu_enable_all_interrupts();
	hal_interrupt_register_isr_handler(PMU_IRQn, maestro_handle_wiu_interrupt);
}

pmu_wakeup_src_t pmu_get_wakeup_source(void)
{
	WIU_IFR_t wiuFlag;

	hal_maestro_get_wiu_interrupt(&wiuFlag);
	hal_maestro_clear_wiu_interrupt();

	if (pmu_aoport_read(PORT_INTRREG) == 1)
		return PMU_WAKEUP_SRC_AO;
//	if (wiuFlag.bits.ifrBit0 == 1)
//		return PMU_WAKEUP_SRC_AO;
	else if (wiuFlag.bits.ifrBit1 == 1)
		return PMU_WAKEUP_SRC_USB;
	else
		return PMU_WAKEUP_SRC_NULL;
}

// power down mode, both ES1 & ES2 supported, can be wakeuped by AO
// after enter this mode, system current will down to 3.8mA
void pmu_enter_es1_lowpower_mode(void)
{
	pmu_init_ao();	// Init always on port for system wakeup.
	pmu_set_maestro_mode(MAESTRO_MODE_EXT_FOFF_NV);
}

// power down, only ES2 supported, can be wakeuped by AO
// after enter this mode, system current will down to 1.3mA
void pmu_enter_es2_lowpower_mode_usb(void)
{
	uint32_t mask;
    uint32_t clkGate = hal_sysctrl_get_clock_gate();

	hal_interrupt_save_and_set_interrupt_mask(&mask);//Interrupts must be turned off before entering low power mode
		
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_MAESTRO);
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_GPIOAO);
	hal_sysctrl_set_clock_gate(clkGate);
	
	write_reg(0x7700F020,0xffffe103) ;

	while (hal_pll_off() != HAL_STATUS_OK);

// SET_DMU_RNO
	// Power off: Can be wakeuped by AO and USB
//	pmu_set_regulator(DMU_RAR, 22);
//	delay_us(10);
	pmu_set_regulator(DMU_eSR, 22);
	delay_us(10);
	pmu_set_regulator(DMU_iLR1, 22);
	delay_us(10);
	pmu_set_regulator(DMU_iLR2, 22);
	delay_us(10);
//	uart_printf("\nSET_DMU_RNO.\n");

// EN_WIU_INTERRUPT
	pmu_enable_all_interrupts();	// WIU0_Bit0 for AO, WIU0_Bit1 for USB
//	uart_printf("\n pmu_enable_all_interrupts.\n");

// Init always on port for system wakeup.
	pmu_init_ao();

// SET_WIU_IFR
	WIU_IFR_t wiuIFR;
	wiuIFR.ifrVal = 0;			// Set WIU_IFR_0 and WIU_IFR_1 to 0
	wiuIFR.bits.ifrBit15 = 1;	// Set bit7 of WIU_IFR_1 to 1
	pmu_set_wiu_ifr(WIU_0, wiuIFR);
}

// power down, only ES2 supported, can be wakeuped by AO
// after enter this mode, system current will down to 1.3mA
void pmu_enter_es2_lowpower_mode(void)
{
	uint32_t mask;
	uint32_t clkGate = hal_sysctrl_get_clock_gate();

	hal_interrupt_save_and_set_interrupt_mask(&mask);//Interrupts must be turned off before entering low power mode
		
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_MAESTRO);
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_GPIOAO);
	hal_sysctrl_set_clock_gate(clkGate);
// SET_DMU_RNO
	// Power off: Can be wakeuped by AO and USB
	pmu_set_regulator(DMU_RAR, 22);
	delay_us(10);
	pmu_set_regulator(DMU_eSR, 22);
	delay_us(10);
	pmu_set_regulator(DMU_iLR1, 22);
	delay_us(10);
	pmu_set_regulator(DMU_iLR2, 22);
	delay_us(10);
	uart_printf("\nSET_DMU_RNO.\n");

// EN_WIU_INTERRUPT
	pmu_enable_all_interrupts();
	uart_printf("\npmu_enable_all_interrupts.\n");

// Init always on port for system wakeup.
	pmu_init_ao();

// SET_WIU_IFR
	WIU_IFR_t wiuIFR;
	wiuIFR.ifrVal = 0;			// Set WIU_IFR_0 and WIU_IFR_1 to 0
	wiuIFR.bits.ifrBit15 = 1;	// Set bit7 of WIU_IFR_1 to 1
	pmu_set_wiu_ifr(WIU_0, wiuIFR);
}

void pmu_init(void)
{
	maestroMode = MAESTRO_MODE_ON_NF_NV;
	dmu_init_regulator_value();
}


