#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
uint8_t LV_EVENT_TEMP_INC = 50;


// SCREEN: ui_SplashWigg
void ui_SplashWigg_screen_init(void);
void ui_event_SplashWigg(lv_event_t * e);
lv_obj_t * ui_SplashWigg;
lv_obj_t * ui_PanelSplashWigg;
lv_obj_t * ui_LogoWigg;
lv_obj_t * ui_LabelWiggVersion;
// CUSTOM VARIABLES

// SCREEN: ui_Language
void ui_Language_screen_init(void);
void ui_event_Language(lv_event_t * e);
lv_obj_t * ui_Language;
void ui_event_PanelEnglish(lv_event_t * e);
lv_obj_t * ui_PanelEnglish;
lv_obj_t * ui_LabelEnglish;
lv_obj_t * ui_LabelEnglishInfo;
void ui_event_PanelChinese(lv_event_t * e);
lv_obj_t * ui_PanelChinese;
lv_obj_t * ui_LabelChinese;
lv_obj_t * ui_LabelChineseInfo;
void ui_event_PanelJapanese(lv_event_t * e);
lv_obj_t * ui_PanelJapanese;
lv_obj_t * ui_LabelJapanese;
lv_obj_t * ui_LabelJapaneseInfo;
void ui_event_PanelKorean(lv_event_t * e);
lv_obj_t * ui_PanelKorean;
lv_obj_t * ui_LabelKorean;
lv_obj_t * ui_LabelKoreanInfo;
// CUSTOM VARIABLES


// SCREEN: ui_Error
void ui_Error_screen_init(void);
void ui_event_Error(lv_event_t * e);
lv_obj_t * ui_Error;
void ui_event_PanelERR1(lv_event_t * e);
lv_obj_t * ui_PanelERR1;
lv_obj_t * ui_IconE1;
lv_obj_t * ui_LabelE1;
void ui_event_PanelERR2(lv_event_t * e);
lv_obj_t * ui_PanelERR2;
lv_obj_t * ui_IconE2;
lv_obj_t * ui_LabelE2;
void ui_event_PanelERR3(lv_event_t * e);
lv_obj_t * ui_PanelERR3;
lv_obj_t * ui_LabelE3;
lv_obj_t * ui_DescE3;
void ui_event_PanelERR4(lv_event_t * e);
lv_obj_t * ui_PanelERR4;
lv_obj_t * ui_LabelE4;
lv_obj_t * ui_DescE4;
lv_obj_t * ui_IconE4;
void ui_event_PanelERR5(lv_event_t * e);
lv_obj_t * ui_PanelERR5;
lv_obj_t * ui_LabelE5;
lv_obj_t * ui_DescE5;
lv_obj_t * ui_IconE5;
void ui_event_PanelERR25(lv_event_t * e);
lv_obj_t * ui_PanelERR25;
lv_obj_t * ui_IconBatLow;
lv_obj_t * ui_DescE25;
void ui_event_PanelERR8(lv_event_t * e);
lv_obj_t * ui_PanelERR8;
lv_obj_t * ui_DescE8;
lv_obj_t * ui_LabelE8;
void ui_event_PanelERR7(lv_event_t * e);
lv_obj_t * ui_PanelERR7;
lv_obj_t * ui_DescE7;
lv_obj_t * ui_LabelE7;
lv_obj_t * ui_IconE7;
// CUSTOM VARIABLES

// SCREEN: ui_About
void ui_About_screen_init(void);
void ui_event_About(lv_event_t * e);
lv_obj_t * ui_About;
void ui_event_PanelAboutInfo(lv_event_t * e);
lv_obj_t * ui_PanelAboutInfo;
lv_obj_t * ui_ImageCompanyLogo;
lv_obj_t * ui_LabelFWversion;
lv_obj_t * ui_LabelHWversion;
lv_obj_t * ui_LabelManufacture;
// CUSTOM VARIABLES

// SCREEN: ui_BluetoothApp
void ui_BluetoothApp_screen_init(void);
void ui_event_BluetoothApp(lv_event_t * e);
lv_obj_t * ui_BluetoothApp;
void ui_event_PanelBluetoothAppScreen(lv_event_t * e);
lv_obj_t * ui_PanelBluetoothAppScreen;
lv_obj_t * ui_ImageAppQRcode;
lv_obj_t * ui_LabelAppQRcode;
// CUSTOM VARIABLES


