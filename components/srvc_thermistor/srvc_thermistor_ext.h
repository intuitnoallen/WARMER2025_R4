/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : srvc_thermistor_ext.h
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 May 21
**  @brief      : Header file contains configuration of Srvc_Thermistor module
**  @namespace  : THER
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  Srvc_Thermistor
** @{
*/

#ifndef __SRVC_THERMISTOR_EXT_H__
#define __SRVC_THERMISTOR_EXT_H__

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
** @brief   This table defines thermistor instances and their configuration.
** @details
**
** Each thermistor instance encapsulates a thermistor componenent and has the following properties:
**
** - Instance_ID            : Alias of a thermistor instance. This alias is used in x_THER_Get_Inst() to
**                            get handle of the corresponding thermistor.
**
** - ADC_Inst_ID            : Alias of the ADC channel instance measuring voltage of thermistor circuit. This alias is
**                            obtained from ADC_Inst_ID column of ADC_INST_TABLE (hwa_adc_ext.h)
**
** - Sampling_Cycle         : Period in milliseconds performing the temperature sampling and calculating.
**
** - Record_Buf_Len         : Number of elements of the buffer recording the calculated temperature. Element 0 stores
**                            current temperature, element 1 stores temperature of previous cycle, and so on
**
** - Filter_Level           : Level of the recursive Infinite Impulse Response (IIR) filter applied on the result
**                            temperature:
**                                  Filtered_Temperature = coeff x Raw_Temperature + (1 - coeff) x Filtered_Temperature
**                            Where:
**                                                   2
**                                  coeff = --------------------
**                                           2^Filter_Level + 1
**
**                            The following filter levels are valid:
**                              + 0 : Filter off
**                              + 1 -> 9 : Minimum filter (fast response) -> Maximum filter (low response)
**
**                            NOTE: if IIR filter is enabled, flt_THER_Get_Temperature() must be called periodically.
**
** - Low_Threshold          : Low limit in Celsius degrees of the measured temperature. Valid range: -40.0 .. 110.0
**
** - High_Threshold         : High limit in Celsius degrees of the measured temperature. Valid range: -40.0 .. 110.0
**
** - Calib_Point            : Calibration points help convert measured voltage into corresponding temperature. Each
**                            calibration point has the following properties:
**      + Voltage           : Measured voltage in volts (0 .. 2.5)
**      + Temperature       : Temperature in Celsius degrees corresponding with the measured voltage
**                            NOTE 1: Calibration points must be listed in incremental order of measured voltage
**                            NOTE 2: There must be at least 2 calibration points
**                            NOTE 3: The calibration points can be obtained via Resistance-Temperature table of the
**                                    thermistor (found in its datasheet) and this web tool:
**                                    https://www.bmotorsports.com/shop/static.php/post/tempcalc
*/
#define THER_INST_TABLE(X)                                                       \
                                                                                 \
/*-----------------------------------------------------------------------------*/\
/*  Instance_ID             Configuration                                      */\
/*-----------------------------------------------------------------------------*/\
                                                                                 \
/*  Thermistor measuring water temperature                                     */\
X(  THER_NTC_TEMP,          /* ADC_Inst_ID      */  ADC_NTC_SENSOR              ,\
                            /* Sampling_Cycle   */  100                         ,\
                            /* Record_Buf_Len   */  30                          ,\
                            /* Filter_Level     */  0                           ,\
                            /* Low_Threshold    */  0.0                         ,\
                            /* High_Threshold   */  65.0                        ,\
                            /* Calib_Point      */  { 0.903, 51.5 }             ,\
                            /* Calib_Point      */  { 0.953, 49.0 }             ,\
                            /* Calib_Point      */  { 0.962, 48.6 }             ,\
                            /* Calib_Point      */  { 0.979, 47.9 }             ,\
                            /* Calib_Point      */  { 1.011, 46.4 }             ,\
                            /* Calib_Point      */  { 1.036, 45.4 }             ,\
                            /* Calib_Point      */  { 1.078, 43.6 }             ,\
                            /* Calib_Point      */  { 1.116, 41.9 }             ,\
                            /* Calib_Point      */  { 1.204, 38.5 }             ,\
                            /* Calib_Point      */  { 1.234, 37.2 }             ,\
                            /* Calib_Point      */  { 1.258, 36.2 }             ,\
                            /* Calib_Point      */  { 1.349, 32.8 }             ,\
                            /* Calib_Point      */  { 1.6,   24.2 }             ,\
                            /* Calib_Point      */  { 1.798, 18.5 }             ,\
                            /* Calib_Point      */  { 1.912, 13.9 }             ,\
                            /* Calib_Point      */  { 2.073,  9.1 }             ,\
                            /* Calib_Point      */  { 2.227,  0.0 }             )\
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

#endif /* __SRVC_THERMISTOR_EXT_H__ */

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
