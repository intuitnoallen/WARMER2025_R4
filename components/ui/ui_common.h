
#ifdef __cplusplus
extern "C" {
#endif
#include "ui.h"
#include "ui_events.h"
#include "common_hdr.h"

#define MOVE_UP_ANIM_MS         200
#define MOVE_DOWN_ANIM_MS       200
#define MOVE_LEFT_ANIM_MS       000

// In ui.h
typedef enum {
    UI_UPDATE_BLE_STATUS,
    UI_UPDATE_SETTING_TEMP,
    UI_UPDATE_THERMOMETER_TIME,
    UI_UPDATE_THERMOMETER_TEMP,
    UI_UPDATE_DONE_SCR,
    UI_UPDATE_WARMING_PERCENT,
    UI_UPDATE_BATTERY_PERCENT,
    UI_UPDATE_CHARGING_ICON,
    UI_UPDATE_BLE_OTA,
    UI_UPDATE_TEXT,
    UI_UPDATE_ERROR,
    UI_UPDATE_MAIN,
    UI_UPDATE_ALL,
} ui_update_t;

typedef struct {
    ui_update_t type;
    int32_t value;   // can be bool, int, percent, etc.
} ui_update_msg_t;

LV_IMG_DECLARE(ui_img_temp_fi_png);    // assets/temp_Ci.png

// static _lock_t lvgl_api_lock;

extern lv_indev_t *indev; 
extern lv_group_t *groupMain;
extern lv_group_t *groupWarming;
extern lv_group_t *groupMenu;
extern lv_group_t *groupHeatingModes;
extern lv_group_t *groupTempSetting;
extern lv_group_t *groupTempUnitSetting;
extern lv_group_t *groupThermometer;
extern lv_group_t *groupThermometerReading;
extern lv_group_t *groupOTAupdate;
extern lv_group_t *groupNotification;
extern lv_group_t *groupAbout;
extern lv_group_t *groupBluetoothApp;
extern lv_group_t *groupUserInstructions;
extern lv_group_t *groupError;
extern lv_group_t *groupLanguage;
extern lv_group_t *groupBottleSize;
extern lv_group_t *groupKeepWarm;
extern lv_group_t *groupDone;

extern lv_obj_t *prev_scr;
extern lv_obj_t *curr_scr;
extern lv_obj_t *focused_obj;

extern bool isErrorUpdateRequired;
extern bool device_stable; //primarily used for stable battery readings at boot
extern bool isMeasuringTemp;
extern float average_measured_temp;

extern void my_focus_cb(lv_group_t *group);
extern void keypad_enable(void);
extern void deinit_LCD(void);
void ui_refresh(ui_update_msg_t *msg);
void u8_warming_percent_update();
void ui_post_update(ui_update_t type, int32_t value); 

void u8_updateSettingTemperature();
void u8_updateBatteryPercent();
void b_updateChargingIcon();
void updateThermometerTime(int32_t t);
void updateThermometerTemp();
void updateAllConfigurations();
// void ToMainScreenNoAnim();
void ToErrorScreen(uint8_t e);
void ToOtaScreen(uint8_t ota);
void updateError(uint8_t e);
void updateDoneScreen(uint8_t d);
void ToDoneScreen(uint8_t done);
#ifdef __cplusplus
} /*extern "C"*/
#endif

