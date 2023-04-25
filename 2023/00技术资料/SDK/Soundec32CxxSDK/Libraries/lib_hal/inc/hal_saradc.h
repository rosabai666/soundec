/** 
***********************************************************
 * 
 * @file name	: hal_saradc.h
 * @author		: RandyFan
 * @version		: V02.01
 * @date		: 2022-02-18
 * @brief		: HAL SAR ADC Driver Head File
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
#ifndef __HAL_SARADC_H__
#define __HAL_SARADC_H__

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "snc8600.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private macro -------------------------------------------------------------*/

/**
 * @name SARADC_REG
 * @brief Read/Write SAR ADC Registers
 * @{
 */
#define SARADC_REG                  ((volatile saradc_regs_t *)SARADC_BASE)
#define SARADC_Read(reg)			(SARADC_REG->reg)
#define SARADC_Write(reg, val)		(SARADC_REG->reg = (val))
/** @} SARADC_REG */

/*! @name SARADC_REG_DOMAIN
 * @brief Shifts and Masks of SAR ADC Registers
 * @{
 */
#define HAL_ADC_ENABLE_MASK                           (0x1U)
#define HAL_ADC_ENABLE_SHIFT                          (0U)
#define HAL_ADC_ENABLE(x)                             (((uint32_t)(((uint32_t)(x)) << HAL_ADC_ENABLE_SHIFT)) & HAL_ADC_ENABLE_MASK)

#define HAL_ADC_RES_MASK                              (0x3U)
#define HAL_ADC_RES_SHIFT                             (0U)
#define HAL_ADC_RES(x)                                (((uint32_t)(((uint32_t)(x)) << HAL_ADC_RES_SHIFT)) & HAL_ADC_RES_MASK)

#define HAL_ADC_OUTPUT_MASK                           (0xFFFU)
#define HAL_ADC_OUTPUT_SHIFT                          (0U)
#define HAL_ADC_OUTPUT(x)                             (((uint32_t)(((uint32_t)(x)) << HAL_ADC_OUTPUT_SHIFT)) & HAL_ADC_OUTPUT_MASK)

#define HAL_ADC_WORKMODE_MASK                         (0x3U)
#define HAL_ADC_WORKMODE_SHIFT                        (0U)
#define HAL_ADC_WORKMODE(x)                           (((uint32_t)(((uint32_t)(x)) << HAL_ADC_WORKMODE_SHIFT)) & HAL_ADC_WORKMODE_MASK)

#define HAL_ADC_SAMPLING_RAT_MASK                     (0xFFFFFU)
#define HAL_ADC_SAMPLING_RAT_SHIFT                    (0U)
#define HAL_ADC_SAMPLING_RAT(x)                       (((uint32_t)(((uint32_t)(x)) << HAL_ADC_SAMPLING_RAT_SHIFT)) & HAL_ADC_SAMPLING_RAT_MASK)

#define HAL_ADC_CHANNEL_MASK                          (0x1U)
#define HAL_ADC_CHANNEL_SHIFT                         (0U)
#define HAL_ADC_CHANNEL(x)                            (((uint32_t)(((uint32_t)(x)) << HAL_ADC_CHANNEL_SHIFT)) & HAL_ADC_CHANNEL_MASK)

#define HAL_ADC_VINPUT_MASK                           (0x1U)
#define HAL_ADC_VINPUT_SHIFT                          (0U)
#define HAL_ADC_VINPUT(x)                             (((uint32_t)(((uint32_t)(x)) << HAL_ADC_VINPUT_SHIFT)) & HAL_ADC_VINPUT_MASK)

#define HAL_ADC_BVOSI_MASK                            (0x7fU)
#define HAL_ADC_BVOSI_SHIFT                           (0U)
#define HAL_ADC_BVOSI(x)                              (((uint32_t)(((uint32_t)(x)) << HAL_ADC_BVOSI_SHIFT)) & HAL_ADC_BVOSI_MASK)

#define HAL_ADC_BVOS_MASK                             (0x7fU)
#define HAL_ADC_BVOS_SHIFT                            (0U)
#define HAL_ADC_BVOS(x)                               (((uint32_t)(((uint32_t)(x)) << HAL_ADC_BVOS_SHIFT)) & HAL_ADC_BVOS_MASK)

