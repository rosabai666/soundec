#ifndef __DRV_SW_I2C_H__
#define __DRV_SW_I2C_H__
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern U8 sw_i2c_rxStr(U8 iDeviceAddr,U8 iAddr,U8 *Rdata_buf,U8 len,U8 noregaddr);
extern U8 sw_i2c_txStr(U8 iDeviceAddr,U8 iAddr,U8 *iDataBuf,U8 iLen,U8 iLen_x);
extern void sw_i2c_init(void);
#ifdef __cplusplus
}
#endif

#endif
