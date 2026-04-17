#ifndef ENCODER_H
#define ENCODER_H

#include "esp_err.h"
#include "app_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize encoder GPIOs and interrupt service
esp_err_t encoder_init();

// Update and compute RPM (call once every 1000ms or as needed)
void encoder_update_rpm(void);

// Get the most recent RPM value
float encoder_get_rpm(void);

// Get raw pulse count (optional)
unsigned long encoder_get_pulse_count(void);

#ifdef __cplusplus
}
#endif

#endif // ENCODER_H
