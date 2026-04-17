/*
 * app_ble.h
 *
 *  Created on: Jan 24, 2022
 *      Author: thaob
 */

#ifndef MAIN_APP_BLE_H_
#define MAIN_APP_BLE_H_

#include "app_cfg.h"

#define BLE_MAX_NAME                    64

#define PROFILE_NUM                 3
#define PROFILE_APP_ID              0    
#define OTA_PROFILE_APP_IDX         1
#define DIS_PROFILE_APP_IDX         2                                                                            
#define ESP_GATT_IF_NONE            0xff 

#define GATTS_SERVICE_UUID              0x30AC
#define GATTS_CHAR_UUID                 0xCC01
#define GATTS_NUM_HANDLE                4

#define BUF_LENGTH         4098

#define BLE_OTA_MAX_CHAR_VAL_LEN  600

#define BLE_OTA_START_CMD         0x0001
#define BLE_OTA_STOP_CMD          0x0002
#define BLE_OTA_ACK_CMD           0x0003

#define CHAR_DECLARATION_SIZE    (sizeof(uint8_t))


#define BLE_PACKET_START_LENGTH                 1
#define BLE_PACKET_FROM_LENGTH                  1
#define BLE_PACKET_TYPE_LENGTH                  1
#define BLE_PACKET_LENGTH_FIELD_LENGTH          1
#define BLE_PACKET_DATA_MAX_LENGTH              128
#define BLE_PACKET_CRC_LENGTH                   1
#define BLE_PACKET_END_LENGTH                   1

#define BLE_PACKET_HEADER_LENGTH                (BLE_PACKET_START_LENGTH + BLE_PACKET_FROM_LENGTH + \
                                                BLE_PACKET_TYPE_LENGTH + BLE_PACKET_LENGTH_FIELD_LENGTH)
#define BLE_PACKET_MIN_LENGTH                   (BLE_PACKET_HEADER_LENGTH + BLE_PACKET_CRC_LENGTH + BLE_PACKET_END_LENGTH)
#define BLE_PACKET_MAX_LENGTH                   (BLE_PACKET_HEADER_LENGTH + BLE_PACKET_DATA_MAX_LENGTH + \
                                                BLE_PACKET_CRC_LENGTH + BLE_PACKET_END_LENGTH)

#define BLE_PACKET_START_BYTE                   '<'
#define BLE_PACKET_END_BYTE                     '>'

#define BLE_DATA_CONTROL_LEN_V1                 7
#define BLE_DATA_CONTROL_LEN_V2                 8
#define BLE_DATA_CONTROL_LEN_V3                 9
/////////////////
/**
 * @brief BLE OAT receive data callback function type
 * @param buf : The pointer to the receive data buffer.
 * @param length : The length of receive data buffer.
 */
typedef void (* esp_ble_ota_recv_fw_cb_t)(uint8_t *buf, uint32_t length);

/**
 * @brief BLE OTA callback functions struct
 */
typedef struct esp_ble_ota_callback_funs {
    esp_ble_ota_recv_fw_cb_t recv_fw_cb;    /*!< BLE OTA data receive callback function */
} esp_ble_ota_callback_funs_t;

/**
 * @brief BLE OTA notification flags struct
 */
typedef struct esp_ble_ota_notification_check {
    bool recv_fw_ntf_enable;        /*!< BLE OTA receive firmware characteristic */
    bool process_bar_ntf_enable;    /*!< BLE OTA notify process bar characteristic */
    bool command_ntf_enable;        /*!< BLE OTA command characteristic */
    bool customer_ntf_enable;       /*!< BLE OTA customer data characteristic */
} esp_ble_ota_notification_check_t;


typedef enum {
    BLE_OTA_CMD_ACK = 0,        /*!< Command Ack */
    BLE_OTA_FW_ACK  = 1,        /*!< Firmware Ack */
} ble_ota_ack_type_t;

typedef enum {
    BLE_OTA_CMD_SUCCESS = 0x0000,       /*!< Success Ack */
    BLE_OTA_REJECT      = 0x0001,       /*!< Reject Cmd Ack */
} ble_ota_cmd_ack_status_t;

