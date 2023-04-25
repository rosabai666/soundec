/** 
***********************************************************
 * 
 * @file name	: drv_i2c.c
 * @author		: RandyFan
 * @version		: V03.00
 * @date		: 2022-08-15
 * @brief		: i2c driver
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

/* Includes ------------------------------------------------------------------*/
#include "hal.h"

#include "hal_inc.h"
#include "hal_i2c.h"
#include "drv_i2c.h"
#include "hal_sysctrl.h"
#include "drv_uart.h"


/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

#define I2C_MASTER_CFG 						\
{												\
	.isMaster 				= 	I2C_MASTER,				\
	.useIntr				=	1,						\
	.speedMode				=	I2C_FAST_SPEED_MODE,	\
	.rxFifo					=	I2C_MASTER_RX_FIFO,		\
	.txFifo					=	I2C_MASTER_TX_FIFO,		\
	.slaveAddr				=	0x00,					\
	.speed					=	400,					\
	.intrGenCallEn			=	0,						\
	.intrStartEn			=	0,						\
	.intrStopEn				=	1,						\
	.intrActivityEn			=	0,						\
	.intrRxDoneEn			=	0,						\
	.intrTxAbortEn			=	0,						\
	.intrRdReqEn			=	0,						\
	.intrTxEmptyEn			=	0,						\
	.intrTxTxOverEn			=	0,						\
	.intrRxFullEn			=	0,						\
	.intrRxOverEn			=	0,						\
	.intrRxUnderEn			=	1,						\
}

#define I2C_SLAVE_CFG 						\
{												\
	.isMaster 				= 	I2C_SLAVE,				\
	.useIntr				=	1,						\
	.speedMode				=	I2C_FAST_SPEED_MODE,	\
	.rxFifo					=	I2C_SLAVE_TX_FIFO,		\
	.txFifo					=	I2C_SLAVE_RX_FIFO,		\
	.slaveAddr				=	SLAVE_ADDRESS,			\
	.speed					=	400,					\
	.intrGenCallEn			=	0,						\
	.intrStartEn			=	0,						\
	.intrStopEn				=	1,						\
	.intrActivityEn			=	0,						\
	.intrRxDoneEn			=	0,						\
	.intrTxAbortEn			=	0,						\
	.intrRdReqEn			=	1,						\
	.intrTxEmptyEn			=	0,						\
	.intrTxTxOverEn			=	0,						\
	.intrRxFullEn			=	1,						\
	.intrRxOverEn			=	0,						\
	.intrRxUnderEn			=	0,						\
}

#if(I2C_ENABLE == 1)
/* Private variables ---------------------------------------------------------*/
i2cMasterHandle_t i2cMasterHandle;
i2cSlaveHandle_t i2cSlaveHandle;
/* Private function prototypes -----------------------------------------------*/
static void i2c_master_isr(void* arg);
static void i2c_slave_isr(void* arg);

/* Private functions ---------------------------------------------------------*/
void i2c_init(void)
{
	uint32_t clkGate = hal_sysctrl_get_clock_gate();
#if(I2C_MASTER_ENABLE)
#if(I2C_MASTER_SEL == I2C1_BASE)
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_I2C1);
#else
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_I2C2);
	i2c2_cfg_gpio();
#endif
#endif
#if(I2C_SLAVE_ENABLE)
#if(I2C_SLAVE_SEL == I2C1_BASE)
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_I2C1);
#else
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_I2C2);
	i2c2_cfg_gpio();
#endif
#endif
	hal_sysctrl_set_clock_gate(clkGate);

#if(I2C_MASTER_ENABLE)
	i2c_cfgs_t i2cMasterCfg = I2C_MASTER_CFG;

	hal_i2c_disable(I2C_MASTER_SEL);
	hal_i2c_set_mode(I2C_MASTER_SEL, &i2cMasterCfg);
	hal_i2c_set_speed(I2C_MASTER_SEL, &i2cMasterCfg);
	hal_i2c_set_tx_fifo_level(I2C_MASTER_SEL, i2cMasterCfg.txFifo);
	hal_i2c_set_rx_fifo_level(I2C_MASTER_SEL, i2cMasterCfg.rxFifo);

	if(i2cMasterCfg.useIntr)
	{
		hal_i2c_set_intr_mask(I2C_MASTER_SEL, &i2cMasterCfg);
		hal_interrupt_register_isr_handler(I2C_MASTER_SEL_IRQ, i2c_master_isr);
		hal_interrupt_enable_irq(I2C_MASTER_SEL_IRQ);
	}

	hal_i2c_enable(I2C_MASTER_SEL);

	memset(&i2cMasterHandle, 0, sizeof(i2cMasterHandle_t));
