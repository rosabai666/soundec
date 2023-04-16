<<<<<<< HEAD
/* 	CONFIDENTIAL
*********************************************************************************

	File:			dsp_access.c
	Version:		1.00.00

	Description:	DSP Access Processing

	Copyright(c) 2011 Yamaha Corporation

*********************************************************************************/
#include	"common.h"
#include	"ioctrl.h"
#include	"dsp_access.h"

/* proto type declaration */
UC GetPRESETROMCommand(US wNum, UC *pbData);
UL GetDSPWordSize(US wAdd);
UL GetDSPDataSize(US wAdd, US wSize);
UC CheckMDSPREQ(void);


/* **************************************************** */
/* GetPRESETROMCommand									*/
/*		parameter:										*/
/*			unsigned short		wNum					*/
/*								(ROM block number)		*/
/*														*/
/*			*unsigned char		pbCommand				*/
/*								(command code)			*/
/*														*/
/*		return value:									*/
/*			unsigned char		processing result		*/
/*														*/
/*		description:									*/
/*			obtain PRESETROM command. 					*/
/* **************************************************** */
UC GetPRESETROMCommand(US wNum, UC *pbCommand)
{
	UC bResult = SUCCESS;
	
	if(wNum <= PRESETROM0_END)
	{
		*pbCommand = 0x70;
		*(pbCommand+1) = wNum;
	}
	else if(wNum <= PRESETROM1_END)
	{
		*pbCommand = 0x71;
		*(pbCommand+1) = wNum - (PRESETROM0_END+1);
	}
	else if(wNum <= PRESETROM2_END)
	{
		*pbCommand = 0x72;
		*(pbCommand+1) = wNum - (PRESETROM1_END+1);
	}
	else if(wNum <= PRESETROM3_END)
	{
		*pbCommand = 0x73;
		*(pbCommand+1) = wNum - (PRESETROM2_END+1);
	}
	else
	{
		bResult = ERROR_PARAM;
	}
	
	return(bResult);
}

/* **************************************************** */
/* GetDSPWordSize										*/
/*		parameter:										*/
/*			unsigned short		wAdd					*/
/*								(DSP memory address)	*/
/*														*/
/*		return value:									*/
/*			unsigned long		word size				*/
/*														*/
/*		descripton:										*/
/*			obtain word size of the DSP memory.			*/
/* **************************************************** */
UL GetDSPWordSize(US wAdd)
{
	if     (wAdd <= DSP_MEM_ADD_WRAM0END)	return(2);
	else if(wAdd <= DSP_MEM_ADD_WRAM1END)	return(4);
	else if(wAdd <= DSP_MEM_ADD_WRAM2END)	return(4);
	else if(wAdd <= DSP_MEM_ADD_CRAMEND)	return(2);
	else if(wAdd <= DSP_MEM_ADD_MPRAMEND)	return(5);
	else if(wAdd <= DSP_MEM_ADD_QBUSEND)	return(4);
	else if(wAdd <= DSP_MEM_ADD_PEQRAMEND)	return(3);
	else return(2);
}

/* **************************************************** */
/* GetDSPDataSize										*/
/*		parameter:										*/
/*			unsigned short		wAdd					*/
/*								(DSP memory address)	*/
/*														*/
/*			unsigned short		wSize					*/
/*								(data size)				*/
/*														*/
/*		return value:									*/
/*			unsigned long		byte size				*/
/*														*/
/*		description:									*/
/*			obtain byte size.							*/
/* **************************************************** */
UL GetDSPDataSize(US wAdd, US wSize)
{
	US wCnt;
	UL dwResult = 0;
	
	for(wCnt = 0; wCnt < wSize; wCnt++)
	{
		dwResult += GetDSPWordSize(wAdd + wCnt);
	}
	
	return(dwResult);
}

/* **************************************************** */
/* CheckMDSPREQ											*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			unsigned char		processing result		*/
/*														*/
/*		description:									*/
/*			check DSP register : 0x13 MDSPREQ.			*/
/* **************************************************** */
UC CheckMDSPREQ(void)
{
	UC bData, bResult;
	
	/* wait for 1 sample time(=21 microseconds) */
	LoopWait(RT_MAX_WAIT);
	
	/* output start condition */
	bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
	
	/* transmit address */
	if(bResult == SUCCESS)
	{
		bResult = Uart0Out(DSP_REG_ADD_MDSPREQ);
	}
	
	/* output start conditions again */
	if(bResult == SUCCESS)
	{
		bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_READ);
	}
	
	/* receive data */
	if(bResult == SUCCESS)
	{
		bResult = Uart0In(&bData);
	}
	
	/* output stop condition */
	Uart0Stop();
	
	if(bResult == SUCCESS)
	{
		if(bData != DSP_REG_DATA_RTREADY)
		{
			return(ERROR_RT);
		}
	}
	return(bResult);
}

/* **************************************************** */
/* CheckError											*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			unsigned char		processing result		*/
/*														*/
/*		description:									*/
/*			check errors of DSP. 						*/
/* **************************************************** */
UC CheckError(void)
{
	UC bMDSPST, bSDSPST, bDAMPST0, bDAMPST1, bPLLERR;
	UC bResult;
	
	bResult  = DSPReadReg(0x1B, &bMDSPST);		/* 0x1B MDSPST */
	bResult += DSPReadReg(0x20, &bSDSPST);		/* 0x20 SDSPST */
	bResult += DSPReadReg(0x23, &bPLLERR);		/* 0x23 PLLERR */

	if(bResult == SUCCESS)
	{
		if( (bMDSPST  & DSP_REG_MASK_MDSPERR)  |	/* check MDSPERR bit */
			(bSDSPST  & DSP_REG_MASK_SDSPERR)  |	/* check SDSPERR bit */
			(bPLLERR  & DSP_REG_MASK_PLLERR) )		/* check PLLERR bit */
			
		{
			return(ERROR_DSP);
		}
	}
	return(bResult);
}

