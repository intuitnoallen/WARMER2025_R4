/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : srvc_ps_mon.h
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 Oct 24
**  @brief      : Public header of Srvc_PS_Mon module. This file is the only header file to include in order to
**                use the module
**  @namespace  : PSMON
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  Srvc_PS_Mon
** @{
*/

#ifndef __SRVC_PS_MON_H__
#define __SRVC_PS_MON_H__

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           INCLUDES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include "common_hdr.h"             /* Use common definitions */
#include "srvc_ps_mon_ext.h"        /* Table of power supply monitoring instances */

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           DEFINES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Handle to manage a power supply monitoring instance */
typedef struct PSMON_obj *          PSMON_inst_t;

/** @brief  Status returned by APIs of Srvc_PS_Mon module */
typedef enum
{
    PSMON_OK            = 0,        //!< The function executed successfully
    PSMON_ERR           = -1,       //!< There is unknown error while executing the function
} PSMON_status_t;

/** @brief  Expand an entry in PSMON_INST_TABLE as enumeration of instance ID */
#define PSMON_INST_TABLE_EXPAND_AS_INST_ID(INST_ID, ...)        INST_ID,
typedef enum
{
    PSMON_INST_TABLE (PSMON_INST_TABLE_EXPAND_AS_INST_ID)
    PSMON_NUM_INST
} PSMON_inst_id_t;

/**
** @brief   Alert status
** @details
**                            Power supply voltage
**                                      ^
**                                      |
**                                      | => PSMON_ALERT_HIGH
**                                      |
**                      High Threshold  +
**                                      |
**                                      | => PSMON_ALERT_NONE
**                                      |
**                       Low Threshold  +
**                                      |
**                                      | => PSMON_ALERT_LOW
**                                      |
**                                  0%  +
**                                      |
**                                      | => PSMON_ALERT_EMPTY
**                                      |
*/
typedef enum
{
    PSMON_ALERT_NONE,               //!< Battery is okay for normal operation
    PSMON_ALERT_EMPTY,              //!< Battery has been used off and the device will be soon shutoff
    PSMON_ALERT_LOW,                //!< Battery is low
    PSMON_ALERT_HIGH,               //!< Battery is too high
    PSMON_ALERT_UNKNOWN
} PSMON_alert_t;

/**
** @brief   External Plug Alert status
** @details
**                                  Plug voltage
**                                      ^
**                                      |
**                                      | => EPMON_ALERT_WRONG_ADAPTER
**                                      |
**                      High Threshold  +
**                                      |
**                                      | => EPMON_ALERT_NONE
**                                      |
**                       Low Threshold  +
**                                      |
**                                      | => EPMON_ALERT_WRONG_ADAPTER
**                                      |
**                                  
*/
typedef enum
{
    EPMON_ALERT_NONE,               //!< Adapter is okay for charging
    EPMON_ALERT_WRONG_ADAPTER,      //!< Adapter is not suitable for charging
    EPMON_ALERT_UNKNOWN
} EPMON_alert_t;

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           PROTOTYPES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/* Gets instance handle of a power supply monitoring instance */
extern PSMON_inst_t x_PSMON_Get_Inst (PSMON_inst_id_t enm_inst_id);

/* Gets voltage (milli-volts) and remaining battery (percents) of a power supply source */
extern PSMON_status_t enm_PSMON_Get_Bat_Info (PSMON_inst_t x_inst, uint16_t * pu16_voltage, uint8_t * pu8_percent);

/* Gets voltage (milli-volts) of a power plug source */
extern PSMON_status_t enm_PSMON_Get_Plug_Info (PSMON_inst_t x_inst, uint16_t * pu16_voltage);

/* Gets current of a power plug source  */
extern PSMON_status_t enm_PSMON_Get_EP_Curr_Info (PSMON_inst_t x_inst, uint16_t * pu16_current);

/* Gets alert status of a power supply source */
extern PSMON_alert_t enm_PSMON_Get_Bat_Alert_Status (PSMON_inst_t x_inst);

/* Gets alert status of a external plug */
extern PSMON_alert_t enm_PSMON_Get_Plug_Alert_Status (PSMON_inst_t x_inst);

#endif /* __SRVC_PS_MON_H__ */

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
