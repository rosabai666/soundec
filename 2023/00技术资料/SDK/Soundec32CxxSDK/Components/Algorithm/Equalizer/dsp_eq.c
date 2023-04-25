/** 
***********************************************************
 * 
 * @file name	: dsp_eq.c
 * @author		: nick
 * @version		: Vxx.xx
 * @date		: 2021-1-12
 * @brief		: dsp eq
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
#include <string.h>
#include <string.h>
#include "hal_bq.h"
#include "soundec_math.h"
#include "altotool.h"
#include "altotool_eq.h"
#include "dsp_eq.h"
#include "equalizer.h"
#include "altotool_eq_digitl_gain_table.h"
#include "dsp_eq_para.h"
#include "hal_sysctrl.h"
/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

#define SPKER_USE_HW_EQ             1
#define MIC_USE_HW_EQ               0
#if (MIC_USE_HW_EQ&SPKER_USE_HW_EQ)
######"Err! Please close one!"
#endif
#define HW_EQ_TAP                   8
#define EQ_DIGITAL_GAIN_MAX         40          

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
#if (SUPPORT_DSP_EQ) 

#if (SUPPORT_SPKER_EQ)
GCoreAudioIntData_t SpkEquSwStateLch[EQ_TAP_NUM][4];
GCoreAudioIntData_t SpkEquSwStateRch[EQ_TAP_NUM][4];

Gcore_DspBiquadConstIntData_t sLCoeff[EQ_TAP_NUM] ;//= {0};
#if (SUPPORT_SPKER_STEREO_EQ)
Gcore_DspBiquadConstIntData_t sRCoeff[EQ_TAP_NUM] ;//= {0};
#endif

biquad_casd_df1_inst_q31 spk_eq_lch;
biquad_casd_df1_inst_q31 spk_eq_rch;
#endif

#if (SUPPORT_MIC_EQ)
GCoreAudioIntData_t MicEquSwStateLch[EQ_TAP_NUM][4];
GCoreAudioIntData_t MicEquSwStateRch[EQ_TAP_NUM][4];

Gcore_DspBiquadConstIntData_t mCoeff[EQ_TAP_NUM] ; //= {0};

biquad_casd_df1_inst_q31 mic_eq_lch;
biquad_casd_df1_inst_q31 mic_eq_rch;
#endif

uint32_t spker_eq_sample_rate = SAMPLING_RATE_48000 ;
uint32_t mic_eq_sample_rate   = SAMPLING_RATE_48000 ;





#if (SUPPORT_SPKER_EQ)

void spker_eqLch_coeff_update(void)
{
#if SPKER_USE_HW_EQ	
		hal_bq_init_new(HAL_BQ_STREAM1, (hal_bq_coef_t*)sLCoeff, 0);
		hal_bq_init_new(HAL_BQ_STREAM2, (hal_bq_coef_t*)sLCoeff, 0);
		hal_bq_init_new(HAL_BQ_STREAM3, (hal_bq_coef_t*)sLCoeff, 1);
		hal_bq_init_new(HAL_BQ_STREAM4, (hal_bq_coef_t*)sLCoeff, 1);
		biquad_cascade_df1_init_q31(&spk_eq_lch, EQ_TAP_NUM - HW_EQ_TAP, (q31_t*)&sLCoeff[HW_EQ_TAP], (q31_t*)SpkEquSwStateLch, BIQ_SHIFT); 
		if((gEqCfg.spkEq.stereo == false)||(SUPPORT_SPKER_STEREO_EQ == 0))
		{   
			biquad_cascade_df1_init_q31(&spk_eq_rch, EQ_TAP_NUM - HW_EQ_TAP, (q31_t*)&sRCoeff[HW_EQ_TAP], (q31_t*)SpkEquSwStateRch, BIQ_SHIFT);
		}
#else
		biquad_cascade_df1_init_q31(&spk_eq_lch, EQ_TAP_NUM, (q31_t*)&sLCoeff[0], (q31_t*)SpkEquSwStateLch, BIQ_SHIFT);
		if((gEqCfg.spkEq.stereo == false)||(SUPPORT_SPKER_STEREO_EQ == 0))
		{
			biquad_cascade_df1_init_q31(&spk_eq_rch, EQ_TAP_NUM, (q31_t*)&sRCoeff[0], (q31_t*)SpkEquSwStateRch, BIQ_SHIFT);
		}
#endif
}


#endif

void spker_eqLch_init(void)
{
#if (SUPPORT_SPKER_EQ)
	int i;
	
	hal_bq_set_post_shift(BIQ_SHIFT);

	for (i = 0; i < EQ_TAP_NUM; i++)
	{
		CalcBiquadFilterInt(spker_eq_sample_rate, &sLCoeff[i], &spker_eqLParam[i]);
	}
    spker_eqLch_coeff_update();
#endif
}


#if (SUPPORT_SPKER_STEREO_EQ)
void spker_eqRch_coeff_update(void)
{
#if 0// SPKER_USE_HW_EQ	
    hal_bq_init_new(HAL_BQ_STREAM1, (hal_bq_coef_t*)sRCoeff, 0);
    hal_bq_init_new(HAL_BQ_STREAM2, (hal_bq_coef_t*)sRCoeff, 0);
    hal_bq_init_new(HAL_BQ_STREAM3, (hal_bq_coef_t*)sRCoeff, 1);
    hal_bq_init_new(HAL_BQ_STREAM4, (hal_bq_coef_t*)sRCoeff, 1);
    biquad_cascade_df1_init_q31(&spk_eq_rch, EQ_TAP_NUM - HW_EQ_TAP, (q31_t*)&sRCoeff[HW_EQ_TAP], (q31_t*)SpkEquSwStateRch, BIQ_SHIFT);
#else
	biquad_cascade_df1_init_q31(&spk_eq_rch, EQ_TAP_NUM, (q31_t*)&sRCoeff[0], (q31_t*)SpkEquSwStateRch, BIQ_SHIFT);
#endif
}

void spker_eqRch_init(void)
{
	int i;
	hal_bq_set_post_shift(BIQ_SHIFT);

	for (i = 0; i < EQ_TAP_NUM; i++)
	{
		CalcBiquadFilterInt(spker_eq_sample_rate, &sRCoeff[i], &spker_eqRParam[i]);
	}
	spker_eqRch_coeff_update();
}
#endif

void spker_equ_do_process(int32_t *data,uint32_t sampleRate)
{
#if (SUPPORT_SPKER_EQ)
    if(sampleRate != SAMPLING_RATE_48000)return;

    if(altotoolPara.isEqReady[EQ_SPKER] == false)return;
	if(gEqCfg.spkEq.bypass)return;
	uint8_t gain_index;
	//static uint8_t previousEqType = 1;
    if(gEqCfg.spkEq.stereo == true)
	{
	    gain_index = EQ_DIGITAL_GAIN_MAX - gEqCfg.spkEq.gainBeforeEq;
	    data[0] = data[0]*eq_digital_gain_table[gain_index];
		
	    gain_index = EQ_DIGITAL_GAIN_MAX - gEqCfg.spkEq.gainBeforeEq_R;
	    data[1] = data[1]*eq_digital_gain_table[gain_index];
	}else
	{
	    gain_index = EQ_DIGITAL_GAIN_MAX - gEqCfg.spkEq.gainBeforeEq;
		data[0] = data[0]*eq_digital_gain_table[gain_index];
		data[1] = data[1]*eq_digital_gain_table[gain_index];
	}
#if SPKER_USE_HW_EQ	
	if((gEqCfg.spkEq.stereo == false)||(SUPPORT_SPKER_STEREO_EQ == 0))
	{
		hal_bq_calc_stream12(data ,data); 
#if (HW_EQ_TAP > 4)
		hal_bq_calc_stream34(data ,data);
#endif
	}
	else
	{
		ALIGN(8) int32_t lr_data[2] = {0};
	    lr_data[0] = lr_data[1] = data[0];
		hal_bq_calc_stream12(lr_data ,lr_data); 
		hal_bq_calc_stream34(lr_data ,lr_data);
		data[0] = lr_data[0]; 
	}
#endif

	biquad_cascade_df1_q31(&spk_eq_lch, &data[DATA_LCH], &data[DATA_LCH], 1);
	biquad_cascade_df1_q31(&spk_eq_rch, &data[DATA_RCH], &data[DATA_RCH], 1);
#endif
}


#if (SUPPORT_MIC_EQ)
void mic_eq_coeff_updadte(void)
{
#if MIC_USE_HW_EQ	
	hal_bq_init_new(HAL_BQ_STREAM1, (hal_bq_coef_t*)mCoeff,0);
	hal_bq_init_new(HAL_BQ_STREAM2, (hal_bq_coef_t*)mCoeff,0);
	hal_bq_init_new(HAL_BQ_STREAM3, (hal_bq_coef_t*)mCoeff,1);
	hal_bq_init_new(HAL_BQ_STREAM4, (hal_bq_coef_t*)mCoeff,1);

	biquad_cascade_df1_init_q31(&mic_eq_lch, EQ_TAP_NUM - HW_EQ_TAP, (q31_t*)&mCoeff[HW_EQ_TAP], (q31_t*)MicEquSwStateLch, BIQ_SHIFT);
	biquad_cascade_df1_init_q31(&mic_eq_rch, EQ_TAP_NUM - HW_EQ_TAP, (q31_t*)&mCoeff[HW_EQ_TAP], (q31_t*)MicEquSwStateRch, BIQ_SHIFT);
#else
	biquad_cascade_df1_init_q31(&mic_eq_lch, EQ_TAP_NUM, (q31_t*)&mCoeff[0], (q31_t*)MicEquSwStateLch, BIQ_SHIFT);
	biquad_cascade_df1_init_q31(&mic_eq_rch, EQ_TAP_NUM, (q31_t*)&mCoeff[0], (q31_t*)MicEquSwStateRch, BIQ_SHIFT);
#endif

}
#endif

void mic_eq_init(void)
{
	int i;
#if (SUPPORT_MIC_EQ)
	hal_bq_set_post_shift(BIQ_SHIFT);

	for (i = 0; i < EQ_TAP_NUM; i++)
	{
		CalcBiquadFilterInt(mic_eq_sample_rate, &mCoeff[i], &mic_eqParam[i]);
	}
	mic_eq_coeff_updadte();

    //uart_printf("0x%08x ,0x%08x ,0x%08x ,0x%08x ,0x%08x \r\n",mCoeff[0].b0 ,mCoeff[0].b1,mCoeff[0].b2 ,mCoeff[0].a1 ,mCoeff[0].a2 );
#endif
}

void mic_equ_do_process(int32_t *data ,uint32_t sampleRate )
{
#if (SUPPORT_MIC_EQ)
//    if(sampleRate != SAMPLING_RATE_48000)return;
    if(altotoolPara.isEqReady[EQ_MIC] == false)return;
	if(gEqCfg.micEq.bypass)return;
 	uint8_t gain_index = EQ_DIGITAL_GAIN_MAX - gEqCfg.micEq.gainBeforeEq ;
	data[0] = data[0]*eq_digital_gain_table[gain_index] ;
	data[1] = data[1]*eq_digital_gain_table[gain_index] ;

	
#if MIC_USE_HW_EQ	
	hal_bq_calc_stream12(data ,data); 
	hal_bq_calc_stream34(data ,data);
#endif

	biquad_cascade_df1_q31(&mic_eq_lch, &data[DATA_LCH], &data[DATA_LCH], 1);
	biquad_cascade_df1_q31(&mic_eq_rch, &data[DATA_RCH], &data[DATA_RCH], 1);
	
 #endif
}

uint8_t check_eq_para(eq_band_t para)
{ 
	int8_t ret = 0;

	if((para.eqType < MINI_EQ_TYPE)||(para.eqType > MAX_EQ_TYPE))
	{
     	ret = 1;
		uart_printf("eqType Err!\r\n");
	}
    else if((para.qbw < MINI_EQ_QBW)||(para.qbw > MAX_EQ_QBW))
    {
     	ret = 1;
	    uart_printf("QBW Err!\r\n");
    }
	else if((para.fc < MINI_EQ_FS_HZ)||(para.fc > MAX_EQ_FS_HZ))
	{
		ret = 1;
		uart_printf("FS_HZ Err!\r\n");
	}
	else if((para.gain < MINI_EQ_GAIN)||(para.gain > MAX_EQ_GAIN))
	{
		ret = 1;
		uart_printf("Gain Err!\r\n");
	}
	else{}

	return ret;
}



uint8_t eq_para_update(uint8_t eqModule ,uint8_t index ,eq_band_t para)
{
    uint8_t ret = 0;
	uart_printf("type = %d  ,qbw = %f  fc_hz = %f  gain_dB  = %f.\r\n",para.eqType,para.qbw,para.fc ,para.gain);

	if(check_eq_para(para))return 1;
	
	uart_printf("eq_module = %d , index = %d  .\r\n" ,eqModule , index);
	
    switch(eqModule)
    {
    	case OP_EQ_SPKER_LCH:
		{
#if (SUPPORT_SPKER_EQ)
			spker_eqLParam[index].fc_Hz    = para.fc;
			spker_eqLParam[index].gain_dB  = para.gain;
			spker_eqLParam[index].qbw	   = 1.0/para.qbw;
			spker_eqLParam[index].type	   = para.eqType;
			
			//CalcBiquadFilterInt(spker_eq_sample_rate, &sLCoeff[index], &spker_eqLParam[index]);
			//spker_eqLch_coeff_update();
			spker_eqLch_init();
            #endif
    	}
		break;
		case OP_EQ_SPKER_RCH:
		{
#if (SUPPORT_SPKER_STEREO_EQ)
			spker_eqRParam[index].fc_Hz	    = para.fc;
			spker_eqRParam[index].gain_dB	= para.gain;
			spker_eqRParam[index].qbw		= 1.0/para.qbw;
			spker_eqRParam[index].type 	    = para.eqType;
			
			CalcBiquadFilterInt(spker_eq_sample_rate, &sRCoeff[index], &spker_eqRParam[index]);
			spker_eqRch_coeff_update();
			//spker_eqRch_init();
            #endif
		}
		break;
		case OP_EQ_MIC:
		{
#if (SUPPORT_MIC_EQ)
			mic_eqParam[index].fc_Hz	  = para.fc;
			mic_eqParam[index].gain_dB	  = para.gain;
			mic_eqParam[index].qbw		  = 1.0/para.qbw;
			mic_eqParam[index].type 	  = para.eqType;
			
			CalcBiquadFilterInt(mic_eq_sample_rate, &mCoeff[index], &mic_eqParam[index]);
			mic_eq_coeff_updadte();
			//mic_eq_init();
            #endif
		}
		break;
		default:
			break;
	}

	return ret;
}


void disable_eq_module_channel( Gcore_DspBiquadConstIntData_t *coeff )
{
	coeff->b0 = 0x08000000;
	coeff->b1 = 0xf0d6b080;
	coeff->b2 = 0x074acd60;
	coeff->a1 = 0x0f294f80;
	coeff->a2 = 0xf8b532a0;
}


void eq_module_channel_switch(uint8_t eqModule ,uint8_t ch ,uint8_t onoff)
{
    if(onoff)return;
	
    switch(eqModule)
    {
    	case OP_EQ_SPKER_LCH:
		{
#if (SUPPORT_SPKER_EQ)
			disable_eq_module_channel(&sLCoeff[ch]);
			spker_eqLch_coeff_update();
            #endif
    	}
		break;
		case OP_EQ_SPKER_RCH:
		{
#if (SUPPORT_SPKER_STEREO_EQ)
			disable_eq_module_channel(&sRCoeff[ch]);
			spker_eqRch_coeff_update();
            #endif
		}
		break;
		case OP_EQ_MIC:
		{
#if (SUPPORT_MIC_EQ)
			disable_eq_module_channel(&mCoeff[ch]);
			mic_eq_coeff_updadte();
            #endif
		}
		break;
		default:
		break;
	}
}


void eq_module_reset(uint8_t eqModule)
{
	int i;

    switch(eqModule)
    {
    	case OP_EQ_SPKER_LCH:
		{
#if (SUPPORT_SPKER_EQ)
			for(i = 0; i < EQ_TAP_NUM ; i++ )
			{
				disable_eq_module_channel(&sLCoeff[i]);
			}
			spker_eqLch_coeff_update();
            #endif
    	}
		break;
		case OP_EQ_SPKER_RCH:
		{
#if (SUPPORT_SPKER_STEREO_EQ)
			for(i = 0; i < EQ_TAP_NUM ; i++ )
			{
				disable_eq_module_channel(&sRCoeff[i]);
			}
			spker_eqRch_coeff_update();
            #endif
		}
		break;
		case OP_EQ_MIC:
		{
#if (SUPPORT_MIC_EQ)
			for(i = 0; i < EQ_TAP_NUM ; i++ )
			{
				disable_eq_module_channel(&mCoeff[i]);
			}
			mic_eq_coeff_updadte();
#endif
		}
		break;
		default:
		break;
	}
}

void eq_module_init(void)
{
    int i;
#if (HW_EQ_TAP > 0)
    uint32_t clkGate = hal_sysctrl_get_clock_gate();
    
    SETBIT(clkGate, HAL_SYSCTRL_CLKGT_BQ);
	hal_sysctrl_set_clock_gate(clkGate);
#endif
#if (SUPPORT_SPKER_EQ)
	spker_eqLch_init();
    for(i = 0; i < EQ_TAP_NUM ; i++ )
	{
	    if(gEqCfg.spkEq.band[i].bandEn)
			disable_eq_module_channel(&sLCoeff[i]);
	}
	spker_eqLch_coeff_update();
		
#if (SUPPORT_SPKER_STEREO_EQ)
    if(gEqCfg.spkEq.stereo)
    {
		spker_eqRch_init();
		for(i = 0; i < EQ_TAP_NUM ; i++ )
		{
			if(gEqCfg.spkEq.bandStereoR[i].bandEn)
				disable_eq_module_channel(&sRCoeff[i]);
		}
		spker_eqRch_coeff_update();
    }
#endif
#endif

#if (SUPPORT_MIC_EQ)

	for(i = 0; i < EQ_TAP_NUM ; i++ )
	{
		if(gEqCfg.micEq.band[i].bandEn)
			disable_eq_module_channel(&mCoeff[i]);
	}
	mic_eq_init();
	//mic_eq_coeff_updadte();
#endif
}


#endif/* SUPPORT_DSP_EQ */