/* **************************************************** */
/* DSPWriteReg											*/
/*		parameter:										*/
/*			unsigned char		bAdd					*/
/*								(destination address)	*/
/*														*/
/*			unsigned char		bData					*/
/*								(setting data)			*/
/*														*/
/*		return value:									*/
/*			unsigned char		processing result		*/
/*														*/
/*		description:									*/
/*			set DSP register.							*/
/* **************************************************** */
UC DSPWriteReg(UC bAdd, UC bData)
{
	UC bResult;
	US wNakCount = 0;
	
	/* check address */
	if( (bAdd < DSP_REG_ADD_TOP) || (bAdd > DSP_REG_ADD_END) )
	{
		return(ERROR_PARAM);
	}
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit address */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bAdd);
		}
		
		/* transmit data */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bData);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* **************************************************** */
/* DSPReadReg											*/
/*		parameter:										*/
/*			unsigned char		bAdd					*/
/*								(source address)		*/
/*														*/
/*			*unsigned char		pbData					*/
/*								(acquired data)			*/
/*														*/
/*		return value:									*/
/*			unsigned char		processing result		*/
/*														*/
/*		description:									*/
/*			obtain data form DSP register.				*/
/* **************************************************** */
UC DSPReadReg(UC bAdd, UC *pbData)
{
	UC bResult;
	US wNakCount = 0;

	/* check address */
	if( (bAdd < DSP_REG_ADD_TOP) || (bAdd > DSP_REG_ADD_END) )
	{
		return(ERROR_PARAM);
	}
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit address */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bAdd);
		}
		
		/* output start conditions again */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_READ);
		}
		
		/* receive data */
		if(bResult == SUCCESS)
		{
			bResult = Uart0In(pbData);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* **************************************************** */
/* DSPPartWriteReg										*/
/*		parameter:										*/
/*			unsigned char		bAdd					*/
/*						   		(destination address)	*/
/*														*/
/*			unsigned char		bData					*/
/*								(setting data)			*/
/*														*/
/*			unsigned char		bMask					*/
/*								(mask)					*/
/*														*/
/*		return value:									*/
/*			unsigned char		processing result		*/
/*														*/
/*		description:									*/
/*			set DSP register by bits					*/
/* **************************************************** */
UC DSPPartWriteReg(UC bAdd, UC bData, UC bMask)
{
	UC bResult, bTmp;
	
	/* obtain data from register */
	bResult = DSPReadReg(bAdd, &bTmp);
	if(bResult != SUCCESS)
	{
		return(bResult);
	}
	
	/* create setting data */
	bData = ( (bTmp & ~bMask) | (bData & bMask) );
	
	/* set register */
	bResult = DSPWriteReg(bAdd, bData);
	
	return(bResult);
}

/* ******************************************************************** */
/* DSPLoadPreset														*/
/*		parameter:														*/
/*			unsigned short		wNum									*/
/*								(ROM block number)						*/
/*																		*/
/*			unsigned short		wAdd									*/
/*								(top address of transfer destination)	*/
/*																		*/
/*			unsigned char		bSetAdd									*/
/*								(1: set address)						*/
/*								(others: not set address)				*/
/*																		*/
/*		return value:													*/
/*			unsigned char		processing result						*/
/*																		*/
/*		description:													*/
/*			load preset pattern.										*/
/* ******************************************************************** */
UC DSPLoadPreset(US wNum, US wAdd, UC bSetAdd)
{
	UC bCommand[2], bStartAdd[2];
	UC bResult;
	US wNakCount = 0;

	/* check ROM block number */
	if( (wNum < DSP_ROM_NUM_MIN) || (wNum > DSP_ROM_NUM_MAX) )
	{
		return(ERROR_PARAM);
	}
	
	/* when setting address */
	if(bSetAdd == TRUE)
	{
		/* check address */
		if( (wAdd < DSP_MEM_ADD_CRAMTOP) || (wAdd > DSP_MEM_ADD_CRAMEND) )
		{
			return(ERROR_PARAM);
		}
		
		bStartAdd[0] = ( (wAdd - DSP_MEM_ADD_CRAMTOP) >> 8 ) & 0xFF;
		bStartAdd[1] = (wAdd - DSP_MEM_ADD_CRAMTOP) & 0xFF;
	}
	
	/* obtain PRESETROM command */
	GetPRESETROMCommand(wNum, bCommand);
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* when setting address */
		if(bSetAdd == TRUE)
		{
			/* transmit WRITE/READ command */
			if(bResult == SUCCESS)
			{
				bResult = Uart0Out(DSP_COM_WRITEREAD);
			}
			
			/* transmit address of RtStartAdr */
			if(bResult == SUCCESS)
			{
				bResult = Uart0Out( (DSP_MEM_ADD_RTSTARTADR >> 8) & 0xFF );
			}
			
			if(bResult == SUCCESS)
			{
				bResult = Uart0Out(DSP_MEM_ADD_RTSTARTADR & 0xFF);
			}
			
			/* transmit top address of transfer destination */
			if(bResult == SUCCESS)
			{
				bResult = Uart0Out(bStartAdd[0]);
			}
			
			if(bResult == SUCCESS)
			{
				bResult = Uart0Out(bStartAdd[1]);
			}
			
			/* output start conditions again */
			if(bResult == SUCCESS)
			{
				bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_WRITE);
			}
		}
		
		/* transmit PRESETROM command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bCommand[0]);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bCommand[1]);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* **************************************************** */
/* DSPStopPreset										*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			unsigned char		processing result		*/
/*														*/
/*		description:									*/
/*			abort load of preset pattern.				*/
/* **************************************************** */
UC DSPStopPreset(void)
{
	UC bResult;
	US wNakCount = 0;
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit PRROMSTOP command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_PRROMSTOP);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* ******************************************************************** */
/* DSPWriteMem															*/
/*		parameter:														*/
/*			unsigned short		wAdd									*/
/*								(top address of transfer destination)	*/
/*																		*/
/*			*unsigned char		pbData									*/
/*								(setting data)							*/
/*																		*/
/*			unsigned short		wWordSize								*/
/*								(data size. word by word.)				*/
/*																		*/
/*		return value:													*/
/*			unsigned char		processing result						*/
/*																		*/
/*		description:													*/
/*			set DSP memory.												*/
/* ******************************************************************** */
UC DSPWriteMem(US wAdd, UC *pbData, US wWordSize)
{
	UL dwByteSize, dwTmp;
	UC bResult;
	US wNakCount = 0;
	
	/* check size */
	if( (wWordSize <= 0) || (wWordSize > TRANS_MAX_SIZE) )
	{
		return(ERROR_PARAM);
	}

	/* check area */
	dwTmp = (UL)wAdd + (UL)wWordSize - 1;
	if( (wAdd < DSP_MEM_ADD_RAMTOP) || (dwTmp > DSP_MEM_ADD_RAMEND) )
	{
		return(ERROR_PARAM);
	}

	/* convert size by words to by bytes */
	dwByteSize = GetDSPDataSize(wAdd, wWordSize);
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit WRITE/READ command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_WRITEREAD);
		}
		
		/* transmit address */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (wAdd >> 8) & 0xFF );
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(wAdd & 0xFF);
		}
		
		/* transmit data */
		if(bResult == SUCCESS)
		{
			bResult = Uart0BurstOut( (UL)pbData, dwByteSize);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* **************************************************************** */
/* DSPReadMem														*/
/*		parameter:													*/
/*			unsigned short		wAdd								*/
/*								(top address of transfer source)	*/
/*																	*/
/*			*unsigned char		pbData								*/
/*								(acquired data)						*/
/*																	*/
/*			unsigned short		wWordSize							*/
/*								(data size. word by word.)			*/
/*		return value:												*/
/*																	*/
/*			unsigned char		processing result					*/
/*																	*/
/*		description:												*/
/*			obtain data form DSP memory.							*/
/* **************************************************************** */
UC DSPReadMem(US wAdd, UC *pbData, US wWordSize)
{
	UL dwByteSize, dwTmp;
	UC bResult;
	US wNakCount = 0;
	
	/* check size */
	if( (wWordSize <= 0) || (wWordSize > TRANS_MAX_SIZE) )
	{
		return(ERROR_PARAM);
	}

	/* check area */
	dwTmp = (UL)wAdd + (UL)wWordSize - 1;
	if( (wAdd < DSP_MEM_ADD_RAMTOP) || (dwTmp > DSP_MEM_ADD_RAMEND) )
	{
		return(ERROR_PARAM);
	}
	
	/* convert size by words to by bytes */
	dwByteSize = GetDSPDataSize(wAdd, wWordSize);

	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit WRITE/READ command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_WRITEREAD);
		}
		
		/* transmit address */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (wAdd >> 8) & 0xFF );
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(wAdd & 0xFF);
		}
		
		/* transmit zero */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(0x00);
		}
		
		/* output start conditions again */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_READ);
		}
		
		/* receive data */
		if(bResult == SUCCESS)
		{
			bResult = Uart0BurstIn( (UL)pbData, dwByteSize);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* ******************************************************************** */
/* MDSPCoefRTDL															*/
/*		parameter:														*/
/*			unsigned short		wAdd									*/
/*								(top address of transfer destination)	*/
/*																		*/
/*			*unsigned char		pbData									*/
/*								(setting data)							*/
/*																		*/
/*			unsigned short		wWordSize								*/
/*								(data size. word by word.)				*/
/*		return value:													*/
/*																		*/
/*			unsigned char		processing result						*/
/*																		*/
/*		description:													*/
/*			set DSP memory by runtime transfer.							*/
/* ******************************************************************** */
UC MDSPCoefRTDL(US wAdd, UC *pbData, US wWordSize)
{
	UL dwByteSize, dwTmp;
	UC bStartAdd[2];
	UC bResult;
	US wNakCount = 0;
	
	/* check size */
	if( (wWordSize < 1) || (wWordSize > RT_MAX_SIZE) )
	{
		return(ERROR_PARAM);
	}
		
	/* check area */
	dwTmp = (UL)wAdd + (UL)wWordSize - 1;
	if( (wAdd < DSP_MEM_ADD_CRAMTOP) || (dwTmp > DSP_MEM_ADD_CRAMEND) )
	{
		return(ERROR_PARAM);
	}
	
	/* prepare transfer data (top address of transfer destination) */
	bStartAdd[0] = ( (wAdd - DSP_MEM_ADD_CRAMTOP) >> 8 ) & 0xFF;
	bStartAdd[1] = (wAdd - DSP_MEM_ADD_CRAMTOP) & 0xFF;
	
	/* convert size by words to by bytes */
	dwByteSize = GetDSPDataSize(wAdd, wWordSize);
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit WRITE/READ command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_WRITEREAD);
		}
		
		/* transmit address of RtStartAdr */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (DSP_MEM_ADD_RTSTARTADR >> 8) & 0xFF );
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_MEM_ADD_RTSTARTADR & 0xFF);
		}
		
		/* transmit top address of transfer destination */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bStartAdd[0]);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bStartAdd[1]);
		}
		
		/* transmit size */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(0x00);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (UC)(wWordSize - 1) );
		}
		
		/* transfer data to RtData*(*=0-9) */
		if(bResult == SUCCESS)
		{
			bResult = Uart0BurstOut( (UL)pbData, dwByteSize);
		}
		
		/* output start conditions again */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		}
		
		/* request runtime transfer */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_REG_ADD_MDSPREQ);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_REG_DATA_RTDL);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	/* when runtime transfer processing is successful */
	if(bResult == SUCCESS)
	{
		/* obtain/inform processing result of DSP */
		return(CheckMDSPREQ());
	}
	
	return(bResult);
}

