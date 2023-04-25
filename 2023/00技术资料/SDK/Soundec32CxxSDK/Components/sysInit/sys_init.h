/**
***********************************************************
 *
 * @file name	: sys_init.h
 * @author		: Rosa.Bai
 * @version		: V0.1
 * @date		: 2020-06-23
 * @brief		: Initialize for System related
 *
***********************************************************
 * @attention
 *
 * Copyright (c) 2020 Soundec Co., Ltd.
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
#ifndef __SYS_INIT_H__
#define __SYS_INIT_H__

#include <xtensa/tie/xt_hifi2.h>
#include <xtensa/xtruntime.h>
#include <xtensa/config/core.h>

#include "types.h"
#include "drv_pll.h"


#ifdef __cplusplus
extern "C" {
#endif


#define system_reset()			write_reg(0x7700F030, 0x00000001)
//#define active_dfu()			write_reg(SYS_USED_INFO_DFU_FLAG_ADDRESS,SYS_USED_INFO_CHAR_DFU_REQ);write_reg(SYS_USED_INFO_DFU_FLAG_BACKUP_ADDRESS,SYS_USED_INFO_CHAR_DFU_REQ);
#define active_dfu()			write_reg(0xA00FF000,SYS_USED_INFO_CHAR_DFU_REQ)

extern void system_init(void);
extern void set_codec33_power_off(void);
extern void get_ic_uniqueID(uint32_t *uid);
void calculate_time_clear(void);
uint32_t calculate_time_get(uint8_t timeUnit);


#ifdef __cplusplus
}
#endif

#endif //__SYS_INIT_H__

