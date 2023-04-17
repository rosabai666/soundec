<<<<<<< HEAD
/* 	CONFIDENTIAL
*********************************************************************************

	File:			ioctl.c
	Version:		1.00.00
	Description:	Input/Output Control Processing

	Copyright(c) 2011 Yamaha Corporation

*********************************************************************************/
#include "common.h"
#include "ioctrl.h"


/* **************************************************** */
/* Uart0Start											*/
/*		parameter:										*/
/* 			unsigned char		bSlaveAddress			*/
/* 								(I2C slave address)		*/
/* 														*/
/* 			unsigned char		bRW						*/
/* 								(0=write, 1=read)		*/
/*		return value:									*/
/* 			unsigned char		processing result		*/
/* 														*/
/*		description:									*/
/* 			set start condition.						*/
/* **************************************************** */
UC Uart0Start(UC bSlaveAddress, UC bRW)
{
/*
	This function depends on the device.
	Please write the program code which realizes the following.
	
	1) To I2C bus, set start condition, and output DD-2SP/SPR-2's I2C slave address (0x6C) and Read/Write bit.
	   
	   if write (bRW=0) : start condition + 0xD8
	   if read  (bRW=1) : start condition + 0xD9
	   
	2) If a result of 1) is normal (acknowledge is returned), return "SUCCESS".
	   Other than the above, return "ERROR_I2C".
*/
}

/* **************************************************** */
/* Uart0Stop											*/
/*		parameter:										*/
/* 			none										*/
/* 														*/
/*		return value:									*/
/* 			none										*/
/* 														*/
/*		description:									*/
/* 			set stop condition.							*/
/* **************************************************** */
void Uart0Stop(void)
{
/*
	This function depends on the device.
	Please write the program code which realizes the following.
	
	- To I2C bus, set stop condition.
*/
}

/* **************************************************** */
/* Uart0Restart											*/
/*		parameter:										*/
/* 			unsigned char		bSlaveAddress			*/
/* 								(I2C slave address)		*/
/* 														*/
/* 			unsigned char		bRW						*/
/* 								(0=write, 1=read)		*/
/*		return value:									*/
/* 			unsigned char		processing result		*/
/* 														*/
/*		description:									*/
/* 			set restart condition.						*/
/* **************************************************** */
UC Uart0Restart(UC bSlaveAddress, UC bRW)
{
	return Uart0Out((bSlaveAddress << 1) | bRW);
}

/* **************************************************** */
/* Uart0Out												*/
/*		parameter:										*/
/* 			unsigned char		bOutdata				*/
/* 								(output data)			*/
/*		return value:									*/
/* 			unsigned char		processing result		*/
/* 														*/
/*		description:									*/
/* 			output 1 byte data.							*/
/* **************************************************** */
UC Uart0Out(UC bOutdata)
{
/*
	This function depends on the device.
	Please write the program code which realizes the following.
	
	1) To I2C bus, output 1 byte data.
	   
	2) If a result of 1) is normal (acknowledge is returned), return "SUCCESS".
	   Other than the above, return "ERROR_I2C".
*/
}

/* **************************************************** */
/* Uart0In												*/
/*		parameter:										*/
/* 			*unsigned char		pbInData				*/
/* 								(receive data pointer)	*/
/* 														*/
/*		return value:									*/
/* 			unsigned char		processing result		*/
/* 														*/
/*		description:									*/
/* 			receive 1 byte data. 						*/
/* **************************************************** */
UC Uart0In(UC *pbInData)
{
/*
	This function depends on the device.
	Please write the program code which realizes the following.
	
	1) From I2C bus, recive 1 byte data.
	   
	2) If a result of 1) is normal, output acknowledge to I2C bus, and return "SUCCESS".
	   Other than the above, output negative acknowledge to I2C bus, and return "ERROR_I2C".
*/
}

/* **************************************************** */
/* Uart0BurstOut										*/
/*		parameter:										*/
/* 			unsigned long		dwAddress				*/
/* 								(source address)		*/
/* 														*/
/* 			unsigned long		dwSize					*/
/* 								(output data size)		*/
/* 														*/
/*		return value:									*/
/* 			unsigned char		processing result		*/
/* 														*/
/*		description:									*/
/* 			output many bytes data. 					*/
/* **************************************************** */
UC Uart0BurstOut(UL dwAddress, UL dwSize)
{
/*
	This function depends on the device.
	Please write the program code which realizes the following.
	
	1) To I2C bus, output "dwSize" bytes data continuously.
	   
	2) If a result of 1) is normal (acknowledge is returned), return "SUCCESS".
	   Other than the above, return "ERROR_I2C".
*/
}

/* **************************************************** */
/* Uart0BurstIn											*/
/*		parameter:										*/
/* 			unsigned long		dwAddress				*/
/* 								(destination address)	*/
/* 														*/
/* 			unsigned long		dwSize					*/
/* 								(receive data size)		*/
/* 														*/
/*		return value:									*/
/* 			unsigned char		processing result		*/
/* 														*/
/*		description:									*/
/* 			receive many bytes data. 					*/
/* **************************************************** */
UC Uart0BurstIn(UL dwAddress, UL dwSize)
{
/*
	This function depends on the device.
	Please write the program code which realizes the
	following.
	
	1) From I2C bus, receive "dwSize" bytes data continuously.
	   Received data are stored in order from "dwAddress" address.
	   
	2) If a result of 1) is normal, output acknowledge to I2C bus, and return "SUCCESS".
	   Other than the above, output negative acknowledge to I2C bus, and return "ERROR_I2C".
*/
}

