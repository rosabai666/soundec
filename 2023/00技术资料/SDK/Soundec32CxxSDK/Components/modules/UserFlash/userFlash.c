/** 
***********************************************************
 * 
 * @file name   : userFlash.c
 * @author      : Rosa.Bai
 * @version     : V1.0
 * @date         : 2020-12-19
 * @brief         : This is user interface for saving information into Flash
 * 
 * @author		: yen.yu
 * @version		: V2.0
 * @date		: 2022-01-12
 * @brief		 : Redefine the interface
 *
 * @author	  	: RandyFan
 * @version	  	: V3.0
 * @date 	   	: 2022-02-25
 * @brief		: Remove redundancy and improve scalability
***********************************************************
 * @attention
 * 
 * Copyright (c) 2019 Soundec Co., Ltd. 
 * All rights reserved.
 * 
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user
 * who uses the software, his/her employer (which must be your employer
 * and Sourced Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 * 
***********************************************************/
#include "common.h"
#include "userFlash.h"
#include "string.h"
#include <stdlib.h>
#include <xtensa/xtruntime.h>
#include "usb_def.h"

/* Private typedef -------------------------------------------------------------*/
#define START_FLAG1_CHECK			0x5a
#define START_FLAG2_CHECK			0x5c
#define U32_INVALID					0xFFFFFFFF
/* Private macro --------------------------------------------------------------*/
#define USER_FLASH_DEBUG			1
#if(USER_FLASH_DEBUG)
#define UFLASH_DEBUG(...)			uart_printf(__VA_ARGS__)
#else
#define UFLASH_DEBUG(...)
#endif

/* Private variables ------------------------------------------------------------*/
#if(FLASH_ENABLE)
user_flash_sys_param_t sysParam = {
	.sysSettingFlag = 0xFFFFFFFF,
	.usbSerialNum = 0xFFFFFFFF,
	.eqMode = 0xFF,
	.usbMode = 0x01,
};

static volatile user_flash_handle_t uFlashHandle;

/* Private function prototypes ----------------------------------------------------*/


static void user_flash_bank_toggle(uint8_t sectorNum)
{
	uFlashHandle.sectorBank1Active[sectorNum] = !uFlashHandle.sectorBank1Active[sectorNum];
}

static uint32_t user_flash_get_bank_address(uint8_t sectorNum)
{
	return USER_FLASH_START_ADDRESS + USER_FLASH_BANK_SIZE * uFlashHandle.sectorBank1Active[sectorNum] +FLASH_SECTOR_SIZE * sectorNum;
}

/**
 * @brief	user_flash_init
 * @note	{description}
 * @return	is flash empty
 * 	@retval false read flash useful data
 * 	@retval true readflash is empty
 */
uint8_t user_flash_init(void)
{
	hal_gpio_set_pinmux_function(GPIO_16, 0);
	hal_gpio_set_direction(GPIO_16, GPIO_DIRECTION_INPUT);

	memset(&uFlashHandle, 0, sizeof(user_flash_handle_t));

	user_flash_region_t region = UFLASH_REGION_SYS_PARAM;
	if(user_flash_load_data(region, (uint32_t*)&sysParam) == FLASH_IS_EMPTY)
	{
		sysParam.sysSettingFlag = SYS_SETTINGS_FLAG;
		sysParam.usbSerialNum = rand();
		sysParam.eqMode = sysParam.usbSerialNum & 0x0000FFFF;
		sysParam.usbMode = 0x01;

		user_flash_save_data(region, (uint32_t*)&sysParam);
		return FLASH_IS_EMPTY;
	}
	else
	{
#if(TEST_USB_SWITCH)
		switch(sysParam.usbMode)
		{
		case 0x01:
			is_usb_dev_UAC20 = UAC_10;
			Usbd_Speed_Cfg = USB_SPEED_FULL;
			break;
		case 0x02:
			is_usb_dev_UAC20 = UAC_10;
			Usbd_Speed_Cfg = USB_SPEED_HIGH;
			break;
		case 0x03:
			is_usb_dev_UAC20 = UAC_20;
			Usbd_Speed_Cfg = USB_SPEED_HIGH;
			break;
		default:
			is_usb_dev_UAC20 = UAC_10;
			Usbd_Speed_Cfg = USB_SPEED_FULL;
			break;
		}
#endif
	}
	return 0;
}

