/**
***********************************************************
 *
 * @file name	: usb_def.h
 * @author		: nick
 * @version		: Vxx.xx
 * @date		: 2020-09-22
 * @brief		: usb definition
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

#ifndef __USB_DEV_DEF_H
#define __USB_DEV_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "types.h"
#include "boardConfig.h"

#if 1//(USB_ENABLE)

#ifndef NULL
#define NULL   0U
#endif /* NULL */
#define __IO volatile


/* ---------- Standard USB Descriptor Types -----------------------*/
#define	DESC_TYPE_DEV								(0x01)
#define DESC_TYPE_CONFIG                    		(0x02)
#define DESC_TYPE_STRING                           	(0x03)
#define DESC_TYPE_INTF                        		(0x04)
#define DESC_TYPE_EP                        		(0x05)
#define DESC_TYPE_DEV_Q                 			(0x06)
#define DESC_TYPE_OS_CONFIG        					(0x07)
#define DESC_TYPE_INTF_ASSOC            			(0x0B)
#define DESC_TYPE_BOS                              	(0x0F)

#define DESC_SIZE_CONFIG							(0x09)
#define DESC_SIZE_INTF								(0x09)
#define DESC_SIZE_EP                         		(0x07)
#define DESC_SIZE_INTF_ASSOC            			(0x08)

/* ---------- Standard USB Descriptors -----------------------*/
#define DESC_CONFIG(wTotalLen, NumIntfs, ConfigVal, iConfig, bmAttrib, MaxPower)				(uint8_t)DESC_SIZE_CONFIG,				\
																								(uint8_t)DESC_TYPE_CONFIG,				\
																								(uint8_t)(wTotalLen & 0xFF),			\
																								(uint8_t)((wTotalLen>>8) & 0xFF),		\
																								(uint8_t)(NumIntfs),					\
																								(uint8_t)(ConfigVal),					\
																								(uint8_t)(iConfig),						\
																								(uint8_t)(bmAttrib),					\
																								(uint8_t)(MaxPower)

#define DESC_INTF(IntfNum, AltSetting, NumEps, IntfClass, IntfSubClass, IntfProtocol, iIntf)	(uint8_t)DESC_SIZE_INTF,				\
																								(uint8_t)DESC_TYPE_INTF,				\
																								(uint8_t)(IntfNum),						\
																								(uint8_t)(AltSetting),					\
																								(uint8_t)(NumEps),						\
																								(uint8_t)(IntfClass),					\
																								(uint8_t)(IntfSubClass),				\
																								(uint8_t)(IntfProtocol),				\
																								(uint8_t)(iIntf)

#define DESC_EP(EpAddr, bmAttrib, wMaxPktSize, Interval)										(uint8_t)DESC_SIZE_EP,					\
																								(uint8_t)DESC_TYPE_EP,					\
																								(uint8_t)(EpAddr),						\
																								(uint8_t)(bmAttrib),					\
																								(uint8_t)(wMaxPktSize & 0xFF),			\
																								(uint8_t)((wMaxPktSize>>8) & 0xFF),		\
																								(uint8_t)(Interval)

/* ---------- Standard Configuration bmAttributes --------------*/
#define BUS_POWERED									(0x80)
#define SELF_POWERED								(0x40)
#define REMOTE_WAKEUP								(0x20)

/* ---------- Standard Endpoint bmAttributes -------------------*/
//Transfer Type
#define EP_TYPE_CTRL								(0x00)
#define EP_TYPE_ISOC								(0x01)
#define EP_TYPE_BULK								(0x02)
#define EP_TYPE_INTR								(0x03)

//Synchronisation Type(ISO Mode)
#define NO_SYNCHRONISATION							(0x00)
#define ASYNCHRONOUS								(0x04)
#define ADAPTIVE									(0x08)
#define SYNCHRONOUS									(0x0C)

//Usage Type(ISO Mode)
#define DATA_EP										(0x00)
#define FB_EP										(0x10)
#define EXPLICIT_FB_EP								(0x20)



/* ---------- Device/Interface Class Codes -------------------*/
#define AUDIO_CLASS									(0x01)
#define CDC_CLASS									(0x02)
#define HID_CLASS									(0x03)
#define PRINTER_CLASS								(0x07)
#define MASS_STORAGE_CLASS							(0x08)

/* Audio Class-Specific Desc Type and Desc Size */
#define DESC_TYPE_AUDIO_INTF						(0x24)
#define DESC_TYPE_AUDIO_EP							(0x25)
/* Audio Class-Specific Audio-Control Intf Desc Subtypes & Desc Sizes*/
#define AC_INTF_SUBTYPE_H							(0x01)	//Header
#define AC_INTF_SUBTYPE_IT							(0x02)	//Input Terminal
#define AC_INTF_SUBTYPE_OT							(0x03)	//Output Terminal
#define AC_INTF_SUBTYPE_MU							(0x04)	//Mixer Unit
#define AC_INTF_SUBTYPE_SU							(0x05)	//Selector Unit
#define AC_INTF_SUBTYPE_FU							(0x06)	//Feature Unit
#define AC_INTF_SUBTYPE_EFU							(0x07)	//Effect Unit
#define AC_INTF_SUBTYPE_PU							(0x08)	//Processing Unit
#define AC_INTF_SUBTYPE_EXU							(0x09)	//Extension Unit
#define AC_INTF_SUBTYPE_CLK_SRC						(0x0A)	//Clock Source
#define AC_INTF_SUBTYPE_CLK_SEL						(0x0B)	//Clock Selector
#define AC_INTF_SUBTYPE_CLK_MULT					(0x0C)	//Clock Multiplier
#define AC_INTF_SUBTYPE_SR_CONV						(0x0D)	//Sample Rate Converter

