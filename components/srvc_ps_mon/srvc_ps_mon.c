/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : srvc_ps_mon.c
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 Oct 24
**  @brief      : Implementation of Srvc_PS_Mon module
**  @namespace  : PSMON
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  Srvc_PS_Mon
** @brief       Srvc_PS_Mon module encapsulates the monitoring of power supply sources and provides API to get voltage,
**              battery's state of charge and status of the monitored power supplies.
**
**                                  +---------------+          +--------------------+
**              ADC reading ------->| Mul_Factor    |-----+----| Voltage_SoC points |-----> State of charge (%)
**                                  | Offset_Factor |     |    +--------------------+
**                                  +---------------+     |
**                                                        +-------------------------------> Power supply voltage (mV)
** @{
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           INCLUDES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include "srvc_ps_mon.h"        /* Public header of this module */
#include "hwa_adc.h"            /* Use hardware abstraction of ADC channels for voltage measurement */

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           DEFINES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Structure wrapping a Voltage_SoC point */
typedef struct
{
    uint16_t            u16_voltage;            //!< Battery voltage in millivolts
    uint8_t             u8_percentage;          //!< Battery charge remaining in percents
} PSMON_vsoc_t;

/** @brief  Structure encapsulating a power supply object */
struct PSMON_obj
{
    bool                b_initialized;          //!< Specifies whether the object has been initialized or not
    PSMON_inst_id_t     enm_inst_id;            //!< Instance ID of this object

    ADC_inst_id_t       enm_adc_inst_id;        //!< Instance ID of the ADC channel measuring power supply voltage
    ADC_inst_t          x_adc_inst;             //!< Instance handle of the ADC channel measuring power supply voltage

    float               flt_mul_factor;         //!< Multiplication factor calculating power supply voltage
    float               flt_offset_factor;      //!< Offset factor calculating power supply voltage
    uint16_t            u16_low_mV;             //!< Low threshold (in mV) of power supply voltage
    uint16_t            u16_high_mV;            //!< High threshold (in mV) of power supply voltage
    uint8_t             u8_num_vsoc_points;     //!< Number of Voltage_SoC points
    PSMON_vsoc_t *      pstru_vsoc_points;      //!< Pointer to array of Voltage_SoC points
};

/** @brief  Macro expanding PSMON_INST_TABLE as initialization value for PSMON_obj struct */
#define INST_TABLE_EXPAND_AS_STRUCT_INIT(INST_ID, ADC, MUL, OFFSET, LOW, HIGH, ...)         \
{                                                                                           \
    .b_initialized      = false,                                                            \
    .enm_inst_id        = INST_ID,                                                          \
                                                                                            \
    .enm_adc_inst_id    = ADC,                                                              \
    .x_adc_inst         = NULL,                                                             \
                                                                                            \
    .flt_mul_factor     = MUL,                                                              \
    .flt_offset_factor  = OFFSET,                                                           \
    .u16_low_mV         = LOW,                                                              \
    .u16_high_mV        = HIGH,                                                             \
    .u8_num_vsoc_points = sizeof (g_astru_vsoc_points_##INST_ID) / sizeof (PSMON_vsoc_t),   \
    .pstru_vsoc_points  = g_astru_vsoc_points_##INST_ID,                                    \
},

/** @brief  Macro expanding PSMON_INST_TABLE as array of Voltage_SoC points */
#define INST_TABLE_EXPAND_AS_VSOC_POINT_ARRAY(INST_ID, ADC, MUL, OFFSET, LOW, HIGH, ...)    \
    static PSMON_vsoc_t g_astru_vsoc_points_##INST_ID [] = { __VA_ARGS__ };

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           VARIABLES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Logging tag of this module */
static const char * TAG = "Srvc_PS_Mon";

/** @brief  Indicates if this module has been initialized */
static bool g_b_initialized = false;

/** @brief  Arrays of all Voltage_SoC points */
PSMON_INST_TABLE (INST_TABLE_EXPAND_AS_VSOC_POINT_ARRAY)

/** @brief  Array of all power supply objects */
static struct PSMON_obj g_astru_psmon_objs[PSMON_NUM_INST] =
{
    PSMON_INST_TABLE (INST_TABLE_EXPAND_AS_STRUCT_INIT)
};

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           PROTOTYPES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

static int32_t s32_PSMON_Init_Module (void);
static int32_t s32_PSMON_Init_Inst (PSMON_inst_t x_inst);
static uint8_t u8_PSMON_Convert_Voltage_To_Percent (PSMON_inst_t x_inst, uint16_t u16_voltage);

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           FUNCTIONS SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets instance handle of a power supply. This instance will be used for other functions in this module.
**
** @param [in]
**      enm_inst_id: Index of the power supply instance to get. Power supply indexes are expanded from Instance_ID
**                   column of PSMON_INST_TABLE (srvc_ps_mon_ext.h)
**
** @return
**      @arg    NULL: Failed to get instance of the power supply
**      @arg    Otherwise: Instance handle of the power supply
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
PSMON_inst_t x_PSMON_Get_Inst (PSMON_inst_id_t enm_inst_id)
{
    PSMON_inst_t    x_inst = NULL;
    int32_t         s32_result = STATUS_OK;

    /* Validation */
    if (enm_inst_id >= PSMON_NUM_INST)
    {
        return NULL;
    }

    /* If this module has not been initialized, do that now */
    if (!g_b_initialized)
    {
        s32_result = s32_PSMON_Init_Module ();
        g_b_initialized = (s32_result == STATUS_OK);
    }

    /* If the retrieved instance has not been initialized yet, do that now */
    if (s32_result == STATUS_OK)
    {
        x_inst = &g_astru_psmon_objs[enm_inst_id];
        if (!x_inst->b_initialized)
        {
            s32_result = s32_PSMON_Init_Inst (x_inst);
            x_inst->b_initialized = (s32_result == STATUS_OK);
        }
    }

    /* Return instance of the required Power supply */
    return (s32_result == STATUS_OK) ? x_inst : NULL;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets voltage (milli-volts) and remaining battery (percents) of a power supply source
**
** @param [in]
**      x_inst: Power supply instance returned by x_PSMON_Get_Inst() function
**
** @param [out]
**      pu16_voltage: Pointer to the container for battery voltage, this can be NULL
**
** @param [out]
**      pu8_percent: Pointer to the container for battery percents, this can be NULL
**
** @return
**      @arg    PSMON_OK
**      @arg    PSMON_ERR
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
PSMON_status_t enm_PSMON_Get_Bat_Info (PSMON_inst_t x_inst, uint16_t * pu16_voltage, uint8_t * pu8_percent)
{
    /* Validation */
    ASSERT_PARAM ((x_inst != NULL) && x_inst->b_initialized);

    /* Get voltage of from the associated ADC channel */
    int16_t s16_adc_voltage = s16_ADC_Get_Voltage (x_inst->x_adc_inst);
    if (s16_adc_voltage < 0)
    {
        LOGE ("Failed to get voltage of power supply instance %d", x_inst->enm_inst_id);
        return PSMON_ERR;
    }

    /* Convert the ADC voltage into power supply voltage */
    float flt_voltage = s16_adc_voltage * x_inst->flt_mul_factor + x_inst->flt_offset_factor;
    if (pu16_voltage != NULL)
    {
        *pu16_voltage = flt_voltage;
    }

    /* Calculate percents of remaining battery */
    if (pu8_percent != NULL)
    {
        *pu8_percent = u8_PSMON_Convert_Voltage_To_Percent (x_inst, flt_voltage);
    }

    return PSMON_OK;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets voltage (milli-volts) of a power plug source
**
** @param [in]
**      x_inst: Power plug instance returned by x_PSMON_Get_Inst() function
**
** @param [out]
**      pu16_voltage: Pointer to the container for plug voltage, this can be NULL
**
** @return
**      @arg    PSMON_OK
**      @arg    PSMON_ERR
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
PSMON_status_t enm_PSMON_Get_Plug_Info (PSMON_inst_t x_inst, uint16_t * pu16_voltage)
{
    /* Validation */
    ASSERT_PARAM ((x_inst != NULL) && x_inst->b_initialized);

    /* Get voltage of from the associated ADC channel */
    int16_t s16_adc_voltage = s16_ADC_Get_Voltage (x_inst->x_adc_inst);
    if (s16_adc_voltage < 0)
    {
        LOGE ("Failed to get voltage of power plug instance %d", x_inst->enm_inst_id);
        return PSMON_ERR;
    }

    /* Convert the ADC voltage into power plug voltage */
    float flt_voltage = s16_adc_voltage * x_inst->flt_mul_factor + x_inst->flt_offset_factor;
    if (pu16_voltage != NULL)
    {
        *pu16_voltage = flt_voltage;
    }

    return PSMON_OK;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets current of a power plug source
**
** @param [in]
**      x_inst: Current sensor instance returned by x_PSMON_Get_Inst() function
**
** @param [out]
**      pu16_current: Pointer to the container for plug current draw, this can be NULL
**
** @return
**      @arg    PSMON_OK
**      @arg    PSMON_ERR
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
PSMON_status_t enm_PSMON_Get_EP_Curr_Info (PSMON_inst_t x_inst, uint16_t * pu16_current)
{
    /* Validation */
    ASSERT_PARAM ((x_inst != NULL) && x_inst->b_initialized);

    /* Get voltage of from the associated ADC channel */
    int16_t s16_adc_voltage = s16_ADC_Get_Voltage (x_inst->x_adc_inst);
    if (s16_adc_voltage < 0)
    {
        LOGE ("Failed to get voltage of power plug instance %d", x_inst->enm_inst_id);
        return PSMON_ERR;
    }

    /* Convert the ADC voltage into power plug voltage current = adc_volatge/(Shunt*GAIN)  */
    float flt_current = s16_adc_voltage / (x_inst->flt_mul_factor * x_inst->flt_offset_factor);
    if (pu16_current != NULL)
    {
        *pu16_current = flt_current;
    }

    return PSMON_OK;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets alert status of a power supply source
**
** @param [in]
**      x_inst: Power supply instance returned by x_PSMON_Get_Inst() function
**
** @return
**      Alert status
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
PSMON_alert_t enm_PSMON_Get_Bat_Alert_Status (PSMON_inst_t x_inst)
{
    /* Validation */
    ASSERT_PARAM ((x_inst != NULL) && x_inst->b_initialized);

    /* Get voltage from the associated ADC channel */
    int16_t s16_adc_voltage = s16_ADC_Get_Voltage (x_inst->x_adc_inst);
    if (s16_adc_voltage < 0)
    {
        LOGE ("Failed to get voltage of power supply instance %d", x_inst->enm_inst_id);
        return PSMON_ALERT_UNKNOWN;
    }

    /* Convert the ADC voltage into power supply voltage */
    float flt_voltage = s16_adc_voltage * x_inst->flt_mul_factor + x_inst->flt_offset_factor;

    /* Determine status */
    if (flt_voltage > x_inst->u16_high_mV)
    {
        return PSMON_ALERT_HIGH;
    }
    else if (flt_voltage < x_inst->u16_low_mV)
    {
        uint8_t u8_percents = u8_PSMON_Convert_Voltage_To_Percent (x_inst, flt_voltage);
        return ((u8_percents == 0) ? PSMON_ALERT_EMPTY : PSMON_ALERT_LOW);
    }
    else
    {
        return PSMON_ALERT_NONE;
    }
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets alert status of a external plug power source
**
** @param [in]
**      x_inst: Power supply instance returned by x_PSMON_Get_Inst() function
**
** @return
**      Alert status
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
PSMON_alert_t enm_PSMON_Get_Plug_Alert_Status (PSMON_inst_t x_inst)
{
    /* Validation */
    ASSERT_PARAM ((x_inst != NULL) && x_inst->b_initialized);

    /* Get voltage from the associated ADC channel */
    int16_t s16_adc_voltage = s16_ADC_Get_Voltage (x_inst->x_adc_inst);
    if (s16_adc_voltage < 0)
    {
        LOGE ("Failed to get voltage of power supply instance %d", x_inst->enm_inst_id);
        return EPMON_ALERT_UNKNOWN;
    }

    /* Convert the ADC voltage into plug voltage */
    float flt_voltage = s16_adc_voltage * x_inst->flt_mul_factor + x_inst->flt_offset_factor;

    /* Determine status */
    if (flt_voltage > x_inst->u16_high_mV  || flt_voltage < x_inst->u16_low_mV)
    {
        return EPMON_ALERT_WRONG_ADAPTER;
    }
    else
    {
        return EPMON_ALERT_NONE;
    }
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Initializes Srvc_PS_Mon module
**
** @return
**      @arg    STATUS_OK
**      @arg    STATUS_ERR
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int32_t s32_PSMON_Init_Module (void)
{
    /* Do nothing */
    return STATUS_OK;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Initializes a Power supply instance
**
** @param [in]
**      x_inst: Power supply instance returned by x_PSMON_Get_Inst() function
**
** @return
**      @arg    STATUS_OK
**      @arg    STATUS_ERR
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int32_t s32_PSMON_Init_Inst (PSMON_inst_t x_inst)
{
    /* Get instance handle of the associated ADC channel used to monitor power supply voltage */
    x_inst->x_adc_inst = x_ADC_Get_Inst (x_inst->enm_adc_inst_id);
    if (x_inst->x_adc_inst == NULL)
    {
        LOGE ("Failed to get instance of ADC channel");
        return STATUS_ERR;
    }

    /* Done */
    return STATUS_OK;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Converts battery voltage into state of charge (percents)
**
** @note
**      Voltage_SoC points must be sorted in incremental order of battery voltage
**
** @note
**      There must be at least 2 Voltage_SoC points
**
** @param [in]
**      x_inst: Power supply instance returned by x_PSMON_Get_Inst() function
**
** @param [in]
**      u16_voltage: Battery voltage in milli-volts
**
** @return
**      Battery's state of charge in percents
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static uint8_t u8_PSMON_Convert_Voltage_To_Percent (PSMON_inst_t x_inst, uint16_t u16_voltage)
{
    PSMON_vsoc_t *  pstru_points = x_inst->pstru_vsoc_points;
    uint8_t         u8_index_1   = 0;
    uint8_t         u8_index_2   = x_inst->u8_num_vsoc_points - 1;

    /* If the battery voltage is on the left of the first point */
    if (u16_voltage <= pstru_points[0].u16_voltage)
    {
        u8_index_2 = 1;
    }

    /* If the battery voltage is on the right of the last point */
    else if (u16_voltage >= pstru_points[u8_index_2].u16_voltage)
    {
        u8_index_1 = x_inst->u8_num_vsoc_points - 2;
    }

    /* Input is in between the first point and the last point */
    else
    {
        /* Determine the two consecutive points that cover the battery voltage */
        for (u8_index_2 = 1; u8_index_2 < x_inst->u8_num_vsoc_points; u8_index_2++)
        {
            if (u16_voltage < pstru_points[u8_index_2].u16_voltage)
            {
                u8_index_1 = u8_index_2 - 1;
                break;
            }
        }
    }

    /* Calculate coefficient of the line created by the 2 Voltage_SoC points */
    float flt_coeff = (float)(pstru_points[u8_index_2].u8_percentage - pstru_points[u8_index_1].u8_percentage) /
                      (float)(pstru_points[u8_index_2].u16_voltage - pstru_points[u8_index_1].u16_voltage);

    /* Battery's state of charge */
    float flt_result = (flt_coeff * (u16_voltage - pstru_points[u8_index_1].u16_voltage)) +
                       pstru_points[u8_index_1].u8_percentage;

    /* Done */
    return ((flt_result >= 100) ? 99 : (flt_result <= 0) ? 1 : (uint8_t)flt_result);
}

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
