/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : app_warmer_mngr.h
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 Sep 21
**  @brief      : Public header of App_Warmer_Mngr module
**  @namespace  : WARMER
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  App_Warmer_Mngr
** @{
*/

#ifndef __APP_WARMERING_MNGR_H__
#define __APP_WARMERING_MNGR_H__

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           INCLUDES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include "common_hdr.h"         /* Use common definitions */

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           DEFINES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Status returned by APIs of App_Warmer_Mngr module */
typedef enum
{
    WARMER_OK         = 0,                  //!< The function executed successfully
    WARMER_ERR        = -1,                 //!< There is unknown error while executing the function
} WARMER_status_t;

/** @brief  States of the warmer */
typedef enum
{
    WARMER_STATE_AWAKING,                   //!< The warmer is awaking after a deep sleep
    WARMER_STATE_IDLE,                      //!< The warmer is idling
    WARMER_STATE_HEATING,                   //!< The warmer is heating water to the set temperature
    WARMER_STATE_MAINTAINING,               //!< Temperature is being maintained around set point
    WARMER_STATE_EXTENDED_MAINTAINING,      //!< Temperature maintainance been extended
    WARMER_STATE_SHUTTING_DOWN,             //!< The warmer is shutting down before deep sleep

} WARMER_state_t;

/** @brief  States of the warmer */
typedef enum
{
    WARMER_READY,                  
    WARMER_HEATING,                      
    WARMER_WARMING,                   
    WARMER_WARMING_DONE,               
    WARMER_KEEPING,      
    WARMER_KEEPING_END,  
} WARMER_running_state_t;


/** @brief  Fluid volume levels */
typedef enum
{
    WARMER_VOLUME_120,                      //!< 120 ml
    WARMER_VOLUME_240,                      //!< 240 ml
} WARMER_volume_t;

typedef enum
{
    BTN_EVENT_IDLE = 0,             // No event when all buttons are released
    BTN_EVENT_BACK,                    // Common: any button that triggered HOLD_START and released before HOLD_SHOT will go back to last screen
    
    
    // --- ON button events ---
    BTN_EVENT_ON_PRESSED,             // Pressed and released before HOLD_START
    BTN_EVENT_ON_HOLD_START,           // Common: button held long 500ms to show countdown ui (3..2..1)
    BTN_EVENT_ON_HOLD_SHORT_PRESSED,   // Hold short triggered
    BTN_EVENT_ON_HOLD_LONG_PRESSED,    // Hold long triggered
    BTN_EVENT_ON_HOLD_SHORT_RELEASED,  // Hold short and released
    BTN_EVENT_ON_HOLD_LONG_RELEASED,   // Hold long and released
    BTN_EVENT_ON_RELEASED,             // Pressed and released before HOLD_START
    
    // --- UP button events ---
    BTN_EVENT_UP_PRESSED,
    BTN_EVENT_UP_HOLD_START,
    BTN_EVENT_UP_HOLD_SHORT_PRESSED,   
    BTN_EVENT_UP_HOLD_LONG_PRESSED,    
    BTN_EVENT_UP_HOLD_SHORT_RELEASED,
    BTN_EVENT_UP_HOLD_LONG_RELEASED,
    BTN_EVENT_UP_RELEASED,
    
    // --- TH button events ---
    BTN_EVENT_TH_PRESSED,
    BTN_EVENT_TH_HOLD_START,
    BTN_EVENT_TH_HOLD_SHORT_PRESSED,   
    BTN_EVENT_TH_HOLD_LONG_PRESSED,    
    BTN_EVENT_TH_HOLD_SHORT_RELEASED,
    BTN_EVENT_TH_HOLD_LONG_RELEASED,
    BTN_EVENT_TH_RELEASED,
    
} WARMER_btn_event_t;

// typedef enum
// {
//     BUZZER_VOLUME_LEVEL_1 = 1,   //!< Volume level 1
//     BUZZER_VOLUME_LEVEL_2 = 2,   //!< Volume level 2
//     BUZZER_VOLUME_LEVEL_3 = 3,  //!< Volume level 3
//     BUZZER_VOLUME_LEVEL_4 = 5,  //!< Volume level 4
//     BUZZER_VOLUME_LEVEL_5 = 50   //!< Volume level 5
// } BUZZER_volume_t;

/** @brief  Structure encapsulating device settings */
typedef struct
{
    uint8_t             u8_temp_setpoint;       //!< Temperature setting in Celsius degrees
    temp_unit_t         enm_temp_unit;          //!< Temperature unitbuf
    uint8_t             u8_heater_mode;         //!< Heater mode
    bool                b_wigg_mode;            //!< Specifies whether WIGG mode is enabled
    bool                b_buzzer_enabled;       //!< Specifies whether buzzer is enabled
    bool                b_vibration_enabled;    //!< Specifies whether vibration is enabled
    BUZZER_volume_t	    enm_buzzer_volume;      //!< Buzzer volume level
    WARMER_volume_t     enm_volume_level;       //!< Fluid volume in ml
    DEVICE_language_t   enm_device_language;    //!< Device language
    uint8_t             u8_Keep_warm_level;     //!< specifies device keep warm level
    uint8_t             u8_Keep_warm_period;    //!< specifies whether keep warm period is enabled
    uint8_t             u8_heater_pwr_lvl;
    bool                b_heater_failure;
    bool                b_low_bat_alert;
    bool                b_high_bat_alert;
} WARMER_config_t;

/** @brief  Minimum and maximum temperature (in Celsius degrees) of temperature set point */
#define TEMPERATURE_SETTING_MIN             35
#define TEMPERATURE_SETTING_MAX             50
#define TEMPERATURE_LOW_E4                 -15 // Error temperature low 
#define TEMPERATURE_HIGH_E5                 75 // Error temperature high

/** @brief  Fixed set point in Celsius degrees for frozen mode */
#define WARMER_FROZEN_SET_POINT_TEMP        40


/** @brief keep warmer level maximum */
#define WARMER_KEEP_WARMER_LEVEL_MAX       0x05

#define WARMER_CUTOFF_CHARGING_A                    270                 //mA
#define WARMER_BAT_FULL_MSG_A                       700                 //mA
#define WARMER_REENABLE_CHARGING_V                  15500               //mV 15500
#define WARMER_REENABLE_CHARGING_T                  (24*60*60*1000)     //ms (24hrs)
#define WARMER_PLUG_IN_BAT_PERCENT_UPDATE_T         (20*1000)         //(3*60*1000)         //ms 15000
#define WARMER_CHARGING_UI_T                        (2*60*1000)         //ms 10000
/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           PROTOTYPES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/* Initializes App_Warmer_Mngr module */
extern WARMER_status_t enm_WARMER_Init (void);

/* Gets current state of the warmer */
extern WARMER_state_t enm_WARMER_Get_State (void);

/* Puts the warmer into deep sleep and disable all pheripherals. Note that this function never returns */
extern void v_WARMER_Shut_Down (void);

/* Puts the warmer into deep sleep when cover is not detected on wakeup. Note that this function never returns */
extern void v_WARMER_Sleep (void);

/* Prevents the warmer to be put into deep sleep */
extern void v_WARMER_Keep_Awaken (void);

/* Gets device configuration */
extern WARMER_config_t * pstru_WARMER_Get_Config (void);

/* Stores device configuration onto non-volatite flash */
extern WARMER_status_t enm_WARMER_Save_Config (void);

/* Get Warmer Running State */
extern WARMER_running_state_t enm_WARMER_Get_Running_State (void);

/* Low Voltage indicator*/
extern void Low_Voltage_Indicator(void);

extern void v_WARMER_INC_TEMP(void);
extern void v_WARMER_DEC_TEMP(void);
extern void v_WARMER_Get_Error(void);

/* Get Warmer Button Event */
extern WARMER_btn_event_t enm_WARMER_Get_Button_Event (void);

/* Enable Sleep Cut Pin*/
void Sleep_Cut_Enable(void);

/* Disable Sleep Cut Pin*/
void Sleep_Cut_Disable(void);

uint8_t u8_convert_ntc_To_Precent(float current_temp, uint8_t setting_temp);

void auto_control_update(float rpm);




#endif /* __APP_WARMERING_MNGR_H__ */

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