#define DESC_SIZE_AC_INTF_H_10						(0x08)
#define DESC_SIZE_AC_INTF_H_20						(0x09)
#define DESC_SIZE_AC_INTF_IT_10						(0x0C)
#define DESC_SIZE_AC_INTF_IT_20						(0x11)
#define DESC_SIZE_AC_INTF_OT_10						(0x09)
#define DESC_SIZE_AC_INTF_OT_20						(0x0C)
#define DESC_SIZE_AC_INTF_FU_10						(0x08)
#define DESC_SIZE_AC_INTF_FU_20						(0x06)
#define DESC_SIZE_AC_INTF_SU						(0x07)

#define DESC_SIZE_AC_INTF_CLK_SRC					(0x08)
#define DESC_SIZE_AC_INTF_CLK_SEL					(0x07)

/* ---------- Audio Class Interface Descriptors -------------------*/
//Class-Specific AudioControl Interface Header Descriptor
#define DESC_AC_INTF_H_10(AudioNum, wTotalLen)													(uint8_t)(DESC_SIZE_AC_INTF_H_10 + AudioNum),	\
																								(uint8_t)DESC_TYPE_AUDIO_INTF,	\
																								(uint8_t)AC_INTF_SUBTYPE_H,			\
																								(uint8_t)0x00,		\
																								(uint8_t)0x01,					\
																								(uint8_t)(wTotalLen & 0xFF),					\
																								(uint8_t)((wTotalLen>>8) & 0xFF)

#define DESC_AC_INTF_H_10_INCOLLECTION_1														(uint8_t)0x01, (uint8_t)0x01
#define DESC_AC_INTF_H_10_INCOLLECTION_2														(uint8_t)0x02, (uint8_t)0x01, (uint8_t)0x02

#define DESC_AC_INTF_IT(TermID, wTermType, AssocTerm, NumChx, wChxConfig)						(uint8_t)DESC_SIZE_AC_INTF_IT_10,	\
																								(uint8_t)DESC_TYPE_AUDIO_INTF,	\
																								(uint8_t)AC_INTF_SUBTYPE_IT,			\
																								(uint8_t)(TermID),		\
																								(uint8_t)(wTermType & 0xFF),					\
																								(uint8_t)((wTermType>>8) & 0xFF), \
																								(uint8_t)(AssocTerm),					\
																								(uint8_t)(NumChx),	\
																								(uint8_t)(wChxConfig & 0xFF),					\
																								(uint8_t)((wChxConfig>>8) & 0xFF), \
																								(uint8_t)0x00, (uint8_t)0x00

#define DESC_AC_INTF_OT(TermID, wTermType, AssocTerm, SrcID)									(uint8_t)DESC_SIZE_AC_INTF_OT_10,	\
																								(uint8_t)DESC_TYPE_AUDIO_INTF,	\
																								(uint8_t)AC_INTF_SUBTYPE_OT,			\
																								(uint8_t)(TermID),		\
																								(uint8_t)(wTermType & 0xFF),					\
																								(uint8_t)((wTermType>>8) & 0xFF), \
																								(uint8_t)(AssocTerm),					\
																								(uint8_t)(SrcID),	\
																								(uint8_t)0x00

#define DESC_AC_INTF_FU(NumChx, UnitID, SrcID, CtrlMaster, CtrlChx)								(uint8_t)(DESC_SIZE_AC_INTF_FU_10 + NumChx),	\
																								(uint8_t)DESC_TYPE_AUDIO_INTF,	\
																								(uint8_t)AC_INTF_SUBTYPE_FU,			\
																								(uint8_t)(UnitID),		\
																								(uint8_t)(SrcID),					\
																								(uint8_t)0x01, \
																								(uint8_t)(CtrlMaster),					\
																								(uint8_t)(CtrlChx), \
																								(uint8_t)(CtrlChx), \
																								(uint8_t)0x00


/* Audio Class-Specific Audio-Streaming Intf Desc Subtypes & Desc Sizes*/
#define AS_INTF_SUBTYPE_GENERAL							(0x01)
#define AS_INTF_SUBTYPE_FORMAT_TYPE						(0x02)
#define AS_INTF_SUBTYPE_ENCODER							(0x03)
#define AS_INTF_SUBTYPE_DECODER							(0x04)

#define DESC_SIZE_AS_INTF_GEN_10						(0x07)
#define DESC_SIZE_AS_INTF_GEN_20						(0x10)
#define DESC_SIZE_AS_INTF_FMT_10						(0x08)
#define DESC_SIZE_AS_INTF_FMT_20						(0x06)

#define DESC_SIZE_AS_EP_STD								(0x09)
#define DESC_SIZE_AS_EP_CS								(0x07)

#define DESC_AS_INTF_GEN(TermID, Delay)															(uint8_t)DESC_SIZE_AS_INTF_GEN_10,	\
																								(uint8_t)DESC_TYPE_AUDIO_INTF,	\
																								(uint8_t)AS_INTF_SUBTYPE_GENERAL,			\
																								(uint8_t)(TermID),		\
																								(uint8_t)(Delay),					\
																								(uint8_t)0x01,					\
																								(uint8_t)0x00

#define DESC_AS_INTF_FMT(NumFs, NumChx, FrSize, BitRes)											(uint8_t)(DESC_SIZE_AS_INTF_FMT_10 + NumFs*3),	\
																								(uint8_t)DESC_TYPE_AUDIO_INTF,	\
																								(uint8_t)AS_INTF_SUBTYPE_FORMAT_TYPE,			\
																								(uint8_t)0x01,		\
																								(uint8_t)(NumChx),					\
																								(uint8_t)(FrSize),					\
																								(uint8_t)(BitRes),		\
																								(uint8_t)(NumFs)

