#include "../ui.h"

void ui_SplashWigg_screen_init(void)
{
    ui_SplashWigg = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_SplashWigg, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_SplashWigg, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SplashWigg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_SplashWigg, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_SplashWigg, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, &ui_SplashWigg);

    ui_PanelSplashWigg = lv_obj_create(ui_SplashWigg);
    lv_obj_set_width(ui_PanelSplashWigg, 227);
    lv_obj_set_height(ui_PanelSplashWigg, 227);
    lv_obj_set_align(ui_PanelSplashWigg, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelSplashWigg, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_PanelSplashWigg, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelSplashWigg, lv_color_hex(0xFAE0EE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelSplashWigg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelSplashWigg, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LogoWigg = lv_image_create(ui_PanelSplashWigg);
    lv_image_set_src(ui_LogoWigg, &ui_img_fc_wigg_200_png);
    lv_obj_set_width(ui_LogoWigg, LV_SIZE_CONTENT);   /// 225
    lv_obj_set_height(ui_LogoWigg, LV_SIZE_CONTENT);    /// 98
    lv_obj_set_x(ui_LogoWigg, 2);
    lv_obj_set_y(ui_LogoWigg, 7);
    lv_obj_set_align(ui_LogoWigg, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_LogoWigg, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                       LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags

    ui_LabelWiggVersion = lv_label_create(ui_PanelSplashWigg);
    lv_obj_set_width(ui_LabelWiggVersion, 86);
    lv_obj_set_height(ui_LabelWiggVersion, 10);
    lv_obj_set_x(ui_LabelWiggVersion, 0);
    lv_obj_set_y(ui_LabelWiggVersion, 95);
    lv_obj_set_align(ui_LabelWiggVersion, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelWiggVersion, "v4.1.9-R4");
    lv_obj_set_style_text_color(ui_LabelWiggVersion, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelWiggVersion, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelWiggVersion, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelWiggVersion, &ui_font_Segoe_UI_SB14, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_SplashWigg, ui_event_SplashWigg, LV_EVENT_ALL, NULL);

}
