/*
    * app_vibrator.c
    *
    *  Created on: Sept 21 2025
    * Author: Hassam Bin Hassan
*/
#include "app_cfg.h"
// Pattern entry: { state (1 = ON, 0 = OFF), duration in ms }
typedef struct {
    uint8_t state;      // 1 = ON, 0 = OFF
    uint32_t duration; // milliseconds
} vib_pattern_t;

void app_vibrator_init( void );

void app_vibrator_off( void );

void app_vibrator_500ms_buzz( void );

void app_vibrator_200ms_buzz( void );

void app_vibrator_200ms_double_buzz( void );

void app_vibrator_500ms_double_buzz( void );

void app_vibrator_soft_buzz( void );

void app_vibrator_play_song( void );