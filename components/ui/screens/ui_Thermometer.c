#include "../ui.h"

void ui_Thermometer_screen_init(void)
{
    ui_Thermometer = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Thermometer, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                       LV_OBJ_FLAG_SCROLL_MOMENTUM);    /// Flags
    lv_obj_set_scrollbar_mode(ui_Thermometer, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_Thermometer, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(ui_Thermometer, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_style_bg_color(ui_Thermometer, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Thermometer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_Thermometer, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelTempMeasuring = lv_obj_create(ui_Thermometer);
    lv_obj_set_width(ui_PanelTempMeasuring, 240);
    lv_obj_set_height(ui_PanelTempMeasuring, 240);
    lv_obj_set_align(ui_PanelTempMeasuring, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_PanelTempMeasuring,
                    LV_OBJ_FLAG_OVERFLOW_VISIBLE | LV_OBJ_FLAG_FLEX_IN_NEW_TRACK | LV_OBJ_FLAG_EVENT_BUBBLE);    /// Flags
    lv_obj_remove_flag(ui_PanelTempMeasuring,
                       LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelTempMeasuring, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelTempMeasuring, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelTempMeasuring, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_TempMeasuringFill = lv_arc_create(ui_PanelTempMeasuring);
    lv_obj_set_width(ui_TempMeasuringFill, 140);
    lv_obj_set_height(ui_TempMeasuringFill, 140);
    lv_obj_set_x(ui_TempMeasuringFill, 0);
    lv_obj_set_y(ui_TempMeasuringFill, -25);
    lv_obj_set_align(ui_TempMeasuringFill, LV_ALIGN_CENTER);
    lv_arc_set_value(ui_TempMeasuringFill, 0);
    lv_arc_set_bg_angles(ui_TempMeasuringFill, 270, 269);
    lv_obj_set_style_arc_color(ui_TempMeasuringFill, lv_color_hex(0x5B5B5B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_TempMeasuringFill, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_TempMeasuringFill, 7, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_arc_color(ui_TempMeasuringFill, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_TempMeasuringFill, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_TempMeasuringFill, 7, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_TempMeasuringFill, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_TempMeasuringFill, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_LabelTempMeasuringCount = lv_label_create(ui_PanelTempMeasuring);
    lv_obj_set_width(ui_LabelTempMeasuringCount, LV_SIZE_CONTENT);   /// 200
    lv_obj_set_height(ui_LabelTempMeasuringCount, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelTempMeasuringCount, 0);
    lv_obj_set_y(ui_LabelTempMeasuringCount, -23);
    lv_obj_set_align(ui_LabelTempMeasuringCount, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelTempMeasuringCount, "15");
    lv_obj_set_style_text_color(ui_LabelTempMeasuringCount, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelTempMeasuringCount, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelTempMeasuringCount, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelTempMeasuringCount, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelTempMeasuring = lv_label_create(ui_PanelTempMeasuring);
    lv_obj_set_width(ui_LabelTempMeasuring, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelTempMeasuring, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelTempMeasuring, 0);
    lv_obj_set_y(ui_LabelTempMeasuring, 77);
    lv_obj_set_align(ui_LabelTempMeasuring, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelTempMeasuring, _("MeasuringTemperature"));
    lv_obj_set_style_text_color(ui_LabelTempMeasuring, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelTempMeasuring, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelTempMeasuring, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelTempMeasuring, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelTempMeasuring, ui_event_PanelTempMeasuring, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Thermometer, ui_event_Thermometer, LV_EVENT_ALL, NULL);
    // lv_obj_add_event_cb(ui_Thermometer, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, &ui_Thermometer);

}
