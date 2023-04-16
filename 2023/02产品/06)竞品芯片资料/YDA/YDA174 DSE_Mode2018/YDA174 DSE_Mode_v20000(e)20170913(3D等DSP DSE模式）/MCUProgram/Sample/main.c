<<<<<<< HEAD
/* 	CONFIDENTIAL
*********************************************************************************

	File:			main.c
	Version:		1.00.00
	Description:	Main Processing

	Copyright(c) 2012 Yamaha Corporation

*********************************************************************************/
#include	"common.h"
#include	"main.h"
#include	"init.h"
#include	"dsp_access.h"


/* ******************************************************** */
/* LoopWait													*/
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
void LoopWait(UL dwWait)
{
	UL i;
	
	/* 
		The "for" sentence processing takes approximately 12 clocks (=500 nsec).
		Doubled "dwWait" is 1 microsecond.
	*/
	dwWait *= 2;
	
	for(i = 0; i < dwWait; i++);
}

/* **************************************************** */
/* CPUInit												*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			initilaizes microprocessor.					*/
/* **************************************************** */
void  CPUInit(void)
{
	/* TODO
		please add microprocessor setting.
	*/
}

/* **************************************************** */
/* DeviceInit											*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			initializes input/output device.			*/
/* **************************************************** */
void DeviceInit(void)
{
	/* TODO
		please add input/output device setting.
	*/
}

/* **************************************************** */
/* main													*/
/*		parameter:										*/
/* 			none										*/
/* 														*/
/*		return  value:									*/
/* 			none										*/
/* 														*/
/*		description:									*/
/* 			main routine. 								*/
/* **************************************************** */
void main(void)
{
	/* initializes CPU */
	CPUInit();
	
	/* initializes input device */
	DeviceInit();
	
	/* initializes DSP */
	DD2SPSetDSEMode();
	
	DSPLoadPreset(0x020, 0, FALSE);		/* [32:Power_On_Short_01] */
	
	/* main loop */
	while(1)
	{
		if(CheckError() == ERROR_DSP)
		{
		}

		/* TODO
			please add the wanted control.
		*/
	}
}

/* end of file*/
=======
/* 	CONFIDENTIAL
*********************************************************************************

	File:			main.c
	Version:		1.00.00
	Description:	Main Processing

	Copyright(c) 2012 Yamaha Corporation

*********************************************************************************/
#include	"common.h"
#include	"main.h"
#include	"init.h"
#include	"dsp_access.h"


/* ******************************************************** */
/* LoopWait													*/
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
void LoopWait(UL dwWait)
{
	UL i;
	
	/* 
		The "for" sentence processing takes approximately 12 clocks (=500 nsec).
		Doubled "dwWait" is 1 microsecond.
	*/
	dwWait *= 2;
	
	for(i = 0; i < dwWait; i++);
}

/* **************************************************** */
/* CPUInit												*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			initilaizes microprocessor.					*/
/* **************************************************** */
void  CPUInit(void)
{
	/* TODO
		please add microprocessor setting.
	*/
}

/* **************************************************** */
/* DeviceInit											*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			none										*/
/*														*/
/*		description:									*/
/*			initializes input/output device.			*/
/* **************************************************** */
void DeviceInit(void)
{
	/* TODO
		please add input/output device setting.
	*/
}

/* **************************************************** */
/* main													*/
/*		parameter:										*/
/* 			none										*/
/* 														*/
/*		return  value:									*/
/* 			none										*/
/* 														*/
/*		description:									*/
/* 			main routine. 								*/
/* **************************************************** */
void main(void)
{
	/* initializes CPU */
	CPUInit();
	
	/* initializes input device */
	DeviceInit();
	
	/* initializes DSP */
	DD2SPSetDSEMode();
	
	DSPLoadPreset(0x020, 0, FALSE);		/* [32:Power_On_Short_01] */
	
	/* main loop */
	while(1)
	{
		if(CheckError() == ERROR_DSP)
		{
		}

		/* TODO
			please add the wanted control.
		*/
	}
}

/* end of file*/
>>>>>>> origin
