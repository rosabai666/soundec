
#ifndef __DSP_INTERRUPT_H__
#define __DSP_INTERRUPT_H__

/* Includes ------------------------------------------------------------------*/
#include "NatureDSP_types.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct{
	void (*Save_INT)(uint32_t *mask);
	void (*Restore_INT)(uint32_t mask);
}DSP_InitHandle;

bool DSP_INT_Init(DSP_InitHandle *handle);
void SaveInt(void);
void RestoreInt(void);

#ifdef __cplusplus
 }
#endif

#endif /*__DSP_INTERRUPT_H__*/
