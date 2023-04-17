<<<<<<< HEAD
/* 	CONFIDENTIAL
*********************************************************************************

	File:			init.c
	Version:		1.00.00
	Description:	Device Initialization Processing

	Copyright(c) 2012 Yamaha Corporation

*********************************************************************************/
#include	"common.h"
#include	"init.h"
#include	"dsp_access.h"
#include	"dsp_control.h"
#include	"dsp_inst_data.h"
#include	"dsp_coef_data.h"


/* ******************************************************** */
/* Wait														*/
/*		parameter:											*/
/*			unsigned long		dwWait						*/
/*								(time)						*/
/*															*/
/*		return value:										*/
/*			none											*/
/*															*/
/*		description:										*/
/*			wait for a specified time. (in microseconds)	*/
/* ******************************************************** */
void Wait(UL dwWait)
{
	/*
		This function depends on the device.
		Please write the program code which realizes the following.
		
		- Wait for "dwWait" time (ms).
	*/
}

/* **************************************************** */
/* DD2SPSetDSEMode										*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			initializes DSP.							*/
/*			Firmware Mode : DSE mode					*/
/* **************************************************** */
void DD2SPSetDSEMode(void)
{
	/* wait for 10ms or more */
	Wait(DSP_PLL_WAIT);
	
	/* reset cancel */
	/* Note:
	    DD2_ICN, DD2_SLEEPN and DD2_MUTEN are CPU's GPIO port. 
	    DD2_ICN is connected with DD-2SP's IC_N pin.
	    DD2_DD2_SLEEPN is connected with DD-2SP's SLEEP_N pin.
	    DD2_MUTEN is connected with DD-2SP's MUTE_N pin.
	*/
	DD2_ICN    = HIGH;
	
	/* wait for 1ms or more */
	Wait(DSP_ICN_WAIT);
	
	DD2_SLEEPN = HIGH;
	DD2_MUTEN  = HIGH;
	
	/* set firmware mode */
	DSPWriteMem(0x4400, bInstData, 0x5FF);
	
	/* set Top firmware coefficients */
	DSPWriteMem(0x4000, bTopData, 0x6);
	
	/* set ILD firmware coefficients */
	DSPWriteMem(0x400B, bIldSDI0Sel, 0x2);
	DSPWriteMem(0x400D, bIldAtt, 0x1);
	DSPWriteMem(0x400E, bIldOff, 0x3);
	
	/* set S3D firmware coefficients */
	DSPWriteMem(0x409D, bS3DNatural, 0x99);
	DSPWriteMem(0x4136, bS3DOff, 0x2);
	
	/* set HXT firmware coefficients */
	DSPWriteMem(0x4149, bHarmonicsExpander, 0x4D);
	DSPWriteMem(0x4196, bHXTOff, 0x2);
	
	/* set AC firmware coefficients */
	DSPWriteMem(0x41F2, bAcAtt, 0x1);
	DSPWriteMem(0x41F3, bAEQCore, 0x204);
	DSPWriteMem(0x43F7, bAcOff, 0x2);
	
	/* set SV firmware coefficients */
	DSPWriteMem(0x4034, bSvAtt, 0x2);
	DSPWriteMem(0x4036, bSvFMMix, 0x2);
	DSPWriteMem(0x403B, bSvMuteOn, 0x1);
	DSPWriteMem(0x4042, b10bPEQOff, 0x2);
	DSPWriteMem(0x4044, bLnDEQ0High, 0xC);
	DSPWriteMem(0x4050, bLnOff, 0x2);
	
	/* set CF firmware coefficients */
	DSPWriteMem(0x41C3, bCf280Hz, 0x28);
	DSPWriteMem(0x41EB, bCf21ch, 0x5);
	
	/* set LMT firmware coefficients */
	DSPWriteMem(0x4066, &bBalanceCoef[33][0], 0x2);
	DSPWriteMem(0x4068, &bBalanceCoef[33][0], 0x2);
	DSPWriteMem(0x406A, bPowerLimit, 0x17);
	DSPWriteMem(0x4081, bLmtOff, 0x4);
	DSPWriteMem(0x4085, bLmtLevAdjDD2SP, 0x4);
	DSPWriteMem(0x4089, bSoftcClipper, 0x3);
	DSPWriteMem(0x408C, bLmtSCOff, 0x2);
	DSPWriteMem(0x408E, bLmtLoMixOff, 0x1);
	DSPWriteMem(0x408F, bLmtSMix, 0x2);
	DSPWriteMem(0x4091, bLmt21ch, 0x6);
	
	/* set SDSP 10band PEQ coefficients */
	DSPWriteMem(0x8000, bAEQPEQ, 0x46);
	
	/* set registers (1) */
	DSPPartWriteReg(0x07, 0x20, 0x3F);	/* PI=1, PIO=0, AIFMD[3:0]=0x0 */
	DSPPartWriteReg(0x0A, 0x02, 0x03);	/* WRAMRTMD=1, WRAMMD=0 */
	DSPPartWriteReg(0x2F, 0x01, 0x03);	/* GAIN[1:0]=01 */
	
	/* set registers (2) */
	/* Note:
		As example, set DD-2SP input/output audio data format to I2S data format.
	*/
	DSPPartWriteReg(0x08, 0x23, 0xFF);	/* SDIFMT[1:0]=10, SDIBIT[1:0]=00, SDIWCKP=1, SDIBCKP=1 */
	DSPPartWriteReg(0x09, 0x23, 0xFF);	/* SDOFMT[1:0]=10, SDOBIT[1:0]=00, SDOWCKP=1, SDOBCKP=1 */
	DSPPartWriteReg(0x0B, 0x00, 0x07);	/* DCCOFF=DEMON=DCM=0 */
	DSPPartWriteReg(0x0C, 0x00, 0x04);	/* PWMFHOP=0 */
	DSPPartWriteReg(0x0D, 0x01, 0x01);	/* EXIRQN=1 */
	DSPPartWriteReg(0x0E, 0x00, 0x03);	/* FSM[1:0]=00 */
	DSPPartWriteReg(0x23, 0x04, 0x04);	/* PLLERRE=1 */
	
	/* start DSP */
	DSPPartWriteReg(0x11, 0x03, 0x03);	/* MDSPSTART=SDSPSTART=1 */
	
	/* load preset patterns */
	DSPLoadPreset(0x02B4, 0, FALSE);	/* [692:SV Curve Mid] */
	DSPLoadPreset(0x031F, 0, FALSE);	/* [799:SV -30.0dB] */
	
	/* wait for 107ms or more */
	Wait(DSP_AMP_WAIT);
	
	/* release mute (1) */
	DSPPartWriteReg(0x10, 0x0F, 0x0F);	/* QMUTEN=FMMUTEN=MDSPMUTEN=SDSPMUTEN=1 */
	
	/* release mute (2) */
	MDSPCoefRTDL(0x403B, bSvMuteOff, 0x1);
}

