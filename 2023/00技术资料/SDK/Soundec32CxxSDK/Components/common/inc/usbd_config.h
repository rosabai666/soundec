/**
***********************************************************
 *
 * @file name	: usbd_config.h
 * @author	: usbd user base config
 * @version	: Vxx.xx
 * @date	: 2020-03-05
 * @brief	: usbd user base config
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CONFIG_H__
#define __USB_CONFIG_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif
#if (!USB_ENABLE)
#define SUPPORT_USB_HID						0
#define SUPPORT_USB_SPK    					0
#define SUPPORT_USB_MIC        				0
#endif


#if(SUPPORT_USB_FEEDBACK)
//#define SUPPORT_FEEDBACK_TUNE
#define FEEDBACK_RATE_VAL				(1 << FEEDBACK_SOF_RATE)
#define SPK_FEEDBACK_DESC_SIZE			9
#define SPK_ENDPOINT_NUM				2
#else
#define SPK_FEEDBACK_DESC_SIZE			0
#define SPK_ENDPOINT_NUM				1
#endif

#define BITWIDTH_16                     (16)
#define BITWIDTH_24                     (24)
#define BITWIDTH_32                     (32)


#define ALL_USB_BITS BITWIDTH_16

#define USB_AUDIO_NUM                                	(SUPPORT_USB_SPK + SUPPORT_USB_MIC)

#if(USB_AUDIO_NUM > 0)
#define USB_AUDIO_CTRL_INTERFACES			1
#else
#define USB_AUDIO_CTRL_INTERFACES			0
#endif

#if SUPPORT_USB_HID
#define USB_HID_INTERFACES                   1
#else
#define USB_HID_INTERFACES                   0
#endif

#if SUPPORT_USB_SPK
#define USB_SPEAKER_INTERFACES               1
#else
#define USB_SPEAKER_INTERFACES               0
#endif

#if SUPPORT_USB_MIC
#define USB_MIC_INTERFACES                   1
#else
#define USB_MIC_INTERFACES                   0
#endif


#define USB_FUN_NUM_INTERFACES          ( USB_AUDIO_CTRL_INTERFACES  +    \
										    USB_HID_INTERFACES +   \
										    USB_SPEAKER_INTERFACES +\
										    USB_MIC_INTERFACES )

/* ----- audio format ---- */
#define	USB_SPK_CHANNELS                    2
#define USB_SPK_CHANNEL_SEL					(AUDIO_CHANNEL_LEFT | AUDIO_CHANNEL_RIGHT)
#define USB_SPK_CONTROL_MASTER				(AUDIO_CONTROL_SUPPORT_MUTE)
#define USB_SPK_CONTROL_CH					(AUDIO_CONTROL_SUPPORT_VOL)

#define USB_MIC_CONTROL_MASTER				(AUDIO_CONTROL_SUPPORT_MUTE | AUDIO_CONTROL_SUPPORT_VOL)
#define USB_MIC_CONTROL_CH					(AUDIO_CONTROL_SUPPORT_NONE)

#define FS_ISO_OUT_ENDP_MAX_PACKET_SIZE_192X24BIT   (0x0800 | 0x240)
#define FS_ISO_IN_ENDP_MAX_PACKET_SIZE_192X24BIT    (0x0800 | 0x240)

#define HID_ISO_IN_ENDP_PACKET_SIZE                 3

#define USB_SHORT_GET_LOW(x)                        (((uint16_t)x) & 0xFFU)
#define USB_SHORT_GET_HIGH(x)                       ((uint8_t)(((uint16_t)x) >> 8U) & 0xFFU)

#if(SUPPORT_USB_SPK)
#if (SPK_FS_48K)
#define SPK_DEFAULT_FREQ						48000
#elif(SPK_FS_44P1K)
#define SPK_DEFAULT_FREQ						44100
#elif(SPK_FS_96K)
#define SPK_DEFAULT_FREQ						96000
#elif(SPK_FS_16K)
#define SPK_DEFAULT_FREQ						16000
#elif(SPK_FS_192K)
#define SPK_DEFAULT_FREQ						192000
#elif(SPK_FS_88P2K)
#define SPK_DEFAULT_FREQ						88200
#elif(SPK_FS_176P4K)
#define SPK_DEFAULT_FREQ						17640
#elif(SPK_FS_32K)
#define SPK_DEFAULT_FREQ						32000
#elif(SPK_FS_8K)
#define SPK_DEFAULT_FREQ						8000
#endif

#if(SPK_AUDIO_24BIT)
#define SPK_DEFAULT_BIT_SLOT					BIT_SLOT_24
#elif(SPK_AUDIO_16BIT)
#define SPK_DEFAULT_BIT_SLOT					BIT_SLOT_16
#else
#define SPK_DEFAULT_BIT_SLOT					BIT_SLOT_32
#endif
#endif

