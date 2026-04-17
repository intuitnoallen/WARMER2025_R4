#include "../ui.h"

void ui_Warming_screen_init(void)
{
    ui_Warming = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Warming, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_side(ui_Warming, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelWarming = lv_obj_create(ui_Warming);
    lv_obj_set_width(ui_PanelWarming, 240);
    lv_obj_set_height(ui_PanelWarming, 240);
    lv_obj_set_align(ui_PanelWarming, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelWarming, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelWarming, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelWarming, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelWarming, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WarmingFiller = lv_arc_create(ui_PanelWarming);
    lv_obj_set_width(ui_WarmingFiller, 225);
    lv_obj_set_height(ui_WarmingFiller, 225);
    lv_obj_set_align(ui_WarmingFiller, LV_ALIGN_CENTER);
    lv_arc_set_value(ui_WarmingFiller, 0);
    lv_obj_set_style_arc_color(ui_WarmingFiller, lv_color_hex(0x2E2E2E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_WarmingFiller, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_WarmingFiller, 37, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui_WarmingFiller, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_arc_color(ui_WarmingFiller, lv_color_hex(0xE0FF4F), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_WarmingFiller, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_WarmingFiller, 37, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui_WarmingFiller, false, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_WarmingFiller, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_WarmingFiller, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_LabelWarmingPercent = lv_label_create(ui_PanelWarming);
    lv_obj_set_width(ui_LabelWarmingPercent, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelWarmingPercent, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelWarmingPercent, 1);
    lv_obj_set_y(ui_LabelWarmingPercent, 1);
    lv_obj_set_align(ui_LabelWarmingPercent, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelWarmingPercent, "0%");
    lv_obj_set_style_text_color(ui_LabelWarmingPercent, lv_color_hex(0xE0FF4F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelWarmingPercent, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelWarmingPercent, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_LabelRPM = lv_label_create(ui_PanelWarming);
    // lv_obj_set_width(ui_LabelRPM, LV_SIZE_CONTENT);   /// 62
    // lv_obj_set_height(ui_LabelRPM, LV_SIZE_CONTENT);    /// 1
    // lv_obj_set_x(ui_LabelRPM, 12);
    // lv_obj_set_y(ui_LabelRPM, 73);
    // lv_obj_set_align(ui_LabelRPM, LV_ALIGN_CENTER);
    // lv_label_set_text(ui_LabelRPM, "0");
    // lv_obj_set_style_text_color(ui_LabelRPM, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_opa(ui_LabelRPM, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_align(ui_LabelRPM, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_font(ui_LabelRPM, &ui_font_Segoe_UI_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_LabelPWM = lv_label_create(ui_PanelWarming);
    // lv_obj_set_width(ui_LabelPWM, LV_SIZE_CONTENT);   /// 62
    // lv_obj_set_height(ui_LabelPWM, LV_SIZE_CONTENT);    /// 1
    // lv_obj_set_x(ui_LabelPWM, 12);
    // lv_obj_set_y(ui_LabelPWM, 94);
    // lv_obj_set_align(ui_LabelPWM, LV_ALIGN_CENTER);
    // lv_label_set_text(ui_LabelPWM, "0");
    // lv_obj_set_style_text_color(ui_LabelPWM, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_opa(ui_LabelPWM, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_align(ui_LabelPWM, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_font(ui_LabelPWM, &ui_font_Segoe_UI_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_LabelPWMRPM = lv_label_create(ui_PanelWarming);
    // lv_obj_set_width(ui_LabelPWMRPM, LV_SIZE_CONTENT);   /// 62
    // lv_obj_set_height(ui_LabelPWMRPM, LV_SIZE_CONTENT);    /// 1
    // lv_obj_set_x(ui_LabelPWMRPM, -32);
    // lv_obj_set_y(ui_LabelPWMRPM, 83);
    // lv_obj_set_align(ui_LabelPWMRPM, LV_ALIGN_CENTER);
    // lv_label_set_text(ui_LabelPWMRPM, "R:\nP:");
    // lv_obj_set_style_text_color(ui_LabelPWMRPM, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_opa(ui_LabelPWMRPM, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_align(ui_LabelPWMRPM, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_font(ui_LabelPWMRPM, &ui_font_Segoe_UI_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelWarming, ui_event_PanelWarming, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Warming, ui_event_Warming, LV_EVENT_ALL, NULL);

}
