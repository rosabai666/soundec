/** 
***********************************************************
 * 
 * @file name	: hal_clock.h
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-02-26
 * @brief		: HAL Clock Driver Header File
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


#ifndef __HAL_CLOCK_H__
#define __HAL_CLOCK_H__

#include "types.h"

#ifdef	__cplusplus
extern "C"
{
#endif

#define	HAL_CODEC_SLAVE_FRACDIV_MASK		(1 << 20)

#if (CODEC_MASTER == 0)

/** 
 * @brief This struct defines the Codec slave mode clock generator registers
 */
typedef struct {
	__IO uint32_t dac_numerator;		/* DAC numerator, normally, it equals to DAC sampling rate */
	__IO uint32_t dac_denominator;		/* DAC denominator, normally, it equals to 12000000 */
	__IO uint32_t adc12_numerator;		/* ADC12 numerator, normally, it equals to ADC12 sampling rate */
	__IO uint32_t adc12_denominator;	/* ADC12 denominator, normally, it equals to 12000000 */
	__IO uint32_t adc3456_numerator;	/* ADC3456 numerator, normally, it equals to ADC3456 sampling rate */
	__IO uint32_t adc3456_denominator;	/* ADC3456 denominator, normally, it equals to 12000000 */
	__IO uint32_t adc789a_numerator;	/* ADC789a numerator, normally, it equals to ADC789a sampling rate */
	__IO uint32_t adc789a_denominator;	/* ADC789a denominator, normally, it equals to 12000000 */
} hal_codec_clock_t;

int hal_codec_slave_clock_init(void);


/** 
 * @brief This function is used to set the sampling rate of the slave DAC
 * @param[in] fs The sampling rate is set.
 * @return
 * 	@retval 0 The sampling rate of the slave DAC is successfully set.
 * 	@retval -1 The parameter is invalid.
 */
int hal_clock_set_slave_dac_samplerate(uint32_t fs);

/** 
 * @brief This function is used to set the sampling rate of the slave ADC12
 * @param[in] fs The sampling rate is set.
 * @return
 * 	@retval 0 The sampling rate of the slave ADC12 is successfully set.
 * 	@retval -1 The parameter is invalid.
 */
int hal_clock_set_slave_adc12_samplerate(uint32_t fs);

/** 
 * @brief This function is used to set the sampling rate of the slave ADC3456
 * @param[in] fs The sampling rate is set.
 * @return
 * 	@retval 0 The sampling rate of the slave ADC3456 is successfully set.
 * 	@retval -1 The parameter is invalid.
 */
int hal_clock_set_slave_adc3456_samplerate(uint32_t fs);

/** 
 * @brief This function is used to set the sampling rate of the slave ADC789a
 * @param[in] fs The sampling rate is set.
 * @return
 *	@retval 0 The sampling rate of the slave ADC789a is successfully set.
 *	@retval -1 The parameter is invalid.
 */
int hal_clock_set_slave_adc789a_samplerate(uint32_t fs);


#endif	//CODEC_MASTER==0

#ifdef	__cplusplus
}
#endif


#endif /* __HAL_CLOCK_H__ */