#if(SUPPORT_USB_MIC)
#if (MIC_FS_48K)
#define MIC_DEFAULT_FREQ						48000
#elif(MIC_FS_44P1K)
#define MIC_DEFAULT_FREQ						44100
#elif(MIC_FS_96K)
#define MIC_DEFAULT_FREQ						96000
#elif(MIC_FS_16K)
#define MIC_DEFAULT_FREQ						16000
#elif(MIC_FS_192K)
#define MIC_DEFAULT_FREQ						192000
#elif(MIC_FS_88P2K)
#define MIC_DEFAULT_FREQ						88200
#elif(MIC_FS_176P4K)
#define MIC_DEFAULT_FREQ						17640
#elif(MIC_FS_32K)
#define MIC_DEFAULT_FREQ						32000
#elif(MIC_FS_8K)
#define MIC_DEFAULT_FREQ						8000
#endif

#if(MIC_AUDIO_24BIT)
#define MIC_DEFAULT_BIT_SLOT					BIT_SLOT_24
#else
#define MIC_DEFAULT_BIT_SLOT					BIT_SLOT_16
#endif
#endif

#define USB_DEV_ENDPOINT_RXFIFO              800
#define USB_DEV_ENDPOINT0_TXFIFO             256//128
#if SUPPORT_USB_SPK
#define USB_DEV_ENDPOINT1_TXFIFO             64    //feedback
#if SUPPORT_USB_MIC
#define USB_DEV_ENDPOINT2_TXFIFO             600
#else
#define USB_DEV_ENDPOINT2_TXFIFO             64
#endif
#else
#if SUPPORT_USB_MIC
#define USB_DEV_ENDPOINT1_TXFIFO             800
#else
#define USB_DEV_ENDPOINT1_TXFIFO             64
#endif
#define USB_DEV_ENDPOINT2_TXFIFO             64
#endif
#define USB_DEV_ENDPOINT3_TXFIFO             64
#define USB_DEV_ENDPOINT4_TXFIFO             64



#define HS_ISO_OUT_ENDP_INTERVAL        (0x04)
#define HS_ISO_IN_ENDP_INTERVAL         (0x04)
#define FS_ISO_OUT_ENDP_INTERVAL        (0x01)
#define FS_ISO_IN_ENDP_INTERVAL         (0x01)
#define HS_INTERRUPT_IN_INTERVAL        (0x07U) /* 2^(7-1) = 8ms */
#define FS_INTERRUPT_IN_INTERVAL        (0x08U)
#define HID_HS_BINTERVAL                (0x07U)
#define HID_FS_BINTERVAL                (0x01U)


#if(USB_SPEED_CFG == USB_SPEED_HIGH)
#define ISO_OUT_ENDP_INTERVAL       HS_ISO_OUT_ENDP_INTERVAL
#define ISO_IN_ENDP_INTERVAL        HS_ISO_IN_ENDP_INTERVAL
#define HID_BINTERVAL               HID_HS_BINTERVAL
#define INTERRUPT_IN_INTERVAL       HS_INTERRUPT_IN_INTERVAL
#elif(USB_SPEED_CFG == USB_SPEED_FULL)
#define ISO_OUT_ENDP_INTERVAL       FS_ISO_OUT_ENDP_INTERVAL
#define ISO_IN_ENDP_INTERVAL        FS_ISO_IN_ENDP_INTERVAL
#define HID_BINTERVAL               HID_FS_BINTERVAL
#define INTERRUPT_IN_INTERVAL       FS_INTERRUPT_IN_INTERVAL
#else	//USB_SPEED_CFG
#define ISO_OUT_ENDP_INTERVAL       FS_ISO_OUT_ENDP_INTERVAL
#define ISO_IN_ENDP_INTERVAL        FS_ISO_IN_ENDP_INTERVAL
#define HID_BINTERVAL               HID_FS_BINTERVAL
#define INTERRUPT_IN_INTERVAL       FS_INTERRUPT_IN_INTERVAL
#endif	//USB_SPEED_CFG

#if(SUPPORT_USB_SPK)

