/** 
***********************************************************
 * 
 * @file name	: hal_common.h
 * @author		: yen.yu
 * @version		: Vxx.xx
 * @date		: 2022-03-26
 * @brief		: Split from common.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_COMMON_H__
#define __HAL_COMMON_H__

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "assert.h"
#include "types.h"
#include "SaveRestoreVectorRegisters.h"
#include "hal_pll.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/****************************************
        macros  for write and read registers
****************************************/
#define write_reg(reg, data)				*(RP)(reg) = data
#define write_reg32(reg, data) 				*(RP)(reg) = data
#define write_reg64(reg, dataH, dataL)		*(RP)(reg) = dataL; *(RP)(reg+0x4) = dataH

#define read_reg(reg)						*(RP)(reg)
#define read_reg32(reg)						*(RP)(reg)

//#define printf(fmt,)						uart_printf(fmt,...)

#define IS_PERIPHERAL_ENABLED(peripherals) ((defined(peripherals ## _ENABLED) && (peripherals ## _ENABLED)) ? 1 : 0)
//#define ASSERT(x)  							while(!(x))
#if 0
/**
  * @brief  Reports the name of the source file and the source line number
  * 		where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None*/
void __assert_failed(U8* file, U32 line)
{
    /* User can add his own implementation to report the file name and line number,
                ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* Infinite loop */
    //#   warning "Wrong parameters value: file %s on line %d\r\n", file, line"

    //#warning ""
    while (1){}
}

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports
  *         	  the name of the source file and the source line number of the call
  *         	  that failed. If expr is true, it returns no value.
  * @retval None
  */
#define assert_param(expr) 				((expr) ? (void)0 : __assert_failed((U8 *)__FILE__, __LINE__))
#endif

#define BLSFT(n)						(1<<(n))		 /*Bit left shift*/
#define SETBIT(a,n) 					(a)|=(1<<(n))    /**< Set bit n of a */
#define CLRBIT(a,n) 					(a)&=~(1<<(n))   /**< Clear bit n of a*/
#define INVBIT(a,n) 					(a)^=(1<<(n))    /**< Reverse bit n of a */

#define GET_MSB32(a) 					(((a)& 0xFF000000)>>24)
#define GET_LSB32(a) 					((a)&0x000000FF)
#define GET_MSB16(a) 					(((a)&0xFF00)>>8)
#define GET_LSB16(a) 					((a)&0x00FF)
#define GET_U32(b3,b2,b1,b0)			(((b3)<<24)|((b2)<<16)|((b1)<<8)|(b0))

#define MAX(x,y)  						(((x)>(y))?(x):(y))
#define MIN(x,y)  						(((x)<(y))?(x):(y))
#define SWAP(x,y)						{(x)=(x)+(y);(y)=(x)-(y);(x)=(x)-(y);}
#define SWAP16(A) 						((((U16)(A)&0xff00)>>8)|(((U16)(A)&0x00ff)<<8))
#define SWAP32(A) 						((((U32)(A)&0xff000000)>>24)|(((U32)(A)&0x00ff0000)>>8)|(((U32)(A)&0x0000ff00)<<8)|(((U32)(A)&0x000000ff)<<24))

#define FLOPW(ray,val) 					(ray)[0]=((val)/256); (ray)[1]=((val)&0xFF)

#define DECCHK(ch) 						((ch)>= '0'&&(ch)<='9')
#define HEXCHK(ch) 						(((ch)>= '0'&&(ch)<='9')||((ch)>= 'A'&&(ch)<='F')||((ch)>= 'a'&&(ch)<='f'))

#define MEMBER_SIZE(types,member)		sizeof(((types *)0)->member)
#define ARRAY_SIZE(a)					(sizeof((a))/sizeof((a[0])))

// For system include
#define CONCAT(x,y)       	 			x ## y
#define SET_STR1(x)         			#x
#define SET_STR(x)          			SET_STR1(x)
#define SET_PATH(x,y) 					SET_STR(CONCAT(x, y))
#define BOARD_PATH						FPGA_TestVersion

/*    Assertion support                   */
#if !defined(_ASSERT)
  #include <assert.h>
  #if defined(_DEBUG) && defined(COMPILER_MSVC)
    #define ASSERT(x)  assert(x)
  #else

//#undef ASSERT
#ifndef ASSERT
	#define ASSERT(_ignore)  ((void)0)
#endif

  #endif  /* _DEBUG */
#else  /* ASSERT*/
  #define ASSERT(exp) \
	{\
  		extern void ExternalAssertHandler(void *, void *, unsigned);\
		(void)( (exp) || (ExternalAssertHandler(#exp, __FILE__, __LINE__), 0) );\
	}
#endif  /* ASSERT */
#ifdef __cplusplus
	}
#endif
	
#endif  /* __HAL_COMMON_H__ */


