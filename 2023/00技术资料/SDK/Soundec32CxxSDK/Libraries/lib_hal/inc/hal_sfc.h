/**
***********************************************************
 *
 * @file name   : hal_sfc.h
 * @author      : RandyFan
 * @version     : V3.2
 * @date        : 2022-01-04
 * @brief       : HAL SFC(SPI Flash Controller) driver header file
 *
 * ***********************************************************
 * @attention
 *
 * Copyright (c) 2022 Soundec Co., Ltd.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user
 * who uses the software, his/her employer (which must be your employer
 * and Sourced Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/

/*-------------------------------------------------------------------------------------------
*********************----XT25F08B-S----********************
*
* --Each Device has--Each block has--Each sector has--Each page has
*		 1M 			64K/32K			4K				256		(bytes)
*		 4K 			256/128 		16				-		(pages)
*		 256			16/8 			-				- 		(sectors)
*		 16/32			-				- 				- 		(block)
*
-------------------------------------------------------------------------------------------	*/

/*-------------------------------------------------------------------------------------------
Table1.0 XT25F08B-S Protected area size (CMP=0) Status Register Content Memory Content
	---------------------------------------------------------------------------------------
	BP3 BP2 BP1 BP0 Blocks 			Addresses 			Density 	Portion
	0 	0 	0 	0 	NONE 			NONE 				NONE 		NONE
	0 	0 	0 	1 	Block 15 		0F0000H-100000H 	64KB 		Upper 1/16
	0 	0 	1 	0 	Block 14-15 	0E0000H-100000H 	128K 		Upper 1/8
	0 	0 	1 	1 	Block 12-15 	0C0000H-100000H 	256K 		Upper 1/4
	0 	1 	0 	0 	Block 8-15 		080000H-100000H 	512K 		Upper 1/2
	0 	1 	0 	1 	Protected all 	000000H-100000H 	1M 			All
	0 	1 	1 	0 	Protected all 	000000H-100000H 	1M 			All
	0 	1 	1 	1 	Protected all 	000000H-100000H 	1M 			All
	1 	X 	X 	X 	Protected all 	000000H-100000H 	1M 			All
-------------------------------------------------------------------------------------------
Table1.1 XT25F08B-S Protected area size (CMP=1)Status Register Content Memory Content
	---------------------------------------------------------------------------------------
	BP3 BP2 BP1 BP0 Blocks 			Addresses 			Density 	Portion
	0 	0 	0 	0 	NONE 			NONE 				NONE 		NONE
	0 	0 	0 	1 	Block 0 		000000H-010000H 	64KB 		Lower 1/16
	0 	0 	1 	0 	Block 0-1 		000000H-020000H 	128K 		Lower 1/8
	0 	0 	1 	1 	Block 0-3 		000000H-040000H 	256K 		Lower 1/4
	0 	1 	0 	0 	Block 0-7 		000000H-080000H 	512K 		Lower 1/2
	0 	1 	0 	1 	Protected all 	000000H-100000H 	1M 			All
	0 	1 	1 	0 	Protected all 	000000H-100000H 	1M 			All
	0 	1 	1 	1 	Protected all 	000000H-100000H 	1M 			All
	1 	X 	X 	X 	Protected all 	000000H-100000H 	1M 			All
-------------------------------------------------------------------------------------------*/

#ifndef __HAL_SFC_H__
#define __HAL_SFC_H__

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "snc8600.h"
#include "hal.h"
#include <stdio.h>

/* Private macro --------------------------------------------------------------*/
/**
 * @name FLASH_ID
 * @brief Chip id of supported flash chips
 * @{
 */
#define FLASH_XT25F08BS_ID					0x0B4014
#define FLASH_P25Q80H_ID					0x856014
#define FLASH_ZB25VQ80A_ID					0x5E6014
/** @} FLASH_ID */

/**
 * @name FLASH_PARAM
 * @brief Flash parameters
 * @{
 */
#define FLASH_PAGE_SIZE       				256     	// 256 bytes per page
#define FLASH_SECTOR_SIZE     				4096    	// 4K per sector
#define FLASH_32K_BLOCK_SIZE				(32*1024)   // 32K
#define FLASH_64K_BLOCK_SIZE				(64*1024)	// 64K
/** @} FLASH_PARAM */

