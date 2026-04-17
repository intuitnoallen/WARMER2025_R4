#ifndef _SQUARELINE_GC9A01_UI_H
#define _SQUARELINE_GC9A01_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"
#include "lv_i18n.h"

// SCREEN: ui_Splash
void ui_Splash_screen_init(void);
void ui_event_Splash(lv_event_t * e);
extern lv_obj_t * ui_Splash;
extern lv_obj_t * ui_Logo;
// CUSTOM VARIABLES

// SCREEN: ui_SplashWigg
void ui_SplashWigg_screen_init(void);
void ui_event_SplashWigg(lv_event_t * e);
extern lv_obj_t * ui_SplashWigg;
extern lv_obj_t * ui_PanelSplashWigg;
extern lv_obj_t * ui_LogoWigg;
extern lv_obj_t * ui_LabelWiggVersion;
// CUSTOM VARIABLES

// SCREEN: ui_Language
void ui_Language_screen_init(void);
void ui_event_Language(lv_event_t * e);
extern lv_obj_t * ui_Language;
void ui_event_PanelEnglish(lv_event_t * e);
extern lv_obj_t * ui_PanelEnglish;
extern lv_obj_t * ui_LabelEnglish;
extern lv_obj_t * ui_LabelEnglishInfo;
void ui_event_PanelChinese(lv_event_t * e);
extern lv_obj_t * ui_PanelChinese;
extern lv_obj_t * ui_LabelChinese;
extern lv_obj_t * ui_LabelChineseInfo;
void ui_event_PanelJapanese(lv_event_t * e);
extern lv_obj_t * ui_PanelJapanese;
extern lv_obj_t * ui_LabelJapanese;
extern lv_obj_t * ui_LabelJapaneseInfo;
void ui_event_PanelKorean(lv_event_t * e);
extern lv_obj_t * ui_PanelKorean;
extern lv_obj_t * ui_LabelKorean;
extern lv_obj_t * ui_LabelKoreanInfo;
// CUSTOM VARIABLES


// SCREEN: ui_Error
void ui_Error_screen_init(void);
void ui_event_Error(lv_event_t * e);
extern lv_obj_t * ui_Error;
void ui_event_PanelERR1(lv_event_t * e);
extern lv_obj_t * ui_PanelERR1;
extern lv_obj_t * ui_IconE1;
extern lv_obj_t * ui_LabelE1;
void ui_event_PanelERR2(lv_event_t * e);
extern lv_obj_t * ui_PanelERR2;
extern lv_obj_t * ui_IconE2;
extern lv_obj_t * ui_LabelE2;
void ui_event_PanelERR3(lv_event_t * e);
extern lv_obj_t * ui_PanelERR3;
extern lv_obj_t * ui_LabelE3;
extern lv_obj_t * ui_DescE3;
void ui_event_PanelERR4(lv_event_t * e);
extern lv_obj_t * ui_PanelERR4;
extern lv_obj_t * ui_LabelE4;
extern lv_obj_t * ui_DescE4;
extern lv_obj_t * ui_IconE4;
void ui_event_PanelERR5(lv_event_t * e);
extern lv_obj_t * ui_PanelERR5;
extern lv_obj_t * ui_LabelE5;
extern lv_obj_t * ui_DescE5;
extern lv_obj_t * ui_IconE5;
void ui_event_PanelERR25(lv_event_t * e);
extern lv_obj_t * ui_PanelERR25;
extern lv_obj_t * ui_IconBatLow;
extern lv_obj_t * ui_DescE25;
void ui_event_PanelERR8(lv_event_t * e);
extern lv_obj_t * ui_PanelERR8;
extern lv_obj_t * ui_DescE8;
extern lv_obj_t * ui_LabelE8;
void ui_event_PanelERR7(lv_event_t * e);
extern lv_obj_t * ui_PanelERR7;
extern lv_obj_t * ui_DescE7;
extern lv_obj_t * ui_LabelE7;
extern lv_obj_t * ui_IconE7;
// CUSTOM VARIABLES

// SCREEN: ui_About
void ui_About_screen_init(void);
void ui_event_About(lv_event_t * e);
extern lv_obj_t * ui_About;
void ui_event_PanelAboutInfo(lv_event_t * e);
extern lv_obj_t * ui_PanelAboutInfo;
extern lv_obj_t * ui_ImageCompanyLogo;
extern lv_obj_t * ui_LabelFWversion;
extern lv_obj_t * ui_LabelHWversion;
extern lv_obj_t * ui_LabelManufacture;
// CUSTOM VARIABLES

