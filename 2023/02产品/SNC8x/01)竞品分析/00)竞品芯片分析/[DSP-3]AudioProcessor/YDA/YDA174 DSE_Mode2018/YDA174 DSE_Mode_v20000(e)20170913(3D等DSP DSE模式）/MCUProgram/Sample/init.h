<<<<<<< HEAD
/* 	CONFIDENTIAL
*********************************************************************************

	File:			init.h
	Version:		1.00.00
	Description:	Header File for Device Initialization Processing

	Copyright(c) 2012 Yamaha Corporation

*********************************************************************************/

/* stabilization time */
#define DSP_PLL_WAIT	(UL)(10000)		/* PLL */
#define DSP_ICN_WAIT	(UL)(1000)		/* IC_N (low to high) */
#define DSP_AMP_WAIT	(UL)(107000)	/* digital amplifier */

/* proto type declaration */
void Wait(UL dwWait);
void DD2SPSetDSEMode(void);
void SPR2SetDSEMode(void);

/* end of file */
=======
/* 	CONFIDENTIAL
*********************************************************************************

	File:			init.h
	Version:		1.00.00
	Description:	Header File for Device Initialization Processing

	Copyright(c) 2012 Yamaha Corporation

*********************************************************************************/

/* stabilization time */
#define DSP_PLL_WAIT	(UL)(10000)		/* PLL */
#define DSP_ICN_WAIT	(UL)(1000)		/* IC_N (low to high) */
#define DSP_AMP_WAIT	(UL)(107000)	/* digital amplifier */

/* proto type declaration */
void Wait(UL dwWait);
void DD2SPSetDSEMode(void);
void SPR2SetDSEMode(void);

/* end of file */
>>>>>>> origin
