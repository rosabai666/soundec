/*
 * usb_descriptor.c
 *
 *  Created on: 2023Äê2ÔÂ17ÈÕ
 *      Author: User
 */


/* Includes ------------------------------------------------------------------*/
#include "usb_descriptor.h"
#include "usb_desc.h"

#if (USB_ENABLE)

/* Private macro -------------------------------------------------------------*/
uint8_t g_UsbDeviceHidKeyboardReportDescriptor[HID_KEYBOARD_DESC_SIZE] =
{
	0x05, 0x0C, // Usage Page (Consumer)
	0x09, 0x01, // Usage(Consumer Control)
	0xA1, 0x01, // Collection(Application )
	/***************************************************/
	0x85, 0x01,
	0x09, 0x04,
	0x15, 0x00,
	0x26, 0xFF, 0x00,
	0x75, 0x08,
	0x95, 0x3F,
	0x91, 0x82,
	/***************************************************/
	0x85, 0x01,
	0x15, 0x00, // Logical Minimum(0x0 )
	0x26, 0xFF, 0x00,
	//0x25, 0x01, // Logical Maximum(0x1 )
	0x75, 0x01, // Report Size(0x1 )
	0x95, 0x01, // Report Count(0x5 )

	0x09, 0xE9, // Usage(Volume Increment)
	0x81, 0x02,

	0x09, 0xEA, // Usage(Volume Decrement)
	0x81, 0x02, // Input(Data, Variable, Absolute, No Wrap, Linear, Preferred State, No Null Position, Bit Field)

	0x09, 0xCD, // Usage(Play/Pause)
	0x81, 0x02,

	0x09, 0xB5, // Usage(Scan Next Track)
	0x81, 0x02,

	0x09, 0xB6, // Usage(Scan Previous Track)
	0x81, 0x02,

	0x09, 0xB7, // Usage(Stop)
	0x81, 0x02,

	0x09, 0xB3, // Usage(Fast Forward)
	0x81, 0x02,

	0x09, 0xB4, // Usage(Rewind)
	0x81, 0x02, // Input(Data, Variable, Absolute, No Wrap, Linear, Preferred State, No Null Position, Bit Field)

	0x05, 0x0B,
	0x09, 0x24,
	0x81, 0x02,

	0x09, 0x20,
	0x81, 0x02, // Input(Data, Variable, Absolute, No Wrap, Linear, Preferred State, No Null Position, Bit Field)

	0x09, 0x2F,
	0x81, 0x06,
	0x95, 0x05, // Report Count(0x5 )
	0x81, 0x01,

	0x85, 0x02,
	0x09, 0x03,
	0x15, 0x00,
	0x26, 0xFF, 0x00,
	0x75, 0x08,
	0x95, 0x3F,
	0x81, 0x02,

	0xC0
};
#define HID_COMMP_DESC_SIZE   (50)

uint8_t g_UsbDeviceHidCommPReportDescriptor[HID_COMMP_DESC_SIZE] =
{
    //0x06, 0x00, 0xFF,
    0x05, 0x0C,       /*Usage Page( Vendor-Defined 11)*/
    0x09, 0x01,       /*Usage (Vendor-Defined 1)*/
    0xA1, 0x01,


    0x85, 0x02,
    0x09, 0x03,
    0x15, 0x00,
    0x26, 0xFF, 0x00,
    0x75, 0x08,
    0x95, 0x3f,
    0x81, 0x02,

    0x85, 0x01,
    0x09, 0x04,
    0x15, 0x00,
    0x26, 0xFF, 0x00,
    0x75, 0x08,
    0x95, 0x3f,
    0x91, 0x82,

    0x09,0x05,
    0x15,0x00,
    0x26,0xFF,0x00,
    0x75,0x08,
    0x95,0x3f,
    0xB1,0x02,

    0xC0,
};

#define DESC_SIZE_HID_KEYBOARD_REPORT (sizeof(g_UsbDeviceHidKeyboardReportDescriptor))

uint8_t uac_CfgDesc[USB_AUDIO_CONFIG_DESC_SIZ] __attribute__ ((aligned (4))) =
{
	/* -- Configuration ---------------------------*/
#if(SUPPORT_USB_LPM)
	DESC_CONFIG(USB_AUDIO_CONFIG_DESC_SIZ, USB_FUN_NUM_INTERFACES, 1, 0, (BUS_POWERED | REMOTE_WAKEUP), 100),
#else
	DESC_CONFIG(USB_AUDIO_CONFIG_DESC_SIZ, USB_FUN_NUM_INTERFACES, 1, 0, BUS_POWERED, 100),
#endif

#if(SUPPORT_USB_SPK || SUPPORT_USB_MIC)  //18byte
	/* -- AudioControl Interface ---------------------------*/
	DESC_INTF(0, 0, 0, AUDIO_CLASS, AUDIO_SUBCLASS_CONTROL, 0, 0),

	// Header
	DESC_AC_INTF_H_10(USB_AUDIO_NUM, AUDIO_AC_INTF_DESC_BYTES),
#if (USB_AUDIO_NUM >1 )
	DESC_AC_INTF_H_10_INCOLLECTION_2,
#else
	DESC_AC_INTF_H_10_INCOLLECTION_1,
#endif

#endif // (SUPPORT_USB_SPK||SUPPORT_USB_MIC)

#if SUPPORT_USB_SPK
 /*
 *	AUDIO CONTROL INTERFACE : SPK
 *		- TERM_ID_SPK_IN    :1
 *		- FU_ID_SPK       :2
 *		- TERM_ID_SPK_OUT :3
 */
	// SPK IT
	DESC_AC_INTF_IT(TERM_ID_SPK_IN, USB_TERMINAL_USB_STREAMING, 0x00, USB_SPK_CHANNELS, USB_SPK_CHANNEL_SEL),

	// SPK FU
#if(SUPPORT_SPK_SIDETONE)
	DESC_AC_INTF_FU(USB_SPK_CHANNELS, FU_ID_SPK, MU_ID, USB_SPK_CONTROL_MASTER, USB_SPK_CONTROL_CH),
#else
	DESC_AC_INTF_FU(USB_SPK_CHANNELS, FU_ID_SPK, TERM_ID_SPK_IN, USB_SPK_CONTROL_MASTER, USB_SPK_CONTROL_CH),
#endif

	// SPK OT
	DESC_AC_INTF_OT(TERM_ID_SPK_OUT, OUTPUT_TERMINAL_HEADPHONE, 0x00, FU_ID_SPK),
#endif // SUPPORT_USB_SPK

#if SUPPORT_USB_MIC  /* Size = 0x1E */
/*
 *	AUDIO CONTROL INTERFACE DESCRIPTOR:	MIC
 *		- TERM_ID_MIC_IN
 *		- FU_ID_MIC
 *		- TERM_ID_MIC_OUT
 */
	// MIC IT
	DESC_AC_INTF_IT(TERM_ID_MIC_IN, INPUT_TERMINAL_MICROPHONE, 0x00, USB_MIC_CHANNELS, USB_MIC_CHANNEL_SEL),

	// MIC FU
	DESC_AC_INTF_FU(USB_MIC_CHANNELS, FU_ID_MIC, TERM_ID_MIC_IN, USB_MIC_CONTROL_MASTER, USB_MIC_CONTROL_CH),

	// MIC OT
	DESC_AC_INTF_OT(TERM_ID_MIC_OUT, USB_TERMINAL_USB_STREAMING, 0x00, SELECTOR_ID),

	//Selector
	0x07, 0x24, 0x05, SELECTOR_ID, 0x01, FU_ID_MIC, 0x00,

#if (SUPPORT_SPK_SIDETONE == 1)
	//Mixer
	0x09, 0x24, 0x06, FU_MIXER_ID, TERM_ID_MIC_IN, 0x01, 0x03, 0x00, 0x00,
	0x0D, 0x24, 0x04, MU_ID, 0x02, TERM_ID_SPK_IN, FU_MIXER_ID, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00,
#endif //SUPPORT_SPK_SIDETONE
#endif // SUPPORT_USB_MIC

#if SUPPORT_USB_SPK
/******************************* Interface 1, alternate 0 ********************************************/
/*
 *	INTERFACE DESCRIPTOR:	Audio Streaming (Speaker)
 */

	/* USB Speaker Standard AS Interface Descriptor - Audio Streaming Zero Bandwith */
	DESC_INTF(INTF_AUDIO_STREAM_SPK, AS_INTF_ALT_OFFLINE, 0, AUDIO_CLASS, AUDIO_SUBCLASS_STREAMING, 0, 0),

#if SPK_AUDIO_16BIT
/******************************* Interface 1, alternate 1 ********************************************/
/*
 *	INTERFACE DESCRIPTOR:	Audio Streaming (Speaker)
 */

	/* USB Speaker Standard AS Interface Descriptor - Audio Streaming Operational */
	DESC_INTF(INTF_AUDIO_STREAM_SPK, AS_INTF_ALT_16BIT, SPK_ENDPOINT_NUM, AUDIO_CLASS, AUDIO_SUBCLASS_STREAMING, 0, 0),

	DESC_AS_INTF_GEN(TERM_ID_SPK_IN, 0x01),

	DESC_AS_INTF_FMT(SPK_16BIT_FS_NUM, USB_SPK_CHANNELS, AUDIO_16BIT_FRAMESIZE, AUDIO_16BIT_BIT_RESOLUTION),

#if SPK_FS_8K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_8000),
#endif
#if SPK_FS_16K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_16000),
#endif
#if SPK_FS_48K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_48000),
#endif
#if SPK_FS_44P1K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_44100),
#endif
#if SPK_FS_88P2K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_88200),
#endif
#if SPK_FS_96K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_96000),
#endif
#if SPK_FS_176P4K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_176400),
#endif
#if SPK_FS_192K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_192000),
#endif

#if(SUPPORT_USB_FEEDBACK)
	DESC_AS_EP_STD(AUDIO_OUT_EP, (EP_TYPE_ISOC | ADAPTIVE), UAC10_OUT_EP_MAX_PKT_SIZE_16BIT, ISO_OUT_ENDP_INTERVAL, 0, AUDIO_FB_EP),
#else
	DESC_AS_EP_STD(AUDIO_OUT_EP, (EP_TYPE_ISOC | ADAPTIVE), UAC10_OUT_EP_MAX_PKT_SIZE_16BIT, ISO_OUT_ENDP_INTERVAL, 0, 0),
#endif

	DESC_AS_EP_CS(1, 1, 1),

#if(SUPPORT_USB_FEEDBACK)
	DESC_AS_EP_STD(AUDIO_FB_EP, (EP_TYPE_ISOC | ASYNCHRONOUS | FB_EP), 4, ISO_OUT_ENDP_INTERVAL, FEEDBACK_SOF_RATE, 0),
#endif
#endif // SPK_AUDIO_16BIT

#if SPK_AUDIO_24BIT   /*24BIT SUPPORT */

	DESC_INTF(INTF_AUDIO_STREAM_SPK, AS_INTF_ALT_24BIT, SPK_ENDPOINT_NUM, AUDIO_CLASS, AUDIO_SUBCLASS_STREAMING, 0, 0),

	DESC_AS_INTF_GEN(TERM_ID_SPK_IN, 0),

	DESC_AS_INTF_FMT(SPK_24BIT_FS_NUM, USB_SPK_CHANNELS, AUDIO_24BIT_FRAMESIZE, AUDIO_24BIT_BIT_RESOLUTION),

#if SPK_FS_48K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_48000),
#endif
#if SPK_FS_44P1K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_44100),
#endif
#if SPK_FS_88P2K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_88200),
#endif
#if SPK_FS_96K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_96000),
#endif

#if(SUPPORT_USB_FEEDBACK)
	DESC_AS_EP_STD(AUDIO_OUT_EP, (EP_TYPE_ISOC | ADAPTIVE), UAC10_OUT_EP_MAX_PKT_SIZE_24BIT, ISO_OUT_ENDP_INTERVAL, 0, AUDIO_FB_EP),
#else
	DESC_AS_EP_STD(AUDIO_OUT_EP, (EP_TYPE_ISOC | ADAPTIVE), UAC10_OUT_EP_MAX_PKT_SIZE_24BIT, ISO_OUT_ENDP_INTERVAL, 0, 0),
#endif

	DESC_AS_EP_CS(MULT_FS_SUPPORT, 1, 1),

#if(SUPPORT_USB_FEEDBACK)
	DESC_AS_EP_STD(AUDIO_FB_EP, (EP_TYPE_ISOC | ASYNCHRONOUS | FB_EP), 4, ISO_OUT_ENDP_INTERVAL, FEEDBACK_SOF_RATE, 0),
#endif
#endif // SPK_AUDIO_24BIT

#if(SPK_24BIT_HS_NUM) /*24BIT High SampleRate SUPPORT */

	DESC_INTF(INTF_AUDIO_STREAM_SPK, AS_INTF_ALT_24BIT_HS, SPK_ENDPOINT_NUM, AUDIO_CLASS, AUDIO_SUBCLASS_STREAMING, 0, 0),

	DESC_AS_INTF_GEN(TERM_ID_SPK_IN, 0),

	DESC_AS_INTF_FMT(SPK_24BIT_HS_NUM, USB_SPK_CHANNELS, AUDIO_24BIT_FRAMESIZE, AUDIO_24BIT_BIT_RESOLUTION),

#ifdef SUPPORT_SPK_176P4KX24BIT
	AUDIO_SAMPLE_FREQ(176400),
#endif
#ifdef SUPPORT_SPK_192KX24BIT
	AUDIO_SAMPLE_FREQ(192000),
#endif

#if(SUPPORT_USB_FEEDBACK)
	DESC_AS_EP_STD(AUDIO_OUT_EP, (EP_TYPE_ISOC | ADAPTIVE), FS_ISO_OUT_ENDP_MAX_PACKET_SIZE_192X24BIT, ISO_OUT_ENDP_INTERVAL, 0, AUDIO_FB_EP),
#else
	DESC_AS_EP_STD(AUDIO_OUT_EP, (EP_TYPE_ISOC | ADAPTIVE), FS_ISO_OUT_ENDP_MAX_PACKET_SIZE_192X24BIT, ISO_OUT_ENDP_INTERVAL, 0, 0),
#endif

	DESC_AS_EP_CS(MULT_FS_SUPPORT, 1, 1),

#if(SUPPORT_USB_FEEDBACK)
	DESC_AS_EP_STD(AUDIO_FB_EP, (EP_TYPE_ISOC | ASYNCHRONOUS | FB_EP), 4, ISO_OUT_ENDP_INTERVAL, FEEDBACK_SOF_RATE, 0),
#endif
#endif // SPK_24BIT_HS_NUM

#endif // SUPPORT_USB_SPK

#if SUPPORT_USB_MIC

	DESC_INTF(INTF_AUDIO_STREAM_MIC, AS_INTF_ALT_OFFLINE, 0, AUDIO_CLASS, AUDIO_SUBCLASS_STREAMING, 0, 0),

#if MIC_AUDIO_16BIT    /*mic 16bit*/
	DESC_INTF(INTF_AUDIO_STREAM_MIC, AS_INTF_ALT_16BIT, 1, AUDIO_CLASS, AUDIO_SUBCLASS_STREAMING, 0, 0),

	DESC_AS_INTF_GEN(TERM_ID_MIC_OUT, 0x01),

	DESC_AS_INTF_FMT(MIC_16BIT_FS_NUM, USB_MIC_CHANNELS, AUDIO_16BIT_FRAMESIZE, AUDIO_16BIT_BIT_RESOLUTION),

#if MIC_FS_8K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_8000),
#endif
#if MIC_FS_16K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_16000),
#endif
#if MIC_FS_32K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_32000),
#endif
#if MIC_FS_44P1K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_44100),
#endif
#if MIC_FS_48K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_48000),
#endif
#if MIC_FS_88P2K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_88200),
#endif
#if MIC_FS_96K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_96000),
#endif
#if MIC_FS_176P4K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_176400),
#endif
#if MIC_FS_192K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_192000),
#endif

	DESC_AS_EP_STD(AUDIO_IN_EP, (EP_TYPE_ISOC | ASYNCHRONOUS), UAC10_IN_EP_MAX_PKT_SIZE_16BIT, ISO_IN_ENDP_INTERVAL, 0, 0),

	DESC_AS_EP_CS(MULT_FS_SUPPORT, 0, 0),
#endif // MIC_AUDIO_16BIT

#if MIC_AUDIO_24BIT      /*mic 24bit*/
	DESC_INTF(INTF_AUDIO_STREAM_MIC, AS_INTF_ALT_24BIT, 1, AUDIO_CLASS, AUDIO_SUBCLASS_STREAMING, 0, 0),

	DESC_AS_INTF_GEN(TERM_ID_MIC_OUT, 0),

	DESC_AS_INTF_FMT(MIC_24BIT_FS_NUM, USB_MIC_CHANNELS, AUDIO_24BIT_FRAMESIZE, AUDIO_24BIT_BIT_RESOLUTION),

#if MIC_FS_48K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_48000),
#endif
#if MIC_FS_44P1K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_44100),
#endif
#if MIC_FS_88P2K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_88200),
#endif
#if MIC_FS_96K
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_96000),
#endif

	DESC_AS_EP_STD(AUDIO_IN_EP, (EP_TYPE_ISOC | ASYNCHRONOUS), UAC10_IN_EP_MAX_PKT_SIZE_24BIT, ISO_IN_ENDP_INTERVAL, 0, 0),

	DESC_AS_EP_CS(MULT_FS_SUPPORT, 0, 0),
