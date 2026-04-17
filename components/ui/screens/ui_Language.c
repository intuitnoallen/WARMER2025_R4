#include "../ui.h"

void ui_Language_screen_init(void)
{
    ui_Language = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Language, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);      /// Flags
    lv_obj_set_scrollbar_mode(ui_Language, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_Language, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(ui_Language, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(ui_Language, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Language, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Language, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Language, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_Language, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelEnglish = lv_obj_create(ui_Language);
    lv_obj_set_width(ui_PanelEnglish, 240);
    lv_obj_set_height(ui_PanelEnglish, 240);
    lv_obj_set_x(ui_PanelEnglish, 76);
    lv_obj_set_y(ui_PanelEnglish, 113);
    lv_obj_set_align(ui_PanelEnglish, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_PanelEnglish, LV_OBJ_FLAG_OVERFLOW_VISIBLE | LV_OBJ_FLAG_FLEX_IN_NEW_TRACK |
                    LV_OBJ_FLAG_EVENT_BUBBLE);    /// Flags
    lv_obj_remove_flag(ui_PanelEnglish, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                       LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelEnglish, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelEnglish, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelEnglish, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelEnglish = lv_label_create(ui_PanelEnglish);
    lv_obj_set_width(ui_LabelEnglish, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelEnglish, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_LabelEnglish, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelEnglish, "EN-US");
    lv_obj_set_style_text_color(ui_LabelEnglish, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelEnglish, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelEnglish, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelEnglish, &ui_font_WIGG_B35, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelEnglishInfo = lv_label_create(ui_PanelEnglish);
    lv_obj_set_width(ui_LabelEnglishInfo, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelEnglishInfo, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelEnglishInfo, 0);
    lv_obj_set_y(ui_LabelEnglishInfo, 78);
    lv_obj_set_align(ui_LabelEnglishInfo, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelEnglishInfo, "English");
    lv_obj_set_style_text_color(ui_LabelEnglishInfo, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelEnglishInfo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelEnglishInfo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelEnglishInfo, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelChinese = lv_obj_create(ui_Language);
    lv_obj_set_width(ui_PanelChinese, 240);
    lv_obj_set_height(ui_PanelChinese, 240);
    lv_obj_set_x(ui_PanelChinese, 4);
    lv_obj_set_y(ui_PanelChinese, 0);
    lv_obj_set_align(ui_PanelChinese, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelChinese, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelChinese, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelChinese, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelChinese, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelChinese = lv_label_create(ui_PanelChinese);
    lv_obj_set_width(ui_LabelChinese, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelChinese, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelChinese, 3);
    lv_obj_set_y(ui_LabelChinese, 0);
    lv_obj_set_align(ui_LabelChinese, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelChinese, "中文");
    lv_obj_set_style_text_color(ui_LabelChinese, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelChinese, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelChinese, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelChinese, &ui_font_WIGG_B35, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_transform_scale(ui_LabelChinese, 256, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelChineseInfo = lv_label_create(ui_PanelChinese);
    lv_obj_set_width(ui_LabelChineseInfo, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelChineseInfo, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelChineseInfo, 3);
    lv_obj_set_y(ui_LabelChineseInfo, 78);
    lv_obj_set_align(ui_LabelChineseInfo, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelChineseInfo, "Chinese");
    lv_obj_set_style_text_color(ui_LabelChineseInfo, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelChineseInfo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelChineseInfo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelChineseInfo, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_transform_scale(ui_LabelChineseInfo, 256, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelJapanese = lv_obj_create(ui_Language);
    lv_obj_set_width(ui_PanelJapanese, 240);
    lv_obj_set_height(ui_PanelJapanese, 240);
    lv_obj_set_x(ui_PanelJapanese, 4);
    lv_obj_set_y(ui_PanelJapanese, 0);
    lv_obj_set_align(ui_PanelJapanese, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelJapanese,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelJapanese, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelJapanese, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelJapanese, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelJapanese = lv_label_create(ui_PanelJapanese);
    lv_obj_set_width(ui_LabelJapanese, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelJapanese, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelJapanese, 3);
    lv_obj_set_y(ui_LabelJapanese, 0);
    lv_obj_set_align(ui_LabelJapanese, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelJapanese, "日語");
    lv_obj_set_style_text_color(ui_LabelJapanese, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelJapanese, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelJapanese, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelJapanese, &ui_font_WIGG_B35, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_transform_scale(ui_LabelJapanese, 256, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelJapaneseInfo = lv_label_create(ui_PanelJapanese);
    lv_obj_set_width(ui_LabelJapaneseInfo, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelJapaneseInfo, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelJapaneseInfo, 3);
    lv_obj_set_y(ui_LabelJapaneseInfo, 78);
    lv_obj_set_align(ui_LabelJapaneseInfo, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelJapaneseInfo, "Japanese");
    lv_obj_set_style_text_color(ui_LabelJapaneseInfo, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelJapaneseInfo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelJapaneseInfo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelJapaneseInfo, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_transform_scale(ui_LabelJapaneseInfo, 256, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelKorean = lv_obj_create(ui_Language);
    lv_obj_set_width(ui_PanelKorean, 240);
    lv_obj_set_height(ui_PanelKorean, 240);
    lv_obj_set_x(ui_PanelKorean, 4);
    lv_obj_set_y(ui_PanelKorean, 0);
    lv_obj_set_align(ui_PanelKorean, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelKorean, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelKorean, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelKorean, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelKorean, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelKorean = lv_label_create(ui_PanelKorean);
    lv_obj_set_width(ui_LabelKorean, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelKorean, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelKorean, 3);
    lv_obj_set_y(ui_LabelKorean, 0);
    lv_obj_set_align(ui_LabelKorean, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelKorean, "한국어");
    lv_obj_set_style_text_color(ui_LabelKorean, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelKorean, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelKorean, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelKorean, &ui_font_WIGG_B35, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_transform_scale(ui_LabelKorean, 256, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelKoreanInfo = lv_label_create(ui_PanelKorean);
    lv_obj_set_width(ui_LabelKoreanInfo, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelKoreanInfo, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelKoreanInfo, 3);
    lv_obj_set_y(ui_LabelKoreanInfo, 78);
    lv_obj_set_align(ui_LabelKoreanInfo, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelKoreanInfo, "Korean");
    lv_obj_set_style_text_color(ui_LabelKoreanInfo, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelKoreanInfo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelKoreanInfo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelKoreanInfo, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_transform_scale(ui_LabelKoreanInfo, 256, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelEnglish, ui_event_PanelEnglish, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelChinese, ui_event_PanelChinese, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelJapanese, ui_event_PanelJapanese, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelKorean, ui_event_PanelKorean, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Language, ui_event_Language, LV_EVENT_ALL, NULL);

}
