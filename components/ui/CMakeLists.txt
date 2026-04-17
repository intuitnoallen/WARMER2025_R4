idf_component_register(SRCS 
    screens/ui_SplashWigg.c
    screens/ui_Error.c
    screens/ui_Main.c
    screens/ui_Menu.c
    screens/ui_Language.c
    screens/ui_KeepWarm.c
    screens/ui_HeatingModes.c
    screens/ui_TempSetting.c
    screens/ui_KeepWarm.c
    screens/ui_BottleSize.c
    screens/ui_TempUnitSetting.c
    screens/ui_Notification.c
    screens/ui_Selected.c
    screens/ui_About.c
    screens/ui_Warming.c
    screens/ui_Done.c
    screens/ui_OTA.c
    screens/ui_Thermometer.c
    screens/ui_ThermometerReading.c
    screens/ui_BluetoothApp.c
    screens/ui_UserInstruction.c

    components/ui_comp_hook.c
    
    ui.c
    ui_helpers.c
    ui_events.c
    lv_i18n.c

    images_compressed/ui_img_temp_ci_png.c
    images_compressed/ui_img_temp_fi_png.c
    images_compressed/ui_img_ble_png.c
    images_compressed/ui_img_battery_png.c
    images_compressed/ui_img_heatmodes_png.c
    # images_compressed/ui_img_temp_set_png.c
    images_compressed/ui_img_temp_unit_png.c
    images_compressed/ui_img_bottlesmall_png.c
    images_compressed/ui_img_bottlelarge_png.c
    # images_compressed/ui_img_maintain_time_png.c
    images_compressed/ui_img_keeping_warm_off_png.c
    images_compressed/ui_img_keeping_warm_png.c
    images_compressed/ui_img_notification_png.c
    images_compressed/ui_img_about_png.c
    images_compressed/ui_img_fc_wigg_180_png.c
    images_compressed/ui_img_fc_wigg_png.c
    images_compressed/ui_img_fc_wigg_200_png.c
    images_compressed/ui_img_iconback_png.c
    images_compressed/ui_img_poweroff_png.c
    images_compressed/ui_img_milk_png.c
    images_compressed/ui_img_water_png.c
    images_compressed/ui_img_defreeze_png.c
    images_compressed/ui_img_babyfood_png.c
    images_compressed/ui_img_deodorize_png.c
    images_compressed/ui_img_temp_c_png.c
    images_compressed/ui_img_temp_f_png.c
    images_compressed/ui_img_vibrate_png.c
    images_compressed/ui_img_volume_disable_png.c
    images_compressed/ui_img_selected_png.c
    images_compressed/ui_img_failed_png.c
    # images_compressed/ui_img_wiggon_png.c
    # images_compressed/ui_img_wiggoff_png.c
    images_compressed/ui_img_e1_png.c
    images_compressed/ui_img_e2_png.c
    images_compressed/ui_img_bat_low_png.c
    images_compressed/ui_img_bat_full_png.c
    images_compressed/ui_img_bat_bar_png.c
    images_compressed/ui_img_charging_png.c
    images_compressed/ui_img_chargingui_s_png.c
    images_compressed/ui_img_ble_app_png.c
    images_compressed/ui_img_user_instructions_png.c
    images_compressed/ui_img_qrcodeapp_png.c
    images_compressed/ui_img_qrcodeavoidleaking_png.c
    images_compressed/ui_img_qrcodewebsite_png.c
    images_compressed/ui_img_language_png.c

    fonts/ui_font_WIGG_B20.c
    fonts/ui_font_WIGG_B35.c
    fonts/ui_font_Segoe_UI_B85.c
    fonts/ui_font_Segoe_UI_SB14.c
    # fonts/ui_font_Segoe_UI_SB50.c
    fonts/ui_font_Segoe_UI_SB57.c
    # fonts/ui_font_Segoe_UI_SB65.c

    INCLUDE_DIRS 
    "."
    REQUIRES 
    "lvgl"
    "services"
    "common"
    "app_warmer_mngr"
    
    )
