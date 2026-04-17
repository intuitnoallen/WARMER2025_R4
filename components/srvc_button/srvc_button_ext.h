/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : srvc_button_ext.h
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 Sep 17
**  @brief      : Header file contains configuration of Srvc_Button module
**  @namespace  : BTN
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  Srvc_Button
** @{
*/

#ifndef __SRVC_BUTTON_EXT_H__
#define __SRVC_BUTTON_EXT_H__

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
** @brief   This table defines button instances and their configuration.
** @details
**
** Button instances can be added to or removed from the table. Each button instance encapsulates a physical button on
** the hardware and has the following properties:
**
** - Button_Inst_ID         : Alias of a button. This alias is used in x_BTN_Get_Inst() to get handle of the
**                            corresponding button instance.
**
** - GPIO_Inst_ID           : Alias of the GPIO pin associated with a button. This alias is obtained from GPIO_Inst_ID
**                            column of GPIO_INST_TABLE (hwa_gpio_ext.h)
**
** - Debounce_Time          : Time in milliseconds (0 -> 65535) to debounce button state due to mechanical noise.
**                            A button must be pressed or released for this amount of time before its state transits
**                            to BTN_STATE_PRESSED or BTN_STATE_RELEASED, respectively.
**
** - Hold_Short_Time        : Time in milliseconds (1 -> 65535) to determine if a button has been hold for a short time.
**                            A button must be pressed and hold for this amount of time before its state transits
**                            to BTN_STATE_HOLD_SHORT.
**                            To disable hold-short feature, use BTN_NO_HOLD for this option.
**
** - Hold_Long_Time         : Time in milliseconds (1 -> 65535) to determine if a button has been hold for a long time.
**                            A button must be pressed and hold for this amount of time before its state transits
**                            to BTN_STATE_HOLD_LONG.
**                            To disable hold-long feature, use BTN_NO_HOLD for this option.
**
*/
#define BTN_INST_TABLE(X)                                                        \
                                                                                 \
/*-----------------------------------------------------------------------------*/\
/* Button_Inst_ID           Configuration                                      */\
/*-----------------------------------------------------------------------------*/\
                                                                                 \
/* Button ON                                                                   */\
X( BTN_BUTTON_ON,           /* GPIO_Inst_ID    */       GPIO_ON_BUTTON          ,\
                            /* Debounce_Time   1000 */  50                      ,\
                            /* Hold_Start_Time  */      HOLD_START_TIME         ,\
                            /* Hold_Short_Time  */      HOLD_ON_SHORT_TIME      ,\
                            /* Hold_Long_Time   */      HOLD_ON_LONG_TIME       )\
                                                                                 \
/* Button UP                                                                   */\
X( BTN_BUTTON_UP,           /* GPIO_Inst_ID    */       GPIO_UP_BUTTON          ,\
                            /* Debounce_Time 100   */   50                      ,\
                            /* Hold_Start_Time  */      HOLD_START_TIME         ,\
                            /* Hold_Short_Time  */      HOLD_UP_SHORT_TIME      ,\
                            /* Hold_Long_Time   */      HOLD_UP_LONG_TIME       )\
                                                                                 \
/* Button Thermometer                                                          */\
X( BTN_BUTTON_TH,           /* GPIO_Inst_ID    */       GPIO_TH_BUTTON          ,\
                            /* Debounce_Time 100   */   50                      ,\
                            /* Hold_Start_Time  */      HOLD_START_TIME         ,\
                            /* Hold_Short_Time  */      HOLD_TH_SHORT_TIME      ,\
                            /* Hold_Long_Time   */      HOLD_TH_LONG_TIME       )\                                                                   
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

#endif /* __SRVC_BUTTON_EXT_H__ */

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