#endif	//I2C_MASTER_ENABLE

#if(I2C_SLAVE_ENABLE)
	i2c_cfgs_t i2cSlaveCfg = I2C_SLAVE_CFG;

	hal_i2c_disable(I2C_SLAVE_SEL);
	hal_i2c_set_mode(I2C_SLAVE_SEL, &i2cSlaveCfg);
	hal_i2c_set_speed(I2C_SLAVE_SEL, &i2cSlaveCfg);
	hal_i2c_set_tx_fifo_level(I2C_SLAVE_SEL, i2cSlaveCfg.txFifo);
	hal_i2c_set_rx_fifo_level(I2C_SLAVE_SEL, i2cSlaveCfg.rxFifo);

	if(i2cSlaveCfg.useIntr)
	{
		hal_i2c_set_intr_mask(I2C_SLAVE_SEL, &i2cSlaveCfg);
		hal_interrupt_register_isr_handler(I2C_SLAVE_SEL_IRQ, i2c_slave_isr);
		hal_interrupt_enable_irq(I2C_SLAVE_SEL_IRQ);
	}

	hal_i2c_enable(I2C_SLAVE_SEL);
#endif	//I2C_SLAVE_ENABLE
}

void i2c_slave_register_mem(uint8_t* pMem, uint32_t len, i2cQueCb_t cb)
{
	memset(&i2cSlaveHandle, 0, sizeof(i2cSlaveHandle_t));
	i2cSlaveHandle.isAddr = 1;
	i2cSlaveHandle.memBuf = pMem;
	i2cSlaveHandle.memLen = len;
	i2cSlaveHandle.memCb = cb;
}

void i2c2_cfg_gpio(void)
{
	hal_gpio_set_pinmux_function(GPIO_10, GPIO_10_FUNC_I2C2_SCl);
	hal_gpio_set_pinmux_function(GPIO_11, GPIO_11_FUNC_I2C2_SDA);
}

#if(I2C_MASTER_ENABLE == 1)
static void i2c_master_start_que(void)
{
	i2cMasterHandle.started = 1;
	i2cMasterQue_t* que = &i2cMasterHandle.que[i2cMasterHandle.queTop];

	hal_i2c_update_tar_addr(I2C_MASTER_SEL, que->tarAddr);

	uint32_t mask;
	uint8_t transLen = 0;

	if(que->dir == I2C_DIR_WRITE)
	{
		transLen = (que->len > I2C_MASTER_TX_FIFO)?I2C_MASTER_TX_FIFO:que->len;

		hal_interrupt_save_and_set_interrupt_mask(&mask);
		hal_i2c_write_data(I2C_MASTER_SEL, que->regAddr);
		hal_i2c_write_bulk(I2C_MASTER_SEL, que->p, transLen);
		hal_interrupt_restore_interrupt_mask(mask);

		que->len -= transLen;
		que->p += transLen;
		que->regAddr += transLen;
	}
	else
	{
		while(hal_i2c_get_rx_fifo_len(I2C_MASTER_SEL))
		{
			hal_i2c_read_data(I2C_MASTER_SEL);
		}
		que->sendAddr = 1;
		hal_i2c_write_data(I2C_MASTER_SEL, que->regAddr);
	}
}

hal_status_t i2c_master_add_que(bool dir, uint8_t tarAddr, uint8_t regAddr, uint8_t* buf, uint8_t len, i2cQueCb_t cb)
{
	if (i2cMasterHandle.isQueueFull)//((i2cMasterHandle.queTail == i2cMasterHandle.queTop) && (i2cMasterHandle.started))
	{
		uart_printf("i2c que full!\n");
		return HAL_STATUS_FAIL;
	}
	i2cMasterQue_t* que = &i2cMasterHandle.que[i2cMasterHandle.queTail];
	que->tarAddr = tarAddr;
	que->regAddr = regAddr;
	que->p = que->buf;
	que->len = len;
	que->dir = dir;
	que->cb = cb;

	if (len>I2C_MASTER_QUE_BUF_LEN)
	{
		return HAL_STATUS_FAIL;
	}

	if (dir == I2C_DIR_WRITE)
		memcpy(que->p, buf, len>I2C_MASTER_QUE_BUF_LEN?I2C_MASTER_QUE_BUF_LEN:len);

	i2cMasterHandle.queTail++;
	if(i2cMasterHandle.queTail == I2C_MASTER_QUE_NUM)
	{
		i2cMasterHandle.isQueueFull = TRUE;
		i2cMasterHandle.queTail = 0;
	}

	if(!i2cMasterHandle.started)
		i2c_master_start_que();
	return HAL_STATUS_OK;
}