// SCREEN: ui_BluetoothApp
void ui_BluetoothApp_screen_init(void);
void ui_event_BluetoothApp(lv_event_t * e);
extern lv_obj_t * ui_BluetoothApp;
void ui_event_PanelBluetoothAppScreen(lv_event_t * e);
extern lv_obj_t * ui_PanelBluetoothAppScreen;
extern lv_obj_t * ui_ImageAppQRcode;
extern lv_obj_t * ui_LabelAppQRcode;
// CUSTOM VARIABLES


// SCREEN: ui_UserInstruction
void ui_UserInstruction_screen_init(void);
void ui_event_UserInstruction(lv_event_t * e);
extern lv_obj_t * ui_UserInstruction;
void ui_event_PanelUserInstruction(lv_event_t * e);
extern lv_obj_t * ui_PanelUserInstruction;
extern lv_obj_t * ui_ImageUserInstruction;
extern lv_obj_t * ui_LabelUserInstruction;
// CUSTOM VARIABLES

// SCREEN: ui_Thermometer
void ui_Thermometer_screen_init(void);
void ui_event_Thermometer(lv_event_t * e);
extern lv_obj_t * ui_Thermometer;
void ui_event_PanelTempMeasuring(lv_event_t * e);
// CUSTOM VARIABLES

// SCREEN: ui_ThermometerReading
void ui_ThermometerReading_screen_init(void);
void ui_event_ThermometerReading(lv_event_t * e);
extern lv_obj_t * ui_ThermometerReading;
extern lv_obj_t * ui_PanelTempMeasuring;
extern lv_obj_t * ui_TempMeasuringFill;
extern lv_obj_t * ui_LabelTempMeasuringCount;
extern lv_obj_t * ui_LabelTempMeasuring;
void ui_event_PanelTempMeasured(lv_event_t * e);
extern lv_obj_t * ui_PanelTempMeasured;
extern lv_obj_t * ui_LabelTempReading;
extern lv_obj_t * ui_LabelTempMeasured;
extern lv_obj_t * ui_IconTempMeasuredUnit;
// CUSTOM VARIABLES

// SCREEN: ui_OTA
void ui_OTA_screen_init(void);
void ui_event_OTA(lv_event_t * e);
extern lv_obj_t * ui_OTA;
extern lv_obj_t * ui_PanelOTAProgress;
extern lv_obj_t * ui_LabelFWProgress;
extern lv_obj_t * ui_FirmwareUpdateBar;
extern lv_obj_t * ui_LabelFWUpdateInfo;
// void ui_event_PanelOTAUpdated(lv_event_t * e);
extern lv_obj_t * ui_PanelOTAUpdated;
extern lv_obj_t * ui_LabelFWUpdated;
extern lv_obj_t * ui_IconFWUpdatesuccess;
extern lv_obj_t * ui_LabelFWUpdatedReboot;
void ui_event_PanelOTAFailed(lv_event_t * e);
extern lv_obj_t * ui_PanelOTAFailed;
extern lv_obj_t * ui_LabelFWFailed;
extern lv_obj_t * ui_IconFWUpdateFailed;
extern lv_obj_t * ui_LabelFWFailedReboot;
// CUSTOM VARIABLES


// SCREEN: ui_Main
void ui_Main_screen_init(void);
void ui_event_Main(lv_event_t * e);
extern lv_obj_t * ui_Main;
void ui_event_PanelMain(lv_event_t * e);
extern lv_obj_t * ui_PanelMain;
extern lv_obj_t * ui_LabelcurrentTemp;
extern lv_obj_t * ui_ImageTempUnitMain;
extern lv_obj_t * ui_LabelstartInfo;
extern lv_obj_t * ui_LabelBatPercent;
extern lv_obj_t * ui_BLEicon;
extern lv_obj_t * ui_Baticon;
extern lv_obj_t * ui_BatBar;
extern lv_obj_t * ui_BatCharging;
// CUSTOM VARIABLES