#if(SPK_AUDIO_24BIT && USB_SPEED_CFG == USB_SPEED_HIGH)
#if(SPK_FS_176P4K)
#define SUPPORT_SPK_176P4KX24BIT      	//ONLY HIGH SPEED SUPPORT
#endif
#if(SPK_FS_192K)
#define SUPPORT_SPK_192KX24BIT      	//ONLY HIGH SPEED SUPPORT
#endif
#endif

	#if(SPK_AUDIO_16BIT)
	#define SPK_16BIT_FS_NUM  ( SPK_FS_8K +\
		SPK_FS_16K + \
		SPK_FS_32K +\
		SPK_FS_44P1K +\
		SPK_FS_48K +\
		SPK_FS_88P2K +\
		SPK_FS_96K  +\
		SPK_FS_176P4K +\
		SPK_FS_192K )
	#define SPK_16BIT_DESC_SIZE	    	40 + SPK_16BIT_FS_NUM*3 + SPK_FEEDBACK_DESC_SIZE
	#else
	#define SPK_16BIT_FS_NUM    0
	#define SPK_16BIT_DESC_SIZE	0
	#endif	//SPK_AUDIO_16BIT

	#if(SPK_AUDIO_24BIT)
	#define SPK_24BIT_FS_NUM  ( SPK_FS_44P1K +\
		SPK_FS_48K +\
		SPK_FS_88P2K +\
		SPK_FS_96K  )
	#define SPK_24BIT_DESC_SIZE	    	40 + SPK_24BIT_FS_NUM *3 + SPK_FEEDBACK_DESC_SIZE
	#else	//SPK_AUDIO_24BIT
	#define SPK_24BIT_FS_NUM      0
	#define SPK_24BIT_DESC_SIZE	0
	#endif	//SPK_AUDIO_24BIT

	#if defined(SUPPORT_SPK_192KX24BIT) && defined(SUPPORT_SPK_176P4KX24BIT)
	#define SPK_24BIT_HS_NUM			2
	#define SPK_24BIT_HS_DESC_SIZE 		40 + 6 + SPK_FEEDBACK_DESC_SIZE
	#elif defined(SUPPORT_SPK_192KX24BIT) || defined(SUPPORT_SPK_176P4KX24BIT)
	#define SPK_24BIT_HS_NUM			1
	#define SPK_24BIT_HS_DESC_SIZE 		40 + 3 + SPK_FEEDBACK_DESC_SIZE
	#else
	#define SPK_24BIT_HS_NUM			0
	#define SPK_24BIT_HS_DESC_SIZE 		0
	#endif

#define SPK_MULT_FS_NUM            ( SPK_16BIT_FS_NUM + SPK_24BIT_FS_NUM + SPK_24BIT_HS_NUM )

#define SPK_AUDIO_INTERFACE_BYTES  ( DESC_SIZE_INTF + \
	SPK_16BIT_DESC_SIZE + \
	SPK_24BIT_DESC_SIZE + \
	SPK_24BIT_HS_DESC_SIZE)

#else	//SUPPORT_USB_SPK
#define SPK_CONTROL_INTERFACE_BYTES  	0
#define SPK_AUDIO_INTERFACE_BYTES    	0
#define SPK_MULT_FS_NUM                	0
#endif	//SUPPORT_USB_SPK

#if(SUPPORT_USB_MIC)

#if(MIC_AUDIO_24BIT && USB_SPEED_CFG == USB_SPEED_HIGH)
#if(MIC_FS_176P4K)
#define SUPPORT_MIC_176P4KX24BIT     	//ONLY HIGH SPEED SUPPORT
#endif
#if(MIC_FS_192K)
#define SUPPORT_MIC_192KX24BIT      	//ONLY HIGH SPEED SUPPORT
#endif
#endif

	#if MIC_AUDIO_16BIT
	#define MIC_16BIT_FS_NUM      ( MIC_FS_8K +\
		MIC_FS_16K + \
		MIC_FS_32K +\
		MIC_FS_44P1K +\
		MIC_FS_48K +\
		MIC_FS_88P2K +\
		MIC_FS_96K	+\
		MIC_FS_176P4K +\
		MIC_FS_192K )
	#define MIC_16BIT_DESC_SIZE	    	40 + MIC_16BIT_FS_NUM*3
	#else
	#define MIC_16BIT_DESC_SIZE	    	0
	#define MIC_16BIT_FS_NUM        	0
	#endif	//MIC_AUDIO_16BIT

	#if MIC_AUDIO_24BIT
	#define MIC_24BIT_FS_NUM      ( MIC_FS_44P1K +\
		MIC_FS_48K +\
		MIC_FS_88P2K +\
		MIC_FS_96K )
	#define MIC_24BIT_DESC_SIZE	        40 + MIC_24BIT_FS_NUM *3
	#else
	#define MIC_24BIT_DESC_SIZE	        0
	#define MIC_24BIT_FS_NUM            0
	#endif	//MIC_AUDIO_24BIT

	#if defined(SUPPORT_MIC_192KX24BIT) && defined(SUPPORT_MIC_176P4KX24BIT)
	#define MIC_24BIT_HS_NUM			2
	#define MIC_24BIT_HS_DESC_SIZE 		40 + 6
	#elif defined(SUPPORT_MIC_192KX24BIT) || defined(SUPPORT_MIC_176P4KX24BIT)
	#define MIC_24BIT_HS_NUM			1
	#define MIC_24BIT_HS_DESC_SIZE 		40 + 3
	#else
	#define MIC_24BIT_HS_NUM			0
	#define MIC_24BIT_HS_DESC_SIZE 		0
	#endif


#define MIC_MULT_FS_NUM            ( MIC_24BIT_FS_NUM + MIC_16BIT_FS_NUM + MIC_24BIT_HS_NUM)

#define MIC_AUDIO_INTERFACE_BYTES  ( DESC_SIZE_INTF + \
    MIC_16BIT_DESC_SIZE + \
    MIC_24BIT_DESC_SIZE + \
    MIC_24BIT_HS_DESC_SIZE)
