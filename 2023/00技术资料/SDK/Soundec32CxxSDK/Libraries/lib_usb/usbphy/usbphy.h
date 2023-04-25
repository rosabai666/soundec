/**
***********************************************************
 *
 * @file name	: usbphy.h
 * @author		: nick
 * @version		: Vxx.xx
 * @date		: 2020-07-09
 * @brief		:
 *
***********************************************************
 * @attention
 *
 * Copyright (c) 2020 Soundec Co., Ltd.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user
 * who uses the software, his/her employer (which must be your employer
 * and Soundec Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 *
***********************************************************/

#ifndef __USBPHY_H__
#define __USBPHY_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif
#if 1//(USB_ENABLE)
typedef struct
{
    __IO uint32_t COMPDISTUNE0;      //0x00	bit[2:0]	r/w
    __IO uint32_t PLLBTUNE;	         //0x04	bit[0]	r/w
    __IO uint32_t PLLITUNE;	         //0x08	bit[1:0]	r/w
    __IO uint32_t PLLPTUNE;	         //0x0C	bit[3:0]	r/w
    __IO uint32_t EQBYPASSENB0;	     //0x10	bit[0]	r/w
    __IO uint32_t SQRCTUNE0;	     //0x14	bit[2:0]	r/w
    __IO uint32_t VDATREFTUNE0;	     //0x18	bit[1:0]	r/w
    __IO uint32_t TXFSLSTUNE0;	     //0x1C	bit[3:0]	r/w
    __IO uint32_t TXPREEMPAMPTUNE0;	 //0x20	bit[1:0]	r/w
    __IO uint32_t TXPREEMPPULSETUNE0;//0x24	bit[0]	r/w
    __IO uint32_t TXRISETUNE0;	     //0x28	bit[1:0]	r/w
    __IO uint32_t TXVREFTUNE0;	     //0x2C	bit[3:0]	r/w
    __IO uint32_t TXHSXVTUNE0;	     //0x30	bit[1:0]	r/w
    __IO uint32_t TXRESTUNE0;	     //0x34	bit[1:0]	r/w
    __IO uint32_t EFUSESEL0;	     //0x38	bit[0]	r/w
    __IO uint32_t EFUSEHSPB0;	     //0x3C	bit[3:0]	r/w
    __IO uint32_t SSSCALEDOWN_MODE;	 //0x40	bit[1:0]	r/w ss_scaledown_mode
    __IO uint32_t phy_por;	         //0x44	bit[0]	r/w
    __IO uint32_t utmi_clk_en;	     //0x48	bit[0]	r/w
    __IO uint32_t prst_n;	         //0x4C	bit[0]	r/w
    __IO uint32_t hreset_n_reg;	     //0x50	bit[0]	r/w
    __IO uint32_t VATESTENB;	     //0x54	bit[1:0]	r/w
    __IO uint32_t SIDDQ;	         //0x58	bit[0]	r/w
    __IO uint32_t LOOPBCKENB;	     //0x5C	bit[0]	r/w
    __IO uint32_t TESTCLK;	         //0x60	bit[0]	r/w
    __IO uint32_t TESTDATAOUTSEL;	 //0x64	bit[0]	r/w
    __IO uint32_t TESTDATAIN;	     //0x68	bit[7:0]	r/w
    __IO uint32_t TESTADDR;	         //0x6C	bit[3:0]	r/w
    __IO uint32_t TESTBURNIN;	     //0x70	bit[0]	r/w
    uint32_t REVERSED;          //0x74	bit[0]	r/w
    __IO uint32_t PROTRESET;	     //0x78	bit[0]	r/w
    __IO uint32_t RESERVED2;
    __IO uint32_t ALTERSET;		   /*offset : 0x80*/
    __IO uint32_t EXTREFCLK; 	   /*offset : 0x84*/
    __IO uint32_t FSEL;			   /*offset : 0x88*/
    __IO uint32_t REFCLKSEL; 	   /*offset : 0x8C*/
    __IO uint32_t COMMONONN; 	   /*offset : 0x90*/
    __IO uint32_t RESREQIN;		   /*offset : 0x94*/
    __IO uint32_t VBUSSVLDEXTSEL0;  /*offset : 0x98*/
    __IO uint32_t OTGDISABLE0;	   /*offset : 0x9C*/
    __IO uint32_t DRVVBUS0;		   /*offset : 0xA0*/
    __IO uint32_t VBUSVLDDEXT0;	   /*offset : 0xA4*/
    __IO uint32_t DPPULLDOWN0;	   /*offset : 0xA8*/
    __IO uint32_t DMPULLDOWN0;	   /*offset : 0xAC*/
    __IO uint32_t SLEEPM0;		   /*offset : 0xB0*/
    __IO uint32_t TXBITSTUFFEN0;    /*offset : 0xB4*/
    __IO uint32_t TXBITSTUFFNH0;    /*offset : 0xB8*/
    __IO uint32_t BYPASSDPDATA0;    /*offset : 0xBC*/
    __IO uint32_t BYPASSDMDATA0;    /*offset : 0xC0*/
    __IO uint32_t BYPASSDPEN0;	   /*offset : 0xC4*/
    __IO uint32_t BYPASSDMEN0;	   /*offset : 0xC8*/
    __IO uint32_t BYPASSSEL0;	   /*offset : 0xCC*/
    __IO uint32_t HSXCVREXTCTL0;    /*offset : 0xD0*/
    __IO uint32_t FSFDATAEXT0;	   /*offset : 0xD4*/
    __IO uint32_t FSSE0EXT0; 	   /*offset : 0xD8*/
    __IO uint32_t TXENABLE0; 	   /*offset : 0xDC*/
    __IO uint32_t RESERVED3;
    __IO uint32_t CHRGSEL0;		   /*offset : 0xE4*/
    __IO uint32_t VDATDETENB0;	   /*offset : 0xE8*/
    __IO uint32_t VDATSRCENB0;	   /*offset : 0xEC*/
    __IO uint32_t DCDENB0;		   /*offset : 0xF0*/
    __IO uint32_t UTMI_CLK_SEL;	   /*offset : 0xF4*/
    __IO uint32_t USB_FPGA_CLKGT;  /*USB_PHY_INT_RST_ENABLE */ /*offset : 0xF8*/
    __IO uint32_t REM_REGISTERS;   /*offset : 0xFC*/
} hal_usb_hw_t;

#define	USBPHY_BASE	    0x7700E000
#define USBDPHY        ((hal_usb_hw_t *)USBPHY_BASE)

void set_usb_internal_resistor_calibrarting(uint8_t efusesel);
void usbphy_reset(void);
void usbphy_enable(void);
void simulation_usb_entering_suspend(void);
uint32_t usbphy_getSuspend_status(uint32_t clr);
uint32_t usbphy_setSuspend_status(uint32_t val);
void usbphy_set_system_crystal(uint8_t sysCrystal);
uint8_t usbphy_get_system_crystal(void);	
void usbphy_set_sys_clk(uint32_t sysClk);
uint32_t usbphy_get_sys_clk(void);
void usbphy_set_peri_clock(uint32_t clock);
uint32_t usbphy_get_peri_clock(void);
void usb_delay(uint32_t ms);
void usb_delay_us(uint32_t us);

#endif
#ifdef __cplusplus
}
#endif

#endif  /* __USBPHY_H__ */