// SCREEN: ui_UserInstruction
void ui_UserInstruction_screen_init(void);
void ui_event_UserInstruction(lv_event_t * e);
lv_obj_t * ui_UserInstruction;
void ui_event_PanelUserInstruction(lv_event_t * e);
lv_obj_t * ui_PanelUserInstruction;
lv_obj_t * ui_ImageUserInstruction;
lv_obj_t * ui_LabelUserInstruction;
// CUSTOM VARIABLES

// SCREEN: ui_Thermometer
void ui_Thermometer_screen_init(void);
void ui_event_Thermometer(lv_event_t * e);
lv_obj_t * ui_Thermometer;
void ui_event_PanelTempMeasuring(lv_event_t * e);
lv_obj_t * ui_PanelTempMeasuring;
lv_obj_t * ui_TempMeasuringFill;
lv_obj_t * ui_LabelTempMeasuringCount;
lv_obj_t * ui_LabelTempMeasuring;
// CUSTOM VARIABLES

// SCREEN: ui_ThermometerReading
void ui_ThermometerReading_screen_init(void);
void ui_event_ThermometerReading(lv_event_t * e);
lv_obj_t * ui_ThermometerReading;
void ui_event_PanelTempMeasured(lv_event_t * e);
lv_obj_t * ui_PanelTempMeasured;
lv_obj_t * ui_LabelTempReading;
lv_obj_t * ui_LabelTempMeasured;
lv_obj_t * ui_IconTempMeasuredUnit;
// CUSTOM VARIABLES

// SCREEN: ui_OTA
void ui_OTA_screen_init(void);
void ui_event_OTA(lv_event_t * e);
lv_obj_t * ui_OTA;
lv_obj_t * ui_PanelOTAProgress;
lv_obj_t * ui_LabelFWProgress;
lv_obj_t * ui_FirmwareUpdateBar;
lv_obj_t * ui_LabelFWUpdateInfo;
// void ui_event_PanelOTAUpdated(lv_event_t * e);
lv_obj_t * ui_PanelOTAUpdated;
lv_obj_t * ui_LabelFWUpdated;
lv_obj_t * ui_IconFWUpdatesuccess;
lv_obj_t * ui_LabelFWUpdatedReboot;
void ui_event_PanelOTAFailed(lv_event_t * e);
lv_obj_t * ui_PanelOTAFailed;
lv_obj_t * ui_LabelFWFailed;
lv_obj_t * ui_IconFWUpdateFailed;
lv_obj_t * ui_LabelFWFailedReboot;

// CUSTOM VARIABLES


// SCREEN: ui_Main
void ui_Main_screen_init(void);
void ui_event_Main(lv_event_t * e);
lv_obj_t * ui_Main;
void ui_event_PanelMain(lv_event_t * e);
lv_obj_t * ui_PanelMain;
lv_obj_t * ui_LabelcurrentTemp;
lv_obj_t * ui_ImageTempUnitMain;
lv_obj_t * ui_LabelstartInfo;
lv_obj_t * ui_LabelBatPercent;
lv_obj_t * ui_BLEicon;
lv_obj_t * ui_Baticon;
lv_obj_t * ui_BatBar;
lv_obj_t * ui_BatCharging;
// CUSTOM VARIABLES

