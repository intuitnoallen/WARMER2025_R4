/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : srvc_button.h
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 Sep 17
**  @brief      : Public header of Srvc_Button module. This file is the only header file to include in order to
**                use the module
**  @namespace  : BTN
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  Srvc_Button
** @{
*/

#ifndef __SRVC_BUTTON_H__
#define __SRVC_BUTTON_H__

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           INCLUDES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include "common_hdr.h"             /* Use common definitions */
#include "srvc_button_ext.h"        /* Table of button instances */

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           DEFINES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Handle to manage a button instance */
typedef struct BTN_obj *            BTN_inst_t;

/** @brief  Expand an entry in BTN_INST_TABLE as enumeration of instance ID */
#define BTN_INST_TABLE_EXPAND_AS_INST_ID(INST_ID, ...)          INST_ID,
typedef enum
{
    BTN_INST_TABLE (BTN_INST_TABLE_EXPAND_AS_INST_ID)
    BTN_NUM_INST
} BTN_inst_id_t;

/**
** @brief   States of a button
** @details Button state transition
** @verbatim
**              BTN_STATE_RELEASED --> BTN_STATE_PRESSED --> BTN_STATE_HOLD_SHORT --> BTN_STATE_HOLD_LONG
**                  ^    ^    ^                |                      |                       |
**                  |    |    +----------------+                      |                       |
**                  |    +--------------------------------------------+                       |
**                  +-------------------------------------------------------------------------+
** @endverbatim
*/
typedef enum
{
    BTN_STATE_RELEASED,                 //!< Button is released and idle
    BTN_STATE_PRESSED,                  //!< Button has just been pressed
    BTN_STATE_HOLD_START,               //!< Button is being pressed and hold started
    BTN_STATE_HOLD_SHORT,               //!< Button is being pressed and hold for a short time
    BTN_STATE_HOLD_LONG,                //!< Button is being pressed and hold for a long time
    BTN_NUM_STATES
} BTN_state_t;

/** @brief  Context data of the events fired by the module */
typedef struct
{
    BTN_inst_t      x_inst;             //!< The instance that fires the event
    void *          pv_arg;             //!< Argument passed when the associated callback function was registered

    /** @brief  Events fired by the module */
    enum
    {
        BTN_EVT_STATE_CHANGED,          //!< State of a button has been changed
    } enm_evt;

    /** @brief  Context data specific for BTN_EVT_STATE_CHANGED */
    struct
    {
        BTN_state_t enm_old_state;      //!< Previous state of the button
        BTN_state_t enm_new_state;      //!< Current state of the button
    } stru_state;

} BTN_evt_data_t;

/**
** @brief   Prototype of callback function invoked when an event occurs
**
** @note    The callback function is executed in the context of the module service task. It is therefore essential
**          that callback functions never attempt to block. For example, a callback function must not call vTaskDelay(),
**          vTaskDelayUntil(), or specify a non zero block time when accessing a queue or a semaphore.
*/
typedef void (*BTN_callback_t) (BTN_evt_data_t * pstru_evt_data);

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           PROTOTYPES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/* Gets instance handle of a button. This instance will be used for other functions in this module */
extern BTN_inst_t x_BTN_Get_Inst (BTN_inst_id_t enm_inst_id);

/* Registers a callback function invoked when an event occurs */
extern void v_BTN_Register_Callback (BTN_inst_t x_inst, BTN_callback_t pfnc_cb, void * pv_arg);

/* Gets current state of a button */
extern BTN_state_t enm_BTN_Get_State (BTN_inst_t x_inst);

/* Gets interval in milliseconds that a button has been pressed or released */
extern uint32_t u32_BTN_Get_Level_Interval (BTN_inst_t x_inst);

#endif /* __SRVC_BUTTON_H__ */

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
