#include "../ui.h"

void ui_HeatingPower_screen_init(void)
{
    ui_HeatingPower = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_HeatingPower, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);      /// Flags
    lv_obj_set_scrollbar_mode(ui_HeatingPower, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_HeatingPower, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(ui_HeatingPower, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(ui_HeatingPower, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_HeatingPower, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_HeatingPower, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_HeatingPower, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_HeatingPower, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelHeatingPowerLvl1 = lv_obj_create(ui_HeatingPower);
    lv_obj_set_width(ui_PanelHeatingPowerLvl1, 240);
    lv_obj_set_height(ui_PanelHeatingPowerLvl1, 240);
    lv_obj_set_x(ui_PanelHeatingPowerLvl1, 4);
    lv_obj_set_y(ui_PanelHeatingPowerLvl1, 0);
    lv_obj_set_align(ui_PanelHeatingPowerLvl1, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelHeatingPowerLvl1,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelHeatingPowerLvl1, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelHeatingPowerLvl1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelHeatingPowerLvl1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelHeatingPowerLvl1, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelHeatingPowerLvl1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelHeatingPowerLvl1, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelHeatingPowerLvl1, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelHeatingPower1 = lv_label_create(ui_PanelHeatingPowerLvl1);
    lv_obj_set_width(ui_LabelHeatingPower1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelHeatingPower1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelHeatingPower1, 0);
    lv_obj_set_y(ui_LabelHeatingPower1, -10);
    lv_obj_set_align(ui_LabelHeatingPower1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelHeatingPower1, "1");
    lv_obj_set_style_text_color(ui_LabelHeatingPower1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelHeatingPower1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelHeatingPower1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelHeatingPower1, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelHeatingPowerLevel1 = lv_label_create(ui_PanelHeatingPowerLvl1);
    lv_obj_set_width(ui_LabelHeatingPowerLevel1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelHeatingPowerLevel1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelHeatingPowerLevel1, 0);
    lv_obj_set_y(ui_LabelHeatingPowerLevel1, 70);
    lv_obj_set_align(ui_LabelHeatingPowerLevel1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelHeatingPowerLevel1, _("Level1"));
    lv_obj_set_style_text_color(ui_LabelHeatingPowerLevel1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelHeatingPowerLevel1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelHeatingPowerLevel1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelHeatingPowerLevel1, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelHeatingPowerLvl2 = lv_obj_create(ui_HeatingPower);
    lv_obj_set_width(ui_PanelHeatingPowerLvl2, 240);
    lv_obj_set_height(ui_PanelHeatingPowerLvl2, 240);
    lv_obj_set_x(ui_PanelHeatingPowerLvl2, 4);
    lv_obj_set_y(ui_PanelHeatingPowerLvl2, 0);
    lv_obj_set_align(ui_PanelHeatingPowerLvl2, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelHeatingPowerLvl2,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelHeatingPowerLvl2, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelHeatingPowerLvl2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelHeatingPowerLvl2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelHeatingPowerLvl2, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelHeatingPowerLvl2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelHeatingPowerLvl2, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelHeatingPowerLvl2, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelHeatingPower2 = lv_label_create(ui_PanelHeatingPowerLvl2);
    lv_obj_set_width(ui_LabelHeatingPower2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelHeatingPower2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelHeatingPower2, 0);
    lv_obj_set_y(ui_LabelHeatingPower2, -10);
    lv_obj_set_align(ui_LabelHeatingPower2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelHeatingPower2, "2");
    lv_obj_set_style_text_color(ui_LabelHeatingPower2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelHeatingPower2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelHeatingPower2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelHeatingPower2, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelHeatingPowerLevel2 = lv_label_create(ui_PanelHeatingPowerLvl2);
    lv_obj_set_width(ui_LabelHeatingPowerLevel2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelHeatingPowerLevel2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelHeatingPowerLevel2, 0);
    lv_obj_set_y(ui_LabelHeatingPowerLevel2, 70);
    lv_obj_set_align(ui_LabelHeatingPowerLevel2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelHeatingPowerLevel2, _("Level2"));
    lv_obj_set_style_text_color(ui_LabelHeatingPowerLevel2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelHeatingPowerLevel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelHeatingPowerLevel2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelHeatingPowerLevel2, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelHeatingPowerLvl3 = lv_obj_create(ui_HeatingPower);
    lv_obj_set_width(ui_PanelHeatingPowerLvl3, 240);
    lv_obj_set_height(ui_PanelHeatingPowerLvl3, 240);
    lv_obj_set_x(ui_PanelHeatingPowerLvl3, 4);
    lv_obj_set_y(ui_PanelHeatingPowerLvl3, 0);
    lv_obj_set_align(ui_PanelHeatingPowerLvl3, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelHeatingPowerLvl3,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelHeatingPowerLvl3, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelHeatingPowerLvl3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelHeatingPowerLvl3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelHeatingPowerLvl3, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelHeatingPowerLvl3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelHeatingPowerLvl3, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelHeatingPowerLvl3, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelHeatingPower3 = lv_label_create(ui_PanelHeatingPowerLvl3);
    lv_obj_set_width(ui_LabelHeatingPower3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelHeatingPower3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelHeatingPower3, 0);
    lv_obj_set_y(ui_LabelHeatingPower3, -10);
    lv_obj_set_align(ui_LabelHeatingPower3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelHeatingPower3, "3");
    lv_obj_set_style_text_color(ui_LabelHeatingPower3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelHeatingPower3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelHeatingPower3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelHeatingPower3, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelHeatingPowerLevel3 = lv_label_create(ui_PanelHeatingPowerLvl3);
    lv_obj_set_width(ui_LabelHeatingPowerLevel3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelHeatingPowerLevel3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelHeatingPowerLevel3, 0);
    lv_obj_set_y(ui_LabelHeatingPowerLevel3, 70);
    lv_obj_set_align(ui_LabelHeatingPowerLevel3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelHeatingPowerLevel3, _("Level3"));
    lv_obj_set_style_text_color(ui_LabelHeatingPowerLevel3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelHeatingPowerLevel3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelHeatingPowerLevel3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelHeatingPowerLevel3, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelHeatingPowerLvl4 = lv_obj_create(ui_HeatingPower);
    lv_obj_set_width(ui_PanelHeatingPowerLvl4, 240);
    lv_obj_set_height(ui_PanelHeatingPowerLvl4, 240);
    lv_obj_set_x(ui_PanelHeatingPowerLvl4, 4);
    lv_obj_set_y(ui_PanelHeatingPowerLvl4, 0);
    lv_obj_set_align(ui_PanelHeatingPowerLvl4, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelHeatingPowerLvl4,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelHeatingPowerLvl4, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelHeatingPowerLvl4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelHeatingPowerLvl4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelHeatingPowerLvl4, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelHeatingPowerLvl4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelHeatingPowerLvl4, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelHeatingPowerLvl4, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelHeatingPower4 = lv_label_create(ui_PanelHeatingPowerLvl4);
    lv_obj_set_width(ui_LabelHeatingPower4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelHeatingPower4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelHeatingPower4, 0);
    lv_obj_set_y(ui_LabelHeatingPower4, -10);
    lv_obj_set_align(ui_LabelHeatingPower4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelHeatingPower4, "4");
    lv_obj_set_style_text_color(ui_LabelHeatingPower4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelHeatingPower4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelHeatingPower4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelHeatingPower4, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelHeatingPowerLevel4 = lv_label_create(ui_PanelHeatingPowerLvl4);
    lv_obj_set_width(ui_LabelHeatingPowerLevel4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelHeatingPowerLevel4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelHeatingPowerLevel4, 0);
    lv_obj_set_y(ui_LabelHeatingPowerLevel4, 70);
    lv_obj_set_align(ui_LabelHeatingPowerLevel4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelHeatingPowerLevel4, _("Level4"));
    lv_obj_set_style_text_color(ui_LabelHeatingPowerLevel4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelHeatingPowerLevel4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelHeatingPowerLevel4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelHeatingPowerLevel4, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelHeatingPowerLvl5 = lv_obj_create(ui_HeatingPower);
    lv_obj_set_width(ui_PanelHeatingPowerLvl5, 240);
    lv_obj_set_height(ui_PanelHeatingPowerLvl5, 240);
    lv_obj_set_x(ui_PanelHeatingPowerLvl5, 4);
    lv_obj_set_y(ui_PanelHeatingPowerLvl5, 0);
    lv_obj_set_align(ui_PanelHeatingPowerLvl5, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelHeatingPowerLvl5,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelHeatingPowerLvl5, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelHeatingPowerLvl5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelHeatingPowerLvl5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelHeatingPowerLvl5, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelHeatingPowerLvl5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelHeatingPowerLvl5, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelHeatingPowerLvl5, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelHeatingPower5 = lv_label_create(ui_PanelHeatingPowerLvl5);
    lv_obj_set_width(ui_LabelHeatingPower5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelHeatingPower5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelHeatingPower5, 0);
    lv_obj_set_y(ui_LabelHeatingPower5, -10);
    lv_obj_set_align(ui_LabelHeatingPower5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelHeatingPower5, "5");
    lv_obj_set_style_text_color(ui_LabelHeatingPower5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelHeatingPower5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelHeatingPower5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelHeatingPower5, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelHeatingPowerLevel5 = lv_label_create(ui_PanelHeatingPowerLvl5);
    lv_obj_set_width(ui_LabelHeatingPowerLevel5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelHeatingPowerLevel5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelHeatingPowerLevel5, 0);
    lv_obj_set_y(ui_LabelHeatingPowerLevel5, 70);
    lv_obj_set_align(ui_LabelHeatingPowerLevel5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelHeatingPowerLevel5, _("Level5"));
    lv_obj_set_style_text_color(ui_LabelHeatingPowerLevel5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelHeatingPowerLevel5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelHeatingPowerLevel5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelHeatingPowerLevel5, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelHeatingPowerLvl1, ui_event_PanelHeatingPowerLvl1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelHeatingPowerLvl2, ui_event_PanelHeatingPowerLvl2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelHeatingPowerLvl3, ui_event_PanelHeatingPowerLvl3, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelHeatingPowerLvl4, ui_event_PanelHeatingPowerLvl4, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelHeatingPowerLvl5, ui_event_PanelHeatingPowerLvl5, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_HeatingPower, ui_event_HeatingPower, LV_EVENT_ALL, NULL);

}
