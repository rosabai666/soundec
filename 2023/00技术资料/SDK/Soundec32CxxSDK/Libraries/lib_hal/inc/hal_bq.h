/**
***********************************************************
 *
 * @file name	: hal_bq.h
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-02-23
 * @brief		: HAL BiQuad Filter Driver Header File
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

#ifndef __HAL_BQ_H__
#define __HAL_BQ_H__

#include "types.h"
#include "hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	This struct defines the registers of the BiQuad Filter
 *
 * @details	{description}
 */
typedef struct {
	__I int32_t reserved;
	__O uint32_t calc_en;	/* Calculation enable */
	__I uint32_t status;	/* Calculation complete status */
	__O int32_t in1;		/* Input data of Stream 1 */
	__O int32_t in2;		/* Input data of Stream 2 */
	__O int32_t in3;		/* Input data of Stream 3 */
	__O int32_t in4;		/* Input data of Stream 4 */

	__O int32_t a1_11;		/* The 1st a1 coefficient in Stream 1 */
	__O int32_t a1_12;		/* The 2nd a1 coefficient in Stream 1 */
	__O int32_t a1_13;		/* The 3rd a1 coefficient in Stream 1 */
	__O int32_t a1_14;		/* The 4th a1 coefficient in Stream 1 */

	__O int32_t a1_21;		/* The 1st a1 coefficient in Stream 2 */
	__O int32_t a1_22;		/* The 2nd a1 coefficient in Stream 2 */
	__O int32_t a1_23;		/* The 3rd a1 coefficient in Stream 2 */
	__O int32_t a1_24;		/* The 4th a1 coefficient in Stream 2 */

	__O int32_t a1_31;		/* The 1st a1 coefficient in Stream 3 */
	__O int32_t a1_32;		/* The 2nd a1 coefficient in Stream 3 */
	__O int32_t a1_33;		/* The 3rd a1 coefficient in Stream 3 */
	__O int32_t a1_34;		/* The 4th a1 coefficient in Stream 3 */

	__O int32_t a1_41;		/* The 1st a1 coefficient in Stream 4 */
	__O int32_t a1_42;		/* The 2nd a1 coefficient in Stream 4 */
	__O int32_t a1_43;		/* The 3rd a1 coefficient in Stream 4 */
	__O int32_t a1_44;		/* The 4th a1 coefficient in Stream 4 */

	__O int32_t a2_11;		/* The 1st a2 coefficient in Stream 1 */
	__O int32_t a2_12;		/* The 2nd a2 coefficient in Stream 1 */
	__O int32_t a2_13;		/* The 3rd a2 coefficient in Stream 1 */
	__O int32_t a2_14;		/* The 4th a2 coefficient in Stream 1 */

	__O int32_t a2_21;		/* The 1st a2 coefficient in Stream 2 */
	__O int32_t a2_22;		/* The 2nd a2 coefficient in Stream 2 */
	__O int32_t a2_23;		/* The 3rd a2 coefficient in Stream 2 */
	__O int32_t a2_24;		/* The 4th a2 coefficient in Stream 2 */

	__O int32_t a2_31;		/* The 1st a2 coefficient in Stream 3 */
	__O int32_t a2_32;		/* The 2nd a2 coefficient in Stream 3 */
	__O int32_t a2_33;		/* The 3rd a2 coefficient in Stream 3 */
	__O int32_t a2_34;		/* The 4th a2 coefficient in Stream 3 */

	__O int32_t a2_41;		/* The 1st a2 coefficient in Stream 4 */
	__O int32_t a2_42;		/* The 2nd a2 coefficient in Stream 4 */
	__O int32_t a2_43;		/* The 3rd a2 coefficient in Stream 4 */
	__O int32_t a2_44;		/* The 4th a2 coefficient in Stream 4 */

	__O int32_t b0_11;		/* The 1st b0 coefficient in Stream 1 */
	__O int32_t b0_12;		/* The 2nd b0 coefficient in Stream 1 */
	__O int32_t b0_13;		/* The 3rd b0 coefficient in Stream 1 */
	__O int32_t b0_14;		/* The 4th b0 coefficient in Stream 1 */

	__O int32_t b0_21;		/* The 1st b0 coefficient in Stream 2 */
	__O int32_t b0_22;		/* The 2nd b0 coefficient in Stream 2 */
	__O int32_t b0_23;		/* The 3rd b0 coefficient in Stream 2 */
	__O int32_t b0_24;		/* The 4th b0 coefficient in Stream 2 */

	__O int32_t b0_31;		/* The 1st b0 coefficient in Stream 3 */
	__O int32_t b0_32;		/* The 2nd b0 coefficient in Stream 3 */
	__O int32_t b0_33;		/* The 3rd b0 coefficient in Stream 3 */
	__O int32_t b0_34;		/* The 4th b0 coefficient in Stream 3 */

	__O int32_t b0_41;		/* The 1st b0 coefficient in Stream 4 */
	__O int32_t b0_42;		/* The 2nd b0 coefficient in Stream 4 */
	__O int32_t b0_43;		/* The 3rd b0 coefficient in Stream 4 */
	__O int32_t b0_44;		/* The 4th b0 coefficient in Stream 4 */

	__O int32_t b1_11;		/* The 1st b1 coefficient in Stream 1 */
	__O int32_t b1_12;		/* The 2nd b1 coefficient in Stream 1 */
	__O int32_t b1_13;		/* The 3rd b1 coefficient in Stream 1 */
	__O int32_t b1_14;		/* The 4th b1 coefficient in Stream 1 */

	__O int32_t b1_21;		/* The 1st b1 coefficient in Stream 2 */
	__O int32_t b1_22;		/* The 2nd b1 coefficient in Stream 2 */
	__O int32_t b1_23;		/* The 3rd b1 coefficient in Stream 2 */
	__O int32_t b1_24;		/* The 4th b1 coefficient in Stream 2 */

	__O int32_t b1_31;		/* The 1st b1 coefficient in Stream 3 */
	__O int32_t b1_32;		/* The 2nd b1 coefficient in Stream 3 */
	__O int32_t b1_33;		/* The 3rd b1 coefficient in Stream 3 */
	__O int32_t b1_34;		/* The 4th b1 coefficient in Stream 3 */

	__O int32_t b1_41;		/* The 1st b1 coefficient in Stream 4 */
	__O int32_t b1_42;		/* The 2nd b1 coefficient in Stream 4 */
	__O int32_t b1_43;		/* The 3rd b1 coefficient in Stream 4 */
	__O int32_t b1_44;		/* The 4th b1 coefficient in Stream 4 */

	__O int32_t b2_11;		/* The 1st b2 coefficient in Stream 1 */
	__O int32_t b2_12;		/* The 2nd b2 coefficient in Stream 1 */
	__O int32_t b2_13;		/* The 3rd b2 coefficient in Stream 1 */
	__O int32_t b2_14;		/* The 4th b2 coefficient in Stream 1 */

	__O int32_t b2_21;		/* The 1st b2 coefficient in Stream 2 */
	__O int32_t b2_22;		/* The 2nd b2 coefficient in Stream 2 */
	__O int32_t b2_23;		/* The 3rd b2 coefficient in Stream 2 */
	__O int32_t b2_24;		/* The 4th b2 coefficient in Stream 2 */

	__O int32_t b2_31;		/* The 1st b2 coefficient in Stream 3 */
	__O int32_t b2_32;		/* The 2nd b2 coefficient in Stream 3 */
	__O int32_t b2_33;		/* The 3rd b2 coefficient in Stream 3 */
	__O int32_t b2_34;		/* The 4th b2 coefficient in Stream 3 */

	__O int32_t b2_41;		/* The 1st b2 coefficient in Stream 4 */
	__O int32_t b2_42;		/* The 2nd b2 coefficient in Stream 4 */
	__O int32_t b2_43;		/* The 3rd b2 coefficient in Stream 4 */
	__O int32_t b2_44;		/* The 4th b2 coefficient in Stream 4 */

	__I int32_t out1;		/* The output data of Stream 1 */
	__I int32_t out2;		/* The output data of Stream 2 */
	__I int32_t out3;		/* The output data of Stream 3 */
	__I int32_t out4;		/* The output data of Stream 4 */
	__O uint32_t post_shift;/* The right shift bits of every coefficient */
	__O uint32_t resetn;	/* Reset BQ, write 0 to reset */
} hal_bq_filter_t;

