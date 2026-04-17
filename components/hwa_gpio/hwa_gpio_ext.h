/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : hwa_gpio_ext.h
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 Sep 14
**  @brief      : Extended header of Hwa_GPIO module. This header is public to other modules for use.
**  @namespace  : GPIO
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  Hwa_GPIO
** @{
*/

#ifndef __HWA_GPIO_EXT_H__
#define __HWA_GPIO_EXT_H__

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
** @brief   This table defines GPIO instances and their configuration.
** @details
**
** Each GPIO instance encapsulates a GPIO pin and has the following properties:
**
** - GPIO_Inst_ID           : Alias of the GPIO instance. This alias is used in s8_GPIO_Get_Inst() to get handle of the
**                            corresponding GPIO instance.
**
** - GPIO_Number            : GPIO number (0 -> 39). For example, GPIO number of "IO14" is "14"
**
** - Direction              : Initial direction of the GPIO pin, direction could be changed during run-time
**      + INPUT             : Input pin
**      + OUTPUT            : Output pin
**
** - Active_Level           : Specifies the logic level at which the component connected the GPIO pin is "active".
**                            For example, if the GPIO pin is driving a LED, active level is the level makes the LED
**                            to turn on; if the GPIO pin is connected a button, active level is the level when the
**                            button is pressed.
**      + 0                 : The GPIO level is "0" when active and "1" when not active
**      + 1                 : The GPIO level is "1" when active and "0" when not active
**
** - Pull_Mode              : Internal pull resistor (45 Kohm). Note that GPIOs 34-39 don't have internal pull resistors
**      + PULLUP_ONLY       : Pad pull up
**      + PULLDOWN_ONLY     : Pad pull down
**      + PULLUP_PULLDOWN   : Pad pull up + pull down
**      + FLOATING          : Pad floating
**
** - Is_Open_Drain          : This option is applied for output pin only
**      + true              : The output pin is in open drain mode
**      + false             : The output pin is in push-pull mode
**
** - Drive_Strength         : Pad drive capacity, for output pins only
**      + 0                 : Weak
**      + 1                 : Stronger
**      + 2                 : Medium
**      + 3                 : Strongest
**      + DEFAULT           : Default drive capacity (2)
**
*/
#define GPIO_INST_TABLE(X)                                                              \
                                                                                        \
/*------------------------------------------------------------------------------------*/\
/*  GPIO_Inst_ID             Configuration                                            */\
/*------------------------------------------------------------------------------------*/\
                                                                                        \
/*  On button. Active when button is pressed                                          */\
X(  GPIO_SLEEP_AUX,         /* GPIO_Number      */      32                             ,\
                            /* Direction        */      OUTPUT                         ,\
                            /* Active_Level     */      0                              ,\
                            /* Pull_Mode        */      FLOATING                       ,\
                            /* Is_Open_Drain    */      true                           ,\
                            /* Drive_Strength   */      DEFAULT                        )\
                                                                                        \
/*  On button. Active when button is pressed                                          */\
X(  GPIO_ON_BUTTON,         /* GPIO_Number 5   35  */   34                             ,\
                            /* Direction        */      INPUT                          ,\
                            /* Active_Level     */      1                              ,\
                            /* Pull_Mode        */      FLOATING                       ,\
                            /* Is_Open_Drain    */      false                          ,\
                            /* Drive_Strength   */      DEFAULT                        )\
                                                                                        \
/*  On button. Active when button is pressed                                          */\
X(  GPIO_UP_BUTTON,         /* GPIO_Number4   34     */ 35                             ,\
                            /* Direction        */      INPUT                          ,\
                            /* Active_Level     */      1                              ,\
                            /* Pull_Mode        */      FLOATING                       ,\
                            /* Is_Open_Drain    */      false                          ,\
                            /* Drive_Strength   */      DEFAULT                        )\
/*  Thermometer button. Active when button is pressed                                 */\
X(  GPIO_TH_BUTTON,         /* GPIO_Number4   34     */ 2                              ,\
                            /* Direction        */      INPUT                          ,\
                            /* Active_Level     */      1                              ,\
                            /* Pull_Mode        */      FLOATING                       ,\
                            /* Is_Open_Drain    */      false                          ,\
                            /* Drive_Strength   */      DEFAULT                        )\
/*  Tilt sensor. Active when the device tilts  */                                       \
X(  GPIO_TILT_SENSOR,       /* GPIO_Number7 38     */    38                            ,\
                            /* Direction        */      INPUT                          ,\
                            /* Active_Level     */      0                              ,\
                            /* Pull_Mode        */      FLOATING                       ,\
                            /* Is_Open_Drain    */      false                          ,\
                            /* Drive_Strength   */      DEFAULT                        )\
/*  Bottle detection switch. Active when the bottle is attached                       */\
X(  GPIO_BOTTLE_SW,         /* GPIO_Number6 19     */   37                             ,\
                            /* Direction        */      INPUT                          ,\
                            /* Active_Level  0   */     0                              ,\
                            /* Pull_Mode        */      FLOATING                       ,\
                            /* Is_Open_Drain    */      false                          ,\
                            /* Drive_Strength   */      DEFAULT                        )\
                                                                                        \
/*  External power detector. Active when the external power adaptor is present        */\
X(  GPIO_EXT_PLUG,         /* GPIO_Number2 36      */   36                             ,\
                            /* Direction        */      INPUT                          ,\
                            /* Active_Level     */      0                              ,\
                            /* Pull_Mode        */      FLOATING                       ,\
                            /* Is_Open_Drain    */      false                          ,\
                            /* Drive_Strength   */      DEFAULT                        )\
                                                                                        \
/*  LED_EN pin to enable LED driver. Active high */                                     \
X(  GPIO_BAT_EN,            /* GPIO_Number2 36      */  26                             ,\
                            /* Direction        */      OUTPUT                         ,\
                            /* Active_Level     */      1                              ,\
                            /* Pull_Mode        */      FLOATING                       ,\
                            /* Is_Open_Drain    */      false                          ,\
                            /* Drive_Strength   */      DEFAULT                        )\                               
 

/*------------------------------------------------------------------------------------*/

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


#endif /* __HWA_GPIO_EXT_H__ */

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