hal_status_t i2c_master_add_write(uint8_t tarAddr, uint8_t regAddr, uint8_t* buf, uint8_t len, i2cQueCb_t cb)
{
	if (i2c_master_add_que(I2C_DIR_WRITE, tarAddr, regAddr, buf, len, cb) == HAL_STATUS_FAIL)
		return HAL_STATUS_FAIL;

	return HAL_STATUS_OK;
}

hal_status_t i2c_master_add_read(uint8_t tarAddr, uint8_t regAddr, uint8_t len, i2cQueCb_t cb)
{
	if (i2c_master_add_que(I2C_DIR_READ, tarAddr, regAddr, NULL, len, cb) == HAL_STATUS_FAIL)
		return HAL_STATUS_FAIL;

	return HAL_STATUS_OK;
}

static void i2c_master_isr(void* arg)
{
	SaveVectors(I2C_MASTER_SEL_IRQ);
	uint16_t stat = hal_i2c_get_intr_stat(I2C_MASTER_SEL);
	hal_i2c_clr_intr(I2C_MASTER_SEL);

	uint8_t transLen = 0;
	i2cMasterQue_t* que = &i2cMasterHandle.que[i2cMasterHandle.queTop];

	if (stat & I2C_INTR_STAT_RX_UNDER)
	{
		transLen = (que->len > I2C_MASTER_RX_FIFO)?I2C_MASTER_RX_FIFO:que->len;

		while(hal_i2c_get_rx_fifo_len(I2C_MASTER_SEL))
		{
			hal_i2c_read_data(I2C_MASTER_SEL);
		}

		for(uint8_t i=0;i<transLen;i++)
		{
			hal_i2c_master_read_command(I2C_MASTER_SEL);
			while(hal_i2c_get_tx_fifo_len(I2C_MASTER_SEL));
		}
	}

	if(stat & I2C_INTR_STAT_STOP_DET)
	{
		if(i2cMasterHandle.queTail == i2cMasterHandle.queTop)
		{
//			uart_printf("normal stop\n");
		}
		else
		{
			if(que->len)
			{
				if(que->dir == I2C_DIR_WRITE)
				{
					hal_i2c_write_data(I2C_MASTER_SEL, que->regAddr);

					transLen = (que->len > I2C_MASTER_TX_FIFO)?I2C_MASTER_TX_FIFO:que->len;

					hal_i2c_write_bulk(I2C_MASTER_SEL, que->p, transLen);
					que->len-=transLen;
					que->p+=transLen;
					que->regAddr+=transLen;
				}
				else
				{
					while(hal_i2c_get_rx_fifo_len(I2C_MASTER_SEL) && que->len)
					{
						*que->p++ = hal_i2c_read_data(I2C_MASTER_SEL);
						que->regAddr++;
						que->len--;
					}

					if(que->sendAddr)
					{
						que->sendAddr = 0;
						hal_i2c_read_data(I2C_MASTER_SEL);
					}
					else if(que->len)
					{
						i2c_master_start_que();
					}
					else
					{
						goto QUE_FINISH;
					}
				}
			}
			else
			{
QUE_FINISH:
				i2cMasterHandle.isQueueFull = FALSE;

				if(que->cb)
				{
					que->cb(que);
				}
				
				i2cMasterHandle.queTop++;
				if(i2cMasterHandle.queTop == I2C_MASTER_QUE_NUM)
					i2cMasterHandle.queTop = 0;

				if(i2cMasterHandle.queTop == i2cMasterHandle.queTail)
				{
					i2cMasterHandle.started = 0;
					uart_printf("que over\n");
				}
				else
				{
					i2c_master_start_que();
				}
			}
		}
	}

	RestoreVectors(I2C_MASTER_SEL_IRQ);
}

void i2c_master_read_bulk(i2cBase_t i2cx, uint8_t devAddr, uint8_t regAddr, uint8_t* buf, uint8_t len)
{
	uint8_t i = 0;
	delay(5);
	while(hal_i2c_get_rx_fifo_len(i2cx))
	{
		hal_i2c_read_data(i2cx);
	}

	uint32_t cnt = 0;

	while(len)
	{
		uint8_t rx = hal_i2c_get_rx_fifo_len(i2cx);

		if(rx < 7 && cnt++ >= 100)
		{
			cnt = 0;
			hal_i2c_master_read_command(i2cx);
		}
		if(rx)
		{
			buf[i++] = hal_i2c_read_data(i2cx);
			len--;
		}
	}
}

