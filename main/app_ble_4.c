/*
 * app_ble.c
 *
 *  Created on: Jan 24, 2022
 *      Author: thaob
 *  New features added on: Aug, 2025
 *      Author: Hassam Bin Hassan
 */
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "esp_idf_version.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "app_warmer_mngr.h"
#include "app_notification.h"
#include "esp_ota_ops.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "app_ble_ota.h"
#include "lcd_task.h"
#include "app_ble.h"
#include "ui_common.h"


static const char *TAG = "APP_BLE";

static uint8_t adv_config_done = 0;
#define adv_config_flag                 (1 << 0)
#define scan_rsp_config_flag            (1 << 1)

//#define CONFIG_SET_RAW_ADV_DATA

#ifdef CONFIG_SET_RAW_ADV_DATA
    #define RAW_ADV_DATA_MAX_LEN        26
    static uint8_t raw_adv_data[RAW_ADV_DATA_MAX_LEN + 1] = {0};
    static uint8_t raw_adv_data_len = 0;
    #else
    //static uint8_t adv_service_uuid128[16] =
    //{
    //    /* LSB <--------------------------------------------------------------------------------> MSB */
    //  //first uuid, 16bit, [12],[13] is the value
    //  0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xEA, 0x00, 0x00, 0x00
    //};

    // The length of adv data must be less than 31 bytes
    static uint8_t ble_manufacturer[] =  {0x12, 0x23, 0x45, 0x56};

    static esp_ble_adv_data_t adv_data =
    {
        .set_scan_rsp = false,
        .include_name = true,
        .include_txpower = false,
    //    .min_interval = 0x0006, //slave connection min interval, Time = min_interval * 1.25 msec
    //    .max_interval = 0x0010, //slave connection max interval, Time = max_interval * 1.25 msec
        .appearance = 0x00,
        .manufacturer_len = sizeof(ble_manufacturer),
        .p_manufacturer_data = ble_manufacturer,
        .service_data_len = ESP_BD_ADDR_LEN,
        .p_service_data = device_data.ble_mac,
    //    .service_uuid_len = sizeof(adv_service_uuid128),
    //    .p_service_uuid = adv_service_uuid128,
        .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
    };
#endif //CONFIG_SET_RAW_ADV_DATA

static esp_ble_adv_params_t adv_params =
{
    .adv_int_min        = 0x20,
    .adv_int_max        = 0x40,
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    //.peer_addr            =
    //.peer_addr_type       =
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

// static esp_ble_adv_params_t ota_adv_params = {
//     .adv_int_min        = 0x20,
//     .adv_int_max        = 0x40,
//     .adv_type           = ADV_TYPE_IND,
//     .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
//     .channel_map        = ADV_CHNL_ALL,
//     .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
// };                                                          

struct gatts_profile_inst
{
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_handle;
    esp_bt_uuid_t char_uuid;
    esp_gatt_perm_t perm;
    esp_gatt_char_prop_t property;
    uint16_t descr_handle;
    esp_bt_uuid_t descr_uuid;
    uint16_t mtu_size;

};

static bool ble_connected = false;
static bool ble_started = false;
static volatile bool s_ota_mode = false;
bool isOTAupgradingStarted = false;

static WARMER_config_t * g_pstru_config = NULL;




// struct gatts_profile_inst {
//     esp_gatts_cb_t gatts_cb;
//     uint16_t gatts_if;
//     uint16_t conn_id;
//     uint16_t mtu_size;
// };




// static const uint8_t ota_adv_data[31] = {
//     0x02, 0x01, 0x06,
//     0x03, 0x03, 0x18, 0x80, //UUID
//     0x09, 0x09, 0x45, 0x53, 0x50, 0x2D, 0x43, 0x39, 0x31, 0x39, //ESP-C919
//     0x0d, 0xff, 0xe5, 0x02, 0x01, 0x01, 0x27, 0x95, 0x01, 0x00, 0x00, 0x00, 0xff, 0xff
// };

// static const uint8_t ota_scan_rsp_data[31] = {
//     0x02, 0x01, 0x06,
//     0x03, 0x03, 0x18, 0x80, //UUID
//     0x09, 0x09, 0x45, 0x53, 0x50, 0x2D, 0x43, 0x39, 0x31, 0x39, //ESP-C919
//     0x0d, 0xff, 0xe5, 0x02, 0x01, 0x01, 0x27, 0x95, 0x01, 0x00, 0x00, 0x00, 0xff, 0xff
// };

esp_ble_ota_callback_funs_t ota_cb_fun_t = {
    .recv_fw_cb = NULL
};

esp_ble_ota_notification_check_t ota_notification = {
    .recv_fw_ntf_enable = false,
    .process_bar_ntf_enable = false,
    .command_ntf_enable = false,
    .customer_ntf_enable = false,
};



///Declare the static function
static void app_ble_gatts_profile_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static void gatts_ota_profile_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static void gatts_dis_profile_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static void app_ble_data_receive (uint16_t conn_id, uint8_t *data, uint16_t data_len);
static void app_ble_parse (uint8_t *data, uint16_t data_len);
static void app_ble_packing_data (ble_packet_t *ble_packet);
static uint8_t app_ble_checksum (uint8_t *data, uint8_t data_len);

/**
 * @brief           This function is called to send notification to remote device
 *
 * @param[in]       ota_char: the characteristic index which to be send
 * @param[in]       value: the pointer to the send value
 * @param[in]       length: the value length
 *
 * @return
 *                  - ESP_OK : success
 *                  - other  : failed
 */
esp_err_t esp_ble_ota_notification_data(esp_ble_ota_char_t ota_char, uint8_t *value, uint8_t length);

/**
 * @brief           This function is called to set the attribute value by the application
 *
 * @param[in]       ota_char: the characteristic index which to be set
 * @param[in]       value: the pointer to the attribute value
 * @param[in]       length: the value length
 *
 * @return
 *                  - ESP_OK : success
 *                  - other  : failed
 */
esp_err_t esp_ble_ota_set_value(esp_ble_ota_char_t ota_char, uint8_t *value, uint8_t length);

static bool start_ota = false;
static unsigned int ota_total_len = 0;
static unsigned int cur_sector = 0;
static unsigned int cur_packet = 0;
static uint8_t *fw_buf = NULL;
static unsigned int fw_buf_offset = 0;
static uint8_t *temp_prep_write_buf = NULL;
static unsigned int temp_buf_len = 0;

static const uint16_t primary_service_uuid = ESP_GATT_UUID_PRI_SERVICE;
static const uint16_t character_declaration_uuid = ESP_GATT_UUID_CHAR_DECLARE;
static const uint16_t character_client_config_uuid = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;

static const uint8_t char_prop_read = ESP_GATT_CHAR_PROP_BIT_READ;
static const uint8_t char_prop_read_indicate = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_INDICATE;
static const uint8_t char_prop_write_indicate = ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_WRITE_NR| ESP_GATT_CHAR_PROP_BIT_INDICATE;

static const uint16_t BLE_OTA_SERVICE_UUID              = 0x8018;

static const uint16_t RECV_FW_UUID                      = 0x8020;
static const uint16_t OTA_BAR_UUID                      = 0x8021;
static const uint16_t COMMAND_UUID                      = 0x8022;
static const uint16_t CUSTOMER_UUID                     = 0x8023;

static uint8_t receive_fw_val[BLE_OTA_MAX_CHAR_VAL_LEN] = {0};
static uint8_t receive_fw_val_ccc[2] = {0x00, 0x00};

static uint8_t ota_status_val[20] = {0};
static uint8_t ota_status_val_ccc[2] = {0x00, 0x00};

static uint8_t command_val[20] = {0};
static uint8_t command_val_ccc[2] = {0x00, 0x00};

static uint8_t custom_val[20] = {0};
static uint8_t custom_val_ccc[2] = {0x00, 0x00};

static uint16_t ota_handle_table[OTA_IDX_NB];

static const uint16_t DIS_SERVICE_UUID    = 0x180A;
static const uint16_t DIS_MODEL_CHAR_UUID = 0x2A24;
static const uint16_t DIS_SN_CHAR_UUID    = 0x2A25;
static const uint16_t DIS_FW_CHAR_UUID    = 0x2A26;

static uint8_t dis_model_value[] = "Espressif";
static uint8_t dis_sn_value[]    = "esp-ota";
static uint8_t dis_fw_value[]    = "1.0";

static uint16_t dis_handle_table[DIS_IDX_NB];

/* OTA Full Database Description - Used to add attributes into the database */
static const esp_gatts_attr_db_t ota_gatt_db[OTA_IDX_NB] = {
    // Service Declaration
    [OTA_SVC_IDX]        =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &primary_service_uuid, ESP_GATT_PERM_READ,
            sizeof(BLE_OTA_SERVICE_UUID), sizeof(BLE_OTA_SERVICE_UUID), (uint8_t *) &BLE_OTA_SERVICE_UUID
        }
    },

    /* Characteristic Declaration */
    [RECV_FW_CHAR_IDX]      =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &character_declaration_uuid, ESP_GATT_PERM_READ,
            CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *) &char_prop_write_indicate
        }
    },

    /* Characteristic Value */
    [RECV_FW_CHAR_VAL_IDX]  =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &RECV_FW_UUID, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
            sizeof(receive_fw_val), sizeof(receive_fw_val), (uint8_t *)receive_fw_val
        }
    },

    //data notify characteristic Declaration
    [RECV_FW_CHAR_NTF_CFG]  =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &character_client_config_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
            sizeof(receive_fw_val_ccc), sizeof(receive_fw_val_ccc), (uint8_t *)receive_fw_val_ccc
        }
    },

    //data receive characteristic Declaration
    [OTA_STATUS_CHAR_IDX]            =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &character_declaration_uuid, ESP_GATT_PERM_READ,
            CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *) &char_prop_read_indicate
        }
    },

    //data receive characteristic Value
    [OTA_STATUS_CHAR_VAL_IDX]               =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &OTA_BAR_UUID, ESP_GATT_PERM_READ,
            sizeof(ota_status_val), sizeof(ota_status_val), (uint8_t *)ota_status_val
        }
    },

    //data notify characteristic Declaration
    [OTA_STATUS_NTF_CFG]  =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &character_client_config_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
            sizeof(ota_status_val_ccc), sizeof(ota_status_val_ccc), (uint8_t *)ota_status_val_ccc
        }
    },

    //data receive characteristic Declaration
    [CMD_CHAR_IDX]            =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &character_declaration_uuid, ESP_GATT_PERM_READ,
            CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *) &char_prop_write_indicate
        }
    },

    //data receive characteristic Value
    [CMD_CHAR_VAL_IDX]              =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &COMMAND_UUID, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
            sizeof(command_val), sizeof(command_val), (uint8_t *)command_val
        }
    },

    //data notify characteristic Declaration
    [CMD_CHAR_NTF_CFG]  =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &character_client_config_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
            sizeof(command_val_ccc), sizeof(command_val_ccc), (uint8_t *)command_val_ccc
        }
    },

    //data receive characteristic Declaration
    [CUS_CHAR_IDX]            =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &character_declaration_uuid, ESP_GATT_PERM_READ,
            CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *) &char_prop_write_indicate
        }
    },

    //data receive characteristic Value
    [CUS_CHAR_VAL_IDX]              =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &CUSTOMER_UUID, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
            sizeof(custom_val), sizeof(custom_val), (uint8_t *)custom_val
        }
    },

    //data notify characteristic Declaration
    [CUS_CHAR_NTF_CFG]  =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &character_client_config_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
            sizeof(custom_val_ccc), sizeof(custom_val_ccc), (uint8_t *)custom_val_ccc
        }
    },

};

