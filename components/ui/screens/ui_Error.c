#include "../ui.h"

void ui_Error_screen_init(void)
{
    ui_Error = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Error, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);      /// Flags
    lv_obj_set_scrollbar_mode(ui_Error, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_Error, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(ui_Error, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(ui_Error, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Error, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Error, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Error, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_Error, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelERR1 = lv_obj_create(ui_Error);
    lv_obj_set_width(ui_PanelERR1, 240);
    lv_obj_set_height(ui_PanelERR1, 240);
    lv_obj_set_x(ui_PanelERR1, 76);
    lv_obj_set_y(ui_PanelERR1, 113);
    lv_obj_set_align(ui_PanelERR1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_PanelERR1, LV_OBJ_FLAG_OVERFLOW_VISIBLE | LV_OBJ_FLAG_FLEX_IN_NEW_TRACK |
                    LV_OBJ_FLAG_EVENT_BUBBLE);   /// Flags
    lv_obj_remove_flag(ui_PanelERR1, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                       LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelERR1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelERR1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelERR1, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconE1 = lv_image_create(ui_PanelERR1);
    lv_image_set_src(ui_IconE1, &ui_img_e1_png);
    lv_obj_set_width(ui_IconE1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconE1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconE1, 2);
    lv_obj_set_y(ui_IconE1, 3);
    lv_obj_set_align(ui_IconE1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconE1, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconE1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelE1 = lv_label_create(ui_PanelERR1);
    lv_obj_set_width(ui_LabelE1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelE1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelE1, 0);
    lv_obj_set_y(ui_LabelE1, -85);
    lv_obj_set_align(ui_LabelE1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelE1, "E-1");
    lv_obj_set_style_text_color(ui_LabelE1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelE1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelE1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelE1, &ui_font_WIGG_B35, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelERR2 = lv_obj_create(ui_Error);
    lv_obj_set_width(ui_PanelERR2, 240);
    lv_obj_set_height(ui_PanelERR2, 240);
    lv_obj_set_x(ui_PanelERR2, 4);
    lv_obj_set_y(ui_PanelERR2, 0);
    lv_obj_set_align(ui_PanelERR2, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelERR2, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelERR2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelERR2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelERR2, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconE2 = lv_image_create(ui_PanelERR2);
    lv_image_set_src(ui_IconE2, &ui_img_e2_png);
    lv_obj_set_width(ui_IconE2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconE2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconE2, -3);
    lv_obj_set_y(ui_IconE2, -1);
    lv_obj_set_align(ui_IconE2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconE2, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconE2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelE2 = lv_label_create(ui_PanelERR2);
    lv_obj_set_width(ui_LabelE2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelE2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelE2, 0);
    lv_obj_set_y(ui_LabelE2, -85);
    lv_obj_set_align(ui_LabelE2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelE2, "E-2");
    lv_obj_set_style_text_color(ui_LabelE2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelE2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelE2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelE2, &ui_font_WIGG_B35, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelERR3 = lv_obj_create(ui_Error);
    lv_obj_set_width(ui_PanelERR3, 240);
    lv_obj_set_height(ui_PanelERR3, 240);
    lv_obj_set_x(ui_PanelERR3, 4);
    lv_obj_set_y(ui_PanelERR3, 0);
    lv_obj_set_align(ui_PanelERR3, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelERR3, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelERR3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelERR3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelERR3, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelE3 = lv_label_create(ui_PanelERR3);
    lv_obj_set_width(ui_LabelE3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelE3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelE3, 0);
    lv_obj_set_y(ui_LabelE3, -85);
    lv_obj_set_align(ui_LabelE3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelE3, "E-3");
    lv_obj_set_style_text_color(ui_LabelE3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelE3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelE3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelE3, &ui_font_WIGG_B35, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DescE3 = lv_label_create(ui_PanelERR3);
    lv_obj_set_width(ui_DescE3, 220);
    lv_obj_set_height(ui_DescE3, 150);
    lv_obj_set_x(ui_DescE3, 0);
    lv_obj_set_y(ui_DescE3, 34);
    lv_obj_set_align(ui_DescE3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_DescE3,
                      _("E-3-Message"));
    lv_obj_set_style_text_color(ui_DescE3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_DescE3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_DescE3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_DescE3, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelERR4 = lv_obj_create(ui_Error);
    lv_obj_set_width(ui_PanelERR4, 240);
    lv_obj_set_height(ui_PanelERR4, 240);
    lv_obj_set_x(ui_PanelERR4, 4);
    lv_obj_set_y(ui_PanelERR4, 0);
    lv_obj_set_align(ui_PanelERR4, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelERR4, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelERR4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelERR4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelERR4, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelE4 = lv_label_create(ui_PanelERR4);
    lv_obj_set_width(ui_LabelE4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelE4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelE4, 0);
    lv_obj_set_y(ui_LabelE4, -85);
    lv_obj_set_align(ui_LabelE4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelE4, "E-4");
    lv_obj_set_style_text_color(ui_LabelE4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelE4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelE4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelE4, &ui_font_WIGG_B35, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DescE4 = lv_label_create(ui_PanelERR4);
    lv_obj_set_width(ui_DescE4, 220);
    lv_obj_set_height(ui_DescE4, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_DescE4, 0);
    lv_obj_set_y(ui_DescE4, -15);
    lv_obj_set_align(ui_DescE4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_DescE4, _("E-4-Message"));
    lv_obj_set_style_text_color(ui_DescE4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_DescE4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_DescE4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_DescE4, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconE4 = lv_image_create(ui_PanelERR4);
    lv_image_set_src(ui_IconE4, &ui_img_qrcodewebsite_png);
    lv_obj_set_width(ui_IconE4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconE4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconE4, 0);
    lv_obj_set_y(ui_IconE4, 72);
    lv_obj_set_align(ui_IconE4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconE4, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconE4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_PanelERR5 = lv_obj_create(ui_Error);
    lv_obj_set_width(ui_PanelERR5, 240);
    lv_obj_set_height(ui_PanelERR5, 240);
    lv_obj_set_x(ui_PanelERR5, 4);
    lv_obj_set_y(ui_PanelERR5, 0);
    lv_obj_set_align(ui_PanelERR5, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelERR5, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelERR5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelERR5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelERR5, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelE5 = lv_label_create(ui_PanelERR5);
    lv_obj_set_width(ui_LabelE5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelE5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelE5, 0);
    lv_obj_set_y(ui_LabelE5, -85);
    lv_obj_set_align(ui_LabelE5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelE5, "E-5");
    lv_obj_set_style_text_color(ui_LabelE5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelE5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelE5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelE5, &ui_font_WIGG_B35, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DescE5 = lv_label_create(ui_PanelERR5);
    lv_obj_set_width(ui_DescE5, 220);
    lv_obj_set_height(ui_DescE5, LV_SIZE_CONTENT);    /// 90
    lv_obj_set_x(ui_DescE5, 0);
    lv_obj_set_y(ui_DescE5, -15);
    lv_obj_set_align(ui_DescE5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_DescE5, _("E-5-Message"));
    lv_obj_set_style_text_color(ui_DescE5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_DescE5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_DescE5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_DescE5, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconE5 = lv_image_create(ui_PanelERR5);
    lv_image_set_src(ui_IconE5, &ui_img_qrcodewebsite_png);
    lv_obj_set_width(ui_IconE5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconE5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconE5, 0);
    lv_obj_set_y(ui_IconE5, 72);
    lv_obj_set_align(ui_IconE5, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconE5, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconE5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_PanelERR25 = lv_obj_create(ui_Error);
    lv_obj_set_width(ui_PanelERR25, 240);
    lv_obj_set_height(ui_PanelERR25, 240);
    lv_obj_set_x(ui_PanelERR25, 4);
    lv_obj_set_y(ui_PanelERR25, 0);
    lv_obj_set_align(ui_PanelERR25, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelERR25, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelERR25, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelERR25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelERR25, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconBatLow = lv_image_create(ui_PanelERR25);
    lv_image_set_src(ui_IconBatLow, &ui_img_bat_low_png);
    lv_obj_set_width(ui_IconBatLow, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconBatLow, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconBatLow, 3);
    lv_obj_set_y(ui_IconBatLow, -17);
    lv_obj_set_align(ui_IconBatLow, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconBatLow, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconBatLow, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_image_recolor(ui_IconBatLow, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor_opa(ui_IconBatLow, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DescE25 = lv_label_create(ui_PanelERR25);
    lv_obj_set_width(ui_DescE25, 220);
    lv_obj_set_height(ui_DescE25, LV_SIZE_CONTENT);    /// 90
    lv_obj_set_x(ui_DescE25, 0);
    lv_obj_set_y(ui_DescE25, 58);
    lv_obj_set_align(ui_DescE25, LV_ALIGN_CENTER);
    lv_label_set_text(ui_DescE25, _("BatteryLow"));
    lv_obj_set_style_text_color(ui_DescE25, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_DescE25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_DescE25, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_DescE25, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelERR8 = lv_obj_create(ui_Error);
    lv_obj_set_width(ui_PanelERR8, 240);
    lv_obj_set_height(ui_PanelERR8, 240);
    lv_obj_set_x(ui_PanelERR8, 4);
    lv_obj_set_y(ui_PanelERR8, 0);
    lv_obj_set_align(ui_PanelERR8, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelERR8, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelERR8, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelERR8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelERR8, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DescE8 = lv_label_create(ui_PanelERR8);
    lv_obj_set_width(ui_DescE8, 220);
    lv_obj_set_height(ui_DescE8, LV_SIZE_CONTENT);    /// 90
    lv_obj_set_x(ui_DescE8, 0);
    lv_obj_set_y(ui_DescE8, 6);
    lv_obj_set_align(ui_DescE8, LV_ALIGN_CENTER);
    lv_label_set_text(ui_DescE8, _("E-8-Message"));
    lv_obj_set_style_text_color(ui_DescE8, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_DescE8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_DescE8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_DescE8, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelE8 = lv_label_create(ui_PanelERR8);
    lv_obj_set_width(ui_LabelE8, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelE8, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelE8, 0);
    lv_obj_set_y(ui_LabelE8, -85);
    lv_obj_set_align(ui_LabelE8, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelE8, "E-8");
    lv_obj_set_style_text_color(ui_LabelE8, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelE8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelE8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelE8, &ui_font_WIGG_B35, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelERR7 = lv_obj_create(ui_Error);
    lv_obj_set_width(ui_PanelERR7, 240);
    lv_obj_set_height(ui_PanelERR7, 240);
    lv_obj_set_x(ui_PanelERR7, 4);
    lv_obj_set_y(ui_PanelERR7, 0);
    lv_obj_set_align(ui_PanelERR7, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelERR7, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelERR7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelERR7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelERR7, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelE7 = lv_label_create(ui_PanelERR7);
    lv_obj_set_width(ui_LabelE7, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelE7, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelE7, 0);
    lv_obj_set_y(ui_LabelE7, -85);
    lv_obj_set_align(ui_LabelE7, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelE7, "E-7");
    lv_obj_set_style_text_color(ui_LabelE7, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelE7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelE7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelE7, &ui_font_WIGG_B35, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DescE7 = lv_label_create(ui_PanelERR7);
    lv_obj_set_width(ui_DescE7, 220);
    lv_obj_set_height(ui_DescE7, LV_SIZE_CONTENT);    /// 90
    lv_obj_set_x(ui_DescE7, 0);
    lv_obj_set_y(ui_DescE7, -15);
    lv_obj_set_align(ui_DescE7, LV_ALIGN_CENTER);
    lv_label_set_text(ui_DescE7, _("E-7-Message"));
    lv_obj_set_style_text_color(ui_DescE7, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_DescE7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_DescE7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_DescE7, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconE7 = lv_image_create(ui_PanelERR7);
    lv_image_set_src(ui_IconE7, &ui_img_qrcodewebsite_png);
    lv_obj_set_width(ui_IconE7, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconE7, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconE7, 0);
    lv_obj_set_y(ui_IconE7, 72);
    lv_obj_set_align(ui_IconE7, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconE7, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconE7, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_add_event_cb(ui_PanelERR1, ui_event_PanelERR1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelERR2, ui_event_PanelERR2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelERR3, ui_event_PanelERR3, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelERR4, ui_event_PanelERR4, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelERR5, ui_event_PanelERR5, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelERR25, ui_event_PanelERR25, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelERR8, ui_event_PanelERR8, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelERR7, ui_event_PanelERR7, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Error, ui_event_Error, LV_EVENT_ALL, NULL);

}