// SCREEN: ui_Menu
void ui_Menu_screen_init(void);
void ui_event_Menu(lv_event_t * e);
lv_obj_t * ui_Menu;
void ui_event_PanelModes(lv_event_t * e);
lv_obj_t * ui_PanelModes;
lv_obj_t * ui_IconModes;
lv_obj_t * ui_LabelModes;
void ui_event_PanelHeatingPower(lv_event_t * e);
lv_obj_t * ui_PanelHeatingPower;
lv_obj_t * ui_IconHeatingPower;
lv_obj_t * ui_LabelHeatingPower;
void ui_event_PanelTempUnit(lv_event_t * e);
lv_obj_t * ui_PanelTempUnit;
lv_obj_t * ui_IconTempUnit;
lv_obj_t * ui_LabelTempUnit;
void ui_event_PanelBottleSize(lv_event_t * e);
lv_obj_t * ui_PanelBottleSize;
lv_obj_t * ui_IconBottleSize;
lv_obj_t * ui_LabelBottleSize;
void ui_event_PanelKeepWarm(lv_event_t * e);
lv_obj_t * ui_PanelKeepWarm;
lv_obj_t * ui_IconKeepWarm;
lv_obj_t * ui_LabelKeepWarm;
void ui_event_PanelNotification(lv_event_t * e);
lv_obj_t * ui_PanelNotification;
lv_obj_t * ui_IconNotification;
lv_obj_t * ui_LabelNotification;
void ui_event_PanelLanguage(lv_event_t * e);
lv_obj_t * ui_PanelLanguage;
lv_obj_t * ui_IconLanguage;
lv_obj_t * ui_LabelLanguage;
void ui_event_PanelBluetoothApp(lv_event_t * e);
lv_obj_t * ui_PanelBluetoothApp;
lv_obj_t * ui_IconBluetoothApp;
lv_obj_t * ui_LabelBluetoothApp;
void ui_event_PanelInstructions(lv_event_t * e);
lv_obj_t * ui_PanelInstructions;
lv_obj_t * ui_IconInstructions;
lv_obj_t * ui_LabelInstructions;
void ui_event_PanelAbout(lv_event_t * e);
lv_obj_t * ui_PanelAbout;
lv_obj_t * ui_IconAbout;
lv_obj_t * ui_LabelAbout;


// CUSTOM VARIABLES

// SCREEN: ui_HeatingModes
void ui_HeatingModes_screen_init(void);
void ui_event_HeatingModes(lv_event_t * e);
lv_obj_t * ui_HeatingModes;
void ui_event_PanelMilk(lv_event_t * e);
lv_obj_t * ui_PanelMilk;
lv_obj_t * ui_IconMilk;
lv_obj_t * ui_LabelMilk;
void ui_event_PanelWaterFormula(lv_event_t * e);
lv_obj_t * ui_PanelWaterFormula;
lv_obj_t * ui_IconWaterFormula;
lv_obj_t * ui_LabelWaterFormula;
void ui_event_PanelDefreeze(lv_event_t * e);
lv_obj_t * ui_PanelDefreeze;
lv_obj_t * ui_IconDefreeze;
lv_obj_t * ui_LabelDefreeze;
void ui_event_PanelBabyFood(lv_event_t * e);
lv_obj_t * ui_PanelBabyFood;
lv_obj_t * ui_IconBabyFood;
lv_obj_t * ui_LabelBabyFood;
void ui_event_PanelDeodorize(lv_event_t * e);
lv_obj_t * ui_PanelDeodorize;
lv_obj_t * ui_IconDeodorize;
lv_obj_t * ui_LabelDeodorize;
// CUSTOM VARIABLES

// SCREEN: ui_HeatingPower
void ui_HeatingPower_screen_init(void);
void ui_event_HeatingPower(lv_event_t * e);
lv_obj_t * ui_HeatingPower;
void ui_event_PanelHeatingPowerLvl1(lv_event_t * e);
lv_obj_t * ui_PanelHeatingPowerLvl1;
lv_obj_t * ui_LabelHeatingPower1;
lv_obj_t * ui_LabelHeatingPowerLevel1;
void ui_event_PanelHeatingPowerLvl2(lv_event_t * e);
lv_obj_t * ui_PanelHeatingPowerLvl2;
lv_obj_t * ui_LabelHeatingPower2;
lv_obj_t * ui_LabelHeatingPowerLevel2;
void ui_event_PanelHeatingPowerLvl3(lv_event_t * e);
lv_obj_t * ui_PanelHeatingPowerLvl3;
lv_obj_t * ui_LabelHeatingPower3;
lv_obj_t * ui_LabelHeatingPowerLevel3;
void ui_event_PanelHeatingPowerLvl4(lv_event_t * e);
lv_obj_t * ui_PanelHeatingPowerLvl4;
lv_obj_t * ui_LabelHeatingPower4;
lv_obj_t * ui_LabelHeatingPowerLevel4;
void ui_event_PanelHeatingPowerLvl5(lv_event_t * e);
lv_obj_t * ui_PanelHeatingPowerLvl5;
lv_obj_t * ui_LabelHeatingPower5;
lv_obj_t * ui_LabelHeatingPowerLevel5;
// CUSTOM VARIABLES