#endif // MIC_AUDIO_24BIT


#if(MIC_24BIT_HS_NUM) /*mic 192 24bit*/
	DESC_INTF(INTF_AUDIO_STREAM_MIC, AS_INTF_ALT_24BIT_HS, 1, AUDIO_CLASS, AUDIO_SUBCLASS_STREAMING, 0, 0),

	DESC_AS_INTF_GEN(TERM_ID_MIC_OUT, 0),

	DESC_AS_INTF_FMT(MIC_24BIT_HS_NUM, USB_MIC_CHANNELS, AUDIO_24BIT_FRAMESIZE, AUDIO_24BIT_BIT_RESOLUTION),

#ifdef SUPPORT_MIC_176P4KX24BIT
	AUDIO_SAMPLE_FREQ(176400),
#endif
#ifdef SUPPORT_MIC_192KX24BIT
	AUDIO_SAMPLE_FREQ(192000),
#endif

	DESC_AS_EP_STD(AUDIO_IN_EP, (EP_TYPE_ISOC | ASYNCHRONOUS), FS_ISO_IN_ENDP_MAX_PACKET_SIZE_192X24BIT, ISO_IN_ENDP_INTERVAL, 0, 0),

	DESC_AS_EP_CS(MULT_FS_SUPPORT, 0, 0),
#endif  //SUPPORT_MIC_192KX24BIT
#endif ///SUPPORT_USB_MIC

#if SUPPORT_USB_HID
	DESC_INTF(INTF_HID, 0, USB_HID_KEYBOARD_ENDPOINT_COUNT, HID_CLASS, 0, 0, 0),

	DESC_HID(0, 1),
	DESC_TYPE_HID_REPORT,                              /* Constant name identifying type of class descriptor. */
	LOBYTE(DESC_SIZE_HID_KEYBOARD_REPORT),/* Numeric expression that is the total size of the Report descriptor. */
	HIBYTE(DESC_SIZE_HID_KEYBOARD_REPORT),

	DESC_EP(AUDIO_HID_IN_EP, EP_TYPE_INTR, 64, 4),
	DESC_EP(AUDIO_HID_OUT_EP, EP_TYPE_INTR, 64, 0x20),
#endif //SUPPORT_USB_HID
};

