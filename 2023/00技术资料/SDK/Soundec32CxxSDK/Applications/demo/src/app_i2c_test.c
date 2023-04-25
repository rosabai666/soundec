#include "app_i2c_test.h"
#include "common.h"
#include "drv_uart.h"

#if(I2C_ENABLE)
#if(I2C_MASTER_ENABLE)
uint8_t testBuf[16] = {0};
uint8_t cnt = 0;

void app_i2c_test_master_wr_cb(void *que);
void app_i2c_test_master_rd_cb(void *que);

void app_i2c_test_master_rd_cb(void *que)
{
	uint8_t i = 0;
	for(i=0;i<16;i++)
	{
		uart_printf("%x ", ((i2cMasterQue_t*)que)->buf[i]);
		testBuf[i] = testBuf[i]+0x10;
	}
	uart_printf("\n");

	cnt++;
	if(cnt<5)
	{
		i2c_master_add_write((0x21<<1), 0x00, testBuf, 16, app_i2c_test_master_wr_cb);
	}
	else
		cnt = 0;
}

void app_i2c_test_master_wr_cb(void *que)
{
	i2c_master_add_read((0x21<<1), 0x00, 16, app_i2c_test_master_rd_cb);
}

void app_i2c_test_master(void)
{
	for(uint8_t i=0;i<16;i++)
	{
		testBuf[i] = i;
	}

	i2c_master_add_write((0x21<<1), 0x00, testBuf, 16, app_i2c_test_master_wr_cb);
}
#endif

#if(I2C_SLAVE_ENABLE)
uint8_t testSlaveBuf[100] = {0};
void app_i2c_test_slave_cb(void *que)
{
	i2cSlaveRxQue_t* rxQue = que;
//	uart_printf("change len: %d\n", rxQue->len);
//	for(uint8_t i=0; i<rxQue->len; i++)
//	{
//		uart_printf("reg: %d, %d\n", rxQue->startRegAddr+i, testSlaveBuf[rxQue->startRegAddr+i]);
//	}

	uart_printf("reg:%d, %02x\n", rxQue->startRegAddr, testSlaveBuf[rxQue->startRegAddr]);
}

void app_i2c_test_slave(void)
{
	for(uint8_t i=0; i<100; i++)
		testSlaveBuf[i] = i;
	i2c_slave_register_mem(testSlaveBuf, 100, app_i2c_test_slave_cb);
}
#endif

#endif