// SCREEN: ui_Menu
void ui_Menu_screen_init(void);
void ui_event_Menu(lv_event_t * e);
extern lv_obj_t * ui_Menu;
void ui_event_PanelModes(lv_event_t * e);
extern lv_obj_t * ui_PanelModes;
extern lv_obj_t * ui_IconModes;
extern lv_obj_t * ui_LabelModes;
void ui_event_PanelHeatingPower(lv_event_t * e);
extern lv_obj_t * ui_PanelHeatingPower;
extern lv_obj_t * ui_IconHeatingPower;
extern lv_obj_t * ui_LabelHeatingPower;
void ui_event_PanelTempUnit(lv_event_t * e);
extern lv_obj_t * ui_PanelTempUnit;
extern lv_obj_t * ui_IconTempUnit;
extern lv_obj_t * ui_LabelTempUnit;
void ui_event_PanelBottleSize(lv_event_t * e);
extern lv_obj_t * ui_PanelBottleSize;
extern lv_obj_t * ui_IconBottleSize;
extern lv_obj_t * ui_LabelBottleSize;
void ui_event_PanelKeepWarm(lv_event_t * e);
extern lv_obj_t * ui_PanelKeepWarm;
extern lv_obj_t * ui_IconKeepWarm;
extern lv_obj_t * ui_LabelKeepWarm;
void ui_event_PanelNotification(lv_event_t * e);
extern lv_obj_t * ui_PanelNotification;
extern lv_obj_t * ui_IconNotification;
extern lv_obj_t * ui_LabelNotification;
void ui_event_PanelLanguage(lv_event_t * e);
extern lv_obj_t * ui_PanelLanguage;
extern lv_obj_t * ui_IconLanguage;
extern lv_obj_t * ui_LabelLanguage;
void ui_event_PanelBluetoothApp(lv_event_t * e);
extern lv_obj_t * ui_PanelBluetoothApp;
extern lv_obj_t * ui_IconBluetoothApp;
extern lv_obj_t * ui_LabelBluetoothApp;
void ui_event_PanelInstructions(lv_event_t * e);
extern lv_obj_t * ui_PanelInstructions;
extern lv_obj_t * ui_IconInstructions;
extern lv_obj_t * ui_LabelInstructions;
void ui_event_PanelAbout(lv_event_t * e);
extern lv_obj_t * ui_PanelAbout;
extern lv_obj_t * ui_IconAbout;
extern lv_obj_t * ui_LabelAbout;
// CUSTOM VARIABLES

// SCREEN: ui_HeatingModes
void ui_HeatingModes_screen_init(void);
void ui_event_HeatingModes(lv_event_t * e);
extern lv_obj_t * ui_HeatingModes;
void ui_event_PanelMilk(lv_event_t * e);
extern lv_obj_t * ui_PanelMilk;
extern lv_obj_t * ui_IconMilk;
extern lv_obj_t * ui_LabelMilk;
void ui_event_PanelWaterFormula(lv_event_t * e);
extern lv_obj_t * ui_PanelWaterFormula;
extern lv_obj_t * ui_IconWaterFormula;
extern lv_obj_t * ui_LabelWaterFormula;
void ui_event_PanelDefreeze(lv_event_t * e);
extern lv_obj_t * ui_PanelDefreeze;
extern lv_obj_t * ui_IconDefreeze;
extern lv_obj_t * ui_LabelDefreeze;
void ui_event_PanelBabyFood(lv_event_t * e);
extern lv_obj_t * ui_PanelBabyFood;
extern lv_obj_t * ui_IconBabyFood;
extern lv_obj_t * ui_LabelBabyFood;
void ui_event_PanelDeodorize(lv_event_t * e);
extern lv_obj_t * ui_PanelDeodorize;
extern lv_obj_t * ui_IconDeodorize;
extern lv_obj_t * ui_LabelDeodorize;
// CUSTOM VARIABLES

// SCREEN: ui_HeatingPower
void ui_HeatingPower_screen_init(void);
void ui_event_HeatingPower(lv_event_t * e);
extern lv_obj_t * ui_HeatingPower;
void ui_event_PanelHeatingPowerLvl1(lv_event_t * e);
extern lv_obj_t * ui_PanelHeatingPowerLvl1;
extern lv_obj_t * ui_LabelHeatingPower1;
extern lv_obj_t * ui_LabelHeatingPowerLevel1;
void ui_event_PanelHeatingPowerLvl2(lv_event_t * e);
extern lv_obj_t * ui_PanelHeatingPowerLvl2;
extern lv_obj_t * ui_LabelHeatingPower2;
extern lv_obj_t * ui_LabelHeatingPowerLevel2;
void ui_event_PanelHeatingPowerLvl3(lv_event_t * e);
extern lv_obj_t * ui_PanelHeatingPowerLvl3;
extern lv_obj_t * ui_LabelHeatingPower3;
extern lv_obj_t * ui_LabelHeatingPowerLevel3;
void ui_event_PanelHeatingPowerLvl4(lv_event_t * e);
extern lv_obj_t * ui_PanelHeatingPowerLvl4;
extern lv_obj_t * ui_LabelHeatingPower4;
extern lv_obj_t * ui_LabelHeatingPowerLevel4;
void ui_event_PanelHeatingPowerLvl5(lv_event_t * e);
extern lv_obj_t * ui_PanelHeatingPowerLvl5;
extern lv_obj_t * ui_LabelHeatingPower5;
extern lv_obj_t * ui_LabelHeatingPowerLevel5;
// CUSTOM VARIABLES


