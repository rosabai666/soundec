/**
***********************************************************
 *
 * @file name	: uac_if.h
 * @author		: nick
 * @version		: Vxx.xx
 * @date		: 2020-07-09
 * @brief		:
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
#ifndef __UAC_IF_H
#define __UAC_IF_H

#include "uac.h"
#if (USB_ENABLE)
extern usb_audio_interface_t uac_fops;

#endif

#endif /* __UAC_IF_H */

