/** 
***********************************************************
 * @attention
 * 
 * Copyright (c) 2022 Soundec Co., Ltd.
 * All rights reserved.
 * 
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user
 * who uses the software, his/her employer (which must be your employer
 * and Soundec Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 * 
***********************************************************
 * 
 * @file name   : hal_dmac.h
 * @author      : RandyFan
 * @version     : V02.02.demo
 * @date        : 2022-01-04
 * @brief       : HAL DMA Controller Driver Head File
 * 
***********************************************************/


#ifndef __HAL_DMAC_H__
#define __HAL_DMAC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"
#include "types.h"
#include "hal.h"
#include "snc8600.h"

#define DMAC_MASKED_VALUE			(0x0100)
#define DMAC_UNMASKED_VALUE			(0x0101)

#if 1//(DMA_ENABLE)
/**
 * @name DMAC_REG
 * @brief Read/Write DMAC Registers
 * @{
 */
#define DMAC_REG						((volatile dmac_regs_t *)DMAC_BASE)
#define DMAC_Read(reg)					(*((uint32_t *) &(DMAC_REG->reg)))
#define DMAC_Read_8(reg)				(*(uint8_t *)(&(DMAC_REG->reg)))
#define DMAC_Write(reg, val)			(*((uint32_t*) &(DMAC_REG->reg)) = (val))
#define DMAC_Write_8(reg, val)			(*(uint8_t *)(&(DMAC_REG->reg)) = (val))
/** @} DMAC_REG */
#endif	//DMA_ENABLE

/*! @name DMAC_REG_DOMAIN
 * @brief Shifts and Masks of DMAC Registers
 * @{
 */
/* CTLx_H register */
#define DMAC_BLOCK_TS_SHIFT			0
#define DMAC_BLOCK_TS_MASK			(0xFFF << DMAC_BLOCK_TS_SHIFT)		/* R/W: Block Transfer Size Mask */
#define DMAC_BLOCK_TS(x)			((((uint32_t)(x)) << DMAC_BLOCK_TS_SHIFT) & DMAC_BLOCK_TS_MASK)
/* CTLx_L register */
#define DMAC_TT_FC_SHIFT			20
#define DMAC_TT_FC_MASK				(0x03 << DMAC_TT_FC_SHIFT) 			/* R/W: Transfer Type Mask */
#define DMAC_TT_FC(x)				((((uint32_t)(x)) << DMAC_TT_FC_SHIFT) & DMAC_TT_FC_MASK)

#define DMAC_SRC_MSIZE_SHIFT		14
#define DMAC_SRC_MSIZE_MASK			(0x07 << DMAC_SRC_MSIZE_SHIFT)		/* R/W: Source Burst Transaction Size Mask */
#define DMAC_SRC_MSIZE(x)			((((uint32_t)(x)) << DMAC_SRC_MSIZE_SHIFT) & DMAC_SRC_MSIZE_MASK)

#define DMAC_DST_MSIZE_SHIFT		11
#define DMAC_DST_MSIZE_MASK			(0x07 << DMAC_DST_MSIZE_SHIFT)		/* R/W: Destination Burst Transaction Size Mask */
#define DMAC_DST_MSIZE(x)			((((uint32_t)(x)) << DMAC_DST_MSIZE_SHIFT) & DMAC_DST_MSIZE_MASK)

#define DMAC_SINC_SHIFT				9
#define DMAC_SINC_MASK				(0x03 << DMAC_SINC_SHIFT) 			/* R/W: Source Address Increment Mask */
#define DMAC_SINC(x)				((((uint32_t)(x)) << DMAC_SINC_SHIFT) & DMAC_SINC_MASK)

#define DMAC_DINC_SHIFT				7
#define DMAC_DINC_MASK				(0x03 << DMAC_DINC_SHIFT) 			/* R/W: Destination Address Increment Mask */
#define DMAC_DINC(x)				((((uint32_t)(x)) << DMAC_DINC_SHIFT) & DMAC_DINC_MASK)

#define DMAC_SRC_TR_WIDTH_SHIFT 	4
#define DMAC_SRC_TR_WIDTH_MASK		(0x07 << DMAC_SRC_TR_WIDTH_SHIFT)	/* R/W: Source Transfer Width Mask */
#define DMAC_SRC_TR_WIDTH(x)		((((uint32_t)(x)) << DMAC_SRC_TR_WIDTH_SHIFT) & DMAC_SRC_TR_WIDTH_MASK)

