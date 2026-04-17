/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : hwa_adc.c
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 May 15
**  @brief      : Implementation of Hwa_ADC module
**  @namespace  : ADC
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup      Hwa_ADC
** @brief           Abstracts Analog-to-Digital Converter (ADC) peripheral of ESP32-C3 and provides API to get
**                  conversion result as well as to determine status of analog input signal
** @{
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           INCLUDES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include "hwa_adc.h"                    /* Public header of this module */

#include "freertos/FreeRTOS.h"          /* Use FreeRTOS */
#include "freertos/timers.h"            /* Use FreeRTOS timers */
#include "driver/adc.h"                 /* Use ESP-IDF's ADC driver */
#include "esp_adc_cal.h"                /* Use ESP-IDF's ADC calibration API */

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           DEFINES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Structure encapsulating an ADC channel object */
struct ADC_obj
{
    bool                            b_initialized;          //!< Specifies whether the object has been initialized or not
    ADC_inst_id_t                   enm_inst_id;            //!< Instance ID of this object

    TimerHandle_t                   x_timer;                //!< Timer processing the ADC channel
    TickType_t                      x_fluctuant_stopwatch;  //!< Stopwatch monitoring interval input signal is changing
    TickType_t                      x_steady_stopwatch;     //!< Stopwatch monitoring interval input signal is steady
    esp_adc_cal_characteristics_t   x_adc_cal;              //!< Calibration characteristics of the ADC channel
    int16_t                         s16_adc_raw;            //!< ADC reading before the IIR filter, negative if error
    int16_t                         s16_adc_raw_prev;       //!< Previous ADC reading
    int16_t                         s16_adc_filtered;       //!< ADC reading after the IIR filter, negative if error
    int16_t                         s16_voltage;            //!< Calibrated voltage in mV, negative in case of error
    uint8_t                         u8_skipped_samples;     //!< Count number of ADC samples skipped filtering at startup
    ADC_input_status_t              enm_input_status;       //!< Status of analog input

    ADC_callback_t                  pfnc_cb;                //!< Callback functions invoked when data is received
    void *                          pv_cb_arg;              //!< Argument passed when the callback function was registered

    adc_unit_t                      enm_adc_unit;           //!< ADC unit of the instance
    adc_channel_t                   enm_adc_channel;        //!< ADC channel number of the instance
    adc_bits_width_t                enm_resolution;         //!< Capture resolution of the ADC unit in bits
    adc_atten_t                     enm_adc_atten;          //!< Attenuation in dB of an ADC channel
    uint16_t                        u16_sampling_cycle;     //!< Period in milliseconds performing the analog conversion
    float                           flt_filter_coeff;       //!< Coefficient of the IIR filter applied on ADC raw reading
    uint16_t                        u16_present_debounce;   //!< Interval (ms) to detect if analog input signal is present
    uint16_t                        u16_missing_debounce;   //!< Interval (ms) to detect if analog input signal is missing
};

/** @brief  Macro expanding ADC_INST_TABLE as initialization value for ADC_obj struct */
#define INST_TABLE_EXPAND_AS_STRUCT_INIT(INST_ID, UNIT, CHANNEL, ATTEN,         \
                                         CYCLE, FILTER, PRESENT, MISSING)       \
{                                                                               \
    .b_initialized                  = false,                                    \
    .enm_inst_id                    = INST_ID,                                  \
                                                                                \
    .x_timer                        = NULL,                                     \
    .x_fluctuant_stopwatch          = 0,                                        \
    .x_steady_stopwatch             = 0,                                        \
    .s16_adc_raw                    = 0,                                        \
    .s16_adc_raw_prev               = 0,                                        \
    .s16_adc_filtered               = -1,                                       \
    .s16_voltage                    = -1,                                       \
    .u8_skipped_samples             = 0,                                        \
    .enm_input_status               = ADC_INPUT_UNKNOWN,                        \
                                                                                \
    .pfnc_cb                        = NULL,                                     \
    .pv_cb_arg                      = NULL,                                     \
                                                                                \
    .enm_adc_unit                   = UNIT,                                     \
    .enm_adc_channel                = ADC_CHANNEL_##CHANNEL,                    \
    .enm_resolution                 = ADC_WIDTH_BIT_12,                         \
    .enm_adc_atten                  = ADC_ATTEN_##ATTEN,                        \
    .u16_sampling_cycle             = CYCLE,                                    \
    .flt_filter_coeff               = 2.0 / ((1UL << (FILTER)) + 1),            \
    .u16_present_debounce           = PRESENT,                                  \
    .u16_missing_debounce           = MISSING,                                  \
},

