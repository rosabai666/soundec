<<<<<<< HEAD
/* 	CONFIDENTIAL
*********************************************************************************

	File:			ioctl.h
	Version:		1.00.00
	Description:	Input/Output Control Processing Header

	Copyright(c) 2011 Yamaha Corporation

*********************************************************************************/

/* definition for I2C access */
#define I2C_WRITE	0
#define I2C_READ	1

/* proto type declaration */
UC Uart0Start(UC bSlaveAddress, UC bRW);
void Uart0Stop(void);
UC Uart0Restart(UC bSlaveAddress, UC bRW);
UC Uart0Out(UC bOutdata);
UC Uart0In(UC *pbInData);
UC Uart0BurstOut(UL dwAddress, UL dwSize);
UC Uart0BurstIn(UL dwAddress, UL dwSize);

/* end of file */
=======
/* 	CONFIDENTIAL
*********************************************************************************

	File:			ioctl.h
	Version:		1.00.00
	Description:	Input/Output Control Processing Header

	Copyright(c) 2011 Yamaha Corporation

*********************************************************************************/

/* definition for I2C access */
#define I2C_WRITE	0
#define I2C_READ	1

/* proto type declaration */
UC Uart0Start(UC bSlaveAddress, UC bRW);
void Uart0Stop(void);
UC Uart0Restart(UC bSlaveAddress, UC bRW);
UC Uart0Out(UC bOutdata);
UC Uart0In(UC *pbInData);
UC Uart0BurstOut(UL dwAddress, UL dwSize);
UC Uart0BurstIn(UL dwAddress, UL dwSize);

/* end of file */
>>>>>>> origin