/**
 * @name FLASH_CMDS
 * @brief Flash commands(Intersection of XTX and PUYA)
 * @{
 */
//Protection
#define FLASH_CMD_WREN						0x06	//Write Enable
#define FLASH_CMD_WRDI						0x04	//Write Disable
#define FLASH_CMD_VWREN						0x50	//Volatile SR Write Enable

//Erase
#define FLASH_CMD_SE						0x20	//Sector Erase
#define FLASH_CMD_BE32						0x52	//32K Block Erase
#define FLASH_CMD_BE64						0xD8	//64K Block Erase
#define FLASH_CMD_CE						0xC7	//Chip Erase	//60H or C7H

//Program
#define FLASH_CMD_PP						0x02	//Page Program
#define FLASH_CMD_QPP						0x32	//Quad Page Program

//Read
#define FLASH_CMD_READ						0x03	//Read(Low Power)
#define FLASH_CMD_FREAD						0x0B	//Fast Read
#define FLASH_CMD_DREAD						0x3B	//Dual Read
#define FLASH_CMD_2READ						0xBB	//2xIO Read
#define FLASH_CMD_QREAD						0x6B	//Quad Read
#define FLASH_CMD_4READ						0xEB	//4xIO Read

//Status Reg
#define FLASH_CMD_RDSR						0x05	//Read Status Register Low Byte
#define FLASH_CMD_RDSR2						0x35	//Read Status Register High Byte
#define FLASH_CMD_WRSR						0x01	//Write Status Register

//Security
#define FLASH_CMD_PRSCUR					0x42	//Program Security Registers
#define FLASH_CMD_ERSCUR					0x44	//Erase Security Registers
#define FLASH_CMD_RDSCUR					0x48	//Read Security Registers

//Read ID
#define FLASH_CMD_RDID						0x9F	//Read JEDEC ID
#define FLASH_CMD_REMS						0x90	//Read Manufacture/device ID
#define FLASH_CMD_DREMS						0x92	//Dual Read Manufacture/device ID
#define FLASH_CMD_QREMS						0x94	//Quad Read Manufacture/device ID
#define FLASH_CMD_RDSFDP					0x5A	//Read SFDP(Serial Flash Discoverable Parameters)
#define FLASH_CMD_RDUID						0x5A	//Read Unique ID

//Deep Power-down
#define FLASH_CMD_DP						0xB9	//Deep Power-down
#define FLASH_CMD_RDP						0xAB	//Release Deep Power-down

//Reset
#define FLASH_CMD_RSTEN						0x66	//Reset Enable
#define FLASH_CMD_RST						0x99	//Reset
/** @} FLASH_CMDS */

/**
 * @name SFC_REG
 * @brief Read/Write SFC Registers
 * @{
 */
#define SFC_REG   						((volatile sfc_regs_t *)SFC_BASE)
#define SFC_Read(reg)     				(SFC_REG->reg)
#define SFC_Write(reg, val) 			(SFC_REG->reg = (val))
/** @} SFC_REG */

/**
 * @name SFC_REG_DOMAIN
 * @brief Shifts and Masks of SFC Registers
 * @{
 */
//REG_START register
#define SFC_REG_START_PS_SHIFT				1
#define SFC_REG_START_PS_MASK				(0x01 << SFC_REG_START_PS_SHIFT)	//Power Start
#define SFC_REG_START_RS_SHIFT				0
#define SFC_REG_START_RS_MASK				(0x01 << SFC_REG_START_RS_SHIFT)	//Reg Start

//REG_CMD register
#define SFC_REG_CMD_SHIFT					24
#define SFC_REG_CMD_MASK					(0xFF << SFC_REG_CMD_SHIFT)	//Cmd

//REG_ADDR register
#define SFC_REG_ADDR_SHIFT					8
#define SFC_REG_ADDR_MASK					(0xFFFFFF << SFC_REG_ADDR_SHIFT)	//Addr

