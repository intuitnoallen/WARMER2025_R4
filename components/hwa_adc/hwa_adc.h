/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : hwa_adc.h
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 May 15
**  @brief      : Public header of Hwa_ADC module
**  @namespace  : ADC
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  Hwa_ADC
** @{
*/

#ifndef __HWA_ADC_H__
#define __HWA_ADC_H__

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           INCLUDES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include "common_hdr.h"         /* Use common definitions */
#include "hwa_adc_ext.h"        /* Table of ADC channel instances */

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           DEFINES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Handle to manage an ADC channel instance */
typedef struct ADC_obj *        ADC_inst_t;

/** @brief  Expand an entry in ADC_INST_TABLE as enumeration of instance ID */
#define ADC_INST_TABLE_EXPAND_AS_INST_ID(INST_ID, ...)          INST_ID,
typedef enum
{
    ADC_INST_TABLE (ADC_INST_TABLE_EXPAND_AS_INST_ID)
    ADC_NUM_INST
} ADC_inst_id_t;

/** @brief  Status of ADC input signal */
typedef enum
{
    ADC_INPUT_PRESENT,          //!< Analog input signal is available
    ADC_INPUT_MISSING,          //!< ADC channel cannot detect analog input signal
    ADC_INPUT_UNKNOWN           //!< Not able to determine status of the input signal
} ADC_input_status_t;

/** @brief  Context data of the events fired by the module */
typedef struct
{
    ADC_inst_t          x_inst;         //!< The instance that fires the event
    void *              pv_arg;         //!< Argument passed when the associated callback function was registered

    /** @brief  Events fired by the module */
    enum
    {
        ADC_EVT_CONVERSION_DONE,        //!< ADC conversion completes
    } enm_evt;

    /** @brief  Context data specific for ADC_EVT_CONVERSION_DONE */
    struct
    {
        int16_t             s16_voltage;    //!< Converted voltage in millivolts, negative value in case of error
        ADC_input_status_t  enm_status;     //!< Status of ADC input signal
    } stru_adc;

} ADC_evt_data_t;

/** @brief  Callback invoked when an event occurs. This callback is invoked in FreeRTOS's timer context */
typedef void (*ADC_callback_t) (ADC_evt_data_t * pstru_evt_data);

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           PROTOTYPES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/* Gets instance of an ADC channel. This instance will be used for the other functions in this module */
extern ADC_inst_t x_ADC_Get_Inst (ADC_inst_id_t enm_inst_id);

/* Registers a callback function invoked when an event occurs */
extern void v_ADC_Register_Callback (ADC_inst_t x_inst, ADC_callback_t pfnc_cb, void * pv_arg);

/* Gets filtered reading of an ADC channel, returns negative value in case of error */
extern int16_t s16_ADC_Get_Raw (ADC_inst_t x_inst);

/* Gets calibrated voltage in millivolt of an ADC channel, returns negative value in case of error */
extern int16_t s16_ADC_Get_Voltage (ADC_inst_t x_inst);

/* Checks status of analog input signal */
extern ADC_input_status_t enm_ADC_Get_Input_Status (ADC_inst_t x_inst);

#endif /* __HWA_ADC_H__ */

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
