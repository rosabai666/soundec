/*
 * agc.h
 *
 *  Created on: 2021Äê5ÔÂ21ÈÕ
 *      Author: User
 */

#ifndef __AGC__
#define __AGC__
#include "NatureDSP_Signal_math.h"
#include <stdlib.h>
#include <math.h>

typedef struct{
	uint32_t   in_sum;
	uint32_t   out_sum;
	float32_t cur_gain;
	float32_t in_ave;
	float32_t out_ave;
	int32_t   delay_pos;
	float32_t in_data;
	int32_t   out_data;
	int32_t   *agc_in_buff;
	int32_t   *agc_out_buff;
}audio_agc;

typedef struct{
	float32_t target_level;
	float32_t detect_level;
	float32_t max_gain;
	float32_t min_gain;
	float32_t hold_time;
	float32_t attack_time;
	float32_t release_time;
	float32_t sample_rate;
	float32_t delta_attack;
	float32_t delta_release;
	int32_t   delay_size;
	float32_t rms_factor;
	float32_t q31tof;
}audio_agc_param;

void AudioAgcInit(void);
void AudioMicAgcProcess(int32_t *data);
void agc_set_sample_rate(float32_t sampleRate);
float agc_get_sample_rate(void);
void agc_set_target(float32_t target);
float agc_get_target(void);
void agc_set_max_gain(float32_t gain);
float agc_get_max_gain(void);
void agc_set_min_gain(float32_t gain);
float agc_get_min_gain(void);
void agc_set_release_time(float32_t releaseTime);
float agc_get_release_time(void);
void agc_set_attack_time(float32_t attackTime);
float agc_get_attack_time(void);
void agc_set_hold_time(float32_t holdTime);
float agc_get_hold_time(void);
void agc_set_noise_threshold(float32_t threshold);
float agc_get_noise_threshold(void);

#endif /*  */