/* **************************************************************** */
/* MDSPCoefRTUL														*/
/*		parameter:													*/
/*			unsigned short		wAdd								*/
/*								(top address of transfer source)	*/
/*																	*/
/*			*unsigned char		pbData								*/
/*								(acquired data)						*/
/*																	*/
/*			unsigned short		usWordSize							*/
/*								(data size.word by word.)			*/
/*																	*/
/*		return value:												*/
/*			unsigned char		processing result					*/
/*																	*/
/*		description:												*/
/*			obtain data from  DSP memory by runtime transfer.		*/
/* **************************************************************** */
UC MDSPCoefRTUL(US wAdd, UC *pbData, US wWordSize)
{
	UL dwByteSize, dwTmp;
	UC bStartAdd[2];
	UC bResult;
	US wNakCount = 0;
	
	/* check size */
	if( (wWordSize < 1) || (wWordSize > RT_MAX_SIZE) )
	{
		return(ERROR_PARAM);
	}
		
	/* check area */
	dwTmp = (UL)wAdd + (UL)wWordSize - 1;
	if( (wAdd < DSP_MEM_ADD_CRAMTOP) || (dwTmp > DSP_MEM_ADD_CRAMEND) )
	{
		return(ERROR_PARAM);
	}
	
	/* prepare the transfer data (top address of transfer source) */
	bStartAdd[0] = ( (wAdd - DSP_MEM_ADD_CRAMTOP) >> 8 ) & 0xFF;
	bStartAdd[1] = (wAdd - DSP_MEM_ADD_CRAMTOP) & 0xFF;
	
	/* convert size by words to by bytes */
	dwByteSize = GetDSPDataSize(wAdd, wWordSize);
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit WRITE/READ command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_WRITEREAD);
		}
		
		/* transmit address of RtStartAdr */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (DSP_MEM_ADD_RTSTARTADR >> 8) & 0xFF );
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_MEM_ADD_RTSTARTADR & 0xFF);
		}
		
		/* transmit top address of transfer source */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bStartAdd[0]);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bStartAdd[1]);
		}
		
		/* transmit size */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(0x00);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (UC)(wWordSize - 1) );
		}
		
		/* output start conditions again */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		}
		
		/* request runtime transfer */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_REG_ADD_MDSPREQ);
		}
		
		if( bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_REG_DATA_RTUL);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	/* when runtime transfer processing is successful */
	if(bResult == SUCCESS)
	{
		/* obtain processing result of DSP */
		bResult = CheckMDSPREQ();
		
		/* when processing for DSP is failed */
		if(bResult != SUCCESS)
		{
			return(bResult);
		}
	}
	
	wNakCount = 0;
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit WRITE/READ command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_WRITEREAD);
		}
		
		/* transmit address of RtData0 */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (DSP_MEM_ADD_RTDATA0 >> 8) & 0xFF );
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_MEM_ADD_RTDATA0 & 0xFF);
		}
		
		/* transmit zero */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(0x00);
		}
		
		/* output start conditions again */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_READ);
		}
		
		/* acquire data from RtData*(*=0-9) */
		if(bResult == SUCCESS)
		{
			bResult = Uart0BurstIn( (UL)pbData, dwByteSize);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* ************************************************************************ */
/* SDSPCoefBT																*/
/*		parameter:															*/
/*			unsigned short		wAdd										*/
/*								(top address of the transfer destination)	*/
/*																			*/
/*			*unsigned char		pbData										*/
/*								(setting data)								*/
/*																			*/
/*			unsigned short		wWordSize									*/
/*								(data size.word by word.)					*/
/*																			*/
/*		return value:														*/
/*			unsigned char		processing result							*/
/*																			*/
/*		description:														*/
/*			set PEQ coefficient RAM by batch transfer.						*/
/* ************************************************************************ */
UC SDSPCoefBT(US wAdd, UC *pbData, US wWordSize)
{
	UL dwByteSize, dwTmp;
	UC bStartAdd[2];
	UC bPEQPrameter, bResult;
	US wNakCount = 0;
	
	/* check size */
	if( (wWordSize != 5) && (wWordSize != 10) )
	{
		return(ERROR_PARAM);
	}
		
	/* check area */
	dwTmp = (UL)wAdd + (UL)wWordSize - 1;
	if( (wAdd < DSP_MEM_ADD_PEQCOEFTOP) || (dwTmp > (DSP_MEM_ADD_PEQCOEFEND)) )
	{
		return(ERROR_PARAM);
	}

	/* prepare transfer parameter */
	if(wWordSize == 0x5)
		bPEQPrameter = ( 0x00 | (wAdd & DSP_RAM_MASK_PEQAD) );
	else
		bPEQPrameter = ( 0x80 | (wAdd & DSP_RAM_MASK_PEQAD) );
	
	/* convert size by words to by bytes */
	dwByteSize = GetDSPDataSize(wAdd, wWordSize);
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit WRITE/READ command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_WRITEREAD);
		}
		
		/* transmit PEQ coefficient buffer address */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (DSP_MEM_ADD_PEQBUFTOP >> 8) & 0xFF );
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_MEM_ADD_PEQBUFTOP & 0xFF);
		}
		
		/* transfer data */
		if(bResult == SUCCESS)
		{
			bResult = Uart0BurstOut( (UL)pbData, dwByteSize);
		}
		
		/* output start conditions again */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		}
		
		/* transmit WRITE/READ command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_WRITEREAD);
		}
		
		/* transmit address of transfer parameter */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (DSP_MEM_ADD_PEQPRAMETER >> 8) & 0xFF );
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_MEM_ADD_PEQPRAMETER & 0xFF);
		}
		
		/* transmit prepare transfer parameter */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bPEQPrameter);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(0x00);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(0x00);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* end of file*/