// SCREEN: ui_TempUnitSetting
void ui_TempUnitSetting_screen_init(void);
void ui_event_TempUnitSetting(lv_event_t * e);
lv_obj_t * ui_TempUnitSetting;
void ui_event_PanelCelsius(lv_event_t * e);
lv_obj_t * ui_PanelCelsius;
lv_obj_t * ui_IconCelsius;
lv_obj_t * ui_LabelCelsius;
void ui_event_PanelFahrenheit(lv_event_t * e);
lv_obj_t * ui_PanelFahrenheit;
lv_obj_t * ui_IconFahrenheit_;
lv_obj_t * ui_LabelFahrenheit;
// CUSTOM VARIABLES

// SCREEN: ui_KeepWarm
void ui_KeepWarm_screen_init(void);
void ui_event_KeepWarm(lv_event_t * e);
lv_obj_t * ui_KeepWarm;
void ui_event_PanelKeepWarmOFF(lv_event_t * e);
lv_obj_t * ui_PanelKeepWarmOFF;
lv_obj_t * ui_IconKeepWarmOFF;
lv_obj_t * ui_LabelKeepWarmOFF;
void ui_event_PanelKeepWarmLvl1(lv_event_t * e);
lv_obj_t * ui_PanelKeepWarmLvl1;
lv_obj_t * ui_LabelKeepWarm1;
lv_obj_t * ui_LabelKeepWarmM1;
void ui_event_PanelKeepWarmLvl2(lv_event_t * e);
lv_obj_t * ui_PanelKeepWarmLvl2;
lv_obj_t * ui_LabelKeepWarm2;
lv_obj_t * ui_LabelKeepWarmM2;
void ui_event_PanelKeepWarmLvl3(lv_event_t * e);
lv_obj_t * ui_PanelKeepWarmLvl3;
lv_obj_t * ui_LabelKeepWarm3;
lv_obj_t * ui_LabelKeepWarmM3;
void ui_event_PanelKeepWarmLvl4(lv_event_t * e);
lv_obj_t * ui_PanelKeepWarmLvl4;
lv_obj_t * ui_LabelKeepWarm4;
lv_obj_t * ui_LabelKeepWarmM4;
void ui_event_PanelKeepWarmLvl5(lv_event_t * e);
lv_obj_t * ui_PanelKeepWarmLvl5;
lv_obj_t * ui_LabelKeepWarm5;
lv_obj_t * ui_LabelKeepWarmM5;
// CUSTOM VARIABLES

// SCREEN: ui_Notification
void ui_Notification_screen_init(void);
void ui_event_Notification(lv_event_t * e);
lv_obj_t * ui_Notification;
void ui_event_PanelVibration(lv_event_t * e);
lv_obj_t * ui_PanelVibration;
lv_obj_t * ui_IconVibration;
lv_obj_t * ui_LabelVibration;
void ui_event_PanelBuzzer(lv_event_t * e);
lv_obj_t * ui_PanelBuzzer;
lv_obj_t * ui_IconBuzzer;
lv_obj_t * ui_LabelBuzzer;
void ui_event_PanelVolumeLvl1(lv_event_t * e);
lv_obj_t * ui_PanelVolumeLvl1;
lv_obj_t * ui_LabelVolume1;
lv_obj_t * ui_LabelLevel1;
void ui_event_PanelVolumeLvl2(lv_event_t * e);
lv_obj_t * ui_PanelVolumeLvl2;
lv_obj_t * ui_LabelVolume2;
lv_obj_t * ui_LabelLevel2;
void ui_event_PanelVolumeLvl3(lv_event_t * e);
lv_obj_t * ui_PanelVolumeLvl3;
lv_obj_t * ui_LabelVolume3;
lv_obj_t * ui_LabelLevel3;
void ui_event_PanelVolumeLvl4(lv_event_t * e);
lv_obj_t * ui_PanelVolumeLvl4;
lv_obj_t * ui_LabelVolume4;
lv_obj_t * ui_LabelLevel4;
void ui_event_PanelVolumeLvl5(lv_event_t * e);
lv_obj_t * ui_PanelVolumeLvl5;
lv_obj_t * ui_LabelVolume5;
lv_obj_t * ui_LabelLevel5;
// CUSTOM VARIABLES


