/**
***********************************************************
 *
 * @file name   : Altotool_eq.c
 * @author      : nick
 * @version     : V0.1
 * @date        : 2022-03-10
 * @brief       : alto tool eq config
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
 * and Sourced Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/

#include "altotool.h"
#include "altotool_eq.h"
#include "dsp_eq.h"
#include "dsp_eq_para.h"
#include "userFlash.h"

#if (SUPPORT_DSP_EQ)

eq_t gEqCfg;
extern eq_chx_t micEqBase;
extern eq_chx_t spkEqBase;

void save_altotool_eq_data(uint8_t eqM)
{
#if(FLASH_SAVE_BANK_ENABLE)
	if(eqM == EQ_SPKER)
	{
		user_flash_region_t region = UFLASH_REGION_SPK_EQ_PARAM(0);
		user_flash_save_data(region, (uint32_t*)&(gEqCfg.spkEq));
      /*save spker eq*/
	}
	else
	{
		user_flash_region_t region = UFLASH_REGION_MIC_EQ_PARAM;
		user_flash_save_data(region, (uint32_t*)&(gEqCfg.micEq));
      /*save mic eq*/
	}
#endif
}

uint8_t set_altotool_spker_eq(uint8_t ch)
{
    uint8_t cmd = altotoolPara.cmd;
	int8_t  ret = 0;
	
	altotoolPara.isEqReady[EQ_SPKER] = false;			
	switch(cmd)
	{
		case CMD_EQ_NUM://				 =	0x80,
			gEqCfg.spkEq.eqNum =	altotoolPara.rxPkt.data[COMM_DATA_POS];
			//switch EQ
#if(FLASH_SAVE_BANK_ENABLE)
			user_flash_region_t spkRegion = UFLASH_REGION_SPK_EQ_PARAM(0);
			if(user_flash_load_data(spkRegion, (uint32_t*)&(gEqCfg.spkEq)) == 0)
			{
				uart_printf("restore spk eq para ok!\r\n");
			}
			else
			{
#endif	//FLASH_SAVE_BANK_ENABLE
				uart_printf("set default spk eq para ok!\r\n");
		#if (SUPPORT_SPKER_STEREO_EQ)
				gEqCfg.spkEq.stereo = 1;
		#endif
				for(int m = 0 ; m < 10; m ++)
				{
					gEqCfg.spkEq.band[m].bandEn = TRUE;
		#if (SUPPORT_SPKER_STEREO_EQ)
					gEqCfg.spkEq.bandStereoR[m].bandEn = TRUE;
		#endif
		            set_spker_eq_para(EQ_SPKER,m);
				}
				gEqCfg.spkEq.gainBeforeEq = 0;
#if(FLASH_SAVE_BANK_ENABLE)
			}
#endif
			break;
		case CMD_EQ_MONO_STEREO_SWITCH://				
			gEqCfg.spkEq.stereo =  altotoolPara.rxPkt.data[COMM_DATA_POS];
			uart_printf("isMonoStereo : %d \r\n",gEqCfg.spkEq.stereo);
			
#if (SUPPORT_SPKER_STEREO_EQ)
			spker_eqLch_init();
			if(gEqCfg.spkEq.stereo)
				spker_eqRch_init();
			
#endif
			break;
		case CMD_EQ_BYBASS:			
		    gEqCfg.spkEq.bypass = altotoolPara.rxPkt.data[COMM_DATA_POS];
			uart_printf("isBypass : %d \r\n", gEqCfg.spkEq.bypass);
			break;
		case CMD_EQ_GAIN:			
		    //gEqCfg.spkEq.gainBeforeEq = altotoolPara.rxPkt.data[COMM_DATA_POS];
			gEqCfg.spkEq.gainBeforeEqType = altotoolPara.rxPkt.data[COMM_DATA_POS+1]; 
			uart_printf("gaineqtype:%d\n",gEqCfg.spkEq.gainBeforeEqType);
			if(gEqCfg.spkEq.gainBeforeEqType==0 ||gEqCfg.spkEq.gainBeforeEqType==1)
			{
				gEqCfg.spkEq.gainBeforeEq =  altotoolPara.rxPkt.data[COMM_DATA_POS];
				uart_printf("gain : %d \r\n",gEqCfg.spkEq.gainBeforeEq);
			}else
			{
				gEqCfg.spkEq.gainBeforeEq_R =  altotoolPara.rxPkt.data[COMM_DATA_POS];
				uart_printf("gainR : %d \r\n",gEqCfg.spkEq.gainBeforeEq_R);
			}	
			
			break;
		case CMD_EQ_RESET:			
			uart_printf("reset!\r\n");
			eq_module_reset(OP_EQ_SPKER_LCH);
			bool isStereo = gEqCfg.spkEq.stereo;
			if(gEqCfg.spkEq.stereo)
			{
				eq_module_reset(OP_EQ_SPKER_RCH);
			}
			memcpy((uint8_t *)&gEqCfg.spkEq, (uint8_t *)&spkEqBase, sizeof(spkEqBase));
			gEqCfg.spkEq.stereo = isStereo;
			gEqCfg.spkEq.bypass   = false;
			break;
		case CMD_EQ_SAVE:			
			uart_printf("save spk eq!\r\n");
 			save_altotool_eq_data(EQ_SPKER);
			break;
		case CMD_EQ_SWITCH_L:			
			gEqCfg.spkEq.band[ch].bandEn = altotoolPara.rxPkt.data[COMM_DATA_POS];
			uart_printf("band_enable[%d] L : %d \r\n",ch , gEqCfg.spkEq.band[ch].bandEn);
			eq_module_channel_switch(OP_EQ_SPKER_LCH,ch,gEqCfg.spkEq.band[ch].bandEn);
			if(gEqCfg.spkEq.band[ch].bandEn)
			{
				ret = eq_para_update(OP_EQ_SPKER_LCH,ch,gEqCfg.spkEq.band[ch]);
			}
			break;
		case CMD_EQ_COEFF_L:
			break;
		case CMD_EQ_PARA_L: 			
			//uart_printf("spker band[%d] enable : %d \r\n",ch , gEqCfg.spkEq.band[ch].bandEn);
			gEqCfg.spkEq.band[ch].qbw        =   byteToFloat(*((uint32_t*)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			gEqCfg.spkEq.band[ch].fc         =   byteToFloat(*((uint32_t*)(&altotoolPara.rxPkt.data[COMM_DATA_POS + 4])));
			gEqCfg.spkEq.band[ch].gain       =   byteToFloat(*((uint32_t*)(&altotoolPara.rxPkt.data[COMM_DATA_POS + 8])));
			gEqCfg.spkEq.band[ch].eqType     =   altotoolPara.rxPkt.data[22];
			//uart_printf("%f %f %0f.\r\n",gEqCfg.spkEq.band[ch].qbw,gEqCfg.spkEq.band[ch].fc,gEqCfg.spkEq.band[ch].gain);
			if(gEqCfg.spkEq.band[ch].bandEn)
			{
				ret = eq_para_update(OP_EQ_SPKER_LCH,ch,gEqCfg.spkEq.band[ch]);
			}
			break;
		case CMD_EQ_REQUEST_L:
			break;
#if (SUPPORT_SPKER_STEREO_EQ)
		case CMD_EQ_SWITCH_R:
			gEqCfg.spkEq.bandStereoR[ch].bandEn = altotoolPara.rxPkt.data[COMM_DATA_POS];
			uart_printf("band_enable[%d] R : %d \r\n",ch,gEqCfg.spkEq.bandStereoR[ch].bandEn);
			eq_module_channel_switch(OP_EQ_SPKER_RCH,ch,gEqCfg.spkEq.bandStereoR[ch].bandEn);	
			if(gEqCfg.spkEq.bandStereoR[ch].bandEn)
			{
				ret = eq_para_update(OP_EQ_SPKER_RCH,ch,gEqCfg.spkEq.bandStereoR[ch]);
			}
		    break;
		case CMD_EQ_COEFF_R:	
			break;
		case CMD_EQ_PARA_R:	
			//uart_printf("band[%d] enable : %d \r\n",ch,gEqCfg.spkEq.bandStereoR[ch].bandEn);
			gEqCfg.spkEq.bandStereoR[ch].qbw       = byteToFloat(*((uint32_t*)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			gEqCfg.spkEq.bandStereoR[ch].fc        = byteToFloat(*((uint32_t*)(&altotoolPara.rxPkt.data[COMM_DATA_POS + 4])));
			gEqCfg.spkEq.bandStereoR[ch].gain      = byteToFloat(*((uint32_t*)(&altotoolPara.rxPkt.data[COMM_DATA_POS + 8])));
			gEqCfg.spkEq.bandStereoR[ch].eqType    = altotoolPara.rxPkt.data[22];
			
			//uart_printf("%f %f %0f.\r\n",gEqCfg.spkEq.band[ch].qbw,gEqCfg.spkEq.band[ch].fc,gEqCfg.spkEq.band[ch].gain);
			if(gEqCfg.spkEq.bandStereoR[ch].bandEn)
			{
				ret = eq_para_update(OP_EQ_SPKER_RCH,ch,gEqCfg.spkEq.bandStereoR[ch]);
			}
			break;
		case CMD_EQ_REQUEST_R:	
			break;
#endif
		case CMD_EQ_MAX:	
			break;
		default:
			break;
	}
	
	altotoolPara.isEqReady[EQ_SPKER] = true ;
	uart_printf("stereo = %d ,bypass = %d , isEqReady = %d \r\n",gEqCfg.spkEq.stereo,gEqCfg.micEq.bypass,altotoolPara.isEqReady[EQ_SPKER]);
	
	return ret;
}


uint8_t set_altotool_mic_eq(uint8_t ch)
{
	uint8_t cmd = altotoolPara.cmd;
	//uint32_t *data = (uint32_t*)(&altotoolPara.rxPkt.data[COMM_DATA_POS]);
	uint8_t ret = 0;
	
	altotoolPara.isEqReady[EQ_MIC] = false ;			
	switch(cmd)
	{
		case CMD_EQ_NUM://				 =	0x80,
			gEqCfg.micEq.eqNum =	altotoolPara.rxPkt.data[COMM_DATA_POS];
			break;
		case CMD_EQ_BYBASS: 		
			gEqCfg.micEq.bypass = altotoolPara.rxPkt.data[COMM_DATA_POS];
			uart_printf("isBypass : %d \r\n", gEqCfg.micEq.bypass);
			break;
		case CMD_EQ_GAIN:			
			gEqCfg.micEq.gainBeforeEq = altotoolPara.rxPkt.data[COMM_DATA_POS];
			uart_printf("gain : %d \r\n",gEqCfg.micEq.gainBeforeEq);
			break;
		case CMD_EQ_RESET:			
			uart_printf("mic reset!\r\n");
			eq_module_reset(OP_EQ_MIC);
			memcpy((uint8_t *)&gEqCfg.micEq, (uint8_t *)&micEqBase, sizeof(micEqBase));
			gEqCfg.micEq.bypass = false;
			break;
		case CMD_EQ_SAVE:			
			uart_printf("mic save !\r\n");
 			save_altotool_eq_data(EQ_MIC);
			break;
		case CMD_EQ_SWITCH_L:			
			gEqCfg.micEq.band[ch].bandEn = altotoolPara.rxPkt.data[COMM_DATA_POS];
			uart_printf("band_enable[%d] L : %d \r\n",ch ,gEqCfg.micEq.band[ch].bandEn);
			if(gEqCfg.micEq.band[ch].bandEn)
			{
				ret = eq_para_update(OP_EQ_MIC,ch,gEqCfg.micEq.band[ch]);
			}
			else
			{
				eq_module_channel_switch(OP_EQ_MIC,ch,gEqCfg.micEq.band[ch].bandEn);
			}
			break;
		case CMD_EQ_COEFF_L:
			break;
		case CMD_EQ_PARA_L: 			

			//uart_printf("mic band[%d] enable : %d \r\n",ch , gEqCfg.micEq.band[ch].bandEn);
			gEqCfg.micEq.band[ch].qbw		 =	 byteToFloat(*((uint32_t*)(&altotoolPara.rxPkt.data[COMM_DATA_POS])));
			gEqCfg.micEq.band[ch].fc		 =	 byteToFloat(*((uint32_t*)(&altotoolPara.rxPkt.data[COMM_DATA_POS + 4])));
			gEqCfg.micEq.band[ch].gain		 =	 byteToFloat(*((uint32_t*)(&altotoolPara.rxPkt.data[COMM_DATA_POS + 8])));
			gEqCfg.micEq.band[ch].eqType	 =	 altotoolPara.rxPkt.data[22];

			//uart_printf("%f %f %0f.\r\n",gEqCfg.spkEq.band[ch].qbw,gEqCfg.spkEq.band[ch].fc,gEqCfg.spkEq.band[ch].gain);
			if(gEqCfg.micEq.band[ch].bandEn){
				ret = eq_para_update(OP_EQ_MIC,ch,gEqCfg.micEq.band[ch]);
			}
			break;
		case CMD_EQ_REQUEST_L:
			break;
		case CMD_EQ_MAX:	
			break;
		default:
			break;
	}
	
	altotoolPara.isEqReady[EQ_MIC] = true ; 		
	uart_printf("bypass = %d , isEqReady = %d \r\n",gEqCfg.micEq.bypass,altotoolPara.isEqReady[EQ_MIC]);
	return ret;
}


void set_spker_eq_para(uint8_t eqModule , uint8_t ch )
{
#if (SUPPORT_SPKER_EQ)
    if(eqModule == EQ_SPKER)
    {
		gEqCfg.spkEq.band[ch].eqType         = spker_eqLParam[ch].type;
		gEqCfg.spkEq.band[ch].fc 	         = spker_eqLParam[ch].fc_Hz;
		gEqCfg.spkEq.band[ch].qbw	         = spker_eqLParam[ch].qbw;
		gEqCfg.spkEq.band[ch].gain	         = spker_eqLParam[ch].gain_dB;
#if (SUPPORT_SPKER_STEREO_EQ)
		gEqCfg.spkEq.bandStereoR[ch].eqType  = spker_eqRParam[ch].type;
		gEqCfg.spkEq.bandStereoR[ch].fc	     = spker_eqRParam[ch].fc_Hz;
		gEqCfg.spkEq.bandStereoR[ch].qbw     = spker_eqRParam[ch].qbw;
		gEqCfg.spkEq.bandStereoR[ch].gain    = spker_eqRParam[ch].gain_dB;
#endif
    }
	else
#endif
	{
#if(SUPPORT_MIC_EQ)
		gEqCfg.micEq.band[ch].eqType          = mic_eqParam[ch].type;
		gEqCfg.micEq.band[ch].fc 	          = mic_eqParam[ch].fc_Hz;
		gEqCfg.micEq.band[ch].qbw	          = mic_eqParam[ch].qbw;
		gEqCfg.micEq.band[ch].gain	          = mic_eqParam[ch].gain_dB;
#endif
	}
}

void get_spker_eq_para(uint8_t eqModule , uint8_t ch )
{
#if (SUPPORT_SPKER_EQ)
	if(eqModule == EQ_SPKER)
	{
		spker_eqLParam[ch].type		= gEqCfg.spkEq.band[ch].eqType; 
		spker_eqLParam[ch].fc_Hz 	= gEqCfg.spkEq.band[ch].fc;
		spker_eqLParam[ch].qbw		= gEqCfg.spkEq.band[ch].qbw;
		spker_eqLParam[ch].gain_dB	= gEqCfg.spkEq.band[ch].gain;
#if (SUPPORT_SPKER_STEREO_EQ)
		spker_eqRParam[ch].type		= gEqCfg.spkEq.bandStereoR[ch].eqType;
		spker_eqRParam[ch].fc_Hz 	= gEqCfg.spkEq.bandStereoR[ch].fc;
		spker_eqRParam[ch].qbw		= gEqCfg.spkEq.bandStereoR[ch].qbw;
		spker_eqRParam[ch].gain_dB	= gEqCfg.spkEq.bandStereoR[ch].gain;
#endif
	}
	else
#endif
	{
#if(SUPPORT_MIC_EQ)
		mic_eqParam[ch].type 		= gEqCfg.micEq.band[ch].eqType;
		mic_eqParam[ch].fc_Hz		= gEqCfg.micEq.band[ch].fc ;
		mic_eqParam[ch].qbw			= gEqCfg.micEq.band[ch].qbw;
		mic_eqParam[ch].gain_dB		= gEqCfg.micEq.band[ch].gain;
#endif
	}
}



void altotool_eq_init(void)
{
	uart_printf("altotool_eq_init!\r\n");
	memset(&gEqCfg , 0 , sizeof(gEqCfg));

#if (SUPPORT_SPKER_EQ)
#if(FLASH_SAVE_BANK_ENABLE)
	user_flash_region_t spkRegion = UFLASH_REGION_SPK_EQ_PARAM(0);

	if(user_flash_load_data(spkRegion, (uint32_t*)&(gEqCfg.spkEq)) == 0)
	{
		uart_printf("restore spk eq para ok!\r\n");
		uart_printf("bypass: %d\n", gEqCfg.spkEq.bypass);

		uart_printf("L-fc[1] = %f , L-fc[2] = %f \r\n",gEqCfg.spkEq.band[1].fc , gEqCfg.spkEq.band[2].fc);
#if (SUPPORT_SPKER_STEREO_EQ)
		uart_printf("R-fc[1] = %f , R-fc[2] = %f \r\n",gEqCfg.spkEq.bandStereoR[1].fc , gEqCfg.spkEq.bandStereoR[2].fc);
#endif
		for(int m = 0 ; m < EQ_NUM; m ++)
		{
			get_spker_eq_para(EQ_SPKER,m);
		}
	}
	else
	{
#endif	//FLASH_SAVE_BANK_ENABLE
		uart_printf("set default spk eq para ok!\r\n");
		gEqCfg.spkEq.stereo = 1;

		for(int m = 0 ; m < EQ_NUM; m ++)
		{
			gEqCfg.spkEq.band[m].bandEn = TRUE;
#if (SUPPORT_SPKER_STEREO_EQ)
			gEqCfg.spkEq.bandStereoR[m].bandEn = TRUE;
#endif
            set_spker_eq_para(EQ_SPKER,m);
		}
		gEqCfg.spkEq.gainBeforeEq = 0;
		gEqCfg.spkEq.gainBeforeEq_R = 0;
#if(FLASH_SAVE_BANK_ENABLE)
	}
#endif	//FLASH_SAVE_BANK_ENABLE
#endif	//SUPPORT_SPKER_EQ
#if(SUPPORT_MIC_EQ)
#if(FLASH_SAVE_BANK_ENABLE)
	user_flash_region_t micRegion = UFLASH_REGION_MIC_EQ_PARAM;
	if(user_flash_load_data(micRegion, (uint32_t*)&(gEqCfg.micEq)) == 0)
	{
    	uart_printf("restore mic eq para ok!\r\n");
		uart_printf("M-fc[1] = %f , M-fc[2] = %f \r\n",gEqCfg.micEq.band[1].fc , gEqCfg.micEq.band[2].fc);
		
		for(int m = 0 ; m < EQ_NUM; m ++)
		{
			get_spker_eq_para(EQ_MIC,m);
		}
	}
	else
	{
#endif	//FLASH_SAVE_BANK_ENABLE
		uart_printf("set default mic eq para ok!\r\n");
		for(int m = 0 ; m < 10; m ++)
		{
			gEqCfg.micEq.band[m].bandEn = TRUE;
            set_spker_eq_para(EQ_MIC,m);
		}
		gEqCfg.micEq.gainBeforeEq = 0;
#if(FLASH_SAVE_BANK_ENABLE)
	}
#endif	//FLASH_SAVE_BANK_ENABLE
#endif	//SUPPORT_MIC_EQ

	altotoolPara.isEqReady[EQ_MIC]   = true;			
	altotoolPara.isEqReady[EQ_SPKER] = true;			

	eq_module_init();

}
#endif/*SUPPORT_DSP_EQ*/
