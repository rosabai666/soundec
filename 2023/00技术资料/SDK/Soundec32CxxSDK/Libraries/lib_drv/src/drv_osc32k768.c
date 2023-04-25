/** 
***********************************************************
 *
 * @file name	: drv_osc32k768.c
 * @author	: Kevin
 * @version	: V00.10
 * @date	: 2020-02-23
 * @brief	: 32.768kHz Oscillator Driver
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
 * and Soundec Co., Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/

/* Includes ------------------------------------------------------------------*/
#include "hal_osc32k768.h"
//#include "drv_osc32k768.h"


/* Private functions ---------------------------------------------------------*/

/** 
 * @brief	This function is used to initialize OSC32K768.
 * @param[in]	cycle       The delay cycles will be set.
 * @param[in]	trim        The trimming will be set.
 * @return
 * 	@retval HAL_STATUS_OK The 32.768kHz Oscillator is successfully initialized.
 */
int osc32k768_init(uint32_t cycle, uint32_t trim)
{
    hal_status_t status;

    /* The default mode is normal mode, set it to standby mode firstly */
    hal_osc32k768_set_mode(HAL_OSC32K768_MODE_STANDBY);

    status = hal_osc32k768_set_delay_cycle(cycle);
    if (status != HAL_STATUS_OK)
        return status;

    status = hal_osc32k768_set_trim(trim);
    if (status != HAL_STATUS_OK)
        return status;

    hal_osc32k768_set_mode(HAL_OSC32K768_MODE_NORMAL);

    return HAL_STATUS_OK;
}

/** 
 * @brief	This function is used to deinitialize OSC32K768.
 * @return
 * 	@retval 0 	The 32.768kHz Oscillator is successfully deinitialized.
 */
int osc32k768_deinit(void)
{
    hal_osc32k768_set_mode(HAL_OSC32K768_MODE_SHUTDOWN);

    return 0;
}


