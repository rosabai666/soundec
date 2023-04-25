/** 
***********************************************************
 * 
 * @file name   : userFlash.h
 * @author      : Rosa.Bai
 * @version     : V1.0
 * @date         : 2020-12-19
 * @brief         : This is user interface for saving information into Flash
 * 
 * @author	   : yen.yu
 * @version	   : V2.0
 * @date 	   : 2022-01-12
 * @brief		: Redefine the interface
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
#ifndef __USER_FLASH__
#define __USER_FLASH__
#include "common.h"
#include "drv_flash.h"
#include "altotool.h"
#include "altotool_eq.h"

/* Private macro --------------------------------------------------------------*/
#define SYS_SETTINGS_FLAG							0x5C5CA0A3

/* -------User defined------- */
/* Address of the Flash from where used for user data */
#define USER_FLASH_START_ADDRESS					(0x90000)
#define USER_FLASH_BANK_SIZE						(0x30000)	/* The flash size must be a multiple of 4K*/
#define USER_FLASH_BANK0_START_ADDRESS				USER_FLASH_START_ADDRESS
#define USER_FLASH_BANK1_START_ADDRESS				(0xC0000)
#define USER_FLASH_END_ADDRESS						(0xEFFFF)

/*Address of the flash from where tones data stored*/
#define USER_FLASH_TONE_START						(0x48000)//about 300K for tone,this size is not fixed

#define UFLASH_REGION_SYS_PARAM						{uFLASH_INDEX_SYS_PARAM, sizeof(user_flash_sys_param_t)}
#if FLASH_WRITE_PROTECT_ENABLE
#define UFLASH_REGION_START_FLAG					{uFLASH_INDEX_START_FLAG, 0x04}
#endif
#define UFLASH_REGION_CODEC_ADC_PARAM				{uFLASH_INDEX_CODEC_ADC_PARAM, sizeof(codec_adc_cfgs_t)}
#define UFLASH_REGION_CODEC_DAC_PARAM				{uFLASH_INDEX_CODEC_DAC_PARAM, sizeof(codec_dac_cfgs_t)}
#define UFLASH_REGION_SPK_EQ_PARAM(x)				{(uFLASH_INDEX_SPK_EQ_PARAM+(x)), sizeof(eq_chx_t)}
#if(SUPPORT_SPKER_STEREO_EQ)
#define UFLASH_REGION_MIC_EQ_PARAM					{(uFLASH_INDEX_MIC_EQ_PARAM), sizeof(eq_chx_t) - sizeof(eq_band_t)*EQ_NUM}
#else
#define UFLASH_REGION_MIC_EQ_PARAM					{(uFLASH_INDEX_MIC_EQ_PARAM), sizeof(eq_chx_t)}
#endif
#define UFLASH_REGION_ALG_PARAM						{uFLASH_INDEX_ALG_PARAM, sizeof(alg_cfgs_t)}
#define UFLASH_REGION_I2S_PARAM						{uFLASH_INDEX_I2S_PARAM, sizeof(i2s_cfgs_t)}

#define USER_FLASH_QUE_LEN 							3
#define FLASH_IS_EMPTY								0xFF

/* Public typedef ----------------------------------------------------------*/
typedef struct{
	uint32_t sectorNum;
	uint32_t regionSize;
}user_flash_region_t;

typedef enum{
	uFLASH_INDEX_SYS_PARAM = 0,
#if FLASH_WRITE_PROTECT_ENABLE
	uFLASH_INDEX_START_FLAG,
#endif
	uFLASH_INDEX_CODEC_ADC_PARAM,
	uFLASH_INDEX_CODEC_DAC_PARAM,
	uFLASH_INDEX_SPK_EQ_PARAM,
	uFLASH_INDEX_MIC_EQ_PARAM = uFLASH_INDEX_SPK_EQ_PARAM + SUPPORT_SPK_EQ_NUM,
	uFLASH_INDEX_ALG_PARAM,
	uFLASH_INDEX_I2S_PARAM,
	uFLASH_INDEX_NUM,
}user_flash_index_t;

typedef struct{
	uint32_t sysSettingFlag;
	uint32_t usbSerialNum;
	uint32_t eqMode;
	uint32_t usbMode;
}user_flash_sys_param_t;

typedef struct
{
	uint8_t procState;
	bool sectorBank1Active[uFLASH_INDEX_NUM];
	uint8_t queTop;
	uint8_t queTail;
	user_flash_region_t que[USER_FLASH_QUE_LEN];
	uint32_t* queBuf[USER_FLASH_QUE_LEN];
#if FLASH_WRITE_PROTECT_ENABLE
	uint8_t timeCount;
#endif
}user_flash_handle_t;

typedef enum{
	uFLASH_IDEL = 0,
	uFLASH_START_SAVE,
	uFLASH_START_ERASE,
	uFLASH_WAIT_ERASE,
}user_flash_state_t;
/* Public variables ------------------------------------------------------------*/
#if(FLASH_ENABLE)
extern user_flash_sys_param_t sysParam;


/* Public function prototypes ----------------------------------------------------*/
uint8_t user_flash_init(void);
void user_flash_save_data(user_flash_region_t region, uint32_t* wrBuf);
uint8_t user_flash_load_data(user_flash_region_t region, uint32_t* rdBuf);
void user_flash_show_param(void);
uint32_t get_param_usb_sn(void);
void user_flash_polling_100ms(void);
void user_flash_check_start_flag(void);
#endif

#endif  /*__USER_FLASH__*/

