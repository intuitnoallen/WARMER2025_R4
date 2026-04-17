#include "../ui.h"

void ui_Notification_screen_init(void)
{
    ui_Notification = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Notification, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);      /// Flags
    lv_obj_set_scrollbar_mode(ui_Notification, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_Notification, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(ui_Notification, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(ui_Notification, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Notification, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Notification, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Notification, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_Notification, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelVibration = lv_obj_create(ui_Notification);
    lv_obj_set_width(ui_PanelVibration, 240);
    lv_obj_set_height(ui_PanelVibration, 240);
    lv_obj_set_x(ui_PanelVibration, 76);
    lv_obj_set_y(ui_PanelVibration, 113);
    lv_obj_set_align(ui_PanelVibration, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_PanelVibration, LV_OBJ_FLAG_OVERFLOW_VISIBLE | LV_OBJ_FLAG_FLEX_IN_NEW_TRACK |
                    LV_OBJ_FLAG_EVENT_BUBBLE);    /// Flags
    lv_obj_remove_flag(ui_PanelVibration,
                       LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelVibration, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelVibration, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelVibration, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelVibration, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelVibration, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelVibration, 255, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_IconVibration = lv_image_create(ui_PanelVibration);
    lv_image_set_src(ui_IconVibration, &ui_img_vibrate_png);
    lv_obj_set_width(ui_IconVibration, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconVibration, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconVibration, 0);
    lv_obj_set_y(ui_IconVibration, -20);
    lv_obj_set_align(ui_IconVibration, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconVibration, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconVibration, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelVibration = lv_label_create(ui_PanelVibration);
    lv_obj_set_width(ui_LabelVibration, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelVibration, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelVibration, 0);
    lv_obj_set_y(ui_LabelVibration, 70);
    lv_obj_set_align(ui_LabelVibration, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelVibration, _("Vibration"));
    lv_obj_set_style_text_color(ui_LabelVibration, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelVibration, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelVibration, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelVibration, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelBuzzer = lv_obj_create(ui_Notification);
    lv_obj_set_width(ui_PanelBuzzer, 240);
    lv_obj_set_height(ui_PanelBuzzer, 240);
    lv_obj_set_x(ui_PanelBuzzer, 4);
    lv_obj_set_y(ui_PanelBuzzer, 0);
    lv_obj_set_align(ui_PanelBuzzer, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelBuzzer, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelBuzzer, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelBuzzer, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelBuzzer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelBuzzer, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelBuzzer, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelBuzzer, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelBuzzer, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_IconBuzzer = lv_image_create(ui_PanelBuzzer);
    lv_image_set_src(ui_IconBuzzer, &ui_img_volume_disable_png);
    lv_obj_set_width(ui_IconBuzzer, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconBuzzer, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconBuzzer, 0);
    lv_obj_set_y(ui_IconBuzzer, -20);
    lv_obj_set_align(ui_IconBuzzer, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconBuzzer, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconBuzzer, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelBuzzer = lv_label_create(ui_PanelBuzzer);
    lv_obj_set_width(ui_LabelBuzzer, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelBuzzer, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelBuzzer, 0);
    lv_obj_set_y(ui_LabelBuzzer, 70);
    lv_obj_set_align(ui_LabelBuzzer, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBuzzer, _("Ringtone"));
    lv_obj_set_style_text_color(ui_LabelBuzzer, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelBuzzer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelBuzzer, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelBuzzer, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelVolumeLvl1 = lv_obj_create(ui_Notification);
    lv_obj_set_width(ui_PanelVolumeLvl1, 240);
    lv_obj_set_height(ui_PanelVolumeLvl1, 240);
    lv_obj_set_x(ui_PanelVolumeLvl1, 4);
    lv_obj_set_y(ui_PanelVolumeLvl1, 0);
    lv_obj_set_align(ui_PanelVolumeLvl1, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelVolumeLvl1,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelVolumeLvl1, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelVolumeLvl1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelVolumeLvl1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelVolumeLvl1, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelVolumeLvl1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelVolumeLvl1, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelVolumeLvl1, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelVolume1 = lv_label_create(ui_PanelVolumeLvl1);
    lv_obj_set_width(ui_LabelVolume1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelVolume1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelVolume1, 0);
    lv_obj_set_y(ui_LabelVolume1, -10);
    lv_obj_set_align(ui_LabelVolume1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelVolume1, "1");
    lv_obj_set_style_text_color(ui_LabelVolume1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelVolume1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelVolume1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelVolume1, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelLevel1 = lv_label_create(ui_PanelVolumeLvl1);
    lv_obj_set_width(ui_LabelLevel1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelLevel1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelLevel1, 0);
    lv_obj_set_y(ui_LabelLevel1, 70);
    lv_obj_set_align(ui_LabelLevel1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelLevel1, _("Level1"));
    lv_obj_set_style_text_color(ui_LabelLevel1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelLevel1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelLevel1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelLevel1, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelVolumeLvl2 = lv_obj_create(ui_Notification);
    lv_obj_set_width(ui_PanelVolumeLvl2, 240);
    lv_obj_set_height(ui_PanelVolumeLvl2, 240);
    lv_obj_set_x(ui_PanelVolumeLvl2, 4);
    lv_obj_set_y(ui_PanelVolumeLvl2, 0);
    lv_obj_set_align(ui_PanelVolumeLvl2, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelVolumeLvl2,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelVolumeLvl2, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelVolumeLvl2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelVolumeLvl2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelVolumeLvl2, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelVolumeLvl2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelVolumeLvl2, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelVolumeLvl2, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelVolume2 = lv_label_create(ui_PanelVolumeLvl2);
    lv_obj_set_width(ui_LabelVolume2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelVolume2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelVolume2, 0);
    lv_obj_set_y(ui_LabelVolume2, -10);
    lv_obj_set_align(ui_LabelVolume2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelVolume2, "2");
    lv_obj_set_style_text_color(ui_LabelVolume2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelVolume2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelVolume2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelVolume2, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelLevel2 = lv_label_create(ui_PanelVolumeLvl2);
    lv_obj_set_width(ui_LabelLevel2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelLevel2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelLevel2, 0);
    lv_obj_set_y(ui_LabelLevel2, 70);
    lv_obj_set_align(ui_LabelLevel2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelLevel2, _("Level2"));
    lv_obj_set_style_text_color(ui_LabelLevel2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelLevel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelLevel2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelLevel2, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelVolumeLvl3 = lv_obj_create(ui_Notification);
    lv_obj_set_width(ui_PanelVolumeLvl3, 240);
    lv_obj_set_height(ui_PanelVolumeLvl3, 240);
    lv_obj_set_x(ui_PanelVolumeLvl3, 4);
    lv_obj_set_y(ui_PanelVolumeLvl3, 0);
    lv_obj_set_align(ui_PanelVolumeLvl3, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelVolumeLvl3,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelVolumeLvl3, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelVolumeLvl3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelVolumeLvl3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelVolumeLvl3, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelVolumeLvl3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelVolumeLvl3, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelVolumeLvl3, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelVolume3 = lv_label_create(ui_PanelVolumeLvl3);
    lv_obj_set_width(ui_LabelVolume3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelVolume3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelVolume3, 0);
    lv_obj_set_y(ui_LabelVolume3, -10);
    lv_obj_set_align(ui_LabelVolume3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelVolume3, "3");
    lv_obj_set_style_text_color(ui_LabelVolume3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelVolume3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelVolume3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelVolume3, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelLevel3 = lv_label_create(ui_PanelVolumeLvl3);
    lv_obj_set_width(ui_LabelLevel3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelLevel3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelLevel3, 0);
    lv_obj_set_y(ui_LabelLevel3, 70);
    lv_obj_set_align(ui_LabelLevel3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelLevel3, _("Level3"));
    lv_obj_set_style_text_color(ui_LabelLevel3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelLevel3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelLevel3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelLevel3, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelVolumeLvl4 = lv_obj_create(ui_Notification);
    lv_obj_set_width(ui_PanelVolumeLvl4, 240);
    lv_obj_set_height(ui_PanelVolumeLvl4, 240);
    lv_obj_set_x(ui_PanelVolumeLvl4, 4);
    lv_obj_set_y(ui_PanelVolumeLvl4, 0);
    lv_obj_set_align(ui_PanelVolumeLvl4, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelVolumeLvl4,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelVolumeLvl4, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelVolumeLvl4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelVolumeLvl4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelVolumeLvl4, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelVolumeLvl4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelVolumeLvl4, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelVolumeLvl4, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelVolume4 = lv_label_create(ui_PanelVolumeLvl4);
    lv_obj_set_width(ui_LabelVolume4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelVolume4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelVolume4, 0);
    lv_obj_set_y(ui_LabelVolume4, -10);
    lv_obj_set_align(ui_LabelVolume4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelVolume4, "4");
    lv_obj_set_style_text_color(ui_LabelVolume4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelVolume4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelVolume4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelVolume4, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelLevel4 = lv_label_create(ui_PanelVolumeLvl4);
    lv_obj_set_width(ui_LabelLevel4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelLevel4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelLevel4, 0);
    lv_obj_set_y(ui_LabelLevel4, 70);
    lv_obj_set_align(ui_LabelLevel4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelLevel4, _("Level4"));
    lv_obj_set_style_text_color(ui_LabelLevel4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelLevel4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelLevel4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelLevel4, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelVolumeLvl5 = lv_obj_create(ui_Notification);
    lv_obj_set_width(ui_PanelVolumeLvl5, 240);
    lv_obj_set_height(ui_PanelVolumeLvl5, 240);
    lv_obj_set_x(ui_PanelVolumeLvl5, 4);
    lv_obj_set_y(ui_PanelVolumeLvl5, 0);
    lv_obj_set_align(ui_PanelVolumeLvl5, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelVolumeLvl5,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_radius(ui_PanelVolumeLvl5, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelVolumeLvl5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelVolumeLvl5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelVolumeLvl5, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelVolumeLvl5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_PanelVolumeLvl5, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui_PanelVolumeLvl5, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelVolume5 = lv_label_create(ui_PanelVolumeLvl5);
    lv_obj_set_width(ui_LabelVolume5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelVolume5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelVolume5, 0);
    lv_obj_set_y(ui_LabelVolume5, -10);
    lv_obj_set_align(ui_LabelVolume5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelVolume5, "5");
    lv_obj_set_style_text_color(ui_LabelVolume5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelVolume5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelVolume5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelVolume5, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelLevel5 = lv_label_create(ui_PanelVolumeLvl5);
    lv_obj_set_width(ui_LabelLevel5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelLevel5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelLevel5, 0);
    lv_obj_set_y(ui_LabelLevel5, 70);
    lv_obj_set_align(ui_LabelLevel5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelLevel5, _("Level5"));
    lv_obj_set_style_text_color(ui_LabelLevel5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelLevel5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelLevel5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelLevel5, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelVibration, ui_event_PanelVibration, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelBuzzer, ui_event_PanelBuzzer, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelVolumeLvl1, ui_event_PanelVolumeLvl1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelVolumeLvl2, ui_event_PanelVolumeLvl2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelVolumeLvl3, ui_event_PanelVolumeLvl3, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelVolumeLvl4, ui_event_PanelVolumeLvl4, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelVolumeLvl5, ui_event_PanelVolumeLvl5, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Notification, ui_event_Notification, LV_EVENT_ALL, NULL);

}
