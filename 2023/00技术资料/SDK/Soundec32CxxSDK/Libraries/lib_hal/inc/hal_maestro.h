/*
 * maestro.h
 *
 *  Created on: 2019Äê11ÔÂ17ÈÕ
 *      Author: 86186
 */

#ifndef __MAESTRO_H__
#define __MAESTRO_H__

#include "snc8600.h"
/********************************************/
#define dbg_test
#define MAESTRO_DLC_INT_VECTOR       13

/********************************************/

/******************SCU*****************************/
/******************SCU*****************************/
/******************SCU*****************************/

/*Shift Control Unit SCU*/
/*
	The SCU is a configurable block managing sequences by sequentially sending write orders on PICL bus.
	The SCU manages:
	1) the start-up sequence (SUS) of the SoC which is triggered by the removal of the reset;
	2) 32 sequences triggered by events controlled by the WIU.
*/




/******************WIU*****************************/
/******************WIU*****************************/
/******************WIU*****************************/
/*
* Maestro WIU, ICUs and DMUs blocks are mapped on PICL bus.
*  1. BLOCK_ADDR (DLC_PCTRL.PADDR[13:6]): 8-bit PICL block selection offset;
*  2. REG_ADDR (DLC_PCTRL.PADDR[5:0]): 6-bit PICL block register offset.
*/
/*
 * The aim of the WIU module is to handle the SoC change mode request and to send it to the SCU.
 * Change mode request can be received from interrupts signals (refer to section 7.2) or from a register
 * access (refer to section 6.3.1).
 * The WIU is powered by the Always-On (AO) power supply.
 * The WIU supports up to 16 interrupts.
*/
/*Base address for WIU*/
#define MAESTRO_WIU_ADDRESS_BASE			          0x01	// Addr: 0x01
/*mapping to address of WIU_0 */
#define MAESTRO_WIU_ADDRESS(wiu_index)			   (MAESTRO_WIU_ADDRESS_BASE+wiu_index)
/* Here is WIU offset from 0-31, Use MAESTRO_WIU_ADDRESS(WIU_index) to mapping to WIU addres: 0x10-0x2F*/
typedef enum{
  WIU_0 = 0,
  /* YOU CAN add WIU_index you used from WIU_0 to WIU_MAX*/
  WIU_MAX = 0, /*Just have 1 WIU here*/
}wiu_offset_t;


/* WIU interrupts
* This section presents Maestro input interrupts dedicated to generate a configured sequence.
*/


/* WIU registers
The WIU registers are mapped on the PICL bus. Refer to section 5.2 to know how to reach these
registers.
*/
#define WIU_ICR_MAX_SIZE    16
typedef enum{
	WIU_ISPMR_0  =  0x00, /*Interrupt Sequence Processing Mask Register 0*/
	WIU_ISPMR_1  =  0x01, /*Interrupt Sequence Processing Mask Register 1*/
	WIU_IFR_0      =  0x02, /*Interrupt Flag Register 0*/
	WIU_IFR_1      =  0x03, /*Interrupt Flag Register 1*/
	WIU_ICR_0      =  0x04,  /*Interrupt Control Register 0*/
	WIU_ICR_1      =  0x05,  /*Interrupt Control Register 1*/
	WIU_ICR_2      =  0x06,   /*Interrupt Control Registe 2 */
	WIU_ICR_3     =	  0x07,  /*Interrupt Control Register 3*/
	WIU_ICR_4     =	  0x08,  /*Interrupt Control Register 4*/
	WIU_ICR_5      =  0x09,  /*Interrupt Control Register 5*/
	WIU_ICR_6      =  0x0A,  /*Interrupt Control Register 6*/
	WIU_ICR_7      =  0x0B,  /*Interrupt Control Register 7*/
	WIU_ICR_8      =  0x0C,  /*Interrupt Control Register 8*/
	WIU_ICR_9      =  0x0D,  /*Interrupt Control Register 9*/
	WIU_ICR_10    =  0x0E,   /*Interrupt Control Register 10*/
	WIU_ICR_11    =  0x0F,   /*Interrupt Control Register 11*/
	WIU_ICR_12    =  0x10,   /*Interrupt Control Register 12*/
	WIU_ICR_13    =  0x11,   /*Interrupt Control Register 13*/
	WIU_ICR_14    =  0x12,   /*Interrupt Control Register 14*/
	WIU_ICR_15   =	 0x13,    /*Interrupt Control Register  15*/
}WIU_registers_t;

