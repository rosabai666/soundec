/**
***********************************************************
 *
 * @file name	: user_config_2mic_usb_headphone.h
 * @author		: yen.yu
 * @version		: Vxx.xx
 * @date		: 2022-03-23
 * @brief		: config for 2mic usb headphone application
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
#ifndef __USER_CONFIG_2MIC_USB_HEADPHONE_BOX__
#define __USER_CONFIG_2MIC_USB_HEADPHONE_BOX__

/* Includes ------------------------------------------------------------------*/

#include "types.h"
#include "snc8600.h"
#include "drv_pll.h"
#include "hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************
 *  Version config
 ****************************************************/
#define CONFIG_PRODUCT_VERSION				0x2020			/*version number*/

/*****************************************************
 *  Chip config
 ****************************************************/
#define CONFIG_CHIP_TYPE					CHIP_SNC8600
#define CONFIG_VDD_IO_18					0				/*0:VDD_IO 3.3V 1:VDD_IO 1.8V*/
/*****************************************************
 *  Module:		PLL
 ****************************************************/
#define SYSTEM_CRYSTAL						SYSTEM_CRYSTAL_24MHZ
// [PLL]
#define PLL_OUT_VALUE						PLLOUT_220MHZ

//#define JTAG_DEBUG_ENABLE

/*****************************************************
 *  Module:		UART
 ****************************************************/
#define UART_ENABLE							1
#if(UART_ENABLE == 1)
//user code
#define UART_BUARD_RATE						BAUDRATE_115200	/*baud rate*/
#define UART_TX_INT_ENABLE					0
#define UART_RX_INT_ENABLE                  1
#define UART_RECEIVE						0
#define UART_QUEUE_SIZE						300				/*decide by delay in onFifoReady() and instruction length*/
#endif

/*****************************************************
 *  Module:		AUDIO_MANAGER
 ****************************************************/
#define AUDIO_MANAGER						1				/*must be 1*/
#if AUDIO_MANAGER
//#define TEST_AU_MANAGER
#endif

/*****************************************************
 *	Module:		DMA
 ****************************************************/
#define DMA_ENABLE							0
#define DMAC_CHX_NULL						6
#if(DMA_ENABLE)
#if(AUDIO_MANAGER)
#define DMA_CHX_MEM_TO_MEM					0
#define DMA_CHX_MEM_TO_I2S1					DMAC_CHANNEL_1
#define DMA_CHX_ADC_TO_MEM					DMAC_CHANNEL_5
#else
#define DMA_TEST_MODE_ENABLE				0
#if(DMA_TEST_MODE_ENABLE)
#define DMA_TEST_I2S_USB_ENABLE				0
#define DMA_TEST_CODEC_ADC_ENABLE			1
#endif	//DMA_TEST_MODE_ENABLE
#endif	//AUDIO_MANAGER
#endif	//DMA_ENABLE

/*****************************************************
 *  Module:		ALG
 ****************************************************/
#define SUPPORT_ALG_2MIC_HEADSET			0

/*****************************************************
 *  Module:		Codec
 ****************************************************/
#define CODEC_ENABLE						1
#if (CODEC_ENABLE)
#define CODEC_ADC_ENABLE  					0
#define CODEC_ADC12_ENABLE					1
#define CODEC_ADC34_ENABLE					0
#define CODEC_ADC56_ENABLE					0
#define CODEC_ADC78_ENABLE					0
#define CODEC_ADC9A_ENABLE					0

#define CODEC_DAC_ENABLE					1

#define CODEC_MASTER						1		/*SLAVE MODE:ADC12 must be initialized, or DAC can not work normally.*/
#define CODEC_ADC_DATA_MODE					CODEC_ADC_DATA_MODE_24BIT
#define CODEC_SLAVE_FRAC_SRC				CODEC_SLAVE_FRAC_SRC_SNC
#define CODEC_SLAVE_FRAC_SHARE				CODEC_SLAVE_FRAC_SHARE_DEDICATED
#define CODEC_FIFO_AE_LEVEL					4		/*Almost empty level*/
#if(DMA_ENABLE && AUDIO_MANAGER)
#define CODEC_FIFO_AF_LEVEL					1		/*Almost full level*/
#else
#define CODEC_FIFO_AF_LEVEL					4		/*Almost full level*/
#endif
#define ADC12_IS_AMIC						1		/* 1:Analog Microphone; 0:Digital Microphone*/
#define ADC_MICBIAS							1
#define CODEC_MIC_AGC_ENABLE				0
#define CODEC_SPK_DRC_ENABLE				0
#define DAC_AIAS 							1		/*DAC sample rate tuning*/
#define ADCX_AIAS 							0		/*ADC sample rate tuning(include all)*/
#define ADC12_AIAS 							1		/*ADC12 sample rate tuning*/
#define ADC3456_AIAS 						0		/*ADC3456 sample rate tuning*/
#define ADC789a_AIAS 						0		/*ADC789a sample rate tuning*/

