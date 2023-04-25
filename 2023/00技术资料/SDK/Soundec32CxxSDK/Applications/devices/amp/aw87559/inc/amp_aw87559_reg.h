/*
 * amp_aw87559.h
 *
 *  Created on: 2021Äê7ÔÂ2ÈÕ
 *      Author: User
 */

#ifndef __AMP_AW87559_REG_H__

#define __AMP_AW87559_REG_H__

/* registers list */
#define AW87559_CHIPID_REG				(0x00)
#define AW87559_SYSCTRL_REG 			(0x01)
#define AW87559_BATSAFE_REG				(0x02)
#define AW87559_BSTOVR_REG				(0x03)
#define AW87559_BSTVPR_REG				(0x04)
#define AW87559_PAGR_REG				(0x05)
#define AW87559_PAGC3OPR_REG			(0x06)
#define AW87559_PAGC3PR_REG				(0x07)
#define AW88759_PAGC2OPR_REG			(0x08)
#define AW88759_PAGC2PR_REG				(0x09)
#define AW88759_PAGC1PR_REG 			(0x0A)

#define AW88759_CHIPID 					0X5A
#define AW88759_I2C_ADDR   		        (0x58 << 1)


#define AW88759_REG_MAX					16

#define AW88759_REG_NONE_ACCESS					(0)
#define AW88759_REG_RD_ACCESS					(1 << 0)
#define AW88759_REG_WR_ACCESS					(1 << 1)


#endif /*  */
