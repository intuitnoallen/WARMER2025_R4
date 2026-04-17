#include "../ui.h"

void ui_Main_screen_init(void)
{
    ui_Main = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Main, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE |
                       LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);     /// Flags
    lv_obj_set_scrollbar_mode(ui_Main, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_Main, LV_DIR_VER);
    lv_obj_set_scroll_snap_x(ui_Main, LV_SCROLL_SNAP_START);
    lv_obj_set_style_bg_color(ui_Main, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Main, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_Main, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Main, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_Main, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_Main, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    lv_obj_set_style_border_side(ui_Main, LV_BORDER_SIDE_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    ui_PanelMain = lv_obj_create(ui_Main);
    lv_obj_set_width(ui_PanelMain, 240);
    lv_obj_set_height(ui_PanelMain, 240);
    lv_obj_set_align(ui_PanelMain, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelMain, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_PanelMain, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelMain, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelMain, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelMain, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelMain, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelMain, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelMain, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelcurrentTemp = lv_label_create(ui_PanelMain);
    lv_obj_set_width(ui_LabelcurrentTemp, 202);
    lv_obj_set_height(ui_LabelcurrentTemp, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelcurrentTemp, -51);
    lv_obj_set_y(ui_LabelcurrentTemp, 1);
    lv_obj_set_align(ui_LabelcurrentTemp, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelcurrentTemp, "35");
    lv_obj_remove_flag(ui_LabelcurrentTemp,
                       LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                       LV_OBJ_FLAG_SNAPPABLE);     /// Flags
    lv_obj_set_style_text_color(ui_LabelcurrentTemp, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelcurrentTemp, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelcurrentTemp, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelcurrentTemp, &ui_font_Segoe_UI_B85, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImageTempUnitMain = lv_image_create(ui_PanelMain);
    lv_image_set_src(ui_ImageTempUnitMain, &ui_img_temp_ci_png);
    lv_obj_set_width(ui_ImageTempUnitMain, LV_SIZE_CONTENT);   /// 34
    lv_obj_set_height(ui_ImageTempUnitMain, LV_SIZE_CONTENT);    /// 50
    lv_obj_set_x(ui_ImageTempUnitMain, 80);
    lv_obj_set_y(ui_ImageTempUnitMain, -13);
    lv_obj_set_align(ui_ImageTempUnitMain, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageTempUnitMain, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_ImageTempUnitMain, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelstartInfo = lv_label_create(ui_PanelMain);
    lv_obj_set_width(ui_LabelstartInfo, 151);
    lv_obj_set_height(ui_LabelstartInfo, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelstartInfo, -1);
    lv_obj_set_y(ui_LabelstartInfo, 78);
    lv_obj_set_align(ui_LabelstartInfo, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelstartInfo, _("StartWarming"));
    lv_obj_set_style_text_color(ui_LabelstartInfo, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelstartInfo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelstartInfo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelstartInfo, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelBatPercent = lv_label_create(ui_PanelMain);
    lv_obj_set_width(ui_LabelBatPercent, 55);
    lv_obj_set_height(ui_LabelBatPercent, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelBatPercent, 7);
    lv_obj_set_y(ui_LabelBatPercent, -74);
    lv_obj_set_align(ui_LabelBatPercent, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBatPercent, "0%");
    lv_obj_set_style_text_color(ui_LabelBatPercent, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelBatPercent, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelBatPercent, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelBatPercent, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BLEicon = lv_image_create(ui_PanelMain);
    lv_image_set_src(ui_BLEicon, &ui_img_ble_png);
    lv_obj_set_width(ui_BLEicon, LV_SIZE_CONTENT);   /// 38
    lv_obj_set_height(ui_BLEicon, LV_SIZE_CONTENT);    /// 23
    lv_obj_set_x(ui_BLEicon, -60);
    lv_obj_set_y(ui_BLEicon, -75);
    lv_obj_set_align(ui_BLEicon, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BLEicon, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_BLEicon, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_image_recolor(ui_BLEicon, lv_color_hex(0x909090), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor_opa(ui_BLEicon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Baticon = lv_image_create(ui_PanelMain);
    lv_image_set_src(ui_Baticon, &ui_img_battery_png);
    lv_obj_set_width(ui_Baticon, 45);
    lv_obj_set_height(ui_Baticon, LV_SIZE_CONTENT);    /// 23
    lv_obj_set_x(ui_Baticon, 144);
    lv_obj_set_y(ui_Baticon, -75);
    lv_obj_set_align(ui_Baticon, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_Baticon, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_Baticon, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_image_set_inner_align(ui_Baticon, LV_IMAGE_ALIGN_LEFT_MID);
    lv_obj_set_style_image_recolor(ui_Baticon, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor_opa(ui_Baticon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BatBar = lv_image_create(ui_PanelMain);
    lv_image_set_src(ui_BatBar, &ui_img_bat_bar_png);
    lv_obj_set_width(ui_BatBar, 36);
    lv_obj_set_height(ui_BatBar, LV_SIZE_CONTENT);    /// 18
    lv_obj_set_x(ui_BatBar, 147);
    lv_obj_set_y(ui_BatBar, -75);
    lv_obj_set_align(ui_BatBar, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_BatBar, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_BatBar, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_image_set_inner_align(ui_BatBar, LV_IMAGE_ALIGN_LEFT_MID);
    lv_obj_set_style_image_recolor(ui_BatBar, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor_opa(ui_BatBar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BatCharging = lv_image_create(ui_PanelMain);
    lv_image_set_src(ui_BatCharging, &ui_img_charging_png);
    lv_obj_set_width(ui_BatCharging, LV_SIZE_CONTENT);   /// 36
    lv_obj_set_height(ui_BatCharging, LV_SIZE_CONTENT);    /// 18
    lv_obj_set_x(ui_BatCharging, 156);
    lv_obj_set_y(ui_BatCharging, -75);
    lv_obj_set_align(ui_BatCharging, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_BatCharging, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_BatCharging, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_image_set_inner_align(ui_BatCharging, LV_IMAGE_ALIGN_LEFT_MID);
    lv_obj_set_style_image_recolor(ui_BatCharging, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor_opa(ui_BatCharging, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelMain, ui_event_PanelMain, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Main, ui_event_Main, LV_EVENT_ALL, NULL);

}
