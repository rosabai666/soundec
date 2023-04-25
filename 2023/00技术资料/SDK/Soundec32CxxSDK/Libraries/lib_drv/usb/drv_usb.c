/**
***********************************************************
 *
 * @file name	: drv_usb.c
 * @author	: nick
 * @version	: Vxx.xx
 * @date	: 2020-02-27
 * @brief	: usb moude
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


#include "types.h"
#include "usb_dev.h"
#include "usbphy.h"
#include "usb_hal.h"
#include "usb_desc.h"
#include "uac.h"
#include "uac_if.h"
#include <xtensa/config/core.h>
#include <xtensa/xtruntime.h>
#include "snc8600.h"
#include "hal_interrupt.h"
#include "sys_init.h"

#include "app_adc_key.h"
//#include "drv_pmu.h"
#include "usb_composite.h"
#ifdef HANDLE_AO_ADC_KEY_WAKEUP_FROM_USB_DEV_SUSPEND
#include "usb_hid.h"
#endif
#if(AUDIO_MANAGER)
#include "audio_session.h"
#include "audio_manager.h"
#endif
#include "hal_sysctrl.h"
#include "drv_pmu.h"
#include "usb_hid.h"
#include "drv_usb.h"

#if (USB_ENABLE)
usb_device_t gUsbDevice = {0};

//extern pmu_wakeup_src_t pmu_get_wakeup_source(void);

#if(TEST_USB_SWITCH)
uac_spec_t is_usb_dev_UAC20 = UAC_10; // 1;
uint8_t Usbd_Speed_Cfg = USB_SPEED_FULL;//USB_SPEED_CFG;
#else
#if(SUPPORT_UAC_SELF_ADAPTION)
uac_spec_t is_usb_dev_UAC20 = UAC_20;
#else
uac_spec_t is_usb_dev_UAC20 = SUPPORT_UAC20;
#endif
uint8_t Usbd_Speed_Cfg = USB_SPEED_CFG;
#endif
const uint8_t usb_interface_num = USB_FUN_NUM_INTERFACES;
const uint16_t ep_rxfifo = USB_DEV_ENDPOINT_RXFIFO;
const bool spkEn = SUPPORT_USB_SPK;
const bool micEn = SUPPORT_USB_MIC;
const bool lpmEn = SUPPORT_USB_LPM;

void usbd_resume_from_ao_wakeup(void)
{
#if SUPPORT_USB_LPM
    if( usbphy_getSuspend_status(1) == 0x3)
    {
        if(pmu_get_wakeup_source() == 0)
        {
            delay(10);
#ifdef HANDLE_AO_ADC_KEY_WAKEUP_FROM_USB_DEV_SUSPEND
            set_always_on_adc_key_status(1);
#endif
        }
    }
#endif
}

int altochip_usb_init(uint8_t speed, uac_spec_t uacType)
{
    //usbd_cfg_init();
    uart_printf("UAC %s init\n", (uacType==UAC_10)?"10":"20");
	uint32_t clkGate = hal_sysctrl_get_clock_gate();
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_USBPHY);
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_USB);
	SETBIT(clkGate, HAL_SYSCTRL_CLKGT_PUSB);
	hal_sysctrl_set_clock_gate(clkGate);

	is_usb_dev_UAC20 = uacType;
	Usbd_Speed_Cfg   = speed;

	usbphy_set_system_crystal(hal_get_system_crystal());
	usbphy_set_sys_clk(SYS_CLK);
	usbphy_set_peri_clock(PERI_CLK_HZ(PERI_CLOCK_AHB));
    usbphy_enable();
    delay(1);

    hal_interrupt_disable_irq(USB_IRQn);
    hal_interrupt_register_isr_handler(USB_IRQn, usb_isr_handler);

	gUsbDevice.ep0TxFifoSize = USB_DEV_ENDPOINT0_TXFIFO;
	gUsbDevice.ep1TxFifoSize = USB_DEV_ENDPOINT1_TXFIFO;
	gUsbDevice.ep2TxFifoSize = USB_DEV_ENDPOINT2_TXFIFO;
	gUsbDevice.ep3TxFifoSize = USB_DEV_ENDPOINT3_TXFIFO;
#if (SUPPORT_USB_MIC)
	gUsbDevice.isUsbMicEn	 = TRUE;
#else
	gUsbDevice.isUsbMicEn	 = FALSE;
#endif
#if (SUPPORT_USB_SPK)
	gUsbDevice.isUsbSpkEn	 = TRUE;
#else
	gUsbDevice.isUsbSpkEn	 = FALSE;
#endif
 
    usb_dev_init(&gUsbDevice, &gUsbDescFun);

    usbd_resume_from_ao_wakeup();
    /* Add Supported Class */
    usb_dev_register_class(&gUsbDevice, usbDevComposite);

    /* Add Interface callbacks for AUDIO Class */
    uac_register_interface(&gUsbDevice, &uac_fops);

    /* Start Device Process */
    usb_dev_start(&gUsbDevice);

    hal_interrupt_enable_irq(USB_IRQn);
	
	hal_usb_set_speed_init_count_zero();
	
    return 0;
}

