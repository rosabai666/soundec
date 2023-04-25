
#ifndef __HAL_CACHE_INTERNAL_H__
#define __HAL_CACHE_INTERNAL_H__ 
#include "hal_cache.h"

#ifdef HAL_CACHE_MODULE_ENABLED

#include "hal_interrupt.h"

#define HAL_CACHE_REGION_SIZE_UNIT  (0x20000000)    /**< CACHE region size must be 512MB aligned */

#define XCHAL_CA_BYPASS         0x2
#define XCHAL_CA_WRITEBACK      0x4

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	HAL_CACHE_IDLE = 0,
	HAL_CACHE_INIT,
	HAL_CACHE_ENABLE,
	HAL_CACHE_DISABLE
} hal_cache_module_status_t;

typedef struct {
	uint32_t start_entry;
	uint32_t end_entry;
} hal_cache_region_entry_t;

#ifdef __cplusplus
}
#endif

#endif /* HAL_CACHE_MODULE_ENABLED */
#endif /* __HAL_CACHE_INTERNAL_H__ */

