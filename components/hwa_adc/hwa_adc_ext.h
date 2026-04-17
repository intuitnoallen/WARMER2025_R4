/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : hwa_adc_ext.h
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 May 15
**  @brief      : Extended header of Hwa_ADC module. This header is public to other modules for use.
**  @namespace  : ADC
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  Hwa_ADC
** @{
*/

#ifndef __HWA_ADC_EXT_H__
#define __HWA_ADC_EXT_H__

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           INCLUDES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           DEFINES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @brief   This table configures instances of ADC channels
** @details
**
** Each ADC instance encapsulates one ADC channel of either ADC unit. Each instance has the following properties:
**
** - ADC_Inst_ID            : Alias of the ADC channel instance
**
** - ADC_Unit               : ADC unit of the instance, which is either ADC_UNIT_1 or ADC_UNIT_2
**
** - ADC_Channel            : ADC channel number of the instance.
**                            On ESP32-C3, the following pins have ADC function:
**
**                            ADC unit      |          ADC_UNIT_1         | ADC_UNIT_2
**                            --------------|-----|-----|-----|-----|-----|------------
**                            ADC channel   |  0  |  1  |  2  |  3  |  4  |     0
**                            --------------|-----|-----|-----|-----|-----|------------
**                            GPIO number   |  0  |  1  |  2  |  3  |  4  |     5
**
** - Attenuation            : Attenuation in dB of an ADC channel. By setting higher attenuation it is possible to
**                            convert voltage higher than reference voltage (1100 mV).
**                            On ESP32-C3, the following attenuation values are supported:
**
**                            Attenuation   | Suggested voltage range
**                            --------------|------------------------
**                              DB_0        | 0 mV ~ 750 mV
**                              DB_2_5      | 0 mV ~ 1050 mV
**                              DB_6        | 0 mV ~ 1300 mV
**                              DB_11       | 0 mV ~ 2500 mV
**
** - Sampling_Cycle         : Period in milliseconds performing the analog conversion and status monitoring.
**
** - Filter_Level           : Level of the recursive Infinite Impulse Response (IIR) filter applied on ADC raw reading:
**                                  Filtered_Raw_ADC = coeff x Raw_ADC + (1 - coeff) x Filtered_Raw_ADC
**                            Where:
**                                                   2
**                                  coeff = --------------------
**                                           2^Filter_Level + 1
**
**                            The following filter levels are valid:
**                              + 0 : Filter off
**                              + 1 -> 9 : Minimum filter (fast response) -> Maximum filter (low response)
**
** - Present_Debounce       : The interval in milliseconds to detect if analog input signal of an ADC channel is
**                            present
**
** - Missing_Debounce       : The interval in milliseconds to detect if analog input signal of an ADC channel is
**                            missing
**
*/
#define ADC_INST_TABLE(X)                                                        \
                                                                                 \
/*-----------------------------------------------------------------------------*/\
/*  ADC_Inst_ID             Configuration                                      */\
/*-----------------------------------------------------------------------------*/\
                                                                                 \
/*  NTC thermistor sensor                                                      */\
X(  ADC_NTC_SENSOR,         /* ADC_Unit         */  ADC_UNIT_2                  ,\
                            /* ADC_Channel  3   */  6                           ,\
                            /* Attenuation      */  DB_11                       ,\
                            /* Sampling_Cycle   */  100                         ,\
                            /* Filter_Level     */  6                           ,\
                            /* Present_Debounce */  200                         ,\
                            /* Missing_Debounce */  1000                        )\
                                                                                 \
/*  Monitor battery supply  */                                                   \
X(  ADC_14V_BP_MON,         /* ADC_Unit         */  ADC_UNIT_1                  ,\
                            /* ADC_Channel      */  3                           ,\
                            /* Attenuation      */  DB_11                       ,\
                            /* Sampling_Cycle   */  10                          ,\
                            /* Filter_Level     */  5                           ,\
                            /* Present_Debounce */  200                         ,\
                            /* Missing_Debounce */  1000                         )\
                                                                                 \
X(  ADC_16V_EP_MON,         /* ADC_Unit         */  ADC_UNIT_1                  ,\
                            /* ADC_Channel      */  1                           ,\
                            /* Attenuation      */  DB_11                       ,\
                            /* Sampling_Cycle   */  30                          ,\
                            /* Filter_Level     */  3                           ,\
                            /* Present_Debounce */  200                         ,\
                            /* Missing_Debounce */  1000                        )\
X(  ADC_CHG_CUR_MON,         /* ADC_Unit         */ ADC_UNIT_2                  ,\
                            /* ADC_Channel      */  7                           ,\
                            /* Attenuation      */  DB_11                       ,\
                            /* Sampling_Cycle   */  100                         ,\
                            /* Filter_Level     */  4                           ,\
                            /* Present_Debounce */  200                         ,\
                            /* Missing_Debounce */  1000                        )\                                                                                  
/*-----------------------------------------------------------------------------*/


/**
** @brief   This table configures ADC units
** @details
**
** ESP32-C3 has two ADC units - ADC1 (5 channels) and ADC2 (1 channels). Each unit has the following properties:
**
** - ADC_Unit               : ADC unit enumeration, which is ADC_UNIT_1 or ADC_UNIT_2
**
** - ADC_Resolution         : Capture resolution of the ADC unit in bits. For ESP32-C3, ADC resolution must be 12.
**
** - ADC_Reference_mV       : Reference voltage of the ADC unit in millivolts. Per chip design the (internal) ADC
**                            reference voltage is 1100 mV, however the true reference voltage can range from 1000 mV
**                            to 1200 mV amongst different chips. This property MAY affect the precision of
**                            s16_ADC_Get_Voltage().
**                            For more details, refer here:
**                            https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/peripherals/adc.html#adc-calibration
**
*/
#define ADC_UNIT_TABLE(X)                                                        \
                                                                                 \
/*-----------------------------------------------------------------------------*/\
/*  ADC_Unit                Configuration                                      */\
/*-----------------------------------------------------------------------------*/\
                                                                                 \
/*  ADC unit 1                                                                 */\
X(  ADC_UNIT_1,             /* ADC_Resolution       */      12                  ,\
                            /* ADC_Reference_mV     */      1100                )\
                                                                                 \
/*  ADC unit 2                                                                 */\
X(  ADC_UNIT_2,             /* ADC_Resolution       */      12                  ,\
                            /* ADC_Reference_mV     */      1100                )\
                                                                                 \
/*-----------------------------------------------------------------------------*/

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


#endif /* __HWA_ADC_EXT_H__ */

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