/*WIU_ISPMR: Interrupt Sequence Processing Mask Register 0*/
typedef union{
	U16 ispmrVal;
	struct{
		U8 WIU_IRQ_M_L; /*WIU_ISPMR_0: WIU_IRQ_7 (bit 7) downto WIU_IRQ_0 (bit 0) masks, active high.*/
		U8 WIU_IRQ_M_M; /*WIU_ISPMR_1: WIU_IRQ_15 (bit 7) downto WIU_IRQ_8 (bit 0) masks, active high.*/
	}bytes;
	struct{
	  U16 ispmrBit0:1;
	  U16 ispmrBit1:1;
	  U16 ispmrBit2:1;
	  U16 ispmrBit3:1;
	  U16 ispmrBit4:1;
	  U16 ispmrBit5:1;
	  U16 ispmrBit6:1;
	  U16 ispmrBit7:1;
	  U16 ispmrBit8:1;
	  U16 ispmrBit9:1;
	  U16 ispmrBit10:1;
	  U16 ispmrBit11:1;
	  U16 ispmrBit12:1;
	  U16 ispmrBit13:1;
	  U16 ispmrBit14:1;
	  U16 ispmrBit15:1;
	  U16 reserved:16;
	}bits;
}WIU_ISPMR_t;

/*WIU_IFR: Interrupt Flag Register*/
/*WIU IRQ flags are cleared when:
 *1  its respective SCU sequence is finished;
 *2  it is unmasked.
 */
typedef union{
	/*
	flags Each bit is set when writing it to 1 or when detecting a rising edge on the corresponding WIU IRQ input.
	Each bit is cleared when the associated SCU sequence is finished or when the corresponding mask is cleared.
	The lowest value corresponds to the highest priority.
	Bit 0 of WIU_IRQ_F field of register WIU_IFR_0 has the highest priority.
	*/
	struct{
		U8 WIU_IRQ_F_L;	/*WIU_IRQ_7 (bit 7 ) downto WIU_IRQ_0 (bit 0)*/
		U8 WIU_IRQ_F_M;	/*WIU_IRQ_15 (bit 7) downto WIU_IRQ_8 (bit 0)*/
	}bytes;
	U16 ifrVal;
	U16 WIU_IRQ_F;
	struct{
		  U16 ifrBit0:1;
		  U16 ifrBit1:1;
		  U16 ifrBit2:1;
		  U16 ifrBit3:1;
		  U16 ifrBit4:1;
		  U16 ifrBit5:1;
		  U16 ifrBit6:1;
		  U16 ifrBit7:1;
		  U16 ifrBit8:1;
		  U16 ifrBit9:1;
		  U16 ifrBit10:1;
		  U16 ifrBit11:1;
		  U16 ifrBit12:1;
		  U16 ifrBit13:1;
		  U16 ifrBit14:1;
		  U16 ifrBit15:1;
		  U16 reserved:16;
	  }bits;
}WIU_IFR_t;

/*WIU_ICR_x: Interrupt Control Register x*/
typedef struct{
  U8 SEQ_SEL_IRQ:4;   /*Defines the sequence to execute for WIU IRQ[0:15]*/
  U8 rev:4;
}WIU_ICR_t;
extern WIU_ICR_t SEQ_SEL_IRQ[WIU_ICR_MAX_SIZE];


/******************WIU*****************************/
/******************WIU*****************************/
/******************WIU*****************************/


/******************ICU*****************************/
/******************ICU*****************************/
/******************ICU*****************************/
/*
 * Island Control Unit (ICU)
 * The ICU is a local mode controller associated to an island of the SoC, and manages the mode
 * transition sequences.
*/

/*Base address for ICU*/
#define MAESTRO_ICU_ADDRESS_BASE			          0x10
/*mapping to address of each ICU from ICU_0 to ICU_MAX, @ref to icu_offset_t */
#define MAESTRO_ICU_ADDRESS(icu_offset)			   (MAESTRO_ICU_ADDRESS_BASE+icu_offset)

