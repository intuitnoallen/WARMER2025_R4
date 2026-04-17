#include "encoder.h"
#include "driver/pcnt.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "ENCODER";
#define ENCODER_PCNT_UNIT PCNT_UNIT_0

// --- FIX 1: Define a spinlock for our critical sections ---
static portMUX_TYPE encoder_mux = portMUX_INITIALIZER_UNLOCKED;

static uint16_t pulses_per_rev = 8;
static volatile int64_t accumulated_pulse_count = 0;
static int16_t last_pcnt_value = 0;
static int32_t lastCount = 0;
static float current_rpm = 0.0f;

static void update_accumulated_count(void)
{
    int16_t current_pcnt_value;
    pcnt_get_counter_value(ENCODER_PCNT_UNIT, &current_pcnt_value);
    int16_t delta = current_pcnt_value - last_pcnt_value;
    last_pcnt_value = current_pcnt_value;
    accumulated_pulse_count += delta;
}

esp_err_t encoder_init()
{
    // (Initialization code is the same as before, no changes here)
    pcnt_config_t pcnt_config = {
        .pulse_gpio_num = ENCODER_A_PIN,
        .ctrl_gpio_num = ENCODER_B_PIN,
        .channel = PCNT_CHANNEL_0,
        .unit = ENCODER_PCNT_UNIT,
        .pos_mode = PCNT_COUNT_DEC,
        .neg_mode = PCNT_COUNT_INC,
        .lctrl_mode = PCNT_MODE_REVERSE,
        .hctrl_mode = PCNT_MODE_KEEP,
    };
    ESP_ERROR_CHECK(pcnt_unit_config(&pcnt_config));
    // gpio_set_pull_mode(ENCODER_A_PIN, GPIO_PULLUP_ONLY);
    // gpio_set_pull_mode(ENCODER_B_PIN, GPIO_PULLUP_ONLY);
    ESP_ERROR_CHECK(pcnt_counter_pause(ENCODER_PCNT_UNIT));
    ESP_ERROR_CHECK(pcnt_counter_clear(ENCODER_PCNT_UNIT));
    pcnt_get_counter_value(ENCODER_PCNT_UNIT, &last_pcnt_value);
    ESP_ERROR_CHECK(pcnt_counter_resume(ENCODER_PCNT_UNIT));
    ESP_LOGI(TAG, "Encoder initialized using PCNT hardware peripheral.");
    return ESP_OK;
}

void encoder_update_rpm(void)
{
    // --- FIX 2: Pass the spinlock to the macros ---
    taskENTER_CRITICAL(&encoder_mux);
    update_accumulated_count();
    taskEXIT_CRITICAL(&encoder_mux);

    int32_t current_total_count = accumulated_pulse_count;
    int32_t delta = current_total_count - lastCount;
    lastCount = current_total_count;

    float revolutions = (float)delta / pulses_per_rev;
    current_rpm = revolutions * 600.0f;
}

float encoder_get_rpm(void)
{
    return current_rpm;
}

unsigned long encoder_get_pulse_count(void)
{
    // --- FIX 2: Pass the spinlock to the macros ---
    taskENTER_CRITICAL(&encoder_mux);
    update_accumulated_count();
    taskEXIT_CRITICAL(&encoder_mux);
    
    return (unsigned long)accumulated_pulse_count;
}