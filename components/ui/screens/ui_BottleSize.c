#include "../ui.h"

void ui_BottleSize_screen_init(void)
{
    ui_BottleSize = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_BottleSize, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);      /// Flags
    lv_obj_set_scrollbar_mode(ui_BottleSize, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_BottleSize, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(ui_BottleSize, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(ui_BottleSize, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_BottleSize, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_BottleSize, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BottleSize, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_BottleSize, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelBottleSmall = lv_obj_create(ui_BottleSize);
    lv_obj_set_width(ui_PanelBottleSmall, 240);
    lv_obj_set_height(ui_PanelBottleSmall, 240);
    lv_obj_set_x(ui_PanelBottleSmall, 76);
    lv_obj_set_y(ui_PanelBottleSmall, 113);
    lv_obj_set_align(ui_PanelBottleSmall, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_PanelBottleSmall,
                    LV_OBJ_FLAG_OVERFLOW_VISIBLE | LV_OBJ_FLAG_FLEX_IN_NEW_TRACK | LV_OBJ_FLAG_EVENT_BUBBLE);    /// Flags
    lv_obj_remove_flag(ui_PanelBottleSmall,
                       LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelBottleSmall, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelBottleSmall, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelBottleSmall, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconBottleSmall = lv_image_create(ui_PanelBottleSmall);
    lv_image_set_src(ui_IconBottleSmall, &ui_img_bottlesmall_png);
    lv_obj_set_width(ui_IconBottleSmall, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconBottleSmall, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconBottleSmall, 0);
    lv_obj_set_y(ui_IconBottleSmall, -20);
    lv_obj_set_align(ui_IconBottleSmall, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconBottleSmall, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconBottleSmall, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelBottleSmall = lv_label_create(ui_PanelBottleSmall);
    lv_obj_set_width(ui_LabelBottleSmall, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelBottleSmall, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelBottleSmall, 0);
    lv_obj_set_y(ui_LabelBottleSmall, 70);
    lv_obj_set_align(ui_LabelBottleSmall, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBottleSmall, _("120ml"));
    lv_obj_set_style_text_color(ui_LabelBottleSmall, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelBottleSmall, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelBottleSmall, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelBottleSmall, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelBottleLarge = lv_obj_create(ui_BottleSize);
    lv_obj_set_width(ui_PanelBottleLarge, 240);
    lv_obj_set_height(ui_PanelBottleLarge, 240);
    lv_obj_set_x(ui_PanelBottleLarge, 4);
    lv_obj_set_y(ui_PanelBottleLarge, 0);
    lv_obj_set_align(ui_PanelBottleLarge, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelBottleLarge,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelBottleLarge, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelBottleLarge, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelBottleLarge, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelBottleLarge, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconBottleLarge = lv_image_create(ui_PanelBottleLarge);
    lv_image_set_src(ui_IconBottleLarge, &ui_img_bottlelarge_png);
    lv_obj_set_width(ui_IconBottleLarge, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconBottleLarge, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconBottleLarge, 0);
    lv_obj_set_y(ui_IconBottleLarge, -25);
    lv_obj_set_align(ui_IconBottleLarge, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconBottleLarge, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconBottleLarge, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelBottleLarge = lv_label_create(ui_PanelBottleLarge);
    lv_obj_set_width(ui_LabelBottleLarge, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelBottleLarge, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelBottleLarge, 0);
    lv_obj_set_y(ui_LabelBottleLarge, 67);
    lv_obj_set_align(ui_LabelBottleLarge, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBottleLarge, _("240ml"));
    lv_obj_set_style_text_color(ui_LabelBottleLarge, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelBottleLarge, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelBottleLarge, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelBottleLarge, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelBottleSmall, ui_event_PanelBottleSmall, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelBottleLarge, ui_event_PanelBottleLarge, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BottleSize, ui_event_BottleSize, LV_EVENT_ALL, NULL);

}
