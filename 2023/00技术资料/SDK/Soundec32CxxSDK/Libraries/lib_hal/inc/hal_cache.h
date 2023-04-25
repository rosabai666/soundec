/** 
***********************************************************
 * 
 * @file name	: hal_cache.h
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-02-20
 * @brief		: HAL Cache Driver Header file
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
 * and Soundec Co., Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 * 
***********************************************************/


#ifndef __HAL_CACHE_H__
#define __HAL_CACHE_H__
#include "snc8600.h"
#include "hal.h"

#ifdef HAL_CACHE_MODULE_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  This macro defines the CACHE line size.
 */
#define HAL_CACHE_LINE_SIZE         (64)


/*****************************************************************************
 * Enums
 *****************************************************************************/

/** 
 * @brief This enum defines the CACHE size. 
 */
typedef enum {
    HAL_CACHE_SIZE_48KB = 1,                    /**< CACHE size is 48kB. */
    HAL_CACHE_SIZE_MAX                          /**< The total number of CACHE sizes (invalid size). */
} hal_cache_size_t;


/** 
 * @brief This enum defines the CACHE region number. 
 */
typedef enum {
    HAL_CACHE_REGION_0 = 0,                     /**< CACHE region 0. */
    HAL_CACHE_REGION_1 = 1,                     /**< CACHE region 1. */
    HAL_CACHE_REGION_2 = 2,                     /**< CACHE region 2. */
    HAL_CACHE_REGION_3 = 3,                     /**< CACHE region 3. */
    HAL_CACHE_REGION_4 = 4,                     /**< CACHE region 4. */
    HAL_CACHE_REGION_5 = 5,                     /**< CACHE region 5. */
    HAL_CACHE_REGION_6 = 6,                     /**< CACHE region 6. */
    HAL_CACHE_REGION_7 = 7,                     /**< CACHE region 7. */
    HAL_CACHE_REGION_MAX                        /**< The total number of CACHE regions (invalid region). */
} hal_cache_region_t;


/*****************************************************************************
 * Structures
 *****************************************************************************/

/** 
 * @brief The CACHE region configuration structure that contains the start address and the size of the region, both must be 512MB aligned. 
 */
typedef struct {
    uint32_t cache_region_address;              /**< Starting address of the CACHE region. */
    uint32_t cache_region_size;                 /**< The CACHE region size. */
} hal_cache_region_config_t;


/*****************************************************************************
 * Functions
 *****************************************************************************/

/** 
 * @brief	This function initializes the I-CACHE and the D-CACHE, sets the default Cache attribute for the memory layout.
 * @note	The current Cache module status should be HAL_CACHE_IDLE.
 * @return	Return the status.
 * 	@retval HAL_STATUS_OK The CACHE is successfully initialized.
 * 	@retval HAL_STATUS_ERROR The current Cache module status isn't HAL_CACHE_IDLE, the Cache has been initialized.
 */
hal_status_t hal_cache_init(void);

/** 
 * @brief	This function deinitializes the I-CACHE and the D-CACHE, reset the CACHE module to its default state.
 * @note	The current Cache module status should not be HAL_CACHE_ENABLE, or else, please disable the Cache firstly.
 * @return	Return the status.
 * 	@retval HAL_STATUS_OK The CACHE is successfully deinitialized.
 * 	@retval HAL_STATUS_ERROR The current Cache module status is HAL_CACHE_ENABLE, will not deinitialize it.
 */
hal_status_t hal_cache_deinit(void);

/** 
 * @brief	This function enables the I-CACHE and the D-CACHE controllers. @sa hal_cache_region_enable().
 * @return	Return the status.
 * 	@retval HAL_STATUS_OK The CACHE is successfully enabled.
 * 	@retval HAL_STATUS_ERROR The Cache hasn't been initialized.
 */
hal_status_t hal_cache_enable(void);

/** 
 * @brief	This function disables the I-CACHE and the D-CACHE controllers. @sa hal_cache_region_disable().
 * @return	Return the status.
 * 	@retval HAL_STATUS_OK The CACHE is successfully disabled.
 * 	@retval HAL_STATUS_ERROR The Cache hasn't been initialized.
 */
hal_status_t hal_cache_disable(void);

/** 
 * @brief	This function enables a single I-CACHE and D-CACHE region. 
 *			Once hal_cache_enable() is called, the settings of the specified region take effect. @sa hal_cache_enable().
 * @param[in]	region      It is the region needs to be enabled, this parameter can only be a value of type #hal_cache_region_t.
 * @return	Returns the status.
 * 	@retval HAL_STATUS_OK The CACHE region is successfully enabled.
 * 	@retval HAL_STATUS_ERROR_CACHE_REGION_ADDRESS The region address is not 512MB aligned
 *  @retval HAL_STATUS_ERROR_CACHE_REGION_SIZE The region size is not 512MB
 */
