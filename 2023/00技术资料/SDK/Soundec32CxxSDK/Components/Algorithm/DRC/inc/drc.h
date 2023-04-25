/*
 * drc.h
 *
 *  Created on: 2021Äê5ÔÂ13ÈÕ
 *      Author: User
 */

#ifndef __DRC_H__
#define __DRC_H__

#include "NatureDSP_Signal_math.h"

#define SPK_CHANNELS 2

typedef struct{
	float32_t spk_in;
	float32_t spk_in_db;
	float32_t spk_out_db;
	float32_t spk_gain;
	float32_t spk_gs[SPK_CHANNELS];
	float32_t spk_auto_gain;
	float32_t spk_gm;
	float32_t spk_gl;
}audio_drc;

typedef struct{
	float32_t threshold;
	float32_t ratio;
	float32_t knee_width;
	float32_t attack_time;
	float32_t release_time;
	float32_t delta_attack;
	float32_t delta_release;
	float32_t sample_rate;
	float32_t q31tof;
}audio_drc_param;

void AudioDrcInit(void);
void AudioSpkDrcProcess(int32_t *data);
void drc_set_threshold(float threshold);
void drc_set_ratio(float ratio);
void drc_set_knee_width(float kneeWidth);
void drc_set_sample_rate(float sampleRate);
void drc_set_attack_time(float attackTime);
void drc_set_release_time(float releaseTime);
void drc_set_auto_gain(float gain);
float drc_get_threshold(void);
float drc_get_ratio(void);
float drc_get_knee_width(void);
float drc_get_sample_rate(void);
float drc_get_attack_time(void);
float drc_get_release_time(void);
float drc_get_auto_gain(void);

#endif /*  */
