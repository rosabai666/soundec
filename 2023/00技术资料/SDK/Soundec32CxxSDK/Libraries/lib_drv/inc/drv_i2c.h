/** 
***********************************************************
 * 
 * @file name	: drv_i2c.c
 * @author		: RandyFan
 * @version		: V03.00
 * @date		: 2022-08-15
 * @brief		: header of drver.c
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
#ifndef __DRV_I2C_H__
#define __DRV_I2C_H__

/* Includes ------------------------------------------------------------------*/
#include "boardConfig.h"
#include "common.h"
#include "hal_i2c.h"



#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define I2C_MASTER_TX_FIFO			7
#define I2C_MASTER_RX_FIFO			7

#define I2C_SLAVE_TX_FIFO			4
#define I2C_SLAVE_RX_FIFO			4

#define I2C_MASTER_QUE_BUF_LEN		100
#define I2C_MASTER_QUE_NUM			5

#define I2C_SLAVE_QUE_NUM			5

typedef void (*i2cQueCb_t) (void* que);

typedef struct{
	uint8_t tarAddr;
	uint8_t regAddr;
	uint8_t buf[I2C_MASTER_QUE_BUF_LEN];
	uint8_t* p;
	uint8_t len;
	bool dir;
	bool sendAddr;
	i2cQueCb_t cb;
}i2cMasterQue_t;

typedef struct{
	uint8_t queTop;
	uint8_t queTail;
	i2cMasterQue_t que[I2C_MASTER_QUE_NUM];
	bool started;
	bool isQueueFull;
}i2cMasterHandle_t;


typedef struct{
	uint32_t startRegAddr;
	uint32_t len;
}i2cSlaveRxQue_t;

typedef struct{
	bool isRd;
	bool isAddr;
	uint8_t queTop;
	uint8_t queTail;
	i2cSlaveRxQue_t que[I2C_SLAVE_QUE_NUM];
	uint8_t* memBuf;
	uint32_t memLen;
	i2cQueCb_t memCb;
}i2cSlaveHandle_t;


#define I2C_DIR_WRITE				0
#define I2C_DIR_READ				1

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
#if(I2C_ENABLE == 1)
void i2c_init(void);
void i2c_slave_register_mem(uint8_t* pMem, uint32_t len, i2cQueCb_t cb);
void i2c_polling(void);
void i2c2_cfg_gpio(void);
hal_status_t i2c_master_add_write(uint8_t tarAddr, uint8_t regAddr, uint8_t* buf, uint8_t len, i2cQueCb_t cb);
hal_status_t i2c_master_add_read(uint8_t tarAddr, uint8_t regAddr, uint8_t len, i2cQueCb_t cb);
void i2c_master_read_bulk(i2cBase_t i2cx, uint8_t devAddr, uint8_t regAddr, uint8_t* buf, uint8_t len);
void i2c_master_write_bulk(i2cBase_t i2cx, uint8_t devAddr, uint8_t regAddr, uint8_t* buf, uint8_t len);


#endif //I2C_ENABLE

#ifdef __cplusplus
}
#endif

#endif  /* __DRV_I2C_H__ */

