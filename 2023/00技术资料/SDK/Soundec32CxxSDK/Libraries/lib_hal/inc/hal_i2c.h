/** 
 * 
 * @file name	: hal_i2c.h
 * @author		: RandyFan
 * @version		: V03.00
 * @date		: 2022-08-15
 * @brief		: hal_i2c
 * 
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
***********************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_I2C_H__
#define __HAL_I2C_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "common.h"
#include "snc8600.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum{
	I2C1 = I2C1_BASE,
	I2C2 = I2C2_BASE,
}i2cBase_t;

#define I2C_MASTER		1
#define I2C_SLAVE		0

#define	I2C_STANDARD_SPEED_MODE		1
#define I2C_FAST_SPEED_MODE			2


typedef struct{
	bool isMaster;
	bool useIntr;
	uint8_t speedMode;
	uint8_t rxFifo;
	uint8_t txFifo;
	uint8_t slaveAddr;
	uint16_t speed;

	bool intrGenCallEn;
	bool intrStartEn;
	bool intrStopEn;
	bool intrActivityEn;
	bool intrRxDoneEn;
	bool intrTxAbortEn;
	bool intrRdReqEn;
	bool intrTxEmptyEn;
	bool intrTxTxOverEn;
	bool intrRxFullEn;
	bool intrRxOverEn;
	bool intrRxUnderEn;
}i2c_cfgs_t;



#define I2C_Read(x, reg)			(((volatile i2c_regs_t *)(x))->reg)
#define I2C_Write(x, reg, val)		(((volatile i2c_regs_t *)(x))->reg = (val))

//IC_CON
#define I2C_SLAVE_DISABLE_SHIFT				6
#define I2C_SLAVE_DISABLE_MASK				(0x01 << I2C_SLAVE_DISABLE_SHIFT)
#define I2C_SLAVE_DISABLE(x)				(((x) << I2C_SLAVE_DISABLE_SHIFT) & I2C_SLAVE_DISABLE_MASK)

#define I2C_RESTART_EN_SHIFT				5
#define I2C_RESTART_EN_MASK					(0x01 << I2C_RESTART_EN_SHIFT)
#define I2C_RESTART_EN(x)					(((x) << I2C_RESTART_EN_SHIFT) & I2C_RESTART_EN_MASK)

#define I2C_10BITADDR_MASTER_SHIFT			4
#define I2C_10BITADDR_MASTER_MASK			(0x01 << I2C_10BITADDR_MASTER_SHIFT)
#define I2C_10BITADDR_MASTER(x)				(((x) << I2C_10BITADDR_MASTER_SHIFT) & I2C_10BITADDR_MASTER_MASK)

#define I2C_10BITADDR_SLAVE_SHIFT			3
#define I2C_10BITADDR_SLAVE_MASK			(0x01 << I2C_10BITADDR_SLAVE_SHIFT)
#define I2C_10BITADDR_SLAVE(x)				(((x) << I2C_10BITADDR_SLAVE_SHIFT) & I2C_10BITADDR_SLAVE_MASK)

#define I2C_SPEED_SHIFT						1
#define I2C_SPEED_MASK						(0x03 << I2C_SPEED_SHIFT)
#define I2C_SPEED(x)						(((x) << I2C_SPEED_SHIFT) & I2C_SPEED_MASK)

#define I2C_MASTER_MODE_SHIFT				0
#define I2C_MASTER_MODE_MASK				(0x01 << I2C_MASTER_MODE_SHIFT)
#define I2C_MASTER_MODE(x)					(((x) << I2C_MASTER_MODE_SHIFT) & I2C_MASTER_MODE_MASK)

//IC_TAR
#define I2C_SPECIAL_SHIFT					11
#define I2C_SPECIAL_MASK					(0x01 << I2C_SPECIAL_SHIFT)
#define I2C_SPECIAL(x)						(((x) << I2C_SPECIAL_SHIFT) & I2C_SPECIAL_MASK)

#define I2C_GC_OR_START_SHIFT				10
#define I2C_GC_OR_START_MASK				(0x01 << I2C_GC_OR_START_SHIFT)
#define I2C_GC_OR_START(x)					(((x) << I2C_GC_OR_START_SHIFT) & I2C_GC_OR_START_MASK)

#define I2C_TAR_SHIFT						0
#define I2C_TAR_MASK						(0x3FF << I2C_TAR_SHIFT)
#define I2C_TAR(x)							(((x) << I2C_TAR_SHIFT) & I2C_TAR_MASK)

//IC_SAR
#define I2C_SAR_SHIFT						0
#define I2C_SAR_MASK						(0x3FF << I2C_SAR_SHIFT)
#define I2C_SAR(x)							(((x) << I2C_SAR_SHIFT) & I2C_SAR_MASK)

//IC_DATA_CMD
#define I2C_CMD_SHIFT						8
#define I2C_CMD_MASK						(0x01 << I2C_CMD_SHIFT)
#define I2C_CMD(x)							(((x) << I2C_CMD_SHIFT) & I2C_CMD_MASK)

#define I2C_DAT_SHIFT						0
#define I2C_DAT_MASK						(0xFF << I2C_DAT_SHIFT)
#define I2C_DAT(x)							(((x) << I2C_DAT_SHIFT) & I2C_DAT_MASK)

//IC_SS_SCL_HCNT
#define I2C_SS_SCL_HCNT_SHIFT				0
#define I2C_SS_SCL_HCNT_MASK				(0xFFFF << I2C_SS_SCL_HCNT_SHIFT)
#define I2C_SS_SCL_HCNT(x)					(((x) << I2C_SS_SCL_HCNT_SHIFT) & I2C_SS_SCL_HCNT_MASK)

//IC_SS_SCL_LCNT
#define I2C_SS_SCL_LCNT_SHIFT				0
#define I2C_SS_SCL_LCNT_MASK				(0xFFFF << I2C_SS_SCL_LCNT_SHIFT)
#define I2C_SS_SCL_LCNT(x)					(((x) << I2C_SS_SCL_LCNT_SHIFT) & I2C_SS_SCL_LCNT_MASK)

//IC_FS_SCL_HCNT
#define I2C_FS_SCL_HCNT_SHIFT				0
#define I2C_FS_SCL_HCNT_MASK				(0xFFFF << I2C_FS_SCL_HCNT_SHIFT)
#define I2C_FS_SCL_HCNT(x)					(((x) << I2C_FS_SCL_HCNT_SHIFT) & I2C_FS_SCL_HCNT_MASK)

//IC_FS_SCL_LCNT
#define I2C_FS_SCL_LCNT_SHIFT				0
#define I2C_FS_SCL_LCNT_MASK				(0xFFFF << I2C_FS_SCL_LCNT_SHIFT)
#define I2C_FS_SCL_LCNT(x)					(((x) << I2C_FS_SCL_LCNT_SHIFT) & I2C_FS_SCL_LCNT_MASK)

//IC_INTR_STAT
#define	I2C_INTR_STAT_GEN_CALL		(0x800)
#define	I2C_INTR_STAT_START_DET		(0x400)
#define	I2C_INTR_STAT_STOP_DET		(0x200)
#define	I2C_INTR_STAT_ACTIVITY		(0x100)
#define	I2C_INTR_STAT_RX_DONE		(0x080)
#define	I2C_INTR_STAT_TX_ABRT		(0x040)
#define	I2C_INTR_STAT_RD_REQ		(0x020)
#define	I2C_INTR_STAT_TX_EMPTY		(0x010)
#define	I2C_INTR_STAT_TX_OVER		(0x008)
#define	I2C_INTR_STAT_RX_FULL		(0x004)
#define	I2C_INTR_STAT_RX_OVER		(0x002)
#define	I2C_INTR_STAT_RX_UNDER		(0x001)

//IC_RX_TL
#define I2C_RX_TL_SHIFT						0
#define I2C_RX_TL_MASK						(0xFF << I2C_RX_TL_SHIFT)
#define I2C_RX_TL(x)						(((x) << I2C_RX_TL_SHIFT) & I2C_RX_TL_MASK)

//IC_TX_TL
#define I2C_TX_TL_SHIFT						0
#define I2C_TX_TL_MASK						(0xFF << I2C_TX_TL_SHIFT)
#define I2C_TX_TL(x)						(((x) << I2C_TX_TL_SHIFT) & I2C_TX_TL_MASK)

//IC_ENABLE
#define I2C_EN_SHIFT						0
#define I2C_EN_MASK							(0x01 << I2C_EN_SHIFT)
#define I2C_EN(x)							(((x) << I2C_EN_SHIFT) & I2C_EN_MASK)

//IC_STATUS
#define I2C_STAT_SLV_ACTIVITY 		(0x040)
#define I2C_STAT_MST_ACTIVITY		(0x020)
#define I2C_STAT_RFF 				(0x010)
#define I2C_STAT_RFNE				(0x008)
#define I2C_STAT_TFE				(0x004)
#define I2C_STAT_TFNF 				(0x002)
#define I2C_STAT_ACTIVITY 			(0x001)







typedef struct
{
	__IO uint32_t IC_CON;				/**< Offset: 0x00*/
	__IO uint32_t IC_TAR;				/**< Offset: 0x04; I2C Target Address*/
	__IO uint32_t IC_SAR;				/**< Offset: 0x08; I2C Slave Address,Reset Value: IC_DEFAULT_SLAVE_ADDR*/
	__IO uint32_t rev0;					/**< Offset: 0x0c; reserved*/
	__IO uint32_t IC_DATA_CMD;			/**< Offset: 0x10; */

	__IO uint32_t IC_SS_SCL_HCNT;		/**< Offset: 0x14; The minimum valid value is 6. 40% of 100KBPs*/
	__IO uint32_t IC_SS_SCL_LCNT;		/**< Offset: 0x18; The minimum valid value is 8. 47% of 400KBPs*/
	__IO uint32_t IC_FS_SCL_HCNT;		/**< Offset: 0x1C; The minimum valid value is 6. 25% of 400KBPs*/
	__IO uint32_t IC_FS_SCL_LCNT;		/**< Offset: 0x20; The minimum valid value is 8. 51% of 400KBPs*/
	__IO uint32_t rev1[2];				/**< Offset: 0x24; reserved*/

	__I	 uint32_t IC_INTR_STAT;	  		/**< Offset: 0x2C; */
	__IO uint32_t IC_INTR_MASK;			/**< Offset: 0x30; These bits mask their corresponding interrupt status bits. They are active high; a value of 0 prevents a bit from generating an interrupt*/
	__I	 uint32_t IC_RAW_INTR_STAT;		/**< Offset: 0x34; All Read Only*/
	__IO uint32_t IC_RX_TL;				/**< Offset: 0x38; */
	__IO uint32_t IC_TX_TL;				/**< Offset: 0x3C */
	__I	 uint32_t IC_CLR_INTR;			/**< Offset: 0x40; */
	__I	 uint32_t IC_CLR_RX_UNDER;		/**< Offset: 0x44; */
	__I	 uint32_t IC_CLR_RX_OVER;		/**< Offset: 0x48; */
	__I	 uint32_t IC_CLR_TX_OVER;		/**< Offset: 0x4C; */
	__I	 uint32_t IC_CLR_RD_REQ;		/**< Offset: 0x50; */
	__I	 uint32_t IC_CLR_TX_ABRT;		/**< Offset: 0x54; */
	__I	 uint32_t IC_CLR_RX_DONE;		/**< Offset: 0x58; */
	__I	 uint32_t IC_CLR_ACTIVITY;		/**< Offset: 0x5C; */
	__I	 uint32_t IC_CLR_STOP_DET;		/**< Offset: 0x60; */
	__I	 uint32_t IC_CLR_START_DET;		/**< Offset: 0x64; */
	__I	 uint32_t IC_CLR_GEN_CALL; 		/**< Offset: 0x68; */
	__IO uint32_t IC_ENABLE;			/**< Offset: 0x6C; */
	__I	 uint32_t IC_STATUS;			/**< Offset: 0x70;*/
	__I	 uint32_t IC_TXFLR;				/**< Offset: 0x74; IC_TXFLR bit[TX_ABW:0], reserved bit[31:RX_ABW+1], The number of valid data entries in the transmit FIFO buffer..*/
	__I	 uint32_t IC_RXFLR;				/**< Offset: 0x78; IC_RXFLR bit[RX_ABW:0], reserved bit[31:RX_ABW+1], The number of valid data entries in the receive FIFO buffer*/
	__IO uint32_t rev2;					/**< Offset: 0x7C; */
	__IO uint32_t IC_TX_ABRT_SOURCE;	/**< Offset: 0x80; */
}i2c_regs_t;





