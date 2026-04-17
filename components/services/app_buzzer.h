/*
 * app_buzzer.h
 *
 *  Created on: Mar 28, 2022
 *      Author: Teppy
 */

#ifndef COMMON_APP_BUZZER_H_
#define COMMON_APP_BUZZER_H_

#include "app_cfg.h"

typedef struct {
    uint32_t freq;
    uint32_t duration; // ms
}note;

void app_buzzer_init( void );

void app_buzzer_off( void );

void app_buzzer_500ms_beep( void );

void app_buzzer_200ms_beep( void );

void app_buzzer_200ms_double_beep( void );

void app_buzzer_500ms_double_beep( void );

void app_buzzer_soft_beep( void );

void app_buzzer_play_song( void );

void app_buzzer_update_volume( void );

#endif /* COMMON_APP_BUZZER_H_ */