//CTRL1 register
#define SFC_ADDR_DMY_CYCLE_SHIFT			24
#define SFC_ADDR_DMY_CYCLE_MASK				(0xFF << SFC_ADDR_DMY_CYCLE_SHIFT)
#define SFC_ADDR_CYCLE_SHIFT				16
#define SFC_ADDR_CYCLE_MASK					(0xFF << SFC_ADDR_CYCLE_SHIFT)
#define SFC_CMD_DMY_CYCLE_SHIFT				8
#define SFC_CMD_DMY_CYCLE_MASK				(0xFF << SFC_CMD_DMY_CYCLE_SHIFT)
#define SFC_CMD_CYCLE_SHIFT					0
#define SFC_CMD_CYCLE_MASK					(0xFF << SFC_CMD_CYCLE_SHIFT)

//CTRL2 register
#define SFC_NEED_JUSTIFY_BUSY_SHIFT			20
#define SFC_NEED_JUSTIFY_BUSY_MASK			(0x01 << SFC_NEED_JUSTIFY_BUSY_SHIFT)
#define SFC_DATA_DUAL_IO_SHIFT				19
#define SFC_DATA_DUAL_IO_MASK				(0x01 << SFC_DATA_DUAL_IO_SHIFT)
#define SFC_DATA_QUAR_IO_SHIFT				18
#define SFC_DATA_QUAR_IO_MASK				(0x01 << SFC_DATA_QUAR_IO_SHIFT)
#define SFC_ADDR_DUAL_IO_SHIFT				17
#define SFC_ADDR_DUAL_IO_MASK				(0x01 << SFC_ADDR_DUAL_IO_SHIFT)
#define SFC_ADDR_QUAR_IO_SHIFT				16
#define SFC_ADDR_QUAR_IO_MASK				(0x01 << SFC_ADDR_QUAR_IO_SHIFT)
#define SFC_RD_DATA_CYCLE_SHIFT				8
#define SFC_RD_DATA_CYCLE_MASK				(0xFF << SFC_RD_DATA_CYCLE_SHIFT)
#define SFC_WR_DATA_CYCLE_SHIFT				0
#define SFC_WR_DATA_CYCLE_MASK				(0xFF << SFC_WR_DATA_CYCLE_SHIFT)

//HOLD_WP register
#define SFC_WP_SHIFT						3
#define SFC_WP_MASK							(0x01 << SFC_WP_SHIFT)
#define SFC_WP_FORCE_SHIFT					2
#define SFC_WP_FORCE_MASK					(0x01 << SFC_WP_FORCE_SHIFT)
#define SFC_HOLD_SHIFT						1
#define SFC_HOLD_MASK						(0x01 << SFC_HOLD_SHIFT)

//MEM_CMD register
#define SFC_MEM_CONT_SHIFT					16
#define SFC_MEM_CONT_MASK					(0xFF << SFC_MEM_CONT_SHIFT)
#define SFC_MEM_RD_SHIFT					8
#define SFC_MEM_RD_MASK						(0xFF << SFC_MEM_RD_SHIFT)
#define SFC_MEM_WR_SHIFT					0
#define SFC_MEM_WR_MASK						(0xFF << SFC_MEM_WR_SHIFT)