/** @brief  Structure encapsulating an ADC unit and its configuration */
struct ADC_unit
{
    adc_unit_t              enm_adc_unit;           //!< ADC unit enumeration
    adc_bits_width_t        enm_resolution;         //!< Capture resolution of the ADC unit in bits
    uint16_t                u16_ref_voltage;        //!< Reference voltage of the ADC unit in millivolts
};

/** @brief  Macro expanding ADC_UNIT_TABLE as initialization value for ADC_unit struct */
#define UNIT_TABLE_EXPAND_AS_STRUCT_INIT(UNIT, RESOLUTION, VREF)                \
{                                                                               \
    .enm_adc_unit           = UNIT,                                             \
    .enm_resolution         = ADC_WIDTH_BIT_##RESOLUTION,                       \
    .u16_ref_voltage        = VREF,                                             \
},

/**
** @brief  Number of ADC samples (0 -> 255) that are not passed through the filter when an instance starts.
**         This wait is needed for the analog signal to be stable
*/
#define ADC_FIST_SAMPLES_NOT_FILTERED       10

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           VARIABLES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Logging tag of this module */
static const char * TAG = "Hwa_ADC";

/** @brief  Indicates if this module has been initialized */
static bool g_b_initialized = false;

/** @brief  Array of all ADC channel objects */
static struct ADC_obj g_astru_adc_objs[ADC_NUM_INST] =
{
    ADC_INST_TABLE (INST_TABLE_EXPAND_AS_STRUCT_INIT)
};

/** @brief  Array of all ADC units */
static struct ADC_unit g_astru_adc_units[] =
{
    ADC_UNIT_TABLE (UNIT_TABLE_EXPAND_AS_STRUCT_INIT)
};

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           PROTOTYPES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