#define CONFIG_CODEC_FREQUENCY				SAMPLING_RATE_48000
#endif

/*****************************************************
 *  Module:		USB
 ****************************************************/
#define USB_ENABLE     						1
#if USB_ENABLE

#define USB_PID								0x3829
#define USB_PRODUCT_STRING        			"USB NC Headset"

#define USB_SPEED_CFG          				USB_SPEED_HIGH	/*UAC10:USB_SPEED_HIGH or USB_SPEED_FULL UAC20:USB_SPEED_HIGH*/

#define SUPPORT_USB_HID						1
#define SUPPORT_USB_SPK    					1
#define SUPPORT_USB_MIC        				1
#define SUPPORT_USB_24BIT_ONLY              0
#define SUPPORT_UAC20						1
#define SUPPORT_USB_LPM						0
#define SUPPORT_USB_USR_STR					0
#if ((SUPPORT_USB_SPK)&&(SUPPORT_USB_MIC))
#define SUPPORT_SPK_SIDETONE            	0
#else
#define SUPPORT_SPK_SIDETONE            	0
#endif


#define SUPPORT_USB_INSERT_DETECTION		1		/*USB insert detect*/
#if(SUPPORT_USB_INSERT_DETECTION)
#define TEST_DETECT_PC						0		/*identify PC or mobile phone*/
#if(SUPPORT_UAC20)
#define SUPPORT_UAC_SELF_ADAPTION			1		/*USB adaption:switch to UAC10 if UAC20 is not supported*/
#endif
#endif

#if(SUPPORT_USB_SPK)
#define SPK_FS_8K           				0
#define SPK_FS_16K          				0
#define SPK_FS_32K          				0
#define SPK_FS_44P1K        				0
#define SPK_FS_48K          				1
#define SPK_FS_88P2K        				0
#define SPK_FS_96K          				0
#define SPK_FS_176P4K       				0
#define SPK_FS_192K         				0
#define SPK_AUDIO_16BIT     				1
#define SPK_AUDIO_24BIT     				1
#define SPK_AUDIO_32BIT     				0
#endif

#if(SUPPORT_USB_MIC)
#define	USB_MIC_CHANNELS                    2
#define USB_MIC_CHANNEL_SEL					(AUDIO_CHANNEL_LEFT | AUDIO_CHANNEL_RIGHT)

#define MIC_FS_8K               			0
#define MIC_FS_16K              			0
#define MIC_FS_32K              			0
#define MIC_FS_44P1K            			0
#define MIC_FS_48K              			1
#define MIC_FS_88P2K            			0
#define MIC_FS_96K              			0
#define MIC_FS_176P4K           			0
#define MIC_FS_192K             			0
#define MIC_AUDIO_16BIT         			1
#define MIC_AUDIO_24BIT         			0
#endif

#define CONGIFG_USB_DEV_VOLUME_FIT_USBH

#ifdef ALTO_USB_LPM_ENABLE
#define HANDLE_AO_ADC_KEY_WAKEUP_FROM_USB_DEV_SUSPEND
#define HANDLE_USB_DEV_SUSPEND_MODE2
#endif

//#define USB_DEV_KEYSIGHT_TEST_MODE
#define SUPPORT_SYNC_HOST_VOL
//#define CONGIFG_USB_DEV_VOLUME_FIT_USBH

#define SUPPORT_USB_FEEDBACK				0
#if(SUPPORT_USB_FEEDBACK)
#define FEEDBACK_SOF_RATE					0x03U	/*2^(x)*/
#endif

#endif

/*****************************************************
 *  Module:		Alto tool
 ****************************************************/
#if USB_ENABLE
#define SUPPORT_ALTOTOOL        			0
#if (SUPPORT_ALTOTOOL)
#define SUPPORT_DSP_EQ    					1
#if (SUPPORT_DSP_EQ)
#define SUPPORT_MIC_EQ                		1
#define SUPPORT_SPKER_EQ              		1
#if (SUPPORT_SPKER_EQ)
#define SUPPORT_SPKER_STEREO_EQ				1
#endif	//SUPPORT_SPKER_EQ
#endif	//SUPPORT_DSP_EQ
#define SUPPORT_ALTOTOOL_CODEDC     		1
#define SUPPORT_ALTOTOOL_I2S           	 	1
#define SUPPORT_ALTOTOOL_DEBUG          	1
#define SUPPORT_ALTOTOOL_COMM           	1

#endif	//SUPPORT_ALTOTOOL
#endif	//USB_ENABLE


/*****************************************************
 *  Module:		I2S
 ****************************************************/