// SCREEN: ui_TempUnitSetting
void ui_TempUnitSetting_screen_init(void);
void ui_event_TempUnitSetting(lv_event_t * e);
extern lv_obj_t * ui_TempUnitSetting;
void ui_event_PanelCelsius(lv_event_t * e);
extern lv_obj_t * ui_PanelCelsius;
extern lv_obj_t * ui_IconCelsius;
extern lv_obj_t * ui_LabelCelsius;
void ui_event_PanelFahrenheit(lv_event_t * e);
extern lv_obj_t * ui_PanelFahrenheit;
extern lv_obj_t * ui_IconFahrenheit_;
extern lv_obj_t * ui_LabelFahrenheit;
// CUSTOM VARIABLES

// SCREEN: ui_KeepWarm
void ui_KeepWarm_screen_init(void);
void ui_event_KeepWarm(lv_event_t * e);
extern lv_obj_t * ui_KeepWarm;
void ui_event_PanelKeepWarmOFF(lv_event_t * e);
extern lv_obj_t * ui_PanelKeepWarmOFF;
extern lv_obj_t * ui_IconKeepWarmOFF;
extern lv_obj_t * ui_LabelKeepWarmOFF;
void ui_event_PanelKeepWarmLvl1(lv_event_t * e);
extern lv_obj_t * ui_PanelKeepWarmLvl1;
extern lv_obj_t * ui_LabelKeepWarm1;
extern lv_obj_t * ui_LabelKeepWarmM1;
void ui_event_PanelKeepWarmLvl2(lv_event_t * e);
extern lv_obj_t * ui_PanelKeepWarmLvl2;
extern lv_obj_t * ui_LabelKeepWarm2;
extern lv_obj_t * ui_LabelKeepWarmM2;
void ui_event_PanelKeepWarmLvl3(lv_event_t * e);
extern lv_obj_t * ui_PanelKeepWarmLvl3;
extern lv_obj_t * ui_LabelKeepWarm3;
extern lv_obj_t * ui_LabelKeepWarmM3;
void ui_event_PanelKeepWarmLvl4(lv_event_t * e);
extern lv_obj_t * ui_PanelKeepWarmLvl4;
extern lv_obj_t * ui_LabelKeepWarm4;
extern lv_obj_t * ui_LabelKeepWarmM4;
void ui_event_PanelKeepWarmLvl5(lv_event_t * e);
extern lv_obj_t * ui_PanelKeepWarmLvl5;
extern lv_obj_t * ui_LabelKeepWarm5;
extern lv_obj_t * ui_LabelKeepWarmM5;
// CUSTOM VARIABLES

// SCREEN: ui_Notification
void ui_Notification_screen_init(void);
void ui_event_Notification(lv_event_t * e);
extern lv_obj_t * ui_Notification;
void ui_event_PanelVibration(lv_event_t * e);
extern lv_obj_t * ui_PanelVibration;
extern lv_obj_t * ui_IconVibration;
extern lv_obj_t * ui_LabelVibration;
void ui_event_PanelBuzzer(lv_event_t * e);
extern lv_obj_t * ui_PanelBuzzer;
extern lv_obj_t * ui_IconBuzzer;
extern lv_obj_t * ui_LabelBuzzer;
void ui_event_PanelVolumeLvl1(lv_event_t * e);
extern lv_obj_t * ui_PanelVolumeLvl1;
extern lv_obj_t * ui_LabelVolume1;
extern lv_obj_t * ui_LabelLevel1;
void ui_event_PanelVolumeLvl2(lv_event_t * e);
extern lv_obj_t * ui_PanelVolumeLvl2;
extern lv_obj_t * ui_LabelVolume2;
extern lv_obj_t * ui_LabelLevel2;
void ui_event_PanelVolumeLvl3(lv_event_t * e);
extern lv_obj_t * ui_PanelVolumeLvl3;
extern lv_obj_t * ui_LabelVolume3;
extern lv_obj_t * ui_LabelLevel3;
void ui_event_PanelVolumeLvl4(lv_event_t * e);
extern lv_obj_t * ui_PanelVolumeLvl4;
extern lv_obj_t * ui_LabelVolume4;
extern lv_obj_t * ui_LabelLevel4;
void ui_event_PanelVolumeLvl5(lv_event_t * e);
extern lv_obj_t * ui_PanelVolumeLvl5;
extern lv_obj_t * ui_LabelVolume5;
extern lv_obj_t * ui_LabelLevel5;
// CUSTOM VARIABLES