/* Here is ICU offset from 0-31, Use MAESTRO_ICU_ADDRESS(ICU_offset) to mapping to ICU addres: 0x10-0x2F*/
typedef enum{
	ICU_0 = 0,
	ICU_1,
	ICU_2,
	ICU_3,
	ICU_4,
	/* YOU CAN add ICU_index you used from ICU_0 to ICU_MAX*/
	ICU_MAX,  //Up to 31,
}icu_offset_t;

/******ICU registers******/
/* The ICU registers are mapped on PICL bus.*/
/* Each ICU contains registers like bellow. The ICU block from 0-31, Offset address from 0x10-0x2F */
typedef enum{
	ICU_MT_SR     =    0x00,   /*ICU Mode Table Status Register*/
	ICU_MT_CR     =    0x01, /*ICU Mode Table Control Register*/
	ICU_IM_SR      =    0x02, /*Island Mode Status Register*/
	ICU_CGM_SR   =    0x03,  /*Clock Generator Mode Status Register*/
	ICU_RM_SR     =    0x04,  /*Regulator Mode Status Register*/
	ICU_BBGM_SR  =    0x05,  /*Body Biasing Generator Mode Status Register*/
	ICU_BBGM_CR  =    0x06,  /*Body Biasing Generator Mode Control Register*/
	ICU_CGO_CR		=	  0x07,  /*Clock Generator Order Control Register: To set PLL value step by step*/
	ICU_RO_CR		=	  0x08,  /*Regulator Order Control Register: To set Regulator value step by step*/
	ICU_BBGO_CR  =    0x09,  /*Body Biasing Generator Order Control Register*/
	ICU_RPC_CR    =    0x0A,  /*Retention Power Controller Control Register*/
	ICU_EPC_CR    =    0x0B,  /*Extinction Power Controller Control Register*/

}ICU_registers_t;

/*Steps from 1 to 4 to adjust.
  [1]: -Current clock generator order "+ or -" 1]
  [2]: -Current clock generator order "+ or -" 2]
  [3]: -Current clock generator order "+ or -" 3]
  [4]: -Current clock generator order "+ or -" 4]
*/
typedef enum{
	ICU_CGO_STEP_1 = 0,
	ICU_CGO_STEP_2,
	ICU_CGO_STEP_3,
	ICU_CGO_STEP_4,
	ICU_CGO_STEP_MAX,
}ICU_cgo_steps_t;

/******ICU registers******/
typedef union
{
	struct
	{
	  /*Current ICU Mode
        Returns the ICU current mode from ICUMT when ICUMTS is low.*/
	 U8 CICUM:4;

     /*ICU Mode Table Status
        When high, indicates that the ICU current mode is not defined in the ICU mode table.
        When low, the value of the current mode is given by the CICUM field.*/
     U8 ICUMTS:1;

     U8 rev:3;
   }bits;

	U8 byte;
}ICU_MT_SR_st;

typedef union
{
	struct
	{
	  /*ICU Mode Table Request
        Writing a mode starts an ICU transition sequence.*/
	  U8 ICUMTR:4;
      U8 rev:4;
   }bits;

	U8 byte;
}ICU_MT_CR_st;

typedef union
{
  /*ICU_CGO_CR: Clock generator current order increment and decrement commands are only available in NF, MF
  and LF modes (an ICU_DENIED is sent in other modes).*/
	struct
	{
		U8 CGOIS:2;  // Clock Generator Order Incrementation Setting
		U8 CGODS:2;		// Clock Generator Order Decrementation Setting
		/*
		 * Clock Generator Order Incrementation Request
		Set to apply the order defined in CGOIS field.
		This field is automatically cleared when the request is
		done.
		Setting or clearing this field when it is already set has
		no effect.
		*/
		U8 CGOIR:1;
		/*
		 * Clock Generator Order Decrementation Request
		Set to apply the order defined in CGODS field.
		This field is automatically cleared when the request is
		done.
		Setting or clearing this field when it is already set has
		no effect.
		 * */
		U8 CGODR:1;
		U8 rev:2;
	}bits;
	U8 byte;
}ICU_CGO_CR_st;
/******************ICU*****************************/
/******************ICU*****************************/
/******************ICU*****************************/