#if(USB_MIC_CHANNELS > 1)
#define MIC_AC_INTF_FU_DESC_SIZE		(DESC_SIZE_AC_INTF_FU_10 + 2)
#else
#define MIC_AC_INTF_FU_DESC_SIZE		(DESC_SIZE_AC_INTF_FU_10 + 1)
#endif

#define MIC_CONTROL_INTERFACE_BYTES   (DESC_SIZE_AC_INTF_IT_10 + \
		DESC_SIZE_AC_INTF_OT_10 + \
		MIC_AC_INTF_FU_DESC_SIZE )
#else	//SUPPORT_USB_MIC
#define MIC_MULT_FS_NUM               0
#define MIC_CONTROL_INTERFACE_BYTES   0
#define MIC_AUDIO_INTERFACE_BYTES     0
#endif	//SUPPORT_USB_MIC

#if (SUPPORT_USB_MIC && SUPPORT_USB_SPK)
#define MULT_FS_SUPPORT       (((MIC_MULT_FS_NUM + SPK_MULT_FS_NUM) >= 3) ? 1:0 )
#elif (SUPPORT_USB_MIC || SUPPORT_USB_SPK)
#define MULT_FS_SUPPORT       (((MIC_MULT_FS_NUM + SPK_MULT_FS_NUM) >= 2) ? 1:0 )
#else
#define MULT_FS_SUPPORT       0
#endif

#if(SUPPORT_SPK_SIDETONE == 1)
#define SIDETONE_CONTROL_INTERFACE_BYTES  0x16
#else
#define SIDETONE_CONTROL_INTERFACE_BYTES  0
#endif

#if SUPPORT_USB_HID
#define HIDKEY_DESC_SIZ                               32
#else
#define HIDKEY_DESC_SIZ                               0
#endif

#if (USB_AUDIO_NUM  > 0)
#define USB_AUDIO_INTERFACE_HEAD_DESC_BYTES             (DESC_SIZE_AC_INTF_H_10 + USB_AUDIO_NUM)
#elif (USB_AUDIO_NUM == 0 )
#define USB_AUDIO_INTERFACE_HEAD_DESC_BYTES             0
#else
##########"Err!"
#endif	//USB_AUDIO_NUM

#if(SUPPORT_USB_SPK)
#define SPK_AC_INTF_FU_DESC_SIZE			(DESC_SIZE_AC_INTF_FU_10 + USB_SPK_CHANNELS)
#define SPK_CONTROL_INTERFACE_BYTES  				(DESC_SIZE_AC_INTF_IT_10 + \
		DESC_SIZE_AC_INTF_OT_10 + \
		SPK_AC_INTF_FU_DESC_SIZE )
#else	//SUPPORT_USB_SPK
#define	SPK_CONTROL_INTERFACE_BYTES					0
#endif	//SUPPORT_USB_SPK

#define AUDIO_AC_INTF_DESC_BYTES       ( USB_AUDIO_INTERFACE_HEAD_DESC_BYTES + \
	DESC_SIZE_AC_INTF_SU + \
    SPK_CONTROL_INTERFACE_BYTES +\
    MIC_CONTROL_INTERFACE_BYTES +\
    SIDETONE_CONTROL_INTERFACE_BYTES \
    )
#if (SUPPORT_USB_SPK ||SUPPORT_USB_MIC)
#define USB_AUDIO_INTERFACE_DESC_BYTES                 (DESC_SIZE_INTF + \
		DESC_SIZE_AC_INTF_SU + \
		USB_AUDIO_INTERFACE_HEAD_DESC_BYTES + \
		SPK_CONTROL_INTERFACE_BYTES + \
		MIC_CONTROL_INTERFACE_BYTES + \
		SIDETONE_CONTROL_INTERFACE_BYTES )
#else
#define USB_AUDIO_INTERFACE_DESC_BYTES                 0
#endif



#if(SUPPORT_UAC20)

#if SUPPORT_USB_HID
#define SUPPORT_HID_KEYBOARD_DES_LENGTH    32
#else
#define SUPPORT_HID_KEYBOARD_DES_LENGTH    0
#endif

#if MIC_AUDIO_16BIT
#define SUPPORT_MIC_16BIT_DES_LENGTH                 46
#else
#define SUPPORT_MIC_16BIT_DES_LENGTH                 0
#endif

#if MIC_AUDIO_24BIT
#define SUPPORT_MIC_24BIT_DES_LENGTH                 46
#else
#define SUPPORT_MIC_24BIT_DES_LENGTH                 0
#endif

#if SUPPORT_USB_MIC
#define UAC_MIC_SUPPORT_SAMPLE_RATE_NUM    (MIC_FS_8K     +  \
										    MIC_FS_16K    +	 \
										    MIC_FS_32K    +	 \
										    MIC_FS_44P1K  +	 \
										    MIC_FS_48K    +  \
										    MIC_FS_88P2K  +  \
										    MIC_FS_96K    +  \
										    MIC_FS_176P4K +  \
										    MIC_FS_192K)
