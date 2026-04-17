/*
 * app_notification.h
 *
 *  Created on: Sept 20, 2025
 *  Author: Hassam Bin Hassan
 */

#ifndef COMMON_APP_NOTIFICATION_H_
#define COMMON_APP_NOTIFICATION_H_

#include "app_buzzer.h"
#include "app_vibrator.h"

void app_notification_init( void );

void app_notification_off( void );

void app_notify_500ms( void );

void app_notify_200ms( void );

void app_notify_200ms_double( void );

void app_notify_500ms_double( void );

void app_notify_soft( void );

void app_notify_play_done( void );


#endif /* COMMON_APP_NOTIFICATION_H_ */