typedef enum {
    BLE_OTA_FW_SUCCESS = 0x0000,        /*!< Success */
    BLE_OTA_FW_CRC_ERR = 0x0001,        /*!< CRC error */
    BLE_OTA_FW_IND_ERR = 0x0002,        /*!< Sector Index error*/
    BLE_OTA_FW_LEN_ERR = 0x0003,        /*!< Payload length error*/
} ble_ota_fw_ack_statuc_t;


/// BLE OTA characteristics
typedef enum {
    RECV_FW_CHAR,
    RECV_FW_CHAR_CCC,

    OTA_STATUS_CHAR,
    OTA_STATUS_CHAR_CCC,

    CMD_CHAR,
    CMD_CHAR_CCC,

    CUS_CHAR,
    CUS_CHAR_CCC,

    INVALID_CHAR,
} esp_ble_ota_char_t;

/// BLE DIS characteristics
typedef enum {
    DIS_SVC_IDX,

    DIS_MODEL_CHAR_IDX,
    DIS_MODEL_CHAR_VAL_IDX,

    DIS_SN_CHAR_IDX,
    DIS_SN_CHAR_VAL_IDX,

    DIS_FW_CHAR_IDX,
    DIS_FW_CHAR_VAL_IDX,

    DIS_IDX_NB,
} esp_ble_dis_service_index_t;

/// BLE OTA characteristics Index
typedef enum {
    OTA_SVC_IDX,

    RECV_FW_CHAR_IDX,
    RECV_FW_CHAR_VAL_IDX,
    RECV_FW_CHAR_NTF_CFG,

    OTA_STATUS_CHAR_IDX,
    OTA_STATUS_CHAR_VAL_IDX,
    OTA_STATUS_NTF_CFG,

    CMD_CHAR_IDX,
    CMD_CHAR_VAL_IDX,
    CMD_CHAR_NTF_CFG,

    CUS_CHAR_IDX,
    CUS_CHAR_VAL_IDX,
    CUS_CHAR_NTF_CFG,

    OTA_IDX_NB,
} esp_ble_ota_service_index_t;

/**
 * @brief           This function is called to Initialization ble ota host
 *
 * @return
 *                  - ESP_OK: success
 *                  - other: failed
 *
 */
esp_err_t esp_ble_ota_host_init(void);

/**
 * @brief           This function is called to register ble ota receive firmware data callback function
 *
 * @param[in]       callback : pointer to the application callback function.
 *
 * @return
 *                  - ESP_OK: success
 *                  - other: failed
 *
 */

esp_err_t esp_ble_ota_recv_fw_data_callback(esp_ble_ota_recv_fw_cb_t callback);

/**
 * @brief           This function is called to Initialization ble ota process
 *
 * @return
 *                  - length of ota firmware
 *
 */
unsigned int esp_ble_ota_get_fw_length(void);

/**
 * @brief           This function is called to indicate OTA end
 *
 */
void esp_ble_ota_finish(void);
 ////////////////

typedef enum
{
    BLE_FROM_DEVICE = 0x00,
    BLE_FROM_APP = 0x01,
}ble_from_t;

typedef enum
{
    BLE_TYPE_UPDATE_DATA_CONTROL = 0x00,
    BLE_TYPE_WIFI_CREDENTIAL = 0x01,
    BLE_TYPE_OTA_CONTROL = 0x02
}ble_type_t;

typedef struct
{
    union {
        struct {
            uint8_t start;
            uint8_t from;
            uint8_t type;
            uint8_t data_length;
        }p;
        uint8_t h[BLE_PACKET_HEADER_LENGTH];
    }header;

    uint8_t data[BLE_PACKET_DATA_MAX_LENGTH];
}ble_packet_t;

void app_ble_init( void );

void app_ble_start( void );

bool app_ble_stop( void );

bool app_ble_isStarted( void );

bool app_ble_IsConnected( void );

void app_ble_send( uint8_t *data, uint16_t data_len );

void app_ble_update_data( void );

#endif /* MAIN_APP_BLE_H_ */
