/* Copyright (C) 2007 Jean-Marc Valin

File: speex_resampler.h
Resampling code

The design goals of this code are:
- Very fast algorithm
- Low memory requirement
- Good *perceptual* quality (and not best SNR)

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

3. The name of the author may not be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef SPEEX_RESAMPLER_H
#define SPEEX_RESAMPLER_H

#include "fixed_generic.h"
#include <stdint.h>
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RESAMPLER_MAX_FS 		192000
#define RESAMPLER_MIN_FS    	44100
#define RESAMPLER_MAX_QULITY 	0

#define RESAMPLER_MAX_CH_NUM    2

#if (RESAMPLER_MAX_FS == 192000) && (RESAMPLER_MIN_FS == 44100)
	#if (RESAMPLER_MAX_QULITY == 3)
		#define MIN_SINC_TABLE_LENGTH 	712
		#define MIN_PROC_BUF_LENGTH     367
	#elif (RESAMPLER_MAX_QULITY == 2)
		#define MIN_SINC_TABLE_LENGTH 	264
		#define MIN_PROC_BUF_LENGTH     303
	#elif (RESAMPLER_MAX_QULITY == 1)
		#define MIN_SINC_TABLE_LENGTH 	136
		#define MIN_PROC_BUF_LENGTH     231
	#elif (RESAMPLER_MAX_QULITY == 0)
		#define MIN_SINC_TABLE_LENGTH 	72
		#define MIN_PROC_BUF_LENGTH     199
	#endif
#endif

#define SPEEX_RESAMPLER_QUALITY_MAX 10
#define SPEEX_RESAMPLER_QUALITY_MIN 0
#define SPEEX_RESAMPLER_QUALITY_DEFAULT 4
#define SPEEX_RESAMPLER_QUALITY_VOIP 3
#define SPEEX_RESAMPLER_QUALITY_DESKTOP 5

	enum {
		RESAMPLER_ERR_SUCCESS = 0,
		RESAMPLER_ERR_ALLOC_FAILED = 1,
		RESAMPLER_ERR_BAD_STATE = 2,
		RESAMPLER_ERR_INVALID_ARG = 3,
		RESAMPLER_ERR_PTR_OVERLAP = 4,
		RESAMPLER_ERR_OVERFLOW = 5,

		RESAMPLER_ERR_MAX_ERROR
	};

	typedef struct ResamplerState_ ResamplerState;

	typedef int(*resampler_basic_func)(ResamplerState *, unsigned int, const int *, unsigned int *, int *, unsigned int *);

	//struct ResamplerState_;
	struct ResamplerState_ {
		unsigned int in_rate;
		unsigned int out_rate;
		unsigned int num_rate;
		unsigned int den_rate;

		int    quality;
		unsigned int nb_channels;
		unsigned int filt_len;
		unsigned int mem_alloc_size;
		unsigned int buffer_size;
		int          int_advance;
		int          frac_advance;
		float  cutoff;
		unsigned int oversample;
		int          initialised;
		int          started;

		/* These are per-channel */
		int  *last_sample;
		unsigned int *samp_frac_num;

		int *mem;
		short *sinc_table;
        short *sinc_table_base;
		unsigned int sinc_table_length;
		resampler_basic_func resampler_ptr;

		int    in_stride;
		int    out_stride;
    };



typedef struct{
	uint32_t inSr;
	uint32_t outSr;
	bool flag;
}RsrArg_t;

extern RsrArg_t rsrArg;
extern ResamplerState* rsrState;
extern volatile bool rsrEn;

void rsr_init(void);
void rsr_initParam(void);




/** Resample an interleaved float array. The input and output buffers must *not* overlap.
* @param st Resampler state
* @param in Input buffer
* @param in_len Number of input samples in the input buffer. Returns the number
* of samples processed. This is all per-channel.
* @param out Output buffer
* @param out_len Size of the output buffer. Returns the number of samples written.
* This is all per-channel.
*/
int resampler_process_interleaved_int(ResamplerState *st,
	const int *in,
	unsigned int *in_len,
	int *out,
	unsigned int *out_len);

#ifdef __cplusplus
}
#endif

#endif
