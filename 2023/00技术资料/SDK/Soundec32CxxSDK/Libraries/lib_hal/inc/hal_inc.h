/*
 * hal_inc.h
 *
 *  Created on: 2020Äê6ÔÂ21ÈÕ
 *      Author: User
 */

#ifndef __HAL__INC__
#define __HAL__INC__

#define HAL_BQ_MODULE_ENABLED
//#define HAL_CACHE_MODULE_ENABLED
#define HAL_CLOCK_MODULE_ENABLED
#define HAL_CODEC_MODULE_ENABLED
#define HAL_DMAC_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
#define HAL_I2S_MODULE_ENABLED
#define HAL_MAESTRO_MODULE_ENABLED
#define HAL_interrupt_MODULE_ENABLED
#define HAL_OSC32768_MODULE_ENABLED
#define HAL_PINMUX_MODULE_ENABLED
#define HAL_PLL_MODULE_ENABLED
#define HAL_PWM_MODULE_ENABLED
#define HAL_RTC_MODULE_ENABLED
#define HAL_SARADC_MODULE_ENABLED
#define HAL_SFC_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED


#ifdef HAL_BQ_MODULE_ENABLED
#include "hal_bq.h"
#endif /*HAL_BQ_MODULE_ENABLED*/

#ifdef HAL_CACHE_MODULE_ENABLED
#include "hal_cache.h"
#endif /*HAL_CACHE_MODULE_ENABLED*/

#ifdef HAL_CLOCK_MODULE_ENABLED
#include "hal_clock.h"
#endif /*HAL_CLOCK_MODULE_ENABLED*/

#ifdef HAL_CODEC_MODULE_ENABLED
#include "hal_codec.h"
#endif /*HAL_CODEC_MODULE_ENABLED*/

#ifdef HAL_DMAC_MODULE_ENABLED
#include "hal_dmac.h"
#endif /*HAL_DMAC_MODULE_ENABLED*/

#ifdef HAL_GPIO_MODULE_ENABLED
#include "hal_gpio.h"
#endif /*HAL_GPIO_MODULE_ENABLED*/

#ifdef HAL_I2S_MODULE_ENABLED
#include "hal_i2s.h"
#endif /*HAL_I2S_MODULE_ENABLED*/

#ifdef HAL_MAESTRO_MODULE_ENABLED
#include "hal_maestro.h"
#endif /*HAL_MAESTRO_MODULE_ENABLED*/

#ifdef HAL_interrupt_MODULE_ENABLED
#include "hal_interrupt.h"
#endif /*HAL_interrupt_MODULE_ENABLED*/

#ifdef HAL_OSC32768_MODULE_ENABLED
#include "hal_osc32k768.h"
#endif /*HAL_OSC32768_MODULE_ENABLED*/

#ifdef HAL_PLL_MODULE_ENABLED
#include "hal_pll.h"
#endif /*HAL_PLL_MODULE_ENABLED*/

#ifdef HAL_PWM_MODULE_ENABLED
#include "hal_pwm.h"
#endif /*HAL_PWM_MODULE_ENABLED*/

#ifdef HAL_RTC_MODULE_ENABLED
#include "hal_rtc.h"
#endif /*HAL_RTC_MODULE_ENABLED*/

#ifdef HAL_SARADC_MODULE_ENABLED
#include "hal_saradc.h"
#endif /*HAL_SARADC_MODULE_ENABLED*/

#ifdef HAL_SFC_MODULE_ENABLED
#include "hal_sfc.h"
#endif /*HAL_SFC_MODULE_ENABLED*/

#ifdef HAL_UART_MODULE_ENABLED
#include "hal_uart.h"
#endif /*HAL_UART_MODULE_ENABLED*/

#ifdef HAL_I2C_MODULE_ENABLED
 #include "hal_i2c.h"
#endif /* HAL_I2C_MODULE_ENABLED */


#endif /*  */