void i2c_master_write_bulk(i2cBase_t i2cx, uint8_t devAddr, uint8_t regAddr, uint8_t* buf, uint8_t len)
{
	uint8_t i = 0;
	hal_i2c_update_tar_addr(i2cx, devAddr);
	hal_i2c_write_data(i2cx, regAddr);
	while(len)
	{
		if((hal_i2c_get_status(i2cx)&I2C_STAT_TFNF))
		{
			hal_i2c_write_data(i2cx, buf[i]);
			i++;
			len--;
		}
	}
}

#endif	//I2C_MASTER_ENABLE

#if(I2C_SLAVE_ENABLE == 1)
static void i2c_slave_isr(void* arg)
{
	SaveVectors(I2C_SLAVE_SEL_IRQ);
	uint16_t stat = hal_i2c_get_intr_stat(I2C_SLAVE_SEL);
	hal_i2c_clr_intr(I2C_SLAVE_SEL);

	i2cSlaveRxQue_t* que = &(i2cSlaveHandle.que[i2cSlaveHandle.queTail]);

	if(stat & I2C_INTR_STAT_START_DET)
	{
		/*you can suspend high-resource-consumption app here, if necessary*/
	}
	if(stat & I2C_INTR_STAT_RD_REQ)
	{
		i2cSlaveHandle.isRd = 1;
		for(uint8_t i=0; i<I2C_SLAVE_TX_FIFO; i++)
		{
			hal_i2c_write_data(I2C_SLAVE_SEL, *(i2cSlaveHandle.memBuf + que->startRegAddr++));
		}
	}
	if(stat & I2C_INTR_STAT_RX_FULL)
	{
		if(i2cSlaveHandle.isAddr)
		{
			que->startRegAddr = hal_i2c_read_data(I2C_SLAVE_SEL);
			i2cSlaveHandle.isAddr = 0;
		}

		while(hal_i2c_get_rx_fifo_len(I2C_SLAVE_SEL))
		{
			if(que->startRegAddr + que->len < i2cSlaveHandle.memLen)
				*(i2cSlaveHandle.memBuf + que->startRegAddr + que->len++) = hal_i2c_read_data(I2C_SLAVE_SEL);
		}
	}
	if(stat & I2C_INTR_STAT_STOP_DET)
	{
		if(i2cSlaveHandle.isAddr)
		{
			que->startRegAddr = hal_i2c_read_data(I2C_SLAVE_SEL);
		}
		if(!i2cSlaveHandle.isRd)
		{
			while(hal_i2c_get_rx_fifo_len(I2C_SLAVE_SEL))
			{
				if(que->startRegAddr + que->len < i2cSlaveHandle.memLen)
					*(i2cSlaveHandle.memBuf + que->startRegAddr + que->len++) = hal_i2c_read_data(I2C_SLAVE_SEL);
			}
			if(que->len)
			{
				i2cSlaveHandle.queTail++;
				if(i2cSlaveHandle.queTail >= I2C_SLAVE_QUE_NUM)
					i2cSlaveHandle.queTail = 0;
				i2cSlaveHandle.que[i2cSlaveHandle.queTail].startRegAddr = 0;
				i2cSlaveHandle.que[i2cSlaveHandle.queTail].len = 0;
			}
		}

		i2cSlaveHandle.isRd = 0;
		i2cSlaveHandle.isAddr = 1;

		/*you can restart high-resource-consumption app here that you closed in start_intr*/
	}
	RestoreVectors(I2C_SLAVE_SEL_IRQ);
}
#endif //I2C_SLAVE_ENABLE

void i2c_polling(void)
{
#if(I2C_MASTER_ENABLE)
	if(i2cMasterHandle.started == 0)
	{
		if(i2cMasterHandle.queTop != i2cMasterHandle.queTail)
		{
			i2c_master_start_que();
		}
	}
#endif	//I2C_MASTER_ENABLE

#if(I2C_SLAVE_ENABLE)
	if(i2cSlaveHandle.memCb)
	{
		while(i2cSlaveHandle.queTop != i2cSlaveHandle.queTail)
		{
			i2cSlaveHandle.memCb(&(i2cSlaveHandle.que[i2cSlaveHandle.queTop]));
			i2cSlaveHandle.queTop++;
			if(i2cSlaveHandle.queTop >= I2C_SLAVE_QUE_NUM)
				i2cSlaveHandle.queTop = 0;
		}
	}
#endif	//I2C_SLAVE_ENABLE
}

#endif //I2C_ENABLE