/* @brief The structure defines the BQ coefficients */
typedef struct {
	int32_t b0;
	int32_t b1;
	int32_t b2;
	int32_t a1;
	int32_t a2;
} hal_bq_coef_t;

/* @brief This enum defines the BQ Streams */
typedef enum {
	HAL_BQ_STREAM1 = 0,
	HAL_BQ_STREAM2,
	HAL_BQ_STREAM3,
	HAL_BQ_STREAM4,
	HAL_BQ_STREAM_NUM,
} hal_bq_stream_t;

/**
 * @brief	This function is used to initialize the specified stream coefficients of the BiQuad Filter
 * @param[in]	stream      The target stream of the BiQuad Filter is initialized.
 * @param[in]	coef        The stream coefficients, the size should be 4
 * @return
 * 	@retval HAL_STATUS_OK The coefficients of the BiQuad Filter is successfully initialized.
 */
hal_status_t hal_bq_init_coef(hal_bq_stream_t stream, hal_bq_coef_t* coef);

/**
 * @brief	This function is used to initialize the post shift setting for the whole BiQuad Filters.
 * @note	The post shift setting is used to prevent the calculation result of the BiQuad Filter is saturated.
 *			So the coefficients should be right shift #post_shift bits before set them to the register.
 * @param[in]	post_shift  The post shift value is set
 * @return
 * 	@retval HAL_STATUS_OK The post shift value is successfully set.
 */