=======
/* 	CONFIDENTIAL
*********************************************************************************

	File:			dsp_access.c
	Version:		1.00.00

	Description:	DSP Access Processing

	Copyright(c) 2011 Yamaha Corporation

*********************************************************************************/
#include	"common.h"
#include	"ioctrl.h"
#include	"dsp_access.h"

/* proto type declaration */
UC GetPRESETROMCommand(US wNum, UC *pbData);
UL GetDSPWordSize(US wAdd);
UL GetDSPDataSize(US wAdd, US wSize);
UC CheckMDSPREQ(void);


/* **************************************************** */
/* GetPRESETROMCommand									*/
/*		parameter:										*/
/*			unsigned short		wNum					*/
/*								(ROM block number)		*/
/*														*/
/*			*unsigned char		pbCommand				*/
/*								(command code)			*/
/*														*/
/*		return value:									*/
/*			unsigned char		processing result		*/
/*														*/
/*		description:									*/
/*			obtain PRESETROM command. 					*/
/* **************************************************** */
UC GetPRESETROMCommand(US wNum, UC *pbCommand)
{
	UC bResult = SUCCESS;
	
	if(wNum <= PRESETROM0_END)
	{
		*pbCommand = 0x70;
		*(pbCommand+1) = wNum;
	}
	else if(wNum <= PRESETROM1_END)
	{
		*pbCommand = 0x71;
		*(pbCommand+1) = wNum - (PRESETROM0_END+1);
	}
	else if(wNum <= PRESETROM2_END)
	{
		*pbCommand = 0x72;
		*(pbCommand+1) = wNum - (PRESETROM1_END+1);
	}
	else if(wNum <= PRESETROM3_END)
	{
		*pbCommand = 0x73;
		*(pbCommand+1) = wNum - (PRESETROM2_END+1);
	}
	else
	{
		bResult = ERROR_PARAM;
	}
	
	return(bResult);
}