#define HAL_ADC_BYPASS_MASK                           (0x1U)
#define HAL_ADC_BYPASS_SHIFT                          (0U)
#define HAL_ADC_BYPASS(x)                             (((uint32_t)(((uint32_t)(x)) << HAL_ADC_BYPASS_SHIFT)) & HAL_ADC_BYPASS_MASK)

#define HAL_ADC_LDO_READY_MASK                        (0x1U)
#define HAL_ADC_LDO_READY_SHIFT                       (0U)
#define HAL_ADC_LDO_READY(x)                          (((uint32_t)(((uint32_t)(x)) << HAL_ADC_LDO_READY_SHIFT)) & HAL_ADC_LDO_READY_MASK)

#define HAL_ADC_EOC_MASK                              (0x1U)
#define HAL_ADC_EOC_SHIFT                             (0U)
#define HAL_ADC_EOC(x)                                (((uint32_t)(((uint32_t)(x)) << HAL_ADC_EOC_SHIFT)) & HAL_ADC_EOC_MASK)

#define HAL_ADC_SOC_MASK                              (0x1U)
#define HAL_ADC_SOC_SHIFT                             (0U)
#define HAL_ADC_SOC(x)                                (((uint32_t)(((uint32_t)(x)) << HAL_ADC_SOC_SHIFT)) & HAL_ADC_SOC_MASK)

#define HAL_ADC_CALON_MASK                            (0x1U)
#define HAL_ADC_CALON_SHIFT                           (0U)
#define HAL_ADC_CALON(x)                              (((uint32_t)(((uint32_t)(x)) << HAL_ADC_CALON_SHIFT)) & HAL_ADC_CALON_MASK)

#define HAL_ADC_LDO_DELAY_MASK                        (0xFFFU)
#define HAL_ADC_LDO_DEADY_SHIFT                       (0U)
#define HAL_ADC_LDO_DEADY(x)                          (((uint32_t)(((uint32_t)(x)) << HAL_ADC_LDO_DEADY_SHIFT)) & HAL_ADC_LDO_DELAY_MASK)

#define HAL_ADC_INTR_MASK_MASK                        (0x1U)
#define HAL_ADC_INTR_MASK_SHIFT                       (0U)
#define HAL_ADC_INTR_MASK(x)                          (((uint32_t)(((uint32_t)(x)) << HAL_ADC_INTR_MASK_SHIFT)) & HAL_ADC_INTR_MASK_MASK)

#define HAL_ADC_INTR_CLR_MASK                         (0x1U)
#define HAL_ADC_INTR_CLR_SHIFT                        (0U)
#define HAL_ADC_INTR_CLR(x)                           (((uint32_t)(((uint32_t)(x)) << HAL_ADC_INTR_CLR_SHIFT)) & HAL_ADC_INTR_CLR_MASK)
/** @} SARADC_REG_DOMAIN */

/* Private typedef -----------------------------------------------------------*/
/**
 * @enum saradc_resolution_t
 * @brief Define SAR ADC Resolution.
 * @ingroup SAR_ADC_Enums
 */
typedef enum
{
    SARADC_RESOLUTION_6_BIT = 0,
    SARADC_RESOLUTION_8_BIT,
    SARADC_RESOLUTION_10_BIT,
    SARADC_RESOLUTION_12_BIT,
}saradc_resolution_t;

/**
 * @enum saradc_work_mode_t
 * @brief Define SAR ADC Workmodes.
 */
typedef enum
{
    SARADC_POWER_ON = 0,
    SARADC_STANDBY,
    SARADC_POWER_DOWN,
}saradc_work_mode_t;

/**
 * @enum saradc_channel_t
 * @brief Define SAR ADC Channel.
 */
typedef enum
{
	 SARADC_FAST_CHANNEL = 0,
	 SARADC_SLOW_CHANNEL,
}saradc_channel_t;

/**
 * @enum saradc_vin_mode_t
 * @brief Define SAR ADC Vinput Mode.
 */
typedef enum
{
	SARADC_SINGLE_ENDED = 0,
	SARADC_DIFFERENTIAL,
}saradc_vin_mode_t;

/**
 * @enum saradc_ldo_ready_t
 * @brief Define SAR ADC Ldo Ready.
 */
typedef enum
{
	SARADC_lDO_READY = 0,
	SARADC_LDO_YET_READY,
}saradc_ldo_ready_t;

/**
 * @enum saradc_samplerate_t
 * @brief Define SAR ADC sample rates.
 */