/* **************************************************** */
/* SPR2SetDSEMode										*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			initializes DSP.							*/
/*			Firmware Mode : DSE mode					*/
/* **************************************************** */
void SPR2SetDSEMode(void)
{
	/* wait for 10ms or more */
	Wait(DSP_PLL_WAIT);
	
	/* reset cancel */
	/* Note:
	    DD2_ICN is CPU's GPIO port. 
	    DD2_ICN is connected with DD-2SP's IC_N pin.
	*/
	DD2_ICN    = HIGH;
	
	/* set firmware mode */
	DSPWriteMem(0x4400, bInstData, 0x5FF);
	
	/* set Top firmware coefficients */
	DSPWriteMem(0x4000, bTopData, 0x6);
	
	/* set ILD firmware coefficients */
	DSPWriteMem(0x400B, bIldSDI0Sel, 0x2);
	DSPWriteMem(0x400D, bIldAtt, 0x1);
	DSPWriteMem(0x400E, bIldOff, 0x3);
	
	/* set S3D firmware coefficients */
	DSPWriteMem(0x409D, bS3DNatural, 0x99);
	DSPWriteMem(0x4136, bS3DOff, 0x2);
	
	/* set HXT firmware coefficients */
	DSPWriteMem(0x4149, bHarmonicsExpander, 0x4D);
	DSPWriteMem(0x4196, bHXTOff, 0x2);
	
	/* set AC firmware coefficients */
	DSPWriteMem(0x41F2, bAcAtt, 0x1);
	DSPWriteMem(0x41F3, bAEQCore, 0x204);
	DSPWriteMem(0x43F7, bAcOff, 0x2);
	
	/* set SV firmware coefficients */
	DSPWriteMem(0x4034, bSvAtt, 0x2);
	DSPWriteMem(0x4036, bSvFMMix, 0x2);
	DSPWriteMem(0x403B, bSvMuteOn, 0x1);
	DSPWriteMem(0x4042, b10bPEQOff, 0x2);
	DSPWriteMem(0x4044, bLnDEQ0High, 0xC);
	DSPWriteMem(0x4050, bLnOff, 0x2);
	
	/* set CF firmware coefficients */
	DSPWriteMem(0x41C3, bCf280Hz, 0x28);
	DSPWriteMem(0x41EB, bCf21ch, 0x5);
	
	/* set LMT firmware coefficients */
	DSPWriteMem(0x4066, &bBalanceCoef[33][0], 0x2);
	DSPWriteMem(0x4068, &bBalanceCoef[33][0], 0x2);
	DSPWriteMem(0x406A, bPowerLimit, 0x17);
	DSPWriteMem(0x4081, bLmtOff, 0x4);
	DSPWriteMem(0x4085, bLmtLevAdjSPR2, 0x4);
	DSPWriteMem(0x4089, bSoftcClipper, 0x3);
	DSPWriteMem(0x408C, bLmtSCOff, 0x2);
	DSPWriteMem(0x408E, bLmtLoMixOff, 0x1);
	DSPWriteMem(0x408F, bLmtSMix, 0x2);
	DSPWriteMem(0x4091, bLmt21ch, 0x6);
	
	/* set SDSP 10band PEQ coefficients */
	DSPWriteMem(0x8000, bAEQPEQ, 0x46);
	
	/* set registers (1) */
	DSPPartWriteReg(0x07, 0x20, 0x3F);	/* PI=1, PIO=0, AIFMD[3:0]=0x0 */
	DSPPartWriteReg(0x0A, 0x02, 0x03);	/* WRAMRTMD=1, WRAMMD=0 */
	
	/* set registers (2) */
	/* Note:
		As example, set SPR-2 input/output audio data format to I2S data format.
	*/
	DSPPartWriteReg(0x08, 0x23, 0xFF);	/* SDIFMT[1:0]=10, SDIBIT[1:0]=00, SDIWCKP=1, SDIBCKP=1 */
	DSPPartWriteReg(0x09, 0x23, 0xFF);	/* SDOFMT[1:0]=10, SDOBIT[1:0]=00, SDOWCKP=1, SDOBCKP=1 */
	DSPPartWriteReg(0x0B, 0x00, 0x03);	/* DEMON=DCM=0 */
	DSPPartWriteReg(0x0D, 0x01, 0x01);	/* EXIRQN=1 */
	DSPPartWriteReg(0x0E, 0x00, 0x03);	/* FSM[1:0]=00 */
	DSPPartWriteReg(0x23, 0x04, 0x04);	/* PLLERRE=1 */
	
	/* start DSP */
	DSPPartWriteReg(0x11, 0x03, 0x03);	/* MDSPSTART=SDSPSTART=1 */
	
	/* load preset patterns */
	DSPLoadPreset(0x02B4, 0, FALSE);	/* [692:SV Curve Mid] */
	DSPLoadPreset(0x02FB, 0, FALSE);	/* [763:SV -12.0dB] */
	
	/* release mute (1) */
	DSPPartWriteReg(0x10, 0x07, 0x07);	/* FMMUTEN=MDSPMUTEN=SDSPMUTEN=1 */
	
	/* release mute (2) */
	MDSPCoefRTDL(0x403B, bSvMuteOff, 0x1);
}

