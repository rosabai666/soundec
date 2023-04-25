/** 
***********************************************************
 * 
 * @file name	: types.h
 * @author		: Rosa.Bai
 * @version		: Vxx.xx
 * @date		: 2020-02-13
 * @brief		: Common used typedefs
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TYPES_H__
#define __TYPES_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
//#include "NatureDSP_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ENABLE               1
#define DISABLE              0


typedef  enum
{
	FALSE = 0,
	TRUE = 1
}TBool;

enum
{
	LOW = 0,
	HIGH = 1,
};

typedef unsigned    char	 BYTE;
typedef unsigned    short    WORD;
typedef unsigned    long     DWORD;
typedef unsigned    long     ULONG;

typedef             char     INT8;
typedef unsigned    char     UINT8;
typedef signed      short    INT16;
typedef unsigned    short    UINT16;
typedef signed      int      INT32;
typedef unsigned    int      UINT32;
typedef signed      long long    INT64;
typedef unsigned    long long    UINT64;

typedef             char     S8;
typedef unsigned    char     U8;
typedef signed      short    S16;
typedef unsigned    short    U16;
typedef signed      int      S32;
typedef unsigned    int      U32;
typedef signed      long long    S64;
typedef unsigned    long long    U64;

typedef	volatile UINT64 *		RP64;
typedef	volatile UINT32 *		RP;
typedef	volatile UINT32 *		RP32;
typedef	volatile UINT16 *		RP16;
typedef	volatile UINT8  *		RP8;


typedef union
{
	U8 byte[4];	//lsb: byte[0]
	U16 word[2];
	U32 dWords;
	struct{
		U8 b0;	//lsb
		U8 b1;
		U8 b2;
		U8 b3;
	}bytes;
}U32_t;

typedef union
{
	U8 byte[2];	//lsb: byte[0]
	U16 words;
	struct{
		U8 b0;	//lsb
		U8 b1;
	}bytes;
}U16_t;


#ifdef __cplusplus
	#define		__I		volatile             /*!< Defines 'read only' permissions                 */
#else
	#define		__I     volatile const       /*!< Defines 'read only' permissions                 */
#endif
#define     	__O 	volatile             /*!< Defines 'write only' permissions                */
#define     	__IO	volatile             /*!< Defines 'read / write' permissions              */

////////////////////////////////////////////////////////////////////////////////
// Null Pointer Definitions ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifndef NULL
#define NULL                ((void *)0)
#endif

/**
 * @brief 32-bit fractional data type in 1.31 format.
 */
typedef int32_t 				q31_t;

/**
 * @brief 64-bit fractional data type in 1.63 format.
 */
typedef int64_t 				q63_t;

/**
 * @brief 32-bit floating-point type definition.
 */
//typedef float 					float32_t;

#define ALIGN(x)    		__attribute__((aligned(x)))

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  /* __TYPES_H__ */