// SCREEN: ui_Selected
void ui_Selected_screen_init(void);
lv_obj_t * ui_Selected;
lv_obj_t * ui_PanelSelected;
lv_obj_t * ui_ImageSelected;
lv_obj_t * ui_LabelCountDown;
lv_obj_t * ui_countDown;
lv_obj_t * ui_LabelSelectedInfo;
// CUSTOM VARIABLES

// SCREEN: ui_Done
void ui_Done_screen_init(void);
void ui_event_Done(lv_event_t * e);
lv_obj_t * ui_Done;
void ui_event_PanelDone(lv_event_t * e);
lv_obj_t * ui_PanelDone;
lv_obj_t * ui_IconDone;
lv_obj_t * ui_LabelDone;
void ui_event_PanelKeepingProgress(lv_event_t * e);
lv_obj_t * ui_PanelKeepingProgress;
lv_obj_t * ui_IconKeepingProgress;
lv_obj_t * ui_LabelKeepingProgress;
void ui_event_PanelChargeProgress(lv_event_t * e);
lv_obj_t * ui_PanelChargeProgress;
lv_obj_t * ui_ChargeArc;
lv_obj_t * ui_LabelChargeInfo;
lv_obj_t * ui_LabelChargePercent;
void ui_event_PanelChargingDone(lv_event_t * e);
lv_obj_t * ui_PanelChargingDone;
lv_obj_t * ui_IconChargingDone;
lv_obj_t * ui_LabelChargingDone;
// CUSTOM VARIABLES


// SCREEN: ui_Warming
void ui_Warming_screen_init(void);
void ui_event_Warming(lv_event_t * e);
lv_obj_t * ui_Warming;
void ui_event_PanelWarming(lv_event_t * e);
lv_obj_t * ui_PanelWarming;
lv_obj_t * ui_WarmingFiller;
lv_obj_t * ui_LabelWarmingPercent;
// lv_obj_t * ui_LabelRPM;
// lv_obj_t * ui_LabelPWM;
// lv_obj_t * ui_LabelPWMRPM;
// CUSTOM VARIABLES

// SCREEN: ui_TempSetting
void ui_TempSetting_screen_init(void);
void ui_event_TempSetting(lv_event_t * e);
lv_obj_t * ui_TempSetting;
void ui_event_PanelSettingTemp(lv_event_t * e);
lv_obj_t * ui_PanelSettingTemp;
lv_obj_t * ui_LabelsettingTemp;
lv_obj_t * ui_ImageTempSettingUnit;
lv_obj_t * ui_LabelstartInfoTempSetting;
// CUSTOM VARIABLES

// SCREEN: ui_BottleSize
void ui_BottleSize_screen_init(void);
void ui_event_BottleSize(lv_event_t * e);
lv_obj_t * ui_BottleSize;
void ui_event_PanelBottleSmall(lv_event_t * e);
lv_obj_t * ui_PanelBottleSmall;
lv_obj_t * ui_IconBottleSmall;
lv_obj_t * ui_LabelBottleSmall;
void ui_event_PanelBottleLarge(lv_event_t * e);
lv_obj_t * ui_PanelBottleLarge;
lv_obj_t * ui_IconBottleLarge;
lv_obj_t * ui_LabelBottleLarge;
// CUSTOM VARIABLES

// EVENTS
lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_SplashWigg(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOADED) {
        SplashWiggToMain(e);
    }
}

void ui_event_Language(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        LanguageGroupUpdate(e);
    }
}


void ui_event_PanelEnglish(lv_event_t * e)
 {
     lv_event_code_t event_code = lv_event_get_code(e);

     if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setLanguageEnglish(e);
     }
 }


void ui_event_PanelChinese(lv_event_t * e)
 {
     lv_event_code_t event_code = lv_event_get_code(e);

     if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setLanguageChinese(e);
     }
 }

void ui_event_PanelJapanese(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setLanguageJapanese(e);
    }
}

void ui_event_PanelKorean(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setLanguageKorean(e);
    }
}



void ui_event_About(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOADED) {
        AboutGroupUpdate(e);
    }
}

void ui_event_PanelAboutInfo(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ToMainScreen(e);
    }
}

void ui_event_PanelLanguage(lv_event_t * e)
 {
     lv_event_code_t event_code = lv_event_get_code(e);

     if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ToLanguageScreen(e);
     }
 }


void ui_event_BluetoothApp(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOADED) {
        BluetoothAppGroupUpdate(e);
    }
}