//MEMRD register
#define SFC_MEM_RD_HOLD_EN_SHIFT			21
#define SFC_MEM_RD_HOLD_EN_MASK				(0x01 << SFC_MEM_RD_HOLD_EN_SHIFT)
#define SFC_MEM_WR_HOLD_EN_SHIFT			20
#define SFC_MEM_WR_HOLD_EN_MASK				(0x01 << SFC_MEM_WR_HOLD_EN_SHIFT)
#define SFC_MEM_RD_DATA_DUAL_IO_SHIFT		19
#define SFC_MEM_RD_DATA_DUAL_IO_MASK		(0x01 << SFC_MEM_RD_DATA_DUAL_IO_SHIFT)
#define SFC_MEM_RD_DATA_QUAR_IO_SHIFT		18
#define SFC_MEM_RD_DATA_QUAR_IO_MSAK		(0x01 << SFC_MEM_RD_DATA_QUAR_IO_SHIFT)
#define SFC_MEM_RD_ADDR_DUAL_IO_SHIFT		17
#define SFC_MEM_RD_ADDR_DUAL_IO_MASK		(0x01 << SFC_MEM_RD_ADDR_DUAL_IO_SHIFT)
#define SFC_MEM_RD_ADDR_QUAR_IO_SHIFT		16
#define SFC_MEM_RD_ADDR_QUAR_IO_MASK		(0x01 << SFC_MEM_RD_ADDR_QUAR_IO_SHIFT)
#define SFC_MEM_RD_ADDR_DMY_CYCLE_SHIFT		12
#define SFC_MEM_RD_ADDR_DMY_CYCLE_MASK		(0x0F << SFC_MEM_RD_ADDR_DMY_CYCLE_SHIFT)
#define SFC_MEM_RD_ADDR_CONT_CYCLE_SHIFT	8
#define SFC_MEM_RD_ADDR_CONT_CYCLE_MASK		(0x0F << SFC_MEM_RD_ADDR_CONT_CYCLE_SHIFT)
#define SFC_MEM_RD_ADDR_CYCLE_SHIFT			0
#define SFC_MEM_RD_ADDR_CYCLE_MASK			(0x1F << SFC_MEM_RD_ADDR_CYCLE_SHIFT)

//PWR_EN register
#define SFC_PWR_EXIT_SHIFT					8
#define SFC_PWR_EXIT_MASK					(0xFF << SFC_PWR_EXIT_SHIFT)
#define SFC_PWR_ENTER_SHIFT					0
#define SFC_PWR_ENTER_MASK					(0xFF << SFC_PWR_ENTER_SHIFT)

//PWR register
#define SFC_PWR_TRES_SHIFT					16
#define SFC_PWR_TRES_MASK					(0xFFFF << SFC_PWR_TRES_SHIFT)
#define SFC_PWR_TDP_SHIFT					0
#define SFC_PWR_TDP_MASK					(0xFFFF << SFC_PWR_TDP_SHIFT)

//CACHE register
#define SFC_DPRE_SHIFT						8
#define SFC_DPRE_MASK						(0x0F << SFC_DPRE_SHIFT)
#define SFC_IPRE_SHIFT						4
#define SFC_IPRE_MASK						(0x0F << SFC_IPRE_SHIFT)
#define SFC_DPREEN_SHIFT					3
#define SFC_DPREEN_MASK						(0x01 << SFC_DPREEN_SHIFT)
#define SFC_IPREEN_SHIFT					2
#define SFC_IPREEN_MASK						(0x01 << SFC_IPREEN_SHIFT)
#define SFC_DCEN_SHIFT						1
#define SFC_DCEN_MASK						(0x01 << SFC_DCEN_SHIFT)
#define SFC_ICEN_SHIFT						0
#define SFC_ICEN_MASK						(0x01 << SFC_ICEN_SHIFT)

//PAGE_MEMWR register
#define SFC_CS_HIGH_CYCLE_PP_SHIFT			28
#define SFC_CS_HIGH_CYCLE_PP_MASK			(0x0F << SFC_CS_HIGH_CYCLE_PP_SHIFT)
#define SFC_CS_HIGH_CYCLE_SHIFT				24
#define SFC_CS_HIGH_CYCLE_MASK				(0x0F << SFC_CS_HIGH_CYCLE_SHIFT)
#define SFC_MEM_WR_ADDR_DMY_CYCLE_SHIFT		20
#define SFC_MEM_WR_ADDR_DMY_CYCLE_MASK		(0x0F << SFC_MEM_WR_ADDR_DMY_CYCLE_SHIFT)
#define SFC_MEM_WR_ADDR_CONT_CYCLE_SHIFT	16
#define SFC_MEM_WR_ADDR_CONT_CYCLE_MASK		(0x0F << SFC_MEM_WR_ADDR_CONT_CYCLE_SHIFT)
#define SFC_MEM_WR_DATA_DUAL_IO_SHIFT		11
#define SFC_MEM_WR_DATA_DUAL_IO_MASK		(0x01 << SFC_MEM_WR_DATA_DUAL_IO_SHIFT)
#define SFC_MEM_WR_DATA_QUAR_IO_SHIFT		10
#define SFC_MEM_WR_DATA_QUAR_IO_MASK		(0x01 << SFC_MEM_WR_DATA_QUAR_IO_SHIFT)
#define SFC_MEM_WR_ADDR_DUAL_IO_SHIFT		9
#define SFC_MEM_WR_ADDR_DUAL_IO_MASK		(0x01 << SFC_MEM_ADDR_DUAL_IO_SHIFT)
#define SFC_MEM_WR_ADDR_QUAR_IO_SHIFT		8
#define SFC_MEM_WR_ADDR_QUAR_IO_MASK		(0x01 << SFC_MEM_WR_ADDR_QUAR_IO_SHIFT)
#define SFC_MEM_WR_ADDR_CYCLE_SHIFT			3
#define SFC_MEM_WR_ADDR_CYCLE_MASK			(0x1F << SFC_MEM_WR_ADDR_CYCLE_SHIFT)
#define SFC_PAGE_SIZE_SHIFT					0
#define SFC_PAGE_SIZE_MASK					(0x07 << SFC_PAGE_SIZE_SHIFT)
/** @} SFC_REG_DOMAIN */