#if(TEST_USB_SWITCH || SUPPORT_UAC_SELF_ADAPTION)
uint8_t uac_fullCfgDesc[USB_FS_AUDIO_CONFIG_DESC_SIZ] __attribute__ ((aligned (4))) =
{
	/* -- Configuration ---------------------------*/
#if(SUPPORT_USB_LPM)
	DESC_CONFIG(USB_FS_AUDIO_CONFIG_DESC_SIZ, USB_FUN_NUM_INTERFACES, 1, 0, (BUS_POWERED | REMOTE_WAKEUP), 100),
#else
	DESC_CONFIG(USB_FS_AUDIO_CONFIG_DESC_SIZ, USB_FUN_NUM_INTERFACES, 1, 0, BUS_POWERED, 100),
#endif

#if(SUPPORT_USB_SPK || SUPPORT_USB_MIC)  //18byte
	/* -- AudioControl Interface ---------------------------*/
	DESC_INTF(0, 0, 0, AUDIO_CLASS, AUDIO_SUBCLASS_CONTROL, 0, 0),

	// Header
	DESC_AC_INTF_H_10(USB_AUDIO_NUM, AUDIO_AC_INTF_DESC_BYTES),
#if (USB_AUDIO_NUM >1 )
	DESC_AC_INTF_H_10_INCOLLECTION_2,
#else
	DESC_AC_INTF_H_10_INCOLLECTION_1,
#endif

#endif // (SUPPORT_USB_SPK||SUPPORT_USB_MIC)

#if SUPPORT_USB_SPK
 /*
 *	AUDIO CONTROL INTERFACE : SPK
 *		- TERM_ID_SPK_IN    :1
 *		- FU_ID_SPK       :2
 *		- TERM_ID_SPK_OUT :3
 */
	// SPK IT
	DESC_AC_INTF_IT(TERM_ID_SPK_IN, USB_TERMINAL_USB_STREAMING, 0x00, USB_SPK_CHANNELS, USB_SPK_CHANNEL_SEL),

	// SPK FU
#if(SUPPORT_SPK_SIDETONE)
	DESC_AC_INTF_FU(USB_SPK_CHANNELS, FU_ID_SPK, MU_ID, USB_SPK_CONTROL_MASTER, USB_SPK_CONTROL_CH),
#else
	DESC_AC_INTF_FU(USB_SPK_CHANNELS, FU_ID_SPK, TERM_ID_SPK_IN, USB_SPK_CONTROL_MASTER, USB_SPK_CONTROL_CH),
#endif

	// SPK OT
	DESC_AC_INTF_OT(TERM_ID_SPK_OUT, OUTPUT_TERMINAL_HEADPHONE, 0x00, FU_ID_SPK),
#endif // SUPPORT_USB_SPK

#if SUPPORT_USB_MIC  /* Size = 0x1E */
/*
 *	AUDIO CONTROL INTERFACE DESCRIPTOR:	MIC
 *		- TERM_ID_MIC_IN
 *		- FU_ID_MIC
 *		- TERM_ID_MIC_OUT
 */
	// MIC IT
	DESC_AC_INTF_IT(TERM_ID_MIC_IN, INPUT_TERMINAL_MICROPHONE, 0x00, USB_MIC_CHANNELS, USB_MIC_CHANNEL_SEL),

	// MIC FU
	DESC_AC_INTF_FU(USB_MIC_CHANNELS, FU_ID_MIC, TERM_ID_MIC_IN, USB_MIC_CONTROL_MASTER, USB_MIC_CONTROL_CH),

	// MIC OT
	DESC_AC_INTF_OT(TERM_ID_MIC_OUT, USB_TERMINAL_USB_STREAMING, 0x00, SELECTOR_ID),

	//Selector
	0x07, 0x24, 0x05, SELECTOR_ID, 0x01, FU_ID_MIC, 0x00,

#if (SUPPORT_SPK_SIDETONE == 1)
	//Mixer
	0x09, 0x24, 0x06, FU_MIXER_ID, TERM_ID_MIC_IN, 0x01, 0x03, 0x00, 0x00,
	0x0D, 0x24, 0x04, MU_ID, 0x02, TERM_ID_SPK_IN, FU_MIXER_ID, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00,
#endif //SUPPORT_SPK_SIDETONE
#endif // SUPPORT_USB_MIC

#if SUPPORT_USB_SPK
/******************************* Interface 1, alternate 0 ********************************************/
/*
 *	INTERFACE DESCRIPTOR:	Audio Streaming (Speaker)
 */

	/* USB Speaker Standard AS Interface Descriptor - Audio Streaming Zero Bandwith */
	DESC_SIZE_INTF,					// bLength
	DESC_TYPE_INTF,					// bDescriptorType
	INTF_AUDIO_STREAM_SPK,	// bInterfaceNumber
	AS_INTF_ALT_OFFLINE,				// bAlternateSetting
	0x00,										// bNumEndpoints
	AUDIO_CLASS,								// bInterfaceClass
	AUDIO_SUBCLASS_STREAMING,					// bInterfaceSubClass
	0x00,										// bInterfaceProtocol
	0x00,										// iInterface */

#if SPK_AUDIO_16BIT
/******************************* Interface 1, alternate 1 ********************************************/
/*
 *	INTERFACE DESCRIPTOR:	Audio Streaming (Speaker)
 */

	/* USB Speaker Standard AS Interface Descriptor - Audio Streaming Operational */
	DESC_SIZE_INTF,							// bLength
	DESC_TYPE_INTF,					// bDescriptorType
	INTF_AUDIO_STREAM_SPK,	// bInterfaceNumber
	AS_INTF_ALT_16BIT,					// bAlternateSetting
	SPK_ENDPOINT_NUM,							// bNumEndpoints
	AUDIO_CLASS,								// bInterfaceClass
	AUDIO_SUBCLASS_STREAMING,					// bInterfaceSubClass
	0x00,										// bInterfaceProtocol
	0x00,										// iInterface

	/* Audio Streaming Interface Descriptor: AS General */
	DESC_SIZE_AS_INTF_GEN_10,					// bLength
	DESC_TYPE_AUDIO_INTF,							// bDescriptorType
	AS_INTF_SUBTYPE_GENERAL,				// bDescriptorSubtype
	TERM_ID_SPK_IN,		// bTerminalLink
	0x01,										// bDelay
	0x01, 0x00,									// wFormatTag

	/* Audio Type I Format Interface Descriptor */
	0X08 + 5*3,				// bLength
	DESC_TYPE_AUDIO_INTF,							// bDescriptorType
	AS_INTF_SUBTYPE_FORMAT_TYPE,			// bDescriptorSubtype
	0x01,										// bFormatType
	USB_SPK_CHANNELS,						// bNrChannels
	AUDIO_16BIT_FRAMESIZE,						// bSubFrameSize
	AUDIO_16BIT_BIT_RESOLUTION,					// bBitResolution
	5,						// bSamFreqType

	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_16000),
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_44100),
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_48000),
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_88200),
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_96000),

	/* Endpoint 1 - Audio Standard AS Descriptor */
	DESC_SIZE_AS_EP_STD,		 // bLength
	DESC_TYPE_EP,		 		 // bDescriptorType
	AUDIO_OUT_EP,						 // bEndpointAddress
	0x05,		 /* bmAttributes:
								 Bits 1..0: Transfer Type
									00 = Control
									01 = Isochronous
									10 = Bulk
									11 = Interrupt
								 Bits 3..2: Synchronization Type
									00 = No Synchronization
									01 = Asynchronous
									10 = Adaptive
									11 = Synchronous
								 Bits 5..4: Usage Type
									00 = Data endpoint
									01 = Feedback endpoint
									10 = Implicit feedback Data endpoint
									11 = Reserved
							  */
	USB_SHORT_GET_LOW(400 ),
	USB_SHORT_GET_HIGH(400),		 /* Maximum packet size for this endpoint is 8 Bytes.  */
	0x01,		 		// bInterval
	0x00,		 						// bRefresh
#if(SUPPORT_USB_FEEDBACK)
	AUDIO_FB_EP,							// bSynchAddress
#else
	0x00,		 						// bSynchAddress
#endif

	/* Endpoint 1 - Audio Class-Specific AS Descriptor */
	DESC_SIZE_AS_EP_CS,			// bLength
	DESC_TYPE_AUDIO_EP,					// bDescriptorType
	0x01,								// bDescriptorSubtype
	MULT_FS_SUPPORT,					// bmAttributes
	0x00,								// bLockDelayUnits
	0x00,
	0x00,								// wLockDelay
#endif // SPK_AUDIO_16BIT

#if SPK_AUDIO_24BIT   /*24BIT SUPPORT */
	/* USB Speaker Standard AS Interface Descriptor - Audio Streaming Operational */
	DESC_SIZE_INTF,							// bLength
	DESC_TYPE_INTF,					// bDescriptorType
	INTF_AUDIO_STREAM_SPK,	// bInterfaceNumber
	AS_INTF_ALT_24BIT,					// bAlternateSetting
	SPK_ENDPOINT_NUM,							// bNumEndpoints
	AUDIO_CLASS,								// bInterfaceClass
	AUDIO_SUBCLASS_STREAMING,					// bInterfaceSubClass
	0x00,										// bInterfaceProtocol
	0x00,										// iInterface

	/* Audio Streaming Interface Descriptor: AS General */
	DESC_SIZE_AS_INTF_GEN_10,					// bLength
	DESC_TYPE_AUDIO_INTF,							// bDescriptorType
	AS_INTF_SUBTYPE_GENERAL,				// bDescriptorSubtype
	TERM_ID_SPK_IN,		// bTerminalLink
	0x00,										// bDelay
	0x01, 0x00,									// wFormatTag

	/* Audio Type I Format Interface Descriptor */
	0X08 + 5*3,				// bLength
	DESC_TYPE_AUDIO_INTF,							// bDescriptorType
	AS_INTF_SUBTYPE_FORMAT_TYPE,			// bDescriptorSubtype
	0x01,										// bFormatType
	USB_SPK_CHANNELS,						// bNrChannels
	AUDIO_24BIT_FRAMESIZE,						// bSubFrameSize
	AUDIO_24BIT_BIT_RESOLUTION,					// bBitResolution
	5,//0x03, //0x07, //

	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_16000),
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_44100),
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_48000),
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_88200),
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_96000),

	/* Endpoint 1 - Audio Standard AS Descriptor */
	DESC_SIZE_AS_EP_STD,		 // bLength
	DESC_TYPE_EP,		 		 // bDescriptorType
	AUDIO_OUT_EP,						 // bEndpointAddress
	0x05,		 /* bmAttributes:
								 Bits 1..0: Transfer Type
									00 = Control
									01 = Isochronous
									10 = Bulk
									11 = Interrupt
								 Bits 3..2: Synchronization Type
									00 = No Synchronization
									01 = Asynchronous
									10 = Adaptive
									11 = Synchronous
								 Bits 5..4: Usage Type
									00 = Data endpoint
									01 = Feedback endpoint
									10 = Implicit feedback Data endpoint
									11 = Reserved
							  */	//
	USB_SHORT_GET_LOW((96 * 3 * 2) ),
	USB_SHORT_GET_HIGH((96 * 3 * 2)),		 /* Maximum packet size for this endpoint is 8 Bytes.  */
	0x01,		 		// bInterval
	0x00,		 						// bRefresh
#if(SUPPORT_USB_FEEDBACK)
	AUDIO_FB_EP,						// bSynchAddress
#else
	0x00,		 						// bSynchAddress
#endif

	/* Endpoint 1 - Audio Class-Specific AS Descriptor */
	DESC_SIZE_AS_EP_CS,			// bLength
	DESC_TYPE_AUDIO_EP,					// bDescriptorType
	0x01,								// bDescriptorSubtype
	MULT_FS_SUPPORT,					// bmAttributes
	0x00,								// bLockDelayUnits
	0x00,
	0x00,								// wLockDelay
#endif // SPK_AUDIO_24BIT


#endif // SUPPORT_USB_SPK

#if SUPPORT_USB_MIC
	DESC_SIZE_INTF,							// bLength
	DESC_TYPE_INTF,					// bDescriptorType
	INTF_AUDIO_STREAM_MIC, // bInterfaceNumber
	AS_INTF_ALT_OFFLINE,				// bAlternateSetting
	0x00,										// bNumEndpoints
	AUDIO_CLASS,								// bInterfaceClass
	AUDIO_SUBCLASS_STREAMING,					// bInterfaceSubClass
	0x00,										// bInterfaceProtocol
	0x00,										// iInterface */