#else
#define UAC_MIC_SUPPORT_SAMPLE_RATE_NUM     0
#endif
#define UAC_MIC_SUPPORT_SAMPLE_RATE_LIST_LEN      (UAC_MIC_SUPPORT_SAMPLE_RATE_NUM * 12 + 2)
#if SUPPORT_USB_MIC
#if SUPPORT_USB_SPK
#define MIC_TRACK_DES_LENGTH                          60
#else
#define MIC_TRACK_DES_LENGTH                          59
#endif
#define MIC_DES_LENGTH                               (9                           + \
												    MIC_TRACK_DES_LENGTH        + \
												    SUPPORT_MIC_16BIT_DES_LENGTH +\
												    SUPPORT_MIC_24BIT_DES_LENGTH )
#else
#define MIC_TRACK_DES_LENGTH                          0
#define MIC_DES_LENGTH                                0
#endif
#if SPK_AUDIO_16BIT
#define SUPPORT_SPK_16BIT_DES_LENGTH                 46
#else
#define SUPPORT_SPK_16BIT_DES_LENGTH                 0
#endif
#if (SPK_AUDIO_24BIT)
#define SUPPORT_SPK_24BIT_DES_LENGTH                 46
#else
#define SUPPORT_SPK_24BIT_DES_LENGTH                 0
#endif


#if SPK_AUDIO_32BIT
#define SUPPORT_SPK_32BIT_DES_LENGTH                 46
#else
#define SUPPORT_SPK_32BIT_DES_LENGTH                 0
#endif

#define SPEAKER_FS_384K    0
#define SPEAKER_FS_352K8   0
#if SUPPORT_USB_SPK
#define USB_SPKER_SUPPORT_SAMPLE_RATE_NUM       (SPK_FS_8K     + \
											     SPK_FS_16K    + \
											     SPK_FS_32K    + \
											     SPK_FS_44P1K  + \
											     SPK_FS_48K    + \
											     SPK_FS_88P2K  + \
											     SPK_FS_96K    + \
											     SPK_FS_176P4K + \
											     SPK_FS_192K   + \
											     SPEAKER_FS_352K8  + \
											     SPEAKER_FS_384K )
#else
#define USB_SPKER_SUPPORT_SAMPLE_RATE_NUM        0
#endif

#define USB_SPKER_SUPPORT_SAMPLE_RATE_LIST_LEN      (USB_SPKER_SUPPORT_SAMPLE_RATE_NUM*12 + 2)
#if SUPPORT_USB_SPK
#if SUPPORT_USB_MIC
#define SPEAKER_TRACK_DES_LENGTH                     64
#else
#define SPEAKER_TRACK_DES_LENGTH                     63
#endif
#define SPEAKER_DES_LENGTH                          (9                            + \
												     SPEAKER_TRACK_DES_LENGTH     + \
												     SUPPORT_SPK_16BIT_DES_LENGTH + \
												     SUPPORT_SPK_24BIT_DES_LENGTH + \
												     SUPPORT_SPK_32BIT_DES_LENGTH )
#else
#define SPEAKER_TRACK_DES_LENGTH                     0
#define SPEAKER_DES_LENGTH                           0
#endif
#if (SUPPORT_USB_MIC||SUPPORT_USB_SPK)
#define AUDIO_HEAD_DES_LENGTH                        (DESC_SIZE_INTF_ASSOC +\
		DESC_SIZE_INTF +\
		DESC_SIZE_AC_INTF_H_20)
#else
#define AUDIO_HEAD_DES_LENGTH                        0
#endif

#if(SUPPORT_SPK_SIDETONE)
#define UAC20_SIDETONE_DES_LENGTH					30
#else
#define UAC20_SIDETONE_DES_LENGTH					0
#endif

#define USB_AUDIO20_CONFIG_DESC_SIZ                 (DESC_SIZE_CONFIG     + \
												    AUDIO_HEAD_DES_LENGTH            + \
												    SPEAKER_DES_LENGTH               + \
												    MIC_DES_LENGTH                   + \
												    UAC20_SIDETONE_DES_LENGTH		+ \
												    SUPPORT_HID_KEYBOARD_DES_LENGTH )
//#else ///SUPPORT_UAC20
#endif//SUPPORT_UAC20

//#endif //SUPPORT_UAC20
//#define USB_AUDIO_CONFIG_DESC_SIZ 					(192 + HIDKEY_DESC_SIZ + HIDCOMPROTOCOL_DESC_SIZ )
#define USB_AUDIO_CONFIG_DESC_SIZ                     ( DESC_SIZE_CONFIG + \
    USB_AUDIO_INTERFACE_DESC_BYTES + \
    HIDKEY_DESC_SIZ  + \
    SPK_AUDIO_INTERFACE_BYTES   + \
    MIC_AUDIO_INTERFACE_BYTES  \
    )

#if(TEST_USB_SWITCH || SUPPORT_UAC_SELF_ADAPTION)
#define USB_FS_AUDIO_CONFIG_DESC_SIZ 				(DESC_SIZE_CONFIG +\
		USB_AUDIO_INTERFACE_DESC_BYTES+\
		HIDKEY_DESC_SIZ+\
		0x09 + (40+5*3)*2 +\
		0x09 + 40+3*3)