typedef enum
{
	FS_10KHZ  = 10000u,        /*!< 10khz. */
	FS_20KHZ  = 20000u,        /*!< 20khz. */
	FS_50KHZ  = 50000u,        /*!< 50khz. */
	FS_100KHZ = 100000u,       /*!< 100khz. */
	FS_200KHZ = 200000u,       /*!< 200khz. */
	FS_400KHZ = 400000u,       /*!< 400khz. */
	FS_500KHZ = 500000u,       /*!< 500khz. */
}saradc_samplerate_t;

/**
 * @struct saradc_regs_t
 * @brief Packing Registers of SAR ADC
 * @ingroup Regs
 */
typedef struct {
	  uint32_t reserved0;
__IO  uint32_t SARADC_CLK_ENABLE;	///<offset=0x04, Enable Module
__IO  uint32_t SARADC_RES;          ///<offset=0x08, Config Resolution:\n [1:0]=11:  12bit\n [1:0]=10: 10bit\n [1:0]=01: 8bit\n [1:0]=00: 6bit
      uint32_t reserved1;
__IO  uint32_t SARADC_OUTPUT;       ///<offset=0x10, Data Output
__IO  uint32_t SARADC_WORKMODE ;    ///<offset=0x14, Work Mode:\n [1:0]=10: Power Down\n [1:0]=01: Standby\n [1:0]=00: Power On
__IO  uint32_t SARADC_FS;           ///<offset=0x18, Sampling Rate(fs)
__IO  uint32_t SARADC_CH_MODE;      ///<offset=0x1c, Select Channel:\n 1: Fast Channel\n 0: Slow Channel
__IO  uint32_t SARADC_VIN_MODE;     ///<offset=0x20, Vinput Mode:\n 1: Differential\n 0: Sigle Ended
__IO  uint32_t SARADC_BVOSI ;       ///<offset=0x24, Input of the Digital Offset Calibration Block
__IO  uint32_t SARADC_BVOS ;        ///<offset=0x28, Output of the Digital Offset Calibration Block
__IO  uint32_t SARADC_BYPASS_MODE;  ///<offset=0x2c,\n 1: Bypass the Offset Calibration Block
__IO  uint32_t SARADC_LDO_R;        ///<offset=0x30,\n 1: Ldo Start-up Finished\n 0: Ldo Start_up Unfinished
__I   uint32_t SARADC_EOC;          ///<offset=0x34,\n 1: End of Conversion
__I   uint32_t SARADC_SOC;          ///<offset=0x38,\n 1: Start of Conversion
__IO  uint32_t SARADC_CALON;        ///<offset=0x3c,\n 1: ADC is in Calibration Mode
      uint32_t reserved2;
__IO  uint32_t SARADC_LDO_DELAY;    ///<offset=0x44, the Counter of Ldo Startup */
      uint32_t reserved3;
__IO  uint32_t SARADC_INTR_MASK;    ///<offset=0x4C,\n 1: Mask Interrupt\n 0: Unmask Interrupt
__IO  uint32_t SARADC_INTR_CLR;     ///<offset=0x50,\n 1: Clear Interrupt
	  uint32_t reserved4;
	  uint32_t reserved5;
__IO  uint32_t SARADC_SELRAN_LDO;   ///<offset=0x5C,\n 1: DVDD 0.99 ~1.21V\n 0: DVDD 0.81~0.99V
}saradc_regs_t;

/* Private functions ---------------------------------------------------------*/
/**
 * @brief This function enables SAR ADC
 */
static __inline__ void hal_saradc_enable(void)
{
	SARADC_Write(SARADC_CLK_ENABLE, 0x01);
}

/**
 * @brief This function disables SAR ADC
 */
static __inline__ void hal_saradc_disable(void)
{
	SARADC_Write(SARADC_CLK_ENABLE, 0x00);
}

/**
 * @brief This function configs SAR ADC's resolution
 * param[in] res resolution
 */
static __inline__ void hal_saradc_set_res(uint8_t res)
{
	SARADC_Write(SARADC_RES, HAL_ADC_RES(res));
}

/**
 * @brief This function gets SAR ADC's resolution
 * @return resolution
 */
static __inline__ uint8_t hal_saradc_get_res(void)
{
	return SARADC_Read(SARADC_RES) & HAL_ADC_RES_MASK;
}

/**
 * @brief This function configs SAR ADC's sampling rate
 * param[in] fs sampling rate
 */
