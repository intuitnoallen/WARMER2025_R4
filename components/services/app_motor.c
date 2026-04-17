/*
 * app_motor.c
 *
 *  Created on: Sept 09, 2025
 *  Author: Hassam Bin Hassan
 */

#include "driver/ledc.h"
#include "app_motor.h"

#define MOTOR_DUTY_RESOLUTION                 LEDC_TIMER_10_BIT     // LEDC_TIMER_10_BIT

static ledc_channel_config_t motor_channel = {
    .channel    = MOTOR_PWM_CHANNEL,
    .duty       = 0,
    .gpio_num   = MOTOR_PIN,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .hpoint     = 0,
    .timer_sel  = MOTOR_PWM_TIMER
};

void app_motor_init (void)
{
    ledc_timer_config_t motor_timer =
    {
        .duty_resolution = MOTOR_DUTY_RESOLUTION,  // resolution of PWM duty
        .freq_hz = 20000,                       // frequency of PWM signal 20Khz
        .speed_mode = motor_channel.speed_mode,// timer mode
        .timer_num = motor_channel.timer_sel,  // timer index
        .clk_cfg = LEDC_AUTO_CLK,               // Auto select the source clock
    };

    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&motor_timer);

    // Set LED Controller with previously prepared configuration
    ledc_channel_config(&motor_channel);

    // Initialize fade service.
    ledc_fade_func_install(0);

    // Turn off heater
    app_motor_off();
}


void app_motor_off (void)
{
    ledc_set_duty(motor_channel.speed_mode, motor_channel.channel, 0);
    ledc_update_duty(motor_channel.speed_mode, motor_channel.channel);
}


void app_motor_on_per (uint8_t u8_power_in_percents)
{
    uint32_t current_duty = ledc_get_duty (motor_channel.speed_mode, motor_channel.channel);
    uint32_t motor_duty = 2 * (1 << (MOTOR_DUTY_RESOLUTION - 1)) * u8_power_in_percents / 100;
    if (motor_duty != current_duty)
    {
        ledc_set_duty (motor_channel.speed_mode, motor_channel.channel, motor_duty);
        ledc_update_duty (motor_channel.speed_mode, motor_channel.channel);
    }
}

void app_motor_on_pwm(uint8_t u8_power_in_pwm_raw)
{
    uint32_t current_duty = ledc_get_duty(motor_channel.speed_mode, motor_channel.channel);
    uint32_t max_duty = (1 << MOTOR_DUTY_RESOLUTION) - 1;
    uint32_t motor_duty = ((uint32_t)u8_power_in_pwm_raw * max_duty) / 255;

    if (motor_duty != current_duty)
    {
        ledc_set_duty(motor_channel.speed_mode, motor_channel.channel, motor_duty);
        ledc_update_duty(motor_channel.speed_mode, motor_channel.channel);
    }
}
