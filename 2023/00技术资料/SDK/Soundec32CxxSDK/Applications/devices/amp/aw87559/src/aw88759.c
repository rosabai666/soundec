/*
 * aw88759.c
 *
 *  Created on: 2021Äê7ÔÂ2ÈÕ
 *      Author: User
 */
#include "common.h"
#if SUPPORT_AMP_AW88759
//#include "amp_aw88298.h"
#include "amp_aw87559_reg.h"
#include "drv_i2c.h"
//#include "awinic_common.h"
#include "hal_gpio.h"
#include "hal_i2c.h"
#include "drv_i2c_sw.h"

#define HAL_I2C_Mem_Write      sw_i2c_txStr
#define HAL_I2C_Mem_Read       sw_i2c_rxStr
void usb_vol_ctrl(void);

typedef struct {
    uint8_t  addr;
    uint8_t data;
}aw88759_reg_cfg_t;

static const aw88759_reg_cfg_t aw88759_reg_cfg[] =
{
//		{0x62,0xB5},
//		{0x78,0x39},
//		{0x79,0xE5},
//		{0x77,0xC1},
//		{0x77,0xC1},
//		{0x78,0x7A},
//		{0x79,0x6C},
//		{0x77,0x81},
//		{0x66,0x69},
//		{0x58,0xBC},
//		{0x02,0x49},
//		{0x03,0x0A},//normal
//		{0x04,0x80},
//		{0x05,0x08},
//		{0x06,0x0C},
//		{0x07,0x83},
//		{0x08,0x4E},
//		{0x09,0x03},
//		{0x0A,0x08},
//		{0x0B,0x4A},
//		{0x0C,0x02},
//		{0x0D,0x77},
//		{0x0E,0x7A},
//		{0x0F,0x51},
//		{0x10,0x58},
//		{0x60,0x26},
//		{0x61,0x15},
//		{0x63,0x5A},
//		{0x64,0xD5},
//		{0x65,0x57},
//		{0x67,0x28},
//		{0x68,0x35},
//		{0x69,0x98},
//		{0x70,0x1C},
//		{0x71,0x9C},
//		{0x72,0x33},
//		{0x73,0x40},
//		{0x74,0x6C},
//		{0x01,0x78},


//		{0x62,0xB5},
//		{0x78,0x39},
//		{0x79,0xE5},
//		{0x77,0xC1},
//		{0x77,0xC1},
//		{0x78,0x7A},
//		{0x79,0x6C},
//		{0x77,0x81},
//		{0x66,0x69},
//		{0x58,0xBC},
//		{0x02,0x09},
//		{0x03,0x0A},//test
//		{0x04,0x80},
//		{0x05,0x08},
//		{0x06,0x07},
//		{0x07,0x93},
//		{0x08,0x4E},
//		{0x09,0x0B},
//		{0x0A,0x08},
//		{0x0B,0x4B},
//		{0x0C,0x00},
//		{0x0D,0x77},
//		{0x0E,0x7A},
//		{0x0F,0x51},
//		{0x10,0x40},
//		{0x60,0x26},
//		{0x61,0x15},
//		{0x63,0x5A},
//		{0x64,0xD5},
//		{0x65,0x57},
//		{0x67,0x28},
//		{0x68,0x22},
//		{0x69,0xA4},
//		{0x70,0x1C},
//		{0x71,0x9C},
//		{0x72,0xB3},
//		{0x73,0x44},
//		{0x74,0x6C},
//		{0x01,0x7C},


		{0x62,0xB5},
		{0x78,0x39},
		{0x79,0xE5},
		{0x77,0xC1},
		{0x77,0xC1},
		{0x78,0x7A},
		{0x79,0x6C},
		{0x77,0x81},
		{0x66,0x69},
		{0x58,0xBC},
		{0x02,0x09},
		{0x03,0x00},//test
		{0x04,0x80},
		{0x05,0x02},
//		{0x06,0x08},
		{0x06,0x0D},
		{0x07,0x93},
		{0x08,0x4E},
		{0x09,0x0B},
		{0x0A,0x08},
		{0x0B,0x4B},
		{0x0C,0x01},
		{0x0D,0x77},
		{0x0E,0x7A},
		{0x0F,0x51},
		{0x10,0x48},
		{0x60,0x26},
		{0x61,0x15},
		{0x63,0x5A},
		{0x64,0xD5},
		{0x65,0x57},
		{0x67,0x28},
		{0x68,0x22},
		{0x69,0xA4},
		{0x70,0x1C},
		{0x71,0x9C},
		{0x72,0xB3},
		{0x73,0x44},
		{0x74,0x6C},
		{0x01,0x78},

};

 static int i2c_read(uint8_t reg_addr, uint8_t *reg_data){

	 int res = 0;
	 uint8_t data = 0;
	 res = HAL_I2C_Mem_Read(AW88759_I2C_ADDR, reg_addr,reg_data, 1, 0);

	 return res;
 }

 static int i2c_write(uint8_t reg_addr, uint8_t reg_data){

	 int res = 0;
	 res = HAL_I2C_Mem_Write(AW88759_I2C_ADDR, reg_addr, &reg_data, 1, 0);
	 return res;
 }

 static int aw87559_i2c_read(uint8_t reg_addr, uint8_t *reg_data){

	 int ret = -1;
	 unsigned char cnt = 0;

	 while (cnt < 5) {
		 ret = i2c_read(reg_addr, reg_data);
		 if (ret == 0) {
			 uart_printf("i2c_read cnt = %d, error = %d\r\n", cnt, ret);
		 } else {
			 break;
		 }
		 cnt ++;
	 }
	 return ret;
 }

 int aw87559_i2c_write(uint8_t reg_addr, uint8_t reg_data){

	 int ret = -1;
	 unsigned char cnt = 0;

	 while (cnt < 5) {
		 ret = i2c_write(reg_addr, reg_data);
		 if (ret == 0) {
			 uart_printf("i2c_write cnt = %d, error = %d\r\n",cnt, ret);
		 } else {
			 break;
		 }
		 cnt ++;
	 }
	 return ret;
 }


 void aw87559_reg_show(void)
 {
	 uint8_t i;
	 uint8_t reg_val;

	 aw87559_i2c_read(0x01, &reg_val);
	 uart_printf("val=0x%02x \r\n", reg_val);

//	 for (i = 0; i < AW88759_REG_MAX; i++) {
//		 //if (aw88298_reg_access[i] & REG_RD_ACCESS)
//		 {
//			 aw87559_i2c_read(i, &reg_val);
//		     //if(i < 2)
//			 {
//				  uart_printf("addr=0x%02x, val=0x%02x \r\n", i, reg_val);
//		     }
//		 }
//	 }

 }

 /*
  * gain [0-27] step by 1.5db
  */