#define DESC_AS_EP_STD(EpAddr, bmAttrib, wMaxPktSize, Interval, Refresh, SyncAddr)				(uint8_t)DESC_SIZE_AS_EP_STD,	\
																								(uint8_t)DESC_TYPE_EP,	\
																								(uint8_t)(EpAddr),			\
																								(uint8_t)(bmAttrib),		\
																								(uint8_t)(wMaxPktSize & 0xFF),			\
																								(uint8_t)((wMaxPktSize>>8) & 0xFF),		\
																								(uint8_t)(Interval),	\
																								(uint8_t)(Refresh),		\
																								(uint8_t)(SyncAddr)

#define DESC_AS_EP_CS(bmAttrib, LockDelayUnit, wLockDelay)										(uint8_t)DESC_SIZE_AS_EP_CS,	\
																								(uint8_t)DESC_TYPE_AUDIO_EP,	\
																								(uint8_t)0x01,			\
																								(uint8_t)(bmAttrib),		\
																								(uint8_t)(LockDelayUnit),		\
																								(uint8_t)(wLockDelay & 0xFF),			\
																								(uint8_t)((wLockDelay>>8) & 0xFF)

/* HID */
#define DESC_TYPE_HID									(0x21)
#define DESC_TYPE_HID_REPORT							(0x22)
#define DESC_TYPE_HID_PHYSICAL							(0x23)
#define DESC_SIZE_HID									(0x09)

#define DESC_HID(CountryCode, NumDescs)															(uint8_t)DESC_SIZE_HID,	\
																								(uint8_t)DESC_TYPE_HID,	\
																								(uint8_t)0x11,			\
																								(uint8_t)0x01,		\
																								(uint8_t)(CountryCode),		\
																								(uint8_t)(NumDescs)

#define AUDIO_16BIT_FRAMESIZE				0x02
#define AUDIO_16BIT_BIT_RESOLUTION			0x10
#define AUDIO_24BIT_FRAMESIZE				0x03
#define AUDIO_24BIT_BIT_RESOLUTION			0x18

#define USB_FUN_MAX_NUM_INTERFACES			4

#define USB_MAX_DEV_NUM_CONFIGURATION         1
#define USB_MAX_DEV_NUM_ENDPOINTS             8U
#define USB_DEV_LPM_ENABLED                   1U
#define USB_DEV_SELF_POWERED                  0U
#define USB_DEV_SUPPORT_USER_STRING           1U

#define USB_DEV_MAX_STR_DESC_SIZ            0x100

#define USB_LEN_DEV_QUALIFIER_DESC          0x0AU
#define USB_LEN_DEV_DESC                    0x12U
#define USB_LEN_CFG_DESC                    0x09U
#define USB_LEN_IF_DESC                     0x09U
#define USB_LEN_EP_DESC                     0x07U
#define USB_LEN_OTG_DESC                    0x03U
#define USB_LEN_LANGID_STR_DESC             0x04U
#define USB_LEN_OTHER_SPEED_DESC_SIZ        0x09U

#define USB_DEV_IDX_LANGID_STR              0x00U
#define USB_DEV_IDX_MFC_STR                 0x01U
#define USB_DEV_IDX_PRODUCT_STR             0x02U
#define USB_DEV_IDX_SERIAL_STR              0x03U
#define USB_DEV_IDX_CONFIG_STR              0x04U
#define USB_DEV_IDX_INTERFACE_STR           0x05U
#define USB_DEV_IDX_OTHER_STR               0x06U
#define USB_DEV_IDX_HEADPHONE               0x11U
#define USB_DEV_IDX_MICROPHONE              0x12U
#define USB_DEV_IDX_NULL_CONGFIG            0x23U   //SongGuang Sondecceheck tool.

#define  USB_REQ_TYPE_STANDARD                          0x00U
#define  USB_REQ_TYPE_CLASS                             0x20U
#define  USB_REQ_TYPE_VENDOR                            0x40U
#define  USB_REQ_TYPE_MASK                              0x60U

#define  USB_REQ_RECIPIENT_DEVICE                       0x00U
#define  USB_REQ_RECIPIENT_INTERFACE                    0x01U
#define  USB_REQ_RECIPIENT_ENDPOINT                     0x02U
#define  USB_REQ_RECIPIENT_MASK                         0x03U

#define  USB_REQ_GET_STATUS                             0x00U
#define  USB_REQ_CLEAR_FEATURE                          0x01U
#define  USB_REQ_SET_FEATURE                            0x03U
#define  USB_REQ_SET_ADDRESS                            0x05U
#define  USB_REQ_GET_DESCRIPTOR                         0x06U
#define  USB_REQ_SET_DESCRIPTOR                         0x07U
#define  USB_REQ_GET_CONFIGURATION                      0x08U
#define  USB_REQ_SET_CONFIGURATION                      0x09U
#define  USB_REQ_GET_INTERFACE                          0x0AU
#define  USB_REQ_SET_INTERFACE                          0x0BU
#define  USB_REQ_SYNCH_FRAME                            0x0CU

/*
 * Audio Device Class Codes
 */

/* Audio Intf Class Code */


/* Audio Intf Subclass Codes */
#define AUDIO_SUBCLASS_CONTROL				0x01
#define AUDIO_SUBCLASS_STREAMING			0x02

/* Audio Interface Protocol Codes */
#define AUDIO_PROTOCOL_VERSION_02_00				0x20

/*
 * Audio Terminal Types
 */

/* USB Terminal Types */
#define USB_TERMINAL_USB_STREAMING		0x0101

