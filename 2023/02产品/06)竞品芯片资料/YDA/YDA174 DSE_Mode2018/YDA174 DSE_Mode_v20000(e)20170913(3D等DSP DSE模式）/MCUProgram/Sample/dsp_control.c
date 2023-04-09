/* 	CONFIDENTIAL
*********************************************************************************

	File:			dsp_control.c
	Version:		1.00.00
	Description:	DSP Control Processing

	Copyright(c) 2012 Yamaha Corporation

*********************************************************************************/
#include	"common.h"
#include	"main.h"
#include	"ioctrl.h"
#include	"dsp_access.h"
#include	"dsp_control.h"
#include	"dsp_coef_data.h"


/* **************************************************** */
/* Mute													*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			set mute.									*/
/* **************************************************** */
void Mute(void)
{
	MDSPCoefRTDL(0x403B, bSvMuteOn, 0x1);			/* SV.NMute[15:0]=0x0000 */
	
	/* wait for 20ms */
	LoopWait(20000);
	
	DSPPartWriteReg(0x10, 0x00, 0x07);				/* FMMUTEN=MDSPMUTEN=SDSPMUTEN=0 */
}

/* **************************************************** */
/* MuteRelease											*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			release mute.								*/
/* **************************************************** */
void MuteRelease(void)
{
	DSPPartWriteReg(0x10, 0x07, 0x07);				/* FMMUTEN=MDSPMUTEN=SDSPMUTEN=1 */
	
	MDSPCoefRTDL(0x403B, bSvMuteOff, 0x1);			/* SV.NMute[15:0]=0x4000 */
}

/* **************************************************** */
/* SetVolume											*/
/*		parameter:										*/
/*			unsigned char		bVolID					*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			set master volume.							*/
/* **************************************************** */
void SetVolume(UC bVolID)
{
	US wPresetNum;
	
	/* master volume setting */
	if(bVolID >= 1 && bVolID <= 0x85)
	{
		/* less than 100dB */
		if(bVolID <= 0xF)
		{
			wPresetNum = 0x03BA - (US)bVolID;
		}
		/* more than 99dB */
		else
		{
			wPresetNum = 0x03A9 - (US)( (bVolID - 0x10) * 2 );
		}
		
		DSPLoadPreset(wPresetNum, 0, FALSE);
	}
}

/* **************************************************** */
/* SetBalance											*/
/*		parameter:										*/
/*			signed char			bBalID					*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			control balance.							*/
/* **************************************************** */
void SetBalance(signed char bBalID)
{
	UC bNum;
	
	if(bBalID >= -33 && bBalID <= 33)
	{
		/* set LMT.Hi*Bal */
		bNum = bBalID + 33;
		MDSPCoefRTDL(0x4066, &bBalanceCoef[bNum][0], 0x2);
	}
}

/* **************************************************** */
/* SetCrossover											*/
/*		parameter:										*/
/*			unsigned char		bNetworkID				*/
/*														*/
/*			unsigned char		*pbData					*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			set digital network.						*/
/* **************************************************** */
void SetCrossover(UC bNetworkID, UC *pbData)
{
	US wPresetNum;
	
	/* digital network setting */
	switch(bNetworkID)
	{
		/* 1way */
		case 0x0:
			DSPWriteMem(0x41EC, bCf1Way, 0x4);
			MDSPCoefRTDL(0x4095, bLmtSDO1Off, 0x2);	/* LMT.*SDO1Out[15:0]=0x0000 */
			break;
			
		/* 2way */
		case 0x1:
			DSPWriteMem(0x41EB, bCf2Way, 0x5);
			MDSPCoefRTDL(0x4095, bLmtSDO1On, 0x2);	/* LMT.*SDO1Out[15:0]=0x4000 */
			break;

		/* 2.1ch */
		case 0x2:
			DSPWriteMem(0x41EB, bCf21ch, 0x5);
			MDSPCoefRTDL(0x4095, bLmtSDO1On, 0x2);	/* LMT.*SDO1Out[15:0]=0x4000 */
			break;
			
		default:
			break;
	}
	
	/* crossover filter setting */
	MDSPCoefRTDL(0x41C3, pbData,        0xA);		/* HPF.* */
	MDSPCoefRTDL(0x41CD, pbData + 0x14, 0xA);		/* HPF.* */
	MDSPCoefRTDL(0x41D7, pbData + 0x28, 0xA);		/* LPF.* */
	MDSPCoefRTDL(0x41E1, pbData + 0x3C, 0xA);		/* LPF.* */
}

/* **************************************************** */
/* SetS3D												*/
/*		parameter:										*/
/*			unsigned char		*pbData					*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			set Spacious sound 3D effect.				*/
/* **************************************************** */
void SetS3D(UC *pbData)
{
	MDSPCoefRTDL(0x409D, pbData,         0x9);		/* S3DCoreData0   - S3DCoreData8   */
	MDSPCoefRTDL(0x40A6, pbData + 0x012, 0xA);		/* S3DCoreData9   - S3DCoreData18  */
	MDSPCoefRTDL(0x40B0, pbData + 0x026, 0xA);		/* S3DCoreData19  - S3DCoreData28  */
	MDSPCoefRTDL(0x40BA, pbData + 0x03A, 0xA);		/* S3DCoreData29  - S3DCoreData38  */
	MDSPCoefRTDL(0x40C4, pbData + 0x04E, 0xA);		/* S3DCoreData39  - S3DCoreData48  */
	MDSPCoefRTDL(0x40CE, pbData + 0x062, 0xA);		/* S3DCoreData49  - S3DCoreData58  */
	MDSPCoefRTDL(0x40D8, pbData + 0x076, 0xA);		/* S3DCoreData59  - S3DCoreData68  */
	MDSPCoefRTDL(0x40E2, pbData + 0x08A, 0xA);		/* S3DCoreData69  - S3DCoreData78  */
	MDSPCoefRTDL(0x40EC, pbData + 0x09E, 0xA);		/* S3DCoreData79  - S3DCoreData88  */
	MDSPCoefRTDL(0x40F6, pbData + 0x0B2, 0xA);		/* S3DCoreData89  - S3DCoreData98  */
	MDSPCoefRTDL(0x4100, pbData + 0x0C6, 0xA);		/* S3DCoreData99  - S3DCoreData108 */
	MDSPCoefRTDL(0x410A, pbData + 0x0DA, 0xA);		/* S3DCoreData109 - S3DCoreData118 */
	MDSPCoefRTDL(0x4114, pbData + 0x0EE, 0xA);		/* S3DCoreData119 - S3DCoreData128 */
	MDSPCoefRTDL(0x411E, pbData + 0x102, 0x8);		/* S3DCoreData129 - S3DCoreData136 */
	MDSPCoefRTDL(0x4126, pbData + 0x112, 0x5);		/* EQ0 */
	MDSPCoefRTDL(0x412B, pbData + 0x11C, 0x5);		/* EQ1 */
	MDSPCoefRTDL(0x4130, pbData + 0x126, 0x5);		/* EQ2 */
	MDSPCoefRTDL(0x4135, pbData + 0x130, 0x1);		/* S3DLev */
	
	S3DOn(TRUE);
}

/* **************************************************** */
/* S3DOn												*/
/*		parameter:										*/
/*			bool				bOn						*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			enable/disable Spacious sound 3D effect.	*/
/* **************************************************** */
void S3DOn(bool bOn)
{
	if(bOn)
	{
		MDSPCoefRTDL(0x4136, bS3DOn, 0x2);			/* ThruSel[15:0]=0x0000, S3DSel[15:0]=0x4000 */
	}
	else
	{
		MDSPCoefRTDL(0x4136, bS3DOff, 0x2);			/* ThruSel[15:0]=0x4000, S3DSel[15:0]=0x0000 */
	}
}

/* **************************************************** */
/* SetHXT												*/
/*		parameter:										*/
/*			unsigned char		*pbData					*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			set Harmonics enhancer effect.				*/
/* **************************************************** */
void SetHXT(UC *pbData)
{
	MDSPCoefRTDL(0x4149, pbData,        0xA);		/* HXTCoreData0  - HXTCoreData9 */
	MDSPCoefRTDL(0x4153, pbData + 0x14, 0xA);		/* HXTCoreData10 - HXTCoreData19 */
	MDSPCoefRTDL(0x415D, pbData + 0x28, 0xA);		/* HXTCoreData20 - HXTCoreData29 */
	MDSPCoefRTDL(0x4167, pbData + 0x3C, 0xA);		/* HXTCoreData30 - HXTCoreData39 */
	MDSPCoefRTDL(0x4171, pbData + 0x50, 0xA);		/* HXTCoreData40 - HXTCoreData49 */
	MDSPCoefRTDL(0x417B, pbData + 0x64, 0xA);		/* HXTCoreData50 - HXTCoreData59 */
	MDSPCoefRTDL(0x4185, pbData + 0x78, 0xA);		/* HXTCoreData60 - HXTCoreData69 */
	MDSPCoefRTDL(0x418F, pbData + 0x8C, 0x5);		/* HXTCoreData70 - HXTCoreData74 */
	MDSPCoefRTDL(0x4194, pbData + 0x96, 0x2);		/* DryMix, WetMix */
	
	HXTOn(TRUE);
}

/* **************************************************** */
/* HXTOn												*/
/*		parameter:										*/
/*			bool				bOn						*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			enable/disable Harmonics enhancer effect.	*/
/* **************************************************** */
void HXTOn(bool bOn)
{
	if(bOn)
	{
		MDSPCoefRTDL(0x4196, bHXTOn, 0x2);			/* ThruSel[15:0]=0x0000, HXTSel[15:0]=0x4000 */
	}
	else
	{
		MDSPCoefRTDL(0x4196, bHXTOff, 0x2);			/* ThruSel[15:0]=0x4000, HXTSel[15:0]=0x0000 */
	}
}

/* **************************************************** */
/* SetAEQ											*/
/*		parameter:										*/
/*			unsigned char		*pbData0				*/
/*														*/
/*			unsigned char		*pbData1				*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			set AEQ effect.							*/
/* **************************************************** */
void SetAEQ(UC *pbData0, UC *pbData1)
{
	MDSPCoefRTDL(0x41F3, pbData0,         0xA);		/* AEQCoreData0   - AEQCoreData9   */
	MDSPCoefRTDL(0x41FD, pbData0 + 0x014, 0xA);		/* AEQCoreData10  - AEQCoreData19  */
	MDSPCoefRTDL(0x4207, pbData0 + 0x028, 0xA);		/* AEQCoreData20  - AEQCoreData29  */
	MDSPCoefRTDL(0x4211, pbData0 + 0x03C, 0xA);		/* AEQCoreData30  - AEQCoreData39  */
	MDSPCoefRTDL(0x421B, pbData0 + 0x050, 0xA);		/* AEQCoreData40  - AEQCoreData49  */
	MDSPCoefRTDL(0x4225, pbData0 + 0x064, 0xA);		/* AEQCoreData50  - AEQCoreData59  */
	MDSPCoefRTDL(0x422F, pbData0 + 0x078, 0xA);		/* AEQCoreData60  - AEQCoreData69  */
	MDSPCoefRTDL(0x4239, pbData0 + 0x08C, 0xA);		/* AEQCoreData70  - AEQCoreData79  */
	MDSPCoefRTDL(0x4243, pbData0 + 0x0A0, 0xA);		/* AEQCoreData80  - AEQCoreData89  */
	MDSPCoefRTDL(0x424D, pbData0 + 0x0B4, 0xA);		/* AEQCoreData90  - AEQCoreData99  */
	MDSPCoefRTDL(0x4257, pbData0 + 0x0C8, 0xA);		/* AEQCoreData100 - AEQCoreData109 */
	MDSPCoefRTDL(0x4261, pbData0 + 0x0DC, 0xA);		/* AEQCoreData110 - AEQCoreData119 */
	MDSPCoefRTDL(0x426B, pbData0 + 0x0F0, 0xA);		/* AEQCoreData120 - AEQCoreData129 */
	MDSPCoefRTDL(0x4275, pbData0 + 0x104, 0xA);		/* AEQCoreData130 - AEQCoreData139 */
	MDSPCoefRTDL(0x427F, pbData0 + 0x118, 0xA);		/* AEQCoreData140 - AEQCoreData149 */
	MDSPCoefRTDL(0x4289, pbData0 + 0x12C, 0xA);		/* AEQCoreData150 - AEQCoreData159 */
	MDSPCoefRTDL(0x4293, pbData0 + 0x140, 0xA);		/* AEQCoreData160 - AEQCoreData169 */
	MDSPCoefRTDL(0x429D, pbData0 + 0x154, 0xA);		/* AEQCoreData170 - AEQCoreData179 */
	MDSPCoefRTDL(0x42A7, pbData0 + 0x168, 0xA);		/* AEQCoreData180 - AEQCoreData189 */
	MDSPCoefRTDL(0x42B1, pbData0 + 0x17C, 0xA);		/* AEQCoreData190 - AEQCoreData199 */
	MDSPCoefRTDL(0x42BB, pbData0 + 0x190, 0xA);		/* AEQCoreData200 - AEQCoreData209 */
	MDSPCoefRTDL(0x42C5, pbData0 + 0x1A4, 0xA);		/* AEQCoreData210 - AEQCoreData219 */
	MDSPCoefRTDL(0x42CF, pbData0 + 0x1B8, 0xA);		/* AEQCoreData220 - AEQCoreData229 */
	MDSPCoefRTDL(0x42D9, pbData0 + 0x1CC, 0xA);		/* AEQCoreData230 - AEQCoreData239 */
	MDSPCoefRTDL(0x42E3, pbData0 + 0x1E0, 0xA);		/* AEQCoreData240 - AEQCoreData249 */
	MDSPCoefRTDL(0x42ED, pbData0 + 0x1F4, 0xA);		/* AEQCoreData250 - AEQCoreData259 */
	MDSPCoefRTDL(0x42F7, pbData0 + 0x208, 0xA);		/* AEQCoreData260 - AEQCoreData269 */
	MDSPCoefRTDL(0x4301, pbData0 + 0x21C, 0xA);		/* AEQCoreData270 - AEQCoreData279 */
	MDSPCoefRTDL(0x430B, pbData0 + 0x230, 0xA);		/* AEQCoreData280 - AEQCoreData289 */
	MDSPCoefRTDL(0x4315, pbData0 + 0x244, 0xA);		/* AEQCoreData290 - AEQCoreData299 */
	MDSPCoefRTDL(0x431F, pbData0 + 0x258, 0xA);		/* AEQCoreData300 - AEQCoreData309 */
	MDSPCoefRTDL(0x4329, pbData0 + 0x26C, 0xA);		/* AEQCoreData310 - AEQCoreData319 */
	MDSPCoefRTDL(0x4333, pbData0 + 0x280, 0xA);		/* AEQCoreData320 - AEQCoreData329 */
	MDSPCoefRTDL(0x433D, pbData0 + 0x294, 0xA);		/* AEQCoreData330 - AEQCoreData339 */
	MDSPCoefRTDL(0x4347, pbData0 + 0x2A8, 0xA);		/* AEQCoreData340 - AEQCoreData349 */
	MDSPCoefRTDL(0x4351, pbData0 + 0x2BC, 0xA);		/* AEQCoreData350 - AEQCoreData359 */
	MDSPCoefRTDL(0x435B, pbData0 + 0x2D0, 0xA);		/* AEQCoreData360 - AEQCoreData369 */
	MDSPCoefRTDL(0x4365, pbData0 + 0x2E4, 0xA);		/* AEQCoreData370 - AEQCoreData379 */
	MDSPCoefRTDL(0x436F, pbData0 + 0x2F8, 0xA);		/* AEQCoreData380 - AEQCoreData389 */
	MDSPCoefRTDL(0x4379, pbData0 + 0x30C, 0xA);		/* AEQCoreData390 - AEQCoreData399 */
	MDSPCoefRTDL(0x4383, pbData0 + 0x320, 0xA);		/* AEQCoreData400 - AEQCoreData409 */
	MDSPCoefRTDL(0x438D, pbData0 + 0x334, 0xA);		/* AEQCoreData410 - AEQCoreData419 */
	MDSPCoefRTDL(0x4397, pbData0 + 0x348, 0xA);		/* AEQCoreData420 - AEQCoreData429 */
	MDSPCoefRTDL(0x43A1, pbData0 + 0x35C, 0xA);		/* AEQCoreData430 - AEQCoreData439 */
	MDSPCoefRTDL(0x43AB, pbData0 + 0x370, 0xA);		/* AEQCoreData440 - AEQCoreData449 */
	MDSPCoefRTDL(0x43B5, pbData0 + 0x384, 0xA);		/* AEQCoreData450 - AEQCoreData459 */
	MDSPCoefRTDL(0x43BF, pbData0 + 0x398, 0xA);		/* AEQCoreData460 - AEQCoreData469 */
	MDSPCoefRTDL(0x43C9, pbData0 + 0x3AC, 0xA);		/* AEQCoreData470 - AEQCoreData479 */
	MDSPCoefRTDL(0x43D3, pbData0 + 0x3C0, 0xA);		/* AEQCoreData480 - AEQCoreData489 */
	MDSPCoefRTDL(0x43DD, pbData0 + 0x3D4, 0xA);		/* AEQCoreData490 - AEQCoreData499 */
	MDSPCoefRTDL(0x43E7, pbData0 + 0x3E8, 0xA);		/* AEQCoreData500 - AEQCoreData509 */
	MDSPCoefRTDL(0x43F1, pbData0 + 0x3FC, 0x4);		/* AEQCoreData510 - AEQCoreData513 */
	MDSPCoefRTDL(0x43F5, pbData0 + 0x404, 0x2);		/* *PostLev */
	
	SDSPCoefBT(0x8000, pbData1,           0x5);		/* SDSP 10bands PEQ PEQ0.* */
	SDSPCoefBT(0x8005, pbData1 + 0xF,     0x5);		/* SDSP 10bands PEQ PEQ1.* */
	SDSPCoefBT(0x800A, pbData1 + 0x1E,    0x5);		/* SDSP 10bands PEQ PEQ2.* */
	SDSPCoefBT(0x800F, pbData1 + 0x2D,    0x5);		/* SDSP 10bands PEQ PEQ3.* */
	SDSPCoefBT(0x8014, pbData1 + 0x3C,    0x5);		/* SDSP 10bands PEQ PEQ4.* */
	SDSPCoefBT(0x8019, pbData1 + 0x4B,    0x5);		/* SDSP 10bands PEQ PEQ5.* */
	SDSPCoefBT(0x801E, pbData1 + 0x5A,    0xA);		/* SDSP 10bands PEQ PEQ6.* */
	SDSPCoefBT(0x8028, pbData1 + 0x78,    0xA);		/* SDSP 10bands PEQ PEQ7.* */
	SDSPCoefBT(0x8032, pbData1 + 0x96,    0xA);		/* SDSP 10bands PEQ PEQ8.* */
	SDSPCoefBT(0x803C, pbData1 + 0xB4,    0xA);		/* SDSP 10bands PEQ PEQ9.* */
	
	AEQOn(TRUE);
}

/* **************************************************** */
/* AEQOn												*/
/*		parameter:										*/
/*			bool				bOn						*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			enable/disable AEQ effect.				*/
/* **************************************************** */
void AEQOn(bool bOn)
{
	if(bOn)
	{
		MDSPCoefRTDL(0x43F7, bAcOn, 0x2);			/* AC.ThruSel[15:0]=0x0000, AC.ACSel[15:0]=0x4000 */
		MDSPCoefRTDL(0x4042, b10bPEQOn, 0x2);		/* SV.ThruSel[15:0]=0x0000, SV.10bPEQSel[15:0]=0x4000 */
	}
	else
	{
		MDSPCoefRTDL(0x43F7, bAcOff, 0x2);			/* AC.ThruSel[15:0]=0x4000, AC.ACSel[15:0]=0x0000 */
		MDSPCoefRTDL(0x4042, b10bPEQOff, 0x2);		/* SV.ThruSel[15:0]=0x4000, SV.10bPEQSel[15:0]=0x0000 */
	}
}

/* **************************************************** */
/* PresetLoudnessSetting								*/
/*		parameter:										*/
/*			unsigned char		bEffectID				*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			set Loudness effect.						*/
/* **************************************************** */
void PresetLoudnessSetting(UC bEffectID)
{
	US wPresetNum;
	
	/* Loudness setting */
	wPresetNum = 0x02B6 + (US)bEffectID;
	DSPLoadPreset(wPresetNum, 0, FALSE);
}

/* **************************************************** */
/* UserLoudnessSetting									*/
/*		parameter:										*/
/*			unsigned char		*pbData					*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			set Loudness effect.						*/
/* **************************************************** */
void UserLoudnessSetting(UC *pbData)
{
	/* Loudness setting */
	MDSPCoefRTDL(0x4044, pbData,        0xA);		/* EQ0.*, EQ1.* */
	MDSPCoefRTDL(0x404E, pbData + 0x14, 0x2);		/* LnRatio, LnShift */
	
	LoudnessOn(TRUE);
}

/* **************************************************** */
/* LoudnessOn											*/
/*		parameter:										*/
/*			bool				bOn						*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			enable/disable Loudness effect.				*/
/* **************************************************** */
void LoudnessOn(bool bOn)
{
	if(bOn)
	{
		MDSPCoefRTDL(0x4050, bLnOn, 0x2);			/* LnSel[15:0]=0x4000, LnThruSel[15:0]=0x0000 */
	}
	else
	{
		MDSPCoefRTDL(0x4050, bLnOff, 0x2);			/* LnSel[15:0]=0x0000, LnThruSel[15:0]=0x4000 */
	}
}

/* **************************************************** */
/* SetPowerLimit										*/
/*		parameter:										*/
/*			unsigned char		*pbData					*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			set Power limit.							*/
/* **************************************************** */
void SetPowerLimit(UC *pbData)
{
	MDSPCoefRTDL(0x406A, pbData,        0x6);		/* HGC.*  */
	MDSPCoefRTDL(0x4070, pbData + 0x0C, 0x6);		/* HATK.* */
	MDSPCoefRTDL(0x4076, pbData + 0x18, 0x5);		/* LGC.*  */
	MDSPCoefRTDL(0x407B, pbData + 0x22, 0x6);		/* LATK.* */
	
	PowerLimitOn(TRUE);
}

/* **************************************************** */
/* PowerLimitOn											*/
/*		parameter:										*/
/*			bool				bOn						*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			enable/disable Power limit.					*/
/* **************************************************** */
void PowerLimitOn(bool bOn)
{
	if(bOn)
	{
		MDSPCoefRTDL(0x4081, bLmtOn, 0x4);			/* *GCThruSel[15:0]=0x0000, *GCSel[15:0]=0x4000 */
	}
	else
	{
		MDSPCoefRTDL(0x4081, bLmtOff, 0x4);			/* *GCThruSel[15:0]=0x4000, *GCSel[15:0]=0x0000 */
	}
}

/* **************************************************** */
/* SetSoftClip											*/
/*		parameter:										*/
/*			unsigned char		*pbData					*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			set SoftClip.								*/
/* **************************************************** */
void SetSoftClip(UC *pbData)
{
	MDSPCoefRTDL(0x4089, pbData, 0x3);				/* PreLev, PostLev, CompLev */

	SoftClipOn(TRUE);
}

/* **************************************************** */
/* SoftClipOn											*/
/*		parameter:										*/
/*			bool				bOn						*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			enable/disable SoftClip.					*/
/* **************************************************** */
void SoftClipOn(bool bOn)
{
	if(bOn)
	{
		MDSPCoefRTDL(0x408C, bLmtSCOn, 0x2);		/* SCThruSel[15:0]=0x0000, SCSel[15:0]=0x4000 */
	}
	else
	{
		MDSPCoefRTDL(0x408C, bLmtSCOff, 0x2);		/* SCThruSel[15:0]=0x4000, SCSel[15:0]=0x0000 */
	}
}

/* **************************************************** */
/* SmallSignalDetectionOn								*/
/*		parameter:										*/
/*			unsigned char		*pbData					*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			enable small signal detection.				*/
/* **************************************************** */
void SmallSignalDetectionOn(UC *pbData)
{
	MDSPCoefRTDL(0x400E, pbData, 0x3);				/* DetLev, DetTime, DetRst */
}

/* **************************************************** */
/* SmallSignalDetectionOff								*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			disable small signal detection.				*/
/* **************************************************** */
void SmallSignalDetectionOff(void)
{
	MDSPCoefRTDL(0x400E, bIldOff, 0x3);				/* DetLev, DetTime, DetRst */
}

/* **************************************************** */
/* PlayFM												*/
/*		parameter:										*/
/*			unsigned char		bContentID				*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			play FM synthesizer.						*/
/* **************************************************** */
void PlayFM(UC bContentID)
{
	US wPresetNum;
	
	if(bContentID <= 0x39)
	{
		wPresetNum = 0x0020 + (US)bContentID;
		DSPLoadPreset(wPresetNum, 0, FALSE);
	}
}

/* end of file */