static __inline__ void hal_i2c_clr_intr(i2cBase_t i2cx)
{
	I2C_Read(i2cx, IC_CLR_INTR);
}

static __inline__ void hal_i2c_clr_rx_under(i2cBase_t i2cx)
{
	I2C_Read(i2cx, IC_CLR_RX_UNDER);
}

static __inline__ void hal_i2c_clr_rx_over(i2cBase_t i2cx)
{
	I2C_Read(i2cx, IC_CLR_RX_OVER);
}

static __inline__ void hal_i2c_clr_tx_over(i2cBase_t i2cx)
{
	I2C_Read(i2cx, IC_CLR_TX_OVER);
}

static __inline__ void hal_i2c_clr_rd_req(i2cBase_t i2cx)
{
	I2C_Read(i2cx, IC_CLR_RD_REQ);
}

static __inline__ void hal_i2c_clr_tx_abrt(i2cBase_t i2cx)
{
	I2C_Read(i2cx, IC_CLR_TX_ABRT);
}

static __inline__ void hal_i2c_clr_rx_done(i2cBase_t i2cx)
{
	I2C_Read(i2cx, IC_CLR_RX_DONE);
}

static __inline__ void hal_i2c_clr_activity(i2cBase_t i2cx)
{
	I2C_Read(i2cx, IC_CLR_ACTIVITY);
}

