/** 
***********************************************************
 * 
 * @file name	: dsp_eq.c
 * @author		: Soundec
 * @version		: Vxx.xx
 * @date		: 2021-1-12
 * @brief		: math
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


#ifndef __SOUNDEC_MATH_H__
#define __SOUNDEC_MATH_H__

#include "types.h"
#include <xtensa/tie/xt_hifi2.h>
#include <xtensa/tie/xt_FP.h>

#ifdef	__cplusplus
extern "C"
{
#endif


typedef struct
{
    uint32_t numStages;      /**< number of stages in the filter. */
    q31_t *pState;           /**< Points to the array of state coefficients. */
    q31_t *pCoeffs;          /**< Points to the array of coefficients. */
    uint8_t postShift;       /**< Additional shift, in bits, applied to each output sample. */
} biquad_casd_df1_inst_q31;

void biquad_cascade_df1_init_q31(
	biquad_casd_df1_inst_q31 * S,
    uint8_t numStages,
    q31_t * pCoeffs,
    q31_t * pState,
    int8_t postShift);

void biquad_cascade_df1_q31(
	biquad_casd_df1_inst_q31 * S,
    q31_t * pSrc,
    q31_t * pDst,
    uint32_t blockSize);

#ifdef	__cplusplus
}
#endif
#endif /* __SOUNDEC_MATH_H__ */