hal_status_t hal_bq_set_post_shift(uint32_t post_shift);

/**
 * @brief	This function is used to calculate the specified stream of the BiQuad Filters.
 * @param[in]	stream      The target stream of the BiQuad Filters needs to be calculated.
 * @param[in]	in          The input data for the specified stream of the BiQuad Filters.
 * @param[out]	out         The calculation result for the specified stream of the BiQuad Filters.
 * @return	None
 */
void hal_bq_calc(hal_bq_stream_t stream, int32_t in, int32_t *out);

/**
 * @brief	This function is used to calculate all streams of the BiQuad Filters.
 * @param[in]	in          The input data array of the BiQuad Filters.
 * @param[out]	out         The calculation results of the BiQuad Filters.
 * @return	None
 */
void hal_bq_calc_all(int32_t *in, int32_t *out);

/**
 * @brief	This function is used to calculate the stream 1&2 of the BiQuad Filters.
 * @param[in]	in          The input data array for the stream 1&2 of the BiQuad Filters.
 * @param[out]	out         The calculation results for the stream 1&2 of the BiQuad Filters.
 * @return	None
 */
void hal_bq_calc_stream12(int32_t *in, int32_t *out);

/**
 * @brief	This function is used to calculate the stream 3&4 of the BiQuad Filters.
 * @param[in]	in          The input data array for the stream 3&4 of the BiQuad Filters.
 * @param[out]	out         The calculation results for the stream 3&4 of the BiQuad Filters.
 * @return	None
 */
void hal_bq_calc_stream34(int32_t *in, int32_t *out);

/**
 * @brief	This function is used to reset the whole BiQuad Filters
 * @note	Especially, clear all delay registers of the BiQuad Filters
 * @return
 * 	@retval HAL_STATUS_OK The BiQuad is successfully reset.
 */
hal_status_t hal_bq_reset(void);

/**
 * @brief	This function is used to reset the specified stream ofthe BiQuad Filter
 * @note	Especially, clear the delay registers of the BiQuad Filters
 * @return
 * 	@retval HAL_STATUS_OK The BiQuad is successfully reset.
 */
hal_status_t hal_bq_reset_stream(hal_bq_stream_t stream);

/**
 * @brief	This function is used to initialize the specified stream of the BiQuad Filter.
 * @param[in]	stream      The target stream of the BiQuad Filters needs to be initialized.
 * @param[in]	coef        The stream coefficients, the size should be 4
 * @return
 * 	@retval HAL_STATUS_OK The specified stream of BiQuad Filter is successfully initialized.
 */
hal_status_t hal_bq_init(hal_bq_stream_t stream, hal_bq_coef_t *coef);


hal_status_t hal_bq_init_new(hal_bq_stream_t stream, hal_bq_coef_t *coef, uint32_t coef_pos);  //coef_pos ; 0 : do nothing,get eq prarmaters  0~4,  1:  get eq prarmaters  4~7.

/**
 * @brief	This function is used to enable the specified stream of the BiQuad Filter.
 * @param[in]	None
 * @return  None
 */
void hal_bq_hardware_enable(void);
void hal_bq_hardware_disable(void);

#ifdef __cplusplus
}
#endif
#endif /* __HAL_BQ_H__ */
