/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : app_main.c
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 Sep 26
**  @brief      : Implementation of App_Main module
**  @namespace  : MAIN
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  App_Main
** @brief       App_Main module is the entry point of Warmer firmware. It initializes and starts other modules
**              of the firmware.
** @{
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           INCLUDES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include "app_main.h"               /* Public header of this module */
#include "app_warmer_mngr.h"        /* Main controller of the warmer device */
#include "srvc_param.h"             /* Use parameter service */

#include "esp_event.h"              /* Use ESP event APIs */
#include "freertos/FreeRTOS.h"

#include "app_notification.h"
#include "app_heater.h"
#include "encoder.h"
#include "app_motor.h"

#include "app_pm.h"
#include "app_ble.h"
#include "app_ble_ota.h"           /* Use BLE OTA */
#include "lcd_task.h"
#include "esp_sleep.h"
/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           DEFINES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           VARIABLES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Logging tag of this module */
static const char * TAG = "App_Main";

/** @brief  Global variable shared across modules */
device_data_t device_data =
{
    .device_Id              = "",
    .ntcTemp_C              = 0.0f,
    .stateTemp              = TEMP_NOT_REACH,
    .warming_percent        = 0,
    .power                  = PWR_ON,
    .heater_enabled         = false,
    .motor_enabled          = false,
    .heater_off_requested   = false,
    .ota_mode               = false,
    .cover_input            = COVER_LOW,
    .tilt_input             = TILT_HIGH,
    .ntc_temp_err           = NTC_TEMP_ERR_NONE,
    .err_code               = 0,
    .battery                = {
        .enable             = BATTERY_DISABLE,
        .charging           = BATTERY_NOT_CHARGING,
        .miliVolts          = 0,
        .percent            = 0,
        .state              = BATTERY_NORMAL,
        .ready              = false,
    },
    .plug                   = {
        .plugdet            = PLUG_NOT_DETECTED,
        .miliVolts          = 0,
        .miliAmps           = 0,
        .status             = PLUG_INVALID,
        .ready              = false,
    },
};

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           PROTOTYPES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

static void v_MAIN_Init (void);

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           FUNCTIONS SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Entry point of Vending machine firmware
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
void app_main (void)
{
        //    // Configure GPIO19 COver Detect as input
        // gpio_config_t cover_conf = {
        //     .pin_bit_mask = 1ULL << GPIO_NUM_19,
        //     .mode = GPIO_MODE_INPUT,
        //     .pull_up_en = GPIO_PULLUP_ENABLE,     
        //     .pull_down_en = GPIO_PULLDOWN_DISABLE 
        // };
        // gpio_config(&cover_conf);
  

    // if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT1) {

    //     if (gpio_get_level(GPIO_NUM_19) == 1) {
    //         printf("E: Cover Not Detetcted, Warmer will not turn ON\n");    // Cover not present → go back to sleep
    //         v_WARMER_Sleep();
    //     }else{
    //         printf("I: Power Button Pressed → System ON\n");
    //     }
    // }

    // if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0){

            // printf("I: Plug Detected → System ON\n");
    // }

    // gpio_reset_pin(GPIO_NUM_19);
    rtc_gpio_hold_dis(GPIO_NUM_26); 
    gpio_reset_pin(GPIO_NUM_26);

    
    ESP_LOGI (TAG, "***********************");
    ESP_LOGI (TAG, "Warmer firmware started");
    ESP_LOGI (TAG, "***********************");

    /* Initialize modules */
    v_MAIN_Init ();
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Initilizes modules for the firmware to work
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void v_MAIN_Init (void)
{
    _LOGW("Main Init");
    // Watchdog Init
    ESP_ERROR_CHECK(esp_task_wdt_init(1, true));

    /* Turn off logging of GPIO module so that it won't flush the console when input/output mode of pins change */
    esp_log_level_set ("gpio", ESP_LOG_NONE);

    /* Initialize Parameter service */
    if (s8_PARAM_Init () != PARAM_OK)
    {
        LOGE ("Failed to initialize Parameter service");
        return;
    }

    /* Create default event loop */
    ESP_ERROR_CHECK (esp_event_loop_create_default ());

    _LOGW("Enabling Warmer");
    enm_WARMER_Init ();
    app_notification_init();
    app_motor_init();
    app_heater_init();
    
    _LOGW("Enabled Warmer");
   
    _LOGW("Enabling LCD");
    lcd_init();    
    _LOGW("Enabled LCD");

    
    _LOGW("Enabling BLE");
    vTaskDelay (pdMS_TO_TICKS (500));
    app_ble_init();     //ble init enable. gap gatts register events
    app_ble_start();    //app profile register
    _LOGW("Enabled BLE");
    
}
