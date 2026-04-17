/*
 *  app_notification.c
 *
 *  Created on: Sept 20, 2025
*   Author: Hassam Bin Hassan
 */

#include "app_notification.h"

void app_notification_init( void )
{
    app_buzzer_init();
    app_vibrator_init();
}

void app_notification_off( void )
{
    app_buzzer_off();
    app_vibrator_off();
}
void app_notify_500ms( void )
{
    app_buzzer_500ms_beep();
    app_vibrator_500ms_buzz();
}

void app_notify_200ms( void )
{
    app_buzzer_200ms_beep();
    app_vibrator_200ms_buzz();
}

void app_notify_200ms_double( void )
{
    app_buzzer_200ms_double_beep();
    app_vibrator_200ms_double_buzz();
}

void app_notify_500ms_double( void )
{
    app_buzzer_500ms_double_beep();
    app_vibrator_500ms_double_buzz();
}

void app_notify_soft( void )
{
    app_buzzer_soft_beep();
    app_vibrator_soft_buzz();
}

void app_notify_play_done( void )
{
    app_buzzer_play_song();
    app_vibrator_play_song();
}