#endif

#define USB_AUDIO_OTHERSPEED_CONFIG_DESC_SIZ           (221 + HIDKEY_DESC_SIZ)

#define AUDIO_DESCRIPTOR_TYPE                         0x21U

/* Audio Control Interface Descriptor Subtypes */
#define AUDIO_CONTROL_MUTE                            0x0001U
#define AUDIO_CONTROL_VOL                             0x0002U

/* Audio Requests */
#define AUDIO_REQ_GET_CUR                             0x81U
#define AUDIO_REQ_GET_MIN                             0x82U
#define AUDIO_REQ_GET_MAX                             0x83U
#define AUDIO_REQ_GET_RES                             0x84U
#define AUDIO_REQ_SET_CUR                             0x01U
#define AUDIO_REQ_SET_MIN                             0x02U
#define AUDIO_REQ_SET_MAX                             0x03U
#define AUDIO_REQ_SET_RES                             0x04U


#define AUDIO_OUT_STREAMING_CTRL                      0x02U

/* Audio Control Requests */
#define AUDIO_CONTROL_REQ                             0x01U
/* Feature Unit, UAC Spec 1.0 p.102 */
#define AUDIO_CONTROL_REQ_FU_MUTE                     0x01U
#define AUDIO_CONTROL_REQ_FU_VOL                      0x02U

#define AUDIO_CONTROL_REQ_FU_RES                      0x04U
/* Audio Streaming Requests */
#define AUDIO_STREAMING_REQ                           0x02U
#define AUDIO_STREAMING_REQ_FREQ_CTRL                 0x01U
#define AUDIO_STREAMING_REQ_PITCH_CTRL                0x02U
#define AUDIO_AUTOMATIC_GAIN_CTRL                     0x07U

#define AUDIO_CONTROL_REQ_MIC_VOL                     0x05
#define AUDIO_CONTROL_REQ_SPKER_VOL                   0x06
#define AUDIO_CONTROL_REQ_SIDETONE_VOL                0x07
#define AUDIO_CONTROL_REQ_MIXER                       0x08

/* ---- Generator Interface alternate index ---- */
enum    {
    AS_INTF_ALT_OFFLINE = 0,
    AS_INTF_ALT_16BIT,
#if SUPPORT_USB_24BIT_ONLY
    AS_INTF_ALT_24BIT = AS_INTF_ALT_16BIT,
#else
	AS_INTF_ALT_24BIT,
#endif
#if(SPK_24BIT_HS_NUM || MIC_24BIT_HS_NUM)
    AS_INTF_ALT_24BIT_HS,
#endif
#if SUPPORT_UAC20
    AS_INTF_ALT_32BIT = AS_INTF_ALT_24BIT + 1,
#endif
};


/* ----- terminal/unit IDs ----- */
#define TERM_ID_SPK_IN 	         							1
#define FU_ID_SPK                							6
#define TERM_ID_SPK_OUT             						3

#define TERM_ID_MIC_IN            							4
#define FU_ID_MIC              								5
#define TERM_ID_MIC_OUT           							2

#define FU_MIXER_ID											7
#define MU_ID												8
#define SELECTOR_ID											0x21

/* ----- interface indexes ----- */
enum	{
    INTF_AUDIO_CONTROL = 0,
#if SUPPORT_USB_SPK
    INTF_AUDIO_STREAM_SPK = 1,
#else
    INTF_AUDIO_STREAM_SPK = 0,
#endif
#if SUPPORT_USB_MIC
    INTF_AUDIO_STREAM_MIC = 1 + INTF_AUDIO_STREAM_SPK,
#else
    INTF_AUDIO_STREAM_MIC = INTF_AUDIO_STREAM_SPK,
#endif
#if SUPPORT_USB_HID
    INTF_HID = 1 + SUPPORT_USB_SPK + SUPPORT_USB_MIC,
#endif

    USB_INTERFACE_COUNT 							// DON'T EDIT THIS LINE
};

/* USB standard descriptor endpoint bmAttributes */
#define USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK (0x80U)
#define USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT (7U)
#define USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT (0U)
#define USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN (0x80U)

#define EP_INPUT    (USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN)
#define EP_OUTPUT   (USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT)
#define USB_SHORT_GET_LOW(x) (((uint16_t)x) & 0xFFU)
#define USB_SHORT_GET_HIGH(x) ((uint8_t)(((uint16_t)x) >> 8U) & 0xFFU)

/* ----- endpoint indexes ----- */

enum	{
    USB_AUDIO_SPK_CONTROL_ENDPOINT		= 1,
    USB_AUDIO_SPK_STREAM_ENDPOINT		= 1,
    USB_AUDIO_SPK_FEEDBACK_ENDPOINT 	= 1,
#if  (SUPPORT_USB_SPK == 0)
    USB_AUDIO_GENERATOR_STREAM_ENDPOINT 	= 1,
#else
    USB_AUDIO_GENERATOR_STREAM_ENDPOINT 	= 1 + SUPPORT_USB_SPK,
#endif
    USB_HID_HID_DEVICE_ENDPOINT				= 1 + SUPPORT_USB_SPK + SUPPORT_USB_MIC,
};

