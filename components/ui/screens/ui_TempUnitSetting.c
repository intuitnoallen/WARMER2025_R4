#include "../ui.h"

void ui_TempUnitSetting_screen_init(void)
{
    ui_TempUnitSetting = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_TempUnitSetting, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);      /// Flags
    lv_obj_set_scrollbar_mode(ui_TempUnitSetting, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_TempUnitSetting, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(ui_TempUnitSetting, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(ui_TempUnitSetting, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_TempUnitSetting, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_TempUnitSetting, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_TempUnitSetting, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_TempUnitSetting, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelCelsius = lv_obj_create(ui_TempUnitSetting);
    lv_obj_set_width(ui_PanelCelsius, 240);
    lv_obj_set_height(ui_PanelCelsius, 240);
    lv_obj_set_x(ui_PanelCelsius, 76);
    lv_obj_set_y(ui_PanelCelsius, 113);
    lv_obj_set_align(ui_PanelCelsius, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_PanelCelsius, LV_OBJ_FLAG_OVERFLOW_VISIBLE | LV_OBJ_FLAG_FLEX_IN_NEW_TRACK |
                    LV_OBJ_FLAG_EVENT_BUBBLE);    /// Flags
    lv_obj_remove_flag(ui_PanelCelsius, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                       LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelCelsius, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelCelsius, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelCelsius, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconCelsius = lv_image_create(ui_PanelCelsius);
    lv_image_set_src(ui_IconCelsius, &ui_img_temp_c_png);
    lv_obj_set_width(ui_IconCelsius, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconCelsius, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconCelsius, -5);
    lv_obj_set_y(ui_IconCelsius, -20);
    lv_obj_set_align(ui_IconCelsius, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconCelsius, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconCelsius, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelCelsius = lv_label_create(ui_PanelCelsius);
    lv_obj_set_width(ui_LabelCelsius, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelCelsius, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelCelsius, 0);
    lv_obj_set_y(ui_LabelCelsius, 70);
    lv_obj_set_align(ui_LabelCelsius, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelCelsius, _("Celsius"));
    lv_obj_set_style_text_color(ui_LabelCelsius, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelCelsius, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelCelsius, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelCelsius, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelFahrenheit = lv_obj_create(ui_TempUnitSetting);
    lv_obj_set_width(ui_PanelFahrenheit, 240);
    lv_obj_set_height(ui_PanelFahrenheit, 240);
    lv_obj_set_x(ui_PanelFahrenheit, 4);
    lv_obj_set_y(ui_PanelFahrenheit, 0);
    lv_obj_set_align(ui_PanelFahrenheit, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelFahrenheit,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelFahrenheit, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelFahrenheit, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelFahrenheit, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelFahrenheit, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconFahrenheit_ = lv_image_create(ui_PanelFahrenheit);
    lv_image_set_src(ui_IconFahrenheit_, &ui_img_temp_f_png);
    lv_obj_set_width(ui_IconFahrenheit_, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconFahrenheit_, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconFahrenheit_, -5);
    lv_obj_set_y(ui_IconFahrenheit_, -20);
    lv_obj_set_align(ui_IconFahrenheit_, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconFahrenheit_, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconFahrenheit_, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelFahrenheit = lv_label_create(ui_PanelFahrenheit);
    lv_obj_set_width(ui_LabelFahrenheit, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelFahrenheit, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelFahrenheit, 0);
    lv_obj_set_y(ui_LabelFahrenheit, 70);
    lv_obj_set_align(ui_LabelFahrenheit, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelFahrenheit, _("Fahrenheit"));
    lv_obj_set_style_text_color(ui_LabelFahrenheit, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelFahrenheit, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelFahrenheit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelFahrenheit, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelCelsius, ui_event_PanelCelsius, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelFahrenheit, ui_event_PanelFahrenheit, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_TempUnitSetting, ui_event_TempUnitSetting, LV_EVENT_ALL, NULL);

}