void user_flash_show_param(void)
{
	uart_printf("sysSettingFlag: %08x\n", sysParam.sysSettingFlag);
	uart_printf("usbSerialNum: %08x\n", sysParam.usbSerialNum);
	uart_printf("eqMode: %08x\n", sysParam.eqMode);
	uart_printf("usbMode: %08x\n", sysParam.usbMode);
}

void user_flash_save_data(user_flash_region_t region, uint32_t* wrBuf)
{
	uFlashHandle.que[uFlashHandle.queTop] = region;
	uFlashHandle.queBuf[uFlashHandle.queTop] = wrBuf;
	UFLASH_DEBUG("queTop:%d, %d, %d\n", uFlashHandle.queTop, region.sectorNum, region.regionSize);
	uFlashHandle.queTop++;
	if(uFlashHandle.queTop == USER_FLASH_QUE_LEN)
	{
		uFlashHandle.queTop = 0;
	}
	UFLASH_DEBUG("queTop: %d\nqueTail:%d\n", uFlashHandle.queTop, uFlashHandle.queTail);
}

uint8_t user_flash_load_data(user_flash_region_t region, uint32_t* rdBuf)
{
	uint32_t addr = user_flash_get_bank_address(region.sectorNum);
	if(flash_read_word(addr) == 0xFFFFFFFF)
	{
		user_flash_bank_toggle(region.sectorNum);
		addr = user_flash_get_bank_address(region.sectorNum);
		if(flash_read_word(addr) == 0xFFFFFFFF)
		{
			return FLASH_IS_EMPTY;
		}
	}
	uint32_t len = region.regionSize / 4;
	flash_read_buf(addr, (uint32_t*)rdBuf, len);
	user_flash_bank_toggle(region.sectorNum);
	return 0;
}

void user_flash_save_proc(void)
{
	switch(uFlashHandle.procState)
	{
	case uFLASH_IDEL:
		if(uFlashHandle.queTail != uFlashHandle.queTop)
		{
			uFlashHandle.procState = uFLASH_START_SAVE;
			UFLASH_DEBUG("start save\n");
		}
		break;
	case uFLASH_START_SAVE:
		if(hal_sfc_check_busy() == 0)
		{
#if FLASH_WRITE_PROTECT_ENABLE
			flash_block_protect(PROTECT_BLOCK_0_7);
			hal_sfc_write_enable();
#endif
			uint32_t addr = user_flash_get_bank_address(uFlashHandle.que[uFlashHandle.queTail].sectorNum);
			uint32_t size = uFlashHandle.que[uFlashHandle.queTail].regionSize;
			uint32_t len = size / 4;
			flash_write_buf(addr, (uint32_t*)uFlashHandle.queBuf[uFlashHandle.queTail], len);
			user_flash_bank_toggle(uFlashHandle.que[uFlashHandle.queTail].sectorNum);
			uFlashHandle.procState = uFLASH_START_ERASE;
		}
		else
		{
			UFLASH_DEBUG("busy\n");
		}
		break;
	case uFLASH_START_ERASE:
		if(hal_sfc_check_busy() == 0)
		{
			flash_erase(ERASE_SECTOR, user_flash_get_bank_address(uFlashHandle.que[uFlashHandle.queTail].sectorNum));
			uFlashHandle.procState = uFLASH_WAIT_ERASE;
			UFLASH_DEBUG("start erase\n");
		}
		else
		{
			UFLASH_DEBUG("busy\n");
		}
		break;
	case uFLASH_WAIT_ERASE:
		if(hal_sfc_check_busy() == 0)
		{
			if(flash_read_word(user_flash_get_bank_address(uFlashHandle.que[uFlashHandle.queTail].sectorNum)) == 0xFFFFFFFF)
			{
				uFlashHandle.queTail++;
				if(uFlashHandle.queTail == USER_FLASH_QUE_LEN)
				{
					uFlashHandle.queTail = 0;
				}
#if FLASH_WRITE_PROTECT_ENABLE
				flash_block_protect(PROTECT_BLOCK_ALL);
#endif

				uFlashHandle.procState = uFLASH_IDEL;
			}
			else
			{
				UFLASH_DEBUG("erase error\n");
				uFlashHandle.procState = uFLASH_START_ERASE;
			}
			hal_sfc_write_enable();
		}
		else
		{
			UFLASH_DEBUG("erasing\n");
		}
		break;
	default:
		UFLASH_DEBUG("user flash proc state error\n");
		break;
	}
}