/* Input Terminal Types */
#define INPUT_TERMINAL_MICROPHONE		0x0201
#define INPUT_TERMINAL_DESKTOP_MIC		0x0202
#define INPUT_TERMINAL_PERSONAL_MIC		0x0203
#define INPUT_TERMINAL_OD_MIC			0x0204
#define INPUT_TERMINAL_MIC_ARRAY		0x0205
#define INPUT_TERMINAL_PROC_MIC_ARRAY	0x0206

/* Output Terminal Types */
#define OUTPUT_TERMINAL_SPEAKER						0x0301
#define OUTPUT_TERMINAL_HEADPHONE					0x0302
#define OUTPUT_TERMINAL_HEAD_MOUNTED_AUDIO			0x0303
#define OUTPUT_TERMINAL_DESKTOP_SPK					0x0304
#define OUTPUT_TERMINAL_ROOM_SPK					0x0305
#define OUTPUT_TERMINAL_COMM_SPK					0x0306
#define OUTPUT_TERMINAL_LOW_FREQ_SPK				0x0307


/* Audio Channel Config Sel Bits */
#define AUDIO_CHANNEL_LEFT						0x01
#define AUDIO_CHANNEL_RIGHT						0x02
#define AUDIO_CHANNEL_CENTER					0x04
#define AUDIO_CHANNEL_LFE						0x08
#define AUDIO_CHANNEL_L_SURROUND				0x10
#define AUDIO_CHANNEL_R_SURROUND				0x20
#define AUDIO_CHANNEL_L_CENTER					0x40
#define AUDIO_CHANNEL_R_CENTER					0x80
#define AUDIO_CHANNEL_SURROUND					0x100
#define AUDIO_CHANNEL_SIDE_L					0x200
#define AUDIO_CHANNEL_SIDE_R					0x400
#define AUDIO_CHANNEL_TOP						0x800

/* Audio Control Support Bits */
#define AUDIO_CONTROL_SUPPORT_NONE				0x00
#define AUDIO_CONTROL_SUPPORT_MUTE				0x01
#define AUDIO_CONTROL_SUPPORT_VOL				0x02

/* Audio Function Category Codes (UAC2.0) */
#define AUDIO_FUNC_SUBCLASS_DESKTOP_SPEAKER			0x01
#define AUDIO_FUNC_SUBCLASS_HOME_THEATER			0x02
#define AUDIO_FUNC_SUBCLASS_MICROPHONE				0x03
#define AUDIO_FUNC_SUBCLASS_HEADSET					0x04
#define AUDIO_FUNC_SUBCLASS_TELEPHONE				0x05
#define AUDIO_FUNC_SUBCLASS_CONVERTER				0x06
#define AUDIO_FUNC_SUBCLASS_SOUND_RECORDER			0x07
#define AUDIO_FUNC_SUBCLASS_IO_BOX					0x08
#define AUDIO_FUNC_SUBCLASS_MUSICAL_INSTRUMENT		0x09
#define AUDIO_FUNC_SUBCLASS_PRO_AUDIO				0x0A
#define AUDIO_FUNC_SUBCLASS_AUDIO_VIDEO				0x0B
#define AUDIO_FUNC_SUBCLASS_CONTROL_PANEL			0x0C
#define AUDIO_FUNC_SUBCLASS_OTHER					0xFF

/* ----- interface indexes ----- */


#define USB_CONFIG_REMOTE_WAKEUP                        0x02U
#define USB_CONFIG_SELF_POWERED                         0x01U

#define USB_FEATURE_EP_HALT                             0x00U
#define USB_FEATURE_REMOTE_WAKEUP                       0x01U
#define USB_FEATURE_TEST_MODE                           0x02U

#define USB_DEVICE_CAPABITY_TYPE                        0x10U

#define USB_HS_MAX_PACKET_SIZE                          512U
#define USB_FS_MAX_PACKET_SIZE                          64U
#define USB_MAX_EP0_SIZE                                64U

/*  Device Status */
#define USB_DEV_STATE_DEFAULT                      0x01U
#define USB_DEV_STATE_ADDRESSED                    0x02U
#define USB_DEV_STATE_CONFIGURED                   0x03U
#define USB_DEV_STATE_SUSPENDED                    0x04U


/*  EP0 State */
#define USB_DEV_EP0_IDLE                           0x00U
#define USB_DEV_EP0_SETUP                          0x01U
#define USB_DEV_EP0_DATA_IN                        0x02U
#define USB_DEV_EP0_DATA_OUT                       0x03U
#define USB_DEV_EP0_STATUS_IN                      0x04U
#define USB_DEV_EP0_STATUS_OUT                     0x05U
#define USB_DEV_EP0_STALL                          0x06U

#define USB_DEV_EP_TYPE_CTRL                       0x00U
#define USB_DEV_EP_TYPE_ISOC                       0x01U
#define USB_DEV_EP_TYPE_BULK                       0x02U
#define USB_DEV_EP_TYPE_INTR                       0x03U

#define USB_DEV_EP_TYPE_ISOC_ASYNC                 0x05U
#define USB_DEV_EP_TYPE_ISOC_ADAPT                 0x09U
#define USB_DEV_EP_TYPE_ISOC_SYNC                  0x0DU


extern uint8_t send_buff[128];
extern int uart_printf(const char *fmt, ...);

typedef enum
{
    HAL_UNLOCKED = 0x00U,
    HAL_LOCKED   = 0x01U
} hal_lock_t;

/**
   * @brief USB_OTG_Core_Registers
   */
