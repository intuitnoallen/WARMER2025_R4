/*
 * app_buzzer.c
 *
 *  Created on: Mar 28, 2022
 *      Author: Teppy
 */
#include "app_buzzer.h"
#include "app_warmer_mngr.h"
#include "driver/ledc.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define TAG "APP_BUZZER"

// Pin config
// #define BUZZER_PIN   33   // Tone output pin
#define VOLUME_PIN   4    // Volume control pin


// LEDC config
#define TONE_CHANNEL   LEDC_CHANNEL_1
#define TONE_TIMER     LEDC_TIMER_1
#define VOLUME_CHANNEL LEDC_CHANNEL_2
#define VOLUME_TIMER   LEDC_TIMER_2

SemaphoreHandle_t buzzer_mutex;
/*
** A notation consists of a pair of {frequency, interval}
** Note that (frequency * interval / 1000) must be less than 1024
*/
const note song[] = {
    {1108, 500}, {1108, 500}, {1318, 750}, {1318, 750}, {1108, 500}, {1108, 500},
    {1318, 750}, {1318, 750}, {1108, 500}, {1318, 500}, {1760, 500}, {1760, 500},
    {1760, 500}, {1661, 500}, {1661, 500}, {1661, 500}, {1479, 500}, {1479, 500},
    {1318, 750}, {1318, 750}, { 987, 500}, {1108, 500}, {1174, 750}, {1174, 750},
    { 987, 500}, {1108, 500}, {1174, 750}, {1174, 750}, { 987, 500}, {1174, 500},
    {1661, 500}, {1479, 500}, {1318, 750}, {1318, 750}, {1661, 500}, {1760, 500},
    {1760, 500}, {1760, 500}, { 880, 500}, {1760, 500}, {1760, 500}, {1760, 500},
    {1479, 500}, {1174, 500}, {1318, 750}, {1318, 750}, {1108, 500}, { 880, 500},
    {1174, 500}, {1318, 500}, {1318, 500}, {1479, 500}, {1479, 500}, {1318, 750},
    {1318, 750}, { 880, 500}, {1760, 500}, {1760, 500}, {1760, 500}, {1479, 500},
    {1174, 500}, {1318, 750}, {1318, 750}, {1108, 500}, { 880, 500}, {1174, 500},
    {1108, 500}, {1108, 500}, {987, 1000}, { 880, 750}, { 880, 750}
};

const note buzzer_500ms_beep[] = {{2700, 370}, {2700, 30}};
const note buzzer_200ms_beep[] = {{2700, 200}};
const note buzzer_200ms_double_beep[] = {{2700, 200}, {0, 100}, {2700, 200}};
const note buzzer_500ms_double_beep[] = {{2700, 370}, {0, 100}, {2700, 370}};
const note buzzer_soft_beep[] = {{4000, 100}};

/* helper to compute array size when array is visible in the same TU */
#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))

/* buzzer playback state */
static const note *s_seq = NULL;
static size_t s_len = 0;
static size_t s_idx = 0;
static esp_timer_handle_t s_note_timer = NULL;
static portMUX_TYPE s_lock = portMUX_INITIALIZER_UNLOCKED;

static WARMER_config_t * g_pstru_config = NULL;

/* forward */
static void play_sequence_nolock(const note *seq, size_t len);

// ===== Init =====
void buzzer_init(void) {

    // buzzer_mutex = xSemaphoreCreateMutex();

    // Tone channel config
    ledc_timer_config_t timer_conf_tone = {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_10_BIT,
        .timer_num        = TONE_TIMER,
        .freq_hz          = 2000, // default
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf_tone);

    ledc_channel_config_t channel_conf_tone= {
        .gpio_num       = BUZZER_PIN,
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = TONE_CHANNEL,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = TONE_TIMER,
        .duty           = 0,
        .hpoint         = 0
    };
    ledc_channel_config(&channel_conf_tone);

    // Volume channel config
    ledc_timer_config_t timer_conf_vol= {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_8_BIT,
        .timer_num        = VOLUME_TIMER,
        .freq_hz          = 25000, // inaudible
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf_vol);

    ledc_channel_config_t channel_conf_vol = {
        .gpio_num       = VOLUME_PIN,
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = VOLUME_CHANNEL,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = VOLUME_TIMER,
        .duty           = 0,
        .hpoint         = 0
    };
    ledc_channel_config(&channel_conf_vol);

}