/* Private typedef -------------------------------------------------------------*/
/**
 * @struct flash_sr_t
 * @brief Packing Flash Status Register
 */
typedef union{
	uint16_t word;
	struct{
		uint8_t lsb;				// s7-s0
		uint8_t msb;				// s15-s8
	}bytes;
	struct{
		uint16_t WIP :1;			// S0
		uint16_t WEL :1;
		uint16_t BP0 :1;
		uint16_t BP1 :1;
		uint16_t BP2 :1;
		uint16_t BP3 :1;
		uint16_t BP4 :1;
		uint16_t SRP :1;			// S7
		uint16_t Reserved0 :1;	// S8;
		uint16_t QE :1;			// S9
		uint16_t LB :1;
		uint16_t Reserved1 :3;
		uint16_t CMP :1;
		uint16_t Reserved2 :1;	// S15
	}bits;
}flash_sr_t;

/**
 * @enum flash_type_t
 * @brief Define flash chip type
 */
typedef enum{
	XT25F08BS = 0,
	P25Q80H,
}flash_type_t;

/**
 * @enum flash_read_type_t
 * @brief Define flash read method
 */
typedef enum{
	FLASH_READ = 0,
	FLASH_READ_FAST,
	FLASH_READ_DUAL,
	FLASH_READ_2_IO,
	FLASH_READ_QUAD,
	FLASH_READ_4_IO,
}flash_read_type_t;

/**
 * @enum flash_write_type_t
 * @brief Define flash write method
 */
typedef enum{
	FLASH_PP = 0,
	FLASH_QPP,
}flash_write_type_t;

/**
 * @enum flash_erase_type_t
 * @brief Define flash erase type
 */
typedef enum{
	ERASE_CHIP = 0,	///<Chip Erase Time: 2.5 ~ 5 s
	ERASE_SECTOR,	///<Sector Erase Time: 60 ~ 300 ms
	ERASE_BLOCK32,	///<32K Bytes Erase Time: 0.15/0.25 s
	ERASE_BLOCK64,	///<64K Bytes Erase Time: 0.3/0.5 s
}flash_erase_type_t;

/**
 * @enum flash_bp_t
 * @brief Define flash block protect type
 */
typedef enum{
	PROTECT_NONE = 0,
	PROTECT_BLOCK_0,		///<000000H-010000H
	PROTECT_BLOCK_0_1,		///<000000H-020000H
	PROTECT_BLOCK_0_3,		///<000000H-040000H
	PROTECT_BLOCK_0_7,		///<000000H-080000H
	PROTECT_BLOCK_8_15,		///<080000H-100000H
	PROTECT_BLOCK_12_15,	///<0C0000H-100000H
	PROTECT_BLOCK_14_15,	///<0E0000H-100000H
	PROTECT_BLOCK_15,		///<0F0000H-100000H
	PROTECT_BLOCK_ALL		///<000000H-100000H
}flash_bp_t;

/**
 * @struct sfc_regs_t
 * @brief Packing Registers of SFC
 * @ingroup Regs
 */