/******************DMU*****************************/
/******************DMU*****************************/
/******************DMU*****************************/
/*The DMU block from 0-31, Offset address from 0x30-0x6F */

/*Base address for DMU*/
#define MAESTRO_DMU_ADDRESS_BASE			            0x30  // From 0 - 63, Addr: 0x30-0x6F

/*mapping to address of each DMU from DMU_0 to DMU_MAX, @ref to dmu_offset_t */
#define MAESTRO_DMU_ADDRESS(dmu_offset)			   		(MAESTRO_DMU_ADDRESS_BASE+dmu_offset)

/* Here is DMU offset from 0-31, Use MAESTRO_DMU_ADDRESS(DMU_offset) to mapping to DMU address: 0x10-0x2F*/
typedef enum{
	DMU_RAR = 0,
	DMU_eSR = 1,
	DMU_iLR1 = 2,
	DMU_iLR2 = 3,
	DMU_OPM = 4,
	DMU_PLL = 5,
	/*.....*/
	/* YOU CAN add DMU_index you used from DMU_0 to DMU_MAX*/
	/*.....*/
	DMU_MAX,  // Up to 31,
}dmu_offset_t;


/* DMU registers.*/
/* The DMU registers are mapped on PICL bus.*/
/*
 * These registers control the minimal and maximal orders sent to the resource, the order to send
 * according to each mode and each resource, and also the resource direct access control.
*/
/* Each DMU contains registers like bellow. */
typedef enum
{
	DMU_RSR =  0x00,      	/*DMU Resource Status Register*/
							/* 3 resources:
								  ¨C 1 regulator;
								  ¨C 1 clock generator;
								  ¨C 1 forward body biasing generator.
							*/

	DMU_MINOR = 0x01,   	/*DMU Minimal Order Register*/
							/*DMU_CGxO_CR registers are available only if the DMU is connected to a clock generator*/

	DMU_MAXOR = 0x02,   	/*DMU Maximal Order Register*/
	DMU_OCR = 0x03,       	/*DMU Order Control Register*/
		                    /* Order Update Request
								Bit[0] Set to 1 to apply the order set in DMU_*O_CR corresponding
								to the resource type and current mode.
								This field is automatically cleared when the request is done.
							*/

	DMU_CGNO_CR = 0x04, 	/*Clock Generator Nominal Order Control Register, for the order sent for NF mode;*/
	DMU_CGMO_CR = 0x05, 	/*Clock Generator Medium Order Control Register, for the order sent for MF mode;*/
	DMU_CGLO_CR = 0x06, 	/*Clock Generator Low Order Control Register, for the order sent for LF mode.*/

	/*DMU_RxO_CR registers are available only if the DMU is connected to a regulator*/
	DMU_RNO_CR = 0x07,  	/*Regulator Nominal Order Control Register, for the order sent for NV mode;*/
	DMU_RMO_CR = 0x08,   	/*Regulator Medium Order Control Register, for the order sent for MV mode;*/
	DMU_RLO_CR = 0x09,    	/*Regulator Low Order Control Register, for the order sent for LV mode;*/
	DMU_RRO_CR = 0x0A,    	/*Regulator Retention Order Control Register, for the order sent for RV mode.*/

	DMU_BBGOO_CR = 0x0B,  	/*Body Biasing Generator On Order Control Register*/

	/* Activation of the direct access is done in setting RDC field of DMU_TCR register. In this case,
	  requests from ICUs and from OUR field of DMU_OCR register are ignored and always accepted.
	*/
	DMU_TCR = 0x0C,			/*DMU Test Control Register, Activation of the direct access is done in setting RDC field of DMU_TCR register*/
	/* The resource mode and order must be set in DMU_TRR register*/
	DMU_TRR = 0x0D,			/*DMU Test Resource Register*/
}DMU_registers_t;


/******************DMU*****************************/
/******************DMU*****************************/
/******************DMU*****************************/


/******************SCU*****************************/
/******************SCU*****************************/
/******************SCU*****************************/
/*Shift Control Unit (SCU), powered by the Always-On (AO) power supply.*/
/* The SCU manages:
	1 the start-up sequence (SUS) of the SoC which is triggered by the removal of the reset;
	2 32 sequences triggered by events controlled by the WIU.

	The SCU is connected to the DLC with a PICL master interface to transmit the requests and with
	an interrupt (SCU_OK or SCU_FL) which gives the sequence status. The SCU is also connected
	to the WIU which generates the sequence trigger.
*/


