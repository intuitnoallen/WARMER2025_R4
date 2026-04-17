#include "../ui.h"

void ui_TempSetting_screen_init(void)
{
    ui_TempSetting = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_TempSetting, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE |
                       LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);     /// Flags
    lv_obj_set_scrollbar_mode(ui_TempSetting, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_TempSetting, LV_DIR_VER);
    lv_obj_set_scroll_snap_x(ui_TempSetting, LV_SCROLL_SNAP_START);
    lv_obj_set_style_bg_color(ui_TempSetting, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_TempSetting, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_TempSetting, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_TempSetting, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_TempSetting, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_TempSetting, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    lv_obj_set_style_border_side(ui_TempSetting, LV_BORDER_SIDE_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    ui_PanelSettingTemp = lv_obj_create(ui_TempSetting);
    lv_obj_set_width(ui_PanelSettingTemp, 240);
    lv_obj_set_height(ui_PanelSettingTemp, 240);
    lv_obj_set_align(ui_PanelSettingTemp, LV_ALIGN_CENTER);
    lv_obj_add_state(ui_PanelSettingTemp, LV_STATE_FOCUSED);       /// States
    lv_obj_remove_flag(ui_PanelSettingTemp, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_PanelSettingTemp, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelSettingTemp, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelSettingTemp, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelSettingTemp, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelSettingTemp, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelSettingTemp, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelSettingTemp, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelsettingTemp = lv_label_create(ui_PanelSettingTemp);
    lv_obj_set_width(ui_LabelsettingTemp, 202);
    lv_obj_set_height(ui_LabelsettingTemp, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelsettingTemp, -51);
    lv_obj_set_y(ui_LabelsettingTemp, 1);
    lv_obj_set_align(ui_LabelsettingTemp, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelsettingTemp, "35");
    lv_obj_remove_flag(ui_LabelsettingTemp,
                       LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                       LV_OBJ_FLAG_SNAPPABLE);     /// Flags
    lv_obj_set_style_text_color(ui_LabelsettingTemp, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelsettingTemp, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelsettingTemp, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelsettingTemp, &ui_font_Segoe_UI_B85, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImageTempSettingUnit = lv_image_create(ui_PanelSettingTemp);
    lv_image_set_src(ui_ImageTempSettingUnit, &ui_img_temp_ci_png);
    lv_obj_set_width(ui_ImageTempSettingUnit, LV_SIZE_CONTENT);   /// 34
    lv_obj_set_height(ui_ImageTempSettingUnit, LV_SIZE_CONTENT);    /// 50
    lv_obj_set_x(ui_ImageTempSettingUnit, -3);
    lv_obj_set_y(ui_ImageTempSettingUnit, -74);
    lv_obj_set_align(ui_ImageTempSettingUnit, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageTempSettingUnit, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_ImageTempSettingUnit, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelstartInfoTempSetting = lv_label_create(ui_PanelSettingTemp);
    lv_obj_set_width(ui_LabelstartInfoTempSetting, 151);
    lv_obj_set_height(ui_LabelstartInfoTempSetting, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelstartInfoTempSetting, -1);
    lv_obj_set_y(ui_LabelstartInfoTempSetting, 78);
    lv_obj_set_align(ui_LabelstartInfoTempSetting, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelstartInfoTempSetting, _("OKtoConfirm"));
    lv_obj_set_style_text_color(ui_LabelstartInfoTempSetting, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelstartInfoTempSetting, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelstartInfoTempSetting, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelstartInfoTempSetting, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelSettingTemp, ui_event_PanelSettingTemp, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_TempSetting, ui_event_TempSetting, LV_EVENT_ALL, NULL);

}
