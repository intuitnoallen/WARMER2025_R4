/*
 * app_motor.h
 *
 *  Created on: Sept 09, 2025
 *  Author: Hassam Bin Hassan
 */

#ifndef COMMON_APP_MOTOR_H_
#define COMMON_APP_MOTOR_H_

#include "app_cfg.h"

void app_motor_init (void);

void app_motor_off (void);

void app_motor_on_per (uint8_t u8_power_in_percents);

void app_motor_on_pwm(uint8_t u8_power_in_pwm_raw);

#endif /* COMMON_APP_MOTOR_H_ */