// SCREEN: ui_Selected
void ui_Selected_screen_init(void);
extern lv_obj_t * ui_Selected;
extern lv_obj_t * ui_PanelSelected;
extern lv_obj_t * ui_ImageSelected;
extern lv_obj_t * ui_LabelCountDown;
extern lv_obj_t * ui_countDown;
extern lv_obj_t * ui_LabelSelectedInfo;
// CUSTOM VARIABLES

// SCREEN: ui_Done
void ui_Done_screen_init(void);
void ui_event_Done(lv_event_t * e);
extern lv_obj_t * ui_Done;
void ui_event_PanelDone(lv_event_t * e);
extern lv_obj_t * ui_PanelDone;
extern lv_obj_t * ui_IconDone;
extern lv_obj_t * ui_LabelDone;
void ui_event_PanelKeepingProgress(lv_event_t * e);
extern lv_obj_t * ui_PanelKeepingProgress;
extern lv_obj_t * ui_IconKeepingProgress;
extern lv_obj_t * ui_LabelKeepingProgress;
void ui_event_PanelChargeProgress(lv_event_t * e);
extern lv_obj_t * ui_PanelChargeProgress;
extern lv_obj_t * ui_ChargeArc;
extern lv_obj_t * ui_LabelChargeInfo;
extern lv_obj_t * ui_LabelChargePercent;
void ui_event_PanelChargingDone(lv_event_t * e);
extern lv_obj_t * ui_PanelChargingDone;
extern lv_obj_t * ui_IconChargingDone;
extern lv_obj_t * ui_LabelChargingDone;
// CUSTOM VARIABLES


// SCREEN: ui_Warming
void ui_Warming_screen_init(void);
void ui_event_Warming(lv_event_t * e);
extern lv_obj_t * ui_Warming;
void ui_event_PanelWarming(lv_event_t * e);
extern lv_obj_t * ui_PanelWarming;
extern lv_obj_t * ui_WarmingFiller;
extern lv_obj_t * ui_LabelWarmingPercent;
// extern lv_obj_t * ui_LabelRPM;
// extern lv_obj_t * ui_LabelPWM;
// extern lv_obj_t * ui_LabelPWMRPM;
// CUSTOM VARIABLES

// SCREEN: ui_TempSetting
void ui_TempSetting_screen_init(void);
void ui_event_TempSetting(lv_event_t * e);
extern lv_obj_t * ui_TempSetting;
void ui_event_PanelSettingTemp(lv_event_t * e);
extern lv_obj_t * ui_PanelSettingTemp;
extern lv_obj_t * ui_LabelsettingTemp;
extern lv_obj_t * ui_ImageTempSettingUnit;
extern lv_obj_t * ui_LabelstartInfoTempSetting;
// CUSTOM VARIABLES

// SCREEN: ui_BottleSize
void ui_BottleSize_screen_init(void);
void ui_event_BottleSize(lv_event_t * e);
extern lv_obj_t * ui_BottleSize;
void ui_event_PanelBottleSmall(lv_event_t * e);
extern lv_obj_t * ui_PanelBottleSmall;
extern lv_obj_t * ui_IconBottleSmall;
extern lv_obj_t * ui_LabelBottleSmall;
void ui_event_PanelBottleLarge(lv_event_t * e);
extern lv_obj_t * ui_PanelBottleLarge;
extern lv_obj_t * ui_IconBottleLarge;
extern lv_obj_t * ui_LabelBottleLarge;
// CUSTOM VARIABLES

// EVENTS

