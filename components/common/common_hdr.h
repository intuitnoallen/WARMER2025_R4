/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : common_hdr.h
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 May 15
**  @brief      : Common header of all modules
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  Common
** @{
*/

#ifndef __COMMON_HDR_H__
#define __COMMON_HDR_H__

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           INCLUDES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <stdint.h>                 /* Standard types */
#include <stdbool.h>                /* Boolean type */
#include <stddef.h>                 /* NULL definition */
#include <stdlib.h>                 /* abort() function */

#include "esp_log.h"                /* Use logging component from ESP-IDF */
#include "app_cfg.h"                /* Common definitions */

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           DEFINES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#define USE_MODULE_ASSERT
#ifdef USE_MODULE_ASSERT

/** @brief  The ASSERT_PARAM macro is used for function's parameters check */
/** @param  expr: If false, it traps the program. If true, it returns no value */
#define ASSERT_PARAM(expr)                                                              \
    if (!(expr))                                                                        \
    {                                                                                   \
        ESP_LOGE (TAG, "Assertion failed at line %d, file %s", __LINE__, __FILE__);     \
        abort();                                                                        \
    }
#else
    #define ASSERT_PARAM(expr) ((void)0)
#endif

/** @brief  Common status codes */
enum
{
    STATUS_OK               = 0,        //!< Success
    STATUS_ERR              = -1,       //!< A general error has occurred
    STATUS_ERR_NOT_INIT     = -2,       //!< Error reason: not initialized yet
    STATUS_ERR_BUSY         = -3,       //!< Error reason: busy
};

/** @brief  Macro to set bits of a left value */
#define SET_BITS(lvalue, bitmask)           (lvalue |= (bitmask))

/** @brief  Macro to clear bits of a left value */
#define CLR_BITS(lvalue, bitmask)           (lvalue &= ~(bitmask))

/** @brief  Macro to invert bits of a left value */
#define INV_BITS(lvalue, bitmask)           (lvalue ^= (bitmask))

/** @brief  Macro to check if all given bits of a variable are set */
#define ALL_BITS_SET(var, bitmask)          (((var) & (bitmask)) == (bitmask))

/** @brief  Macro to check if any of the given bits of a variable is/are set */
#define ANY_BITS_SET(var, bitmask)          (((var) & (bitmask)) != 0)

/** @brief  Macro to check if all given bits of a variable are reset */
#define ALL_BITS_CLR(var, bitmask)          (((var) & (bitmask)) == 0)

/** @brief  Macro to check if any of the given bits of a variable is/are reset */
#define ANY_BITS_CLR(var, bitmask)          (((var) & (bitmask)) != (bitmask))

/** @brief   Macro to convert a 2-byte buffer to an uint16_t value (little endian format) */
#define ENDIAN_GET16(any_addr)              (((uint16_t)((uint8_t *)(any_addr))[1] << 8) | \
                                             ((uint16_t)((uint8_t *)(any_addr))[0]     ))

/** @brief   Macro to convert a 2-byte buffer to an uint16_t value (big endian format) */
#define ENDIAN_GET16_BE(any_addr)           (((uint16_t)((uint8_t *)(any_addr))[0] << 8) | \
                                             ((uint16_t)((uint8_t *)(any_addr))[1]     ))

/** @brief   Macro to write an uint16_t value to 2-byte buffer (little endian format) */
#define ENDIAN_PUT16(any_addr, u16_data)    {((uint8_t *)(any_addr))[1] = (uint8_t)((u16_data) >> 8); \
                                             ((uint8_t *)(any_addr))[0] = (uint8_t)((u16_data));}

/** @brief   Macro to write an uint16_t value to 2-byte buffer (big endian format) */
#define ENDIAN_PUT16_BE(any_addr, u16_data) {((uint8_t *)(any_addr))[0] = (uint8_t)((u16_data) >> 8); \
                                             ((uint8_t *)(any_addr))[1] = (uint8_t)((u16_data));}

/** @brief   Macro to convert a 4-byte buffer to an uint32_t value (little endian format) */
#define ENDIAN_GET32(any_addr)              (((uint32_t)((uint8_t *)(any_addr))[3] << 24) | \
                                             ((uint32_t)((uint8_t *)(any_addr))[2] << 16) | \
                                             ((uint32_t)((uint8_t *)(any_addr))[1] <<  8) | \
                                             ((uint32_t)((uint8_t *)(any_addr))[0]      ))

/** @brief   Macro to convert a 4-byte buffer to an uint32_t value (big endian format) */
#define ENDIAN_GET32_BE(any_addr)           (((uint32_t)((uint8_t *)(any_addr))[0] << 24) | \
                                             ((uint32_t)((uint8_t *)(any_addr))[1] << 16) | \
                                             ((uint32_t)((uint8_t *)(any_addr))[2] <<  8) | \
                                             ((uint32_t)((uint8_t *)(any_addr))[3]      ))