/* DIS Full Database Description - Used to add attributes into the database */
static const esp_gatts_attr_db_t dis_gatt_db[DIS_IDX_NB] = {
    // Service Declaration
    [DIS_SVC_IDX]        =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &primary_service_uuid, ESP_GATT_PERM_READ,
            sizeof(DIS_SERVICE_UUID), sizeof(DIS_SERVICE_UUID), (uint8_t *) &DIS_SERVICE_UUID
        }
    },

    /* Characteristic Declaration */
    [DIS_MODEL_CHAR_IDX]      =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &character_declaration_uuid, ESP_GATT_PERM_READ,
            CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *) &char_prop_read
        }
    },

    /* Characteristic Value */
    [DIS_MODEL_CHAR_VAL_IDX]  =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &DIS_MODEL_CHAR_UUID, ESP_GATT_PERM_READ,
            sizeof(dis_model_value), sizeof(dis_model_value), (uint8_t *)dis_model_value
        }
    },

    /* Characteristic Declaration */
    [DIS_SN_CHAR_IDX]      =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &character_declaration_uuid, ESP_GATT_PERM_READ,
            CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *) &char_prop_read
        }
    },

    /* Characteristic Value */
    [DIS_SN_CHAR_VAL_IDX]  =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &DIS_SN_CHAR_UUID, ESP_GATT_PERM_READ,
            sizeof(dis_sn_value), sizeof(dis_sn_value), (uint8_t *)dis_sn_value
        }
    },

    /* Characteristic Declaration */
    [DIS_FW_CHAR_IDX]      =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &character_declaration_uuid, ESP_GATT_PERM_READ,
            CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *) &char_prop_read
        }
    },

    /* Characteristic Value */
    [DIS_FW_CHAR_VAL_IDX]  =
    {   {ESP_GATT_AUTO_RSP}, {
            ESP_UUID_LEN_16, (uint8_t *) &DIS_FW_CHAR_UUID, ESP_GATT_PERM_READ,
            sizeof(dis_fw_value), sizeof(dis_fw_value), (uint8_t *)dis_fw_value
        }
    },
};

static uint16_t crc16_ccitt(const unsigned char *buf, int len)
{
    uint16_t crc16 = 0;
    int32_t i;

    while (len--) {
        crc16 ^= *buf++ << 8;

        for (i = 0; i < 8; i++) {
            if (crc16 & 0x8000) {
                crc16 = (crc16 << 1) ^ 0x1021;
            } else {
                crc16 = crc16 << 1;
            }
        }
    }

    return crc16;
}

void esp_ble_ota_set_fw_length(unsigned int length)
{
    ota_total_len = length;
}

unsigned int esp_ble_ota_get_fw_length(void)
{
    return ota_total_len;
}

/* One gatt-based profile one app_id and one gatts_if, this array will store the gatts_if returned by ESP_GATTS_REG_EVT */
static struct gatts_profile_inst gl_profile_tab[PROFILE_NUM] =
{
    [PROFILE_APP_ID] =
    {
        .gatts_cb = app_ble_gatts_profile_event_handler,
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
        // .mtu_size = 247,
    },
     [OTA_PROFILE_APP_IDX] = {
        .gatts_cb = gatts_ota_profile_event_handler,
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
        .conn_id  = 0xff,
        .mtu_size = 23,
    },
    [DIS_PROFILE_APP_IDX] = {
        .gatts_cb = gatts_dis_profile_event_handler,
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
        .conn_id  = 0xff,
        .mtu_size = 23,
    },

};
                                                                                         
static void app_ble_gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)  //single gap handler for
{
 esp_bd_addr_t bd_addr;

    ESP_LOGD(TAG, "GAP_EVT, event %d\n", event);

    switch (event)
    {
        // --- APP_BLE_4 priority cases ---
#ifdef CONFIG_SET_RAW_ADV_DATA
        case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:
        {
            adv_config_done &= (~adv_config_flag);
            if (adv_config_done == 0) {
                esp_ble_gap_start_advertising(&adv_params);
            }
            break;
        }

        case ESP_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT:
        {
            adv_config_done &= (~scan_rsp_config_flag);
            if (adv_config_done == 0) {
                esp_ble_gap_start_advertising(&adv_params);
            }
            break;
        }
#else
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
        {
            adv_config_done &= (~adv_config_flag);
            if (adv_config_done == 0) {
                esp_ble_gap_start_advertising(&adv_params);
            }
            break;
        }

        case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
        {
            adv_config_done &= (~scan_rsp_config_flag);
            if (adv_config_done == 0) {
                esp_ble_gap_start_advertising(&adv_params);
            }
            break;
        }
#endif

        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        {
            if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                APP_DEBUG(TAG, "Advertising start failed\n");
            } else {
                APP_DEBUG(TAG, "Advertising started successfully\n");
            }
            break;
        }

        case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
        {
            if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                APP_DEBUG(TAG, "Advertising stop failed\n");
            } else {
                APP_DEBUG(TAG, "Stop adv successfully\n");
            }
            break;
        }

        case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
        {
            APP_DEBUG(TAG, "update connection params status = %d, min_int = %d, max_int = %d,conn_int = %d,latency = %d, timeout = %d\r\n",
                      param->update_conn_params.status,
                      param->update_conn_params.min_int,
                      param->update_conn_params.max_int,
                      param->update_conn_params.conn_int,
                      param->update_conn_params.latency,
                      param->update_conn_params.timeout);
            break;
        }

        // --- OTA-specific cases (only missing ones) ---
#ifdef CONFIG_BT_BLE_50_FEATURES_SUPPORTED
        case ESP_GAP_BLE_EXT_ADV_SET_PARAMS_COMPLETE_EVT:
            esp_ble_gap_config_ext_adv_data_raw(EXT_ADV_HANDLE, sizeof(ext_ota_adv_data), &ext_ota_adv_data[0]);
            break;

        case ESP_GAP_BLE_EXT_ADV_DATA_SET_COMPLETE_EVT:
            esp_ble_gap_ext_adv_start(NUM_EXT_ADV_SET, &ext_adv[0]);
            break;

        case ESP_GAP_BLE_EXT_ADV_START_COMPLETE_EVT:
            ESP_LOGI(TAG, "Ext adv start, status = %d", param->ext_adv_data_set.status);
            break;
#else
        // OTA raw advertising if not already handled
        case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:
            esp_ble_gap_start_advertising(&adv_params);
            break;
#endif

        // --- Common security events ---
        case ESP_GAP_BLE_SEC_REQ_EVT:
            for (int i = 0; i < ESP_BD_ADDR_LEN; i++) {
                ESP_LOGI(TAG, "%x:", param->ble_security.ble_req.bd_addr[i]);
            }
            esp_ble_gap_security_rsp(param->ble_security.ble_req.bd_addr, true);
            break;

        case ESP_GAP_BLE_AUTH_CMPL_EVT:
            memcpy(bd_addr, param->ble_security.auth_cmpl.bd_addr, sizeof(esp_bd_addr_t));
            ESP_LOGI(TAG, "remote BD_ADDR: %08x%04x", 
                     (bd_addr[0] << 24) + (bd_addr[1] << 16) + (bd_addr[2] << 8) + bd_addr[3],
                     (bd_addr[4] << 8) + bd_addr[5]);
            ESP_LOGI(TAG, "address type = %d", param->ble_security.auth_cmpl.addr_type);
            ESP_LOGI(TAG, "pair status = %s", param->ble_security.auth_cmpl.success ? "success" : "fail");
            if (!param->ble_security.auth_cmpl.success) {
                ESP_LOGE(TAG, "fail reason = 0x%x", param->ble_security.auth_cmpl.fail_reason);
            }
            break;

        default:
            break;
    }
}