#define DMAC_DST_TR_WIDTH_SHIFT 	1
#define DMAC_DST_TR_WIDTH_MASK		(0x07 << DMAC_DST_TR_WIDTH_SHIFT)	/* R/W: Destination Transfer Width Mask */
#define DMAC_DST_TR_WIDTH(x)		((((uint32_t)(x)) << DMAC_DST_TR_WIDTH_SHIFT) & DMAC_DST_TR_WIDTH_MASK)

#define DMAC_INT_EN_SHIFT 			0
#define DMAC_INT_EN_MASK			(0x01 << DMAC_INT_EN_SHIFT)			/* R/W: Interrupt Enable Mask */
#define DMAC_INT_EN(x)				((((uint32_t)(x)) << DMAC_INT_EN_SHIFT) & DMAC_INT_EN_MASK)

/* CFGx_H register */
#define DMAC_DST_PER_SHIFT			11
#define DMAC_DST_PER_MASK			(0x0F << DMAC_DST_PER_SHIFT)		/* R/W: Hardware Destination Handshaking Interface Mask */
#define DMAC_DST_PER(x)				((((uint32_t)(x)) << DMAC_DST_PER_SHIFT) & DMAC_DST_PER_MASK)

#define DMAC_SRC_PER_SHIFT			7
#define DMAC_SRC_PER_MASK			(0x0F << DMAC_SRC_PER_SHIFT)		/* R/W: Hardware Source Handshaking Interface Mask */
#define DMAC_SRC_PER(x)				((((uint32_t)(x)) << DMAC_SRC_PER_SHIFT) & DMAC_SRC_PER_MASK)

#define DMAC_PROTCTL_SHIFT			2
#define DMAC_PROTCTL_MASK			(0x07 << DMAC_PROTCTL_SHIFT)		/* R/W: Protection Control Mask */
#define DMAC_PROTCTL(x)				((((uint32_t)(x)) << DMAC_PROTCTL_SHIFT) & DMAC_PROTCTL_MASK)

#define DMAC_FIFO_MODE_SHIFT		1
#define DMAC_FIFO_MODE_MASK			(0x01 << DMAC_FIFO_MODE_SHIFT)		/* R/W: FIFO Mode Select Mask */
#define DMAC_FIFO_MODE(x)			((((uint32_t)(x)) << DMAC_FIFO_MODE_SHIFT) & DMAC_FIFO_MODE_MASK)

/* CFGx_L register */
#define DMAC_SRC_HS_POL_SHIFT		19
#define DMAC_SRC_HS_POL_MASK		(0x01 << DMAC_SRC_HS_POL_SHIFT)		/* R/W: Source Handshaking Interface Polarity Mask */
#define DMAC_SRC_HS_POL(x)			((((uint32_t)(x)) << DMAC_SRC_HS_POL_SHIFT) & DMAC_SRC_HS_POL_MASK)

#define DMAC_DST_HS_POL_SHIFT		18
#define DMAC_DST_HS_POL_MASK		(0x01 << DMAC_DST_HS_POL_SHIFT)		/* R/W: Destination Handshaking Interface Polarity Mask */
#define DMAC_DST_HS_POL(x)			((((uint32_t)(x)) << DMAC_DST_HS_POL_SHIFT) & DMAC_DST_HS_POL_MASK)

#define DMAC_HS_SEL_SRC_SHIFT		11
#define DMAC_HS_SEL_SRC_MASK		(0x01 << DMAC_HS_SEL_SRC_SHIFT)		/* R/W: Source Software or Hardware Handshaking Interface Select Mask */
#define DMAC_HS_SEL_SRC(x)			((((uint32_t)(x)) << DMAC_HS_SEL_SRC_SHIFT) & DMAC_HS_SEL_SRC_MASK)

#define DMAC_HS_SEL_DST_SHIFT		10
#define DMAC_HS_SEL_DST_MASK		(0x01 << DMAC_HS_SEL_DST_SHIFT)		/* R/W: Destination Software or Hardware Handshaking Interface Select Mask */
#define DMAC_HS_SEL_DST(x)			((((uint32_t)(x)) << DMAC_HS_SEL_DST_SHIFT) & DMAC_HS_SEL_DST_MASK)

