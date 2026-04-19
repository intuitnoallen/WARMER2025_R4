#include "ui.h"
#include "ui_common.h"
#include "ui_events.h"
// #include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/rtc_io.h"
// #include "esp_log.h"
#include "esp_sleep.h"
#include "app_warmer_mngr.h"
#include "app_cfg.h"
#include "app_notification.h"


static WARMER_config_t * g_pstru_config = NULL;
bool keypad_enabled=false;

static void async_refresh_cb(void * user_data)
{
    ui_update_msg_t *msg = (ui_update_msg_t *)user_data;

	if (!msg) return;

    switch(msg->type) {
    case UI_UPDATE_BLE_STATUS:
        if (msg->value){
            lv_obj_set_style_image_recolor(ui_BLEicon, lv_color_hex(0x1945F2), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_image_recolor(ui_BLEiconOTA, lv_color_hex(0x1945F2), LV_PART_MAIN | LV_STATE_DEFAULT);
		}
        else{
            lv_obj_set_style_image_recolor(ui_BLEicon, lv_color_hex(0x909090), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_image_recolor(ui_BLEiconOTA, lv_color_hex(0x909090), LV_PART_MAIN | LV_STATE_DEFAULT);
		}
        break;
	
	// case UI_UPDATE_RPM:
	// 	lv_label_set_text_fmt(ui_LabelRPM, "%d", msg->value);
	// break;

	// case UI_UPDATE_PWM:
	// 	lv_label_set_text_fmt(ui_LabelPWM, "%d", msg->value);
	// break;

	case UI_UPDATE_SETTING_TEMP:
		u8_updateSettingTemperature();
		break;

	case UI_UPDATE_THERMOMETER_TIME:
		 updateThermometerTime(msg->value);
		break;
		
	case UI_UPDATE_THERMOMETER_TEMP:
		 updateThermometerTemp();
		break;	
  
	case UI_UPDATE_DONE_SCR:
		ToDoneScreen(msg->value);
	break;

	case UI_UPDATE_WARMING_PERCENT:
		 u8_warming_percent_update();
	break;

	case UI_UPDATE_BATTERY_PERCENT:
		 u8_updateBatteryPercent();
	break;

	case UI_UPDATE_CHARGING_ICON:
		 b_updateChargingIcon();
	break;

	case UI_UPDATE_BLE_OTA:
		 ToOtaScreen(msg->value);
	break;

	case UI_UPDATE_TEXT:
		if(msg->value ==0){
			lv_label_set_text(ui_LabelstartInfo, _("StartWarming")); //Reset "Keeping in Progress" main info label when back to IDLE,
		}
	break;

	case UI_UPDATE_ERROR:
		 ToErrorScreen(msg->value);
	break;

	case UI_UPDATE_MAIN:
		 ToMainScreenNoAnim(NULL);
	break;
	
    case UI_UPDATE_ALL:
		updateAllConfigurations();
        break;
	}

    free(msg);  // cleanup here
	// lv_free(msg);
}

void ui_refresh(ui_update_msg_t *msg)
{
    lv_async_call(async_refresh_cb, msg);
}

void ui_post_update(ui_update_t type, int32_t value)
{
    ui_update_msg_t *msg = malloc(sizeof(ui_update_msg_t));
    if (msg) {
        msg->type  = type;
        msg->value = value;
        ui_refresh(msg);   // schedule async call
    }
}

void lv_reboot_timer_cb(lv_timer_t * timer)
{
    esp_restart();   // Restart ESP32 safely
}

void updateError(uint8_t e){
	if(e==0){
		//no error, return to main screen
		ToMainScreenNoAnim(NULL);
	}
	else if(e==1){
		lv_obj_scroll_to_view(ui_PanelERR1, LV_ANIM_OFF);
		lv_group_focus_obj(ui_PanelERR1);
	}else if(e==2){
		lv_obj_scroll_to_view(ui_PanelERR2, LV_ANIM_OFF);
		lv_group_focus_obj(ui_PanelERR2);
	}else if(e==3){
		lv_obj_scroll_to_view(ui_PanelERR3, LV_ANIM_OFF);
		lv_group_focus_obj(ui_PanelERR3);
	}else if(e==4){
		lv_obj_scroll_to_view(ui_PanelERR4, LV_ANIM_OFF);
		lv_group_focus_obj(ui_PanelERR4);
	}else if(e==5){
		lv_obj_scroll_to_view(ui_PanelERR5, LV_ANIM_OFF);
		lv_group_focus_obj(ui_PanelERR5);
	}else if(e==7){
        lv_obj_scroll_to_view(ui_PanelERR7, LV_ANIM_OFF);
        lv_group_focus_obj(ui_PanelERR7);	
	}else if(e==8){
		lv_obj_scroll_to_view(ui_PanelERR8, LV_ANIM_OFF);
		lv_group_focus_obj(ui_PanelERR8);
	}else if(e==25){
		lv_obj_scroll_to_view(ui_PanelERR25, LV_ANIM_OFF);
		lv_group_focus_obj(ui_PanelERR25);
	}
}

void updateThermometerTime(int32_t t){
	lv_label_set_text_fmt(ui_LabelTempReading, "%d", t);
}

void updateThermometerTemp(){
	isMeasuringTemp=false;
	if(g_pstru_config->enm_temp_unit == TEMP_UNIT_C){

		char temp_str[6]; 
		snprintf(temp_str, sizeof(temp_str), "%.2f", average_measured_temp);
		lv_label_set_text(ui_LabelTempReading, temp_str);

		// lv_label_set_text_fmt(ui_LabelTempReading, "%d", (int32_t)average_measured_temp);
		// LOGW("Average Temperature: %.2fC\n", average_measured_temp);

	}else{
		// lv_label_set_text_fmt(ui_LabelTempReading, "%.2f", TEMP_C_TO_F(average_measured_temp));
		char temp_str[6]; 
		snprintf(temp_str, sizeof(temp_str), "%.2f", TEMP_C_TO_F(average_measured_temp));
		lv_label_set_text(ui_LabelTempReading, temp_str);
	}
	// lv_obj_scroll_to_view(ui_PanelTempMeasured, LV_ANIM_OFF);
	// lv_group_focus_obj(ui_PanelTempMeasured);
	_ui_screen_change(&ui_ThermometerReading, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Thermometer_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
	app_notify_200ms();
}

void ThermometerReadingGroupUpdate(lv_event_t * e)
{
       
	lv_indev_set_group(indev, groupThermometerReading);
	
	lv_obj_scroll_to_view(ui_PanelTempMeasured, LV_ANIM_OFF);
	lv_group_set_focus_cb(groupThermometerReading, my_focus_cb);
	lv_group_focus_obj(ui_PanelTempMeasured);
}


void ToErrorScreen(uint8_t e){
	isMeasuringTemp = false;
	_ui_screen_change(&ui_Error, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Error_screen_init);
	updateError(e);
	vTaskDelay(pdMS_TO_TICKS(20));
}

void ToLanguageScreen(lv_event_t * e)
{
    if (ui_Language == NULL) {
       ui_Language_screen_init();
    }
    _ui_screen_change(&ui_Language, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Language_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
}

void LanguageGroupUpdate(lv_event_t * e)
{
    lv_group_add_obj(groupLanguage, ui_PanelEnglish);
    lv_group_add_obj(groupLanguage, ui_PanelChinese);
    lv_group_add_obj(groupLanguage, ui_PanelJapanese);
    lv_group_add_obj(groupLanguage, ui_PanelKorean);

    lv_indev_set_group(indev, groupLanguage);
    lv_obj_scroll_to_view(ui_PanelEnglish, LV_ANIM_OFF);
	lv_group_set_focus_cb(groupLanguage, my_focus_cb);
	lv_group_focus_obj(ui_PanelEnglish);
}

void setLanguageEnglish(lv_event_t * e)
{
    //    lv_i18n_set_locale("en-US");
	   g_pstru_config->enm_device_language = DEVICE_LANGUAGE_EN_US;
	   updateAllConfigurations();
	   vTaskDelay(pdMS_TO_TICKS(20));
	   esp_restart();
	   
}

void setLanguageChinese(lv_event_t * e)
{
    //    lv_i18n_set_locale("zh-SG");
	   g_pstru_config->enm_device_language = DEVICE_LANGUAGE_ZH_SG;
	   updateAllConfigurations();
	   vTaskDelay(pdMS_TO_TICKS(20));
	   esp_restart();
}

void setLanguageJapanese(lv_event_t * e)
{
    //    lv_i18n_set_locale("ja-JP");
	   g_pstru_config->enm_device_language = DEVICE_LANGUAGE_JA_JP;
	   updateAllConfigurations();
	   vTaskDelay(pdMS_TO_TICKS(20));
	   esp_restart();
}

void setLanguageKorean(lv_event_t * e)
{
    //    lv_i18n_set_locale("ko-KR");
	   g_pstru_config->enm_device_language = DEVICE_LANGUAGE_KO_KR;
	   updateAllConfigurations();
	   vTaskDelay(pdMS_TO_TICKS(20));
	   esp_restart();
}

void toHeatingPowerScreen(lv_event_t * e)
{
       if (ui_HeatingPower == NULL) {
       ui_HeatingPower_screen_init();
       }
       _ui_screen_change(&ui_HeatingPower, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_HeatingPower_screen_init);
       vTaskDelay(pdMS_TO_TICKS(20));
}

void HeatingPowerGroupUpdate(lv_event_t * e)
{
    lv_group_add_obj(groupHeatingPower, ui_PanelHeatingPowerLvl1);
    lv_group_add_obj(groupHeatingPower, ui_PanelHeatingPowerLvl2);
    lv_group_add_obj(groupHeatingPower, ui_PanelHeatingPowerLvl3);
    lv_group_add_obj(groupHeatingPower, ui_PanelHeatingPowerLvl4);
    lv_group_add_obj(groupHeatingPower, ui_PanelHeatingPowerLvl5);

    lv_indev_set_group(indev, groupHeatingPower);
    lv_obj_scroll_to_view(ui_PanelHeatingPowerLvl1, LV_ANIM_OFF);
    lv_group_set_focus_cb(groupHeatingPower, my_focus_cb);
    lv_group_focus_obj(ui_PanelHeatingPowerLvl1);
}

void setHeatingPowerLvl1(lv_event_t * e)
{
       g_pstru_config->u8_heater_pwr_lvl = 1;
       ToMainScreenNoAnim(e);
}

void setHeatingPowerLvl2(lv_event_t * e)
{
       g_pstru_config->u8_heater_pwr_lvl = 2;
       ToMainScreenNoAnim(e);
}

void setHeatingPowerLvl3(lv_event_t * e)
{
       g_pstru_config->u8_heater_pwr_lvl = 3;
       ToMainScreenNoAnim(e);
}
void setHeatingPowerLvl4(lv_event_t * e)
{
       g_pstru_config->u8_heater_pwr_lvl = 4;
       ToMainScreenNoAnim(e);
}
void setHeatingPowerLvl5(lv_event_t * e)
{
       g_pstru_config->u8_heater_pwr_lvl = 5;
       ToMainScreenNoAnim(e);
}


void ToOtaScreen(uint8_t ota){
	if(ota==0){
		ToMainScreenNoAnim(NULL);
	}
	else if(ota==1){ //started
		 isErrorUpdateRequired=false; //dont show error on screen while ota update
		 isMeasuringTemp=false;
		_ui_screen_change(&ui_OTA, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_OTA_screen_init);
		vTaskDelay(pdMS_TO_TICKS(20));
		lv_obj_scroll_to_view(ui_PanelOTAProgress, LV_ANIM_OFF);
	}else if(ota==2){ //success
		lv_obj_scroll_to_view(ui_PanelOTAUpdated, LV_ANIM_OFF);
		app_notify_200ms();
	}else if(ota==3){ //fail
		lv_obj_scroll_to_view(ui_PanelOTAFailed, LV_ANIM_OFF);
		app_notify_200ms_double ();
		lv_timer_t * reboot_timer = lv_timer_create(lv_reboot_timer_cb, 3000, NULL);
    	lv_timer_set_repeat_count(reboot_timer, 1);  // Run once only
	}
}

void ToBluetoothAppScreen(lv_event_t * e)
{
    if (ui_BluetoothApp == NULL) {
	    ui_BluetoothApp_screen_init();
    }
    
	   _ui_screen_change(&ui_BluetoothApp, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_BluetoothApp_screen_init);
	   vTaskDelay(pdMS_TO_TICKS(20));
}

void ToInstructionsScreen(lv_event_t * e)
{
    if (ui_UserInstruction == NULL) {
	    ui_UserInstruction_screen_init();
    }
	   _ui_screen_change(&ui_UserInstruction, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_UserInstruction_screen_init);
	   vTaskDelay(pdMS_TO_TICKS(20));
}

void ToDoneScreen(uint8_t done){
	lv_arc_set_value(ui_WarmingFiller, 0);
    lv_label_set_text_fmt(ui_LabelWarmingPercent, "%d%%", 0);
	updateDoneScreen(done);
}

void updateDoneScreen(uint8_t d){

	if (d==0){
	WarmingToMainScreen(NULL);
	}else if(d == 1){
	ToWarmingScreen(NULL);
	}else if(d == 2){//Warming Done

	lv_label_set_text(ui_LabelDone, _("WarmingDone"));
	// ToDoneScreen();
	_ui_screen_change(&ui_Done, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Done_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
	lv_obj_scroll_to_view(ui_PanelDone, LV_ANIM_OFF); //Warming Done
	lv_group_focus_obj(ui_PanelDone);

	// lv_obj_scroll_to_view(ui_PanelERR1, LV_ANIM_OFF);
	// lv_group_focus_obj(ui_PanelERR1);

	}else if (d == 3){//Warming Done Keeping in Progress

	// ToDoneScreen();
	_ui_screen_change(&ui_Done, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Done_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
	lv_obj_scroll_to_view(ui_PanelKeepingProgress, LV_ANIM_OFF); //Warming Done Keeping in Progress
	lv_group_focus_obj(ui_PanelKeepingProgress);
	}else if (d == 4){//Keeping Done
	lv_label_set_text(ui_LabelDone, _("KeepingDone"));
	// ToDoneScreen();
	_ui_screen_change(&ui_Done, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Done_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
	lv_obj_scroll_to_view(ui_PanelDone, LV_ANIM_OFF); 
	lv_group_focus_obj(ui_PanelDone);
	}else if (d == 5){//Charging Done

	// ToDoneScreen();
	_ui_screen_change(&ui_Done, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Done_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
	lv_obj_scroll_to_view(ui_PanelChargingDone, LV_ANIM_OFF); //Charging Done
	lv_group_focus_obj(ui_PanelChargingDone);
	}else if (d == 6){ //Charging UI 
	_ui_screen_change(&ui_Done, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Done_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
	lv_obj_scroll_to_view(ui_PanelChargeProgress, LV_ANIM_OFF); 
	lv_group_focus_obj(ui_PanelChargeProgress);
	}
}

void u8_updateSettingTemperature(){
	if(g_pstru_config->enm_temp_unit == TEMP_UNIT_C){
			lv_label_set_text_fmt(ui_LabelcurrentTemp, "%d", g_pstru_config->u8_temp_setpoint);
			lv_label_set_text_fmt(ui_LabelsettingTemp, "%d", g_pstru_config->u8_temp_setpoint);
			lv_img_set_src(ui_ImageTempUnitMain, &ui_img_temp_ci_png );
			lv_img_set_src(ui_ImageTempSettingUnit, &ui_img_temp_ci_png );
			lv_img_set_src(ui_IconTempMeasuredUnit, &ui_img_temp_ci_png );
	}else{
			lv_label_set_text_fmt(ui_LabelcurrentTemp, "%d", (int)TEMP_C_TO_F(g_pstru_config->u8_temp_setpoint));
			lv_label_set_text_fmt(ui_LabelsettingTemp, "%d", (int)TEMP_C_TO_F(g_pstru_config->u8_temp_setpoint));
			lv_img_set_src(ui_ImageTempUnitMain, &ui_img_temp_fi_png );
			lv_img_set_src(ui_ImageTempSettingUnit, &ui_img_temp_fi_png );
			lv_img_set_src(ui_IconTempMeasuredUnit, &ui_img_temp_fi_png );
	}
}

void b_updateChargingIcon(){
	// if(device_data.plug.miliAmps > WARMER_CUTOFF_CHARGING &&  device_data.battery.state != BATTERY_FULL){
	if(device_data.plug.plugdet){
		lv_obj_clear_flag(ui_BatCharging, LV_OBJ_FLAG_HIDDEN);  // Show the object
	}else{
		lv_obj_add_flag(ui_BatCharging, LV_OBJ_FLAG_HIDDEN);   // Hide the object
		if(curr_scr==ui_Done && focused_obj==ui_PanelChargeProgress){
			ToMainScreenNoAnim(NULL);
		}
	}

}

void u8_updateBatteryPercent(){
	//Update Battery Icon and Percentage on Main Screen
	lv_label_set_text_fmt(ui_LabelBatPercent, "%d%%", device_data.battery.percent);
	int pixel_width = (device_data.battery.percent * 36) / 100;   // 0% → 0px, 100% → 36px
	lv_obj_set_width(ui_BatBar, pixel_width);

	if(device_data.battery.percent < 10){
		lv_obj_set_style_text_color(ui_LabelBatPercent, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT); // red #F70808
		lv_obj_set_style_image_recolor(ui_Baticon, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT);//red #F70808
		lv_obj_set_style_image_recolor(ui_BatBar, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT);//red #F70808
	}else if(device_data.battery.percent < 60){
		lv_obj_set_style_text_color(ui_LabelBatPercent, lv_color_hex(0xFF9201), LV_PART_MAIN | LV_STATE_DEFAULT); // //orange #FE9F38 #FF9201
		lv_obj_set_style_image_recolor(ui_Baticon, lv_color_hex(0xFF9201), LV_PART_MAIN | LV_STATE_DEFAULT); //orange #FE9F38 #FECE00
		lv_obj_set_style_image_recolor(ui_BatBar, lv_color_hex(0xFF9201), LV_PART_MAIN | LV_STATE_DEFAULT); //orange #FE9F38 #FECE00
	}else{
		lv_obj_set_style_text_color(ui_LabelBatPercent, lv_color_hex(0x9AD01A), LV_PART_MAIN | LV_STATE_DEFAULT); //lime green #00F00B #00A308 #9AD01A
		lv_obj_set_style_image_recolor(ui_Baticon, lv_color_hex(0x9AD01A), LV_PART_MAIN | LV_STATE_DEFAULT);//green #00F00B #00A308 #9AD01A
		lv_obj_set_style_image_recolor(ui_BatBar, lv_color_hex(0x9AD01A), LV_PART_MAIN | LV_STATE_DEFAULT);//green #00F00B #00A308 #9AD01A
	}

	if(device_data.battery.charging == BATTERY_CHARGING){
		lv_label_set_text_fmt(ui_LabelChargePercent, "%d", device_data.battery.percent);
		lv_arc_set_value(ui_ChargeArc, device_data.battery.percent);

		if(device_data.battery.percent < 10){
			lv_obj_set_style_text_color(ui_LabelChargePercent, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT); // red #F70808
			lv_obj_set_style_text_color(ui_LabelChargeInfo, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT); // red #F70808
			// lv_obj_set_style_image_recolor(ui_IconChargingProgress, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT);//red #F70808
			lv_obj_set_style_arc_color(ui_ChargeArc, lv_color_hex(0xF70808), LV_PART_INDICATOR | LV_STATE_DEFAULT);      // #F70808
		}else if(device_data.battery.percent < 60){
			lv_obj_set_style_text_color(ui_LabelChargePercent, lv_color_hex(0xFF9201), LV_PART_MAIN | LV_STATE_DEFAULT); // //orange #FE9F38 #FF9201
			lv_obj_set_style_text_color(ui_LabelChargeInfo, lv_color_hex(0xFF9201), LV_PART_MAIN | LV_STATE_DEFAULT); // //orange #FE9F38 #FF9201
			// lv_obj_set_style_image_recolor(ui_IconChargingProgress, lv_color_hex(0xFF9201), LV_PART_MAIN | LV_STATE_DEFAULT); //orange #FE9F38 #FECE00
			lv_obj_set_style_arc_color(ui_ChargeArc, lv_color_hex(0xFF9201), LV_PART_INDICATOR | LV_STATE_DEFAULT);  //orange #FE9F38 #FECE00
		}else{
			lv_obj_set_style_text_color(ui_LabelChargePercent, lv_color_hex(0x9AD01A), LV_PART_MAIN | LV_STATE_DEFAULT); //lime green #00F00B #00A308 #9AD01A
			lv_obj_set_style_text_color(ui_LabelChargeInfo, lv_color_hex(0x9AD01A), LV_PART_MAIN | LV_STATE_DEFAULT); //lime green #00F00B #00A308 #9AD01A
			// lv_obj_set_style_image_recolor(ui_IconChargingProgress, lv_color_hex(0x9AD01A), LV_PART_MAIN | LV_STATE_DEFAULT);//green #00F00B #00A308 #9AD01A
			lv_obj_set_style_arc_color(ui_ChargeArc, lv_color_hex(0x9AD01A), LV_PART_INDICATOR | LV_STATE_DEFAULT);  //green #00F00B #00A308 #9AD01A
		}
	}
}


void updateAllConfigurations(){
	//Updare Setting Temperature and Unit on Main Screen
	
	enm_WARMER_Save_Config ();
	u8_updateSettingTemperature();
	
	
}

void ToBottleSize(lv_event_t * e)
{
	if (ui_BottleSize == NULL) {
       ui_BottleSize_screen_init();
    } 
	   _ui_screen_change(&ui_BottleSize, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_BottleSize_screen_init);
	   vTaskDelay(pdMS_TO_TICKS(20));
}

void toKeepWarmScreen(lv_event_t * e)
{
   	// if (ui_KeepWarm == NULL) {
    // 	ui_KeepWarm_screen_init();
	// }
	   _ui_screen_change(&ui_KeepWarm, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_KeepWarm_screen_init);
	   vTaskDelay(pdMS_TO_TICKS(20));
}

void KeepWarmGroupUpdate(lv_event_t * e)
{
 	lv_group_add_obj(groupKeepWarm, ui_PanelKeepWarmOFF);
    lv_group_add_obj(groupKeepWarm, ui_PanelKeepWarmLvl1);
    lv_group_add_obj(groupKeepWarm, ui_PanelKeepWarmLvl2);
    lv_group_add_obj(groupKeepWarm, ui_PanelKeepWarmLvl3);
    lv_group_add_obj(groupKeepWarm, ui_PanelKeepWarmLvl4);
    lv_group_add_obj(groupKeepWarm, ui_PanelKeepWarmLvl5);

	lv_indev_set_group(indev, groupKeepWarm);
	lv_obj_scroll_to_view(ui_PanelKeepWarmOFF, LV_ANIM_OFF);
	lv_group_set_focus_cb(groupKeepWarm, my_focus_cb);
	lv_group_focus_obj(ui_PanelKeepWarmOFF);
}

void keepWarmOFF(lv_event_t * e)
{
     
	   g_pstru_config->u8_Keep_warm_level = 0;  
	   ToMainScreenNoAnim(e);
}

void setKeepWarmLvl1(lv_event_t * e)
{
     g_pstru_config->u8_Keep_warm_level = 1;
	 ToMainScreenNoAnim(e);
}

void setKeepWarmLvl2(lv_event_t * e)
{
     
	g_pstru_config->u8_Keep_warm_level = 2;
	   ToMainScreenNoAnim(e);
}

void setKeepWarmLvl3(lv_event_t * e)
{
      
	g_pstru_config->u8_Keep_warm_level = 3;
	   ToMainScreenNoAnim(e);
}

void setKeepWarmLvl4(lv_event_t * e)
{
       
	g_pstru_config->u8_Keep_warm_level = 4;
	   ToMainScreenNoAnim(e);
}

void setKeepWarmLvl5(lv_event_t * e)
{
    
	g_pstru_config->u8_Keep_warm_level = 5;
	   ToMainScreenNoAnim(e);
}

void BottleSizeGroupUpdate(lv_event_t * e)
{
	lv_group_add_obj(groupBottleSize, ui_PanelBottleSmall);
    lv_group_add_obj(groupBottleSize, ui_PanelBottleLarge);
    
	lv_indev_set_group(indev, groupBottleSize);
	lv_obj_scroll_to_view(ui_PanelBottleSmall, LV_ANIM_OFF);
	lv_group_set_focus_cb(groupBottleSize, my_focus_cb);
	lv_group_focus_obj(ui_PanelBottleSmall);
}

void setBottleSizeSmall(lv_event_t * e)
{
      g_pstru_config->enm_volume_level = WARMER_VOLUME_120;
	    ToMainScreenNoAnim(e);
}

void setBottleSizeLarge(lv_event_t * e)
{
    
	   g_pstru_config->enm_volume_level = WARMER_VOLUME_240;
	   ToMainScreenNoAnim(e);
}


void ThermometerGroupUpdate(lv_event_t * e)
{
    lv_group_add_obj(groupThermometer, ui_PanelTempMeasuring); 
	lv_indev_set_group(indev, groupThermometer);
	lv_obj_scroll_to_view(ui_PanelTempMeasuring, LV_ANIM_OFF);
	lv_group_set_focus_cb(groupThermometer, my_focus_cb);
	lv_group_focus_obj(ui_PanelTempMeasuring);
}

void ToThermometerScreen(lv_event_t * e)
{
	if (ui_Thermometer == NULL) {
    	ui_Thermometer_screen_init();
	}
   
	_ui_screen_change(&ui_Thermometer, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Thermometer_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
	isMeasuringTemp = true;
}

void ThermoToMainScreen(lv_event_t * e)
{
	   isMeasuringTemp = false;
	   _ui_screen_change(&ui_Main, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, MOVE_DOWN_ANIM_MS, 0, &ui_Main_screen_init);//400
	   vTaskDelay(pdMS_TO_TICKS(20));
}



void OTAGroupUpdate(lv_event_t * e)
{
  
	lv_indev_set_group(indev, groupOTAupdate);
	lv_obj_scroll_to_view(ui_PanelOTAProgress, LV_ANIM_OFF);
	lv_group_set_focus_cb(groupOTAupdate, my_focus_cb);
	lv_group_focus_obj(ui_PanelOTAProgress);
}



void EspReboot(lv_event_t * e)
{
      
	   esp_restart();
}



void SplashWiggToMain(lv_event_t * e)
{
	
	 /* Read device configuration */
	// app_notify_500ms();
	g_pstru_config = pstru_WARMER_Get_Config ();
 	updateAllConfigurations();
	_ui_screen_change(&ui_Main, LV_SCR_LOAD_ANIM_NONE, 0, 3500, &ui_Main_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
}

void ToMainScreen(lv_event_t * e)
{
	isErrorUpdateRequired = true; 		//start showing error after device is stable at main screen
	isMeasuringTemp = false;
	// lv_arc_set_value(ui_WarmingFiller, 0);
    // lv_label_set_text_fmt(ui_LabelWarmingPercent, "%d%%", 0);
	_ui_screen_change(&ui_Main, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, MOVE_DOWN_ANIM_MS, 0, &ui_Main_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
	updateAllConfigurations();
}

void ToMainScreenNoAnim(lv_event_t * e)
{
	isErrorUpdateRequired = true;
	isMeasuringTemp = false;
	_ui_screen_change(&ui_Main, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Main_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
	updateAllConfigurations();
}

void ToMenuScreen(lv_event_t * e)
{
	if(ui_Menu == NULL){
		ui_Menu_screen_init();
	}
	_ui_screen_change(&ui_Menu, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Menu_screen_init);//400
	vTaskDelay(pdMS_TO_TICKS(20));
}


void ErrToMenu(lv_event_t * e)
{
	if(ui_Menu == NULL){
		ui_Menu_screen_init();
	}
	   isErrorUpdateRequired = false;  //stop showing error (only screen update) if moving to menu screen from error screen 	
	   _ui_screen_change(&ui_Menu, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Menu_screen_init);//400
	   vTaskDelay(pdMS_TO_TICKS(20));
}


void MainGroupUpdate(lv_event_t * e)
{
	lv_indev_set_group(indev, groupMain);
	vTaskDelay(pdMS_TO_TICKS(20));
	isErrorUpdateRequired = true; 		//start showing error after device is stable at main screen
	device_stable = true;  //update battery when device stable
	if(keypad_enabled==false){
		keypad_enabled=true;
		vTaskDelay(pdMS_TO_TICKS(150));
		keypad_enable();
	}
}

void MenuGroupUpdate(lv_event_t * e)
{
	lv_group_add_obj(groupMenu, ui_PanelModes);
    lv_group_add_obj(groupMenu, ui_PanelHeatingPower);   
    lv_group_add_obj(groupMenu, ui_PanelTempUnit);
    lv_group_add_obj(groupMenu, ui_PanelBottleSize);
    lv_group_add_obj(groupMenu, ui_PanelKeepWarm);
    lv_group_add_obj(groupMenu, ui_PanelNotification);
    lv_group_add_obj(groupMenu, ui_PanelLanguage);
    lv_group_add_obj(groupMenu, ui_PanelBluetoothApp);
    lv_group_add_obj(groupMenu, ui_PanelInstructions);
    lv_group_add_obj(groupMenu, ui_PanelAbout);
	lv_indev_set_group(indev, groupMenu);
	
	// lv_obj_t * focused_obj = lv_group_get_focused(groupMenu);
    lv_obj_scroll_to_view(ui_PanelModes, LV_ANIM_OFF);
	lv_group_set_focus_cb(groupMenu, my_focus_cb);
	lv_group_focus_obj(ui_PanelModes);
	
}

void ToHeatingModes(lv_event_t * e)
{
	if (ui_HeatingModes == NULL) {
    	ui_HeatingModes_screen_init();
	}	
	_ui_screen_change(&ui_HeatingModes, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_HeatingModes_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
}

void HeatingModesGroupUpdate(lv_event_t * e)
{
	lv_group_add_obj(groupHeatingModes, ui_PanelMilk);
    lv_group_add_obj(groupHeatingModes, ui_PanelWaterFormula);
    lv_group_add_obj(groupHeatingModes, ui_PanelDefreeze);
    lv_group_add_obj(groupHeatingModes, ui_PanelBabyFood);
    lv_group_add_obj(groupHeatingModes, ui_PanelDeodorize);

	lv_indev_set_group(indev, groupHeatingModes);

	// lv_obj_t * focused_obj = lv_group_get_focused(groupHeatingModes);
    lv_obj_scroll_to_view(ui_PanelMilk, LV_ANIM_OFF);
	lv_group_set_focus_cb(groupHeatingModes, my_focus_cb);
	lv_group_focus_obj(ui_PanelMilk);
}

void ToTempUnitSetting(lv_event_t * e)
{
	if (ui_TempUnitSetting == NULL) {
    	ui_TempUnitSetting_screen_init();
	}
	_ui_screen_change(&ui_TempUnitSetting, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_TempUnitSetting_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
}

void TempUnitGroupUpdate(lv_event_t * e)
{
	lv_group_add_obj(groupTempUnitSetting, ui_PanelCelsius);
    lv_group_add_obj(groupTempUnitSetting, ui_PanelFahrenheit);
	// ESP_LOGI("TempUnitGroupUpdate", "Setting input device group to groupTempUnitSetting");
	lv_indev_set_group(indev, groupTempUnitSetting);

	// lv_obj_t * focused_obj = lv_group_get_focused(groupHeatingModes);
    lv_obj_scroll_to_view(ui_PanelCelsius, LV_ANIM_OFF);
	lv_group_set_focus_cb(groupTempUnitSetting, my_focus_cb);
	lv_group_focus_obj(ui_PanelCelsius);
}

void toNotificationScreen(lv_event_t * e)
{
	if (ui_Notification == NULL) {
       ui_Notification_screen_init();
    }
	_ui_screen_change(&ui_Notification, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Notification_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
}

void ToAboutScreen(lv_event_t * e)
{
    if (ui_About == NULL) {
	    ui_About_screen_init();
    }
	_ui_screen_change(&ui_About, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_About_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
}

void NotificationGroupUpdate(lv_event_t * e)
{
	lv_group_add_obj(groupNotification, ui_PanelVibration);
    lv_group_add_obj(groupNotification, ui_PanelBuzzer);
    lv_group_add_obj(groupNotification, ui_PanelVolumeLvl1);
    lv_group_add_obj(groupNotification, ui_PanelVolumeLvl2);
    lv_group_add_obj(groupNotification, ui_PanelVolumeLvl3);
    lv_group_add_obj(groupNotification, ui_PanelVolumeLvl4);
    lv_group_add_obj(groupNotification, ui_PanelVolumeLvl5);

	lv_indev_set_group(indev, groupNotification);
    lv_obj_scroll_to_view(ui_PanelVibration, LV_ANIM_OFF);
	lv_group_set_focus_cb(groupNotification, my_focus_cb);
	lv_group_focus_obj(ui_PanelVibration);
}

void enableVibration(lv_event_t * e)
{
	

	if(g_pstru_config->b_vibration_enabled){
		g_pstru_config->b_vibration_enabled = false;
		
	}else{
		g_pstru_config->b_vibration_enabled = true;
	}
	 
	// lv_scr_load(curr_scr);
	ToMainScreenNoAnim(e);
	
}

void disableBuzzer(lv_event_t * e)
{
	
	if(g_pstru_config->b_buzzer_enabled){
		g_pstru_config->b_buzzer_enabled = false;
		
	}
	// else{
	// 	g_pstru_config->b_buzzer_enabled = true;
	// }
	 
	// lv_scr_load(curr_scr);
	ToMainScreenNoAnim(e);
	
}

void setVolumeLvl1(lv_event_t * e)
{
	g_pstru_config->enm_buzzer_volume = BUZZER_VOLUME_LEVEL_1;
	g_pstru_config->b_buzzer_enabled = true;
	app_buzzer_update_volume();
	ToMainScreenNoAnim(e);
	// lv_scr_load(prev_scr);
	
}

void setVolumeLvl2(lv_event_t * e)
{
	g_pstru_config->enm_buzzer_volume = BUZZER_VOLUME_LEVEL_2;
	g_pstru_config->b_buzzer_enabled = true;
	app_buzzer_update_volume();
	ToMainScreenNoAnim(e);

}

void setVolumeLvl3(lv_event_t * e)
{
	g_pstru_config->enm_buzzer_volume = BUZZER_VOLUME_LEVEL_3;
	g_pstru_config->b_buzzer_enabled = true;
	app_buzzer_update_volume();
	ToMainScreenNoAnim(e);

}

void setVolumeLvl4(lv_event_t * e)
{
	g_pstru_config->enm_buzzer_volume = BUZZER_VOLUME_LEVEL_4;
	g_pstru_config->b_buzzer_enabled = true;
	app_buzzer_update_volume();
	ToMainScreenNoAnim(e);

}

void setVolumeLvl5(lv_event_t * e)
{
	g_pstru_config->enm_buzzer_volume = BUZZER_VOLUME_LEVEL_5;
	g_pstru_config->b_buzzer_enabled = true;
	app_buzzer_update_volume();
	ToMainScreenNoAnim(e);

}

void setSelectedModeWaterFormula(lv_event_t * e)
{
	
	g_pstru_config->u8_heater_mode = HEATER_M_WATER;
	ToMainScreenNoAnim(e);
}

void setSelectedModeMilk(lv_event_t * e)
{
	g_pstru_config->u8_heater_mode = HEATER_M_MILK;
	ToMainScreenNoAnim(e);

}

void setSelectedModeDefreeze(lv_event_t * e)
{
	
	g_pstru_config->u8_heater_mode = HEATER_M_FROZEN;
	g_pstru_config->u8_temp_setpoint = WARMER_FROZEN_SET_POINT_TEMP;
	ToMainScreenNoAnim(e);
}

void setSelectedModeBabyFood(lv_event_t * e)
{
	
	g_pstru_config->u8_heater_mode = HEATER_M_BABYFOOD;
	ToMainScreenNoAnim(e);
}

void setSelectedModeDeodorize(lv_event_t * e)
{

	ToMainScreenNoAnim(e);
}


void setSelectedTempUnitC(lv_event_t * e)
{
	 g_pstru_config->enm_temp_unit = TEMP_UNIT_C;
	 ToMainScreenNoAnim(e);
}

void setSelectedTempUnitF(lv_event_t * e)
{
	
	 g_pstru_config->enm_temp_unit = TEMP_UNIT_F;
	 ToMainScreenNoAnim(e);
}

void AboutGroupUpdate(lv_event_t * e)
{
	lv_group_add_obj(groupAbout, ui_PanelAboutInfo);
	lv_indev_set_group(indev, groupAbout);
	
	lv_obj_scroll_to_view(ui_PanelAboutInfo, LV_ANIM_OFF);
	lv_group_set_focus_cb(groupAbout, my_focus_cb);
	lv_group_focus_obj(ui_PanelAboutInfo);
}

void BluetoothAppGroupUpdate(lv_event_t * e)
{
    lv_group_add_obj(groupBluetoothApp, ui_PanelBluetoothAppScreen); 
	lv_indev_set_group(indev, groupBluetoothApp);
	
	lv_obj_scroll_to_view(ui_PanelBluetoothApp, LV_ANIM_OFF);
	lv_group_set_focus_cb(groupBluetoothApp, my_focus_cb);
	lv_group_focus_obj(ui_PanelBluetoothApp);
}

void UserInstructionsGroupUpdate(lv_event_t * e)
{
	lv_group_add_obj(groupUserInstructions, ui_PanelUserInstruction);
	lv_indev_set_group(indev, groupUserInstructions);
	lv_obj_scroll_to_view(ui_PanelUserInstruction, LV_ANIM_OFF);
	lv_group_set_focus_cb(groupUserInstructions, my_focus_cb);
	lv_group_focus_obj(ui_PanelUserInstruction);
}


void ToWarmingScreen(lv_event_t * e)
{

	lv_arc_set_value(ui_WarmingFiller, 0);
    lv_label_set_text_fmt(ui_LabelWarmingPercent, "%d%%", 0);
	_ui_screen_change(&ui_Warming, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Warming_screen_init);
	vTaskDelay(pdMS_TO_TICKS(20));
	// WARMER_state_t enm_warmer_state = enm_WARMER_Get_State();
}

void WarmingGroupUpdate(lv_event_t * e)
{
	
	lv_indev_set_group(indev, groupWarming);
	
	lv_obj_scroll_to_view(ui_PanelWarming, LV_ANIM_OFF);
	lv_group_set_focus_cb(groupWarming, my_focus_cb);
	lv_group_focus_obj(ui_PanelWarming);
}



// float percent=0;
// lv_timer_t *warming_timer = NULL; // store the timer handle
// bool is_initial_pwmSet=false;
// bool is_final_pwmSet=false;

// void motor_set_duty_percent(uint8_t percent)
// {
//     uint32_t duty = (percent * 1023) / 100;
//     ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty);
//     ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
	
// }


void u8_warming_percent_update()
{
	lv_arc_set_value(ui_WarmingFiller, device_data.warming_percent);
	lv_label_set_text_fmt(ui_LabelWarmingPercent, "%d%%", device_data.warming_percent);

}

void WarmingToMainScreen(lv_event_t * e)
{
		// percent = 0.0f; // reset if already done
	 	_ui_screen_change(&ui_Main, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Main_screen_init);//400
		vTaskDelay(pdMS_TO_TICKS(20));
		 lv_arc_set_value(ui_WarmingFiller, 0);
    	lv_label_set_text_fmt(ui_LabelWarmingPercent, "%d%%", 0);
	// }

}



void ToTempSettingScreen(lv_event_t * e)
{

	_ui_screen_change(&ui_TempSetting, LV_SCR_LOAD_ANIM_MOVE_TOP, MOVE_UP_ANIM_MS, 0, &ui_TempSetting_screen_init);//300
	vTaskDelay(pdMS_TO_TICKS(20));
}

void SettingTempGroupUpdate(lv_event_t * e)
{

lv_indev_set_group(indev, groupTempSetting);
}

void ErrorGroupUpdate(lv_event_t * e)
{

	lv_indev_set_group(indev, groupError);

	// lv_obj_t * focused_obj = lv_group_get_focused(groupError);
    // lv_obj_scroll_to_view(focused_obj, LV_ANIM_OFF);
	// lv_group_set_focus_cb(groupError, my_focus_cb);
	// lv_group_focus_obj(ui_PanelCelsius);
}

void DoneGroupUpdate(lv_event_t * e)
{
 
	lv_indev_set_group(indev, groupDone);
	//    lv_obj_scroll_to_view(ui_PanelDone, LV_ANIM_OFF);
	// lv_group_set_focus_cb(groupDone, my_focus_cb);
	// lv_group_focus_obj(ui_PanelDone);
}