typedef struct
{
    __IO uint32_t GOTGCTL;      /*USB_BASE_ADRRESS + 0x0 USB_OTG Control and Status Register          000h */
    __IO uint32_t GOTGINT;      /*USB_BASE_ADRRESS + 0x4 USB_OTG Interrupt Register                   004h */
    __IO uint32_t GAHBCFG;      /*USB_BASE_ADRRESS + 0x8 Core AHB Configuration Register              008h */
    __IO uint32_t GUSBCFG;      /*USB_BASE_ADRRESS + 0xc Core USB Configuration Register              00Ch */
    __IO uint32_t GRSTCTL;      /*USB_BASE_ADRRESS + 0x10 Core Reset Register                          010h */
    __IO uint32_t GINTSTS;      /*USB_BASE_ADRRESS + 0x14 Core Interrupt Register                      014h */
    __IO uint32_t GINTMSK;      /*USB_BASE_ADRRESS + 0x18 Core Interrupt Mask Register                 018h */
    __IO uint32_t GRXSTSR;      /*USB_BASE_ADRRESS + 0x1c Receive Sts Q Read Register                  01Ch */
    __IO uint32_t GRXSTSP;      /*USB_BASE_ADRRESS + 0x20 Receive Sts Q Read & POP Register            020h */
    __IO uint32_t GRXFSIZ;      /*USB_BASE_ADRRESS + 0x24 Receive FIFO Size Register                   024h */
    __IO uint32_t GNPTXFSIZ;    /*USB_BASE_ADRRESS + 0x28 EP0 / Non Periodic Tx FIFO Size Register     028h */
    uint32_t Reserved2c[2];     /*USB_BASE_ADRRESS + 0x2c ~ 0x30 Reserved*/
    __IO uint32_t GPVNDCTL;     /*USB_BASE_ADRRESS + 0x34 PHY Vendor Control Register The application can use this register to access PHY registers. It is...*/
    uint32_t Reserved38[2];     /*USB_BASE_ADRRESS + 0x38 ~ 0x3c Reserved*/
    __IO uint32_t GSNPSID;      /*USB_BASE_ADRRESS + 0x40 Synopsys ID Register This read-only register contains the release number of the core being...*/
    __IO uint32_t GHWCFG1;      /*USB_BASE_ADRRESS + 0x44 User Hardware Configuration 1 Register This register contains the logical endpoint direction(s)...*/
    __IO uint32_t GHWCFG2;      /*USB_BASE_ADRRESS + 0x48 User Hardware Configuration 2 Register This register contains configuration options selected using...*/
    __IO uint32_t GHWCFG3;      /*USB_BASE_ADRRESS + 0x4c User Hardware Configuration 3 Register*/
    __IO uint32_t GHWCFG4;      /*USB_BASE_ADRRESS + 0x50 User Hardware Configuration 4 Register Note: Bit [31] is available only when Scatter/Gather DMA...*/
    __IO uint32_t GLPMCFG;      /*USB_BASE_ADRRESS + 0x54 LPM Config Register GPWRDN on page 250 0x58 Global Power Down register This is the external Hibernation control register. This register is...*/
    __IO uint32_t GPWRDN;       /*USB_BASE_ADRRESS + 0x58 Reserved*/
    __IO uint32_t GDFIFOCFG;    /*USB_BASE_ADRRESS + 0x5c Global DFIFO Configuration Register GREFCLK on page 260 0x64 ref_clk Control Register*/
    uint32_t Reserved60;        /*USB_BASE_ADRRESS + 0x60 Reserved*/
    __IO uint32_t GREFCLK;      /*USB_BASE_ADRRESS + 0x64 Global DFIFO Configuration Register GREFCLK on page 260 0x64 ref_clk Control Register*/
    __IO uint32_t GINTMSK2;     /*USB_BASE_ADRRESS + 0x68 Interrupt Mask Register 2 This register works with the Interrupt Register (GINTSTS2) to interrupt...*/
    __IO uint32_t GINTSTS2;     /*USB_BASE_ADRRESS + 0x6c Interrupt Register 2 This register interrupts the application for system-level events in the current...*/
} usb_device_reg_t;

/**
   * @brief USB_OTG_device_Registers
   */
typedef struct
{
    __IO uint32_t DCFG;            /*!< dev Configuration Register   800h */
    __IO uint32_t DCTL;            /*!< dev Control Register         804h */
    __IO uint32_t DSTS;            /*!< dev Status Register (RO)     808h */
    uint32_t Reserved0C;           /*!< Reserved                     80Ch */
    __IO uint32_t DIEPMSK;         /*!< dev IN Endpoint Mask         810h */
    __IO uint32_t DOEPMSK;         /*!< dev OUT Endpoint Mask        814h */
    __IO uint32_t DAINT;           /*!< dev All Endpoints Itr Reg    818h */
    __IO uint32_t DAINTMSK;        /*!< dev All Endpoints Itr Mask   81Ch */
    uint32_t  Reserved20;          /*!< Reserved                     820h */
    uint32_t Reserved9;            /*!< Reserved                     824h */
    __IO uint32_t DVBUSDIS;        /*!< dev VBUS discharge Register  828h */
    __IO uint32_t DVBUSPULSE;      /*!< dev VBUS Pulse Register      82Ch */
    __IO uint32_t DTHRCTL;         /*!< dev threshold                830h */
    __IO uint32_t DIEPEMPMSK;      /*!< dev empty msk                834h */
    __IO uint32_t DEACHINT;        /*!< dedicated EP interrupt       838h */
    __IO uint32_t DEACHMSK;        /*!< dedicated EP msk             83Ch */
    uint32_t Reserved40;           /*!< dedicated EP mask            840h */
    __IO uint32_t DINEP1MSK;       /*!< dedicated EP mask            844h */
    uint32_t  Reserved44[15];      /*!< Reserved                 844-87Ch */
    __IO uint32_t DOUTEP1MSK;      /*!< dedicated EP msk             884h */
} USB_OTG_DeviceTypeDef;