/* end of file */
=======
/* 	CONFIDENTIAL
*********************************************************************************

	File:			ioctl.c
	Version:		1.00.00
	Description:	Input/Output Control Processing

	Copyright(c) 2011 Yamaha Corporation

*********************************************************************************/
#include "common.h"
#include "ioctrl.h"


/* **************************************************** */
/* Uart0Start											*/
/*		parameter:										*/
/* 			unsigned char		bSlaveAddress			*/
/* 								(I2C slave address)		*/
/* 														*/
/* 			unsigned char		bRW						*/
/* 								(0=write, 1=read)		*/
/*		return value:									*/
/* 			unsigned char		processing result		*/
/* 														*/
/*		description:									*/
/* 			set start condition.						*/
/* **************************************************** */
UC Uart0Start(UC bSlaveAddress, UC bRW)
{
/*
	This function depends on the device.
	Please write the program code which realizes the following.
	
	1) To I2C bus, set start condition, and output DD-2SP/SPR-2's I2C slave address (0x6C) and Read/Write bit.
	   
	   if write (bRW=0) : start condition + 0xD8
	   if read  (bRW=1) : start condition + 0xD9
	   
	2) If a result of 1) is normal (acknowledge is returned), return "SUCCESS".
	   Other than the above, return "ERROR_I2C".
*/
}

/* **************************************************** */
/* Uart0Stop											*/
/*		parameter:										*/
/* 			none										*/
/* 														*/
/*		return value:									*/
/* 			none										*/
/* 														*/
/*		description:									*/
/* 			set stop condition.							*/
/* **************************************************** */
void Uart0Stop(void)
{
/*
	This function depends on the device.
	Please write the program code which realizes the following.
	
	- To I2C bus, set stop condition.
*/
}

/* **************************************************** */
/* Uart0Restart											*/
/*		parameter:										*/
/* 			unsigned char		bSlaveAddress			*/
/* 								(I2C slave address)		*/
/* 														*/
/* 			unsigned char		bRW						*/
/* 								(0=write, 1=read)		*/
/*		return value:									*/
/* 			unsigned char		processing result		*/
/* 														*/
/*		description:									*/
/* 			set restart condition.						*/
/* **************************************************** */
UC Uart0Restart(UC bSlaveAddress, UC bRW)
{
	return Uart0Out((bSlaveAddress << 1) | bRW);
}

/* **************************************************** */
/* Uart0Out												*/
/*		parameter:										*/
/* 			unsigned char		bOutdata				*/
/* 								(output data)			*/
/*		return value:									*/
/* 			unsigned char		processing result		*/
/* 														*/
/*		description:									*/
/* 			output 1 byte data.							*/
/* **************************************************** */
UC Uart0Out(UC bOutdata)
{
/*
	This function depends on the device.
	Please write the program code which realizes the following.
	
	1) To I2C bus, output 1 byte data.
	   
	2) If a result of 1) is normal (acknowledge is returned), return "SUCCESS".
	   Other than the above, return "ERROR_I2C".
*/
}

/* **************************************************** */
/* Uart0In												*/
/*		parameter:										*/
/* 			*unsigned char		pbInData				*/
/* 								(receive data pointer)	*/
/* 														*/
/*		return value:									*/
/* 			unsigned char		processing result		*/
/* 														*/
/*		description:									*/
/* 			receive 1 byte data. 						*/
/* **************************************************** */
UC Uart0In(UC *pbInData)
{
/*
	This function depends on the device.
	Please write the program code which realizes the following.
	
	1) From I2C bus, recive 1 byte data.
	   
	2) If a result of 1) is normal, output acknowledge to I2C bus, and return "SUCCESS".
	   Other than the above, output negative acknowledge to I2C bus, and return "ERROR_I2C".
*/
}

/* **************************************************** */
/* Uart0BurstOut										*/
/*		parameter:										*/
/* 			unsigned long		dwAddress				*/
/* 								(source address)		*/
/* 														*/
/* 			unsigned long		dwSize					*/
/* 								(output data size)		*/
/* 														*/
/*		return value:									*/
/* 			unsigned char		processing result		*/
/* 														*/
/*		description:									*/
/* 			output many bytes data. 					*/
/* **************************************************** */
UC Uart0BurstOut(UL dwAddress, UL dwSize)
{
/*
	This function depends on the device.
	Please write the program code which realizes the following.
	
	1) To I2C bus, output "dwSize" bytes data continuously.
	   
	2) If a result of 1) is normal (acknowledge is returned), return "SUCCESS".
	   Other than the above, return "ERROR_I2C".
*/
}

/* **************************************************** */
/* Uart0BurstIn											*/
/*		parameter:										*/
/* 			unsigned long		dwAddress				*/
/* 								(destination address)	*/
/* 														*/
/* 			unsigned long		dwSize					*/
/* 								(receive data size)		*/
/* 														*/
/*		return value:									*/
/* 			unsigned char		processing result		*/
/* 														*/
/*		description:									*/
/* 			receive many bytes data. 					*/
/* **************************************************** */
UC Uart0BurstIn(UL dwAddress, UL dwSize)
{
/*
	This function depends on the device.
	Please write the program code which realizes the
	following.
	
	1) From I2C bus, receive "dwSize" bytes data continuously.
	   Received data are stored in order from "dwAddress" address.
	   
	2) If a result of 1) is normal, output acknowledge to I2C bus, and return "SUCCESS".
	   Other than the above, output negative acknowledge to I2C bus, and return "ERROR_I2C".
*/
}

/* end of file */
>>>>>>> origin
