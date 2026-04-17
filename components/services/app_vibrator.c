/* 
    * app_vibrator.c
    *
    *  Created on: Sept 21, 2025
    * Author: Hassam Bin Hassan
*/
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "app_warmer_mngr.h"
#include "app_vibrator.h"

#define VIBRATOR_GPIO   GPIO_NUM_10   // Your motor pin
#define TAG  "APP_VIBRATOR"

#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))

SemaphoreHandle_t vibration_mutex;

const vib_pattern_t song_buzz[] = {
    
    {1, 300}, {0, 500}, {1, 300}, {0, 1000}, {1, 300}, {0, 500}, {1, 300}, {0, 1000},   
    {1, 300}, {0, 500}, {1, 300}, {0, 1000}, {1, 300}, {0, 500}, {1, 300}, {0, 1000},   
    {1, 300}, {0, 500}, {1, 300}, {0, 1000}, {1, 300}, {0, 500}, {1, 300}, {0, 1000},   
    {1, 300}, {0, 500}, {1, 300}, {0, 1000}, {1, 300}, {0, 500}, {1, 300}, {0, 1000},   
    {1, 300}, {0, 500}, {1, 300}, {0, 1000}, {1, 300}, {0, 500}, {1, 300}, {0, 1000},  
    {1, 300}, {0, 500}, {1, 300}, {0, 1000}, {1, 300}, {0, 500}, {1, 300}, {0, 1000}, 
    {1, 300}, {0, 500}, {1, 300}, {0, 1000}, {1, 300}, {0, 500}, {1, 300}, {0, 1000}, 
    {1, 300}, {0, 500}, {1, 300}, {0, 1000}, {1, 300}, {0, 500}, {1, 300}, {0, 1000}, 
    {1, 300}, {0, 500}, {1, 300}, {0, 1000}, {1, 300}, {0, 500}, {1, 300}, {0, 1000},
    {0, 200}
};


const vib_pattern_t vibration_500ms_buzz[] = {{1, 300},{0,300}};
const vib_pattern_t vibration_200ms_buzz[] = {{1, 200},{0,200}};
const vib_pattern_t vibration_500ms_double_buzz[] = {{1, 300},{0,300},{1, 300},{0,300}};
const vib_pattern_t vibration_200ms_double_buzz[] = {{1, 100}, {0, 100}, {1, 100},{0, 100}};
const vib_pattern_t vibration_soft_buzz[] = {{1, 100},{0,100}};

/* Example usage */
const vib_pattern_t vib_short[] = {
    {1, 100}, {0, 200}, {1, 100}, {0, 400}
};

static const vib_pattern_t *v_seq = NULL;
static size_t v_len = 0;
static size_t v_idx = 0;
static portMUX_TYPE v_lock = portMUX_INITIALIZER_UNLOCKED;
static esp_timer_handle_t v_timer = NULL;

static WARMER_config_t * g_pstru_config = NULL;

/* Ensure vibrator is off */
void app_vibrator_off(void)
{
    portENTER_CRITICAL(&v_lock);
    // xSemaphoreTake(vibration_mutex, portMAX_DELAY);

    // Stop vibration timer if active
    if (v_timer) {
        esp_timer_stop(v_timer);
    }

    // Turn off vibrator GPIO immediately
    gpio_set_level(VIBRATOR_GPIO, 0);

    // Reset sequence-related variables
    v_seq = NULL;
    v_len = 0;
    v_idx = 0;

    portEXIT_CRITICAL(&v_lock);
    // xSemaphoreGive(vibration_mutex);
}

/* Timer callback: advance to next state */
static void vibrator_timer_cb(void *arg)
{
    portENTER_CRITICAL(&v_lock);

    if (!v_seq) {
        portEXIT_CRITICAL(&v_lock);
        return;
    }

    v_idx++;
    if (v_idx >= v_len) {
        // sequence finished
        v_seq = NULL;
        v_len = 0;
        v_idx = 0;
        app_vibrator_off();
        portEXIT_CRITICAL(&v_lock);
        // ESP_LOGI(TAG, "vibrator sequence finished");
        return;
    }

    const vib_pattern_t step = v_seq[v_idx];
    gpio_set_level(VIBRATOR_GPIO, step.state);

    esp_err_t err = esp_timer_start_once(v_timer, step.duration * 1000ULL);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "esp_timer_start_once err %d", err);
    }

    portEXIT_CRITICAL(&v_lock);
}

/* Public: init vibrator driver */
void app_vibrator_init(void)
{
    // vibration_mutex = xSemaphoreCreateMutex();
    g_pstru_config = pstru_WARMER_Get_Config ();
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << VIBRATOR_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    app_vibrator_off();

    if (v_timer == NULL) {
        const esp_timer_create_args_t args = {
            .callback = &vibrator_timer_cb,
            .name = "vibrator_timer"
        };
        esp_timer_create(&args, &v_timer);
    }
}

/* Public: start sequence (non-blocking) */
static void play_vibrator_sequence_nolock(const vib_pattern_t *seq, size_t len)
{
    if (seq == NULL || len == 0) return;

    portENTER_CRITICAL(&v_lock);
    // xSemaphoreTake(vibration_mutex, portMAX_DELAY);
    if (v_timer) {
        esp_timer_stop(v_timer);
    }
    app_vibrator_off();

    v_seq = seq;
    v_len = len;
    v_idx = 0;

    const vib_pattern_t step = v_seq[0];
    gpio_set_level(VIBRATOR_GPIO, step.state);

    esp_err_t err = esp_timer_start_once(v_timer, step.duration * 1000ULL);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "esp_timer_start_once start err %d", err);
    }
    portEXIT_CRITICAL(&v_lock);
    // xSemaphoreGive(vibration_mutex);
}

void app_vibrator_play_song(void)
{
    if (g_pstru_config->b_vibration_enabled)
    {
        play_vibrator_sequence_nolock(song_buzz, ARRAY_SIZE(song_buzz));
    }
}

void app_vibrator_soft_buzz(void)
{
    if (g_pstru_config->b_vibration_enabled)
    {
        play_vibrator_sequence_nolock(vibration_soft_buzz, ARRAY_SIZE(vibration_soft_buzz));
    }
}

void app_vibrator_500ms_buzz( void ){
    if (g_pstru_config->b_vibration_enabled)
    {
        play_vibrator_sequence_nolock(vibration_500ms_buzz, ARRAY_SIZE(vibration_500ms_buzz));
    }
}

void app_vibrator_200ms_buzz( void ){
    if (g_pstru_config->b_vibration_enabled)
    {
        play_vibrator_sequence_nolock(vibration_200ms_buzz, ARRAY_SIZE(vibration_200ms_buzz));
    }
}

void app_vibrator_200ms_double_buzz( void ){
    if (g_pstru_config->b_vibration_enabled)
    {
        play_vibrator_sequence_nolock(vibration_200ms_double_buzz, ARRAY_SIZE(vibration_200ms_double_buzz));
    }
}

void app_vibrator_500ms_double_buzz( void ){
    if (g_pstru_config->b_vibration_enabled)
    {
        play_vibrator_sequence_nolock(vibration_500ms_double_buzz, ARRAY_SIZE(vibration_500ms_double_buzz));
    }
}