#define DMAC_FIFO_EMPTY_SHIFT		9
#define DMAC_FIFO_EMPTY_MASK		(0x01 << DMAC_FIFO_EMPTY_SHIFT)		/* R: Channel FIFO Empty Mask */
#define DMAC_FIFO_EMPTY(x)			((((uint32_t)(x)) << DMAC_FIFO_EMPTY_SHIFT) & DMAC_FIFO_EMPTY_MASK)

#define DMAC_CH_SUSP_SHIFT			8
#define DMAC_CH_SUSP_MASK			(0x01 << DMAC_CH_SUSP_SHIFT)		/* R/W: Channel Suspend Mask */
#define DMAC_CH_SUSP(x)				((((uint32_t)(x)) << DMAC_CH_SUSP_SHIFT) & DMAC_CH_SUSP_MASK)

#define DMAC_CH_PRIOR_SHIFT			5
#define DMAC_CH_PRIOR_MASK			(0x07 << DMAC_CH_PRIOR_SHIFT)		/* R/W: Channel Priority Mask */
#define DMAC_CH_PRIOR(x)			((((uint32_t)(x)) << DMAC_CH_PRIOR_SHIFT) & DMAC_CH_PRIOR_MASK)
/** @} DMAC_REG_DOMAIN */

/* DMAC Enums --------------------------------------------------------------*/
/**
 * @brief This enum defines the DMAC channels.
 */
typedef enum{
	DMAC_CHANNEL_0 = 0,
	DMAC_CHANNEL_1,
	DMAC_CHANNEL_2,
	DMAC_CHANNEL_3,
	DMAC_CHANNEL_4,
	DMAC_CHANNEL_5,
	DMAC_CHANNEL_NUM,
}dmac_channel_num_t;

/**
 * @brief This enum defines the DMAC interrupt types.
 */
typedef enum{
	DMAC_INTERRUPT_BLOCK = 0,
	DMAC_INTERRUPT_SRC_TRAN,
	DMAC_INTERRUPT_DST_TRAN,
	DMAC_INTERRUPT_NUM
}dmac_intr_type_t;

/**
 * @brief This enum defines the transfer type of DMAC.
 */
typedef enum{
	DMAC_MEMORY_TO_MEMORY = 0,
	DMAC_MEMORY_TO_PERIPHERAL,
	DMAC_PERIPHERAL_TO_MEMORY,
	DMAC_PERIPHERAL_TO_PERIPHERAL,
	DMAC_TRANSFER_TYPE_NUM
}dmac_transfer_type_t;

/** 
 * @brief This enum defines the number of data items to be transferred
 * (of width CTLx.SRC_TR_WIDTH or CTLx.DST_TR_WIDTH)
 */
typedef enum{
	DMAC_MSIZE_1 = 0,   /* 1 data item */
	DMAC_MSIZE_4,       /* 4 data items */
	DMAC_MSIZE_8,       /* 8 data items */
	DMAC_MSIZE_16,      /* 16 data items, only for CH0 */
	DMAC_MSIZE_NUM,
}dmac_msize_t;

/**
 * @brief This enum defines Transfer Width 
 */
typedef enum{
	DMAC_TR_WIDTH_8BIT = 0, /* 8 bit */
	DMAC_TR_WIDTH_16BIT,    /* 16 bit */
	DMAC_TR_WIDTH_32BIT,    /* 32 bit */
	DMAC_TR_WIDTH_64BIT,    /* 64 bit */
	DMAC_TR_WIDTH_128BIT,   /* 128 bit */
	DMAC_TR_WIDTH_256BIT,   /* 256 bit */
}dmac_tr_width_t;

/** 
 * @brief 	This enum defines the increment or decrement the source address on every source transfer. 
 *
 * @details If the device is fetching data from a source peripheral FIFO with a fixed address, then set this field to ¡°No change.¡±
 * 			If the device is writing data to a destination peripheral FIFO with a fixed address, then set this field to ¡°No change.¡±
 */
typedef enum{
	DMAC_INC_INCREMENT = 0, /* Increment */
	DMAC_INC_DECREMENT,     /* Decrement */
	DMAC_INC_UNCHANGED,     /* No change */
}dmac_inc_t;

/**
 * @brief This enum defines Software or Hardware Handshaking Select 
 */
