/**
***********************************************************
 *
 * @file name   : drv_flash.c
 * @author      : RandyFan
 * @version     : V3.2
 * @date        : 2022-01-04
 * @brief       : Flash driver file
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
 * and Sourced Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "drv_uart.h"
#include "hal_sfc.h"
#include "drv_flash.h"
#include "hal_sysctrl.h"

#if(FLASH_ENABLE)
#if(FLASH_DEBUG_ENABLE)
#define FLASH_DEBUG(...)		uart_printf(__VA_ARGS__)
#else
#define FLASH_DEBUG(...)
#endif

flash_cfg_t flashCfg = {
	.rdType = FLASH_READ_4_IO,
	.wrType = FLASH_QPP,
	.bpType = PROTECT_BLOCK_ALL,
};

/* Private function prototypes ----------------------------------------------------*/
/**
 * @brief Init flash, check flash type and cfg read/write/block protect type
 * @return Init ok or error
 */
hal_status_t flash_init(void)
{
	uint32_t clkGate = hal_sysctrl_get_clock_gate();
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_SFC);
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_SPIF);
	hal_sysctrl_set_clock_gate(clkGate);

	uint32_t chip = hal_sfc_check_chip_type();
	if(chip == XT25F08BS)
	{
		FLASH_DEBUG("Flash Type: XT25F08BS\n");
	}
	else if(chip == P25Q80H)
	{
		FLASH_DEBUG("Flash Type: P25Q80H\n");
	}
	else
	{
		FLASH_DEBUG("Flash JEDEC ID Error:0x%08x\n, Abort\n", chip);
		return HAL_STATUS_ERROR;
	}

	hal_sfc_read_write_init(flashCfg.rdType, flashCfg.wrType);

#if(FLASH_SFC_CACHE_ENABLE)
	hal_sfc_cache_enable(1);
#endif

#if (FLASH_WRITE_PROTECT_ENABLE == 1)
	if(flash_block_protect(flashCfg.bpType) == HAL_STATUS_OK)
	{
		FLASH_DEBUG("Flash Protect: Success\n");
	}
	else
	{
		FLASH_DEBUG("Flash Protect: Fail\n");
		return HAL_STATUS_FAIL;
	}
#endif

	hal_sfc_write_enable();

	return HAL_STATUS_OK;
}

/**
 * @brief active dfu by writing 0x5AA5C33C to 0xA000FF000
 */
void flash_active_dfu(void)
{
#if (FLASH_WRITE_PROTECT_ENABLE == 1)
	if(flash_disable_block_protect() == HAL_STATUS_OK)
		uart_printf("deprotect flash!\r\n");
	else
		uart_printf("deprotect not ok\n");
#endif
	uint8_t cnt = 5;
	while(cnt--)
	{
		flash_write_word(0x000FF000,SYS_USED_INFO_CHAR_DFU_REQ);
		hal_sfc_wait_program_over();
		if(flash_read_word(0x000FF000) == 0x5AA5C33C)
		{
			uart_printf("Active_system_dfu !\r\n");
			return;
		}
		else
		{
			uart_printf("%x\n",flash_read_word(0x000FF000));

			flash_erase(ERASE_SECTOR, 0x000FF000);
			hal_sfc_wait_program_over();
			hal_sfc_write_enable();
		}
	}

	uart_printf("active dfu fail\n");
}

/**
 * @brief set flash block protect
 * @param[in] bp block protect type
 * @return block protect ok or not
 */
hal_status_t flash_block_protect(flash_bp_t bp)
{
	ASSERT(bp >= PROTECT_NONE && bp <= PROTECT_BLOCK_ALL);
	return hal_sfc_block_protect(bp);
}

/**
 * @brief disable flash block protect
 * @return disable block protect ok or not
 */
hal_status_t flash_disable_block_protect(void)
{
	return hal_sfc_block_protect(PROTECT_NONE);
}

/**
 * @brief Check whether the given address is 4 aligned or not
 * @param[in] addr
 * @return check result
 */
hal_status_t flash_check_addr(uint32_t addr)
{
	if(addr & 0x3) //check whether addr is aligned by 4
		return HAL_STATUS_FAIL;
	return HAL_STATUS_OK;
}

/**
 * @brief Read a 32-bit word from addr
 * @param[in] addr
 * @return word
 */
uint32_t flash_read_word(uint32_t addr)
{
	if(flash_check_addr(addr) != HAL_STATUS_OK)
	{
		FLASH_DEBUG("rd addr is not aligned: 0x%x !\n", addr);
		return HAL_STATUS_FAIL;
	}
	return read_reg(FLASH_START_ADDRESS + addr);
}