static void app_ble_gatts_profile_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    switch (event)
    {
        case ESP_GATTS_REG_EVT:
        {
            APP_DEBUG(TAG, "REGISTER_APP_EVT, status %d, app_id %d\n", param->reg.status, param->reg.app_id);

            char ble_name[BLE_MAX_NAME] = {0};
            int id_len = strlen(device_data.device_Id);
            snprintf(ble_name, sizeof(ble_name), "%s-%c%c%c%c", DEVICE_NAME,
                                        device_data.device_Id[id_len - 4], device_data.device_Id[id_len - 3],
                                        device_data.device_Id[id_len - 2], device_data.device_Id[id_len - 1]);
            esp_err_t set_dev_name_ret = esp_ble_gap_set_device_name(ble_name);
            APP_DEBUG(TAG, "BLE name: %s\r\n", ble_name);
            // esp_err_t set_dev_name_ret = esp_ble_gap_set_device_name(DEVICE_NAME);
            if (set_dev_name_ret)
            {
                APP_DEBUG(TAG, "set device name failed, error code = %x\r\n", set_dev_name_ret);
            }
#ifdef CONFIG_SET_RAW_ADV_DATA
            esp_err_t raw_adv_ret = esp_ble_gap_config_adv_data_raw(raw_adv_data, sizeof(raw_adv_data));
            if (raw_adv_ret)
            {
                APP_DEBUG_E(TAG, "config raw adv data failed, error code = %x ", raw_adv_ret);
            }
            adv_config_done |= adv_config_flag;
//          esp_err_t raw_scan_ret = esp_ble_gap_config_scan_rsp_data_raw(raw_scan_rsp_data, sizeof(raw_scan_rsp_data));
//          if (raw_scan_ret)
//          {
//              ESP_LOGE(TAG, "config raw scan rsp data failed, error code = %x", raw_scan_ret);
//          }
//          adv_config_done |= scan_rsp_config_flag;
#else
            //config adv data
            esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
            if (ret)
            {
                APP_DEBUG(TAG, "config adv data failed, error code = %x\r\n", ret);
            }
            adv_config_done |= adv_config_flag;
            //config scan response data
//          ret = esp_ble_gap_config_adv_data(&scan_rsp_data);
//          if (ret)
//          {
//              APP_DEBUG(TAG, "config scan response data failed, error code = %x\r\n", ret);
//          }
//          adv_config_done |= scan_rsp_config_flag;
#endif

            gl_profile_tab[PROFILE_APP_ID].service_id.is_primary = true;
            gl_profile_tab[PROFILE_APP_ID].service_id.id.inst_id = 0x00;
            gl_profile_tab[PROFILE_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
            gl_profile_tab[PROFILE_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID;
            esp_ble_gatts_create_service(gatts_if, &gl_profile_tab[PROFILE_APP_ID].service_id, GATTS_NUM_HANDLE);
            break;
        }

        case ESP_GATTS_READ_EVT:
        {
            APP_DEBUG(TAG, "GATT_READ_EVT, conn_id %d, trans_id %d, handle %d\n", param->read.conn_id, param->read.trans_id, param->read.handle);
            esp_gatt_rsp_t rsp;

            ble_packet_t ble_packet = {0};
            app_ble_packing_data(&ble_packet);
            uint8_t buf[BLE_PACKET_MAX_LENGTH + 1] = {0};
            uint8_t buf_len = 0;
            memcpy(&buf[buf_len], ble_packet.header.h, BLE_PACKET_HEADER_LENGTH);
            buf_len += BLE_PACKET_HEADER_LENGTH;
            memcpy(&buf[buf_len], ble_packet.data, ble_packet.header.p.data_length);
            buf_len += ble_packet.header.p.data_length;
            uint8_t cs = app_ble_checksum(buf, buf_len);
            buf[buf_len++] = cs;
            buf[buf_len++] = BLE_PACKET_END_BYTE;

            memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
            rsp.attr_value.handle = param->read.handle;
            memcpy((char *)rsp.attr_value.value, buf, buf_len);
            rsp.attr_value.len = buf_len;
            esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id, ESP_GATT_OK, &rsp);
            break;
        }

        case ESP_GATTS_WRITE_EVT:
        {
            APP_DEBUG(TAG, "GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d\r\n", param->write.conn_id, param->write.trans_id, param->write.handle);
            esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
            app_ble_data_receive(param->write.conn_id, param->write.value, param->write.len);
            break;
        }

        case ESP_GATTS_EXEC_WRITE_EVT:
        {
            APP_DEBUG(TAG,"ESP_GATTS_EXEC_WRITE_EVT\r\n");
            esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
            break;
        }

        case ESP_GATTS_MTU_EVT:
        {
            // gl_profile_tab[PROFILE_APP_ID].mtu_size = param->mtu.mtu;
            APP_DEBUG(TAG, "ESP_GATTS_MTU_EVT, MTU %d\r\n", param->mtu.mtu);
            break;
        }

        case ESP_GATTS_UNREG_EVT:
        {
            break;
        }

        case ESP_GATTS_CREATE_EVT:
        {
            APP_DEBUG(TAG, "CREATE_SERVICE_EVT, status %d, service_handle %d\n", param->create.status, param->create.service_handle);
            gl_profile_tab[PROFILE_APP_ID].service_handle = param->create.service_handle;
            gl_profile_tab[PROFILE_APP_ID].char_uuid.len = ESP_UUID_LEN_16;
            gl_profile_tab[PROFILE_APP_ID].char_uuid.uuid.uuid16 = GATTS_CHAR_UUID;

            esp_ble_gatts_start_service(gl_profile_tab[PROFILE_APP_ID].service_handle);
            esp_gatt_char_prop_t property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY;
            esp_err_t add_char_ret = esp_ble_gatts_add_char(gl_profile_tab[PROFILE_APP_ID].service_handle, &gl_profile_tab[PROFILE_APP_ID].char_uuid,
                                                            ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                                            property,
                                                            NULL, NULL);
            if (add_char_ret)
            {
                APP_DEBUG(TAG, "add char failed, error code =%x\r\n",add_char_ret);
            }
            break;
        }

        case ESP_GATTS_ADD_INCL_SRVC_EVT:
        {
            APP_DEBUG(TAG, "ESP_GATTS_ADD_INCL_SRVC_EVT\r\n");
            break;
        }

        case ESP_GATTS_ADD_CHAR_EVT:
        {
            uint16_t length = 0;
            const uint8_t *prf_char;

            APP_DEBUG(TAG, "ADD_CHAR_EVT, status %d, attr_handle %d, service_handle %d\n",
                    param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
            gl_profile_tab[PROFILE_APP_ID].char_handle = param->add_char.attr_handle;
            gl_profile_tab[PROFILE_APP_ID].descr_uuid.len = ESP_UUID_LEN_16;
            gl_profile_tab[PROFILE_APP_ID].descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
            esp_err_t get_attr_ret = esp_ble_gatts_get_attr_value(param->add_char.attr_handle,  &length, &prf_char);
            if (get_attr_ret == ESP_FAIL)
            {
                APP_DEBUG(TAG, "ILLEGAL HANDLE\r\n");
            }

            APP_DEBUG(TAG, "the gatts char length = %x\n", length);
            for(int i = 0; i < length; i++)
            {
                APP_DEBUG(TAG, "prf_char[%d] = %x\n", i, prf_char[i]);
            }
            esp_err_t add_descr_ret = esp_ble_gatts_add_char_descr(gl_profile_tab[PROFILE_APP_ID].service_handle, &gl_profile_tab[PROFILE_APP_ID].descr_uuid,
                                                                    ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, NULL, NULL);
            if (add_descr_ret)
            {
                APP_DEBUG(TAG, "add char descr failed, error code = %x\r\n", add_descr_ret);
            }
            break;
        }

        case ESP_GATTS_ADD_CHAR_DESCR_EVT:
        {
            gl_profile_tab[PROFILE_APP_ID].descr_handle = param->add_char_descr.attr_handle;
            APP_DEBUG(TAG, "ADD_DESCR_EVT, status %d, attr_handle %d, service_handle %d\n",
                     param->add_char_descr.status, param->add_char_descr.attr_handle, param->add_char_descr.service_handle);
            break;
        }

        case ESP_GATTS_DELETE_EVT:
        {
            APP_DEBUG(TAG, "ESP_GATTS_DELETE_EVT\r\n");
            break;
        }

        case ESP_GATTS_START_EVT:
        {
            APP_DEBUG(TAG, "SERVICE_START_EVT, status %d, service_handle %d\n", param->start.status, param->start.service_handle);
            break;
        }

        case ESP_GATTS_STOP_EVT:
        {
            APP_DEBUG(TAG, "ESP_GATTS_STOP_EVT\r\n");
            break;
        }

        case ESP_GATTS_CONNECT_EVT:
        {
            esp_ble_conn_update_params_t conn_params = {0};
            memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
            /* For the IOS system, please reference the apple official documents about the ble connection parameters restrictions. */
            conn_params.latency = 0;
            conn_params.max_int = 0x20;    // max_int = 0x20*1.25ms = 40ms
            conn_params.min_int = 0x10;    // min_int = 0x10*1.25ms = 20ms
            conn_params.timeout = 400;    // timeout = 400*10ms = 4000ms
            APP_DEBUG(TAG, "ESP_GATTS_CONNECT_EVT, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x\r\n",
                     param->connect.conn_id,
                     param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                     param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5]);
            gl_profile_tab[PROFILE_APP_ID].conn_id = param->connect.conn_id;
            //start sent the update connection parameters to the peer device.
            esp_ble_gap_update_conn_params(&conn_params);
            ble_connected = true;
            ui_post_update(UI_UPDATE_BLE_STATUS, 1);   // connected
            vTaskDelay(pdMS_TO_TICKS(20));
            break;
        }

        case ESP_GATTS_DISCONNECT_EVT:
        {
            ble_connected = false;
            APP_DEBUG(TAG, "ESP_GATTS_DISCONNECT_EVT, disconnect reason 0x%02X\r\n", param->disconnect.reason);
            
            if(isOTAupgradingStarted){
                ui_post_update(UI_UPDATE_BLE_OTA,3); //failed update
                 vTaskDelay(pdMS_TO_TICKS(20));
            }else{
                ui_post_update(UI_UPDATE_BLE_STATUS, 0);   // disconnected icon update
                 vTaskDelay(pdMS_TO_TICKS(20));
            }
        
            esp_ble_gap_start_advertising(&adv_params);
            break;
        }

        case ESP_GATTS_CONF_EVT:
        {
            // APP_DEBUG(TAG, "ESP_GATTS_CONF_EVT, status %d attr_handle %d\r\n", param->conf.status, param->conf.handle);
            // if (param->conf.status != ESP_GATT_OK)
            // {
                // esp_log_buffer_hex(TAG, param->conf.value, param->conf.len);
            // }
            break;
        }

        case ESP_GATTS_OPEN_EVT:
        case ESP_GATTS_CANCEL_OPEN_EVT:
        case ESP_GATTS_CLOSE_EVT:
        case ESP_GATTS_LISTEN_EVT:
        case ESP_GATTS_CONGEST_EVT:
        default:
        {
            break;
        }
    }
}