typedef enum{
	DMAC_HANDSHAKE_HARDWARE = 0,	/* Hardware handshake */
	DMAC_HANDSHAKE_SOFTWARE,		/* Sofeware handshake */
	DMAC_HANDSHAKE_NUM,
}dmac_hs_sel_t;

/**
 * @brief This enum defines the Handshaking Interface Polarity 
 */
typedef enum{
	DMAC_HS_POL_HIGH = 0,   /* Active high */
	DMAC_HS_POL_LOW,        /* Active low */
}dmac_hs_pol_t;

/** 
 * @brief	This enum defines the FIFO Mode Selection
 *
 * @details	Determines how much space or data needs to be available in the FIFO before a burst transaction request is serviced.
 */
typedef enum{
	DMAC_FIFO_MODE_SINGLE_TRANSACTION = 0,	/* Space/data available for single AHB transfer of the specified transfer width. */
	DMAC_FIFO_MODE_HALF_FIFO_DEPTH,			/* Data available is greater than or equal to half the FIFO depth for destination
												 * transfers and space available is greater than half the fifo depth for source transfers
												 */
}dmac_fifo_mode_t;

/**
 * @struct dmac_chx_soft_hs_cfg_t
 * @brief This structure defines the channel's software handshaking peripherals configuration
 */
typedef struct{
	uint8_t srcMsize;			/*!< Source Burst Transaction Length */
	uint8_t dstMsize;			/*!< Destination Burst Transaction Length */
	uint8_t srcTrfWidth;	/*!< Source Transfer Width */
	uint8_t dstTrfWidth;	/*!< Destination Transfer Width */
	uint8_t srcInc;
	uint8_t dstInc;
	bool intrSrcTranEn;				/*!< Source Transaction Complete Interrupt Enable */
	bool intrDstTranEn;				/*!< Destination Transaction Complete Interrupt Enable */
}dmac_chx_soft_hs_cfg_t;

/**
 * @struct dmac_channel_cfg_t
 * @brief This structure defines the channel configuration information of DMA Controller
 */
typedef struct{
	uint8_t trfType;
	uint8_t chPriority;
	uint16_t blockSize;
	uint32_t srcAddr;
	uint32_t dstAddr;
	uint8_t i2sSel;
	bool intrBlockEn;
	dmac_chx_soft_hs_cfg_t perCfg;
}dmac_channel_cfg_t;


/**
 * @struct dmac_channel_regs_t
 * @brief Packing Channel Registers of DMAC
 * @ingroup Regs
 */
typedef struct {
	__IO uint32_t SAR;				///< offset=0x000+0x58*n, Channel n Source Address Register
	uint32_t rev0;
	__IO uint32_t DAR;				///< offset=0x008+0x58*n, Channel n Destination Address Register
	uint32_t rev1;
	__IO uint64_t rev2;
	__IO uint32_t CTL_L;			///< offset=0x018+0x58*n, Channel n Control Register
	__IO uint32_t CTL_H;
	__IO uint64_t rev3;
	__IO uint64_t rev4;
	__IO uint64_t rev5;
	__IO uint64_t rev6;
	__IO uint32_t CFG_L;			///< offset=0x040+0x58*n, Channel n Configuration Register
	__IO uint32_t CFG_H;
	__IO uint64_t rev7;
	__IO uint64_t rev8;
}dmac_channel_regs_t;

/**
 * @struct dmac_regs_t
 * @brief Packing Registers of DMAC
 * @ingroup Regs
 */