/* end of file */
=======
/* 	CONFIDENTIAL
*********************************************************************************

	File:			init.c
	Version:		1.00.00
	Description:	Device Initialization Processing

	Copyright(c) 2012 Yamaha Corporation

*********************************************************************************/
#include	"common.h"
#include	"init.h"
#include	"dsp_access.h"
#include	"dsp_control.h"
#include	"dsp_inst_data.h"
#include	"dsp_coef_data.h"


/* ******************************************************** */
/* Wait														*/
/*		parameter:											*/
/*			unsigned long		dwWait						*/
/*								(time)						*/
/*															*/
/*		return value:										*/
/*			none											*/
/*															*/
/*		description:										*/
/*			wait for a specified time. (in microseconds)	*/
/* ******************************************************** */
void Wait(UL dwWait)
{
	/*
		This function depends on the device.
		Please write the program code which realizes the following.
		
		- Wait for "dwWait" time (ms).
	*/
}

/* **************************************************** */
/* DD2SPSetDSEMode										*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			initializes DSP.							*/
/*			Firmware Mode : DSE mode					*/
/* **************************************************** */
void DD2SPSetDSEMode(void)
{
	/* wait for 10ms or more */
	Wait(DSP_PLL_WAIT);
	
	/* reset cancel */
	/* Note:
	    DD2_ICN, DD2_SLEEPN and DD2_MUTEN are CPU's GPIO port. 
	    DD2_ICN is connected with DD-2SP's IC_N pin.
	    DD2_DD2_SLEEPN is connected with DD-2SP's SLEEP_N pin.
	    DD2_MUTEN is connected with DD-2SP's MUTE_N pin.
	*/
	DD2_ICN    = HIGH;
	
	/* wait for 1ms or more */
	Wait(DSP_ICN_WAIT);
	
	DD2_SLEEPN = HIGH;
	DD2_MUTEN  = HIGH;
	
	/* set firmware mode */
	DSPWriteMem(0x4400, bInstData, 0x5FF);
	
	/* set Top firmware coefficients */
	DSPWriteMem(0x4000, bTopData, 0x6);
	
	/* set ILD firmware coefficients */
	DSPWriteMem(0x400B, bIldSDI0Sel, 0x2);
	DSPWriteMem(0x400D, bIldAtt, 0x1);
	DSPWriteMem(0x400E, bIldOff, 0x3);
	
	/* set S3D firmware coefficients */
	DSPWriteMem(0x409D, bS3DNatural, 0x99);
	DSPWriteMem(0x4136, bS3DOff, 0x2);
	
	/* set HXT firmware coefficients */
	DSPWriteMem(0x4149, bHarmonicsExpander, 0x4D);
	DSPWriteMem(0x4196, bHXTOff, 0x2);
	
	/* set AC firmware coefficients */
	DSPWriteMem(0x41F2, bAcAtt, 0x1);
	DSPWriteMem(0x41F3, bAEQCore, 0x204);
	DSPWriteMem(0x43F7, bAcOff, 0x2);
	
	/* set SV firmware coefficients */
	DSPWriteMem(0x4034, bSvAtt, 0x2);
	DSPWriteMem(0x4036, bSvFMMix, 0x2);
	DSPWriteMem(0x403B, bSvMuteOn, 0x1);
	DSPWriteMem(0x4042, b10bPEQOff, 0x2);
	DSPWriteMem(0x4044, bLnDEQ0High, 0xC);
	DSPWriteMem(0x4050, bLnOff, 0x2);
	
	/* set CF firmware coefficients */
	DSPWriteMem(0x41C3, bCf280Hz, 0x28);
	DSPWriteMem(0x41EB, bCf21ch, 0x5);
	
	/* set LMT firmware coefficients */
	DSPWriteMem(0x4066, &bBalanceCoef[33][0], 0x2);
	DSPWriteMem(0x4068, &bBalanceCoef[33][0], 0x2);
	DSPWriteMem(0x406A, bPowerLimit, 0x17);
	DSPWriteMem(0x4081, bLmtOff, 0x4);
	DSPWriteMem(0x4085, bLmtLevAdjDD2SP, 0x4);
	DSPWriteMem(0x4089, bSoftcClipper, 0x3);
	DSPWriteMem(0x408C, bLmtSCOff, 0x2);
	DSPWriteMem(0x408E, bLmtLoMixOff, 0x1);
	DSPWriteMem(0x408F, bLmtSMix, 0x2);
	DSPWriteMem(0x4091, bLmt21ch, 0x6);
	
	/* set SDSP 10band PEQ coefficients */
	DSPWriteMem(0x8000, bAEQPEQ, 0x46);
	
	/* set registers (1) */
	DSPPartWriteReg(0x07, 0x20, 0x3F);	/* PI=1, PIO=0, AIFMD[3:0]=0x0 */
	DSPPartWriteReg(0x0A, 0x02, 0x03);	/* WRAMRTMD=1, WRAMMD=0 */
	DSPPartWriteReg(0x2F, 0x01, 0x03);	/* GAIN[1:0]=01 */
	
	/* set registers (2) */
	/* Note:
		As example, set DD-2SP input/output audio data format to I2S data format.
	*/
	DSPPartWriteReg(0x08, 0x23, 0xFF);	/* SDIFMT[1:0]=10, SDIBIT[1:0]=00, SDIWCKP=1, SDIBCKP=1 */
	DSPPartWriteReg(0x09, 0x23, 0xFF);	/* SDOFMT[1:0]=10, SDOBIT[1:0]=00, SDOWCKP=1, SDOBCKP=1 */
	DSPPartWriteReg(0x0B, 0x00, 0x07);	/* DCCOFF=DEMON=DCM=0 */
	DSPPartWriteReg(0x0C, 0x00, 0x04);	/* PWMFHOP=0 */
	DSPPartWriteReg(0x0D, 0x01, 0x01);	/* EXIRQN=1 */
	DSPPartWriteReg(0x0E, 0x00, 0x03);	/* FSM[1:0]=00 */
	DSPPartWriteReg(0x23, 0x04, 0x04);	/* PLLERRE=1 */
	
	/* start DSP */
	DSPPartWriteReg(0x11, 0x03, 0x03);	/* MDSPSTART=SDSPSTART=1 */
	
	/* load preset patterns */
	DSPLoadPreset(0x02B4, 0, FALSE);	/* [692:SV Curve Mid] */
	DSPLoadPreset(0x031F, 0, FALSE);	/* [799:SV -30.0dB] */
	
	/* wait for 107ms or more */
	Wait(DSP_AMP_WAIT);
	
	/* release mute (1) */
	DSPPartWriteReg(0x10, 0x0F, 0x0F);	/* QMUTEN=FMMUTEN=MDSPMUTEN=SDSPMUTEN=1 */
	
	/* release mute (2) */
	MDSPCoefRTDL(0x403B, bSvMuteOff, 0x1);
}

