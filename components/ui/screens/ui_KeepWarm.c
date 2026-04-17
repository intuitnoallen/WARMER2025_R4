#include "../ui.h"

void ui_KeepWarm_screen_init(void)
{
    ui_KeepWarm = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_KeepWarm, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);      /// Flags
    lv_obj_set_scrollbar_mode(ui_KeepWarm, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_KeepWarm, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(ui_KeepWarm, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(ui_KeepWarm, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_KeepWarm, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_KeepWarm, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_KeepWarm, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_KeepWarm, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelKeepWarmOFF = lv_obj_create(ui_KeepWarm);
    lv_obj_set_width(ui_PanelKeepWarmOFF, 240);
    lv_obj_set_height(ui_PanelKeepWarmOFF, 240);
    lv_obj_set_x(ui_PanelKeepWarmOFF, 4);
    lv_obj_set_y(ui_PanelKeepWarmOFF, 0);
    lv_obj_set_align(ui_PanelKeepWarmOFF, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelKeepWarmOFF,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelKeepWarmOFF, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelKeepWarmOFF, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelKeepWarmOFF, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelKeepWarmOFF, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelKeepWarmOFF, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelKeepWarmOFF, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelKeepWarmOFF, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_IconKeepWarmOFF = lv_image_create(ui_PanelKeepWarmOFF);
    lv_image_set_src(ui_IconKeepWarmOFF, &ui_img_keeping_warm_off_png);
    lv_obj_set_width(ui_IconKeepWarmOFF, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconKeepWarmOFF, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconKeepWarmOFF, 2);
    lv_obj_set_y(ui_IconKeepWarmOFF, -20);
    lv_obj_set_align(ui_IconKeepWarmOFF, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconKeepWarmOFF, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconKeepWarmOFF, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelKeepWarmOFF = lv_label_create(ui_PanelKeepWarmOFF);
    lv_obj_set_width(ui_LabelKeepWarmOFF, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelKeepWarmOFF, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelKeepWarmOFF, 0);
    lv_obj_set_y(ui_LabelKeepWarmOFF, 70);
    lv_obj_set_align(ui_LabelKeepWarmOFF, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelKeepWarmOFF, _("OFF"));
    lv_obj_set_style_text_color(ui_LabelKeepWarmOFF, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelKeepWarmOFF, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelKeepWarmOFF, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelKeepWarmOFF, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelKeepWarmLvl1 = lv_obj_create(ui_KeepWarm);
    lv_obj_set_width(ui_PanelKeepWarmLvl1, 240);
    lv_obj_set_height(ui_PanelKeepWarmLvl1, 240);
    lv_obj_set_x(ui_PanelKeepWarmLvl1, 4);
    lv_obj_set_y(ui_PanelKeepWarmLvl1, 0);
    lv_obj_set_align(ui_PanelKeepWarmLvl1, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelKeepWarmLvl1,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelKeepWarmLvl1, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelKeepWarmLvl1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelKeepWarmLvl1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelKeepWarmLvl1, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelKeepWarmLvl1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelKeepWarmLvl1, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelKeepWarmLvl1, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelKeepWarm1 = lv_label_create(ui_PanelKeepWarmLvl1);
    lv_obj_set_width(ui_LabelKeepWarm1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelKeepWarm1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelKeepWarm1, 0);
    lv_obj_set_y(ui_LabelKeepWarm1, -5);
    lv_obj_set_align(ui_LabelKeepWarm1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelKeepWarm1, "1");
    lv_obj_set_style_text_color(ui_LabelKeepWarm1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelKeepWarm1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelKeepWarm1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelKeepWarm1, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelKeepWarmM1 = lv_label_create(ui_PanelKeepWarmLvl1);
    lv_obj_set_width(ui_LabelKeepWarmM1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelKeepWarmM1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelKeepWarmM1, 0);
    lv_obj_set_y(ui_LabelKeepWarmM1, 70);
    lv_obj_set_align(ui_LabelKeepWarmM1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelKeepWarmM1, _("Minute"));
    lv_obj_set_style_text_color(ui_LabelKeepWarmM1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelKeepWarmM1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelKeepWarmM1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelKeepWarmM1, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelKeepWarmLvl2 = lv_obj_create(ui_KeepWarm);
    lv_obj_set_width(ui_PanelKeepWarmLvl2, 240);
    lv_obj_set_height(ui_PanelKeepWarmLvl2, 240);
    lv_obj_set_x(ui_PanelKeepWarmLvl2, 4);
    lv_obj_set_y(ui_PanelKeepWarmLvl2, 0);
    lv_obj_set_align(ui_PanelKeepWarmLvl2, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelKeepWarmLvl2,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelKeepWarmLvl2, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelKeepWarmLvl2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelKeepWarmLvl2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelKeepWarmLvl2, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelKeepWarmLvl2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelKeepWarmLvl2, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelKeepWarmLvl2, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelKeepWarm2 = lv_label_create(ui_PanelKeepWarmLvl2);
    lv_obj_set_width(ui_LabelKeepWarm2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelKeepWarm2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelKeepWarm2, 0);
    lv_obj_set_y(ui_LabelKeepWarm2, -5);
    lv_obj_set_align(ui_LabelKeepWarm2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelKeepWarm2, "2");
    lv_obj_set_style_text_color(ui_LabelKeepWarm2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelKeepWarm2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelKeepWarm2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelKeepWarm2, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelKeepWarmM2 = lv_label_create(ui_PanelKeepWarmLvl2);
    lv_obj_set_width(ui_LabelKeepWarmM2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelKeepWarmM2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelKeepWarmM2, 0);
    lv_obj_set_y(ui_LabelKeepWarmM2, 70);
    lv_obj_set_align(ui_LabelKeepWarmM2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelKeepWarmM2, _("Minutes"));
    lv_obj_set_style_text_color(ui_LabelKeepWarmM2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelKeepWarmM2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelKeepWarmM2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelKeepWarmM2, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelKeepWarmLvl3 = lv_obj_create(ui_KeepWarm);
    lv_obj_set_width(ui_PanelKeepWarmLvl3, 240);
    lv_obj_set_height(ui_PanelKeepWarmLvl3, 240);
    lv_obj_set_x(ui_PanelKeepWarmLvl3, 4);
    lv_obj_set_y(ui_PanelKeepWarmLvl3, 0);
    lv_obj_set_align(ui_PanelKeepWarmLvl3, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelKeepWarmLvl3,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelKeepWarmLvl3, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelKeepWarmLvl3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelKeepWarmLvl3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelKeepWarmLvl3, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelKeepWarmLvl3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelKeepWarmLvl3, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelKeepWarmLvl3, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelKeepWarm3 = lv_label_create(ui_PanelKeepWarmLvl3);
    lv_obj_set_width(ui_LabelKeepWarm3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelKeepWarm3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelKeepWarm3, 0);
    lv_obj_set_y(ui_LabelKeepWarm3, -5);
    lv_obj_set_align(ui_LabelKeepWarm3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelKeepWarm3, "3");
    lv_obj_set_style_text_color(ui_LabelKeepWarm3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelKeepWarm3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelKeepWarm3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelKeepWarm3, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelKeepWarmM3 = lv_label_create(ui_PanelKeepWarmLvl3);
    lv_obj_set_width(ui_LabelKeepWarmM3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelKeepWarmM3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelKeepWarmM3, 0);
    lv_obj_set_y(ui_LabelKeepWarmM3, 70);
    lv_obj_set_align(ui_LabelKeepWarmM3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelKeepWarmM3, _("Minutes"));
    lv_obj_set_style_text_color(ui_LabelKeepWarmM3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelKeepWarmM3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelKeepWarmM3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelKeepWarmM3, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelKeepWarmLvl4 = lv_obj_create(ui_KeepWarm);
    lv_obj_set_width(ui_PanelKeepWarmLvl4, 240);
    lv_obj_set_height(ui_PanelKeepWarmLvl4, 240);
    lv_obj_set_x(ui_PanelKeepWarmLvl4, 4);
    lv_obj_set_y(ui_PanelKeepWarmLvl4, 0);
    lv_obj_set_align(ui_PanelKeepWarmLvl4, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelKeepWarmLvl4,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelKeepWarmLvl4, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelKeepWarmLvl4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelKeepWarmLvl4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelKeepWarmLvl4, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelKeepWarmLvl4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelKeepWarmLvl4, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelKeepWarmLvl4, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelKeepWarm4 = lv_label_create(ui_PanelKeepWarmLvl4);
    lv_obj_set_width(ui_LabelKeepWarm4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelKeepWarm4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelKeepWarm4, 0);
    lv_obj_set_y(ui_LabelKeepWarm4, -5);
    lv_obj_set_align(ui_LabelKeepWarm4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelKeepWarm4, "4");
    lv_obj_set_style_text_color(ui_LabelKeepWarm4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelKeepWarm4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelKeepWarm4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelKeepWarm4, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelKeepWarmM4 = lv_label_create(ui_PanelKeepWarmLvl4);
    lv_obj_set_width(ui_LabelKeepWarmM4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelKeepWarmM4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelKeepWarmM4, 0);
    lv_obj_set_y(ui_LabelKeepWarmM4, 70);
    lv_obj_set_align(ui_LabelKeepWarmM4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelKeepWarmM4, _("Minutes"));
    lv_obj_set_style_text_color(ui_LabelKeepWarmM4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelKeepWarmM4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelKeepWarmM4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelKeepWarmM4, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelKeepWarmLvl5 = lv_obj_create(ui_KeepWarm);
    lv_obj_set_width(ui_PanelKeepWarmLvl5, 240);
    lv_obj_set_height(ui_PanelKeepWarmLvl5, 240);
    lv_obj_set_x(ui_PanelKeepWarmLvl5, 4);
    lv_obj_set_y(ui_PanelKeepWarmLvl5, 0);
    lv_obj_set_align(ui_PanelKeepWarmLvl5, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelKeepWarmLvl5,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelKeepWarmLvl5, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelKeepWarmLvl5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelKeepWarmLvl5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelKeepWarmLvl5, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelKeepWarmLvl5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelKeepWarmLvl5, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelKeepWarmLvl5, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelKeepWarm5 = lv_label_create(ui_PanelKeepWarmLvl5);
    lv_obj_set_width(ui_LabelKeepWarm5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelKeepWarm5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelKeepWarm5, 0);
    lv_obj_set_y(ui_LabelKeepWarm5, -5);
    lv_obj_set_align(ui_LabelKeepWarm5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelKeepWarm5, "5");
    lv_obj_set_style_text_color(ui_LabelKeepWarm5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelKeepWarm5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelKeepWarm5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelKeepWarm5, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelKeepWarmM5 = lv_label_create(ui_PanelKeepWarmLvl5);
    lv_obj_set_width(ui_LabelKeepWarmM5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelKeepWarmM5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelKeepWarmM5, 0);
    lv_obj_set_y(ui_LabelKeepWarmM5, 70);
    lv_obj_set_align(ui_LabelKeepWarmM5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelKeepWarmM5, _("Minutes"));
    lv_obj_set_style_text_color(ui_LabelKeepWarmM5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelKeepWarmM5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelKeepWarmM5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelKeepWarmM5, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelKeepWarmOFF, ui_event_PanelKeepWarmOFF, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelKeepWarmLvl1, ui_event_PanelKeepWarmLvl1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelKeepWarmLvl2, ui_event_PanelKeepWarmLvl2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelKeepWarmLvl3, ui_event_PanelKeepWarmLvl3, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelKeepWarmLvl4, ui_event_PanelKeepWarmLvl4, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelKeepWarmLvl5, ui_event_PanelKeepWarmLvl5, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_KeepWarm, ui_event_KeepWarm, LV_EVENT_ALL, NULL);

}
