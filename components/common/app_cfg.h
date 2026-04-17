/*
 * app_cfg.h
 *
 *  Created on: Jan 23, 2022
 *      Author: thaob
 */

#ifndef MAIN_APP_CFG_H_
#define MAIN_APP_CFG_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_task_wdt.h"
#include "esp_log.h"
#include "esp_system.h"
#include "driver/gpio.h"

#define DEVICE_NAME                         "WIGG"
#define DEVICE_HARDWARE_VERSION              0x04


/*
** Uncomment this option to enable development test mode:
** + Heater component is never turned on regardless the control state
** + Thermistor temperature is simulated
*/
//#define DEVELOPMENT_TEST_MODE
#ifdef DEVELOPMENT_TEST_MODE
    #warning ************ Development test mode is ON ************
#endif

#define DEVICE_SERIAL_NUM_MAX_LEN           32

#define APP_DEBUG(tag, msg,...)             printf("---> [%s-%d]: " msg, tag, __LINE__, ##__VA_ARGS__)
#define APP_DEBUG_RAW                       printf
#define APP_DEBUG_E(tag, msg,...)           printf("\033[0;31m---> [%s-%d]: " msg "\033[0m", tag, __LINE__, ##__VA_ARGS__)
#define APP_DEBUG_W(tag, msg,...)           printf("\033[0;33m---> [%s-%d]: " msg "\033[0m", tag, __LINE__, ##__VA_ARGS__)
#define APP_DEBUG_I(tag, msg,...)           printf("\033[0;32m---> [%s-%d]: " msg "\033[0m", tag, __LINE__, ##__VA_ARGS__)
#define APP_DEBUG_RAW_I(msg,...)            printf("\033[0;32m" msg "\033[0m", ##__VA_ARGS__)

#define APP_DELAY_MS(ms)                    vTaskDelay(ms / portTICK_PERIOD_MS)
#define APP_GET_TICK_MS()                   (esp_timer_get_time() / 1000)

//IO
#define GPIO_GET_INPUT(pin)                 gpio_get_level(pin)
#define GPIO_OUTPUT_LOW(pin)                gpio_set_level(pin, 0)
#define GPIO_OUTPUT_HIGH(pin)               gpio_set_level(pin, 1)


//pinout
#define BTN_ON_PIN                          34 //GPIO_NUM_5        :UP34/ON35 //old UP5 ON4
#define BTN_BOTTLE_SENSOR_PIN               GPIO_NUM_19//GPIO_NUM_6

//Heater
#define HEATER_PIN                          GPIO_NUM_25//GPIO_NUM_0
#define HEATER_PWM_CHANNEL                  LEDC_CHANNEL_0  //LEDC LOW SPEED
#define HEATER_PWM_TIMER                    LEDC_TIMER_1

//Encoder
#define ENCODER_A_PIN                       GPIO_NUM_13
#define ENCODER_B_PIN                       GPIO_NUM_15


//Motor
#define MOTOR_PIN                          GPIO_NUM_9//GPIO_NUM_0
#define MOTOR_PWM_CHANNEL                  LEDC_CHANNEL_0   //LEDC_HIGH_SPEED
#define MOTOR_PWM_TIMER                    LEDC_TIMER_0     //LEDC_HIGH_SPEED

//Buzzer
#define BUZZER_PIN                          GPIO_NUM_33//GPIO_NUM_10
#define BUZZER_PWM_CHANNEL                  LEDC_CHANNEL_1
#define BUZZER_PWM_TIMER                    LEDC_TIMER_2
#define BUZZER_RMT_TX_CHANNEL               RMT_CHANNEL_0

//led 7 segment
#define LED_7_SEG_SDA_PIN                  GPIO_NUM_18
#define LED_7_SEG_SCK_PIN                  GPIO_NUM_19

typedef enum
{
    LED_CODE_E1,    //tilt
    LED_CODE_E2,    //cover
    LED_CODE_E3,    //timeout
    LED_CODE_E4,    // -15
    LED_CODE_E5,    // 60
    LED_CODE_BL,
    LED_CODE_BF,
    LED_CODE_CL,
    LED_CODE_B,
    LED_CODE_M,     /* maintain Temperature */
    LED_CODE_MAX,
}led_code_t;

#define TEMP_C_TO_F(temp)                   (((float)temp * 9 / 5) + 32)
#define TEMP_F_TO_C(temp)                   (((float)temp - 32) * 5 / 9)

typedef enum
{
    PWR_OFF,
    PWR_ON,
}power_state_t;

typedef enum
{
    COVER_LOW,
    COVER_HIGH, //error
}cover_detect_t;

typedef enum
{
    TILT_LOW, //error
    TILT_HIGH,
}tilt_input_t;

typedef enum
{
    NTC_TEMP_ERR_NONE,
    NTC_TEMP_LOW_ERR,       //error <-15C degree
    NTC_TEMP_HIGH_ERR,      //error >60C degree
}ntc_temp_err_t;

typedef enum
{
    HEATER_M_MILK,
    HEATER_M_WATER,
    HEATER_M_FROZEN,
    HEATER_M_BABYFOOD,
}heater_mode_t;