/******************DLC Domain Link Connector********************/
/* The control of Maestro is performed through registers accessible directly by the control interface (in DLC bridge).*/
/********************************************/
/* Each data structure to maestro_registers_t */
/* DLC_PCTRL, RW, PICL Control Register */
/* PICL: The Power Island Control Link, through what we can access WIU ICU and DMU*/
typedef union{
	struct{
		//LSB
		/* This field remains high until the end of the sequence,
		which means that no new access can be performed if an access is on going. */
		U32 start:1;        /*Start of PICL access sequence.*/

		/*reg_addr combine with blk_addr: Address of the transfer on the PICL bus.*/
		U32 reg_addr:6;   	/*reg_addr: Registers address contains in  each block such as WIU\ICU\DMU\*/
		U32 blk_addr:8;    	/*blk_addr: Block address(index) of each block such as WIU\ICU\DMU\ */
		U32 dir:1;       	/*Direction of the transfer on the PICL bus, 0: write, 1: read*/
		U32 pwdata:8;     	/*Data to write on PICL bus. */
		U32 rev:8;
		//MSB
	}bits;
	U32 dwords;
}DLC_PCTRL_st;

/* DLC_PRDATA, RO, PICL Data Read Register */
typedef union{
	struct{
		//LSB
		U32 PRDATA:8;   	/*Valid only after a PICL read operation when the PICL_BUSY bit of the DLC_SR register becomes low.*/
		U32 rev:24;
		//MSB
	}bits;
	U32 dwords;
}DLC_PRDATA_st;

/*DLC_SR, RO, Status Register*/
typedef union{
	struct{
		//LSB
		U32 PICL_BUSY:1;
		U32 SCU_BUSY:1;
		U32 rev:30;
		//MSB
	}bits;
	U32 dwords;
}DLC_SR_st;

/*DLC_IMR, Interrupt Mask Register*/
/*This interrupt can be masked on o_irq_scu_ok output signal in setting the DLC_IMR interrupt to 1.*/
typedef union{
	struct{
		U32 ICU_OK_M:1; 		// Mask of ICU_OK interrupt.
		U32 ICU_DELAYED_M:1;	// Mask of ICU_DELAYED interrupt.
		U32 ICU_DENIED_M:1;		// Mask of ICU_DENIED interrupt.
		U32 ICU_UPDATED_M:1;	// Mask of ICU_UPDATED interrupt.
		U32 rev1:2;
		U32 PICL_OK_M:1;		// Mask of PICL_OK interrupt.
		U32 SCU_OK_M:1;			// Mask of SCU_OK interrupt.
		U32 SCU_FL_M:1;			// Mask of SCU_FL interrupt.
		U32 rev2:23;
	}bits;
	U32 dwords;
}DLC_IMR_st;

/* DLC_IFR, RW, Interrupt Flag Register*/
typedef union{
	struct{
      /**** ICU_OK interrupt signal, active high. Indicates that the ICU has reached the requested mode or order.*/
		/* Set when at least one of the bit of the DLC_IOK_IFR register is set. Cleared when reading DCL_IOK_IFR register.*/
		U32 ICU_OK_F:1;
      /**** ICU_DELAYED interrupt signal, active high. Indicates that the ICU can not reach instantly the requested mode or order */
		/*Set when at least one of the bit of the DLC_IDL_IFR register is set. Cleared when reading DCL_IDL_IFR register.*/
		U32 ICU_DELAYED_F:1;
      /**** ICU_DENIED interrupt signal, active high. Indicates that the ICU can not perform the requested mode or order.*/
		/* Set when at least one of the bit of the DLC_IDN_IFR register is set. Cleared when reading DCL_IDN_IFR register.*/
		U32 ICU_DENIED_F:1;
      /**** ICU_UPDATED interrupt signal, active high. Indicates that a mode or order has changed.*/
		/* Set when at least one of the bit of the DLC_IUP_IFR register is set. Cleared when reading DCL_IUP_IFR register.*/
		U32 ICU_UPDATED_F:1;
		U32 rev1:2;
      /**** PICL_OK interrupt signal, active high. Indicates that the indirect access register transaction has finished.*/
		/* Set when PICL transfer is finished. Cleared when writing 1 in this field.*/
		U32 PICL_OK_F:1;
      /**** SCU_OK interrupt signal, active high. Indicates that the SCU sequence has finished without error.*/
		/*Set when SCU sequence is finished without error. 	Cleared when writing 1 in this field.*/
		U32 SCU_OK_F:1;
      /**** SCU_FL interrupt signal, active high. Indicates that the SCU sequence has finished with error.*/
		/* Set when SCU sequence is finished with error. Cleared when writing 1 in this field.*/
		U32 SCU_FL_F:1;
		U32 rev2:23;
	}bits;
	U32 dwords;
}DLC_IFR_st;