U8 aw87559_set_gain(float gain)
 {
	 U8 reg_val = gain / 1.5;
	 uart_printf("reg_val = %d.\r\n", reg_val);
	 if((reg_val >= 0) && (reg_val <= 7)){
		 aw87559_i2c_write(0x01,  0x7C);
		 aw87559_i2c_write(0x06, reg_val);
		 delay(1);
		 return 1;
	 }

	 if((reg_val > 7) && (reg_val <= 18)){
		 aw87559_i2c_write(0x01,  0x78);
		 aw87559_i2c_write(0x06, reg_val);
		 delay(1);
		 return 1;
	 }
	 return 0;

 }

 void aw87559_load_reg_cfg(void)
  {
 	 uint16_t i = 0 , data = 0;

 	 for (i = 0; i < sizeof(aw88759_reg_cfg) / sizeof(aw88759_reg_cfg[0]); i ++){
 		aw87559_i2c_write(aw88759_reg_cfg[i].addr, aw88759_reg_cfg[i].data);
 		delay(1);
 	 }

  }

void aw87559_init(void)
{
	hal_gpio_set_pinmux_function(9, GPIO_9_FUNC_GPIO);
	hal_gpio_set_direction(9, GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(9, GPIO_DATA_HIGH);
	delay(5);

	sw_i2c_init();

	aw87559_load_reg_cfg();

	aw87559_reg_show();
	usb_vol_ctrl();
}

int8_t usb_volume = -10;
extern void codec_set_hp_left_analog_gain(int8_t gain);
extern void codec_set_hp_right_analog_gain(int8_t gain);
void usb_vol_ctrl(void)
{
	if(usb_volume <= -54){

	}else if(usb_volume <= -32){
		codec_set_hp_left_analog_gain(-19);
		codec_set_hp_right_analog_gain(-19);
	}else if(usb_volume <= -23){
		codec_set_hp_left_analog_gain(-14);
		codec_set_hp_right_analog_gain(-14);
	}else if(usb_volume <= -17){
		codec_set_hp_left_analog_gain(-12);
		codec_set_hp_right_analog_gain(-12);
	}else if(usb_volume <= -13){
		codec_set_hp_left_analog_gain(-10);
		codec_set_hp_right_analog_gain(-10);
	}else if(usb_volume <= -10){
		codec_set_hp_left_analog_gain(-8);
		codec_set_hp_right_analog_gain(-8);
	}else if(usb_volume <= -7){
		codec_set_hp_left_analog_gain(-6);
		codec_set_hp_right_analog_gain(-6);
	}else if(usb_volume <= -5){
		codec_set_hp_right_analog_gain(-5);
		codec_set_hp_left_analog_gain(-5);
	}else if(usb_volume <= -3){
		codec_set_hp_left_analog_gain(-4);
		codec_set_hp_right_analog_gain(-4);
	}else if(usb_volume <= -1){
		codec_set_hp_left_analog_gain(-3);
		codec_set_hp_right_analog_gain(-3);
	}else{
		codec_set_hp_left_analog_gain(-2);
		codec_set_hp_right_analog_gain(-2);
	}
}
#endif