typedef struct
{
	uint32_t REG_START;		///<offset=0x00, Enable SFC Send Cmd to Flash
	uint32_t REG_DATA;		///<offset=0x04, Data Written to or Read from Flash's Status Register and Security Register
	uint32_t REG_CMD;		///<offset=0x08, Cmd Sent to Flash
	uint32_t REG_ADDR;		///<offset=0x0c, Address Sent to Flash together with Cmd
	uint32_t CTRL1;			///<offset=0x10, SFC Control 1
	uint32_t CTRL2;			///<offset=0x14, SFC Control 2
	uint32_t JUSTIFY;		///<offset=0x18, Justify Waiting Time
	uint32_t HOLD_WP;		///<offset=0x1c, Hold and Write Protect
	uint32_t MEM_CMD;		///<offset=0x20, Flash's Read and Write Cmd
	uint32_t MEMRD;			///<offset=0x24, Control Flash's Read
	uint32_t PWR_EN;		///<offset=0x28, Flash's Enter or Exit Power-down Mode Cmd
	uint32_t PWR;			///<offset=0x2c, Delay Time of Flash's Enter or Exit Power-down Mode
	uint32_t CACHE;			///<offset=0x30, Control SFC's Cache
	uint32_t PAGE_MEMWR;	///<offset=0x34, Flash Page Size and Control Flash's Write
}sfc_regs_t;
/* Private variables ------------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------------*/
#if 1//(FLASH_ENABLE)
/**
 * @brief Check whether flash is busy
 * @return 1: busy\n 0: idle
 */
bool hal_sfc_check_busy(void);

/**
 * @brief Check flash chip type, by reading jedec id
 * @return flash chip type
 */
uint32_t hal_sfc_check_chip_type(void);

/**
 * @brief Init flash read method and write method
 * @param[in] rdType read method
 * @param[in] wrType write method
 */
void hal_sfc_read_write_init(uint8_t rdType, uint8_t wrType);

/**
 * @brief Set block protect by setting bp bits of flash's status register
 * @param[in] bp block protect type
 * @return whether block protect is set successfully
 */
hal_status_t hal_sfc_block_protect(flash_bp_t bp);

/**
 * @brief Check whether block protect is set successfully
 * @param[in] bp block protect type
 * @return whether block protect is set successfully
 */
hal_status_t hal_sfc_check_block_protect(flash_bp_t _bp);

/**
 * @brief Wait until flash is not busy
 * @return flash is not busy, or timeout
 */
hal_status_t hal_sfc_wait_program_over(void);

/**
 * @brief Read flash's jedec id
 * @return jedec id
 */
uint32_t hal_sfc_read_jedec_id(void);

/**
 * @brief Read flash's unique id
 * @param[out] *uid unique id
 * @param[in] len length
 */
void hal_sfc_read_uid(uint32_t *uid, uint8_t len);

/**
 * @brief Read flash's status register
 * @return reg value
 */
flash_sr_t hal_sfc_read_status_reg(void);

/**
 * @brief Write flash's status register
 * @param[in] reg value
 * @return write finished
 * @note write enable first before write status register
 * @warning after write status register, flash must be re-write-enabled before another write or erase
 */
hal_status_t hal_sfc_write_status_reg(flash_sr_t regVal);

/**
 * @brief Enable flash write
 * @return cmd executed
 */
hal_status_t hal_sfc_write_enable(void);

/**
 * @brief Erase flash
 * @param[in] cmd erase type
 * @param[in] addr erase start address
 * @note erase will be executed only if Write Enable has been set and the correspond area is not protected.
 * @warning after erase, flash must be re-write-enabled before another write or erase
 */
void hal_sfc_erase(uint8_t cmd, uint32_t addr);

/**
 * @brief Enable SFC's cache
 * @param[in] en 1: enable; 0: disable
 */
void hal_sfc_cache_enable(bool en);

/**
 * @brief Flash enter deep power-down
 * @return cmd excuted
 * @note if another flash operation is excuted, flash will exit deep power-down automatically
 */
hal_status_t hal_sfc_dp(void);
#endif	//FLASH_ENABLE

#endif

