/**
 ***********************************************************
  * @file name	 : drv_flash.h
  * @author 	 : RandyFan
  * @version	 : V3.2
  * @date		 : 2022-01-04
  * @brief		 : Flash driver header file
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

#ifndef __BSP_FLASH__
#define __BSP_FLASH__

/* Includes ------------------------------------------------------------------*/
#include "hal_inc.h"
#include "hal.h"

/* Private typedef -----------------------------------------------------------*/
/**
 * @struct flash_cfg_t
 * @brief Configs of flash read/write type and block protect
 */
typedef struct{
	uint8_t rdType;
	uint8_t wrType;
	uint8_t bpType;
}flash_cfg_t;

/* Private macro --------------------------------------------------------------*/
#define FLASH_ADDR_ALIGN     						4

/* ---------------------------------------------------------------------------
*  --Memory map FLASH
* 0			~ 0x7fff  : 32K size for flashBoot32k.bin
* 0x8000	~ 0x47fff : 256K size for program code
* 0x48000	~ 0xeefff : 604K size for user stored data before the program starts,such as tones data.Erasure is not allowed!!!
* 0Xef000	~ 0xfefff : 2*64k for user read erase and write
* 0xff000	~ 0xfffff : last 4K for DFU
* ---------------------------------------------------------------------------*/
/* Address of the Flash where program started*/
#define FLASH_START_ADDRESS							0xA0000000
#define FLASH_SIZE									0x100000	/* 1M */
#define FLASH_END_ADDRESS							(FLASH_START_ADDRESS + FLASH_SIZE -1)

/* !!! Reserved for DFU usage.. Don't change !!! */
/* Address of the Flash from where DFU used */
#define SYS_USED_INFO_SIZE							(FLASH_SECTOR_SIZE)
#define SYS_USED_INFO_START_ADDRESS					(FLASH_END_ADDRESS - SYS_USED_INFO_SIZE + 1)
#define SYS_RESERVED_SIZE							0
#define SYS_USED_SIZE								(SYS_RESERVED_SIZE+SYS_USED_INFO_SIZE)
#define SYS_USED_START_ADDRESS						(FLASH_END_ADDRESS - SYS_USED_SIZE + 1)
#define SYS_USED_INFO_CHAR_DFU_REQ					0x5AA5C33C			// When receive a DFU request
#define SYS_USED_INFO_DFU_FLAG_ADDRESS				SYS_USED_INFO_START_ADDRESS
#define SYS_USED_INFO_DFU_IMG_VERSION_ADDRESS		(SYS_USED_INFO_DFU_FLAG_ADDRESS+4)
#define SYS_USED_INFO_DFU_FLAG_BACKUP_ADDRESS		(SYS_USED_INFO_DFU_IMG_VERSION_ADDRESS+4)
/* !!! Reserved for DFU usage.. Don't change !!! */

/*Address of the flash from where tones data stored*/
#define USER_FLASH_TONE_START						(0x48000)//about 300K for tone,this size is not fixed

#if(FLASH_ENABLE)
/* Private function prototypes -----------------------------------------------*/
/**
 * @brief Init flash, check flash type and cfg read/write/block protect type
 * @return Init ok or error
 */
hal_status_t flash_init(void);

/**
 * @brief active dfu by writing 0x5AA5C33C to 0xA000FF000
 */
void flash_active_dfu(void);

/**
 * @brief set flash block protect
 * @param[in] bp block protect type
 * @return block protect ok or not
 */
hal_status_t flash_block_protect(flash_bp_t bp);

/**
 * @brief disable flash block protect
 * @return disable block protect ok or not
 */
hal_status_t flash_disable_block_protect(void);

/**
 * @brief Check whether the given address is 4 aligned or not
 * @param[in] addr
 * @return check result
 */
hal_status_t flash_check_addr(uint32_t addr);

/**
 * @brief Read a 32-bit word from addr
 * @param[in] addr
 * @return word
 */
uint32_t flash_read_word(uint32_t addr);

/**
 * @brief Read continuos 32-bit words to given buffer from addr
 * @param[in] addr
 * @param[in] *rdBuf
 * @param[in] len
 * @return read ok or not
 */
hal_status_t flash_read_buf(uint32_t addr, uint32_t *rdBuf, uint32_t len);

/**
 * @brief Write a 32-bit word to addr
 * @param[in] addr
 * @param[in] word
 */
hal_status_t flash_write_word(uint32_t addr, uint32_t word);

/**
 * @brief Write continuos 32-bit words from buffer to addr
 * @param[in] addr
 * @param[in] wrBuf
 * @param[in] len
 * @return write ok or not
 */
hal_status_t flash_write_buf(uint32_t addr, uint32_t *wrBuf, uint32_t len);

/**
 * @brief Erase flash
 * @param[in] erType erase type
 * @param[in] addr
 */
void flash_erase(uint8_t erType, uint32_t addr);

/**
 * @brief Erase flash by sectors
 * @param[in] stAddr start address
 * @param[in] endAddr end address
 */
void flash_erase_sectors_by_addr(uint32_t stAddr, uint32_t endAddr);

/**
 * @brief Read flash's unique id
 * @param[in] uid
 */
void flash_get_uid(uint32_t *uid);
#endif

#endif /*  */