int altochip_usb_deinit(void)
{
    usb_dev_stop(&gUsbDevice);
    hal_interrupt_disable_irq(USB_IRQn);
	hal_usb_set_speed_init_count_zero();
    return 0;
}


#if(AUDIO_MANAGER)
usbDevInfos_t usbDevInfo[2];	
auSlotSize_t usb_transform_bitslot(auSlotSize_t bitslot)
{
	return bitslot;
}

drv_status_t usb_init(auDriverConfig_t *spkCfg, auDriverConfig_t *micCfg)
{
	if (spkCfg)
	{
//		usbDevInfo[USBSPKER].role						= spkCfg->attr->role;
		usbDevInfo[USBSPKER].enable						= spkCfg->attr->enable;

#if(SUPPORT_USB_SPK)
		usbDevInfo[USBSPKER].format.bitSlot				= SPK_DEFAULT_BIT_SLOT;
		usbDevInfo[USBSPKER].format.sampleRateHz		= SPK_DEFAULT_FREQ;
		usbDevInfo[USBSPKER].format.channlesPerFrame	= USB_SPK_CHANNELS;
#else
		usbDevInfo[USBSPKER].format.bitSlot				= spkCfg->format->bitSlot;
		usbDevInfo[USBSPKER].format.sampleRateHz		= spkCfg->format->sampleRateHz;
		usbDevInfo[USBSPKER].format.channlesPerFrame	= spkCfg->format->channlesPerFrame;
#endif
		usbDevInfo[USBSPKER].isrCb						= spkCfg->attr->isrCb;
		usbDevInfo[USBSPKER].updateInfoCb				= spkCfg->attr->updateInfoCb;
		usbDevInfo[USBSPKER].streamHandler				= spkCfg->attr->pStream;
	}
	if (micCfg)
	{
//		usbDevInfo[USBMIC].role							= micCfg->attr->role;
		usbDevInfo[USBMIC].enable						= micCfg->attr->enable;
#if(SUPPORT_USB_MIC)
		usbDevInfo[USBMIC].format.bitSlot				= MIC_DEFAULT_BIT_SLOT;
		usbDevInfo[USBMIC].format.sampleRateHz			= MIC_DEFAULT_FREQ;
		usbDevInfo[USBMIC].format.channlesPerFrame		= USB_MIC_CHANNELS;
#else
		usbDevInfo[USBMIC].format.bitSlot				= micCfg->format->bitSlot;
		usbDevInfo[USBMIC].format.sampleRateHz			= micCfg->format->sampleRateHz;
		usbDevInfo[USBMIC].format.channlesPerFrame		= micCfg->format->channlesPerFrame;
#endif
		usbDevInfo[USBMIC].isrCb						= micCfg->attr->isrCb;
		usbDevInfo[USBMIC].updateInfoCb					= micCfg->attr->updateInfoCb;
		usbDevInfo[USBMIC].streamHandler				= micCfg->attr->pStream;
	}
		
	uart_printf("Usbd_Speed_Cfg:%d\n", Usbd_Speed_Cfg);
	altochip_usb_init(Usbd_Speed_Cfg, is_usb_dev_UAC20);

	usbDevInfo[USBSPKER].initialized			= TRUE;
	usbDevInfo[USBMIC].initialized				= TRUE;

	return 0;
}

