/*
 * app_heater.h
 *
 *  Created on: Mar 28, 2022
 *      Author: Teppy
 */

#ifndef COMMON_APP_HEATER_H_
#define COMMON_APP_HEATER_H_

#include "app_cfg.h"

void app_heater_init (void);

void app_heater_off (void);

void app_heater_on (uint8_t u8_power_in_percents);

#endif /* COMMON_APP_HEATER_H_ */