/* **************************************************** */
/* GetDSPWordSize										*/
/*		parameter:										*/
/*			unsigned short		wAdd					*/
/*								(DSP memory address)	*/
/*														*/
/*		return value:									*/
/*			unsigned long		word size				*/
/*														*/
/*		descripton:										*/
/*			obtain word size of the DSP memory.			*/
/* **************************************************** */
UL GetDSPWordSize(US wAdd)
{
	if     (wAdd <= DSP_MEM_ADD_WRAM0END)	return(2);
	else if(wAdd <= DSP_MEM_ADD_WRAM1END)	return(4);
	else if(wAdd <= DSP_MEM_ADD_WRAM2END)	return(4);
	else if(wAdd <= DSP_MEM_ADD_CRAMEND)	return(2);
	else if(wAdd <= DSP_MEM_ADD_MPRAMEND)	return(5);
	else if(wAdd <= DSP_MEM_ADD_QBUSEND)	return(4);
	else if(wAdd <= DSP_MEM_ADD_PEQRAMEND)	return(3);
	else return(2);
}

/* **************************************************** */
/* GetDSPDataSize										*/
/*		parameter:										*/
/*			unsigned short		wAdd					*/
/*								(DSP memory address)	*/
/*														*/
/*			unsigned short		wSize					*/
/*								(data size)				*/
/*														*/
/*		return value:									*/
/*			unsigned long		byte size				*/
/*														*/
/*		description:									*/
/*			obtain byte size.							*/
/* **************************************************** */
UL GetDSPDataSize(US wAdd, US wSize)
{
	US wCnt;
	UL dwResult = 0;
	
	for(wCnt = 0; wCnt < wSize; wCnt++)
	{
		dwResult += GetDSPWordSize(wAdd + wCnt);
	}
	
	return(dwResult);
}

/* **************************************************** */
/* CheckMDSPREQ											*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			unsigned char		processing result		*/
/*														*/
/*		description:									*/
/*			check DSP register : 0x13 MDSPREQ.			*/
/* **************************************************** */
UC CheckMDSPREQ(void)
{
	UC bData, bResult;
	
	/* wait for 1 sample time(=21 microseconds) */
	LoopWait(RT_MAX_WAIT);
	
	/* output start condition */
	bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
	
	/* transmit address */
	if(bResult == SUCCESS)
	{
		bResult = Uart0Out(DSP_REG_ADD_MDSPREQ);
	}
	
	/* output start conditions again */
	if(bResult == SUCCESS)
	{
		bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_READ);
	}
	
	/* receive data */
	if(bResult == SUCCESS)
	{
		bResult = Uart0In(&bData);
	}
	
	/* output stop condition */
	Uart0Stop();
	
	if(bResult == SUCCESS)
	{
		if(bData != DSP_REG_DATA_RTREADY)
		{
			return(ERROR_RT);
		}
	}
	return(bResult);
}

/* **************************************************** */
/* CheckError											*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			unsigned char		processing result		*/
/*														*/
/*		description:									*/
/*			check errors of DSP. 						*/
/* **************************************************** */
UC CheckError(void)
{
	UC bMDSPST, bSDSPST, bDAMPST0, bDAMPST1, bPLLERR;
	UC bResult;
	
	bResult  = DSPReadReg(0x1B, &bMDSPST);		/* 0x1B MDSPST */
	bResult += DSPReadReg(0x20, &bSDSPST);		/* 0x20 SDSPST */
	bResult += DSPReadReg(0x23, &bPLLERR);		/* 0x23 PLLERR */

	if(bResult == SUCCESS)
	{
		if( (bMDSPST  & DSP_REG_MASK_MDSPERR)  |	/* check MDSPERR bit */
			(bSDSPST  & DSP_REG_MASK_SDSPERR)  |	/* check SDSPERR bit */
			(bPLLERR  & DSP_REG_MASK_PLLERR) )		/* check PLLERR bit */
			
		{
			return(ERROR_DSP);
		}
	}
	return(bResult);
}

