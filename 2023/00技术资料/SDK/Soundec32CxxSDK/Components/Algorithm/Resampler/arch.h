/* Copyright (C) 2003 Jean-Marc Valin */
/**
   @file arch.h
   @brief Various architecture definitions Speex
*/
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   - Neither the name of the Xiph.org Foundation nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef ARCH_H
#define ARCH_H

/* A couple test to catch stupid option combinations */

#define ABS(x) ((x) < 0 ? (-(x)) : (x))      /**< Absolute integer value. */
#define ABS16(x) ((x) < 0 ? (-(x)) : (x))    /**< Absolute 16-bit value.  */
#define MIN16(a,b) ((a) < (b) ? (a) : (b))   /**< Maximum 16-bit value.   */
#define MAX16(a,b) ((a) > (b) ? (a) : (b))   /**< Maximum 16-bit value.   */
#define ABS32(x) ((x) < 0 ? (-(x)) : (x))    /**< Absolute 32-bit value.  */
#define MIN32(a,b) ((a) < (b) ? (a) : (b))   /**< Maximum 32-bit value.   */
#define MAX32(a,b) ((a) > (b) ? (a) : (b))   /**< Maximum 32-bit value.   */


#define Q15ONE 1.0f
#define LPC_SCALING  1.f
#define SIG_SCALING  1.f
#define LSP_SCALING  1.f
#define GAMMA_SCALING 1.f
#define GAIN_SCALING 1.f
#define GAIN_SCALING_1 1.f


#define VERY_SMALL 1e-15f
#define VERY_LARGE32 1e15f
#define VERY_LARGE16 1e15f
#define Q15_ONE ((float)1.f)

#define QCONST16(x,bits) (x)
#define QCONST32(x,bits) (x)

#define NEG16(x) (-(x))
#define NEG32(x) (-(x))
#define EXTRACT16(x) (x)
#define EXTEND32(x) (x)
#define SHR16(a,shift) (a)
#define SHL16(a,shift) (a)
#define SHR32(a,shift) (a)
#define SHL32(a,shift) (a)
#define PSHR16(a,shift) (a)
#define PSHR32(a,shift) (a)
#define VSHR32(a,shift) (a)
#define SATURATE16(x,a) (x)
#define SATURATE32(x,a) (x)
#define SATURATE32PSHR(x,shift,a) (x)

#define PSHR(a,shift)       (a)
#define SHR(a,shift)       (a)
#define SHL(a,shift)       (a)
#define SATURATE(x,a) (x)

#define ADD16(a,b) ((a)+(b))
#define SUB16(a,b) ((a)-(b))
#define ADD32(a,b) ((a)+(b))
#define SUB32(a,b) ((a)-(b))
#define MULT16_16_16(a,b)     ((a)*(b))
#define MULT16_16(a,b)     ((float)(a)*(float)(b))
#define MAC16_16(c,a,b)     ((c)+(float)(a)*(float)(b))

#define MULT16_32_Q11(a,b)     ((a)*(b))
#define MULT16_32_Q13(a,b)     ((a)*(b))
#define MULT16_32_Q14(a,b)     ((a)*(b))
#define MULT16_32_Q15(a,b)     ((a)*(b))
#define MULT16_32_P15(a,b)     ((a)*(b))

#define MAC16_32_Q11(c,a,b)     ((c)+(a)*(b))
#define MAC16_32_Q15(c,a,b)     ((c)+(a)*(b))

#define MAC16_16_Q11(c,a,b)     ((c)+(a)*(b))
#define MAC16_16_Q13(c,a,b)     ((c)+(a)*(b))
#define MAC16_16_P13(c,a,b)     ((c)+(a)*(b))
#define MULT16_16_Q11_32(a,b)     ((a)*(b))
#define MULT16_16_Q13(a,b)     ((a)*(b))
#define MULT16_16_Q14(a,b)     ((a)*(b))
#define MULT16_16_Q15(a,b)     ((a)*(b))
#define MULT16_16_P15(a,b)     ((a)*(b))
#define MULT16_16_P13(a,b)     ((a)*(b))
#define MULT16_16_P14(a,b)     ((a)*(b))

#define DIV32_16(a,b)     (((float)(a))/(float)(b))
#define PDIV32_16(a,b)     (((float)(a))/(float)(b))
#define DIV32(a,b)     (((float)(a))/(float)(b))
#define PDIV32(a,b)     (((float)(a))/(float)(b))

#define WORD2INT(x) ((x) < -32767.5f ? -32768 : \
                    ((x) > 32766.5f ? 32767 : (int16_t)floor(.5 + (x))))

#define BYTES_PER_CHAR 1
#define BITS_PER_CHAR 8
#define LOG2_BITS_PER_CHAR 3


#endif