static void app_ble_gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
    {
        /* If event is register event, store the gatts_if for each profile */
        if (event == ESP_GATTS_REG_EVT)
        {
            if (param->reg.status == ESP_GATT_OK)
            {
                gl_profile_tab[param->reg.app_id].gatts_if = gatts_if;
            }
            else
            {
                APP_DEBUG(TAG, "Reg app failed, app_id %04x, status %d\n", param->reg.app_id, param->reg.status);
                return;
            }
        }

        /* If the gatts_if equal to profile A, call profile A cb handler,
        * so here call each profile's callback */
        int idx;
        for (idx = 0; idx < PROFILE_NUM; idx++)
        {
            /* ESP_GATT_IF_NONE, not specify a certain gatt_if, need to call every profile cb function */
            if (gatts_if == ESP_GATT_IF_NONE || gatts_if == gl_profile_tab[idx].gatts_if)
            {
                if (gl_profile_tab[idx].gatts_cb)
                {
                    gl_profile_tab[idx].gatts_cb(event, gatts_if, param);
                }
            }
        }
    }

static void gatts_dis_profile_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    esp_err_t ret;

    ESP_LOGD(TAG, "%s - event: %d", __func__, event);

    switch (event) {
    case ESP_GATTS_REG_EVT:
        ret = esp_ble_gatts_create_attr_tab(dis_gatt_db, gatts_if, DIS_IDX_NB, DIS_PROFILE_APP_IDX);
        if (ret) {
            ESP_LOGE(TAG, "%s - create attr table failed, error code = %x", __func__, ret);
        }
        break;
    case ESP_GATTS_READ_EVT:
        ESP_LOGI(TAG, "DIS ESP_GATTS_READ_EVT");
        break;
    case ESP_GATTS_WRITE_EVT:
        ESP_LOGI(TAG, "DIS ESP_GATTS_WRITE_EVT");
        break;
    case ESP_GATTS_EXEC_WRITE_EVT:
        ESP_LOGI(TAG, "DIS ESP_GATTS_EXEC_WRITE_EVT");
        break;
    case ESP_GATTS_MTU_EVT:
        ESP_LOGI(TAG, "DIS ESP_GATTS_MTU_EVT  mtu = %d", param->mtu.mtu);
        break;
    case ESP_GATTS_CONF_EVT:
        break;
    case ESP_GATTS_START_EVT:
        ESP_LOGI(TAG, "DIS SERVICE_START_EVT, status %d, service_handle %d", param->start.status, param->start.service_handle);
        if (param->start.status != ESP_GATT_OK) {
            ESP_LOGE(TAG, "SERVICE START FAIL, status %d", param->start.status);
            break;
        }
        break;
    case ESP_GATTS_CONNECT_EVT:
        break;
    case ESP_GATTS_DISCONNECT_EVT:
        break;
    case ESP_GATTS_CREAT_ATTR_TAB_EVT:
        if (param->add_attr_tab.status != ESP_GATT_OK) {
            ESP_LOGE(TAG, "dis create attribute table failed, error code=0x%x", param->add_attr_tab.status);
        } else if (param->add_attr_tab.num_handle != DIS_IDX_NB) {
            ESP_LOGE(TAG, "dis create attribute table abnormally, num_handle (%d) doesn't equal to DIS_IDX_NB(%d)", param->add_attr_tab.num_handle, DIS_IDX_NB);
        } else {
            ESP_LOGI(TAG, "dis create attribute table successfully, the number handle = %d\n", param->add_attr_tab.num_handle);
            memcpy(dis_handle_table, param->add_attr_tab.handles, sizeof(dis_handle_table));
            esp_ble_gatts_start_service(dis_handle_table[DIS_SVC_IDX]);
        }
        break;
    case ESP_GATTS_STOP_EVT:
        break;
    case ESP_GATTS_OPEN_EVT:
        break;
    case ESP_GATTS_CANCEL_OPEN_EVT:
        break;
    case ESP_GATTS_CLOSE_EVT:
        break;
    case ESP_GATTS_LISTEN_EVT:
        break;
    case ESP_GATTS_CONGEST_EVT:
        break;
    case ESP_GATTS_UNREG_EVT:
        break;
    case ESP_GATTS_DELETE_EVT:
        break;
    default:
        break;
    }
}

static esp_ble_ota_char_t find_ota_char_and_desr_by_handle(uint16_t handle)
{
    esp_ble_ota_char_t ret = INVALID_CHAR;

    for (int i = 0; i < OTA_IDX_NB ; i++) {
        if (handle == ota_handle_table[i]) {
            switch (i) {
            case RECV_FW_CHAR_VAL_IDX:
                ret = RECV_FW_CHAR;
                break;
            case RECV_FW_CHAR_NTF_CFG:
                ret = RECV_FW_CHAR_CCC;
                break;
            case OTA_STATUS_CHAR_VAL_IDX:
                ret = OTA_STATUS_CHAR;
                break;
            case OTA_STATUS_NTF_CFG:
                ret = OTA_STATUS_CHAR_CCC;
                break;
            case CMD_CHAR_VAL_IDX:
                ret = CMD_CHAR;
                break;
            case CMD_CHAR_NTF_CFG:
                ret = CMD_CHAR_CCC;
                break;
            case CUS_CHAR_VAL_IDX:
                ret = CUS_CHAR;
                break;
            case CUS_CHAR_NTF_CFG:
                ret = CUS_CHAR_CCC;
                break;
            default:
                ret = INVALID_CHAR;
                break;
            }
        }
    }

    return ret;
}

esp_err_t esp_ble_ota_recv_fw_handler(uint8_t *buf, unsigned int length)
{
    if (ota_cb_fun_t.recv_fw_cb) {
        ota_cb_fun_t.recv_fw_cb(buf, length);
    }

    return ESP_OK;
}

void esp_ble_ota_send_ack_data(ble_ota_ack_type_t ack_type, uint16_t ack_status, uint16_t ack_param)
{
    uint8_t cmd_ack[20] = {0};
    uint16_t crc16 = 0;

    switch (ack_type) {
    case BLE_OTA_CMD_ACK:
        cmd_ack[0] = (BLE_OTA_ACK_CMD & 0xff);
        cmd_ack[1] = (BLE_OTA_ACK_CMD & 0xff00) >> 8;

        cmd_ack[2] = (ack_param & 0xff);
        cmd_ack[3] = (ack_param & 0xff00) >> 8;

        cmd_ack[4] = (ack_status & 0xff);
        cmd_ack[5] = (ack_status & 0xff00) >> 8;

        crc16 = crc16_ccitt(cmd_ack, 18);
        cmd_ack[18] = crc16 & 0xff;
        cmd_ack[19] = (crc16 & 0xff00) >> 8;

        esp_ble_ota_notification_data(CMD_CHAR, cmd_ack, 20);
        break;
    case BLE_OTA_FW_ACK:
        cmd_ack[0] = (ack_param & 0xff);
        cmd_ack[1] = (ack_param & 0xff00) >> 8;

        cmd_ack[2] = (ack_status & 0xff);
        cmd_ack[3] = (ack_status & 0xff00) >> 8;

        cmd_ack[4] = (cur_sector & 0xff);
        cmd_ack[5] = (cur_sector & 0xff00) >> 8;

        crc16 = crc16_ccitt(cmd_ack, 18);
        cmd_ack[18] = crc16 & 0xff;
        cmd_ack[19] = (crc16 & 0xff00) >> 8;

        esp_ble_ota_notification_data(RECV_FW_CHAR, cmd_ack, 20);
        break;
    default:
        break;
    }
}

void esp_ble_ota_process_recv_data(esp_ble_ota_char_t ota_char, uint8_t *val, uint16_t val_len)
{
      if(!s_ota_mode){
        APP_DEBUG(TAG, "BLE OTA MODE is not enabled\n");
        return;
    }
    unsigned int recv_sector = 0;

    switch (ota_char) {
    case CMD_CHAR:
        // Start BLE OTA Process
        if ((val[0] == 0x01) && (val[1] == 0x00)) {
            if (start_ota) {
                esp_ble_ota_send_ack_data(BLE_OTA_CMD_ACK, BLE_OTA_REJECT, BLE_OTA_START_CMD);
            } else {
                start_ota = true;
                // Calculating Firmware Length
                ota_total_len = (val[2]) +
                                (val[3] * 256) +
                                (val[4] * 256 * 256) +
                                (val[5] * 256 * 256 * 256);
                esp_ble_ota_set_fw_length(ota_total_len);
                ESP_LOGI(TAG, "Recv ota start cmd, fw_length = %d", ota_total_len);
                // Malloc buffer to store receive Firmware
                fw_buf = (uint8_t *)malloc(BUF_LENGTH * sizeof(uint8_t));
                if (fw_buf == NULL) {
                    ESP_LOGE(TAG, "Malloc fail");
                    break;
                } else {
                    memset(fw_buf, 0x0, BUF_LENGTH);
                }

                esp_ble_ota_send_ack_data(BLE_OTA_CMD_ACK, BLE_OTA_CMD_SUCCESS, BLE_OTA_START_CMD);
            }
        }
        // Stop BLE OTA Process
        else if ((val[0] == 0x02) && (val[1] == 0x00)) {
            if (start_ota) {
                extern SemaphoreHandle_t notify_sem;
                xSemaphoreTake(notify_sem, portMAX_DELAY);

                start_ota = false;
                esp_ble_ota_set_fw_length(0);

                xSemaphoreGive(notify_sem);

                ESP_LOGI(TAG, "recv ota stop cmd");
                ui_post_update(UI_UPDATE_BLE_OTA,2); //success
                vTaskDelay(pdMS_TO_TICKS(3000)); 
                esp_ble_ota_send_ack_data(BLE_OTA_CMD_ACK, BLE_OTA_CMD_SUCCESS, BLE_OTA_STOP_CMD);

                if (fw_buf) {
                    free(fw_buf);
                    fw_buf = NULL;
                }
            } else {
                esp_ble_ota_send_ack_data(BLE_OTA_CMD_ACK, BLE_OTA_REJECT, BLE_OTA_STOP_CMD);
            }

        } else {
            ESP_LOGE(TAG, "Unknown Command [0x%02x%02x]", val[1], val[0]);
        }
        break;
    case RECV_FW_CHAR:
        if (start_ota) {
            // Calculating the received sector index
            recv_sector = (val[0] + (val[1] * 256));

            if (recv_sector != cur_sector) { // sector error
                if (recv_sector == 0xffff) { // last sector
                    ESP_LOGI(TAG, "Laster sector");
                } else {  // sector error
                    ESP_LOGE(TAG, "Sector index error, cur: %d, recv: %d", cur_sector, recv_sector);
                    esp_ble_ota_send_ack_data(BLE_OTA_FW_ACK, BLE_OTA_FW_IND_ERR, recv_sector);
                }
            }

            if (val[2] != cur_packet) { // packet seq error
                if (val[2] == 0xff) { // last packet
                    ESP_LOGI(TAG, "laster packet");
                    goto write_ota_data;
                } else { // packet seq error
                    ESP_LOGE(TAG, "Packet index error, cur: %d, recv: %d", cur_packet, val[2]);
                }
            }
write_ota_data:
            memcpy(fw_buf + fw_buf_offset, val + 3, val_len - 3);
            fw_buf_offset += val_len - 3;
            ESP_LOGI(TAG, "DEBUG: Sector:%d, total length:%d, length:%d", cur_sector, fw_buf_offset, val_len - 3);
            if (val[2] == 0xff) {
                cur_packet = 0;
                cur_sector++;
                ESP_LOGD(TAG, "DEBUG: recv %d sector", cur_sector);
                goto sector_end;
            } else {
                cur_packet++;
            }

            break;
sector_end:
            esp_ble_ota_recv_fw_handler(fw_buf, 4096);
            memset(fw_buf, 0x0, 4096);
            fw_buf_offset = 0;
            esp_ble_ota_send_ack_data(BLE_OTA_FW_ACK, BLE_OTA_FW_SUCCESS, recv_sector);
        } else {
            ESP_LOGE(TAG, "BLE OTA hasn't started yet");
        }
        break;
    case OTA_STATUS_CHAR:
        break;
    case CUS_CHAR:
        break;
    default:
        ESP_LOGW(TAG, "Invalid data was received, char[%d]", ota_char);
        ESP_LOG_BUFFER_HEX(TAG, val, val_len);
        break;
    }
}