/* timer callback: advance to next note (runs in esp_timer task context) */
static void buzzer_note_timer_cb(void *arg)
{
    portENTER_CRITICAL(&s_lock);
    if (!s_seq) {
        portEXIT_CRITICAL(&s_lock);
        return;
    }

    s_idx++;
    if (s_idx >= s_len) {
        /* finished */
        s_seq = NULL;
        s_len = 0;
        s_idx = 0;
        /* ensure tone is off */
        ledc_stop(LEDC_HIGH_SPEED_MODE, TONE_CHANNEL, 0);
        portEXIT_CRITICAL(&s_lock);
        // ESP_LOGI(TAG, "buzzer sequence finished");
        return;
    }

    /* play next note */
    const note n = s_seq[s_idx];

    /* always stop previous tone before starting new one */
    // (void)ledc_stop(LEDC_HIGH_SPEED_MODE, TONE_CHANNEL, 0);

    if (n.freq == 0) {
        /* gap: ensure output is silent */
        (void)ledc_set_duty(LEDC_HIGH_SPEED_MODE, TONE_CHANNEL, 0);
        (void)ledc_update_duty(LEDC_HIGH_SPEED_MODE, TONE_CHANNEL);
        esp_err_t err = esp_timer_start_once(s_note_timer, n.duration * 1000ULL);
        if (err != ESP_OK) ESP_LOGW(TAG, "esp_timer_start_once gap err %d", err);
    } else {
        ledc_set_freq(LEDC_HIGH_SPEED_MODE, TONE_TIMER, n.freq);
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, TONE_CHANNEL, 512);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, TONE_CHANNEL);

        esp_err_t err = esp_timer_start_once(s_note_timer,n.duration * 1000ULL);
        if (err != ESP_OK) ESP_LOGW(TAG, "esp_timer_start_once tone err %d", err);
    }

    portEXIT_CRITICAL(&s_lock);
}

/* internal: start sequence (non-blocking). Caller must hold no locks. */
static void play_sequence_nolock(const note *seq, size_t len)
{
    if (seq == NULL || len == 0) return;

    portENTER_CRITICAL(&s_lock);
    // xSemaphoreTake(buzzer_mutex, portMAX_DELAY);
    /* stop any running timer & tone */
    if (s_note_timer) {
        esp_timer_stop(s_note_timer);
    }
    ledc_stop(LEDC_HIGH_SPEED_MODE, TONE_CHANNEL, 0);

    s_seq = seq;
    s_len = len;
    s_idx = 0;

    /* start first note */
    const note n = s_seq[0];
    if (n.freq == 0) {
        (void)ledc_set_duty(LEDC_HIGH_SPEED_MODE, TONE_CHANNEL, 0);
        (void)ledc_update_duty(LEDC_HIGH_SPEED_MODE, TONE_CHANNEL);
        esp_err_t err = esp_timer_start_once(s_note_timer, n.duration * 1000ULL);
        if (err != ESP_OK) ESP_LOGW(TAG, "esp_timer_start_once gap err %d", err);
    } else {
        ledc_set_freq(LEDC_HIGH_SPEED_MODE, TONE_TIMER, n.freq);
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, TONE_CHANNEL, 512);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, TONE_CHANNEL);
        esp_err_t err = esp_timer_start_once(s_note_timer, n.duration * 1000ULL);
        if (err != ESP_OK) ESP_LOGW(TAG, "esp_timer_start_once tone err %d", err);
    }
    portEXIT_CRITICAL(&s_lock);
    // xSemaphoreGive(buzzer_mutex);
}

/* public API implementations */

void app_buzzer_init(void)
{
     g_pstru_config = pstru_WARMER_Get_Config ();
    /* configure LEDC channels and timers (calls your existing init) */
    buzzer_init();

    /* create single one-shot timer used to sequence notes */
    if (s_note_timer == NULL) {
        const esp_timer_create_args_t targs = {
            .callback = buzzer_note_timer_cb,
            .arg = NULL,
            .name = "buzzer_note_tmr"
        };
        esp_err_t err = esp_timer_create(&targs, &s_note_timer);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "esp_timer_create failed: %d", err);
        }
    }

    /* set initial volume to 80% as requested */
    app_buzzer_update_volume();
    ESP_LOGI(TAG, "Buzzer initialized");
    app_buzzer_off();
}

