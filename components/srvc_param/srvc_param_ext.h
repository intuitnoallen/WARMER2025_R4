/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : srvc_param_ext.h
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 Sep 26
**  @brief      : Extended public header of Srvc_Param module
**  @namespace  : PARAM
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  Srvc_Param
** @{
*/

#ifndef __SRVC_PARAM_EXT_H__
#define __SRVC_PARAM_EXT_H__

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
** @brief   Parameter table
** @details
**
** Definition of parameter table. Each parameter has following properties:
**
** - Param_ID       : Unique literal definition of a parameter
**
** - PUC            : Parameter unique code (16-bit number) uniquely identifying a parameter
**
** - Type           : Data type of a parameter. The following types are supported:
**                      + uint8_t, int8_t
**                      + uint16_t, int16_t
**                      + uint32_t, int32_t
**                      + uint64_t, int64_t
**                      + string: NUL-terminated string
**                      + blob: variable length binary data (array of uint8_t elements)
**
** - Min, Max       : Minimum and maximum value of a parameter. If the parameter value is not within this range, it
**                    shall be reset to default value when Srvc_Param is initialized. Setting both Min and Max to 0
**                    shall disable these limits.
**                    For parameters of types string and blob, these settings are minimum and maximum length in bytes
**                    of data stored.
**
** - Default        : Initialized value of the parameter when it is first created or restored from a corruption.
**                    Some examples:
**                         Type     |   Default
**                      ------------+-----------------------
**                         uint8_t  |   12
**                         int16_t  |   -1234
**                         string   |   "Default string"
**                         blob     |   {1, 2, 3, 4, 5, 6}
*/
#define PARAM_TABLE(X)                                                                                                 \
                                                                                                                       \
/*-------------------------------------------------------------------------------------------------------------------*/\
/* Param_ID                         PUC         Type        Min         Max         Default                          */\
/*-------------------------------------------------------------------------------------------------------------------*/\
                                                                                                                       \
/* Temperature set point in Celsius degrees */                                                                         \
X( PARAM_TEMP_SET_POINT,            0x0100,     uint8_t,    30,         50,         38                                )\
                                                                                                                       \
/* Temperature unit: 0 = Celius degree, 1 = Fahrenheit */                                                              \
X( PARAM_TEMP_UNIT,                 0x0101,     uint8_t,    0,          1,          0                                 )\
                                                                                                                       \
/* Heater mode: 0 = Milk, 1 = Water, 2 = Frozen */                                                                     \
X( PARAM_HEATER_MODE,               0x0102,     uint8_t,    0,          3,          0                                 )\
                                                                                                                       \
/* Specifies whether wigg mode is enabled: 0 = disabled, 1 = enabled */ /*NOT USED NOW*/                               \
X( PARAM_CHILD_LOCK_ENABLED,        0x0103,     uint8_t,    0,          1,          0                                 )\
                                                                                                                       \
/* Specifies whether buzzer is enabled: 0 = disabled, 1 = enabled */                                                   \
X( PARAM_BUZZER_ENABLED,            0x0104,     uint8_t,    0,          1,          1                                 )\
																													   \
/* Volume level: 0 = 120ml, 1 = 240ml */                                                                               \
X( PARAM_VOLUME_LEVEL,              0x0105,     uint8_t,    0,          1,          0                                 )\
                                                                                                                       \
/* HEATER Keep Level */                                                                                                \
X( PARAM_EXTENDED_WARMER_LEVEL,     0x0106,     uint8_t,    0,          5,          0                                 )\
                                                                                                                       \
/* Buzzer Volume Level when buzzer enabled: 1,2,3,5,50 */                                                              \
X( PARAM_BUZZER_VOLUME,             0x0107,     uint8_t,     1,         100,        100                                )\
                                                                                                                       \
/* Specifies whether vibration is enabled: 0 = disabled, 1 = enabled */                                                \
X( PARAM_VIBRATION_ENABLED,         0x0108,     uint8_t,     0,          1,         1                                 )\
                                                                                                                       \
/* Specifies whether wigg mode is enabled: 0 = disabled, 1 = enabled */                                                \
X( PARAM_WIGG_MODE_ENABLED,         0x0109,     uint8_t,      0,          1,         1                                )\
                                                                                                                       \
/* Specifies language, 0=en-US, 1=zh-SG, 2=ja-JP, 3=ko-KR */                                                           \
X( PARAM_DEVICE_LANGUAGE,           0x0110,     uint8_t,      0,          3,         0                                )\
                                                                                                                       \
/* LOW BAT ALERT vbat < 11700 LOW=1  NORMAL=0*/                                                                        \
X( PARAM_LOW_BAT_ALERT,             0x0111,     uint8_t,      0,          1,         0                                )\
                                                                                                                       \
/* HIGH BAT ALERT vbat > 16500 HIGH=1  NORMAL=0*/                                                                      \
X( PARAM_HIGH_BAT_ALERT,            0x0112,     uint8_t,      0,          1,         0                                )\
                                                                                                                       \
/* HIGH BAT ALERT vbat > 16500 HIGH=1  NORMAL=0*/                                                                      \
X( PARAM_BAT_PERCENT,               0x0113,     uint8_t,      0,          100,       0                                )\




/*-------------------------------------------------------------------------------------------------------------------*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           PROTOTYPES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#endif /* __SRVC_PARAM_EXT_H__ */

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