/* **************************************************** */
/* DSPWriteReg											*/
/*		parameter:										*/
/*			unsigned char		bAdd					*/
/*								(destination address)	*/
/*														*/
/*			unsigned char		bData					*/
/*								(setting data)			*/
/*														*/
/*		return value:									*/
/*			unsigned char		processing result		*/
/*														*/
/*		description:									*/
/*			set DSP register.							*/
/* **************************************************** */
UC DSPWriteReg(UC bAdd, UC bData)
{
	UC bResult;
	US wNakCount = 0;
	
	/* check address */
	if( (bAdd < DSP_REG_ADD_TOP) || (bAdd > DSP_REG_ADD_END) )
	{
		return(ERROR_PARAM);
	}
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit address */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bAdd);
		}
		
		/* transmit data */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bData);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* **************************************************** */
/* DSPReadReg											*/
/*		parameter:										*/
/*			unsigned char		bAdd					*/
/*								(source address)		*/
/*														*/
/*			*unsigned char		pbData					*/
/*								(acquired data)			*/
/*														*/
/*		return value:									*/
/*			unsigned char		processing result		*/
/*														*/
/*		description:									*/
/*			obtain data form DSP register.				*/
/* **************************************************** */
UC DSPReadReg(UC bAdd, UC *pbData)
{
	UC bResult;
	US wNakCount = 0;

	/* check address */
	if( (bAdd < DSP_REG_ADD_TOP) || (bAdd > DSP_REG_ADD_END) )
	{
		return(ERROR_PARAM);
	}
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit address */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bAdd);
		}
		
		/* output start conditions again */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_READ);
		}
		
		/* receive data */
		if(bResult == SUCCESS)
		{
			bResult = Uart0In(pbData);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* **************************************************** */
/* DSPPartWriteReg										*/
/*		parameter:										*/
/*			unsigned char		bAdd					*/
/*						   		(destination address)	*/
/*														*/
/*			unsigned char		bData					*/
/*								(setting data)			*/
/*														*/
/*			unsigned char		bMask					*/
/*								(mask)					*/
/*														*/
/*		return value:									*/
/*			unsigned char		processing result		*/
/*														*/
/*		description:									*/
/*			set DSP register by bits					*/
/* **************************************************** */
UC DSPPartWriteReg(UC bAdd, UC bData, UC bMask)
{
	UC bResult, bTmp;
	
	/* obtain data from register */
	bResult = DSPReadReg(bAdd, &bTmp);
	if(bResult != SUCCESS)
	{
		return(bResult);
	}
	
	/* create setting data */
	bData = ( (bTmp & ~bMask) | (bData & bMask) );
	
	/* set register */
	bResult = DSPWriteReg(bAdd, bData);
	
	return(bResult);
}

/* ******************************************************************** */
/* DSPLoadPreset														*/
/*		parameter:														*/
/*			unsigned short		wNum									*/
/*								(ROM block number)						*/
/*																		*/
/*			unsigned short		wAdd									*/
/*								(top address of transfer destination)	*/
/*																		*/
/*			unsigned char		bSetAdd									*/
/*								(1: set address)						*/
/*								(others: not set address)				*/
/*																		*/
/*		return value:													*/
/*			unsigned char		processing result						*/
/*																		*/
/*		description:													*/
/*			load preset pattern.										*/
/* ******************************************************************** */
UC DSPLoadPreset(US wNum, US wAdd, UC bSetAdd)
{
	UC bCommand[2], bStartAdd[2];
	UC bResult;
	US wNakCount = 0;

	/* check ROM block number */
	if( (wNum < DSP_ROM_NUM_MIN) || (wNum > DSP_ROM_NUM_MAX) )
	{
		return(ERROR_PARAM);
	}
	
	/* when setting address */
	if(bSetAdd == TRUE)
	{
		/* check address */
		if( (wAdd < DSP_MEM_ADD_CRAMTOP) || (wAdd > DSP_MEM_ADD_CRAMEND) )
		{
			return(ERROR_PARAM);
		}
		
		bStartAdd[0] = ( (wAdd - DSP_MEM_ADD_CRAMTOP) >> 8 ) & 0xFF;
		bStartAdd[1] = (wAdd - DSP_MEM_ADD_CRAMTOP) & 0xFF;
	}
	
	/* obtain PRESETROM command */
	GetPRESETROMCommand(wNum, bCommand);
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* when setting address */
		if(bSetAdd == TRUE)
		{
			/* transmit WRITE/READ command */
			if(bResult == SUCCESS)
			{
				bResult = Uart0Out(DSP_COM_WRITEREAD);
			}
			
			/* transmit address of RtStartAdr */
			if(bResult == SUCCESS)
			{
				bResult = Uart0Out( (DSP_MEM_ADD_RTSTARTADR >> 8) & 0xFF );
			}
			
			if(bResult == SUCCESS)
			{
				bResult = Uart0Out(DSP_MEM_ADD_RTSTARTADR & 0xFF);
			}
			
			/* transmit top address of transfer destination */
			if(bResult == SUCCESS)
			{
				bResult = Uart0Out(bStartAdd[0]);
			}
			
			if(bResult == SUCCESS)
			{
				bResult = Uart0Out(bStartAdd[1]);
			}
			
			/* output start conditions again */
			if(bResult == SUCCESS)
			{
				bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_WRITE);
			}
		}
		
		/* transmit PRESETROM command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bCommand[0]);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bCommand[1]);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* **************************************************** */
/* DSPStopPreset										*/
/*		parameter:										*/
/*			none										*/
/*														*/
/*		return value:									*/
/*			unsigned char		processing result		*/
/*														*/
/*		description:									*/
/*			abort load of preset pattern.				*/
/* **************************************************** */
UC DSPStopPreset(void)
{
	UC bResult;
	US wNakCount = 0;
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit PRROMSTOP command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_PRROMSTOP);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* ******************************************************************** */
/* DSPWriteMem															*/
/*		parameter:														*/
/*			unsigned short		wAdd									*/
/*								(top address of transfer destination)	*/
/*																		*/
/*			*unsigned char		pbData									*/
/*								(setting data)							*/
/*																		*/
/*			unsigned short		wWordSize								*/
/*								(data size. word by word.)				*/
/*																		*/
/*		return value:													*/
/*			unsigned char		processing result						*/
/*																		*/
/*		description:													*/
/*			set DSP memory.												*/
/* ******************************************************************** */
UC DSPWriteMem(US wAdd, UC *pbData, US wWordSize)
{
	UL dwByteSize, dwTmp;
	UC bResult;
	US wNakCount = 0;
	
	/* check size */
	if( (wWordSize <= 0) || (wWordSize > TRANS_MAX_SIZE) )
	{
		return(ERROR_PARAM);
	}

	/* check area */
	dwTmp = (UL)wAdd + (UL)wWordSize - 1;
	if( (wAdd < DSP_MEM_ADD_RAMTOP) || (dwTmp > DSP_MEM_ADD_RAMEND) )
	{
		return(ERROR_PARAM);
	}

	/* convert size by words to by bytes */
	dwByteSize = GetDSPDataSize(wAdd, wWordSize);
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit WRITE/READ command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_WRITEREAD);
		}
		
		/* transmit address */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (wAdd >> 8) & 0xFF );
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(wAdd & 0xFF);
		}
		
		/* transmit data */
		if(bResult == SUCCESS)
		{
			bResult = Uart0BurstOut( (UL)pbData, dwByteSize);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* **************************************************************** */
/* DSPReadMem														*/
/*		parameter:													*/
/*			unsigned short		wAdd								*/
/*								(top address of transfer source)	*/
/*																	*/
/*			*unsigned char		pbData								*/
/*								(acquired data)						*/
/*																	*/
/*			unsigned short		wWordSize							*/
/*								(data size. word by word.)			*/
/*		return value:												*/
/*																	*/
/*			unsigned char		processing result					*/
/*																	*/
/*		description:												*/
/*			obtain data form DSP memory.							*/
/* **************************************************************** */
UC DSPReadMem(US wAdd, UC *pbData, US wWordSize)
{
	UL dwByteSize, dwTmp;
	UC bResult;
	US wNakCount = 0;
	
	/* check size */
	if( (wWordSize <= 0) || (wWordSize > TRANS_MAX_SIZE) )
	{
		return(ERROR_PARAM);
	}

	/* check area */
	dwTmp = (UL)wAdd + (UL)wWordSize - 1;
	if( (wAdd < DSP_MEM_ADD_RAMTOP) || (dwTmp > DSP_MEM_ADD_RAMEND) )
	{
		return(ERROR_PARAM);
	}
	
	/* convert size by words to by bytes */
	dwByteSize = GetDSPDataSize(wAdd, wWordSize);

	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit WRITE/READ command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_WRITEREAD);
		}
		
		/* transmit address */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (wAdd >> 8) & 0xFF );
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(wAdd & 0xFF);
		}
		
		/* transmit zero */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(0x00);
		}
		
		/* output start conditions again */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_READ);
		}
		
		/* receive data */
		if(bResult == SUCCESS)
		{
			bResult = Uart0BurstIn( (UL)pbData, dwByteSize);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* ******************************************************************** */
/* MDSPCoefRTDL															*/
/*		parameter:														*/
/*			unsigned short		wAdd									*/
/*								(top address of transfer destination)	*/
/*																		*/
/*			*unsigned char		pbData									*/
/*								(setting data)							*/
/*																		*/
/*			unsigned short		wWordSize								*/
/*								(data size. word by word.)				*/
/*		return value:													*/
/*																		*/
/*			unsigned char		processing result						*/
/*																		*/
/*		description:													*/
/*			set DSP memory by runtime transfer.							*/
/* ******************************************************************** */
UC MDSPCoefRTDL(US wAdd, UC *pbData, US wWordSize)
{
	UL dwByteSize, dwTmp;
	UC bStartAdd[2];
	UC bResult;
	US wNakCount = 0;
	
	/* check size */
	if( (wWordSize < 1) || (wWordSize > RT_MAX_SIZE) )
	{
		return(ERROR_PARAM);
	}
		
	/* check area */
	dwTmp = (UL)wAdd + (UL)wWordSize - 1;
	if( (wAdd < DSP_MEM_ADD_CRAMTOP) || (dwTmp > DSP_MEM_ADD_CRAMEND) )
	{
		return(ERROR_PARAM);
	}
	
	/* prepare transfer data (top address of transfer destination) */
	bStartAdd[0] = ( (wAdd - DSP_MEM_ADD_CRAMTOP) >> 8 ) & 0xFF;
	bStartAdd[1] = (wAdd - DSP_MEM_ADD_CRAMTOP) & 0xFF;
	
	/* convert size by words to by bytes */
	dwByteSize = GetDSPDataSize(wAdd, wWordSize);
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit WRITE/READ command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_WRITEREAD);
		}
		
		/* transmit address of RtStartAdr */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (DSP_MEM_ADD_RTSTARTADR >> 8) & 0xFF );
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_MEM_ADD_RTSTARTADR & 0xFF);
		}
		
		/* transmit top address of transfer destination */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bStartAdd[0]);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bStartAdd[1]);
		}
		
		/* transmit size */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(0x00);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (UC)(wWordSize - 1) );
		}
		
		/* transfer data to RtData*(*=0-9) */
		if(bResult == SUCCESS)
		{
			bResult = Uart0BurstOut( (UL)pbData, dwByteSize);
		}
		
		/* output start conditions again */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		}
		
		/* request runtime transfer */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_REG_ADD_MDSPREQ);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_REG_DATA_RTDL);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	/* when runtime transfer processing is successful */
	if(bResult == SUCCESS)
	{
		/* obtain/inform processing result of DSP */
		return(CheckMDSPREQ());
	}
	
	return(bResult);
}