#define I2S_ENABLE    						0
#if (I2S_ENABLE == 1)
#define I2S1_ENABLE							0
#define I2S2_ENABLE							0
#define I2S3_ENABLE							0
#define I2S_TX_FIFO_LEVEL					8  /*1-16*/
#define I2S_RX_FIFO_LEVEL					8  /*1-16*/
#define I2S_SAMPLE_RATE_DETECT 				0
#define I2S_FIFO_WORD_LENGTH				5  /*2:16bit 4:24bit 5:32bit  192k do not support 32bit*/
#if(DMA_ENABLE)
#define I2S_DMA_ENABLE
#endif
#if(USB_ENABLE && SUPPORT_USB_SPK)
//#define TEST_USB_I2S_SYNC
#endif
#if(CODEC_ENABLE && CODEC_DAC_ENABLE)
//#define TEST_I2S_DAC_SYNC
#endif
#endif

/*****************************************************
 *	Module:		Flash
 ****************************************************/
#define FLASH_ENABLE						1
#if(FLASH_ENABLE)
#define FLASH_DEBUG_ENABLE					1
#define FLASH_WRITE_PROTECT_ENABLE			0
#define FLASH_SFC_CACHE_ENABLE				0
#define FLASH_SAVE_BANK_ENABLE				1
#if(FLASH_SAVE_BANK_ENABLE)
#define FLASH_SAVE_BANK_DEBUG_ENABLE		1
#endif
#define FLASH_GET_UID_ENABLE				1
#if(FLASH_GET_UID_ENABLE)
#define FLASH_UID_AS_USB_SN_ENABLE			1
#endif
#endif

/*****************************************************
 *  Module:		AUX ADC
 ****************************************************/
#define AUX_ADC_ENABLE						0
#if(AUX_ADC_ENABLE)
#define AUX_ADC_INTR_ENABLE					0
#define AUX_ADC_USE_CH0						1	/*if 0, use ch1*/
#define APP_ADC_KEY_ENABLE					1
#endif	//AUX_ADC_ENABLE

/*****************************************************
 *  Module:		RTC
 ****************************************************/
#define RTC_ENABLE							0
#if RTC_ENABLE == 1
//use code
#define RTC_ALARM_INT_ENABLE				1
#define RTC_MIN_INT_ENABLE					1
#define RTC_SEC_INT_ENABLE					1
#define RTC_SAMP_INT_ENABLE					1
#define RTC_WD_INT_ENABLE					1
#endif

/*****************************************************
 *  Module:		PWM
 ****************************************************/
#define PWMX_ENABLE							0
#if PWMX_ENABLE == 1
//use code
#define PWM_NORMAL_ENABLE					1
#define PWM_GPIO_MODE_ENABLE				1
#define PWM_TIMER1_MODE_ENABLE				1	/*count a*/
#define PWM_TIMER2_MODE_ENABLE				1 	/*cycle count*/

#define PWM_INT_ENABLE						1
#endif

/*****************************************************
 *  Module:		I2C
 ****************************************************/
#define I2C_ENABLE							0
#if I2C_ENABLE == 1

#define I2C_MASTER_ENABLE					1
#if(I2C_MASTER_ENABLE == 1)
#define I2C_MASTER_SEL						I2C1_BASE
#define I2C_MASTER_SEL_IRQ  				I2C1_IRQn
#endif //I2C_MASTER_ENABLE

#define I2C_SLAVE_ENABLE					0
#if(I2C_SLAVE_ENABLE == 1)
#define I2C_SLAVE_SEL						I2C2_BASE
#define I2C_SLAVE_SEL_IRQ  					I2C2_IRQn
#endif	//I2C_SLAVE_ENABLE

#endif //I2C_ENABLE

/*****************************************************
 *  Module:		Play tone
 ****************************************************/
#define SUPPORT_PLAY_TONE					1
#if SUPPORT_PLAY_TONE
	#define D_DAC_PLAY_TONE_ENABLE			1
	#define D_I2S_PLAY_TONE_ENABLE			1
	#define D_USB_PLAY_TONE_ENABLE			1
	#define PREEMPTIVE_MODE					1	/*1:Play the latest tone immediately*/
										 		/*0:Play the nest tone only after the last tone has played*/
	#define D_PLAY_TONE_OUTPUT_MIX			0 	/*1:tone+music,0:only tone*/
	#define D_USE_SOUNDEC_WAV				1 	/*choose wav files*/

	#define TEST_PLAY_TONE					0
#endif

/*****************************************************
 *  Module:		Debug
 ****************************************************/
#define CPU_USAGE_DEBUG						0
#define MEMORY_SPACE_MONITOR				0

/*****************************************************
 *			Compiler warnings
 ****************************************************/
#if(CONFIG_CHIP_TYPE == CHIP_SNC8600A)
#if I2S2_ENABLE || CODEC_ADC78_ENABLE || CODEC_ADC9A_ENABLE
#error "8600A can not support I2S2 or mic 789A!"
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif  /* __USER_CONFIG_2MIC_USB_HEADPHONE_BOX__ */