#if MIC_AUDIO_16BIT    /*mic 16bit*/
	DESC_SIZE_INTF,							// bLength
	DESC_TYPE_INTF,					// bDescriptorType
	INTF_AUDIO_STREAM_MIC,	//bInterfaceNumber
	AS_INTF_ALT_16BIT,					// bAlternateSetting
	0x01,										// bNumEndpoints
	AUDIO_CLASS,								// bInterfaceClass
	AUDIO_SUBCLASS_STREAMING,					// bInterfaceSubClass
	0x00,										// bInterfaceProtocol
	0x00,										// iInterface

	DESC_SIZE_AS_INTF_GEN_10,					// bLength
	DESC_TYPE_AUDIO_INTF,							// bDescriptorType
	AS_INTF_SUBTYPE_GENERAL,				// bDescriptorSubtype
	0x02,										// bTerminalLink
	0x01,										// bDelay
	0x01, 0x00,									// wFormatTag

	0x08 + 3*3,					// bLength
	DESC_TYPE_AUDIO_INTF,							// bDescriptorType
	AS_INTF_SUBTYPE_FORMAT_TYPE,			// bDescriptorSubtype
	0x01,										// bFormatType
	USB_MIC_CHANNELS,						// bNrChannels
	AUDIO_16BIT_FRAMESIZE,						// bSubFrameSize
	AUDIO_16BIT_BIT_RESOLUTION,					// bBitResolution
	3, 							// bSamFreqType

	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_16000),
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_44100),
	AUDIO_SAMPLE_FREQ(SAMPLING_RATE_48000),

	DESC_SIZE_AS_EP_STD,		 // bLength
	DESC_TYPE_EP,		 		 // bDescriptorType
	AUDIO_IN_EP,						 // bEndpointAddress
	0x01,	//
	USB_SHORT_GET_LOW((48 * 2 * 2) ),
	USB_SHORT_GET_HIGH((48 * 2 * 2)),		 /* Maximum packet size for this endpoint is 8 Bytes.  */
	0x01,		 		// bInterval
	0x00,		 						// bRefresh
	0x00,								// bSynchAddress

	DESC_SIZE_AS_EP_CS,			// bLength
	DESC_TYPE_AUDIO_EP,					// bDescriptorType
	0x01,								// bDescriptorSubtype
	MULT_FS_SUPPORT,					// bmAttributes
	0x00,								// bLockDelayUnits
	0x00,
	0x00,								// wLockDelay
#endif // MIC_AUDIO_16BIT


#endif ///SUPPORT_USB_MIC

#if SUPPORT_USB_HID
	DESC_SIZE_INTF,                             /* Size of this descriptor in bytes */
	DESC_TYPE_INTF,	                             /* INTERFACE Descriptor Type */
	INTF_HID,                            /* Number of this interface. */
	0x00U,							                             /* Value used to select this alternate setting
	                                                                                                                            for the interface identified in the prior field */
	USB_HID_KEYBOARD_ENDPOINT_COUNT,                             /* Number of endpoints used by this interface (excluding endpoint zero). */
	HID_CLASS, 		                             /* Class code (assigned by the USB-IF). */
	0x00U,		                             					 /* Subclass code (assigned by the USB-IF). */
	0X00U,		                             					 /* Protocol code (assigned by the USB). */
	0x00U,							                             /* Index of string descriptor describing this interface */


	DESC_SIZE_HID,		                             /* Numeric expression that is the total size of the HID descriptor. */
	DESC_TYPE_HID,		                             /* Constant name specifying type of HID descriptor. */
	0x11U, 0x01U,					                             /* Numeric expression identifying the HID Class Specification release. */
	0x00U,							                             /* Numeric expression identifying country code of the localized hardware */
	0x01U,							                             /* Numeric expression specifying the number of
	                                                                                                                          class descriptors(at least one report descriptor) */
	DESC_TYPE_HID_REPORT,                              /* Constant name identifying type of class descriptor. */
	USB_SHORT_GET_LOW(DESC_SIZE_HID_KEYBOARD_REPORT),/* Numeric expression that is the total size of the Report descriptor. */
	USB_SHORT_GET_HIGH(DESC_SIZE_HID_KEYBOARD_REPORT),


	/*OUT*/
	0x07, 							 		 					  /* Size of this descriptor in bytes */
	DESC_TYPE_EP,								  /* endpoint Descriptor Type */

	AUDIO_HID_OUT_EP,					                          /* The address of the endpoint on the USB device
	                                                                                                                                     described by this descriptor. */
	EP_TYPE_INTR, 									  /* This field describes the endpoint's attributes */
	0x40,														  /* Maximum packet size this endpoint is capable of
	                                                                                                                                     sending or receiving when this configuration is selected. */
	0x00,

	0x20,												           /* Interval for polling endpoint for data transfers. */



	/*HID KEY IN*/
	DESC_SIZE_EP,                               /* Size of this descriptor in bytes */
	DESC_TYPE_EP,	                              /* endpoint Descriptor Type */
	AUDIO_HID_IN_EP,
	/* The address of the endpoint on the USB device
	                                                                                                                   described by this descriptor. */
	EP_TYPE_INTR,                                       /* This field describes the endpoint's attributes */
	0x40,                                                         /* Maximum packet size this endpoint is capable of
	                                                                                                                   sending or receiving when this configuration is selected. */

	0x00,

	0x04,                                                        /* Interval for polling endpoint for data transfers. */

#endif //SUPPORT_USB_HID
};
#endif

uint8_t uac_OtherSpeed_CfgDesc[USB_AUDIO_OTHERSPEED_CONFIG_DESC_SIZ] __attribute__ ((aligned (4))) =
{
    0x09,	// bLength
    0x02,	// bDescriptorType	CONFIGURATION (0x02)
    LOBYTE(USB_AUDIO_OTHERSPEED_CONFIG_DESC_SIZ),
    HIBYTE(USB_AUDIO_OTHERSPEED_CONFIG_DESC_SIZ),
    USB_FUN_NUM_INTERFACES,	//0x03,	//
    0x01,	//
    0x00,	//
    0xC0,	//
    0x32,	// /* bMaxPower = 100 mA*/
    0x09,	//
    0x04,	//
    0x00,	//
    0x00,	//
    0x00,	//
    0x01,	//
    0x01,	//
    0x00,	//
    0x00,	//
    0x0A,	//
    0x24,	//
    0x01,	//
    0x00,	//
    0x01,	//
    0x5D,	//
    0x00,	//
    0x02,	//
    0x01,	//
    0x02,	//
    0x0C,	//
    0x24,	//
    0x02,	//
    0x01,	//  ID
    0x01,	//
    0x01,	//
    0x00,	//
    0x02,	//
    0x03,	//
    0x00,	//
    0x00,	//
    0x00,	//
    0x0C,	//
    0x24,	//
    0x02,	//
    0x04,	//
    0x01,	//
    0x02,	//
    0x00,	//0x03
    0x02,	//
    0x03,	//
    0x00,	//
    0x00,	//
    0x00,	//
    0x09,	//
    0x24,	//
    0x03,	//
    0x03,	//
    0x01,	//
    0x03,	//
    0x00,	//0x04
    0x06,	//
    0x00,	//
    0x09,	//
    0x24,	//
    0x03,	//
    0x02,	//
    0x01,	//
    0x01,	//
    0x00,	//
    0x05,	//
    0x00,	//
#if 0
    0x0F,
    0x24,
    0x08,
    0x09,
    0xDA,
    0x0B,
    0x01,
    0x05,
    0x02,
    0x03,
    0x00,
    0x00,
    0x01,
    0x01,
    0x00,
    0x07,
    0x24,
    0x05, /*Select Unit*/
    0x09,
    0x01,
    0x05,
    0x00,
#endif
    0x0A,	//
    0x24,	//
    0x06,	//
    0x06,	//
    0x08,	//
    0x01,	//
    0x01,	//
    0x02,	//
    0x02,	//
    0x00,	//
    0x09,	//
    0x24,	//
    0x06,	//
    0x05,	//
    0x04,	//
    0x01,	//
    0x03,	// 0x43
    0x00,	//
    0x00,	//
    0x09,	//
    0x24,	//
    0x06,	//
    0x07,	//
    0x04,	//
    0x01,	//
    0x03,	//
    0x00,	//
    0x00,	//
    0x0D,	//
    0x24,	//
    0x04,	//
    0x08,	//
    0x02,	//
    0x01,	//
    0x07,	//
    0x02,	//
    0x03,	//
    0x00,	//
    0x00,	//
    0x00,	//
    0x00,	//
    0x09,	//
    0x04,	//
    0x01,	//
    0x00,	//
    0x00,	//
    0x01,	//
    0x02,	//
    0x00,	//
    0x00,	//
    0x09,	//
    0x04,	//
    0x01,	//
    0x01,	//
    0x01,	//
    0x01,	//
    0x02,	//
    0x00,	//
    0x00,	//
    0x07,	//
    0x24,	//
    0x01,	//
    0x01,	//
    0x01,	//
    0x01,	//
    0x00,	//
    0x0E,//0x1D,	//
    0x24,	//
    0x02,	//
    0x01,	//
    0x02,	//
    0x02,	//
    0x10,	//
    0x02,//0x07,	//
    0x80,0xBB,0x00,
    0x44,0xAC,0x00,
    0x09,		 //
    0x05,		 //
    0x01,		 // 0x02
    0x01,		 //
    USB_SHORT_GET_LOW(UAC10_OUT_EP_MAX_PKT_SIZE_16BIT ),
    USB_SHORT_GET_HIGH(UAC10_OUT_EP_MAX_PKT_SIZE_16BIT),		 /* Maximum packet size for this endpoint is 8 Bytes.  */
    0x04,		 //
    0x00,		 //
    0x00,		 //
    0x07,	//
    0x25,	//
    0x01,	//
    0x01,	//
    0x00,	//
    0x00,	//
    0x00,	//
    0x09,	//
    0x04,	//
    0x02,	//
    0x00,	//
    0x00,	//
    0x01,	//
    0x02,	//
    0x00,	//
    0x00,	//
#if 1 /*mic 16bit*/
    0x09,	//
    0x04,	//
    0x02,	//
    0x01,	//
    0x01,	//
    0x01,	//
    0x02,	//
    0x00,	//
    0x00,	//
    0x07,	//
    0x24,	//
    0x01,	//
    0x02,	//0x06
    0x00,	//
    0x01,	//
    0x00,	//
    0x0E,	//
    0x24,	//
    0x02,	//
    0x01,	//
    0x02,	//0x01,	//
    0x02,	//
    0x10,	//
    0x02,	//
    0x80,0xBB,0x00,	//
    0x44,0xAC,0x00,
    0x09,	//
    0x05,	//
    0x82,	// 0x83
    0x01,	//
    USB_SHORT_GET_LOW(UAC10_IN_EP_MAX_PKT_SIZE_16BIT ),
    USB_SHORT_GET_HIGH(UAC10_IN_EP_MAX_PKT_SIZE_16BIT),		 /* Maximum packet size for this endpoint is 8 Bytes.  */
    0x04,	//
    0x00,	//
    0x00,	//
    0x07,	//
    0x25,	//
    0x01,	//
    0x01,	//
    0x00,	//
    0x00,	//
    0x00,	//
#endif

#if SUPPORT_USB_HID
    DESC_SIZE_INTF,                             /* Size of this descriptor in bytes */
    DESC_TYPE_INTF,	                             /* INTERFACE Descriptor Type */
    INTF_HID,                            /* Number of this interface. */
    0x00U,							                             /* Value used to select this alternate setting
                                                                    for the interface identified in the prior field */
    USB_HID_KEYBOARD_ENDPOINT_COUNT,                             /* Number of endpoints used by this interface (excluding endpoint zero). */
    HID_CLASS, 		                             /* Class code (assigned by the USB-IF). */
    0x00U,		                             					 /* Subclass code (assigned by the USB-IF). */
    0X00U,		                             					 /* Protocol code (assigned by the USB). */
    0x00U,							                             /* Index of string descriptor describing this interface */


    DESC_SIZE_HID,		                             /* Numeric expression that is the total size of the HID descriptor. */
    DESC_TYPE_HID,		                             /* Constant name specifying type of HID descriptor. */
    0x11U, 0x01U,					                             /* Numeric expression identifying the HID Class Specification release. */
    0x00U,							                             /* Numeric expression identifying country code of the localized hardware */
    0x01U,							                             /* Numeric expression specifying the number of
                                                                    class descriptors(at least one report descriptor) */
    DESC_TYPE_HID_REPORT,                              /* Constant name identifying type of class descriptor. */
    USB_SHORT_GET_LOW(DESC_SIZE_HID_KEYBOARD_REPORT),/* Numeric expression that is the total size of the Report descriptor. */
    USB_SHORT_GET_HIGH(DESC_SIZE_HID_KEYBOARD_REPORT),


    /*HID KEY IN*/
    DESC_SIZE_EP,                               /* Size of this descriptor in bytes */
    DESC_TYPE_EP,	                              /* endpoint Descriptor Type */
    AUDIO_HID_IN_EP,
    /* The address of the endpoint on the USB device  described by this descriptor. */

    EP_TYPE_INTR,                                       /* This field describes the endpoint's attributes */
    0x40,                                                         /* Maximum packet size this endpoint is capable of
                                                                     sending or receiving when this configuration is selected. */
    0x00,
    0x20,                                                        /* Interval for polling endpoint for data transfers. */
#endif
};