static void gatts_ota_profile_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    esp_err_t ret;
    esp_ble_ota_char_t ota_char;

    ESP_LOGI(TAG, "%s - event: %d", __func__, event);

    switch (event) {
    case ESP_GATTS_REG_EVT:
        // ret = esp_ble_gap_set_device_name(DEVICE_NAME);
        // if (ret) {
        //     ESP_LOGE(TAG, "set device name failed, error code = %x", ret);
        // }
#ifdef CONFIG_BT_BLE_50_FEATURES_SUPPORTED
        ret = esp_ble_gap_ext_adv_set_params(EXT_ADV_HANDLE, &ext_ota_adv_params);
        if (ret) {
            ESP_LOGE(TAG, "set ext adv params failed, error code = %x", ret);
        }
#else
        // ret = esp_ble_gap_config_adv_data_raw((uint8_t *)ota_adv_data, sizeof(ota_adv_data));
        // if (ret) {
        //     ESP_LOGE(TAG, "set adv data failed, error code = %x", ret);
        // }

        // ret = esp_ble_gap_config_scan_rsp_data_raw((uint8_t *)ota_scan_rsp_data, sizeof(ota_scan_rsp_data));
        // if (ret) {
        //     ESP_LOGE(TAG, "set scan rsp data failed, error code = %x", ret);
        // }
#endif

        ret = esp_ble_gatts_create_attr_tab(ota_gatt_db, gatts_if, OTA_IDX_NB, OTA_PROFILE_APP_IDX);
        if (ret) {
            ESP_LOGE(TAG, "Create attr table failed, error code = %x", ret);
        }
        break;
    case ESP_GATTS_READ_EVT:
        ota_char = find_ota_char_and_desr_by_handle(param->read.handle);
        ESP_LOGI(TAG, "Read event - ota_char: %d", ota_char);
        break;
    case ESP_GATTS_WRITE_EVT:
        if(!s_ota_mode){
            ESP_LOGE(TAG, "BLE OTA MODE is not enabled\n");
            return;
        }
        ota_char = find_ota_char_and_desr_by_handle(param->write.handle);
        ESP_LOGD(TAG, "Write event - ota_char: %d", ota_char);
        // Enable indication
        if ((param->write.len == 2) && (param->write.value[0] == 0x02) && (param->write.value[1] == 0x00)) {
            if (ota_char == OTA_STATUS_CHAR_CCC) {
                ota_notification.process_bar_ntf_enable = true;
            }
            if (ota_char == RECV_FW_CHAR_CCC) {
                ota_notification.recv_fw_ntf_enable = true;
            }
            if (ota_char == CMD_CHAR_CCC) {
                ota_notification.command_ntf_enable = true;
            }
            if (ota_char == CUS_CHAR_CCC) {
                ota_notification.customer_ntf_enable = true;
            }
        }
        // Disable indication
        else if ((param->write.len == 2) && (param->write.value[0] == 0x00) && (param->write.value[1] == 0x00)) {
            if (ota_char == OTA_STATUS_CHAR_CCC) {
                ota_notification.process_bar_ntf_enable = false;
            }
            if (ota_char == RECV_FW_CHAR_CCC) {
                ota_notification.recv_fw_ntf_enable = false;
            }
            if (ota_char == CMD_CHAR_CCC) {
                ota_notification.command_ntf_enable = false;
            }
            if (ota_char == CUS_CHAR_CCC) {
                ota_notification.customer_ntf_enable = false;
            }
        }

        if (param->write.is_prep == false) {
            esp_ble_ota_process_recv_data(ota_char, param->write.value, param->write.len);
        } else {
            if (temp_prep_write_buf == NULL) {
                temp_prep_write_buf = (uint8_t *)malloc(BLE_OTA_MAX_CHAR_VAL_LEN * sizeof(uint8_t));
                if (temp_prep_write_buf == NULL) {
                    ESP_LOGE(TAG, "Malloc buffer for prep write fail");
                    break;
                }

                memset(temp_prep_write_buf, 0x0, BLE_OTA_MAX_CHAR_VAL_LEN);
                temp_buf_len = 0;
            }

            memcpy(temp_prep_write_buf + temp_buf_len, param->write.value, param->write.len);
            temp_buf_len += param->write.len;
        }
        break;
    case ESP_GATTS_EXEC_WRITE_EVT:
        if (param->exec_write.exec_write_flag == ESP_GATT_PREP_WRITE_EXEC) {
            if (temp_prep_write_buf) {
                esp_ble_ota_process_recv_data(RECV_FW_CHAR, temp_prep_write_buf, temp_buf_len);
            }
        } else {
            if (temp_prep_write_buf) {
                free(temp_prep_write_buf);
                temp_prep_write_buf = NULL;
            }

            temp_buf_len = 0;
        }
        break;
    case ESP_GATTS_MTU_EVT:
        ESP_LOGI(TAG, "ESP_GATTS_MTU_EVT - mtu = %d", param->mtu.mtu);
        // ota_profile_tab[OTA_PROFILE_APP_IDX].mtu_size = param->mtu.mtu;
        gl_profile_tab[OTA_PROFILE_APP_IDX].mtu_size = param->mtu.mtu;
        break;
    case ESP_GATTS_CONF_EVT:
        break;
    case ESP_GATTS_START_EVT:
        ESP_LOGI(TAG, "SERVICE_START_EVT, status %d, service_handle %d", param->start.status, param->start.service_handle);
        if (param->start.status != ESP_GATT_OK) {
            ESP_LOGE(TAG, "SERVICE START FAIL, status %d", param->start.status);
            break;
        }
        break;
    case ESP_GATTS_CONNECT_EVT:
        gl_profile_tab[OTA_PROFILE_APP_IDX].conn_id = param->connect.conn_id;
        esp_ble_conn_update_params_t conn_params = {0};
        memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
        /* For the IOS system, please reference the apple official documents about the ble connection parameters restrictions. */
        conn_params.latency = 0;
        conn_params.max_int = 0x06;    // max_int = 0x6*1.25ms = 7.5ms
        conn_params.min_int = 0x06;    // min_int = 0x6*1.25ms = 7.5ms
        conn_params.timeout = 400;     // timeout = 400*10ms = 4000ms
        ESP_LOGI(TAG, "ESP_GATTS_CONNECT_EVT, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x:",
                 param->connect.conn_id,
                 param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                 param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5]);
        //start sent the update connection parameters to the peer device.
        esp_ble_gap_update_conn_params(&conn_params);
        break;
    case ESP_GATTS_DISCONNECT_EVT:
#ifdef CONFIG_BT_BLE_50_FEATURES_SUPPORTED
        esp_ble_gap_ext_adv_start(NUM_EXT_ADV_SET, &ext_adv[0]);
#else
        esp_ble_gap_start_advertising(&adv_params);
#endif
        gl_profile_tab[OTA_PROFILE_APP_IDX].mtu_size = 23;
        break;
    case ESP_GATTS_CREAT_ATTR_TAB_EVT:
        if (param->add_attr_tab.status != ESP_GATT_OK) {
            ESP_LOGE(TAG, "create attribute table failed, error code=0x%x", param->add_attr_tab.status);
        } else if (param->add_attr_tab.num_handle != OTA_IDX_NB) {
            ESP_LOGE(TAG, "create attribute table abnormally, num_handle (%d) doesn't equal to OTA_IDX_NB(%d)", param->add_attr_tab.num_handle, OTA_IDX_NB);
        } else {
            ESP_LOGI(TAG, "create attribute table successfully, the number handle = %d\n", param->add_attr_tab.num_handle);
            memcpy(ota_handle_table, param->add_attr_tab.handles, sizeof(ota_handle_table));
            esp_ble_gatts_start_service(ota_handle_table[OTA_SVC_IDX]);
        }
        break;
    case ESP_GATTS_STOP_EVT:
        break;
    case ESP_GATTS_OPEN_EVT:
        break;
    case ESP_GATTS_CANCEL_OPEN_EVT:
        break;
    case ESP_GATTS_CLOSE_EVT:
        break;
    case ESP_GATTS_LISTEN_EVT:
        break;
    case ESP_GATTS_CONGEST_EVT:
        break;
    case ESP_GATTS_UNREG_EVT:
        break;
    case ESP_GATTS_DELETE_EVT:
        break;
    default:
        break;
    }
}