void ui_event_PanelBluetoothAppScreen(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ToMainScreen(e);
    }
}


void ui_event_UserInstruction(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOADED) {
        UserInstructionsGroupUpdate(e);
    }
}

void ui_event_PanelUserInstruction(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ToMainScreen(e);
    }
}


void ui_event_Thermometer(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        ThermometerGroupUpdate(e);
    }
}

void ui_event_PanelTempMeasured(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ThermoToMainScreen(e);
    }
}

void ui_event_PanelTempMeasuring(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ThermoToMainScreen(e);
    }
}

void ui_event_ThermometerReading(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        ThermometerReadingGroupUpdate(e);
    }
}




void ui_event_OTA(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        OTAGroupUpdate(e);
    }
}

// void ui_event_PanelOTAUpdated(lv_event_t * e)
// {
//     lv_event_code_t event_code = lv_event_get_code(e);

//     if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
//         EspReboot(e);
//     }
// }

void ui_event_PanelOTAFailed(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        EspReboot(e);
    }
}

void ui_event_Error(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        ErrorGroupUpdate(e);
    }
}

void ui_event_PanelERR1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ErrToMenu(e);
    }
}

void ui_event_PanelERR2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ErrToMenu(e);
    }
}

void ui_event_PanelERR3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ErrToMenu(e);
    }
}

void ui_event_PanelERR4(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ErrToMenu(e);
    }
}

void ui_event_PanelERR5(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ErrToMenu(e);
    }
}

void ui_event_PanelERR25(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ErrToMenu(e);
    }
}

void ui_event_PanelERR8(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ErrToMenu(e);
    }
}

void ui_event_PanelERR7(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ErrToMenu(e);
    }
}

void ui_event_Main(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOADED) {
        MainGroupUpdate(e);
    }
}

void ui_event_PanelMain(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    //Disabled on purpose, bypass-event in lcd_task
    // if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_DOWN) {
    //     ToWarmingScreen(e);
    // }
    // if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_LEFT) {
    //     ToThermometerScreen(e);
    // }
    // if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_RIGHT) {
    //     ToTempSettingScreen(e);
    // }
    // if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_UP) {
    //     ToMenuScreen(e);
    // }
}

void ui_event_Menu(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        MenuGroupUpdate(e);
    }
}

void ui_event_PanelModes(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ToHeatingModes(e);
    }
}

void ui_event_PanelTempUnit(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ToTempUnitSetting(e);
    }
}

void ui_event_PanelBottleSize(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ToBottleSize(e);
    }
}

void ui_event_PanelKeepWarm(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        toKeepWarmScreen(e);
    }
}

void ui_event_PanelNotification(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        toNotificationScreen(e);
    }
}

void ui_event_PanelBluetoothApp(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ToBluetoothAppScreen(e);
    }
}

void ui_event_PanelInstructions(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ToInstructionsScreen(e);
    }
}

void ui_event_PanelAbout(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ToAboutScreen(e);
    }
}

void ui_event_HeatingModes(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        HeatingModesGroupUpdate(e);
    }
}

void ui_event_PanelMilk(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setSelectedModeMilk(e);
    }
}

void ui_event_PanelWaterFormula(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setSelectedModeWaterFormula(e);
    }
}

void ui_event_PanelDefreeze(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setSelectedModeDefreeze(e);
    }
}

void ui_event_PanelBabyFood(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setSelectedModeBabyFood(e);
    }
}

void ui_event_PanelDeodorize(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setSelectedModeDeodorize(e);
    }
}

void ui_event_PanelHeatingPower(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        toHeatingPowerScreen(e);
    }
}

void ui_event_HeatingPower(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        HeatingPowerGroupUpdate(e);
    }
}

void ui_event_PanelHeatingPowerLvl1(lv_event_t * e)
{
     lv_event_code_t event_code = lv_event_get_code(e);

     if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setHeatingPowerLvl1(e);
     }
}

void ui_event_PanelHeatingPowerLvl2(lv_event_t * e)
{
     lv_event_code_t event_code = lv_event_get_code(e);

     if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setHeatingPowerLvl2(e);
     }
}

void ui_event_PanelHeatingPowerLvl3(lv_event_t * e)
{
     lv_event_code_t event_code = lv_event_get_code(e);

     if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setHeatingPowerLvl3(e);
     }
}