typedef struct {
	dmac_channel_regs_t Chx[DMAC_CHANNEL_NUM];
	dmac_channel_regs_t Reserved[8 - DMAC_CHANNEL_NUM];

	/* DMA Interrupt Registers */
	 __I uint64_t RawTfr;			///< offset=0x2c0, Raw Status for IntTfr Interrupt
	 __I uint64_t RawBlock;			///< offset=0x2c8, Raw Status for IntBlock Interrupt
	 __I uint64_t RawSrcTran;		///< offset=0x2d0, Raw Status for IntSrcTran Interrupt
	 __I uint64_t RawDstTran;		///< offset=0x2d8, Raw Status for IntDstTran Interrupt
	 __I uint64_t RawErr;			///< offset=0x2e0, Raw Status for IntErr Interrupt
	 __I uint64_t StatusTfr;		///< offset=0x2e8, Status for IntTfr Interrupt
	 __I uint64_t StatusBlock;		///< offset=0x2f0, Status for IntBlock Interrupt
	 __I uint64_t StatusSrcTran;	///< offset=0x2f8, Status for IntSrcTran Interrupt
	 __I uint64_t StatusDstTran;	///< offset=0x300, Status for IntDstTran Interrupt
	 __I uint64_t StatusErr;		///< offset=0x308, Status for IntErr Interrupt
	__IO uint64_t MaskTfr;			///< offset=0x310, Mask for IntTfr Interrupt
	__IO uint64_t MaskBlock;		///< offset=0x318, Mask for IntBlock Interrupt
	__IO uint64_t MaskSrcTran;		///< offset=0x320, Mask for IntSrcTran Interrupt
	__IO uint64_t MaskDstTran;		///< offset=0x328, Mask for IntDstTran Interrupt
	__IO uint64_t MaskErr;			///< offset=0x330, Mask for IntErr Interrupt
	 __O uint64_t ClearTfr;			///< offset=0x338, Clear for IntTfr Interrupt
	 __O uint64_t ClearBlock;		///< offset=0x340, Clear for IntBlock Interrupt
	 __O uint64_t ClearSrcTran;		///< offset=0x348, Clear for IntSrcTran Interrupt
	 __O uint64_t ClearDstTran;		///< offset=0x350, Clear for IntDstTran Interrupt
	 __O uint64_t ClearErr;			///< offset=0x358, Clear for IntErr Interrupt
	__IO uint64_t StatusInt;		///< offset=0x360, Status for Each Interrupt Type

	/* DMA Software Handshaking Registers */
	__IO uint64_t ReqSrcReg;		///< offset=0x368, Source Software Transaction Request Register
	__IO uint64_t ReqDstReg;		///< offset=0x370, Destination Software Transaction Request Register
	__IO uint64_t SglReqSrcReg;		///< offset=0x378, Single Source Transaction Request Register
	__IO uint64_t SglReqDstReg;		///< offset=0x380, Single Destination Transaction Request Register
	__IO uint64_t rev0;
	__IO uint64_t rev1;

	/* DMA Miscellaneous Registers */
	__IO uint64_t DmaCfgReg;		///< offset=0x398, DMA Confituration Register
	__IO uint64_t ChEnReg;			///< offset=0x3a0, DMA Channel Enable Register
	 __I uint64_t DmaIdReg;			///< offset=0x3a8, DMA ID Register
	__IO uint64_t DmaTestReg;		///< offset=0x3b0, DMA Test Register
}dmac_regs_t;




#if 1//(DMA_ENABLE)
/**
 * @brief	This function enables DMA Controller
 * @note	None
 * @return	None
 */
static __inline__ void hal_dmac_enable_global(void)
{
	DMAC_Write(DmaCfgReg, 1);
}

/**
 * @brief	This function disables DMA Controller
 * @note	None
 * @return	None
 */
static __inline__ void hal_dmac_disable_global(void)
{
	DMAC_Write(DmaCfgReg, 0);
}
/**
 * @brief	This function enables the specified channel of the DMA Controller
 * @note	None
 * @param[in]	chx     The target channel needs to be enabled.
 * @return	None
 */
static __inline__ void hal_dmac_enable_channel(dmac_channel_num_t chx)
{
	DMAC_Write(ChEnReg, 0x0101 << chx);
}

/**
 * @brief	This function disables the specified channel of the DMA Controller
 * @note	None
 * @param[in]	chx     The target channel needs to be enabled.
 * @return	None
 */
static __inline__ void hal_dmac_disable_channel(dmac_channel_num_t chx)
{
	DMAC_Write(ChEnReg, 0x0100 << chx);
}

/**
 * @brief	This function gets current 6 channels' open or close status
 * @note	None
 * @return	channel status
 */
static __inline__ uint8_t hal_dmac_get_channel_st(void)
{
	return DMAC_Read(ChEnReg);
}

/**
 * @brief	This function gets current 6 channels' block interrupt status
 * @note	None
 * @return	block interrupt status
 */
static __inline__ uint8_t hal_dmac_get_intr_block(void)
{
	return DMAC_Read(StatusBlock);
}

/**
 * @brief	This function clears block interrupt of selected channel
 * @param[in]	chx     The target channel selected.
 * @note	None
 * @return	None
 */
static __inline__ void hal_dmac_clr_intr_block(dmac_channel_num_t chx)
{
	DMAC_Write_8(ClearBlock, 0x01 << chx);
}

