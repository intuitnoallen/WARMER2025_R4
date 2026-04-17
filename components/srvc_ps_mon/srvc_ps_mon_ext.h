/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : srvc_ps_mon_ext.h
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 Oct 24
**  @brief      : Header file contains configuration of Srvc_PS_Mon module
**  @namespace  : PSMON
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  Srvc_PS_Mon
** @{
*/

#ifndef __SRVC_PS_MON_EXT_H__
#define __SRVC_PS_MON_EXT_H__

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
** @brief   This table defines power supply instances and their configuration.
** @details
**
** Each power supply instance monitoring a power supply and has the following properties:
**
** - Instance_ID            : Alias of a power supply monitoring voltage. This alias is used in x_PSMON_Get_Inst() to
**                            get handle of the corresponding monitoring instance.
**
** - ADC_Inst_ID            : Alias of the ADC channel instance measuring power supply voltage. This alias is obtained
**                            from ADC_Inst_ID column of ADC_INST_TABLE (hwa_adc_ext.h)
**
** - Mul_Factor             : Multiplication factor calculating power supply voltage from ADC measured voltage
**                              Power supply voltage = Measured voltage * Mul_Factor + Offset_Factor
**
** - Offset_Factor          : Offset factor (in millivolts) calculating power supply voltage from ADC measured voltage
**                              Power supply voltage = Measured voltage * Mul_Factor + Offset_Factor
**
** - Low_Threshold          : If the power supply voltage is below this threshold (millivolts), the voltage is too low.
**
** - High_Threshold         : If the power supply voltage is above this threshold (millivolts), the voltage is too high
**                            and may cause battery damage.
**
** - Voltage_SoC            : Voltage_SoC points are used for battery power supply. They help convert power supply
**                            voltage into state of charge of the battery (percentage remaining). The relationship
**                            between SoC and battery voltage depends on the chemical material of the battery.
**                            Each Voltage_SoC point has the following properties:
**      + Voltage           : Battery voltage in millivolts (integer number, 0 -> 65535)
**      + State_Of_Charge   : Battery charge remaining in percents (integer number, 0 -> 100)
**                            NOTE 1: Voltage_SoC points must be listed in incremental order of measured voltage
**                            NOTE 2: There must be at least 2 Voltage_SoC points
**                            NOTE 3: The Voltage_SoC points can be calculated from State-of-charge Vs Voltage table
**                                    provided in datasheet of the battery used
*/
#define PSMON_INST_TABLE(X)                                                      \
                                                                                 \
/*-----------------------------------------------------------------------------*/\
/*  Instance_ID             Configuration                                      */\
/*-----------------------------------------------------------------------------*/\
                                                                                 \
/*  14V power supply                                                           */\
    X(  BP_MON_14V,             /* ADC_Inst_ID      */  ADC_14V_BP_MON          ,\
                            /* Mul_Factor       */  (281.2 / 41.2)              ,\
                            /* Offset_Factor    */  0                           ,\
                            /* Low_Threshold    */  13500                       ,\
                            /* High_Threshold   */  16500                       ,\
                            /* Voltage_SoC      */  { 11700,   1 }              ,\
                            /* Voltage_SoC      */  { 13500,   10 }              ,\
                            /* Voltage_SoC      */  { 16500/*15900*/, 100 }     )\
                                                                                 \
/*  16V ext plug                                                               */\
X(  EP_MON_16V,             /* ADC_Inst_ID      */  ADC_16V_EP_MON              ,\
                            /* Mul_Factor       */  (281.2 / 41.2)              ,\
                            /* Offset_Factor    */  0                           ,\
                            /* Low_Threshold    */  16300                       ,\
                            /* High_Threshold   */  17000                       ,\
                                                                                )\
/*  ext plug current */                                                          \
X(  EP_CURR_MON,            /* ADC_Inst_ID      */  ADC_CHG_CUR_MON             ,\
                            /* Mul_Factor=Shunt */  0.025                       ,\
                            /* Offset_Factor=GAIn*/ 20                          ,\
                            /* Low_Threshold    */  0                           ,\
                            /* High_Threshold   */  0                           ,\
                                                                                 )\             
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

#endif /* __SRVC_PS_MON_EXT_H__ */

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
