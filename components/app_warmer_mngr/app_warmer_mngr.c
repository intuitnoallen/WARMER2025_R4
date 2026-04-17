/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : app_warmer_mngr.c
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 Sep 21
**  @brief      : Implementation of App_Warmer_Mngr module
**  @namespace  : WARMER
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  App_Warmer_Mngr
** @brief       Central processing of the warmer
** @{
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           INCLUDES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include "app_warmer_mngr.h"            /* Public header of this module */
#include "srvc_button.h"                /* Process UP and ON buttons */
#include "srvc_thermistor.h"            /* Get temperature and status from temperature sensor */
#include "srvc_ps_mon.h"                /* Get battery information */
#include "srvc_param.h"                 /* Use parameter service */
#include "hwa_gpio.h"                   /* Get state of tilt sensor and battery charging */
#include "driver/rtc_io.h"
// #include "driver/adc.h"                 /* Use ESP-IDF's ADC driver */
// #include "esp_adc_cal.h"    

#include "app_cfg.h"                    /* Use the global shared data 'device_data' and related definitions */
#include "app_notification.h"           /* Controls Notifications */
#include "app_heater.h"                 /* Control heater */
#include "encoder.h"
#include "app_motor.h"                  /* Control motor */
#include "app_ble.h"                    /* Use BLE */
#include "app_pm.h"                     /* Use power management */
#include "ui_common.h"                         /* Use UI */

#include "freertos/FreeRTOS.h"          /* Use FreeRTOS */
#include "freertos/task.h"              /* Use FreeRTOS task */
#include "freertos/event_groups.h"      /* Use FreeRTOS event group */

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           DEFINES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Logging tag of this module */
#define TAG                                     "App_Warmer_Mngr"

/** @brief  ID of the CPU that App_Warmer_Mngr task runs on */
#define WARMER_TASK_CPU_ID                      0

/** @brief  Stack size (in bytes) of App_Warmer_Mngr task */
#define WARMER_TASK_STACK_SIZE                  (3*1024)        

/** @brief  Priority of App_Warmer_Mngr task */
#define WARMER_TASK_PRIORITY                    (tskIDLE_PRIORITY + 1)

/** @brief  Cycle in milliseconds of App_Warmer_Mngr task */
#define WARMER_TASK_PERIOD_MS                   10

/** @brief  Interval in milliseconds to shutdown the warmer if it's idle for too long */
#define WARMER_IDLE_BEFORE_SLEEP_MS             (2 * 60 * 1000)

/** @brief  Interval in milliseconds to shutdown the warmer if it's idle for too long */
#define WARMER_IDLE_IMMEDIATE_SLEEP_MS          (5 *1000)


/** @brief  Period in milliseconds sending device measurements and statuses over Bluetooth */
#define WARMER_BLE_PERIOD                       500

/** @brief  Interval (milliseconds) heatind and warming  */
#define WARMER_HEATING_MILK                     (9 * 1000)//(3 * 1000)
#define WARMER_WARMING_MILK                     (1 * 1000)//(2 * 1000)

#define WARMER_HEATING_WATER                    (9 * 1000)//(8 * 1000)
#define WARMER_WARMING_WATER                    (1 * 1000)//(0 * 1000)

#define WARMER_HEATING_BABYFOOD                 (5 * 1000)
#define WARMER_WARMING_BABYFOOD                 (0 * 1000)

#define WARMER_HEATING_FROZEN                   (8 * 1000)
#define WARMER_WARMING_FROZEN                   (2 * 1000)

#define WARMER_HEATING_FAILURE                  (30 * 1000)
#define WARMER_TEMP_READ_FAILURE                (20 * 60 * 1000)
#define WARMER_INTERVAL_CHECK                   (5 * 1000) 

/** @brief  Interval (milliseconds) maintaining temperature after reaching set point for different fluids and volumes */
#define WARMER_MAINTAIN_MILK_120                (2 * 1000) //(60 * 1000)
#define WARMER_MAINTAIN_MILK_240                (80 * 1000)//(150 * 1000)
#define WARMER_MAINTAIN_WATER_120               (2 * 1000) //(60 * 1000)
#define WARMER_MAINTAIN_WATER_240               (80 * 1000)//(150 * 1000)
#define WARMER_MAINTAIN_FROZEN_120              (300 * 1000)
#define WARMER_MAINTAIN_FROZEN_240              (300 * 1000)
#define WARMER_MAINTAIN_BABY_FOOD               (300 * 1000)

/** @brief  Heater power (percents) in different use cases */
#define WARMER_HEATER_OFF                       0
#define WARMER_MOTOR_OFF                        0
#define WARMER_MOTOR_INITIAL_PWM                230
// #define WARMER_MOTOR_FINAL_PWM               25

#define WARMER_HEATER_MILK                      80
#define WARMER_HEATER_WATER                     80
#define WARMER_HEATER_FROZEN                    80

#define WARMER_STARTING_DUTY_CYCLE              80
#define WARMER_LOW_VOLTAGE_DUTY_CYCLE           85//95
#define WARMER_MEDIUM_VOLTAGE_DUTY_CYCLE        100//90
#define WARMER_HIGH_VOLTAGE_DUTY_CYCLE          100//85
#define WARMER_BABY_FOOD_DUTY_CYCLE				85

/** @brief  Battery voltage (in millivolts) that heating is not allowed */
#define WARMER_HEATER_CUTOFF_VOLTAGE            11700
#define WARMER_HEATER_NORMAL_VOLTAGE            13500

// #define DEVELOPMENT_TEST_MODE
// #define SIMULATED_INPUT_MODE


// #define TEST_MODE
// #ifdef TEST_MODE

// #define WARMER_HEATER_OFF                       0
// #define WARMER_MOTOR_OFF                        0
// #define WARMER_MOTOR_INITIAL_PWM                128
// // #define WARMER_MOTOR_FINAL_PWM               10

// #define WARMER_HEATER_MILK                      15
// #define WARMER_HEATER_WATER                     15
// #define WARMER_HEATER_FROZEN                    15

// #define WARMER_STARTING_DUTY_CYCLE              15
// #define WARMER_LOW_VOLTAGE_DUTY_CYCLE           5
// #define WARMER_MEDIUM_VOLTAGE_DUTY_CYCLE        15
// #define WARMER_HIGH_VOLTAGE_DUTY_CYCLE          15
// #define WARMER_BABY_FOOD_DUTY_CYCLE				15

// // #define WARMER_IDLE_BEFORE_SLEEP_MS             (20 * 60 * 1000)
// // /** @brief  Battery voltage (in millivolts) that heating is not allowed */
// // #define WARMER_HEATER_CUTOFF_VOLTAGE            7000
// // #define WARMER_HEATER_NORMAL_VOLTAGE            9500
// // #define WARMER_HEATER_CUTOFF_VOLTAGE            200
// // #define WARMER_HEATER_NORMAL_VOLTAGE            300
// #endif


/**
** @brief   Battery is full if its percent is >= WARMER_BAT_FULL_THRESHOLD (percents)
**          for an interval of WARMER_BAT_FULL_INTERVAL (milliseconds)
*/
#define WARMER_BAT_FULL_THRESHOLD               99
#define WARMER_BAT_FULL_INTERVAL                (10 * 1000)

/** @brief  Cycle (in milliseconds) of the guardian timer */
#define WARMER_GUARDIAN_CYCLE                   20


#ifdef DEVELOPMENT_TEST_MODE
#warning "DEVELOPMENT_TEST_MODE is ENABLED , disable before production!"
#endif

#ifdef SIMULATED_INPUT_MODE
#warning "SIMULATED_INPUT_MODE is ENABLED , disable before production!"
#endif

/** @brief  FreeRTOS events sent to the module */
enum
{
     WARMER_BOTTLE_EVT           = BIT0,         //!< Event fired when the bottle is attached/detached
};

/** @brief  Structure wrapping state machine of the warmer */
typedef struct
{
    WARMER_state_t      enm_curr_state;         //!< Current state
    WARMER_state_t      enm_prev_state;         //!< Previous state
    uint32_t            u32_state_counter;      //!< Counter counting number of time a state has been running
} WARMER_sm_t;


/// @brief Structure wrapping warming percent and starting temperature
typedef struct
{
    float start_temperature;
    uint8_t warming_curr_percent;
    uint8_t warming_prev_percent;
    uint32_t u32_maintaining_ms;
    bool b_start_maintaining_percent;
} WARMER_percent_t;

/** @brief  Macro switch the state machine to another state */
#define WARMER_CHANGE_STATE(enm_new_state)                  \
{                                                           \
    g_stru_sm.enm_prev_state = g_stru_sm.enm_curr_state;    \
    g_stru_sm.enm_curr_state = enm_new_state;               \
    g_stru_sm.u32_state_counter = 0xFFFFFFFF;               \
}

/** @brief  Structure wrapping device inputs and sensor measurements */
typedef struct
{
    bool            b_tilted;           //!< Whether the warm tilts
    bool            b_bottle_attached;  //!< Whether bottle is attached to the warmer
    bool            b_ntc_low_err;      //!< Whether the NTC sensor is in error state (low temperature)
    bool            b_ntc_high_err;     //!< Whether the NTC sensor is in error state (high temperature)
    bool            b_wigg_seal_err;    //!< Whether the Wigg is correct or not

    float           flt_temperature;    //!< Temperature of the warmer in Celsius degree
    float           motor_rpm;          //!< Motor RPM
    uint16_t        u16_bat_voltage;    //!< Voltage of the warmer's battery in mV
    uint16_t        u16_plug_voltage;   //!< Voltage of the warmer's external power plug in mV
    uint16_t        u16_plug_current;   //!< Current of the warmer's external power plug in mAmp
    uint8_t         u8_bat_percent;     //!< Percentage of the warmer's battery in %
    PSMON_alert_t   enm_bat_status;     //!< Battery status
    EPMON_alert_t   enm_plug_status;    //!< External power plug status
    bool            b_bat_enable;       //!< Whether the warmer's battery is enabled
    bool            b_plug;             //!< Whether the warmer is plugged to external power
    bool            b_charging;         //!< Whether the warmer's battery is being charged
} WARMER_dev_t;



/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           VARIABLES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Structure that will hold the TCB of the task being created */
static StaticTask_t g_x_task_buffer;

/** @brief  Buffer that the task being created will use as its stack */
static StackType_t g_x_task_stack [WARMER_TASK_STACK_SIZE];

// static TaskHandle_t xWarmerMngrHandle = NULL;

/** @brief  Indicates if this module has been initialized */
static bool g_b_initialized = false;

/** @brief  Warmer's sensor inputs and measurements */
static WARMER_dev_t g_stru_dev;

/** @brief  Warmer's state */
static WARMER_sm_t g_stru_sm;

/// @brief Warming percent
static WARMER_percent_t g_stru_wp;

/** @brief  FreeRTOS event group */
static EventGroupHandle_t g_x_event_group;

/** @brief  Instance handle of UP button */
static BTN_inst_t g_x_btn_up_inst;

/** @brief  Instance handle of ON button */
static BTN_inst_t g_x_btn_on_inst;

/** @brief  Instance handle of TH button */
static BTN_inst_t g_x_btn_th_inst;

/** @brief  Instance handle of the GPIO pin detecting if the bottle is attached to the Warmer */
static GPIO_inst_t g_x_bottle_inst;

/** @brief  Instance handle of SLEEP CUT EN */
static GPIO_inst_t g_x_sleep_cut_aux_inst;

/** @brief  Instance handle of BAT_EN */
static GPIO_inst_t x_bat_en_inst;

/** @brief  Timer measuring idle time of the warmer */
static TickType_t g_x_idle_timer = 0;

static TickType_t g_x_charging_ui_timer=0;

static TickType_t x_stable_timer = 0; //for heating

// static TickType_t x_warmer_start_timer = 0; //time since boot

// static TickType_t x_battery_100_timer = 0;

/** @brief  Device configuration */
static WARMER_config_t g_stru_config;

/** @brief  Currently active button instance */
static WARMER_btn_event_t g_enm_button_event = BTN_EVENT_IDLE;

/* Track currently active button */
static BTN_inst_t *p_x_active_btn = NULL;

/** @brief  Currently active error: NONE */
static uint8_t error=0; //NONE

static bool is_ready_for_charging=true;
static bool show_bat_full_msg=true;

/** @brief  Indicates if button is ready for gpio read */
// static bool b_btn_ready=true;

static bool update_plugged_in_bat_percent=true;

/** @brief  Indicates if error update is required */
bool isErrorUpdateRequired = false;

/** @brief  Indicates if device is stable after boot */
bool device_stable = false;

bool back_to_prev_scr=false;

/** @brief  Indicates if device boot up from sleep first time */
bool isBoot=true;

/** @brief  Indicates if device is currently measuring temperature */
bool isMeasuringTemp=false;

/** @brief  True when motor rpm is stable at set value */
bool startHeating=false;


bool isMotor_PID_En = false;
// static bool g_low_voltage_alert = false;

static uint8_t temp_reach_counter=0;
static uint8_t checkTimes     = 5;	 //Times
float average_measured_temp = 0.0f;


// If oscillation → reduce KI first, then KP.
#define KP                   0.45f       // proportional gain, If response too slow → increase KP by +0.02.
#define KI                   0.00000f      // integral gain (optional, small) If steady error stays → increase KI by +0.0005.
#define KD                   0.001f           //derivative gain
#define PWM_RAMP_LIMIT       150.0f       // max PWM change per 100 ms step
#define PWM_SMOOTH_ALPHA     0.1f       // smoothing blend (0-1)  // 0.45f
#define PWM_MIN              25.0f       //19.6% =50   //15%=  38.4  // 10% = 25.6      //7%=17.92pwm
#define PWM_MAX              240.0f
#define TARGET_RPM           600.0f//600.0f
// #define RPM_TOLERANCE        1.0f//30.0f
#define DT                   0.1f       //for 100ms loop


static float current_pwm = WARMER_MOTOR_INITIAL_PWM;
static float integral = 0.0f;
static float prev_error = 0.0f;

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           PROTOTYPES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

static void v_WARMER_Main_Task (void * pv_param);
static void v_WARMER_Update_Inputs (void);
static void v_WARMER_Run_State_Machine (void);
static void v_WARMER_Process_Device (void);
static void v_WARMER_Handle_Errors(void);
static void v_WARMER_Control_Heater (uint8_t u8_power);
static void v_WARMER_Control_Motor (uint8_t u8_power);
static void v_WARMER_Control_Battery(bool b_enable);
static void v_WARNER_Button_Event_Handler (BTN_evt_data_t * pstru_evt_data);
static void v_WARMER_Bottle_Detach_Isr (GPIO_evt_data_t * pstru_evt_data);
static void v_WARMER_Guardian_Timer_Cb (TimerHandle_t x_timer);
static void v_WARMER_Print_Info (void);


/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           FUNCTIONS SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Initializes App_Warmer_Mngr module
**
** @return
**      @arg    WARMER_OK
**      @arg    WARMER_ERR
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
WARMER_status_t enm_WARMER_Init (void)
{
    /* Do nothing if this module has been initialized */
    if (g_b_initialized)
    {
        return WARMER_OK;
    }

    /* Load device configuration */
    uint8_t u8_value;
    s8_PARAM_Get_Uint8 (PARAM_TEMP_SET_POINT, &u8_value);
    g_stru_config.u8_temp_setpoint = (g_stru_config.u8_heater_mode == HEATER_M_FROZEN) ?
                                     WARMER_FROZEN_SET_POINT_TEMP : u8_value;

    s8_PARAM_Get_Uint8 (PARAM_TEMP_UNIT, &u8_value);
    g_stru_config.enm_temp_unit = u8_value;

    s8_PARAM_Get_Uint8 (PARAM_HEATER_MODE, &u8_value);
    g_stru_config.u8_heater_mode = u8_value;

    s8_PARAM_Get_Uint8 (PARAM_WIGG_MODE_ENABLED, &u8_value);
    g_stru_config.b_wigg_mode = u8_value;

    s8_PARAM_Get_Uint8 (PARAM_BUZZER_ENABLED, &u8_value);
    g_stru_config.b_buzzer_enabled = u8_value;

    s8_PARAM_Get_Uint8 (PARAM_VIBRATION_ENABLED, &u8_value);
    g_stru_config.b_vibration_enabled = u8_value;

    s8_PARAM_Get_Uint8 (PARAM_VOLUME_LEVEL, &u8_value);
    g_stru_config.enm_volume_level = u8_value;

    s8_PARAM_Get_Uint8 (PARAM_EXTENDED_WARMER_LEVEL, &u8_value);
    g_stru_config.u8_Keep_warm_level = u8_value;

    s8_PARAM_Get_Uint8 (PARAM_BUZZER_VOLUME, &u8_value);
    g_stru_config.enm_buzzer_volume = u8_value;

    s8_PARAM_Get_Uint8 (PARAM_DEVICE_LANGUAGE, &u8_value);
    g_stru_config.enm_device_language = u8_value;

    s8_PARAM_Get_Uint8 (PARAM_LOW_BAT_ALERT, &u8_value);
    g_stru_config.b_low_bat_alert = u8_value;

    s8_PARAM_Get_Uint8 (PARAM_HIGH_BAT_ALERT, &u8_value);
    g_stru_config.b_high_bat_alert = u8_value;

    g_stru_config.b_heater_failure = false;

    /* Initialize state machine of the warmer */
    g_stru_sm.enm_curr_state = WARMER_STATE_AWAKING;
    g_stru_sm.enm_prev_state = WARMER_STATE_SHUTTING_DOWN;
    g_stru_sm.u32_state_counter = 0;

    g_stru_wp.start_temperature = -16.0f;
    g_stru_wp.warming_curr_percent = 0;
    g_stru_wp.warming_prev_percent = 0;
    g_stru_wp.u32_maintaining_ms = 0;
    g_stru_wp.b_start_maintaining_percent = false;

    /* Disable VBAT normally */
    v_WARMER_Control_Battery(false);

    /* Get instance handle of device's buttons */
    g_x_btn_up_inst = x_BTN_Get_Inst (BTN_BUTTON_UP);
    g_x_btn_on_inst = x_BTN_Get_Inst (BTN_BUTTON_ON);
    g_x_btn_th_inst = x_BTN_Get_Inst (BTN_BUTTON_TH);
    if ((g_x_btn_up_inst == NULL) || (g_x_btn_on_inst == NULL) || (g_x_btn_th_inst == NULL))
    {
        LOGE ("Failed to get instance of device's buttons");
        return WARMER_ERR;
    }
    else
    {
        /* Register handlers processing events of buttons UP and ON */
        v_BTN_Register_Callback (g_x_btn_up_inst, v_WARNER_Button_Event_Handler, NULL);
        v_BTN_Register_Callback (g_x_btn_on_inst, v_WARNER_Button_Event_Handler, NULL);
        v_BTN_Register_Callback (g_x_btn_th_inst, v_WARNER_Button_Event_Handler, NULL);
    }

    /* Register an interrupt handle that will be invoked when the bottle is detached from the warmer */
    if (s8_GPIO_Get_Inst (GPIO_BOTTLE_SW, &g_x_bottle_inst) != GPIO_OK)
    {
        LOGE ("Failed to get instance handle of bottle detection pin");
        return WARMER_ERR;
    }
    else
    {
        s8_GPIO_Enable_Interrupt (g_x_bottle_inst, GPIO_INT_BOTH_EDGE, v_WARMER_Bottle_Detach_Isr, NULL);
        // printf ("Cover Detector Interrupt\n");
    }

    encoder_init(13,15,32);

    // /* Get instance for LED_EN */
    // if (s8_GPIO_Get_Inst (GPIO_LED_EN, &g_x_led_en_inst) != GPIO_OK)
    // {
    //     LOGE ("Failed to get instance handle of LED EN pin");
    //     return WARMER_ERR;
    // }
    // else
    // {
    //  	s8_GPIO_Write_Level(g_x_led_en_inst, 1);  
    // }
    
  
    /* Create FreeRTOS event group */
    g_x_event_group = xEventGroupCreate ();
    ASSERT_PARAM (g_x_event_group != NULL);

    /* Create task running this module */
    xTaskCreateStaticPinnedToCore ( v_WARMER_Main_Task,         /* Function that implements the task */
                                    "App_Warmer_Mngr",          /* Text name for the task */
                                    WARMER_TASK_STACK_SIZE,     /* Stack size in bytes, not words */
                                    NULL,                       /* Parameter passed into the task */
                                    WARMER_TASK_PRIORITY,       /* Priority at which the task is created */
                                    g_x_task_stack,             /* Array to use as the task's stack */
                                    &g_x_task_buffer,           /* Variable to hold the task's data structure */
                                    WARMER_TASK_CPU_ID);        /* ID of the CPU that the task runs on */

    /* Create and start the timer protecting the warmer from unexpected working conditions */
    TimerHandle_t x_guardian_timer =
        xTimerCreate (NULL,                                     /* Human readable text name */
                      pdMS_TO_TICKS (WARMER_GUARDIAN_CYCLE),    /* Timer period */
                      pdTRUE,                                   /* Auto reload */
                      NULL,                                     /* Timer ID */
                      v_WARMER_Guardian_Timer_Cb);              /* Callback function when the timer expires */
    ASSERT_PARAM (x_guardian_timer != NULL);
    ASSERT_PARAM (xTimerStart (x_guardian_timer, portMAX_DELAY) == pdPASS);

    /* Done */
    g_b_initialized = true;
    return WARMER_OK;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets current state of the warmer
**
** @return
**      Current state of the warmer
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
WARMER_state_t enm_WARMER_Get_State (void)
{
    ASSERT_PARAM (g_b_initialized);

    /* Return current state of the warmer */
    return g_stru_sm.enm_curr_state;
}


void v_WARMER_Sleep (void){
    app_pm_deep_sleep();
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Puts the warmer into deep sleep
**
** @note
**      This function never returns
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
void v_WARMER_Shut_Down (void)
{
    ASSERT_PARAM (g_b_initialized);

    /* Prepare for deep sleep */
    device_data.power = PWR_OFF;
    device_data.stateTemp = TEMP_NOT_REACH;
    
    v_WARMER_Control_Heater (WARMER_HEATER_OFF);
    v_WARMER_Control_Motor  (WARMER_MOTOR_OFF);
    v_WARMER_Control_Battery(false);

    
    app_heater_off ();
    app_ble_stop ();
	
	
    vTaskDelay (pdMS_TO_TICKS (800));
    app_notification_off();
    vTaskDelay (pdMS_TO_TICKS (50));
    APP_DEBUG(TAG, "Diable LCD\n");
    deinit_LCD();
    vTaskDelay (pdMS_TO_TICKS (50));
    Sleep_Cut_Disable();
    vTaskDelay (pdMS_TO_TICKS (50));
    app_pm_deep_sleep();
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Prevents the warmer to be put into deep sleep
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
void v_WARMER_Keep_Awaken (void)
{
    TIMER_RESET (g_x_idle_timer);
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets device configuration
**
** @return
**      Pointer to the buffer storing device configuration
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
WARMER_config_t * pstru_WARMER_Get_Config (void)
{
    ASSERT_PARAM (g_b_initialized);
    return &g_stru_config;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Stores device configuration onto non-volatite flash
**
** @return
**      @arg    WARMER_OK
**      @arg    WARMER_ERR
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
WARMER_status_t enm_WARMER_Save_Config (void)
{
    WARMER_status_t enm_result = WARMER_OK;
    uint8_t         u8_value;

    u8_value = g_stru_config.u8_temp_setpoint;
    if (s8_PARAM_Set_Uint8 (PARAM_TEMP_SET_POINT, u8_value) != PARAM_OK)
    {
        LOGE ("Failed to write PARAM_TEMP_SET_POINT parameter");
        enm_result = WARMER_ERR;
    }

    u8_value = g_stru_config.enm_temp_unit;
    if (s8_PARAM_Set_Uint8 (PARAM_TEMP_UNIT, u8_value) != PARAM_OK)
    {
        LOGE ("Failed to write PARAM_TEMP_UNIT parameter");
        enm_result = WARMER_ERR;
    }

    u8_value = g_stru_config.u8_heater_mode;
    if (s8_PARAM_Set_Uint8 (PARAM_HEATER_MODE, u8_value) != PARAM_OK)
    {
        LOGE ("Failed to write PARAM_HEATER_MODE parameter");
        enm_result = WARMER_ERR;
    }

    u8_value = g_stru_config.b_wigg_mode;
    if (s8_PARAM_Set_Uint8 (PARAM_WIGG_MODE_ENABLED, u8_value) != PARAM_OK)
    {
        LOGE ("Failed to write PARAM_WIGG_MODE_ENABLED parameter");
        enm_result = WARMER_ERR;
    }

    u8_value = g_stru_config.b_buzzer_enabled;
    if (s8_PARAM_Set_Uint8 (PARAM_BUZZER_ENABLED, u8_value) != PARAM_OK)
    {
        LOGE ("Failed to write PARAM_BUZZER_ENABLED parameter");
        enm_result = WARMER_ERR;
    }

    u8_value = g_stru_config.b_vibration_enabled;
    if (s8_PARAM_Set_Uint8 (PARAM_VIBRATION_ENABLED, u8_value) != PARAM_OK)
    {
        LOGE ("Failed to write PARAM_VIBRATION_ENABLED parameter");
        enm_result = WARMER_ERR;
    }

    u8_value = g_stru_config.enm_volume_level;
    if (s8_PARAM_Set_Uint8 (PARAM_VOLUME_LEVEL, u8_value) != PARAM_OK)
    {
        LOGE ("Failed to write PARAM_VOLUME_LEVELs parameter");
        enm_result = WARMER_ERR;
    }

    u8_value = g_stru_config.u8_Keep_warm_level;
    if (s8_PARAM_Set_Uint8 (PARAM_EXTENDED_WARMER_LEVEL, u8_value) != PARAM_OK)
    {
        LOGE ("Failed to write PARAM_EXTENDED_WARMER_TIME parameter");
        enm_result = WARMER_ERR;
    }

    u8_value = g_stru_config.enm_buzzer_volume;
	if (s8_PARAM_Set_Uint8 (PARAM_BUZZER_VOLUME, u8_value) != PARAM_OK)
	{
		LOGE ("Failed to write PARAM_BUZZER_VOLUME parameter");
		enm_result = WARMER_ERR;
	}

    u8_value = g_stru_config.enm_device_language;
	if (s8_PARAM_Set_Uint8 (PARAM_DEVICE_LANGUAGE, u8_value) != PARAM_OK)
	{
		LOGE ("Failed to write PARAM_DEVICE_LANGUAGE parameter");
		enm_result = WARMER_ERR;
	}

    u8_value = g_stru_config.b_low_bat_alert;
    if (s8_PARAM_Set_Uint8 (PARAM_LOW_BAT_ALERT, u8_value) != PARAM_OK)
	{
		LOGE ("Failed to write PARAM_LOW_BAT_ALERT parameter");
		enm_result = WARMER_ERR;
	}

    u8_value = g_stru_config.b_high_bat_alert;
    if (s8_PARAM_Set_Uint8 (PARAM_HIGH_BAT_ALERT, u8_value) != PARAM_OK)
	{
		LOGE ("Failed to write PARAM_HIGH_BAT_ALERT parameter");
		enm_result = WARMER_ERR;
	}

    return enm_result;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Task running App_Warmer_Mngr module
**
** @param [in]
**      pv_param: Parameter passed into the task
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void v_WARMER_Main_Task (void * pv_param)
{
    EventBits_t x_fired_events;
    EventBits_t x_wait_events = WARMER_BOTTLE_EVT;
    TickType_t  x_ble_timer = 0;
    TickType_t  x_log_timer = 0;
    
// LOGI("Wake stub left state = %d", stub_state);

    Sleep_Cut_Disable();
    vTaskDelay (pdMS_TO_TICKS (50));
    app_notify_200ms();
    vTaskDelay (pdMS_TO_TICKS (250));
    Sleep_Cut_Enable();
    // vTaskDelay (pdMS_TO_TICKS (1500));
    /* Endless loop operating warmer */
    while (true)
    {
        /* Waiting for task tick or a FreeRTOS event occurs */
        x_fired_events = xEventGroupWaitBits (g_x_event_group,   /* The event group to test the bits */
                                              x_wait_events,     /* Events to wait for */
                                              pdTRUE,            /* Whether the tested bits are cleared on exit */
                                              pdFALSE,           /* Whether to wait for all test bits to be set */
                                              pdMS_TO_TICKS (WARMER_TASK_PERIOD_MS));

        /* The bottle is attached or detached front the warmer */
        if (x_fired_events & WARMER_BOTTLE_EVT)
        {
            bool b_bottle_attached = false;
            s8_GPIO_Read_Active (g_x_bottle_inst, &b_bottle_attached);
            if (b_bottle_attached)
            {
                LOGI ("The bottle is attached to the warmer");
            }
            else
            {
                LOGW ("The bottle is detached from the warmer");
            }
            
        }
        esp_task_wdt_reset();
        vTaskDelay(pdMS_TO_TICKS(10));

        /* Update sensor inputs and measurements */
        v_WARMER_Update_Inputs ();

        /* Handle errors */
        v_WARMER_Handle_Errors();

        /* Run state machine of the warmer */
        v_WARMER_Run_State_Machine ();


        /* Common process of Warmer device */
        v_WARMER_Process_Device ();

        /* Update data for BLE */
        if (TIMER_ELAPSED (x_ble_timer) >= pdMS_TO_TICKS (WARMER_BLE_PERIOD))
        {
            TIMER_RESET (x_ble_timer);
            app_ble_update_data ();
        }

        /* Display log for debugging purpose */
        if (TIMER_ELAPSED (x_log_timer) >= pdMS_TO_TICKS (3000))
        {
            TIMER_RESET (x_log_timer);
            v_WARMER_Print_Info ();
        }

     

        /* Display remaining stack space every 30s */
        // PRINT_STACK_USAGE (3000); //30000
    }
}

// uint8_t get_stable_battery_percent(uint8_t real_percent, bool plug_detected)
// {
//     const uint8_t HYST = 1;                // hysteresis threshold 
//     static uint8_t stable_percent = 0;     // displayed value
//     static bool prev_plug_state = false;
//     static bool initialized = false;
//     // First call initialization
//     if (!initialized)
//     {
//         stable_percent = real_percent;
//         prev_plug_state = plug_detected;
//         initialized = true;
//         return stable_percent;
//     }
//     // If charger state changed, reset stable_percent to current reading
//     if (plug_detected != prev_plug_state)
//     {
//         stable_percent = real_percent;
//         prev_plug_state = plug_detected;
//         return stable_percent;
//     }
//     // Only update when change exceeds hysteresis
//     if (real_percent > stable_percent)
//     {
//         if ((real_percent - stable_percent) > HYST)
//             stable_percent = real_percent;
//         else if (plug_detected)
//             stable_percent = real_percent;  // charging: prefer higher side within small fluctuation
//     }
//     else if (real_percent < stable_percent)
//     {
//         if ((stable_percent - real_percent) > HYST)
//             stable_percent = real_percent;
//         else if (!plug_detected)
//             stable_percent = real_percent;  // discharging: prefer lower side within small fluctuation
//     }
//     return stable_percent;
// }

// uint8_t get_stable_battery_percent(uint8_t real_percent, bool plug_detected)
// {
//     const uint8_t HYST = 1;                // hysteresis threshold (±1%)
//     static uint8_t stable_percent = 0;
//     static bool initialized = false;
//     // First call initialization
//     if (!initialized)
//     {
//         stable_percent = real_percent;
//         initialized = true;
//         return stable_percent;
//     }
//     // Always bias downward — only lower values are accepted
//     if (real_percent < stable_percent)
//     {
//         // Falling: accept if drop is large OR within hysteresis
//         if ((stable_percent - real_percent) > HYST)
//             stable_percent = real_percent;
//         // else
//         //     stable_percent = real_percent;   // small drop also accepted
//     }
//     else if (real_percent > stable_percent)
//     {
//         // Rising: accept ONLY if rise is larger than hysteresis
//         if ((real_percent - stable_percent) > HYST)
//             stable_percent = real_percent;
//         // If rise is small (tiny fluctuation upward), ignore
//     }
//     return stable_percent;
// }

uint8_t get_stable_battery_percent(uint8_t real_percent)
{
    const uint8_t HYST = 1;                
    static uint8_t stable_percent = 0;
    static TickType_t x_chrg_stable_timer = 0;
    static bool initialized = false;

    if (!initialized)
    {
        stable_percent = real_percent;
        initialized = true;
        return stable_percent;
    }

    if (device_data.plug.plugdet==PLUG_DETECTED)
    {

        // When plugged in, stable percent should always rise to the higher value immediately
        if (real_percent > stable_percent)
        {
            stable_percent = real_percent;
        }
        else if (real_percent < stable_percent)
        {
            // Falling values still use hysteresis
            if ((stable_percent - real_percent) > HYST){
                stable_percent = real_percent;
            }
        }

        if(device_data.battery.charging==BATTERY_NOT_CHARGING && TIMER_ELAPSED(x_chrg_stable_timer) >= pdMS_TO_TICKS(2000)){
            if (stable_percent == 99)
                stable_percent = 100;
        }

    }
    else
    {
        // Original behavior (hysteresis both ways)
        if (real_percent < stable_percent)
        {
            if ((stable_percent - real_percent) > HYST)
                stable_percent = real_percent;
        }
        else if (real_percent > stable_percent)
        {
            if ((real_percent - stable_percent) > HYST)
                stable_percent = real_percent;
        }

        if ((stable_percent == 99 && g_stru_config.b_high_bat_alert==true))// || (stable_percent == 99 && TIMER_ELAPSED(x_warmer_start_timer) < pdMS_TO_TICKS (4000) ))
            stable_percent = 100;
    
        TIMER_RESET(x_chrg_stable_timer);
    }

    return stable_percent;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Update sensor inputs and measurements
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

uint8_t prev_percent = 0;
float start_temp = -16.0f;
static void v_WARMER_Update_Inputs (void)
{
    static TickType_t x_encoder_timer = 0;
    static TickType_t x_rpm_stable_timer = 0;
    static TickType_t x_rpm_E7_timer = 0; 
    static TickType_t x_charger_plug_in_timer = 0;
    static TickType_t x_plug_in_batt_percent_update_timer = 0;  

    /* Update temperature measurement */
    THER_inst_t x_ntc_inst = x_THER_Get_Inst (THER_NTC_TEMP);
    if (x_ntc_inst == NULL)
    {
        LOGE ("Failed to get instance handle of thermistor sensor");
    }
    else
    {
        float flt_current_temp = flt_THER_Get_Temperature (x_ntc_inst);
        float flt_history_temp = flt_THER_Get_Record (x_ntc_inst, THER_OLDEST_RECORD_IDX);

#ifdef DEVELOPMENT_TEST_MODE
        static float flt_simulated_temp = 31;
        static bool b_increased = true;
        if ((g_stru_sm.enm_curr_state == WARMER_STATE_HEATING) )
        {
            b_increased = (flt_simulated_temp >= 55) ? false :
                          (flt_simulated_temp <= 30) ? true : b_increased;
            flt_simulated_temp = b_increased ? flt_simulated_temp + (WARMER_TASK_PERIOD_MS / 1000.0) :
                                               flt_simulated_temp - (WARMER_TASK_PERIOD_MS / 1000.0);
        }else if ((g_stru_sm.enm_curr_state == WARMER_STATE_MAINTAINING)){

        	b_increased = (flt_simulated_temp > g_stru_config.u8_temp_setpoint +2) ? false :
        	              (flt_simulated_temp < g_stru_config.u8_temp_setpoint -5) ? true : b_increased;
        	flt_simulated_temp = b_increased ? flt_simulated_temp + (WARMER_TASK_PERIOD_MS / 1000.0) :
        	                                   flt_simulated_temp - (WARMER_TASK_PERIOD_MS / 1000.0);

        }
        else
        {
            flt_simulated_temp = 31;
        }
        flt_history_temp = flt_simulated_temp;
        // flt_history_temp = 32.5f; //===========================================================================================================>>>
#endif

        g_stru_dev.flt_temperature = (flt_history_temp != THER_INVALID_TEMP) ? flt_history_temp : flt_current_temp;
        //E$/E%
        
        if(g_stru_sm.enm_curr_state == WARMER_STATE_HEATING)
        {
            // g_stru_dev.flt_temperature = (g_stru_dev.flt_temperature - 2.5f);  //temporary disabled for testing wigg heating
            device_data.stateTemp = (g_stru_dev.flt_temperature >= g_stru_config.u8_temp_setpoint) ?
                                    TEMP_REACH : TEMP_NOT_REACH;
        }
        else
        {
            device_data.stateTemp = (g_stru_dev.flt_temperature >= g_stru_config.u8_temp_setpoint) ?
                                    TEMP_REACH : TEMP_NOT_REACH;
        }

        /* If temperature reaches the set point while heating, display the set point instead of the real temperature */
        device_data.ntcTemp_C = (g_stru_sm.enm_curr_state == WARMER_STATE_MAINTAINING || temp_reach_counter!=0) ?
                                g_stru_config.u8_temp_setpoint : g_stru_dev.flt_temperature;


        if(g_stru_sm.enm_curr_state == WARMER_STATE_HEATING || g_stru_sm.enm_curr_state == WARMER_STATE_MAINTAINING || g_stru_sm.enm_curr_state == WARMER_STATE_EXTENDED_MAINTAINING)
        {
            //  device_data.warming_percent = g_stru_dev.u8_warming_percent;   
             g_stru_wp.warming_curr_percent = u8_convert_ntc_To_Precent(g_stru_dev.flt_temperature, g_stru_config.u8_temp_setpoint); 
             if (g_stru_wp.warming_curr_percent > g_stru_wp.warming_prev_percent) {
                 g_stru_wp.warming_prev_percent = g_stru_wp.warming_curr_percent;
                device_data.warming_percent = g_stru_wp.warming_curr_percent;
                ui_post_update(UI_UPDATE_WARMING_PERCENT,0);
                vTaskDelay(pdMS_TO_TICKS(20));
            }

        }
    }

        /* Determine if battery is being charged */
    GPIO_inst_t x_plug_inst;
    if (s8_GPIO_Get_Inst (GPIO_EXT_PLUG, &x_plug_inst) != GPIO_OK)
    {
        LOGE ("Failed to get instance handle of plug detection pin");
    }
    else
    {
       s8_GPIO_Read_Active (x_plug_inst, &g_stru_dev.b_plug);
       
       if(device_stable){
        device_data.plug.plugdet = g_stru_dev.b_plug ? PLUG_DETECTED : PLUG_NOT_DETECTED;
       }else{
        device_data.plug.plugdet= PLUG_NOT_DETECTED;
       }
       

       if(device_data.plug.plugdet==PLUG_NOT_DETECTED){
        TIMER_RESET(x_charger_plug_in_timer);
       }

       if( device_data.plug.plugdet == PLUG_DETECTED){  // &&  device_data.battery.state != BATTERY_FULL ){
            
            if(device_data.plug.miliAmps >= WARMER_CUTOFF_CHARGING_A ){
                g_stru_dev.b_charging = BATTERY_CHARGING;
            }else{
                g_stru_dev.b_charging = BATTERY_NOT_CHARGING;
            }
            
            TIMER_RESET (g_x_idle_timer);
          
       }
       else{
             g_stru_dev.b_charging = BATTERY_NOT_CHARGING;
             TIMER_RESET(g_x_charging_ui_timer);
       }
         device_data.battery.charging = g_stru_dev.b_charging ? BATTERY_CHARGING : BATTERY_NOT_CHARGING;
    }






    /* Determine if battery is enabled */
    if (s8_GPIO_Get_Inst (GPIO_BAT_EN, &x_bat_en_inst) != GPIO_OK)
    {
        LOGE ("Failed to get instance handle of battery enable pin");
    }
    else
    {
       s8_GPIO_Read_Active (x_bat_en_inst, &g_stru_dev.b_bat_enable);
       device_data.battery.enable = g_stru_dev.b_bat_enable ? BATTERY_ENABLE : BATTERY_DISABLE;
    }

    /* Update battery voltage, percentage, and status */
    PSMON_inst_t x_bat_inst = x_PSMON_Get_Inst (BP_MON_14V);
    if (x_bat_inst == NULL)
    {
        LOGE ("Failed to get instance handle of battery measurement");
    }
    else
    {
        static TickType_t x_bat_full_timer = 0;
        static TickType_t x_show_bat_full_msg_timer = 0;
        static TickType_t x_bat_update_timer = 0;
        static TickType_t x_charging_icon_timer=0;
        
        if(g_stru_sm.enm_curr_state != WARMER_STATE_IDLE){
            TIMER_RESET(x_bat_update_timer);
        }

        enm_PSMON_Get_Bat_Info (x_bat_inst, &g_stru_dev.u16_bat_voltage, &g_stru_dev.u8_bat_percent);
        
        if(device_data.plug.plugdet==PLUG_NOT_DETECTED ){
        device_data.battery.miliVolts = g_stru_dev.u16_bat_voltage;
            // device_data.battery.percent = g_stru_dev.u8_bat_percent;
            if(device_data.battery.state != BATTERY_FULL){

                if(g_stru_sm.enm_curr_state == WARMER_STATE_IDLE){
                device_data.battery.percent = get_stable_battery_percent(g_stru_dev.u8_bat_percent);
                }

                if(device_data.battery.percent <= 98 && g_stru_config.b_high_bat_alert==true && device_stable){
                    g_stru_config.b_high_bat_alert=false;
                    enm_WARMER_Save_Config();
                }
                
            }
            TIMER_RESET (x_bat_full_timer);
            TIMER_RESET(x_show_bat_full_msg_timer);
            // show_bat_full_msg=false;
        }else{
            if(TIMER_ELAPSED(x_plug_in_batt_percent_update_timer) >= pdMS_TO_TICKS(WARMER_PLUG_IN_BAT_PERCENT_UPDATE_T) && g_stru_sm.enm_curr_state == WARMER_STATE_IDLE){
                    TIMER_RESET(x_plug_in_batt_percent_update_timer);
                    // TIMER_RESET(x_plugged_in_bat_percent_measure_timer);
                        update_plugged_in_bat_percent=true;
            }
        }
        
        // if (g_stru_dev.u8_bat_percent < WARMER_BAT_FULL_THRESHOLD)
        // {
        //     TIMER_RESET (x_bat_full_timer);
        // }

        g_stru_dev.enm_bat_status = enm_PSMON_Get_Bat_Alert_Status (x_bat_inst);
        if(is_ready_for_charging){// && TIMER_ELAPSED(x_bat_full_timer) >= pdMS_TO_TICKS (3000)){
            if ((g_stru_dev.enm_bat_status == PSMON_ALERT_HIGH) && (device_data.plug.miliAmps < WARMER_BAT_FULL_MSG_A) && device_data.plug.plugdet && g_stru_sm.enm_curr_state == WARMER_STATE_IDLE)
            {
                // device_data.battery.state = (TIMER_ELAPSED (x_bat_full_timer) >= pdMS_TO_TICKS (WARMER_BAT_FULL_INTERVAL)) ?
                //                             BATTERY_FULL : BATTERY_NORMAL;

                if( (device_data.plug.miliAmps < WARMER_BAT_FULL_MSG_A) &&  TIMER_ELAPSED (x_show_bat_full_msg_timer) >= pdMS_TO_TICKS (8*1000) && show_bat_full_msg){
                        // TIMER_RESET(x_show_bat_full_msg_timer);
                        // LOGW("in 500");
                        show_bat_full_msg=false;
                        device_data.battery.percent = 100;
                        g_stru_config.b_high_bat_alert = true;
                        enm_WARMER_Save_Config();

                        
                    ui_post_update(UI_UPDATE_DONE_SCR,5);
                    // v_WARMER_Control_Battery(false);
                    app_notify_200ms_double();

                 }

                 if( (device_data.plug.miliAmps < WARMER_CUTOFF_CHARGING_A) &&  TIMER_ELAPSED (x_bat_full_timer) >= pdMS_TO_TICKS (WARMER_BAT_FULL_INTERVAL)){
                        device_data.battery.state = BATTERY_FULL;
                        // LOGW("in 270");
                 }
                 
                          
                if(device_data.battery.state == BATTERY_FULL ){
                    // LOGW("in BAT FULL");
                    device_data.battery.percent = 100;
                    // if(is_ready_for_charging ){
                        
                        is_ready_for_charging=false;
                        g_stru_config.b_high_bat_alert = true;
                        enm_WARMER_Save_Config();
                }

                // if(show_bat_full_msg){
                //     show_bat_full_msg=false;
                    
                    
                // }
            }
            else
            {
                device_data.battery.state = ( g_stru_config.b_low_bat_alert == true) ? BATTERY_LOW : BATTERY_NORMAL;
  
                // PARAM_LOW_BAT_ALERT
                // device_data.battery.percent = get_stable_battery_percent(g_stru_dev.u8_bat_percent, g_stru_dev.b_plug);
            }
        }else{
             device_data.battery.state = ( g_stru_config.b_low_bat_alert == true) ? BATTERY_LOW : BATTERY_NORMAL;
        }

        if(TIMER_ELAPSED (x_bat_update_timer) >= pdMS_TO_TICKS (3000) && (curr_scr==ui_Main || curr_scr==ui_SplashWigg || curr_scr==ui_Done))//2000
        {
            ui_post_update(UI_UPDATE_BATTERY_PERCENT,0);
            vTaskDelay(pdMS_TO_TICKS(20));
            TIMER_RESET(x_bat_update_timer);
        }

        if(TIMER_ELAPSED (x_charging_icon_timer) >= pdMS_TO_TICKS (1000) && (curr_scr==ui_Main || curr_scr==ui_SplashWigg || curr_scr==ui_Done) && device_stable)
        {
            ui_post_update(UI_UPDATE_CHARGING_ICON,0);
            vTaskDelay(pdMS_TO_TICKS(20));
            TIMER_RESET(x_charging_icon_timer);
        }

        if(TIMER_ELAPSED (g_x_charging_ui_timer) >= pdMS_TO_TICKS (WARMER_CHARGING_UI_T) && g_stru_sm.enm_curr_state == WARMER_STATE_IDLE && device_data.battery.charging && show_bat_full_msg && device_stable)
        {
            ui_post_update(UI_UPDATE_DONE_SCR,6);   //charging ui
            vTaskDelay(pdMS_TO_TICKS(20));
            TIMER_RESET(g_x_charging_ui_timer);
        }

        if(update_plugged_in_bat_percent){
            TIMER_RESET(x_show_bat_full_msg_timer);
            v_WARMER_Control_Battery(false);
            if(TIMER_ELAPSED(x_plug_in_batt_percent_update_timer)>=pdMS_TO_TICKS (1000)){ //measure vbat stable reading timer
            
            device_data.battery.miliVolts = g_stru_dev.u16_bat_voltage;
            if(g_stru_sm.enm_curr_state == WARMER_STATE_IDLE){
                device_data.battery.percent = get_stable_battery_percent(g_stru_dev.u8_bat_percent);
            }

            update_plugged_in_bat_percent=false;
            v_WARMER_Control_Battery(true);
            if(device_stable){
                LOGI ("\t+ Measured Battery voltage plugged in    : %.2fV", device_data.battery.miliVolts / 1000.0);
                LOGI ("\t+ Measured Battery percent plugged in    : %d%%", device_data.battery.percent);      
            }
            
            }
        }
        

        if(device_data.battery.miliVolts < WARMER_REENABLE_CHARGING_V || device_data.plug.plugdet== PLUG_NOT_DETECTED){
            if(TIMER_ELAPSED(x_charger_plug_in_timer) < pdMS_TO_TICKS(WARMER_REENABLE_CHARGING_T) && !is_ready_for_charging){ //24h timer
                is_ready_for_charging=true;
                show_bat_full_msg=true;
                // LOGW("Next Charging");
            }
            if(!show_bat_full_msg){
                show_bat_full_msg=true;
                // LOGW("Next BAT MSG");
            }
        }

        
#ifdef SIMULATED_INPUT_MODE
        g_stru_dev.b_tilted = TILT_HIGH;
        g_stru_dev.u16_bat_voltage = WARMER_HEATER_CUTOFF_VOLTAGE + 1;
        device_data.battery.state = BATTERY_NORMAL; //===========================================================================================================>>>
#endif
    }

    // Disabled plug sense on request
    /* Update plug voltage and status */
    PSMON_inst_t x_ep_curr_inst = x_PSMON_Get_Inst (EP_CURR_MON);
    if (x_ep_curr_inst == NULL)
    {
        LOGE ("Failed to get instance handle of external plug");
    }
    else
    {
    
        enm_PSMON_Get_EP_Curr_Info (x_ep_curr_inst, &g_stru_dev.u16_plug_current);
        device_data.plug.miliAmps = g_stru_dev.u16_plug_current;
      

        // g_stru_dev.enm_plug_status = enm_PSMON_Get_Plug_Alert_Status (x_ep_curr_inst);
        // if (g_stru_dev.enm_plug_status == EPMON_ALERT_WRONG_ADAPTER)
        // {
        //     device_data.plug.status = PLUG_INVALID;   
        // }
        // else if (g_stru_dev.enm_plug_status == EPMON_ALERT_NONE)
        // {
        //     device_data.plug.status = PLUG_VALID;
        // }
    }

    // Disabled plug sense on request
    /* Update plug voltage and status */
    // PSMON_inst_t x_ext_pwr_inst = x_PSMON_Get_Inst (EP_MON_16V);
    // if (x_ext_pwr_inst == NULL)
    // {
    //     LOGE ("Failed to get instance handle of external plug");
    // }
    // else
    // {
    
    //     enm_PSMON_Get_Plug_Info (x_ext_pwr_inst, &g_stru_dev.u16_plug_voltage);
    //     device_data.plug.miliVolts = g_stru_dev.u16_plug_voltage;
      

    //     g_stru_dev.enm_plug_status = enm_PSMON_Get_Plug_Alert_Status (x_ext_pwr_inst);
    //     if (g_stru_dev.enm_plug_status == EPMON_ALERT_WRONG_ADAPTER)
    //     {
    //         device_data.plug.status = PLUG_INVALID;   
    //     }
    //     else if (g_stru_dev.enm_plug_status == EPMON_ALERT_NONE)
    //     {
    //         device_data.plug.status = PLUG_VALID;
    //     }
    // }

    /* Determine if water bottle is attached to the warmer */
    s8_GPIO_Read_Active (g_x_bottle_inst, &g_stru_dev.b_bottle_attached);
    // LOGE ("here");
#ifndef SIMULATED_INPUT_MODE
    device_data.cover_input = g_stru_dev.b_bottle_attached ? COVER_LOW : COVER_HIGH;
#endif

    

#ifdef SIMULATED_INPUT_MODE
     g_stru_dev.b_bottle_attached = true;
     device_data.cover_input= COVER_LOW;  //=====================================================================================================================>>>
#endif



    /* Determine whether the warmer tilts */
    GPIO_inst_t x_tilt_inst;
    if (s8_GPIO_Get_Inst (GPIO_TILT_SENSOR, &x_tilt_inst) != GPIO_OK)
    {
        LOGE ("Failed to get instance handle of tilt detection pin");
    }
    else
    {
        s8_GPIO_Read_Active (x_tilt_inst, &g_stru_dev.b_tilted);
        if(g_stru_config.u8_heater_mode != HEATER_M_BABYFOOD)
        {
            device_data.tilt_input = (g_stru_dev.b_tilted)   ? TILT_LOW : TILT_HIGH;
        }
        else
        {
            device_data.tilt_input = TILT_HIGH;
        }

#ifdef SIMULATED_INPUT_MODE
      device_data.tilt_input = TILT_HIGH; //===================================================================================================================>>>
#endif


    }

    if(g_stru_dev.flt_temperature >= TEMPERATURE_HIGH_E5){
            g_stru_dev.b_ntc_high_err = true;   
        }
    else if(g_stru_dev.flt_temperature <= TEMPERATURE_LOW_E4){
            g_stru_dev.b_ntc_low_err = true; 
        }
      
        
    /*Determine if ntc temperature is in error state */
    if (g_stru_dev.b_ntc_high_err)
    {
        device_data.ntc_temp_err = NTC_TEMP_HIGH_ERR;
    }
    else if (g_stru_dev.b_ntc_low_err)
    {
        device_data.ntc_temp_err = NTC_TEMP_LOW_ERR;
    }
    else
    {
        device_data.ntc_temp_err = NTC_TEMP_ERR_NONE;
    }

#ifdef SIMULATED_INPUT_MODE
     device_data.ntc_temp_err = NTC_TEMP_ERR_NONE;  //==============================================================================================================>>>
#endif
    if(isBoot && isErrorUpdateRequired){
       ui_post_update(UI_UPDATE_MAIN,0);
        vTaskDelay(pdMS_TO_TICKS(20));
    }

    // update RPM reading
    if(isMotor_PID_En){
    if(TIMER_ELAPSED(x_encoder_timer)>= pdMS_TO_TICKS (100)){
    TIMER_RESET(x_encoder_timer);
    encoder_update_rpm();
    float rpm = encoder_get_rpm();  // Read current value anytime
    g_stru_dev.motor_rpm = rpm;
    
        if(!startHeating){
            static bool timer_started = false;
            static bool E7_timeout_active = false;

            if(!E7_timeout_active){
                TIMER_RESET(x_rpm_E7_timer);
                E7_timeout_active=true;
             }

            if(TIMER_ELAPSED(x_rpm_E7_timer) >= pdMS_TO_TICKS(18* 1000) && E7_timeout_active){

                // LOGE("RPM NOT GETTING STABLE E-7");
                E7_timeout_active=false;
                isMotor_PID_En=false;
                TIMER_RESET(x_rpm_E7_timer);    
                g_stru_dev.b_wigg_seal_err = true;
                v_WARMER_Control_Motor (WARMER_MOTOR_OFF);

            }

            if(rpm==600.0f){

                 if (!timer_started) {
                    timer_started = true;
                    // LOGW("Timer Started");
                    TIMER_RESET(x_rpm_stable_timer);   // start timing from first hit
                }
            }
            
            if (TIMER_ELAPSED(x_rpm_stable_timer) >= pdMS_TO_TICKS(3000) && timer_started) {
                startHeating = true;
                timer_started = false;
                E7_timeout_active=false;
                TIMER_RESET(x_rpm_E7_timer);
                LOGW("Motor RPM stable");
            }
        }
        auto_control_update(rpm);
    }
    
    }

}


void auto_control_update(float rpm)
{
    
    float error = TARGET_RPM - rpm;

    // ignore very small noise near tolerance
    // if (error < RPM_TOLERANCE && error > -RPM_TOLERANCE)
    //     error = 0;

    // integrate over time for slow load tracking
    integral += error * DT;
    if (integral > 10000.0f) integral = 10000.0f;
    if (integral < -10000.0f) integral = -10000.0f;

    // Calculate the Derivative term.
    float derivative = (error - prev_error) / DT;
    prev_error = error;

    // proportional + integral control
    float control_output = (KP * error) + (KI * integral) + (KD * derivative);
    float new_pwm = current_pwm + control_output;

    // apply soft ramping to avoid big jumps
    float delta = new_pwm - current_pwm;
    if (delta > PWM_RAMP_LIMIT) delta = PWM_RAMP_LIMIT;
    if (delta < -PWM_RAMP_LIMIT) delta = -PWM_RAMP_LIMIT;
    new_pwm = current_pwm + delta;

    // clamp limits
    if (new_pwm > PWM_MAX) new_pwm = PWM_MAX;
    if (new_pwm < PWM_MIN) new_pwm = PWM_MIN;

    // optional smoothing
    current_pwm = (PWM_SMOOTH_ALPHA * new_pwm) +
                  ((1.0f - PWM_SMOOTH_ALPHA) * current_pwm);

    v_WARMER_Control_Motor((uint32_t)current_pwm);
}


/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Runs state machine of the warmer
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void v_WARMER_Run_State_Machine (void)
{
    // static TickType_t x_back_to_prev_scr_timer;
    static bool b_heating_done = false;
    static bool extended_maintaining_done = false;
    static bool b_ready_for_heating_prev = false;
    bool        b_ready_for_heating = false;
    
    static bool b_btn_on_prev_hold_short = false;
    bool        b_btn_on_hold_short_pressed = false;
    bool        b_btn_on_hold_short_released = false;

    static bool b_btn_on_prev_hold_long = false;
    bool        b_btn_on_hold_long_pressed = false;
    bool        b_btn_on_hold_long_released = false;

    static bool b_btn_up_prev = false;
    bool        b_btn_up_hold_short_edge = false;
    bool        b_btn_up_released_edge = false;

    static bool b_btn_th_prev = false;
    bool        b_btn_th_hold_short_edge = false;
    bool        b_btn_th_released_edge = false;

    bool b_btn_on_hold_short_released_main_screen = false;
    bool b_btn_th_curr = (enm_BTN_Get_State (g_x_btn_th_inst) == BTN_STATE_HOLD_SHORT || 
                          enm_BTN_Get_State (g_x_btn_th_inst) == BTN_STATE_HOLD_LONG);

    bool b_btn_up_curr = (enm_BTN_Get_State (g_x_btn_up_inst) == BTN_STATE_HOLD_SHORT || 
                          enm_BTN_Get_State (g_x_btn_up_inst) == BTN_STATE_HOLD_LONG);


    bool b_btn_on_curr_hold_short = (enm_BTN_Get_State (g_x_btn_on_inst) == BTN_STATE_HOLD_SHORT);
    bool b_btn_on_curr_hold_long = (enm_BTN_Get_State (g_x_btn_on_inst) == BTN_STATE_HOLD_LONG);  

    vTaskDelay(pdMS_TO_TICKS(5));
           
    if ((p_x_active_btn != NULL) && (p_x_active_btn != g_x_btn_on_inst))
    {
        b_btn_on_curr_hold_short = false;
        b_btn_on_curr_hold_long = false;
    }
    
    if ((p_x_active_btn != NULL) && (p_x_active_btn != g_x_btn_up_inst))
    {
        b_btn_up_curr = false;
    } 

    if ((p_x_active_btn != NULL) && (p_x_active_btn != g_x_btn_th_inst))
    {
        b_btn_th_curr = false;
    }
    
    if(p_x_active_btn == NULL){
        b_btn_on_curr_hold_short = false;
        b_btn_on_curr_hold_long = false;
        b_btn_up_curr = false;
        b_btn_th_curr = false;
    }

    // if(!back_to_prev_scr){
    //     TIMER_RESET(x_back_to_prev_scr_timer);
    // }

    // if(TIMER_ELAPSED(x_back_to_prev_scr_timer) >= pdMS_TO_TICKS(400)){
    //     back_to_prev_scr=false;
    // }

                                
        b_btn_on_hold_short_pressed = ((b_btn_on_prev_hold_short == false) && (b_btn_on_curr_hold_short == true));
        b_btn_on_hold_short_released = ((b_btn_on_prev_hold_short == true) && (b_btn_on_curr_hold_short == false) && (b_btn_on_curr_hold_long == false));
        b_btn_on_prev_hold_short = b_btn_on_curr_hold_short;

        b_btn_on_hold_long_pressed = ((b_btn_on_prev_hold_long == false) && (b_btn_on_curr_hold_long == true));
        b_btn_on_hold_long_released = ((b_btn_on_prev_hold_long == true) && (b_btn_on_curr_hold_long == false));
        b_btn_on_prev_hold_long = b_btn_on_curr_hold_long;

        if (b_btn_on_hold_short_pressed)
        {
            g_enm_button_event = BTN_EVENT_ON_HOLD_SHORT_PRESSED;

        }
        if(b_btn_on_hold_short_released){
            if(curr_scr==ui_Main || curr_scr==ui_Warming){
                        b_btn_on_hold_short_released_main_screen = true;
                }
            // ESP_LOGI(TAG, "Button Event: BTN_EVENT_ON_HOLD_SHORT_RELESED");
            g_enm_button_event = BTN_EVENT_ON_HOLD_SHORT_RELEASED;
            p_x_active_btn = NULL;
        }

        if (b_btn_on_hold_long_pressed)
        {
            g_enm_button_event = BTN_EVENT_ON_HOLD_LONG_PRESSED;
            // ESP_LOGI(TAG, "Button Event: BTN_EVENT_ON_HOLD_SHORT_PRESSED");
        }
        if(b_btn_on_hold_long_released){
            // ESP_LOGI(TAG, "Button Event: BTN_EVENT_ON_HOLD_SHORT_RELESED");
            g_enm_button_event = BTN_EVENT_ON_HOLD_LONG_RELEASED;
            p_x_active_btn = NULL;
        }

                            
        b_btn_up_hold_short_edge = ((b_btn_up_prev == false) && (b_btn_up_curr == true));
        b_btn_up_released_edge = ((b_btn_up_prev == true) && (b_btn_up_curr == false));
        b_btn_up_prev = b_btn_up_curr;

        if (b_btn_up_hold_short_edge)
        {
            // ESP_LOGI(TAG, "Button Event: BTN_EVENT_UP_HOLD_SHORT_PRESSED");
            g_enm_button_event = BTN_EVENT_UP_HOLD_SHORT_PRESSED;

        }
        if(b_btn_up_released_edge){
            // ESP_LOGI(TAG, "Button Event: BTN_EVENT_UP_HOLD_SHORT_RELEASED");
            g_enm_button_event = BTN_EVENT_UP_HOLD_SHORT_RELEASED;
            p_x_active_btn = NULL;
        }

                        
        b_btn_th_hold_short_edge = ((b_btn_th_prev == false) && (b_btn_th_curr == true));
        b_btn_th_released_edge = ((b_btn_th_prev == true) && (b_btn_th_curr == false));
        b_btn_th_prev = b_btn_th_curr;

        if (b_btn_th_hold_short_edge )
        {
            // ESP_LOGI(TAG, "Button Event: BTN_EVENT_TH_HOLD_SHORT_PRESSED");
            g_enm_button_event = BTN_EVENT_TH_HOLD_SHORT_PRESSED;

        }
        if(b_btn_th_released_edge ){
            // ESP_LOGI(TAG, "Button Event: BTN_EVENT_TH_HOLD_SHORT_RELEASED");
            g_enm_button_event = BTN_EVENT_TH_HOLD_SHORT_RELEASED;
            p_x_active_btn = NULL;
        }

     

    if(g_stru_config.u8_heater_mode != HEATER_M_BABYFOOD)
    {
        /* Determine if all conditions for heating are met */
        b_ready_for_heating =   (g_stru_dev.b_bottle_attached) &&
                                (!g_stru_dev.b_tilted) &&
                                // (!g_stru_dev.b_charging) && //allow heating while charging, changed on request
                                (g_stru_dev.u16_bat_voltage > WARMER_HEATER_CUTOFF_VOLTAGE) &&
                                (!g_stru_dev.b_wigg_seal_err) &&
                                (!g_stru_dev.b_ntc_low_err) &&
                                (!g_stru_dev.b_ntc_high_err);
    }
    else
    {
        /* Determine if all conditions for heating are met */
        b_ready_for_heating =     
                                (g_stru_dev.b_bottle_attached) &&
                                // (!g_stru_dev.b_charging) && //allow heating while charging, changed on request
                                (g_stru_dev.u16_bat_voltage > WARMER_HEATER_CUTOFF_VOLTAGE) &&
                                (!g_stru_dev.b_ntc_low_err) &&
                                (!g_stru_dev.b_ntc_high_err);
    }

    /* Low voltage indication to LED task */
    if(g_stru_dev.u16_bat_voltage < WARMER_HEATER_CUTOFF_VOLTAGE && device_stable)
    {
        // g_low_voltage_alert = true;
        g_stru_config.b_low_bat_alert = true;
        enm_WARMER_Save_Config();
    }
                          
    if (b_ready_for_heating_prev != b_ready_for_heating)
    {
        /* Stop any buzzer playing if it is or is not ready for heating */
        b_ready_for_heating_prev = b_ready_for_heating;
        // app_buzzer_off (); //hbh
    }

    Low_Voltage_Indicator();


    
    

    /* Process state of the warmer */
    switch (g_stru_sm.enm_curr_state)
    {
        /* The warmer is awaking after a deep sleep */
        case WARMER_STATE_AWAKING:
        {
            /* State entry */
                if (g_stru_sm.u32_state_counter == 0)
            {
                LOGI (">>> WARMER_STATE_AWAKING");
            }

            /* Wait until all buttons are released */
            if ((enm_BTN_Get_State (g_x_btn_on_inst) == BTN_STATE_RELEASED) &&
                (enm_BTN_Get_State (g_x_btn_up_inst) == BTN_STATE_RELEASED) &&
                (enm_BTN_Get_State (g_x_btn_th_inst) == BTN_STATE_RELEASED))
            {
                g_stru_dev.b_wigg_seal_err = false;
                WARMER_CHANGE_STATE (WARMER_STATE_IDLE);
            }
            break;
        }

        /* The warmer is idling */
        case WARMER_STATE_IDLE:
        {
            static TickType_t x_thermometer_wait_timer = 0;
            // static TickType_t x_thermometer_measure_timer = 0;
            // static float measured_temp = 0.0f;
            // static uint32_t measured_temp_count = 0;
            static uint32_t last_remaining_sec = 0; // for thermometer display update
                      
            /* State entry */
            if (g_stru_sm.u32_state_counter == 0)
            {
                LOGI (">>> WARMER_STATE_IDLE");
                v_WARMER_Control_Heater (WARMER_HEATER_OFF);
                TIMER_RESET (g_x_idle_timer);
                TIMER_RESET(g_x_charging_ui_timer);
                isMeasuringTemp = false;
                
                temp_reach_counter = 0;
                device_data.warming_percent=0;
                g_stru_wp.warming_curr_percent=0;
                g_stru_wp.warming_prev_percent=0;
                g_stru_wp.start_temperature = -16.0f;
                g_stru_wp.b_start_maintaining_percent = false;
                /* Notify user if heating stopped */
                if ((g_stru_sm.enm_prev_state == WARMER_STATE_HEATING) ||
                    (g_stru_sm.enm_prev_state == WARMER_STATE_MAINTAINING ))
                { 
                    LOGI ("Stop heating");
                    app_notify_200ms_double ();
                         
                    /* Play notification song if heating has been done normally */
                    if (b_heating_done)
                    {
                        app_notify_play_done ();
                        vTaskDelay(pdMS_TO_TICKS(20));
                        ui_post_update(UI_UPDATE_DONE_SCR,2); 
                        vTaskDelay (pdMS_TO_TICKS (3000)); 
                        v_WARMER_Control_Motor (WARMER_MOTOR_OFF);
                        

                    }

                    if(isMotor_PID_En){
                        // vTaskDelay(pdMS_TO_TICKS(3000));
                        isMotor_PID_En = false;
                        // lv_label_set_text_fmt(ui_LabelRPM, "%d", 0);
                        
                        // lv_label_set_text_fmt(ui_LabelPWM, "%d", 0);
                        startHeating = false;
                        v_WARMER_Control_Motor (WARMER_MOTOR_OFF);
                        g_stru_dev.motor_rpm = 0.0f;
                    }
                }

                if(g_stru_sm.enm_prev_state == WARMER_STATE_EXTENDED_MAINTAINING){
                    ui_post_update(UI_UPDATE_TEXT,0);  //Reset "Keeping in Progress" main info label when back to IDLE,
                   if(extended_maintaining_done){
                        app_notify_500ms ();
                        ui_post_update(UI_UPDATE_DONE_SCR,4);
                        vTaskDelay(pdMS_TO_TICKS(3000));
                        v_WARMER_Control_Motor (WARMER_MOTOR_OFF);
                    }

                    if(isMotor_PID_En){
                    // vTaskDelay (pdMS_TO_TICKS (3000)); 
                    isMotor_PID_En = false;
                    startHeating = false;
                    v_WARMER_Control_Motor (WARMER_MOTOR_OFF);
                    g_stru_dev.motor_rpm = 0.0f;
                    }


                }

                /* Ensure that button ON is released */
                if (enm_BTN_Get_State (g_x_btn_on_inst) != BTN_STATE_RELEASED)
                {
                    while (enm_BTN_Get_State (g_x_btn_on_inst) != BTN_STATE_RELEASED)
                    {
                        vTaskDelay (pdMS_TO_TICKS (WARMER_TASK_PERIOD_MS));
                    }
                    b_btn_on_prev_hold_short = false;
                    break;
                }

         
            }

            else if (TIMER_ELAPSED (g_x_idle_timer) >= pdMS_TO_TICKS (WARMER_IDLE_BEFORE_SLEEP_MS))
            {
                WARMER_CHANGE_STATE (WARMER_STATE_SHUTTING_DOWN);
            }

            /* If button ON is released or the heater is on via BLE, validate required conditions and start heating */
            if (((b_btn_on_hold_short_released_main_screen ) || device_data.heater_enabled) )
            {
                
                if (b_ready_for_heating && (device_data.battery.state != BATTERY_LOW) && g_stru_dev.flt_temperature < g_stru_config.u8_temp_setpoint )
                {
                    WARMER_CHANGE_STATE (WARMER_STATE_HEATING);
                    ui_post_update(UI_UPDATE_DONE_SCR,1);
                    vTaskDelay(pdMS_TO_TICKS(20));
                }
                else
                {   
                    if(device_data.battery.state == BATTERY_LOW)
                    {
                        app_notify_500ms(); 
                    }                    
                    device_data.heater_enabled = false;

                    if(g_stru_dev.flt_temperature >= g_stru_config.u8_temp_setpoint)
                    {
                        ui_post_update(UI_UPDATE_ERROR,8); //already warm screen
                        vTaskDelay(pdMS_TO_TICKS(2000));
                        
                    }
                    
                    LOGW ("Cannot start the heater:");
                    LOGW ("+ Bottle is attached: %s", g_stru_dev.b_bottle_attached ? "OK" : "False");
                    LOGW ("+ Warmer does not tilt: %s", g_stru_dev.b_tilted ? "False" : "OK");
                    // LOGW ("+ Battery is not charging: %s", g_stru_dev.b_charging ? "False" : "OK");
                    LOGW ("+ Battery voltage is sufficient: %s",
                            (g_stru_dev.u16_bat_voltage > WARMER_HEATER_CUTOFF_VOLTAGE) ? "OK" : "False");
                    // LOGW ("+ Battery status: %s",
                    //         (device_data.battery.state == BATTERY_LOW) ? "LOW" : "OK");
                    LOGW ("+ Error State E5: %s", g_stru_dev.b_ntc_high_err ? "False" : "OK");
                    LOGW ("+ Error State E4: %s", g_stru_dev.b_ntc_low_err ? "False" : "OK");
                    LOGW ("+ Error State E7: %s", g_stru_dev.b_wigg_seal_err ? "False" : "OK"); 
                    LOGW ("+ Already Warm: %s", (g_stru_dev.flt_temperature >= g_stru_config.u8_temp_setpoint) ? "Yes" : "No");
                    ui_post_update(UI_UPDATE_ERROR,device_data.err_code);
                    vTaskDelay(pdMS_TO_TICKS(20));
                }
            }

            //start measuring temp with thermometer call
            if(isMeasuringTemp){
            
            uint32_t elapsed_ticks = TIMER_ELAPSED(x_thermometer_wait_timer);
            uint32_t elapsed_sec = elapsed_ticks/100 ;
            uint32_t remaining_sec = (elapsed_sec < 15) ? (15 - elapsed_sec) : 0;

                // if(elapsed_ticks < pdMS_TO_TICKS(10000))
                // {
                //     TIMER_RESET(x_thermometer_measure_timer);
                // }else{

                //     if(TIMER_ELAPSED(x_thermometer_measure_timer) <= pdMS_TO_TICKS(5000)){
                //          measured_temp += g_stru_dev.flt_temperature;
                //          measured_temp_count++;
                        
                //     }else{
                //         average_measured_temp = measured_temp / measured_temp_count;
                //         LOGW("Average Temperature: %.2fC\n", average_measured_temp);
                //         ui_post_update(UI_UPDATE_THERMOMETER_TEMP, 0);
                //         vTaskDelay(pdMS_TO_TICKS(10));
                //     }

                // }

                  if(elapsed_ticks < pdMS_TO_TICKS(15000))
                {
                    // TIMER_RESET(x_thermometer_measure_timer);
                }else{

                    // if(TIMER_ELAPSED(x_thermometer_measure_timer) <= pdMS_TO_TICKS(5000)){
                        //  measured_temp += g_stru_dev.flt_temperature;
                        //  measured_temp_count++;
                        
                    // }
                    // else{
                        average_measured_temp = g_stru_dev.flt_temperature;
                        // average_measured_temp = measured_temp / measured_temp_count;
                        LOGW("Average Temperature: %.2fC\n", average_measured_temp);
                        ui_post_update(UI_UPDATE_THERMOMETER_TEMP, 0);
                        vTaskDelay(pdMS_TO_TICKS(20));
                    // }

                }

                 
                if (remaining_sec != last_remaining_sec) {
                    last_remaining_sec = remaining_sec;

                    uint32_t percentage = (100 * (1500 -elapsed_ticks)) / 1500;
                    if (percentage > 100) percentage = 100;

                    // update arc
                    lv_arc_set_value(ui_TempMeasuringFill, percentage);                     
                    lv_label_set_text_fmt(ui_LabelTempMeasuringCount, "%d", remaining_sec);
                }
                    


            }else{
                last_remaining_sec = 0;
                // TIMER_RESET(x_thermometer_measure_timer);
                TIMER_RESET(x_thermometer_wait_timer);

            }

            break;
        }


        /* The warmer is heating water to the set temperature */
        case WARMER_STATE_HEATING:
        {
            static TickType_t x_heating_timer = 0;
            static TickType_t x_motor_timer = 0;
            static TickType_t x_heating_err_timer = 0;
            static TickType_t x_startup_delay_timer = 0;
            // static TickType_t x_initial_cycles_timer = 0;        //for first 3 cycles pwm X, 2 seconds ON, 3 seconds OFF for all modes
            static TickType_t x_temp_reach_check_timer = 0;		//for temperature interval check timer
            float  flt_start_temperature = 0;

            /* State entry */
            if (g_stru_sm.u32_state_counter == 0)
            {
                LOGI (">>> WARMER_STATE_HEATING");
                LOGI ("Start heating");
                TIMER_RESET (x_heating_timer);
                TIMER_RESET (x_motor_timer);
                TIMER_RESET (x_heating_err_timer);
                TIMER_RESET(x_startup_delay_timer);
                // TIMER_RESET(x_initial_cycles_timer);
                TIMER_RESET(x_temp_reach_check_timer);

                b_heating_done = false;
                
                extended_maintaining_done = false;
                device_data.heater_off_requested = false;
                g_stru_config.b_heater_failure = false;
                flt_start_temperature = g_stru_dev.flt_temperature;
                // app_buzzer_off (); //hbh
                app_notify_500ms();
            }
            /*Motor start/stop 3 seconds time*/
            
            // if(TIMER_ELAPSED(x_motor_timer) > pdMS_TO_TICKS(2000)){  
            //         v_WARMER_Control_Motor (12);
            //     }
            // else if(TIMER_ELAPSED(x_motor_timer) > pdMS_TO_TICKS(3000) && device_data.stateTemp == TEMP_REACH){
            //         v_WARMER_Control_Motor (WARMER_MOTOR_OFF);
            // } 

            /* Heater startup delay */
            if(TIMER_ELAPSED(x_startup_delay_timer) < pdMS_TO_TICKS(5000))
            {
                TIMER_RESET (x_heating_timer);
                TIMER_RESET (x_heating_err_timer);
                // TIMER_RESET (x_initial_cycles_timer);
                
                
                if(TIMER_ELAPSED(x_motor_timer) > pdMS_TO_TICKS(1600)){  
                    // v_WARMER_Control_Motor (WARMER_MOTOR_FINAL_PWM);
                    if(g_stru_config.u8_heater_mode != HEATER_M_BABYFOOD)
                    {
                        isMotor_PID_En = true;
                    }else{
                        isMotor_PID_En = false;
                        startHeating = true;
                    }
                }
                else if(TIMER_ELAPSED(x_motor_timer) > pdMS_TO_TICKS(100)){  
                    if(g_stru_config.u8_heater_mode != HEATER_M_BABYFOOD)
                    { 
                     v_WARMER_Control_Motor (WARMER_MOTOR_INITIAL_PWM);
                    }
                    // LOGW ("Motor ON");
                    
                }
            }
            // else if(TIMER_ELAPSED(x_initial_cycles_timer) < pdMS_TO_TICKS(15000)){  //3 cycle time
            //     if (TIMER_ELAPSED (x_heating_timer) < pdMS_TO_TICKS (2000) && device_data.stateTemp == TEMP_NOT_REACH)
            //         {
            //             v_WARMER_Control_Heater (WARMER_STARTING_DUTY_CYCLE);
            //         }
            //         else if (TIMER_ELAPSED (x_heating_timer) < pdMS_TO_TICKS (5000))
            //         {
            //         	v_WARMER_Control_Heater (WARMER_HEATER_OFF);
            //         }
            //         else
            //         {
            //             TIMER_RESET (x_heating_timer);
            //             // TIMER_RESET (x_motor_timer);
            //         }
            // }
            else if(startHeating)
            {
                // LOGW("In Heating");           
                /* For milk, heating pattern is [ON 3 second -> OFF 2 second] */
                if (g_stru_config.u8_heater_mode == HEATER_M_MILK)
                {
                    if (TIMER_ELAPSED (x_heating_timer) < pdMS_TO_TICKS (9000/*3000*/) && device_data.stateTemp == TEMP_NOT_REACH)
                    {   
                        
                        v_WARMER_Control_Heater ((g_stru_dev.u16_bat_voltage < 13500) && device_data.plug.plugdet ? WARMER_LOW_VOLTAGE_DUTY_CYCLE : 
												(g_stru_dev.u16_bat_voltage < 15490) ? WARMER_MEDIUM_VOLTAGE_DUTY_CYCLE : WARMER_HIGH_VOLTAGE_DUTY_CYCLE);
                    }
                    else if (TIMER_ELAPSED (x_heating_timer) < pdMS_TO_TICKS (10000/*5000*/))
                    {
                    	v_WARMER_Control_Heater (WARMER_HEATER_OFF);
                        
                    }
                    else
                    {
                        TIMER_RESET (x_heating_timer);
                        // TIMER_RESET (x_motor_timer);
                    }
                }
                /* For frozen, heating pattern is [ON 5 seconds -> OFF 2 second] */
                else if (g_stru_config.u8_heater_mode == HEATER_M_FROZEN)
                {
                    if (TIMER_ELAPSED (x_heating_timer) < pdMS_TO_TICKS (8000) && device_data.stateTemp == TEMP_NOT_REACH)
                    {
                        v_WARMER_Control_Heater ((g_stru_dev.u16_bat_voltage < 13500) && device_data.plug.plugdet ? WARMER_LOW_VOLTAGE_DUTY_CYCLE : 
                                                (g_stru_dev.u16_bat_voltage < 15490) ? WARMER_MEDIUM_VOLTAGE_DUTY_CYCLE : WARMER_HIGH_VOLTAGE_DUTY_CYCLE);
                    }
                    else if (TIMER_ELAPSED (x_heating_timer) < pdMS_TO_TICKS (10000))
                    {
                        v_WARMER_Control_Heater (WARMER_HEATER_OFF);
                    }
                    else
                    {
                        TIMER_RESET (x_heating_timer);
                        // TIMER_RESET (x_motor_timer);
                    }
                }
				/* For Baby, heating pattern is [ON 8 seconds -> OFF 2 second] */
                else if (g_stru_config.u8_heater_mode == HEATER_M_BABYFOOD)
                {
                    if (TIMER_ELAPSED (x_heating_timer) < pdMS_TO_TICKS (8000) && device_data.stateTemp == TEMP_NOT_REACH)
                    {
                        v_WARMER_Control_Heater (WARMER_BABY_FOOD_DUTY_CYCLE);
                    }
                    else if (TIMER_ELAPSED (x_heating_timer) < pdMS_TO_TICKS (10000))
                    {
                        v_WARMER_Control_Heater (WARMER_HEATER_OFF);
                    }
                    else
                    {
                        TIMER_RESET (x_heating_timer);
                        // TIMER_RESET (x_motor_timer);
                    }
                }
                /* For water, heating pattern is [ON 8 seconds -> OFF 2 second] */
                else
                {
                    if (TIMER_ELAPSED (x_heating_timer) < pdMS_TO_TICKS (9000/*8000*/) && device_data.stateTemp == TEMP_NOT_REACH)
                    {
                        v_WARMER_Control_Heater ((g_stru_dev.u16_bat_voltage < 13500) && device_data.plug.plugdet ? WARMER_LOW_VOLTAGE_DUTY_CYCLE : 
												(g_stru_dev.u16_bat_voltage < 15490) ? WARMER_MEDIUM_VOLTAGE_DUTY_CYCLE : WARMER_HIGH_VOLTAGE_DUTY_CYCLE);
                    }       
                    else if (TIMER_ELAPSED (x_heating_timer) < pdMS_TO_TICKS (10000))
                    {
                        v_WARMER_Control_Heater (WARMER_HEATER_OFF);
                    }             
                    else
                    {
                        TIMER_RESET (x_heating_timer);
                        // TIMER_RESET (x_motor_timer);
                    }
                }
              
                if (TIMER_ELAPSED (x_heating_err_timer) >= pdMS_TO_TICKS (WARMER_TEMP_READ_FAILURE)) //
                {
                    g_stru_config.b_heater_failure = true;
                    WARMER_CHANGE_STATE (WARMER_STATE_IDLE);
                    v_WARMER_Control_Heater (WARMER_HEATER_OFF);
                }
                else if(TIMER_ELAPSED (x_heating_err_timer) >= pdMS_TO_TICKS (WARMER_HEATING_FAILURE))
                {   
                 
                    if(flt_start_temperature == g_stru_dev.flt_temperature) 
                    {
                        g_stru_config.b_heater_failure = true;
                        WARMER_CHANGE_STATE (WARMER_STATE_IDLE);
                        v_WARMER_Control_Heater (WARMER_HEATER_OFF);
                       
                    }
                }
                else
                {
                    //do nothing
                }
                
            }


               if (device_data.stateTemp == TEMP_REACH){
                   
                    if (TIMER_ELAPSED(x_temp_reach_check_timer) >= pdMS_TO_TICKS(WARMER_INTERVAL_CHECK)){

                        TIMER_RESET(x_temp_reach_check_timer);
                    	temp_reach_counter++;
                        // v_WARMER_Control_Heater (WARMER_HEATER_OFF);
                        LOGI ("<================ Check: %d --- Interval: 5 Seconds ================>", temp_reach_counter);
                        if (temp_reach_counter >= checkTimes)
                        {
                            temp_reach_counter = 0;
                             
                            WARMER_CHANGE_STATE(WARMER_STATE_MAINTAINING);
                             
                        }
                    }
                }

            /*
            ** If button ON is released, or heater is turned off via BLE,
            ** or the conditions for heating are not met, stop heating immediately
            */
            if ((b_btn_on_hold_short_released_main_screen) || device_data.heater_off_requested || (!b_ready_for_heating)) //  || (device_data.battery.charging == BATTERY_CHARGING))
            {
            	device_data.heater_enabled = false;
                WARMER_CHANGE_STATE (WARMER_STATE_IDLE);
                v_WARMER_Control_Heater (WARMER_HEATER_OFF);
                v_WARMER_Control_Motor (WARMER_MOTOR_OFF);
                
                if (!b_ready_for_heating)
                {
                    LOGW ("H: Stop heating because heating conditions are not met");
                    LOGW ("+ Bottle is attached: %s", g_stru_dev.b_bottle_attached ? "OK" : "False");
                    LOGW ("+ Warmer does not tilt: %s", g_stru_dev.b_tilted ? "False" : "OK");
                    // LOGW ("+ Battery is not charging: %s", g_stru_dev.b_charging ? "False" : "OK");
                    LOGW ("+ Battery is sufficient: %s",
                            (g_stru_dev.u16_bat_voltage > WARMER_HEATER_CUTOFF_VOLTAGE) ? "OK" : "False");
                    LOGW ("+ Error State E5: %s", g_stru_dev.b_ntc_high_err ? "False" : "OK");
                    LOGW ("+ Error State E4: %s", g_stru_dev.b_ntc_low_err ? "False" : "OK");
                    LOGW ("+ Error State E7: %s", g_stru_dev.b_wigg_seal_err ? "False" : "OK"); 
                    // if(g_stru_dev.u16_bat_voltage > WARMER_HEATER_CUTOFF_VOLTAGE){ device_data.err_code = 25;} //battery low
                    ui_post_update(UI_UPDATE_ERROR,device_data.err_code);
                    vTaskDelay(pdMS_TO_TICKS(20));
                }else{
                    ui_post_update(UI_UPDATE_DONE_SCR,0);
                    vTaskDelay(pdMS_TO_TICKS(20));
                }
            }
            break;
        }
        /* Temperature has reached the set point and is being maintained around set point */
        case WARMER_STATE_MAINTAINING:
        {
            
            static TickType_t x_heating_warming_timer = 0;
            
            static WARMER_state_t enm_delay_state = WARMER_STATE_HEATING;

            /* State entry */
            if (g_stru_sm.u32_state_counter == 0)
            {
                LOGI (">>> WARMER_STATE_MAINTAINING");
                TIMER_RESET (x_stable_timer);
                TIMER_RESET (x_heating_warming_timer);
                enm_delay_state = WARMER_STATE_HEATING;
            }

            /* Maintain the temperature at set point for a while */
            uint32_t u32_maintain_ms = 0;
            uint32_t u32_heating_ms  = 0;
            uint32_t u32_warming_ms  = 0;
            if (g_stru_config.u8_heater_mode == HEATER_M_MILK)
            {
                u32_maintain_ms = (g_stru_config.enm_volume_level == WARMER_VOLUME_120) ?
                                  WARMER_MAINTAIN_MILK_120 : WARMER_MAINTAIN_MILK_240;
                u32_heating_ms  = WARMER_HEATING_MILK;
                u32_warming_ms  = WARMER_WARMING_MILK;
            }
            else if (g_stru_config.u8_heater_mode == HEATER_M_WATER)
            {
                u32_maintain_ms = (g_stru_config.enm_volume_level == WARMER_VOLUME_120) ?
                                  WARMER_MAINTAIN_WATER_120 : WARMER_MAINTAIN_WATER_240;
                u32_heating_ms  = WARMER_HEATING_WATER;
                u32_warming_ms  = WARMER_WARMING_WATER;
            }
            else if (g_stru_config.u8_heater_mode == HEATER_M_BABYFOOD)
            {
                u32_maintain_ms = WARMER_MAINTAIN_BABY_FOOD;
                u32_heating_ms  = WARMER_HEATING_BABYFOOD;
                u32_warming_ms  = WARMER_WARMING_BABYFOOD;
            }
            else
            {
                u32_maintain_ms = (g_stru_config.enm_volume_level == WARMER_VOLUME_120) ?
                                  WARMER_MAINTAIN_FROZEN_120 : WARMER_MAINTAIN_FROZEN_240;
                u32_heating_ms  = WARMER_HEATING_FROZEN;
                u32_warming_ms  = WARMER_WARMING_FROZEN;
            }

            g_stru_wp.u32_maintaining_ms = u32_maintain_ms ;
            g_stru_wp.b_start_maintaining_percent=true;
            if (TIMER_ELAPSED (x_stable_timer) >= pdMS_TO_TICKS (u32_maintain_ms))
            {
                /* Heating is now actually done */
                b_heating_done = true;
                if (g_stru_config.u8_Keep_warm_period != 0) 
                {
                   
                    WARMER_CHANGE_STATE(WARMER_STATE_EXTENDED_MAINTAINING);
                }
                else
                {
                   WARMER_CHANGE_STATE(WARMER_STATE_IDLE);
                
                }
            }
            else
            {
                if(enm_delay_state == WARMER_STATE_HEATING)
                {
                    if (TIMER_ELAPSED (x_heating_warming_timer) < pdMS_TO_TICKS (u32_heating_ms))
                    {
                        if (device_data.stateTemp == TEMP_REACH)
                        {
                            v_WARMER_Control_Heater (WARMER_HEATER_OFF);
                            TIMER_RESET (x_heating_warming_timer);
                            enm_delay_state = WARMER_STATE_IDLE;
                            
                        }
                        else
                        {
                            if(g_stru_config.u8_heater_mode == HEATER_M_BABYFOOD)
                            {
                                v_WARMER_Control_Heater(WARMER_BABY_FOOD_DUTY_CYCLE);
                            }
                            else
                            {
                                v_WARMER_Control_Heater ((g_stru_dev.u16_bat_voltage < 13500) && device_data.plug.plugdet ? WARMER_LOW_VOLTAGE_DUTY_CYCLE : 
                                                        (g_stru_dev.u16_bat_voltage < 15490) ? WARMER_MEDIUM_VOLTAGE_DUTY_CYCLE : WARMER_HIGH_VOLTAGE_DUTY_CYCLE);
                            }
                        }           
                    }
                    else
                    {
                        TIMER_RESET (x_heating_warming_timer);
                        enm_delay_state = WARMER_STATE_IDLE;
                    }
                }
                else
                {
                    if (TIMER_ELAPSED (x_heating_warming_timer) < pdMS_TO_TICKS (u32_warming_ms))
                    {
                        v_WARMER_Control_Heater (WARMER_HEATER_OFF);
                    }
                    else
                    {
                        TIMER_RESET (x_heating_warming_timer);
                        enm_delay_state = WARMER_STATE_HEATING;
                    }
                }               
            }

            /*
            ** If button ON is released, or heater is turned off via BLE,
            ** or the conditions for heating are not met, stop heating immediately
            */
            if ((b_btn_on_hold_short_released_main_screen) || device_data.heater_off_requested || (!b_ready_for_heating))// || (device_data.battery.charging == BATTERY_CHARGING))
            {
    	        device_data.heater_enabled = false;
                WARMER_CHANGE_STATE (WARMER_STATE_IDLE);
                v_WARMER_Control_Heater (WARMER_HEATER_OFF);
                v_WARMER_Control_Motor (WARMER_MOTOR_OFF);
                // ui_post_update(UI_UPDATE_DONE_SCR,0);
                if (!b_ready_for_heating)
                {
                    LOGW ("M: Stop heating because heating conditions are not met");
                    LOGW ("+ Bottle is attached: %s", g_stru_dev.b_bottle_attached ? "OK" : "False");
                    LOGW ("+ Warmer does not tilt: %s", g_stru_dev.b_tilted ? "False" : "OK");
                    // LOGW ("+ Battery is not charging: %s", g_stru_dev.b_charging ? "False" : "OK");
                    LOGW ("+ Battery is sufficient: %s",
                            (g_stru_dev.u16_bat_voltage > WARMER_HEATER_CUTOFF_VOLTAGE) ? "OK" : "False");
                    LOGW ("+ Error State E5: %s", g_stru_dev.b_ntc_high_err ? "False" : "OK");
                    LOGW ("+ Error State E4: %s", g_stru_dev.b_ntc_low_err ? "False" : "OK");
                    LOGW ("+ Error State E7: %s", g_stru_dev.b_wigg_seal_err ? "False" : "OK");
                    ui_post_update(UI_UPDATE_ERROR,device_data.err_code);
                    vTaskDelay(pdMS_TO_TICKS(20));
                }else{
                    ui_post_update(UI_UPDATE_DONE_SCR,0);
                    vTaskDelay(pdMS_TO_TICKS(20));
                }
            }
            break;
        }
        case WARMER_STATE_EXTENDED_MAINTAINING:
        {
            static TickType_t x_extended_warming_timer = 0;
            static TickType_t x_extended_heating_warming_timer = 0;
            static WARMER_state_t enm_delay_state = WARMER_STATE_HEATING;
            
            /* State entry */
            if (g_stru_sm.u32_state_counter == 0)
            {
                LOGI (">>> WARMER_STATE_EXTENDED_MAINTAINING");
                TIMER_RESET (x_extended_warming_timer);
                TIMER_RESET (x_extended_heating_warming_timer);
                enm_delay_state = WARMER_STATE_HEATING;
                /* Play notification song if heating has been done normally */
                b_heating_done = false;
                vTaskDelay (pdMS_TO_TICKS (1000));
                // v_WARMER_Control_Motor (WARMER_MOTOR_OFF); //Motor Off while extended maintaining
                 ui_post_update(UI_UPDATE_DONE_SCR,3);
                 vTaskDelay(pdMS_TO_TICKS(20));
                app_notify_play_done ();
            }

            /* Maintain the temperature at set point for a while */            
            uint32_t u32_extended_heating_ms  = 0;
            uint32_t u32_extended_warming_ms  = 0;
            if (g_stru_config.u8_heater_mode == HEATER_M_MILK)
            {

                u32_extended_heating_ms  = WARMER_HEATING_MILK;
                u32_extended_warming_ms  = WARMER_WARMING_MILK;
            }
            else if (g_stru_config.u8_heater_mode == HEATER_M_WATER)
            {

                u32_extended_heating_ms  = WARMER_HEATING_WATER;
                u32_extended_warming_ms  = WARMER_WARMING_WATER;
            }
            else if (g_stru_config.u8_heater_mode == HEATER_M_BABYFOOD)
            {
                
                u32_extended_heating_ms  = WARMER_HEATING_BABYFOOD;
                u32_extended_warming_ms  = WARMER_WARMING_BABYFOOD;
            }
            else
            {
                u32_extended_heating_ms  = WARMER_HEATING_FROZEN;
                u32_extended_warming_ms  = WARMER_WARMING_FROZEN;
            }

            if (TIMER_ELAPSED (x_extended_warming_timer) >= pdMS_TO_TICKS (g_stru_config.u8_Keep_warm_period * 60 * 1000))
            {
                /* extended Heating is now actually done */
                extended_maintaining_done = true;               
                WARMER_CHANGE_STATE (WARMER_STATE_IDLE);
            }
            else
            {
                if(enm_delay_state == WARMER_STATE_HEATING)
                {
                    if (TIMER_ELAPSED (x_extended_heating_warming_timer) < pdMS_TO_TICKS (u32_extended_heating_ms))
                    {
                        if (device_data.stateTemp == TEMP_REACH)
                        {
                            v_WARMER_Control_Heater (WARMER_HEATER_OFF);
                            TIMER_RESET (x_extended_heating_warming_timer);
                            enm_delay_state = WARMER_STATE_IDLE;
                            
                        }
                        else
                        {
                            if(g_stru_config.u8_heater_mode == HEATER_M_BABYFOOD)
                            {
                                v_WARMER_Control_Heater(WARMER_BABY_FOOD_DUTY_CYCLE);
                            }
                            else
                            {                            
                                v_WARMER_Control_Heater ((g_stru_dev.u16_bat_voltage < 13500) && device_data.plug.plugdet ? WARMER_LOW_VOLTAGE_DUTY_CYCLE : 
                                                    (g_stru_dev.u16_bat_voltage < 15490) ? WARMER_MEDIUM_VOLTAGE_DUTY_CYCLE : WARMER_HIGH_VOLTAGE_DUTY_CYCLE);                        
                            }           
                        }
                    }
                    else
                    {
                        TIMER_RESET (x_extended_heating_warming_timer);
                        enm_delay_state = WARMER_STATE_IDLE;
                    }
                }
                else
                {
                    if (TIMER_ELAPSED (x_extended_heating_warming_timer) < pdMS_TO_TICKS (u32_extended_warming_ms))
                    {
                        v_WARMER_Control_Heater (WARMER_HEATER_OFF);
                    }
                    else
                    {
                        TIMER_RESET (x_extended_heating_warming_timer);
                        enm_delay_state = WARMER_STATE_HEATING;
                    }
                }               
            }

            /*
            ** If button ON is released, or heater is turned off via BLE,
            ** or the conditions for heating are not met, stop heating immediately
            */
            if ((b_btn_on_hold_short_released_main_screen ) || device_data.heater_off_requested || (!b_ready_for_heating)) // || (device_data.battery.charging == BATTERY_CHARGING))
            {
            	device_data.heater_enabled = false;
                WARMER_CHANGE_STATE (WARMER_STATE_IDLE);
                v_WARMER_Control_Heater (WARMER_HEATER_OFF);
                v_WARMER_Control_Motor (WARMER_MOTOR_OFF);
                // ui_post_update(UI_UPDATE_DONE_SCR,0);
                if (!b_ready_for_heating)
                {
                    LOGW ("EM: Stop heating because heating conditions are not met");
                    LOGW ("+ Bottle is attached: %s", g_stru_dev.b_bottle_attached ? "OK" : "False");
                    LOGW ("+ Warmer does not tilt: %s", g_stru_dev.b_tilted ? "False" : "OK");
                    // LOGW ("+ Battery is not charging: %s", g_stru_dev.b_charging ? "False" : "OK");
                    LOGW ("+ Battery is sufficient: %s",
                            (g_stru_dev.u16_bat_voltage > WARMER_HEATER_CUTOFF_VOLTAGE) ? "OK" : "False");
                    LOGW ("+ Error State E5: %s", g_stru_dev.b_ntc_high_err ? "False" : "OK");
                    LOGW ("+ Error State E4: %s", g_stru_dev.b_ntc_low_err ? "False" : "OK");
                    LOGW ("+ Error State E7: %s", g_stru_dev.b_wigg_seal_err ? "False" : "OK");
                    ui_post_update(UI_UPDATE_ERROR,device_data.err_code);
                        vTaskDelay(pdMS_TO_TICKS(20));
                }else{
                    ui_post_update(UI_UPDATE_DONE_SCR,0);
                    vTaskDelay(pdMS_TO_TICKS(20));
                }
            }


            break;
        }

        /* The warmer is shutting down before deep sleep */
        case WARMER_STATE_SHUTTING_DOWN:
        {
            /* State entry */
            if (g_stru_sm.u32_state_counter == 0)
            {
                LOGI (">>> WARMER_STATE_SHUTTING_DOWN");
                v_WARMER_Shut_Down ();
            }
            break;
        }
    }

    /* Update the state machine */
    g_stru_sm.u32_state_counter++;
}

static void v_WARMER_Handle_Errors(){

    // static TickType_t x_error_update_timer = 0;
    static uint8_t last_error = 0;   // invalid initial state, so first update always triggers
        
    if(isErrorUpdateRequired){

        if (g_stru_dev.b_tilted) {
            error = 1;
        } else if (!g_stru_dev.b_bottle_attached) {
            error = 2;
        } else if (g_stru_config.b_heater_failure) {
            error = 3;
        } else if (g_stru_dev.b_ntc_low_err) {
            error = 4;
        } else if (g_stru_dev.b_ntc_high_err) {
            error = 5;
        } else if (g_stru_dev.b_wigg_seal_err) {
            error = 7;
        } else if (g_stru_dev.u16_bat_voltage < WARMER_HEATER_CUTOFF_VOLTAGE || device_data.battery.state == BATTERY_LOW) {
            error = 25;
        } else {
            error = 0;
        }

        // Only update if error state changed
        if (error != last_error ) {
            if(error==2){
                app_notification_off();
                // vTaskDelay(pdMS_TO_TICKS(10));
            }
            isMeasuringTemp=false;
            ui_post_update(UI_UPDATE_ERROR, error);
            vTaskDelay(pdMS_TO_TICKS(50));
            last_error = error;
            device_data.err_code= error;
            
            // vTaskDelay (pdMS_TO_TICKS (1000)); 
        } 
    }
}

void v_WARMER_Get_Error(void){
    return error;   
 }

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Common process of Warmer device
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void v_WARMER_Process_Device (void)
{
    /* Shut down the warmer if requested so */
    if (device_data.power == PWR_OFF && device_data.battery.charging == BATTERY_NOT_CHARGING)
    {
        v_WARMER_Shut_Down ();
    }


    /* Enable battery for charging*/
    if (device_data.plug.plugdet==PLUG_DETECTED && is_ready_for_charging && !update_plugged_in_bat_percent){
            v_WARMER_Control_Battery(true);
    }
    else if((g_stru_dev.b_charging==BATTERY_NOT_CHARGING && g_stru_sm.enm_curr_state == WARMER_STATE_IDLE) || device_data.battery.state == BATTERY_FULL){
        v_WARMER_Control_Battery(false);
    }

    if(device_data.plug.plugdet==PLUG_DETECTED){
        g_stru_config.u8_Keep_warm_period = (g_stru_config.u8_Keep_warm_level == 0) ? 0 :
                                            (g_stru_config.u8_Keep_warm_level == 1) ? KEEP_WARM_PLUGGED_LEVEL_1 :
                                            (g_stru_config.u8_Keep_warm_level == 2) ? KEEP_WARM_PLUGGED_LEVEL_2 :
                                            (g_stru_config.u8_Keep_warm_level == 3) ? KEEP_WARM_PLUGGED_LEVEL_3 :
                                            (g_stru_config.u8_Keep_warm_level == 4) ? KEEP_WARM_PLUGGED_LEVEL_4 :
                                            (g_stru_config.u8_Keep_warm_level == 5) ? KEEP_WARM_PLUGGED_LEVEL_5 : 0;

    }else{
        g_stru_config.u8_Keep_warm_period = (g_stru_config.u8_Keep_warm_level == 0) ? 0 :
                                            (g_stru_config.u8_Keep_warm_level == 1) ? KEEP_WARM_LEVEL_1 :
                                            (g_stru_config.u8_Keep_warm_level == 2) ? KEEP_WARM_LEVEL_2 :
                                            (g_stru_config.u8_Keep_warm_level == 3) ? KEEP_WARM_LEVEL_3 :
                                            (g_stru_config.u8_Keep_warm_level == 4) ? KEEP_WARM_LEVEL_4 :
                                            (g_stru_config.u8_Keep_warm_level == 5) ? KEEP_WARM_LEVEL_5 : 0;
    }

    /* Disable sleep for OTA mode / Stop heating */
    if(device_data.ota_mode)
    {
       device_data.heater_enabled = false;
       WARMER_CHANGE_STATE (WARMER_STATE_IDLE);
       v_WARMER_Control_Heater (WARMER_HEATER_OFF);
       v_WARMER_Control_Motor (WARMER_MOTOR_OFF);
       vTaskDelay(pdMS_TO_TICKS(20));
       vTaskDelete(NULL);   // Self-delete, no handle required

    }

    if(isBoot){ //after first boot cycle
        isBoot=false;
    }
  
}

void v_WARMER_INC_TEMP(void){
    if (g_stru_config.u8_heater_mode == HEATER_M_FROZEN)
    {
        g_stru_config.u8_temp_setpoint = WARMER_FROZEN_SET_POINT_TEMP;
    }
    else
    {
        g_stru_config.u8_temp_setpoint = (g_stru_config.u8_temp_setpoint >= TEMPERATURE_SETTING_MAX) ?
                                            TEMPERATURE_SETTING_MIN : g_stru_config.u8_temp_setpoint + 1;
    }
    enm_WARMER_Save_Config ();
    ui_post_update(UI_UPDATE_SETTING_TEMP, 0); 
    vTaskDelay(pdMS_TO_TICKS(20));
    LOGI ("Temperature set point = %d °C", g_stru_config.u8_temp_setpoint);
}

void v_WARMER_DEC_TEMP(void){
    if (g_stru_config.u8_heater_mode == HEATER_M_FROZEN)
    {
        g_stru_config.u8_temp_setpoint = WARMER_FROZEN_SET_POINT_TEMP;
    }
    else
    {
        g_stru_config.u8_temp_setpoint = (g_stru_config.u8_temp_setpoint <= TEMPERATURE_SETTING_MIN) ? TEMPERATURE_SETTING_MAX : g_stru_config.u8_temp_setpoint - 1;
    }
    enm_WARMER_Save_Config ();
    ui_post_update(UI_UPDATE_SETTING_TEMP, 0); 
    vTaskDelay(pdMS_TO_TICKS(20));
    LOGI ("Temperature set point = %d °C", g_stru_config.u8_temp_setpoint);
}

WARMER_btn_event_t enm_WARMER_Get_Button_Event (void)
{
    /* Return button event */
    return g_enm_button_event;
}

WARMER_running_state_t enm_WARMER_Get_Running_State (void)
{
    static WARMER_running_state_t state = WARMER_READY;
	static WARMER_state_t running_state = WARMER_READY;
	
    static TickType_t x_idle_timer = 0;	

	if((running_state == g_stru_sm.enm_curr_state) && (TIMER_ELAPSED(x_idle_timer) > pdMS_TO_TICKS(1000)) )
	{
		
        state = WARMER_READY;
	}
	
	
    
    if((g_stru_sm.enm_prev_state == WARMER_STATE_MAINTAINING) && (g_stru_sm.enm_curr_state == WARMER_STATE_IDLE )&& (running_state != g_stru_sm.enm_curr_state))
    {	
    	running_state = g_stru_sm.enm_curr_state;
        state = WARMER_WARMING_DONE;
		TIMER_RESET (x_idle_timer);
        
    }
    else if((g_stru_sm.enm_prev_state == WARMER_STATE_EXTENDED_MAINTAINING) && (g_stru_sm.enm_curr_state == WARMER_STATE_IDLE )&& (running_state != g_stru_sm.enm_curr_state))
    {
	    running_state = g_stru_sm.enm_curr_state;
        state = WARMER_KEEPING_END;
		TIMER_RESET (x_idle_timer);
        
    }
    else if((g_stru_sm.enm_curr_state == WARMER_STATE_HEATING) && (running_state != g_stru_sm.enm_curr_state))
    {
        state = WARMER_HEATING;
		running_state = g_stru_sm.enm_curr_state;
		TIMER_RESET (x_idle_timer);
        
    }
    else if((g_stru_sm.enm_curr_state == WARMER_STATE_MAINTAINING) && (running_state != g_stru_sm.enm_curr_state))
    {
        state = WARMER_WARMING;
		running_state = g_stru_sm.enm_curr_state;
		TIMER_RESET (x_idle_timer);
      
    }
    else if((g_stru_sm.enm_curr_state == WARMER_STATE_EXTENDED_MAINTAINING) && (running_state != g_stru_sm.enm_curr_state))
    {
        state = WARMER_KEEPING;
		running_state = g_stru_sm.enm_curr_state;
		TIMER_RESET (x_idle_timer);
      
    }
    else
    {
		
    }

    return state;
}
/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Controls heater
**
** @param [in]
**      u8_power: power (in percents) of the heater, use 0 to turn off heater
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void v_WARMER_Control_Heater (uint8_t u8_power)
{
    static uint8_t u8_current = 0;

    /* Do nothing if control state is not changed */
    if (u8_power == u8_current)
    {
        return;
    }

    /* Control heater */
    u8_current = u8_power;
    if (u8_power != 0)
    {
        LOGW ("Heater ON");
#ifndef DEVELOPMENT_TEST_MODE
        app_heater_on (u8_current);
#endif
        v_WARMER_Control_Battery(true);
        device_data.heater_enabled = true;
    }
    else
    {
        LOGW ("Heater OFF");
        app_heater_off ();
        device_data.heater_enabled = false;
        v_WARMER_Control_Battery(false);
    }
}


/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Controls motor
**
** @param [in]
**      u8_power: power (in percents) of the motor, use 0 to turn off motor
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void v_WARMER_Control_Motor (uint8_t u8_power)
{
    static uint8_t u8_current = 0;

    /* Do nothing if control state is not changed */
    if (u8_power == u8_current)
    {
        return;
    }

    /* Control motor */
    u8_current = u8_power;
    if (u8_power != 0)
    {
        // LOGW ("Motor ON");
#ifndef DEVELOPMENT_TEST_MODE
        app_motor_on_pwm (u8_current);
#endif
         v_WARMER_Control_Battery(true);
        device_data.motor_enabled = true;
    }
    else
    {
        LOGW ("Motor OFF");
        app_motor_off ();
        current_pwm=WARMER_MOTOR_INITIAL_PWM;
        device_data.motor_enabled = false;
         v_WARMER_Control_Battery(false);
    }
}


/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Processes the events fired by UP and ON buttons
**
** @param [in]
**      pstru_evt_data: Context data of the event
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void v_WARNER_Button_Event_Handler (BTN_evt_data_t * pstru_evt_data)
{
    /* Pressing any keys prevent the warmer to go to sleep */
    TIMER_RESET (g_x_idle_timer);
    TIMER_RESET(g_x_charging_ui_timer);
    
    /* If a button is already active and this event is from another button, ignore it */
    if ((p_x_active_btn != NULL) && (p_x_active_btn != pstru_evt_data->x_inst))
    {
        // LOGW("Ignoring button event while another button is active");
        return;
    }

    /* Get current state */
    BTN_state_t new_state = pstru_evt_data->stru_state.enm_new_state;

    /* If this button was just pressed, mark it as active */
    if (new_state == BTN_STATE_PRESSED )
    {
        p_x_active_btn = pstru_evt_data->x_inst;
        
    }

    /* If this button is released, clear the active button */
    if (new_state == BTN_STATE_RELEASED)
    {
        p_x_active_btn = NULL;
    }

   

    /* Process events of UP button */
    if (pstru_evt_data->x_inst == g_x_btn_up_inst)
    {
            if(new_state == BTN_STATE_PRESSED){
                // ESP_LOGI(TAG, "Button UP Event: BTN_EVENT_UP_PRESSED");
                g_enm_button_event = BTN_EVENT_UP_PRESSED;
               
            }
            if(new_state == BTN_STATE_RELEASED){
                if(g_enm_button_event == BTN_EVENT_UP_PRESSED){
                    // ESP_LOGI(TAG, "Button UP Event: BTN_EVENT_UP_RELEASED");
                    g_enm_button_event = BTN_EVENT_UP_RELEASED;

                }else if(g_enm_button_event == BTN_EVENT_UP_HOLD_START){
                    // ESP_LOGW(TAG, "Button UP Event: BTN_EVENT_BACK");
                    g_enm_button_event = BTN_EVENT_BACK;
                    // back_to_prev_scr=true;
                    p_x_active_btn = NULL;
                }
                
            }

            if(new_state == BTN_STATE_HOLD_START){// && !back_to_prev_scr){
                g_enm_button_event = BTN_EVENT_UP_HOLD_START;
                // ESP_LOGI(TAG, "Button UP Event: BTN_EVENT_HOLD_START");
            }
            
    }
    else if (pstru_evt_data->x_inst == g_x_btn_th_inst ){
            if(new_state == BTN_STATE_PRESSED){
                // ESP_LOGI(TAG, "Button TH Event: BTN_EVENT_TH_PRESSED");
                g_enm_button_event = BTN_EVENT_TH_PRESSED;
                
            }
            if(new_state == BTN_STATE_RELEASED){            // && g_enm_button_event < BTN_EVENT_HOLD_START){
                if(g_enm_button_event == BTN_EVENT_TH_PRESSED){
                    // ESP_LOGI(TAG, "Button TH Event: BTN_EVENT_TH_RELEASED");
                    g_enm_button_event = BTN_EVENT_TH_RELEASED;
                }else if(g_enm_button_event == BTN_EVENT_TH_HOLD_START){
                    // ESP_LOGW(TAG, "Button TH Event: BTN_EVENT_BACK");
                    g_enm_button_event = BTN_EVENT_BACK;
                    // back_to_prev_scr=true;
                     p_x_active_btn = NULL;
                }
                
            }
            if(new_state == BTN_STATE_HOLD_START){// && !back_to_prev_scr){
                g_enm_button_event = BTN_EVENT_TH_HOLD_START;
                // ESP_LOGI(TAG, "Button TH Event: BTN_EVENT_HOLD_START");
            }

    }
     /* Process events of ON button */
    else if (pstru_evt_data->x_inst == g_x_btn_on_inst )
    {
            if(new_state == BTN_STATE_PRESSED){
                // ESP_LOGI(TAG, "Button ON Event: BTN_EVENT_ON_PRESSED");
                g_enm_button_event = BTN_EVENT_ON_PRESSED;
             
            }
            if(new_state == BTN_STATE_RELEASED){
                // LOGW("ON button released");
                if(g_enm_button_event == BTN_EVENT_ON_PRESSED){
                    // ESP_LOGI(TAG, "Button ON Event: BTN_EVENT_ON_RELEASED");
                    g_enm_button_event = BTN_EVENT_ON_RELEASED;
                }else if(g_enm_button_event == BTN_EVENT_ON_HOLD_START){
                    // ESP_LOGI(TAG, "Button ON Event: BTN_EVENT_BACK");
                    g_enm_button_event = BTN_EVENT_BACK;
                    //  back_to_prev_scr=true;
                     p_x_active_btn = NULL;
                }
               
            }
            if(new_state == BTN_STATE_HOLD_START){// && !back_to_prev_scr){
                g_enm_button_event = BTN_EVENT_ON_HOLD_START;
                // ESP_LOGI(TAG, "Button ON Event: BTN_EVENT_HOLD_START");
            }
    }
}

uint8_t u8_convert_ntc_To_Precent(float current_temp, uint8_t setting_temp)
{
    // to store initial temp once
    uint8_t percent = g_stru_wp.warming_curr_percent;        // to store last high percent

    if (g_stru_sm.enm_curr_state == WARMER_STATE_HEATING) 
    {   

            if(/*device_data.stateTemp == TEMP_REACH*/ temp_reach_counter!=0 ){

                 uint8_t new_percent = 90 + (temp_reach_counter * (5 / 5));  // 5% / 5checks
                 if (new_percent > percent) {
                    percent = new_percent;
                 }
            }else{

                if (g_stru_wp.start_temperature < -15.0f ) {
                    g_stru_wp.start_temperature = current_temp;
                    // LOGW("Set Start Temperature");
                }

                if ((percent == 0) && ((g_stru_wp.start_temperature - current_temp) >= 2.0f)){
                    g_stru_wp.start_temperature = current_temp;
                    // LOGW("Starting Temperature Recalibrated");
                }

                float range = setting_temp - g_stru_wp.start_temperature;
                if (range > 0.0f) {
                    int8_t new_percent = ((current_temp - g_stru_wp.start_temperature) * 90.0f) / range;
                    if (new_percent < 0) new_percent = 0;
                    if (new_percent > 90) new_percent = 90; // cap at 90
                    if (new_percent > percent) {
                        percent = new_percent;
                    }
                }
            }
           
    }else if(g_stru_sm.enm_curr_state == WARMER_STATE_MAINTAINING && g_stru_wp.b_start_maintaining_percent){


        uint32_t progress = (TIMER_ELAPSED(x_stable_timer) * 5) / pdMS_TO_TICKS( g_stru_wp.u32_maintaining_ms); // 5%
        // if (progress > 4) progress = 4;

        uint8_t new_percent = 95 + (uint8_t)progress;
        if (new_percent > 100) new_percent = 100;
        if (new_percent > percent) {
            percent = new_percent;
        }

    }


    return percent;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Low Voltage indicator
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
void Low_Voltage_Indicator(void)
{
    if(( g_stru_config.b_low_bat_alert == true) && (g_stru_dev.u16_bat_voltage >= WARMER_HEATER_NORMAL_VOLTAGE))
    {
        // g_low_voltage_alert = false;
        g_stru_config.b_low_bat_alert = false;
        enm_WARMER_Save_Config();
        
    }  
}


void Sleep_Cut_Disable(void){
     /* Get instance for SLEEPCUT_AUX */
   
        if (s8_GPIO_Get_Inst (GPIO_SLEEP_AUX, &g_x_sleep_cut_aux_inst) != GPIO_OK)
        {
            LOGE ("Failed to get instance handle of SLEEPCUT_AUX pin");
            return GPIO_ERR; //error
        }
        else
        {
            s8_GPIO_Write_Level(g_x_sleep_cut_aux_inst, 0);  
        }	
        // LOGW ("Sleep Cut Disabled");
    
}

void Sleep_Cut_Enable(void){
     /* Get instance for SLEEPCUT_AUX */
    if (s8_GPIO_Get_Inst (GPIO_SLEEP_AUX, &g_x_sleep_cut_aux_inst) != GPIO_OK)
    {
        LOGE ("Failed to get instance handle of SLEEPCUT_AUX pin");
        return GPIO_ERR; //error
    }
    else
    {
     	s8_GPIO_Write_Level(g_x_sleep_cut_aux_inst, 1);  
    }	
}

static void v_vBat_enable(void){
 /* Enable */
    
    if (s8_GPIO_Get_Inst (GPIO_BAT_EN, &x_bat_en_inst) != GPIO_OK)
    {
        LOGE ("Failed to get instance handle of battery enable pin");
        return GPIO_ERR; //error
    }
    else
    {
     	s8_GPIO_Write_Level(x_bat_en_inst, 1); // 1=enable 0=disable
    }

}

 void v_vBat_disable(void){
 /* Enable */
    
    if (s8_GPIO_Get_Inst (GPIO_BAT_EN, &x_bat_en_inst) != GPIO_OK)
    {
        LOGE ("Failed to get instance handle of battery enable pin");
        return GPIO_ERR; //error
    }
    else
    {
     	s8_GPIO_Write_Level(x_bat_en_inst, 0); // 1=enable 0=disable
    }

}

static void v_WARMER_Control_Battery(bool b_enable){

    static bool en = false;
    // Only update if new state is different
    if (en != b_enable) {
        en = b_enable;  // update stored state

        if (b_enable) {
            v_vBat_enable();
        } else {
            v_vBat_disable();
        }
    }

}




/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Interrupt service routine invoked when the bottle is attached or detached from the warmer
**
** @param [in]
**      pstru_evt_data: Context data of the event
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void v_WARMER_Bottle_Detach_Isr (GPIO_evt_data_t * pstru_evt_data)
{
    if (pstru_evt_data->enm_evt == GPIO_EVT_EDGE_DETECTED)
    {
        // LOGE("SW EDGE DETECTED");
        /* If the bottle is detached from the warmer, turn off the heater immediately */
        if (!pstru_evt_data->stru_state.b_active)
        {
            app_heater_off ();
            s8_GPIO_Read_Active (g_x_bottle_inst, &g_stru_dev.b_bottle_attached);
            #ifndef SIMULATED_INPUT_MODE
            device_data.cover_input = g_stru_dev.b_bottle_attached ? COVER_LOW : COVER_HIGH;
            #endif
        }

        /* Notify warmer manager */
        // xEventGroupSetBits (g_x_event_group, WARMER_BOTTLE_EVT);
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xEventGroupSetBitsFromISR(g_x_event_group, WARMER_BOTTLE_EVT, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    }

}


/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Callback function invoked when the timer protecting the Warmer from unexpected working conditions expires
**
** @param [in]
**      x_timer: Handle of the timer
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void v_WARMER_Guardian_Timer_Cb (TimerHandle_t x_timer)
{
    /* Ensure that the heater is turned off while there is no bottle attached to the Warmer */
    bool b_bottle_attached;
    s8_GPIO_Read_Active (g_x_bottle_inst, &b_bottle_attached);
    if (!b_bottle_attached)
    {
        app_heater_off ();
        // app_notification_off();
        #ifndef SIMULATED_INPUT_MODE
        device_data.cover_input = b_bottle_attached ? COVER_LOW : COVER_HIGH;
        #endif
    }
}
/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Displays operational information on the console for debugging purpose
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void v_WARMER_Print_Info (void)
{
    LOGI ("********** Instantaneous data & status **********");
    LOGI ("\t+ Temp setting       : %d°C", g_stru_config.u8_temp_setpoint);
    LOGI ("\t+ Temp measurement   : %.1f°C", g_stru_dev.flt_temperature);
    LOGI ("\t+ Warming Percent    : %d%%", device_data.warming_percent);
    // LOGI ("\t+ Wigg Mode          : %s", g_stru_config.b_wigg_mode ? "Enabled" : "Disabled");
    LOGI ("\t+ Heating status     : %s", (device_data.stateTemp == TEMP_NOT_REACH) ? "Not reached" : "Reached");
    LOGI ("\t+ Heater mode        : %s", (g_stru_config.u8_heater_mode == HEATER_M_FROZEN) ? "Frozen" :
                                         (g_stru_config.u8_heater_mode == HEATER_M_MILK) ? "Milk" :
                                         (g_stru_config.u8_heater_mode == HEATER_M_WATER) ?"Water" : "BABYFOOD");
                                         
    LOGI ("\t+ Keep Warm Period   : %d minute", g_stru_config.u8_Keep_warm_period);
    
    LOGI ("\t+ Bottle Size        : %s", (g_stru_config.enm_volume_level == WARMER_VOLUME_120)        ? "Small" :
									     (g_stru_config.enm_volume_level == WARMER_VOLUME_240)        ? "Large" : "Not Available");

    LOGI ("\t+ Heater state       : %s", device_data.heater_enabled ? "On" : "Off");
    LOGI ("\t+ Motor state        : %s", device_data.motor_enabled ? "On" : "Off");
    LOGI ("\t+ Motor RPM          : %.1f", g_stru_dev.motor_rpm);
    LOGI ("\t+ Motor PWM          : %d", (uint16_t)current_pwm);
    LOGI ("\t+ Warmer State       : %s",
									    (g_stru_sm.enm_curr_state == WARMER_STATE_AWAKING)            ? "Awaking" :
									    (g_stru_sm.enm_curr_state == WARMER_STATE_IDLE)               ? "Idle" :
									    (g_stru_sm.enm_curr_state == WARMER_STATE_HEATING)            ? "Heating" :
									    (g_stru_sm.enm_curr_state == WARMER_STATE_MAINTAINING)        ? "Maintaining" :
									    (g_stru_sm.enm_curr_state == WARMER_STATE_EXTENDED_MAINTAINING) ? "Extended Maintaining" :
									    (g_stru_sm.enm_curr_state == WARMER_STATE_SHUTTING_DOWN)      ? "Shutting Down" : "Not Available");
    LOGI ("\t+ Is tilted          : %s", (device_data.tilt_input == TILT_LOW) ? "Yes" : "No");
    LOGI ("\t+ Is bottle attached : %s", (device_data.cover_input == COVER_LOW) ? "Yes" : "No");
    LOGI ("\t+ Error Code         : %s", device_data.err_code==0                     ? "NONE" :
                                         device_data.err_code==1                     ? "E-1"  :
                                         device_data.err_code==2                     ? "E-2"  :
                                         device_data.err_code==3                     ? "E-3"  :
                                         device_data.err_code==4                     ? "E-4"  :
                                         device_data.err_code==5                     ? "E-5"  :
                                         device_data.err_code==7                     ? "E-7"  :
                                         device_data.err_code==25                    ? "E-25" : "-1");
    LOGI ("\t+ Plug Detected      : %s", (device_data.plug.plugdet) ? "Yes" : "No");
    // LOGI ("\t+ Plug Status        : %s", (device_data.plug.status == PLUG_INVALID) ? "Plug Invalid" :
    //                                      (device_data.plug.status == PLUG_VALID) ? "Plug Valid" : "Unknown");
    // LOGI ("\t+ Plug voltage       : %.2fV", device_data.plug.miliVolts / 1000.0);   
    LOGI ("\t+ Plug current       : %dmA",  device_data.plug.miliAmps);   
    LOGI ("\t+ Battery enabled    : %s", (device_data.battery.enable == BATTERY_ENABLE) ? "Yes" : "No");
	LOGI ("\t+ Battery charging   : %s", (device_data.battery.charging == BATTERY_CHARGING) ? "Yes" : "No");
    // LOGI ("\t+ Bat~Plug voltage   : %.2fV", g_stru_dev.u16_bat_voltage / 1000.0);
    LOGI ("\t+ Battery voltage    : %.2fV", device_data.battery.miliVolts / 1000.0);
    // LOGI ("\t+ Bat %% real         : %d%%", g_stru_dev.u8_bat_percent);
    LOGI ("\t+ Battery percent    : %d%%", device_data.battery.percent);
    LOGI ("\t+ Battery state      : %s", (device_data.battery.state == BATTERY_NORMAL) ? "Normal" :
                                         (device_data.battery.state == BATTERY_LOW) ? "Low" : "Full");
    LOGI ("\t+ Vibration status   : %s", g_stru_config.b_vibration_enabled ? "Enabled" : "Disabled");
    LOGI ("\t+ Buzzer status      : %s", g_stru_config.b_buzzer_enabled ? "Enabled" : "Disabled");
    LOGI ("\t+ Buzzer volume      : %s", !g_stru_config.b_buzzer_enabled                     ? "LEVEL 0" :
										 (g_stru_config.enm_buzzer_volume == BUZZER_VOLUME_LEVEL_1) ? "LEVEL 1" :
										 (g_stru_config.enm_buzzer_volume == BUZZER_VOLUME_LEVEL_2) ? "LEVEL 2" :
										 (g_stru_config.enm_buzzer_volume == BUZZER_VOLUME_LEVEL_3) ? "LEVEL 3" :
										 (g_stru_config.enm_buzzer_volume == BUZZER_VOLUME_LEVEL_4) ? "LEVEL 4" :
										 (g_stru_config.enm_buzzer_volume == BUZZER_VOLUME_LEVEL_5) ? "LEVEL 5" : "Not Available");
    LOGI ("\t+ Device Language    : %s", (g_stru_config.enm_device_language == DEVICE_LANGUAGE_EN_US) ? "English"  :
                                         (g_stru_config.enm_device_language == DEVICE_LANGUAGE_ZH_SG) ? "Chinese"  :
                                         (g_stru_config.enm_device_language == DEVICE_LANGUAGE_JA_JP) ? "Japanese" :
                                         (g_stru_config.enm_device_language == DEVICE_LANGUAGE_KO_KR) ? "Korean"   : "Not Available");      
                                         
    // ESP_LOGI("HEAP", "Free Heap = %d", esp_get_free_heap_size());   
    // heap_caps_print_heap_info(MALLOC_CAP_8BIT);                                      
    LOGI ("*************************************************");
}

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