uint8_t uac_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __attribute__ ((aligned (4))) = {
    USB_LEN_DEV_QUALIFIER_DESC,
    DESC_TYPE_DEV_Q,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
};

/* USB AUDIO device Configuration Descriptor */
#if(SUPPORT_UAC20)
uint8_t uac20_CfgDesc[USB_AUDIO20_CONFIG_DESC_SIZ] __attribute__ ((aligned (4))) =
{
	/*Configuaration Descriptor*/
	DESC_SIZE_CONFIG, /*bLength*/
	DESC_TYPE_CONFIG, /*bDescriptorType CONFIGURATION (0x02)*/
	LOBYTE(USB_AUDIO20_CONFIG_DESC_SIZ),
	HIBYTE(USB_AUDIO20_CONFIG_DESC_SIZ),

	/*0x01,*/ /*wTotalLength*/
	USB_FUN_NUM_INTERFACES,//0x05, /*bNumInterfaces*/
	0x01, /*bConfigurationValue*/
	0x04, /*iConfiguration	Headset (4)*/
#if SUPPORT_USB_LPM
	0xA0,
#else
	0xC0,
#endif
	/*
	bmAttributes.Reserved 0
	bmAttributes.RemoteWakeup RemoteWakeup Supported (0b1)
	bmAttributes.SelfPowered	Bus Powered (0b0)
	*/
	0x32, /*bMaxPower	100mA (0x32)*/

#if (SUPPORT_USB_SPK||SUPPORT_USB_MIC)

	/*Standard Interface Association Descriptor*/
	DESC_SIZE_INTF_ASSOC, /*bLength*/
	DESC_TYPE_INTF_ASSOC, /*bDescriptorType INTERFACE_ASSOCIATION (0x0b)*/
	0x00, /*bFirstInterface*/
	USB_FUN_NUM_INTERFACES - 1,//0x03, /*bInterfaceCount*/
	AUDIO_CLASS, /*bFunctionClass	Audio (0x01)*/
	0x00, /*bFunctionSubClass	Undefined (0x00)*/
	AUDIO_PROTOCOL_VERSION_02_00, /*bFunctionProtocol	IP Version 02 00 (0x20)*/
	0x00, /*iFunction	None (0)*/

	/*Standard AC Interface Descriptor*/
	DESC_SIZE_INTF, /*bLength*/
	DESC_TYPE_INTF, /*bDescriptorType INTERFACE (0x04)*/
	0x00, /*bInterfaceNumber*/
	0x00, /*bAlternateSetting*/
	0x00, /*bNumEndpoints*/
	AUDIO_CLASS, /*bInterfaceClass Audio (0x01)*/
	AUDIO_SUBCLASS_CONTROL, /*bInterfaceSubClass	Audio control (0x01)*/
	AUDIO_PROTOCOL_VERSION_02_00, /*bInterfaceProtocol	IP Version 02 00 (0x20)*/
	0x00, /*iInterface*/

	/*Class-Specific AC Interface Descriptor*/
	DESC_SIZE_AC_INTF_H_20, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AC_INTF_SUBTYPE_H, /*bDescriptorSubtype 	HEADER (0x01)*/
	0x00,
	0x02, /*bcdADC	2.00 (0x0200)*/
	AUDIO_FUNC_SUBCLASS_HEADSET, /*bCategory	HEADSET (0x04)*/
#if(SUPPORT_SPK_SIDETONE)
	0xA3, /*0x85*/
	0x00, /*wTotalLength	133*/
#else
	0x85,//0x85, /*0x85*/
	0x00, /*wTotalLength	133*/
#endif
	0x00, /*bmControls*/

#endif

#if SUPPORT_USB_SPK
	/*Clock Source Descriptor*/
	DESC_SIZE_AC_INTF_CLK_SRC, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AC_INTF_SUBTYPE_CLK_SRC, /*bDescriptorSubtype	CLOCK_SOURCE (0x0a)*/
	0x09, /*bClockID*/
	0x03, /*bmAttributes	Internal programmable Clock (0b011)*/
	0x07, /*bmControls	 (0b00000111)*/
	0x00, /*bAssocTerminal*/
	0x00, /*iClockSource*/

	/*Clock Selector Descriptor*/
#if SUPPORT_USB_MIC
	(DESC_SIZE_AC_INTF_CLK_SEL + 2), /*bLength*/
#else
	0x08, /*bLength*/
#endif
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AC_INTF_SUBTYPE_CLK_SEL, /*bDescriptorSubtype	CLOCK_SELECTOR (0x0b)*/
	0x0B, /*bClockID*/
#if SUPPORT_USB_MIC
	0x02, /*bNrInPins*/
	0x09, /*baCSourceID (1)*/
	0x0A, /*baCSourceID (2)*/
#else
	0x01, /*bNrInPins*/
	0x09, /*baCSourceID (1)*/
#endif
	0x03, /*bmControls	0b00000011*/
	0x00, /*iClockSelector*/

	/**********************Spker out track :INPUT_TERMINAL************************************/
	/*Input Terminal Descriptor*/
	DESC_SIZE_AC_INTF_IT_20, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AC_INTF_SUBTYPE_IT, /*bDescriptorSubtype	INPUT_TERMINAL (0x02)*/
	0x01, /*bTerminalID*/
	LOBYTE(USB_TERMINAL_USB_STREAMING), /**/
	HIBYTE(USB_TERMINAL_USB_STREAMING), /*wTerminalType	USB streaming (0x0101)*/
	0x00, /*bAssocTerminal*/
	0x0B, /*bCSourceID*/
	USB_SPK_CHANNELS, /*bNrChannels*/
	0x03, /**/
	0x00, /**/
	0x00, /**/
	0x00, /*bmChannelConfig 0b00000000000000000000000000000011*/
	0x00, /*iChannelNames*/
	0x00, /**/
	0x00, /*bmControls	0b0000000000000000*/
	0x00, /*iTerminal*/

	/********************Spker out track :FEATURE_UNIT***************************************/
	/*Feature Unit Descriptor*/
	(DESC_SIZE_AC_INTF_FU_20 + (USB_SPK_CHANNELS + 1) * 4), /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AC_INTF_SUBTYPE_FU, /*bDescriptorSubtype	FEATURE_UNIT (0x06)*/
	0x02, /*bUnitID*/
#if(SUPPORT_SPK_SIDETONE)
	0x0E, /*bSourceID*/
#else
	0x01, /*bSourceID*/
#endif
	0x03, /**/
	0x00, /**/
	0x00, /**/
	0x00, /*bmaControls(1)	0b00000000000000000000000000000011*/
	0x0C, /**/
	0x00, /**/
	0x00, /**/
	0x00, /*bmaControls(2)	0b00000000000000000000000000001100*/
	0x0C, /**/
	0x00, /**/
	0x00, /**/
	0x00, /*bmaControls(3)	0b00000000000000000000000000001100*/
	0x04, /*iFeature	Headset (4)*/

	/**********************************************Spker out track :OUTPUT_TERMINAL******************************************************************/
	/*Output Terminal Descriptor*/
	DESC_SIZE_AC_INTF_OT_20, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AC_INTF_SUBTYPE_OT, /*bDescriptorSubtype	OUTPUT_TERMINAL (0x03)*/
	0x03, /*bTerminalID*/
	LOBYTE(OUTPUT_TERMINAL_HEADPHONE), /**/
	HIBYTE(OUTPUT_TERMINAL_HEADPHONE), /*wTerminalType	Headset (0x0301)*/
#if(SUPPORT_SPK_SIDETONE)
	0x00, //0x04, /*bAssocTerminal*/
#else
#if SUPPORT_USB_MIC
	0x04, /*bAssocTerminal*/
#else
	0x04, /*bAssocTerminal*/
#endif
#endif
	0x02, /*bSourceID*/
	0x0B, /*bCSourceID*/
	0x00, /**/
	0x00, /*bmControls	0b0000000000000000*/
	0x00, /*iTerminal*/

#endif
#if SUPPORT_USB_MIC
	/*Clock Source Descriptor*/
	DESC_SIZE_AC_INTF_CLK_SRC, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AC_INTF_SUBTYPE_CLK_SRC, /*bDescriptorSubtype	CLOCK_SOURCE (0x0a)*/
	0x0A, /*bClockID*/
	0x03, /*bmAttributes	Internal programmable Clock (0b011)*/
	0x07, /*bmControls	 (0b00000111)*/
	0x00, /*bAssocTerminal*/
	0x00, /*iClockSource*/

#if SUPPORT_USB_SPK
	/*Clock Selector Descriptor*/
	(DESC_SIZE_AC_INTF_CLK_SEL + 2), /*bLength*/
#else
	0x08, /*bLength*/
#endif
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AC_INTF_SUBTYPE_CLK_SEL, /*bDescriptorSubtype	CLOCK_SELECTOR (0x0b)*/
	0x0C, /*bClockID*/
#if SUPPORT_USB_SPK
	0x02, /*bNrInPins*/
	0x09, /*baCSourceID (1)*/
	0x0A, /*baCSourceID (2)*/
#else
	0x01, /*bNrInPins*/
	0x0A, /*baCSourceID (2)*/
#endif
	0x03, /*bmControls	0b00000011*/
	0x00, /*iClockSelector*/

	/* Output Terminal Descriptor */
	DESC_SIZE_AC_INTF_OT_20, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AC_INTF_SUBTYPE_OT, /*bDescriptorSubtype	OUTPUT_TERMINAL (0x03)*/
	0x06, /*bTerminalID*/
	LOBYTE(USB_TERMINAL_USB_STREAMING), /**/
	HIBYTE(USB_TERMINAL_USB_STREAMING), /*wTerminalType	USB streaming (0x0101)*/
	0x00, /*bAssocTerminal*/
	0x05, /*bSourceID*/
	0x0C, /*bCSourceID*/
	0x00, /**/
	0x00, /*bmControls	0b0000000000000000*/
	0x00, /*iTerminal*/

	/*Feature Unit Descriptor*/
	0x0E, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AC_INTF_SUBTYPE_FU, /*bDescriptorSubtype	FEATURE_UNIT (0x06)*/
	0x05, /*bUnitID*/
	0x04, /*bSourceID*/
	0x03, /**/
	0x00, /**/
	0x00, /**/
	0x00, /*bmaControls(1)	0b00000000000000000000000000000011*/
	0x0C, /**/
	0x00, /**/
	0x00, /**/
	0x00, /*bmaControls(2)	0b00000000000000000000000000001100*/
	0x06, /*iFeature	Headset (6)*/

	/*Input Terminal*/
	DESC_SIZE_AC_INTF_IT_20, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AC_INTF_SUBTYPE_IT, /*bDescriptorSubtype	INPUT_TERMINAL (0x02)*/
	0x04, /*bTerminalID*/
	LOBYTE(INPUT_TERMINAL_MICROPHONE), /**/
	HIBYTE(INPUT_TERMINAL_MICROPHONE), /*wTerminalType	Headset (0x0402)*/
#if(SUPPORT_SPK_SIDETONE)
	0x00, /*bAssocTerminal*/
#else
#if SUPPORT_USB_SPK
	0x03, /*bAssocTerminal*/
#else
	0x00, /*bAssocTerminal*/
#endif
#endif
	0x0C, /*bCSourceID*/
	USB_MIC_CHANNELS, /*bNrChannels*/
	LOBYTE(USB_MIC_CHANNEL_SEL),
	HIBYTE(USB_MIC_CHANNEL_SEL),
	0x00, /**/
	0x00, /*bmChannelConfig 0b00000000000000000000000000000001*/
	0x00, /*iChannelNames*/
	0x00, /**/
	0x00, /*bmControls	0b0000000000000000*/
	0x00, /*iTerminal*/
#endif
#if(SUPPORT_SPK_SIDETONE)
	0x0E, /*bLength*/
	0x24, /*bDescriptorType CS_INTERFACE (0x24)*/
	0x06, /*bDescriptorSubtype	FEATURE_UNIT (0x06)*/
	0x0D, /*bUnitID*/
	0x04, /*bSourceID*/
	0x03, /**/
	0x00, /**/
	0x00, /**/
	0x00, /*bmaControls(1)	0b00000000000000000000000000000011*/
	0x0C, /**/
	0x00, /**/
	0x00, /**/
	0x00, /*bmaControls(2)	0b00000000000000000000000000001100*/
	0x00, /*iFeature	Headset (6)*/
	0x10,   /*bLength*/
	0x24,   /*bDescriptorType*/
	0x04,   /*bDescriptorSubType*/
	0x0E,   /*bUnitID*/
	0x02,   /*bNrInPins*/
	0x01,   /*baSourceID0*/
	0x0D,   /*baSourceid1*/
	0x02,   /*bNrChannels*/
	0x03,
	0x00,
	0x00,
	0x00,   /*bmChannelConfig*/
	0x00,   /*iChanelName*/
	0x00,
	0x01,
	0x00,
#endif

#if  SUPPORT_USB_SPK
	/************bInterfaceNumber : 1,bAlternateSetting = 0 : offline ***********************************/
	/*Standard AS Interface Descriptor*/
	DESC_SIZE_INTF, /*bLength*/
	DESC_TYPE_INTF, /*bDescriptorType INTERFACE (0x04)*/
	0x01, /*bInterfaceNumber*/
	0x00, /*bAlternateSetting*/
	0x00, /*bNumEndpoints*/
	AUDIO_CLASS, /*bInterfaceClass Audio (0x01)*/
	AUDIO_SUBCLASS_STREAMING, /*bInterfaceSubClass	Audio streaming (0x02)*/
	AUDIO_PROTOCOL_VERSION_02_00, /*bInterfaceProtocol	IP Version 02 00 (0x20)*/
	0x00, /*iInterface*/

#if SPK_AUDIO_16BIT   /*46*/
	/***********bInterfaceNumber : 1, bAlternateSetting = 1        ***********************************/

	/*Standard AS Interface Descriptor*/
	DESC_SIZE_INTF, /*bLength*/
	DESC_TYPE_INTF, /*bDescriptorType INTERFACE (0x04)*/
	INTF_AUDIO_STREAM_SPK,//0x01, /*bInterfaceNumber*/
	AS_INTF_ALT_16BIT,//0x01, /*bAlternateSetting*/
	0x01, /*bNumEndpoints*/
	AUDIO_CLASS, /*bInterfaceClass Audio (0x01)*/
	AUDIO_SUBCLASS_STREAMING, /*bInterfaceSubClass	Audio streaming (0x02)*/
	AUDIO_PROTOCOL_VERSION_02_00, /*bInterfaceProtocol	IP Version 02 00 (0x20)*/
	0x00, /*iInterface*/

	/* Class-Specific AS Interface Descriptor*/
	DESC_SIZE_AS_INTF_GEN_20, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AS_INTF_SUBTYPE_GENERAL, /*bDescriptorSubtype	AS_GENERAL (0x01)*/
	0x01, /*bTerminalLink*/
	0x00, /*bmControls*/
	0x01, /*bFormatType*/
	0x01, /**/
	0x00, /**/
	0x00, /**/
	0x00, /*bmFormats	0b00000000000000000000000000000001*/
	USB_SPK_CHANNELS, /*bNrChannels*/
	0x03, /**/
	0x00, /**/
	0x00, /**/
	0x00, /*bmChannelConfig 0b00000000000000000000000000000011*/
	0x00, /*iChannelNames*/

	/*Type I Format Descriptor*/
	DESC_SIZE_AS_INTF_FMT_20, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AS_INTF_SUBTYPE_FORMAT_TYPE, /*bDescriptorSubtype	FORMAT_TYPE (0x02)*/
	0x01, /*bFormatType FORMAT_TYPE_I (0x01)*/
	0x02, /*bSubslotSize*/
	0x10, /*bBitResolution*/

	/*Standard AS Endpoint Descriptor*/
	DESC_SIZE_EP, /*bLength*/
	DESC_TYPE_EP, /*bDescriptorType ENDPOINT (0x05)*/
	0x01, /*bEndpointAddress	1 OUT (0b00000001)*/
	0x0D, //0x01|0x04,//0x0D,
	/*
	bmAttributes.TransferType	Isochronous (0b01)
	bmAttributes.SynchronizationType	Synchronous (0b11)
	bmAttributes.UsageType	Data endpoint (0b00)
	*/
	LOBYTE(UAC20_OUT_EP_MAX_PKT_SIZE_16BIT),
	HIBYTE(UAC20_OUT_EP_MAX_PKT_SIZE_16BIT), /*
	wMaxPacketSize.PacketSize	384
	wMaxPacketSize.Transactions One transaction per microframe if HS (0b00)
	*/
	0x01, /*bInterval*/

	/*Class-Specific AS Endpoint Descriptor*/
	0x08, /*bLength*/
	DESC_TYPE_AUDIO_EP, /*bDescriptorType CS_ENDPOINT (0x25)*/
	0x01, /*bDescriptorSubtype	EP_GENERAL (0x01)*/
	0x00, /**/
	0x00, /*
	bmAttributes.Reserved	0b0000000
	bmAttributes.MaxPacketsOnly 0b0
	*/
	0x02, /*bLockDelayUnits Decoded PCM samples (2)*/
	0x02, /*wLockDelay*/
	0x00, /**/
#endif

#if SPK_AUDIO_24BIT
	/************bInterfaceNumber : 1,bAlternateSetting = 2	**************************/

	/*Interface Descriptor*/
	DESC_SIZE_INTF, /*bLengths*/
	DESC_TYPE_INTF, /*bDescriptorType INTERFACE (0x04)*/
	INTF_AUDIO_STREAM_SPK,//0x01, /*bInterfaceNumber*/
	AS_INTF_ALT_24BIT,//0x02, /*bAlternateSetting*/
	0x01, /*bNumEndpoints*/
	0x01, /*bInterfaceClass Audio (0x01)*/
	0x02, /*bInterfaceSubClass	Audio streaming (0x02)*/
	0x20, /*bInterfaceProtocol	IP Version 02 00 (0x20)*/
	0x00, /*iInterface*/

	/*Audio AS Descriptor*/
	DESC_SIZE_AS_INTF_GEN_20, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AS_INTF_SUBTYPE_GENERAL, /*bDescriptorSubtype	AS_GENERAL (0x01)*/
	0x01, /*bTerminalLink*/
	0x00, /*bmControls*/
	0x01, /*bFormatType*/
	0x01, /**/
	0x00, /**/
	0x00, /**/
	0x00, /*bmFormats	0b00000000000000000000000000000001*/
	0x02, /*bNrChannels*/
	0x03, /**/
	0x00, /**/
	0x00, /**/
	0x00, /*bmChannelConfig 0b00000000000000000000000000000011*/
	0x00, /*iChannelNames*/

	/*Type I Format Descriptor*/
	DESC_SIZE_AS_INTF_FMT_20, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AS_INTF_SUBTYPE_FORMAT_TYPE, /*bDescriptorSubtype	FORMAT_TYPE (0x02)*/
	0x01, /*bFormatType FORMAT_TYPE_I (0x01)*/
	0x03, /*bSubslotSize*/
	0x18, /*bBitResolution*/

	/* Endpoint Descriptor*/
	DESC_SIZE_EP, /*bLength*/
	DESC_TYPE_EP, /*bDescriptorType ENDPOINT (0x05)*/
	0x01, /*bEndpointAddress	1 OUT (0b00000001)*/
	0x0D, //0x01|0x04,//0x0D,
	/*
	bmAttributes.TransferType	Isochronous (0b01)
	bmAttributes.SynchronizationType	Synchronous (0b11)
	bmAttributes.UsageType	Data endpoint (0b00)
	*/
	LOBYTE(UAC20_OUT_EP_MAX_PKT_SIZE_24BIT),
	HIBYTE(UAC20_OUT_EP_MAX_PKT_SIZE_24BIT),/*
	wMaxPacketSize.PacketSize	576
	wMaxPacketSize.Transactions One transaction per microframe if HS (0b00)
	*/
	0x01, /*bInterval*/

	/* Audio EP Descriptor*/
	0x08, /*bLength*/
	0x25, /*bDescriptorType CS_ENDPOINT (0x25)*/
	0x01, /*bDescriptorSubtype	EP_GENERAL (0x01)*/
	0x00, /**/
	0x00, /*
	bmAttributes.Reserved	0b0000000
	bmAttributes.MaxPacketsOnly 0b0
	*/
	0x02, /*bLockDelayUnits Decoded PCM samples (2)*/
	0x02, /*wLockDelay*/
	0x00, /**/
#endif
#if  SPK_AUDIO_32BIT  //size 46
	/**********bInterfaceNumber : 1,bAlternateSetting = 3 *******************************/

	/*Interface Descriptor*/
	0x09, /*bLength*/
	0x04, /*bDescriptorType INTERFACE (0x04)*/
	INTF_AUDIO_STREAM_SPK,//0x01, /*bInterfaceNumber*/
	AS_INTF_ALT_32BIT,//0x03, /*bAlternateSetting*/
	0x01, /*bNumEndpoints*/
	0x01, /*bInterfaceClass Audio (0x01)*/
	0x02, /*bInterfaceSubClass	Audio streaming (0x02)*/
	0x20, /*bInterfaceProtocol	IP Version 02 00 (0x20)*/
	0x00, /*iInterface*/

	/*Audio AS Descriptor*/
	0x10, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	0x01, /*bDescriptorSubtype	AS_GENERAL (0x01)*/
	0x01, /*bTerminalLink*/
	0x00, /*bmControls*/
	0x01, /*bFormatType*/
	0x01, /**/
	0x00, /**/
	0x00, /**/
	0x00, /*bmFormats	0b00000000000000000000000000000001*/
	0x02, /*bNrChannels*/
	0x03, /**/
	0x00, /**/
	0x00, /**/
	0x00, /*bmChannelConfig 0b00000000000000000000000000000011*/
	0x00, /*iChannelNames*/

	/*Audio AS Descriptor*/
	0x06, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	0x02, /*bDescriptorSubtype	FORMAT_TYPE (0x02)*/
	0x01, /*bFormatType FORMAT_TYPE_I (0x01)*/
	0x04, /*bSubslotSize*/
	0x20, /*bBitResolution*/

	/*Endpoint Descriptor*/
	0x07, /*bLength*/
	0x05, /*bDescriptorType ENDPOINT (0x05)*/
	0x01, /*bEndpointAddress	1 OUT (0b00000001)*/
	0x0D, //0x01|0x04,//0x0D,
	/*
	bmAttributes.TransferType	Isochronous (0b01)
	bmAttributes.SynchronizationType	Synchronous (0b11)
	bmAttributes.UsageType	Data endpoint (0b00)
	*/
	0x00, /**/
	0x03, /*
	wMaxPacketSize.PacketSize	768
	wMaxPacketSize.Transactions One transaction per microframe if HS (0b00)
	*/
	0x01, /*bInterval*/

	/*Audio EP Descriptor*/
	0x08, /*bLength*/
	0x25, /*bDescriptorType CS_ENDPOINT (0x25)*/
	0x01, /*bDescriptorSubtype	EP_GENERAL (0x01)*/
	0x00, /**/
	0x00, /*
	bmAttributes.Reserved	0b0000000
	bmAttributes.MaxPacketsOnly 0b0
	*/
	0x02, /*bLockDelayUnits Decoded PCM samples (2)*/
	0x02, /*wLockDelay*/
	0x00, /**/
#endif
#endif //SUPPORT_USB_SPK

#if  SUPPORT_USB_MIC
	/**********bInterfaceNumber : 1,bAlternateSetting = 3 ***************************/

	/*Standard AS Interface Descriptor*/
	DESC_SIZE_INTF, /*bLength*/
	DESC_TYPE_INTF, /*bDescriptorType INTERFACE (0x04)*/
	INTF_AUDIO_STREAM_MIC,//0x02, /*bInterfaceNumber*/
	AS_INTF_ALT_OFFLINE,//0x00, /*bAlternateSetting*/
	0x00, /*bNumEndpoints*/
	AUDIO_CLASS, /*bInterfaceClass Audio (0x01)*/
	AUDIO_SUBCLASS_STREAMING, /*bInterfaceSubClass	Audio streaming (0x02)*/
	AUDIO_PROTOCOL_VERSION_02_00, /*bInterfaceProtocol	IP Version 02 00 (0x20)*/
	0x00, /*iInterface*/
#if MIC_AUDIO_16BIT
	DESC_SIZE_INTF, /*bLength*/
	DESC_TYPE_INTF, /*bDescriptorType INTERFACE (0x04)*/
	INTF_AUDIO_STREAM_MIC,//0x02, /*bInterfaceNumber*/
	AS_INTF_ALT_16BIT,//0x01, /*bAlternateSetting*/
	0x01, /*bNumEndpoints*/
	AUDIO_CLASS, /*bInterfaceClass Audio (0x01)*/
	AUDIO_SUBCLASS_STREAMING, /*bInterfaceSubClass	Audio streaming (0x02)*/
	AUDIO_PROTOCOL_VERSION_02_00, /*bInterfaceProtocol	IP Version 02 00 (0x20)*/
	0x00, /*iInterface*/

	/*Audio AS Descriptor*/
	DESC_SIZE_AS_INTF_GEN_20, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AS_INTF_SUBTYPE_GENERAL, /*bDescriptorSubtype	AS_GENERAL (0x01)*/
	0x06, /*bTerminalLink*/
	0x00, /*bmControls*/
	0x01, /*bTerminalLink*/
	0x01, /*bFormatType*/
	0x00, /**/
	0x00, /**/
	0x00, /**/

	USB_MIC_CHANNELS, /*bNrChannels*/
	LOBYTE(USB_MIC_CHANNEL_SEL),
	HIBYTE(USB_MIC_CHANNEL_SEL),
	0x00, /**/
	0x00, /*bmChannelConfig 0b00000000000000000000000000000011*/
	0x00, /*iChannelNames*/

	/*Type I Format Descriptor*/
	DESC_SIZE_AS_INTF_FMT_20, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AS_INTF_SUBTYPE_FORMAT_TYPE, /*bDescriptorSubtype	FORMAT_TYPE (0x02)*/
	0x01, /*bFormatType FORMAT_TYPE_I (0x01)*/
	0x02, /*bSubslotSize*/
	0x10, /*bBitResolution*/

	/*Endpoint Descriptor*/
	DESC_SIZE_EP, /*bLength*/
	DESC_TYPE_EP, /*bDescriptorType ENDPOINT (0x05)*/
	0x82, /*bEndpointAddress	1 IN (0b10000001)*/
	0x0D, //0x01|0x04,//0x0D,
	/*
	bmAttributes.TransferType	Isochronous (0b01)
	bmAttributes.SynchronizationType	Synchronous (0b11)
	bmAttributes.UsageType	Data endpoint (0b00)
	*/

	LOBYTE(UAC20_IN_EP_MAX_PKT_SIZE_16BIT),
	HIBYTE(UAC20_IN_EP_MAX_PKT_SIZE_16BIT),
	0x01, /*bInterval*/

	0x08, /*bLength*/
	0x25, /*bDescriptorType CS_ENDPOINT (0x25)*/
	0x01, /*bDescriptorSubtype	EP_GENERAL (0x01)*/
	0x00, /**/
	0x00, /*
	bmAttributes.Reserved 0b0000000
	bmAttributes.MaxPacketsOnly	0b0
	*/
	0x02, /*bLockDelayUnits Decoded PCM samples (2)*/
	0x02, /*wLockDelay*/
	0x00, /**/
#endif

#if  MIC_AUDIO_24BIT //size 46
	/***********bInterfaceNumber 2: bAlternateSetting = 2	 ***********************************/

	/* Interface Descriptor*/
	DESC_SIZE_INTF, /*bLength*/
	DESC_TYPE_INTF, /*bDescriptorType INTERFACE (0x04)*/
	INTF_AUDIO_STREAM_MIC,//0x02, /*bInterfaceNumber*/
	AS_INTF_ALT_24BIT,//0x02, /*bAlternateSetting*/
	0x01, /*bNumEndpoints*/
	AUDIO_CLASS, /*bInterfaceClass Audio (0x01)*/
	AUDIO_SUBCLASS_STREAMING, /*bInterfaceSubClass	Audio streaming (0x02)*/
	AUDIO_PROTOCOL_VERSION_02_00, /*bInterfaceProtocol	IP Version 02 00 (0x20)*/
	0x00, /*iInterface*/

	/* Audio AS Descriptor*/
	DESC_SIZE_AS_INTF_GEN_20, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AS_INTF_SUBTYPE_GENERAL, /*bDescriptorSubtype	AS_GENERAL (0x01)*/
	0x06, /*bTerminalLink*/
	0x05, /*bmControls*/
	0x01, /*bFormatType*/
	0x01, /**/
	0x00, /**/
	0x00, /**/
	0x00, /*bmFormats	0b00000000000000000000000000000001*/

	USB_MIC_CHANNELS, /*bNrChannels*/
	LOBYTE(USB_MIC_CHANNEL_SEL),
	HIBYTE(USB_MIC_CHANNEL_SEL),
	0x00, /**/
	0x00, /*bmChannelConfig 0b00000000000000000000000000000001*/
	0x00, /*iChannelNames*/

	/*Type I Format Descriptor*/
	DESC_SIZE_AS_INTF_FMT_20, /*bLength*/
	DESC_TYPE_AUDIO_INTF, /*bDescriptorType CS_INTERFACE (0x24)*/
	AS_INTF_SUBTYPE_FORMAT_TYPE, /*bDescriptorSubtype	FORMAT_TYPE (0x02)*/
	0x01, /*bFormatType FORMAT_TYPE_I (0x01)*/
	0x03, /*bSubslotSize*/
	0x18, /*bBitResolution*/

	DESC_SIZE_EP, /*bLength*/
	DESC_TYPE_EP, /*bDescriptorType ENDPOINT (0x05)*/
	0x82, /*bEndpointAddress	1 IN (0b10000001)*/
	0x0D, //0x01, //0x0D,
	LOBYTE(UAC20_IN_EP_MAX_PKT_SIZE_24BIT),
	HIBYTE(UAC20_IN_EP_MAX_PKT_SIZE_24BIT),

	/*
	wMaxPacketSize.PacketSize	288
	wMaxPacketSize.Transactions One transaction per microframe if HS (0b00)
	*/
	0x01, /*bInterval*/

	/*Audio EP Descriptor*/
	0x08, /*bLength*/
	0x25, /*bDescriptorType CS_ENDPOINT (0x25)*/
	0x01, /*bDescriptorSubtype	EP_GENERAL (0x01)*/
	0x00, /**/
	0x00, /*
	bmAttributes.Reserved	0b0000000
	bmAttributes.MaxPacketsOnly 0b0
	*/
	0x02, /*bLockDelayUnits Decoded PCM samples (2)*/
	0x02, /*wLockDelay*/
	0x00, /**/
#endif
#endif //SUPPORT_USB_MIC

#if SUPPORT_USB_HID
	DESC_SIZE_INTF,                             /* Size of this descriptor in bytes */
	DESC_TYPE_INTF,	                             /* INTERFACE Descriptor Type */
	INTF_HID,                            /* Number of this interface. */
	0x00U,							                             /* Value used to select this alternate setting
	                    for the interface identified in the prior field */
	USB_HID_KEYBOARD_ENDPOINT_COUNT,                             /* Number of endpoints used by this interface (excluding endpoint zero). */
	HID_CLASS, 		                             /* Class code (assigned by the USB-IF). */
	0x00U,		                             					 /* Subclass code (assigned by the USB-IF). */
	0X00U,		                             					 /* Protocol code (assigned by the USB). */
	0x00U,							                             /* Index of string descriptor describing this interface */


	DESC_SIZE_HID,		                             /* Numeric expression that is the total size of the HID descriptor. */
	DESC_TYPE_HID,		                             /* Constant name specifying type of HID descriptor. */
	0x11U, 0x01U,					                             /* Numeric expression identifying the HID Class Specification release. */
	0x00U,							                             /* Numeric expression identifying country code of the localized hardware */
	0x01U,							                             /* Numeric expression specifying the number of
	                  class descriptors(at least one report descriptor) */
	DESC_TYPE_HID_REPORT,                              /* Constant name identifying type of class descriptor. */
	USB_SHORT_GET_LOW(DESC_SIZE_HID_KEYBOARD_REPORT),/* Numeric expression that is the total size of the Report descriptor. */
	USB_SHORT_GET_HIGH(DESC_SIZE_HID_KEYBOARD_REPORT),


	/*OUT*/
	0x07, 							 		 					  /* Size of this descriptor in bytes */
	DESC_TYPE_EP,								  /* endpoint Descriptor Type */

	AUDIO_HID_OUT_EP,					                          /* The address of the endpoint on the USB device
	                             described by this descriptor. */
	EP_TYPE_INTR, 									  /* This field describes the endpoint's attributes */
	0x40,														  /* Maximum packet size this endpoint is capable of
	                             sending or receiving when this configuration is selected. */
	0x00,

	0x20,												           /* Interval for polling endpoint for data transfers. */



	/*HID KEY IN*/
	DESC_SIZE_EP,                               /* Size of this descriptor in bytes */
	DESC_TYPE_EP,	                              /* endpoint Descriptor Type */
	AUDIO_HID_IN_EP,
	/* The address of the endpoint on the USB device
	           described by this descriptor. */
	EP_TYPE_INTR,                                       /* This field describes the endpoint's attributes */
	0x40,                                                         /* Maximum packet size this endpoint is capable of
	           sending or receiving when this configuration is selected. */

	0x00,

	0x04,                                                        /* Interval for polling endpoint for data transfers. */

#endif //SUPPORT_USB_HID
	};