/** @brief   Macro to write an uint32_t value to 4-byte buffer (little endian format) */
#define ENDIAN_PUT32(any_addr, u32_data)    {((uint8_t *)(any_addr))[3] = (uint8_t)((u32_data) >> 24); \
                                             ((uint8_t *)(any_addr))[2] = (uint8_t)((u32_data) >> 16); \
                                             ((uint8_t *)(any_addr))[1] = (uint8_t)((u32_data) >>  8); \
                                             ((uint8_t *)(any_addr))[0] = (uint8_t)((u32_data));}

/** @brief   Macro to write an uint32_t value to 4-byte buffer (big endian format) */
#define ENDIAN_PUT32_BE(any_addr, u32_data) {((uint8_t *)(any_addr))[0] = (uint8_t)((u32_data) >> 24); \
                                             ((uint8_t *)(any_addr))[1] = (uint8_t)((u32_data) >> 16); \
                                             ((uint8_t *)(any_addr))[2] = (uint8_t)((u32_data) >>  8); \
                                             ((uint8_t *)(any_addr))[3] = (uint8_t)((u32_data));}

/** @brief  Macro to start time monitoring */
#define TIMER_RESET(x_timer)                (x_timer) = xTaskGetTickCount ()

/** @brief  Macro to check time (in FreeRTOS ticks) elapsed since a timer was started */
#define TIMER_ELAPSED(x_timer)              (xTaskGetTickCount () - (x_timer))


#define ENABLE_APP_DEBUG 1   // <-- Set to 0 before production release if needed
#define ENABLE_TEST_DEBUG 0  // <-- Set to 0 before production release if needed

// /** @brief  Macros printing app debug log */
#if ENABLE_APP_DEBUG
    #define LOGV(...)                ESP_LOGV(TAG, __VA_ARGS__)
    #define LOGD(...)                ESP_LOGD(TAG, __VA_ARGS__)
    #define LOGI(...)                ESP_LOGI(TAG, __VA_ARGS__)
    #define LOGW(...)                ESP_LOGW(TAG, __VA_ARGS__)
    #define LOGE(...)                ESP_LOGE(TAG, __VA_ARGS__)
    #define LOG_DATA(pv_data, len)   ESP_LOG_BUFFER_HEX(TAG, pv_data, len)
// #else
//     #define LOGV(...)                do {} while (0)
//     #define LOGD(...)                do {} while (0)
//     #define LOGI(...)                do {} while (0)
//     #define LOGW(...)                do {} while (0)
//     #define LOGE(...)                do {} while (0)
//     #define LOG_DATA(pv_data, len)   do {} while (0)
#endif

// /** @brief  Macros printing test debug log */
#if ENABLE_TEST_DEBUG
    #define _LOGV(...)                ESP_LOGV(TAG, __VA_ARGS__)
    #define _LOGD(...)                ESP_LOGD(TAG, __VA_ARGS__)
    #define _LOGI(...)                ESP_LOGI(TAG, __VA_ARGS__)
    #define _LOGW(...)                ESP_LOGW(TAG, __VA_ARGS__)
    #define _LOGE(...)                ESP_LOGE(TAG, __VA_ARGS__)
    #define _LOG_DATA(pv_data, len)   ESP_LOG_BUFFER_HEX(TAG, pv_data, len)
#else
    #define _LOGV(...)                do {} while (0)
    #define _LOGD(...)                do {} while (0)
    #define _LOGI(...)                do {} while (0)
    #define _LOGW(...)                do {} while (0)
    #define _LOGE(...)                do {} while (0)
    #define _LOG_DATA(pv_data, len)   do {} while (0)
#endif


/** @brief  Macro converting a literal definition to string */
#define TO_STR(DEFINE)                      _TO_STR(DEFINE)
#define _TO_STR(DEFINE)                     #DEFINE

/**
** @brief  Macro displaying the minimum amount of remaining stack space that was available to the task since
**         the task started executing
*/
#define PRINT_STACK_USAGE(u16_period_ms)                                                    \
{                                                                                           \
    static TickType_t x_print_timer;                                                        \
    if ((TIMER_ELAPSED (x_print_timer) >= pdMS_TO_TICKS (u16_period_ms)))                   \
    {                                                                                       \
        TIMER_RESET (x_print_timer);                                                        \
        UBaseType_t x_stack_remain = uxTaskGetStackHighWaterMark (NULL);                    \
        LOGI ("Remaining stack of task %s = %d", pcTaskGetName (NULL), x_stack_remain);     \
    }                                                                                       \
}

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           VARIABLES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           PROTOTYPES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#endif /* __COMMON_HDR_H__ */

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