static int32_t s32_ADC_Init_Module (void);
static int32_t s32_ADC_Init_Inst (ADC_inst_t x_inst);
static void v_ADC_Timer_Cb (TimerHandle_t x_timer);

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           FUNCTIONS SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets instance of an ADC channel. This instance will be used for the other functions in this module
**
** @param [in]
**      enm_inst_id: Index of the ADC channel to get. The index is expanded from ADC_INST_TABLE
**
** @return
**      @arg    NULL: Failed to get instance of the ADC channel
**      @arg    Otherwise: Instance handle of the ADC channel
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
ADC_inst_t x_ADC_Get_Inst (ADC_inst_id_t enm_inst_id)
{
    ADC_inst_t  x_inst = NULL;
    int32_t     s32_result = STATUS_OK;

    /* Validation */
    if (enm_inst_id >= ADC_NUM_INST)
    {
        return NULL;
    }

    /* If this module has not been initialized, do that now */
    if (!g_b_initialized)
    {
        s32_result = s32_ADC_Init_Module ();
        g_b_initialized = (s32_result == STATUS_OK);
    }

    /* If the retrieved instance has not been initialized yet, do that now */
    if (s32_result == STATUS_OK)
    {
        x_inst = &g_astru_adc_objs[enm_inst_id];
        if (!x_inst->b_initialized)
        {
            s32_result = s32_ADC_Init_Inst (x_inst);
            x_inst->b_initialized = (s32_result == STATUS_OK);
        }
    }

    /* Return instance of the required ADC channel */
    return (s32_result == STATUS_OK) ? x_inst : NULL;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Registers a callback function invoked when an event occurs
**
** @param [in]
**      x_inst: ADC instance returned by x_ADC_Get_Inst() function
**
** @param [in]
**      pfnc_cb: Callback function to register
**
** @param [in]
**      pv_arg: Optional argument which will be forwarded to the data of callback function when it's invoked
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
void v_ADC_Register_Callback (ADC_inst_t x_inst, ADC_callback_t pfnc_cb, void * pv_arg)
{
    /* Validation */
    ASSERT_PARAM ((x_inst != NULL) && (x_inst->b_initialized));

    /* Store the callback function */
    x_inst->pfnc_cb = pfnc_cb;
    x_inst->pv_cb_arg = pv_arg;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets filtered reading of an ADC channel
**
** @param [in]
**      x_inst: ADC instance returned by x_ADC_Get_Inst() function
**
** @return
**      @arg    >= 0: ADC reading (after the IRR filter) of an ADC channel
**      @arg    < 0: failed to get ADC reading
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int16_t s16_ADC_Get_Raw (ADC_inst_t x_inst)
{
    /* Validation */
    ASSERT_PARAM (x_inst->b_initialized);

    /* Return filtered ADC reading */
    return x_inst->s16_adc_filtered;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets calibrated voltage in millivolt of an ADC channel
**
** @note
**      The precision of the returned voltage depends on if the relevant ADC unit was calibrated. Read more about
**      ADC calibration here:
**      https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html#adc-calibration
**
** @param [in]
**      x_inst: ADC instance returned by x_ADC_Get_Inst() function
**
** @return
**      @arg    >= 0: ADC voltage of an ADC channel
**      @arg    < 0: failed to get ADC voltage
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int16_t s16_ADC_Get_Voltage (ADC_inst_t x_inst)
{
    /* Validation */
    ASSERT_PARAM (x_inst->b_initialized);

    /* Return ADC voltage */
    return x_inst->s16_voltage;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Checks status of analog input signal
**
** @param [in]
**      x_inst: ADC instance returned by x_ADC_Get_Inst() function
**
** @return
**      Status of analog input
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
ADC_input_status_t enm_ADC_Get_Input_Status (ADC_inst_t x_inst)
{
    /* Validation */
    ASSERT_PARAM (x_inst->b_initialized);

    /* Return status of analog input signal */
    return x_inst->enm_input_status;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Initializes Hwa_ADC module
**
** @return
**      @arg    STATUS_OK
**      @arg    STATUS_ERR
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int32_t s32_ADC_Init_Module (void)
{
    /* Configure all ADC units used */
    uint8_t u8_num_units = sizeof (g_astru_adc_units) / sizeof (g_astru_adc_units[0]);
    for (uint8_t u8_unit_idx = 0; u8_unit_idx < u8_num_units; u8_unit_idx++)
    {
        /* Configure resolution of ADC1 */
        if (g_astru_adc_units[u8_unit_idx].enm_adc_unit == ADC_UNIT_1)
        {
            ESP_ERROR_CHECK (adc1_config_width (g_astru_adc_units[u8_unit_idx].enm_resolution));
        }

        /* Characterize all ADC channels of this unit */
        for (uint8_t u8_inst_id = 0; u8_inst_id < ADC_NUM_INST; u8_inst_id++)
        {
            ADC_inst_t x_inst = &g_astru_adc_objs[u8_inst_id];
            if (x_inst->enm_adc_unit == g_astru_adc_units[u8_unit_idx].enm_adc_unit)
            {
                x_inst->enm_resolution = g_astru_adc_units[u8_unit_idx].enm_resolution;
                esp_adc_cal_value_t enm_cal_type =
                    esp_adc_cal_characterize (x_inst->enm_adc_unit,
                                              x_inst->enm_adc_atten,
                                              x_inst->enm_resolution,
                                              g_astru_adc_units[u8_unit_idx].u16_ref_voltage,
                                              &x_inst->x_adc_cal);
                if (enm_cal_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
                {
                    LOGI ("ADC instance %d is calibrated using eFuse Vref", u8_inst_id);
                }
                else if (enm_cal_type == ESP_ADC_CAL_VAL_EFUSE_TP)
                {
                    LOGI ("ADC instance %d is calibrated using eFuse Two Point", u8_inst_id);
                }
                else
                {
                    LOGI ("ADC instance %d is calibrated using provided Vref value = %d mV",
                          u8_inst_id, g_astru_adc_units[u8_unit_idx].u16_ref_voltage);
                }
            }
        }
    }

    return STATUS_OK;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Initializes an ADC channel instance
**
** @param [in]
**      x_inst: ADC instance returned by x_ADC_Get_Inst() function
**
** @return
**      @arg    STATUS_OK
**      @arg    STATUS_ERR
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int32_t s32_ADC_Init_Inst (ADC_inst_t x_inst)
{
    /* Configure the ADC channel */
    if (x_inst->enm_adc_unit == ADC_UNIT_1)
    {
        ESP_ERROR_CHECK (adc1_config_channel_atten (x_inst->enm_adc_channel, x_inst->enm_adc_atten));
    }
    else
    {
        ESP_ERROR_CHECK (adc2_config_channel_atten (x_inst->enm_adc_channel, x_inst->enm_adc_atten));
    }

    /* Create timer processing the ADC channel */
    x_inst->x_timer =
        xTimerCreate (NULL,                                         /* Human readable text name */
                      pdMS_TO_TICKS (x_inst->u16_sampling_cycle),   /* Timer period */
                      pdTRUE,                                       /* Auto reload */
                      x_inst,                                       /* Timer ID */
                      v_ADC_Timer_Cb);                              /* Callback function when the timer expires */
    ASSERT_PARAM (x_inst->x_timer != NULL);

    /* Run ADC for the first time to initalize channel's value */
    v_ADC_Timer_Cb (x_inst->x_timer);

    /* Start the timer */
    ASSERT_PARAM (xTimerStart (x_inst->x_timer, portMAX_DELAY) == pdPASS);

    return STATUS_OK;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Callback function invoked when the timer processing an ADC channel expires
**
** @param [in]
**      x_timer: Handle of the timer
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void IRAM_ATTR v_ADC_Timer_Cb (TimerHandle_t x_timer)
{
    /* Timer ID of the timer contains the corresponding ADC channel instance */
    ADC_inst_t x_inst = pvTimerGetTimerID (x_timer);

    /* Get ADC raw value of the channel */
    if (x_inst->enm_adc_unit == ADC_UNIT_1)
    {
        x_inst->s16_adc_raw = adc1_get_raw (x_inst->enm_adc_channel);
    }
    else
    {
        int s32_adc_raw;
        if (adc2_get_raw (x_inst->enm_adc_channel, x_inst->enm_resolution, &s32_adc_raw) != ESP_OK)
        {
            x_inst->s16_adc_raw = -1;
        }
        else
        {
            x_inst->s16_adc_raw = s32_adc_raw;
        }
    }

    /* If there is error getting ADC reading */
    if (x_inst->s16_adc_raw < 0)
    {
        /* Error getting ADC reading */
        x_inst->enm_input_status = ADC_INPUT_UNKNOWN;
        x_inst->s16_adc_filtered = -1;
        x_inst->s16_voltage = -1;
        return;
    }

    /* Determine status of analog input signal */
    if (x_inst->s16_adc_raw != x_inst->s16_adc_raw_prev)
    {
        x_inst->s16_adc_raw_prev = x_inst->s16_adc_raw;
        TIMER_RESET (x_inst->x_steady_stopwatch);
        if (x_inst->enm_input_status != ADC_INPUT_PRESENT)
        {
            if (TIMER_ELAPSED (x_inst->x_fluctuant_stopwatch) >= pdMS_TO_TICKS (x_inst->u16_present_debounce))
            {
                x_inst->enm_input_status = ADC_INPUT_PRESENT;
            }
        }
    }
    else
    {
        TIMER_RESET (x_inst->x_fluctuant_stopwatch);
        if (x_inst->enm_input_status != ADC_INPUT_MISSING)
        {
            if (TIMER_ELAPSED (x_inst->x_steady_stopwatch) >= pdMS_TO_TICKS (x_inst->u16_missing_debounce))
            {
                x_inst->enm_input_status = ADC_INPUT_MISSING;
            }
        }
    }

    /*
    ** If this instance just started, do not do the filtering for some first ADC samples
    ** so that the relevant analog signal has time for being stable
    */
    if (x_inst->u8_skipped_samples < ADC_FIST_SAMPLES_NOT_FILTERED)
    {
        x_inst->u8_skipped_samples++;
        x_inst->s16_adc_filtered = x_inst->s16_adc_raw;
    }
    else
    {
        /* Filter the ADC reading */
        if (x_inst->s16_adc_filtered < 0)
        {
            x_inst->s16_adc_filtered = x_inst->s16_adc_raw;
        }
        else
        {
            x_inst->s16_adc_filtered = x_inst->flt_filter_coeff * x_inst->s16_adc_raw +
                                       (1 - x_inst->flt_filter_coeff) * x_inst->s16_adc_filtered;
        }
    }

    /* Convert the filtered ADC reading to voltage in mV */
    x_inst->s16_voltage = esp_adc_cal_raw_to_voltage (x_inst->s16_adc_filtered, &x_inst->x_adc_cal);

    /* Invoke callback */
    if (x_inst->pfnc_cb != NULL)
    {
        ADC_evt_data_t stru_evt_data =
        {
            .x_inst             = x_inst,
            .pv_arg             = x_inst->pv_cb_arg,
            .enm_evt            = ADC_EVT_CONVERSION_DONE,
            .stru_adc           =
            {
                .s16_voltage    = x_inst->s16_voltage,
                .enm_status     = x_inst->enm_input_status,
            },
        };
        x_inst->pfnc_cb (&stru_evt_data);
    }
}

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
