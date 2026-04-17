/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : srvc_thermistor.h
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 May 21
**  @brief      : Public header of Srvc_Thermistor module. This file is the only header file to include in order to
**                use the module
**  @namespace  : THER
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  Srvc_Thermistor
** @{
*/

#ifndef __SRVC_THERMISTOR_H__
#define __SRVC_THERMISTOR_H__

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           INCLUDES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include "common_hdr.h"             /* Use common definitions */
#include "srvc_thermistor_ext.h"    /* Table of thermistor instances */
#include "float.h"                  /* Use floating number for temperature */

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           DEFINES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Handle to manage a thermistor instance */
typedef struct THER_obj *           THER_inst_t;

/** @brief  Status returned by APIs of Srvc_Thermistor module */
typedef enum
{
    THER_OK             = 0,        //!< The function executed successfully
    THER_ERR            = -1,       //!< There is unknown error while executing the function
} THER_status_t;

/** @brief  Expand an entry in THER_INST_TABLE as enumeration of instance ID */
#define THER_INST_TABLE_EXPAND_AS_INST_ID(INST_ID, ...)        INST_ID,
typedef enum
{
    THER_INST_TABLE (THER_INST_TABLE_EXPAND_AS_INST_ID)
    THER_NUM_INST
} THER_inst_id_t;

/**
** @brief   Alert status
** @details
**                            Power supply voltage
**                                      ^
**                                      |
**                                      | => THER_ALERT_HIGH
**                                      |
**                      High Threshold  +
**                                      |
**                                      | => THER_ALERT_NONE
**                                      |
**                       Low Threshold  +
**                                      |
**                                      | => THER_ALERT_LOW
**                                      |
*/
typedef enum
{
    THER_ALERT_NONE,
    THER_ALERT_LOW,
    THER_ALERT_HIGH,
    THER_ALERT_UNKNOWN
} THER_alert_t;

/** @brief  Invalid voltage */
#define THER_INVALID_TEMP           FLT_MIN

/** @brief  Index of the oldest recored temperature in the past, used for flt_THER_Get_Record() */
#define THER_OLDEST_RECORD_IDX      0xFFFF

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           PROTOTYPES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/* Gets instance handle of a thermistor instance */
extern THER_inst_t x_THER_Get_Inst (THER_inst_id_t enm_inst_id);

/*
** Gets instantaneous filtered temperature (in Celsius degrees) of a thermistor,
** Returns THER_INVALID_TEMP in case of failure
*/
extern float flt_THER_Get_Temperature (THER_inst_t x_inst);

/*
** Gets filtered temperature value of one sampling cycle in the past which is stored in recording buffer
** Index 0 is current temperature, index 1 is of 1 cycle ago, ..., index 2 is of 2 cycles ago
** THER_OLDEST_RECORD_IDX is index of the oldest temperature recoreded
** This function returns THER_INVALID_TEMP in case of failure
*/
extern float flt_THER_Get_Record (THER_inst_t x_inst, uint16_t u16_past_idx);

/* Gets alert status of a thermistor */
extern THER_alert_t enm_THER_Get_Alert_Status (THER_inst_t x_inst);

#endif /* __SRVC_THERMISTOR_H__ */

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