/**
   * @brief USB_OTG_IN_Endpoint-Specific_Register
   */
typedef struct
{
    __IO uint32_t DIEPCTL;           /*!< dev IN Endpoint Control Reg    900h + (ep_num * 20h) + 00h */
    uint32_t Reserved04;             /*!< Reserved                       900h + (ep_num * 20h) + 04h */
    __IO uint32_t DIEPINT;           /*!< dev IN Endpoint Itr Reg        900h + (ep_num * 20h) + 08h */
    uint32_t Reserved0C;             /*!< Reserved                       900h + (ep_num * 20h) + 0Ch */
    __IO uint32_t DIEPTSIZ;          /*!< IN Endpoint Txfer Size         900h + (ep_num * 20h) + 10h */
    __IO uint32_t DIEPDMA;           /*!< IN Endpoint DMA Address Reg    900h + (ep_num * 20h) + 14h */
    __IO uint32_t DTXFSTS;           /*!< IN Endpoint Tx FIFO Status Reg 900h + (ep_num * 20h) + 18h */
    __IO uint32_t DIEPDMAB;          /*!< Device IN Endpoint 16 Buffer Address Register  900h+ (ep_num * 20h) + 1Ch */
} USB_OTG_INEndpointTypeDef;

/**
   * @brief USB_OTG_OUT_Endpoint-Specific_Registers
   */
typedef struct
{
    __IO uint32_t DOEPCTL;       /*!< dev OUT Endpoint Control Reg           B00h + (ep_num * 20h) + 00h */
    uint32_t Reserved04;         /*!< Reserved                               B00h + (ep_num * 20h) + 04h */
    __IO uint32_t DOEPINT;       /*!< dev OUT Endpoint Itr Reg               B00h + (ep_num * 20h) + 08h */
    uint32_t Reserved0C;         /*!< Reserved                               B00h + (ep_num * 20h) + 0Ch */
    __IO uint32_t DOEPTSIZ;      /*!< dev OUT Endpoint Txfer Size            B00h + (ep_num * 20h) + 10h */
    __IO uint32_t DOEPDMA;       /*!< dev OUT Endpoint DMA Address           B00h + (ep_num * 20h) + 14h */
    __IO uint32_t DOEPDMAB;      /*!< Device IN Endpoint 16 Buffer Address Register  B00h + (ep_num * 20h) + 14h + 18h */
    uint32_t Reserved1c;         /*!< Reserved                               B00h + (ep_num * 20h) + 1Ch */
} USB_OTG_OUTEndpointTypeDef;

/**
   * @brief USB_OTG_Host_Mode_Register_Structures
   */
typedef struct
{
    __IO uint32_t HCFG;             /*!< Host Configuration Register          400h */
    __IO uint32_t HFIR;             /*!< Host Frame Interval Register         404h */
    __IO uint32_t HFNUM;            /*!< Host Frame Nbr/Frame Remaining       408h */
    uint32_t Reserved40C;           /*!< Reserved                             40Ch */
    __IO uint32_t HPTXSTS;          /*!< Host Periodic Tx FIFO/ Queue Status  410h */
    __IO uint32_t HAINT;            /*!< Host All Channels Interrupt Register 414h */
    __IO uint32_t HAINTMSK;         /*!< Host All Channels Interrupt Mask     418h */
} USB_OTG_HostTypeDef;

/**
   * @brief USB_OTG_Host_Channel_Specific_Registers
   */
typedef struct
{
    __IO uint32_t HCCHAR;           /*!< Host Channel Characteristics Register    500h */
    __IO uint32_t HCSPLT;           /*!< Host Channel Split Control Register      504h */
    __IO uint32_t HCINT;            /*!< Host Channel Interrupt Register          508h */
    __IO uint32_t HCINTMSK;         /*!< Host Channel Interrupt Mask Register     50Ch */
    __IO uint32_t HCTSIZ;           /*!< Host Channel Transfer Size Register      510h */
    __IO uint32_t HCDMA;            /*!< Host Channel DMA Address Register        514h */
    uint32_t Reserved[2];           /*!< Reserved                                      */
} USB_OTG_HostChannelTypeDef;

#define USBx_PCGCCTL    *(__IO uint32_t *)((uint32_t)usb_base_addr + USB_OTG_PCGCCTL_BASE)
#define USBx_PCGCCTL1   *(__IO uint32_t *)((uint32_t)usb_base_addr + USB_OTG_PCGCCTL1_BASE)
#define USBx_HPRT0      *(__IO uint32_t *)((uint32_t)usb_base_addr + USB_OTG_HOST_PORT_BASE)

#define USBx_DEVICE     ((USB_OTG_DeviceTypeDef *)(usb_base_addr + USB_OTG_DEVICE_BASE))
#define USBx_INEP(i)    ((USB_OTG_INEndpointTypeDef *)(usb_base_addr + USB_OTG_IN_ENDPOINT_BASE + ((i) * USB_OTG_EP_REG_SIZE)))
#define USBx_OUTEP(i)   ((USB_OTG_OUTEndpointTypeDef *)(usb_base_addr + USB_OTG_OUT_ENDPOINT_BASE + ((i) * USB_OTG_EP_REG_SIZE)))
#define USBx_DFIFO(i)   *(__IO uint32_t *)(usb_base_addr + USB_OTG_FIFO_BASE + ((i) * USB_OTG_FIFO_SIZE))
#define USBx_DIEPTXF(i) *(__IO uint32_t *)(usb_base_addr + USB_OTG_IN_FIFO_BASE + ((i) * 4))
#define USBx_HOST       ((USB_OTG_HostTypeDef *)(usb_base_addr + USB_OTG_HOST_BASE))
#define USBx_HC(i)      ((USB_OTG_HostChannelTypeDef *)(usb_base_addr + USB_OTG_HOST_CHANNEL_BASE + ((i) * USB_OTG_HOST_CHANNEL_SIZE)))