uint8_t usbd_audio_spker_sample_rate_default[] =
{
    USB_SPKER_SUPPORT_SAMPLE_RATE_NUM,							 /* wNumSubRanges */
    #if SPK_FS_48K
    0x00,0x80,0xbb,0x00,
    0x00,0x80,0xbb,0x00,
    0x00,0x00,0x00,0x00,
    #elif SPK_FS_44P1K
    0x00,0x44,0xac,0x00,
    0x00,0x44,0xac,0x00,
    0x00,0x00,0x00,0x00,
    #elif SPK_FS_88P2K
    0x00,0x88,0x58,0x01,
    0x00,0x88,0x58,0x01,
    0x00,0x00,0x00,0x00,
    #elif SPK_FS_96K
    0x00,0x00,0x77,0x01,
    0x00,0x00,0x77,0x01,
    0x00,0x00,0x00,0x00,
    #elif SPK_FS_176P4K
    0x00,0x10,0xb1,0x02,
    0x00,0x10,0xb1,0x02,
    0x00,0x00,0x00,0x00,
    #elif SPK_FS_192K
    0x00,0x00,0xEE,0x02,
    0x00,0x00,0xEE,0x02,
    0x00,0x00,0x00,0x00,

    #elif SPEAKER_FS_352K8
    0x00,0x20,0x62,0x05,
    0x00,0x20,0x62,0x05,
    0x00,0x00,0x00,0x00,
    #elif SPEAKER_FS_384K
    0x00,0x00,0xDC,0x05,
    0x00,0x00,0xDC,0x05,
    0x00,0x00,0x00,0x00,
    #else
    0x00,0x80,0xbb,0x00,
    0x00,0x80,0xbb,0x00,
    0x00,0x00,0x00,0x00,
    #endif
    0x00,
};

