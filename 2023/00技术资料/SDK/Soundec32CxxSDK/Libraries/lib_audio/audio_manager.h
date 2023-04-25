/** 
***********************************************************
 * 
 * @file name	: audio_manager.h
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2022-02-09
 * @brief		: Include file of audio manager
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
#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

/* Includes ---------------------------------------------------------------*/
#include "audio_session.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef ---------------------------------------------------------*/

/* Private macro ----------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/

/* Private function prototypes ------------------------------------------------*/

/* Private functions --------------------------------------------------------*/

typedef struct{
	auSessionId_t activeSession;
	auSessionId_t defaultRoute;
	auSession_t session[AUDIO_SESSION_MAX];

}auManagerHandler_t;

auSession_t *auSession_get_session(auSessionId_t id);

auManagerSt_t auManager_open_session(auSessionId_t sessionID);

auManagerSt_t auManager_close_session(auSessionId_t sessionID);

void audioManager_init(void);

#ifdef __cplusplus
}
#endif

#endif  /* __AUDIO_MANAGER_H__ */