typedef enum
{
    BUZZER_VOLUME_LEVEL_1 = 30,   //!< Volume level 1
    BUZZER_VOLUME_LEVEL_2 = 40,   //!< Volume level 2
    BUZZER_VOLUME_LEVEL_3 = 50,   //!< Volume level 3
    BUZZER_VOLUME_LEVEL_4 = 60,   //!< Volume level 4
    BUZZER_VOLUME_LEVEL_5 = 100,  //!< Volume level 5
} BUZZER_volume_t;

typedef enum
{
    HEATER_PWR_P_LEVEL_1 = 20,   //!< Heater Power Percent level 1
    HEATER_PWR_P_LEVEL_2 = 40,   //!< Heater Power Percent level 2
    HEATER_PWR_P_LEVEL_3 = 60,   //!< Heater Power Percent level 3
    HEATER_PWR_P_LEVEL_4 = 80,   //!< Heater Power Percent level 4
    HEATER_PWR_P_LEVEL_5 = 100,  //!< Heater Power Percent level 5
} HEATER_PWR_P_t;

typedef enum
{
    KEEP_WARM_LEVEL_1 = 1,   //!< KEEP_WARM no plug level 1
    KEEP_WARM_LEVEL_2 = 2,   //!< KEEP_WARM no plug level 2
    KEEP_WARM_LEVEL_3 = 3,   //!< KEEP_WARM no plug level 3
    KEEP_WARM_LEVEL_4 = 4,   //!< KEEP_WARM no plug level 4
    KEEP_WARM_LEVEL_5 = 5,   //!< KEEP_WARM no plug level 5
} KEEP_WARM_lvl_t;

typedef enum
{
    KEEP_WARM_PLUGGED_LEVEL_1 = 1,   //!< KEEP_WARM no plug level 1
    KEEP_WARM_PLUGGED_LEVEL_2 = 5,   //!< KEEP_WARM no plug level 2
    KEEP_WARM_PLUGGED_LEVEL_3 = 10,   //!< KEEP_WARM no plug level 3
    KEEP_WARM_PLUGGED_LEVEL_4 = 15,   //!< KEEP_WARM no plug level 4
    KEEP_WARM_PLUGGED_LEVEL_5 = 30,   //!< KEEP_WARM no plug level 5
} KEEP_WARM_plugged_lvl_t;

typedef enum
{
    TEMP_NOT_REACH = 0x00,
    TEMP_REACH = 0x01,
}state_temperature_t;

typedef enum
{
    TEMP_UNIT_C = 0,
    TEMP_UNIT_F = 1,
}temp_unit_t;

typedef enum
{
    BATTERY_NORMAL,
    BATTERY_LOW,
    BATTERY_FULL,
}battery_state_t;

typedef enum
{
    PLUG_VALID,
    PLUG_INVALID,
} plug_status_t;

typedef enum
{
    BATTERY_NOT_CHARGING,
    BATTERY_CHARGING,
}battery_charging_t;

typedef enum
{
    BATTERY_DISABLE,
    BATTERY_ENABLE,
}is_bat_enable_t;

typedef enum
{
    PLUG_NOT_DETECTED,
    PLUG_DETECTED,
}plug_detect_t;

typedef enum
{
    DEVICE_LANGUAGE_EN_US = 0,   //!< en-US
    DEVICE_LANGUAGE_ZH_SG = 1,   //!< zh-SG
    DEVICE_LANGUAGE_JA_JP = 2,   //!< ja-JP
    DEVICE_LANGUAGE_KO_KR = 3,   //!< ko-KR
} DEVICE_language_t;

typedef struct
{
    is_bat_enable_t     enable;
    battery_charging_t  charging;
    uint16_t            miliVolts;
    uint8_t             percent;
    battery_state_t     state;
    bool				ready;
}battery_t;

typedef struct
{
    plug_detect_t       plugdet;
    uint16_t            miliVolts;
    uint16_t            miliAmps;
    plug_status_t        status;
    bool				ready;
}plug_t;


typedef struct
{
    char                    device_Id[DEVICE_SERIAL_NUM_MAX_LEN];
    uint8_t                 ble_mac[6];
    float                   ntcTemp_C;
    state_temperature_t     stateTemp;
    uint8_t                 warming_percent;
    power_state_t           power;
    bool                    heater_enabled;
    bool                    motor_enabled;
    bool                    heater_off_requested;
    bool                    ota_mode;
    cover_detect_t          cover_input;
    tilt_input_t            tilt_input;
    ntc_temp_err_t          ntc_temp_err;
    uint8_t                 err_code;
    battery_t               battery;
    plug_t                  plug;
}device_data_t;

#define HOLD_START_TIME     700
#define HOLD_UI_SAFE_TIME   100

#define HOLD_ON_SHORT_TIME  2200
#define HOLD_UP_SHORT_TIME  3700
#define HOLD_TH_SHORT_TIME  3700

#define HOLD_ON_LONG_TIME   5700
#define HOLD_UP_LONG_TIME   5700
#define HOLD_TH_LONG_TIME   5700

extern device_data_t device_data;

#endif /* MAIN_APP_CFG_H_ */