/* Private functions ---------------------------------------------------------*/

/** 
 * @brief	get_param_usb_sn
 * @note	{description}
 * @par Example:
 * @code
 * 	get_param_usb_sn();
 * @endcode
 */
uint32_t get_param_usb_sn(void)
{
	return sysParam.usbSerialNum;
}

#if FLASH_WRITE_PROTECT_ENABLE
uint32_t startFlag = SYS_SETTINGS_FLAG;
void user_flash_check_start_flag(void)
{
	user_flash_region_t region = UFLASH_REGION_START_FLAG;
	uFlashHandle.sectorBank1Active[region.sectorNum] = 0;
	if(flash_read_word(user_flash_get_bank_address(region.sectorNum)) != startFlag)
	{
		uart_printf("startFlag1: %08x\n", flash_read_word(user_flash_get_bank_address(region.sectorNum)));
		user_flash_bank_toggle(region.sectorNum);
		if(flash_read_word(user_flash_get_bank_address(region.sectorNum)) == startFlag)
		{
			uart_printf("deprotect flash!");
			flash_block_protect(PROTECT_NONE);
		}
		else
		{
			flash_block_protect(PROTECT_BLOCK_0_7);
			flash_write_word(user_flash_get_bank_address(region.sectorNum), startFlag);
			flash_block_protect(PROTECT_BLOCK_ALL);
		}
	}
	else
	{
		flash_block_protect(PROTECT_BLOCK_0_7);
		flash_erase(ERASE_SECTOR, user_flash_get_bank_address(region.sectorNum));
		hal_sfc_wait_program_over();
		user_flash_bank_toggle(region.sectorNum);
		if(flash_read_word(user_flash_get_bank_address(region.sectorNum)) == startFlag)
		{
			flash_erase(ERASE_SECTOR, user_flash_get_bank_address(region.sectorNum));
			hal_sfc_wait_program_over();
		}
		user_flash_bank_toggle(region.sectorNum);
		flash_block_protect(PROTECT_BLOCK_ALL);
	}
	hal_sfc_write_enable();
	uFlashHandle.sectorBank1Active[region.sectorNum] = 0;
	user_flash_save_data(region, &startFlag);
}
#endif

/** 
 * @brief	user_flash_polling_100ms
 * @note	add at sysPolling_100ms_task()
 * @param[in]	void  
 * @return	None
 * 
 * @pre		{Preconditions}
 * @par Example:
 * @code
 * 	user_flash_polling_100ms()
 * @endcode
 */
void user_flash_polling_100ms(void)
{
	user_flash_save_proc();
#if FLASH_WRITE_PROTECT_ENABLE
	if(uFlashHandle.timeCount++ < 50)
	{
		if (hal_gpio_get_input(GPIO_16) == GPIO_DATA_LOW)
		{
			uart_printf("DFU_TRIGGER_LOW.\r\n");
			uFlashHandle.timeCount = 50;
			flash_disable_block_protect();
		}
	}
#endif
}
#endif	//FLASH_ENABLE