hal_status_t hal_cache_region_enable(hal_cache_region_t region);

/** 
 * @brief	This function disables a single I-CACHE and D-CACHE region. When this function is called, the CACHE settings of corresponding region are disabled, even if the hal_cache_enable() function is called. @sa hal_cache_disable().
 * @param[in]	region      It is the region needs to be disabled, this parameter can only be a value of type #hal_cache_region_t.
 * @return	Returns the status.
 * 	@retval HAL_STATUS_OK The CACHE region is successfully disabled.
 * 	@retval HAL_STATUS_ERROR The Cache hasn't been initialized.
 */
hal_status_t hal_cache_region_disable(hal_cache_region_t region);

/** 
 * @brief	This function configures the I-CACHE and the D-CACHE.
 * @note	{description}
 * @param[in]	region      The region to configure.
 * @param[in]	region_config The configuration information of the region.
 * @return	
 * 	@retval HAL_STATUS_OK The CACHE region is successfully configured
 *  @retval HAL_STATUS_ERROR_CACHE_REGION_ADDRESS The region address is not 512MB aligned
 *  @retval HAL_STATUS_ERROR_CACHE_REGION_SIZE The region size is not 512MB
 */
hal_status_t hal_cache_region_config(hal_cache_region_t region, const hal_cache_region_config_t *region_config);

/** 
 * @brief	This function sets the total size of the I-CACHE and the total size of the D-CACHE.
 * @note	{description}
 * @param[in]	cache_size  Should only be any value of type #hal_cache_size_t.
 * @return	
 * 	@retval HAL_STATUS_OK Set the CACHE size successfully.
 * 	@retval HAL_STATUS_ERROR_CACHE_SIZE The CACHE size is invalid.
 */
hal_status_t hal_cache_set_size(hal_cache_size_t cache_size);

/** 
 * @brief	This function invalidates a I-CACHE line and a D-CACHE line at a given address.
 * @param[in]	address     The start address of the memory that is invalidated, should be cache line size aligned
 * @return	
 * 	@retval HAL_CACHE_STATUS_OK The CACHE line is successfully invalidated.
 */
hal_status_t hal_cache_invalidate_one_cache_line(uint32_t address);

/** 
 * @brief	This function invalidates the I-CACHE lines and D-CACHE lines by address and length.
 * @param[in]	address     The start address of the memory that is invalidated, should be cache line size aligned.
 * @param[in]	length      The length of memory that to be invalidated, should be cache line size aligned.
 * @return	
 * 	@retval HAL_STATUS_OK The CACHE is successfully invalidated.
 */
hal_status_t hal_cache_invalidate_multiple_cache_lines(uint32_t address, uint32_t length);

/** 
 * @brief	This function invalidates the whole I-CACHE and D-CACHE.
 * @return	
 * 	@retval HAL_STATUS_OK The CACHE is successfully invalidated.
 */
hal_status_t hal_cache_invalidate_all_cache_lines(void);

/** 
 * @brief	This function flushes one D-CACHE line by address.
 * @param[in]	address     The start address of the memory that is flushed, should be cache line size aligned.
 * @return	
 * 	@retval HAL_STATUS_OK The CACHE is successfully flushed.
 */
hal_status_t hal_cache_flush_one_cache_line(uint32_t address);

/** 
 * @brief	This function flushes the D-CACHE lines by address and length.
 * @param[in]	address     The start address of the memory to flush, should be cache line size aligned.
 * @param[in]	length      The length of the memory to be flushed, should be cache line size aligned.
 * @return	
 * 	@retval HAL_STATUS_OK The CACHE is successfully flushed.
 */
hal_status_t hal_cache_flush_multiple_cache_lines(uint32_t address, uint32_t length);

/** 
 * @brief	This function flushes the whole D-CACHE.
 * @return	
 * 	@retval HAL_STATUS_OK The CACHE is successfully flushed.
 */
hal_status_t hal_cache_flush_all_cache_lines(void);

/** 
 * @brief	This function checks whether the given memory is cacheable.
 * @note	{description}
 * @param[in]	address     The memory address to check.
 * @return	
 * 	@retval true The memory is cacheable.
 * 	@retval false The memory is non-cacheable.
 */
bool hal_cache_is_cacheable(uint32_t address);



#ifdef __cplusplus
}
#endif

/**
 * @}
 * @}
*/
#endif /*HAL_CACHE_MODULE_ENABLED*/
#endif /* __HAL_CACHE_H__ */


