/*
 * app_heater.c
 *
 *  Created on: Mar 28, 2022
 *      Author: Teppy
 */
#include "driver/ledc.h"
#include "app_heater.h"

#define HEATER_DUTY_RESOLUTION                  LEDC_TIMER_12_BIT

static ledc_channel_config_t heater_channel = {
    .channel    = HEATER_PWM_CHANNEL,
    .duty       = 0,
    .gpio_num   = HEATER_PIN,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .hpoint     = 0,
    .timer_sel  = HEATER_PWM_TIMER
};

void app_heater_init (void)
{
    ledc_timer_config_t heater_timer =
    {
        .duty_resolution = HEATER_DUTY_RESOLUTION,  // resolution of PWM duty
        .freq_hz = 1000,                       // frequency of PWM signal 1Khz
        .speed_mode = heater_channel.speed_mode,// timer mode
        .timer_num = heater_channel.timer_sel,  // timer index
        .clk_cfg = LEDC_AUTO_CLK,               // Auto select the source clock
    };

    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&heater_timer);

    // Set LED Controller with previously prepared configuration
    ledc_channel_config(&heater_channel);

    // Initialize fade service.
    ledc_fade_func_install(0);

    // Turn off heater
    app_heater_off();
}


void app_heater_off (void)
{
    ledc_set_duty(heater_channel.speed_mode, heater_channel.channel, 0);
    ledc_update_duty(heater_channel.speed_mode, heater_channel.channel);
}


void app_heater_on (uint8_t u8_power_in_percents)
{
    uint32_t current_duty = ledc_get_duty (heater_channel.speed_mode, heater_channel.channel);
    uint32_t heater_duty = 2 * (1 << (HEATER_DUTY_RESOLUTION - 1)) * u8_power_in_percents / 100;
    if (heater_duty != current_duty)
    {
        ledc_set_duty (heater_channel.speed_mode, heater_channel.channel, heater_duty);
        ledc_update_duty (heater_channel.speed_mode, heater_channel.channel);
    }
}