static __inline__ void hal_i2c_clr_stop_det(i2cBase_t i2cx)
{
	I2C_Read(i2cx, IC_CLR_STOP_DET);
}

static __inline__ void hal_i2c_clr_start_det(i2cBase_t i2cx)
{
	I2C_Read(i2cx, IC_CLR_START_DET);
}

static __inline__ void hal_i2c_clr_gen_call(i2cBase_t i2cx)
{
	I2C_Read(i2cx, IC_CLR_GEN_CALL);
}

void hal_i2c_enable(i2cBase_t i2cx);
void hal_i2c_disable(i2cBase_t i2cx);
void hal_i2c_set_mode(i2cBase_t i2cx, i2c_cfgs_t* cfg);
hal_status_t hal_i2c_set_speed(i2cBase_t i2cx, i2c_cfgs_t* cfg);
void hal_i2c_set_intr_mask(i2cBase_t i2cx, i2c_cfgs_t* cfg);
void hal_i2c_update_tar_addr(i2cBase_t i2cx, uint8_t addr);
void hal_i2c_master_read_command(i2cBase_t i2cx);
uint8_t hal_i2c_read_data(i2cBase_t i2cx);
void hal_i2c_write_data(i2cBase_t i2cx, uint8_t data);
void hal_i2c_write_bulk(i2cBase_t i2cx, uint8_t* data, uint8_t len);
uint8_t hal_i2c_get_tx_fifo_len(i2cBase_t i2cx);
uint8_t hal_i2c_get_rx_fifo_len(i2cBase_t i2cx);
void hal_i2c_set_tx_fifo_level(i2cBase_t i2cx, uint8_t level);
void hal_i2c_set_rx_fifo_level(i2cBase_t i2cx, uint8_t level);
uint16_t hal_i2c_get_intr_stat(i2cBase_t i2cx);
uint16_t hal_i2c_get_intr_raw(i2cBase_t i2cx);
uint16_t hal_i2c_get_status(i2cBase_t i2cx);

#ifdef __cplusplus
}
#endif


#endif	/* __HAL_I2C_H__ */