uint8_t usbd_audio_spker_sample_rate_list[] =
{
    USB_SPKER_SUPPORT_SAMPLE_RATE_NUM,							 /* wNumSubRanges */
    #if SPK_FS_8K
    0x00,0x40,0x1f,0x00,			                                     /* dMIN(1) */
    0x00,0x40,0x1f,0x00,			                                     /* dMAX(1) */
    0x00,0x00,0x00,0x00,			                                     /* dRES(1) */
    #endif
    #if SPK_FS_16K
    0x00,0x80,0x3E,0x00,			                                     /* dMIN(2) */
    0x00,0x80,0x3E,0x00,			                                     /* dMAX(2) */
    0x00,0x00,0x00,0x00,			                                     /* dRES(2) */
    #endif
    #if	SPK_FS_32K
    0x00,0x00,0x7d,0x00,
    0x00,0x00,0x7d,0x00,
    0x00,0x00,0x00,0x00,
    #endif
    #if SPK_FS_44P1K
    0x00,0x44,0xac,0x00,
    0x00,0x44,0xac,0x00,
    0x00,0x00,0x00,0x00,
    #endif
    #if SPK_FS_48K
    0x00,0x80,0xbb,0x00,
    0x00,0x80,0xbb,0x00,
    0x00,0x00,0x00,0x00,
    #endif
    #if SPK_FS_88P2K
    0x00,0x88,0x58,0x01,
    0x00,0x88,0x58,0x01,
    0x00,0x00,0x00,0x00,
    #endif

    #if SPK_FS_96K
    0x00,0x00,0x77,0x01,
    0x00,0x00,0x77,0x01,
    0x00,0x00,0x00,0x00,
    #endif

    #if SPK_FS_176P4K
    0x00,0x10,0xb1,0x02,
    0x00,0x10,0xb1,0x02,
    0x00,0x00,0x00,0x00,
    #endif

    #if SPK_FS_192K
    0x00,0x00,0xEE,0x02,
    0x00,0x00,0xEE,0x02,
    0x00,0x00,0x00,0x00,
    #endif

    #if SPEAKER_FS_352K8
    0x00,0x20,0x62,0x05,
    0x00,0x20,0x62,0x05,
    0x00,0x00,0x00,0x00,
    #endif

    #if SPEAKER_FS_384K
    0x00,0x00,0xDC,0x05,
    0x00,0x00,0xDC,0x05,
    0x00,0x00,0x00,0x00,
    #endif
    0x00,
};

