#include "../ui.h"

void ui_OTA_screen_init(void)
{
    ui_OTA = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_OTA, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);      /// Flags
    lv_obj_set_scrollbar_mode(ui_OTA, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_OTA, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(ui_OTA, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(ui_OTA, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_OTA, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_OTA, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_OTA, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_OTA, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelOTAProgress = lv_obj_create(ui_OTA);
    lv_obj_set_width(ui_PanelOTAProgress, 240);
    lv_obj_set_height(ui_PanelOTAProgress, 240);
    lv_obj_set_x(ui_PanelOTAProgress, 76);
    lv_obj_set_y(ui_PanelOTAProgress, 113);
    lv_obj_set_align(ui_PanelOTAProgress, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_PanelOTAProgress,
                    LV_OBJ_FLAG_OVERFLOW_VISIBLE | LV_OBJ_FLAG_FLEX_IN_NEW_TRACK | LV_OBJ_FLAG_EVENT_BUBBLE);    /// Flags
    lv_obj_remove_flag(ui_PanelOTAProgress,
                       LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelOTAProgress, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelOTAProgress, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelOTAProgress, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelFWProgress = lv_label_create(ui_PanelOTAProgress);
    lv_obj_set_width(ui_LabelFWProgress, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelFWProgress, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelFWProgress, 2);
    lv_obj_set_y(ui_LabelFWProgress, -57);
    lv_obj_set_align(ui_LabelFWProgress, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelFWProgress, _("FWUpdateProgress"));
    lv_obj_set_style_text_color(ui_LabelFWProgress, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelFWProgress, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelFWProgress, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelFWProgress, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_FirmwareUpdateBar = lv_slider_create(ui_PanelOTAProgress);
    lv_slider_set_value(ui_FirmwareUpdateBar, 0, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_FirmwareUpdateBar) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_FirmwareUpdateBar, 0,
                                                                                                      LV_ANIM_OFF);
    lv_obj_set_width(ui_FirmwareUpdateBar, 209);
    lv_obj_set_height(ui_FirmwareUpdateBar, 18);
    lv_obj_set_align(ui_FirmwareUpdateBar, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_FirmwareUpdateBar, lv_color_hex(0x505050), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_FirmwareUpdateBar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_FirmwareUpdateBar, lv_color_hex(0xC3F73A), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_FirmwareUpdateBar, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_FirmwareUpdateBar, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_FirmwareUpdateBar, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    //Compensating for LVGL9.1 draw crash with bar/slider max value when top-padding is nonzero and right-padding is 0
    if(lv_obj_get_style_pad_top(ui_FirmwareUpdateBar, LV_PART_MAIN) > 0) lv_obj_set_style_pad_right(ui_FirmwareUpdateBar,
                                                                                                        lv_obj_get_style_pad_right(ui_FirmwareUpdateBar, LV_PART_MAIN) + 1, LV_PART_MAIN);
    ui_LabelFWUpdateInfo = lv_label_create(ui_PanelOTAProgress);
    lv_obj_set_width(ui_LabelFWUpdateInfo, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelFWUpdateInfo, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelFWUpdateInfo, 2);
    lv_obj_set_y(ui_LabelFWUpdateInfo, 70);
    lv_obj_set_align(ui_LabelFWUpdateInfo, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelFWUpdateInfo, _("FWUpgradeCaution"));
    lv_obj_set_style_text_color(ui_LabelFWUpdateInfo, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelFWUpdateInfo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelFWUpdateInfo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelFWUpdateInfo, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelOTAUpdated = lv_obj_create(ui_OTA);
    lv_obj_set_width(ui_PanelOTAUpdated, 240);
    lv_obj_set_height(ui_PanelOTAUpdated, 240);
    lv_obj_set_x(ui_PanelOTAUpdated, 4);
    lv_obj_set_y(ui_PanelOTAUpdated, 0);
    lv_obj_set_align(ui_PanelOTAUpdated, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelOTAUpdated,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelOTAUpdated, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelOTAUpdated, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelOTAUpdated, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelFWUpdated = lv_label_create(ui_PanelOTAUpdated);
    lv_obj_set_width(ui_LabelFWUpdated, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelFWUpdated, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelFWUpdated, 0);
    lv_obj_set_y(ui_LabelFWUpdated, 56);
    lv_obj_set_align(ui_LabelFWUpdated, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelFWUpdated, _("FirmwareUpdated"));
    lv_obj_set_style_text_color(ui_LabelFWUpdated, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelFWUpdated, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelFWUpdated, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelFWUpdated, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconFWUpdatesuccess = lv_image_create(ui_PanelOTAUpdated);
    lv_image_set_src(ui_IconFWUpdatesuccess, &ui_img_selected_png);
    lv_obj_set_width(ui_IconFWUpdatesuccess, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconFWUpdatesuccess, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconFWUpdatesuccess, 0);
    lv_obj_set_y(ui_IconFWUpdatesuccess, -25);
    lv_obj_set_align(ui_IconFWUpdatesuccess, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconFWUpdatesuccess, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconFWUpdatesuccess, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_image_recolor(ui_IconFWUpdatesuccess, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor_opa(ui_IconFWUpdatesuccess, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelFWUpdatedReboot = lv_label_create(ui_PanelOTAUpdated);
    lv_obj_set_width(ui_LabelFWUpdatedReboot, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelFWUpdatedReboot, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelFWUpdatedReboot, 0);
    lv_obj_set_y(ui_LabelFWUpdatedReboot, 90);
    lv_obj_set_align(ui_LabelFWUpdatedReboot, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelFWUpdatedReboot, _("Rebooting"));
    lv_obj_set_style_text_color(ui_LabelFWUpdatedReboot, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelFWUpdatedReboot, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelFWUpdatedReboot, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelFWUpdatedReboot, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelOTAFailed = lv_obj_create(ui_OTA);
    lv_obj_set_width(ui_PanelOTAFailed, 240);
    lv_obj_set_height(ui_PanelOTAFailed, 240);
    lv_obj_set_x(ui_PanelOTAFailed, 4);
    lv_obj_set_y(ui_PanelOTAFailed, 0);
    lv_obj_set_align(ui_PanelOTAFailed, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelOTAFailed,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelOTAFailed, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelOTAFailed, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelOTAFailed, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelFWFailed = lv_label_create(ui_PanelOTAFailed);
    lv_obj_set_width(ui_LabelFWFailed, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelFWFailed, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelFWFailed, 0);
    lv_obj_set_y(ui_LabelFWFailed, 56);
    lv_obj_set_align(ui_LabelFWFailed, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelFWFailed, _("UpdateFailed"));
    lv_obj_set_style_text_color(ui_LabelFWFailed, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelFWFailed, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelFWFailed, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelFWFailed, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconFWUpdateFailed = lv_image_create(ui_PanelOTAFailed);
    lv_image_set_src(ui_IconFWUpdateFailed, &ui_img_failed_png);
    lv_obj_set_width(ui_IconFWUpdateFailed, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconFWUpdateFailed, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconFWUpdateFailed, 0);
    lv_obj_set_y(ui_IconFWUpdateFailed, -25);
    lv_obj_set_align(ui_IconFWUpdateFailed, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconFWUpdateFailed, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconFWUpdateFailed, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_image_recolor(ui_IconFWUpdateFailed, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor_opa(ui_IconFWUpdateFailed, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelFWFailedReboot = lv_label_create(ui_PanelOTAFailed);
    lv_obj_set_width(ui_LabelFWFailedReboot, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelFWFailedReboot, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelFWFailedReboot, 0);
    lv_obj_set_y(ui_LabelFWFailedReboot, 90);
    lv_obj_set_align(ui_LabelFWFailedReboot, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelFWFailedReboot, _("Rebooting"));
    lv_obj_set_style_text_color(ui_LabelFWFailedReboot, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelFWFailedReboot, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelFWFailedReboot, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelFWFailedReboot, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelOTAFailed, ui_event_PanelOTAFailed, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_OTA, ui_event_OTA, LV_EVENT_ALL, NULL);

}