extern lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS
// LV_IMG_DECLARE(ui_img_fc_wigg_png);    // assets/FC_Wigg.png
LV_IMG_DECLARE(ui_img_e1_png);    // assets/E1.png
LV_IMG_DECLARE(ui_img_e2_png);    // assets/E2.png
LV_IMG_DECLARE(ui_img_bat_low_png);    // assets/BAT_LOW.png
LV_IMG_DECLARE(ui_img_fc_wigg_180_png);    // assets/FC_Wigg_180.png
LV_IMG_DECLARE(ui_img_fc_wigg_200_png);    // assets/FC_Wigg_200.png
// LV_IMG_DECLARE(ui_img_fc_wigg_s_png);    // assets/FC_Wigg_180.png
LV_IMG_DECLARE(ui_img_failed_png);    // assets/fail.png
LV_IMG_DECLARE(ui_img_temp_ci_png);    // assets/temp_Ci.png
LV_IMG_DECLARE(ui_img_ble_png);    // assets/ble.png
LV_IMG_DECLARE(ui_img_battery_png);    // assets/battery.png
LV_IMG_DECLARE(ui_img_bat_bar_png);    // assets/bat_bar.png
LV_IMG_DECLARE(ui_img_charging_png);    // assets/charging.png
LV_IMG_DECLARE(ui_img_heatmodes_png);    // assets/heatmodes.png
LV_IMG_DECLARE(ui_img_heating_pwr_png);    // assets/heating_pwr.png
LV_IMG_DECLARE(ui_img_temp_unit_png);    // assets/temp_unit.png
LV_IMG_DECLARE(ui_img_bottlesmall_png);    // assets/bottleSmall.png
LV_IMG_DECLARE(ui_img_keeping_warm_png);    // assets/keeping_warm.png
LV_IMG_DECLARE(ui_img_keeping_warm_off_png);    // assets/keeping_warm_off.png
LV_IMG_DECLARE(ui_img_notification_png);    // assets/notification.png
LV_IMG_DECLARE(ui_img_ble_app_png);    // assets/BLE_APP.png
LV_IMG_DECLARE(ui_img_user_instructions_png);    // assets/user_instructions.png
LV_IMG_DECLARE(ui_img_about_png);    // assets/about.png
LV_IMG_DECLARE(ui_img_milk_png);    // assets/milk.png
LV_IMG_DECLARE(ui_img_water_png);    // assets/water.png
LV_IMG_DECLARE(ui_img_defreeze_png);    // assets/defreeze.png
LV_IMG_DECLARE(ui_img_babyfood_png);    // assets/babyFood.png
LV_IMG_DECLARE(ui_img_deodorize_png);    // assets/deodorize.png
LV_IMG_DECLARE(ui_img_temp_c_png);    // assets/temp_C.png
LV_IMG_DECLARE(ui_img_temp_f_png);    // assets/temp_F.png
LV_IMG_DECLARE(ui_img_vibrate_png);    // assets/vibrate.png
LV_IMG_DECLARE(ui_img_volume_disable_png);    // assets/volume_disable.png
LV_IMG_DECLARE(ui_img_selected_png);    // assets/selected.png
LV_IMG_DECLARE(ui_img_poweroff_png);    // assets/select.png
LV_IMG_DECLARE(ui_img_bottlelarge_png);    // assets/bottleLarge.png
LV_IMG_DECLARE(ui_img_qrcodeapp_png);    // assets/QRcodeApp.png
LV_IMG_DECLARE(ui_img_qrcodeavoidleaking_png);    // assets/QRcodeAvoidLeaking.png
LV_IMG_DECLARE(ui_img_qrcodewebsite_png);    // assets/websiteHome_S.png
LV_IMG_DECLARE(ui_img_bat_full_png);    // assets/BAT_FULL.png
LV_IMG_DECLARE(ui_img_language_png);    // assets/language.png





// FONTS
// LV_FONT_DECLARE(ui_font_Segoe_UI_B20);
LV_FONT_DECLARE(ui_font_WIGG_B20);
// LV_FONT_DECLARE(ui_font_Segoe_UI_B25);
LV_FONT_DECLARE(ui_font_WIGG_B35);
LV_FONT_DECLARE(ui_font_Segoe_UI_B85);
LV_FONT_DECLARE(ui_font_Segoe_UI_SB14);
// LV_FONT_DECLARE(ui_font_Segoe_UI_SB50);
LV_FONT_DECLARE(ui_font_Segoe_UI_SB57);
// LV_FONT_DECLARE(ui_font_Segoe_UI_SB65);

// UI INIT
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