/* **************************************************************** */
/* MDSPCoefRTUL														*/
/*		parameter:													*/
/*			unsigned short		wAdd								*/
/*								(top address of transfer source)	*/
/*																	*/
/*			*unsigned char		pbData								*/
/*								(acquired data)						*/
/*																	*/
/*			unsigned short		usWordSize							*/
/*								(data size.word by word.)			*/
/*																	*/
/*		return value:												*/
/*			unsigned char		processing result					*/
/*																	*/
/*		description:												*/
/*			obtain data from  DSP memory by runtime transfer.		*/
/* **************************************************************** */
UC MDSPCoefRTUL(US wAdd, UC *pbData, US wWordSize)
{
	UL dwByteSize, dwTmp;
	UC bStartAdd[2];
	UC bResult;
	US wNakCount = 0;
	
	/* check size */
	if( (wWordSize < 1) || (wWordSize > RT_MAX_SIZE) )
	{
		return(ERROR_PARAM);
	}
		
	/* check area */
	dwTmp = (UL)wAdd + (UL)wWordSize - 1;
	if( (wAdd < DSP_MEM_ADD_CRAMTOP) || (dwTmp > DSP_MEM_ADD_CRAMEND) )
	{
		return(ERROR_PARAM);
	}
	
	/* prepare the transfer data (top address of transfer source) */
	bStartAdd[0] = ( (wAdd - DSP_MEM_ADD_CRAMTOP) >> 8 ) & 0xFF;
	bStartAdd[1] = (wAdd - DSP_MEM_ADD_CRAMTOP) & 0xFF;
	
	/* convert size by words to by bytes */
	dwByteSize = GetDSPDataSize(wAdd, wWordSize);
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit WRITE/READ command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_WRITEREAD);
		}
		
		/* transmit address of RtStartAdr */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (DSP_MEM_ADD_RTSTARTADR >> 8) & 0xFF );
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_MEM_ADD_RTSTARTADR & 0xFF);
		}
		
		/* transmit top address of transfer source */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bStartAdd[0]);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bStartAdd[1]);
		}
		
		/* transmit size */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(0x00);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (UC)(wWordSize - 1) );
		}
		
		/* output start conditions again */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		}
		
		/* request runtime transfer */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_REG_ADD_MDSPREQ);
		}
		
		if( bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_REG_DATA_RTUL);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	/* when runtime transfer processing is successful */
	if(bResult == SUCCESS)
	{
		/* obtain processing result of DSP */
		bResult = CheckMDSPREQ();
		
		/* when processing for DSP is failed */
		if(bResult != SUCCESS)
		{
			return(bResult);
		}
	}
	
	wNakCount = 0;
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit WRITE/READ command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_WRITEREAD);
		}
		
		/* transmit address of RtData0 */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (DSP_MEM_ADD_RTDATA0 >> 8) & 0xFF );
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_MEM_ADD_RTDATA0 & 0xFF);
		}
		
		/* transmit zero */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(0x00);
		}
		
		/* output start conditions again */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_READ);
		}
		
		/* acquire data from RtData*(*=0-9) */
		if(bResult == SUCCESS)
		{
			bResult = Uart0BurstIn( (UL)pbData, dwByteSize);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* ************************************************************************ */
/* SDSPCoefBT																*/
/*		parameter:															*/
/*			unsigned short		wAdd										*/
/*								(top address of the transfer destination)	*/
/*																			*/
/*			*unsigned char		pbData										*/
/*								(setting data)								*/
/*																			*/
/*			unsigned short		wWordSize									*/
/*								(data size.word by word.)					*/
/*																			*/
/*		return value:														*/
/*			unsigned char		processing result							*/
/*																			*/
/*		description:														*/
/*			set PEQ coefficient RAM by batch transfer.						*/
/* ************************************************************************ */
UC SDSPCoefBT(US wAdd, UC *pbData, US wWordSize)
{
	UL dwByteSize, dwTmp;
	UC bStartAdd[2];
	UC bPEQPrameter, bResult;
	US wNakCount = 0;
	
	/* check size */
	if( (wWordSize != 5) && (wWordSize != 10) )
	{
		return(ERROR_PARAM);
	}
		
	/* check area */
	dwTmp = (UL)wAdd + (UL)wWordSize - 1;
	if( (wAdd < DSP_MEM_ADD_PEQCOEFTOP) || (dwTmp > (DSP_MEM_ADD_PEQCOEFEND)) )
	{
		return(ERROR_PARAM);
	}

	/* prepare transfer parameter */
	if(wWordSize == 0x5)
		bPEQPrameter = ( 0x00 | (wAdd & DSP_RAM_MASK_PEQAD) );
	else
		bPEQPrameter = ( 0x80 | (wAdd & DSP_RAM_MASK_PEQAD) );
	
	/* convert size by words to by bytes */
	dwByteSize = GetDSPDataSize(wAdd, wWordSize);
	
	while(1)
	{
		/* output start condition */
		bResult = Uart0Start(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		
		/* transmit WRITE/READ command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_WRITEREAD);
		}
		
		/* transmit PEQ coefficient buffer address */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (DSP_MEM_ADD_PEQBUFTOP >> 8) & 0xFF );
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_MEM_ADD_PEQBUFTOP & 0xFF);
		}
		
		/* transfer data */
		if(bResult == SUCCESS)
		{
			bResult = Uart0BurstOut( (UL)pbData, dwByteSize);
		}
		
		/* output start conditions again */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Restart(I2C_SLAVE_ADD_DSP, I2C_WRITE);
		}
		
		/* transmit WRITE/READ command */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_COM_WRITEREAD);
		}
		
		/* transmit address of transfer parameter */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out( (DSP_MEM_ADD_PEQPRAMETER >> 8) & 0xFF );
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(DSP_MEM_ADD_PEQPRAMETER & 0xFF);
		}
		
		/* transmit prepare transfer parameter */
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(bPEQPrameter);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(0x00);
		}
		
		if(bResult == SUCCESS)
		{
			bResult = Uart0Out(0x00);
		}
		
		/* output stop condition */
		Uart0Stop();
		
		if(bResult == ERROR_I2C)
		{
			if(wNakCount++ >= I2C_RETRYCOUNT)
			{
				return(bResult);
			}
		}
		else
		{
			break;
		}
	}
	
	return(bResult);
}

/* end of file*/
>>>>>>> origin
