<<<<<<< HEAD
/* 	CONFIDENTIAL
*********************************************************************************

	File:			dsp_access.h
	Version:		1.00.00

	Description:	DSP Access Processing Header File

	Copyright(c) 2011 Yamaha Corporation

*********************************************************************************/

/* I2C slave address definition */
#define	I2C_SLAVE_ADD_DSP	0x6C		/* DD-2SP/SPR-2 */

/* number of I2C communication retransmission */
#define I2C_RETRYCOUNT		10

/* DSP control command code */
#define DSP_COM_WRITEREAD	0x80
#define DSP_COM_PRROMSTOP	0x43

/* definitions related to PRESETROM command */
#define DSP_ROM_NUM_MIN		0x000
#define DSP_ROM_NUM_MAX		0x3FF
#define PRESETROM0_END		0x0FF
#define PRESETROM1_END		0x1FF
#define PRESETROM2_END		0x2FF
#define PRESETROM3_END		0x3FF

/* DSP register address */
#define DSP_REG_ADD_TOP			0x00	/* register top */
#define DSP_REG_ADD_END			0x3F	/* register end */
#define DSP_REG_ADD_MDSPREQ		0x13	/* MDSPREQ */

/* DSP register bit mask */
#define DSP_REG_MASK_MDSPERR	0x01 	/* MDSPERR */
#define DSP_REG_MASK_SDSPERR	0x01 	/* SDSPERR */
#define DSP_REG_MASK_PLLERR		0x01 	/* PLLERR */

/* DSP register setting data */
#define DSP_REG_DATA_RTREADY	0x00
#define DSP_REG_DATA_RTDL		0x02
#define DSP_REG_DATA_RTUL		0x03

/* DSP memory address */
#define DSP_MEM_ADD_RAMTOP		0x0000	/* embedded RAM top */
#define DSP_MEM_ADD_RAMEND		0xFFFF	/* embedded RAM end */
#define DSP_MEM_ADD_RTSTARTADR	0x0000	/* RtStartAdr */
#define DSP_MEM_ADD_RTDATA0		0x0002	/* RtData0 */
#define DSP_MEM_ADD_WRAM0END	0x001F	/* WRAM0 end */
#define DSP_MEM_ADD_WRAM1END	0x03FF	/* WRAM1 end */
#define DSP_MEM_ADD_WRAM2END	0x3FFF	/* WRAM2 end */
#define DSP_MEM_ADD_CRAMTOP		0x4000	/* CRAM top */
#define DSP_MEM_ADD_CRAMEND		0x43FF	/* CRAM end */
#define DSP_MEM_ADD_MPRAMEND	0x49FF	/* MPRAM end */
#define DSP_MEM_ADD_QBUSEND		0x7FFF	/* QBUS end */
#define DSP_MEM_ADD_PEQCOEFTOP	0x8000	/* PEQ coefficient area top */
#define DSP_MEM_ADD_PEQCOEFEND	0x8045	/* PEQ coefficient area end */
#define DSP_MEM_ADD_PEQBUFTOP	0x8046	/* PEQ transfer buffer top */
#define DSP_MEM_ADD_PEQPRAMETER	0x8050	/* PEQ transfer parameter area */
#define DSP_MEM_ADD_PEQRAMEND	0xBFFF	/* PEQ coefficient RAM End */

/* DSP memory data mask */
#define DSP_RAM_MASK_PEQAD		0x7F	/* bit[6:0]=1 */

/* definitions related to transfer */
#define TRANS_MAX_SIZE	(US)(2048)		/* maximum transfer size */

/* definitions related to runtime transfer */
#define RT_MAX_SIZE		(US)(10)		/* maximum transfer size */
#define RT_MAX_WAIT		(UL)(21)		/* maximum processing time */

/* proto type declaration */
UC CheckError(void);
UC DSPWriteReg(UC bAdd, UC bData);
UC DSPReadReg(UC bAdd, UC *pbData);
UC DSPPartWriteReg(UC bAdd, UC bData, UC bMask);
UC DSPLoadPreset(US wNum, US wAdd, UC bSetAdd);
UC DSPStopPreset(void);
UC DSPWriteMem(US wAdd, UC *pbData, US wWordSize);
UC DSPReadMem(US wAdd, UC *pbData, US wWordSize);
UC MDSPCoefRTDL(US wAdd, UC *pbData, US wWordSize);
UC MDSPCoefRTUL(US wAdd, UC *pbData, US wWordSize);
UC SDSPCoefBT(US wAdd, UC *pbData, US wWordSize);