/* **************************************************** */
/* SPR2SetDSEMode										*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			initializes DSP.							*/
/*			Firmware Mode : DSE mode					*/
/* **************************************************** */
void SPR2SetDSEMode(void)
{
	/* wait for 10ms or more */
	Wait(DSP_PLL_WAIT);
	
	/* reset cancel */
	/* Note:
	    DD2_ICN is CPU's GPIO port. 
	    DD2_ICN is connected with DD-2SP's IC_N pin.
	*/
	DD2_ICN    = HIGH;
	
	/* set firmware mode */
	DSPWriteMem(0x4400, bInstData, 0x5FF);
	
	/* set Top firmware coefficients */
	DSPWriteMem(0x4000, bTopData, 0x6);
	
	/* set ILD firmware coefficients */
	DSPWriteMem(0x400B, bIldSDI0Sel, 0x2);
	DSPWriteMem(0x400D, bIldAtt, 0x1);
	DSPWriteMem(0x400E, bIldOff, 0x3);
	
	/* set S3D firmware coefficients */
	DSPWriteMem(0x409D, bS3DNatural, 0x99);
	DSPWriteMem(0x4136, bS3DOff, 0x2);
	
	/* set HXT firmware coefficients */
	DSPWriteMem(0x4149, bHarmonicsExpander, 0x4D);
	DSPWriteMem(0x4196, bHXTOff, 0x2);
	
	/* set AC firmware coefficients */
	DSPWriteMem(0x41F2, bAcAtt, 0x1);
	DSPWriteMem(0x41F3, bAEQCore, 0x204);
	DSPWriteMem(0x43F7, bAcOff, 0x2);
	
	/* set SV firmware coefficients */
	DSPWriteMem(0x4034, bSvAtt, 0x2);
	DSPWriteMem(0x4036, bSvFMMix, 0x2);
	DSPWriteMem(0x403B, bSvMuteOn, 0x1);
	DSPWriteMem(0x4042, b10bPEQOff, 0x2);
	DSPWriteMem(0x4044, bLnDEQ0High, 0xC);
	DSPWriteMem(0x4050, bLnOff, 0x2);
	
	/* set CF firmware coefficients */
	DSPWriteMem(0x41C3, bCf280Hz, 0x28);
	DSPWriteMem(0x41EB, bCf21ch, 0x5);
	
	/* set LMT firmware coefficients */
	DSPWriteMem(0x4066, &bBalanceCoef[33][0], 0x2);
	DSPWriteMem(0x4068, &bBalanceCoef[33][0], 0x2);
	DSPWriteMem(0x406A, bPowerLimit, 0x17);
	DSPWriteMem(0x4081, bLmtOff, 0x4);
	DSPWriteMem(0x4085, bLmtLevAdjSPR2, 0x4);
	DSPWriteMem(0x4089, bSoftcClipper, 0x3);
	DSPWriteMem(0x408C, bLmtSCOff, 0x2);
	DSPWriteMem(0x408E, bLmtLoMixOff, 0x1);
	DSPWriteMem(0x408F, bLmtSMix, 0x2);
	DSPWriteMem(0x4091, bLmt21ch, 0x6);
	
	/* set SDSP 10band PEQ coefficients */
	DSPWriteMem(0x8000, bAEQPEQ, 0x46);
	
	/* set registers (1) */
	DSPPartWriteReg(0x07, 0x20, 0x3F);	/* PI=1, PIO=0, AIFMD[3:0]=0x0 */
	DSPPartWriteReg(0x0A, 0x02, 0x03);	/* WRAMRTMD=1, WRAMMD=0 */
	
	/* set registers (2) */
	/* Note:
		As example, set SPR-2 input/output audio data format to I2S data format.
	*/
	DSPPartWriteReg(0x08, 0x23, 0xFF);	/* SDIFMT[1:0]=10, SDIBIT[1:0]=00, SDIWCKP=1, SDIBCKP=1 */
	DSPPartWriteReg(0x09, 0x23, 0xFF);	/* SDOFMT[1:0]=10, SDOBIT[1:0]=00, SDOWCKP=1, SDOBCKP=1 */
	DSPPartWriteReg(0x0B, 0x00, 0x03);	/* DEMON=DCM=0 */
	DSPPartWriteReg(0x0D, 0x01, 0x01);	/* EXIRQN=1 */
	DSPPartWriteReg(0x0E, 0x00, 0x03);	/* FSM[1:0]=00 */
	DSPPartWriteReg(0x23, 0x04, 0x04);	/* PLLERRE=1 */
	
	/* start DSP */
	DSPPartWriteReg(0x11, 0x03, 0x03);	/* MDSPSTART=SDSPSTART=1 */
	
	/* load preset patterns */
	DSPLoadPreset(0x02B4, 0, FALSE);	/* [692:SV Curve Mid] */
	DSPLoadPreset(0x02FB, 0, FALSE);	/* [763:SV -12.0dB] */
	
	/* release mute (1) */
	DSPPartWriteReg(0x10, 0x07, 0x07);	/* FMMUTEN=MDSPMUTEN=SDSPMUTEN=1 */
	
	/* release mute (2) */
	MDSPCoefRTDL(0x403B, bSvMuteOff, 0x1);
}

/* end of file */
>>>>>>> origin
