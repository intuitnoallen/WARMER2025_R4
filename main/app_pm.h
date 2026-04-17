/*
 * esp_pm.h
 *
 *  Created on: Sep 15, 2021
 *      Author: thaob
 */

#ifndef MAIN_ESP_INTERNAL_APP_PM_H_
#define MAIN_ESP_INTERNAL_APP_PM_H_

#include "app_cfg.h"
#include "driver/rtc_io.h"

#define PIN_WAKEUP              BTN_ON_PIN
#define WAKEUP_LEVEL            1

void app_pm_config( void );

void app_pm_light_sleep( void );

void app_pm_deep_sleep( void );

#endif /* MAIN_ESP_INTERNAL_APP_PM_H_ */