/**
 * @brief Read continuos 32-bit words to given buffer from addr
 * @param[in] addr
 * @param[in] *rdBuf
 * @param[in] len
 * @return read ok or not
 */
hal_status_t flash_read_buf(uint32_t addr, uint32_t *rdBuf, uint32_t len)
{
	if(flash_check_addr(addr) != HAL_STATUS_OK)
	{
		FLASH_DEBUG("rd addr is not aligned: 0x%x !\n", addr);
		return HAL_STATUS_FAIL;
	}
	if(rdBuf == NULL || !len)
	{
		FLASH_DEBUG("rd NULL\n");
		return HAL_STATUS_FAIL;
	}

	uint32_t i =0;
	for(i=0; i<len; i++)
		rdBuf[i]=read_reg(FLASH_START_ADDRESS + addr + 0x4*(i));
	return HAL_STATUS_OK;
}

/**
 * @brief Write a 32-bit word to addr
 * @param[in] addr
 * @param[in] word
 * @return write ok or not
 */
hal_status_t flash_write_word(uint32_t addr, uint32_t word)
{
	if(flash_check_addr(addr) != HAL_STATUS_OK)
	{
		FLASH_DEBUG("wr addr is not aligned: 0x%x !\n", addr);
		return HAL_STATUS_FAIL;
	}
	write_reg(FLASH_START_ADDRESS + addr, word);
	return HAL_STATUS_OK;
}

/**
 * @brief Write continuos 32-bit words from buffer to addr
 * @param[in] addr
 * @param[in] wrBuf
 * @param[in] len
 * @return write ok or not
 */
hal_status_t flash_write_buf(uint32_t addr, uint32_t *wrBuf, uint32_t len)
{
	if(flash_check_addr(addr) != HAL_STATUS_OK)
	{
		FLASH_DEBUG("wr addr is not aligned: 0x%x !\n", addr);
		return HAL_STATUS_FAIL;
	}
	if(wrBuf == NULL || !len)
	{
		FLASH_DEBUG("wr NULL\n");
		return HAL_STATUS_FAIL;
	}

	uint32_t i =0;
	for(i=0; i<len; i++)
		write_reg(FLASH_START_ADDRESS + addr + 0x4*(i), wrBuf[i]);
	return HAL_STATUS_OK;
}

/**
 * @brief Erase flash
 * @param[in] erType erase type
 * @param[in] addr
 */
void flash_erase(uint8_t erType, uint32_t addr)
{
	switch(erType)
	{
	case ERASE_CHIP:
		hal_sfc_erase(FLASH_CMD_CE, 0);
		break;
	case ERASE_SECTOR:
		hal_sfc_erase(FLASH_CMD_SE, (addr & ~0xFFF));
		break;
	case ERASE_BLOCK32:
		hal_sfc_erase(FLASH_CMD_BE32, (addr & ~0x7FFF));
		break;
	case ERASE_BLOCK64:
		hal_sfc_erase(FLASH_CMD_BE64, (addr & ~0xFFFF));
		break;
	default:
		break;
	}
}

/**
 * @brief Erase flash by sectors
 * @param[in] stAddr start address
 * @param[in] endAddr end address
 */
void flash_erase_sectors_by_addr(uint32_t stAddr, uint32_t endAddr)
{
	ASSERT(stAddr < endAddr);

	if((stAddr & ~0xFFF) == (endAddr & ~0xFFF)) //same sector
	{
		hal_sfc_erase(FLASH_CMD_SE, (stAddr & ~0xFFF));
	}
	else
	{
		uint8_t size = ((endAddr & ~0xFFF) - (stAddr & ~0xFFF)) >> 12;
		uint8_t i = 0;
		for(i=0; i<= size; i++)
		{
			hal_sfc_erase(FLASH_CMD_SE, (stAddr & ~0xFFF) + i*0x1000);
			hal_sfc_wait_program_over();
		}
	}
}

/**
 * @brief Read flash's unique id
 * @param[in] uid
 */
void flash_get_uid(uint32_t *uid)
{
	uint32_t clkGate = hal_sysctrl_get_clock_gate();
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_SFC);
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_SPIF);
	hal_sysctrl_set_clock_gate(clkGate);

	memset(uid,0,4);
	hal_sfc_read_uid(uid, 4);

#if(FLASH_DEBUG_ENABLE)
	//uint8_t i = 0;
	for (int i =0; i <4;i++)
		FLASH_DEBUG("%08x,",uid[i]);
	FLASH_DEBUG("\n");
#endif
}
#endif	//FLASH_ENABLE
