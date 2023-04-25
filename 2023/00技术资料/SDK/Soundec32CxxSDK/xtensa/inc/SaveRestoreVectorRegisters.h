/*
 * SaveRestoreVectorRegisters.h
 *
 *  Created on: 2020Äê10ÔÂ9ÈÕ
 *      Author: User
 */

#ifndef __SAVE_RESTORE_VECTOR_REGISTERS_H__
#define __SAVE_RESTORE_VECTOR_REGISTERS_H__

#ifdef __cplusplus
extern "C" {
#endif

void SaveVectors(int intr);

void RestoreVectors(int intr);

#ifdef __cplusplus
}
#endif

#endif /* __SAVE_RESTORE_VECTOR_REGISTERS_H__ */