#define USB_BASE_ADRRESS   0x75100000UL


#define USB_OTG_GLOBAL_BASE                  0x000UL
#define USB_OTG_IN_FIFO_BASE                 0x104UL
#define USB_OTG_DEVICE_BASE                  0x800UL
#define USB_OTG_IN_ENDPOINT_BASE             0x900UL
#define USB_OTG_OUT_ENDPOINT_BASE            0xB00UL
#define USB_OTG_EP_REG_SIZE                  0x20UL
#define USB_OTG_HOST_BASE                    0x400UL
#define USB_OTG_HOST_PORT_BASE               0x440UL
#define USB_OTG_HOST_CHANNEL_BASE            0x500UL
#define USB_OTG_HOST_CHANNEL_SIZE            0x20UL
#define USB_OTG_PCGCCTL_BASE                 0xE00UL
#define USB_OTG_PCGCCTL1_BASE                0xE04UL
#define USB_OTG_FIFO_BASE                    0x1000UL
#define USB_OTG_FIFO_SIZE                    0x1000UL


#define USB_OTG_FS                           ((usb_device_reg_t *) USB_BASE_ADRRESS)

#define GRXSTS_PKTSTS_IN                       2U
#define GRXSTS_PKTSTS_IN_XFER_COMP             3U
#define GRXSTS_PKTSTS_DATA_TOGGLE_ERR          5U
#define GRXSTS_PKTSTS_CH_HALTED                7U

#define EP_ADDR_MSK                            0xFU


/* Following USB Device status */

typedef enum
{
    USBD_OK   = 0U,
    USBD_BUSY,
    USBD_FAIL,
    USBD_TIMEOUT
}usb_dev_status_t;

#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */

#define USB_PHY_ULPI                 1U
#define USB_PHY_EMBEDDED             2U

/**
   * @brief  USB OTG Initialization Structure definition
   */
typedef struct
{
    uint32_t devEndpoints;
    uint32_t speed;
    uint32_t dmaEnable;
    uint32_t phyInterface;
    uint32_t sofEnable;
    uint32_t lowPowerEnable;
    uint32_t lpmEnable;
    uint32_t vbusSensingEnable;
    uint32_t useDedicatedEp1;
    uint32_t useExternalVbus;
} usb_dev_cfg_t;

typedef struct
{
    uint8_t   num;                  // EP number[1, 15]
    uint8_t   isIn;                 // EP direction
    uint8_t   isStall;              // EP stall condition
    uint8_t   type;                 // Bulk, Control, Iso, Inter
    uint8_t   pidStart;             // Initial data PID
    uint16_t  txFifoNum;            // Transmission FIFO number[1, 15]
    uint32_t  maxPacket;            // EP Max packet size, between  0 and  64KB */
    uint8_t   *xferBuff;            // Pointer to transfer buffer
    uint32_t  dmaAddr;             // 32 bits aligned transfer buffer address
    uint32_t  xferLen;             // Current transfer length
} usb_dev_ep_t;

typedef enum
{
    HAL_USB_STATE_RESET   = 0x00,
    HAL_USB_STATE_READY   = 0x01,
    HAL_USB_STATE_ERROR   = 0x02,
    HAL_USB_STATE_BUSY    = 0x03,
    HAL_USB_STATE_TIMEOUT = 0x04
} hal_usb_state_t;

/* Device LPM suspend state */
typedef enum
{
    LPM_L0 = 0x00, /* on */
    LPM_L1 = 0x01, /* LPM L1 sleep */
    LPM_L2 = 0x02, /* suspend */
    LPM_L3 = 0x03, /* off */
} hal_usb_lpm_state_t;


#define SETUPBUFF_SIZE USB_MAX_EP0_SIZE

typedef struct USB_Device_HandleTypeDef
{
    usb_device_reg_t        *Instance;
    usb_dev_cfg_t           Init;
    __IO uint8_t            usbAddr;
    usb_dev_ep_t            inEndPoint[USB_MAX_DEV_NUM_ENDPOINTS];
    usb_dev_ep_t            outEndPoint[USB_MAX_DEV_NUM_ENDPOINTS];
    hal_lock_t              lock;
    __IO hal_usb_state_t    State;
    uint8_t                 Setup[SETUPBUFF_SIZE];
    hal_usb_lpm_state_t     lpmState;
    void                    *pData;
} usb_dev_handler_t;

typedef enum
{
    UAC_10  = 0U,
    UAC_20  = 1U,
}uac_spec_t;

#define USB_SPEED_HIGH			0U	//480Mbps->60MBps
#define USB_SPEED_FULL			1U	//12Mbps - > 1.5MBps
#define USB_SPEED_LOW			2U	//1.5Mbps- >192KBps

extern uac_spec_t is_usb_dev_UAC20;
extern uint8_t Usbd_Speed_Cfg;
extern const uint8_t usb_interface_num;
extern const uint16_t ep_rxfifo;
extern const bool spkEn;
extern const bool micEn;
extern const bool lpmEn;


typedef struct
{
    uint8_t   bmRequest;
    uint8_t   bRequest;
    uint16_t  wValue;
    uint16_t  wIndex;
    uint16_t  wLength;
}usb_setup_req_t;

