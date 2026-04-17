#ifndef _UI_EVENTS_H
#define _UI_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

void SplashWiggToMain(lv_event_t * e);
void ErrorGroupUpdate(lv_event_t * e);
void DoneGroupUpdate(lv_event_t * e);
void LanguageGroupUpdate(lv_event_t * e);
void setLanguageEnglish(lv_event_t * e);
void setLanguageChinese(lv_event_t * e);
void setLanguageJapanese(lv_event_t * e);
void setLanguageKorean(lv_event_t * e);
void ToMainScreen(lv_event_t * e);
void ToMainScreenNoAnim(lv_event_t * e);
void MainGroupUpdate(lv_event_t * e);
void ToTempSettingScreen(lv_event_t * e);
void ToMenuScreen(lv_event_t * e);
void ToWarmingScreen(lv_event_t * e);
void ErrToMenu(lv_event_t * e);
void MenuGroupUpdate(lv_event_t * e);
void AboutGroupUpdate(lv_event_t * e);
void ToHeatingModes(lv_event_t * e);
void ToTempUnitSetting(lv_event_t * e);
void ToLanguageScreen(lv_event_t * e);
void ToBottleSize(lv_event_t * e);
void toKeepWarmScreen(lv_event_t * e);
void toNotificationScreen(lv_event_t * e);
void ToBluetoothAppScreen(lv_event_t * e);
void ToInstructionsScreen(lv_event_t * e);
void BluetoothAppGroupUpdate(lv_event_t * e);
void UserInstructionsGroupUpdate(lv_event_t * e);
void ToAboutScreen(lv_event_t * e);
void HeatingModesGroupUpdate(lv_event_t * e);
void setSelectedModeMilk(lv_event_t * e);
void setSelectedModeWaterFormula(lv_event_t * e);
void setSelectedModeDefreeze(lv_event_t * e);
void setSelectedModeBabyFood(lv_event_t * e);
void setSelectedModeDeodorize(lv_event_t * e);
void TempUnitGroupUpdate(lv_event_t * e);
void setSelectedTempUnitC(lv_event_t * e);
void setSelectedTempUnitF(lv_event_t * e);
void KeepWarmGroupUpdate(lv_event_t * e);
void keepWarmOFF(lv_event_t * e);
void setKeepWarmLvl1(lv_event_t * e);
void setKeepWarmLvl2(lv_event_t * e);
void setKeepWarmLvl3(lv_event_t * e);
void setKeepWarmLvl4(lv_event_t * e);
void setKeepWarmLvl5(lv_event_t * e);
void NotificationGroupUpdate(lv_event_t * e);
void enableVibration(lv_event_t * e);
void disableBuzzer(lv_event_t * e);
void setVolumeLvl1(lv_event_t * e);
void setVolumeLvl2(lv_event_t * e);
void setVolumeLvl3(lv_event_t * e);
void setVolumeLvl4(lv_event_t * e);
void setVolumeLvl5(lv_event_t * e);
void WarmingGroupUpdate(lv_event_t * e);
void WarmingToMainScreen(lv_event_t * e);
void SettingTempGroupUpdate(lv_event_t * e);
void BottleSizeGroupUpdate(lv_event_t * e);
void setBottleSizeSmall(lv_event_t * e);
void setBottleSizeLarge(lv_event_t * e);
void ThermometerGroupUpdate(lv_event_t * e);
void ToThermometerScreen(lv_event_t * e);  
void ThermometerReadingGroupUpdate(lv_event_t * e);
void ThermoToMainScreen(lv_event_t * e);
void OTAGroupUpdate(lv_event_t * e);
void EspReboot(lv_event_t * e);



#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