void app_buzzer_off(void)
{
    portENTER_CRITICAL(&s_lock);
    // xSemaphoreTake(buzzer_mutex, portMAX_DELAY);
    if (s_note_timer) {
        (void)esp_timer_stop(s_note_timer);
    }
    s_seq = NULL;
    s_len = 0;
    s_idx = 0;
    (void)ledc_stop(LEDC_HIGH_SPEED_MODE, TONE_CHANNEL, 0);
    /* optionally mute volume channel if you want:
       (void)ledc_set_duty(LEDC_HIGH_SPEED_MODE, VOLUME_CHANNEL, 0);
       (void)ledc_update_duty(LEDC_HIGH_SPEED_MODE, VOLUME_CHANNEL);
    */
    portEXIT_CRITICAL(&s_lock);
    // ESP_LOGI(TAG, "buzzer off");
    // xSemaphoreGive(buzzer_mutex);
}

void app_buzzer_update_volume(void)
{
    
    /* 80% on volume channel (8-bit resolution) */
    uint32_t max_duty = (1 << LEDC_TIMER_8_BIT) - 1;
    uint32_t duty = (max_duty * g_pstru_config->enm_buzzer_volume) / 100; /* vol % */
    uint32_t vol_freq = g_pstru_config->enm_buzzer_volume == BUZZER_VOLUME_LEVEL_5 ? 25000  :
                        g_pstru_config->enm_buzzer_volume == BUZZER_VOLUME_LEVEL_4 ? 70000  :
                        g_pstru_config->enm_buzzer_volume == BUZZER_VOLUME_LEVEL_3 ? 100000  :
                        g_pstru_config->enm_buzzer_volume == BUZZER_VOLUME_LEVEL_2 ? 125000 :
                        g_pstru_config->enm_buzzer_volume == BUZZER_VOLUME_LEVEL_1 ? 150000 : 25000;

    (void)ledc_set_freq(LEDC_HIGH_SPEED_MODE, VOLUME_CHANNEL, vol_freq);
    (void)ledc_set_duty(LEDC_HIGH_SPEED_MODE, VOLUME_CHANNEL, duty);
    (void)ledc_update_duty(LEDC_HIGH_SPEED_MODE, VOLUME_CHANNEL);
    
    
}

/* wrappers that play predefined sequences - these expect the arrays to be visible
   in the same compilation unit. If in another file, ensure they are extern and
   visible at link time. */

void app_buzzer_500ms_beep(void)
{
    if (g_pstru_config->b_buzzer_enabled)
    {
    play_sequence_nolock(buzzer_500ms_beep, ARRAY_SIZE(buzzer_500ms_beep));
    }
}

void app_buzzer_200ms_beep(void)
{
    if (g_pstru_config->b_buzzer_enabled)
    {
    play_sequence_nolock(buzzer_200ms_beep, ARRAY_SIZE(buzzer_200ms_beep));
    }
}

void app_buzzer_200ms_double_beep(void)
{
    if (g_pstru_config->b_buzzer_enabled)
    {
    play_sequence_nolock(buzzer_200ms_double_beep, ARRAY_SIZE(buzzer_200ms_double_beep));
    }
}

void app_buzzer_500ms_double_beep(void)
{
    if (g_pstru_config->b_buzzer_enabled)
    {
    play_sequence_nolock(buzzer_500ms_double_beep, ARRAY_SIZE(buzzer_500ms_double_beep));
    }
}

void app_buzzer_soft_beep(void)
{
    if (g_pstru_config->b_buzzer_enabled)
    {
    play_sequence_nolock(buzzer_soft_beep, ARRAY_SIZE(buzzer_soft_beep));
    }
}

void app_buzzer_play_song(void)
{
    if (g_pstru_config->b_buzzer_enabled)
    {
    play_sequence_nolock(song, ARRAY_SIZE(song));
    }
}