/* USB Device handle structure */
typedef struct
{
    uint32_t    status;
    uint32_t    isUsed;
    uint32_t    totalLen;
    uint32_t    remLen;
    uint32_t    maxPacket;
} usb_dev_endpoint_t;

/* USB Device descriptors structure */
typedef struct
{
    uint8_t  *(*GetDeviceDescriptor)          ( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetLangIDStrDescriptor)       ( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetManufacturerStrDescriptor) ( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetProductStrDescriptor)      ( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetSerialStrDescriptor)       ( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetConfigurationStrDescriptor)( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetInterfaceStrDescriptor)    ( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetProfileStrDescriptor)        ( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetHeadphoneStrDescriptor)    ( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetMicrophoneStrDescriptor)   ( uint8_t speed, uint16_t *length);
#if (USB_DEV_LPM_ENABLED == 1U)
    uint8_t  *(*GetBOSDescriptor)             ( uint8_t speed, uint16_t *length);
#endif
#if (USB_DEV_SUPPORT_USER_STRING == 1U)
    uint8_t  *(*GetUsrStrDescriptor)          ( uint8_t speed, uint8_t idx, uint16_t *length);
#endif
} usb_dev_get_desc_t;

struct _usb_device_type;

typedef struct _device_callback
{
    uint8_t  (*Init)             (struct _usb_device_type *pdev, uint8_t cfgidx);
    uint8_t  (*DeInit)           (struct _usb_device_type *pdev, uint8_t cfgidx);
    /* Control Endpoints*/
    uint8_t  (*Setup)            (struct _usb_device_type *pdev, usb_setup_req_t  *req);
    uint8_t  (*EP0_TxSent)       (struct _usb_device_type *pdev );
    uint8_t  (*EP0_RxReady)      (struct _usb_device_type *pdev );
    /* Class Specific EPs*/
    uint8_t  (*DataIn)           (struct _usb_device_type *pdev, uint8_t epnum);
    uint8_t  (*DataOut)          (struct _usb_device_type *pdev, uint8_t epnum);
    uint8_t  (*SOF)              (struct _usb_device_type *pdev);
    uint8_t  (*IsoINIncomplete)  (struct _usb_device_type *pdev, uint8_t epnum);
    uint8_t  (*IsoOUTIncomplete) (struct _usb_device_type *pdev, uint8_t epnum);

    uint8_t *(*GetHSConfigDescriptor)         (uint16_t *length);
    uint8_t *(*GetFSConfigDescriptor)         (uint16_t *length);
    uint8_t *(*GetOtherSpeedConfigDescriptor) (uint16_t *length);
    uint8_t *(*GetDeviceQualifierDescriptor)  (uint16_t *length);
#if (USB_DEV_SUPPORT_USER_STRING == 1U)
    uint8_t *(*GetUsrStrDescriptor) (struct _usb_device_type *pdev,uint8_t index, uint16_t *length);
#endif
} usb_dev_class_cb_t;

typedef struct _usb_device_type
{
    uint8_t                 devState;
    uint8_t                 devOldState;
    uint8_t                 devAddress;

    uint32_t                devConfig;
    uint32_t                devDefaultConfig;
    uint32_t                devConfigStatus;
    uint8_t             devSpeed;
	uac_spec_t              devUacType;
    usb_dev_endpoint_t      epIn[USB_MAX_DEV_NUM_ENDPOINTS];
    usb_dev_endpoint_t      epOut[USB_MAX_DEV_NUM_ENDPOINTS];
	uint16_t                ep0TxFifoSize;
	uint16_t                ep1TxFifoSize;
	uint16_t                ep2TxFifoSize;
	uint16_t                ep3TxFifoSize;
    uint32_t                ep0State;
    uint32_t                ep0DataLen;
    uint32_t                devRemoteWakeup;
    uint32_t                devLpmConfig;
    uint32_t                irqPriority;

    usb_setup_req_t         request;
    usb_dev_get_desc_t      *pDesc;
    usb_dev_class_cb_t      *pClass[USB_FUN_MAX_NUM_INTERFACES];
    void                    *pClassData;
    void                    *pUserData;
    void                    *pData;
    bool					isUsbMicEn;
    bool					isUsbSpkEn;
}usb_device_t;


#define SWAPBYTE(addr) (((uint16_t)(*((uint8_t *)(addr)))) + (((uint16_t)(*(((uint8_t *)(addr)) + 1U))) << 8U))
#define LOBYTE(x)  ((uint8_t)(x & 0x00FFU))
#define HIBYTE(x)  ((uint8_t)((x & 0xFF00U) >> 8U))


#if  defined ( __GNUC__ )
#ifndef __weak
#define __weak   __attribute__((weak))
#endif /* __weak */
#ifndef __packed
//#define __packed __attribute__((packed))
#define __packed
#endif /* __packed */
#endif /* __GNUC__ */


/* In HS mode and when the DMA is used, all variables and data structures dealing
   with the DMA during the transaction process should be 4-bytes aligned */

#if defined  (__GNUC__)        /* GNU Compiler */

#define __ALIGN_END    __attribute__ ((aligned (4)))
#define __ALIGN_BEGIN

#else
#define __ALIGN_END
#if defined   (__CC_ARM)      /* ARM Compiler */
#define __ALIGN_BEGIN    __align(4)
#elif defined (__ICCARM__)    /* IAR Compiler */
#define __ALIGN_BEGIN
#elif defined  (__TASKING__)  /* TASKING Compiler */
#define __ALIGN_BEGIN    __align(4)
#endif /* __CC_ARM */
#endif /* __GNUC__ */

#endif
#ifdef __cplusplus
}
#endif

#endif /* __USB_DEV_DEF_H */