void ui_event_PanelHeatingPowerLvl4(lv_event_t * e)
{
     lv_event_code_t event_code = lv_event_get_code(e);

     if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
       setHeatingPowerLvl4(e);
     }
}

void ui_event_PanelHeatingPowerLvl5(lv_event_t * e)
{
     lv_event_code_t event_code = lv_event_get_code(e);

     if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setHeatingPowerLvl5(e);
     }
}


void ui_event_TempUnitSetting(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        TempUnitGroupUpdate(e);
    }
}

void ui_event_PanelCelsius(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setSelectedTempUnitC(e);
    }
}

void ui_event_PanelFahrenheit(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setSelectedTempUnitF(e);
    }
}

void ui_event_KeepWarm(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        KeepWarmGroupUpdate(e);
    }
}

void ui_event_PanelKeepWarmOFF(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        keepWarmOFF(e);
    }
}

void ui_event_PanelKeepWarmLvl1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setKeepWarmLvl1(e);
    }
}

void ui_event_PanelKeepWarmLvl2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setKeepWarmLvl2(e);
    }
}

void ui_event_PanelKeepWarmLvl3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setKeepWarmLvl3(e);
    }
}

void ui_event_PanelKeepWarmLvl4(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setKeepWarmLvl4(e);
    }
}

void ui_event_PanelKeepWarmLvl5(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setKeepWarmLvl5(e);
    }
}


void ui_event_Notification(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        NotificationGroupUpdate(e);
    }
}

void ui_event_PanelVibration(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        enableVibration(e);
    }
}

void ui_event_PanelBuzzer(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        disableBuzzer(e);
    }
}

void ui_event_PanelVolumeLvl1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setVolumeLvl1(e);
    }
}

void ui_event_PanelVolumeLvl2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setVolumeLvl2(e);
    }
}

void ui_event_PanelVolumeLvl3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setVolumeLvl3(e);
    }
}

void ui_event_PanelVolumeLvl4(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setVolumeLvl4(e);
    }
}

void ui_event_PanelVolumeLvl5(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setVolumeLvl5(e);
    }
}

void ui_event_Warming(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        WarmingGroupUpdate(e);
    }
}

void ui_event_PanelWarming(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    // if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_UP) {
    //     WarmingToMainScreen(e);
    // }
}

void ui_event_TempSetting(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOADED) {
        SettingTempGroupUpdate(e);
    }
}

void ui_event_PanelSettingTemp(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ToMainScreen(e);
    }
}

void ui_event_BottleSize(lv_event_t * e)
 {
     lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        BottleSizeGroupUpdate(e);
     }
 }

void ui_event_PanelBottleSmall(lv_event_t * e)
 {
     lv_event_code_t event_code = lv_event_get_code(e);

     if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setBottleSizeSmall(e);
    }
}

void ui_event_PanelBottleLarge(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        setBottleSizeLarge(e);
     }
 }


void ui_event_Done(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        DoneGroupUpdate(e);
    }
}

void ui_event_PanelDone(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
         ToMainScreenNoAnim(e);

    }
}

void ui_event_PanelKeepingProgress(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
         ToMainScreen(e);

    }
}

void ui_event_PanelChargeProgress(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ToMainScreenNoAnim(e);
    }
}


void ui_event_PanelChargingDone(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_KEY &&  lv_event_get_key(e) == LV_KEY_ENTER) {
        ToMainScreenNoAnim(e);
     }
 }

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_display_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_SplashWigg_screen_init();
    ui_Main_screen_init();
    ui_TempSetting_screen_init();
    ui_Error_screen_init();
    ui_Done_screen_init();
    ui_ThermometerReading_screen_init();
    ui_Menu_screen_init();
    ui_Selected_screen_init();
    ui_Warming_screen_init();
    ui_OTA_screen_init();
    // ui_Language_screen_init();
    // ui_HeatingModes_screen_init();
    // ui_HeatingPower_screen_init();
    // ui_TempUnitSetting_screen_init();
    // ui_KeepWarm_screen_init();
    // ui_Notification_screen_init();
    // ui_About_screen_init();
    // ui_BluetoothApp_screen_init();
    // ui_UserInstruction_screen_init();
    // ui_Thermometer_screen_init();
    // ui_BottleSize_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_SplashWigg);
}
