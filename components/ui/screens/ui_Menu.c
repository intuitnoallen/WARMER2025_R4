#include "../ui.h"

void ui_Menu_screen_init(void)
{
    ui_Menu = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Menu, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);      /// Flags
    lv_obj_set_scrollbar_mode(ui_Menu, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_Menu, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(ui_Menu, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(ui_Menu, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Menu, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Menu, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Menu, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_Menu, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelModes = lv_obj_create(ui_Menu);
    lv_obj_set_width(ui_PanelModes, 240);
    lv_obj_set_height(ui_PanelModes, 240);
    lv_obj_set_x(ui_PanelModes, 76);
    lv_obj_set_y(ui_PanelModes, 113);
    lv_obj_set_align(ui_PanelModes, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_PanelModes, LV_OBJ_FLAG_OVERFLOW_VISIBLE | LV_OBJ_FLAG_FLEX_IN_NEW_TRACK |
                    LV_OBJ_FLAG_EVENT_BUBBLE);   /// Flags
    lv_obj_remove_flag(ui_PanelModes, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                       LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelModes, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelModes, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelModes, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconModes = lv_image_create(ui_PanelModes);
    lv_image_set_src(ui_IconModes, &ui_img_heatmodes_png);
    lv_obj_set_width(ui_IconModes, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconModes, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconModes, 0);
    lv_obj_set_y(ui_IconModes, -25);
    lv_obj_set_align(ui_IconModes, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconModes, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconModes, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelModes = lv_label_create(ui_PanelModes);
    lv_obj_set_width(ui_LabelModes, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelModes, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelModes, 0);
    lv_obj_set_y(ui_LabelModes, 81);
    lv_obj_set_align(ui_LabelModes, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelModes, _("HeatingMode"));
    lv_obj_set_style_text_color(ui_LabelModes, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelModes, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelModes, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelModes, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelTempUnit = lv_obj_create(ui_Menu);
    lv_obj_set_width(ui_PanelTempUnit, 240);
    lv_obj_set_height(ui_PanelTempUnit, 240);
    lv_obj_set_x(ui_PanelTempUnit, 4);
    lv_obj_set_y(ui_PanelTempUnit, 0);
    lv_obj_set_align(ui_PanelTempUnit, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelTempUnit,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelTempUnit, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelTempUnit, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelTempUnit, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconTempUnit = lv_image_create(ui_PanelTempUnit);
    lv_image_set_src(ui_IconTempUnit, &ui_img_temp_unit_png);
    lv_obj_set_width(ui_IconTempUnit, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconTempUnit, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconTempUnit, 0);
    lv_obj_set_y(ui_IconTempUnit, -31);
    lv_obj_set_align(ui_IconTempUnit, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconTempUnit, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconTempUnit, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelTempUnit = lv_label_create(ui_PanelTempUnit);
    lv_obj_set_width(ui_LabelTempUnit, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelTempUnit, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelTempUnit, 0);
    lv_obj_set_y(ui_LabelTempUnit, 77);
    lv_obj_set_align(ui_LabelTempUnit, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelTempUnit, _("TemperatureUnit"));
    lv_obj_set_style_text_color(ui_LabelTempUnit, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelTempUnit, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelTempUnit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelTempUnit, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelBottleSize = lv_obj_create(ui_Menu);
    lv_obj_set_width(ui_PanelBottleSize, 240);
    lv_obj_set_height(ui_PanelBottleSize, 240);
    lv_obj_set_x(ui_PanelBottleSize, 4);
    lv_obj_set_y(ui_PanelBottleSize, 0);
    lv_obj_set_align(ui_PanelBottleSize, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelBottleSize,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelBottleSize, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelBottleSize, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelBottleSize, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconBottleSize = lv_image_create(ui_PanelBottleSize);
    lv_image_set_src(ui_IconBottleSize, &ui_img_bottlesmall_png);
    lv_obj_set_width(ui_IconBottleSize, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconBottleSize, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconBottleSize, 0);
    lv_obj_set_y(ui_IconBottleSize, -24);
    lv_obj_set_align(ui_IconBottleSize, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconBottleSize, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconBottleSize, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelBottleSize = lv_label_create(ui_PanelBottleSize);
    lv_obj_set_width(ui_LabelBottleSize, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelBottleSize, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelBottleSize, 0);
    lv_obj_set_y(ui_LabelBottleSize, 79);
    lv_obj_set_align(ui_LabelBottleSize, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBottleSize, _("BottleSize"));
    lv_obj_set_style_text_color(ui_LabelBottleSize, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelBottleSize, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelBottleSize, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelBottleSize, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelKeepWarm = lv_obj_create(ui_Menu);
    lv_obj_set_width(ui_PanelKeepWarm, 240);
    lv_obj_set_height(ui_PanelKeepWarm, 240);
    lv_obj_set_x(ui_PanelKeepWarm, 4);
    lv_obj_set_y(ui_PanelKeepWarm, 0);
    lv_obj_set_align(ui_PanelKeepWarm, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelKeepWarm,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelKeepWarm, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelKeepWarm, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelKeepWarm, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconKeepWarm = lv_image_create(ui_PanelKeepWarm);
    lv_image_set_src(ui_IconKeepWarm, &ui_img_keeping_warm_png);
    lv_obj_set_width(ui_IconKeepWarm, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconKeepWarm, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconKeepWarm, 0);
    lv_obj_set_y(ui_IconKeepWarm, -22);
    lv_obj_set_align(ui_IconKeepWarm, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconKeepWarm, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconKeepWarm, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelKeepWarm = lv_label_create(ui_PanelKeepWarm);
    lv_obj_set_width(ui_LabelKeepWarm, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelKeepWarm, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelKeepWarm, 0);
    lv_obj_set_y(ui_LabelKeepWarm, 82);
    lv_obj_set_align(ui_LabelKeepWarm, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelKeepWarm, _("KeepWarmTime"));
    lv_obj_set_style_text_color(ui_LabelKeepWarm, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelKeepWarm, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelKeepWarm, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelKeepWarm, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelNotification = lv_obj_create(ui_Menu);
    lv_obj_set_width(ui_PanelNotification, 240);
    lv_obj_set_height(ui_PanelNotification, 240);
    lv_obj_set_x(ui_PanelNotification, 4);
    lv_obj_set_y(ui_PanelNotification, 0);
    lv_obj_set_align(ui_PanelNotification, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelNotification,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelNotification, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelNotification, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelNotification, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconNotification = lv_image_create(ui_PanelNotification);
    lv_image_set_src(ui_IconNotification, &ui_img_notification_png);
    lv_obj_set_width(ui_IconNotification, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconNotification, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconNotification, -6);
    lv_obj_set_y(ui_IconNotification, -31);
    lv_obj_set_align(ui_IconNotification, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconNotification, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconNotification, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelNotification = lv_label_create(ui_PanelNotification);
    lv_obj_set_width(ui_LabelNotification, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelNotification, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelNotification, 0);
    lv_obj_set_y(ui_LabelNotification, 72);
    lv_obj_set_align(ui_LabelNotification, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelNotification, _("Notification"));
    lv_obj_set_style_text_color(ui_LabelNotification, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelNotification, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelNotification, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelNotification, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelLanguage = lv_obj_create(ui_Menu);
    lv_obj_set_width(ui_PanelLanguage, 240);
    lv_obj_set_height(ui_PanelLanguage, 240);
    lv_obj_set_x(ui_PanelLanguage, 4);
    lv_obj_set_y(ui_PanelLanguage, 0);
    lv_obj_set_align(ui_PanelLanguage, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelLanguage,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelLanguage, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelLanguage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelLanguage, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconLanguage = lv_image_create(ui_PanelLanguage);
    lv_image_set_src(ui_IconLanguage, &ui_img_language_png);
    lv_obj_set_width(ui_IconLanguage, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconLanguage, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconLanguage, 0);
    lv_obj_set_y(ui_IconLanguage, -15);
    lv_obj_set_align(ui_IconLanguage, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconLanguage, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconLanguage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelLanguage = lv_label_create(ui_PanelLanguage);
    lv_obj_set_width(ui_LabelLanguage, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelLanguage, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelLanguage, 0);
    lv_obj_set_y(ui_LabelLanguage, 79);
    lv_obj_set_align(ui_LabelLanguage, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelLanguage, _("Language"));
    lv_obj_set_style_text_color(ui_LabelLanguage, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelLanguage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelLanguage, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelLanguage, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelBluetoothApp = lv_obj_create(ui_Menu);
    lv_obj_set_width(ui_PanelBluetoothApp, 240);
    lv_obj_set_height(ui_PanelBluetoothApp, 240);
    lv_obj_set_x(ui_PanelBluetoothApp, 4);
    lv_obj_set_y(ui_PanelBluetoothApp, 0);
    lv_obj_set_align(ui_PanelBluetoothApp, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelBluetoothApp,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelBluetoothApp, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelBluetoothApp, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelBluetoothApp, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconBluetoothApp = lv_image_create(ui_PanelBluetoothApp);
    lv_image_set_src(ui_IconBluetoothApp, &ui_img_ble_app_png);
    lv_obj_set_width(ui_IconBluetoothApp, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconBluetoothApp, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconBluetoothApp, 0);
    lv_obj_set_y(ui_IconBluetoothApp, -20);
    lv_obj_set_align(ui_IconBluetoothApp, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconBluetoothApp, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconBluetoothApp, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelBluetoothApp = lv_label_create(ui_PanelBluetoothApp);
    lv_obj_set_width(ui_LabelBluetoothApp, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelBluetoothApp, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelBluetoothApp, 0);
    lv_obj_set_y(ui_LabelBluetoothApp, 75);
    lv_obj_set_align(ui_LabelBluetoothApp, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBluetoothApp, _("Bluetooth"));
    lv_obj_set_style_text_color(ui_LabelBluetoothApp, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelBluetoothApp, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelBluetoothApp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelBluetoothApp, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelInstructions = lv_obj_create(ui_Menu);
    lv_obj_set_width(ui_PanelInstructions, 240);
    lv_obj_set_height(ui_PanelInstructions, 240);
    lv_obj_set_x(ui_PanelInstructions, 4);
    lv_obj_set_y(ui_PanelInstructions, 0);
    lv_obj_set_align(ui_PanelInstructions, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelInstructions,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelInstructions, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelInstructions, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelInstructions, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconInstructions = lv_image_create(ui_PanelInstructions);
    lv_image_set_src(ui_IconInstructions, &ui_img_user_instructions_png);
    lv_obj_set_width(ui_IconInstructions, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconInstructions, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconInstructions, 4);
    lv_obj_set_y(ui_IconInstructions, -23);
    lv_obj_set_align(ui_IconInstructions, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconInstructions, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconInstructions, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelInstructions = lv_label_create(ui_PanelInstructions);
    lv_obj_set_width(ui_LabelInstructions, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelInstructions, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelInstructions, 0);
    lv_obj_set_y(ui_LabelInstructions, 68);
    lv_obj_set_align(ui_LabelInstructions, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelInstructions, _("UserInstructions"));
    lv_obj_set_style_text_color(ui_LabelInstructions, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelInstructions, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelInstructions, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelInstructions, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelAbout = lv_obj_create(ui_Menu);
    lv_obj_set_width(ui_PanelAbout, 240);
    lv_obj_set_height(ui_PanelAbout, 240);
    lv_obj_set_x(ui_PanelAbout, 4);
    lv_obj_set_y(ui_PanelAbout, 0);
    lv_obj_set_align(ui_PanelAbout, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelAbout, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelAbout, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelAbout, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelAbout, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconAbout = lv_image_create(ui_PanelAbout);
    lv_image_set_src(ui_IconAbout, &ui_img_about_png);
    lv_obj_set_width(ui_IconAbout, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconAbout, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconAbout, 0);
    lv_obj_set_y(ui_IconAbout, -20);
    lv_obj_set_align(ui_IconAbout, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconAbout, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconAbout, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelAbout = lv_label_create(ui_PanelAbout);
    lv_obj_set_width(ui_LabelAbout, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelAbout, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelAbout, 0);
    lv_obj_set_y(ui_LabelAbout, 75);
    lv_obj_set_align(ui_LabelAbout, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelAbout, _("About"));
    lv_obj_set_style_text_color(ui_LabelAbout, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelAbout, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelAbout, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelAbout, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelModes, ui_event_PanelModes, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelTempUnit, ui_event_PanelTempUnit, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelBottleSize, ui_event_PanelBottleSize, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelKeepWarm, ui_event_PanelKeepWarm, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelNotification, ui_event_PanelNotification, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelLanguage, ui_event_PanelLanguage, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelBluetoothApp, ui_event_PanelBluetoothApp, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelInstructions, ui_event_PanelInstructions, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelAbout, ui_event_PanelAbout, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Menu, ui_event_Menu, LV_EVENT_ALL, NULL);

}