/*DLC_IOK_IFR, RW, ICU_OK Interrupt Flag Register*/
typedef struct{
	/*Flags of the ICU_OK interrupts.
		Each bit is set if the requested mode change from the control
		interface on the corresponding ICU was performed.
		Reading this register clears each bit and the bit ICU_OK_F of the DLC_IFR register.*/
	U32 ICU_OK_FLAGS;
}DLC_IOK_IFR_st;

/*DLC_IDL_IFR, RW, ICU_DELAYED Interrupt Flag Register*/
typedef struct{
	/*Flags of the ICU_DELAYED interrupts.
		Each bit is set if the requested mode change from the
		control interface on the corresponding ICU was delayed.
		Reading this register clears each bit and the bit
		ICU_DELAYED_F of the DLC_IFR register*/
	U32 ICU_DELAYED_FLAGS;
}DLC_IDL_IFR_st;

/*DLC_IDN_IFR, RW, ICU_DENIED Interrupt Flag Register*/
typedef struct{
    /*Flags of the ICU_DENIED interrupts.
		Each bit is set if the requested mode change from
		the control interface on the corresponding ICU was
		denied.
		Reading this register clears each bit and the bit
		ICU_DENIED_F of the DLC_IFR register.*/
	U32 ICU_DENIED_FLAGS;
}DLC_IDN_IFR_st;

/*DLC_IUP_IFR, RW, ICU_UPDATED Interrupt Flag Register*/
typedef struct{
    /*Flags of the ICU_UPDATED interrupts.
		Each bit is set when the corresponding ICU
		changed its mode or order.
		Reading this register clears each bit and the bit
		ICU_UPDATED_F of the DLC_IFR register.*/
	U32 ICU_UPDATED_FLAGS;
}DLC_IUP_IFR_st;

// Structure for Maestro control and data registers
// Direct access registers
typedef struct
{
	U32 DLC_PCTRL;				// 0x00   PICL control register
	U32 DLC_PRDATA;				// 0x04   PICL read data register
	U32 DLC_SR;					// 0x08   Interrupt status register
	U32 DLC_IMR;				// 0x0C   Interrupt mask register
	U32 DLC_IFR;				// 0x10   Interrupt flags register
	U32 DLC_IOK_IFR;			// 0x14   ICU OK interrupt flags register, the ICUs which generate this flag are shown in DLC_IOK_IFR register.
	U32 DLC_IDL_IFR;			// 0x18   ICU delayed interrupt flags register, the ICUs which generate this flag are shown in DLC_IDL_IFR register.
	U32 DLC_IDN_IFR;			// 0x1C   ICU denied interrupt flags register, the ICUs which generate this flag are shown in DLC_IDN_IFR register.
	U32 DLC_IUP_IFR;			// 0x20   ICU mode changed interrupt flags register, the ICUs which generate this flag are shown in DLC_IUP_IFR register.
} maestro_registers_t;

// The base address for the DLC registers
#define MAESTRO_DLC_BASE_ADDRESS    0x76000000