/**
 * @brief	This function gets current 6 channels' source transaction interrupt status
 * @note	None
 * @return	source transaction interrupt status
 */
static __inline__ uint8_t hal_dmac_get_intr_src_tran(void)
{
	return DMAC_Read(StatusSrcTran);
}

/**
 * @brief	This function clears source transaction interrupt of selected channel
 * @param[in]	chx     The target channel selected.
 * @note	None
 * @return	None
 */
static __inline__ void hal_dmac_clr_intr_src_tran(dmac_channel_num_t chx)
{
	DMAC_Write_8(ClearSrcTran, 0x01 << chx);
}

/**
 * @brief	This function gets current 6 channels' destination transaction interrupt status
 * @note	None
 * @return	source transaction interrupt status
 */
static __inline__ uint8_t hal_dmac_get_intr_dst_tran(void)
{
	return DMAC_Read(StatusDstTran);
}

/**
 * @brief	This function clears destination transaction interrupt of selected channel
 * @param[in]	chx     The target channel selected.
 * @note	None
 * @return	None
 */
static __inline__ void hal_dmac_clr_intr_dst_tran(dmac_channel_num_t chx)
{
	DMAC_Write_8(ClearDstTran, 0x01 << chx);
}

/**
 * @brief	This function sets source software handshaking of selected channel
 * @param[in]	chx     The target channel selected.
 * @note	None
 * @return	None
 */
static __inline__ void hal_dmac_soft_hs_set_src(dmac_channel_num_t chx)
{
	DMAC_Write(ReqSrcReg, (0x0101 << chx));
	DMAC_Write(SglReqSrcReg, (0x0101 << chx));
}

/**
 * @brief	This function sets destination software handshaking of selected channel
 * @param[in]	chx     The target channel selected.
 * @note	None
 * @return	None
 */
static __inline__ void hal_dmac_soft_hs_set_dst(dmac_channel_num_t chx)
{
	DMAC_Write(ReqDstReg, (0x0101 << chx));
	DMAC_Write(SglReqDstReg, (0x0101 << chx));
}

/** 
 * @brief	This function clears all interrupts of the DMA Controller.
 *			These interrupts include:
 *			-- Transfer Complete Interrupts
 *			-- Block Transfer Complete Interrupts
 *			-- Source Transaction Complete Interrupts
 *			-- Destination Transaction Complete Interrupts
 *			-- Error Interrupts
 * @note	None
 * @return	None
 */
void hal_dmac_clear_all_interrupts(void);

/**
 * @brief	This function configs DMA channels with transfer type Memory-to-Memory.
 * @param[in] chx	the DMA channel selected
 * @param[in] *cfg	the pointer of channel configuration
 * @note	None
 * @return	None
 */
void hal_dmac_config_channel_mem_to_mem(dmac_channel_num_t chx, dmac_channel_cfg_t* cfg);

/**
 * @brief	This function configs DMA channels used to transfer between Memory and I2S using hardware handshaking.
 * @param[in] chx	the DMA channel selected
 * @param[in] *cfg	the pointer of channel configuration
 * @note	None
 * @return	None
 */
void hal_dmac_config_channel_mem_to_i2s(dmac_channel_num_t chx, dmac_channel_cfg_t* cfg);

/**
 * @brief	This function configs DMA channels used to transfer between Memory and Peripherals using software handshaking.
 * @param[in] chx	the DMA channel selected
 * @param[in] *cfg	the pointer of channel configuration
 * @note	None
 * @return	None
 */
void hal_dmac_config_channel_mem_to_per_soft(dmac_channel_num_t chx, dmac_channel_cfg_t* cfg);

/**
 * @brief	This function re-configs an already configured channel
 * @param[in] chx		the DMA channel selected
 * @param[in] srcAddr	new Source Address
 * @param[in] dstAddr	new Destination Address
 * @param[in] block		new Block Size
 * @note	If srcAddr/dstAddr is 0, then the value would be ignored. This can be useful when the channel's srcAddr/dstAddr need no change.
 * @return	None
 */
void hal_dmac_recfg_channel(dmac_channel_num_t chx, uint32_t srcAddr, uint32_t dstAddr, uint16_t block);

#endif	//DMA_ENABLE

#ifdef __cplusplus
}
#endif
#endif /* __HAL_DMAC_H__ */