/* bEndpointAddress, see UAC 1.0 spec, p.61 */
#define AUDIO_CTRL_EP              (USB_AUDIO_SPK_CONTROL_ENDPOINT  | EP_INPUT)
#define AUDIO_OUT_EP               (USB_AUDIO_SPK_STREAM_ENDPOINT | EP_OUTPUT)
#define AUDIO_FB_EP                (USB_AUDIO_SPK_FEEDBACK_ENDPOINT | EP_INPUT)
#define AUDIO_IN_EP               (USB_AUDIO_GENERATOR_STREAM_ENDPOINT | EP_INPUT)
#define AUDIO_HID_IN_EP            (USB_HID_HID_DEVICE_ENDPOINT | EP_INPUT)
#define AUDIO_HID_OUT_EP           (USB_HID_HID_DEVICE_ENDPOINT | EP_OUTPUT)

// clang-format off
#define AUDIO_SAMPLE_FREQ(frq) (uint8_t)(frq), (uint8_t)((frq >> 8)), (uint8_t)((frq >> 16))

/* Feature Unit Config */
#define AUDIO_CONTROL_FEATURES AUDIO_CONTROL_MUTE | AUDIO_CONTROL_VOL

#define USB_HID_KEYBOARD_ENDPOINT_COUNT             (2U)

#define HS_HID_KEYBOARD_INTERRUPT_IN_PACKET_SIZE    (16U)
#define FS_HID_KEYBOARD_INTERRUPT_IN_PACKET_SIZE    (16U)
#define HS_HID_KEYBOARD_INTERRUPT_IN_INTERVAL       (0x06U)
#define FS_HID_KEYBOARD_INTERRUPT_IN_INTERVAL       (0x03)

#if (USB_SPEED_CFG == USB_SPEED_FULL)
#if(SPK_FS_176P4K || SPK_FS_192K)
#define UAC10_OUT_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_192000/1000 * USB_SPK_CHANNELS * 2 )
#define UAC10_OUT_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_96000/1000 * USB_SPK_CHANNELS * 3 )
#elif(SPK_FS_88P2K || SPK_FS_96K)
#define UAC10_OUT_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_96000/1000 * USB_SPK_CHANNELS * 2 )
#define UAC10_OUT_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_96000/1000 * USB_SPK_CHANNELS * 3 )
#elif(SPK_FS_44P1K || SPK_FS_48K)
#define UAC10_OUT_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_48000/1000 * USB_SPK_CHANNELS * 2 )
#define UAC10_OUT_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_48000/1000 * USB_SPK_CHANNELS * 3 )
#else
#define UAC10_OUT_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_16000/1000 * USB_SPK_CHANNELS * 2 )
#define UAC10_OUT_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_16000/1000 * USB_SPK_CHANNELS * 3 )
#endif

#if(MIC_FS_176P4K || MIC_FS_192K)
#define UAC10_IN_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_192000/1000 * USB_MIC_CHANNELS * 2 )
#define UAC10_IN_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_96000/1000 * USB_MIC_CHANNELS * 3 )
#elif(MIC_FS_88P2K || MIC_FS_96K)
#define UAC10_IN_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_96000/1000 * USB_MIC_CHANNELS * 2 )
#define UAC10_IN_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_96000/1000 * USB_MIC_CHANNELS * 3 )
#elif(MIC_FS_44P1K || MIC_FS_48K)
#define UAC10_IN_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_48000/1000 * USB_MIC_CHANNELS * 2 )
#define UAC10_IN_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_48000/1000 * USB_MIC_CHANNELS * 3 )
#else
#define UAC10_IN_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_16000/1000 * USB_MIC_CHANNELS * 2 )
#define UAC10_IN_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_16000/1000 * USB_MIC_CHANNELS * 3 )
#endif

#else //!USB_SPEED_FULL
#if(SPK_FS_176P4K || SPK_FS_192K)
#define UAC10_OUT_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_192000/1000 * USB_SPK_CHANNELS * 2 + 2*20)
#define UAC10_OUT_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_96000/1000  * USB_SPK_CHANNELS * 3 + 3*20)
#elif(SPK_FS_88P2K || SPK_FS_96K)
#define UAC10_OUT_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_96000/1000 * USB_SPK_CHANNELS * 2 + 2*20)
#define UAC10_OUT_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_96000/1000  * USB_SPK_CHANNELS * 3 + 3*20)
#elif(SPK_FS_44P1K || SPK_FS_48K)
#define UAC10_OUT_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_48000/1000 * USB_SPK_CHANNELS * 2 + 2*20)
#define UAC10_OUT_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_48000/1000  * USB_SPK_CHANNELS * 3 + 3*20)
#else
#define UAC10_OUT_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_16000/1000 * USB_SPK_CHANNELS * 2 + 2*20)
#define UAC10_OUT_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_16000/1000  * USB_SPK_CHANNELS * 3 + 3*20)
#endif