static __inline__ void hal_saradc_set_fs(uint32_t fs)
{
	SARADC_Write(SARADC_FS, HAL_ADC_SAMPLING_RAT(fs));
}

/**
 * @brief This function gets SAR ADC's sampling rate
 * @return sampling rate
 */
static __inline__ uint32_t hal_saradc_get_fs(void)
{
	return SARADC_Read(SARADC_FS);
}

/**
 * @brief This function configs SAR ADC's work mode
 * param[in] mode work mode
 */
static __inline__ void hal_saradc_set_workmode(uint8_t mode)
{
	SARADC_Write(SARADC_WORKMODE, HAL_ADC_WORKMODE(mode));
}

/**
 * @brief This function gets SAR ADC's work mode
 * return work mode
 */
static __inline__ uint8_t hal_saradc_get_workmode(void)
{
	return SARADC_Read(SARADC_WORKMODE);
}

/**
 * @brief This function configs SAR ADC's channel mode
 * param[in] mode channel mode
 */
static __inline__ void hal_saradc_set_channel_mode(bool mode)
{
	SARADC_Write(SARADC_CH_MODE, HAL_ADC_CHANNEL(mode));
}

/**
 * @brief This function gets SAR ADC's channel mode
 * return channel mode
 */
static __inline__ bool hal_saradc_get_channel_mode(void)
{
	return SARADC_Read(SARADC_CH_MODE) & 0x01;
}

/**
 * @brief This function configs SAR ADC's Vinput mode
 * param[in] mode Vinput mode
 */
static __inline__ void hal_saradc_set_vin_mode(uint8_t mode)
{
	SARADC_Write(SARADC_VIN_MODE, HAL_ADC_VINPUT(mode));
}

/**
 * @brief This function gets SAR ADC's Ldo status
 * return Ldo status, 1: ready, 0: unready
 */
static __inline__ bool hal_saradc_get_ldo_ready(void)
{
	return SARADC_Read(SARADC_LDO_R) & HAL_ADC_LDO_READY_MASK;
}

/**
 * @brief This function gets SAR ADC's data
 * return data
 */
static __inline__ uint16_t hal_saradc_get_val(void)
{
	return SARADC_Read(SARADC_OUTPUT) & HAL_ADC_OUTPUT_MASK;
}

/**
 * @brief This function configs SAR ADC's calibration
 * param[in] cal calibration
 */
static __inline__ void hal_saradc_set_calibration(uint8_t cal)
{
	SARADC_Write(SARADC_BVOSI, HAL_ADC_BVOSI(cal));
}

/**
 * @brief This function gets SAR ADC's calibration
 * return calibration
 */
static __inline__ uint8_t hal_saradc_get_calibration(void)
{
	return SARADC_Read(SARADC_BVOS) & HAL_ADC_BVOSI_MASK;
}

/**
 * @brief This function gets SAR ADC's calibration status
 * return calibration status, 1: unfinished, 0: finished
 */
static __inline__ bool hal_saradc_get_calibration_status(void)
{
	return SARADC_Read(SARADC_CALON) & HAL_ADC_CALON_MASK;
}

/**
 * @brief This function gets SAR ADC's EOC status
 * return EOC status, 1: End of Conversion
 */
static __inline__ bool hal_saradc_get_eoc_status(void)
{
	return SARADC_Read(SARADC_EOC) & HAL_ADC_EOC_MASK;
}

/**
 * @brief This function gets SAR ADC's SOC status
 * return SOC status, 1: Start of Conversion
 */
static __inline__ bool hal_saradc_get_soc_status(void)
{
	return SARADC_Read(SARADC_SOC) & HAL_ADC_SOC_MASK;
}

/**
 * @brief This function clears SAR ADC's interrupt
 */
static __inline__ void hal_saradc_clr_irq(void)
{
	SARADC_Write(SARADC_INTR_CLR, HAL_ADC_INTR_CLR(1));
}

/**
 * @brief This function enables SAR ADC's interrupt
 */
static __inline__ void hal_saradc_enable_intr(void)
{
	SARADC_Write(SARADC_INTR_MASK, 0);
}

/**
 * @brief This function bypasses SAR ADC's calibration
 * param[in] bypass 1:bypass mode
 */
static __inline__ void hal_saradc_set_bypass(bool bypass)
{
	SARADC_Write(SARADC_BYPASS_MODE, bypass);
}

#ifdef __cplusplus
}
#endif

#endif  /* __HAL_SARADC_H__ */
