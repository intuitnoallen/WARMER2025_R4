/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : srvc_thermistor.c
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 May 21
**  @brief      : Implementation of Srvc_Thermistor module
**  @namespace  : THER
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  Srvc_Thermistor
** @brief       Srvc_Thermistor module encapsulates a temperature measurement circuit using thermistor and provides
**              API to get measured temperature and check status of the temperature.
**
**                                  +--------------+  Raw temperature   +------------+
**              ADC reading ------->| Calib points |------------------->| IIR Filter |---+-----> Filtered temperature
**                                  +--------------+                    +------------+   |
**                                                                                       +-----> Recording buffer
** @{
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           INCLUDES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include "srvc_thermistor.h"            /* Public header of this module */
#include "hwa_adc.h"                    /* Use hardware abstraction of ADC channels for voltage measurement */

#include "freertos/FreeRTOS.h"          /* Use FreeRTOS */
#include "freertos/timers.h"            /* Use FreeRTOS timers */

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           DEFINES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Structure wrapping an calibration point */
typedef struct
{
    float               flt_voltage;            //!< Measured voltage in volts
    float               flt_temperature;        //!< Temperature (Celsius degree) corresponding with the measured voltage
} THER_calib_t;

/** @brief  Structure encapsulating a thermistor object */
struct THER_obj
{
    bool                b_initialized;          //!< Specifies whether the object has been initialized or not
    THER_inst_id_t      enm_inst_id;            //!< Instance ID of this object

    ADC_inst_id_t       enm_adc_inst_id;        //!< Instance ID of the ADC channel measuring thermistor voltage
    ADC_inst_t          x_adc_inst;             //!< Instance handle of the ADC channel measuring thermistor voltage

    TimerHandle_t       x_timer;                //!< Timer processing the temperature measurement
    float               flt_temp_raw;           //!< Temperature before the IIR filter
    float               flt_temp_filtered;      //!< Temperature after the IIR filter

    float *             pflt_record_buf;        //!< Pointer to the buffer recording the filtered temperature values
    uint16_t            u16_record_buf_len;     //!< Number of elements of the recording buffer
    uint16_t            u16_latest_record_idx;  //!< Index of the element in recording buffer storing the latest value

    uint16_t            u16_sampling_cycle;     //!< Period in milliseconds processing the temperature measurement
    float               flt_filter_coeff;       //!< Coefficient of the IIR filter applied on calculated temperature
    float               flt_low_threshold;      //!< Low threshold (in Celsius degrees) of the measured temperature
    float               flt_high_threshold;     //!< High threshold (in Celsius degrees) of the measured temperature
    uint8_t             u8_num_calib_points;    //!< Number of calibration points
    THER_calib_t *      pstru_calib_points;     //!< Pointer to array of calibration points
};

/** @brief  Macro expanding THER_INST_TABLE as initialization value for THER_obj struct */
#define INST_TABLE_EXPAND_AS_STRUCT_INIT(INST_ID, ADC, CYCLE, RECORD, FILTER, LOW, HIGH, ...)   \
{                                                                                               \
    .b_initialized          = false,                                                            \
    .enm_inst_id            = INST_ID,                                                          \
                                                                                                \
    .enm_adc_inst_id        = ADC,                                                              \
    .x_adc_inst             = NULL,                                                             \
                                                                                                \
    .x_timer                = NULL,                                                             \
    .flt_temp_raw           = 0,                                                                \
    .flt_temp_filtered      = THER_INVALID_TEMP,                                                \
                                                                                                \
    .pflt_record_buf        = NULL,                                                             \
    .u16_record_buf_len     = RECORD,                                                           \
    .u16_latest_record_idx  = 0,                                                                \
                                                                                                \
    .u16_sampling_cycle     = CYCLE,                                                            \
    .flt_filter_coeff       = 2.0 / ((1UL << (FILTER)) + 1),                                    \
    .flt_low_threshold      = LOW,                                                              \
    .flt_high_threshold     = HIGH,                                                             \
    .u8_num_calib_points    = sizeof (g_astru_calib_points_##INST_ID) / sizeof (THER_calib_t),  \
    .pstru_calib_points     = g_astru_calib_points_##INST_ID,                                   \
},

/** @brief  Macro expanding THER_INST_TABLE as array of calibration points */
#define INST_TABLE_EXPAND_AS_CALIB_POINT_ARRAY(INST_ID, ADC, CYCLE, RECORD, FILTER, LOW, HIGH, ...) \
    static THER_calib_t g_astru_calib_points_##INST_ID [] = { __VA_ARGS__ };

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           VARIABLES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Logging tag of this module */
static const char * TAG = "Srvc_Thermistor";

/** @brief  Indicates if this module has been initialized */
static bool g_b_initialized = false;

/** @brief  Arrays of all calibration points */
THER_INST_TABLE (INST_TABLE_EXPAND_AS_CALIB_POINT_ARRAY)

/** @brief  Array of all thermistor objects */
static struct THER_obj g_astru_thermistor_objs[THER_NUM_INST] =
{
    THER_INST_TABLE (INST_TABLE_EXPAND_AS_STRUCT_INIT)
};

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           PROTOTYPES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

static int32_t s32_THER_Init_Module (void);
static int32_t s32_THER_Init_Inst (THER_inst_t x_inst);
static float flt_THER_Calib (THER_inst_t x_inst, float flt_voltage);
static void v_THER_Timer_Cb (TimerHandle_t x_timer);

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           FUNCTIONS SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets instance handle of a thermistor. This instance will be used for other functions in this module.
**
** @param [in]
**      enm_inst_id: Index of the thermistor instance to get. Thermistor indexes are expanded from Instance_ID
**                   column of THER_INST_TABLE (srvc_thermistor_ext.h)
**
** @return
**      @arg    NULL: Failed to get instance of the thermistor
**      @arg    Otherwise: Instance handle of the thermistor
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
THER_inst_t x_THER_Get_Inst (THER_inst_id_t enm_inst_id)
{
    THER_inst_t     x_inst = NULL;
    int32_t         s32_result = STATUS_OK;

    /* Validation */
    if (enm_inst_id >= THER_NUM_INST)
    {
        return NULL;
    }

    /* If this module has not been initialized, do that now */
    if (!g_b_initialized)
    {
        s32_result = s32_THER_Init_Module ();
        g_b_initialized = (s32_result == STATUS_OK);
    }

    /* If the retrieved instance has not been initialized yet, do that now */
    if (s32_result == STATUS_OK)
    {
        x_inst = &g_astru_thermistor_objs[enm_inst_id];
        if (!x_inst->b_initialized)
        {
            s32_result = s32_THER_Init_Inst (x_inst);
            x_inst->b_initialized = (s32_result == STATUS_OK);
        }
    }

    /* Return instance of the required thermistor */
    return (s32_result == STATUS_OK) ? x_inst : NULL;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets filtered temperature (in Celsius degrees) of a thermistor
**
** @param [in]
**      x_inst: Thermistor instance returned by x_THER_Get_Inst() function
**
** @return
**      @arg    THER_INVALID_TEMP: failed to get temperature
**      @arg    otherwise: temperature in Celsius degrees
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
float flt_THER_Get_Temperature (THER_inst_t x_inst)
{
    /* Validation */
    ASSERT_PARAM ((x_inst != NULL) && x_inst->b_initialized);

    return x_inst->flt_temp_filtered;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets filtered temperature value of one sampling cycle in the past which is stored in recording buffer
**
** @param [in]
**      x_inst: Thermistor instance returned by x_THER_Get_Inst() function
**
** @param [in]
**      u16_past_idx: Index in the recording buffer to get
**                    Index 0 is current temperature, index 1 is of 1 cycle ago, ..., index 2 is of 2 cycles ago
**                    THER_OLDEST_RECORD_IDX is index of the oldest temperature recoreded
**
** @return
**      @arg    THER_INVALID_TEMP: failed to get temperature
**      @arg    otherwise: temperature in Celsius degrees
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
float flt_THER_Get_Record (THER_inst_t x_inst, uint16_t u16_past_idx)
{
    /* Validation */
    ASSERT_PARAM ((x_inst != NULL) && x_inst->b_initialized);

    /* Ensure that the index does not exceed length of the recording buffer */
    if (u16_past_idx > x_inst->u16_record_buf_len - 1)
    {
        u16_past_idx = x_inst->u16_record_buf_len - 1;
    }

    /* Calculate the corresponding index of the element in the recording buffer to get */
    uint16_t u16_index = x_inst->u16_latest_record_idx + x_inst->u16_record_buf_len - u16_past_idx;
    u16_index %= x_inst->u16_record_buf_len;

    return x_inst->pflt_record_buf[u16_index];
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets alert status of a thermistor
**
** @param [in]
**      x_inst: Thermistor instance returned by x_THER_Get_Inst() function
**
** @return
**      Alert status
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
THER_alert_t enm_THER_Get_Alert_Status (THER_inst_t x_inst)
{
    /* Validation */
    ASSERT_PARAM ((x_inst != NULL) && x_inst->b_initialized);

    /* Determine status */
    if (x_inst->flt_temp_filtered == THER_INVALID_TEMP)
    {
        return THER_ALERT_UNKNOWN;
    }
    else if (x_inst->flt_temp_filtered > x_inst->flt_high_threshold)
    {
        return THER_ALERT_HIGH;
    }
    else if (x_inst->flt_temp_filtered < x_inst->flt_low_threshold)
    {
        return THER_ALERT_LOW;
    }
    else
    {
        return THER_ALERT_NONE;
    }
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Initializes Srvc_Thermistor module
**
** @return
**      @arg    STATUS_OK
**      @arg    STATUS_ERR
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int32_t s32_THER_Init_Module (void)
{
    /* Do nothing */
    return STATUS_OK;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Initializes a thermistor instance
**
** @param [in]
**      x_inst: Thermistor instance returned by x_THER_Get_Inst() function
**
** @return
**      @arg    STATUS_OK
**      @arg    STATUS_ERR
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int32_t s32_THER_Init_Inst (THER_inst_t x_inst)
{
    /* Get instance handle of the associated ADC channel used to measure thermistor voltage */
    x_inst->x_adc_inst = x_ADC_Get_Inst (x_inst->enm_adc_inst_id);
    if (x_inst->x_adc_inst == NULL)
    {
        LOGE ("Failed to get instance of ADC channel");
        return STATUS_ERR;
    }

    /* Allocate recording buffer */
    x_inst->pflt_record_buf = calloc (x_inst->u16_record_buf_len, sizeof (float));
    if (x_inst->pflt_record_buf == NULL)
    {
        LOGE ("Failed to allocate memory for recording buffer");
        return STATUS_ERR;
    }

    /* Initialize recording buffer */
    for (uint16_t u16_idx = 0; u16_idx < x_inst->u16_record_buf_len; u16_idx++)
    {
        x_inst->pflt_record_buf[u16_idx] = THER_INVALID_TEMP;
    }

    /* Create timer processing the ADC channel */
    x_inst->x_timer =
        xTimerCreate (NULL,                                         /* Human readable text name */
                      pdMS_TO_TICKS (x_inst->u16_sampling_cycle),   /* Timer period */
                      pdTRUE,                                       /* Auto reload */
                      x_inst,                                       /* Timer ID */
                      v_THER_Timer_Cb);                             /* Callback function when the timer expires */
    ASSERT_PARAM (x_inst->x_timer != NULL);

    /* Start the timer */
    ASSERT_PARAM (xTimerStart (x_inst->x_timer, portMAX_DELAY) == pdPASS);

    /* Done */
    return STATUS_OK;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Converts measured voltage into corresponding temperature using calibration point
**
** @note
**      Calibration points must be sorted in incremental order of measured voltage
**
** @note
**      There must be at least 2 calibration points
**
** @param [in]
**      x_inst: Thermistor instance returned by x_THER_Get_Inst() function
**
** @param [in]
**      flt_voltage: Measured voltage in volts
**
** @return
**      Corresponding temperature in Celsius degrees
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static float flt_THER_Calib (THER_inst_t x_inst, float flt_voltage)
{
    THER_calib_t *  pstru_points = x_inst->pstru_calib_points;
    uint8_t         u8_index_1   = 0;
    uint8_t         u8_index_2   = x_inst->u8_num_calib_points - 1;

    /* If the measured voltage is on the left of the first point */
    if (flt_voltage <= pstru_points[0].flt_voltage)
    {
        u8_index_2 = 1;
    }

    /* If the measured voltage is on the right of the last point */
    else if (flt_voltage >= pstru_points[u8_index_2].flt_voltage)
    {
        u8_index_1 = x_inst->u8_num_calib_points - 2;
    }

    /* Input is in between the first point and the last point */
    else
    {
        /* Determine the two consecutive points that cover the measured voltage */
        for (u8_index_2 = 1; u8_index_2 < x_inst->u8_num_calib_points; u8_index_2++)
        {
            if (flt_voltage < pstru_points[u8_index_2].flt_voltage)
            {
                u8_index_1 = u8_index_2 - 1;
                break;
            }
        }
    }

    /* Calculate coefficient of the line created by the 2 calibration points */
    float flt_coeff = (pstru_points[u8_index_2].flt_temperature - pstru_points[u8_index_1].flt_temperature) /
                      (pstru_points[u8_index_2].flt_voltage - pstru_points[u8_index_1].flt_voltage);

    /* Calibrating */
    float flt_result = (flt_coeff * (flt_voltage - pstru_points[u8_index_1].flt_voltage)) +
                       pstru_points[u8_index_1].flt_temperature;

    /* Done */
    return flt_result;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Callback function invoked when the timer processing temperature measurement expires
**
** @param [in]
**      x_timer: Handle of the timer
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void v_THER_Timer_Cb (TimerHandle_t x_timer)
{
    /* Timer ID of the timer contains the corresponding thermistor instance */
    THER_inst_t x_inst = pvTimerGetTimerID (x_timer);

    /* Get voltage of the associated ADC channel */
    int16_t s16_adc_voltage = s16_ADC_Get_Voltage (x_inst->x_adc_inst);
    if (s16_adc_voltage < 0)
    {
        LOGE ("Failed to get voltage of thermistor instance %d", x_inst->enm_inst_id);
        x_inst->flt_temp_filtered = THER_INVALID_TEMP;
        return;
    }

    /* Convert the ADC voltage into temperature using calibration points */
    x_inst->flt_temp_raw = flt_THER_Calib (x_inst, s16_adc_voltage / 1000.0);

    /* Apply IIR filter on the calculated temperature */
    if (x_inst->flt_temp_filtered == THER_INVALID_TEMP)
    {
        x_inst->flt_temp_filtered = x_inst->flt_temp_raw;
    }
    else
    {
        x_inst->flt_temp_filtered = x_inst->flt_filter_coeff * x_inst->flt_temp_raw +
                                    (1 - x_inst->flt_filter_coeff) * x_inst->flt_temp_filtered;
    }

    /* Put the filtered temperature value into the recording buffer */
    x_inst->u16_latest_record_idx = (x_inst->u16_latest_record_idx + 1) % x_inst->u16_record_buf_len;
    x_inst->pflt_record_buf[x_inst->u16_latest_record_idx] = x_inst->flt_temp_filtered;
}

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
