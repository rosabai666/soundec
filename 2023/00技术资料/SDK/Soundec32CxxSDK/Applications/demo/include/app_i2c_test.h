#ifndef __APP_I2C_TEST_H__
#define __APP_I2C_TEST_H__

#include "drv_i2c.h"

#if(I2C_MASTER_ENABLE)
void app_i2c_test_master(void);
#endif

#if(I2C_SLAVE_ENABLE)
void app_i2c_test_slave(void);
#endif

#endif /* __APP_I2C_TEST_H__ */