static esp_err_t esp_ble_ota_set_characteristic_value(esp_ble_ota_service_index_t index, uint8_t *value, uint8_t length)
{
    esp_err_t ret;

    ret = esp_ble_gatts_set_attr_value(ota_handle_table[index], length, value);
    if (ret) {
        ESP_LOGE(TAG, "%s set attribute value fail: %s\n", __func__, esp_err_to_name(ret));
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t esp_ble_ota_set_value(esp_ble_ota_char_t ota_char, uint8_t *value, uint8_t length)
{
    esp_err_t ret;

    switch (ota_char) {
    case RECV_FW_CHAR:
        ret = esp_ble_ota_set_characteristic_value(RECV_FW_CHAR_VAL_IDX, value, length);
        break;
    case OTA_STATUS_CHAR:
        ret = esp_ble_ota_set_characteristic_value(OTA_STATUS_CHAR_VAL_IDX, value, length);
        break;
    case CMD_CHAR:
        ret = esp_ble_ota_set_characteristic_value(CMD_CHAR_VAL_IDX, value, length);
        break;
    case CUS_CHAR:
        ret = esp_ble_ota_set_characteristic_value(CUS_CHAR_VAL_IDX, value, length);
        break;
    default:
        ret = ESP_FAIL;
        break;
    }

    if (ret) {
        ESP_LOGE(TAG, "%s set characteristic value fail: %s\n", __func__, esp_err_to_name(ret));
        return ESP_FAIL;
    }

    return ESP_OK;
}

static esp_err_t esp_ble_ota_send_indication(esp_ble_ota_service_index_t index, uint8_t *value, uint8_t length, bool need_ack)
{
    esp_err_t ret;
    uint16_t offset = 0;

    if (length <= (gl_profile_tab[OTA_PROFILE_APP_IDX].mtu_size - 3)) {
        ret = esp_ble_gatts_send_indicate(gl_profile_tab[OTA_PROFILE_APP_IDX].gatts_if, gl_profile_tab[OTA_PROFILE_APP_IDX].conn_id, ota_handle_table[index], length, value, need_ack);
        if (ret) {
            ESP_LOGE(TAG, "%s send notification fail: %s\n", __func__, esp_err_to_name(ret));
            return ESP_FAIL;
        }
    } else {
        while ((length - offset) > (gl_profile_tab[OTA_PROFILE_APP_IDX].mtu_size - 3)) {
            ret = esp_ble_gatts_send_indicate(gl_profile_tab[OTA_PROFILE_APP_IDX].gatts_if, gl_profile_tab[OTA_PROFILE_APP_IDX].conn_id, ota_handle_table[index], (gl_profile_tab[OTA_PROFILE_APP_IDX].mtu_size - 3), value + offset, need_ack);
            if (ret) {
                ESP_LOGE(TAG, "%s send notification fail: %s\n", __func__, esp_err_to_name(ret));
                return ESP_FAIL;
            }
            offset += (gl_profile_tab[OTA_PROFILE_APP_IDX].mtu_size - 3);
        }

        if ((length - offset) > 0) {
            ret = esp_ble_gatts_send_indicate(gl_profile_tab[OTA_PROFILE_APP_IDX].gatts_if, gl_profile_tab[OTA_PROFILE_APP_IDX].conn_id, ota_handle_table[index], (length - offset), value + offset, need_ack);
            if (ret) {
                ESP_LOGE(TAG, "%s send notification fail: %s\n", __func__, esp_err_to_name(ret));
                return ESP_FAIL;
            }
        }
    }

    return ESP_OK;
}

esp_err_t esp_ble_ota_notification_data(esp_ble_ota_char_t ota_char, uint8_t *value, uint8_t length)
{
    esp_err_t ret = ESP_FAIL;

    switch (ota_char) {
    case RECV_FW_CHAR:
        if (ota_notification.recv_fw_ntf_enable) {
            ret = esp_ble_ota_send_indication(RECV_FW_CHAR_VAL_IDX, value, length, false);
        } else {
            ESP_LOGE(TAG, "notify isn't enable");
        }
        break;
    case OTA_STATUS_CHAR:
        if (ota_notification.process_bar_ntf_enable) {
            ret = esp_ble_ota_send_indication(OTA_STATUS_CHAR_VAL_IDX, value, length, false);
        } else {
            ESP_LOGE(TAG, "notify isn't enable");
        }
        break;
    case CMD_CHAR:
        if (ota_notification.command_ntf_enable) {
            ret = esp_ble_ota_send_indication(CMD_CHAR_VAL_IDX, value, length, false);
        } else {
            ESP_LOGE(TAG, "notify isn't enable");
        }
        break;
    case CUS_CHAR:
        if (ota_notification.customer_ntf_enable) {
            ret = esp_ble_ota_send_indication(CUS_CHAR_VAL_IDX, value, length, false);
        } else {
            ESP_LOGE(TAG, "notify isn't enable");
        }
        break;
    default:
        ret = ESP_FAIL;
        break;
    }

    if (ret) {
        ESP_LOGE(TAG, "%s notification fail: %s\n", __func__, esp_err_to_name(ret));
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t esp_ble_ota_recv_fw_data_callback(esp_ble_ota_recv_fw_cb_t callback)
{
    ota_cb_fun_t.recv_fw_cb = callback;
    return ESP_OK;
}

esp_err_t esp_ble_ota_host_init(void)
{

    /* set the security iocap & auth_req & key size & init key response key parameters to the stack*/
    esp_ble_auth_req_t auth_req = ESP_LE_AUTH_BOND;     //bonding with peer device after authentication
    esp_ble_io_cap_t iocap = ESP_IO_CAP_NONE;           //set the IO capability to No output No input
    uint8_t key_size = 16;      //the key size should be 7~16 bytes
    uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size, sizeof(uint8_t));
    /* If your BLE device act as a Slave, the init_key means you hope which types of key of the master should distribute to you,
    and the response key means which key you can distribute to the Master;
    If your BLE device act as a master, the response key means you hope which types of key of the slave should distribute to you,
    and the init key means which key you can distribute to the slave. */
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));

    return ESP_OK;
}

void app_ble_init( void )
{
    esp_err_t ret;
    ble_connected = false;

    /* Read device configuration */
    g_pstru_config = pstru_WARMER_Get_Config ();

    uint8_t mac_addr[8] = {0};
    esp_efuse_mac_get_default(mac_addr);
    snprintf(device_data.device_Id, DEVICE_SERIAL_NUM_MAX_LEN, "%02X%02X%02X%02X%02X%02X",
            mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    mac_addr[5] += 2;
    memcpy(device_data.ble_mac, mac_addr, 6);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret)
    {
        APP_DEBUG(TAG, "initialize controller failed: %s\n", esp_err_to_name(ret));
        return;
    }

    ble_connected = false;
    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret)
    {
        APP_DEBUG(TAG, "enable controller failed: %s\n", esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_init();
    if (ret)
    {
        APP_DEBUG(TAG, "init bluetooth failed: %s\n", esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_enable();
    if (ret)
    {
        APP_DEBUG(TAG, "enable bluetooth failed: %s\n", esp_err_to_name(ret));
        return;
    }

    ret = esp_ble_gatts_register_callback(app_ble_gatts_event_handler);
    if (ret)
    {
        APP_DEBUG(TAG, "gatts register error, error code = %x\r\n", ret);
        return;
    }

    ret = esp_ble_gap_register_callback(app_ble_gap_event_handler);
    if (ret)
    {
        APP_DEBUG(TAG, "gap register error, error code = %x\r\n", ret);
        return;
    }
     
}


void app_ble_start (void)
{
   esp_err_t ret;

    ret = esp_ble_gatts_app_register(PROFILE_APP_ID);
    if (ret)
    {
        APP_DEBUG(TAG, "gatts app register error, error code = %x\r\n", ret);
        return;
    }

     vTaskDelay(pdMS_TO_TICKS(100));  

    ret = esp_ble_gatts_app_register(OTA_PROFILE_APP_IDX);
    if (ret)
    {
        APP_DEBUG(TAG, "gatts app register error, error code = %x\r\n", ret);
        return;
    }

     vTaskDelay(pdMS_TO_TICKS(100));  

    ret = esp_ble_gatts_app_register(DIS_PROFILE_APP_IDX);
    if (ret)
    {
        APP_DEBUG(TAG, "gatts app register error, error code = %x\r\n", ret);
        return;
    }

    vTaskDelay(pdMS_TO_TICKS(100));   
    if (esp_ble_ota_host_init() != ESP_OK) {
        ESP_LOGE(TAG, "%s ble ota host init fail: %s\n", __func__, esp_err_to_name(ESP_FAIL));
        return;
    }

    ret = esp_ble_gatt_set_local_mtu(500);
    if (ret)
    {
        APP_DEBUG(TAG, "set local  MTU failed, error code = %x\r\n", ret);
    }

    ble_started = true;
    return;

}


bool app_ble_stop (void)
{
    ble_connected = false;
    ble_started = false;

    esp_bluedroid_disable();
    esp_bluedroid_deinit();  //**
    esp_bt_controller_disable();

    return true;
}


bool app_ble_isStarted (void)
{
    return ble_started;
}


bool app_ble_IsConnected (void)
{
    return ble_connected;
}


void app_ble_send (uint8_t *data, uint16_t data_len)
{
    if( ble_connected == false )
        return;

    esp_err_t err = esp_ble_gatts_send_indicate(gl_profile_tab[PROFILE_APP_ID].gatts_if, gl_profile_tab[PROFILE_APP_ID].conn_id,
                                                gl_profile_tab[PROFILE_APP_ID].char_handle, data_len, data, false);
    if( err != ESP_OK )
    {
        APP_DEBUG(TAG, "send fail. %d\r\n", err);
    }
}

static void app_ble_data_receive (uint16_t conn_id, uint8_t *data, uint16_t data_len)
{
    if (s_ota_mode) {
        return;
    }
    APP_DEBUG(TAG, "BLE recv = ");
    for( uint8_t i = 0; i < data_len; i++)
    {
        APP_DEBUG_RAW("%02X ", data[i]);
    }
    APP_DEBUG_RAW("\r\n");
    app_ble_parse(data, data_len);
}

static void app_ble_parse (uint8_t *data, uint16_t data_len) //from app
{
    if( data_len < BLE_PACKET_MIN_LENGTH )
        return;

    v_WARMER_Keep_Awaken ();

    ble_packet_t ble_packet = {0};
    memcpy(ble_packet.header.h, data, BLE_PACKET_HEADER_LENGTH);
    if( (ble_packet.header.p.start != BLE_PACKET_START_BYTE) ||
        (ble_packet.header.p.from != BLE_FROM_APP) ||
        (ble_packet.header.p.data_length != (data_len - BLE_PACKET_MIN_LENGTH)) ||
        (data[data_len -1] != BLE_PACKET_END_BYTE)
        )
    {
        return;
    }

    uint8_t checksum = data[data_len - 2];
    if( checksum != app_ble_checksum(data, data_len - 2) )
    {
        return;
    }

    if( ble_packet.header.p.data_length > 0  )
    {
        memcpy(ble_packet.data, &data[BLE_PACKET_HEADER_LENGTH], ble_packet.header.p.data_length);
    }

    if( ble_packet.header.p.type == BLE_TYPE_UPDATE_DATA_CONTROL )  //data and control command
    {
        // uint8_t u8_control_len = data_len - BLE_PACKET_MIN_LENGTH;
        // if ((u8_control_len != BLE_DATA_CONTROL_LEN_V1) &&
        //     (u8_control_len != BLE_DATA_CONTROL_LEN_V2) && 
        //     ((u8_control_len != BLE_DATA_CONTROL_LEN_V3)))
        // {
        //     return;
        // }

//      Temperature Unit
        if( ble_packet.data[0] == TEMP_UNIT_C )
            g_pstru_config->enm_temp_unit = TEMP_UNIT_C;
        else if( ble_packet.data[0] == TEMP_UNIT_F )
            g_pstru_config->enm_temp_unit = TEMP_UNIT_F;

//      temperature setting
        uint8_t u8_setpoint = (ble_packet.data[1] < TEMPERATURE_SETTING_MIN) ? TEMPERATURE_SETTING_MIN :
                              (ble_packet.data[1] > TEMPERATURE_SETTING_MAX) ? TEMPERATURE_SETTING_MAX :
                               ble_packet.data[1];
        if (g_pstru_config->u8_temp_setpoint != u8_setpoint)
        {
            app_notify_soft();
            if (g_pstru_config->u8_heater_mode != HEATER_M_FROZEN)
            {
                g_pstru_config->u8_temp_setpoint = u8_setpoint;
            }
        }

//      heater enable
        if( ble_packet.data[2] == 0x00 )//heater disabled
        {
            device_data.heater_off_requested = true;
            device_data.heater_enabled = false;
        }
        else if( ble_packet.data[2] == 0x01 )//heater enabled
        {
            device_data.heater_enabled = true;
            APP_DEBUG(TAG, "BLE: Heater enabled %d\r\n", ble_packet.data[2]);
        }
        
//      heater mode
        if( ble_packet.data[3] == 0x00 )
        {
            g_pstru_config->u8_heater_mode = HEATER_M_MILK;
            g_pstru_config->u8_temp_setpoint =
                (g_pstru_config->u8_temp_setpoint < TEMPERATURE_SETTING_MIN) ? TEMPERATURE_SETTING_MIN :
                (g_pstru_config->u8_temp_setpoint > TEMPERATURE_SETTING_MAX) ? TEMPERATURE_SETTING_MAX :
                g_pstru_config->u8_temp_setpoint;
        }
        else if( ble_packet.data[3] == 0x01 )
        {
            g_pstru_config->u8_heater_mode = HEATER_M_WATER;
            g_pstru_config->u8_temp_setpoint =
                (g_pstru_config->u8_temp_setpoint < TEMPERATURE_SETTING_MIN) ? TEMPERATURE_SETTING_MIN :
                (g_pstru_config->u8_temp_setpoint > TEMPERATURE_SETTING_MAX) ? TEMPERATURE_SETTING_MAX :
                g_pstru_config->u8_temp_setpoint;
        }
        else if( ble_packet.data[3] == 0x02 )
        {
            g_pstru_config->u8_heater_mode = HEATER_M_FROZEN;
            g_pstru_config->u8_temp_setpoint = WARMER_FROZEN_SET_POINT_TEMP;
        }
		else if( ble_packet.data[3] == 0x03 )
        {
            g_pstru_config->u8_heater_mode = HEATER_M_BABYFOOD;
            g_pstru_config->u8_temp_setpoint =
                (g_pstru_config->u8_temp_setpoint < TEMPERATURE_SETTING_MIN) ? TEMPERATURE_SETTING_MIN :
                (g_pstru_config->u8_temp_setpoint > TEMPERATURE_SETTING_MAX) ? TEMPERATURE_SETTING_MAX :
                g_pstru_config->u8_temp_setpoint;
        }
        else if( ble_packet.data[3] == 0x04 )
        {
            //Deodorize Not yet available
        }
		
//      Buzzer setting
        if( ble_packet.data[4] == 0x00 )
        {
            g_pstru_config->b_buzzer_enabled = false;
        }
        else if( ble_packet.data[4] == 0x01 )
        {
        	g_pstru_config->enm_buzzer_volume = BUZZER_VOLUME_LEVEL_5	;
        	app_buzzer_update_volume();
            g_pstru_config->b_buzzer_enabled = true;
        }
        else if( ble_packet.data[4] == 0x02 )
        {
        	g_pstru_config->enm_buzzer_volume = BUZZER_VOLUME_LEVEL_1;
        	app_buzzer_update_volume();
            g_pstru_config->b_buzzer_enabled = true;
        }
        else if( ble_packet.data[4] == 0x03 )
		{
        	g_pstru_config->enm_buzzer_volume = BUZZER_VOLUME_LEVEL_2;
        	app_buzzer_update_volume();
			g_pstru_config->b_buzzer_enabled = true;
		}
        else if( ble_packet.data[4] == 0x04 )
		{
        	g_pstru_config->enm_buzzer_volume = BUZZER_VOLUME_LEVEL_3;
        	app_buzzer_update_volume();
			g_pstru_config->b_buzzer_enabled = true;
		}
        else if( ble_packet.data[4] == 0x05 )
		{
        	g_pstru_config->enm_buzzer_volume = BUZZER_VOLUME_LEVEL_4;
        	app_buzzer_update_volume();
			g_pstru_config->b_buzzer_enabled = true;
		}

//      Wigg Mode
        if( ble_packet.data[5] == 0x00 )
        {
            g_pstru_config->b_wigg_mode = false;
        }
        else if( ble_packet.data[5] == 0x01 )
        {
            g_pstru_config->b_wigg_mode = true;
        }
        //Power control
        if( ble_packet.data[6] == 0x00 )
        {
            device_data.power = PWR_OFF;
        }

        //volume setting
        // if (u8_control_len >= BLE_DATA_CONTROL_LEN_V2)
        // {
            /* Fluid volume level */
            if ((ble_packet.data[7] == WARMER_VOLUME_120) ||
                (ble_packet.data[7] == WARMER_VOLUME_240))
            {
                g_pstru_config->enm_volume_level = ble_packet.data[7];
            }
        // }

        //keep-warm period
        // if (u8_control_len == BLE_DATA_CONTROL_LEN_V3)
        // {
        //     /* keep warm period */
        //     (ble_packet.data[8]> WARMER_KEEP_WARMER_LEVEL_MAX) ? (g_pstru_config->u8_Keep_warm_level = 0) : (g_pstru_config->u8_Keep_warm_level = ble_packet.data[8]);
        // }
         if( ble_packet.data[8] == 0x00 )
        {
            g_pstru_config->u8_Keep_warm_level = 0;
        }
        else if( ble_packet.data[8] == 0x01 )
        {
        	g_pstru_config->u8_Keep_warm_level = 1;
        }
        else if( ble_packet.data[8] == 0x02 )
        {
        	g_pstru_config->u8_Keep_warm_level = 2;
        }
        else if( ble_packet.data[8] == 0x03 )
		{
        	g_pstru_config->u8_Keep_warm_level = 3;
		}
        else if( ble_packet.data[8] == 0x04 )
		{
        	g_pstru_config->u8_Keep_warm_level = 4;
		}
        else if( ble_packet.data[8] == 0x05 )
		{
        	g_pstru_config->u8_Keep_warm_level = 5;
		}else{
            g_pstru_config->u8_Keep_warm_level = 0;
        }

        //Vibration Setting
        if( ble_packet.data[9] == 0x00 )
        {
            g_pstru_config->b_vibration_enabled = false;
        }
        else if( ble_packet.data[9] == 0x01 )
        {
            g_pstru_config->b_vibration_enabled = true;
        }

        //Language Update
        uint8_t lang = g_pstru_config->enm_device_language;
        if(ble_packet.data[10] == 0x00){
            g_pstru_config->enm_device_language = DEVICE_LANGUAGE_EN_US;
        }else if(ble_packet.data[10] == 0x01){
            g_pstru_config->enm_device_language = DEVICE_LANGUAGE_ZH_SG;
        }else if(ble_packet.data[10] == 0x02){
            g_pstru_config->enm_device_language = DEVICE_LANGUAGE_JA_JP;
        }else if(ble_packet.data[10] == 0x03){
            g_pstru_config->enm_device_language = DEVICE_LANGUAGE_KO_KR;
        }
      
        /* Store new configuration persistently */
        enm_WARMER_Save_Config ();
        ui_post_update(UI_UPDATE_ALL, 0);  
        vTaskDelay(pdMS_TO_TICKS(20));
        APP_DEBUG(TAG, "APP: Temp Unit %s\r\n", (g_pstru_config->enm_temp_unit == TEMP_UNIT_C)? "C" : "F");
        APP_DEBUG(TAG, "APP: Setting Temp %d\r\n", g_pstru_config->u8_temp_setpoint);
        APP_DEBUG(TAG, "APP: Heater enabled %s\r\n", (device_data.heater_enabled == true)? "TRUE" : "FALSE");
        APP_DEBUG(TAG, "APP: Heater Mode %s\r\n", (g_pstru_config->u8_heater_mode == HEATER_M_FROZEN) ? "Frozen" :
                                                  (g_pstru_config->u8_heater_mode == HEATER_M_MILK) ? "Milk" :
												  (g_pstru_config->u8_heater_mode == HEATER_M_WATER) ? "Water" : "Baby Food");
        APP_DEBUG(TAG, "APP: Buzzer enabled %s\r\n", g_pstru_config->b_buzzer_enabled ? "TRUE" : "FALSE");
        APP_DEBUG(TAG, "APP: Buzzer Volume %d\r\n", g_pstru_config->enm_buzzer_volume);
        APP_DEBUG(TAG, "APP: Vibration enabled %s\r\n", g_pstru_config->b_vibration_enabled ? "TRUE" : "FALSE");
        APP_DEBUG(TAG, "APP: Wigg Mode %s\r\n", g_pstru_config->b_wigg_mode ? "TRUE" : "FALSE");
        APP_DEBUG(TAG, "APP: Volume %s\r\n", (g_pstru_config->enm_volume_level == WARMER_VOLUME_120) ? "120ml" : "240ml");

        if(lang != g_pstru_config->enm_device_language){
                    APP_DEBUG(TAG, "APP: Device Language %s\r\n", (g_pstru_config->enm_device_language == DEVICE_LANGUAGE_EN_US) ? "English"  :
                                                                  (g_pstru_config->enm_device_language == DEVICE_LANGUAGE_ZH_SG) ? "Chinese"  :
                                                                  (g_pstru_config->enm_device_language == DEVICE_LANGUAGE_JA_JP) ? "Japanese" :
                                                                  (g_pstru_config->enm_device_language == DEVICE_LANGUAGE_KO_KR) ? "Korean"   : "Unknown");
        
                    vTaskDelay(pdMS_TO_TICKS(20));
	                esp_restart();
        }

    }
    else if (ble_packet.header.p.type == BLE_TYPE_WIFI_CREDENTIAL) //wifi command
    {
        // APP_DEBUG(TAG, " WIFI MODE ");
    }else if (ble_packet.header.p.type == BLE_TYPE_OTA_CONTROL)
    {
        
        if (ble_packet.header.p.data_length >= 1) {
            if (ble_packet.data[0] == 0x01) {           // start OTA
                if (!s_ota_mode) {
                    APP_DEBUG(TAG, " BLE OTA MODE: ENABLED \n");
                    s_ota_mode = true;
                    device_data.ota_mode = s_ota_mode;
                    vTaskDelay (pdMS_TO_TICKS (20));
                    ota_task_init();
                    
                    // vTaskDelay (pdMS_TO_TICKS (1000)); 
                }
            } else if (ble_packet.data[0] == 0x00) {    // optional: cancel (no-op in practice)
                // s_ota_mode = false;
                APP_DEBUG(TAG, " BLE OTA MODE: DISABLED \n");
                esp_restart();
            }
        }
    }
}


static void app_ble_packing_data (ble_packet_t *ble_packet) //to app
{
    if (ble_connected == false)
    {
        return;
    }

    WARMER_state_t enm_warmer_state = enm_WARMER_Get_State();
    uint8_t data_len = 0;

    ble_packet->header.p.start  = BLE_PACKET_START_BYTE;
    ble_packet->header.p.from   = BLE_FROM_DEVICE;
    ble_packet->header.p.type   = BLE_TYPE_UPDATE_DATA_CONTROL;

    ble_packet->data[data_len++] = (g_pstru_config->enm_temp_unit == TEMP_UNIT_C) ? 0x00 : 0x01;

    uint32_t ntc_temp = (uint32_t)(device_data.ntcTemp_C * 100);
    ble_packet->data[data_len++] = (ntc_temp >> 0 ) & 0xFF;
    ble_packet->data[data_len++] = (ntc_temp >> 8 ) & 0xFF;
    ble_packet->data[data_len++] = (ntc_temp >> 16) & 0xFF;
    ble_packet->data[data_len++] = (ntc_temp >> 24) & 0xFF;
    ble_packet->data[data_len++] = g_pstru_config->u8_temp_setpoint;
    ble_packet->data[data_len++] = (device_data.plug.plugdet == PLUG_NOT_DETECTED) ? 0x00 : 0x01;
    ble_packet->data[data_len++] = ((enm_warmer_state == WARMER_STATE_HEATING) ||
                                    (enm_warmer_state == WARMER_STATE_MAINTAINING) || (enm_warmer_state == WARMER_STATE_EXTENDED_MAINTAINING)) ? PWR_ON : PWR_OFF;
   ble_packet->data[data_len++] = (g_pstru_config->u8_heater_mode == HEATER_M_MILK) ? 0x00 :
                                   (g_pstru_config->u8_heater_mode == HEATER_M_WATER) ? 0x01 : 
								   (g_pstru_config->u8_heater_mode == HEATER_M_FROZEN)? 0x02 : 0x03;
    ble_packet->data[data_len++] = (device_data.tilt_input == TILT_LOW)? 0x00 : 0x01;
    ble_packet->data[data_len++] = (device_data.cover_input == COVER_LOW)? 0x01 : 0x00;
//    ble_packet->data[data_len++] = (device_data.battery.charging == BATTERY_NOT_CHARGING)? 0x00 : 0x01;
    ble_packet->data[data_len++] =  (device_data.ntc_temp_err == NTC_TEMP_LOW_ERR) ? 0x01 :
    								(device_data.ntc_temp_err == NTC_TEMP_HIGH_ERR) ? 0x02 : 0x00;

    ble_packet->data[data_len++] = (device_data.battery.miliVolts >> 0 ) & 0xFF;
    ble_packet->data[data_len++] = (device_data.battery.miliVolts >> 8 ) & 0xFF;
    ble_packet->data[data_len++] = device_data.battery.percent;
    ble_packet->data[data_len++] = (enm_warmer_state < WARMER_STATE_HEATING) ? device_data.battery.state : BATTERY_NORMAL;
    ble_packet->data[data_len++] = g_pstru_config->b_wigg_mode ? 0x01 : 0x00;
    ble_packet->data[data_len++] = !g_pstru_config->b_buzzer_enabled ? 0x00 :
                                    (g_pstru_config->enm_buzzer_volume == BUZZER_VOLUME_LEVEL_5)  ? 0x01 :
                                    (g_pstru_config->enm_buzzer_volume == BUZZER_VOLUME_LEVEL_1)  ? 0x02 :
                                    (g_pstru_config->enm_buzzer_volume == BUZZER_VOLUME_LEVEL_2)  ? 0x03 :
                                    (g_pstru_config->enm_buzzer_volume == BUZZER_VOLUME_LEVEL_3)  ? 0x04 :
                                    (g_pstru_config->enm_buzzer_volume == BUZZER_VOLUME_LEVEL_4)  ? 0x05 : 0x05;

    /* Fluid volume level */
    ble_packet->data[data_len++] = g_pstru_config->enm_volume_level;

    /* Firmware version */
    const esp_app_desc_t * pstru_app_desc = esp_ota_get_app_description ();
    int32_t s32_major = 0;
    int32_t s32_minor = 0;
    int32_t s32_patch = 0;
    int32_t s32_build = 0;
    sscanf (pstru_app_desc->version, "%d.%d.%d.%d", &s32_major, &s32_minor, &s32_patch, &s32_build);
    ble_packet->data[data_len++] = ((s32_patch & 0xF) << 4) | ((s32_build & 0xF) << 0);
    ble_packet->data[data_len++] = ((s32_major & 0xF) << 4) | ((s32_minor & 0xF) << 0);
    
    ble_packet->data[data_len++] = g_pstru_config->u8_Keep_warm_period;

    ble_packet->data[data_len++] = enm_WARMER_Get_Running_State();	//Warmer State
    ble_packet->data[data_len++] = g_pstru_config->b_vibration_enabled ? 0x01 : 0x00;
    ble_packet->data[data_len++] = device_data.warming_percent;
    ble_packet->data[data_len++] = g_pstru_config->enm_device_language == DEVICE_LANGUAGE_EN_US ? 0x00 :
                                   g_pstru_config->enm_device_language == DEVICE_LANGUAGE_ZH_SG ? 0x01 :
                                   g_pstru_config->enm_device_language == DEVICE_LANGUAGE_JA_JP ? 0x02 :
                                   g_pstru_config->enm_device_language == DEVICE_LANGUAGE_KO_KR ? 0x03 : 0x00;
    ble_packet->data[data_len++] = DEVICE_HARDWARE_VERSION;                               
    ble_packet->header.p.data_length = data_len;
}

void app_ble_update_data (void)
{
    static ble_packet_t ble_packet = {0};
    app_ble_packing_data(&ble_packet);

    static uint8_t buf[BLE_PACKET_MAX_LENGTH + 1] = {0};
    uint8_t buf_len = 0;
    memcpy(&buf[buf_len], ble_packet.header.h, BLE_PACKET_HEADER_LENGTH);
    buf_len += BLE_PACKET_HEADER_LENGTH;
    memcpy(&buf[buf_len], ble_packet.data, ble_packet.header.p.data_length);
    buf_len += ble_packet.header.p.data_length;
    uint8_t cs = app_ble_checksum(buf, buf_len);
    buf[buf_len++] = cs;
    buf[buf_len++] = BLE_PACKET_END_BYTE;
    app_ble_send( buf, buf_len );
    
}

static uint8_t app_ble_checksum (uint8_t *data, uint8_t data_len)
{
    uint8_t cs = 0;
    for (uint16_t i = 0; i < data_len; i++)
    {
        cs += data[i];
    }
    return cs;
}