/*
 *  @brief a pointer to the DLC registers data structure(Domain Link Connector)
 *  @warning this macro relies on the
 *  @ref MAESTRO_DLC_BASE_ADDRESS macro
*/
#define MAESTRO_DLC   ((volatile maestro_registers_t *)MAESTRO_DLC_BASE_ADDRESS)
/*
  * @brief a macro that reads a DLC register
  * @param reg the register that is read
*/
#define MAESTRO_Read(reg)     (MAESTRO_DLC->reg)
/*
 * @brief a macro that writes a DLC register
 * @param reg the register that is read
 * @param value the value that is written
*/
#define MAESTRO_Write(reg, value) (MAESTRO_DLC->reg = (value))


/************ MAESTRO Data & Status ****************/
typedef enum {
    MAESTRO_OK = 0,
    MAESTRO_ERROR_INCORRECT_ICU_ID,
    MAESTRO_ERROR_ICU_BUSY,
    MAESTRO_ERROR_INCORRECT_VAL
}maestro_status_t;

typedef enum{
	MAESTRO_MODE_NOT_AVAILABLE = -1,
	MAESTRO_MODE_ON_NF_NV = 0, 	/*ON, NF, NV*/
	MAESTRO_MODE_EXT_FOFF_NV,  	/*EXT, FOFF, NV*/
	MAESTRO_MODE_2,
	/* YOU CAN add MAESTRO_MODE you used until reached to MAESTRO_MODE_MAX*/
	MAESTRO_MODE_MAX, // Up to 15,
}maestro_mode_t;

typedef struct{
	ICU_MT_SR_st icumtMode;      /* Current ICU Mode*/
	ICU_MT_CR_st icumtReq;        /* ICU Mode Table Request*/
}maestro_icumt_state_t;          /* ICU Mode Table Status*/

typedef struct {
  maestro_icumt_state_t mICUs[ICU_MAX];
  U32 mIdleState;        // An Idle state with an unknown value
  U32 mModeRequest;
  U32 mPendingModeRequest;
  U32 mFlagsOK;          // Each bit of mFlagsOK is set if the requested mode change from the control interface on the corresponding ICU was performed.
  U32 mFlagsDelayed;   // Each bit of mFlagsDelayed is set if the requested mode change from the control interface on the corresponding ICU was delayed..
  U32 mFlagsUpdated;  // Each bit of mFlagsUpdated is set if the requested mode change from the control interface on the corresponding ICU was changed/updated.
  U32 mFlagsDenied;    // Each bit of mFlagsDenied is set if the requested mode change from the control interface on the corresponding ICU was denied.
  U32 mLastPCtrl;
}maestro_state_st;

#define MAESTRO_GET_ICU_BIT(v, i)   	((gMaestroState.m##v>> i) & 1)
#define MAESTRO_SET_ICU_BIT(v, i)   	gMaestroState.m##v |= 1 << i
#define MAESTRO_CLEAR_ICU_BIT(v, i) 	gMaestroState.m##v &= ~(1 << i)


/******************Function declarations*****************************/
extern maestro_state_st gMaestroState;
extern maestro_status_t setIcuModeUntilStable(icu_offset_t icu_index, maestro_mode_t icuMode);
extern maestro_status_t hal_maestro_set_icu_mode(icu_offset_t icu_index, maestro_mode_t maestro_mode);
extern maestro_status_t hal_maestro_get_current_icu_mode(icu_offset_t icu_index, U8 *getIcuMtSr);
extern void hal_maestro_set_dmu_cgno(dmu_offset_t dmu_index, U8 value);
extern void hal_maestro_set_dmu_rno(dmu_offset_t dmu_index, U8 value);
extern void hal_maestro_set_icu_cgo(U8 dir, ICU_cgo_steps_t step);
extern void hal_maestro_set_icu_ro(U8 dir, ICU_cgo_steps_t step);
extern void hal_maestro_clear_wiu_interrupt(void);
extern void hal_maestro_set_wiu_interrupt(wiu_offset_t wiu_index, WIU_ISPMR_t maskInt);
extern void hal_maestro_set_dmu_manual_mode(dmu_offset_t dmu_index);
extern U8 hal_maestro_get_icu_cgo(void);
extern void hal_maestro_set_wiu_register(wiu_offset_t wiu_index, WIU_registers_t wiu_reg, U8 value);
void hal_maestro_mask_wiu_interrupt(void);
void hal_maestro_get_wiu_interrupt(WIU_IFR_t *wiuFlag);

#endif /* __MAESTRO_H__ */