drv_status_t usb_deinit(auDriverConfig_t *spkCfg, auDriverConfig_t *micCfg)
{
	altochip_usb_deinit();
	return 0;
}

drv_status_t usb_reinit(auDriverConfig_t *spkCfg, auDriverConfig_t *micCfg)
{
	altochip_usb_deinit();
	usb_dev_deinit(&gUsbDevice);
	delay(100);
	usbphy_reset();
	delay(500);
	usb_init(spkCfg, micCfg);
	return 0;
}

#if(TEST_USB_SWITCH || SUPPORT_UAC_SELF_ADAPTION)
bool usbSwitchFlag = 0;
bool usbRestartFlag = 0;
#endif
#if(SUPPORT_USB_INSERT_DETECTION)
extern bool insFlag;
extern uint32_t insCnt;

#if(SUPPORT_UAC20)
extern bool setCurFlag;
#endif
#if(SUPPORT_UAC_SELF_ADAPTION)
extern bool enumFlag;
#endif
#endif

void usb_insert_detection(void)
{
#if(TEST_USB_SWITCH || SUPPORT_UAC_SELF_ADAPTION)
	if(usbRestartFlag)
	{
		usbRestartFlag = 0;
		altochip_usb_init(Usbd_Speed_Cfg, is_usb_dev_UAC20);
	}
	if(usbSwitchFlag)
	{
		usbSwitchFlag = 0;
		delay(100);
		usbphy_reset();
		usbRestartFlag = 1;
	}
#endif

#if(SUPPORT_USB_INSERT_DETECTION)
	static uint8_t checkCnt = 0;
	if(insFlag)
	{
		if(insCnt == 0 && checkCnt++ > UNPLUG_DEVICE_THRES)
		{
			insFlag = 0;
			checkCnt = 0;
#if(SUPPORT_UAC20)
			setCurFlag = 0;
#endif
#if(SUPPORT_UAC_SELF_ADAPTION)
			enumFlag = 0;
#endif
			uart_printf("plug out\n");
		}
		insCnt = 0;
	}
#if(SUPPORT_UAC_SELF_ADAPTION)
	if(!setCurFlag && is_usb_dev_UAC20 == UAC_20 && insFlag && checkCnt++ > UAC_SELF_ADAPTION_THRES)
	{
		altochip_usb_deinit();
		uart_printf("restart\n");
		checkCnt = 0;
		is_usb_dev_UAC20 = UAC_10;
		Usbd_Speed_Cfg = USB_SPEED_FULL;
		usbphy_reset();
		usbRestartFlag = 1;
		insFlag = 0;
		enumFlag = 0;
	}
#endif
#endif
}

#endif

#define USB_SPEED_INIT_LIMIT 15 // range 10-50
void usb_speed_init_error_detect(void)
{
  if(hal_usb_get_speed_init_count() >= USB_SPEED_INIT_LIMIT)
    {
        uart_printf("usb deinit\n");
        altochip_usb_deinit();
        delay(1000);
        system_reset();
        uart_printf("sys reset\n");
    }
}

#else
uac_spec_t is_usb_dev_UAC20 = 0;
uint8_t Usbd_Speed_Cfg = 0;
const uint8_t usb_interface_num = 0;
const uint16_t ep_rxfifo = 0;
const bool spkEn = 0;
const bool micEn = 0;
const bool lpmEn = 0;
#endif






