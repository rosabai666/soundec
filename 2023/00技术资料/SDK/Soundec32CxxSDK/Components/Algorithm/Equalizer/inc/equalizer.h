/** 
***********************************************************
 * 
 * @file name	: equalizer.h
 * @author	    : usbd use
 * @version	    : nick
 * @date		: 2020-09-02
 * @brief		: spker eq
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
#if 1 //def SUPPORT_DSP_EQ

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EQUALIZER2_H__
#define __EQUALIZER2_H__

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "hal_bq.h"
#include "boardConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
typedef enum {
	GCORE_TYPE_BIQUAD_FILTER_BAND_PEAKING  = 0,	    /*!< Select Band Peaking Filter type */
	GCORE_TYPE_BIQUAD_FILTER_LOW_SHELF,			    /*!< Select Low Shelf Filter type */
	GCORE_TYPE_BIQUAD_FILTER_HIGH_SHELF,			/*!< Select High Shelf Filter type */
	GCORE_TYPE_BIQUAD_FILTER_BAND_PASS, 			/*!< Select Band Pass Filter type (constant gain_dB) */
	GCORE_TYPE_BIQUAD_FILTER_LOW_PASS,				/*!< Select Low pass Filter type */
	GCORE_TYPE_BIQUAD_FILTER_HIGH_PASS,				/*!< Select High pass Filter type */
	GCORE_TYPE_BIQUAD_FILTER_LOW_PASS_ONE_TAP,		/*!< Select Low pass Filter type */
	GCORE_TYPE_BIQUAD_FILTER_HIGH_PASS_ONE_TAP,		/*!< Select High pass Filter type */
	GCORE_TYPE_BIQUAD_FILTER_BAND_STOP,				/*!< Select Band Stop Filter type */
	GCORE_TYPE_BIQUAD_FILTER_ALL_PASS,				/*!< Select All Pass Filter type  */
	GCORE_TYPE_BIQUAD_FILTER_MAX,
} Gcore_DspBiquadFilterType_t;

enum
{
	DATA_LCH	= 0,
	DATA_RCH	= 1
};

/* Private macro -------------------------------------------------------------*/
typedef int16_t q15_t;

typedef float32_t	GCoreAudioDspData_t;
typedef q31_t 		GCoreAudioIntData_t;
typedef q15_t		GcoreRawAudioData_t;
/**
  * @brief  Bi-Quad Filter Raw parameters
  * @note 	Out = b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2]
  */
typedef struct {
	GCoreAudioDspData_t
		b0,		/*!< Parameter b0 */
		b1,		/*!< Parameter b1 */
		b2,		/*!< Parameter b2 */
	    a0, 	/*!< Parameter a0 */
		a1,		/*!< Parameter a1 */
		a2;		/*!< Parameter a2 */
} Gcore_DspBiquadConstData_t;

typedef struct {
	GCoreAudioIntData_t
		b0,		/*!< Parameter b0 */
		b1,		/*!< Parameter b1 */
		b2,		/*!< Parameter b2 */
		a1,		/*!< Parameter a1 */
		a2;		/*!< Parameter a2 */
} Gcore_DspBiquadConstIntData_t;

typedef struct {
	GCoreAudioDspData_t
		x1,		/*!< Parameter b1 */
		x2,		/*!< Parameter b2 */
		y1,		/*!< Parameter a1 */
		y2;		/*!< Parameter a2 */
} Gcore_DspBiquadState_t;

typedef struct {
	Gcore_DspBiquadFilterType_t type;	/*!< Select Filter type (See GCORE_TYPE_BIQUAD_FILTER)*/
	GCoreAudioDspData_t fc_Hz;			/*!< filter cut off frequency[Hz] */
	GCoreAudioDspData_t qbw;			/*!< filter Q or Band Width [Octave]*/
	GCoreAudioDspData_t gain_dB;		/*!< filter Gain [dB]*/
	uint8_t				shift_bits;		/*!< filter Pre shift [bit]*/
} Gcore_DspBiquadParams_t;

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
extern void CalcBiquadFilterInt(
		int sampleRate,
		Gcore_DspBiquadConstIntData_t *intResult,
		const Gcore_DspBiquadParams_t *param
		);

GCoreAudioDspData_t DoBiquadFilterF(GCoreAudioDspData_t x0, Gcore_DspBiquadConstData_t coff, Gcore_DspBiquadState_t *state);
float DspPow(float x, float y);
void SpkBoost_ClearData(void);
#ifdef __cplusplus
}
#endif

#endif  /* __SPKEREQ_H__ */

#endif