/* end of file */
=======
/* 	CONFIDENTIAL
*********************************************************************************

	File:			dsp_access.h
	Version:		1.00.00

	Description:	DSP Access Processing Header File

	Copyright(c) 2011 Yamaha Corporation

*********************************************************************************/

/* I2C slave address definition */
#define	I2C_SLAVE_ADD_DSP	0x6C		/* DD-2SP/SPR-2 */

/* number of I2C communication retransmission */
#define I2C_RETRYCOUNT		10

/* DSP control command code */
#define DSP_COM_WRITEREAD	0x80
#define DSP_COM_PRROMSTOP	0x43

/* definitions related to PRESETROM command */
#define DSP_ROM_NUM_MIN		0x000
#define DSP_ROM_NUM_MAX		0x3FF
#define PRESETROM0_END		0x0FF
#define PRESETROM1_END		0x1FF
#define PRESETROM2_END		0x2FF
#define PRESETROM3_END		0x3FF

/* DSP register address */
#define DSP_REG_ADD_TOP			0x00	/* register top */
#define DSP_REG_ADD_END			0x3F	/* register end */
#define DSP_REG_ADD_MDSPREQ		0x13	/* MDSPREQ */

/* DSP register bit mask */
#define DSP_REG_MASK_MDSPERR	0x01 	/* MDSPERR */
#define DSP_REG_MASK_SDSPERR	0x01 	/* SDSPERR */
#define DSP_REG_MASK_PLLERR		0x01 	/* PLLERR */

/* DSP register setting data */
#define DSP_REG_DATA_RTREADY	0x00
#define DSP_REG_DATA_RTDL		0x02
#define DSP_REG_DATA_RTUL		0x03

/* DSP memory address */
#define DSP_MEM_ADD_RAMTOP		0x0000	/* embedded RAM top */
#define DSP_MEM_ADD_RAMEND		0xFFFF	/* embedded RAM end */
#define DSP_MEM_ADD_RTSTARTADR	0x0000	/* RtStartAdr */
#define DSP_MEM_ADD_RTDATA0		0x0002	/* RtData0 */
#define DSP_MEM_ADD_WRAM0END	0x001F	/* WRAM0 end */
#define DSP_MEM_ADD_WRAM1END	0x03FF	/* WRAM1 end */
#define DSP_MEM_ADD_WRAM2END	0x3FFF	/* WRAM2 end */
#define DSP_MEM_ADD_CRAMTOP		0x4000	/* CRAM top */
#define DSP_MEM_ADD_CRAMEND		0x43FF	/* CRAM end */
#define DSP_MEM_ADD_MPRAMEND	0x49FF	/* MPRAM end */
#define DSP_MEM_ADD_QBUSEND		0x7FFF	/* QBUS end */
#define DSP_MEM_ADD_PEQCOEFTOP	0x8000	/* PEQ coefficient area top */
#define DSP_MEM_ADD_PEQCOEFEND	0x8045	/* PEQ coefficient area end */
#define DSP_MEM_ADD_PEQBUFTOP	0x8046	/* PEQ transfer buffer top */
#define DSP_MEM_ADD_PEQPRAMETER	0x8050	/* PEQ transfer parameter area */
#define DSP_MEM_ADD_PEQRAMEND	0xBFFF	/* PEQ coefficient RAM End */

/* DSP memory data mask */
#define DSP_RAM_MASK_PEQAD		0x7F	/* bit[6:0]=1 */

/* definitions related to transfer */
#define TRANS_MAX_SIZE	(US)(2048)		/* maximum transfer size */

/* definitions related to runtime transfer */
#define RT_MAX_SIZE		(US)(10)		/* maximum transfer size */
#define RT_MAX_WAIT		(UL)(21)		/* maximum processing time */

/* proto type declaration */
UC CheckError(void);
UC DSPWriteReg(UC bAdd, UC bData);
UC DSPReadReg(UC bAdd, UC *pbData);
UC DSPPartWriteReg(UC bAdd, UC bData, UC bMask);
UC DSPLoadPreset(US wNum, US wAdd, UC bSetAdd);
UC DSPStopPreset(void);
UC DSPWriteMem(US wAdd, UC *pbData, US wWordSize);
UC DSPReadMem(US wAdd, UC *pbData, US wWordSize);
UC MDSPCoefRTDL(US wAdd, UC *pbData, US wWordSize);
UC MDSPCoefRTUL(US wAdd, UC *pbData, US wWordSize);
UC SDSPCoefBT(US wAdd, UC *pbData, US wWordSize);

/* end of file */
>>>>>>> origin