#if(MIC_FS_176P4K || MIC_FS_192K)
#define UAC10_IN_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_192000/1000 * USB_MIC_CHANNELS * 2 + 2*20)
#define UAC10_IN_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_96000/1000 * USB_MIC_CHANNELS * 3 + 3*20)
#elif(MIC_FS_88P2K || MIC_FS_96K)
#define UAC10_IN_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_96000/1000 * USB_MIC_CHANNELS * 2 + 2*20)
#define UAC10_IN_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_96000/1000 * USB_MIC_CHANNELS * 3 + 3*20)
#elif(MIC_FS_44P1K || MIC_FS_48K)
#define UAC10_IN_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_48000/1000 * USB_MIC_CHANNELS * 2 + 2*20)
#define UAC10_IN_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_48000/1000 * USB_MIC_CHANNELS * 3 + 3*20)
#else
#define UAC10_IN_EP_MAX_PKT_SIZE_16BIT      (SAMPLING_RATE_16000/1000 * USB_MIC_CHANNELS * 2 + 2*20)
#define UAC10_IN_EP_MAX_PKT_SIZE_24BIT      (SAMPLING_RATE_16000/1000 * USB_MIC_CHANNELS * 3 + 3*20)
#endif

#if(SUPPORT_UAC20)
#define UAC20_OUT_EP_MAX_PKT_SIZE_16BIT		(UAC10_OUT_EP_MAX_PKT_SIZE_16BIT/8)
#define UAC20_IN_EP_MAX_PKT_SIZE_16BIT			(UAC10_IN_EP_MAX_PKT_SIZE_16BIT/8)
#if(SPK_FS_176P4K || SPK_FS_192K)
#define UAC20_OUT_EP_MAX_PKT_SIZE_24BIT		((SAMPLING_RATE_192000/1000  * USB_SPK_CHANNELS * 3 + 3*20)/8)
#else
#define UAC20_OUT_EP_MAX_PKT_SIZE_24BIT		(UAC10_OUT_EP_MAX_PKT_SIZE_24BIT/8)
#endif
#if(MIC_FS_176P4K || MIC_FS_192K)
#define UAC20_IN_EP_MAX_PKT_SIZE_24BIT			((SAMPLING_RATE_192000/1000  * USB_MIC_CHANNELS * 3 + 3*20)/8)
#else
#define UAC20_IN_EP_MAX_PKT_SIZE_24BIT			(UAC10_IN_EP_MAX_PKT_SIZE_24BIT/8)
#endif
#endif

#endif // USB_SPEED_FULL

#define USB_DEV_HID_BDESCRIPTORTYPE                 0x0A
#define SPKER_CLOCK_SOURCE_BCLK_ID                  0x09
#define SPKER_CLOCK_SELECTOR_BCK_ID                 0x0B
#define MIC_CLOCK_SOURCE_BCLK_ID                    0x0A
#define MIC_CLOCK_SELECTOR_BCK_ID                   0x0C
#define USB_DEV_SPKER_VOLUME_RANGE_ID               0x02
#define USB_DEV_MIC_VOLUME_RANGE_ID                 0x05
#define USB_DEV_MIXER_VOLUME_RANGE_ID               0x07
#define SPKER_CLOCK_SELECTOR_BICK_INDEX             0x01

#if SUPPORT_USB_SPK
#define MIC_CLOCK_SELECTOR_BICK_INDEX               0x02
#else
#define MIC_CLOCK_SELECTOR_BICK_INDEX               0x01
#endif

#define SPKER_CLOCK_BICK_NUM                        0x01
#define MIC_CLOCK_BICK_NUM                          0x01
#define SPKER_SAMPLING_FREQUENCY_LEN                0x04
#define MIC_SAMPLING_FREQUENCY_LEN                  0x04

#define USB_DEV_SPKER_VOLUME_RANGE_LEFT_ID          0x01
#define USB_DEV_SPKER_VOLUME_RANGE_RIGHT_ID         0x02
#define AUDIO_REQ_CUR                               0x01
#define AUDIO_REQ_RANGE                             0x02

#define AUDIO_SAM_FREQ_AND_CLK_SELECT               0x01
#define UAC_SPKER_SAMPLE_RATE_NUM                   0x02
#define UAC_MIC_SAMPLE_RATE_NUM                     0x02
#define USB_DEVICE_AUDIO_SET_REQUEST_INTERFACE      (0x21)
#define USB_DEVICE_AUDIO_SET_REQUEST_ENDPOINT       (0x22)

#define USB_DEVICE_AUDIO_GET_REQUEST_INTERFACE      (0xA1)
#define USB_DEVICE_AUDIO_GET_REQUEST_ENDPOINT       (0xA2)


#define AUDIO_SPKER_REQ_FREQ_CTRL                     0x01U
#define AUDIO_MIC_REQ_FREQ_CTRL                       0x02U

#ifdef __cplusplus
}
#endif

#endif  /* __USB_CONFIG_H__ */

