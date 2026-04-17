#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/ringbuf.h"
#include "app_ble.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "nvs_flash.h"
#include "esp_bt.h"

#include "freertos/semphr.h"


void ota_task_init(void);
void ota_task_init_attached(void);
void ota_recv_fw_cb(uint8_t *buf, uint32_t length);
void ota_task(void *arg);
size_t write_to_ringbuf(const uint8_t *data, size_t size);
bool ble_ota_ringbuf_init(uint32_t ringbuf_size);
extern bool isOTAupgradingStarted ;