uint8_t usbd_audio_mic_sample_rate_default[] =
{
    UAC_MIC_SUPPORT_SAMPLE_RATE_NUM, 						 /* wNumSubRanges */
    #if MIC_FS_48K
    0x00,0x80,0xbb,0x00,
    0x00,0x80,0xbb,0x00,
    0x00,0x00,0x00,0x00,
    #elif MIC_FS_44P1K
    0x00,0x44,0xac,0x00,
    0x00,0x44,0xac,0x00,
    0x00,0x00,0x00,0x00,
    #elif MIC_FS_88P2K
    0x00,0x88,0x58,0x01,
    0x00,0x88,0x58,0x01,
    0x00,0x00,0x00,0x00,
    #elif MIC_FS_96K
    0x00,0x00,0x77,0x01,
    0x00,0x00,0x77,0x01,
    0x00,0x00,0x00,0x00,
    #elif MIC_FS_192K
    0x00,0x00,0xEE,0x02,
    0x00,0x00,0xEE,0x02,
    0x00,0x00,0x00,0x00,
    #else
    0x00,0x80,0xbb,0x00,
    0x00,0x80,0xbb,0x00,
    0x00,0x00,0x00,0x00,
    #endif
    0x00,
};


uint8_t usbd_audio_mic_sample_rate_list[] = {
    UAC_MIC_SUPPORT_SAMPLE_RATE_NUM,							 /* wNumSubRanges */
    #if MIC_FS_8K
    0x00,0x40,0x1f,0x00,			                                 /* dMIN(1) */
    0x00,0x40,0x1f,0x00,			                                 /* dMAX(1) */
    0x00,0x00,0x00,0x00,			                                 /* dRES(1) */
    #endif
    #if MIC_FS_16K
    0x00,0x80,0x3E,0x00,			                                  /* dMIN(2) */
    0x00,0x80,0x3E,0x00,			                                  /* dMAX(2) */
    0x00,0x00,0x00,0x00,			                                  /* dRES(2) */
    #endif
    #if MIC_FS_32K
    0x00,0x00,0x7d,0x00,
    0x00,0x00,0x7d,0x00,
    0x00,0x00,0x00,0x00,
    #endif
    #if MIC_FS_44P1K
    0x00,0x44,0xac,0x00,
    0x00,0x44,0xac,0x00,
    0x00,0x00,0x00,0x00,
    #endif
    #if MIC_FS_48K
    0x00,0x80,0xbb,0x00,
    0x00,0x80,0xbb,0x00,
    0x00,0x00,0x00,0x00,
    #endif
    #if MIC_FS_88P2K
    0x00,0x88,0x58,0x01,
    0x00,0x88,0x58,0x01,
    0x00,0x00,0x00,0x00,
    #endif
    #if MIC_FS_96K
    0x00,0x00,0x77,0x01,
    0x00,0x00,0x77,0x01,
    0x00,0x00,0x00,0x00,
    #endif
	#if MIC_FS_176P4K
    0x00,0x10,0xb1,0x02,
    0x00,0x10,0xb1,0x02,
    0x00,0x00,0x00,0x00,
	#endif
    #if MIC_FS_192K
    0x00,0x00,0xEE,0x02,
    0x00,0x00,0xEE,0x02,
    0x00,0x00,0x00,0x00,
    #endif

    0x00,



};

uint8_t get_spker_range_feature_unitid[GET_RANGE_SPKER_FUATURE_LEN]  =
{
	0x01,
	0x00,
    LOBYTE(USB_DEV_SPK_VOL_MIN),
    HIBYTE(USB_DEV_SPK_VOL_MIN),
    LOBYTE(USB_DEV_SPK_VOL_MAX),
    HIBYTE(USB_DEV_SPK_VOL_MAX),
    LOBYTE(USB_DEV_SPK_VOL_STEP),
    HIBYTE(USB_DEV_SPK_VOL_STEP),
};

uint8_t get_mic_range_feature_unitid[GET_RANGE_MIC_FUATURE_LEN]  =
{
	0x01,
	0x00,
    LOBYTE(USB_DEV_MIC_VOL_MIN),
    HIBYTE(USB_DEV_MIC_VOL_MIN),
    LOBYTE(USB_DEV_MIC_VOL_MAX),
    HIBYTE(USB_DEV_MIC_VOL_MAX),
    LOBYTE(USB_DEV_MIC_VOL_STEP),
    HIBYTE(USB_DEV_MIC_VOL_STEP),
};

uint8_t get_mixer_range_feature_unitid[GET_RANGE_MIXER_FUATURE_LEN] =
{
    0x01,
    0x00,
    LOBYTE(USB_DEV_SLIDETONE_VOL_MIN),
    HIBYTE(USB_DEV_SLIDETONE_VOL_MIN),
    LOBYTE(USB_DEV_SLIDETONE_VOL_MAX),
    HIBYTE(USB_DEV_SLIDETONE_VOL_MAX),
    LOBYTE(USB_DEV_SLIDETONE_VOL_STEP),
    HIBYTE(USB_DEV_SLIDETONE_VOL_STEP),
};

#endif // SUPPORT_UAC20

#endif
