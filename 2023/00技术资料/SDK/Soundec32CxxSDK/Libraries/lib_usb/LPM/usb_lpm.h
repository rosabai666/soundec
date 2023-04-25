/*
 * usb_lpm.h
 *
 *  Created on: 2020Äê11ÔÂ6ÈÕ
 *      Author: User
 */
#include "types.h"

#if 1//(USB_ENABLE)
#ifndef USB_DEV_LPM_SUPPORT
#define USB_DEV_LPM_SUPPORT
#define USB_SLEEP_CLR_POS                 (0U)
#define USB_SLEEP_CLR_Msk                 (0x1U << USB_SLEEP_CLR_POS)
#define USB_SLEEP_CLR                      USB_SLEEP_CLR_Msk
#define USB_SLEEP_MASK_POS                (1U)
#define USB_SLEEP_MASK_MSK                (0x1U << USB_SLEEP_MASK_POS)
#define USB_SLEEP_MASK                     USB_SLEEP_MASK_MSK
#define USB_SLEEP_INTSRC_POS               (2U)
#define USB_SLEEP_INTSRC_MSK               (0x1U << USB_SLEEP_INTSRC_POS)
#define USB_SLEEP_INTSRC                   USB_SLEEP_INTSRC_MSK
#define USB_SLEEP_INTR_POS                  (3U)
#define USB_SLEEP_INTR_MSK                  (0x1U << USB_SLEEP_INTR_POS)
#define USB_SLEEP_INTR                       USB_SLEEP_INTR_MSK


#define USB_L1_SUSPEND_N_CLR_POS                  (0U)
#define USB_L1_SUSPEND_N_CLR_MSK                  (0x1U << USB_L1_SUSPEND_N_CLR_POS)
#define USB_L1_SUSPEND_N_CLR                       USB_L1_SUSPEND_N_CLR_MSK
#define USB_L1_SUSPEND_N_MASK_POS                  (1U)
#define USB_L1_SUSPEND_N_MASK_MSK                  (0x1U << USB_L1_SUSPEND_N_MASK_POS)
#define USB_L1_SUSPEND_N_MASK                      USB_L1_SUSPEND_N_MASK_MSK
#define USB_L1_SUSPEND_N_INTSRC_POS                (2U)
#define USB_L1_SUSPEND_N_INTSRC_MSK                (0x1U << USB_L1_SUSPEND_N_INTSRC_POS)
#define USB_L1_SUSPEND_N_INTSRC                    USB_L1_SUSPEND_N_INTSRC_MSK
#define USB_L1_SUSPEND_N_INTR_POS                   (3U)
#define USB_L1_SUSPEND_N_INTR_MSK                   (0x1U << USB_L1_SUSPEND_N_INTR_POS)
#define USB_L1_SUSPEND_N_INTR                       USB_L1_SUSPEND_N_INTR_MSK

#define USB_DEV_HIRD_VLD_TGL_CLR_POS               (0U)
#define USB_DEV_HIRD_VLD_TGL_CLR_MSK               (0x1U << USB_DEV_HIRD_VLD_TGL_CLR_POS)
#define USB_DEV_HIRD_VLD_TGL_CLR                   USB_DEV_HIRD_VLD_TGL_CLR_MSK
#define USB_DEV_HIRD_VLD_TGL_MASK_POS              (1U)
#define USB_DEV_HIRD_VLD_TGL_MASK_MSK              (0x1U << USB_DEV_HIRD_VLD_TGL_MASK_POS)
#define USB_DEV_HIRD_VLD_TGL_MASK                   USB_DEV_HIRD_VLD_TGL_MASK_MSK
#define USB_DEV_HIRD_VLD_TGL_INTSRC_POS            (2U)
#define USB_DEV_HIRD_VLD_TGL_INTSRC_MSK            (0x1U << USB_DEV_HIRD_VLD_TGL_INTSRC_POS)
#define USB_DEV_HIRD_VLD_TGL_INTSRC                 USB_DEV_HIRD_VLD_TGL_INTSRC_MSK
#define USB_DEV_HIRD_VLD_TGL_INTR_POS               (3U)
#define USB_DEV_HIRD_VLD_TGL_INTR_MSK               (0x1U << USB_DEV_HIRD_VLD_TGL_INTR_POS)
#define USB_DEV_HIRD_VLD_TGL_INTR                    USB_DEV_HIRD_VLD_TGL_INTR_MSK

#define USB_DEV_HIRD_VLD_TGL_COUNT_SET_POS         (0U)
#define USB_DEV_HIRD_VLD_TGL_COUNT_SET_MSK         (0xFU << USB_DEV_HIRD_VLD_TGL_COUNT_SET_POS)
#define USB_DEV_HIRD_VLD_TGL_COUNT_SET              USB_DEV_HIRD_VLD_TGL_COUNT_SET_MSK
#define USB_DEV_HIRD_VLD_TGL_COUNT_POS             (4U)
#define USB_DEV_HIRD_VLD_TGL_COUNT_MSK             (0xFU << USB_DEV_HIRD_VLD_TGL_COUNT_POS)
#define USB_DEV_HIRD_VLD_TGL_COUNT                  USB_DEV_HIRD_VLD_TGL_COUNT_MSK

#define USB_INTSRC_USB_SLEEP_ENBALE_POS            (1U)
#define USB_INTSRC_USB_SLEEP_ENBALE_MSK            (0x1U << USB_INTSRC_USB_SLEEP_ENBALE_POS)
#define USB_INTSRC_USB_SLEEP_ENBALE                 USB_INTSRC_USB_SLEEP_ENBALE_MSK



typedef struct{
    __IO uint32_t CTL_SLEEP;
    __IO uint32_t CTL_SUSPEND;
    __IO uint32_t CTL_HIRD0;
    __IO uint32_t CTL_HIRD1;
    __IO uint32_t CTL_SLEEP_EN;

}slp_t;
#define USB_DEV_LPM_INT_BASE  0x75200000
#define USB_DEV_INT                 ((slp_t *)USB_DEV_LPM_INT_BASE)
#endif


#endif /*  */
