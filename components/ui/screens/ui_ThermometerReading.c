#include "../ui.h"

void ui_ThermometerReading_screen_init(void)
{
    ui_ThermometerReading = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_ThermometerReading,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);     /// Flags
    lv_obj_set_scrollbar_mode(ui_ThermometerReading, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_ThermometerReading, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(ui_ThermometerReading, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_style_bg_color(ui_ThermometerReading, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ThermometerReading, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_ThermometerReading, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelTempMeasured = lv_obj_create(ui_ThermometerReading);
    lv_obj_set_width(ui_PanelTempMeasured, 240);
    lv_obj_set_height(ui_PanelTempMeasured, 240);
    lv_obj_set_align(ui_PanelTempMeasured, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelTempMeasured,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelTempMeasured, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelTempMeasured, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelTempMeasured, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelTempReading = lv_label_create(ui_PanelTempMeasured);
    lv_obj_set_width(ui_LabelTempReading, LV_SIZE_CONTENT);   /// 200
    lv_obj_set_height(ui_LabelTempReading, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelTempReading, 0);
    lv_obj_set_y(ui_LabelTempReading, 5);
    lv_obj_set_align(ui_LabelTempReading, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelTempReading, "20.76");
    lv_obj_set_style_text_color(ui_LabelTempReading, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelTempReading, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelTempReading, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelTempReading, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelTempMeasured = lv_label_create(ui_PanelTempMeasured);
    lv_obj_set_width(ui_LabelTempMeasured, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelTempMeasured, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelTempMeasured, 2);
    lv_obj_set_y(ui_LabelTempMeasured, 78);
    lv_obj_set_align(ui_LabelTempMeasured, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelTempMeasured, _("MeasuredTemperature"));
    lv_obj_set_style_text_color(ui_LabelTempMeasured, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelTempMeasured, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelTempMeasured, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelTempMeasured, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconTempMeasuredUnit = lv_image_create(ui_PanelTempMeasured);
    lv_image_set_src(ui_IconTempMeasuredUnit, &ui_img_temp_ci_png);
    lv_obj_set_width(ui_IconTempMeasuredUnit, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconTempMeasuredUnit, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconTempMeasuredUnit, -2);
    lv_obj_set_y(ui_IconTempMeasuredUnit, -73);
    lv_obj_set_align(ui_IconTempMeasuredUnit, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconTempMeasuredUnit, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconTempMeasuredUnit, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_add_event_cb(ui_PanelTempMeasured, ui_event_PanelTempMeasured, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ThermometerReading, ui_event_ThermometerReading, LV_EVENT_ALL, NULL);

}
